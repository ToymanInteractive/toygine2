//
// Copyright (c) 2025-2026 Toyman Interactive
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
/*!
  \file   assertion_windows.cpp
  \brief  Windows implementation of the \ref toy::assertion crash handler and stack trace capture.
*/

#include "core.hpp"

namespace toy::assertion {

static scorpio::assert::AssertCallback    s_assertCallback    = nullptr;
static scorpio::assert::StackWalkCallback s_stackWalkCallback = nullptr;

static HANDLE                       s_processHandle                 = 0;
static LPTOP_LEVEL_EXCEPTION_FILTER s_previousFilterFunctionPointer = nullptr;

static const size_t STACKWALK_MAX_NAMELEN = 1024;

struct ExceptionInfo {
  DWORD        id;
  const char * description;
};

static const ExceptionInfo sc_exceptionsInfo[] = {
  {        EXCEPTION_ACCESS_VIOLATION,         "access violation"},
  {   EXCEPTION_ARRAY_BOUNDS_EXCEEDED,    "array bounds exceeded"},
  {              EXCEPTION_BREAKPOINT,               "breakpoint"},
  {   EXCEPTION_DATATYPE_MISALIGNMENT,    "datatype misalignment"},
  {    EXCEPTION_FLT_DENORMAL_OPERAND,   "float denormal operand"},
  {      EXCEPTION_FLT_DIVIDE_BY_ZERO,     "float divide by zero"},
  {      EXCEPTION_FLT_INEXACT_RESULT,     "float inexact result"},
  {   EXCEPTION_FLT_INVALID_OPERATION,  "float invalid operation"},
  {            EXCEPTION_FLT_OVERFLOW,           "float overflow"},
  {         EXCEPTION_FLT_STACK_CHECK,        "float stack check"},
  {           EXCEPTION_FLT_UNDERFLOW,          "float underflow"},
  {     EXCEPTION_ILLEGAL_INSTRUCTION,      "illegal instruction"},
  {           EXCEPTION_IN_PAGE_ERROR,            "in page error"},
  {      EXCEPTION_INT_DIVIDE_BY_ZERO,   "integer divide by zero"},
  {            EXCEPTION_INT_OVERFLOW,         "integer overflow"},
  {     EXCEPTION_INVALID_DISPOSITION,      "invalid disposition"},
  {EXCEPTION_NONCONTINUABLE_EXCEPTION, "noncontinuable exception"},
  {        EXCEPTION_PRIV_INSTRUCTION,   "privileged instruction"},
  {             EXCEPTION_SINGLE_STEP,              "single step"},
  {          EXCEPTION_STACK_OVERFLOW,           "stack overflow"}
};

static const char * exceptionDescription(DWORD id) {
  for (size_t i = 0; i < ARRAY_SIZE(sc_exceptionsInfo); ++i) {
    if (sc_exceptionsInfo[i].id == id)
      return sc_exceptionsInfo[i].description;
  }

  return nullptr;
}

class StackWalker {
public:
  StackWalker(DWORD processId, HANDLE processH);
  ~StackWalker();

  bool showCallStack(HANDLE threadH, const CONTEXT * context, size_t destSize = 0, char * dest = nullptr);

private:
  struct CallStackEntry {
    DWORD64       offset; // if 0, we have no valid entry
    wchar_t       moduleName[STACKWALK_MAX_NAMELEN];
    wchar_t       name[STACKWALK_MAX_NAMELEN];
    wchar_t       undName[STACKWALK_MAX_NAMELEN];
    wchar_t       undFullName[STACKWALK_MAX_NAMELEN];
    std::uint32_t lineNumber;
  };

  using PReadProcessMemoryRoutine = BOOL(__stdcall *)(HANDLE hProcess, DWORD64 qwBaseAddress, PVOID lpBuffer,
                                                      DWORD nSize, LPDWORD lpNumberOfBytesRead, LPVOID pUserData);

  bool loadModules();
  void onCallStackEntry(const CallStackEntry & entry, char * dest, size_t destSize);

  HANDLE    _processH;
  DWORD     _processId;
  bool      _modulesLoaded;
  wchar_t * _symPath;

