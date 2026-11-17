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
  \file   format_pattern.test.cpp
  \brief  Unit tests for \ref toy::FormatPatternValidationError and toy::validateFormatPattern().
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

// Text without braces validates against an argument count of zero, escaped braces included.
TEST_CASE("format_pattern/validate/valid_without_placeholders") {
  CHECK(validateFormatPattern("", 0) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("plain text", 0) == FormatPatternValidationError::none);

  // A doubled brace escapes a single brace and produces no placeholder.
  CHECK(validateFormatPattern("{{", 0) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("}}", 0) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("{{}}", 0) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("a{{b}}c", 0) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("{{0}}", 0) == FormatPatternValidationError::none);

  static_assert(validateFormatPattern("", 0) == FormatPatternValidationError::none,
                "an empty pattern requires no arguments");
  static_assert(validateFormatPattern("plain text", 0) == FormatPatternValidationError::none,
                "text without braces requires no arguments");
  static_assert(validateFormatPattern("{{", 0) == FormatPatternValidationError::none,
                "an escaped opening brace produces no placeholder");
  static_assert(validateFormatPattern("}}", 0) == FormatPatternValidationError::none,
                "an escaped closing brace produces no placeholder");
  static_assert(validateFormatPattern("{{}}", 0) == FormatPatternValidationError::none,
                "a pair of escaped braces produces no placeholder");
  static_assert(validateFormatPattern("a{{b}}c", 0) == FormatPatternValidationError::none,
                "escaped braces inside text produce no placeholder");
  static_assert(validateFormatPattern("{{0}}", 0) == FormatPatternValidationError::none,
                "an index written inside doubled braces produces no placeholder");
}

// Auto placeholders match the argument count exactly, wherever the surrounding text puts them.
TEST_CASE("format_pattern/validate/valid_auto_placeholders") {
  CHECK(validateFormatPattern("{}", 1) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("{} {}", 2) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("{} {} {}", 3) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("pre{}post{}", 2) == FormatPatternValidationError::none);

  static_assert(validateFormatPattern("{}", 1) == FormatPatternValidationError::none,
                "one auto placeholder requires one argument");
  static_assert(validateFormatPattern("{} {}", 2) == FormatPatternValidationError::none,
                "two auto placeholders require two arguments");
  static_assert(validateFormatPattern("{} {} {}", 3) == FormatPatternValidationError::none,
                "three auto placeholders require three arguments");
  static_assert(validateFormatPattern("pre{}post{}", 2) == FormatPatternValidationError::none,
                "auto placeholders work inside surrounding text");
}

// Positional placeholders only require each index to name an argument; repeats and unused arguments are fine.
TEST_CASE("format_pattern/validate/valid_positional_placeholders") {
  CHECK(validateFormatPattern("{0}", 1) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("{1}", 2) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("{1} {0}", 2) == FormatPatternValidationError::none);

  // A repeated index reuses one argument, and the count check never asks for every argument to appear.
  CHECK(validateFormatPattern("{0} {0}", 1) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("{0}", 2) == FormatPatternValidationError::none);

  // Leading zeros keep decimal parsing.
  CHECK(validateFormatPattern("{01}", 2) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern("{0000000002}", 3) == FormatPatternValidationError::none);

  static_assert(validateFormatPattern("{0}", 1) == FormatPatternValidationError::none,
                "index zero names the first argument");
  static_assert(validateFormatPattern("{1}", 2) == FormatPatternValidationError::none,
                "an index names the argument at its position");
  static_assert(validateFormatPattern("{1} {0}", 2) == FormatPatternValidationError::none,
                "positional placeholders may appear in any order");
  static_assert(validateFormatPattern("{0} {0}", 1) == FormatPatternValidationError::none,
                "a repeated index reuses one argument");
  static_assert(validateFormatPattern("{0}", 2) == FormatPatternValidationError::none,
                "a positional pattern need not use every argument");
  static_assert(validateFormatPattern("{01}", 2) == FormatPatternValidationError::none,
                "a leading zero keeps decimal parsing");
  static_assert(validateFormatPattern("{0000000002}", 3) == FormatPatternValidationError::none,
                "multi-digit indices with leading zeros parse as one index");
}

