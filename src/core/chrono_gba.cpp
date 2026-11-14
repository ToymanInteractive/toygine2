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
  \file   chrono_gba.cpp
  \brief  Implementations of \ref toy::chrono::SteadyClock, \ref toy::chrono::SystemClock, and
          \ref toy::chrono::Timestamp over the Timer 2+3 cascade.
*/

#include <gba_timers.h>

#include "core.hpp"

namespace toy::chrono {

namespace {

/// Wrap count of the Timer 3 cascade, incremented by its overflow interrupt handler
volatile uint32_t overflowCount = 0;

/// Raw cascade reading the Timestamp origin counts from, taken by initialize()
SteadyClock::rep timestampOrigin = 0;

/// Prescaler bits of a timer control word; this backend always runs prescaler 1, so they must read zero
constexpr uint16_t timerPrescalerMask = 0x0003;

/// Control bits this backend configures: prescaler 1, Timer 3 cascade with overflow IRQ, both started
constexpr uint16_t timerControlMask = timerPrescalerMask | TIMER_COUNT | TIMER_IRQ | TIMER_START;

/// Overflow interrupt handler of Timer 3, extends readings past the 32-bit wrap
void onTimer3Overflow() noexcept {
  overflowCount = overflowCount + 1;
}

} // namespace

void initialize() noexcept {
  assert_message(INT_VECTOR == ::IntrMain,
                 "chrono::initialize: the interrupt dispatcher is not installed, call irqInit() first");

  if ((REG_TM2CNT_H & timerControlMask) == TIMER_START
      && (REG_TM3CNT_H & timerControlMask) == (TIMER_COUNT | TIMER_IRQ | TIMER_START)) {
    // The running cascade is this backend's: preserve the steady counter and re-fix the timestamp origin only.
    timestampOrigin = SteadyClock::nowTicks();
    ::irqSet(IRQ_TIMER3, &onTimer3Overflow);
    ::irqEnable(IRQ_TIMER3);

    return;
  }

  ::irqDisable(IRQ_TIMER3);

  REG_TM2CNT_H = 0;
  REG_TM3CNT_H = 0;
  REG_TM2CNT_L = 0;
  REG_TM3CNT_L = 0;

  // A wrap left pending by the previous epoch must not extend the fresh one.
  REG_IF          = IRQ_TIMER3;
  overflowCount   = 0;
  timestampOrigin = 0;

  ::irqSet(IRQ_TIMER3, &onTimer3Overflow);

  REG_TM3CNT_H = TIMER_COUNT | TIMER_IRQ | TIMER_START;
  REG_TM2CNT_H = TIMER_START;

  ::irqEnable(IRQ_TIMER3);
}

SteadyClock::time_point SteadyClock::now() noexcept {
  assert_message(REG_TM2CNT_H & TIMER_START, "SteadyClock::now: initialize() has not run");

  return time_point{duration{nowTicks()}};
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  for (;;) {
    const auto overflows = overflowCount;
    const auto high      = REG_TM3CNT_L;
    const auto low       = REG_TM2CNT_L;

    if (REG_TM3CNT_L != high || overflowCount != overflows)
      continue;

    if (REG_IF & IRQ_TIMER3) {
      // The wrap's handler has not run; within one wrap window the pending carry is exactly one.
      if (REG_TM3CNT_L != high)
        continue;

      return (static_cast<rep>(overflows) + 1) << 32 | static_cast<rep>(high) << 16 | low;
    }

    // A completed delivery pairs the acked flag with the incremented word, so a clear IF means the snapshot is whole.
    return static_cast<rep>(overflows) << 32 | static_cast<rep>(high) << 16 | low;
  }
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return steadyClockPeriodDenominator;
}

CalendarTime SystemClock::now() noexcept {
  const auto uptime = duration_cast<Duration<int64_t, std::milli>>(SteadyClock::now().time_since_epoch());

  return CalendarTime::fromUptime(uptime);
}

Timestamp Timestamp::now() noexcept {
  assert_message(REG_TM2CNT_H & TIMER_START, "Timestamp::now: initialize() has not run");

  const auto delta = SteadyClock::nowTicks() - timestampOrigin;

  const int64_t milliseconds = delta / steadyClockPeriodDenominator * 1000
                               + delta % steadyClockPeriodDenominator * 1000 / steadyClockPeriodDenominator;

  return fromMilliseconds(milliseconds);
}

} // namespace toy::chrono
