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
  \file   chrono_posix.cpp
  \brief  Implementations of \ref toy::chrono::SteadyClock, \ref toy::chrono::SystemClock, and
          \ref toy::chrono::Timestamp over the POSIX clock APIs, shared by macOS and Linux.
*/

#include <time.h>

#include "core.hpp"

namespace toy::chrono {

namespace {

/// Counter reading taken by initialize(), the origin Timestamp counts from
int64_t startTicks = 0;

/*!
  \brief Computes the local offset from UTC in minutes from two breakdowns of one instant.

  \param local Local-time breakdown of the instant.
  \param utc   UTC breakdown of the same instant.

  \return Minutes the local time runs ahead of UTC, east positive.

  \note A year-day difference beyond one wraps the year boundary and counts as a one-day step the other way.
*/
int localUtcOffsetMinutes(const tm & local, const tm & utc) noexcept {
  int dayDelta = local.tm_yday - utc.tm_yday;
  if (dayDelta > 1)
    dayDelta = -1;
  if (dayDelta < -1)
    dayDelta = 1;

  return dayDelta * (24 * 60) + (local.tm_hour - utc.tm_hour) * 60 + (local.tm_min - utc.tm_min);
}

} // namespace

void initialize() noexcept {
  startTicks = SteadyClock::nowTicks();
}

SteadyClock::time_point SteadyClock::now() noexcept {
  return time_point{duration{nowTicks()}};
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  timespec  ts{};
  const int rc = clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  assert_message(rc == 0, "SteadyClock::nowTicks: clock_gettime(CLOCK_MONOTONIC_RAW) failed");

  return static_cast<rep>(ts.tv_sec) * steadyClockPeriodDenominator + ts.tv_nsec;
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return steadyClockPeriodDenominator;
}

CalendarTime SystemClock::now() noexcept {
  timespec  realtime{};
  const int rc = clock_gettime(CLOCK_REALTIME, &realtime);
  assert_message(rc == 0, "SystemClock::now: clock_gettime(CLOCK_REALTIME) failed");

  tm local{};
  tm utc{};
  localtime_r(&realtime.tv_sec, &local);
  gmtime_r(&realtime.tv_sec, &utc);

  return CalendarTime{.year            = static_cast<int16_t>(local.tm_year + 1900),
                      .month           = static_cast<uint8_t>(local.tm_mon + 1),
                      .day             = static_cast<uint8_t>(local.tm_mday),
                      .dayOfWeek       = static_cast<uint8_t>(local.tm_wday),
                      .hour            = static_cast<uint8_t>(local.tm_hour),
                      .minute          = static_cast<uint8_t>(local.tm_min),
                      .second          = static_cast<uint8_t>(local.tm_sec),
                      .millisecond     = static_cast<uint16_t>(realtime.tv_nsec / 1'000'000),
                      .tzOffsetMinutes = static_cast<int16_t>(localUtcOffsetMinutes(local, utc))};
}

Timestamp Timestamp::now() noexcept {
  const auto elapsed = duration_cast<Duration<int64_t, std::milli>>(SteadyClock::duration{SteadyClock::nowTicks()
                                                                                          - startTicks});

  return fromMilliseconds(elapsed.count());
}

} // namespace toy::chrono
