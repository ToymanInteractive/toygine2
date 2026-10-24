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

// The capture lives in the case that reads it; the seam carries its address, the buffer behind it is the target.
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

// Recording through the context fixes the file and line; a macro would paste a path and a shifting line number.
void bodyFailsTwice(toy::test::Context & context) {
  static_cast<void>(context.record(false, "1 == 2", "first.cpp", 11));
  static_cast<void>(context.record(false, "2 == 3", "second.cpp", 22));
}

// A colon starts a YAML mapping and an apostrophe closes a quoted scalar, so both go through the quoting writer.
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

// Only the middle branch fails, so the branches around it must keep their own verdicts.
void bodyFailsInMiddleSubcase(toy::test::Context & toyTestContext) {
  TOY_TEST_SUBCASE("first") {
    static_cast<void>(toyTestContext.record(true, "1 == 1", "middle.cpp", 3));
  }
  TOY_TEST_SUBCASE("second") {
    static_cast<void>(toyTestContext.record(false, "2 == 3", "middle.cpp", 6));
  }
  TOY_TEST_SUBCASE("third") {
    static_cast<void>(toyTestContext.record(true, "3 == 3", "middle.cpp", 9));
  }
}

// The failure sits before the subcases, so every run of the body repeats it.
void bodyFailsInSharedPrefix(toy::test::Context & toyTestContext) {
  static_cast<void>(toyTestContext.record(false, "setup()", "prefix.cpp", 2));

  TOY_TEST_SUBCASE("a") {}
  TOY_TEST_SUBCASE("b") {}
}

// Fails inside the outer subcase and then nests, so the report carries both diagnostics.
void bodyFailsAndNestsSubcases(toy::test::Context & toyTestContext) {
  TOY_TEST_SUBCASE("outer") {
    static_cast<void>(toyTestContext.record(false, "1 == 2", "nest.cpp", 4));

    TOY_TEST_SUBCASE("inner") {}
  }
}

static_assert(toy::test::Context::maxSubcaseNames == 16, "the body below declares one branch past the kept names");

// The seventeenth branch has no kept name, so only the failure block can say which subcase failed.
void bodyFailsPastKeptSubcaseNames(toy::test::Context & toyTestContext) {
  TOY_TEST_SUBCASE("b01") {}
  TOY_TEST_SUBCASE("b02") {}
  TOY_TEST_SUBCASE("b03") {}
  TOY_TEST_SUBCASE("b04") {}
  TOY_TEST_SUBCASE("b05") {}
  TOY_TEST_SUBCASE("b06") {}
  TOY_TEST_SUBCASE("b07") {}
  TOY_TEST_SUBCASE("b08") {}
  TOY_TEST_SUBCASE("b09") {}
  TOY_TEST_SUBCASE("b10") {}
  TOY_TEST_SUBCASE("b11") {}
  TOY_TEST_SUBCASE("b12") {}
  TOY_TEST_SUBCASE("b13") {}
  TOY_TEST_SUBCASE("b14") {}
  TOY_TEST_SUBCASE("b15") {}
  TOY_TEST_SUBCASE("b16") {}
  TOY_TEST_SUBCASE("b17") {
    static_cast<void>(toyTestContext.record(false, "1 == 2", "many.cpp", 17));
  }
}

static_assert(toy::test::Context::maxInfoDepth == 8, "the body below pushes two entries past the info depth");

// Ten nested info entries, so the failure block keeps the outer eight and counts the two it lost.
void bodyFailsPastTheInfoDepth(toy::test::Context & toyTestContext) {
  TOY_TEST_INFO("i1");
  TOY_TEST_INFO("i2");
  TOY_TEST_INFO("i3");
  TOY_TEST_INFO("i4");
  TOY_TEST_INFO("i5");
  TOY_TEST_INFO("i6");
  TOY_TEST_INFO("i7");
  TOY_TEST_INFO("i8");
  TOY_TEST_INFO("i9");
  TOY_TEST_INFO("i10");
  static_cast<void>(toyTestContext.record(false, "1 == 2", "deep.cpp", 10));
}

