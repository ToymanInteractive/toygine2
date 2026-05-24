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
  \file   ellipse.benchmark.cpp
  \brief  Implementation of picobench benchmarks for \ref toy::geometry::Ellipse.
*/

#include "geometry.hpp"
#include "picobench/picobench.hpp"

namespace toy::geometry {

using fixed_type = math::fixed<int32_t, int64_t, 24>;

static void ellipseFloatArea(picobench::state & state) noexcept {
  Ellipse e(math::Vector2(0.0F, 0.0F), math::Vector2(10.0F, 5.0F));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    e.radiuses.x  = 10.0F + static_cast<float>(i & 1);
    result       += static_cast<size_t>(e.area());
  }
  state.set_result(result);
}

static void ellipseFixedArea(picobench::state & state) noexcept {
  Ellipse e(math::Vector2(fixed_type(0), fixed_type(0)), math::Vector2(fixed_type(10), fixed_type(5)));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    e.radiuses.x  = 10 + (i & 1);
    result       += static_cast<size_t>(e.area().rawValue());
  }
  state.set_result(result);
}

static void ellipseFloatReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Ellipse e(math::Vector2(10.0F, 20.0F), math::Vector2(5.0F + static_cast<float>(i & 1), 3.0F));
    e.reset();
    result += static_cast<size_t>(e.radiuses.x);
  }
  state.set_result(result);
}

static void ellipseFixedReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Ellipse e(math::Vector2(fixed_type(10), fixed_type(20)), math::Vector2(fixed_type(5 + (i & 1)), fixed_type(3)));
    e.reset();
    result += static_cast<size_t>(e.radiuses.x.rawValue());
  }
  state.set_result(result);
}

static void ellipseFloatIsReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Ellipse e(math::Vector2(10.0F, 20.0F), math::Vector2(5.0F + static_cast<float>(i & 1), 3.0F));
    e.reset();
    result += e.isReset() ? 1 : 0;
  }
  state.set_result(result);
}

static void ellipseFixedIsReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Ellipse e(math::Vector2(fixed_type(10), fixed_type(20)), math::Vector2(fixed_type(5 + (i & 1)), fixed_type(3)));
    e.reset();
    result += e.isReset() ? 1 : 0;
  }
  state.set_result(result);
}

static void ellipseFloatIsValid(picobench::state & state) noexcept {
  Ellipse e(math::Vector2(0.0F, 0.0F), math::Vector2(1.0F, 1.0F));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    e.radiuses.x  = 1.0F + static_cast<float>(i & 1);
    result       += e.isValid() ? 1 : 0;
  }
  state.set_result(result);
}

static void ellipseFixedIsValid(picobench::state & state) noexcept {
  Ellipse e(math::Vector2(fixed_type(0), fixed_type(0)), math::Vector2(fixed_type(1), fixed_type(1)));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    e.radiuses.x  = 1 + (i & 1);
    result       += e.isValid() ? 1 : 0;
  }
  state.set_result(result);
}

static void ellipseFloatIsContain(picobench::state & state) noexcept {
  constexpr Ellipse e(math::Vector2(0.0F, 0.0F), math::Vector2(10.0F, 5.0F));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += static_cast<size_t>(e.isContain(math::Vector2(5.0F + static_cast<float>(i & 1), 2.0F)));

  state.set_result(result);
}

static void ellipseFixedIsContain(picobench::state & state) noexcept {
  constexpr Ellipse e(math::Vector2(fixed_type(0), fixed_type(0)), math::Vector2(fixed_type(10), fixed_type(5)));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += static_cast<size_t>(e.isContain(math::Vector2(fixed_type(5 + (i & 1)), fixed_type(2))));

  state.set_result(result);
}

static void ellipseFloatEqual(picobench::state & state) noexcept {
  constexpr Ellipse a(math::Vector2(1.0F, 2.0F), math::Vector2(3.0F, 4.0F));
  Ellipse           b(math::Vector2(1.0F, 2.0F), math::Vector2(3.0F, 4.0F));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.radiuses.x  = 3.0F + static_cast<float>(i & 1);
    result       += static_cast<size_t>(a == b);
  }
  state.set_result(result);
}

static void ellipseFixedEqual(picobench::state & state) noexcept {
  constexpr Ellipse a(math::Vector2(fixed_type(1), fixed_type(2)), math::Vector2(fixed_type(3), fixed_type(4)));
  Ellipse           b(math::Vector2(fixed_type(1), fixed_type(2)), math::Vector2(fixed_type(3), fixed_type(4)));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.radiuses.x  = 3 + (i & 1);
    result       += static_cast<size_t>(a == b);
  }
  state.set_result(result);
}

static void ellipseFloatNotEqual(picobench::state & state) noexcept {
  constexpr Ellipse a(math::Vector2(1.0F, 2.0F), math::Vector2(3.0F, 4.0F));
  Ellipse           b(math::Vector2(1.0F, 2.0F), math::Vector2(3.0F, 5.0F));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.radiuses.x  = 3.0F + static_cast<float>(i & 1);
    result       += static_cast<size_t>(a != b);
  }
  state.set_result(result);
}

static void ellipseFixedNotEqual(picobench::state & state) noexcept {
  constexpr Ellipse a(math::Vector2(fixed_type(1), fixed_type(2)), math::Vector2(fixed_type(3), fixed_type(4)));
  Ellipse           b(math::Vector2(fixed_type(1), fixed_type(2)), math::Vector2(fixed_type(3), fixed_type(5)));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.radiuses.x  = 3 + (i & 1);
    result       += static_cast<size_t>(a != b);
  }
  state.set_result(result);
}

PICOBENCH_SUITE("toy::geometry::Ellipse");
PICOBENCH(ellipseFloatConstruct);
PICOBENCH(ellipseFixedConstruct);
PICOBENCH(ellipseFloatArea);
PICOBENCH(ellipseFixedArea);
PICOBENCH(ellipseFloatReset);
PICOBENCH(ellipseFixedReset);
PICOBENCH(ellipseFloatIsReset);
PICOBENCH(ellipseFixedIsReset);
PICOBENCH(ellipseFloatIsValid);
PICOBENCH(ellipseFixedIsValid);
PICOBENCH(ellipseFloatIsContain);
PICOBENCH(ellipseFixedIsContain);
PICOBENCH(ellipseFloatEqual);
PICOBENCH(ellipseFixedEqual);
PICOBENCH(ellipseFloatNotEqual);
PICOBENCH(ellipseFixedNotEqual);

} // namespace toy::geometry
