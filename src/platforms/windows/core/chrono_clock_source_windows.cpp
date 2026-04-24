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
  \file   chrono_clock_source_windows.cpp
  \brief  Implementations for \ref toy::chrono::ClockSource and \ref toy::chrono::SteadyClock on Windows.

  Uses \c QueryPerformanceCounter for tick reads and \c QueryPerformanceFrequency for the hardware-reported frequency.
  Covers all Windows architectures (x64, ARM64). On modern hardware QPC is backed by the invariant TSC (x64) or
  \c CNTVCT_EL0 (ARM64) and is accessed through a kernel-mapped page without a syscall.
*/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include "core.hpp"

namespace toy::chrono {

static ClockSource * g_active{nullptr};
static int64_t       g_qpcBase{0};
static double        g_nsPerTick{0.0};

inline int64_t readQpc() noexcept {
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);

  return counter.QuadPart;
}

ClockSource::ClockSource() noexcept {
  assert_message(g_active == nullptr, "A ClockSource is already active; only one instance per process is allowed");

  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  _frequency = freq.QuadPart;

  g_nsPerTick = 1e9 / static_cast<double>(_frequency);
  g_qpcBase   = readQpc();
  g_active    = this;
}

ClockSource::~ClockSource() noexcept {
  g_active = nullptr;
}

int64_t ClockSource::nowTicks() const noexcept {
  return readQpc() - g_qpcBase;
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  return readQpc() - g_qpcBase;
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return g_active->frequency();
}

SteadyClock::time_point SteadyClock::now() noexcept {
  const auto ns = static_cast<rep>(static_cast<double>(readQpc() - g_qpcBase) * g_nsPerTick);

  return time_point{duration{ns}};
}

} // namespace toy::chrono
