//
// Copyright (c) 2026 Toyman Interactive
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
  \file   assertion_stub.cpp
  \brief  Implementations of \ref toy::assertion for targets with no reporting backend of their own.

  Formats the failure into a fixed 4096-byte buffer and hands it to the registered
  \ref toy::assertion::AssertionCallback, then answers what that handler decided. An unregistered handler answers
  \ref toy::assertion::Action::Break, and a description that overruns the buffer ends in \c "...[TRUNCATED]". A failure
  raised while another is still being reported answers \ref toy::assertion::Action::Ignore without a second report.
  Stack walking is absent, so the \ref toy::assertion::StackWalkCallback passed to toy::assertion::setCallbacks() is
  ignored.

  \note Selected by the platform CMake list of every target that carries no assertion backend of its own.
*/

#include <cstdlib>

#include "core.hpp"

namespace toy::assertion {

namespace {

#ifdef TOYGINE_DEBUG

/// Capacity of the buffer a failure description is formatted into, terminator included.
constexpr size_t stringBufferSize = 4096;

/// Handler last given to setCallbacks(), or \c nullptr while a failed check reports nothing.
AssertionCallback _assertionCallback{nullptr};

#endif // TOYGINE_DEBUG

} // namespace

void initialize() noexcept {
#ifdef TOYGINE_DEBUG
  _assertionCallback = nullptr;
#endif // TOYGINE_DEBUG
}

void deInitialize() noexcept {
#ifdef TOYGINE_DEBUG
  _assertionCallback = nullptr;
#endif // TOYGINE_DEBUG
}

void setCallbacks([[maybe_unused]] AssertionCallback assertionCallback,
                  [[maybe_unused]] StackWalkCallback stackWalkCallback) noexcept {
#ifdef TOYGINE_DEBUG
  _assertionCallback = assertionCallback;
#endif // TOYGINE_DEBUG
}

#ifdef TOYGINE_DEBUG

Action assertion(const char * code, const char * message, const char * fileName, const char * functionName,
                 size_t lineNumber) noexcept {
#ifndef __MEGA_DRIVE__
  static bool reEnter = false;
  if (reEnter)
    return Action::Ignore;

  reEnter = true;

  char assertionString[stringBufferSize];
  int  written;

  if (message == nullptr)
    written = snprintf(assertionString, stringBufferSize, "%s @ %s (%zu):\r\n\r\n%s", functionName, fileName,
                       lineNumber, code);
  else
    written = snprintf(assertionString, stringBufferSize, "%s @ %s (%zu):\r\n\r\n%s: %s", functionName, fileName,
                       lineNumber, message, code);

  if (written >= static_cast<int>(stringBufferSize)) {
    constexpr const char * truncationMessage = "...[TRUNCATED]";
    constexpr auto         truncationLength  = std::char_traits<char>::length(truncationMessage) + 1;
    std::memcpy(&assertionString[stringBufferSize - truncationLength], truncationMessage, truncationLength);
  }

  // An unregistered handler leaves the failure unreported, so the caller still has to stop at it.
  const Action action = _assertionCallback != nullptr ? _assertionCallback(assertionString) : Action::Break;

  reEnter = false;

  if (action == Action::Abort)
    std::abort();

  return action;
#else
  return Action::Break;
#endif // __MEGA_DRIVE__
}

#endif // TOYGINE_DEBUG

} // namespace toy::assertion
