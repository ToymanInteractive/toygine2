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
  \file   format_pattern.test.cpp
  \brief  Unit tests for `toy::validateFormatPattern()`.
*/

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

// Empty pattern with zero arguments is valid.
TEST_CASE("format_pattern/none_empty_zero_args") {
  REQUIRE(validateFormatPattern(CStringView(""), 0) == FormatPatternValidationError::none);

  static_assert(validateFormatPattern(CStringView(""), 0) == FormatPatternValidationError::none,
                "empty pattern with zero arguments must validate");
}

// Literal text without placeholders is valid when no arguments are expected.
TEST_CASE("format_pattern/none_literals_only") {
  REQUIRE(validateFormatPattern(CStringView("plain"), 0) == FormatPatternValidationError::none);

  static_assert(validateFormatPattern(CStringView("plain"), 0) == FormatPatternValidationError::none,
                "literal-only pattern with zero arguments must validate");
}

// Auto-indexed placeholders: count of {} must equal argCount.
TEST_CASE("format_pattern/none_auto_indexed_matches") {
  REQUIRE(validateFormatPattern(CStringView("{}"), 1) == FormatPatternValidationError::none);
  REQUIRE(validateFormatPattern(CStringView("{}{}"), 2) == FormatPatternValidationError::none);

  static_assert(validateFormatPattern(CStringView("{}"), 1) == FormatPatternValidationError::none,
                "single auto placeholder with one argument must validate");
  static_assert(validateFormatPattern(CStringView("{}{}"), 2) == FormatPatternValidationError::none,
                "two auto placeholders with two arguments must validate");
}

// Positional placeholders: each index must be less than argCount; sparse indices are allowed.
TEST_CASE("format_pattern/none_positional_in_range") {
  REQUIRE(validateFormatPattern(CStringView("{0}"), 1) == FormatPatternValidationError::none);
  REQUIRE(validateFormatPattern(CStringView("{1}"), 2) == FormatPatternValidationError::none);
  REQUIRE(validateFormatPattern(CStringView("{2}"), 3) == FormatPatternValidationError::none);

  static_assert(validateFormatPattern(CStringView("{0}"), 1) == FormatPatternValidationError::none,
                "positional index zero with one argument must validate");
  static_assert(validateFormatPattern(CStringView("{1}"), 2) == FormatPatternValidationError::none,
                "positional index one with two arguments must validate");
  static_assert(validateFormatPattern(CStringView("{2}"), 3) == FormatPatternValidationError::none,
                "positional index two with three arguments must validate");
}

// Doubled braces are literals and do not introduce placeholders.
TEST_CASE("format_pattern/none_escaped_braces_only") {
  REQUIRE(validateFormatPattern(CStringView("{{}}"), 0) == FormatPatternValidationError::none);
  REQUIRE(validateFormatPattern(CStringView("a{{b}}c"), 0) == FormatPatternValidationError::none);

  static_assert(validateFormatPattern(CStringView("{{}}"), 0) == FormatPatternValidationError::none,
                "escaped braces only must validate with zero arguments");
  static_assert(validateFormatPattern(CStringView("a{{b}}c"), 0) == FormatPatternValidationError::none,
                "escaped braces only must validate with zero arguments");
}

// Mixed auto {} and positional {N} in one pattern is invalid.
TEST_CASE("format_pattern/mixed_placeholders") {
  REQUIRE(validateFormatPattern(CStringView("{} {0}"), 2) == FormatPatternValidationError::mixedPlaceholders);

  static_assert(validateFormatPattern(CStringView("{} {0}"), 2) == FormatPatternValidationError::mixedPlaceholders,
                "mixed auto and positional placeholders must be rejected");
}

// Non-empty pattern with no placeholders but positive argCount is invalid.
TEST_CASE("format_pattern/arg_count_mismatch_no_placeholder") {
  REQUIRE(validateFormatPattern(CStringView("x"), 1) == FormatPatternValidationError::argCountMismatch);

  static_assert(validateFormatPattern(CStringView("x"), 1) == FormatPatternValidationError::argCountMismatch,
                "literals with unused arguments must be rejected");
}

// Auto mode: number of {} must equal argCount.
TEST_CASE("format_pattern/arg_count_mismatch_auto") {
  REQUIRE(validateFormatPattern(CStringView("{}"), 0) == FormatPatternValidationError::argCountMismatch);
  REQUIRE(validateFormatPattern(CStringView("{}"), 2) == FormatPatternValidationError::argCountMismatch);

  static_assert(validateFormatPattern(CStringView("{}"), 0) == FormatPatternValidationError::argCountMismatch,
                "one auto placeholder with zero arguments must fail");
  static_assert(validateFormatPattern(CStringView("{}"), 2) == FormatPatternValidationError::argCountMismatch,
                "one auto placeholder with two arguments must fail");
}

// Stray `{` without closing placeholder is invalid.
TEST_CASE("format_pattern/unmatched_brace_open_eof") {
  REQUIRE(validateFormatPattern(CStringView("{"), 0) == FormatPatternValidationError::unmatchedBrace);

  static_assert(validateFormatPattern(CStringView("{"), 0) == FormatPatternValidationError::unmatchedBrace,
                "open brace at end must be unmatched");
}

// `{` followed by neither `}` nor a digit is invalid.
TEST_CASE("format_pattern/unmatched_brace_open_non_placeholder") {
  REQUIRE(validateFormatPattern(CStringView("{a}"), 0) == FormatPatternValidationError::unmatchedBrace);

  static_assert(validateFormatPattern(CStringView("{a}"), 0) == FormatPatternValidationError::unmatchedBrace,
                "non-placeholder content after open brace must be unmatched");
}

// Single `}` that is not part of `}}` is invalid.
TEST_CASE("format_pattern/unmatched_brace_close") {
  REQUIRE(validateFormatPattern(CStringView("}"), 0) == FormatPatternValidationError::unmatchedBrace);

  static_assert(validateFormatPattern(CStringView("}"), 0) == FormatPatternValidationError::unmatchedBrace,
                "stray close brace must be unmatched");
}

// Positional index must be closed by `}`; missing closer yields invalid content.
TEST_CASE("format_pattern/invalid_content_unclosed_index") {
  REQUIRE(validateFormatPattern(CStringView("{0"), 1) == FormatPatternValidationError::invalidContent);

  static_assert(validateFormatPattern(CStringView("{0"), 1) == FormatPatternValidationError::invalidContent,
                "unclosed positional index must be invalid content");
}

// Characters after a positional index are invalid.
TEST_CASE("format_pattern/invalid_content_trailing_chars") {
  REQUIRE(validateFormatPattern(CStringView("{0x}"), 1) == FormatPatternValidationError::invalidContent);

  static_assert(validateFormatPattern(CStringView("{0x}"), 1) == FormatPatternValidationError::invalidContent,
                "characters after a positional index must be rejected");
}

// Positional index must be less than argCount.
TEST_CASE("format_pattern/index_out_of_range") {
  REQUIRE(validateFormatPattern(CStringView("{0}"), 0) == FormatPatternValidationError::indexOutOfRange);
  REQUIRE(validateFormatPattern(CStringView("{1}"), 1) == FormatPatternValidationError::indexOutOfRange);

  static_assert(validateFormatPattern(CStringView("{1}"), 1) == FormatPatternValidationError::indexOutOfRange,
                "index equal to argCount must be out of range");
}

} // namespace toy