// Compares the bytes handed over and requires the expected text to end there: a prefix is not the report.
// Comparing to the terminator would ignore the captured length and pass a report whose writer miscounted it.
[[nodiscard]] bool reportMatches(const CapturedReport & captured, const char * expected) noexcept {
  return std::strncmp(captured.text, expected, captured.length) == 0 && expected[captured.length] == '\0';
}

} // namespace

// Both texts reach the failure message: a folded bool would report only that they differ, never where.
// One INFO per expansion: doctest names captures by __LINE__ on Clang 22 and newer, so a second INFO collides.
#define REQUIRE_REPORT(captured, expected)                                                                             \
  do {                                                                                                                 \
    INFO("actual:\n" << (captured).text << "\nexpected:\n" << (expected));                                             \
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

// A failing branch between passing ones fails alone, and its failure condemns the case.
TEST_CASE("test/write_report/failing_middle_branch_keeps_its_neighbours_ok") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/middle", "a.cpp", 1, &bodyFailsInMiddleSubcase};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 1);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "# Subtest: sample/middle\n"
                           "    ok 1 - first\n"
                           "    not ok 2 - second\n"
                           "      ---\n"
                           "      severity: fail\n"
                           "      failures:\n"
                           "        - at:\n"
                           "            file: 'middle.cpp'\n"
                           "            line: 6\n"
                           "          expr: '2 == 3'\n"
                           "          subcase: 'second'\n"
                           "      ...\n"
                           "    ok 3 - third\n"
                           "    1..3\n"
                           "not ok 1 - sample/middle\n"
                           "1..1\n"
                           "# assertions passed=2 failed=1\n");
}

// Code before the subcases runs on every branch, so its failure is reported under each one and names no subcase.
TEST_CASE("test/write_report/shared_prefix_failure_repeats_under_every_branch") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/prefix", "a.cpp", 1, &bodyFailsInSharedPrefix};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 1);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "# Subtest: sample/prefix\n"
                           "    not ok 1 - a\n"
                           "      ---\n"
                           "      severity: fail\n"
                           "      failures:\n"
                           "        - at:\n"
                           "            file: 'prefix.cpp'\n"
                           "            line: 2\n"
                           "          expr: 'setup()'\n"
                           "      ...\n"
                           "    not ok 2 - b\n"
                           "      ---\n"
                           "      severity: fail\n"
                           "      failures:\n"
                           "        - at:\n"
                           "            file: 'prefix.cpp'\n"
                           "            line: 2\n"
                           "          expr: 'setup()'\n"
                           "      ...\n"
                           "    1..2\n"
                           "not ok 1 - sample/prefix\n"
                           "1..1\n"
                           "# assertions passed=0 failed=2\n");
}

// A failure inside a branch and a nested subcase each keep their own diagnostic, under the branch and the case.
TEST_CASE("test/write_report/nested_subcase_and_failure_both_reach_the_report") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/nest", "a.cpp", 1, &bodyFailsAndNestsSubcases};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 1);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "# Subtest: sample/nest\n"
                           "    not ok 1 - outer\n"
                           "      ---\n"
                           "      severity: fail\n"
                           "      failures:\n"
                           "        - at:\n"
                           "            file: 'nest.cpp'\n"
                           "            line: 4\n"
                           "          expr: '1 == 2'\n"
                           "          subcase: 'outer'\n"
                           "      ...\n"
                           "    1..1\n"
                           "not ok 1 - sample/nest\n"
                           "  ---\n"
                           "  severity: fail\n"
                           "  error: 'nested subcase'\n"
                           "  outer: 'outer'\n"
                           "  inner: 'inner'\n"
                           "  ...\n"
                           "1..1\n"
                           "# assertions passed=0 failed=1\n");
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
                           "  outer: 'outer'\n"
                           "  inner: 'inner'\n"
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
                           "          subcase: 'branch'\n"
                           "          info:\n"
                           "            - text: 'index'\n"
                           "              value: 7\n"
                           "      ...\n"
                           "    1..1\n"
                           "not ok 1 - sample/info\n"
                           "1..1\n"
                           "# assertions passed=0 failed=1\n");
}

