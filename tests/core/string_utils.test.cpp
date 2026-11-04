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
  \file   string_utils.test.cpp
  \brief  Unit tests for the \ref toy::string_utils functions.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy::string_utils {

namespace {

// A literal whose groups repeat, so a search has more than one candidate to pick between.
constexpr const char * repeated               = "abracadabra";
constexpr size_t       repeatedLength         = std::char_traits<char>::length(repeated);
// The same literal with more after it, longer than the range the first one spans.
constexpr const char * repeatedLonger         = "abracadabra and then some";
// The same literal with its last character replaced, which only a comparison reading to the end can reject.
constexpr const char * repeatedLastDiffers    = "abracadabrx";
// The same again, differing in its first character and in one byte of its middle.
constexpr const char * repeatedFirstDiffers   = "xbracadabra";
constexpr const char * repeatedMiddleDiffers  = "abXacadabra";
// And again, differing in the byte just before the last, which a middle comparison short by one would skip.
constexpr const char * repeatedNearEndDiffers = "abracadabXa";
// A leading part of the literal that ends before its second "abra", 'b', and 'r'.
constexpr size_t       leadingLength          = 7;

// One character three times, then another just past the counted range.
constexpr const char * runOfA       = "aaab";
constexpr size_t       runOfALength = 3;

// A byte above the ASCII range inside a string, so a character-set search meets a value negative as a plain char.
constexpr const char * highByteInside       = "a\x{ff}"
                                              "b";
constexpr size_t       highByteInsideLength = std::char_traits<char>::length(highByteInside);

// The literal's characters without a terminator; reading past them fails constant evaluation.
constexpr char unterminated[] = {'a', 'b', 'r', 'a', 'c', 'a', 'd', 'a', 'b', 'r', 'a'};

// A terminator inside the range, which a measured length would stop at.
constexpr char embeddedNull[] = {'p', 'l', '\0', 'a', 'y'};

// The same ranges as spans, so the length travels with the pointer and a call cannot pair them wrongly.
constexpr span repeatedRange{repeated, repeatedLength};
constexpr span runOfARange{runOfA, runOfALength};
constexpr span unterminatedRange{unterminated};
constexpr span embeddedNullRange{embeddedNull};
constexpr span highByteInsideRange{highByteInside, highByteInsideLength};

// A literal as a measured range, its terminator left out, which is what a pattern or a set argument reads.
constexpr span<const char> asRange(const char * text) noexcept {
  return {text, std::char_traits<char>::length(text)};
}

constexpr span repeatedLongerRange         = asRange(repeatedLonger);
constexpr span repeatedFirstDiffersRange   = asRange(repeatedFirstDiffers);
constexpr span repeatedMiddleDiffersRange  = asRange(repeatedMiddleDiffers);
constexpr span repeatedNearEndDiffersRange = asRange(repeatedNearEndDiffers);
constexpr span repeatedLastDiffersRange    = asRange(repeatedLastDiffers);

// A null pointer naming bytes or units, which no literal turns into a span.
constexpr const char *     nullUtf8  = nullptr;
constexpr const char16_t * nullUtf16 = nullptr;
constexpr char *           nullBytes = nullptr;
constexpr char16_t *       nullUnits = nullptr;

// UTF-16 units as a measured range, the terminator code unit left out.
constexpr span<const char16_t> unitsRange(const char16_t * units) noexcept {
  return {units, std::char_traits<char16_t>::length(units)};
}

// Literals of one encoding width each, so an expected count derives from the byte length instead of a hand count.
constexpr const char * ascii    = "player";
constexpr const char * cyrillic = "привет";
constexpr const char * cjk      = "日本語";
constexpr const char * emoji    = "🎮🎲";

// The same four scripts in one string, the parts separated by three spaces.
constexpr const char * mixed = "player привет 日本語 🎮🎲";

// The shortest and the longest sequence each narrowed second-byte range admits: U+0800, U+D7FF, U+10000, U+10FFFF.
constexpr char lowestThreeByte[]  = {'\x{E0}', '\x{A0}', '\x{80}', '\0'};
constexpr char beforeSurrogates[] = {'\x{ED}', '\x{9F}', '\x{BF}', '\0'};
constexpr char lowestFourByte[]   = {'\x{F0}', '\x{90}', '\x{80}', '\x{80}', '\0'};
constexpr char highestCodePoint[] = {'\x{F4}', '\x{8F}', '\x{BF}', '\x{BF}', '\0'};

// A continuation byte, which the lookup table maps to a length of zero because it starts no sequence.
constexpr const char * continuationByte = "\x{80}";

// A three-byte lead byte the terminator cuts short after a single continuation byte.
constexpr char truncatedSymbol[] = {'a', '\x{E6}', '\x{97}', '\0'};

// A two-byte lead byte followed by an ASCII character rather than a byte of the 10xxxxxx form.
constexpr char lonelyLead[] = {'\x{C2}', 'A', '\0'};

// The ill-formed sequences the lead byte alone admits: an overlong '/', the surrogate U+D800, and the code point one
// past U+10FFFF.
constexpr char overlongSymbol[]  = {'\x{E0}', '\x{80}', '\x{AF}', '\0'};
constexpr char surrogateSymbol[] = {'\x{ED}', '\x{A0}', '\x{80}', '\0'};
constexpr char aboveRange[]      = {'\x{F4}', '\x{90}', '\x{80}', '\x{80}', '\0'};

// A terminator between two ASCII characters, which a counted range reads as one more character.
constexpr char innerTerminator[] = {'a', '\0', 'b'};

// The UTF-16 counterpart of innerTerminator.
constexpr char16_t utf16InnerTerminator[] = {u'a', u'\0', u'b'};

// A UTF-16 literal and the UTF-8 literal of the same text.
struct Utf16Sample {
  const char16_t * utf16;
  const char *     utf8;
};

// UTF-16 literals of one encoding width each, then all four widths in one string.
constexpr array<Utf16Sample, 5> utf16Samples{
  {
   {u"player", ascii},
   {u"привет", cyrillic},
   {u"日本語", cjk},
   {u"🎮🎲", emoji},
   {u"player привет 日本語 🎮🎲", mixed},
   }
};

// Surrogates outside a pair after an 'a': high before ASCII, high at the end, lone low, and a reversed pair.
constexpr array<array<char16_t, 4>, 4> loneSurrogates{
  {
   {u'a', 0xD83C, u'b', u'\0'},
   {u'a', 0xD83C, u'\0', u'\0'},
   {u'a', 0xDFAE, u'b', u'\0'},
   {u'a', 0xDFAE, 0xD83C, u'\0'},
   }
};

// The units at both ends of the surrogate range, alone or in a pair: U+D7FF, U+E000, U+10000, U+10FFFF.
constexpr char16_t utf16BeforeSurrogates[] = {0xD7FF, u'\0'};
constexpr char16_t utf16AfterSurrogates[]  = {0xE000, u'\0'};
constexpr char16_t utf16LowestPair[]       = {0xD800, 0xDC00, u'\0'};
constexpr char16_t utf16HighestCodePoint[] = {0xDBFF, 0xDFFF, u'\0'};

// The sequences toy::string_utils::validateUtf8() rejects, the three a narrowed second-byte range catches among them.
constexpr array<const char *, 5> rejectedSequences{continuationByte, lonelyLead, overlongSymbol, surrogateSymbol,
                                                   aboveRange};

// A byte the conversion never writes, so a surviving one shows where the call stopped.
constexpr char sentinel = '#';

// The UTF-16 counterpart of sentinel, for buffers of char16_t.
constexpr char16_t utf16Sentinel = u'#';

// The decimal text toy::string_utils::itoa() writes, moved to the front of a fixed array so it compares as a literal.
template <typename T>
constexpr auto decimalText(T value) noexcept {
  array<char, std::numeric_limits<T>::digits10 + 3> buffer{};
  const char * const                                start = string_utils::itoa(span{buffer}, value);

  array<char, 24> text{};
  for (size_t index = 0; start[index] != '\0'; ++index)
    text[index] = start[index];

  return text;
}

// The decimal text toy::string_utils::utoa() writes into destSize bytes, moved to the front the same way.
template <std::unsigned_integral T>
constexpr auto unsignedText(T value, size_t destSize) noexcept {
  array<char, 24>    buffer{};
  const char * const start = string_utils::utoa(span{buffer}.first(destSize), value);

  array<char, 24> text{};
  for (size_t index = 0; start[index] != '\0'; ++index)
    text[index] = start[index];

  return text;
}

// The hexadecimal text toy::string_utils::utoaHex() writes into destSize bytes, moved to the front the same way.
template <std::unsigned_integral T>
constexpr auto hexText(T value, size_t destSize) noexcept {
  array<char, 24>    buffer{};
  const char * const start = string_utils::utoaHex(span{buffer}.first(destSize), value);

  array<char, 24> text{};
  for (size_t index = 0; start[index] != '\0'; ++index)
    text[index] = start[index];

  return text;
}

} // namespace

// Where a character first occurs, counting from the offset the search starts at.
TEST_CASE("string_utils/find/first_character_at_or_after_offset") {
  CHECK(string_utils::find(repeatedRange, 'a', 0) == 0);
  CHECK(string_utils::find(repeatedRange, 'a', 1) == 3);
  CHECK(string_utils::find(repeatedRange, 'd', 0) == 6);
  CHECK(string_utils::find(repeatedRange, 'z', 0) == npos);

  // A start at or past the end matches nowhere, and so does any start in an empty range.
  CHECK(string_utils::find(repeatedRange, 'a', repeatedLength) == npos);
  CHECK(string_utils::find(repeatedRange, 'a', npos) == npos);
  CHECK(string_utils::find(repeatedRange.first(0), 'a', 0) == npos);

  static_assert(string_utils::find(repeatedRange, 'a', 0) == 0, "the first occurrence wins");
  static_assert(string_utils::find(repeatedRange, 'a', 1) == 3, "the offset skips earlier matches");
  static_assert(string_utils::find(repeatedRange, 'z', 0) == npos, "an absent character reports npos");
  static_assert(string_utils::find(repeatedRange, 'a', repeatedLength) == npos, "a start at the end matches nowhere");
  static_assert(string_utils::find(repeatedRange, 'c', 0) == 4, "'c' sits at offset 4 of \"abracadabra\"");
  static_assert(string_utils::find(span<const char>{}, 'a', 0) == npos, "a range with no data holds no character");
}

// Where the first match of a character sequence starts, counting from the offset the search starts at.
TEST_CASE("string_utils/find/first_sequence_at_or_after_offset") {
  CHECK(string_utils::find(repeatedRange, asRange("abra"), 0) == 0);
  CHECK(string_utils::find(repeatedRange, asRange("abra"), 1) == 7);
  CHECK(string_utils::find(repeatedRange, asRange("abra"), 8) == npos);
  CHECK(string_utils::find(repeatedRange, asRange("cad"), 0) == 4);
  CHECK(string_utils::find(repeatedRange, asRange("xyz"), 0) == npos);

  // A pattern longer than the range matches nowhere, decided on the lengths before any character is read.
  CHECK(string_utils::find(repeatedRange, repeatedLongerRange, 0) == npos);

  // An empty pattern matches at the start offset, up to the end of the range and no further.
  CHECK(string_utils::find(repeatedRange, asRange(""), 0) == 0);
  CHECK(string_utils::find(repeatedRange, asRange(""), 3) == 3);
  CHECK(string_utils::find(repeatedRange, asRange(""), repeatedLength) == repeatedLength);
  CHECK(string_utils::find(repeatedRange, asRange(""), repeatedLength + 1) == npos);

  // The pattern's own length bounds how much of it the search reads.
  CHECK(string_utils::find(repeatedRange, asRange("abrasive").first(4), 0) == 0);
  CHECK(string_utils::find(repeatedRange, asRange("abrasive").first(4), 1) == 7);

  // A pattern of one matches at the last position, where the comparison behind it reads nothing.
  CHECK(string_utils::find(repeatedRange, asRange("a"), repeatedLength - 1) == repeatedLength - 1);

  // A candidate sharing only its first characters must not end the scan.
  CHECK(string_utils::find(repeatedRange, asRange("abz"), 0) == npos);

  // A pattern the size of the range leaves a single candidate, which its last character decides.
  CHECK(string_utils::find(repeatedRange, repeatedRange, 0) == 0);
  CHECK(string_utils::find(repeatedRange, repeatedLastDiffersRange, 0) == npos);

  static_assert(string_utils::find(repeatedRange, asRange("abra"), 0) == 0, "the first match wins");
  static_assert(string_utils::find(repeatedRange, asRange("abra"), 1) == 7, "the offset skips earlier matches");
  static_assert(string_utils::find(repeatedRange, asRange("xyz"), 0) == npos, "an absent pattern reports npos");
  static_assert(string_utils::find(repeatedRange, repeatedLongerRange, 0) == npos,
                "a pattern longer than the range reports npos");
  static_assert(string_utils::find(repeatedRange, asRange(""), 3) == 3, "an empty pattern matches at the start offset");
  static_assert(string_utils::find(repeatedRange, asRange(""), repeatedLength + 1) == npos,
                "an empty pattern matches nowhere past the end");
  static_assert(string_utils::find(repeatedRange, asRange("abrasive").first(4), 1) == 7,
                "the pattern length bounds what is read");
  static_assert(string_utils::find(repeatedRange, asRange("a"), repeatedLength - 1) == repeatedLength - 1,
                "a pattern of one matches at the last position");
  static_assert(string_utils::find(repeatedRange, asRange("abz"), 0) == npos,
                "a candidate sharing only its first characters is rejected");
  static_assert(string_utils::find(repeatedRange, repeatedRange, 0) == 0,
                "a pattern the size of the range matches at the front");
  static_assert(string_utils::find(repeatedRange, repeatedLastDiffersRange, 0) == npos,
                "a pattern differing in its last character matches nowhere");

  // The range ends where its length says, so a terminator inside it is just another byte to match.
  static_assert(string_utils::find(embeddedNullRange, span{"\0", 1}, 0) == 2,
                "a pattern holding one terminator byte matches inside the range");
}

// Where a character last occurs, at or before the offset the search starts at.
TEST_CASE("string_utils/rfind/last_character_at_or_before_offset") {
  CHECK(string_utils::rfind(repeatedRange, 'a', npos) == repeatedLength - 1);
  CHECK(string_utils::rfind(repeatedRange, 'a', 9) == 7);
  CHECK(string_utils::rfind(repeatedRange, 'd', npos) == 6);
  CHECK(string_utils::rfind(repeatedRange, 'd', 5) == npos);
  CHECK(string_utils::rfind(repeatedRange, 'z', npos) == npos);

  // An offset of zero leaves the first character as the only candidate.
  CHECK(string_utils::rfind(repeatedRange, 'b', 0) == npos);
  CHECK(string_utils::rfind(repeatedRange, 'a', 0) == 0);

  // Any offset past the range covers all of it, and an empty range holds no candidate.
  CHECK(string_utils::rfind(repeatedRange, 'a', repeatedLength + 5) == repeatedLength - 1);
  CHECK(string_utils::rfind(repeatedRange.first(0), 'a', npos) == npos);

  static_assert(string_utils::rfind(repeatedRange, 'a', npos) == repeatedLength - 1, "the last occurrence wins");
  static_assert(string_utils::rfind(repeatedRange, 'a', 9) == 7, "the offset caps how far back a match may sit");
  static_assert(string_utils::rfind(repeatedRange, 'z', npos) == npos, "an absent character reports npos");
  static_assert(string_utils::rfind(repeatedRange, 'b', 0) == npos,
                "an offset of zero leaves only the first character");
  static_assert(string_utils::rfind(repeatedRange, 'c', npos) == 4, "'c' occurs once, at offset 4 of \"abracadabra\"");
  static_assert(string_utils::rfind(span<const char>{}, 'a', npos) == npos, "a range with no data holds no character");
}

// Where the last match of a character sequence starts, at or before the offset the search starts at.
TEST_CASE("string_utils/rfind/last_sequence_at_or_before_offset") {
  CHECK(string_utils::rfind(repeatedRange, asRange("abra"), npos) == 7);
  CHECK(string_utils::rfind(repeatedRange, asRange("abra"), 7) == 7);
  CHECK(string_utils::rfind(repeatedRange, asRange("abra"), 6) == 0);
  CHECK(string_utils::rfind(repeatedRange, asRange("abra"), 0) == 0);
  CHECK(string_utils::rfind(repeatedRange, asRange("cad"), npos) == 4);
  CHECK(string_utils::rfind(repeatedRange, asRange("cad"), 3) == npos);
  CHECK(string_utils::rfind(repeatedRange, asRange("xyz"), npos) == npos);
  CHECK(string_utils::rfind(repeatedRange, repeatedLongerRange, npos) == npos);

  // An empty pattern matches at the offset, capped at the end of the range.
  CHECK(string_utils::rfind(repeatedRange, asRange(""), 5) == 5);
  CHECK(string_utils::rfind(repeatedRange, asRange(""), repeatedLength) == repeatedLength);
  CHECK(string_utils::rfind(repeatedRange, asRange(""), repeatedLength + 1) == repeatedLength);
  CHECK(string_utils::rfind(repeatedRange, asRange(""), npos) == repeatedLength);

  // The pattern's own length bounds how much of it the search reads.
  CHECK(string_utils::rfind(repeatedRange, asRange("abrasive").first(4), npos) == 7);

  // A pattern the size of the range leaves one candidate, which any of its three parts can reject.
  CHECK(string_utils::rfind(repeatedRange, repeatedRange, npos) == 0);
  CHECK(string_utils::rfind(repeatedRange, repeatedFirstDiffersRange, npos) == npos);
  CHECK(string_utils::rfind(repeatedRange, repeatedMiddleDiffersRange, npos) == npos);
  CHECK(string_utils::rfind(repeatedRange, repeatedNearEndDiffersRange, npos) == npos);
  CHECK(string_utils::rfind(repeatedRange, repeatedLastDiffersRange, npos) == npos);

  // Patterns of one and two bytes, where the comparison behind the ends has nothing left to read.
  CHECK(string_utils::rfind(repeatedRange, asRange("a"), npos) == repeatedLength - 1);
  CHECK(string_utils::rfind(repeatedRange, asRange("ab"), npos) == 7);

  static_assert(string_utils::rfind(repeatedRange, asRange("abra"), npos) == 7, "the last match wins");
  static_assert(string_utils::rfind(repeatedRange, asRange("abra"), 6) == 0, "the offset caps where a match starts");
  static_assert(string_utils::rfind(repeatedRange, asRange("xyz"), npos) == npos, "an absent pattern reports npos");
  static_assert(string_utils::rfind(repeatedRange, repeatedLongerRange, npos) == npos,
                "a pattern longer than the range reports npos");
  static_assert(string_utils::rfind(repeatedRange, asRange(""), repeatedLength) == repeatedLength
                  && string_utils::rfind(repeatedRange, asRange(""), repeatedLength + 1) == repeatedLength,
                "an empty pattern must match at the end for an offset at or past it");
  static_assert(string_utils::rfind(repeatedRange, asRange(""), npos) == repeatedLength,
                "an empty pattern matches at the end of the range");
  static_assert(string_utils::rfind(repeatedRange, asRange("abrasive").first(4), npos) == 7,
                "the pattern length bounds what is read");
  static_assert(string_utils::rfind(repeatedRange, repeatedRange, npos) == 0,
                "a pattern the size of the range matches at the front");
  static_assert(string_utils::rfind(repeatedRange, repeatedFirstDiffersRange, npos) == npos,
                "a pattern differing in its first character matches nowhere");
  static_assert(string_utils::rfind(repeatedRange, repeatedMiddleDiffersRange, npos) == npos,
                "a pattern differing in its middle matches nowhere");
  static_assert(string_utils::rfind(repeatedRange, repeatedNearEndDiffersRange, npos) == npos,
                "a pattern differing just before its last character matches nowhere");
  static_assert(string_utils::rfind(repeatedRange, repeatedLastDiffersRange, npos) == npos,
                "a pattern differing in its last character matches nowhere");
  static_assert(string_utils::rfind(repeatedRange, asRange("ab"), npos) == 7,
                "a pattern of two is decided by its ends");
}

// Which character first belongs to a set, counting from the offset the search starts at.
TEST_CASE("string_utils/find_first_of/first_member_of_set") {
  CHECK(string_utils::findFirstOf(repeatedRange, asRange("rc"), 0) == 2);
  CHECK(string_utils::findFirstOf(repeatedRange, asRange("rc"), 3) == 4);
  CHECK(string_utils::findFirstOf(repeatedRange, asRange("rc"), 10) == npos);
  CHECK(string_utils::findFirstOf(repeatedRange, asRange("xyz"), 0) == npos);

  // The count bounds how much of the set is read.
  CHECK(string_utils::findFirstOf(repeatedRange, asRange("rcxyz").first(2), 0) == 2);
  CHECK(string_utils::findFirstOf(repeatedRange, asRange("rcxyz").first(2), 3) == 4);

  // An empty set holds no character to match, unlike an empty pattern in a sequence search.
  CHECK(string_utils::findFirstOf(repeatedRange, asRange(""), 0) == npos);

  // Neither does a start past the end, nor an empty range.
  CHECK(string_utils::findFirstOf(repeatedRange, asRange("a"), repeatedLength) == npos);
  CHECK(string_utils::findFirstOf(repeatedRange.first(0), asRange("a"), 0) == npos);

  static_assert(string_utils::findFirstOf(repeatedRange, asRange("rc"), 0) == 2, "the first member of the set wins");
  static_assert(string_utils::findFirstOf(repeatedRange, asRange("rc"), 3) == 4, "the offset skips earlier matches");
  static_assert(string_utils::findFirstOf(repeatedRange, asRange("rcxyz").first(2), 3) == 4,
                "the count bounds what is read");
  static_assert(string_utils::findFirstOf(repeatedRange, asRange(""), 0) == npos, "an empty set matches nothing");
  static_assert(string_utils::findFirstOf(repeatedRange, span<const char>{}, 0) == npos,
                "a set with no data holds no character");
}

// Which character last belongs to a set, at or before the offset the search starts at.
TEST_CASE("string_utils/find_last_of/last_member_of_set") {
  CHECK(string_utils::findLastOf(repeatedRange, asRange("rc"), npos) == 9);
  CHECK(string_utils::findLastOf(repeatedRange, asRange("rc"), 8) == 4);
  CHECK(string_utils::findLastOf(repeatedRange, asRange("rc"), 1) == npos);
  CHECK(string_utils::findLastOf(repeatedRange, asRange("xyz"), npos) == npos);

  // The count bounds how much of the set is read.
  CHECK(string_utils::findLastOf(repeatedRange, asRange("rcxyz").first(2), npos) == 9);
  CHECK(string_utils::findLastOf(repeatedRange, asRange("rcxyz").first(2), 8) == 4);

  // An empty set matches nothing, and an empty range holds nothing to match.
  CHECK(string_utils::findLastOf(repeatedRange, asRange(""), npos) == npos);
  CHECK(string_utils::findLastOf(repeatedRange.first(0), asRange("a"), npos) == npos);

  static_assert(string_utils::findLastOf(repeatedRange, asRange("rc"), npos) == 9, "the last member of the set wins");
  static_assert(string_utils::findLastOf(repeatedRange, asRange("rc"), 8) == 4,
                "the offset caps how far back a match may sit");
  static_assert(string_utils::findLastOf(repeatedRange, asRange("rcxyz").first(2), 8) == 4,
                "the count bounds what is read");
  static_assert(string_utils::findLastOf(repeatedRange, asRange(""), npos) == npos, "an empty set matches nothing");
}

// Which character first falls outside a single character or a set, counting from the start offset.
TEST_CASE("string_utils/find_first_not_of/first_character_outside") {
  CHECK(string_utils::findFirstNotOf(repeatedRange, 'a', 0) == 1);
  CHECK(string_utils::findFirstNotOf(repeatedRange, 'a', 3) == 4);
  CHECK(string_utils::findFirstNotOf(runOfARange, 'a', 0) == npos);

  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange("ab"), 0) == 2);
  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange("ab"), 3) == 4);
  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange("abr"), 0) == 4);
  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange("ab"), 10) == npos);

  // A set holding every character the range uses leaves nothing to report.
  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange("abcdr"), 0) == npos);

  // The count bounds how much of the set is read.
  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange("abxyz").first(2), 0) == 2);
  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange("abxyz").first(2), 3) == 4);

  // No character belongs to an empty set, so the start offset matches while it lies inside the range.
  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange(""), 0) == 0);
  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange(""), 2) == 2);
  CHECK(string_utils::findFirstNotOf(repeatedRange, asRange(""), repeatedLength) == npos);
  CHECK(string_utils::findFirstNotOf(repeatedRange.first(0), asRange(""), 0) == npos);

  static_assert(string_utils::findFirstNotOf(repeatedRange, 'a', 0) == 1,
                "the first character other than the skipped one wins");
  static_assert(string_utils::findFirstNotOf(runOfARange, 'a', 0) == npos,
                "a range of the skipped character reports npos");
  static_assert(string_utils::findFirstNotOf(span<const char>{}, 'a', 0) == npos,
                "a range with no data has no character to report");
  static_assert(string_utils::findFirstNotOf(repeatedRange, asRange("ab"), 3) == 4,
                "the offset skips earlier characters");
  static_assert(string_utils::findFirstNotOf(repeatedRange, asRange("abcdr"), 0) == npos,
                "a set covering every character reports npos");
  static_assert(string_utils::findFirstNotOf(repeatedRange, asRange("abxyz").first(2), 3) == 4,
                "the count bounds what is read");
  static_assert(string_utils::findFirstNotOf(repeatedRange, asRange(""), 2) == 2, "an empty set excludes nothing");
  static_assert(string_utils::findFirstNotOf(repeatedRange, span<const char>{}, 0) == 0,
                "a set with no data excludes nothing either");
}

