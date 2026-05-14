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

namespace toy::geometry {

void circleGeometryBenchmarks(ankerl::nanobench::Bench &) noexcept;
void ellipseGeometryBenchmarks(ankerl::nanobench::Bench &) noexcept;
void sectionGeometryBenchmarks(ankerl::nanobench::Bench &) noexcept;

} // namespace toy::geometry

using namespace toy::geometry;

using fixed_type = toy::math::fixed<int32_t, int64_t, 24>;

void runGeometryBenchmarks(ankerl::nanobench::Bench bench) noexcept {
  toy::geometry::circleGeometryBenchmarks(bench);
  toy::geometry::ellipseGeometryBenchmarks(bench);
  toy::geometry::sectionGeometryBenchmarks(bench);
}
