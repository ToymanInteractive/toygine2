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
  \brief  Nanobench benchmarks for the math module (fixed, Point, Vector2, etc.).
*/

#include "benchmark_factory.hpp"
#include "math.hpp"

using namespace toy::math;

using Fixed = fixed<int32_t, int64_t, 24>;

void runMathBenchmarks() noexcept {
  auto bench = createBench("Math module");

  // fixed benchmarks
  {
    bench.run("fixed = fixed", [] {
      Fixed a(0);
      Fixed b(42);
      a = b;
      doNotOptimize(a);
    });
    bench.run("fixed = int", [] {
      Fixed a(0);
      a = 42;
      doNotOptimize(a);
    });

    bench.run("fixed == fixed", [] {
      Fixed a(42);
      Fixed b(42);
      auto r = (a == b);
      doNotOptimize(r);
    });
    bench.run("fixed == int", [] {
      Fixed f(42);
      int i = 42;
      auto r = (f == i);
      doNotOptimize(r);
    });
    bench.run("int == fixed", [] {
      int i = 42;
      Fixed f(42);
      auto r = (i == f);
      doNotOptimize(r);
    });

    bench.run("fixed <=> fixed", [] {
      Fixed a(42);
      Fixed b(42);
      auto r = (a <=> b);
      doNotOptimize(r);
    });
    bench.run("fixed <=> int", [] {
      Fixed f(42);
      int i = 42;
      auto r = (f <=> i);
      doNotOptimize(r);
    });
    bench.run("int <=> fixed", [] {
      int i = 42;
      Fixed f(42);
      auto r = (i <=> f);
      doNotOptimize(r);
    });
  }

  // Point benchmarks
  {
    bench.run("Point coordinate construct", [] {
      Point p(10, 20);
      doNotOptimize(p);
    });

    bench.run("Point operator+=", [] {
      Point p(10, 20);
      p += Point(5, -3);
      doNotOptimize(p);
    });

    bench.run("Point operator-=", [] {
      Point p(15, 25);
      p -= Point(5, 10);
      doNotOptimize(p);
    });

    bench.run("Point operator*=", [] {
      Point p(10, 20);
      p *= 2;
      doNotOptimize(p);
    });

    bench.run("Point operator*= float", [] {
      Point p(10, 20);
      p *= 2.5f;
      doNotOptimize(p);
    });

    bench.run("Point operator*= Fixed", [] {
      Point p(10, 20);
      p *= Fixed(2);
      doNotOptimize(p);
    });

    bench.run("Point operator/=", [] {
      Point p(30, 60);
      p /= 3;
      doNotOptimize(p);
    });

    bench.run("Point operator/= float", [] {
      Point p(25, 50);
      p /= 2.5f;
      doNotOptimize(p);
    });

    bench.run("Point operator/= Fixed", [] {
      Point p(30, 60);
      p /= Fixed(3);
      doNotOptimize(p);
    });

    bench.run("Point operator+", [] {
      Point a(10, 20);
      Point b(5, -10);
      auto r = a + b;
      doNotOptimize(r);
    });

    bench.run("Point operator-", [] {
      Point a(15, 25);
      Point b(5, 10);
      auto r = a - b;
      doNotOptimize(r);
    });

    bench.run("Point operator- unary", [] {
      Point p(10, -20);
      auto r = -p;
      doNotOptimize(r);
    });

    bench.run("Point operator* scalar", [] {
      Point p(10, 20);
      auto r = p * 3;
      doNotOptimize(r);
    });

    bench.run("Point operator* float", [] {
      Point p(10, 20);
      auto r = p * 2.5f;
      doNotOptimize(r);
    });

    bench.run("Point operator* Fixed", [] {
      Point p(10, 20);
      auto r = p * Fixed(2);
      doNotOptimize(r);
    });

    bench.run("Point Fixed * point", [] {
      Point p(5, 10);
      auto r = Fixed(3) * p;
      doNotOptimize(r);
    });

    bench.run("Point operator/ scalar", [] {
      Point p(30, 60);
      auto r = p / 3;
      doNotOptimize(r);
    });

    bench.run("Point operator/ float", [] {
      Point p(25, 50);
      auto r = p / 2.5f;
      doNotOptimize(r);
    });

    bench.run("Point operator/ Fixed", [] {
      Point p(30, 60);
      auto r = p / Fixed(3);
      doNotOptimize(r);
    });

    bench.run("Point isZero", [] {
      Point p(0, 0);
      auto r = p.isZero();
      doNotOptimize(r);
    });

    bench.run("Point isEqual", [] {
      Point a(10, 20);
      Point b(10, 20);
      auto r = a.isEqual(b);
      doNotOptimize(r);
    });

    bench.run("Point operator==", [] {
      Point a(10, 20);
      Point b(10, 20);
      auto r = (a == b);
      doNotOptimize(r);
    });

    bench.run("Point c_arr", [] {
      Point p(42, -17);
      auto * arr = p.c_arr();
      doNotOptimize(arr);
    });
  }

  // Vector2 benchmarks (float and Fixed)
  {
    bench.run("Vector2<float> coordinate construct", [] {
      Vector2 v(10.0f, 20.0f);
      doNotOptimize(v);
    });
    bench.run("Vector2<Fixed> coordinate construct", [] {
      Vector2 v(Fixed(10), Fixed(20));
      doNotOptimize(v);
    });

    bench.run("Vector2<float> operator+=", [] {
      Vector2 v(10.0f, 20.0f);
      v += Vector2(5.0f, -10.0f);
      doNotOptimize(v);
    });
    bench.run("Vector2<Fixed> operator+=", [] {
      Vector2<Fixed> v(Fixed(10), Fixed(20));
      v += Vector2<Fixed>(Fixed(5), Fixed(-10));
      doNotOptimize(v);
    });

    bench.run("Vector2<float> operator-=", [] {
      Vector2 v(15.0f, 25.0f);
      v -= Vector2(5.0f, 10.0f);
      doNotOptimize(v);
    });
    bench.run("Vector2<Fixed> operator-=", [] {
      Vector2 v(Fixed(15), Fixed(25));
      v -= Vector2(Fixed(5), Fixed(10));
      doNotOptimize(v);
    });

    bench.run("Vector2<float> operator*=", [] {
      Vector2 v(10.0f, 20.0f);
      v *= 2.5f;
      doNotOptimize(v);
    });
    bench.run("Vector2<Fixed> operator*=", [] {
      Vector2 v(Fixed(10), Fixed(20));
      v *= Fixed(2);
      doNotOptimize(v);
    });

    bench.run("Vector2<float> operator/=", [] {
      Vector2 v(20.0f, 45.0f);
      v /= 2.5f;
      doNotOptimize(v);
    });
    bench.run("Vector2<Fixed> operator/=", [] {
      Vector2 v(Fixed(20), Fixed(45));
      v /= Fixed(2);
      doNotOptimize(v);
    });

    bench.run("Vector2<float> sqrMagnitude", [] {
      Vector2 v(3.0f, 4.0f);
      auto r = v.sqrMagnitude();
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> sqrMagnitude", [] {
      Vector2 v(Fixed(3), Fixed(4));
      auto r = v.sqrMagnitude();
      doNotOptimize(r);
    });

    bench.run("Vector2<float> setZero", [] {
      Vector2 v(100.0f, 200.0f);
      v.setZero();
      doNotOptimize(v);
    });
    bench.run("Vector2<Fixed> setZero", [] {
      Vector2 v(Fixed(100), Fixed(200));
      v.setZero();
      doNotOptimize(v);
    });

    bench.run("Vector2<float> isZero", [] {
      Vector2 v(0.0f, 0.0f);
      auto r = v.isZero();
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> isZero", [] {
      Vector2 v(Fixed(0), Fixed(0));
      auto r = v.isZero();
      doNotOptimize(r);
    });

    bench.run("Vector2<float> isEqual", [] {
      Vector2 a(10.0f, 20.0f);
      Vector2 b(10.0f, 20.0f);
      auto r = a.isEqual(b);
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> isEqual", [] {
      Vector2 a(Fixed(10), Fixed(20));
      Vector2 b(Fixed(10), Fixed(20));
      auto r = a.isEqual(b);
      doNotOptimize(r);
    });

    bench.run("Vector2<float> c_arr", [] {
      Vector2 v(42.0f, -17.0f);
      auto * arr = v.c_arr();
      doNotOptimize(arr);
    });
    bench.run("Vector2<Fixed> c_arr", [] {
      Vector2 v(Fixed(42), Fixed(-17));
      auto * arr = v.c_arr();
      doNotOptimize(arr);
    });

    bench.run("Vector2<float> operator+", [] {
      Vector2 a(10.0f, 20.0f);
      Vector2 b(5.0f, -10.0f);
      auto r = a + b;
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> operator+", [] {
      Vector2 a(Fixed(10), Fixed(20));
      Vector2 b(Fixed(5), Fixed(-10));
      auto r = a + b;
      doNotOptimize(r);
    });

    bench.run("Vector2<float> operator-", [] {
      Vector2 a(15.0f, 25.0f);
      Vector2 b(5.0f, 10.0f);
      auto r = a - b;
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> operator-", [] {
      Vector2 a(Fixed(15), Fixed(25));
      Vector2 b(Fixed(5), Fixed(10));
      auto r = a - b;
      doNotOptimize(r);
    });

    bench.run("Vector2<float> operator- unary", [] {
      Vector2 v(10.0f, -20.0f);
      auto r = -v;
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> operator- unary", [] {
      Vector2 v(Fixed(10), Fixed(-20));
      auto r = -v;
      doNotOptimize(r);
    });

    bench.run("Vector2<float> operator* scalar", [] {
      Vector2 v(10.0f, 20.0f);
      auto r = v * 2.5f;
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> operator* scalar", [] {
      Vector2 v(Fixed(10), Fixed(20));
      auto r = v * Fixed(2);
      doNotOptimize(r);
    });

    bench.run("Vector2<float> scalar * vector", [] {
      Vector2 v(10.0f, 20.0f);
      auto r = 2.5f * v;
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> scalar * vector", [] {
      Vector2 v(Fixed(10), Fixed(20));
      auto r = Fixed(2) * v;
      doNotOptimize(r);
    });

    bench.run("Vector2<float> dot product", [] {
      Vector2 a(10.0f, 20.0f);
      Vector2 b(5.0f, 10.0f);
      auto r = a * b;
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> dot product", [] {
      Vector2 a(Fixed(10), Fixed(20));
      Vector2 b(Fixed(5), Fixed(10));
      auto r = a * b;
      doNotOptimize(r);
    });

    bench.run("Vector2<float> operator/", [] {
      Vector2 v(25.0f, 50.0f);
      auto r = v / 2.5f;
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> operator/", [] {
      Vector2 v(Fixed(25), Fixed(50));
      auto r = v / Fixed(2);
      doNotOptimize(r);
    });

    bench.run("Vector2<float> operator==", [] {
      Vector2 a(10.0f, 20.0f);
      Vector2 b(10.0f, 20.0f);
      auto r = (a == b);
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> operator==", [] {
      Vector2 a(Fixed(10), Fixed(20));
      Vector2 b(Fixed(10), Fixed(20));
      auto r = (a == b);
      doNotOptimize(r);
    });

    bench.run("Vector2<float> operator!=", [] {
      Vector2 a(10.0f, 20.0f);
      Vector2 b(11.0f, 20.0f);
      auto r = (a != b);
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> operator!=", [] {
      Vector2 a(Fixed(10), Fixed(20));
      Vector2 b(Fixed(11), Fixed(20));
      auto r = (a != b);
      doNotOptimize(r);
    });

    bench.run("Vector2<float> cross", [] {
      Vector2 a(3.0f, 4.0f);
      Vector2 b(1.0f, 0.0f);
      auto r = cross(a, b);
      doNotOptimize(r);
    });
    bench.run("Vector2<Fixed> cross", [] {
      Vector2 a(Fixed(3), Fixed(4));
      Vector2 b(Fixed(1), Fixed(0));
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
