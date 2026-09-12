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
  \brief  Entry point of the benchmark binary: applies the command line and prints one table per selected benchmark.
*/

// nanobench keeps its implementation behind this macro, and exactly one translation unit may define it.
#define ANKERL_NANOBENCH_IMPLEMENT

#include <chrono>
#include <cstdio>
#include <cstring>
#include <optional>
#include <string_view>

#include "toy_benchmark.hpp"

namespace {

// What the leading options asked for; the arguments from firstPattern on are patterns.
struct Options final {
  bool         listOnly     = false;
  const char * bmfPath      = nullptr;
  int          firstPattern = 1;
};

// Reads the leading --list and --bmf <path> in any order. Empty when --bmf is the last argument.
[[nodiscard]] std::optional<Options> parseOptions(int argc, char ** argv) noexcept {
  Options options;

  while (options.firstPattern < argc) {
    if (const char * argument = argv[options.firstPattern]; std::strcmp(argument, "--list") == 0) {
      options.listOnly = true;
    } else if (std::strcmp(argument, "--bmf") == 0) {
      if (options.firstPattern + 1 == argc)
        return std::nullopt;

      options.bmfPath = argv[options.firstPattern + 1];
      ++options.firstPattern;
    } else {
      break;
    }

    ++options.firstPattern;
  }

  return options;
}

// Whether a name carries any of the patterns as a substring. No pattern at all selects every benchmark.
[[nodiscard]] bool selected(std::string_view name, int count, char ** patterns) noexcept {
  if (count <= 0)
    return true;

  for (int index = 0; index < count; ++index)
    if (name.contains(patterns[index]))
      return true;

  return false;
}

} // namespace

int main(int argc, char ** argv) {
  const auto options = parseOptions(argc, argv);
  if (!options) {
    std::fprintf(stderr, "usage: %s [--list] [--bmf <path>] [pattern...]\n", argv[0]);

    return 1;
  }

  // A name titles its table and selects it on the command line, so two under one name cannot be told apart.
  if (const auto * duplicate = ::toy::benchmark::detail::findDuplicateName(::toy::benchmark::detail::caseListHead);
      duplicate != nullptr) {
    std::fprintf(stderr, "duplicate benchmark name: %s\n  %s:%d\n  %s:%d\n", duplicate->name(), duplicate->file(),
                 duplicate->line(), duplicate->next()->file(), duplicate->next()->line());

    return 1;
  }

  // Running one benchmark alone separates a surprising number from a benchmark that disturbs its neighbours.
  char **   patterns = argv + options->firstPattern;
  const int count    = argc - options->firstPattern;

  for (const auto * benchmarkCase = ::toy::benchmark::detail::caseListHead; benchmarkCase != nullptr;
       benchmarkCase              = benchmarkCase->next()) {
    if (!selected(benchmarkCase->name(), count, patterns))
      continue;

    if (options->listOnly) {
      std::printf("%s\t%s:%d\n", benchmarkCase->name(), benchmarkCase->file(), benchmarkCase->line());
      continue;
    }

    ankerl::nanobench::Bench bench;

    // Set here rather than in a body: a shared ratio column and epoch length keep two tables comparable.
    bench.title(benchmarkCase->name()).relative(true).minEpochTime(std::chrono::milliseconds(1));

    benchmarkCase->body()(bench);
  }

  return 0;
}
