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
  \brief  Unit tests for toy::format(), toy::formatTo(), toy::vformat(), and toy::vformatTo().
*/

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

// Pattern with no placeholders is copied verbatim.
TEST_CASE("format/no_placeholders") {
  constexpr auto result = format<32>("Hello World");

  REQUIRE(result == "Hello World");

  static_assert(result == "Hello World", "literal pattern must be copied verbatim");
}

// Single auto placeholder substitutes an integer.
TEST_CASE("format/auto_single_int") {
  constexpr auto result = format<32>("value: {}", 42);

  REQUIRE(result == "value: 42");

  static_assert(result == "value: 42", "single auto placeholder must be replaced by the argument");
}

// Multiple auto placeholders are substituted left to right.
TEST_CASE("format/auto_multiple") {
  constexpr auto result = format<64>("{} and {}", 1, 2);

  REQUIRE(result == "1 and 2");

  static_assert(result == "1 and 2", "auto placeholders must be substituted in argument order");
}

// Auto placeholder with bool produces "true" or "false".
TEST_CASE("format/auto_bool") {
  constexpr auto t = format<16>("{}", true);
  constexpr auto f = format<16>("{}", false);

  REQUIRE(t == "true");
  REQUIRE(f == "false");

  static_assert(t == "true", "bool true must format as 'true'");
  static_assert(f == "false", "bool false must format as 'false'");
}

// Auto placeholder with char inserts the character directly.
TEST_CASE("format/auto_char") {
  constexpr auto result = format<8>("{}", 'X');

  REQUIRE(result == "X");

  static_assert(result == "X", "char argument must be inserted as the character itself");
}

// Auto placeholder with a C-string argument.
TEST_CASE("format/auto_c_string") {
  const auto result = format<32>("say: {}", "hello");

  REQUIRE(result == "say: hello");
}

// Auto placeholder with a CStringView argument.
TEST_CASE("format/auto_string_view") {
  constexpr CStringView greeting("hi");
  constexpr auto        result = format<16>("{}", greeting);

  REQUIRE(result == "hi");

  static_assert(result == "hi", "CStringView argument must be substituted correctly");
}

// Auto placeholder with negative integer includes the minus sign.
TEST_CASE("format/auto_negative_int") {
  constexpr auto result = format<16>("{}", -7);

  REQUIRE(result == "-7");

  static_assert(result == "-7", "negative integer must include the minus sign");
}

// Auto placeholder with unsigned integer.
TEST_CASE("format/auto_unsigned_int") {
  constexpr auto result = format<16>("{}", 255U);

  REQUIRE(result == "255");

  static_assert(result == "255", "unsigned integer must format correctly");
}

// Positional placeholders in declaration order.
TEST_CASE("format/positional_direct") {
  constexpr auto result = format<32>("{0} {1}", 10, 20);

  REQUIRE(result == "10 20");

  static_assert(result == "10 20", "positional placeholders in order must match argument order");
}

// Positional placeholders in reverse order.
TEST_CASE("format/positional_reorder") {
  constexpr auto result = format<32>("{1} {0}", 10, 20);

  REQUIRE(result == "20 10");

  static_assert(result == "20 10", "reversed positional indices must swap argument order");
}

// Same positional index used more than once.
TEST_CASE("format/positional_repeat") {
  constexpr auto result = format<16>("{0}{0}", 7);

  REQUIRE(result == "77");

  static_assert(result == "77", "repeated positional index must substitute the same argument twice");
}

// Sparse positional index: only {2} appears; {0} and {1} are unused.
TEST_CASE("format/positional_sparse") {
  constexpr auto result = format<16>("{2}", 1, 2, 3);

  REQUIRE(result == "3");

  static_assert(result == "3", "sparse positional index must select the correct argument");
}

// Escaped {{ emits a literal left brace.
TEST_CASE("format/escaped_open_brace") {
  constexpr auto result = format<8>("{{");

  REQUIRE(result == "{");

  static_assert(result == "{", "{{ must emit a single {");
}

// Escaped }} emits a literal right brace.
TEST_CASE("format/escaped_close_brace") {
  constexpr auto result = format<8>("}}");

  REQUIRE(result == "}");

  static_assert(result == "}", "}} must emit a single }");
}

// Escaped braces surrounding a placeholder.
TEST_CASE("format/escaped_around_placeholder") {
  constexpr auto result = format<16>("{{{}}}", 42);

  REQUIRE(result == "{42}");

  static_assert(result == "{42}", "{{ and }} surrounding a placeholder must emit literal braces");
}

// Empty format string produces an empty result.
TEST_CASE("format/empty_pattern") {
  constexpr auto result = format<8>("");

  REQUIRE(result.empty());

  static_assert(result.empty(), "empty pattern must produce an empty string");
}

// formatTo replaces the content of an existing string.
TEST_CASE("format_to/replaces_output") {
  FixedString<64> output("stale-content");

  formatTo(output, "{} + {} = {}", 1, 2, 3);

  REQUIRE(output == "1 + 2 = 3");
}

// formatTo with positional placeholders.
TEST_CASE("format_to/positional") {
  FixedString<32> output("");

  formatTo(output, "{{{1}/{0}}}", 2024, 12);

  REQUIRE(output == "{12/2024}");
}

