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
  \file   utils.benchmark.cpp
  \brief  Nanobench benchmarks for \c toy::math angle conversion and related utilities (\c float vs \c fixed).
*/

#include "../benchmark_factory.hpp"
#include "math.hpp"

namespace toy::math {

using fixed_type = fixed<int32_t, int64_t, 24>;

// Utils benchmarks
void utilsMathBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  const float angleDegFloat = 15.0f;
  const float angleRadFloat = 1.25f;

  const fixed_type angleDegFixed{15.0f};
  const fixed_type angleRadFixed{1.25f};

  bench.run("deg2rad float", [&] {
    auto result = deg2rad(angleDegFloat);
    doNotOptimize(result);
  });

  bench.run("deg2rad fixed", [&] {
    auto result = deg2rad(angleDegFixed);
    doNotOptimize(result);
  });

  bench.run("rad2deg float", [&] {
    auto result = rad2deg(angleRadFloat);
    doNotOptimize(result);
  });

  bench.run("rad2deg fixed", [&] {
    auto result = rad2deg(angleRadFixed);
    doNotOptimize(result);
  });
}

} // namespace toy::math
