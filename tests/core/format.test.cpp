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
  \brief  Unit tests for toy::format(), toy::formatTo(), toy::vformatTo(), and toy::makeVFormatArguments().
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
  constexpr auto        result = format<16>("{}", greeting);

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
  FixedString<64> output("stale-content");

  formatTo(output, "{} + {} = {}", 1, 2, 3);

  REQUIRE(output == "1 + 2 = 3");
}

// formatTo with positional placeholders.
TEST_CASE("core/format_to/positional") {
  FixedString<32> output("");

  formatTo(output, "{{{1}/{0}}}", 2024, 12);

  REQUIRE(output == "{12/2024}");
}

// ----- vformat (array overload) -----

// Pattern from a runtime variable — the key differentiator from toy::format().
TEST_CASE("core/vformat/runtime_pattern") {
  // Build the pattern at runtime so it cannot be a compile-time FormatString.
  FixedString<32> pattern;
  pattern.append("x=");
  pattern.append("{}");
  const CStringView runtimePattern(pattern.c_str());

  // Keep the original value alive for the duration of the FormatArgument array.
  const int x      = 99;
  auto      args   = makeVFormatArguments(x);
  auto      result = vformat<32>(runtimePattern, args);

  REQUIRE(result == "x=99");
}

// ----- vformatTo (variadic overload) -----

// Literal-only pattern is copied verbatim.
TEST_CASE("core/vformat_to/no_placeholders_variadic") {
  FixedString<32> output;

  vformatTo(output, CStringView("Hello World"));

  REQUIRE(output == "Hello World");
}

// Empty pattern produces an empty output.
TEST_CASE("core/vformat_to/empty_pattern_variadic") {
  FixedString<16> output;

  vformatTo(output, CStringView(""));

  REQUIRE(output.empty());
}

// Single auto placeholder substitutes an integer.
TEST_CASE("core/vformat_to/auto_single_int_variadic") {
  FixedString<32> output;

  vformatTo(output, CStringView("value: {}"), 42);

  REQUIRE(output == "value: 42");
}

// Negative integer includes the minus sign.
TEST_CASE("core/vformat_to/auto_negative_int_variadic") {
  FixedString<32> output;

  vformatTo(output, CStringView("{}"), -7);

  REQUIRE(output == "-7");
}

// Unsigned integer formats correctly.
TEST_CASE("core/vformat_to/auto_unsigned_int_variadic") {
  FixedString<16> output;

  vformatTo(output, CStringView("{}"), 255U);

  REQUIRE(output == "255");
}

// Bool true formats as "true" and false as "false".
TEST_CASE("core/vformat_to/auto_bool_variadic") {
  FixedString<16> outTrue;
  FixedString<16> outFalse;

  vformatTo(outTrue, CStringView("{}"), true);
  vformatTo(outFalse, CStringView("{}"), false);

  REQUIRE(outTrue == "true");
  REQUIRE(outFalse == "false");
}

// Char argument inserts the character directly.
TEST_CASE("core/vformat_to/auto_char_variadic") {
  FixedString<8> output;

  vformatTo(output, CStringView("{}"), 'X');

  REQUIRE(output == "X");
}

// C string argument formats correctly.
TEST_CASE("core/vformat_to/auto_c_string_variadic") {
  FixedString<32> output;
  const char *    msg = "hello";

  vformatTo(output, CStringView("say: {}"), msg);

  REQUIRE(output == "say: hello");
}

// FixedString argument formats correctly.
TEST_CASE("core/vformat_to/auto_fixed_string_variadic") {
  FixedString<32> output;
  FixedString<16> name;
  name.append("engine");

  vformatTo(output, CStringView("toy {}"), name);

  REQUIRE(output == "toy engine");
}

// CStringView argument formats correctly.
TEST_CASE("core/vformat_to/auto_c_string_view_variadic") {
  FixedString<32> output;
  CStringView     sv("world");

  vformatTo(output, CStringView("hello {}"), sv);

  REQUIRE(output == "hello world");
}

// Multiple auto placeholders are substituted left to right.
TEST_CASE("core/vformat_to/auto_multiple_variadic") {
  FixedString<64> output;

  vformatTo(output, CStringView("{} and {}"), 1, 2);

  REQUIRE(output == "1 and 2");
}

// Multiple arguments of different types format correctly.
TEST_CASE("core/vformat_to/auto_mixed_types_variadic") {
  FixedString<64> output;

  vformatTo(output, CStringView("{} {} {}"), 10, "mid", false);

  REQUIRE(output == "10 mid false");
}

