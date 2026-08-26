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
  \file   report.test.cpp
  \brief  Unit tests for the TOYTEST report: entry per case, failure blocks, plan line, summary and exit code.
*/

#include <cstddef>

#include <doctest/doctest.h>

#include "report.hpp"

namespace {

constexpr std::size_t c_captureCapacity = 1024;

// The report arrives here as raw bytes and is terminated, so a case can compare it against a literal in one call.
struct CapturedReport final {
  char        text[c_captureCapacity] = {};
  std::size_t length                  = 0;
};

// The capture lives in the case that reads it and arrives through the writer data, so no state outlives a case.
void captureWrite(const char * text, std::size_t length, void * writerData) noexcept {
  CapturedReport & captured = *static_cast<CapturedReport *>(writerData);

  for (std::size_t index = 0; index < length && captured.length + 1 < c_captureCapacity; ++index) {
    captured.text[captured.length] = text[index];
    ++captured.length;
  }
}

// Bodies use the internal macro names, because the short ones belong to doctest inside this binary.
void bodyPasses(toy::test::Context & toyTestContext) {
  TOY_TEST_CHECK(1 == 1);
}

// Recording straight through the context is what keeps the file and line in the expected report fixed; a macro would
// paste this file's own path and a line number that any edit above shifts.
void bodyFailsTwice(toy::test::Context & context) {
  static_cast<void>(context.record(false, "1 == 2", "first.cpp", 11));
  static_cast<void>(context.record(false, "2 == 3", "second.cpp", 22));
}

void bodyRunsThreeSubcases(toy::test::Context & toyTestContext) {
  TOY_TEST_SUBCASE("first") {
    TOY_TEST_CHECK(1 == 1);
  }
  TOY_TEST_SUBCASE("second") {
    TOY_TEST_CHECK(1 == 1);
  }
  TOY_TEST_SUBCASE("third") {
    TOY_TEST_CHECK(1 == 1);
  }
}

void bodyNestsSubcases(toy::test::Context & toyTestContext) {
  TOY_TEST_SUBCASE("outer") {
    TOY_TEST_SUBCASE("inner") {
      TOY_TEST_CHECK(1 == 1);
    }
  }
}

void bodyFailsInsideSubcase(toy::test::Context & toyTestContext) {
  TOY_TEST_SUBCASE("branch") {
    TOY_TEST_INFO("index", 7);
    static_cast<void>(toyTestContext.record(false, "values[index] == 0", "info.cpp", 33));
  }
}

} // namespace

// A run without a failure prints one entry per case, the plan line, the summary, and reports success.
TEST_CASE("test/write_report/passing_run_prints_an_entry_per_case") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar second{head, "sample/b/second", "b.cpp", 2, &bodyPasses};
  toy::test::CaseRegistrar first{head, "sample/a/first", "a.cpp", 1, &bodyPasses};

  CapturedReport captured;

  const int code = toy::test::writeReport(&captureWrite, &captured, head);

  REQUIRE(code == 0);
  REQUIRE(toy::test::detail::compareNames(captured.text, "TOYTEST 1\n"
                                                         "ok 1 - sample/a/first\n"
                                                         "ok 2 - sample/b/second\n"
                                                         "1..2\n"
                                                         "TOYTEST SUMMARY passed=2 failed=0\n")
          == 0);
}

// Two failures in one case share a single header and each print their own location block.
TEST_CASE("test/write_report/failures_share_one_case_header") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/fails", "a.cpp", 1, &bodyFailsTwice};

  CapturedReport captured;

  const int code = toy::test::writeReport(&captureWrite, &captured, head);

  REQUIRE(code == 1);
  REQUIRE(toy::test::detail::compareNames(captured.text, "TOYTEST 1\n"
                                                         "not ok 1 - sample/fails\n"
                                                         "  file: first.cpp\n"
                                                         "  line: 11\n"
                                                         "  expr: 1 == 2\n"
                                                         "  file: second.cpp\n"
                                                         "  line: 22\n"
                                                         "  expr: 2 == 3\n"
                                                         "1..1\n"
                                                         "TOYTEST SUMMARY passed=0 failed=2\n")
          == 0);
}

// Three branches give three runs of the body, so the summary counts three passing assertions under one entry.
TEST_CASE("test/write_report/subcases_run_once_each_under_one_entry") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/subcases", "a.cpp", 1, &bodyRunsThreeSubcases};

  CapturedReport captured;

  const int code = toy::test::writeReport(&captureWrite, &captured, head);

  REQUIRE(code == 0);
  REQUIRE(toy::test::detail::compareNames(captured.text, "TOYTEST 1\n"
                                                         "ok 1 - sample/subcases\n"
                                                         "1..1\n"
                                                         "TOYTEST SUMMARY passed=3 failed=0\n")
          == 0);
}

// A nested subcase condemns its case even though no assertion failed, and the run reports failure.
TEST_CASE("test/write_report/nested_subcase_condemns_the_case") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/nested", "a.cpp", 1, &bodyNestsSubcases};

  CapturedReport captured;

  const int code = toy::test::writeReport(&captureWrite, &captured, head);

  REQUIRE(code == 1);
  REQUIRE(toy::test::detail::compareNames(captured.text, "TOYTEST 1\n"
                                                         "not ok 1 - sample/nested\n"
                                                         "  error: nested subcase\n"
                                                         "1..1\n"
                                                         "TOYTEST SUMMARY passed=0 failed=0\n")
          == 0);
}

// The subcase and the info entries live at the moment of the failure, so both reach the block under it.
TEST_CASE("test/write_report/failure_block_carries_subcase_and_info") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/info", "a.cpp", 1, &bodyFailsInsideSubcase};

  CapturedReport captured;

  const int code = toy::test::writeReport(&captureWrite, &captured, head);

  REQUIRE(code == 1);
  REQUIRE(toy::test::detail::compareNames(captured.text, "TOYTEST 1\n"
                                                         "not ok 1 - sample/info\n"
                                                         "  file: info.cpp\n"
                                                         "  line: 33\n"
                                                         "  expr: values[index] == 0\n"
                                                         "  subcase: branch\n"
                                                         "  info: index: 7\n"
                                                         "1..1\n"
                                                         "TOYTEST SUMMARY passed=0 failed=1\n")
          == 0);
}

// A repeated case name aborts the run before any body executes and reports the reserved code.
TEST_CASE("test/write_report/duplicate_name_aborts_before_the_first_case") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar first{head, "sample/duplicate", "a.cpp", 1, &bodyPasses};
  toy::test::CaseRegistrar second{head, "sample/duplicate", "b.cpp", 2, &bodyPasses};

  CapturedReport captured;

  const int code = toy::test::writeReport(&captureWrite, &captured, head);

  REQUIRE(code == 2);
  REQUIRE(toy::test::detail::compareNames(captured.text, "TOYTEST 1\n"
                                                         "TOYTEST ERROR duplicate case name: sample/duplicate\n")
          == 0);
}

// An empty registry is a run of no cases, not an error.
TEST_CASE("test/write_report/empty_registry_reports_an_empty_plan") {
  CapturedReport captured;

  const int code = toy::test::writeReport(&captureWrite, &captured, nullptr);

  REQUIRE(code == 0);
  REQUIRE(toy::test::detail::compareNames(captured.text, "TOYTEST 1\n"
                                                         "1..0\n"
                                                         "TOYTEST SUMMARY passed=0 failed=0\n")
          == 0);
}
