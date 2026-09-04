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
  \file   utils.test.cpp
  \brief  Unit tests for the core UTF-8 utilities.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// Literals of one encoding width each, so an expected count derives from the byte length instead of a hand count.
constexpr const char * c_ascii    = "player";
constexpr const char * c_cyrillic = "привет";
constexpr const char * c_cjk      = "日本語";
constexpr const char * c_emoji    = "🎮🎲";

// The same four scripts in one string, the parts separated by three spaces.
constexpr const char * c_mixed = "player привет 日本語 🎮🎲";

// The shortest and the longest sequence each narrowed second-byte range admits: U+0800, U+D7FF, U+10000, U+10FFFF.
constexpr char c_lowestThreeByte[]  = {'\xE0', '\xA0', '\x80', '\0'};
constexpr char c_beforeSurrogates[] = {'\xED', '\x9F', '\xBF', '\0'};
constexpr char c_lowestFourByte[]   = {'\xF0', '\x90', '\x80', '\x80', '\0'};
constexpr char c_highestCodePoint[] = {'\xF4', '\x8F', '\xBF', '\xBF', '\0'};

// A continuation byte, which the lookup table maps to a length of zero because it starts no sequence.
constexpr const char * c_continuationByte = "\x80";

// A three-byte lead byte the terminator cuts short after a single continuation byte.
constexpr char c_truncatedSymbol[] = {'a', '\xE6', '\x97', '\0'};

// A two-byte lead byte followed by an ASCII character rather than a byte of the 10xxxxxx form.
constexpr char c_lonelyLead[] = {'\xC2', 'A', '\0'};

// The ill-formed sequences the lead byte alone admits: an overlong '/', the surrogate U+D800, and the code point one
// past U+10FFFF.
constexpr char c_overlongSymbol[]  = {'\xE0', '\x80', '\xAF', '\0'};
constexpr char c_surrogateSymbol[] = {'\xED', '\xA0', '\x80', '\0'};
constexpr char c_aboveRange[]      = {'\xF4', '\x90', '\x80', '\x80', '\0'};

} // namespace

// How many characters a UTF-8 string holds, against the bytes it occupies.
TEST_CASE("utf8_len/counts_characters_not_bytes") {
  CHECK(utf8Len(c_ascii) == char_traits<char>::length(c_ascii));
  CHECK(utf8Len(c_cyrillic) == char_traits<char>::length(c_cyrillic) / 2);
  CHECK(utf8Len(c_cjk) == char_traits<char>::length(c_cjk) / 3);
  CHECK(utf8Len(c_emoji) == char_traits<char>::length(c_emoji) / 4);

  // A mixed string counts every character once, whatever width each one is stored in.
  CHECK(utf8Len(c_mixed) == utf8Len(c_ascii) + utf8Len(c_cyrillic) + utf8Len(c_cjk) + utf8Len(c_emoji) + 3);

  CHECK(utf8Len("") == 0);
}

// Which sequences the narrowed second-byte ranges still accept, at both ends of every narrowed range.
TEST_CASE("utf8_len/counts_sequences_at_the_range_edges") {
  CHECK(utf8Len(c_lowestThreeByte) == 1);
  CHECK(utf8Len(c_beforeSurrogates) == 1);
  CHECK(utf8Len(c_lowestFourByte) == 1);
  CHECK(utf8Len(c_highestCodePoint) == 1);
}

// What a shipping build reports for input a debug build rejects with an assertion first.
//
// Compiled out where _DEBUG is defined: there the assertion runs, and the runner's callback leaves utf8Len() through a
// noexcept boundary, which ends the process instead of failing the case.
#ifndef _DEBUG

TEST_CASE("utf8_len/counts_nothing_for_malformed_input") {
  // A null pointer names no string to measure.
  CHECK(utf8Len(nullptr) == 0);

  // A byte that starts no sequence stops the walk before it counts anything.
  CHECK(utf8Len(c_continuationByte) == 0);

  // So does a sequence the terminator cuts short, however many characters preceded it.
  CHECK(utf8Len(c_truncatedSymbol) == 0);
}

#endif // _DEBUG

// Which strings the validator accepts: every encoding width and both ends of every narrowed range.
TEST_CASE("validate_utf8/accepts_well_formed_sequences") {
  CHECK(validateUtf8(c_ascii));
  CHECK(validateUtf8(c_cyrillic));
  CHECK(validateUtf8(c_cjk));
  CHECK(validateUtf8(c_emoji));
  CHECK(validateUtf8(c_mixed));

  CHECK(validateUtf8(c_lowestThreeByte));
  CHECK(validateUtf8(c_beforeSurrogates));
  CHECK(validateUtf8(c_lowestFourByte));
  CHECK(validateUtf8(c_highestCodePoint));

  // A string holding no character holds no ill-formed one either.
  CHECK(validateUtf8(""));
}

// Which strings it rejects, among them the sequences a lead byte alone admits.
TEST_CASE("validate_utf8/rejects_ill_formed_sequences") {
  // A null pointer names no string to check, which the validator answers rather than asserts.
  CHECK_FALSE(validateUtf8(nullptr));

  CHECK_FALSE(validateUtf8(c_continuationByte));
  CHECK_FALSE(validateUtf8(c_truncatedSymbol));

  // A trailing byte outside the 10xxxxxx form, whatever width the lead byte announced.
  CHECK_FALSE(validateUtf8(c_lonelyLead));

  // The three a narrowed second-byte range rejects: overlong, surrogate, and past U+10FFFF.
  CHECK_FALSE(validateUtf8(c_overlongSymbol));
  CHECK_FALSE(validateUtf8(c_surrogateSymbol));
  CHECK_FALSE(validateUtf8(c_aboveRange));
}

} // namespace toy
