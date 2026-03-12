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

  // Point benchmarks
  {
    bench.run("Point default construct", [] {
      Point p;
      doNotOptimize(p);
    });

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

    bench.run("Point sqrMagnitude", [] {
      Point p(3, 4);
      auto r = p.sqrMagnitude();
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
}
