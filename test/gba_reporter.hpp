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

#include <cstring>

#include <doctest/doctest.h>
#include <stdarg.h>

/*!
  \struct GBAReporter

  \brief No-op DocTest reporter implementing \c doctest::IReporter.

  Used when running tests on targets where console or file output is
  unavailable (e.g. GBA). All virtual methods are overridden with empty
  bodies; test execution still runs, but no output is produced.
*/
class GBAReporter : public doctest::IReporter {
  /// mGBA log level: fatal (0x100).
  static constexpr uint16_t _logLevelFatal = 0x100;
  /// mGBA log level: error (0x101).
  static constexpr uint16_t _logLevelError = 0x101;
  /// mGBA log level: warning (0x102).
  static constexpr uint16_t _logLevelWarn = 0x102;
  /// mGBA log level: info (0x103).
  static constexpr uint16_t _logLevelInfo = 0x103;
  /// mGBA log level: debug (0x104).
  static constexpr uint16_t _logLevelDebug = 0x104;

public:
  /*!
    \brief Constructs the reporter with the given DocTest context options.

    \param options DocTest context options (e.g. from the running test run).
  */
  explicit GBAReporter(const doctest::ContextOptions & options) noexcept
    : _options(options) {}

  /// Called when the whole test run starts.
  void test_run_start() override {
    _report(_logLevelInfo, "[doctest] doctest version is %s", DOCTEST_VERSION_STR);
  }

  /// Called when the whole test run ends (caching a pointer to the input doesn't make sense here)
  void test_run_end(const doctest::TestRunStats & stats) noexcept override {
    _separatorToStream();

    _report(_logLevelInfo, "[doctest] test cases: %u | %u passed | %u failed | %u skipped",
            stats.numTestCasesPassingFilters, stats.numTestCasesPassingFilters - stats.numTestCasesFailed,
            stats.numTestCasesFailed, stats.numTestCases - stats.numTestCasesPassingFilters);

    _report(_logLevelInfo, "[doctest] assertions: %i | %i passed | %i failed |", stats.numAsserts,
            stats.numAsserts - stats.numAssertsFailed, stats.numAssertsFailed);

    _report(_logLevelInfo, "[doctest] Status: %s", (stats.numTestCasesFailed > 0) ? "FAILURE!" : "SUCCESS!");
  }

  /// Called when a test case is started (safe to cache a pointer to the input)
  void test_case_start(const doctest::TestCaseData & data) noexcept override {
    _hasLoggedCurrentTestStart = false;
    _testCaseData = &data;
    _subcasesStack.clear();
    _currentSubcaseLevel = 0;
  }

  /// Called when a test case is reentered because of unfinished subcases (safe to cache a pointer to the input)
  void test_case_reenter([[maybe_unused]] const doctest::TestCaseData & data) noexcept override {
    _subcasesStack.clear();
  }

  /// Called when a test case has ended
  void test_case_end(const doctest::CurrentTestCaseStats & stats) noexcept override {
    if (_testCaseData->m_no_output)
      return;

    // log the preamble of the test case only if there is something else to print - something other than that an assert
    // has failed
    if (_options.duration || stats.failure_flags)
      _logTestStart();

    if (_options.duration)
      _report(_logLevelInfo, "%f s: %s", stats.seconds, _testCaseData->m_name);

    if (stats.failure_flags & doctest::TestCaseFailureReason::AssertFailure)
      _report(_logLevelInfo, "One or more assertions failed");

    if (stats.failure_flags & doctest::TestCaseFailureReason::Timeout)
      _report(_logLevelInfo, "Test case exceeded time limit of %f", _testCaseData->m_timeout);

    if (stats.failure_flags & doctest::TestCaseFailureReason::ShouldHaveFailedButDidnt) {
      _report(_logLevelInfo, "Should have failed but didn't! Marking it as failed!");
    } else if (stats.failure_flags & doctest::TestCaseFailureReason::ShouldHaveFailedAndDid) {
      _report(_logLevelInfo, "Failed as expected so marking it as not failed");
    } else if (stats.failure_flags & doctest::TestCaseFailureReason::CouldHaveFailedAndDid) {
      _report(_logLevelInfo, "Allowed to fail so marking it as not failed");
    } else if (stats.failure_flags & doctest::TestCaseFailureReason::DidntFailExactlyNumTimes) {
      _report(_logLevelInfo, "Didn't fail exactly %i times so marking it as failed!",
              _testCaseData->m_expected_failures);
    } else if (stats.failure_flags & doctest::TestCaseFailureReason::FailedExactlyNumTimes) {
      _report(_logLevelInfo, "Failed exactly %i times as expected so marking it as not failed!",
              _testCaseData->m_expected_failures);
    }

    if (stats.failure_flags & doctest::TestCaseFailureReason::TooManyFailedAsserts) {
      _report(_logLevelInfo, "Aborting - too many failed asserts!");
    }
  }

  // called when an exception is thrown from the test case (or it crashes)
  void test_case_exception(const doctest::TestCaseException & exception) noexcept override {
    if (_testCaseData->m_no_output)
      return;

    _logTestStart();
    _fileLineToStream(_testCaseData->m_file.c_str(), _testCaseData->m_line, " ");

    _report(_logLevelInfo, "%s: %s: %s",
            _successOrFailString(false,
                                 exception.is_crash ? doctest::assertType::is_require : doctest::assertType::is_check),
            exception.is_crash ? "test case CRASHED" : "test case THREW exception", exception.error_string.c_str());
  }

  void log_assert([[maybe_unused]] const doctest::AssertData & data) noexcept override {}

  void log_message([[maybe_unused]] const doctest::MessageData & data) noexcept override {}

