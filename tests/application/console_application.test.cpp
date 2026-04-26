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
  \file   console_application.test.cpp
  \brief  Unit tests for \ref toy::application::ConsoleApplication.
*/

#include <doctest/doctest.h>

#include "application.hpp"

namespace toy::application {

class TestConsoleApp : public ConsoleApplication {
public:
  TestConsoleApp() noexcept = default;
  ~TestConsoleApp() noexcept override;

  TestConsoleApp(const TestConsoleApp &)             = delete;
  TestConsoleApp(TestConsoleApp &&)                  = delete;
  TestConsoleApp & operator=(const TestConsoleApp &) = delete;
  TestConsoleApp & operator=(TestConsoleApp &&)      = delete;

  bool runInternal() noexcept override;
};

TestConsoleApp::~TestConsoleApp() noexcept = default;

bool TestConsoleApp::runInternal() noexcept {
  return true;
}

// ConsoleApplication compile-time type properties.
TEST_CASE("app/console_application/object_structure") {
  static_assert(!std::is_trivial_v<ConsoleApplication>,
                "ConsoleApplication must not be trivial (has non-trivial default init)");
  static_assert(!std::is_trivially_copyable_v<ConsoleApplication>, "ConsoleApplication must not be trivially copyable");
  static_assert(!std::is_standard_layout_v<ConsoleApplication>, "ConsoleApplication must not have standard layout");

  static_assert(std::is_abstract_v<ConsoleApplication>,
                "ConsoleApplication must be abstract; runInternal() is pure virtual");
  static_assert(std::is_base_of_v<CoreApplication, ConsoleApplication>,
                "ConsoleApplication must derive from CoreApplication");
  static_assert(!std::is_copy_constructible_v<ConsoleApplication>,
                "ConsoleApplication copy construction must be deleted (singleton invariant)");
  static_assert(!std::is_move_constructible_v<ConsoleApplication>,
                "ConsoleApplication move construction must be deleted (singleton invariant)");
  static_assert(std::has_virtual_destructor_v<ConsoleApplication>, "ConsoleApplication must have a virtual destructor");
}

// Singleton: no-arg construction registers and clears the global instance.
TEST_CASE("app/console_application/singleton") {
  // instance() returns nullptr when no application exists.
  SUBCASE("no_instance_before_construction") {
    REQUIRE(CoreApplication::instance() == nullptr);
  }

  // instance() returns this during the application lifetime.
  SUBCASE("instance_returns_this_during_lifetime") {
    TestConsoleApp app;

    REQUIRE(CoreApplication::instance() == &app);
  }

  // instance() is nullptr after the application is destroyed.
  SUBCASE("cleared_after_destruction") {
    { TestConsoleApp app; }

    REQUIRE(CoreApplication::instance() == nullptr);
  }
}

} // namespace toy::application
