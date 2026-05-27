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
  \file   vector2.benchmark.cpp
  \brief  Implementation of picobench benchmarks for \ref toy::math::Vector2.
*/

#include "math.hpp"
#include "picobench/picobench.hpp"

namespace toy::math {

using fixed_type = fixed<int32_t, int64_t, 24>;

static void vector2FloatAddAssign(picobench::state & state) noexcept {
  constexpr Vector2 addition(5.0F, -10.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(10.0F + static_cast<float>(i & 1), 20.0F);
    v      += addition;
    result += static_cast<size_t>(v.x + v.y);
  }
  state.set_result(result);
}

static void vector2FixedAddAssign(picobench::state & state) noexcept {
  constexpr Vector2 addition(fixed_type(5), fixed_type(-10));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(10 + (i & 1)), fixed_type(20));
    v      += addition;
    result += static_cast<size_t>((v.x + v.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatSubtractAssign(picobench::state & state) noexcept {
  constexpr Vector2 subtraction(5.0F, 10.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(15.0F + static_cast<float>(i & 1), 25.0F);
    v      -= subtraction;
    result += static_cast<size_t>(v.x + v.y);
  }
  state.set_result(result);
}

static void vector2FixedSubtractAssign(picobench::state & state) noexcept {
  constexpr Vector2 subtraction(fixed_type(5), fixed_type(10));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(15 + (i & 1)), fixed_type(25));
    v      -= subtraction;
    result += static_cast<size_t>((v.x + v.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatMultiplyAssign(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(10.0F + static_cast<float>(i & 1), 20.0F);
    v      *= 2.5F;
    result += static_cast<size_t>(v.x + v.y);
  }
  state.set_result(result);
}

static void vector2FixedMultiplyAssign(picobench::state & state) noexcept {
  constexpr fixed_type scalar(2);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(10 + (i & 1)), fixed_type(20));
    v      *= scalar;
    result += static_cast<size_t>((v.x + v.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatDivideAssign(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(20.0F + static_cast<float>(i & 1), 45.0F);
    v      /= 2.5F;
    result += static_cast<size_t>(v.x + v.y);
  }
  state.set_result(result);
}

static void vector2FixedDivideAssign(picobench::state & state) noexcept {
  constexpr fixed_type scalar(2);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(20 + (i & 1)), fixed_type(45));
    v      /= scalar;
    result += static_cast<size_t>((v.x + v.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatSqrMagnitude(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(3.0F + static_cast<float>(i & 1), 4.0F);
    result += static_cast<size_t>(v.sqrMagnitude());
  }
  state.set_result(result);
}

static void vector2FixedSqrMagnitude(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(3 + (i & 1)), fixed_type(4));
    result += static_cast<size_t>(v.sqrMagnitude().rawValue());
  }
  state.set_result(result);
}

static void vector2FloatSetZero(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(100.0F + static_cast<float>(i & 1), 200.0F);
    v.setZero();
    result += static_cast<size_t>(v.x + v.y);
  }
  state.set_result(result);
}

static void vector2FixedSetZero(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(100 + (i & 1)), fixed_type(200));
    v.setZero();
    result += static_cast<size_t>((v.x + v.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatIsZero(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(static_cast<float>(i & 1), 0.0F);
    result += v.isZero() ? 1 : 0;
  }
  state.set_result(result);
}

static void vector2FixedIsZero(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(i & 1), fixed_type(0));
    result += v.isZero() ? 1 : 0;
  }
  state.set_result(result);
}

static void vector2FloatIsEqual(picobench::state & state) noexcept {
  constexpr Vector2 a(10.0F, 20.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 b(10.0F + static_cast<float>(i & 1), 20.0F);
    result += a.isEqual(b) ? 1 : 0;
  }
  state.set_result(result);
}

static void vector2FixedIsEqual(picobench::state & state) noexcept {
  constexpr Vector2 a(fixed_type(10), fixed_type(20));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 b(fixed_type(10 + (i & 1)), fixed_type(20));
    result += a.isEqual(b) ? 1 : 0;
  }
  state.set_result(result);
}

static void vector2FloatAdd(picobench::state & state) noexcept {
  constexpr Vector2 b(5.0F, -10.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 a(10.0F + static_cast<float>(i & 1), 20.0F);
    auto    r  = a + b;
    result    += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void vector2FixedAdd(picobench::state & state) noexcept {
  constexpr Vector2 b(fixed_type(5), fixed_type(-10));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 a(fixed_type(10 + (i & 1)), fixed_type(20));
    auto    r  = a + b;
    result    += static_cast<size_t>((r.x + r.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatSubtract(picobench::state & state) noexcept {
  constexpr Vector2 b(5.0F, 10.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 a(15.0F + static_cast<float>(i & 1), 25.0F);
    auto    r  = a - b;
    result    += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void vector2FixedSubtract(picobench::state & state) noexcept {
  constexpr Vector2 b(fixed_type(5), fixed_type(10));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 a(fixed_type(15 + (i & 1)), fixed_type(25));
    auto    r  = a - b;
    result    += static_cast<size_t>((r.x + r.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatNegate(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(10.0F + static_cast<float>(i & 1), -20.0F);
    auto    r  = -v;
    result    += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void vector2FixedNegate(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(10 + (i & 1)), fixed_type(-20));
    auto    r  = -v;
    result    += static_cast<size_t>((r.x + r.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatMultiplyScalar(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(10.0F + static_cast<float>(i & 1), 20.0F);
    auto    r  = v * 2.5F;
    result    += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void vector2FixedMultiplyScalar(picobench::state & state) noexcept {
  constexpr fixed_type scalar(2);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(10 + (i & 1)), fixed_type(20));
    auto    r  = v * scalar;
    result    += static_cast<size_t>((r.x + r.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatScalarMultiply(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(10.0F + static_cast<float>(i & 1), 20.0F);
    auto    r  = 2.5F * v;
    result    += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void vector2FixedScalarMultiply(picobench::state & state) noexcept {
  constexpr fixed_type scalar(2);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(10 + (i & 1)), fixed_type(20));
    auto    r  = scalar * v;
    result    += static_cast<size_t>((r.x + r.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatDot(picobench::state & state) noexcept {
  constexpr Vector2 b(5.0F, 10.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 a(10.0F + static_cast<float>(i & 1), 20.0F);
    auto    r  = a * b;
    result    += static_cast<size_t>(r);
  }
  state.set_result(result);
}

static void vector2FixedDot(picobench::state & state) noexcept {
  constexpr Vector2 b(fixed_type(5), fixed_type(10));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 a(fixed_type(10 + (i & 1)), fixed_type(20));
    auto    r  = a * b;
    result    += static_cast<size_t>(r.rawValue());
  }
  state.set_result(result);
}

static void vector2FloatDivide(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(25.0F + static_cast<float>(i & 1), 50.0F);
    auto    r  = v / 2.5F;
    result    += static_cast<size_t>(r.x + r.y);
  }
  state.set_result(result);
}

static void vector2FixedDivide(picobench::state & state) noexcept {
  constexpr fixed_type scalar(2);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 v(fixed_type(25 + (i & 1)), fixed_type(50));
    auto    r  = v / scalar;
    result    += static_cast<size_t>((r.x + r.y).rawValue());
  }
  state.set_result(result);
}

static void vector2FloatEqual(picobench::state & state) noexcept {
  constexpr Vector2 a(10.0F, 20.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 b(10.0F + static_cast<float>(i & 1), 20.0F);
    result += (a == b) ? 1 : 0;
  }
  state.set_result(result);
}

static void vector2FixedEqual(picobench::state & state) noexcept {
  constexpr Vector2 a(fixed_type(10), fixed_type(20));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 b(fixed_type(10 + (i & 1)), fixed_type(20));
    result += (a == b) ? 1 : 0;
  }
  state.set_result(result);
}

static void vector2FloatNotEqual(picobench::state & state) noexcept {
  constexpr Vector2 a(10.0F, 20.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 b(11.0F + static_cast<float>(i & 1), 20.0F);
    result += (a != b) ? 1 : 0;
  }
  state.set_result(result);
}

static void vector2FixedNotEqual(picobench::state & state) noexcept {
  constexpr Vector2 a(fixed_type(10), fixed_type(20));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 b(fixed_type(11 + (i & 1)), fixed_type(20));
    result += (a != b) ? 1 : 0;
  }
  state.set_result(result);
}

static void vector2FloatCross(picobench::state & state) noexcept {
  constexpr Vector2 b(1.0F, 0.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 a(3.0F + static_cast<float>(i & 1), 4.0F);
    auto    r  = cross(a, b);
    result    += static_cast<size_t>(r);
  }
  state.set_result(result);
}

static void vector2FixedCross(picobench::state & state) noexcept {
  constexpr Vector2 b(fixed_type(1), fixed_type(0));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Vector2 a(fixed_type(3 + (i & 1)), fixed_type(4));
    auto    r  = cross(a, b);
    result    += static_cast<size_t>(r.rawValue());
  }
  state.set_result(result);
}

PICOBENCH_SUITE("toy::math::Vector2");
PICOBENCH(vector2FloatAddAssign);
PICOBENCH(vector2FixedAddAssign);
PICOBENCH(vector2FloatSubtractAssign);
PICOBENCH(vector2FixedSubtractAssign);
PICOBENCH(vector2FloatMultiplyAssign);
PICOBENCH(vector2FixedMultiplyAssign);
PICOBENCH(vector2FloatDivideAssign);
PICOBENCH(vector2FixedDivideAssign);
PICOBENCH(vector2FloatSqrMagnitude);
PICOBENCH(vector2FixedSqrMagnitude);
PICOBENCH(vector2FloatSetZero);
PICOBENCH(vector2FixedSetZero);
PICOBENCH(vector2FloatIsZero);
PICOBENCH(vector2FixedIsZero);
PICOBENCH(vector2FloatIsEqual);
PICOBENCH(vector2FixedIsEqual);
PICOBENCH(vector2FloatAdd);
PICOBENCH(vector2FixedAdd);
PICOBENCH(vector2FloatSubtract);
PICOBENCH(vector2FixedSubtract);
PICOBENCH(vector2FloatNegate);
PICOBENCH(vector2FixedNegate);
PICOBENCH(vector2FloatMultiplyScalar);
PICOBENCH(vector2FixedMultiplyScalar);
PICOBENCH(vector2FloatScalarMultiply);
PICOBENCH(vector2FixedScalarMultiply);
PICOBENCH(vector2FloatDot);
PICOBENCH(vector2FixedDot);
PICOBENCH(vector2FloatDivide);
PICOBENCH(vector2FixedDivide);
PICOBENCH(vector2FloatEqual);
PICOBENCH(vector2FixedEqual);
PICOBENCH(vector2FloatNotEqual);
PICOBENCH(vector2FixedNotEqual);
PICOBENCH(vector2FloatCross);
PICOBENCH(vector2FixedCross);

} // namespace toy::math
