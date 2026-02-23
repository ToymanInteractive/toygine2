//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \file   gba_reporter.hpp
  \brief  Minimal DocTest reporter for Nintendo GBA.
*/

#ifndef TEST_GBA_REPORTER_HPP_
#define TEST_GBA_REPORTER_HPP_

#include <doctest/doctest.h>

/*!
  \struct GBAReporter

  \brief No-op DocTest reporter implementing \c doctest::IReporter.

  Used when running tests on targets where console or file output is unavailable (e.g. GBA). All virtual methods are
  overridden with empty bodies; test execution still runs, but no output is produced.
*/
struct GBAReporter : public doctest::IReporter {
  explicit GBAReporter([[maybe_unused]] const doctest::ContextOptions & in_options) noexcept {}

  GBAReporter(const GBAReporter &) = delete;
  GBAReporter(GBAReporter &&) = delete;

  GBAReporter & operator=(const GBAReporter &) = delete;
  GBAReporter & operator=(GBAReporter &&) = delete;

  void test_run_start() override {}

  void test_run_end([[maybe_unused]] const doctest::TestRunStats & stats) noexcept override {}

  void test_case_start([[maybe_unused]] const doctest::TestCaseData & data) noexcept override {}

  void test_case_end([[maybe_unused]] const doctest::CurrentTestCaseStats & stats) noexcept override {}

  void test_case_exception([[maybe_unused]] const doctest::TestCaseException & exception) noexcept override {}

  void log_assert([[maybe_unused]] const doctest::AssertData & data) noexcept override {}

  void log_message([[maybe_unused]] const doctest::MessageData & data) noexcept override {}

  void test_case_skipped([[maybe_unused]] const doctest::TestCaseData & data) noexcept override {}

  void report_query([[maybe_unused]] const doctest::QueryData & data) noexcept override {}

  void subcase_start([[maybe_unused]] const doctest::SubcaseSignature & signature) noexcept override {}

  void subcase_end() noexcept override {}

  void test_case_reenter([[maybe_unused]] const doctest::TestCaseData & data) noexcept override {}
};

#endif // TEST_GBA_REPORTER_HPP_
