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
  \brief  Unit tests for the TAP report: test points, subtests, YAML diagnostics, plan line, summary and exit code.
*/

#include <cstddef>
#include <cstring>
#include <string>

#include <doctest/doctest.h>

#include "report.hpp"

namespace {

constexpr std::size_t c_captureCapacity = 1024;

// The report arrives here as raw bytes and is terminated, so a case can compare it against a literal in one call.
struct CapturedReport final {
  char        text[c_captureCapacity] = {};
  std::size_t length                  = 0;
};

// The capture lives in the case that reads it and arrives through the writer data, so no state outlives a case. The
// seam carries the address of the capture rather than the capture itself: what it promises to leave alone is the
// pointer, while the buffer behind it is the destination this writer exists to fill.
void captureWrite(const char * text, std::size_t length, const void * writerData) noexcept {
  CapturedReport & captured = **static_cast<CapturedReport * const *>(writerData);

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

// A colon inside a plain YAML scalar starts a mapping and an apostrophe closes a quoted one, so both go through the
// writer that quotes.
void bodyFailsWithAwkwardText(toy::test::Context & context) {
  static_cast<void>(context.record(false, "map['a: b'] == c", "od'd.cpp", 7));
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

void bodyRunsOneSubcase(toy::test::Context & toyTestContext) {
  TOY_TEST_SUBCASE("only") {
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

// Compares exactly the bytes the writer handed over, then requires the expected text to end there too: a report that
// merely starts with the expected text is not the expected report. Comparing to the terminator instead would ignore
// the captured length and pass a report whose writer miscounted it.
[[nodiscard]] bool reportMatches(const CapturedReport & captured, const char * expected) noexcept {
  return std::strncmp(captured.text, expected, captured.length) == 0 && expected[captured.length] == '\0';
}

} // namespace

// Puts both texts into the failure message, because a comparison folded into a bool reports only that they differ,
// and the whole point of comparing the report in full is to see where it went wrong.
#define REQUIRE_REPORT(captured, expected)                                                                             \
  do {                                                                                                                 \
    INFO("actual:\n" << (captured).text);                                                                              \
    INFO("expected:\n" << (expected));                                                                                 \
    REQUIRE(reportMatches((captured), (expected)));                                                                    \
  } while (false)

// A run without a failure prints one test point per case, the plan line, the summary, and reports success.
TEST_CASE("test/write_report/passing_run_prints_an_entry_per_case") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar second{head, "sample/b/second", "b.cpp", 2, &bodyPasses};
  toy::test::CaseRegistrar first{head, "sample/a/first", "a.cpp", 1, &bodyPasses};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 0);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "ok 1 - sample/a/first\n"
                           "ok 2 - sample/b/second\n"
                           "1..2\n"
                           "# assertions passed=2 failed=0\n");
}

// Two failures in one case share a single test point, and the YAML block under it carries one list item each.
TEST_CASE("test/write_report/failures_share_one_case_header") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/fails", "a.cpp", 1, &bodyFailsTwice};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 1);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "not ok 1 - sample/fails\n"
                           "  ---\n"
                           "  severity: fail\n"
                           "  failures:\n"
                           "    - at:\n"
                           "        file: 'first.cpp'\n"
                           "        line: 11\n"
                           "      expr: '1 == 2'\n"
                           "    - at:\n"
                           "        file: 'second.cpp'\n"
                           "        line: 22\n"
                           "      expr: '2 == 3'\n"
                           "  ...\n"
                           "1..1\n"
                           "# assertions passed=0 failed=2\n");
}

// Three branches give three runs of the body, and each run is a test point of the subtest the case becomes.
TEST_CASE("test/write_report/subcases_print_a_subtest_per_branch") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/subcases", "a.cpp", 1, &bodyRunsThreeSubcases};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 0);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "# Subtest: sample/subcases\n"
                           "    ok 1 - first\n"
                           "    ok 2 - second\n"
                           "    ok 3 - third\n"
                           "    1..3\n"
                           "ok 1 - sample/subcases\n"
                           "1..1\n"
                           "# assertions passed=3 failed=0\n");
}

// A nested subcase condemns its case even though no assertion failed, and the run reports failure.
TEST_CASE("test/write_report/nested_subcase_condemns_the_case") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/nested", "a.cpp", 1, &bodyNestsSubcases};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 1);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "# Subtest: sample/nested\n"
                           "    ok 1 - outer\n"
                           "    1..1\n"
                           "not ok 1 - sample/nested\n"
                           "  ---\n"
                           "  severity: fail\n"
                           "  error: 'nested subcase'\n"
                           "  ...\n"
                           "1..1\n"
                           "# assertions passed=0 failed=0\n");
}

