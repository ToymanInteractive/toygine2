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

#define PICOBENCH_IMPLEMENT
#include "picobench/picobench.hpp"

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

} // namespace

int main(int argc, char * argv[]) noexcept {
  picobench::runner runner;

  runner.run_benchmarks(123);
  auto report = runner.generate_report();
  report.to_text(std::cout);

  auto bench
    = ankerl::nanobench::Bench().title("toygine2").warmup(100).epochs(100).minEpochIterations(1000).relative(true);

  for (const auto & entry : c_benchmarks)
    entry.fn(bench.unit(entry.name));

  if (argc > 1) {
    std::ofstream json(argv[1]);
    if (!json.is_open()) {
      std::cerr << "Failed to open output file: " << argv[1] << std::endl;

      return 1;
    }

    // Bencher Metric Format (BMF) :
    // https://bencher.dev/docs/reference/bencher-metric-format/#example-bencher-metric-format-bmf-json

    json << "{" << std::endl;
    for (const auto & suite : report.suites) {
      for (size_t benchmarkIndex = 0; benchmarkIndex < suite.benchmarks.size(); ++benchmarkIndex) {
        const auto & benchmark            = suite.benchmarks[benchmarkIndex];
        auto         latencyLowerValue    = std::numeric_limits<double>::max();
        auto         latencyUpperValue    = std::numeric_limits<double>::min();
        auto         throughputLowerValue = std::numeric_limits<double>::max();
        auto         throughputUpperValue = std::numeric_limits<double>::min();

        for (const auto & data : benchmark.data) {
          const auto latencyValue = static_cast<double>(data.total_time_ns) / data.dimension;
          latencyLowerValue       = std::min(latencyLowerValue, latencyValue);
          latencyUpperValue       = std::max(latencyUpperValue, latencyValue);

          const auto throughputValue = data.dimension * (1000000000.0 / static_cast<double>(data.total_time_ns));
          throughputLowerValue       = std::min(throughputLowerValue, throughputValue);
          throughputUpperValue       = std::max(throughputUpperValue, throughputValue);
        }

        json << "  \"" << suite.name << "." << benchmark.name << "\": {";

        json << "\"latency\":{";
        json << "\"value\": " << (latencyLowerValue + latencyUpperValue) * 0.5 << ",";
        json << "\"lower_value\": " << latencyLowerValue << ",";
        json << "\"upper_value\": " << latencyUpperValue;
        json << "},";

        json << "\"throughput\":{";
        json << "\"value\": " << (throughputLowerValue + throughputUpperValue) * 0.5 << ",";
        json << "\"lower_value\": " << throughputLowerValue << ",";
        json << "\"upper_value\": " << throughputUpperValue;
        json << "}";

        json << "}" << (benchmarkIndex < suite.benchmarks.size() - 1 ? "," : "") << std::endl;
      }
    }
    json << "}" << std::endl;

    json.flush();
    if (!json.good()) {
      std::cerr << "Failed to write benchmark output file: " << argv[1] << std::endl;

      return 1;
    }
  }

  return 0;
}
