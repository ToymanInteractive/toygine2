//
// Copyright (c) 2026 Toyman Interactive
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
  \file   bench.hpp
  \brief  Picks the table a benchmark body writes to: nanobench on desktop, the built-in table on consoles.

  Makes \ref toy::benchmark::Bench and toy::benchmark::doNotOptimizeAway() name the same calls on every target, so one
  \c BENCHMARK_CASE body builds wherever benchmarks build.

  \note Reached through toy_benchmark.hpp; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_BENCH_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_BENCH_HPP_

#if defined(TOYGINE_BENCHMARKS_USE_NANOBENCH)

#include <nanobench.h>

namespace toy::benchmark {

/// Table a desktop benchmark body writes to.
using Bench = ankerl::nanobench::Bench;

using ankerl::nanobench::doNotOptimizeAway;

} // namespace toy::benchmark

#else // TOYGINE_BENCHMARKS_USE_NANOBENCH

#include "builtin_bench.hpp"

#endif // TOYGINE_BENCHMARKS_USE_NANOBENCH

#endif // INCLUDE_BENCHMARKS_RUNNER_BENCH_HPP_
