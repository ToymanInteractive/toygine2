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
  \brief  Unit tests for the \ref toy::string_utils search functions.
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
constexpr const char * highByteInside       = "a\xff"
                                              "b";
constexpr size_t       highByteInsideLength = std::char_traits<char>::length(highByteInside);

// The literal's characters without a terminator; reading past them fails constant evaluation.
constexpr char unterminated[] = {'a', 'b', 'r', 'a', 'c', 'a', 'd', 'a', 'b', 'r', 'a'};

// A terminator inside the range, which a measured length would stop at.
constexpr char embeddedNull[] = {'p', 'l', '\0', 'a', 'y'};

// The same ranges as spans, so the length travels with the pointer and a call cannot pair them wrongly.
constexpr std::span<const char> repeatedRange{repeated, repeatedLength};
constexpr std::span<const char> runOfARange{runOfA, runOfALength};
constexpr std::span<const char> unterminatedRange{unterminated};
constexpr std::span<const char> embeddedNullRange{embeddedNull};
constexpr std::span<const char> highByteInsideRange{highByteInside, highByteInsideLength};

// A literal as a measured range, its terminator left out, which is what a pattern or a set argument reads.
constexpr std::span<const char> asRange(const char * text) noexcept {
  return {text, std::char_traits<char>::length(text)};
}