// Which character last falls outside a single character or a set, at or before the start offset.
TEST_CASE("string_utils/find_last_not_of/last_character_outside") {
  CHECK(string_utils::findLastNotOf(repeatedRange, 'a', npos) == 9);
  CHECK(string_utils::findLastNotOf(repeatedRange, 'a', 8) == 8);
  CHECK(string_utils::findLastNotOf(runOfARange, 'a', npos) == npos);

  CHECK(string_utils::findLastNotOf(repeatedRange, asRange("ab"), npos) == 9);
  CHECK(string_utils::findLastNotOf(repeatedRange, asRange("ab"), 8) == 6);
  CHECK(string_utils::findLastNotOf(repeatedRange, asRange("ab"), 1) == npos);
  CHECK(string_utils::findLastNotOf(repeatedRange, asRange("abcdr"), npos) == npos);

  // The count bounds how much of the set is read.
  CHECK(string_utils::findLastNotOf(repeatedRange, asRange("abxyz").first(2), npos) == 9);
  CHECK(string_utils::findLastNotOf(repeatedRange, asRange("abxyz").first(2), 8) == 6);

  // No character belongs to an empty set, so the offset matches, capped at the last character of the range.
  CHECK(string_utils::findLastNotOf(repeatedRange, asRange(""), npos) == repeatedLength - 1);
  CHECK(string_utils::findLastNotOf(repeatedRange, asRange(""), 2) == 2);
  CHECK(string_utils::findLastNotOf(repeatedRange.first(0), asRange(""), npos) == npos);

  static_assert(string_utils::findLastNotOf(repeatedRange, 'a', npos) == 9,
                "the last character other than the skipped one wins");
  static_assert(string_utils::findLastNotOf(span<const char>{}, 'a', npos) == npos,
                "a range with no data has no last character");
  static_assert(string_utils::findLastNotOf(repeatedRange, asRange("ab"), 8) == 6,
                "the offset caps how far back a match may sit");
  static_assert(string_utils::findLastNotOf(repeatedRange, asRange("abcdr"), npos) == npos,
                "a set covering every character reports npos");
  static_assert(string_utils::findLastNotOf(repeatedRange, asRange("abxyz").first(2), 8) == 6,
                "the count bounds what is read");
  static_assert(string_utils::findLastNotOf(repeatedRange, asRange(""), npos) == repeatedLength - 1,
                "an empty set excludes nothing");
  static_assert(string_utils::findLastNotOf(repeatedRange.first(0), asRange(""), npos) == npos,
                "an empty range has no last character");
}