// Entries past the info depth are lost, so the failure block reports how many.
TEST_CASE("test/write_report/failure_block_counts_dropped_info") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/deep", "a.cpp", 1, &bodyFailsPastTheInfoDepth};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 1);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "not ok 1 - sample/deep\n"
                           "  ---\n"
                           "  severity: fail\n"
                           "  failures:\n"
                           "    - at:\n"
                           "        file: 'deep.cpp'\n"
                           "        line: 10\n"
                           "      expr: '1 == 2'\n"
                           "      info:\n"
                           "        - text: 'i1'\n"
                           "        - text: 'i2'\n"
                           "        - text: 'i3'\n"
                           "        - text: 'i4'\n"
                           "        - text: 'i5'\n"
                           "        - text: 'i6'\n"
                           "        - text: 'i7'\n"
                           "        - text: 'i8'\n"
                           "      info_dropped: 2\n"
                           "  ...\n"
                           "1..1\n"
                           "# assertions passed=0 failed=1\n");
}

// A branch past the kept names prints the case name as its description, and the failure block still names the subcase.
TEST_CASE("test/write_report/failure_block_names_a_subcase_past_the_kept_names") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "sample/many", "a.cpp", 1, &bodyFailsPastKeptSubcaseNames};

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  const int code = toy::test::writeReport(&captureWrite, &destination, head);

  REQUIRE(code == 1);
  REQUIRE_REPORT(captured, "TAP version 14\n"
                           "# Subtest: sample/many\n"
                           "    ok 1 - b01\n"
                           "    ok 2 - b02\n"
                           "    ok 3 - b03\n"
                           "    ok 4 - b04\n"
                           "    ok 5 - b05\n"
                           "    ok 6 - b06\n"
                           "    ok 7 - b07\n"
                           "    ok 8 - b08\n"
                           "    ok 9 - b09\n"
                           "    ok 10 - b10\n"
                           "    ok 11 - b11\n"
                           "    ok 12 - b12\n"
                           "    ok 13 - b13\n"
                           "    ok 14 - b14\n"
                           "    ok 15 - b15\n"
                           "    ok 16 - b16\n"
                           "    not ok 17 - sample/many\n"
                           "      ---\n"
                           "      severity: fail\n"
                           "      failures:\n"
                           "        - at:\n"
                           "            file: 'many.cpp'\n"
                           "            line: 17\n"
                           "          expr: '1 == 2'\n"
                           "          subcase: 'b17'\n"
                           "      ...\n"
                           "    1..17\n"
                           "not ok 1 - sample/many\n"
                           "1..1\n"
                           "# assertions passed=0 failed=1\n");
}

// Bytes that would end a scalar early arrive quoted: the apostrophe doubled, the colon left as it is.
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

// A comment is never escaped and a description always is, so one name reaches the report in two spellings. A
// harness unescapes the description before matching it against the raw name, which keeps the pair correlated.
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

// TAP reads a bare hash in a description as a directive, so it and its backslash both leave the writer escaped.
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

// An over-long case name is cut and the line keeps its newline: two report lines run together are unreadable.
TEST_CASE("test/write_report/overlong_case_name_keeps_the_line_break") {
  constexpr std::size_t capacity   = toy::test::detail::ReportWriter::lineCapacity;
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

// A scalar quoted near the end of a line is cut but still closes, and the line keeps its break.
TEST_CASE("test::detail/report_writer/quoted_scalar_cut_at_the_capacity_still_closes") {
  constexpr std::size_t capacity     = toy::test::detail::ReportWriter::lineCapacity;
  constexpr std::size_t fillerLength = capacity - 4;

  char filler[fillerLength + 1] = {};

  for (std::size_t index = 0; index < fillerLength; ++index)
    filler[index] = 'x';

  CapturedReport   captured;
  CapturedReport * destination = &captured;

  toy::test::detail::ReportWriter writer{&captureWrite, &destination};

  writer.addText(filler);
  writer.addQuoted("abcdef");
  writer.flush();

  // Three bytes are left before the break: the opening quote, one character and the closing quote.
  REQUIRE(captured.length == capacity);
  REQUIRE(std::strncmp(captured.text + fillerLength, "'a'\n", 4) == 0);
}
