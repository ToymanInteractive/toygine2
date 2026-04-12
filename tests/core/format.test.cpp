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
  \file   format.test.cpp
  \brief  Unit tests for toy::format() and toy::formatTo().
*/

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

// Pattern with no placeholders is copied verbatim.
TEST_CASE("core/format/no_placeholders") {
  constexpr auto result = format<32>("Hello World");

  REQUIRE(result == "Hello World");

  static_assert(result == "Hello World", "literal pattern must be copied verbatim");
}

// Single auto placeholder substitutes an integer.
TEST_CASE("core/format/auto_single_int") {
  constexpr auto result = format<32>("value: {}", 42);

  REQUIRE(result == "value: 42");

  static_assert(result == "value: 42", "single auto placeholder must be replaced by the argument");
}

// Multiple auto placeholders are substituted left to right.
TEST_CASE("core/format/auto_multiple") {
  constexpr auto result = format<64>("{} and {}", 1, 2);

  REQUIRE(result == "1 and 2");

  static_assert(result == "1 and 2", "auto placeholders must be substituted in argument order");
}

// Auto placeholder with bool produces "true" or "false".
TEST_CASE("core/format/auto_bool") {
  constexpr auto t = format<16>("{}", true);
  constexpr auto f = format<16>("{}", false);

  REQUIRE(t == "true");
  REQUIRE(f == "false");

  static_assert(t == "true", "bool true must format as 'true'");
  static_assert(f == "false", "bool false must format as 'false'");
}

// Auto placeholder with char inserts the character directly.
TEST_CASE("core/format/auto_char") {
  constexpr auto result = format<8>("{}", 'X');

  REQUIRE(result == "X");

  static_assert(result == "X", "char argument must be inserted as the character itself");
}

// Auto placeholder with a C-string argument.
TEST_CASE("core/format/auto_c_string") {
  const auto result = format<32>("say: {}", "hello");

  REQUIRE(result == "say: hello");
}

// Auto placeholder with a CStringView argument.
TEST_CASE("core/format/auto_string_view") {
  constexpr CStringView greeting("hi");
  constexpr auto result = format<16>("{}", greeting);

  REQUIRE(result == "hi");

  static_assert(result == "hi", "CStringView argument must be substituted correctly");
}

// Auto placeholder with negative integer includes the minus sign.
TEST_CASE("core/format/auto_negative_int") {
  constexpr auto result = format<16>("{}", -7);

  REQUIRE(result == "-7");

  static_assert(result == "-7", "negative integer must include the minus sign");
}

// Auto placeholder with unsigned integer.
TEST_CASE("core/format/auto_unsigned_int") {
  constexpr auto result = format<16>("{}", 255U);

  REQUIRE(result == "255");

  static_assert(result == "255", "unsigned integer must format correctly");
}

// Positional placeholders in declaration order.
TEST_CASE("core/format/positional_direct") {
  constexpr auto result = format<32>("{0} {1}", 10, 20);

  REQUIRE(result == "10 20");

  static_assert(result == "10 20", "positional placeholders in order must match argument order");
}

// Positional placeholders in reverse order.
TEST_CASE("core/format/positional_reorder") {
  constexpr auto result = format<32>("{1} {0}", 10, 20);

  REQUIRE(result == "20 10");

  static_assert(result == "20 10", "reversed positional indices must swap argument order");
}

// Same positional index used more than once.
TEST_CASE("core/format/positional_repeat") {
  constexpr auto result = format<16>("{0}{0}", 7);

  REQUIRE(result == "77");

  static_assert(result == "77", "repeated positional index must substitute the same argument twice");
}

// Sparse positional index: only {2} appears; {0} and {1} are unused.
TEST_CASE("core/format/positional_sparse") {
  constexpr auto result = format<16>("{2}", 1, 2, 3);

  REQUIRE(result == "3");

  static_assert(result == "3", "sparse positional index must select the correct argument");
}

// Escaped {{ emits a literal left brace.
TEST_CASE("core/format/escaped_open_brace") {
  constexpr auto result = format<8>("{{");

  REQUIRE(result == "{");

  static_assert(result == "{", "{{ must emit a single {");
}

// Escaped }} emits a literal right brace.
TEST_CASE("core/format/escaped_close_brace") {
  constexpr auto result = format<8>("}}");

  REQUIRE(result == "}");

  static_assert(result == "}", "}} must emit a single }");
}

// Escaped braces surrounding a placeholder.
TEST_CASE("core/format/escaped_around_placeholder") {
  constexpr auto result = format<16>("{{{}}}", 42);

  REQUIRE(result == "{42}");

  static_assert(result == "{42}", "{{ and }} surrounding a placeholder must emit literal braces");
}

// Empty format string produces an empty result.
TEST_CASE("core/format/empty_pattern") {
  constexpr auto result = format<8>("");

  REQUIRE(result.empty());

  static_assert(result.empty(), "empty pattern must produce an empty string");
}

// formatTo replaces the content of an existing string.
TEST_CASE("core/format_to/replaces_output") {
  FixedString<64> output;
  formatTo(output, "{} + {} = {}", 1, 2, 3);

  REQUIRE(output == "1 + 2 = 3");
}

// formatTo with positional placeholders.
TEST_CASE("core/format_to/positional") {
  FixedString<32> output;
  formatTo(output, "{{{1}/{0}}}", 2024, 12);

  REQUIRE(output == "{12/2024}");
}

} // namespace toy
