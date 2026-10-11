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
  \file   base_application_winapi.cpp
  \brief  Implementations of toy::application::BaseApplication::pid(), sleep(), and yield() for Windows.
*/

#include <windows.h>

// Undefine Windows macros that conflict with std:: functions
#undef min
#undef max

#include "application.hpp"

namespace toy::application {

uint32_t BaseApplication::pid() noexcept {
  static_assert(sizeof(uint32_t) == sizeof(DWORD), "uint32_t must hold a Windows process ID");

  return bit_cast<uint32_t>(::GetCurrentProcessId());
}

void BaseApplication::sleep(size_t milliseconds) noexcept {
  constexpr size_t maxSleepDuration = INFINITE - 1;

  do {
    const auto interval = std::min(milliseconds, maxSleepDuration);

    ::Sleep(static_cast<DWORD>(interval));

    milliseconds -= interval;
  } while (milliseconds > 0);
}

void BaseApplication::yield() noexcept {
  ::SwitchToThread();
}

} // namespace toy::application
