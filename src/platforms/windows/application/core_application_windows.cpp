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
  \file   core_application_windows.cpp
  \brief  Windows implementations of \ref toy::application::CoreApplication via POSIX APIs.
*/

#include <windows.h>

#include "application.hpp"

namespace toy::application {

uint32_t CoreApplication::pid() const noexcept {
  static_assert(sizeof(uint32_t) == sizeof(DWORD), "uint32_t must be large enough to hold DWORD");

  return bit_cast<uint32_t>(GetCurrentProcessId());
}

void CoreApplication::sleep(size_t milliseconds) const noexcept {
  assert_message(milliseconds <= UINT32_MAX, "milliseconds must be less than or equal to UINT32_MAX");

  Sleep(static_cast<DWORD>(milliseconds));
}

} // namespace toy::application
