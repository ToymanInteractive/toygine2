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
  \file   system_clock_posix.cpp
  \brief  POSIX implementations of \ref toy::chrono::SystemClock.
*/

#include "core.hpp"

namespace toy::chrono {

CalendarTime SystemClock::now() noexcept {
  timespec  ts{};
  const int rc = clock_gettime(CLOCK_REALTIME, &ts);
  assert_message(rc == 0, "SystemClock::now: clock_gettime(CLOCK_REALTIME) failed");
  if (rc != 0)
    return CalendarTime::invalid();

  tm utcTM;
  localtime_r(&ts.tv_sec, &utcTM);

  return CalendarTime{
    .year        = static_cast<int16_t>(utcTM.tm_year + 1900),
    .month       = static_cast<uint8_t>(utcTM.tm_mon + 1),
    .day         = static_cast<uint8_t>(utcTM.tm_mday),
    .dayOfWeek   = static_cast<uint8_t>(utcTM.tm_wday),
    .hour        = static_cast<uint8_t>(utcTM.tm_hour),
    .minute      = static_cast<uint8_t>(utcTM.tm_min),
    .second      = static_cast<uint8_t>(utcTM.tm_sec),
    .millisecond = static_cast<uint16_t>(ts.tv_nsec / 1'000'000),
  };
}

} // namespace toy::chrono