// A brace that opens nothing and a brace that closes nothing both report the unmatched brace error.
TEST_CASE("format_pattern/validate/unmatched_brace") {
  CHECK(validateFormatPattern("{", 0) == FormatPatternValidationError::unmatchedBrace);
  CHECK(validateFormatPattern("a{", 0) == FormatPatternValidationError::unmatchedBrace);
  CHECK(validateFormatPattern("}", 0) == FormatPatternValidationError::unmatchedBrace);
  CHECK(validateFormatPattern("a}b", 0) == FormatPatternValidationError::unmatchedBrace);

  // Brace content that is neither an index nor the closing brace never opens a placeholder.
  CHECK(validateFormatPattern("{x}", 0) == FormatPatternValidationError::unmatchedBrace);
  CHECK(validateFormatPattern("{ }", 0) == FormatPatternValidationError::unmatchedBrace);
  CHECK(validateFormatPattern("{ {", 0) == FormatPatternValidationError::unmatchedBrace);

  // A stray closing brace fails the pattern even after a well-formed placeholder.
  CHECK(validateFormatPattern("{0}}", 1) == FormatPatternValidationError::unmatchedBrace);

  static_assert(validateFormatPattern("{", 0) == FormatPatternValidationError::unmatchedBrace,
                "a trailing opening brace matches nothing");
  static_assert(validateFormatPattern("a{", 0) == FormatPatternValidationError::unmatchedBrace,
                "a trailing opening brace after text matches nothing");
  static_assert(validateFormatPattern("}", 0) == FormatPatternValidationError::unmatchedBrace,
                "a lone closing brace matches nothing");
  static_assert(validateFormatPattern("a}b", 0) == FormatPatternValidationError::unmatchedBrace,
                "a stray closing brace inside text matches nothing");
  static_assert(validateFormatPattern("{x}", 0) == FormatPatternValidationError::unmatchedBrace,
                "non-index content inside braces opens no placeholder");
  static_assert(validateFormatPattern("{ }", 0) == FormatPatternValidationError::unmatchedBrace,
                "whitespace inside braces opens no placeholder");
  static_assert(validateFormatPattern("{ {", 0) == FormatPatternValidationError::unmatchedBrace,
                "a nested opening brace inside braces opens no placeholder");
  static_assert(validateFormatPattern("{0}}", 1) == FormatPatternValidationError::unmatchedBrace,
                "a stray closing brace fails the pattern after a well-formed placeholder");
}

// Digits inside braces must be followed by the closing brace, before any range check runs.
TEST_CASE("format_pattern/validate/invalid_content") {
  CHECK(validateFormatPattern("{0", 1) == FormatPatternValidationError::invalidContent);
  CHECK(validateFormatPattern("{0x}", 1) == FormatPatternValidationError::invalidContent);
  CHECK(validateFormatPattern("{0 }", 1) == FormatPatternValidationError::invalidContent);
  CHECK(validateFormatPattern("{01", 2) == FormatPatternValidationError::invalidContent);
  CHECK(validateFormatPattern("{0{", 1) == FormatPatternValidationError::invalidContent);

  // The content check precedes the range check, whatever the index reads.
  CHECK(validateFormatPattern("{9", 0) == FormatPatternValidationError::invalidContent);

  static_assert(validateFormatPattern("{0", 1) == FormatPatternValidationError::invalidContent,
                "an index running into the end of the pattern misses its closing brace");
  static_assert(validateFormatPattern("{0x}", 1) == FormatPatternValidationError::invalidContent,
                "only the closing brace may follow an index");
  static_assert(validateFormatPattern("{0 }", 1) == FormatPatternValidationError::invalidContent,
                "whitespace may not follow an index inside the braces");
  static_assert(validateFormatPattern("{01", 2) == FormatPatternValidationError::invalidContent,
                "a multi-digit index misses its closing brace at the end of the pattern");
  static_assert(validateFormatPattern("{0{", 1) == FormatPatternValidationError::invalidContent,
                "an opening brace may not follow an index inside the braces");
  static_assert(validateFormatPattern("{9", 0) == FormatPatternValidationError::invalidContent,
                "the content check precedes the range check");
}

// Auto and positional placeholders may not share one pattern, whichever comes first.
TEST_CASE("format_pattern/validate/mixed_placeholders") {
  CHECK(validateFormatPattern("{} {0}", 2) == FormatPatternValidationError::mixedPlaceholders);
  CHECK(validateFormatPattern("{0} {}", 2) == FormatPatternValidationError::mixedPlaceholders);
  CHECK(validateFormatPattern("{} {1}", 2) == FormatPatternValidationError::mixedPlaceholders);
  CHECK(validateFormatPattern("{0} {1} {}", 3) == FormatPatternValidationError::mixedPlaceholders);

  static_assert(validateFormatPattern("{} {0}", 2) == FormatPatternValidationError::mixedPlaceholders,
                "a positional placeholder after an auto one mixes the modes");
  static_assert(validateFormatPattern("{0} {}", 2) == FormatPatternValidationError::mixedPlaceholders,
                "an auto placeholder after a positional one mixes the modes");
  static_assert(validateFormatPattern("{} {1}", 2) == FormatPatternValidationError::mixedPlaceholders,
                "a positional index after an auto one mixes the modes");
  static_assert(validateFormatPattern("{0} {1} {}", 3) == FormatPatternValidationError::mixedPlaceholders,
                "one auto placeholder is enough to mix the modes");
}

