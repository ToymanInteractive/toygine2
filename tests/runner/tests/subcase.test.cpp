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
  \file   subcase.test.cpp
  \brief  Unit tests for \ref toy::test::detail::SubcaseGuard and the runner's repeated-run loop.
*/

#include <cstddef>

#include <doctest/doctest.h>

#include "toy_test.hpp"

namespace {

std::size_t                g_runCount       = 0;
std::array<std::size_t, 3> g_enterCount     = {0, 0, 0};
std::size_t                g_outerBodyCount = 0;

void resetCounters() noexcept {
  g_runCount       = 0;
  g_outerBodyCount = 0;

  for (std::size_t index = 0; index < 3; ++index)
    g_enterCount[index] = 0;
}

// A body without any subcase: the loop must call it exactly once.
void bodyWithoutSubcases(toy::test::Context & context) {
  static_cast<void>(context);
  ++g_runCount;
}

// Three sibling subcases, written the way the SUBCASE macro will expand in task 5.
void bodyWithThreeSubcases(toy::test::Context & context) {
  ++g_runCount;

  {
    const toy::test::detail::SubcaseGuard guard{context, "first"};
    if (guard.entered())
      ++g_enterCount[0];
  }
  {
    const toy::test::detail::SubcaseGuard guard{context, "second"};
    if (guard.entered())
      ++g_enterCount[1];
  }
  {
    const toy::test::detail::SubcaseGuard guard{context, "third"};
    if (guard.entered())
      ++g_enterCount[2];
  }
}

// A subcase inside a subcase, which the rules forbid and the guard must diagnose.
void bodyWithNestedSubcase(toy::test::Context & context) {
  ++g_runCount;

  const toy::test::detail::SubcaseGuard outer{context, "outer"};
  if (outer.entered()) {
    ++g_outerBodyCount;

    const toy::test::detail::SubcaseGuard inner{context, "inner"};
    static_cast<void>(inner);
  }
}

// Records the subcase name visible at the moment of a failed assertion.
void bodyFailingInsideSubcase(toy::test::Context & context) {
  const toy::test::detail::SubcaseGuard guard{context, "the_subcase"};
  if (guard.entered())
    context.record(false, "false", "file.cpp", 5);
}

// Two sibling subcases where only the first fails, so a later clean run follows the failing one.
void bodyFailingInTheFirstOfTwo(toy::test::Context & context) {
  ++g_runCount;

  {
    const toy::test::detail::SubcaseGuard guard{context, "first"};
    if (guard.entered()) {
      ++g_enterCount[0];
      context.record(false, "false", "file.cpp", 7);
    }
  }
  {
    const toy::test::detail::SubcaseGuard guard{context, "second"};
    if (guard.entered()) {
      ++g_enterCount[1];
      context.record(true, "true", "file.cpp", 12);
    }
  }
}

const char * g_capturedSubcaseName = nullptr;

void captureSubcaseName(const toy::test::Context & context, const toy::test::FailureRecord & failure) noexcept {
  static_cast<void>(context);
  g_capturedSubcaseName = failure.subcaseName;
}

} // namespace

// A case without subcases runs once.
TEST_CASE("test/run_case/case_without_subcases_runs_once") {
  resetCounters();
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "some/case", &bodyWithoutSubcases);

  REQUIRE(g_runCount == 1);
  REQUIRE(context.subcaseCount() == 0);
}

// Three subcases produce three runs, and each subcase body executes exactly once.
TEST_CASE("test/run_case/each_subcase_runs_exactly_once") {
  resetCounters();
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "some/case", &bodyWithThreeSubcases);

  REQUIRE(g_runCount == 3);
  REQUIRE(context.subcaseCount() == 3);
  REQUIRE(g_enterCount[0] == 1);
  REQUIRE(g_enterCount[1] == 1);
  REQUIRE(g_enterCount[2] == 1);
}

// Entering a subcase from inside a subcase is diagnosed and the inner one is not entered.
TEST_CASE("test/run_case/nested_subcase_is_diagnosed") {
  resetCounters();
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "some/case", &bodyWithNestedSubcase);

  REQUIRE(context.nestedSubcaseDetected() == true);
  REQUIRE(g_outerBodyCount == 1);
}

// The running subcase name reaches the failure record, so a report names the failing branch.
TEST_CASE("test/run_case/failure_carries_subcase_name") {
  resetCounters();
  g_capturedSubcaseName = nullptr;
  toy::test::Context context{&captureSubcaseName};

  toy::test::runCase(context, "some/case", &bodyFailingInsideSubcase);

  REQUIRE(g_capturedSubcaseName != nullptr);
  REQUIRE(toy::test::detail::compareNames(g_capturedSubcaseName, "the_subcase") == 0);
}

// Leaving a subcase clears the name, so a later failure outside one carries no stale branch.
TEST_CASE("test/run_case/subcase_name_clears_on_exit") {
  resetCounters();
  toy::test::Context context{nullptr};
  context.beginCase("some/case");
  context.beginRun(0);

  const bool entered = context.enterSubcase("branch");
  REQUIRE(entered == true);
  REQUIRE(toy::test::detail::compareNames(context.subcaseName(), "branch") == 0);

  context.leaveSubcase();

  REQUIRE(context.subcaseName() == nullptr);
}

// A failing case keeps its verdict across runs; one bad subcase condemns the case even when a later run passes.
TEST_CASE("test/run_case/verdict_survives_across_runs") {
  resetCounters();
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "some/case", &bodyFailingInTheFirstOfTwo);

  REQUIRE(g_runCount == 2);
  REQUIRE(g_enterCount[0] == 1);
  REQUIRE(g_enterCount[1] == 1);
  REQUIRE(context.caseFailed() == true);
  REQUIRE(context.failedCount() == 1);
  REQUIRE(context.passedCount() == 1);
}
