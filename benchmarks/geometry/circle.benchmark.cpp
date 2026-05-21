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
  \file   circle.benchmark.cpp
  \brief  Implementation of picobench benchmarks for \ref toy::geometry::Circle.
*/

#include "geometry.hpp"
#include "picobench/picobench.hpp"

namespace toy::geometry {

using fixed_type = math::fixed<int32_t, int64_t, 24>;

static void circleFloatConstruct(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(5.0F, 10.0F), 3.0F);
    result += static_cast<size_t>(c.radius);
  }
  state.set_result(result);
}

static void circleFixedConstruct(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(fixed_type(5), fixed_type(10)), fixed_type(3));
    result += static_cast<size_t>(c.radius.rawValue());
  }
  state.set_result(result);
}

static void circleFloatArea(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(0.0F, 0.0F), 10.0F);
    result += static_cast<size_t>(c.area());
  }
  state.set_result(result);
}

static void circleFixedArea(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(10));
    result += static_cast<size_t>(c.area().rawValue());
  }
  state.set_result(result);
}

static void circleFloatReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(10.0F, 20.0F), 5.0F);
    c.reset();
    result += static_cast<size_t>(c.radius);
  }
  state.set_result(result);
}

static void circleFixedReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(fixed_type(10), fixed_type(20)), fixed_type(5));
    c.reset();
    result += static_cast<size_t>(c.radius.rawValue());
  }
  state.set_result(result);
}

static void circleFloatIsReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle<float> c;
    c.reset();
    result += static_cast<size_t>(c.isReset());
  }
  state.set_result(result);
}

static void circleFixedIsReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle<fixed_type> c;
    c.reset();
    result += static_cast<size_t>(c.isReset());
  }
  state.set_result(result);
}

static void circleFloatIsValid(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(0.0F, 0.0F), 1.0F);
    result += static_cast<size_t>(c.isValid());
  }
  state.set_result(result);
}

static void circleFixedIsValid(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(1));
    result += static_cast<size_t>(c.isValid());
  }
  state.set_result(result);
}

static void circleFloatIsContain(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(0.0F, 0.0F), 10.0F);
    result += static_cast<size_t>(c.isContain(math::Vector2(5.0F, 0.0F)));
  }
  state.set_result(result);
}

static void circleFixedIsContain(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle c(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(10));
    result += static_cast<size_t>(c.isContain(math::Vector2(fixed_type(5), fixed_type(0))));
  }
  state.set_result(result);
}

static void circleFloatEqual(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle a(math::Vector2(1.0F, 2.0F), 3.0F);
    Circle b(math::Vector2(1.0F, 2.0F), 3.0F);
    result += static_cast<size_t>(a == b);
  }
  state.set_result(result);
}

static void circleFixedEqual(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle a(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
    Circle b(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
    result += static_cast<size_t>(a == b);
  }
  state.set_result(result);
}

static void circleFloatNotEqual(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle a(math::Vector2(1.0F, 2.0F), 3.0F);
    Circle b(math::Vector2(1.0F, 2.0F), 4.0F);
    result += static_cast<size_t>(a != b);
  }
  state.set_result(result);
}

static void circleFixedNotEqual(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Circle a(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
    Circle b(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(4));
    result += static_cast<size_t>(a != b);
  }
  state.set_result(result);
}

PICOBENCH_SUITE("toy::geometry::Circle");
PICOBENCH(circleFloatConstruct);
PICOBENCH(circleFixedConstruct);
PICOBENCH(circleFloatArea);
PICOBENCH(circleFixedArea);
PICOBENCH(circleFloatReset);
PICOBENCH(circleFixedReset);
PICOBENCH(circleFloatIsReset);
PICOBENCH(circleFixedIsReset);
PICOBENCH(circleFloatIsValid);
PICOBENCH(circleFixedIsValid);
PICOBENCH(circleFloatIsContain);
PICOBENCH(circleFixedIsContain);
PICOBENCH(circleFloatEqual);
PICOBENCH(circleFixedEqual);
PICOBENCH(circleFloatNotEqual);
PICOBENCH(circleFixedNotEqual);

} // namespace toy::geometry