  static BOOL __stdcall myReadProcMem(HANDLE hProcess, DWORD64 qwBaseAddress, PVOID lpBuffer, DWORD nSize,
                                      LPDWORD lpNumberOfBytesRead);
};

static bool getModuleInfo(HANDLE processH, DWORD64 baseAddr, IMAGEHLP_MODULE64 * moduleInfo) {
  memset(moduleInfo, 0, sizeof(IMAGEHLP_MODULE64));
  moduleInfo->SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
  return (SymGetModuleInfo64(processH, baseAddr, moduleInfo) != FALSE);
}

// **************************************** ToolHelp32 ************************
static bool getModuleListTH32(HANDLE processH, DWORD processID) {
  const HANDLE snapH = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
  if (snapH == INVALID_HANDLE_VALUE)
    return false;

  MODULEENTRY32W me;
  me.dwSize        = sizeof(me);
  bool   keepGoing = (Module32FirstW(snapH, &me) != FALSE);
  size_t cnt       = 0;

  while (keepGoing) {
    SymLoadModuleExW(processH, 0, me.szExePath, me.szModule, reinterpret_cast<DWORD64>(me.modBaseAddr), me.modBaseSize,
                     nullptr, 0);
    ++cnt;
    keepGoing = (Module32NextW(snapH, &me) != FALSE);
  }

  CloseHandle(snapH);
  return (cnt > 0);
}

// **************************************** PSAPI ************************
static bool getModuleListPSAPI(HANDLE processH) {
  HMODULE hMods[1024];
  DWORD   cbNeeded;

#if (TARGET_CPU == CPU_INTEL_x86)
  if (EnumProcessModules(processH, hMods, sizeof(hMods), &cbNeeded) == FALSE)
    return false;
#elif (TARGET_CPU == CPU_INTEL_x64)
  if (EnumProcessModulesEx(processH, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL) == FALSE)
    return false;
#endif // TARGET_CPU

  if (cbNeeded > sizeof(hMods))
    return false;

  size_t cnt = 0;
  for (size_t i = 0; i < (cbNeeded / sizeof(hMods[0])); ++i) {
    MODULEINFO mi;

    // base address, size
    GetModuleInformation(processH, hMods[i], &mi, sizeof(mi));

    // image file name
    wchar_t tt[MAX_PATH];
    tt[0] = L'\0';
    GetModuleFileNameExW(processH, hMods[i], tt, ARRAY_SIZE(tt));

    // module name
    wchar_t tt2[MAX_PATH];
    tt2[0] = L'\0';
    GetModuleBaseNameW(processH, hMods[i], tt2, ARRAY_SIZE(tt2));

    SymLoadModuleExW(processH, 0, tt, tt2, reinterpret_cast<DWORD64>(mi.lpBaseOfDll), mi.SizeOfImage, nullptr, 0);
    ++cnt;
  }

  return (cnt > 0);
}

long __stdcall SysUnhandledExceptionFilter(EXCEPTION_POINTERS * exceptionInfo) {
  if (exceptionInfo == nullptr || s_stackWalkCallback == nullptr)
    return EXCEPTION_CONTINUE_SEARCH;

  char   szTopFunc[4096] = "";
  size_t startCharIndex  = 0;
  startCharIndex         = strlen(szTopFunc);
  sprintf_s(szTopFunc + startCharIndex, ARRAY_SIZE(szTopFunc) - startCharIndex, "Handled exception 0x%08X",
            exceptionInfo->ExceptionRecord->ExceptionCode);
  const char * excDesk = exceptionDescription(exceptionInfo->ExceptionRecord->ExceptionCode);
  if (excDesk != nullptr) {
    startCharIndex = strlen(szTopFunc);
    sprintf_s(szTopFunc + startCharIndex, ARRAY_SIZE(szTopFunc) - startCharIndex, " (%s)\r\n\r\n", excDesk);
  } else
    strcat_s<ARRAY_SIZE(szTopFunc)>(szTopFunc, "\r\n\r\n");

  strcat_s<ARRAY_SIZE(szTopFunc)>(szTopFunc, "call stack:\r\n");

  const size_t szTopFuncDataLen = strlen(szTopFunc);
  StackWalker  walker(GetCurrentProcessId(), GetCurrentProcess());
  walker.showCallStack(GetCurrentThread(), exceptionInfo->ContextRecord, ARRAY_SIZE(szTopFunc) - szTopFuncDataLen,
                       szTopFunc + szTopFuncDataLen);

  (*s_stackWalkCallback)(szTopFunc);
  return EXCEPTION_EXECUTE_HANDLER;
}

StackWalker::StackWalker(DWORD processId, HANDLE processH)
  : _processH(processH)
  , _processId(processId)
  , _modulesLoaded(false)
  , _symPath(nullptr) {}

StackWalker::~StackWalker() {
  SAFE_FREE(_symPath);
  SymCleanup(_processH);
}

bool StackWalker::loadModules() {
  if (_modulesLoaded)
    return true;

  // Build the sym-path:
  wchar_t szSymPath[4096] = {L'\0'};
  // Now first add the (optional) provided sympath:
  if (_symPath != NULL) {
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, _symPath);
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L";");
  }

  wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L".;");

  wchar_t szTemp[1024];
  // Now add the current directory:
  if (GetCurrentDirectoryW(ARRAY_SIZE(szTemp), szTemp) != 0) {
    szTemp[ARRAY_SIZE(szTemp) - 1] = L'\0';
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, szTemp);
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L";");
  }

  // Now add the path for the main-module:
  if (GetModuleFileNameW(NULL, szTemp, ARRAY_SIZE(szTemp)) != 0) {
    szTemp[ARRAY_SIZE(szTemp) - 1] = L'\0';
    for (wchar_t * p = (szTemp + wcslen(szTemp) - 1); p >= szTemp; --p) {
      // locate the rightmost path separator
      if ((*p == L'\\') || (*p == L'/') || (*p == L':')) {
        *p = L'\0';
        break;
      }
    } // for (search for path separator...)
    if (szTemp[0] != L'\0') {
      wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, szTemp);
      wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L";");
    }
  }
  if (GetEnvironmentVariableW(L"_NT_SYMBOL_PATH", szTemp, ARRAY_SIZE(szTemp)) != 0) {
    szTemp[ARRAY_SIZE(szTemp) - 1] = L'\0';
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, szTemp);
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L";");
  }
  if (GetEnvironmentVariableW(L"_NT_ALTERNATE_SYMBOL_PATH", szTemp, ARRAY_SIZE(szTemp)) != 0) {
    szTemp[ARRAY_SIZE(szTemp) - 1] = L'\0';
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, szTemp);
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L";");
  }
  if (GetEnvironmentVariableW(L"SYSTEMROOT", szTemp, ARRAY_SIZE(szTemp)) != 0) {
    szTemp[ARRAY_SIZE(szTemp) - 1] = L'\0';
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, szTemp);
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L";");
    // also add the "system32"-directory:
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, szTemp);
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L"\\system32;");
  }
  if (GetEnvironmentVariableW(L"SYSTEMDRIVE", szTemp, ARRAY_SIZE(szTemp)) != 0) {
    szTemp[ARRAY_SIZE(szTemp) - 1] = '\0';
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L"SRV*");
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, szTemp);
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L"\\websymbols*http://msdl.microsoft.com/download/symbols;");
  } else
    wcscat_s<ARRAYSIZE(szSymPath)>(szSymPath, L"SRV*c:\\websymbols*http://msdl.microsoft.com/download/symbols;");

  // First Init the whole stuff...
  SymInitializeW(_processH, szSymPath, TRUE);
  SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES | SYMOPT_FAIL_CRITICAL_ERRORS /* | SYMOPT_NO_PROMPTS */);

  _modulesLoaded = getModuleListTH32(_processH, _processId); // first try toolhelp32
  if (!_modulesLoaded)
    _modulesLoaded = getModuleListPSAPI(_processH);          // then try psapi

  return _modulesLoaded;
}

