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
  \file   section.benchmark.cpp
  \brief  Implementations for section nanobench benchmarks in the geometry module.
*/

#include "../utils.hpp"
#include "geometry.hpp"

namespace toy::geometry {

using fixed_type = math::fixed<int32_t, int64_t, 24>;

// Section benchmarks
void sectionGeometryBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  bench.run("Section<int> construct min max", [] {
    Section s(10, 20);

    doNotOptimize(s);
  });
  bench.run("Section<float> construct min max", [] {
    Section s(10.0F, 20.0F);

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
    Section s(10.0F, 20.0F);

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
    Section s(10.0F, 20.0F);

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
    Section s(10.0F, 20.0F);

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
    Section s(10.0F, 20.0F);

    s.expand(5.0F);

    doNotOptimize(s);
  });
  bench.run("Section<fixed_type> expand value", [] {
    Section s(fixed_type(10), fixed_type(20));

    s.expand(fixed_type(5));

    doNotOptimize(s);
  });

  bench.run("Section<int> expand section", [] {
    Section other(25, 30);
    Section s(10, 20);

    s.expand(other);

    doNotOptimize(s);
  });
  bench.run("Section<float> expand section", [] {
    Section other(25.0F, 30.0F);
    Section s(10.0F, 20.0F);

    s.expand(other);

    doNotOptimize(s);
  });
  bench.run("Section<fixed_type> expand section", [] {
    Section other(fixed_type(25), fixed_type(30));
    Section s(fixed_type(10), fixed_type(20));

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
    Section s(10.0F, 20.0F);

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
    Section s(10.0F, 20.0F);

    auto r = s.isContains(15.0F);

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
    Section a(10.0F, 20.0F);
    Section b(10.0F, 20.0F);

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
    Section a(10.0F, 20.0F);
    Section b(15.0F, 25.0F);

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

} // namespace toy::geometry
