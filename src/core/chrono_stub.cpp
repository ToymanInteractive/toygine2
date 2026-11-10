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
  \file   chrono_stub.cpp
  \brief  Implementations of \ref toy::chrono::SteadyClock, \ref toy::chrono::SystemClock, and
          \ref toy::chrono::Timestamp for targets without a counter backend: ticks stay zero.
*/

#include "core.hpp"

namespace toy::chrono {

void initialize() noexcept {}

SteadyClock::time_point SteadyClock::now() noexcept {
  return time_point{};
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  return 0;
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return steadyClockPeriodDenominator;
}

CalendarTime SystemClock::now() noexcept {
  const auto uptime = duration_cast<Duration<int64_t, std::milli>>(SteadyClock::now().time_since_epoch());

  return CalendarTime::fromUptime(uptime);
}

Timestamp Timestamp::now() noexcept {
  return Timestamp{};
}

} // namespace toy::chrono