// A byte above the ASCII range reaches a character-set search, where a signed index would read outside the table.
TEST_CASE("string_utils/search/matches_high_byte_in_set") {
  CHECK(string_utils::findFirstOf(highByteInsideRange, asRange("\x{FF}"), 0) == 1);
  CHECK(string_utils::findLastOf(highByteInsideRange, asRange("\x{FF}"), npos) == 1);
  CHECK(string_utils::findFirstNotOf(highByteInsideRange, asRange("a\x{FF}"), 0) == 2);
  CHECK(string_utils::findLastNotOf(highByteInsideRange, asRange("b\x{FF}"), npos) == 0);

  static_assert(string_utils::findFirstOf(highByteInsideRange, asRange("\x{FF}"), 0) == 1,
                "a high byte is found in a set");
  static_assert(string_utils::findLastOf(highByteInsideRange, asRange("\x{FF}"), npos) == 1,
                "a high byte is found scanning backwards");
  static_assert(string_utils::findFirstNotOf(highByteInsideRange, asRange("a\x{FF}"), 0) == 2,
                "a high byte counts as a member of the set");
  static_assert(string_utils::findLastNotOf(highByteInsideRange, asRange("b\x{FF}"), npos) == 0,
                "a high byte counts as a member scanning backwards");
}