constexpr std::span<const char> repeatedLongerRange         = asRange(repeatedLonger);
constexpr std::span<const char> repeatedFirstDiffersRange   = asRange(repeatedFirstDiffers);
constexpr std::span<const char> repeatedMiddleDiffersRange  = asRange(repeatedMiddleDiffers);
constexpr std::span<const char> repeatedNearEndDiffersRange = asRange(repeatedNearEndDiffers);
constexpr std::span<const char> repeatedLastDiffersRange    = asRange(repeatedLastDiffers);

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
  static_assert(string_utils::find(std::span<const char>{}, 'a', 0) == npos, "a range with no data holds no character");
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

  // The pattern's own length bounds how much of it is read.
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
  static_assert(string_utils::find(embeddedNullRange, std::span<const char>{"\0", 1}, 0) == 2,
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
  static_assert(string_utils::rfind(std::span<const char>{}, 'a', npos) == npos,
                "a range with no data holds no character");
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

  // The pattern's own length bounds how much of it is read.
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

  // A terminator at the start of a pattern is one more byte of it, read for exactly the pattern's length.
  static_assert(string_utils::rfind(embeddedNullRange, std::span<const char>{"\0a", 2}, npos) == 2,
                "a pattern beginning with a terminator byte matches inside the range");
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
  static_assert(string_utils::findFirstOf(repeatedRange, std::span<const char>{}, 0) == npos,
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
  static_assert(string_utils::findFirstNotOf(std::span<const char>{}, 'a', 0) == npos,
                "a range with no data has no character to report");
  static_assert(string_utils::findFirstNotOf(repeatedRange, asRange("ab"), 3) == 4,
                "the offset skips earlier characters");
  static_assert(string_utils::findFirstNotOf(repeatedRange, asRange("abcdr"), 0) == npos,
                "a set covering every character reports npos");
  static_assert(string_utils::findFirstNotOf(repeatedRange, asRange("abxyz").first(2), 3) == 4,
                "the count bounds what is read");
  static_assert(string_utils::findFirstNotOf(repeatedRange, asRange(""), 2) == 2, "an empty set excludes nothing");
  static_assert(string_utils::findFirstNotOf(repeatedRange, std::span<const char>{}, 0) == 0,
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
  static_assert(string_utils::findLastNotOf(std::span<const char>{}, 'a', npos) == npos,
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
  CHECK(string_utils::findFirstOf(highByteInsideRange, asRange("\xff"), 0) == 1);
  CHECK(string_utils::findLastOf(highByteInsideRange, asRange("\xff"), npos) == 1);
  CHECK(string_utils::findFirstNotOf(highByteInsideRange, asRange("a\xff"), 0) == 2);
  CHECK(string_utils::findLastNotOf(highByteInsideRange, asRange("b\xff"), npos) == 0);

  static_assert(string_utils::findFirstOf(highByteInsideRange, asRange("\xff"), 0) == 1,
                "a high byte is found in a set");
  static_assert(string_utils::findLastOf(highByteInsideRange, asRange("\xff"), npos) == 1,
                "a high byte is found scanning backwards");
  static_assert(string_utils::findFirstNotOf(highByteInsideRange, asRange("a\xff"), 0) == 2,
                "a high byte counts as a member of the set");
  static_assert(string_utils::findLastNotOf(highByteInsideRange, asRange("b\xff"), npos) == 0,
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
  CHECK(string_utils::findFirstOf(repeatedRange, std::span<const char>{lastByte, 1}, 1) == 3);
  CHECK(string_utils::findLastOf(repeatedRange, std::span<const char>{lastByte, 1}, npos) == 10);
  CHECK(string_utils::findFirstNotOf(repeatedRange, std::span<const char>{lastByte, 1}, 0) == 1);
  CHECK(string_utils::findLastNotOf(repeatedRange, std::span<const char>{lastByte, 1}, npos) == 9);

  static_assert(string_utils::find(repeatedLongerRange, unterminatedRange, 0) == 0,
                "a forward search must read the pattern no further than its count");
  static_assert(string_utils::rfind(repeatedLongerRange, unterminatedRange, npos) == 0,
                "a backward search must read the pattern no further than its count");
  static_assert(string_utils::findFirstOf(repeatedRange, std::span<const char>{lastByte, 1}, 1) == 3,
                "a set search must read the set no further than its count");
  static_assert(string_utils::findLastOf(repeatedRange, std::span<const char>{lastByte, 1}, npos) == 10,
                "a backward set search must read the set no further than its count");
  static_assert(string_utils::findFirstNotOf(repeatedRange, std::span<const char>{lastByte, 1}, 0) == 1,
                "a search outside a set must read the set no further than its count");
  static_assert(string_utils::findLastNotOf(repeatedRange, std::span<const char>{lastByte, 1}, npos) == 9,
                "a backward search outside a set must read the set no further than its count");
}

// How a terminator inside the range compares: like any other character, in a pattern, in a set, and as a skip.
TEST_CASE("string_utils/search/matches_embedded_null") {
  CHECK(string_utils::find(embeddedNullRange, '\0', 0) == 2);
  CHECK(string_utils::find(embeddedNullRange, 'a', 0) == 3);
  CHECK(string_utils::rfind(embeddedNullRange, std::span<const char>{"\0a", 2}, npos) == 2);
  CHECK(string_utils::findFirstOf(embeddedNullRange, std::span<const char>{"y\0", 2}, 0) == 2);
  CHECK(string_utils::findFirstNotOf(embeddedNullRange, asRange("pl"), 0) == 2);
  CHECK(string_utils::findLastNotOf(embeddedNullRange, 'y', npos) == 3);

  static_assert(string_utils::find(embeddedNullRange, 'a', 0) == 3,
                "a search must continue past a terminator inside the range");
  static_assert(string_utils::rfind(embeddedNullRange, std::span<const char>{"\0a", 2}, npos) == 2,
                "a terminator must match inside a sequence");
  static_assert(string_utils::findFirstOf(embeddedNullRange, std::span<const char>{"y\0", 2}, 0) == 2,
                "a terminator must count as a member of a set");
}

// What a null pointer with a zero length yields: the empty-range or empty-set answer, with nothing read.
TEST_CASE("string_utils/search/accepts_null_with_zero_length") {
  CHECK(string_utils::find(std::span<const char>{}, 'a', 0) == npos);
  CHECK(string_utils::find(std::span<const char>{}, asRange("a"), 0) == npos);
  CHECK(string_utils::find(std::span<const char>{}, std::span<const char>{}, 0) == 0);
  CHECK(string_utils::rfind(std::span<const char>{}, 'a', npos) == npos);
  CHECK(string_utils::rfind(std::span<const char>{}, std::span<const char>{}, npos) == 0);
  CHECK(string_utils::findLastOf(std::span<const char>{}, asRange("a"), npos) == npos);
  CHECK(string_utils::findFirstNotOf(std::span<const char>{}, 'a', 0) == npos);
  CHECK(string_utils::findLastNotOf(std::span<const char>{}, std::span<const char>{}, npos) == npos);

  // A null pattern or set with a zero count, searched for in a non-empty range.
  CHECK(string_utils::find(repeatedRange, std::span<const char>{}, 4) == 4);
  CHECK(string_utils::findFirstOf(repeatedRange, std::span<const char>{}, 0) == npos);
  CHECK(string_utils::findFirstNotOf(repeatedRange, std::span<const char>{}, 2) == 2);
  CHECK(string_utils::findLastNotOf(repeatedRange, std::span<const char>{}, npos) == repeatedLength - 1);

  static_assert(string_utils::find(std::span<const char>{}, std::span<const char>{}, 0) == 0,
                "an empty pattern matches at the start of an empty range");
  static_assert(string_utils::rfind(std::span<const char>{}, 'a', npos) == npos, "an empty range holds no character");
  static_assert(string_utils::findFirstNotOf(repeatedRange, std::span<const char>{}, 2) == 2,
                "a null set with a zero count excludes nothing");
}

// How many characters a range encodes: one per byte outside the continuation form, read over exactly the range.
TEST_CASE("string_utils/utf8_size/counts_characters_in_range") {
  constexpr const char * cyrillic       = "привет";
  constexpr size_t       cyrillicLength = std::char_traits<char>::length(cyrillic);

  CHECK(string_utils::utf8Size(repeatedRange) == repeatedLength);
  CHECK(string_utils::utf8Size(asRange(cyrillic)) == cyrillicLength / 2);
  CHECK(string_utils::utf8Size(asRange(cyrillic).first(4)) == 2);
  CHECK(string_utils::utf8Size(std::span<const char>{}) == 0);

  // A null byte inside the range is a character of its own, not an end.
  CHECK(string_utils::utf8Size(embeddedNullRange) == std::size(embeddedNull));

  static_assert(string_utils::utf8Size(repeatedRange) == repeatedLength, "an ASCII range counts one per byte");
  static_assert(string_utils::utf8Size(asRange(cyrillic)) == cyrillicLength / 2, "a two-byte character counts once");
  static_assert(string_utils::utf8Size(asRange(cyrillic).first(4)) == 2, "only the bytes the range names must count");
  static_assert(string_utils::utf8Size(std::span<const char>{}) == 0, "an empty range must count nothing");
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
  CHECK(string_utils::compare(std::span<const char>{}, std::span<const char>{}) == 0);

  static_assert(string_utils::compare(repeatedRange, unterminatedRange) == 0,
                "the same characters in another buffer must compare equal");
  static_assert(string_utils::compare(std::span<const char>{}, std::span<const char>{}) == 0,
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
  CHECK(string_utils::compare(std::span<const char>{highByteInside + 1, 1}, std::span<const char>{highByteInside, 1})
        > 0);

  static_assert(string_utils::compare(repeatedRange, repeatedFirstDiffersRange) < 0,
                "a smaller first character must order first");
  static_assert(string_utils::compare(repeatedFirstDiffersRange, repeatedRange) > 0,
                "swapping the ranges must flip the order");
  static_assert(string_utils::compare(repeatedRange, repeatedLastDiffersRange) < 0,
                "a difference in the last character must still decide");
  static_assert(string_utils::compare(asRange("b"), repeatedRange) > 0,
                "the first difference must outweigh the lengths");
  static_assert(string_utils::compare(std::span<const char>{highByteInside + 1, 1},
                                      std::span<const char>{highByteInside, 1})
                  > 0,
                "a high byte must order after an ASCII one");
}

// When one range is a prefix of the other, the shorter one orders first.
TEST_CASE("string_utils/compare/shorter_prefix_orders_first") {
  CHECK(string_utils::compare(repeatedRange, repeatedLongerRange) < 0);
  CHECK(string_utils::compare(repeatedLongerRange, repeatedRange) > 0);
  CHECK(string_utils::compare(repeatedRange.first(leadingLength), repeatedRange) < 0);

  // An empty range is a prefix of every other one.
  CHECK(string_utils::compare(std::span<const char>{}, repeatedRange) < 0);
  CHECK(string_utils::compare(repeatedRange, asRange("")) > 0);

  static_assert(string_utils::compare(repeatedRange, repeatedLongerRange) < 0,
                "a prefix must order before the longer range");
  static_assert(string_utils::compare(repeatedLongerRange, repeatedRange) > 0,
                "the longer range must order after its prefix");
  static_assert(string_utils::compare(std::span<const char>{}, repeatedRange) < 0, "an empty range must order first");
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

} // namespace toy::string_utils
