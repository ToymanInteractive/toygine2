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
  \brief  Implementations for circle nanobench benchmarks in the geometry module.
*/

#include "../utils.hpp"
#include "geometry.hpp"

namespace toy::geometry {

using fixed_type = math::fixed<int32_t, int64_t, 24>;

// Circle benchmarks
void circleGeometryBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  bench.run("Circle<float> construct center radius", [] {
    Circle c(math::Vector2(5.0f, 10.0f), 3.0f);

    doNotOptimize(c);
  });
  bench.run("Circle<fixed_type> construct center radius", [] {
    Circle c(math::Vector2(fixed_type(5), fixed_type(10)), fixed_type(3));

    doNotOptimize(c);
  });

  bench.run("Circle<float> area", [] {
    Circle c(math::Vector2(0.0f, 0.0f), 10.0f);

    auto r = c.area();

    doNotOptimize(r);
  });
  bench.run("Circle<fixed_type> area", [] {
    Circle c(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(10));

    auto r = c.area();

    doNotOptimize(r);
  });

  bench.run("Circle<float> reset", [] {
    Circle c(math::Vector2(10.0f, 20.0f), 5.0f);

    c.reset();

    doNotOptimize(c);
  });
  bench.run("Circle<fixed_type> reset", [] {
    Circle c(math::Vector2(fixed_type(10), fixed_type(20)), fixed_type(5));

    c.reset();

    doNotOptimize(c);
  });

  bench.run("Circle<float> isReset", [] {
    Circle<float> c;
    c.reset();

    auto r = c.isReset();

    doNotOptimize(r);
  });
  bench.run("Circle<fixed_type> isReset", [] {
    Circle<fixed_type> c;
    c.reset();

    auto r = c.isReset();

    doNotOptimize(r);
  });

  bench.run("Circle<float> isValid", [] {
    Circle c(math::Vector2(0.0f, 0.0f), 1.0f);

    auto r = c.isValid();

    doNotOptimize(r);
  });
  bench.run("Circle<fixed_type> isValid", [] {
    Circle c(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(1));

    auto r = c.isValid();

    doNotOptimize(r);
  });

  bench.run("Circle<float> isContain", [] {
    Circle c(math::Vector2(0.0f, 0.0f), 10.0f);

    auto r = c.isContain(math::Vector2(5.0f, 0.0f));

    doNotOptimize(r);
  });
  bench.run("Circle<fixed_type> isContain", [] {
    Circle c(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(10));

    auto r = c.isContain(math::Vector2(fixed_type(5), fixed_type(0)));

    doNotOptimize(r);
  });

  bench.run("Circle<float> operator==", [] {
    Circle a(math::Vector2(1.0f, 2.0f), 3.0f);
    Circle b(math::Vector2(1.0f, 2.0f), 3.0f);

    auto r = (a == b);

    doNotOptimize(r);
  });
  bench.run("Circle<fixed_type> operator==", [] {
    Circle a(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
    Circle b(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));

    auto r = (a == b);

    doNotOptimize(r);
  });

  bench.run("Circle<float> operator!=", [] {
    Circle a(math::Vector2(1.0f, 2.0f), 3.0f);
    Circle b(math::Vector2(1.0f, 2.0f), 4.0f);

    auto r = (a != b);

    doNotOptimize(r);
  });
  bench.run("Circle<fixed_type> operator!=", [] {
    Circle a(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
    Circle b(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(4));

    auto r = (a != b);

    doNotOptimize(r);
  });
}

} // namespace toy::geometry
