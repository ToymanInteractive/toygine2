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
  \file   context.test.cpp
  \brief  Unit tests for \ref toy::test::Context: assertion counters, failure reporting and the info stack.
*/

#include <cstddef>

#include <doctest/doctest.h>

#include "toy_test.hpp"

namespace {

struct CapturedFailure final {
  const char * caseName   = nullptr;
  const char * expression = nullptr;
  const char * file       = nullptr;
  int          line       = 0;
  std::size_t  infoCount  = 0;
  const char * firstInfo  = nullptr;
  long long    firstValue = 0;
  std::size_t  callCount  = 0;
};

CapturedFailure g_captured;

void captureFailure(const toy::test::Context & context, const toy::test::FailureRecord & failure) noexcept {
  g_captured.caseName   = failure.caseName;
  g_captured.expression = failure.expression;
  g_captured.file       = failure.file;
  g_captured.line       = failure.line;
  g_captured.infoCount  = context.infoCount();
  g_captured.firstInfo  = context.infoCount() > 0 ? context.infoAt(0).text : nullptr;
  g_captured.firstValue = context.infoCount() > 0 ? context.infoAt(0).value : 0;
  ++g_captured.callCount;
}

// Fresh capture plus a context wired to it; every case starts from this to stay order-independent.
CapturedFailure & resetCapture() {
  g_captured = CapturedFailure{};

  return g_captured;
}

} // namespace

// A passing assertion counts as passed, returns true and leaves the case clean.
TEST_CASE("test/context/record_passing_assertion") {
  resetCapture();
  toy::test::Context context{&captureFailure};
  context.beginCase("some/case");

  const bool result = context.record(true, "1 == 1", "file.cpp", 10);

  REQUIRE(result == true);
  REQUIRE(context.passedCount() == 1);
  REQUIRE(context.failedCount() == 0);
  REQUIRE(context.caseFailed() == false);
  REQUIRE(g_captured.callCount == 0);
}

// A failing assertion counts as failed, returns false and marks the case.
TEST_CASE("test/context/record_failing_assertion") {
  resetCapture();
  toy::test::Context context{&captureFailure};
  context.beginCase("some/case");

  const bool result = context.record(false, "1 == 2", "file.cpp", 42);

  REQUIRE(result == false);
  REQUIRE(context.passedCount() == 0);
  REQUIRE(context.failedCount() == 1);
  REQUIRE(context.caseFailed() == true);
}

// The reporter receives the case name and the assertion's own source location.
TEST_CASE("test/context/failure_reaches_reporter") {
  resetCapture();
  toy::test::Context context{&captureFailure};
  context.beginCase("core/fixed_string/append");

  context.record(false, "str.size() == 4", "tests/core/fixed_string.test.cpp", 88);

  REQUIRE(g_captured.callCount == 1);
  REQUIRE(toy::test::detail::compareNames(g_captured.caseName, "core/fixed_string/append") == 0);
  REQUIRE(toy::test::detail::compareNames(g_captured.expression, "str.size() == 4") == 0);
  REQUIRE(toy::test::detail::compareNames(g_captured.file, "tests/core/fixed_string.test.cpp") == 0);
  REQUIRE(g_captured.line == 88);
}

// Info entries pushed before a failure reach the reporter through the context.
TEST_CASE("test/context/info_reaches_reporter") {
  resetCapture();
  toy::test::Context context{&captureFailure};
  context.beginCase("some/case");
  context.pushInfo("index", 3);

  context.record(false, "value == 0", "file.cpp", 7);

  REQUIRE(g_captured.infoCount == 1);
  REQUIRE(toy::test::detail::compareNames(g_captured.firstInfo, "index") == 0);
  REQUIRE(g_captured.firstValue == 3);
}

// Popping removes the entry, so a later failure carries no stale context.
TEST_CASE("test/context/pop_info_removes_entry") {
  resetCapture();
  toy::test::Context context{&captureFailure};
  context.beginCase("some/case");
  context.pushInfo("index", 3);
  context.popInfo();

  context.record(false, "value == 0", "file.cpp", 7);

  REQUIRE(g_captured.infoCount == 0);
}

// A message without a value reports hasValue false, so the reporter can omit the number.
TEST_CASE("test/context/info_without_value") {
  resetCapture();
  toy::test::Context context{&captureFailure};
  context.beginCase("some/case");
  context.pushInfo("after reload");

  REQUIRE(context.infoCount() == 1);
  REQUIRE(context.infoAt(0).hasValue == false);
}

// Pushing past the fixed depth drops the surplus but keeps push and pop balanced.
TEST_CASE("test/context/info_stack_overflow_stays_balanced") {
  resetCapture();
  toy::test::Context context{&captureFailure};
  context.beginCase("some/case");

  for (std::size_t index = 0; index < 12; ++index)
    context.pushInfo("entry", static_cast<long long>(index));

  REQUIRE(context.infoCount() == 8);

  for (std::size_t index = 0; index < 12; ++index)
    context.popInfo();

  REQUIRE(context.infoCount() == 0);
}

// beginCase clears the per-case verdict but keeps the totals, which span the whole run.
TEST_CASE("test/context/begin_case_clears_verdict_and_keeps_totals") {
  resetCapture();
  toy::test::Context context{&captureFailure};
  context.beginCase("first/case");
  context.record(false, "1 == 2", "file.cpp", 1);

  context.beginCase("second/case");

  REQUIRE(context.caseFailed() == false);
  REQUIRE(context.failedCount() == 1);
  REQUIRE(toy::test::detail::compareNames(context.caseName(), "second/case") == 0);
  REQUIRE(context.subcaseName() == nullptr);
}
