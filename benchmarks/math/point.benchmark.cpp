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
  \file   point.benchmark.cpp
  \brief  Implementation of picobench benchmarks for \ref toy::math::Point.
*/

#include "math.hpp"
#include "picobench/picobench.hpp"

namespace toy::math {

using fixed_type = fixed<int32_t, int64_t, 24>;

static void pointAddAssign(picobench::state & state) noexcept {
  constexpr Point addition(5, -3);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(10 + (i & 1), 20);
    p      += addition;
    result += static_cast<size_t>(p.x + p.y);
  }
  state.set_result(result);
}

static void pointSubtractAssign(picobench::state & state) noexcept {
  constexpr Point subtraction(5, 10);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(15 + (i & 1), 25);
    p      -= subtraction;
    result += static_cast<size_t>(p.x + p.y);
  }
  state.set_result(result);
}

static void pointMultiplyAssignInt(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(10 + (i & 1), 20);
    p      *= 2;
    result += static_cast<size_t>(p.x + p.y);
  }
  state.set_result(result);
}

static void pointMultiplyAssignFloat(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(10 + (i & 1), 20);
    p      *= 2.5F;
    result += static_cast<size_t>(p.x + p.y);
  }
  state.set_result(result);
}

static void pointMultiplyAssignFixed(picobench::state & state) noexcept {
  constexpr fixed_type scalar(2);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(10 + (i & 1), 20);
    p      *= scalar;
    result += static_cast<size_t>(p.x + p.y);
  }
  state.set_result(result);
}

static void pointDivideAssignInt(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(30 + (i & 1), 60);
    p      /= 3;
    result += static_cast<size_t>(p.x + p.y);
  }
  state.set_result(result);
}

static void pointDivideAssignFloat(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(25 + (i & 1), 50);
    p      /= 2.5F;
    result += static_cast<size_t>(p.x + p.y);
  }
  state.set_result(result);
}

static void pointDivideAssignFixed(picobench::state & state) noexcept {
  constexpr fixed_type scalar(3);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(30 + (i & 1), 60);
    p      /= scalar;
    result += static_cast<size_t>(p.x + p.y);
  }
  state.set_result(result);
}

static void pointAdd(picobench::state & state) noexcept {
  constexpr Point b(5, -10);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point a(10 + (i & 1), 20);
    auto  r  = a + b;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void pointSubtract(picobench::state & state) noexcept {
  constexpr Point b(5, 10);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point a(15 + (i & 1), 25);
    auto  r  = a - b;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void pointNegate(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(10 + (i & 1), -20);
    auto  r  = -p;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void pointMultiplyInt(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(10 + (i & 1), 20);
    auto  r  = p * 3;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void pointMultiplyFloat(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(10 + (i & 1), 20);
    auto  r  = p * 2.5F;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void pointMultiplyFixed(picobench::state & state) noexcept {
  constexpr fixed_type scalar(2);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(10 + (i & 1), 20);
    auto  r  = p * scalar;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void fixedMultiplyPoint(picobench::state & state) noexcept {
  constexpr fixed_type scalar(3);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(5 + (i & 1), 10);
    auto  r  = scalar * p;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void pointDivideInt(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(30 + (i & 1), 60);
    auto  r  = p / 3;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void pointDivideFloat(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(25 + (i & 1), 50);
    auto  r  = p / 2.5F;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void pointDivideFixed(picobench::state & state) noexcept {
  constexpr fixed_type scalar(3);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(30 + (i & 1), 60);
    auto  r  = p / scalar;
    result  += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void pointIsZero(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point p(i & 1, 0);
    result += p.isZero() ? 1 : 0;
  }
  state.set_result(result);
}

static void pointIsEqual(picobench::state & state) noexcept {
  constexpr Point a(10, 20);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point b(10 + (i & 1), 20);
    result += a.isEqual(b) ? 1 : 0;
  }
  state.set_result(result);
}

static void pointEqual(picobench::state & state) noexcept {
  constexpr Point a(10, 20);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Point b(10 + (i & 1), 20);
    result += (a == b) ? 1 : 0;
  }
  state.set_result(result);
}

PICOBENCH_SUITE("toy::math::point");
PICOBENCH(pointAddAssign);
PICOBENCH(pointSubtractAssign);
PICOBENCH(pointMultiplyAssignInt);
PICOBENCH(pointMultiplyAssignFloat);
PICOBENCH(pointMultiplyAssignFixed);
PICOBENCH(pointDivideAssignInt);
PICOBENCH(pointDivideAssignFloat);
PICOBENCH(pointDivideAssignFixed);
PICOBENCH(pointAdd);
PICOBENCH(pointSubtract);
PICOBENCH(pointNegate);
PICOBENCH(pointMultiplyInt);
PICOBENCH(pointMultiplyFloat);
PICOBENCH(pointMultiplyFixed);
PICOBENCH(fixedMultiplyPoint);
PICOBENCH(pointDivideInt);
PICOBENCH(pointDivideFloat);
PICOBENCH(pointDivideFixed);
PICOBENCH(pointIsZero);
PICOBENCH(pointIsEqual);
PICOBENCH(pointEqual);

} // namespace toy::math
