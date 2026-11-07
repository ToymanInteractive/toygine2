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
  \file   chrono_windows.cpp
  \brief  Implementations of \ref toy::chrono::SteadyClock, \ref toy::chrono::SystemClock, and
          \ref toy::chrono::Timestamp over QueryPerformanceCounter and the Windows local-time APIs.
*/

#include "core.hpp"

namespace toy::chrono {

namespace {

/// QueryPerformanceCounter ticks per second, cached by initialize()
int64_t cachedFrequency = 0;

/// Counter reading taken by initialize(), the origin Timestamp counts from
int64_t startTicks = 0;

} // namespace

void initialize() noexcept {
  LARGE_INTEGER frequency{};
  LARGE_INTEGER counter{};

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&counter);

  cachedFrequency = frequency.QuadPart;
  startTicks      = counter.QuadPart;
}

SteadyClock::time_point SteadyClock::now() noexcept {
  assert_message(cachedFrequency > 0, "SteadyClock::now: initialize() has not run");

  const rep ticks = nowTicks();

  const rep seconds     = ticks / cachedFrequency;
  const rep remainder   = ticks % cachedFrequency;
  const rep nanoseconds = seconds * steadyClockPeriodDenominator
                          + remainder * steadyClockPeriodDenominator / cachedFrequency;

  return time_point{duration{nanoseconds}};
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  LARGE_INTEGER counter{};

  QueryPerformanceCounter(&counter);

  return counter.QuadPart;
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return cachedFrequency;
}

CalendarTime SystemClock::now() noexcept {
  SYSTEMTIME local{};
  GetLocalTime(&local);

  TIME_ZONE_INFORMATION timeZone{};
  const DWORD           zoneId = GetTimeZoneInformation(&timeZone);
  assert_message(zoneId != TIME_ZONE_ID_INVALID, "SystemClock::now: GetTimeZoneInformation failed");

  int32_t offsetMinutes = -timeZone.Bias;
  if (zoneId == TIME_ZONE_ID_DAYLIGHT)
    offsetMinutes -= timeZone.DaylightBias;
  else if (zoneId == TIME_ZONE_ID_STANDARD)
    offsetMinutes -= timeZone.StandardBias;

  return CalendarTime{.year            = static_cast<int16_t>(local.wYear),
                      .month           = static_cast<uint8_t>(local.wMonth),
                      .day             = static_cast<uint8_t>(local.wDay),
                      .dayOfWeek       = static_cast<uint8_t>(local.wDayOfWeek),
                      .hour            = static_cast<uint8_t>(local.wHour),
                      .minute          = static_cast<uint8_t>(local.wMinute),
                      .second          = static_cast<uint8_t>(local.wSecond),
                      .millisecond     = static_cast<uint16_t>(local.wMilliseconds),
                      .tzOffsetMinutes = static_cast<int16_t>(offsetMinutes)};
}

Timestamp Timestamp::now() noexcept {
  assert_message(cachedFrequency > 0, "Timestamp::now: initialize() has not run");

  const SteadyClock::rep delta = SteadyClock::nowTicks() - startTicks;

  const int64_t milliseconds = delta / cachedFrequency * 1000 + delta % cachedFrequency * 1000 / cachedFrequency;

  return fromMilliseconds(milliseconds);
}

} // namespace toy::chrono
