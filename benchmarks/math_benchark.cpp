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
  \file   math_benchark.cpp
  \brief  Implementation of nanobench benchmarks for the math module.
*/

#include "benchmark_factory.hpp"
#include "math.hpp"

namespace toy::math {

void fixedMathBenchmarks(ankerl::nanobench::Bench &) noexcept;
void pointMathBenchmarks(ankerl::nanobench::Bench & bench) noexcept;

} // namespace toy::math

using namespace toy::math;

using fixed_type = fixed<int32_t, int64_t, 24>;

void runMathBenchmarks() noexcept {
  auto bench = createBench("Math module");

  toy::math::fixedMathBenchmarks(bench);
  toy::math::pointMathBenchmarks(bench);

  // Vector2 benchmarks (float and fixed_type)
  {
    bench.run("Vector2<float> coordinate construct", [] {
      Vector2 v(10.0f, 20.0f);
      doNotOptimize(v);
    });
    bench.run("Vector2<fixed_type> coordinate construct", [] {
      Vector2 v(fixed_type(10), fixed_type(20));
      doNotOptimize(v);
    });

    bench.run("Vector2<float> operator+=", [] {
      Vector2 v(10.0f, 20.0f);
      v += Vector2(5.0f, -10.0f);
      doNotOptimize(v);
    });
    bench.run("Vector2<fixed_type> operator+=", [] {
      Vector2<fixed_type> v(fixed_type(10), fixed_type(20));
      v += Vector2<fixed_type>(fixed_type(5), fixed_type(-10));
      doNotOptimize(v);
    });

    bench.run("Vector2<float> operator-=", [] {
      Vector2 v(15.0f, 25.0f);
      v -= Vector2(5.0f, 10.0f);
      doNotOptimize(v);
    });
    bench.run("Vector2<fixed_type> operator-=", [] {
      Vector2 v(fixed_type(15), fixed_type(25));
      v -= Vector2(fixed_type(5), fixed_type(10));
      doNotOptimize(v);
    });

    bench.run("Vector2<float> operator*=", [] {
      Vector2 v(10.0f, 20.0f);
      v *= 2.5f;
      doNotOptimize(v);
    });
    bench.run("Vector2<fixed_type> operator*=", [] {
      Vector2 v(fixed_type(10), fixed_type(20));
      v *= fixed_type(2);
      doNotOptimize(v);
    });

    bench.run("Vector2<float> operator/=", [] {
      Vector2 v(20.0f, 45.0f);
      v /= 2.5f;
      doNotOptimize(v);
    });
    bench.run("Vector2<fixed_type> operator/=", [] {
      Vector2 v(fixed_type(20), fixed_type(45));
      v /= fixed_type(2);
      doNotOptimize(v);
    });

    bench.run("Vector2<float> sqrMagnitude", [] {
      Vector2 v(3.0f, 4.0f);
      auto r = v.sqrMagnitude();
      doNotOptimize(r);
    });
    bench.run("Vector2<fixed_type> sqrMagnitude", [] {
      Vector2 v(fixed_type(3), fixed_type(4));
      auto r = v.sqrMagnitude();
      doNotOptimize(r);
    });

    bench.run("Vector2<float> setZero", [] {
      Vector2 v(100.0f, 200.0f);
      v.setZero();
      doNotOptimize(v);
    });
    bench.run("Vector2<fixed_type> setZero", [] {
      Vector2 v(fixed_type(100), fixed_type(200));
      v.setZero();
      doNotOptimize(v);
    });

    bench.run("Vector2<float> isZero", [] {
      Vector2 v(0.0f, 0.0f);
      auto r = v.isZero();
      doNotOptimize(r);
    });
    bench.run("Vector2<fixed_type> isZero", [] {
      Vector2 v(fixed_type(0), fixed_type(0));
      auto r = v.isZero();
      doNotOptimize(r);
    });

    bench.run("Vector2<float> isEqual", [] {
      Vector2 a(10.0f, 20.0f);
      Vector2 b(10.0f, 20.0f);
      auto r = a.isEqual(b);
      doNotOptimize(r);
    });
    bench.run("Vector2<fixed_type> isEqual", [] {
      Vector2 a(fixed_type(10), fixed_type(20));
      Vector2 b(fixed_type(10), fixed_type(20));
      auto r = a.isEqual(b);
      doNotOptimize(r);
    });

    bench.run("Vector2<float> c_arr", [] {
      Vector2 v(42.0f, -17.0f);
      auto * arr = v.c_arr();
      doNotOptimize(arr);
    });
    bench.run("Vector2<fixed_type> c_arr", [] {
      Vector2 v(fixed_type(42), fixed_type(-17));
      auto * arr = v.c_arr();
      doNotOptimize(arr);
    });

    bench.run("Vector2<float> operator+", [] {
      Vector2 a(10.0f, 20.0f);
      Vector2 b(5.0f, -10.0f);
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
      Vector2 a(15.0f, 25.0f);
      Vector2 b(5.0f, 10.0f);
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
      Vector2 v(10.0f, -20.0f);
      auto r = -v;
      doNotOptimize(r);
    });
    bench.run("Vector2<fixed_type> operator- unary", [] {
      Vector2 v(fixed_type(10), fixed_type(-20));
      auto r = -v;
      doNotOptimize(r);
    });

    bench.run("Vector2<float> operator* scalar", [] {
      Vector2 v(10.0f, 20.0f);
      auto r = v * 2.5f;
      doNotOptimize(r);
    });
    bench.run("Vector2<fixed_type> operator* scalar", [] {
      Vector2 v(fixed_type(10), fixed_type(20));
      auto r = v * fixed_type(2);
      doNotOptimize(r);
    });

    bench.run("Vector2<float> scalar * vector", [] {
      Vector2 v(10.0f, 20.0f);
      auto r = 2.5f * v;
      doNotOptimize(r);
    });
    bench.run("Vector2<fixed_type> scalar * vector", [] {
      Vector2 v(fixed_type(10), fixed_type(20));
      auto r = fixed_type(2) * v;
      doNotOptimize(r);
    });

    bench.run("Vector2<float> dot product", [] {
      Vector2 a(10.0f, 20.0f);
      Vector2 b(5.0f, 10.0f);
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
      Vector2 v(25.0f, 50.0f);
      auto r = v / 2.5f;
      doNotOptimize(r);
    });
    bench.run("Vector2<fixed_type> operator/", [] {
      Vector2 v(fixed_type(25), fixed_type(50));
      auto r = v / fixed_type(2);
      doNotOptimize(r);
    });

    bench.run("Vector2<float> operator==", [] {
      Vector2 a(10.0f, 20.0f);
      Vector2 b(10.0f, 20.0f);
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
      Vector2 a(10.0f, 20.0f);
      Vector2 b(11.0f, 20.0f);
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
      Vector2 a(3.0f, 4.0f);
      Vector2 b(1.0f, 0.0f);
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

  // Utils benchmarks
  {
    float angle = 15.0f;
    float angleRad = 1.25f;

    bench.run("deg2rad float", [&] {
      auto result = deg2rad(angle);
      doNotOptimize(result);
    });

    bench.run("rad2deg float", [&] {
      auto result = rad2deg(angleRad);
      doNotOptimize(result);
    });
  }
}
