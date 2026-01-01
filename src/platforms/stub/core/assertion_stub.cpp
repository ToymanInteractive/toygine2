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
  \brief  Stub implementation of assertion system for platforms without native assertion support.
*/

#include <cstdio>

#include "core.hpp"

namespace toy::assertion {

static AssertionCallback s_assertionCallback = nullptr;

void initialize() {
  s_assertionCallback = nullptr;
}

void deInitialize() {
  s_assertionCallback = nullptr;
}

void setCallbacks(AssertionCallback assertionCallback, [[maybe_unused]] StackWalkCallback stackWalkCallback) {
  s_assertionCallback = assertionCallback;
}

#ifdef _DEBUG

void assertion(const char * code, const char * message, const char * fileName, const char * functionName,
               size_t lineNumber) {
  char assertionString[4096];
  int written;

  if (message == nullptr)
    written = snprintf(assertionString, sizeof(assertionString), "%s @ %s (%zu):\r\n\r\n%s", functionName, fileName,
                       lineNumber, code);
  else
    written = snprintf(assertionString, sizeof(assertionString), "%s @ %s (%zu):\r\n\r\n%s: %s", functionName, fileName,
                       lineNumber, message, code);

  // Check for truncation
  if (written >= static_cast<int>(sizeof(assertionString))) {
    constexpr const char * const truncationMessage = "...[TRUNCATED]";
    constexpr auto truncationLength = char_traits<char>::length(truncationMessage) + 1;
#if defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
    strcpy_s(&assertionString[sizeof(assertionString) - truncationLength], truncationLength, truncationMessage);
#else // defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
    std::strncpy(&assertionString[sizeof(assertionString) - truncationLength], truncationMessage, truncationLength);
#endif
  }

  static bool assertReEnter = false;
  if (assertReEnter)
    return;

  assertReEnter = true;

  if (s_assertionCallback != nullptr)
    (*s_assertionCallback)(assertionString);

  assertReEnter = false;
}

#endif // _DEBUG

} // namespace toy::assertion
