#include <doctest/doctest.h>

struct GBAReporter : public doctest::IReporter {
  GBAReporter([[maybe_unused]] const doctest::ContextOptions & in_options) {}

  void test_run_start() override {}

  void test_run_end([[maybe_unused]] const doctest::TestRunStats & stats) override {}

  void test_case_start([[maybe_unused]] const doctest::TestCaseData & data) override {}

  void test_case_end([[maybe_unused]] const doctest::CurrentTestCaseStats & stats) override {}

  void test_case_exception([[maybe_unused]] const doctest::TestCaseException & exception) override {}

  void log_assert([[maybe_unused]] const doctest::AssertData & data) override {}

  void log_message([[maybe_unused]] const doctest::MessageData & data) override {}

  void test_case_skipped([[maybe_unused]] const doctest::TestCaseData & data) override {}

  void report_query([[maybe_unused]] const doctest::QueryData & data) override {}
  void subcase_start([[maybe_unused]] const doctest::SubcaseSignature & signature) override {}
  void subcase_end() override {}
  void test_case_reenter([[maybe_unused]] const doctest::TestCaseData & data) override {}
};
