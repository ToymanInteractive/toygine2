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
#include "geometry.hpp"

using namespace toy::geometry;

using Fixed = toy::math::fixed<int32_t, int64_t, 24>;

void runGeometryBenchmarks() noexcept {
  auto bench = createBench("Geometry module");

  // Circle benchmarks
  {
    bench.run("Circle<float> construct center radius", [] {
      Circle c(toy::math::Vector2D(5.0f, 10.0f), 3.0f);
      doNotOptimize(c);
    });
    bench.run("Circle<Fixed> construct center radius", [] {
      Circle c(toy::math::Vector2D(Fixed(5), Fixed(10)), Fixed(3));
      doNotOptimize(c);
    });

    bench.run("Circle<float> area", [] {
      Circle c(toy::math::Vector2D(0.0f, 0.0f), 10.0f);
      auto r = c.area();
      doNotOptimize(r);
    });
    bench.run("Circle<Fixed> area", [] {
      Circle c(toy::math::Vector2D(Fixed(0), Fixed(0)), Fixed(10));
      auto r = c.area();
      doNotOptimize(r);
    });

    bench.run("Circle<float> reset", [] {
      Circle c(toy::math::Vector2D(10.0f, 20.0f), 5.0f);
      c.reset();
      doNotOptimize(c);
    });
    bench.run("Circle<Fixed> reset", [] {
      Circle c(toy::math::Vector2D(Fixed(10), Fixed(20)), Fixed(5));
      c.reset();
      doNotOptimize(c);
    });

    bench.run("Circle<float> isReset", [] {
      Circle<float> c;
      c.reset();
      auto r = c.isReset();
      doNotOptimize(r);
    });
    bench.run("Circle<Fixed> isReset", [] {
      Circle<Fixed> c;
      auto r = c.isReset();
      doNotOptimize(r);
    });

    bench.run("Circle<float> isValid", [] {
      Circle c(toy::math::Vector2D(0.0f, 0.0f), 1.0f);
      auto r = c.isValid();
      doNotOptimize(r);
    });
    bench.run("Circle<Fixed> isValid", [] {
      Circle c(toy::math::Vector2D(Fixed(0), Fixed(0)), Fixed(1));
      auto r = c.isValid();
      doNotOptimize(r);
    });

    bench.run("Circle<float> isContain", [] {
      Circle c(toy::math::Vector2D(0.0f, 0.0f), 10.0f);
      auto r = c.isContain(toy::math::Vector2D(5.0f, 0.0f));
      doNotOptimize(r);
    });
    bench.run("Circle<Fixed> isContain", [] {
      Circle c(toy::math::Vector2D(Fixed(0), Fixed(0)), Fixed(10));
      auto r = c.isContain(toy::math::Vector2D(Fixed(5), Fixed(0)));
      doNotOptimize(r);
    });

    bench.run("Circle<float> operator==", [] {
      Circle a(toy::math::Vector2D(1.0f, 2.0f), 3.0f);
      Circle b(toy::math::Vector2D(1.0f, 2.0f), 3.0f);
      auto r = (a == b);
      doNotOptimize(r);
    });
    bench.run("Circle<Fixed> operator==", [] {
      Circle a(toy::math::Vector2D(Fixed(1), Fixed(2)), Fixed(3));
      Circle b(toy::math::Vector2D(Fixed(1), Fixed(2)), Fixed(3));
      auto r = (a == b);
      doNotOptimize(r);
    });

    bench.run("Circle<float> operator!=", [] {
      Circle a(toy::math::Vector2D(1.0f, 2.0f), 3.0f);
      Circle b(toy::math::Vector2D(1.0f, 2.0f), 4.0f);
      auto r = (a != b);
      doNotOptimize(r);
    });
    bench.run("Circle<Fixed> operator!=", [] {
      Circle a(toy::math::Vector2D(Fixed(1), Fixed(2)), Fixed(3));
      Circle b(toy::math::Vector2D(Fixed(1), Fixed(2)), Fixed(4));
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
    bench.run("Ellipse<Fixed> default construct", [] {
      Ellipse<Fixed> e;
      doNotOptimize(e);
    });

    bench.run("Ellipse<float> construct center radiuses", [] {
      Ellipse e(toy::math::Vector2D(5.0f, 10.0f), toy::math::Vector2D(3.0f, 4.0f));
      doNotOptimize(e);
    });
    bench.run("Ellipse<Fixed> construct center radiuses", [] {
      Ellipse e(toy::math::Vector2D(Fixed(5), Fixed(10)), toy::math::Vector2D(Fixed(3), Fixed(4)));
      doNotOptimize(e);
    });

    bench.run("Ellipse<float> area", [] {
      Ellipse e(toy::math::Vector2D(0.0f, 0.0f), toy::math::Vector2D(10.0f, 5.0f));
      auto r = e.area();
      doNotOptimize(r);
    });
    bench.run("Ellipse<Fixed> area", [] {
      Ellipse e(toy::math::Vector2D(Fixed(0), Fixed(0)), toy::math::Vector2D(Fixed(10), Fixed(5)));
      auto r = e.area();
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> reset", [] {
      Ellipse e(toy::math::Vector2D(10.0f, 20.0f), toy::math::Vector2D(5.0f, 3.0f));
      e.reset();
      doNotOptimize(e);
    });
    bench.run("Ellipse<Fixed> reset", [] {
      Ellipse e(toy::math::Vector2D(Fixed(10), Fixed(20)), toy::math::Vector2D(Fixed(5), Fixed(3)));
      e.reset();
      doNotOptimize(e);
    });

    bench.run("Ellipse<float> isReset", [] {
      Ellipse<float> e;
      e.reset();
      auto r = e.isReset();
      doNotOptimize(r);
    });
    bench.run("Ellipse<Fixed> isReset", [] {
      Ellipse<Fixed> e;
      auto r = e.isReset();
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> isValid", [] {
      Ellipse e(toy::math::Vector2D(0.0f, 0.0f), toy::math::Vector2D(1.0f, 1.0f));
      auto r = e.isValid();
      doNotOptimize(r);
    });
    bench.run("Ellipse<Fixed> isValid", [] {
      Ellipse e(toy::math::Vector2D(Fixed(0), Fixed(0)), toy::math::Vector2D(Fixed(1), Fixed(1)));
      auto r = e.isValid();
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> isContain", [] {
      Ellipse e(toy::math::Vector2D(0.0f, 0.0f), toy::math::Vector2D(10.0f, 5.0f));
      auto r = e.isContain(toy::math::Vector2D(5.0f, 2.0f));
      doNotOptimize(r);
    });
    bench.run("Ellipse<Fixed> isContain", [] {
      Ellipse e(toy::math::Vector2D(Fixed(0), Fixed(0)), toy::math::Vector2D(Fixed(10), Fixed(5)));
      auto r = e.isContain(toy::math::Vector2D(Fixed(5), Fixed(2)));
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> operator==", [] {
      Ellipse a(toy::math::Vector2D(1.0f, 2.0f), toy::math::Vector2D(3.0f, 4.0f));
      Ellipse b(toy::math::Vector2D(1.0f, 2.0f), toy::math::Vector2D(3.0f, 4.0f));
      auto r = (a == b);
      doNotOptimize(r);
    });
    bench.run("Ellipse<Fixed> operator==", [] {
      Ellipse a(toy::math::Vector2D(Fixed(1), Fixed(2)), toy::math::Vector2D(Fixed(3), Fixed(4)));
      Ellipse b(toy::math::Vector2D(Fixed(1), Fixed(2)), toy::math::Vector2D(Fixed(3), Fixed(4)));
      auto r = (a == b);
      doNotOptimize(r);
    });

    bench.run("Ellipse<float> operator!=", [] {
      Ellipse a(toy::math::Vector2D(1.0f, 2.0f), toy::math::Vector2D(3.0f, 4.0f));
      Ellipse b(toy::math::Vector2D(1.0f, 2.0f), toy::math::Vector2D(3.0f, 5.0f));
      auto r = (a != b);
      doNotOptimize(r);
    });
    bench.run("Ellipse<Fixed> operator!=", [] {
      Ellipse a(toy::math::Vector2D(Fixed(1), Fixed(2)), toy::math::Vector2D(Fixed(3), Fixed(4)));
      Ellipse b(toy::math::Vector2D(Fixed(1), Fixed(2)), toy::math::Vector2D(Fixed(3), Fixed(5)));
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
    bench.run("Section<Fixed> construct min max", [] {
      Section s(Fixed(10), Fixed(20));
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
    bench.run("Section<Fixed> midpoint", [] {
      Section s(Fixed(10), Fixed(20));
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
    bench.run("Section<Fixed> length", [] {
      Section s(Fixed(10), Fixed(20));
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
    bench.run("Section<Fixed> reset", [] {
      Section s(Fixed(10), Fixed(20));
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
    bench.run("Section<Fixed> expand value", [] {
      Section s(Fixed(10), Fixed(20));
      s.expand(Fixed(5));
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
    bench.run("Section<Fixed> expand section", [] {
      Section s(Fixed(10), Fixed(20));
      Section other(Fixed(25), Fixed(30));
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
    bench.run("Section<Fixed> isReset", [] {
      Section<Fixed> s;
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
    bench.run("Section<Fixed> isValid", [] {
      Section s(Fixed(10), Fixed(20));
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
    bench.run("Section<Fixed> isContains", [] {
      Section s(Fixed(10), Fixed(20));
      auto r = s.isContains(Fixed(15));
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
    bench.run("Section<Fixed> operator==", [] {
      Section a(Fixed(10), Fixed(20));
      Section b(Fixed(10), Fixed(20));
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
    bench.run("Section<Fixed> operator!=", [] {
      Section a(Fixed(10), Fixed(20));
      Section b(Fixed(15), Fixed(25));
      auto r = (a != b);
      doNotOptimize(r);
    });
  }
}
