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
  \brief  Implementations for ellipse nanobench benchmarks in the geometry module.
*/

#include "../utils.hpp"
#include "geometry.hpp"

namespace toy::geometry {

using fixed_type = math::fixed<int32_t, int64_t, 24>;

// Ellipse benchmarks
void ellipseGeometryBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  bench.run("Ellipse<float> construct center radiuses", [] {
    Ellipse e(math::Vector2(5.0F, 10.0F), math::Vector2(3.0F, 4.0F));

    doNotOptimize(e);
  });
  bench.run("Ellipse<fixed_type> construct center radiuses", [] {
    Ellipse e(math::Vector2(fixed_type(5), fixed_type(10)), math::Vector2(fixed_type(3), fixed_type(4)));

    doNotOptimize(e);
  });

  bench.run("Ellipse<float> area", [] {
    constexpr Ellipse e(math::Vector2(0.0F, 0.0F), math::Vector2(10.0F, 5.0F));

    auto r = e.area();

    doNotOptimize(r);
  });
  bench.run("Ellipse<fixed_type> area", [] {
    constexpr Ellipse e(math::Vector2(fixed_type(0), fixed_type(0)), math::Vector2(fixed_type(10), fixed_type(5)));

    auto r = e.area();

    doNotOptimize(r);
  });

  bench.run("Ellipse<float> reset", [] {
    Ellipse e(math::Vector2(10.0F, 20.0F), math::Vector2(5.0F, 3.0F));

    e.reset();

    doNotOptimize(e);
  });
  bench.run("Ellipse<fixed_type> reset", [] {
    Ellipse e(math::Vector2(fixed_type(10), fixed_type(20)), math::Vector2(fixed_type(5), fixed_type(3)));

    e.reset();

    doNotOptimize(e);
  });

  bench.run("Ellipse<float> isReset", [] {
    Ellipse<float> e;
    e.reset();

    auto r = e.isReset();

    doNotOptimize(r);
  });
  bench.run("Ellipse<fixed_type> isReset", [] {
    Ellipse<fixed_type> e;
    e.reset();

    auto r = e.isReset();

    doNotOptimize(r);
  });

  bench.run("Ellipse<float> isValid", [] {
    constexpr Ellipse e(math::Vector2(0.0F, 0.0F), math::Vector2(1.0F, 1.0F));

    auto r = e.isValid();

    doNotOptimize(r);
  });
  bench.run("Ellipse<fixed_type> isValid", [] {
    constexpr Ellipse e(math::Vector2(fixed_type(0), fixed_type(0)), math::Vector2(fixed_type(1), fixed_type(1)));

    auto r = e.isValid();

    doNotOptimize(r);
  });

  bench.run("Ellipse<float> isContain", [] {
    constexpr Ellipse e(math::Vector2(0.0F, 0.0F), math::Vector2(10.0F, 5.0F));

    auto r = e.isContain(math::Vector2(5.0F, 2.0F));

    doNotOptimize(r);
  });
  bench.run("Ellipse<fixed_type> isContain", [] {
    constexpr Ellipse e(math::Vector2(fixed_type(0), fixed_type(0)), math::Vector2(fixed_type(10), fixed_type(5)));

    auto r = e.isContain(math::Vector2(fixed_type(5), fixed_type(2)));

    doNotOptimize(r);
  });

  bench.run("Ellipse<float> operator==", [] {
    constexpr Ellipse a(math::Vector2(1.0F, 2.0F), math::Vector2(3.0F, 4.0F));
    constexpr Ellipse b(math::Vector2(1.0F, 2.0F), math::Vector2(3.0F, 4.0F));

    auto r = (a == b);

    doNotOptimize(r);
  });
  bench.run("Ellipse<fixed_type> operator==", [] {
    constexpr Ellipse a(math::Vector2(fixed_type(1), fixed_type(2)), math::Vector2(fixed_type(3), fixed_type(4)));
    constexpr Ellipse b(math::Vector2(fixed_type(1), fixed_type(2)), math::Vector2(fixed_type(3), fixed_type(4)));

    auto r = (a == b);

    doNotOptimize(r);
  });

  bench.run("Ellipse<float> operator!=", [] {
    constexpr Ellipse a(math::Vector2(1.0F, 2.0F), math::Vector2(3.0F, 4.0F));
    constexpr Ellipse b(math::Vector2(1.0F, 2.0F), math::Vector2(3.0F, 5.0F));

    auto r = (a != b);

    doNotOptimize(r);
  });
  bench.run("Ellipse<fixed_type> operator!=", [] {
    constexpr Ellipse a(math::Vector2(fixed_type(1), fixed_type(2)), math::Vector2(fixed_type(3), fixed_type(4)));
    constexpr Ellipse b(math::Vector2(fixed_type(1), fixed_type(2)), math::Vector2(fixed_type(3), fixed_type(5)));

    auto r = (a != b);

    doNotOptimize(r);
  });
}

} // namespace toy::geometry