// ----- vformat -----

// Single auto placeholder substitutes an integer and returns by value.
TEST_CASE("vformat/single_int") {
  const auto result = vformat<32>(CStringView("value: {}"), 42);

  REQUIRE(result == "value: 42");
}

// Multiple arguments of different types are substituted left to right.
TEST_CASE("vformat/mixed_types") {
  const auto result = vformat<64>(CStringView("{} {} {}"), 42, "world", true);

  REQUIRE(result == "42 world true");
}

// Positional placeholders select arguments by index.
TEST_CASE("vformat/positional_reorder") {
  const auto result = vformat<32>(CStringView("{1} before {0}"), 10, 20);

  REQUIRE(result == "20 before 10");
}

// Escaped {{ and }} emit literal braces around a placeholder.
TEST_CASE("vformat/escaped_braces") {
  const auto result = vformat<32>(CStringView("{{{}}}"), 42);

  REQUIRE(result == "{42}");
}

// std::nullptr_t is formatted as "nullptr".
TEST_CASE("vformat/nullptr") {
  const auto result = vformat<16>(CStringView("{}"), nullptr);

  REQUIRE(result == "nullptr");
}

// ----- vformatTo -----

// Literal-only pattern is copied verbatim.
TEST_CASE("vformat_to/no_placeholders") {
  FixedString<32> output;

  vformatTo(output, CStringView("Hello World"));

  REQUIRE(output == "Hello World");
}

// Empty pattern produces an empty output.
TEST_CASE("vformat_to/empty_pattern") {
  FixedString<16> output;

  vformatTo(output, CStringView(""));

  REQUIRE(output.empty());
}

// Integer argument is type-erased and formatted via the generic OStringStream path.
TEST_CASE("vformat_to/single_int") {
  FixedString<32> output;

  vformatTo(output, CStringView("value: {}"), 42);

  REQUIRE(output == "value: 42");
}

// Bool true formats as "true" and false as "false".
TEST_CASE("vformat_to/bool") {
  FixedString<16> outTrue;
  FixedString<16> outFalse;

  vformatTo(outTrue, CStringView("{}"), true);
  vformatTo(outFalse, CStringView("{}"), false);

  REQUIRE(outTrue == "true");
  REQUIRE(outFalse == "false");
}

// Char argument inserts the character directly.
TEST_CASE("vformat_to/char") {
  FixedString<8> output;

  vformatTo(output, CStringView("{}"), 'X');

  REQUIRE(output == "X");
}

// Non-null char pointer takes the char-pointer fast path.
TEST_CASE("vformat_to/c_string") {
  FixedString<32> output;
  const char *    msg = "hello";

  vformatTo(output, CStringView("say: {}"), msg);

  REQUIRE(output == "say: hello");
}

// Null char pointer produces no output for that placeholder.
TEST_CASE("vformat_to/null_c_string") {
  FixedString<16> output;
  const char *    msg = nullptr;

  vformatTo(output, CStringView("[{}]"), msg);

  REQUIRE(output == "[]");
}

// FixedString argument takes the StringLike fast path.
TEST_CASE("vformat_to/fixed_string") {
  FixedString<32> output;
  FixedString<16> name("engine");

  vformatTo(output, CStringView("toy {}"), name);

  REQUIRE(output == "toy engine");
}

// CStringView argument takes the StringLike fast path.
TEST_CASE("vformat_to/c_string_view") {
  FixedString<32> output;
  CStringView     sv("world");

  vformatTo(output, CStringView("hello {}"), sv);

  REQUIRE(output == "hello world");
}

// Multiple arguments of different types are substituted left to right.
TEST_CASE("vformat_to/mixed_types") {
  FixedString<64> output;

  vformatTo(output, CStringView("{} {} {}"), 10, "mid", false);

  REQUIRE(output == "10 mid false");
}

// Positional placeholders in reverse order swap arguments.
TEST_CASE("vformat_to/positional_reorder") {
  FixedString<32> output;

  vformatTo(output, CStringView("{1} {0}"), 10, 20);

  REQUIRE(output == "20 10");
}

// Escaped braces surrounding a placeholder emit literal braces around the value.
TEST_CASE("vformat_to/escaped_braces") {
  FixedString<16> output;

  vformatTo(output, CStringView("{{{}}}"), 42);

  REQUIRE(output == "{42}");
}

// Previous content of output is replaced.
TEST_CASE("vformat_to/replaces_output") {
  FixedString<64> output;
  output.append("stale-content");

  vformatTo(output, CStringView("{} + {} = {}"), 1, 2, 3);

  REQUIRE(output == "1 + 2 = 3");
}

// Pattern built at runtime formats correctly.
TEST_CASE("vformat_to/runtime_pattern") {
  FixedString<32> pattern;
  pattern.append("x={}");

  FixedString<32> output;

  vformatTo(output, CStringView(pattern.c_str()), 99);

  REQUIRE(output == "x=99");
}

// Literal text after the last placeholder is preserved.
TEST_CASE("vformat_to/trailing_literal") {
  FixedString<32> output;

  vformatTo(output, CStringView("{} items remaining"), 5);

  REQUIRE(output == "5 items remaining");
}

} // namespace toy
