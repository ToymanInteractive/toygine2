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
  \file   math_benchmark.cpp
  \brief  Implementation of nanobench benchmarks for the math module.
*/

#include "benchmark_factory.hpp"
#include "math.hpp"

namespace toy::math {

void fixedMathBenchmarks(ankerl::nanobench::Bench &) noexcept;
void pointMathBenchmarks(ankerl::nanobench::Bench &) noexcept;
void utilsMathBenchmarks(ankerl::nanobench::Bench &) noexcept;
void vector2MathBenchmarks(ankerl::nanobench::Bench &) noexcept;

} // namespace toy::math

using namespace toy::math;

using fixed_type = fixed<int32_t, int64_t, 24>;

ankerl::nanobench::Bench runMathBenchmarks() noexcept {
  auto bench = createBench("Math module");

  toy::math::fixedMathBenchmarks(bench);
  toy::math::pointMathBenchmarks(bench);
  toy::math::utilsMathBenchmarks(bench);
  toy::math::vector2MathBenchmarks(bench);

  return bench;
}
