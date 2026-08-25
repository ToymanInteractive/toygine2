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
#include <type_traits>

#include <doctest/doctest.h>

#include "context.hpp"
#include "utils.hpp"

static_assert(!std::is_copy_constructible_v<toy::test::Context>, "Context must not be copy constructible");
static_assert(!std::is_copy_assignable_v<toy::test::Context>, "Context must not be copy assignable");
static_assert(!std::is_move_constructible_v<toy::test::Context>, "Context must not be move constructible");
static_assert(!std::is_move_assignable_v<toy::test::Context>, "Context must not be move assignable");

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

// The capture lives in the case that reads it and arrives here through the context's reporter data, so no state
// outlives a case or travels between two of them.
void captureFailure(const toy::test::Context & context, const toy::test::FailureRecord & failure,
                    void * reporterData) noexcept {
  CapturedFailure & captured = *static_cast<CapturedFailure *>(reporterData);

  captured.caseName   = failure.caseName;
  captured.expression = failure.expression;
  captured.file       = failure.file;
  captured.line       = failure.line;
  captured.infoCount  = context.infoCount();
  captured.firstInfo  = context.infoCount() > 0 ? context.infoAt(0).text : nullptr;
  captured.firstValue = context.infoCount() > 0 ? context.infoAt(0).value : 0;
  ++captured.callCount;
}

} // namespace

// A passing assertion counts as passed, returns true and leaves the case clean.
TEST_CASE("test/context/record_passing_assertion") {
  CapturedFailure    captured;
  toy::test::Context context{&captureFailure, &captured};
  context.beginCase("some/case");

  const bool result = context.record(true, "1 == 1", "file.cpp", 10);

  REQUIRE(result == true);
  REQUIRE(context.passedCount() == 1);
  REQUIRE(context.failedCount() == 0);
  REQUIRE(context.caseFailed() == false);
  REQUIRE(captured.callCount == 0);
}

// A failing assertion counts as failed, returns false and marks the case.
TEST_CASE("test/context/record_failing_assertion") {
  CapturedFailure    captured;
  toy::test::Context context{&captureFailure, &captured};
  context.beginCase("some/case");

  const bool result = context.record(false, "1 == 2", "file.cpp", 42);

  REQUIRE(result == false);
  REQUIRE(context.passedCount() == 0);
  REQUIRE(context.failedCount() == 1);
  REQUIRE(context.caseFailed() == true);
}

// The reporter receives the case name and the assertion's own source location.
TEST_CASE("test/context/failure_reaches_reporter") {
  CapturedFailure    captured;
  toy::test::Context context{&captureFailure, &captured};
  context.beginCase("core/fixed_string/append");

  context.record(false, "str.size() == 4", "tests/core/fixed_string.test.cpp", 88);

  // Without a call the captured pointers stay null, which makes every comparison below meaningless.
  REQUIRE(captured.callCount == 1);
  REQUIRE(captured.caseName != nullptr);
  REQUIRE(captured.expression != nullptr);
  REQUIRE(captured.file != nullptr);

  REQUIRE(toy::test::detail::compareNames(captured.caseName, "core/fixed_string/append") == 0);
  REQUIRE(toy::test::detail::compareNames(captured.expression, "str.size() == 4") == 0);
  REQUIRE(toy::test::detail::compareNames(captured.file, "tests/core/fixed_string.test.cpp") == 0);
  REQUIRE(captured.line == 88);
}

// Info entries pushed before a failure reach the reporter through the context.
TEST_CASE("test/context/info_reaches_reporter") {
  CapturedFailure    captured;
  toy::test::Context context{&captureFailure, &captured};
  context.beginCase("some/case");
  context.pushInfo("index", 3);

  context.record(false, "value == 0", "file.cpp", 7);

  REQUIRE(captured.infoCount == 1);
  REQUIRE(captured.firstInfo != nullptr);

  REQUIRE(toy::test::detail::compareNames(captured.firstInfo, "index") == 0);
  REQUIRE(captured.firstValue == 3);
}

// Popping removes the entry, so a later failure carries no stale context.
TEST_CASE("test/context/pop_info_removes_entry") {
  CapturedFailure    captured;
  toy::test::Context context{&captureFailure, &captured};
  context.beginCase("some/case");
  context.pushInfo("index", 3);
  context.popInfo();

  context.record(false, "value == 0", "file.cpp", 7);

  REQUIRE(captured.infoCount == 0);
}

// A message without a value reports hasValue false, so the reporter can omit the number.
TEST_CASE("test/context/info_without_value") {
  CapturedFailure    captured;
  toy::test::Context context{&captureFailure, &captured};
  context.beginCase("some/case");
  context.pushInfo("after reload");

  // infoAt() requires an index below infoCount(), so the count gates the read.
  REQUIRE(context.infoCount() == 1);

  REQUIRE(context.infoAt(0).hasValue == false);
}

// Pushing past the fixed depth drops the surplus but keeps push and pop balanced.
TEST_CASE("test/context/info_stack_overflow_stays_balanced") {
  CapturedFailure    captured;
  toy::test::Context context{&captureFailure, &captured};
  context.beginCase("some/case");

  for (std::size_t index = 0; index < toy::test::Context::c_maxInfoDepth + 4; ++index)
    context.pushInfo("entry", static_cast<long long>(index));

  REQUIRE(context.infoCount() == toy::test::Context::c_maxInfoDepth);

  for (std::size_t index = 0; index < toy::test::Context::c_maxInfoDepth + 4; ++index)
    context.popInfo();

  REQUIRE(context.infoCount() == 0);
}

// beginCase clears the per-case verdict but keeps the totals, which span the whole run.
TEST_CASE("test/context/begin_case_clears_verdict_and_keeps_totals") {
  CapturedFailure    captured;
  toy::test::Context context{&captureFailure, &captured};
  context.beginCase("first/case");
  context.record(false, "1 == 2", "file.cpp", 1);

  context.beginCase("second/case");

  REQUIRE(context.caseFailed() == false);
  REQUIRE(context.failedCount() == 1);
  REQUIRE(context.subcaseName() == nullptr);

  REQUIRE(context.caseName() != nullptr);

  REQUIRE(toy::test::detail::compareNames(context.caseName(), "second/case") == 0);
}