// What a search sees of a buffer the length cuts short: nothing past it, and no terminator required.
TEST_CASE("string_utils/search/reads_only_the_counted_range") {
  // Every match below exists in the whole literal, but only past the leading part the length names.
  CHECK(string_utils::find(repeatedRange.first(leadingLength), 'b', 2) == npos);
  CHECK(string_utils::find(repeatedRange.first(leadingLength), asRange("abra"), 1) == npos);
  CHECK(string_utils::rfind(repeatedRange.first(leadingLength), 'a', npos) == 5);
  CHECK(string_utils::rfind(repeatedRange.first(leadingLength), asRange("abra"), npos) == 0);
  CHECK(string_utils::findFirstOf(repeatedRange.first(leadingLength), asRange("r"), 3) == npos);
  CHECK(string_utils::findLastOf(repeatedRange.first(leadingLength), asRange("br"), npos) == 2);
  CHECK(string_utils::findFirstNotOf(runOfARange, asRange("a"), 0) == npos);
  CHECK(string_utils::findLastNotOf(runOfARange, asRange("a"), npos) == npos);

  // Constant evaluation rejects a read past the array, so each assertion proves the search stays inside it.
  static_assert(string_utils::find(unterminatedRange, 'z', 0) == npos, "a full scan must stop at the end of the range");
  static_assert(string_utils::find(unterminatedRange, asRange("bra"), 2) == 8,
                "a sequence search must stop at the end of the range");
  static_assert(string_utils::rfind(unterminatedRange, asRange("bra"), npos) == 8,
                "a backward sequence search must start inside the range");
  static_assert(string_utils::findFirstOf(unterminatedRange, asRange("z"), 0) == npos,
                "a set search must stop at the end of the range");
  static_assert(string_utils::findLastNotOf(unterminatedRange, 'a', npos) == 9,
                "a backward search must start at the last character");
}

// What a search reads of its pattern or set: exactly the count, so either may end without a terminator.
TEST_CASE("string_utils/search/reads_only_the_counted_pattern") {
  // The pattern and the one-byte sets end where the array does, so a read past the count runs off it.
  constexpr const char * lastByte = unterminated + std::size(unterminated) - 1;

  CHECK(string_utils::find(repeatedLongerRange, unterminatedRange, 0) == 0);
  CHECK(string_utils::rfind(repeatedLongerRange, unterminatedRange, npos) == 0);
  CHECK(string_utils::findFirstOf(repeatedRange, span{lastByte, 1}, 1) == 3);
  CHECK(string_utils::findLastOf(repeatedRange, span{lastByte, 1}, npos) == 10);
  CHECK(string_utils::findFirstNotOf(repeatedRange, span{lastByte, 1}, 0) == 1);
  CHECK(string_utils::findLastNotOf(repeatedRange, span{lastByte, 1}, npos) == 9);

  static_assert(string_utils::find(repeatedLongerRange, unterminatedRange, 0) == 0,
                "a forward search must read the pattern no further than its count");
  static_assert(string_utils::rfind(repeatedLongerRange, unterminatedRange, npos) == 0,
                "a backward search must read the pattern no further than its count");
  static_assert(string_utils::findFirstOf(repeatedRange, span{lastByte, 1}, 1) == 3,
                "a set search must read the set no further than its count");
  static_assert(string_utils::findLastOf(repeatedRange, span{lastByte, 1}, npos) == 10,
                "a backward set search must read the set no further than its count");
  static_assert(string_utils::findFirstNotOf(repeatedRange, span{lastByte, 1}, 0) == 1,
                "a search outside a set must read the set no further than its count");
  static_assert(string_utils::findLastNotOf(repeatedRange, span{lastByte, 1}, npos) == 9,
                "a backward search outside a set must read the set no further than its count");
}

// How a terminator inside the range compares: like any other character, in a pattern, in a set, and as a skip.
TEST_CASE("string_utils/search/matches_embedded_null") {
  CHECK(string_utils::find(embeddedNullRange, '\0', 0) == 2);
  CHECK(string_utils::find(embeddedNullRange, 'a', 0) == 3);
  CHECK(string_utils::rfind(embeddedNullRange, span{"\0a", 2}, npos) == 2);
  CHECK(string_utils::findFirstOf(embeddedNullRange, span{"y\0", 2}, 0) == 2);
  CHECK(string_utils::findFirstNotOf(embeddedNullRange, asRange("pl"), 0) == 2);
  CHECK(string_utils::findLastNotOf(embeddedNullRange, 'y', npos) == 3);

  static_assert(string_utils::find(embeddedNullRange, 'a', 0) == 3,
                "a search must continue past a terminator inside the range");
  static_assert(string_utils::rfind(embeddedNullRange, span{"\0a", 2}, npos) == 2,
                "a terminator must match inside a sequence");
  static_assert(string_utils::findFirstOf(embeddedNullRange, span{"y\0", 2}, 0) == 2,
                "a terminator must count as a member of a set");
}

// What a null pointer with a zero length yields: the empty-range or empty-set answer, with nothing read.
TEST_CASE("string_utils/search/accepts_null_with_zero_length") {
  CHECK(string_utils::find(span<const char>{}, asRange("a"), 0) == npos);
  CHECK(string_utils::find(span<const char>{}, span<const char>{}, 0) == 0);
  CHECK(string_utils::rfind(span<const char>{}, span<const char>{}, npos) == 0);
  CHECK(string_utils::findLastOf(span<const char>{}, asRange("a"), npos) == npos);
  CHECK(string_utils::findLastNotOf(span<const char>{}, span<const char>{}, npos) == npos);

  // A null pattern or set with a zero count, searched for in a non-empty range.
  CHECK(string_utils::find(repeatedRange, span<const char>{}, 4) == 4);
  CHECK(string_utils::findFirstNotOf(repeatedRange, span<const char>{}, 2) == 2);
  CHECK(string_utils::findLastNotOf(repeatedRange, span<const char>{}, npos) == repeatedLength - 1);

  static_assert(string_utils::find(span<const char>{}, span<const char>{}, 0) == 0,
                "an empty pattern matches at the start of an empty range");
  static_assert(string_utils::findFirstNotOf(repeatedRange, span<const char>{}, 2) == 2,
                "a null set with a zero count excludes nothing");
}

// How many characters a range encodes: one per byte outside the continuation form, read over exactly the range.
TEST_CASE("string_utils/utf8_size/counts_characters_in_range") {
  constexpr size_t cyrillicLength = std::char_traits<char>::length(cyrillic);

  CHECK(string_utils::utf8Size(repeatedRange) == repeatedLength);
  CHECK(string_utils::utf8Size(asRange(cyrillic)) == cyrillicLength / 2);
  CHECK(string_utils::utf8Size(asRange(cyrillic).first(4)) == 2);
  CHECK(string_utils::utf8Size(span<const char>{}) == 0);

  // A null byte inside the range is a character of its own, not an end.
  CHECK(string_utils::utf8Size(embeddedNullRange) == std::size(embeddedNull));

  static_assert(string_utils::utf8Size(repeatedRange) == repeatedLength, "an ASCII range counts one per byte");
  static_assert(string_utils::utf8Size(asRange(cyrillic)) == cyrillicLength / 2, "a two-byte character counts once");
  static_assert(string_utils::utf8Size(asRange(cyrillic).first(4)) == 2, "only the bytes the range names must count");
  static_assert(string_utils::utf8Size(span<const char>{}) == 0, "an empty range must count nothing");
  static_assert(string_utils::utf8Size(embeddedNullRange) == std::size(embeddedNull),
                "a null byte inside the range must count as a character");
  static_assert(string_utils::utf8Size(unterminatedRange) == std::size(unterminated),
                "the count must stop at the end of the range");
}

// Two ranges holding the same characters compare equal, whichever buffers hold them.
TEST_CASE("string_utils/compare/equal_ranges") {
  CHECK(string_utils::compare(repeatedRange, repeatedRange) == 0);
  CHECK(string_utils::compare(repeatedRange, unterminatedRange) == 0);

  // Two empty ranges compare equal, a null pointer with a zero length among them.
  CHECK(string_utils::compare(repeatedRange.first(0), asRange("")) == 0);
  CHECK(string_utils::compare(span<const char>{}, span<const char>{}) == 0);

  static_assert(string_utils::compare(repeatedRange, unterminatedRange) == 0,
                "the same characters in another buffer must compare equal");
  static_assert(string_utils::compare(span<const char>{}, span<const char>{}) == 0,
                "two empty ranges must compare equal");
}

