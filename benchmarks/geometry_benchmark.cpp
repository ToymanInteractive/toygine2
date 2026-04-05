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
  \file   geometry_benchmark.cpp
  \brief  Implementation of nanobench benchmarks for the geometry module.
*/

#include "benchmark_factory.hpp"
#include "geometry.hpp"

using namespace toy::geometry;

using fixed_type = toy::math::fixed<int32_t, int64_t, 24>;

ankerl::nanobench::Bench runGeometryBenchmarks() noexcept {
  auto bench = createBench("Geometry module");

  // Circle benchmarks
  {
    bench.run("Circle<float> construct center radius", [] {
      Circle c(toy::math::Vector2(5.0f, 10.0f), 3.0f);
      doNotOptimize(c);
    });
    bench.run("Circle<fixed_type> construct center radius", [] {
      Circle c(toy::math::Vector2(fixed_type(5), fixed_type(10)), fixed_type(3));
      doNotOptimize(c);
    });

    bench.run("Circle<float> area", [] {
      Circle c(toy::math::Vector2(0.0f, 0.0f), 10.0f);
      auto r = c.area();
      doNotOptimize(r);
    });
    bench.run("Circle<fixed_type> area", [] {
      Circle c(toy::math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(10));
      auto r = c.area();
      doNotOptimize(r);
    });

    bench.run("Circle<float> reset", [] {
      Circle c(toy::math::Vector2(10.0f, 20.0f), 5.0f);
      c.reset();
      doNotOptimize(c);
    });
    bench.run("Circle<fixed_type> reset", [] {
      Circle c(toy::math::Vector2(fixed_type(10), fixed_type(20)), fixed_type(5));
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
      auto r = c.isReset();
      doNotOptimize(r);
    });

    bench.run("Circle<float> isValid", [] {
      Circle c(toy::math::Vector2(0.0f, 0.0f), 1.0f);
      auto r = c.isValid();
      doNotOptimize(r);
    });
    bench.run("Circle<fixed_type> isValid", [] {
      Circle c(toy::math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(1));
      auto r = c.isValid();
      doNotOptimize(r);
    });

    bench.run("Circle<float> isContain", [] {
      Circle c(toy::math::Vector2(0.0f, 0.0f), 10.0f);
      auto r = c.isContain(toy::math::Vector2(5.0f, 0.0f));
      doNotOptimize(r);
    });
    bench.run("Circle<fixed_type> isContain", [] {
      Circle c(toy::math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(10));
      auto r = c.isContain(toy::math::Vector2(fixed_type(5), fixed_type(0)));
      doNotOptimize(r);
    });

    bench.run("Circle<float> operator==", [] {
      Circle a(toy::math::Vector2(1.0f, 2.0f), 3.0f);
      Circle b(toy::math::Vector2(1.0f, 2.0f), 3.0f);
      auto r = (a == b);
      doNotOptimize(r);
    });
    bench.run("Circle<fixed_type> operator==", [] {
      Circle a(toy::math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
      Circle b(toy::math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
      auto r = (a == b);
      doNotOptimize(r);
    });

    bench.run("Circle<float> operator!=", [] {
      Circle a(toy::math::Vector2(1.0f, 2.0f), 3.0f);
      Circle b(toy::math::Vector2(1.0f, 2.0f), 4.0f);
      auto r = (a != b);
      doNotOptimize(r);
    });
    bench.run("Circle<fixed_type> operator!=", [] {
      Circle a(toy::math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
      Circle b(toy::math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(4));
      auto r = (a != b);
      doNotOptimize(r);
    });
  }

  // Ellipse benchmarks
  {
    bench.run("Ellipse<float> default construct", [] {
      Ellipse<float> e;
      doNotOptimize(e);
    });
    bench.run("Ellipse<fixed_type> default construct", [] {
      Ellipse<fixed_type> e;
      doNotOptimize(e);
    });

    bench.run("Ellipse<float> construct center radiuses", [] {
      Ellipse e(toy::math::Vector2(5.0f, 10.0f), toy::math::Vector2(3.0f, 4.0f));
      doNotOptimize(e);
    });
    bench.run("Ellipse<fixed_type> construct center radiuses", [] {
      Ellipse e(toy::math::Vector2(fixed_type(5), fixed_type(10)), toy::math::Vector2(fixed_type(3), fixed_type(4)));
      doNotOptimize(e);
    });

    bench.run("Ellipse<float> area", [] {
      Ellipse e(toy::math::Vector2(0.0f, 0.0f), toy::math::Vector2(10.0f, 5.0f));
      auto r = e.area();
      doNotOptimize(r);
    });
    bench.run("Ellipse<fixed_type> area", [] {
      Ellipse e(toy::math::Vector2(fixed_type(0), fixed_type(0)), toy::math::Vector2(fixed_type(10), fixed_type(5)));
      auto r = e.area();
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> reset", [] {
      Ellipse e(toy::math::Vector2(10.0f, 20.0f), toy::math::Vector2(5.0f, 3.0f));
      e.reset();
      doNotOptimize(e);
    });
    bench.run("Ellipse<fixed_type> reset", [] {
      Ellipse e(toy::math::Vector2(fixed_type(10), fixed_type(20)), toy::math::Vector2(fixed_type(5), fixed_type(3)));
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
      auto r = e.isReset();
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> isValid", [] {
      Ellipse e(toy::math::Vector2(0.0f, 0.0f), toy::math::Vector2(1.0f, 1.0f));
      auto r = e.isValid();
      doNotOptimize(r);
    });
    bench.run("Ellipse<fixed_type> isValid", [] {
      Ellipse e(toy::math::Vector2(fixed_type(0), fixed_type(0)), toy::math::Vector2(fixed_type(1), fixed_type(1)));
      auto r = e.isValid();
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> isContain", [] {
      Ellipse e(toy::math::Vector2(0.0f, 0.0f), toy::math::Vector2(10.0f, 5.0f));
      auto r = e.isContain(toy::math::Vector2(5.0f, 2.0f));
      doNotOptimize(r);
    });
    bench.run("Ellipse<fixed_type> isContain", [] {
      Ellipse e(toy::math::Vector2(fixed_type(0), fixed_type(0)), toy::math::Vector2(fixed_type(10), fixed_type(5)));
      auto r = e.isContain(toy::math::Vector2(fixed_type(5), fixed_type(2)));
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> operator==", [] {
      Ellipse a(toy::math::Vector2(1.0f, 2.0f), toy::math::Vector2(3.0f, 4.0f));
      Ellipse b(toy::math::Vector2(1.0f, 2.0f), toy::math::Vector2(3.0f, 4.0f));
      auto r = (a == b);
      doNotOptimize(r);
    });
    bench.run("Ellipse<fixed_type> operator==", [] {
      Ellipse a(toy::math::Vector2(fixed_type(1), fixed_type(2)), toy::math::Vector2(fixed_type(3), fixed_type(4)));
      Ellipse b(toy::math::Vector2(fixed_type(1), fixed_type(2)), toy::math::Vector2(fixed_type(3), fixed_type(4)));
      auto r = (a == b);
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> operator!=", [] {
      Ellipse a(toy::math::Vector2(1.0f, 2.0f), toy::math::Vector2(3.0f, 4.0f));
      Ellipse b(toy::math::Vector2(1.0f, 2.0f), toy::math::Vector2(3.0f, 5.0f));
      auto r = (a != b);
      doNotOptimize(r);
    });
    bench.run("Ellipse<fixed_type> operator!=", [] {
      Ellipse a(toy::math::Vector2(fixed_type(1), fixed_type(2)), toy::math::Vector2(fixed_type(3), fixed_type(4)));
      Ellipse b(toy::math::Vector2(fixed_type(1), fixed_type(2)), toy::math::Vector2(fixed_type(3), fixed_type(5)));
      auto r = (a != b);
      doNotOptimize(r);
    });
  }

  // Section benchmarks
  {
    bench.run("Section<int> construct min max", [] {
      Section s(10, 20);
      doNotOptimize(s);
    });
    bench.run("Section<float> construct min max", [] {
      Section s(10.0f, 20.0f);
      doNotOptimize(s);
    });
    bench.run("Section<fixed_type> construct min max", [] {
      Section s(fixed_type(10), fixed_type(20));
      doNotOptimize(s);
    });

    bench.run("Section<int> midpoint", [] {
      Section s(10, 20);
      auto r = s.midpoint();
      doNotOptimize(r);
    });
    bench.run("Section<float> midpoint", [] {
      Section s(10.0f, 20.0f);
      auto r = s.midpoint();
      doNotOptimize(r);
    });
    bench.run("Section<fixed_type> midpoint", [] {
      Section s(fixed_type(10), fixed_type(20));
      auto r = s.midpoint();
      doNotOptimize(r);
    });

    bench.run("Section<int> length", [] {
      Section s(10, 20);
      auto r = s.length();
      doNotOptimize(r);
    });
    bench.run("Section<float> length", [] {
      Section s(10.0f, 20.0f);
      auto r = s.length();
      doNotOptimize(r);
    });
    bench.run("Section<fixed_type> length", [] {
      Section s(fixed_type(10), fixed_type(20));
      auto r = s.length();
      doNotOptimize(r);
    });

    bench.run("Section<int> reset", [] {
      Section s(10, 20);
      s.reset();
      doNotOptimize(s);
    });
    bench.run("Section<float> reset", [] {
      Section s(10.0f, 20.0f);
      s.reset();
      doNotOptimize(s);
    });
    bench.run("Section<fixed_type> reset", [] {
      Section s(fixed_type(10), fixed_type(20));
      s.reset();
      doNotOptimize(s);
    });

    bench.run("Section<int> expand value", [] {
      Section s(10, 20);
      s.expand(5);
      doNotOptimize(s);
    });
    bench.run("Section<float> expand value", [] {
      Section s(10.0f, 20.0f);
      s.expand(5.0f);
      doNotOptimize(s);
    });
    bench.run("Section<fixed_type> expand value", [] {
      Section s(fixed_type(10), fixed_type(20));
      s.expand(fixed_type(5));
      doNotOptimize(s);
    });

    bench.run("Section<int> expand section", [] {
      Section s(10, 20);
      Section other(25, 30);
      s.expand(other);
      doNotOptimize(s);
    });
    bench.run("Section<float> expand section", [] {
      Section s(10.0f, 20.0f);
      Section other(25.0f, 30.0f);
      s.expand(other);
      doNotOptimize(s);
    });
    bench.run("Section<fixed_type> expand section", [] {
      Section s(fixed_type(10), fixed_type(20));
      Section other(fixed_type(25), fixed_type(30));
      s.expand(other);
      doNotOptimize(s);
    });

    bench.run("Section<int> isReset", [] {
      Section<int> s;
      auto r = s.isReset();
      doNotOptimize(r);
    });
    bench.run("Section<float> isReset", [] {
      Section<float> s;
      auto r = s.isReset();
      doNotOptimize(r);
    });
    bench.run("Section<fixed_type> isReset", [] {
      Section<fixed_type> s;
      auto r = s.isReset();
      doNotOptimize(r);
    });

    bench.run("Section<int> isValid", [] {
      Section s(10, 20);
      auto r = s.isValid();
      doNotOptimize(r);
    });
    bench.run("Section<float> isValid", [] {
      Section s(10.0f, 20.0f);
      auto r = s.isValid();
      doNotOptimize(r);
    });
    bench.run("Section<fixed_type> isValid", [] {
      Section s(fixed_type(10), fixed_type(20));
      auto r = s.isValid();
      doNotOptimize(r);
    });

    bench.run("Section<int> isContains", [] {
      Section s(10, 20);
      auto r = s.isContains(15);
      doNotOptimize(r);
    });
    bench.run("Section<float> isContains", [] {
      Section s(10.0f, 20.0f);
      auto r = s.isContains(15.0f);
      doNotOptimize(r);
    });
    bench.run("Section<fixed_type> isContains", [] {
      Section s(fixed_type(10), fixed_type(20));
      auto r = s.isContains(fixed_type(15));
      doNotOptimize(r);
    });

    bench.run("Section<int> operator==", [] {
      Section a(10, 20);
      Section b(10, 20);
      auto r = (a == b);
      doNotOptimize(r);
    });
    bench.run("Section<float> operator==", [] {
      Section a(10.0f, 20.0f);
      Section b(10.0f, 20.0f);
      auto r = (a == b);
      doNotOptimize(r);
    });
    bench.run("Section<fixed_type> operator==", [] {
      Section a(fixed_type(10), fixed_type(20));
      Section b(fixed_type(10), fixed_type(20));
      auto r = (a == b);
      doNotOptimize(r);
    });

    bench.run("Section<int> operator!=", [] {
      Section a(10, 20);
      Section b(15, 25);
      auto r = (a != b);
      doNotOptimize(r);
    });
    bench.run("Section<float> operator!=", [] {
      Section a(10.0f, 20.0f);
      Section b(15.0f, 25.0f);
      auto r = (a != b);
      doNotOptimize(r);
    });
    bench.run("Section<fixed_type> operator!=", [] {
      Section a(fixed_type(10), fixed_type(20));
      Section b(fixed_type(15), fixed_type(25));
      auto r = (a != b);
      doNotOptimize(r);
    });
  }

  return bench;
}