// Positional placeholders in reverse order swap arguments.
TEST_CASE("core/vformat_to/positional_reorder_variadic") {
  FixedString<32> output;

  vformatTo(output, CStringView("{1} {0}"), 10, 20);

  REQUIRE(output == "20 10");
}

// Escaped braces surrounding a placeholder emit literal braces around the value.
TEST_CASE("core/vformat_to/escaped_around_placeholder_variadic") {
  FixedString<16> output;

  vformatTo(output, CStringView("{{{}}}"), 42);

  REQUIRE(output == "{42}");
}

// Previous content of output is replaced.
TEST_CASE("core/vformat_to/replaces_output_variadic") {
  FixedString<64> output;
  output.append("stale-content");

  vformatTo(output, CStringView("{} + {} = {}"), 1, 2, 3);

  REQUIRE(output == "1 + 2 = 3");
}

// Pattern built at runtime formats correctly via both overloads.
TEST_CASE("core/vformat_to/runtime_pattern") {
  FixedString<32> pattern;
  pattern.append("x=");
  pattern.append("{}");

  const CStringView runtimePattern(pattern.c_str());

  SUBCASE("variadic") {
    FixedString<32> output;

    vformatTo(output, runtimePattern, 99);

    REQUIRE(output == "x=99");
  }

  SUBCASE("array") {
    FixedString<32> output;
    const int       x    = 99;
    auto            args = makeVFormatArguments(x);

    vformatTo(output, runtimePattern, args);

    REQUIRE(output == "x=99");
  }
}

// Literal text after the last placeholder is preserved.
TEST_CASE("core/vformat_to/trailing_literal_variadic") {
  FixedString<32> output;

  vformatTo(output, CStringView("{} items remaining"), 5);

  REQUIRE(output == "5 items remaining");
}

// ----- vformatTo (array overload) -----

// Literal-only pattern is copied verbatim.
TEST_CASE("core/vformat_to/no_placeholders") {
  FixedString<32> output;
  auto            args = makeVFormatArguments();

  vformatTo(output, CStringView("Hello World"), args);

  REQUIRE(output == "Hello World");
}

// Empty pattern produces an empty output.
TEST_CASE("core/vformat_to/empty_pattern") {
  FixedString<16> output;
  auto            args = makeVFormatArguments();

  vformatTo(output, CStringView(""), args);

  REQUIRE(output.empty());
}

// Single auto placeholder substitutes an integer.
TEST_CASE("core/vformat_to/auto_single_int") {
  FixedString<32> output;

  const int x    = 42;
  auto      args = makeVFormatArguments(x);

  vformatTo(output, CStringView("value: {}"), args);

  REQUIRE(output == "value: 42");
}

// Negative integer includes the minus sign.
TEST_CASE("core/vformat_to/auto_negative_int") {
  FixedString<32> output;

  const int x    = -7;
  auto      args = makeVFormatArguments(x);

  vformatTo(output, CStringView("{}"), args);

  REQUIRE(output == "-7");
}

// Unsigned integer formats correctly.
TEST_CASE("core/vformat_to/auto_unsigned_int") {
  FixedString<16> output;

  const unsigned int x    = 255U;
  auto               args = makeVFormatArguments(x);

  vformatTo(output, CStringView("{}"), args);

  REQUIRE(output == "255");
}

// Bool true formats as "true" and false as "false".
TEST_CASE("core/vformat_to/auto_bool") {
  FixedString<16> outTrue;
  FixedString<16> outFalse;

  const bool t         = true;
  const bool f         = false;
  auto       argsTrue  = makeVFormatArguments(t);
  auto       argsFalse = makeVFormatArguments(f);

  vformatTo(outTrue, CStringView("{}"), argsTrue);
  vformatTo(outFalse, CStringView("{}"), argsFalse);

  REQUIRE(outTrue == "true");
  REQUIRE(outFalse == "false");
}

// Char argument inserts the character directly.
TEST_CASE("core/vformat_to/auto_char") {
  FixedString<8> output;

  const char ch   = 'X';
  auto       args = makeVFormatArguments(ch);

  vformatTo(output, CStringView("{}"), args);

  REQUIRE(output == "X");
}

// C string argument formats correctly.
TEST_CASE("core/vformat_to/auto_c_string") {
  FixedString<32> output;

  const char * msg  = "hello";
  auto         args = makeVFormatArguments(msg);

  vformatTo(output, CStringView("say: {}"), args);

  REQUIRE(output == "say: hello");
}

// FixedString argument formats correctly.
TEST_CASE("core/vformat_to/auto_fixed_string") {
  FixedString<32> output;

  FixedString<16> name;
  name.append("engine");
  auto args = makeVFormatArguments(name);

  vformatTo(output, CStringView("toy {}"), args);

  REQUIRE(output == "toy engine");
}