// The first differing character decides the order, whatever the lengths, and swapping the ranges flips it.
TEST_CASE("string_utils/compare/first_difference_decides") {
  CHECK(string_utils::compare(repeatedRange, repeatedFirstDiffersRange) < 0);
  CHECK(string_utils::compare(repeatedFirstDiffersRange, repeatedRange) > 0);
  CHECK(string_utils::compare(repeatedRange, repeatedMiddleDiffersRange) > 0);
  CHECK(string_utils::compare(repeatedRange, repeatedLastDiffersRange) < 0);

  // A difference in the first character outweighs a shorter length.
  CHECK(string_utils::compare(asRange("b"), repeatedRange) > 0);

  // Bytes order as unsigned char, so a byte above the ASCII range follows an ASCII one.
  CHECK(string_utils::compare(span{highByteInside + 1, 1}, span{highByteInside, 1}) > 0);

  static_assert(string_utils::compare(repeatedRange, repeatedFirstDiffersRange) < 0,
                "a smaller first character must order first");
  static_assert(string_utils::compare(repeatedFirstDiffersRange, repeatedRange) > 0,
                "swapping the ranges must flip the order");
  static_assert(string_utils::compare(repeatedRange, repeatedLastDiffersRange) < 0,
                "a difference in the last character must still decide");
  static_assert(string_utils::compare(asRange("b"), repeatedRange) > 0,
                "the first difference must outweigh the lengths");
  static_assert(string_utils::compare(span{highByteInside + 1, 1}, span{highByteInside, 1}) > 0,
                "a high byte must order after an ASCII one");
}

// When one range is a prefix of the other, the shorter one orders first.
TEST_CASE("string_utils/compare/shorter_prefix_orders_first") {
  CHECK(string_utils::compare(repeatedRange, repeatedLongerRange) < 0);
  CHECK(string_utils::compare(repeatedLongerRange, repeatedRange) > 0);
  CHECK(string_utils::compare(repeatedRange.first(leadingLength), repeatedRange) < 0);

  // An empty range is a prefix of every other one.
  CHECK(string_utils::compare(span<const char>{}, repeatedRange) < 0);
  CHECK(string_utils::compare(repeatedRange, asRange("")) > 0);

  static_assert(string_utils::compare(repeatedRange, repeatedLongerRange) < 0,
                "a prefix must order before the longer range");
  static_assert(string_utils::compare(repeatedLongerRange, repeatedRange) > 0,
                "the longer range must order after its prefix");
  static_assert(string_utils::compare(span<const char>{}, repeatedRange) < 0, "an empty range must order first");
}

// What a comparison sees of its ranges: the counted characters only, a null byte among them included.
TEST_CASE("string_utils/compare/reads_only_the_counted_ranges") {
  // The lengths cut both literals before they part, so the parts compare equal.
  CHECK(string_utils::compare(repeatedRange.first(leadingLength), repeatedLastDiffersRange.first(leadingLength)) == 0);

  // A null byte compares as a character ordering before any letter, where a terminator scan would stop.
  CHECK(string_utils::compare(embeddedNullRange, asRange("pl")) > 0);
  CHECK(string_utils::compare(embeddedNullRange, asRange("pla")) < 0);

  static_assert(string_utils::compare(repeatedRange.first(leadingLength), repeatedLastDiffersRange.first(leadingLength))
                  == 0,
                "characters past the lengths must not take part");
  static_assert(string_utils::compare(embeddedNullRange, asRange("pla")) < 0,
                "a null byte must compare as a character");

  // Constant evaluation rejects a read past the array, so the comparison stays inside both ranges.
  static_assert(string_utils::compare(unterminatedRange, repeatedLongerRange) < 0,
                "the comparison must stop at the end of the shorter range");
}

// How many characters a UTF-8 string holds, against the bytes it occupies.
TEST_CASE("string_utils/utf8_len/counts_characters_not_bytes") {
  CHECK(string_utils::utf8Len(ascii) == std::char_traits<char>::length(ascii));
  CHECK(string_utils::utf8Len(cyrillic) == std::char_traits<char>::length(cyrillic) / 2);
  CHECK(string_utils::utf8Len(cjk) == std::char_traits<char>::length(cjk) / 3);
  CHECK(string_utils::utf8Len(emoji) == std::char_traits<char>::length(emoji) / 4);

  // A mixed string counts every character once, whatever width each one is stored in.
  CHECK(string_utils::utf8Len(mixed)
        == string_utils::utf8Len(ascii) + string_utils::utf8Len(cyrillic) + string_utils::utf8Len(cjk)
             + string_utils::utf8Len(emoji) + 3);

  CHECK(string_utils::utf8Len("") == 0);
}

// Which sequences the narrowed second-byte ranges still accept, at both ends of every narrowed range.
TEST_CASE("string_utils/utf8_len/counts_sequences_at_the_range_edges") {
  CHECK(string_utils::utf8Len(lowestThreeByte) == 1);
  CHECK(string_utils::utf8Len(beforeSurrogates) == 1);
  CHECK(string_utils::utf8Len(lowestFourByte) == 1);
  CHECK(string_utils::utf8Len(highestCodePoint) == 1);
}

// A surrogate pair counts as one character, so every sample holds as many characters as its UTF-8 form.
TEST_CASE("string_utils/utf16_len/counts_characters_not_units") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];

    CHECK(string_utils::utf16Len(utf16) == string_utils::utf8Len(utf8));
  }

  CHECK(string_utils::utf16Len(u"🎮🎲") == std::char_traits<char16_t>::length(u"🎮🎲") / 2);
  CHECK(string_utils::utf16Len(u"") == 0);
}

// A character at either edge of the surrogate range counts once, whether it takes one unit or a pair.
TEST_CASE("string_utils/utf16_len/counts_units_at_the_surrogate_edges") {
  CHECK(string_utils::utf16Len(utf16BeforeSurrogates) == 1);
  CHECK(string_utils::utf16Len(utf16AfterSurrogates) == 1);
  CHECK(string_utils::utf16Len(utf16LowestPair) == 1);
  CHECK(string_utils::utf16Len(utf16HighestCodePoint) == 1);
}

// Which strings the validator accepts: every encoding width and both ends of every narrowed range.
TEST_CASE("string_utils/validate_utf8/accepts_well_formed_sequences") {
  CHECK(string_utils::validateUtf8(ascii));
  CHECK(string_utils::validateUtf8(cyrillic));
  CHECK(string_utils::validateUtf8(cjk));
  CHECK(string_utils::validateUtf8(emoji));
  CHECK(string_utils::validateUtf8(mixed));

  CHECK(string_utils::validateUtf8(lowestThreeByte));
  CHECK(string_utils::validateUtf8(beforeSurrogates));
  CHECK(string_utils::validateUtf8(lowestFourByte));
  CHECK(string_utils::validateUtf8(highestCodePoint));

  // A string holding no character holds no ill-formed one either.
  CHECK(string_utils::validateUtf8(""));

  // The counted form accepts the same strings, measured by their length.
  CHECK(string_utils::validateUtf8(asRange(mixed)));
  CHECK(string_utils::validateUtf8(span{highestCodePoint}.first(std::size(highestCodePoint) - 1)));
  CHECK(string_utils::validateUtf8(span<const char>{}));
}

// Which strings it rejects, among them the sequences a lead byte alone admits.
TEST_CASE("string_utils/validate_utf8/rejects_ill_formed_sequences") {
  // A null pointer names no string to check, which the validator answers rather than asserts.
  CHECK_FALSE(string_utils::validateUtf8(nullptr));

  CHECK_FALSE(string_utils::validateUtf8(continuationByte));
  CHECK_FALSE(string_utils::validateUtf8(truncatedSymbol));

  // A trailing byte outside the 10xxxxxx form, whatever width the lead byte announced.
  CHECK_FALSE(string_utils::validateUtf8(lonelyLead));

  // The three a narrowed second-byte range rejects: overlong, surrogate, and past U+10FFFF.
  CHECK_FALSE(string_utils::validateUtf8(overlongSymbol));
  CHECK_FALSE(string_utils::validateUtf8(surrogateSymbol));
  CHECK_FALSE(string_utils::validateUtf8(aboveRange));

  // The counted form rejects the same sequences, each bounded by its length.
  CHECK_FALSE(string_utils::validateUtf8(span{continuationByte, 1}));
  CHECK_FALSE(string_utils::validateUtf8(span{lonelyLead}.first(std::size(lonelyLead) - 1)));
  CHECK_FALSE(string_utils::validateUtf8(span{overlongSymbol}.first(std::size(overlongSymbol) - 1)));
  CHECK_FALSE(string_utils::validateUtf8(span{surrogateSymbol}.first(std::size(surrogateSymbol) - 1)));
  CHECK_FALSE(string_utils::validateUtf8(span{aboveRange}.first(std::size(aboveRange) - 1)));
}

// A counted range ends at its length: bytes past it go unread, and a terminator inside it is an ordinary byte.
TEST_CASE("string_utils/validate_utf8/reads_only_the_counted_range") {
  // The ill-formed tail sits past the length, so the range holds the leading 'a' alone.
  CHECK(string_utils::validateUtf8(span{truncatedSymbol}.first(1)));

  // A length ending inside a sequence cuts it short, even where the bytes after it would complete it.
  CHECK_FALSE(string_utils::validateUtf8(asRange(cyrillic).first(1)));
  CHECK(string_utils::validateUtf8(asRange(cyrillic).first(2)));

  // A terminator inside the range is U+0000, a well-formed one-byte character.
  CHECK(string_utils::validateUtf8(span{innerTerminator}));
}

