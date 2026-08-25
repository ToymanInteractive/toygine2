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
  \file   macros.test.cpp
  \brief  Unit tests for the built-in runner's case, subcase, assertion and info macros.
*/

#include <array>
#include <cstddef>

#include <doctest/doctest.h>

#include "toy_test.hpp"

namespace {

// Bodies use the internal macro names, because the short ones belong to doctest inside this binary.
//
// The passing assertion is the witness: it is recorded only if the body reaches the line after the failing one.
void bodyWithFailingRequire(toy::test::Context & toyTestContext) {
  TOY_TEST_REQUIRE(1 == 2);
  TOY_TEST_CHECK(1 == 1);
}

void bodyWithFailingCheck(toy::test::Context & toyTestContext) {
  TOY_TEST_CHECK(1 == 2);
  TOY_TEST_CHECK(1 == 1);
}

void bodyWithInfo(toy::test::Context & toyTestContext) {
  TOY_TEST_INFO("index", 7);
  TOY_TEST_CHECK(1 == 2);
}

void bodyWithNegations(toy::test::Context & toyTestContext) {
  TOY_TEST_REQUIRE_FALSE(1 == 2);
  TOY_TEST_CHECK_FALSE(1 == 2);
  TOY_TEST_REQUIRE(1.0 == toy::test::Approx(1.0));
}

// The first info entry visible at the moment of the failure, written into storage the reading case owns.
struct CapturedInfo final {
  const char * text  = nullptr;
  long long    value = 0;
};

void captureInfo(const toy::test::Context & context, const toy::test::FailureRecord & failure,
                 void * reporterData) noexcept {
  static_cast<void>(failure);

  if (context.infoCount() > 0) {
    CapturedInfo & captured = *static_cast<CapturedInfo *>(reporterData);

    captured.text  = context.infoAt(0).text;
    captured.value = context.infoAt(0).value;
  }
}

} // namespace

// The expected line is captured directly above the declaration, so an edit to the body cannot shift it.
constexpr int c_registeredCaseLine = __LINE__ + 3;

// A case declared through the macro lands in the registry with its name and source location.
TOY_TEST_CASE("generated/registered/through_macro") {
  TOY_TEST_CHECK(1 == 1);
}

// The registry holds the case the macro above declared.
TEST_CASE("test/macros/case_macro_registers_the_case") {
  const toy::test::CaseRegistrar * registered = nullptr;

  for (const toy::test::CaseRegistrar * node = toy::test::detail::caseListHead; node != nullptr; node = node->next())
    if (toy::test::detail::compareNames(node->name(), "generated/registered/through_macro") == 0)
      registered = node;

  REQUIRE(registered != nullptr);
  CHECK(toy::test::detail::compareNames(registered->file(), __FILE__) == 0);
  CHECK(registered->line() == c_registeredCaseLine);
}

// The subcase macro produces one run per branch, and each branch executes once. The run loop is spelled out here
// rather than delegated to toy::test::runCase(), because a body it accepts is a bare function pointer and could
// report which branch ran only through state outside the case.
TEST_CASE("test/macros/subcase_expands_to_one_run_per_branch") {
  // The macros reach the run state through a name they choose themselves, so the case declares the context under it.
  toy::test::Context toyTestContext{nullptr};
  toyTestContext.beginCase("generated/case");

  // One more run than the case declares subcases, so a loop that fails to terminate is a failed expectation below
  // rather than a hung test.
  constexpr std::size_t c_maxRuns = 4;

  std::array<std::size_t, 3> hits{};
  std::size_t                runCount = 0;

  do {
    toyTestContext.beginRun(runCount);

    TOY_TEST_SUBCASE("one") {
      ++hits[0];
    }
    TOY_TEST_SUBCASE("two") {
      ++hits[1];
    }
    TOY_TEST_SUBCASE("three") {
      ++hits[2];
    }

    ++runCount;
  } while (runCount < toyTestContext.subcaseCount() && runCount < c_maxRuns);

  REQUIRE(runCount == 3);
  REQUIRE(toyTestContext.subcaseCount() == 3);
  REQUIRE(hits[0] == 1);
  REQUIRE(hits[1] == 1);
  REQUIRE(hits[2] == 1);
}

// A failed require returns from the body, so the assertion after it is never recorded.
TEST_CASE("test/macros/failing_require_stops_the_body") {
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "generated/case", &bodyWithFailingRequire);

  REQUIRE(context.failedCount() == 1);
  REQUIRE(context.passedCount() == 0);
}

// A failed check records the failure and lets the body continue to the assertion after it.
TEST_CASE("test/macros/failing_check_continues_the_body") {
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "generated/case", &bodyWithFailingCheck);

  REQUIRE(context.failedCount() == 1);
  REQUIRE(context.passedCount() == 1);
}

// An info message reaches the reporter and is gone once the scope ends.
TEST_CASE("test/macros/info_is_visible_during_failure_only") {
  CapturedInfo       captured;
  toy::test::Context context{&captureInfo, &captured};

  toy::test::runCase(context, "generated/case", &bodyWithInfo);

  REQUIRE(captured.text != nullptr);
  REQUIRE(toy::test::detail::compareNames(captured.text, "index") == 0);
  REQUIRE(captured.value == 7);
  REQUIRE(context.infoCount() == 0);
}

// The negated forms and Approx pass through the macro layer.
TEST_CASE("test/macros/negated_forms_and_approx") {
  toy::test::Context context{nullptr};

  toy::test::runCase(context, "generated/case", &bodyWithNegations);

  REQUIRE(context.failedCount() == 0);
  REQUIRE(context.passedCount() == 3);
}
