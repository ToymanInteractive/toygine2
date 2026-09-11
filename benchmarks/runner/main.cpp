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
  \brief  Entry point of the benchmark binary: prints one table per registered benchmark.
*/

#include <chrono>
#include <cstdio>
#include <cstring>

#include "toy_benchmark.hpp"

namespace {

// Whether a name carries any of the patterns as a substring. No pattern at all selects every benchmark.
[[nodiscard]] bool selected(const char * name, int count, char ** patterns) noexcept {
  if (count <= 0)
    return true;

  for (int index = 0; index < count; ++index)
    if (std::strstr(name, patterns[index]) != nullptr)
      return true;

  return false;
}

} // namespace

int main(int argc, char ** argv) {
  // Running one benchmark alone is what tells a surprising number from a benchmark that disturbs its neighbours.
  const bool listOnly = argc > 1 && std::strcmp(argv[1], "--list") == 0;
  const int  skipped  = listOnly ? 2 : 1;

  char **   patterns = argv + skipped;
  const int count    = argc - skipped;

  for (const auto * benchmarkCase = ::toy::benchmark::detail::caseListHead; benchmarkCase != nullptr;
       benchmarkCase              = benchmarkCase->next()) {
    if (!selected(benchmarkCase->name(), count, patterns))
      continue;

    if (listOnly) {
      std::printf("%s\t%s:%d\n", benchmarkCase->name(), benchmarkCase->file(), benchmarkCase->line());
      continue;
    }

    ankerl::nanobench::Bench bench;

    // Every table is configured here rather than in a body: the ratio column and the epoch length are what make two
    // tables comparable, and an epoch shorter than this measures the clock instead of the call.
    bench.title(benchmarkCase->name()).relative(true).minEpochTime(std::chrono::milliseconds(1));

    benchmarkCase->body()(bench);
  }

  return 0;
}
