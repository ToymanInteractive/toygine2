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
  \brief  Implementations for vector2 nanobench benchmarks in the math module.
*/

#include "../utils.hpp"
#include "math.hpp"

namespace toy::math {

using fixed_type = fixed<int32_t, int64_t, 24>;

// Vector2 benchmarks
void vector2MathBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  bench.run("Vector2<float> coordinate construct", [] {
    Vector2 v(10.0F, 20.0F);

    doNotOptimize(v);
  });
  bench.run("Vector2<fixed_type> coordinate construct", [] {
    fixed_type x(10);
    fixed_type y(20);

    Vector2 v(x, y);

    doNotOptimize(v);
  });

  bench.run("Vector2<float> operator+=", [] {
    Vector2 addition(5.0F, -10.0F);
    Vector2 v(10.0F, 20.0F);

    v += addition;

    doNotOptimize(v);
  });
  bench.run("Vector2<fixed_type> operator+=", [] {
    Vector2 addition(fixed_type(5), fixed_type(-10));
    Vector2 v(fixed_type(10), fixed_type(20));

    v += addition;

    doNotOptimize(v);
  });

  bench.run("Vector2<float> operator-=", [] {
    Vector2 subtraction(5.0F, 10.0F);
    Vector2 v(15.0F, 25.0F);

    v -= subtraction;

    doNotOptimize(v);
  });
  bench.run("Vector2<fixed_type> operator-=", [] {
    Vector2 subtraction(fixed_type(5), fixed_type(10));
    Vector2 v(fixed_type(15), fixed_type(25));

    v -= subtraction;

    doNotOptimize(v);
  });

  bench.run("Vector2<float> operator*=", [] {
    Vector2 v(10.0F, 20.0F);

    v *= 2.5F;

    doNotOptimize(v);
  });
  bench.run("Vector2<fixed_type> operator*=", [] {
    fixed_type scalar(2);
    Vector2    v(fixed_type(10), fixed_type(20));

    v *= scalar;

    doNotOptimize(v);
  });

  bench.run("Vector2<float> operator/=", [] {
    Vector2 v(20.0F, 45.0F);

    v /= 2.5F;

    doNotOptimize(v);
  });
  bench.run("Vector2<fixed_type> operator/=", [] {
    fixed_type scalar(2);
    Vector2    v(fixed_type(20), fixed_type(45));

    v /= scalar;

    doNotOptimize(v);
  });

  bench.run("Vector2<float> sqrMagnitude", [] {
    Vector2 v(3.0F, 4.0F);

    auto r = v.sqrMagnitude();

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> sqrMagnitude", [] {
    Vector2 v(fixed_type(3), fixed_type(4));

    auto r = v.sqrMagnitude();

    doNotOptimize(r);
  });

  bench.run("Vector2<float> setZero", [] {
    Vector2 v(100.0F, 200.0F);

    v.setZero();

    doNotOptimize(v);
  });
  bench.run("Vector2<fixed_type> setZero", [] {
    Vector2 v(fixed_type(100), fixed_type(200));

    v.setZero();

    doNotOptimize(v);
  });

  bench.run("Vector2<float> isZero", [] {
    Vector2 v(0.0F, 0.0F);

    auto r = v.isZero();

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> isZero", [] {
    Vector2 v(fixed_type(0), fixed_type(0));

    auto r = v.isZero();

    doNotOptimize(r);
  });

  bench.run("Vector2<float> isEqual", [] {
    Vector2 a(10.0F, 20.0F);
    Vector2 b(10.0F, 20.0F);

    auto r = a.isEqual(b);

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> isEqual", [] {
    Vector2 a(fixed_type(10), fixed_type(20));
    Vector2 b(fixed_type(10), fixed_type(20));

    auto r = a.isEqual(b);

    doNotOptimize(r);
  });

  bench.run("Vector2<float> operator+", [] {
    Vector2 a(10.0F, 20.0F);
    Vector2 b(5.0F, -10.0F);

    auto r = a + b;

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> operator+", [] {
    Vector2 a(fixed_type(10), fixed_type(20));
    Vector2 b(fixed_type(5), fixed_type(-10));

    auto r = a + b;

    doNotOptimize(r);
  });

  bench.run("Vector2<float> operator-", [] {
    Vector2 a(15.0F, 25.0F);
    Vector2 b(5.0F, 10.0F);

    auto r = a - b;

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> operator-", [] {
    Vector2 a(fixed_type(15), fixed_type(25));
    Vector2 b(fixed_type(5), fixed_type(10));

    auto r = a - b;

    doNotOptimize(r);
  });

  bench.run("Vector2<float> operator- unary", [] {
    Vector2 v(10.0F, -20.0F);

    auto r = -v;

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> operator- unary", [] {
    Vector2 v(fixed_type(10), fixed_type(-20));

    auto r = -v;

    doNotOptimize(r);
  });

  bench.run("Vector2<float> operator* scalar", [] {
    Vector2 v(10.0F, 20.0F);

    auto r = v * 2.5F;

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> operator* scalar", [] {
    Vector2    v(fixed_type(10), fixed_type(20));
    fixed_type scalar(2);

    auto r = v * scalar;

    doNotOptimize(r);
  });

  bench.run("Vector2<float> scalar * vector", [] {
    Vector2 v(10.0F, 20.0F);

    auto r = 2.5F * v;

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> scalar * vector", [] {
    Vector2 v(fixed_type(10), fixed_type(20));

    auto r = fixed_type(2) * v;

    doNotOptimize(r);
  });

  bench.run("Vector2<float> dot product", [] {
    Vector2 a(10.0F, 20.0F);
    Vector2 b(5.0F, 10.0F);

    auto r = a * b;

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> dot product", [] {
    Vector2 a(fixed_type(10), fixed_type(20));
    Vector2 b(fixed_type(5), fixed_type(10));

    auto r = a * b;

    doNotOptimize(r);
  });

  bench.run("Vector2<float> operator/", [] {
    Vector2 v(25.0F, 50.0F);

    auto r = v / 2.5F;

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> operator/", [] {
    Vector2    v(fixed_type(25), fixed_type(50));
    fixed_type scalar(2);

    auto r = v / scalar;

    doNotOptimize(r);
  });

  bench.run("Vector2<float> operator==", [] {
    Vector2 a(10.0F, 20.0F);
    Vector2 b(10.0F, 20.0F);

    auto r = (a == b);

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> operator==", [] {
    Vector2 a(fixed_type(10), fixed_type(20));
    Vector2 b(fixed_type(10), fixed_type(20));

    auto r = (a == b);

    doNotOptimize(r);
  });

  bench.run("Vector2<float> operator!=", [] {
    Vector2 a(10.0F, 20.0F);
    Vector2 b(11.0F, 20.0F);

    auto r = (a != b);

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> operator!=", [] {
    Vector2 a(fixed_type(10), fixed_type(20));
    Vector2 b(fixed_type(11), fixed_type(20));

    auto r = (a != b);

    doNotOptimize(r);
  });

  bench.run("Vector2<float> cross", [] {
    Vector2 a(3.0F, 4.0F);
    Vector2 b(1.0F, 0.0F);

    auto r = cross(a, b);

    doNotOptimize(r);
  });
  bench.run("Vector2<fixed_type> cross", [] {
    Vector2 a(fixed_type(3), fixed_type(4));
    Vector2 b(fixed_type(1), fixed_type(0));

    auto r = cross(a, b);

    doNotOptimize(r);
  });
}

} // namespace toy::math
