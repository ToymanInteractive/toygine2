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
  \file   main.cpp
  \brief  Entry point of the benchmark binary: applies the command line, prints the tables and the epochs behind them.
*/

// nanobench keeps its implementation behind this macro, and exactly one translation unit may define it.
#define ANKERL_NANOBENCH_IMPLEMENT

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>

#include "command_line.hpp"
#include "toy_benchmark.hpp"

namespace {

// Picoseconds in a second: nanobench times an epoch in seconds, and the CSV carries whole picoseconds.
constexpr double c_picosecondsPerSecond = 1e12;

// A CSV field, quoted when the text carries a comma, a quote, a tab or a line break, with inner quotes doubled.
[[nodiscard]] std::string csvField(std::string_view text) {
  if (text.find_first_of(",\"\n\r\t") == std::string_view::npos)
    return std::string{text};

  std::string quoted;
  quoted.reserve(text.size() + 2);
  quoted.push_back('"');

  for (const char character : text) {
    if (character == '"')
      quoted.push_back('"');

    quoted.push_back(character);
  }

  quoted.push_back('"');

  return quoted;
}

// Prints one CSV line per row of a finished table: the names once, then a pair of integers per timed epoch.
void printCsvRows(std::string_view caseName, const ankerl::nanobench::Bench & bench) {
  for (const auto & result : bench.results()) {
    const std::size_t epochs = result.size();

    // A desktop has no timer tick: ps_per_tick is one, so the epoch lengths below are picosecond counts.
    std::cout << "#csv " << csvField(caseName) << ',' << csvField(result.config().mBenchmarkName) << ','
              << csvField(result.config().mUnit) << ",1," << epochs;

    for (std::size_t epoch = 0; epoch < epochs; ++epoch) {
      const double iterations = result.get(epoch, ankerl::nanobench::Result::Measure::iterations);
      const auto   ticks = static_cast<std::uint64_t>(result.get(epoch, ankerl::nanobench::Result::Measure::elapsed)
                                                      * iterations * c_picosecondsPerSecond);

      std::cout << ',' << static_cast<std::uint64_t>(iterations) << ',' << ticks;
    }

    std::cout << '\n';
  }
}

} // namespace

int main(int argc, char ** argv) {
  const auto options = toy::benchmark::detail::parseOptions(argc, argv);

  // A name titles its table and selects it on the command line, so two under one name cannot be told apart.
  if (const auto * duplicate = ::toy::benchmark::detail::findDuplicateName(::toy::benchmark::detail::caseListHead);
      duplicate != nullptr) {
    std::fprintf(stderr, "duplicate benchmark name: %s\n  %s:%d\n  %s:%d\n", duplicate->name(), duplicate->file(),
                 duplicate->line(), duplicate->next()->file(), duplicate->next()->line());

    return 1;
  }

  // Running one benchmark alone separates a surprising number from a benchmark that disturbs its neighbours.
  char **   patterns = argv + options.firstPattern;
  const int count    = argc - options.firstPattern;

  if (options.csv)
    std::cout << "#csv case,row,unit,ps_per_tick,epochs,iters,ticks\n";

  for (const auto * benchmarkCase = ::toy::benchmark::detail::caseListHead; benchmarkCase != nullptr;
       benchmarkCase              = benchmarkCase->next()) {
    if (!toy::benchmark::detail::selected(benchmarkCase->name(), count, patterns))
      continue;

    if (options.listOnly) {
      std::printf("%s\t%s:%d\n", benchmarkCase->name(), benchmarkCase->file(), benchmarkCase->line());
      continue;
    }

    ankerl::nanobench::Bench bench;

    // Set here rather than in a body: a shared ratio column and epoch length keep two tables comparable.
    bench.title(benchmarkCase->name()).relative(true).minEpochTime(std::chrono::milliseconds(1));

    // nanobench prints the table from run(), so taking its stream away suppresses it.
    if (options.noTable)
      bench.output(nullptr);

    benchmarkCase->body()(bench);

    if (options.csv)
      printCsvRows(benchmarkCase->name(), bench);
  }

  return 0;
}
