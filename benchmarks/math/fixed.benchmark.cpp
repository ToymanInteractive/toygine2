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
  \file   fixed.benchmark.cpp
  \brief  Implementation of fixed-point nanobench benchmarks for the math module.
*/

#include "../benchmark_factory.hpp"
#include "math.hpp"

namespace toy::math {

using fixed_type = fixed<int32_t, int64_t, 24>;

// fixed benchmarks
void fixedMathBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  bench.run("fixed = fixed", [] {
    fixed_type a(0);
    fixed_type b(42);
    a = b;
    doNotOptimize(a);
  });
  bench.run("fixed = int", [] {
    fixed_type a(0);
    a = 42;
    doNotOptimize(a);
  });

  bench.run("fixed == fixed", [] {
    fixed_type a(42);
    fixed_type b(42);
    auto r = (a == b);
    doNotOptimize(r);
  });
  bench.run("fixed == int", [] {
    fixed_type f(42);
    int i = 42;
    auto r = (f == i);
    doNotOptimize(r);
  });
  bench.run("int == fixed", [] {
    int i = 42;
    fixed_type f(42);
    auto r = (i == f);
    doNotOptimize(r);
  });

  bench.run("fixed <=> fixed", [] {
    fixed_type a(42);
    fixed_type b(42);
    auto r = (a <=> b);
    doNotOptimize(r);
  });
  bench.run("fixed <=> int", [] {
    fixed_type f(42);
    int i = 42;
    auto r = (f <=> i);
    doNotOptimize(r);
  });
  bench.run("int <=> fixed", [] {
    int i = 42;
    fixed_type f(42);
    auto r = (i <=> f);
    doNotOptimize(r);
  });
}

} // namespace toy::math