bool StackWalker::showCallStack(HANDLE threadH, const CONTEXT * context, size_t destSize, char * dest) {
  if (!_modulesLoaded)
    loadModules(); // ignore the result...

  CONTEXT        c;
  CallStackEntry csEntry;

  if (context == nullptr) {
    if (threadH == GetCurrentThread()) {
      do {
        memset(&c, 0, sizeof(CONTEXT));
        c.ContextFlags = CONTEXT_FULL;
        RtlCaptureContext(&c);
      } while (0);
    } else {
      SuspendThread(threadH);
      memset(&c, 0, sizeof(c));
      c.ContextFlags = CONTEXT_FULL;
      if (GetThreadContext(threadH, &c) == FALSE) {
        ResumeThread(threadH);
        return false;
      }
    }
  } else
    c = *context;

  // init STACKFRAME for first call
  STACKFRAME64 s; // in/out stackframe
  memset(&s, 0, sizeof(s));
  DWORD imageType;
#ifdef _M_IX86
  // normally, call ImageNtHeader() and use machine info from PE header
  imageType          = IMAGE_FILE_MACHINE_I386;
  s.AddrPC.Offset    = c.Eip;
  s.AddrPC.Mode      = AddrModeFlat;
  s.AddrFrame.Offset = c.Ebp;
  s.AddrFrame.Mode   = AddrModeFlat;
  s.AddrStack.Offset = c.Esp;
  s.AddrStack.Mode   = AddrModeFlat;
#elif _M_X64
  imageType          = IMAGE_FILE_MACHINE_AMD64;
  s.AddrPC.Offset    = c.Rip;
  s.AddrPC.Mode      = AddrModeFlat;
  s.AddrFrame.Offset = c.Rsp;
  s.AddrFrame.Mode   = AddrModeFlat;
  s.AddrStack.Offset = c.Rsp;
  s.AddrStack.Mode   = AddrModeFlat;
#elif _M_IA64
  imageType           = IMAGE_FILE_MACHINE_IA64;
  s.AddrPC.Offset     = c.StIIP;
  s.AddrPC.Mode       = AddrModeFlat;
  s.AddrFrame.Offset  = c.IntSp;
  s.AddrFrame.Mode    = AddrModeFlat;
  s.AddrBStore.Offset = c.RsBSP;
  s.AddrBStore.Mode   = AddrModeFlat;
  s.AddrStack.Offset  = c.IntSp;
  s.AddrStack.Mode    = AddrModeFlat;
#else
#error "Platform not supported!"
#endif

  SYMBOL_INFOW * pSym
    = reinterpret_cast<SYMBOL_INFOW *>(MALLOC(sizeof(SYMBOL_INFOW) + STACKWALK_MAX_NAMELEN * sizeof(wchar_t)));
  if (pSym == nullptr) // not enough memory...
  {
    if (context == nullptr)
      ResumeThread(threadH);

    return true;
  }

  memset(pSym, 0, sizeof(SYMBOL_INFOW) + STACKWALK_MAX_NAMELEN * sizeof(wchar_t));
  pSym->SizeOfStruct = sizeof(SYMBOL_INFOW);
  pSym->MaxNameLen   = STACKWALK_MAX_NAMELEN;

  IMAGEHLP_LINEW64 Line = {0};
  Line.SizeOfStruct     = sizeof(Line);

  IMAGEHLP_MODULE64 Module = {0};
  Module.SizeOfStruct      = sizeof(Module);

  while (true) {
    // get next stack frame (StackWalk64(), SymFunctionTableAccess64(), SymGetModuleBase64())
    // if this returns ERROR_INVALID_ADDRESS (487) or ERROR_NOACCESS (998), you can
    // assume that either you are done, or that the stack is so hosed that the next
    // deeper frame could not be found.
    // CONTEXT need not to be supplied if imageTyp is IMAGE_FILE_MACHINE_I386!
    if (StackWalk64(imageType, _processH, threadH, &s, &c, myReadProcMem, SymFunctionTableAccess64, SymGetModuleBase64,
                    nullptr)
        == FALSE)
      break;

    csEntry.offset         = s.AddrPC.Offset;
    csEntry.moduleName[0]  = '\0';
    csEntry.name[0]        = L'\0';
    csEntry.undName[0]     = L'\0';
    csEntry.undFullName[0] = L'\0';
    csEntry.lineNumber     = 0;

    if (s.AddrPC.Offset == s.AddrReturn.Offset)
      break;

    if (s.AddrPC.Offset != 0) {
      // we seem to have a valid PC
      if (SymFromAddrW(_processH, s.AddrPC.Offset, nullptr, pSym) != FALSE) {
        wcscpy_s<ARRAY_SIZE(csEntry.name)>(csEntry.name, pSym->Name);
        UnDecorateSymbolNameW(pSym->Name, csEntry.undName, STACKWALK_MAX_NAMELEN, UNDNAME_NAME_ONLY);
        UnDecorateSymbolNameW(pSym->Name, csEntry.undFullName, STACKWALK_MAX_NAMELEN, UNDNAME_COMPLETE);
      }

      // show line number info
      DWORD bytesDisplacement;
      if (SymGetLineFromAddrW64(_processH, s.AddrPC.Offset, &bytesDisplacement, &Line) != FALSE)
        csEntry.lineNumber = Line.LineNumber;
      else
        csEntry.lineNumber = UINT32_MAX;

      // show module info (SymGetModuleInfoW64())
      if (getModuleInfo(_processH, s.AddrPC.Offset, &Module) != FALSE) { // got module info OK
        size_t imageNameLen = strlen(Module.ImageName);
        while (imageNameLen > 0) {
          if (Module.ImageName[imageNameLen - 1] == '\\' || Module.ImageName[imageNameLen - 1] == '/'
              || Module.ImageName[imageNameLen - 1] == ':')
            break;

          --imageNameLen;
        }

        scorpio::utf8towchar(csEntry.moduleName, ARRAY_SIZE(csEntry.moduleName),
                             static_cast<const char *>(&(Module.ImageName[imageNameLen])));
      } // got module info OK
    } // we seem to have a valid PC

    onCallStackEntry(csEntry, dest, destSize);
    if (s.AddrReturn.Offset == 0)
      break;
  }

  SAFE_FREE(pSym);
  if (context == nullptr)
    ResumeThread(threadH);

  return true;
}

