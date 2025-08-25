//
// Copyright (c) 2025 by Toyman Interactive
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

#include <array>

#include <cxxabi.h>
#include <execinfo.h>
#include <signal.h>

#include "core.hpp"
#include "system_signal.hpp"

static toygine::assertion::AssertCallback s_assertionCallback = nullptr;
static toygine::assertion::StackWalkCallback s_stackWalkCallback = nullptr;

static const std::array<SystemSignal, 31> sc_signalsDescriptions{
  {{SIGHUP, "Terminal line hangup"},
   {SIGINT, "Interrupt program"},
   {SIGQUIT, "Quit program"},
   {SIGILL, "Illegal instruction"},
   {SIGTRAP, "Trace trap"},
   {SIGABRT, "Abort program"},
   {SIGEMT, "EMT instruction"},
   {SIGFPE, "Floating-point exception"},
   {SIGKILL, "Kill program"},
   {SIGBUS, "Bus error"},
   {SIGSEGV, "Segmentation violation"},
   {SIGSYS, "Non-existent system call invoked"},
   {SIGPIPE, "Write on a pipe with no reader"},
   {SIGALRM, "Real-time timer expired"},
   {SIGTERM, "Software termination signal"},
   {SIGURG, "Urgent condition present on socket"},
   {SIGSTOP, "Stop (cannot be caught or ignored)"},
   {SIGTSTP, "Stop signal generated from keyboard"},
   {SIGCONT, "Continue a stopped process"},
   {SIGCHLD, "Child status has changed"},
   {SIGTTIN, "Background read attempted from control terminal"},
   {SIGTTOU, "Background write attempted to control terminal"},
   {SIGIO, "I/O is possible on a descriptor"},
   {SIGXCPU, "Exceeded CPU time limit"},
   {SIGXFSZ, "Exceeded file size limit"},
   {SIGVTALRM, "Virtual time alarm"},
   {SIGPROF, "Profiling timer alarm"},
   {SIGWINCH, "Window size change"},
   {SIGINFO, "Information request"},
   {SIGUSR1, "User defined signal 1"},
   {SIGUSR2, "User defined signal 2"}}};