  void test_case_skipped([[maybe_unused]] const doctest::TestCaseData & data) noexcept override {}

  void report_query([[maybe_unused]] const doctest::QueryData & data) noexcept override {}

  void subcase_start([[maybe_unused]] const doctest::SubcaseSignature & signature) noexcept override {}

  void subcase_end() noexcept override {}

private:
  GBAReporter(const GBAReporter &) = delete;
  GBAReporter(GBAReporter &&) = delete;

  GBAReporter & operator=(const GBAReporter &) = delete;
  GBAReporter & operator=(GBAReporter &&) = delete;

  /// DocTest context options passed at construction.
  const doctest::ContextOptions & _options;
  /// Pointer to the currently running test case data (set in test_case_start; valid until test_case_end).
  const doctest::TestCaseData * _testCaseData = nullptr;

  /// \c true once the start of the current test case has been logged (avoids duplicate run-start messages).
  bool _hasLoggedCurrentTestStart = false;
  /// Stack of active subcase signatures for the current test case.
  std::vector<doctest::SubcaseSignature> _subcasesStack{};
  /// Current subcase nesting level (0 = test case root).
  size_t _currentSubcaseLevel = 0U;

  /*!
    \brief Detects whether the code is running under the mGBA emulator.

    Writes a magic value to the mGBA debug register and reads it back; mGBA
    rewrites it to a known response. On non-GBA builds always returns \c false.

    \return \c true if mGBA debug protocol is present, \c false otherwise.
  */
  static inline bool _mGbaDetect() noexcept {
    // mGBA debug register
    (*(volatile uint16_t *)0x04FFF780) = 0xC0DE;

    return (*(volatile uint16_t *)0x04FFF780) == 0x1DEA;
  }

  /*!
    \brief Sends a formatted message to mGBA (if present) and to stdout.

    Uses mGBA log level and string registers when \c _mGbaDetect() is true.
    On GBA, long strings are truncated to 30 characters for display.

    \param level mGBA log level (e.g. \c _logLevelInfo).
    \param fmt \c printf-style format string.
    \param ... Format arguments.
  */
  static inline void _report(uint16_t level, const char * fmt, ...) noexcept {
    va_list args;
    va_start(args, fmt);

    char buffer[256];

    vsnprintf(buffer, sizeof(buffer), fmt, args);

    if (_mGbaDetect()) {
      (*(volatile uint16_t *)0x04FFF700) = level; // MGBA Log Level Register

      char * dst = ((char *)0x04FFF600); // MGBA Log String Register
      const char * src = buffer;
      while (*src) {
        *dst++ = *src++;
      }
      *dst = 0;
    }

    if (std::strlen(buffer) > 30) {
      // Truncate long strings for GBA on-screen log
      buffer[27] = '.';
      buffer[28] = '.';
      buffer[29] = '.';
      buffer[30] = '\0';
    }

    std::cout << buffer << "\n";

    va_end(args);
  }

  /// Logs a separator line.
  void _separatorToStream() {
    _report(_logLevelInfo, "===============================================================================");
  }

  /// Logs file and line (and optional tail) according to context options (GNU vs MSVC style).
  void _fileLineToStream(const char * file, int line, const char * tail = "") {
    _report(_logLevelInfo, "%s%s%d%s%s", file, _options.gnu_file_line ? ":" : "(", _options.no_line_numbers ? 0 : line,
            _options.gnu_file_line ? ":" : "):", tail);
  }

  /// Logs the current test case and subcase stack once per test (idempotent).
  void _logTestStart() {
    if (_hasLoggedCurrentTestStart)
      return;

    _separatorToStream();
    _fileLineToStream(_testCaseData->m_file.c_str(), _testCaseData->m_line);
    if (_testCaseData->m_description)
      _report(_logLevelInfo, "DESCRIPTION: %s", _testCaseData->m_description);
    if (_testCaseData->m_test_suite && _testCaseData->m_test_suite[0] != '\0')
      _report(_logLevelInfo, "TEST SUITE: %s", _testCaseData->m_test_suite);

    _report(_logLevelInfo, "%s%s", std::strncmp(_testCaseData->m_name, "  Scenario:", 11) != 0 ? "TEST CASE:  " : "",
            _testCaseData->m_name);

    for (size_t i = 0; i < _currentSubcaseLevel; ++i) {
      if (_subcasesStack[i].m_name[0] != '\0')
        _report(_logLevelInfo, "%s", _subcasesStack[i].m_name);
    }

    if (_currentSubcaseLevel != _subcasesStack.size()) {
      _report(_logLevelInfo, "\nDEEPEST SUBCASE STACK REACHED (DIFFERENT FROM THE CURRENT ONE):\n");
      for (size_t i = 0; i < _subcasesStack.size(); ++i) {
        if (_subcasesStack[i].m_name[0] != '\0')
          _report(_logLevelInfo, "%s", _subcasesStack[i].m_name);
      }
    }

    _report(_logLevelInfo, "");

    _hasLoggedCurrentTestStart = true;
  }

  /// Returns a label for the assertion result: success string, "WARNING", "ERROR", "FATAL ERROR", or empty.
  [[nodiscard]] const char * _successOrFailString(bool success, doctest::assertType::Enum at,
                                                  const char * successString = "SUCCESS") noexcept {
    if (success)
      return successString;

    if (at & doctest::assertType::is_warn)
      return "WARNING";
    if (at & doctest::assertType::is_check)
      return "ERROR";
    if (at & doctest::assertType::is_require)
      return "FATAL ERROR";

    return "";
  }
};

#endif // TEST_GBA_REPORTER_HPP_
