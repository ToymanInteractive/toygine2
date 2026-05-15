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

#include <array>
#include <fstream>
#include <iostream>

#include <nanobench.h>

#include "benchmark_factory.hpp"

void runCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void runGeometryBenchmarks(ankerl::nanobench::Bench &) noexcept;
void runMathBenchmarks(ankerl::nanobench::Bench &) noexcept;

namespace {

using benchmark_fn_type = void (*)(ankerl::nanobench::Bench &) noexcept;

struct BenchmarkEntry {
  const char *      name;
  benchmark_fn_type fn;
};

constexpr std::array<BenchmarkEntry, 3> c_benchmarks{
  {
   {"Core module", runCoreBenchmarks},
   {"Geometry module", runGeometryBenchmarks},
   {"Math module", runMathBenchmarks},
   }
};

// Renders results in Bencher Metric Format (BMF) for bencher.dev ingestion.
// Schema: https://bencher.dev/bmf.json
constexpr char c_bmfTemplate[] = R"({
{{#result}}  "{{title}}/{{name}}": {"latency": {"value": {{median(elapsed)}}}}{{^-last}},{{/-last}}
{{/result}}})";

} // namespace

int main(int argc, char * argv[]) noexcept {
  if (argc > 1) {
    auto bench = createBench("toygine2");

    for (const auto & entry : c_benchmarks)
      entry.fn(bench);

    std::ofstream out(argv[1]);
    if (!out.is_open()) {
      std::cerr << "Failed to open output file: " << argv[1] << std::endl;

      return 1;
    }

    ankerl::nanobench::render(c_bmfTemplate, bench, out);
    out.flush();
    if (!out.good()) {
      std::cerr << "Failed to write benchmark output file: " << argv[1] << std::endl;

      return 1;
    }

  } else {
    for (const auto & entry : c_benchmarks) {
      auto bench = createBench(entry.name);
      entry.fn(bench);
    }
  }

  return 0;
}
