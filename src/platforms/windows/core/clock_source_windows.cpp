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
  \file   clock_source_windows.cpp
  \brief  Windows implementations of \ref toy::chrono::ClockSource, \ref toy::chrono::SteadyClock, and
          \ref toy::chrono::SystemClock.
*/

#include "core.hpp"

namespace toy::chrono {

namespace {

/// Process-wide active source; null when none is registered.
ClockSource * activeSource{nullptr};

/// QueryPerformanceFrequency result cached at construction; constant for the process lifetime.
int64_t qpcFrequency{1};

} // namespace

ClockSource::ClockSource()
  : _frequency{c_steadyClockPeriodDenominator} {
  assert_message(activeSource == nullptr, "ClockSource: at most one active instance is allowed per process");

  LARGE_INTEGER freq{};
  const BOOL    ok = QueryPerformanceFrequency(&freq);
  assert_message(ok && freq.QuadPart > 0, "ClockSource: QueryPerformanceFrequency failed");
  if (!ok || freq.QuadPart <= 0)
    return;

  qpcFrequency = freq.QuadPart;
  activeSource = this;
}

ClockSource::~ClockSource() {
  activeSource = nullptr;
}

int64_t ClockSource::nowTicks() const noexcept {
  LARGE_INTEGER counter{};

  const BOOL ok = QueryPerformanceCounter(&counter);
  assert_message(ok, "ClockSource::nowTicks: QueryPerformanceCounter failed");
  if (!ok)
    return 0;

  // Convert QPC ticks to nanoseconds in two steps to avoid int64_t overflow.
  // At a typical QPC frequency of 10 MHz, direct multiplication (ticks * 1e9) overflows int64_t after ~920 seconds.
  const int64_t ticks     = counter.QuadPart;
  const int64_t seconds   = ticks / qpcFrequency;
  const int64_t remainder = ticks % qpcFrequency;

  return seconds * c_steadyClockPeriodDenominator + remainder * c_steadyClockPeriodDenominator / qpcFrequency;
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  assert_message(activeSource != nullptr, "SteadyClock::nowTicks: no active ClockSource");

  return activeSource != nullptr ? activeSource->nowTicks() : 0;
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  assert_message(activeSource != nullptr, "SteadyClock::frequency: no active ClockSource");

  return activeSource != nullptr ? activeSource->frequency() : 0;
}

SteadyClock::time_point SteadyClock::now() noexcept {
  assert_message(activeSource != nullptr, "SteadyClock::now: no active ClockSource");

  return activeSource != nullptr ? time_point{duration{activeSource->nowTicks()}} : time_point{};
}

SystemClock::time_point SystemClock::now() noexcept {
  return time_point{};
}

} // namespace toy::chrono
