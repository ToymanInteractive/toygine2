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
  \file   assertion_stub.cpp
  \brief  Stub definitions for the assertion callback API declared in \c core/assertion.hpp.
*/

#include <cstdio>

#include "core.hpp"

namespace toy::assertion {

namespace {

/// Size in bytes of the stack trace and assertion string buffers.
constexpr size_t c_stringBufferSize = 4096;

/// Active assertion callback; invoked when an assertion fires in debug builds.
AssertionCallback _assertionCallback{nullptr};

} // namespace

void initialize() noexcept {
  _assertionCallback = nullptr;
}

void deInitialize() noexcept {
  _assertionCallback = nullptr;
}

void setCallbacks(AssertionCallback assertionCallback, [[maybe_unused]] StackWalkCallback stackWalkCallback) noexcept {
  _assertionCallback = assertionCallback;
}

#ifdef _DEBUG

void assertion(const char * code, const char * message, const char * fileName, const char * functionName,
               size_t lineNumber) noexcept {
  static bool reEnter = false;
  if (reEnter)
    return;

  reEnter = true;

  char assertionString[c_stringBufferSize];
  int  written;

  if (message == nullptr)
    written = snprintf(assertionString, c_stringBufferSize, "%s @ %s (%zu):\r\n\r\n%s", functionName, fileName,
                       lineNumber, code);
  else
    written = snprintf(assertionString, c_stringBufferSize, "%s @ %s (%zu):\r\n\r\n%s: %s", functionName, fileName,
                       lineNumber, message, code);

  if (written >= static_cast<int>(c_stringBufferSize)) {
    constexpr const char * truncationMessage = "...[TRUNCATED]";
    constexpr auto         truncationLength  = char_traits<char>::length(truncationMessage) + 1;
#if defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
    strcpy_s(&assertionString[c_stringBufferSize - truncationLength], truncationLength, truncationMessage);
#else // defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
    std::strncpy(&assertionString[c_stringBufferSize - truncationLength], truncationMessage, truncationLength);
    assertionString[c_stringBufferSize - 1] = '\0';
#endif
  }

  if (_assertionCallback != nullptr)
    _assertionCallback(assertionString);

  reEnter = false;
}

#endif // _DEBUG

} // namespace toy::assertion
