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
  \file   toy_benchmark.hpp
  \brief  Benchmark macros and registry declarations shared by every benchmark translation unit.

  Defines the benchmark registry \ref toy::benchmark::detail::caseListHead and the \c BENCHMARK_CASE macro, and pulls
  in the registration node they rest on. A benchmark file needs no other include: nanobench arrives through this
  header.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_TOY_BENCHMARK_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_TOY_BENCHMARK_HPP_

#include "case_registrar.hpp"

namespace toy::benchmark::detail {

/*!
  \brief Head of the registry every \c BENCHMARK_CASE registers into.

  Initialized with a constant, so it is set before any dynamic initialization runs and no registrar can observe it
  uninitialized.
*/
inline CaseRegistrar * caseListHead = nullptr;

} // namespace toy::benchmark::detail

// Two levels of indirection are what makes __COUNTER__ expand before it is pasted.
#define TOY_BENCHMARK_CONCAT_INNER(lhs, rhs) lhs##rhs
#define TOY_BENCHMARK_CONCAT(lhs, rhs)       TOY_BENCHMARK_CONCAT_INNER(lhs, rhs)
#define TOY_BENCHMARK_UNIQUE(prefix)         TOY_BENCHMARK_CONCAT(prefix, __COUNTER__)

// The registrar is not const: a later registration writes the link of an earlier node, see
// toy::benchmark::CaseRegistrar.
#define TOY_BENCHMARK_CASE_IMPL(caseName, bodyName, registrarName)                                                     \
  static void                            bodyName(::ankerl::nanobench::Bench & bench);                                 \
  static ::toy::benchmark::CaseRegistrar registrarName{::toy::benchmark::detail::caseListHead, caseName, __FILE__,     \
                                                       __LINE__, &bodyName};                                           \
  static void                            bodyName(::ankerl::nanobench::Bench & bench)

/// Declares and registers a benchmark case; see \ref toy::benchmark::CaseRegistrar.
#define BENCHMARK_CASE(caseName)                                                                                       \
  TOY_BENCHMARK_CASE_IMPL(caseName, TOY_BENCHMARK_UNIQUE(toyBenchmarkBody), TOY_BENCHMARK_UNIQUE(toyBenchmarkReg))

#endif // INCLUDE_BENCHMARKS_RUNNER_TOY_BENCHMARK_HPP_
