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
  \file   benchmark_factory.hpp
  \brief  Factory for a preconfigured \c ankerl::nanobench::Bench instance.

  Used by benchmark translation units under \c benchmarks/ to share warmup, epoch, and reporting defaults.
*/

#ifndef BENCHMARKS_BENCHMARK_FACTORY_HPP_
#define BENCHMARKS_BENCHMARK_FACTORY_HPP_

#include <nanobench.h>

/*!
  \brief Creates an ankerl::nanobench::Bench configured with default warmup, epochs, and relative results.

  \param title Benchmark title shown in the report.

  \return A Bench instance with warmup(100), epochs(100), minEpochIterations(1000), relative(true).
*/
inline ankerl::nanobench::Bench createBench(const char * title) noexcept {
  return ankerl::nanobench::Bench().title(title).warmup(100).epochs(100).minEpochIterations(1000).relative(true);
}

/*!
  \brief Prevents the compiler from optimizing away \a value in benchmarks.

  Forwards to ankerl::nanobench::doNotOptimizeAway so that computed results are not removed as dead code.

  \tparam T Deduced type of the value (forwarding reference).

  \param value Result or variable that must appear used to the compiler.
*/
template <typename T>
inline void doNotOptimize(T && value) {
  ankerl::nanobench::doNotOptimizeAway(std::forward<T>(value));
}

#endif // BENCHMARKS_BENCHMARK_FACTORY_HPP_
