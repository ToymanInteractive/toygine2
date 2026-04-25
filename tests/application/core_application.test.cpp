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
  \file   core_application.test.cpp
  \brief  Unit tests for \ref toy::application::CoreApplication.
*/

#include <doctest/doctest.h>

#include "application.hpp"

namespace toy::application {

class TestApp : public CoreApplication {
public:
  explicit TestApp(bool runResult = true) noexcept
    : CoreApplication(nullptr, nullptr)
    , internalRunResult(runResult) {}

  bool runInternal() noexcept override;

  bool runInternalCalled{false};

  bool internalRunResult;
};

bool TestApp::runInternal() noexcept {
  runInternalCalled = true;

  return internalRunResult;
}

// CoreApplication compile-time type properties.
TEST_CASE("app/core_application/object_structure") {
  static_assert(!std::is_trivial_v<CoreApplication>,
                "CoreApplication must not be trivial (has non-trivial default init)");
  static_assert(!std::is_trivially_copyable_v<CoreApplication>, "CoreApplication must not be trivially copyable");
  static_assert(!std::is_standard_layout_v<CoreApplication>, "CoreApplication must not have standard layout");
}

// Singleton: instance() tracks the active CoreApplication lifetime.
TEST_CASE("app/core_application/singleton") {
  // instance() returns nullptr when no application exists.
  SUBCASE("no_instance_before_construction") {
    REQUIRE(CoreApplication::instance() == nullptr);
  }

  // instance() returns this during the application lifetime.
  SUBCASE("instance_returns_this_during_lifetime") {
    TestApp app;

    REQUIRE(CoreApplication::instance() == &app);
  }

  // instance() is nullptr after the application is destroyed.
  SUBCASE("cleared_after_destruction") {
    { TestApp app; }

    REQUIRE(CoreApplication::instance() == nullptr);
  }
}

// Version: version() and setVersion() manage the application version.
TEST_CASE("app/core_application/version") {
  TestApp app;

  // Default-constructed application holds a zero version.
  SUBCASE("default_is_zero") {
    REQUIRE(app.version() == Version{});
  }

  // setVersion stores the value and version() retrieves it.
  SUBCASE("set_and_get") {
    constexpr Version v{1, 2, 3, 4};

    app.setVersion(v);

    REQUIRE(app.version() == v);
  }

  // setVersion overwrites the previous version.
  SUBCASE("overwrite") {
    app.setVersion({1, 0, 0, 0});
    app.setVersion({2, 0, 0, 0});

    REQUIRE(app.version() == Version{2, 0, 0, 0});
  }
}

// Arguments: argumentsCount(), arguments(), argument(), and setArguments() behavior.
TEST_CASE("app/core_application/arguments") {
  TestApp app;

  // Default argument count is zero and arguments() is nullptr.
  SUBCASE("defaults") {
    REQUIRE(app.argumentsCount() == 0);
    REQUIRE(app.arguments() == nullptr);
  }

  // setArguments stores the count and the pointer.
  SUBCASE("set_stores_count_and_pointer") {
    char   arg0[] = "myapp";
    char   arg1[] = "--flag";
    char * argv[] = {arg0, arg1};

    app.setArguments(2, argv);

    REQUIRE(app.argumentsCount() == 2);
    REQUIRE(app.arguments() == argv);
  }

  // argument() returns the string at the given valid index.
  SUBCASE("argument_by_valid_index") {
    char   arg0[] = "myapp";
    char   arg1[] = "--verbose";
    char * argv[] = {arg0, arg1};

    app.setArguments(2, argv);

    REQUIRE(strcmp(app.argument(0), "myapp") == 0);
    REQUIRE(strcmp(app.argument(1), "--verbose") == 0);
  }

  // argument() returns nullptr when the argument vector is null.
  SUBCASE("argument_returns_nullptr_for_null_vector") {
    app.setArguments(1, nullptr);

    REQUIRE(app.argument(0) == nullptr);
  }

  // setArguments with argc == 0 leaves argumentsCount unchanged.
  SUBCASE("zero_argc_does_not_update_count") {
    char   arg0[] = "myapp";
    char * argv[] = {arg0};

    app.setArguments(0, argv);

    REQUIRE(app.argumentsCount() == 0);
    REQUIRE(app.arguments() == argv);
  }
}

// run(): stores arguments and delegates to runInternal().
TEST_CASE("app/core_application/run") {
  char   arg0[] = "myapp";
  char * argv[] = {arg0};

  // run() invokes runInternal().
  SUBCASE("calls_run_internal") {
    TestApp app;

    const bool result = app.run(1, argv);

    REQUIRE(app.runInternalCalled);
    REQUIRE(result);
  }

  // run() returns true when runInternal() returns true.
  SUBCASE("returns_run_internal_result_true") {
    TestApp app{true};

    REQUIRE(app.run(1, argv) == true);
  }

  // run() returns false when runInternal() returns false.
  SUBCASE("returns_run_internal_result_false") {
    TestApp app{false};

    REQUIRE(app.run(1, argv) == false);
  }

  // run() stores argc/argv before calling runInternal().
  SUBCASE("stores_arguments_before_run_internal") {
    TestApp app;

    const bool result = app.run(1, argv);

    REQUIRE(app.argumentsCount() == 1);
    REQUIRE(app.arguments() == argv);
    REQUIRE(result);
  }
}

// sleep(): completes without error in the stub implementation.
TEST_CASE("app/core_application/sleep") {
  TestApp app;

  app.sleep(0);
  app.sleep(1);
}

} // namespace toy::application
