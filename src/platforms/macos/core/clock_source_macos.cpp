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
  \file   clock_source_macos.cpp
  \brief  macOS implementation of \ref toy::chrono::ClockSource using CLOCK_MONOTONIC_RAW.
*/

#include <time.h>

#include "core.hpp"

namespace toy::chrono {

namespace {

/// Process-wide active source; null when none is registered.
ClockSource * activeSource{nullptr};

} // namespace

ClockSource::ClockSource()
  : _frequency{c_steadyClockPeriodDenominator} {
  assert_message(activeSource == nullptr, "ClockSource: at most one active instance is allowed per process");

  activeSource = this;
}

ClockSource::~ClockSource() {
  activeSource = nullptr;
}

int64_t ClockSource::nowTicks() const noexcept {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

  return ts.tv_sec * c_steadyClockPeriodDenominator + ts.tv_nsec;
}

} // namespace toy::chrono