namespace toygine {
namespace assertion {

static sig_t s_previousUSR1Handler = nullptr;
static sig_t s_previousBUShandler = nullptr;
static sig_t s_previousFPEhandler = nullptr;
static sig_t s_previousEGVhandler = nullptr;

/*!
  \brief Returns a human-readable description of the given system signal id.

  \param id A signal code (SIGINT, SIGSEGV, etc.).

  \return A pointer to a null-terminated string containing the description of the signal, or nullptr if the signal is
  not recognized.
*/
static const char * signalDescription(int id) {
  for (std::size_t i = 0; i < sc_signalsDescriptions.size(); ++i) {
    if (sc_signalsDescriptions[i].id == id)
      return sc_signalsDescriptions[i].description;
  }

  return nullptr;
}

/*!
  \brief Prints a demangled stack backtrace to the specified destination buffer.

  This function captures the current execution stack up to a specified number of frames, demangles the function names,
  and formats the stack trace into a human-readable string. The formatted stack trace is written into the provided
  destination buffer.

  \param dest       A pointer to the destination buffer where the stack trace will be stored.
  \param destSize   The size of the destination buffer.
  \param maxFrames  The maximum number of stack frames to capture. Defaults to 63.
  \param skipFrames The number of initial stack frames to skip, typically used to skip the current function's frame.
                    Defaults to 1.
*/
static void printStackTrace(char * dest, std::size_t destSize, std::size_t maxFrames = 63, std::size_t skipFrames = 1) {
  snprintf(dest, destSize, "CallStack functions:\r\n\r\n");

  // storage array for stack trace address data
  void * addressList[maxFrames + 1];

  // retrieve current stack addresses
  const auto addressLength = backtrace(addressList, static_cast<int>(maxFrames + 1));
  if (addressLength == 0) {
    const auto bufferTailIndex = strlen(dest);
    snprintf(dest + bufferTailIndex, destSize - bufferTailIndex, "\t<empty, possibly corrupt>\r\n");

    return;
  }

  // resolve addresses into strings containing "filename(function+address)", this array must be free()-ed
  char ** symbolList = backtrace_symbols(addressList, addressLength);

  // allocate string which will be filled with the demangled function name
  size_t functionNameSize = 1024;
  char * functionNameBuffer = static_cast<char *>(malloc(functionNameSize));
  char * functionName = functionNameBuffer;

  // iterate over the returned symbol lines. skip the first, it is the address of this function.
  for (auto index = skipFrames; index < addressLength; ++index) {
    const char * moduleName = nullptr;
    const char * functionAddress = nullptr;
    const char * mangledFunctionName = nullptr;
    std::int32_t functionAddrShift = 0;

    auto findSpace = false;

    for (auto symbol = symbolList[index]; *symbol != '\0'; ++symbol) {
      if (*symbol == ' ') {
        if (!findSpace)
          *symbol = '\0';

        findSpace = true;
      } else {
        if (!findSpace)
          continue;

        findSpace = false;

        if (moduleName == nullptr)
          moduleName = symbol;
        else if (functionAddress == nullptr)
          functionAddress = symbol;
        else if (mangledFunctionName == nullptr)
          mangledFunctionName = symbol;
        else if (*symbol == '+')
          functionAddrShift = strtol(symbol + 2, nullptr, 10);
      }
    }

    int status;
    char * ret = abi::__cxa_demangle(mangledFunctionName, functionName, &functionNameSize, &status);
    if (status == 0) {
      functionName = ret; // use possibly realloc()-ed string
      auto startCharIndex = strlen(dest);
      snprintf(dest + startCharIndex, destSize - startCharIndex, "%s!%s + %i bytes\r\n", moduleName, functionName,
               functionAddrShift);
    } else {
      // demangling failed. Output function name as a C function with no arguments.
      size_t startCharIndex = strlen(dest);
      snprintf(dest + startCharIndex, destSize - startCharIndex, "%s!%s() + %i bytes\r\n", moduleName,
               mangledFunctionName, functionAddrShift);
    }
  }

  free(functionNameBuffer);
  free(symbolList);
}

/*!
  \brief Signal handler for handled signals.

  If the signal is not SIGUSR1, the function prints a message with the signal number and description (if available), and
  then prints the stack trace with the specified number of frames and continues to the next instruction.

  If the signal is SIGUSR1, the function prints a stack trace with the specified number of frames and continues to the
  next instruction.

  The function then calls the stack walk callback with the constructed string and exits the process with a successful
  status.

  \param sig The signal number.
*/
[[noreturn]]
static void signalHandler(int sig) {
  if (s_stackWalkCallback == nullptr)
    exit(EXIT_FAILURE);

  char szTopFunc[4096] = "";
  if (sig != SIGUSR1) {
    auto startCharIndex = strlen(szTopFunc);
    snprintf(szTopFunc + startCharIndex, ArraySize(szTopFunc) - startCharIndex, "Handled signal %d", sig);

    const char * sigDesk = signalDescription(sig);
    startCharIndex = strlen(szTopFunc);
    if (sigDesk != nullptr) {
      snprintf(szTopFunc + startCharIndex, ArraySize(szTopFunc) - startCharIndex, " (%s)\r\n", sigDesk);
    } else {
      strncat(szTopFunc, "\r\n\r\n", ArraySize(szTopFunc) - startCharIndex - 1);
    }
  }

  const auto freeCharIndex = strlen(szTopFunc);
  printStackTrace(szTopFunc + freeCharIndex, ArraySize(szTopFunc) - freeCharIndex, 63, sig == SIGUSR1 ? 6 : 4);

  (*s_stackWalkCallback)(szTopFunc);
  exit(EXIT_SUCCESS);
}

void initialize() {
  s_assertionCallback = nullptr;
  s_stackWalkCallback = nullptr;

  auto previousHandler = signal(SIGUSR1, signalHandler);
  if (previousHandler != SIG_ERR)
    s_previousUSR1Handler = previousHandler;

  previousHandler = signal(SIGBUS, signalHandler);
  if (previousHandler != SIG_ERR)
    s_previousBUShandler = previousHandler;

  previousHandler = signal(SIGFPE, signalHandler);
  if (previousHandler != SIG_ERR)
    s_previousFPEhandler = previousHandler;

  previousHandler = signal(SIGSEGV, signalHandler);
  if (previousHandler != SIG_ERR)
    s_previousEGVhandler = previousHandler;
}

void deinitialize() {
  if (s_previousEGVhandler != nullptr)
    signal(SIGSEGV, s_previousEGVhandler);
  if (s_previousFPEhandler != nullptr)
    signal(SIGFPE, s_previousFPEhandler);
  if (s_previousBUShandler != nullptr)
    signal(SIGBUS, s_previousBUShandler);
  if (s_previousUSR1Handler != nullptr)
    signal(SIGUSR1, s_previousUSR1Handler);

  s_stackWalkCallback = nullptr;
  s_assertionCallback = nullptr;
}

void setcallbacks(AssertCallback assertionCallback, StackWalkCallback stackWalkCallback) {
  s_assertionCallback = assertionCallback;
  s_stackWalkCallback = stackWalkCallback;
}

#ifdef _DEBUG

void assertion(const char * code, const char * message, const char * fileName, const char * functionName,
               std::size_t lineNumber) {
  char assertionString[4096];
  if (message == nullptr)
    snprintf(assertionString, sizeof(assertionString), "%s @ %s (%zu):\r\n\r\n%s", functionName, fileName, lineNumber,
             code);
  else
    snprintf(assertionString, sizeof(assertionString), "%s @ %s (%zu):\r\n\r\n%s: %s", functionName, fileName,
             lineNumber, message, code);

  static auto assertReEnter = false;
  if (assertReEnter)
    return;

  assertReEnter = true;

  if (s_assertionCallback != nullptr && !(*s_assertionCallback)(assertionString))
    raise(SIGUSR1); // raise USR1 signal

  assertReEnter = false;
}

#endif // _DEBUG

} // namespace assertion
} // namespace toygine
