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
/*
#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

// FormatString constructor stores the format pattern.
TEST_CASE("core/format_string/constructor") {
  // String without placeholders stored as given.
  SUBCASE("string_without_placeholders") {
    constexpr FormatString<> format("Hello World");

    REQUIRE(format.get() == "Hello World");

    static_assert(format.get() == "Hello World", "stored format string must equal the given literal");
  }

  // Single placeholder preserved.
  SUBCASE("single_placeholder") {
    constexpr FormatString<int> format("Value: {}");

    REQUIRE(format.get() == "Value: {}");

    static_assert(format.get() == "Value: {}", "stored pattern must preserve placeholder");
  }

  // Multiple placeholders preserved.
  SUBCASE("multiple_placeholders") {
    constexpr FormatString<int, float, double> format("{}, {}, {}");

    REQUIRE(format.get() == "{}, {}, {}");

    static_assert(format.get() == "{}, {}, {}", "stored pattern must preserve all placeholders");
  }

  // Escaped braces stored as literal.
  SUBCASE("escaped_braces") {
    constexpr FormatString<> format("{{}}");

    REQUIRE(format.get() == "{{}}");

    static_assert(format.get() == "{{}}", "escaped braces must be stored as literal");
  }

  // Mixed placeholders and escaped braces preserved.
  SUBCASE("mixed_placeholders_and_escaped_braces") {
    constexpr FormatString<int, float> format("{{{} and {} and }}");

    REQUIRE(format.get() == "{{{} and {} and }}");

    static_assert(format.get() == "{{{} and {} and }}", "mix of placeholders and escaped braces must be preserved");
  }

  // Adjacent placeholders stored as given.
  SUBCASE("adjacent_placeholders") {
    constexpr FormatString<int, int> format("{}{}");

    REQUIRE(format.get() == "{}{}");

    static_assert(format.get() == "{}{}", "adjacent placeholders must be stored as given");
  }
}

// get() returns the stored string; two calls return equal values.
TEST_CASE("core/format_string/get_method") {
  // get() returns the stored string.
  SUBCASE("returns_stored_string") {
    constexpr FormatString<> format("Test");
    constexpr const auto result = format.get();

    REQUIRE(result == "Test");

    static_assert(result == "Test", "get() must return the stored format string");
  }

  // Successive get() calls return equal values.
  SUBCASE("successive_calls_return_equal") {
    constexpr FormatString<> format("Reference");
    constexpr const auto result1 = format.get();
    constexpr const auto result2 = format.get();

    REQUIRE(result1 == "Reference");
    REQUIRE(result2 == "Reference");
    REQUIRE(result1 == result2);

    static_assert(result1 == "Reference", "first get() must return stored string");
    static_assert(result2 == "Reference", "second get() must return stored string");
    static_assert(result1 == result2, "successive get() calls must return equal values");
  }
}

} // namespace toy
*/
