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

  // Vector2D benchmarks (float and Fixed)
  {
    bench.run("Vector2D<float> coordinate construct", [] {
      Vector2D v(10.0f, 20.0f);
      doNotOptimize(v);
    });
    bench.run("Vector2D<Fixed> coordinate construct", [] {
      Vector2D v(Fixed(10), Fixed(20));
      doNotOptimize(v);
    });

    bench.run("Vector2D<float> operator+=", [] {
      Vector2D v(10.0f, 20.0f);
      v += Vector2D(5.0f, -10.0f);
      doNotOptimize(v);
    });
    bench.run("Vector2D<Fixed> operator+=", [] {
      Vector2D<Fixed> v(Fixed(10), Fixed(20));
      v += Vector2D<Fixed>(Fixed(5), Fixed(-10));
      doNotOptimize(v);
    });

    bench.run("Vector2D<float> operator-=", [] {
      Vector2D v(15.0f, 25.0f);
      v -= Vector2D(5.0f, 10.0f);
      doNotOptimize(v);
    });
    bench.run("Vector2D<Fixed> operator-=", [] {
      Vector2D v(Fixed(15), Fixed(25));
      v -= Vector2D(Fixed(5), Fixed(10));
      doNotOptimize(v);
    });

    bench.run("Vector2D<float> operator*=", [] {
      Vector2D v(10.0f, 20.0f);
      v *= 2.5f;
      doNotOptimize(v);
    });
    bench.run("Vector2D<Fixed> operator*=", [] {
      Vector2D v(Fixed(10), Fixed(20));
      v *= Fixed(2);
      doNotOptimize(v);
    });

    bench.run("Vector2D<float> operator/=", [] {
      Vector2D v(20.0f, 45.0f);
      v /= 2.5f;
      doNotOptimize(v);
    });
    bench.run("Vector2D<Fixed> operator/=", [] {
      Vector2D v(Fixed(20), Fixed(45));
      v /= Fixed(2);
      doNotOptimize(v);
    });

    bench.run("Vector2D<float> sqrMagnitude", [] {
      Vector2D v(3.0f, 4.0f);
      auto r = v.sqrMagnitude();
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> sqrMagnitude", [] {
      Vector2D v(Fixed(3), Fixed(4));
      auto r = v.sqrMagnitude();
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> setZero", [] {
      Vector2D v(100.0f, 200.0f);
      v.setZero();
      doNotOptimize(v);
    });
    bench.run("Vector2D<Fixed> setZero", [] {
      Vector2D v(Fixed(100), Fixed(200));
      v.setZero();
      doNotOptimize(v);
    });

    bench.run("Vector2D<float> isZero", [] {
      Vector2D v(0.0f, 0.0f);
      auto r = v.isZero();
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> isZero", [] {
      Vector2D v(Fixed(0), Fixed(0));
      auto r = v.isZero();
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> isEqual", [] {
      Vector2D a(10.0f, 20.0f);
      Vector2D b(10.0f, 20.0f);
      auto r = a.isEqual(b);
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> isEqual", [] {
      Vector2D a(Fixed(10), Fixed(20));
      Vector2D b(Fixed(10), Fixed(20));
      auto r = a.isEqual(b);
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> c_arr", [] {
      Vector2D v(42.0f, -17.0f);
      auto * arr = v.c_arr();
      doNotOptimize(arr);
    });
    bench.run("Vector2D<Fixed> c_arr", [] {
      Vector2D v(Fixed(42), Fixed(-17));
      auto * arr = v.c_arr();
      doNotOptimize(arr);
    });

    bench.run("Vector2D<float> operator+", [] {
      Vector2D a(10.0f, 20.0f);
      Vector2D b(5.0f, -10.0f);
      auto r = a + b;
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> operator+", [] {
      Vector2D a(Fixed(10), Fixed(20));
      Vector2D b(Fixed(5), Fixed(-10));
      auto r = a + b;
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> operator-", [] {
      Vector2D a(15.0f, 25.0f);
      Vector2D b(5.0f, 10.0f);
      auto r = a - b;
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> operator-", [] {
      Vector2D a(Fixed(15), Fixed(25));
      Vector2D b(Fixed(5), Fixed(10));
      auto r = a - b;
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> operator- unary", [] {
      Vector2D v(10.0f, -20.0f);
      auto r = -v;
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> operator- unary", [] {
      Vector2D v(Fixed(10), Fixed(-20));
      auto r = -v;
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> operator* scalar", [] {
      Vector2D v(10.0f, 20.0f);
      auto r = v * 2.5f;
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> operator* scalar", [] {
      Vector2D v(Fixed(10), Fixed(20));
      auto r = v * Fixed(2);
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> scalar * vector", [] {
      Vector2D v(10.0f, 20.0f);
      auto r = 2.5f * v;
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> scalar * vector", [] {
      Vector2D v(Fixed(10), Fixed(20));
      auto r = Fixed(2) * v;
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> dot product", [] {
      Vector2D a(10.0f, 20.0f);
      Vector2D b(5.0f, 10.0f);
      auto r = a * b;
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> dot product", [] {
      Vector2D a(Fixed(10), Fixed(20));
      Vector2D b(Fixed(5), Fixed(10));
      auto r = a * b;
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> operator/", [] {
      Vector2D v(25.0f, 50.0f);
      auto r = v / 2.5f;
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> operator/", [] {
      Vector2D v(Fixed(25), Fixed(50));
      auto r = v / Fixed(2);
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> operator==", [] {
      Vector2D a(10.0f, 20.0f);
      Vector2D b(10.0f, 20.0f);
      auto r = (a == b);
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> operator==", [] {
      Vector2D a(Fixed(10), Fixed(20));
      Vector2D b(Fixed(10), Fixed(20));
      auto r = (a == b);
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> operator!=", [] {
      Vector2D a(10.0f, 20.0f);
      Vector2D b(11.0f, 20.0f);
      auto r = (a != b);
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> operator!=", [] {
      Vector2D a(Fixed(10), Fixed(20));
      Vector2D b(Fixed(11), Fixed(20));
      auto r = (a != b);
      doNotOptimize(r);
    });

    bench.run("Vector2D<float> cross", [] {
      Vector2D a(3.0f, 4.0f);
      Vector2D b(1.0f, 0.0f);
      auto r = cross(a, b);
      doNotOptimize(r);
    });
    bench.run("Vector2D<Fixed> cross", [] {
      Vector2D a(Fixed(3), Fixed(4));
      Vector2D b(Fixed(1), Fixed(0));
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
