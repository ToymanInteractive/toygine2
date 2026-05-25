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
  \file   fixed.benchmark.cpp
  \brief  Implementation of picobench benchmarks for \ref toy::math::fixed.
*/

#include "math.hpp"
#include "picobench/picobench.hpp"

namespace toy::math {

using fixed_type = fixed<int32_t, int64_t, 24>;

static void fixedAssignFixed(picobench::state & state) noexcept {
  fixed_type a(0);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    fixed_type b(42 + (i & 1));
    a       = b;
    result += static_cast<size_t>(a.rawValue());
  }
  state.set_result(result);
}

static void fixedAssignInt(picobench::state & state) noexcept {
  fixed_type a(0);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    a       = 42 + (i & 1);
    result += static_cast<size_t>(a.rawValue());
  }
  state.set_result(result);
}

static void fixedEqualFixed(picobench::state & state) noexcept {
  constexpr fixed_type a(42);
  fixed_type           b(42);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b       = fixed_type(42 + (i & 1));
    result += (a == b) ? 1 : 0;
  }
  state.set_result(result);
}

static void fixedEqualInt(picobench::state & state) noexcept {
  constexpr fixed_type a(42);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    int b   = 42 + (i & 1);
    result += (a == b) ? 1 : 0;
  }
  state.set_result(result);
}

static void intEqualFixed(picobench::state & state) noexcept {
  constexpr fixed_type a(42);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    int b   = 42 + (i & 1);
    result += (b == a) ? 1 : 0;
  }
  state.set_result(result);
}

static void fixedCompareFixed(picobench::state & state) noexcept {
  constexpr fixed_type a(42);
  fixed_type           b(42);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b       = fixed_type(42 + (i & 1));
    result += ((a <=> b) < 0) ? 1 : 0;
  }
  state.set_result(result);
}

static void fixedCompareInt(picobench::state & state) noexcept {
  constexpr fixed_type a(42);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    int b   = 42 + (i & 1);
    result += ((a <=> b) < 0) ? 1 : 0;
  }
  state.set_result(result);
}

static void intCompareFixed(picobench::state & state) noexcept {
  constexpr fixed_type a(42);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    int b   = 42 + (i & 1);
    result += ((b <=> a) < 0) ? 1 : 0;
  }
  state.set_result(result);
}

PICOBENCH_SUITE("toy::math::fixed");
PICOBENCH(fixedAssignFixed);
PICOBENCH(fixedAssignInt);
PICOBENCH(fixedEqualFixed);
PICOBENCH(fixedEqualInt);
PICOBENCH(intEqualFixed);
PICOBENCH(fixedCompareFixed);
PICOBENCH(fixedCompareInt);
PICOBENCH(intCompareFixed);

} // namespace toy::math