// Which strings the validator accepts: both widths and both ends of the surrogate range.
TEST_CASE("string_utils/validate_utf16/accepts_well_formed_strings") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const char16_t * const utf16 = utf16Samples[index].utf16;

    CHECK(string_utils::validateUtf16(utf16));
    CHECK(string_utils::validateUtf16(unitsRange(utf16)));
  }

  CHECK(string_utils::validateUtf16(utf16BeforeSurrogates));
  CHECK(string_utils::validateUtf16(utf16AfterSurrogates));
  CHECK(string_utils::validateUtf16(utf16LowestPair));
  CHECK(string_utils::validateUtf16(utf16HighestCodePoint));

  // A string holding no character holds no ill-formed one either.
  CHECK(string_utils::validateUtf16(u""));
  CHECK(string_utils::validateUtf16(span<const char16_t>{}));
}

// Which strings it rejects: a surrogate outside a pair, wherever it sits, and a null pointer.
TEST_CASE("string_utils/validate_utf16/rejects_ill_formed_strings") {
  // A null pointer names no string to check, which the validator answers rather than asserts.
  CHECK_FALSE(string_utils::validateUtf16(nullptr));

  for (size_t index = 0; index < loneSurrogates.size(); ++index) {
    INFO("index ", index);
    const char16_t * const units = loneSurrogates[index].data();

    CHECK_FALSE(string_utils::validateUtf16(units));
    CHECK_FALSE(string_utils::validateUtf16(unitsRange(units)));
  }
}

// A counted range ends at its length: units past it go unread, and a terminator inside it is an ordinary unit.
TEST_CASE("string_utils/validate_utf16/reads_only_the_counted_range") {
  // The high surrogate sits past the length, so the range holds the leading 'a' alone.
  CHECK(string_utils::validateUtf16(span{loneSurrogates[1].data(), 1}));

  // A length ending between the halves of a pair cuts it short, even where the unit after it would complete it.
  CHECK_FALSE(string_utils::validateUtf16(span{u"a🎮", 2}));
  CHECK(string_utils::validateUtf16(span{u"a🎮", 3}));

  // A terminator inside the range is U+0000, a well-formed one-unit character.
  CHECK(string_utils::validateUtf16(span{utf16InnerTerminator}));
}

// Every width converts to the bytes of the matching UTF-8 literal, and the call hands back the buffer it wrote.
TEST_CASE("string_utils/utf16_to_utf8/encodes_every_width") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];
    const auto length          = std::char_traits<char>::length(utf8);

    array<char, 64> buffer{};
    CHECK(string_utils::utf16ToUtf8(span{buffer}, unitsRange(utf16)) == buffer.data());
    CHECK(std::char_traits<char>::length(buffer.data()) == length);
    CHECK(std::char_traits<char>::compare(buffer.data(), utf8, length) == 0);
  }
}

// The unit count bounds the input: a terminator inside it is encoded, and a pair it cuts short ends the call.
TEST_CASE("string_utils/utf16_to_utf8/reads_only_the_counted_units") {
  array<char, 8> inner{sentinel, sentinel, sentinel, sentinel, sentinel};
  string_utils::utf16ToUtf8(span{inner}, span{utf16InnerTerminator});
  CHECK(inner[0] == 'a');
  CHECK(inner[1] == '\0');
  CHECK(inner[2] == 'b');
  CHECK(inner[3] == '\0');

  // Three units of a two-byte script encode three characters and leave the rest unread.
  const auto prefixBytes = std::char_traits<char>::length("при");

  array<char, 16> prefix{};
  string_utils::utf16ToUtf8(span{prefix}, span{u"привет", 3});
  CHECK(std::char_traits<char>::length(prefix.data()) == prefixBytes);
  CHECK(std::char_traits<char>::compare(prefix.data(), "при", prefixBytes) == 0);

  // A count ending between the halves of a pair keeps what precedes the pair and stops there.
  array<char, 16> cut{sentinel, sentinel, sentinel, sentinel};
  string_utils::utf16ToUtf8(span{cut}, span{u"a🎮", 2});
  CHECK(cut[0] == 'a');
  CHECK(cut[1] == '\0');
}

// A character that does not fit whole stays out, and the terminator always fits.
TEST_CASE("string_utils/utf16_to_utf8/stops_before_a_character_that_does_not_fit") {
  const auto length = std::char_traits<char>::length(cyrillic);
  const auto units  = std::char_traits<char16_t>::length(u"привет");

  // A buffer of exactly the encoded length plus the terminator holds the whole string.
  array<char, 32> exact{};
  string_utils::utf16ToUtf8(span{exact}.first(length + 1), span{u"привет", units});
  CHECK(std::char_traits<char>::length(exact.data()) == length);

  // One byte less leaves out the whole last two-byte character.
  array<char, 32> shorter{};
  string_utils::utf16ToUtf8(span{shorter}.first(length), span{u"привет", units});
  CHECK(std::char_traits<char>::length(shorter.data()) == length - 2);
  CHECK(std::char_traits<char>::compare(shorter.data(), cyrillic, length - 2) == 0);

  // ASCII and a three-byte script each take their own branch of the encoder and stop at a full buffer the same way.
  const auto asciiLength = std::char_traits<char>::length(ascii);

  array<char, 16> asciiShorter{};
  string_utils::utf16ToUtf8(span{asciiShorter}.first(asciiLength), unitsRange(u"player"));
  CHECK(std::char_traits<char>::length(asciiShorter.data()) == asciiLength - 1);
  CHECK(std::char_traits<char>::compare(asciiShorter.data(), ascii, asciiLength - 1) == 0);

  const auto cjkLength    = std::char_traits<char>::length(cjk);
  const auto cjkCharBytes = cjkLength / string_utils::utf8Len(cjk);

  array<char, 16> cjkShorter{};
  string_utils::utf16ToUtf8(span{cjkShorter}.first(cjkLength), unitsRange(u"日本語"));
  CHECK(std::char_traits<char>::length(cjkShorter.data()) == cjkLength - cjkCharBytes);
  CHECK(std::char_traits<char>::compare(cjkShorter.data(), cjk, cjkLength - cjkCharBytes) == 0);

  // A single byte holds the terminator alone, and nothing past the size is written.
  array<char, 4> single{sentinel, sentinel, sentinel, sentinel};
  string_utils::utf16ToUtf8(span{single}.first(1), span{u"привет", units});
  CHECK(single[0] == '\0');
  CHECK(single[1] == sentinel);
}

// A surrogate pair goes in as one four-byte sequence or not at all.
TEST_CASE("string_utils/utf16_to_utf8/writes_a_surrogate_pair_whole_or_not_at_all") {
  const auto pairBytes = std::char_traits<char>::length("🎮");
  const auto pairUnits = std::char_traits<char16_t>::length(u"🎮");

  array<char, 8> pairShort{sentinel, sentinel, sentinel, sentinel};
  string_utils::utf16ToUtf8(span{pairShort}.first(pairBytes), span{u"🎮", pairUnits});
  CHECK(pairShort[0] == '\0');

  array<char, 8> pairWhole{};
  string_utils::utf16ToUtf8(span{pairWhole}.first(pairBytes + 1), span{u"🎮", pairUnits});
  CHECK(std::char_traits<char>::compare(pairWhole.data(), "🎮", pairBytes + 1) == 0);
}

// A surrogate outside a pair encodes no code point, so the call keeps what precedes it and stops there.
TEST_CASE("string_utils/utf16_to_utf8/stops_before_a_lone_surrogate") {
  for (size_t index = 0; index < loneSurrogates.size(); ++index) {
    INFO("index ", index);

    const char16_t * const units = loneSurrogates[index].data();

    array<char, 16> buffer{};
    string_utils::utf16ToUtf8(span{buffer}, unitsRange(units));
    CHECK(std::char_traits<char>::length(buffer.data()) == 1);
    CHECK(buffer[0] == 'a');
  }
}

// A null or empty source converts as an empty string; a buffer that cannot hold the terminator gets no write at all.
TEST_CASE("string_utils/utf16_to_utf8/handles_null_and_empty_arguments") {
  array<char, 4> untouched{sentinel, sentinel, sentinel, sentinel};
  CHECK(string_utils::utf16ToUtf8(span{nullBytes, untouched.size()}, unitsRange(u"player").first(1)) == nullptr);
  CHECK(string_utils::utf16ToUtf8(span{untouched}.first(0), unitsRange(u"player").first(1)) == nullptr);
  CHECK(untouched[0] == sentinel);

  array<char, 4> fromNull{sentinel, sentinel, sentinel, sentinel};
  CHECK(string_utils::utf16ToUtf8(span{fromNull}, span{nullUtf16, 4}) == fromNull.data());
  CHECK(fromNull[0] == '\0');

  array<char, 4> fromEmpty{sentinel, sentinel, sentinel, sentinel};
  CHECK(string_utils::utf16ToUtf8(span{fromEmpty}, unitsRange(u"player").first(0)) == fromEmpty.data());
  CHECK(fromEmpty[0] == '\0');
}

// Every width measures the byte length of the matching UTF-8 literal.
TEST_CASE("string_utils/utf16_in_utf8_size/matches_every_width") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];

    CHECK(string_utils::utf16InUtf8Size(unitsRange(utf16)) == std::char_traits<char>::length(utf8));
  }
}

