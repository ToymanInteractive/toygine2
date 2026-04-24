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
  \file   chrono_clock_source_gba.cpp
  \brief  Implementations for \ref toy::chrono::ClockSource and \ref toy::chrono::SteadyClock on GBA.

  Builds a 32-bit tick counter by cascading hardware timers TM2 (low 16 bits, prescaler = 1) and TM3 (high 16 bits,
  cascade mode). The combined counter runs at the GBA system clock (16.78 MHz) and rolls over every ~256 seconds.

  \note The counter rolls over after approximately 256 seconds at 16.78 MHz. Extended uptime requires a software
        extension via VBlank IRQ, which is deferred to a future module.
*/

#include "core.hpp"

namespace toy::chrono {

static ClockSource * g_active = nullptr;

constexpr uintptr_t c_regTm2CntL = 0x04000108;
constexpr uintptr_t c_regTm2CntH = 0x0400010A;
constexpr uintptr_t c_regTm3CntL = 0x0400010C;
constexpr uintptr_t c_regTm3CntH = 0x0400010E;

constexpr uint16_t c_tmEnable  = 0x0080;
constexpr uint16_t c_tmCascade = 0x0004;

inline volatile uint16_t & reg16(uintptr_t address) noexcept {
  return *reinterpret_cast<volatile uint16_t *>(address);
}

inline int64_t readCascade() noexcept {
  // Double-read the high word to guard against a carry between TM2 and TM3 reads.
  const uint16_t high1 = reg16(c_regTm3CntL);
  const uint16_t low   = reg16(c_regTm2CntL);
  const uint16_t high2 = reg16(c_regTm3CntL);
  const uint16_t high  = (high1 != high2) ? reg16(c_regTm3CntL) : high1;

  return (static_cast<int64_t>(high) << 16) | static_cast<int64_t>(low);
}

ClockSource::ClockSource() noexcept {
  assert_message(g_active == nullptr, "A ClockSource is already active; only one instance per process is allowed");

  reg16(c_regTm2CntH) = 0;
  reg16(c_regTm3CntH) = 0;
  reg16(c_regTm2CntL) = 0;
  reg16(c_regTm3CntL) = 0;
  reg16(c_regTm3CntH) = c_tmEnable | c_tmCascade;
  reg16(c_regTm2CntH) = c_tmEnable;
  _frequency          = static_cast<int64_t>(c_steadyClockPeriodDenominator);
  g_active            = this;
}

ClockSource::~ClockSource() noexcept {
  reg16(c_regTm2CntH) = 0;
  reg16(c_regTm3CntH) = 0;
  g_active            = nullptr;
}

int64_t ClockSource::nowTicks() const noexcept {
  return readCascade();
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  return readCascade();
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return g_active->frequency();
}

SteadyClock::time_point SteadyClock::now() noexcept {
  return time_point{duration{readCascade()}};
}

} // namespace toy::chrono
