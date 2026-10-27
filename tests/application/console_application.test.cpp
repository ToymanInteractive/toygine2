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
  \file   console_application.test.cpp
  \brief  Unit tests for \ref toy::application::ConsoleApplication.
*/

#include "application.hpp"
#include "toy_test.hpp"

namespace toy::application {

namespace {

// Static storage, so the revision outlives every application the cases build.
constexpr const char * revision = "f7e57d4";

// The build every case hands to an application.
constexpr Version build{26, 20, 0, revision};

// Smallest concrete console application: the class is abstract and its constructor is protected.
class StubConsoleApplication final : public ConsoleApplication {
public:
  explicit constexpr StubConsoleApplication(const Version & version) noexcept
    : ConsoleApplication(version) {}

private:
  bool runInternal() noexcept override {
    return true;
  }
};

// Built at compile time, so the cases can read version() in a constant expression.
constexpr StubConsoleApplication compileTimeApplication{build};

} // namespace

// The class exists only as part of a derived application, and it passes the version on to the base.
TEST_CASE("application/console_application/construction") {
  const StubConsoleApplication application{build};

  CHECK(application.version() == build);

  static_assert(std::is_base_of_v<BaseApplication, ConsoleApplication>,
                "a console application must provide the base application services");
  static_assert(std::is_abstract_v<ConsoleApplication>,
                "a console application must implement runInternal() before it can exist");
  static_assert(!std::is_constructible_v<ConsoleApplication, const Version &>,
                "only a derived application may construct a console application");
  static_assert(!std::is_destructible_v<ConsoleApplication>,
                "only a derived application may destroy a console application");

  static_assert(compileTimeApplication.version() == build, "a constexpr console application must report its version");
}

// Every write call is static and non-throwing, and takes an owning string as well as a view.
TEST_CASE("application/console_application/write_calls") {
  static_assert(noexcept(ConsoleApplication::writeOutput(StringView{})), "writeOutput() must be declared non-throwing");
  static_assert(noexcept(ConsoleApplication::writeOutputLine(StringView{})),
                "writeOutputLine() must be declared non-throwing");
  static_assert(noexcept(ConsoleApplication::writeError(StringView{})), "writeError() must be declared non-throwing");
  static_assert(noexcept(ConsoleApplication::writeErrorLine(StringView{})),
                "writeErrorLine() must be declared non-throwing");

  static_assert(
    requires(const FixedString<16> & text) { ConsoleApplication::writeOutput(text); },
    "the write calls must accept an owning string as well as a view"
  );
}

} // namespace toy::application