// Each width boundary counts on its side: U+007F one byte, U+0080 and U+07FF two, U+0800 three, a pair four.
TEST_CASE("string_utils/utf16_in_utf8_size/counts_the_width_edges") {
  constexpr char16_t widthEdges[] = {0x7F, 0x80, 0x7FF, 0x800};

  CHECK(string_utils::utf16InUtf8Size(span{&widthEdges[0], 1}) == 1);
  CHECK(string_utils::utf16InUtf8Size(span{&widthEdges[1], 1}) == 2);
  CHECK(string_utils::utf16InUtf8Size(span{&widthEdges[2], 1}) == 2);
  CHECK(string_utils::utf16InUtf8Size(span{&widthEdges[3], 1}) == 3);
  CHECK(string_utils::utf16InUtf8Size(span{utf16BeforeSurrogates}.first(1)) == 3);
  CHECK(string_utils::utf16InUtf8Size(span{utf16AfterSurrogates}.first(1)) == 3);
  CHECK(string_utils::utf16InUtf8Size(span{utf16LowestPair}.first(2)) == 4);
  CHECK(string_utils::utf16InUtf8Size(span{utf16HighestCodePoint}.first(2)) == 4);
}

// The unit count bounds the input: a terminator inside it counts one byte, and a pair it cuts short ends the count.
TEST_CASE("string_utils/utf16_in_utf8_size/reads_only_the_counted_units") {
  CHECK(string_utils::utf16InUtf8Size(span{utf16InnerTerminator}) == 3);
  CHECK(string_utils::utf16InUtf8Size(span{u"привет", 3}) == std::char_traits<char>::length("при"));
  CHECK(string_utils::utf16InUtf8Size(span{u"a🎮", 2}) == 1);
}

// A surrogate outside a pair ends the count where toy::string_utils::utf16ToUtf8() stops encoding.
TEST_CASE("string_utils/utf16_in_utf8_size/stops_before_a_lone_surrogate") {
  for (size_t index = 0; index < loneSurrogates.size(); ++index) {
    INFO("index ", index);

    const char16_t * const units = loneSurrogates[index].data();
    const auto             count = std::char_traits<char16_t>::length(units);

    array<char, 16> buffer{};
    string_utils::utf16ToUtf8(span{buffer}, span{units, count});
    CHECK(string_utils::utf16InUtf8Size(span{units, count}) == std::char_traits<char>::length(buffer.data()));
    CHECK(string_utils::utf16InUtf8Size(span{units, count}) == 1);
  }
}

// Over the units a decode wrote, the count gives the bytes the decode consumed before an ill-formed sequence.
TEST_CASE("string_utils/utf16_in_utf8_size/measures_the_bytes_a_decode_consumed") {
  for (size_t index = 0; index < rejectedSequences.size(); ++index) {
    INFO("index ", index);
    const char * const rejected = rejectedSequences[index];

    array<char, 64> input{};
    const auto      prefixLength = std::char_traits<char>::length(mixed);
    std::char_traits<char>::copy(input.data(), mixed, prefixLength);
    std::char_traits<char>::copy(input.data() + prefixLength, rejected, std::char_traits<char>::length(rejected));

    array<char16_t, 64> decoded{};
    string_utils::utf8ToUtf16(span{decoded}, asRange(input.data()));
    CHECK(string_utils::utf16InUtf8Size(unitsRange(decoded.data())) == prefixLength);
  }
}

// A null or empty source measures nothing, whatever the count.
TEST_CASE("string_utils/utf16_in_utf8_size/handles_null_and_empty_arguments") {
  CHECK(string_utils::utf16InUtf8Size(span<const char16_t>{}) == 0);
  CHECK(string_utils::utf16InUtf8Size(span{nullUtf16, 4}) == 0);
  CHECK(string_utils::utf16InUtf8Size(span{u"player", 0}) == 0);
}

// Every width decodes to the matching UTF-16 literal, a pair for four bytes, and the call hands back its buffer.
TEST_CASE("string_utils/utf8_to_utf16/decodes_every_width") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];
    const auto length          = std::char_traits<char16_t>::length(utf16);

    array<char16_t, 64> buffer{};
    CHECK(string_utils::utf8ToUtf16(span{buffer}, asRange(utf8)) == buffer.data());
    CHECK(std::char_traits<char16_t>::length(buffer.data()) == length);
    CHECK(std::char_traits<char16_t>::compare(buffer.data(), utf16, length) == 0);
  }
}

// The byte count bounds the input: a terminator inside it is decoded, and a sequence it cuts short ends the call.
TEST_CASE("string_utils/utf8_to_utf16/reads_only_the_counted_bytes") {
  array<char16_t, 8> inner{};
  string_utils::utf8ToUtf16(span{inner}, span{innerTerminator});
  CHECK(inner[0] == u'a');
  CHECK(inner[1] == u'\0');
  CHECK(inner[2] == u'b');
  CHECK(inner[3] == u'\0');

  // Three bytes of a two-byte script hold one whole character and the lead byte of the next.
  array<char16_t, 8> cut{};
  string_utils::utf8ToUtf16(span{cut}, span{cyrillic, 3});
  CHECK(std::char_traits<char16_t>::length(cut.data()) == 1);
  CHECK(cut[0] == u'п');
}

