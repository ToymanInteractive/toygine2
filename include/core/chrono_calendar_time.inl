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
  \file   chrono_calendar_time.inl
  \brief  Inline implementations for \ref toy::chrono::CalendarTime uptime synthesis.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_CALENDAR_TIME_INL_
#define INCLUDE_CORE_CHRONO_CALENDAR_TIME_INL_

namespace toy::chrono {

constexpr CalendarTime CalendarTime::fromUptime(const Duration<int64_t, std::milli> uptime) noexcept {
  assert_message(uptime.count() >= 0, "CalendarTime::fromUptime: the uptime must be non-negative");

  constexpr int64_t msPerSecond   = 1000;
  constexpr int64_t secondsPerDay = 24LL * 60 * 60;
  constexpr int64_t daysPerMonth  = 30;
  constexpr int64_t monthsPerYear = 12;

  const int64_t totalSeconds = uptime.count() / msPerSecond;
  const int64_t days         = totalSeconds / secondsPerDay;
  const int64_t years        = days / (daysPerMonth * monthsPerYear);

  assert_message(years <= std::numeric_limits<int16_t>::max(),
                 "CalendarTime::fromUptime: the uptime exceeds what the year field holds");

  return CalendarTime{.year        = static_cast<int16_t>(years),
                      .month       = static_cast<uint8_t>((days % (daysPerMonth * monthsPerYear)) / daysPerMonth),
                      .day         = static_cast<uint8_t>(days % daysPerMonth),
                      .hour        = static_cast<uint8_t>((totalSeconds / (60 * 60)) % 24),
                      .minute      = static_cast<uint8_t>((totalSeconds / 60) % 60),
                      .second      = static_cast<uint8_t>(totalSeconds % 60),
                      .millisecond = static_cast<uint16_t>(uptime.count() % msPerSecond)};
}

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_CALENDAR_TIME_INL_
