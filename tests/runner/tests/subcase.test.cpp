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

#include <array>
#include <cstddef>

#include <doctest/doctest.h>

#include "toy_test.hpp"

namespace {

// A case body reaches the runner as a bare function pointer, so it carries no state of its own. Every count a case
// below asserts therefore travels through the context that case owns: one recorded assertion per run, one more per
// entered subcase.
void bodyWithoutSubcases(toy::test::Context & context) {
  context.record(true, "the body ran", "file.cpp", 1);
}

// A subcase inside a subcase, which the rules forbid and the guard must diagnose.
void bodyWithNestedSubcase(toy::test::Context & context) {
  const toy::test::detail::SubcaseGuard outer{context, "outer"};
  if (outer.entered()) {
    context.record(true, "outer is running", "file.cpp", 1);

    const toy::test::detail::SubcaseGuard inner{context, "inner"};
    context.record(!inner.entered(), "a nested subcase must not be entered", "file.cpp", 2);
  }
}

// Fails inside its only subcase, so the failure record carries a subcase name.
void bodyFailingInsideSubcase(toy::test::Context & context) {
  const toy::test::detail::SubcaseGuard guard{context, "the_subcase"};
  if (guard.entered())
    context.record(false, "false", "file.cpp", 5);
}

// Two sibling subcases where only the first fails, so a clean run follows the failing one.
void bodyFailingInTheFirstOfTwo(toy::test::Context & context) {
  {
    const toy::test::detail::SubcaseGuard guard{context, "first"};
    if (guard.entered())
      context.record(false, "false", "file.cpp", 7);
  }
  {
    const toy::test::detail::SubcaseGuard guard{context, "second"};
    if (guard.entered())
      context.record(true, "true", "file.cpp", 12);
  }
}

// A reporter is a bare function pointer as well, and the context it receives is const, so the record it inspects can
// only leave through storage outside the call. The one case reading this clears it before the run.
const char * g_capturedSubcaseName = nullptr;

void captureSubcaseName(const toy::test::Context & context, const toy::test::FailureRecord & failure) noexcept {
  static_cast<void>(context);
  g_capturedSubcaseName = failure.subcaseName;
}

} // namespace

// A case without subcases runs once, because the loop is a do-while over a count that stays zero.
TEST_CASE("test/context/case_without_subcases_runs_once") {
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "some/case", &bodyWithoutSubcases);

  REQUIRE(context.passedCount() == 1);
  REQUIRE(context.subcaseCount() == 0);
}

// Three subcases produce three runs, and each run enters a different one, in declaration order. The repeated-run loop
// is spelled out here rather than driven through runCase(), so the name the guard admits on every run lands in storage
// local to the case; a run entering the wrong branch then names it instead of vanishing into a total.
TEST_CASE("test::detail/subcase_guard/each_subcase_runs_exactly_once") {
  toy::test::Context context{nullptr};
  context.beginCase("some/case");

  // One slot per run, holding the subcase that run entered. The empty string stands for a run that entered none, and
  // keeps a comparison below from dereferencing a null pointer.
  std::array<const char *, 4> entered{"", "", "", ""};
  std::size_t                 runCount = 0;

  do {
    context.beginRun(runCount);

    {
      const toy::test::detail::SubcaseGuard guard{context, "first"};
      if (guard.entered())
        entered[runCount] = context.subcaseName();
    }
    {
      const toy::test::detail::SubcaseGuard guard{context, "second"};
      if (guard.entered())
        entered[runCount] = context.subcaseName();
    }
    {
      const toy::test::detail::SubcaseGuard guard{context, "third"};
      if (guard.entered())
        entered[runCount] = context.subcaseName();
    }

    ++runCount;
  } while (runCount < context.subcaseCount() && runCount < entered.size());

  REQUIRE(runCount == 3);
  REQUIRE(context.subcaseCount() == 3);
  REQUIRE(toy::test::detail::compareNames(entered[0], "first") == 0);
  REQUIRE(toy::test::detail::compareNames(entered[1], "second") == 0);
  REQUIRE(toy::test::detail::compareNames(entered[2], "third") == 0);
}

// Entering a subcase from inside a subcase is diagnosed, and the inner one is not entered or counted.
TEST_CASE("test::detail/subcase_guard/nested_entry_is_diagnosed") {
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "some/case", &bodyWithNestedSubcase);

  REQUIRE(context.nestedSubcaseDetected() == true);
  REQUIRE(context.subcaseCount() == 1);
  REQUIRE(context.passedCount() == 2);
  REQUIRE(context.failedCount() == 0);
}

// The running subcase name reaches the failure record, so a report names the failing branch.
TEST_CASE("test::detail/subcase_guard/failure_carries_the_subcase_name") {
  g_capturedSubcaseName = nullptr;
  toy::test::Context context{&captureSubcaseName};

  toy::test::runCase(context, "some/case", &bodyFailingInsideSubcase);

  REQUIRE(g_capturedSubcaseName != nullptr);
  REQUIRE(toy::test::detail::compareNames(g_capturedSubcaseName, "the_subcase") == 0);
}

// Leaving a subcase clears the name, so a later failure outside one carries no stale branch.
TEST_CASE("test/context/subcase_name_clears_on_exit") {
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
TEST_CASE("test/context/verdict_survives_across_runs") {
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "some/case", &bodyFailingInTheFirstOfTwo);

  REQUIRE(context.subcaseCount() == 2);
  REQUIRE(context.caseFailed() == true);
  REQUIRE(context.failedCount() == 1);
  REQUIRE(context.passedCount() == 1);
}