// A sequence the validator rejects ends the call, and the characters before it stay.
TEST_CASE("string_utils/utf8_to_utf16/stops_at_a_sequence_it_cannot_decode") {
  for (size_t index = 0; index < rejectedSequences.size(); ++index) {
    INFO("index ", index);
    const char * const sequence = rejectedSequences[index];

    array<char16_t, 4> buffer{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
    string_utils::utf8ToUtf16(span{buffer}, asRange(sequence));
    CHECK(buffer[0] == u'\0');
  }

  array<char16_t, 4> prefix{};
  string_utils::utf8ToUtf16(span{prefix}, span{truncatedSymbol}.first(std::size(truncatedSymbol) - 1));
  CHECK(prefix[0] == u'a');
  CHECK(prefix[1] == u'\0');
}

// A character that does not fit stays out, and the terminator always fits.
TEST_CASE("string_utils/utf8_to_utf16/stops_when_the_buffer_is_full") {
  const auto bytes  = std::char_traits<char>::length(cyrillic);
  const auto length = string_utils::utf8Len(cyrillic);

  // A buffer of exactly the character count plus the terminator holds the whole string.
  array<char16_t, 16> exact{};
  string_utils::utf8ToUtf16(span{exact}.first(length + 1), span{cyrillic, bytes});
  CHECK(std::char_traits<char16_t>::compare(exact.data(), u"привет", length) == 0);
  CHECK(exact[length] == u'\0');

  // One element less leaves out the last character.
  array<char16_t, 16> shorter{};
  string_utils::utf8ToUtf16(span{shorter}.first(length), span{cyrillic, bytes});
  CHECK(std::char_traits<char16_t>::length(shorter.data()) == length - 1);
  CHECK(std::char_traits<char16_t>::compare(shorter.data(), u"привет", length - 1) == 0);

  // ASCII takes a shorter path through the decoder and stops at a full buffer the same way.
  const auto asciiLength = std::char_traits<char>::length(ascii);

  array<char16_t, 16> asciiShorter{};
  string_utils::utf8ToUtf16(span{asciiShorter}.first(asciiLength), asRange(ascii));
  CHECK(std::char_traits<char16_t>::length(asciiShorter.data()) == asciiLength - 1);
  CHECK(std::char_traits<char16_t>::compare(asciiShorter.data(), u"player", asciiLength - 1) == 0);

  // A single element holds the terminator alone, and nothing past the size is written.
  array<char16_t, 4> single{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  string_utils::utf8ToUtf16(span{single}.first(1), span{cyrillic, bytes});
  CHECK(single[0] == u'\0');
  CHECK(single[1] == utf16Sentinel);
}

// A four-byte character goes in as a whole surrogate pair or not at all.
TEST_CASE("string_utils/utf8_to_utf16/writes_a_surrogate_pair_whole_or_not_at_all") {
  const auto pairBytes = std::char_traits<char>::length("🎮");
  const auto pairUnits = std::char_traits<char16_t>::length(u"🎮");

  array<char16_t, 4> pairShort{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  string_utils::utf8ToUtf16(span{pairShort}.first(pairUnits), span{"🎮", pairBytes});
  CHECK(pairShort[0] == u'\0');

  array<char16_t, 4> pairWhole{};
  string_utils::utf8ToUtf16(span{pairWhole}.first(pairUnits + 1), span{"🎮", pairBytes});
  CHECK(std::char_traits<char16_t>::compare(pairWhole.data(), u"🎮", pairUnits + 1) == 0);
}

// A null or empty source decodes as an empty string; a buffer that cannot hold the terminator gets no write at all.
TEST_CASE("string_utils/utf8_to_utf16/handles_null_and_empty_arguments") {
  array<char16_t, 4> untouched{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  CHECK(string_utils::utf8ToUtf16(span{nullUnits, untouched.size()}, asRange(ascii).first(1)) == nullptr);
  CHECK(string_utils::utf8ToUtf16(span{untouched}.first(0), asRange(ascii).first(1)) == nullptr);
  CHECK(untouched[0] == utf16Sentinel);

  array<char16_t, 4> fromNull{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  CHECK(string_utils::utf8ToUtf16(span{fromNull}, span{nullUtf8, 4}) == fromNull.data());
  CHECK(fromNull[0] == u'\0');

  array<char16_t, 4> fromEmpty{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  CHECK(string_utils::utf8ToUtf16(span{fromEmpty}, asRange(ascii).first(0)) == fromEmpty.data());
  CHECK(fromEmpty[0] == u'\0');
}

// A non-negative value prints its digits in order, zero included, and the string ends at the buffer's last byte.
TEST_CASE("string_utils/itoa/writes_non_negative_values_in_order") {
  array<char, 16> buffer{};

  const char * start = string_utils::itoa(span{buffer}, 0);
  CHECK(std::char_traits<char>::compare(start, "0", 2) == 0);
  CHECK(start + 2 == buffer.data() + buffer.size());

  // The digits arrive from the least significant, so the printed order shows no reversal step is missing.
  start = string_utils::itoa(span{buffer}, 12345);
  CHECK(std::char_traits<char>::compare(start, "12345", 6) == 0);

  start = string_utils::itoa(span{buffer}, std::numeric_limits<int32_t>::max());
  CHECK(std::char_traits<char>::compare(start, "2147483647", 11) == 0);
  CHECK(start + 11 == buffer.data() + buffer.size());

  static_assert(std::char_traits<char>::compare(decimalText(0).data(), "0", 2) == 0, "zero must print alone");
  static_assert(std::char_traits<char>::compare(decimalText(10).data(), "10", 3) == 0, "a ten must take two digits");
  static_assert(std::char_traits<char>::compare(decimalText(12345).data(), "12345", 6) == 0,
                "the digits must come out in order");
  static_assert(std::char_traits<char>::compare(decimalText<int32_t>(std::numeric_limits<int32_t>::max()).data(),
                                                "2147483647", 11)
                  == 0,
                "the largest value of the width must print in full");
}

// A minus before the digits of a negative value, and each type's minimum printed whole by the overflow guard.
TEST_CASE("string_utils/itoa/writes_a_minus_before_negative_values") {
  array<char, 16> buffer{};

  const char * start = string_utils::itoa(span{buffer}, -1);
  CHECK(std::char_traits<char>::compare(start, "-1", 3) == 0);
  CHECK(start + 3 == buffer.data() + buffer.size());

  start = string_utils::itoa(span{buffer}, -12345);
  CHECK(std::char_traits<char>::compare(start, "-12345", 7) == 0);

  array<char, 32> wide{};

  start = string_utils::itoa(span{wide}, std::numeric_limits<int32_t>::min());
  CHECK(std::char_traits<char>::compare(start, "-2147483648", 12) == 0);

  start = string_utils::itoa(span{wide}, std::numeric_limits<int64_t>::min());
  CHECK(std::char_traits<char>::compare(start, "-9223372036854775808", 21) == 0);

  static_assert(std::char_traits<char>::compare(decimalText(-1).data(), "-1", 3) == 0,
                "a negative value must start with a minus");
  static_assert(std::char_traits<char>::compare(decimalText(-12345).data(), "-12345", 7) == 0,
                "the sign must precede digits that come out in order");
  static_assert(std::char_traits<char>::compare(decimalText<int8_t>(std::numeric_limits<int8_t>::min()).data(), "-128",
                                                5)
                  == 0,
                "the smallest width must print its minimum in full");
  static_assert(std::char_traits<char>::compare(decimalText<int32_t>(std::numeric_limits<int32_t>::min()).data(),
                                                "-2147483648", 12)
                  == 0,
                "the middle width must print its minimum in full");
  static_assert(std::char_traits<char>::compare(decimalText<int64_t>(std::numeric_limits<int64_t>::min()).data(),
                                                "-9223372036854775808", 21)
                  == 0,
                "the widest width must print its minimum in full");
}

// itoa right-aligns in the window it is given and leaves the bytes before the string as the caller left them.
TEST_CASE("string_utils/itoa/keeps_the_bytes_before_the_string") {
  array<char, 16> buffer{};
  buffer.fill(sentinel);

  const char * start = string_utils::itoa(span{buffer}, 12345);
  CHECK(start == buffer.data() + 10);
  CHECK(buffer[9] == sentinel);
  CHECK(buffer[15] == '\0');

  // Twelve bytes is the minimum int32_t asserts, so this value fills the window from its first byte.
  array<char, 16> tight{};
  tight.fill(sentinel);

  start = string_utils::itoa(span{tight}.first(12), std::numeric_limits<int32_t>::min());
  CHECK(start == tight.data());
  CHECK(tight[11] == '\0');
  CHECK(tight[12] == sentinel);

  constexpr auto filled = [] {
    array<char, 16> text{};
    text.fill(sentinel);
    (void)string_utils::itoa(span{text}.first(12), std::numeric_limits<int32_t>::min());
    return text;
  }();

  static_assert(filled[0] == '-' && filled[11] == '\0', "the widest value must fit the asserted minimum whole");
  static_assert(filled[12] == sentinel, "a byte past the window must stay as the caller left it");
}

// The shortest decimal form of an unsigned value: no padding, a narrow window keeps the low digits.
TEST_CASE("string_utils/utoa/writes_the_shortest_decimal_form") {
  array<char, 16> buffer{};

  const char * start = string_utils::utoa(span{buffer}, uint32_t{0});
  CHECK(std::char_traits<char>::compare(start, "0", 2) == 0);
  CHECK(start + 2 == buffer.data() + buffer.size());

  // A value below a hundred takes both digits from the pair table in one read.
  start = string_utils::utoa(span{buffer}, uint32_t{42});
  CHECK(std::char_traits<char>::compare(start, "42", 3) == 0);
  CHECK(start + 3 == buffer.data() + buffer.size());

  start = string_utils::utoa(span{buffer}, UINT64_C(987654321));
  CHECK(std::char_traits<char>::compare(start, "987654321", 10) == 0);

  start = string_utils::utoa(span{buffer}.first(11), std::numeric_limits<uint32_t>::max());
  CHECK(std::char_traits<char>::compare(start, "4294967295", 11) == 0);
  CHECK(start == buffer.data());

  // A narrow window keeps its low digits; a one-byte window keeps the terminator.
  start = string_utils::utoa(span{buffer}.first(4), uint32_t{123456});
  CHECK(std::char_traits<char>::compare(start, "456", 4) == 0);
  CHECK(start == buffer.data());

  array<char, 1> oneByte{};
  oneByte.fill(sentinel);
  CHECK(string_utils::utoa(span{oneByte}, uint32_t{7}) == oneByte.data());
  CHECK(oneByte[0] == '\0');

  constexpr auto tenDigits = unsignedText(std::numeric_limits<uint32_t>::max(), 11);

  static_assert(std::char_traits<char>::compare(tenDigits.data(), "4294967295", 11) == 0,
                "the widest u32 value must fit its digit count exactly");
  static_assert(std::char_traits<char>::compare(unsignedText(uint8_t{200}, 4).data(), "200", 4) == 0,
                "u8 prints up to three digits");
  static_assert(std::char_traits<char>::compare(unsignedText(uint32_t{42}, 3).data(), "42", 3) == 0,
                "a two-digit value must print both of its digits");
  static_assert(std::char_traits<char>::compare(unsignedText(UINT64_C(1234567890123456789), 20).data(),
                                                "1234567890123456789", 20)
                  == 0,
                "u64 prints its widest value in full");

  // Four bytes hold terminator plus three digits: the leading half of 123456 goes missing, "456" stays.
  constexpr auto cut = unsignedText(uint32_t{123456}, 4);

  static_assert(std::char_traits<char>::compare(cut.data(), "456", 4) == 0,
                "a truncated value must keep its low digits in order");
  static_assert(std::char_traits<char>::compare(unsignedText(uint32_t{7}, 1).data(), "", 1) == 0,
                "a one-byte window holds only the terminator");
}

// Full-width uppercase hexadecimal of each unsigned type, zero-padded, without a prefix.
TEST_CASE("string_utils/utoa_hex/formats_the_full_width_of_the_type") {
  array<char, 9> byteWindow{};

  const char * start = string_utils::utoaHex(span{byteWindow}, uint8_t{0x0F});
  CHECK(std::char_traits<char>::compare(start, "0F", 3) == 0);
  CHECK(start + 3 == byteWindow.data() + byteWindow.size());

  array<char, 17> hexWindow{};

  start = string_utils::utoaHex(span{hexWindow}.first(9), 0x1234ABCDU);
  CHECK(std::char_traits<char>::compare(start, "1234ABCD", 9) == 0);
  CHECK(start == hexWindow.data());

  start = string_utils::utoaHex(span{hexWindow}, UINT64_C(0xDEADBEEFCAFEBABE));
  CHECK(std::char_traits<char>::compare(start, "DEADBEEFCAFEBABE", 17) == 0);
  CHECK(start + 17 == hexWindow.data() + hexWindow.size());

  static_assert(std::char_traits<char>::compare(hexText(uint16_t{0x0021}, 5).data(), "0021", 5) == 0,
                "u16 pads to four digits");
  static_assert(std::char_traits<char>::compare(hexText(std::numeric_limits<uint32_t>::max(), 9).data(), "FFFFFFFF", 9)
                  == 0,
                "u32 pads to eight digits");
  static_assert(std::char_traits<char>::compare(hexText(UINT64_C(1), 17).data(), "0000000000000001", 17) == 0,
                "u64 pads to sixteen digits");

  // Six bytes hold the terminator plus five digits: the two leading nibbles of 0x12345678 go missing.
  static_assert(std::char_traits<char>::compare(hexText(uint32_t{0x12345678}, 6).data(), "45678", 6) == 0,
                "a truncated window must keep its low digits");
  static_assert(std::char_traits<char>::compare(hexText(uint8_t{0xAB}, 1).data(), "", 1) == 0,
                "a one-byte window holds only the terminator");
}

} // namespace toy::string_utils
