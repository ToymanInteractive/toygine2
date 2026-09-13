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
  \file   builtin_bench.test.cpp
  \brief  Unit tests for the CSV rows the console benchmark runner prints.
*/

#include <cstddef>
#include <cstdint>
#include <string>

#include <doctest/doctest.h>

#include "builtin_bench.hpp"

namespace {

// Moves only when an operation moves it, so every epoch lasts exactly what its calls add.
struct ManualClock final {
  std::uint32_t ticks;
};

std::uint32_t readManualClock(const void * clockData) noexcept {
  return static_cast<const ManualClock *>(clockData)->ticks;
}

struct CapturedLines final {
  std::string text;
};

void captureLines(const char * text, std::size_t length, const void * writerData) noexcept {
  CapturedLines & captured = **static_cast<CapturedLines * const *>(writerData);

  captured.text.append(text, length);
}

} // namespace

// Every row carries the names once, then the calls and the length of each of the seven epochs.
TEST_CASE("benchmark/bench/prints_one_csv_row_per_run") {
  ManualClock           clock{};
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;

  const toy::benchmark::Clock timer{.now                = &readManualClock,
                                    .clockData          = &clock,
                                    .picosecondsPerTick = 1000,
                                    .minEpochTicks      = 16,
                                    .maxIterations      = 1024};

  toy::benchmark::Bench bench{
    timer, {.write = &captureLines, .writerData = &capturedPointer},
     "core/fake/title"
  };

  bench.unit("search");
  bench.run("fast", [&clock] {
    clock.ticks += 3;
  });
  bench.run("slow", [&clock] {
    clock.ticks += 6;
  });

  CHECK(captured.text
        == "#csv core/fake/title,fast,search,1000,7,8,24,8,24,8,24,8,24,8,24,8,24,8,24\n"
           "#csv core/fake/title,slow,search,1000,7,4,24,4,24,4,24,4,24,4,24,4,24,4,24\n");
}

// A row name with a template argument list keeps the field count: the comma inside it is quoted.
TEST_CASE("benchmark/bench/quotes_a_row_name_with_a_comma") {
  ManualClock           clock{};
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;

  const toy::benchmark::Clock timer{.now                = &readManualClock,
                                    .clockData          = &clock,
                                    .picosecondsPerTick = 1000,
                                    .minEpochTicks      = 16,
                                    .maxIterations      = 1024};

  toy::benchmark::Bench bench{
    timer, {.write = &captureLines, .writerData = &capturedPointer},
     "core/fixed_string/append"
  };

  bench.run("toy::FixedString<32, char>", [&clock] {
    clock.ticks += 16;
  });

  CHECK(
    captured.text
    == "#csv core/fixed_string/append,\"toy::FixedString<32, char>\",op,1000,7,1,16,1,16,1,16,1,16,1,16,1,16,1,16\n"
  );
}

// A row longer than the line becomes a comment: a cut CSV row would end the parse of the whole log.
TEST_CASE("benchmark/bench/overlong_row_becomes_a_comment") {
  ManualClock           clock{};
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;
  const std::string     longName(300, 'x');

  const toy::benchmark::Clock timer{.now                = &readManualClock,
                                    .clockData          = &clock,
                                    .picosecondsPerTick = 1000,
                                    .minEpochTicks      = 16,
                                    .maxIterations      = 1024};

  toy::benchmark::Bench bench{
    timer, {.write = &captureLines, .writerData = &capturedPointer},
     "core/fake/title"
  };

  bench.run(longName.c_str(), [&clock] {
    clock.ticks += 16;
  });

  CHECK(captured.text.starts_with("# xxx"));
  CHECK(captured.text.find("#csv ") == std::string::npos);
  CHECK(captured.text.back() == '\n');
}

// A timer standing still gets a comment naming both limits in place of the row.
TEST_CASE("benchmark/bench/stalled_timer_names_the_limits") {
  ManualClock           clock{};
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;

  const toy::benchmark::Clock timer{.now                = &readManualClock,
                                    .clockData          = &clock,
                                    .picosecondsPerTick = 1000,
                                    .minEpochTicks      = 16,
                                    .maxIterations      = 4};

  toy::benchmark::Bench bench{
    timer, {.write = &captureLines, .writerData = &capturedPointer},
     "core/fake/title"
  };

  bench.run("idle", [] {});

  CHECK(captured.text == "# idle: no epoch reached 16 ticks within 4 calls\n");
}

// Calibration stops at a limit that is not a power of two instead of doubling past it.
TEST_CASE("benchmark/bench/calibration_stays_within_the_limit") {
  ManualClock           clock{};
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;
  std::uint32_t         calls           = 0;

  const toy::benchmark::Clock timer{.now                = &readManualClock,
                                    .clockData          = &clock,
                                    .picosecondsPerTick = 1000,
                                    .minEpochTicks      = 16,
                                    .maxIterations      = 3};

  toy::benchmark::Bench bench{
    timer, {.write = &captureLines, .writerData = &capturedPointer},
     "core/fake/title"
  };

  bench.run("idle", [&calls] {
    ++calls;
  });

  // Epochs of one, two and three calls; a fourth epoch would have run four.
  CHECK(calls == 6);
  CHECK(captured.text == "# idle: no epoch reached 16 ticks within 3 calls\n");
}

// A limit below one call ends the row without running the operation.
TEST_CASE("benchmark/bench/zero_limit_runs_nothing") {
  ManualClock           clock{};
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;
  std::uint32_t         calls           = 0;

  const toy::benchmark::Clock timer{.now                = &readManualClock,
                                    .clockData          = &clock,
                                    .picosecondsPerTick = 1000,
                                    .minEpochTicks      = 16,
                                    .maxIterations      = 0};

  toy::benchmark::Bench bench{
    timer, {.write = &captureLines, .writerData = &capturedPointer},
     "core/fake/title"
  };

  bench.run("idle", [&calls] {
    ++calls;
  });

  CHECK(calls == 0);
  CHECK(captured.text == "# idle: no epoch reached 16 ticks within 0 calls\n");
}

// A timer that advances below the epoch length reports the same two limits, so the message stays true for it too.
TEST_CASE("benchmark/bench/short_epochs_name_the_limits") {
  ManualClock           clock{};
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;
  std::uint32_t         calls           = 0;

  const toy::benchmark::Clock timer{.now                = &readManualClock,
                                    .clockData          = &clock,
                                    .picosecondsPerTick = 1000,
                                    .minEpochTicks      = 16,
                                    .maxIterations      = 4};

  toy::benchmark::Bench bench{
    timer, {.write = &captureLines, .writerData = &capturedPointer},
     "core/fake/title"
  };

  bench.run("slow", [&clock, &calls] {
    ++clock.ticks;
    ++calls;
  });

  // Epochs of one, two and four calls last one, two and four ticks, all short of sixteen.
  CHECK(calls == 7);
  CHECK(captured.text == "# slow: no epoch reached 16 ticks within 4 calls\n");
}