// CStringView argument formats correctly.
TEST_CASE("core/vformat_to/auto_c_string_view") {
  FixedString<32> output;

  CStringView sv("world");
  auto        args = makeVFormatArguments(sv);

  vformatTo(output, CStringView("hello {}"), args);

  REQUIRE(output == "hello world");
}

// Multiple auto placeholders are substituted left to right.
TEST_CASE("core/vformat_to/auto_multiple") {
  FixedString<64> output;

  const int a    = 1;
  const int b    = 2;
  auto      args = makeVFormatArguments(a, b);

  vformatTo(output, CStringView("{} and {}"), args);

  REQUIRE(output == "1 and 2");
}

// Multiple arguments of different types format correctly.
TEST_CASE("core/vformat_to/auto_mixed_types") {
  FixedString<64> output;

  const int    a    = 10;
  const char * b    = "mid";
  const bool   c    = false;
  auto         args = makeVFormatArguments(a, b, c);

  vformatTo(output, CStringView("{} {} {}"), args);

  REQUIRE(output == "10 mid false");
}

// Positional placeholders in declaration order.
TEST_CASE("core/vformat_to/positional_direct") {
  FixedString<32> output;

  const int a    = 10;
  const int b    = 20;
  auto      args = makeVFormatArguments(a, b);

  vformatTo(output, CStringView("{0} {1}"), args);

  REQUIRE(output == "10 20");
}

// Positional placeholders in reverse order swap arguments.
TEST_CASE("core/vformat_to/positional_reorder") {
  FixedString<32> output;

  const int a    = 10;
  const int b    = 20;
  auto      args = makeVFormatArguments(a, b);

  vformatTo(output, CStringView("{1} {0}"), args);

  REQUIRE(output == "20 10");
}

// Same positional index repeated substitutes the same argument.
TEST_CASE("core/vformat_to/positional_repeat") {
  FixedString<16> output;

  const int x    = 7;
  auto      args = makeVFormatArguments(x);

  vformatTo(output, CStringView("{0}{0}{0}"), args);

  REQUIRE(output == "777");
}

// Escaped {{ emits a literal left brace.
TEST_CASE("core/vformat_to/escaped_open_brace") {
  FixedString<8> output;
  auto           args = makeVFormatArguments();

  vformatTo(output, CStringView("{{"), args);

  REQUIRE(output == "{");
}

// Escaped }} emits a literal right brace.
TEST_CASE("core/vformat_to/escaped_close_brace") {
  FixedString<8> output;
  auto           args = makeVFormatArguments();

  vformatTo(output, CStringView("}}"), args);

  REQUIRE(output == "}");
}

// Escaped braces surrounding a placeholder emit literal braces around the value.
TEST_CASE("core/vformat_to/escaped_around_placeholder") {
  FixedString<16> output;

  const int x    = 42;
  auto      args = makeVFormatArguments(x);

  vformatTo(output, CStringView("{{{}}}"), args);

  REQUIRE(output == "{42}");
}

// Previous content of output is replaced.
TEST_CASE("core/vformat_to/replaces_output") {
  FixedString<64> output("stale-content");

  const int a    = 1;
  const int b    = 2;
  const int c    = 3;
  auto      args = makeVFormatArguments(a, b, c);

  vformatTo(output, CStringView("{} + {} = {}"), args);

  REQUIRE(output == "1 + 2 = 3");
}

// Literal text after the last placeholder is preserved.
TEST_CASE("core/vformat_to/trailing_literal") {
  FixedString<32> output;

  const int x    = 5;
  auto      args = makeVFormatArguments(x);

  vformatTo(output, CStringView("{} items remaining"), args);

  REQUIRE(output == "5 items remaining");
}

// Null C string argument produces no output for that placeholder.
TEST_CASE("core/vformat_to/null_c_string") {
  FixedString<32> output;

  const char * msg  = nullptr;
  auto         args = makeVFormatArguments(msg);

  vformatTo(output, CStringView("[{}]"), args);

  REQUIRE(output == "[]");
}

// ----- makeVFormatArguments -----

template <size_t N>
FormatContext makeTestCtx(FixedString<N> & buf) noexcept {
  return FormatContext{static_cast<void *>(&buf), [](void * c, const char * data, size_t n) noexcept {
                         static_cast<FixedString<N> *>(c)->append(data, n);
                       }};
}