// The info entries live at the moment of the failure, so they reach the list item under the branch that failed.
TEST_CASE("test/write_report/failure_block_carries_info") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/info", "a.cpp", 1, &bodyFailsInsideSubcase};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 1);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "# Subtest: sample/info\n"
                           "    not ok 1 - branch\n"
                           "      ---\n"
                           "      severity: fail\n"
                           "      failures:\n"
                           "        - at:\n"
                           "            file: 'info.cpp'\n"
                           "            line: 33\n"
                           "          expr: 'values[index] == 0'\n"
                           "          info:\n"
                           "            - text: 'index'\n"
                           "              value: 7\n"
                           "      ...\n"
                           "    1..1\n"
                           "not ok 1 - sample/info\n"
                           "1..1\n"
                           "# assertions passed=0 failed=1\n");
}

// The bytes that would end a scalar early reach the block quoted: the apostrophe doubled, the colon left alone inside
// the quotes it can no longer escape.
TEST_CASE("test/write_report/failure_block_quotes_yaml_syntax") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/awkward", "a.cpp", 1, &bodyFailsWithAwkwardText};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 1);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "not ok 1 - sample/awkward\n"
                           "  ---\n"
                           "  severity: fail\n"
                           "  failures:\n"
                           "    - at:\n"
                           "        file: 'od''d.cpp'\n"
                           "        line: 7\n"
                           "      expr: 'map[''a: b''] == c'\n"
                           "  ...\n"
                           "1..1\n"
                           "# assertions passed=0 failed=1\n");
}

// A comment is never escaped and a description always is, so one name reaches the report in two spellings. A harness
// unescapes the description before matching it against the raw name, which is what keeps the pair correlated.
TEST_CASE("test/write_report/subtest_header_keeps_the_name_unescaped") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/hash#case", "a.cpp", 1, &bodyRunsOneSubcase};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 0);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "# Subtest: sample/hash#case\n"
                           "    ok 1 - only\n"
                           "    1..1\n"
                           "ok 1 - sample/hash\\#case\n"
                           "1..1\n"
                           "# assertions passed=1 failed=0\n");
}

// A repeated case name bails the run out before any body executes and reports the reserved code.
TEST_CASE("test/write_report/duplicate_name_aborts_before_the_first_case") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar first{head, "sample/duplicate", "a.cpp", 1, &bodyPasses};
  toy::test::CaseRegistrar second{head, "sample/duplicate", "b.cpp", 2, &bodyPasses};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 2);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "Bail out! duplicate case name: sample/duplicate\n");
}

// TAP reads an unescaped hash in a description as the start of a directive, so both it and the backslash that
// escapes it leave the writer escaped.
TEST_CASE("test/write_report/description_escapes_hash_and_backslash") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/hash#and\\back", "a.cpp", 1, &bodyPasses};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 0);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "ok 1 - sample/hash\\#and\\\\back\n"
                           "1..1\n"
                           "# assertions passed=1 failed=0\n");
}

// An empty registry is a run of no cases, not an error.
TEST_CASE("test/write_report/empty_registry_reports_an_empty_plan") {
  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, nullptr);

  REQUIRE(code == 0);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "1..0\n"
                           "# assertions passed=0 failed=0\n");
}

// A case name past the line capacity is cut to it and the line keeps its newline, because two report lines run
// together are unreadable while a truncated name costs only that name.
TEST_CASE("test/write_report/overlong_case_name_keeps_the_line_break") {
  constexpr std::size_t capacity   = toy::test::detail::ReportWriter::c_lineCapacity;
  constexpr std::size_t nameLength = capacity * 2;

  char name[nameLength + 1] = {};

  for (std::size_t index = 0; index < nameLength; ++index)
    name[index] = 'x';

  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, name, "a.cpp", 1, &bodyPasses};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  const std::size_t entryOffset = std::char_traits<char>::length("TAP version 14\n");

  REQUIRE(code == 0);
  REQUIRE(captured.text[entryOffset + capacity - 1] == '\n');
  REQUIRE(captured.text[entryOffset + capacity - 2] == 'x');

  // The plan line is what a missing terminator would swallow into the entry above it.
  REQUIRE(std::strncmp(captured.text + entryOffset + capacity, "1..1\n", 5) == 0);
}
