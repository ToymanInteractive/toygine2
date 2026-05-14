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
  \file   main.cpp
  \brief  Entry point for nanobench benchmarks (core, geometry, math).
*/

#include <fstream>

#include <nanobench.h>

#include "benchmark_factory.hpp"

void runCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void runGeometryBenchmarks(ankerl::nanobench::Bench &) noexcept;
void runMathBenchmarks(ankerl::nanobench::Bench &) noexcept;

int main(int argc, char * argv[]) noexcept {
  auto bench         = createBench("toygine2");
  auto coreBench     = createBench("Core module");
  auto geometryBench = createBench("Geometry module");
  auto mathBench     = createBench("Math module");

  runCoreBenchmarks(argc > 1 ? bench : coreBench);
  runGeometryBenchmarks(argc > 1 ? bench : geometryBench);
  runMathBenchmarks(argc > 1 ? bench : mathBench);

  if (argc > 1) {
    std::ofstream out(argv[1]);
    if (!out.is_open())
      return 1;

    ankerl::nanobench::render(ankerl::nanobench::templates::json(), bench, out);
  }

  return 0;
}