void StackWalker::onCallStackEntry(const CallStackEntry & entry, char * dest, size_t destSize) {
  if (entry.offset != 0) {
    const wchar_t * entryName = nullptr;
    if (entry.undFullName[0] != L'\0')
      entryName = entry.undFullName;
    else if (entry.undName[0] != L'\0')
      entryName = entry.undName;
    else if (entry.name[0] != L'\0')
      entryName = entry.name;

    wchar_t buffer[STACKWALK_MAX_NAMELEN];
    if (entryName != nullptr) {
      if (entry.lineNumber != UINT32_MAX)
        swprintf_s<ARRAY_SIZE(buffer)>(buffer, L"%s!%s() Line %u\r\n", entry.moduleName, entryName, entry.lineNumber);
      else
        swprintf_s<ARRAY_SIZE(buffer)>(buffer, L"%s!%s()\r\n", entry.moduleName, entryName);
    } else
      swprintf_s<ARRAY_SIZE(buffer)>(buffer, L"%s!0x%016llX\r\n", entry.moduleName, entry.offset);

    if (dest != nullptr) {
      char bufferUTF8[STACKWALK_MAX_NAMELEN * scorpio::wcharInUtf8MaxSize];
      scorpio::wchartoutf8(bufferUTF8, ARRAY_SIZE(bufferUTF8), buffer);
      strcat_s(dest, destSize, bufferUTF8);
    } else
      OutputDebugStringW(buffer);
  }
}

