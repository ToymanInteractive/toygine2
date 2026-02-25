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
    _report(_logLevelInfo, "===============================================================================");

    _report(_logLevelInfo, "[doctest] test cases: %u | %u passed | %u failed | %u skipped",
            stats.numTestCasesPassingFilters, stats.numTestCasesPassingFilters - stats.numTestCasesFailed,
            stats.numTestCasesFailed, stats.numTestCases - stats.numTestCasesPassingFilters);

    _report(_logLevelInfo, "[doctest] assertions: %i | %i passed | %i failed |", stats.numAsserts,
            stats.numAsserts - stats.numAssertsFailed, stats.numAssertsFailed);

    _report(_logLevelInfo, "[doctest] Status: %s", (stats.numTestCasesFailed > 0) ? "FAILURE!" : "SUCCESS!");
  }

  // Called when a test case is started (safe to cache a pointer to the input)
  void test_case_start(const doctest::TestCaseData & data) noexcept override {
    _hasLoggedCurrentTestStart = false;
    _testCaseData = &data;
    _subcasesStack.clear();
    _currentSubcaseLevel = 0;
  }

  void test_case_end([[maybe_unused]] const doctest::CurrentTestCaseStats & stats) noexcept override {}

  void test_case_exception([[maybe_unused]] const doctest::TestCaseException & exception) noexcept override {}

  void log_assert([[maybe_unused]] const doctest::AssertData & data) noexcept override {}

  void log_message([[maybe_unused]] const doctest::MessageData & data) noexcept override {}

  void test_case_skipped([[maybe_unused]] const doctest::TestCaseData & data) noexcept override {}

  void report_query([[maybe_unused]] const doctest::QueryData & data) noexcept override {}

  void subcase_start([[maybe_unused]] const doctest::SubcaseSignature & signature) noexcept override {}

  void subcase_end() noexcept override {}

  void test_case_reenter([[maybe_unused]] const doctest::TestCaseData & data) noexcept override {}

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
#if defined(__GBA__)
    // mGBA debug register
    (*(volatile uint16_t *)0x04FFF780) = 0xC0DE;

    return (*(volatile uint16_t *)0x04FFF780) == 0x1DEA;
#else
    return false;
#endif
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

#if defined(__GBA__)
    if (std::strlen(buffer) > 30) {
      // Truncate long strings for GBA on-screen log
      buffer[27] = '.';
      buffer[28] = '.';
      buffer[29] = '.';
      buffer[30] = '\0';
    }
#endif

    std::cout << buffer << "\n";

    va_end(args);
  }
};

#endif // TEST_GBA_REPORTER_HPP_
