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
  \file   chrono_clock_source_arm64.cpp
  \brief  Implementations for \ref toy::chrono::ClockSource and \ref toy::chrono::SteadyClock on AArch64.

  Reads \c CNTVCT_EL0 (virtual counter) for tick values and \c CNTFRQ_EL0 for the hardware-reported frequency. No
  runtime calibration is required: the frequency register is architecturally stable. A base tick captured at
  construction is subtracted from every read to keep floating-point conversion in a range where nanosecond precision is
  maintained throughout typical engine uptime.
*/

#include "core.hpp"

namespace toy::chrono {

static ClockSource * g_active{nullptr};
static int64_t       g_cntBase{0};
static double        g_nsPerTick{0.0};

inline int64_t readCntvct() noexcept {
  uint64_t val;

  __asm__ volatile("mrs %0, cntvct_el0" : "=r"(val));

  return static_cast<int64_t>(val);
}

inline int64_t readCntfrq() noexcept {
  uint64_t freq;

  __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(freq));

  return static_cast<int64_t>(freq);
}

ClockSource::ClockSource() {
  assert_message(g_active == nullptr, "A ClockSource is already active; only one instance per process is allowed");

  _frequency = readCntfrq();

  g_nsPerTick = 1e9 / static_cast<double>(_frequency);
  g_cntBase   = readCntvct();
  g_active    = this;
}

ClockSource::~ClockSource() {
  g_active = nullptr;
}

int64_t ClockSource::nowTicks() const noexcept {
  return readCntvct() - g_cntBase;
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  return readCntvct() - g_cntBase;
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return g_active->frequency();
}

SteadyClock::time_point SteadyClock::now() noexcept {
  const auto ns = static_cast<rep>(static_cast<double>(readCntvct() - g_cntBase) * g_nsPerTick);

  return time_point{duration{ns}};
}

} // namespace toy::chrono
