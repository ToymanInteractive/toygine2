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
  \file   console_report.test.cpp
  \brief  Unit tests for the report the console benchmark runner prints.
*/

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include <doctest/doctest.h>

#include "console_report.hpp"

namespace {

// Moves a fixed step on every read, so each epoch lasts one step whatever the calls inside it.
struct SteppingClock final {
  std::uint32_t ticks;
  std::uint32_t step;
};

std::uint32_t readSteppingClock(const void * clockData) noexcept {
  SteppingClock & clock = **static_cast<SteppingClock * const *>(clockData);

  clock.ticks += clock.step;

  return clock.ticks;
}

struct CapturedLines final {
  std::string text;
};

void captureLines(const char * text, std::size_t length, const void * writerData) noexcept {
  CapturedLines & captured = **static_cast<CapturedLines * const *>(writerData);

  captured.text.append(text, length);
}

void oneRow(toy::benchmark::Bench & bench) {
  bench.run("row", [] {});
}

constexpr std::array<const char *, 2> c_heading = {"Test", "abc1234"};

} // namespace

// The heading and the CSV header come first, rows follow in name order, and "# done" closes the report.
TEST_CASE("benchmark/write_report/prints_rows_in_name_order") {
  toy::benchmark::CaseRegistrar * head = nullptr;
  toy::benchmark::CaseRegistrar   second{head, "core/b", "b.cpp", 20, &oneRow};
  toy::benchmark::CaseRegistrar   first{head, "core/a", "a.cpp", 10, &oneRow};

  SteppingClock         clock{.ticks = 0, .step = 5};
  SteppingClock * const clockPointer = &clock;
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;

  const int code = toy::benchmark::writeReport({.now                = &readSteppingClock,
                                                .clockData          = &clockPointer,
                                                .picosecondsPerTick = 1000,
                                                .minEpochTicks      = 4,
                                                .maxIterations      = 16},
                                               {.write = &captureLines, .writerData = &capturedPointer}, c_heading,
                                               head);

  CHECK(code == 0);
  CHECK(captured.text
        == "# toygine2 benchmarks: Test abc1234\n"
           "#csv case,row,unit,ps_per_tick,epochs,iters,ticks\n"
           "#csv core/a,row,op,1000,7,1,5,1,5,1,5,1,5,1,5,1,5,1,5\n"
           "#csv core/b,row,op,1000,7,1,5,1,5,1,5,1,5,1,5,1,5,1,5\n"
           "# done\n");
}

// Rows without numbers are summed across benchmarks into "# failed rows", which takes the place of "# done".
TEST_CASE("benchmark/write_report/failed_rows_withhold_done") {
  toy::benchmark::CaseRegistrar * head = nullptr;
  toy::benchmark::CaseRegistrar   second{head, "core/b", "b.cpp", 20, &oneRow};
  toy::benchmark::CaseRegistrar   first{head, "core/a", "a.cpp", 10, &oneRow};

  SteppingClock         clock{.ticks = 0, .step = 0};
  SteppingClock * const clockPointer = &clock;
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;

  const int code = toy::benchmark::writeReport({.now                = &readSteppingClock,
                                                .clockData          = &clockPointer,
                                                .picosecondsPerTick = 1000,
                                                .minEpochTicks      = 4,
                                                .maxIterations      = 16},
                                               {.write = &captureLines, .writerData = &capturedPointer}, c_heading,
                                               head);

  CHECK(code == 1);
  CHECK(captured.text
        == "# toygine2 benchmarks: Test abc1234\n"
           "#csv case,row,unit,ps_per_tick,epochs,iters,ticks\n"
           "# no epoch reached 4 ticks within 16 calls: row\n"
           "# no epoch reached 4 ticks within 16 calls: row\n"
           "# failed rows: 2\n");
}

// A repeated name prints both declarations and stops before any row.
TEST_CASE("benchmark/write_report/repeated_name_stops_before_rows") {
  toy::benchmark::CaseRegistrar * head = nullptr;
  toy::benchmark::CaseRegistrar   earlier{head, "core/same", "a.cpp", 10, &oneRow};
  toy::benchmark::CaseRegistrar   later{head, "core/same", "b.cpp", 20, &oneRow};

  SteppingClock         clock{.ticks = 0, .step = 5};
  SteppingClock * const clockPointer = &clock;
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;

  const int code = toy::benchmark::writeReport({.now                = &readSteppingClock,
                                                .clockData          = &clockPointer,
                                                .picosecondsPerTick = 1000,
                                                .minEpochTicks      = 4,
                                                .maxIterations      = 16},
                                               {.write = &captureLines, .writerData = &capturedPointer}, c_heading,
                                               head);

  CHECK(code == 1);
  CHECK(captured.text
        == "# toygine2 benchmarks: Test abc1234\n"
           "# duplicate benchmark name: core/same\n"
           "#   b.cpp:20\n"
           "#   a.cpp:10\n");
}