BOOL __stdcall StackWalker::myReadProcMem(HANDLE processH, DWORD64 qwBaseAddress, PVOID lpBuffer, DWORD nSize,
                                          LPDWORD lpNumberOfBytesRead) {
  SIZE_T     st;
  BOOL const retValue
    = ReadProcessMemory(processH, reinterpret_cast<LPVOID>(qwBaseAddress), lpBuffer, static_cast<SIZE_T>(nSize), &st);
  *lpNumberOfBytesRead = static_cast<DWORD>(st);
  return retValue;
}

namespace scorpio {
namespace assert {

void initialize() {
  s_assertCallback                = nullptr;
  s_stackWalkCallback             = nullptr;
  s_processHandle                 = GetCurrentProcess();
  s_previousFilterFunctionPointer = SetUnhandledExceptionFilter(SysUnhandledExceptionFilter);
  SymInitializeW(s_processHandle, nullptr, TRUE);
  CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

void deinitialize() {
  s_stackWalkCallback = nullptr;
  s_assertCallback    = nullptr;
  CoUninitialize();
  SetUnhandledExceptionFilter(s_previousFilterFunctionPointer);
  SymCleanup(s_processHandle);
}

void setcallbacks(AssertCallback callbackFunction1, StackWalkCallback callbackFunction2) {
  s_assertCallback    = callbackFunction1;
  s_stackWalkCallback = callbackFunction2;
}

#ifdef _DEBUG

void assertion(const char * code, const char * message, const char * fileName, const char * functionName,
               std::uint32_t lineNumber) {
  char assertionString[4096];
  if (message == nullptr)
    sprintf_s(assertionString, ARRAY_SIZE(assertionString), "%s @ %s (%u):\r\n\r\n%s", functionName, fileName,
              lineNumber, code);
  else
    sprintf_s(assertionString, ARRAY_SIZE(assertionString), "%s @ %s (%u):\r\n\r\n%s: %s", functionName, fileName,
              lineNumber, message, code);

  static bool assertReEnter = false;
  if (assertReEnter)
    return;

  assertReEnter = true;

  LOG("Assert Failed\r\n%s", assertionString);

  if (IsDebuggerPresent())
    DebugBreak();
  else if ((s_assertCallback != nullptr) && !(*s_assertCallback)(assertionString)) {
    // raise unhandled exception
    if (s_processHandle)
      RaiseException(EXCEPTION_BREAKPOINT, 0, 0, 0);
  }

  assertReEnter = false;
}

#endif // _DEBUG

} // namespace toy::assertion