// Auto placeholder counts and argument counts must agree; plain text must carry no arguments.
TEST_CASE("format_pattern/validate/arg_count_mismatch") {
  CHECK(validateFormatPattern("", 1) == FormatPatternValidationError::argCountMismatch);
  CHECK(validateFormatPattern("plain text", 1) == FormatPatternValidationError::argCountMismatch);

  CHECK(validateFormatPattern("{}", 0) == FormatPatternValidationError::argCountMismatch);
  CHECK(validateFormatPattern("{}", 2) == FormatPatternValidationError::argCountMismatch);
  CHECK(validateFormatPattern("{} {}", 1) == FormatPatternValidationError::argCountMismatch);
  CHECK(validateFormatPattern("{} {}", 3) == FormatPatternValidationError::argCountMismatch);

  // Escaped braces produce no placeholders, so they cannot satisfy an argument count either.
  CHECK(validateFormatPattern("{{}}", 1) == FormatPatternValidationError::argCountMismatch);

  static_assert(validateFormatPattern("", 1) == FormatPatternValidationError::argCountMismatch,
                "arguments without placeholders have nothing to bind them");
  static_assert(validateFormatPattern("plain text", 1) == FormatPatternValidationError::argCountMismatch,
                "text without placeholders has nothing to bind an argument");
  static_assert(validateFormatPattern("{}", 0) == FormatPatternValidationError::argCountMismatch,
                "an auto placeholder with no argument has nothing to bind");
  static_assert(validateFormatPattern("{}", 2) == FormatPatternValidationError::argCountMismatch,
                "two arguments cannot bind one auto placeholder");
  static_assert(validateFormatPattern("{} {}", 1) == FormatPatternValidationError::argCountMismatch,
                "one argument cannot bind two auto placeholders");
  static_assert(validateFormatPattern("{} {}", 3) == FormatPatternValidationError::argCountMismatch,
                "three arguments cannot bind two auto placeholders");
  static_assert(validateFormatPattern("{{}}", 1) == FormatPatternValidationError::argCountMismatch,
                "escaped braces produce no placeholders to bind an argument");
}

// Every positional index must name an argument, and an index that overflows size_t reports the same error.
TEST_CASE("format_pattern/validate/index_out_of_range") {
  CHECK(validateFormatPattern("{0}", 0) == FormatPatternValidationError::indexOutOfRange);
  CHECK(validateFormatPattern("{1}", 1) == FormatPatternValidationError::indexOutOfRange);
  CHECK(validateFormatPattern("{2}", 2) == FormatPatternValidationError::indexOutOfRange);
  CHECK(validateFormatPattern("{01}", 1) == FormatPatternValidationError::indexOutOfRange);

  // An index that wraps size_t reads as zero, which the range check would accept; the guard has to reject it first.
  CHECK(validateFormatPattern("{18446744073709551616}", 1) == FormatPatternValidationError::indexOutOfRange);

  static_assert(validateFormatPattern("{0}", 0) == FormatPatternValidationError::indexOutOfRange,
                "the smallest index is out of range without arguments");
  static_assert(validateFormatPattern("{1}", 1) == FormatPatternValidationError::indexOutOfRange,
                "an index must stay below the argument count");
  static_assert(validateFormatPattern("{2}", 2) == FormatPatternValidationError::indexOutOfRange,
                "an index equal to the argument count is out of range");
  static_assert(validateFormatPattern("{01}", 1) == FormatPatternValidationError::indexOutOfRange,
                "a leading zero keeps the index it names out of range");
  static_assert(validateFormatPattern("{18446744073709551616}", 1) == FormatPatternValidationError::indexOutOfRange,
                "an index that wraps size_t to zero must be rejected before the range check sees it");
}

// A fixed string validates under the same rules as any other source, by its own size and pointer.
TEST_CASE("format_pattern/validate/string_like_sources") {
  const FixedString<3> autoPlaceholder{"{}"};
  CHECK(validateFormatPattern(autoPlaceholder, 1) == FormatPatternValidationError::none);
  CHECK(validateFormatPattern(autoPlaceholder, 0) == FormatPatternValidationError::argCountMismatch);

  static_assert(validateFormatPattern(FixedString<3>{"{}"}, 1) == FormatPatternValidationError::none,
                "a fixed string source validates under the same rules");
  static_assert(validateFormatPattern(FixedString<3>{"{}"}, 0) == FormatPatternValidationError::argCountMismatch,
                "a fixed string source counts its placeholders against the arguments");
}

} // namespace toy
