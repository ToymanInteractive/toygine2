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

#include <catch2/catch_test_macros.hpp>

#include "core.hpp"

namespace toy {

TEST_CASE("FormatString constructor", "[core][format]") {
  SECTION("String without placeholders") {
    constexpr FormatString<> format("Hello World");

    REQUIRE(format.get() == "Hello World");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "Hello World");
  }

  SECTION("Single placeholder") {
    constexpr FormatString<int> format("Value: {}");

    REQUIRE(format.get() == "Value: {}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "Value: {}");
  }

  SECTION("Multiple placeholders") {
    constexpr FormatString<int, float, double> format("{}, {}, {}");

    REQUIRE(format.get() == "{}, {}, {}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "{}, {}, {}");
  }

  SECTION("Escaped braces") {
    constexpr FormatString<> format("{{}}");

    REQUIRE(format.get() == "{{}}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "{{}}");
  }

  SECTION("Mixed placeholders and escaped braces") {
    constexpr FormatString<int, float> format("{{{} and {} and }}");

    REQUIRE(format.get() == "{{{} and {} and }}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "{{{} and {} and }}");
  }

  SECTION("Adjacent placeholders") {
    constexpr FormatString<int, int> format("{}{}");

    REQUIRE(format.get() == "{}{}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "{}{}");
  }
}

TEST_CASE("FormatString get method", "[core][format]") {
  SECTION("Returns stored string") {
    constexpr FormatString<> format("Test");

    constexpr const auto result = format.get();

    REQUIRE(result == "Test");

    // Compile-time checks
    STATIC_REQUIRE(result == "Test");
  }

  SECTION("Returns copy of format string") {
    constexpr FormatString<> format("Reference");

    constexpr const auto result1 = format.get();
    constexpr const auto result2 = format.get();

    REQUIRE(result1 == "Reference");
    REQUIRE(result2 == "Reference");
    REQUIRE(result1 == result2);

    // Compile-time checks
    STATIC_REQUIRE(result1 == "Reference");
    STATIC_REQUIRE(result2 == "Reference");
    STATIC_REQUIRE(result1 == result2);
  }
}

} // namespace toy
