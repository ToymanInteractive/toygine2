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
          \ref toy::chrono::Timestamp over the Timer 2+3 cascade and the cartridge RTC.

  The system clock reads the cartridge RTC chip through the vendored agbabi driver: the chip is probed once by
  toy::chrono::initialize(), and every reading pays one GPIO transaction on the order of a hundred microseconds.
  Where the chip is absent or its reading unusable, the system clock synthesizes the calendar from the uptime.
*/

#include <agbabi.h>
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

/// BCD-encoded byte to binary; the RTC chip stores every field as two packed BCD digits
[[nodiscard]] constexpr int bcdToDecimal(uint8_t bcd) noexcept {
  return (bcd & 0x0F) + (bcd >> 4) * 10;
}

/// Day count of \p month in the 2000-2099 range; every fourth year of the range is leap
[[nodiscard]] constexpr int monthLength(int month, bool leapYear) noexcept {
  switch (month) {
    case 2:
      return leapYear ? 29 : 28;

    case 4:
    case 6:
    case 9:
    case 11:
      return 30;

    default:
      return 31;
  }
}

/// Lifecycle of the one-shot RTC probe
enum class RtcProbe : uint8_t {
  NotProbed,   ///< initialize() has not probed the cartridge chip yet.
  Available,   ///< The probe found a usable RTC chip.
  Unavailable, ///< The probe found no chip or an unusable one; the system clock synthesizes.
};

/// State of the one-shot RTC probe; initialize() advances it once
RtcProbe rtcProbeState = RtcProbe::NotProbed;

/// Emulator marker scanned by tooling; initialize() touches it through a volatile load, pinning it into the ROM
alignas(int) const char rtcMarker[] = "SIIRTC_V001";

/// Overflow interrupt handler of Timer 3, extends readings past the 32-bit wrap
void onTimer3Overflow() noexcept {
  overflowCount = overflowCount + 1;
}

} // namespace

void initialize() noexcept {
  assert_message(INT_VECTOR == ::IntrMain,
                 "chrono::initialize: the interrupt dispatcher is not installed, call irqInit() first");

  if (rtcProbeState == RtcProbe::NotProbed)
    rtcProbeState = ::__agbabi_rtc_init() == 0 ? RtcProbe::Available : RtcProbe::Unavailable;

  // The observable volatile load keeps the marker's section reachable through gc-sections; the value is not used
  static_cast<void>(*static_cast<const volatile char *>(rtcMarker));

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
  if (rtcProbeState == RtcProbe::Available) {
    const auto datetime = ::__agbabi_rtc_datetime();

    // datetime[0]: year | month << 8 | day << 16 | weekday << 24, datetime[1]: hour | minute << 8 | second << 16;
    // the driver always reports 24-hour time, its PM fix already folded the flag
    const int year      = 2000 + bcdToDecimal(static_cast<uint8_t>(datetime[0] & 0xFF));
    const int month     = bcdToDecimal(static_cast<uint8_t>(datetime[0] >> 8 & 0xFF));
    const int day       = bcdToDecimal(static_cast<uint8_t>(datetime[0] >> 16 & 0xFF));
    const int dayOfWeek = bcdToDecimal(static_cast<uint8_t>(datetime[0] >> 24 & 0xFF));
    const int hour      = bcdToDecimal(static_cast<uint8_t>(datetime[1] & 0xFF));
    const int minute    = bcdToDecimal(static_cast<uint8_t>(datetime[1] >> 8 & 0xFF));
    const int second    = bcdToDecimal(static_cast<uint8_t>(datetime[1] >> 16 & 0xFF));

    // Garbage from a failing chip is data, not a caller invariant: fall back to synthesizing instead of asserting
    const bool usable = month >= 1 && month <= 12 && day >= 1 && day <= monthLength(month, year % 4 == 0)
                        && dayOfWeek <= 6 && hour <= 23 && minute <= 59 && second <= 59;

    if (usable) {
      return CalendarTime{.year            = static_cast<int16_t>(year),
                          .month           = static_cast<uint8_t>(month),
                          .day             = static_cast<uint8_t>(day),
                          .dayOfWeek       = static_cast<uint8_t>(dayOfWeek),
                          .hour            = static_cast<uint8_t>(hour),
                          .minute          = static_cast<uint8_t>(minute),
                          .second          = static_cast<uint8_t>(second),
                          .millisecond     = 0,
                          .tzOffsetMinutes = 0};
    }
  }

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
