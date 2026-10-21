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
  \file   base_application.test.cpp
  \brief  Unit tests for \ref toy::application::BaseApplication.
*/

#include "application.hpp"
#include "toy_test.hpp"

namespace toy::application {

namespace {

// Static storage, so the revision outlives every application the cases build.
constexpr const char * c_revision = "f7e57d4";

// The build every case hands to an application.
constexpr Version c_build{26, 20, 0, c_revision};

// Smallest concrete application: the base is abstract and its constructor is protected.
class StubApplication final : public BaseApplication {
public:
  explicit constexpr StubApplication(const Version & version, bool result = true) noexcept
    : BaseApplication(version)
    , _result(result) {}

  // How many times run() reached runInternal().
  size_t runCount = 0;

  // Argument count runInternal() read on its last call.
  size_t argsCountInRun = 0;

private:
  bool runInternal() noexcept override {
    ++runCount;
    argsCountInRun = argsCount();

    return _result;
  }

  bool _result;
};

// Built at compile time, so the cases can read version() in a constant expression.
constexpr StubApplication c_application{c_build};

} // namespace

// The base exists only as part of a derived application that implements runInternal().
TEST_CASE("application/base_application/construction") {
  static_assert(std::is_abstract_v<BaseApplication>, "an application must implement runInternal() before it can exist");
  static_assert(!std::is_constructible_v<BaseApplication, const Version &>,
                "only a derived application may construct the base");
}

// What version() hands back, and that it reports the version passed at construction.
TEST_CASE("application/base_application/version") {
  const StubApplication application{c_build};

  CHECK(application.version() == c_build);
  CHECK(application.version().major == 26);
  CHECK(application.version().minor == 20);
  CHECK(application.version().patch == 0);

  static_assert(std::is_same_v<decltype(c_application.version()), const Version &>,
                "the accessor must return a read-only reference, not a copy");
  static_assert(noexcept(c_application.version()), "the accessor must be declared non-throwing");

  static_assert(c_application.version() == c_build, "a constexpr application must report its version");
}

// The application keeps its own copy of the version, while the revision characters stay with the caller.
TEST_CASE("application/base_application/version_ownership") {
  Version               source{c_build};
  const StubApplication application{source};

  source.patch = 1;

  CHECK(application.version().patch == 0);
  CHECK(application.version().revision.data() == c_revision);

  static_assert(c_application.version().revision.data() == c_revision,
                "the revision must view the caller's characters, not a copy");
}

// Before the first run(), the application holds no command-line arguments.
TEST_CASE("application/base_application/arguments_before_run") {
  StubApplication application{c_build};

  CHECK(application.argsCount() == 0);
  CHECK(application.args() == nullptr);

  static_assert(noexcept(application.argsCount()), "the argument count accessor must be declared non-throwing");
  static_assert(noexcept(application.args()), "the argument array accessor must be declared non-throwing");
}

// run() calls runInternal() once and returns its result.
TEST_CASE("application/base_application/run_result") {
  char   program[] = "toygine";
  char * argv[]    = {program, nullptr};

  StubApplication succeeding{c_build, true};
  StubApplication failing{c_build, false};

  CHECK(succeeding.run(1, argv));
  CHECK_FALSE(failing.run(1, argv));
  CHECK(succeeding.runCount == 1);
  CHECK(failing.runCount == 1);

  static_assert(noexcept(succeeding.run(1, argv)), "run() must be declared non-throwing");
}

// run() stores the arguments before runInternal() reads them and keeps the caller's array without copying it.
TEST_CASE("application/base_application/arguments") {
  char   program[] = "toygine";
  char   flag[]    = "--verbose";
  char * argv[]    = {program, flag, nullptr};

  StubApplication application{c_build};

  REQUIRE(application.run(2, argv));

  CHECK(application.argsCountInRun == 2);
  CHECK(application.argsCount() == 2);
  CHECK(application.args() == argv);
  CHECK(application.arg(0) == program);
  CHECK(application.arg(1) == flag);

  static_assert(noexcept(application.arg(0)), "the argument accessor must be declared non-throwing");
}

} // namespace toy::application
