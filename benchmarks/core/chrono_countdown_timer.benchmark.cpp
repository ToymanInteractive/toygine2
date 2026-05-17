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
  \file   chrono_countdown_timer.benchmark.cpp
  \brief  Picobench benchmarks for \ref toy::chrono::CountdownTimer.
*/

#include "core.hpp"
#include "picobench/picobench.hpp"

namespace toy::chrono {

constexpr auto timeout = seconds{1};

static void constructor(picobench::state & state) noexcept {
  ClockSource clock;

  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    CountdownTimer timer{timeout};
}

static void expired(picobench::state & state) noexcept {
  ClockSource clock;

  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    CountdownTimer timer{timeout};

    [[maybe_unused]] auto val = timer.expired();
  }
}

static void remaining(picobench::state & state) noexcept {
  ClockSource clock;

  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    CountdownTimer timer{timeout};

    [[maybe_unused]] auto val = timer.remaining();
  }
}

PICOBENCH_SUITE("toy::chrono::CountdownTimer");
PICOBENCH(constructor);
PICOBENCH(expired);
PICOBENCH(remaining);

} // namespace toy::chrono