// Zero arguments produces a zero-length array.
TEST_CASE("core/make_v_format_arguments/empty") {
  auto args = makeVFormatArguments();

  REQUIRE(args.size() == 0);
}

// Single int argument: array has one entry with the correct value pointer.
TEST_CASE("core/make_v_format_arguments/single_int_pointer") {
  const int x    = 42;
  auto      args = makeVFormatArguments(x);

  REQUIRE(args.size() == 1);
  REQUIRE(args[0].value == static_cast<const void *>(&x));
}

// Single int argument: formatFn produces the correct decimal representation.
TEST_CASE("core/make_v_format_arguments/single_int_format") {
  const int x    = 42;
  auto      args = makeVFormatArguments(x);

  FixedString<32> buf;
  auto            ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf == "42");
}

// Negative integer includes the minus sign.
TEST_CASE("core/make_v_format_arguments/negative_int") {
  const int x    = -7;
  auto      args = makeVFormatArguments(x);

  FixedString<32> buf;
  auto            ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf == "-7");
}

// Bool true formats as "true".
TEST_CASE("core/make_v_format_arguments/bool_true") {
  const bool v    = true;
  auto       args = makeVFormatArguments(v);

  FixedString<16> buf;
  auto            ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf == "true");
}

// Bool false formats as "false".
TEST_CASE("core/make_v_format_arguments/bool_false") {
  const bool v    = false;
  auto       args = makeVFormatArguments(v);

  FixedString<16> buf;
  auto            ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf == "false");
}

// Char argument formats as a single character.
TEST_CASE("core/make_v_format_arguments/char") {
  const char ch   = 'Z';
  auto       args = makeVFormatArguments(ch);

  FixedString<8> buf;
  auto           ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf == "Z");
}

// C string argument takes the char-pointer path.
TEST_CASE("core/make_v_format_arguments/c_string") {
  const char * msg  = "hello";
  auto         args = makeVFormatArguments(msg);

  FixedString<32> buf;
  auto            ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf == "hello");
}

// Null char pointer produces no output.
TEST_CASE("core/make_v_format_arguments/null_c_string") {
  const char * msg  = nullptr;
  auto         args = makeVFormatArguments(msg);

  FixedString<16> buf;
  auto            ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf.empty());
}

// StringLike argument (FixedString) takes the direct c_str/size path.
TEST_CASE("core/make_v_format_arguments/fixed_string") {
  FixedString<16> str;
  str.append("engine");
  auto args = makeVFormatArguments(str);

  FixedString<32> buf;
  auto            ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf == "engine");
}

// StringLike argument (CStringView) takes the direct c_str/size path.
TEST_CASE("core/make_v_format_arguments/c_string_view") {
  CStringView sv("world");
  auto        args = makeVFormatArguments(sv);

  FixedString<32> buf;
  auto            ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf == "world");
}

// Multiple arguments: each entry points to the correct original.
TEST_CASE("core/make_v_format_arguments/multiple_pointers") {
  const int    a    = 1;
  const char * b    = "two";
  const bool   c    = true;
  auto         args = makeVFormatArguments(a, b, c);

  REQUIRE(args.size() == 3);
  REQUIRE(args[0].value == static_cast<const void *>(&a));
  REQUIRE(args[1].value == static_cast<const void *>(&b));
  REQUIRE(args[2].value == static_cast<const void *>(&c));
}

// Multiple arguments: each formatFn is non-null.
TEST_CASE("core/make_v_format_arguments/multiple_format_fn") {
  const int a    = 1;
  const int b    = 2;
  auto      args = makeVFormatArguments(a, b);

  REQUIRE(args[0].formatFn != nullptr);
  REQUIRE(args[1].formatFn != nullptr);
}

// Multiple arguments: formatting each produces the expected text in order.
TEST_CASE("core/make_v_format_arguments/multiple_format_output") {
  const int    a    = 10;
  const char * b    = "mid";
  const bool   c    = false;
  auto         args = makeVFormatArguments(a, b, c);

  const char * expected[] = {"10", "mid", "false"};

  for (size_t i = 0; i < args.size(); ++i) {
    FixedString<32> buf;
    auto            ctx = makeTestCtx(buf);

    args[i].formatFn(args[i].value, ctx);

    REQUIRE(buf == expected[i]);
  }
}

// Unsigned integer argument formats correctly through OStringStream path.
TEST_CASE("core/make_v_format_arguments/unsigned_int") {
  const unsigned int x    = 255U;
  auto               args = makeVFormatArguments(x);

  FixedString<16> buf;
  auto            ctx = makeTestCtx(buf);

  args[0].formatFn(args[0].value, ctx);

  REQUIRE(buf == "255");
}

} // namespace toy
