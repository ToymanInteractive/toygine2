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
  \file   assertion_macos.cpp
  \brief  macOS implementation of the \ref toy::assertion crash handler and stack trace capture.

  Registers signal handlers for SIGBUS, SIGFPE, SIGSEGV, and SIGUSR1. On signal delivery, captures a demangled stack
  trace via backtrace(3) and dladdr(3) and forwards it to the registered \ref toy::assertion::StackWalkCallback.
*/

#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <signal.h>

#include "core.hpp"

namespace toy::assertion {

namespace {

/// Maximum number of stack frames captured per stack trace.
constexpr size_t c_maxStackFrames = 63;

/// Size in bytes of the stack trace and assertion string buffers.
constexpr size_t c_stringBufferSize = 4096;

/// Active assertion callback; invoked when an assertion fires in debug builds.
AssertionCallback _assertCallback = nullptr;

/// Active stack-walk callback; invoked with the formatted stack trace on crash.
StackWalkCallback _stackWalkCallback = nullptr;

/// Set after the first initialize(); prevents overwriting saved handlers on repeated calls.
bool _initialized = false;

/// Previous SIGUSR1 handler, restored on deInitialize().
sig_t _previousUSR1Handler = nullptr;
/// Previous SIGBUS handler, restored on deInitialize().
sig_t _previousBUShandler  = nullptr;
/// Previous SIGFPE handler, restored on deInitialize().
sig_t _previousFPEhandler  = nullptr;
/// Previous SIGSEGV handler, restored on deInitialize().
sig_t _previousEGVhandler  = nullptr;

/*!
  \struct SignalInfo

  \brief  Maps a POSIX signal identifier to a human-readable description.
*/
struct SignalInfo {
  /// POSIX signal number (e.g. \c SIGSEGV).
  int          id;
  /// Short human-readable description of the signal's cause.
  const char * description;
};

/// Lookup table mapping POSIX signal identifiers to short descriptions.
constexpr array<SignalInfo, 31> c_signals = {
  {{SIGHUP, "terminal line hangup"},
   {SIGINT, "interrupt program"},
   {SIGQUIT, "quit program"},
   {SIGILL, "illegal instruction"},
   {SIGTRAP, "trace trap"},
   {SIGABRT, "abort program"},
   {SIGEMT, "emulate instruction executed"},
   {SIGFPE, "floating-point exception"},
   {SIGKILL, "kill program"},
   {SIGBUS, "bus error"},
   {SIGSEGV, "segmentation violation"},
   {SIGSYS, "non-existent system call invoked"},
   {SIGPIPE, "write on a pipe with no reader"},
   {SIGALRM, "real-time timer expired"},
   {SIGTERM, "software termination signal"},
   {SIGURG, "urgent condition present on socket"},
   {SIGSTOP, "stop (cannot be caught or ignored)"},
   {SIGTSTP, "stop signal generated from keyboard"},
   {SIGCONT, "continue after stop"},
   {SIGCHLD, "child status has changed"},
   {SIGTTIN, "background read attempted from control terminal"},
   {SIGTTOU, "background write attempted to control terminal"},
   {SIGIO, "I/O is possible on a descriptor"},
   {SIGXCPU, "cpu time limit exceeded"},
   {SIGXFSZ, "file size limit exceeded"},
   {SIGVTALRM, "virtual time alarm"},
   {SIGPROF, "profiling timer alarm"},
   {SIGWINCH, "Window size change"},
   {SIGINFO, "status request from keyboard"},
   {SIGUSR1, "User defined signal 1"},
   {SIGUSR2, "User defined signal 2"}}
};

/*!
  \brief  Returns the human-readable description for a POSIX signal identifier.

  \param  id  POSIX signal identifier.

  \return Description string if \a id is present in \c c_signals, \c nullptr otherwise.
*/
[[nodiscard]] const char * signalDescription(int id) noexcept {
  for (const auto & signal : c_signals) {
    if (signal.id == id)
      return signal.description;
  }

  return nullptr;
}

/*!
  \brief  Captures the current call stack and formats it into \a dest.

  Each frame is written as <tt>module!symbol + N bytes</tt>, one per line. C++ symbols are demangled via
  abi::__cxa_demangle when possible; raw mangled names are used on failure.

  \param  dest        Destination buffer.
  \param  destSize    Capacity of \a dest in bytes.
  \param  skipFrames  Number of top frames to omit to hide signal-handling boilerplate (default: \c 1).
*/
void fillStacktrace(char * dest, size_t destSize, size_t skipFrames = 1) noexcept {
  array<void *, c_maxStackFrames + 1> addrList;

  const auto frameCount = static_cast<size_t>(backtrace(addrList.data(), static_cast<int>(addrList.size())));
  if (frameCount == 0) {
    snprintf(dest, destSize, "\t<empty, possibly corrupt>\n");

    return;
  }

  size_t offset = 0;

  for (size_t index = skipFrames; index < frameCount && offset < destSize; ++index) {
    Dl_info info = {};
    if (!dladdr(addrList[index], &info))
      continue;

    const char * fullName   = info.dli_fname != nullptr ? info.dli_fname : "?";
    const char * lastSlash  = strrchr(fullName, '/');
    const char * moduleName = (lastSlash != nullptr) ? lastSlash + 1 : fullName;

    const char * symbolName = info.dli_sname != nullptr ? info.dli_sname : "?";
    const auto   shift
      = static_cast<ptrdiff_t>(static_cast<const char *>(addrList[index]) - static_cast<const char *>(info.dli_saddr));

    int          status;
    char *       demangled = abi::__cxa_demangle(symbolName, nullptr, nullptr, &status);
    const char * name      = (status == 0) ? demangled : symbolName;

    if (const int n = snprintf(dest + offset, destSize - offset, "%s!%s + %td bytes\n", moduleName, name, shift); n > 0)
      offset += static_cast<size_t>(n);

    free(demangled);
  }
}

/*!
  \brief  Signal handler invoked on SIGBUS, SIGFPE, SIGSEGV, and SIGUSR1.

  Formats the signal description and a demangled stack trace into a fixed-size buffer, then forwards the result to \c
  _stackWalkCallback. Terminates the process after the callback returns.

  \param  signalId  POSIX signal identifier delivered to the process.
*/
[[noreturn]] void handler(int signalId) noexcept {
  if (_stackWalkCallback == nullptr)
    exit(EXIT_FAILURE);

  char   buffer[c_stringBufferSize] = "";
  size_t offset                     = 0;

  if (signalId != SIGUSR1) {
    const auto * description = signalDescription(signalId);
    const auto   number = (description != nullptr)
                            ? snprintf(buffer, c_stringBufferSize, "Handled signal %d (%s)\n\n", signalId, description)
                            : snprintf(buffer, c_stringBufferSize, "Handled signal %d\n\n", signalId);
    if (number > 0)
      offset = static_cast<size_t>(number);
  }

  fillStacktrace(buffer + offset, c_stringBufferSize - offset, signalId == SIGUSR1 ? 6 : 4);

  _stackWalkCallback(buffer);

  exit(EXIT_FAILURE);
}

} // namespace

void initialize() noexcept {
  if (_initialized)
    return;

  _initialized = true;

  _assertCallback    = nullptr;
  _stackWalkCallback = nullptr;

  sig_t prevHandler = signal(SIGUSR1, handler);
  if (prevHandler != SIG_ERR)
    _previousUSR1Handler = prevHandler;

  prevHandler = signal(SIGBUS, handler);
  if (prevHandler != SIG_ERR)
    _previousBUShandler = prevHandler;

  prevHandler = signal(SIGFPE, handler);
  if (prevHandler != SIG_ERR)
    _previousFPEhandler = prevHandler;

  prevHandler = signal(SIGSEGV, handler);
  if (prevHandler != SIG_ERR)
    _previousEGVhandler = prevHandler;
}

void deInitialize() noexcept {
  if (!_initialized)
    return;

  _stackWalkCallback = nullptr;
  _assertCallback    = nullptr;

  if (_previousEGVhandler != nullptr)
    signal(SIGSEGV, _previousEGVhandler);
  if (_previousUSR1Handler != nullptr)
    signal(SIGUSR1, _previousUSR1Handler);
  if (_previousBUShandler != nullptr)
    signal(SIGBUS, _previousBUShandler);
  if (_previousFPEhandler != nullptr)
    signal(SIGFPE, _previousFPEhandler);

  _previousEGVhandler  = nullptr;
  _previousUSR1Handler = nullptr;
  _previousBUShandler  = nullptr;
  _previousFPEhandler  = nullptr;

  _initialized = false;
}

void setCallbacks(AssertionCallback assertionCallback, StackWalkCallback stackWalkCallback) noexcept {
  _assertCallback    = assertionCallback;
  _stackWalkCallback = stackWalkCallback;
}

#ifdef _DEBUG

void assertion(const char * code, const char * message, const char * fileName, const char * functionName,
               size_t lineNumber) noexcept {
  static bool reEnter = false;
  if (reEnter)
    return;

  reEnter = true;

  char assertionString[c_stringBufferSize];
  if (message == nullptr)
    snprintf(assertionString, c_stringBufferSize, "%s @ %s (%zu):\r\n\r\n%s", functionName, fileName, lineNumber, code);
  else
    snprintf(assertionString, c_stringBufferSize, "%s @ %s (%zu):\r\n\r\n%s: %s", functionName, fileName, lineNumber,
             message, code);

  if (_assertCallback != nullptr && !_assertCallback(assertionString))
    raise(SIGUSR1);

  reEnter = false;
}

#endif // _DEBUG

} // namespace toy::assertion
