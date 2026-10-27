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
constexpr size_t       repeatedLongerLength   = std::char_traits<char>::length(repeatedLonger);
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

} // namespace

// Where a character first occurs, counting from the offset the search starts at.
TEST_CASE("string_utils/find/first_character_at_or_after_offset") {
  CHECK(find(repeated, repeatedLength, 'a', 0) == 0);
  CHECK(find(repeated, repeatedLength, 'a', 1) == 3);
  CHECK(find(repeated, repeatedLength, 'd', 0) == 6);
  CHECK(find(repeated, repeatedLength, 'z', 0) == npos);

  // A start at or past the end matches nowhere, and so does any start in an empty range.
  CHECK(find(repeated, repeatedLength, 'a', repeatedLength) == npos);
  CHECK(find(repeated, repeatedLength, 'a', npos) == npos);
  CHECK(find(repeated, 0, 'a', 0) == npos);

  static_assert(find(repeated, repeatedLength, 'a', 0) == 0, "the first occurrence wins");
  static_assert(find(repeated, repeatedLength, 'a', 1) == 3, "the offset skips earlier matches");
  static_assert(find(repeated, repeatedLength, 'z', 0) == npos, "an absent character reports npos");
  static_assert(find(repeated, repeatedLength, 'a', repeatedLength) == npos, "a start at the end matches nowhere");
}

// Where the first match of a character sequence starts, counting from the offset the search starts at.
TEST_CASE("string_utils/find/first_sequence_at_or_after_offset") {
  CHECK(find(repeated, repeatedLength, "abra", 0, 4) == 0);
  CHECK(find(repeated, repeatedLength, "abra", 1, 4) == 7);
  CHECK(find(repeated, repeatedLength, "abra", 8, 4) == npos);
  CHECK(find(repeated, repeatedLength, "cad", 0, 3) == 4);
  CHECK(find(repeated, repeatedLength, "xyz", 0, 3) == npos);

  // A pattern longer than the range matches nowhere, decided on the lengths before any character is read.
  CHECK(find(repeated, repeatedLength, repeatedLonger, 0, repeatedLongerLength) == npos);

  // An empty pattern matches at the start offset, up to the end of the range and no further.
  CHECK(find(repeated, repeatedLength, "", 0, 0) == 0);
  CHECK(find(repeated, repeatedLength, "", 3, 0) == 3);
  CHECK(find(repeated, repeatedLength, "", repeatedLength, 0) == repeatedLength);
  CHECK(find(repeated, repeatedLength, "", repeatedLength + 1, 0) == npos);

  // The count bounds how much of the pattern is read.
  CHECK(find(repeated, repeatedLength, "abrasive", 0, 4) == 0);
  CHECK(find(repeated, repeatedLength, "abrasive", 1, 4) == 7);

  // A count of one matches at the last position, where the comparison behind the matched character reads nothing.
  CHECK(find(repeated, repeatedLength, "a", repeatedLength - 1, 1) == repeatedLength - 1);

  // A candidate sharing only its first characters must not end the scan.
  CHECK(find(repeated, repeatedLength, "abz", 0, 3) == npos);

  // A pattern the size of the range leaves a single candidate, which its last character decides.
  CHECK(find(repeated, repeatedLength, repeated, 0, repeatedLength) == 0);
  CHECK(find(repeated, repeatedLength, repeatedLastDiffers, 0, repeatedLength) == npos);

  static_assert(find(repeated, repeatedLength, "abra", 0, 4) == 0, "the first match wins");
  static_assert(find(repeated, repeatedLength, "abra", 1, 4) == 7, "the offset skips earlier matches");
  static_assert(find(repeated, repeatedLength, "xyz", 0, 3) == npos, "an absent pattern reports npos");
  static_assert(find(repeated, repeatedLength, repeatedLonger, 0, repeatedLongerLength) == npos,
                "a pattern longer than the range reports npos");
  static_assert(find(repeated, repeatedLength, "", 3, 0) == 3, "an empty pattern matches at the start offset");
  static_assert(find(repeated, repeatedLength, "", repeatedLength + 1, 0) == npos,
                "an empty pattern matches nowhere past the end");
  static_assert(find(repeated, repeatedLength, "abrasive", 1, 4) == 7, "the count bounds what is read");
  static_assert(find(repeated, repeatedLength, "a", repeatedLength - 1, 1) == repeatedLength - 1,
                "a count of one matches at the last position");
  static_assert(find(repeated, repeatedLength, "abz", 0, 3) == npos,
                "a candidate sharing only its first characters is rejected");
  static_assert(find(repeated, repeatedLength, repeated, 0, repeatedLength) == 0,
                "a pattern the size of the range matches at the front");
  static_assert(find(repeated, repeatedLength, repeatedLastDiffers, 0, repeatedLength) == npos,
                "a pattern differing in its last character matches nowhere");
}

// Where a character last occurs, at or before the offset the search starts at.
TEST_CASE("string_utils/rfind/last_character_at_or_before_offset") {
  CHECK(rfind(repeated, repeatedLength, 'a', npos) == repeatedLength - 1);
  CHECK(rfind(repeated, repeatedLength, 'a', 9) == 7);
  CHECK(rfind(repeated, repeatedLength, 'd', npos) == 6);
  CHECK(rfind(repeated, repeatedLength, 'd', 5) == npos);
  CHECK(rfind(repeated, repeatedLength, 'z', npos) == npos);

  // An offset of zero leaves the first character as the only candidate.
  CHECK(rfind(repeated, repeatedLength, 'b', 0) == npos);
  CHECK(rfind(repeated, repeatedLength, 'a', 0) == 0);

  // Any offset past the range covers all of it, and an empty range holds no candidate.
  CHECK(rfind(repeated, repeatedLength, 'a', repeatedLength + 5) == repeatedLength - 1);
  CHECK(rfind(repeated, 0, 'a', npos) == npos);

  static_assert(rfind(repeated, repeatedLength, 'a', npos) == repeatedLength - 1, "the last occurrence wins");
  static_assert(rfind(repeated, repeatedLength, 'a', 9) == 7, "the offset caps how far back a match may sit");
  static_assert(rfind(repeated, repeatedLength, 'z', npos) == npos, "an absent character reports npos");
  static_assert(rfind(repeated, repeatedLength, 'b', 0) == npos, "an offset of zero leaves only the first character");
}

// Where the last match of a character sequence starts, at or before the offset the search starts at.
TEST_CASE("string_utils/rfind/last_sequence_at_or_before_offset") {
  CHECK(rfind(repeated, repeatedLength, "abra", npos, 4) == 7);
  CHECK(rfind(repeated, repeatedLength, "abra", 7, 4) == 7);
  CHECK(rfind(repeated, repeatedLength, "abra", 6, 4) == 0);
  CHECK(rfind(repeated, repeatedLength, "abra", 0, 4) == 0);
  CHECK(rfind(repeated, repeatedLength, "cad", npos, 3) == 4);
  CHECK(rfind(repeated, repeatedLength, "cad", 3, 3) == npos);
  CHECK(rfind(repeated, repeatedLength, "xyz", npos, 3) == npos);
  CHECK(rfind(repeated, repeatedLength, repeatedLonger, npos, repeatedLongerLength) == npos);

  // An empty pattern matches at the offset, capped at the end of the range.
  CHECK(rfind(repeated, repeatedLength, "", 5, 0) == 5);
  CHECK(rfind(repeated, repeatedLength, "", repeatedLength, 0) == repeatedLength);
  CHECK(rfind(repeated, repeatedLength, "", repeatedLength + 1, 0) == repeatedLength);
  CHECK(rfind(repeated, repeatedLength, "", npos, 0) == repeatedLength);

  // The count bounds how much of the pattern is read.
  CHECK(rfind(repeated, repeatedLength, "abrasive", npos, 4) == 7);

  // A pattern the size of the range leaves one candidate, which any of its three parts can reject.
  CHECK(rfind(repeated, repeatedLength, repeated, npos, repeatedLength) == 0);
  CHECK(rfind(repeated, repeatedLength, repeatedFirstDiffers, npos, repeatedLength) == npos);
  CHECK(rfind(repeated, repeatedLength, repeatedMiddleDiffers, npos, repeatedLength) == npos);
  CHECK(rfind(repeated, repeatedLength, repeatedNearEndDiffers, npos, repeatedLength) == npos);
  CHECK(rfind(repeated, repeatedLength, repeatedLastDiffers, npos, repeatedLength) == npos);

  // Counts of one and two, where the comparison behind the ends has nothing left to read.
  CHECK(rfind(repeated, repeatedLength, "a", npos, 1) == repeatedLength - 1);
  CHECK(rfind(repeated, repeatedLength, "ab", npos, 2) == 7);

  static_assert(rfind(repeated, repeatedLength, "abra", npos, 4) == 7, "the last match wins");
  static_assert(rfind(repeated, repeatedLength, "abra", 6, 4) == 0, "the offset caps where a match starts");
  static_assert(rfind(repeated, repeatedLength, "xyz", npos, 3) == npos, "an absent pattern reports npos");
  static_assert(rfind(repeated, repeatedLength, repeatedLonger, npos, repeatedLongerLength) == npos,
                "a pattern longer than the range reports npos");
  static_assert(rfind(repeated, repeatedLength, "", repeatedLength, 0) == repeatedLength
                  && rfind(repeated, repeatedLength, "", repeatedLength + 1, 0) == repeatedLength,
                "an empty pattern must match at the end for an offset at or past it");
  static_assert(rfind(repeated, repeatedLength, "", npos, 0) == repeatedLength,
                "an empty pattern matches at the end of the range");
  static_assert(rfind(repeated, repeatedLength, "abrasive", npos, 4) == 7, "the count bounds what is read");
  static_assert(rfind(repeated, repeatedLength, repeated, npos, repeatedLength) == 0,
                "a pattern the size of the range matches at the front");
  static_assert(rfind(repeated, repeatedLength, repeatedFirstDiffers, npos, repeatedLength) == npos,
                "a pattern differing in its first character matches nowhere");
  static_assert(rfind(repeated, repeatedLength, repeatedMiddleDiffers, npos, repeatedLength) == npos,
                "a pattern differing in its middle matches nowhere");
  static_assert(rfind(repeated, repeatedLength, repeatedNearEndDiffers, npos, repeatedLength) == npos,
                "a pattern differing just before its last character matches nowhere");
  static_assert(rfind(repeated, repeatedLength, repeatedLastDiffers, npos, repeatedLength) == npos,
                "a pattern differing in its last character matches nowhere");
  static_assert(rfind(repeated, repeatedLength, "ab", npos, 2) == 7, "a count of two is decided by its ends");
}

// Which character first belongs to a set, counting from the offset the search starts at.
TEST_CASE("string_utils/find_first_of/first_member_of_set") {
  CHECK(findFirstOf(repeated, repeatedLength, "rc", 0, 2) == 2);
  CHECK(findFirstOf(repeated, repeatedLength, "rc", 3, 2) == 4);
  CHECK(findFirstOf(repeated, repeatedLength, "rc", 10, 2) == npos);
  CHECK(findFirstOf(repeated, repeatedLength, "xyz", 0, 3) == npos);

  // The count bounds how much of the set is read.
  CHECK(findFirstOf(repeated, repeatedLength, "rcxyz", 0, 2) == 2);
  CHECK(findFirstOf(repeated, repeatedLength, "rcxyz", 3, 2) == 4);

  // An empty set holds no character to match, unlike an empty pattern in a sequence search.
  CHECK(findFirstOf(repeated, repeatedLength, "", 0, 0) == npos);

  // Neither does a start past the end, nor an empty range.
  CHECK(findFirstOf(repeated, repeatedLength, "a", repeatedLength, 1) == npos);
  CHECK(findFirstOf(repeated, 0, "a", 0, 1) == npos);

  static_assert(findFirstOf(repeated, repeatedLength, "rc", 0, 2) == 2, "the first member of the set wins");
  static_assert(findFirstOf(repeated, repeatedLength, "rc", 3, 2) == 4, "the offset skips earlier matches");
  static_assert(findFirstOf(repeated, repeatedLength, "rcxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(findFirstOf(repeated, repeatedLength, "", 0, 0) == npos, "an empty set matches nothing");
}

// Which character last belongs to a set, at or before the offset the search starts at.
TEST_CASE("string_utils/find_last_of/last_member_of_set") {
  CHECK(findLastOf(repeated, repeatedLength, "rc", npos, 2) == 9);
  CHECK(findLastOf(repeated, repeatedLength, "rc", 8, 2) == 4);
  CHECK(findLastOf(repeated, repeatedLength, "rc", 1, 2) == npos);
  CHECK(findLastOf(repeated, repeatedLength, "xyz", npos, 3) == npos);

  // The count bounds how much of the set is read.
  CHECK(findLastOf(repeated, repeatedLength, "rcxyz", npos, 2) == 9);
  CHECK(findLastOf(repeated, repeatedLength, "rcxyz", 8, 2) == 4);

  // An empty set matches nothing, and an empty range holds nothing to match.
  CHECK(findLastOf(repeated, repeatedLength, "", npos, 0) == npos);
  CHECK(findLastOf(repeated, 0, "a", npos, 1) == npos);

  static_assert(findLastOf(repeated, repeatedLength, "rc", npos, 2) == 9, "the last member of the set wins");
  static_assert(findLastOf(repeated, repeatedLength, "rc", 8, 2) == 4, "the offset caps how far back a match may sit");
  static_assert(findLastOf(repeated, repeatedLength, "rcxyz", 8, 2) == 4, "the count bounds what is read");
  static_assert(findLastOf(repeated, repeatedLength, "", npos, 0) == npos, "an empty set matches nothing");
}

// Which character first falls outside a single character or a set, counting from the start offset.
TEST_CASE("string_utils/find_first_not_of/first_character_outside") {
  CHECK(findFirstNotOf(repeated, repeatedLength, 'a', 0) == 1);
  CHECK(findFirstNotOf(repeated, repeatedLength, 'a', 3) == 4);
  CHECK(findFirstNotOf(runOfA, runOfALength, 'a', 0) == npos);

  CHECK(findFirstNotOf(repeated, repeatedLength, "ab", 0, 2) == 2);
  CHECK(findFirstNotOf(repeated, repeatedLength, "ab", 3, 2) == 4);
  CHECK(findFirstNotOf(repeated, repeatedLength, "abr", 0, 3) == 4);
  CHECK(findFirstNotOf(repeated, repeatedLength, "ab", 10, 2) == npos);

  // A set holding every character the range uses leaves nothing to report.
  CHECK(findFirstNotOf(repeated, repeatedLength, "abcdr", 0, 5) == npos);

  // The count bounds how much of the set is read.
  CHECK(findFirstNotOf(repeated, repeatedLength, "abxyz", 0, 2) == 2);
  CHECK(findFirstNotOf(repeated, repeatedLength, "abxyz", 3, 2) == 4);

  // No character belongs to an empty set, so the start offset matches while it lies inside the range.
  CHECK(findFirstNotOf(repeated, repeatedLength, "", 0, 0) == 0);
  CHECK(findFirstNotOf(repeated, repeatedLength, "", 2, 0) == 2);
  CHECK(findFirstNotOf(repeated, repeatedLength, "", repeatedLength, 0) == npos);
  CHECK(findFirstNotOf(repeated, 0, "", 0, 0) == npos);

  static_assert(findFirstNotOf(repeated, repeatedLength, 'a', 0) == 1,
                "the first character other than the skipped one wins");
  static_assert(findFirstNotOf(runOfA, runOfALength, 'a', 0) == npos, "a range of the skipped character reports npos");
  static_assert(findFirstNotOf(repeated, repeatedLength, "ab", 3, 2) == 4, "the offset skips earlier characters");
  static_assert(findFirstNotOf(repeated, repeatedLength, "abcdr", 0, 5) == npos,
                "a set covering every character reports npos");
  static_assert(findFirstNotOf(repeated, repeatedLength, "abxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(findFirstNotOf(repeated, repeatedLength, "", 2, 0) == 2, "an empty set excludes nothing");
}

// Which character last falls outside a single character or a set, at or before the start offset.
TEST_CASE("string_utils/find_last_not_of/last_character_outside") {
  CHECK(findLastNotOf(repeated, repeatedLength, 'a', npos) == 9);
  CHECK(findLastNotOf(repeated, repeatedLength, 'a', 8) == 8);
  CHECK(findLastNotOf(runOfA, runOfALength, 'a', npos) == npos);

  CHECK(findLastNotOf(repeated, repeatedLength, "ab", npos, 2) == 9);
  CHECK(findLastNotOf(repeated, repeatedLength, "ab", 8, 2) == 6);
  CHECK(findLastNotOf(repeated, repeatedLength, "ab", 1, 2) == npos);
  CHECK(findLastNotOf(repeated, repeatedLength, "abcdr", npos, 5) == npos);

  // The count bounds how much of the set is read.
  CHECK(findLastNotOf(repeated, repeatedLength, "abxyz", npos, 2) == 9);
  CHECK(findLastNotOf(repeated, repeatedLength, "abxyz", 8, 2) == 6);

  // No character belongs to an empty set, so the offset matches, capped at the last character of the range.
  CHECK(findLastNotOf(repeated, repeatedLength, "", npos, 0) == repeatedLength - 1);
  CHECK(findLastNotOf(repeated, repeatedLength, "", 2, 0) == 2);
  CHECK(findLastNotOf(repeated, 0, "", npos, 0) == npos);

  static_assert(findLastNotOf(repeated, repeatedLength, 'a', npos) == 9,
                "the last character other than the skipped one wins");
  static_assert(findLastNotOf(repeated, repeatedLength, "ab", 8, 2) == 6,
                "the offset caps how far back a match may sit");
  static_assert(findLastNotOf(repeated, repeatedLength, "abcdr", npos, 5) == npos,
                "a set covering every character reports npos");
  static_assert(findLastNotOf(repeated, repeatedLength, "abxyz", 8, 2) == 6, "the count bounds what is read");
  static_assert(findLastNotOf(repeated, repeatedLength, "", npos, 0) == repeatedLength - 1,
                "an empty set excludes nothing");
  static_assert(findLastNotOf(repeated, 0, "", npos, 0) == npos, "an empty range has no last character");
}

// A byte above the ASCII range reaches a character-set search, where a signed index would read outside the table.
TEST_CASE("string_utils/search/matches_high_byte_in_set") {
  CHECK(findFirstOf(highByteInside, highByteInsideLength, "\xff", 0, 1) == 1);
  CHECK(findLastOf(highByteInside, highByteInsideLength, "\xff", npos, 1) == 1);
  CHECK(findFirstNotOf(highByteInside, highByteInsideLength, "a\xff", 0, 2) == 2);
  CHECK(findLastNotOf(highByteInside, highByteInsideLength, "b\xff", npos, 2) == 0);

  static_assert(findFirstOf(highByteInside, highByteInsideLength, "\xff", 0, 1) == 1, "a high byte is found in a set");
  static_assert(findLastOf(highByteInside, highByteInsideLength, "\xff", npos, 1) == 1,
                "a high byte is found scanning backwards");
  static_assert(findFirstNotOf(highByteInside, highByteInsideLength, "a\xff", 0, 2) == 2,
                "a high byte counts as a member of the set");
  static_assert(findLastNotOf(highByteInside, highByteInsideLength, "b\xff", npos, 2) == 0,
                "a high byte counts as a member scanning backwards");
}

// What a search sees of a buffer the length cuts short: nothing past it, and no terminator required.
TEST_CASE("string_utils/search/reads_only_the_counted_range") {
  // Every match below exists in the whole literal, but only past the leading part the length names.
  CHECK(find(repeated, leadingLength, 'b', 2) == npos);
  CHECK(find(repeated, leadingLength, "abra", 1, 4) == npos);
  CHECK(rfind(repeated, leadingLength, 'a', npos) == 5);
  CHECK(rfind(repeated, leadingLength, "abra", npos, 4) == 0);
  CHECK(findFirstOf(repeated, leadingLength, "r", 3, 1) == npos);
  CHECK(findLastOf(repeated, leadingLength, "br", npos, 2) == 2);
  CHECK(findFirstNotOf(runOfA, runOfALength, "a", 0, 1) == npos);
  CHECK(findLastNotOf(runOfA, runOfALength, "a", npos, 1) == npos);

  // Constant evaluation rejects a read past the array, so each assertion proves the search stays inside it.
  static_assert(find(unterminated, std::size(unterminated), 'z', 0) == npos,
                "a full scan must stop at the end of the range");
  static_assert(find(unterminated, std::size(unterminated), "bra", 2, 3) == 8,
                "a sequence search must stop at the end of the range");
  static_assert(rfind(unterminated, std::size(unterminated), "bra", npos, 3) == 8,
                "a backward sequence search must start inside the range");
  static_assert(findFirstOf(unterminated, std::size(unterminated), "z", 0, 1) == npos,
                "a set search must stop at the end of the range");
  static_assert(findLastNotOf(unterminated, std::size(unterminated), 'a', npos) == 9,
                "a backward search must start at the last character");
}

// What a search reads of its pattern or set: exactly the count, so either may end without a terminator.
TEST_CASE("string_utils/search/reads_only_the_counted_pattern") {
  // The pattern and the one-byte sets end where the array does, so a read past the count runs off it.
  constexpr const char * lastByte = unterminated + std::size(unterminated) - 1;

  CHECK(find(repeatedLonger, repeatedLongerLength, unterminated, 0, std::size(unterminated)) == 0);
  CHECK(rfind(repeatedLonger, repeatedLongerLength, unterminated, npos, std::size(unterminated)) == 0);
  CHECK(findFirstOf(repeated, repeatedLength, lastByte, 1, 1) == 3);
  CHECK(findLastOf(repeated, repeatedLength, lastByte, npos, 1) == 10);
  CHECK(findFirstNotOf(repeated, repeatedLength, lastByte, 0, 1) == 1);
  CHECK(findLastNotOf(repeated, repeatedLength, lastByte, npos, 1) == 9);

  static_assert(find(repeatedLonger, repeatedLongerLength, unterminated, 0, std::size(unterminated)) == 0,
                "a forward search must read the pattern no further than its count");
  static_assert(rfind(repeatedLonger, repeatedLongerLength, unterminated, npos, std::size(unterminated)) == 0,
                "a backward search must read the pattern no further than its count");
  static_assert(findFirstOf(repeated, repeatedLength, lastByte, 1, 1) == 3,
                "a set search must read the set no further than its count");
  static_assert(findLastOf(repeated, repeatedLength, lastByte, npos, 1) == 10,
                "a backward set search must read the set no further than its count");
  static_assert(findFirstNotOf(repeated, repeatedLength, lastByte, 0, 1) == 1,
                "a search outside a set must read the set no further than its count");
  static_assert(findLastNotOf(repeated, repeatedLength, lastByte, npos, 1) == 9,
                "a backward search outside a set must read the set no further than its count");
}

// How a terminator inside the range compares: like any other character, in a pattern, in a set, and as a skip.
TEST_CASE("string_utils/search/matches_embedded_null") {
  CHECK(find(embeddedNull, std::size(embeddedNull), '\0', 0) == 2);
  CHECK(find(embeddedNull, std::size(embeddedNull), 'a', 0) == 3);
  CHECK(rfind(embeddedNull, std::size(embeddedNull), "\0a", npos, 2) == 2);
  CHECK(findFirstOf(embeddedNull, std::size(embeddedNull), "y\0", 0, 2) == 2);
  CHECK(findFirstNotOf(embeddedNull, std::size(embeddedNull), "pl", 0, 2) == 2);
  CHECK(findLastNotOf(embeddedNull, std::size(embeddedNull), 'y', npos) == 3);

  static_assert(find(embeddedNull, std::size(embeddedNull), 'a', 0) == 3,
                "a search must continue past a terminator inside the range");
  static_assert(rfind(embeddedNull, std::size(embeddedNull), "\0a", npos, 2) == 2,
                "a terminator must match inside a sequence");
  static_assert(findFirstOf(embeddedNull, std::size(embeddedNull), "y\0", 0, 2) == 2,
                "a terminator must count as a member of a set");
}

// What a null pointer with a zero length yields: the empty-range or empty-set answer, with nothing read.
TEST_CASE("string_utils/search/accepts_null_with_zero_length") {
  CHECK(find(nullptr, 0, 'a', 0) == npos);
  CHECK(find(nullptr, 0, "a", 0, 1) == npos);
  CHECK(find(nullptr, 0, nullptr, 0, 0) == 0);
  CHECK(rfind(nullptr, 0, 'a', npos) == npos);
  CHECK(rfind(nullptr, 0, nullptr, npos, 0) == 0);
  CHECK(findLastOf(nullptr, 0, "a", npos, 1) == npos);
  CHECK(findFirstNotOf(nullptr, 0, 'a', 0) == npos);
  CHECK(findLastNotOf(nullptr, 0, nullptr, npos, 0) == npos);

  // A null pattern or set with a zero count, searched for in a non-empty range.
  CHECK(find(repeated, repeatedLength, nullptr, 4, 0) == 4);
  CHECK(findFirstOf(repeated, repeatedLength, nullptr, 0, 0) == npos);
  CHECK(findFirstNotOf(repeated, repeatedLength, nullptr, 2, 0) == 2);
  CHECK(findLastNotOf(repeated, repeatedLength, nullptr, npos, 0) == repeatedLength - 1);

  static_assert(find(nullptr, 0, nullptr, 0, 0) == 0, "an empty pattern matches at the start of an empty range");
  static_assert(rfind(nullptr, 0, 'a', npos) == npos, "an empty range holds no character");
  static_assert(findFirstNotOf(repeated, repeatedLength, nullptr, 2, 0) == 2,
                "a null set with a zero count excludes nothing");
}

// How many characters a range encodes: one per byte outside the continuation form, read over exactly the range.
TEST_CASE("string_utils/utf8_size/counts_characters_in_range") {
  constexpr const char * cyrillic       = "привет";
  constexpr size_t       cyrillicLength = std::char_traits<char>::length(cyrillic);

  CHECK(utf8Size(repeated, repeatedLength) == repeatedLength);
  CHECK(utf8Size(cyrillic, cyrillicLength) == cyrillicLength / 2);
  CHECK(utf8Size(cyrillic, 4) == 2);
  CHECK(utf8Size(nullptr, 0) == 0);

  // A null byte inside the range is a character of its own, not an end.
  CHECK(utf8Size(embeddedNull, std::size(embeddedNull)) == std::size(embeddedNull));

  static_assert(utf8Size(repeated, repeatedLength) == repeatedLength, "an ASCII range counts one per byte");
  static_assert(utf8Size(cyrillic, cyrillicLength) == cyrillicLength / 2, "a two-byte character counts once");
  static_assert(utf8Size(cyrillic, 4) == 2, "only the bytes the size names must count");
  static_assert(utf8Size(nullptr, 0) == 0, "an empty range must count nothing");
  static_assert(utf8Size(embeddedNull, std::size(embeddedNull)) == std::size(embeddedNull),
                "a null byte inside the range must count as a character");
  static_assert(utf8Size(unterminated, std::size(unterminated)) == std::size(unterminated),
                "the count must stop at the end of the range");
}

// Two ranges holding the same characters compare equal, whichever buffers hold them.
TEST_CASE("string_utils/compare/equal_ranges") {
  CHECK(compare(repeated, repeatedLength, repeated, repeatedLength) == 0);
  CHECK(compare(repeated, repeatedLength, unterminated, std::size(unterminated)) == 0);

  // Two empty ranges compare equal, a null pointer with a zero length among them.
  CHECK(compare(repeated, 0, "", 0) == 0);
  CHECK(compare(nullptr, 0, nullptr, 0) == 0);

  static_assert(compare(repeated, repeatedLength, unterminated, std::size(unterminated)) == 0,
                "the same characters in another buffer must compare equal");
  static_assert(compare(nullptr, 0, nullptr, 0) == 0, "two empty ranges must compare equal");
}

// The first differing character decides the order, whatever the lengths, and swapping the ranges flips it.
TEST_CASE("string_utils/compare/first_difference_decides") {
  CHECK(compare(repeated, repeatedLength, repeatedFirstDiffers, repeatedLength) < 0);
  CHECK(compare(repeatedFirstDiffers, repeatedLength, repeated, repeatedLength) > 0);
  CHECK(compare(repeated, repeatedLength, repeatedMiddleDiffers, repeatedLength) > 0);
  CHECK(compare(repeated, repeatedLength, repeatedLastDiffers, repeatedLength) < 0);

  // A difference in the first character outweighs a shorter length.
  CHECK(compare("b", 1, repeated, repeatedLength) > 0);

  // Bytes order as unsigned char, so a byte above the ASCII range follows an ASCII one.
  CHECK(compare(highByteInside + 1, 1, highByteInside, 1) > 0);

  static_assert(compare(repeated, repeatedLength, repeatedFirstDiffers, repeatedLength) < 0,
                "a smaller first character must order first");
  static_assert(compare(repeatedFirstDiffers, repeatedLength, repeated, repeatedLength) > 0,
                "swapping the ranges must flip the order");
  static_assert(compare(repeated, repeatedLength, repeatedLastDiffers, repeatedLength) < 0,
                "a difference in the last character must still decide");
  static_assert(compare("b", 1, repeated, repeatedLength) > 0, "the first difference must outweigh the lengths");
  static_assert(compare(highByteInside + 1, 1, highByteInside, 1) > 0, "a high byte must order after an ASCII one");
}

// When one range is a prefix of the other, the shorter one orders first.
TEST_CASE("string_utils/compare/shorter_prefix_orders_first") {
  CHECK(compare(repeated, repeatedLength, repeatedLonger, repeatedLongerLength) < 0);
  CHECK(compare(repeatedLonger, repeatedLongerLength, repeated, repeatedLength) > 0);
  CHECK(compare(repeated, leadingLength, repeated, repeatedLength) < 0);

  // An empty range is a prefix of every other one.
  CHECK(compare(nullptr, 0, repeated, repeatedLength) < 0);
  CHECK(compare(repeated, repeatedLength, "", 0) > 0);

  static_assert(compare(repeated, repeatedLength, repeatedLonger, repeatedLongerLength) < 0,
                "a prefix must order before the longer range");
  static_assert(compare(repeatedLonger, repeatedLongerLength, repeated, repeatedLength) > 0,
                "the longer range must order after its prefix");
  static_assert(compare(nullptr, 0, repeated, repeatedLength) < 0, "an empty range must order first");
}

// What a comparison sees of its ranges: the counted characters only, a null byte among them included.
TEST_CASE("string_utils/compare/reads_only_the_counted_ranges") {
  // The lengths cut both literals before they part, so the parts compare equal.
  CHECK(compare(repeated, leadingLength, repeatedLastDiffers, leadingLength) == 0);

  // A null byte compares as a character ordering before any letter, where a terminator scan would stop.
  CHECK(compare(embeddedNull, std::size(embeddedNull), "pl", 2) > 0);
  CHECK(compare(embeddedNull, std::size(embeddedNull), "pla", 3) < 0);

  static_assert(compare(repeated, leadingLength, repeatedLastDiffers, leadingLength) == 0,
                "characters past the lengths must not take part");
  static_assert(compare(embeddedNull, std::size(embeddedNull), "pla", 3) < 0,
                "a null byte must compare as a character");

  // Constant evaluation rejects a read past the array, so the comparison stays inside both ranges.
  static_assert(compare(unterminated, std::size(unterminated), repeatedLonger, repeatedLongerLength) < 0,
                "the comparison must stop at the end of the shorter range");
}

namespace {

// Literals of one encoding width each, so an expected count derives from the byte length instead of a hand count.
constexpr const char * ascii    = "player";
constexpr const char * cyrillic = "привет";
constexpr const char * cjk      = "日本語";
constexpr const char * emoji    = "🎮🎲";

// The same four scripts in one string, the parts separated by three spaces.
constexpr const char * mixed = "player привет 日本語 🎮🎲";

// The shortest and the longest sequence each narrowed second-byte range admits: U+0800, U+D7FF, U+10000, U+10FFFF.
constexpr char lowestThreeByte[]  = {'\xE0', '\xA0', '\x80', '\0'};
constexpr char beforeSurrogates[] = {'\xED', '\x9F', '\xBF', '\0'};
constexpr char lowestFourByte[]   = {'\xF0', '\x90', '\x80', '\x80', '\0'};
constexpr char highestCodePoint[] = {'\xF4', '\x8F', '\xBF', '\xBF', '\0'};

// A continuation byte, which the lookup table maps to a length of zero because it starts no sequence.
constexpr const char * continuationByte = "\x80";

// A three-byte lead byte the terminator cuts short after a single continuation byte.
constexpr char truncatedSymbol[] = {'a', '\xE6', '\x97', '\0'};

// A two-byte lead byte followed by an ASCII character rather than a byte of the 10xxxxxx form.
constexpr char lonelyLead[] = {'\xC2', 'A', '\0'};

// The ill-formed sequences the lead byte alone admits: an overlong '/', the surrogate U+D800, and the code point one
// past U+10FFFF.
constexpr char overlongSymbol[]  = {'\xE0', '\x80', '\xAF', '\0'};
constexpr char surrogateSymbol[] = {'\xED', '\xA0', '\x80', '\0'};
constexpr char aboveRange[]      = {'\xF4', '\x90', '\x80', '\x80', '\0'};

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
constexpr std::array<Utf16Sample, 5> utf16Samples{
  {
   {u"player", ascii},
   {u"привет", cyrillic},
   {u"日本語", cjk},
   {u"🎮🎲", emoji},
   {u"player привет 日本語 🎮🎲", mixed},
   }
};

// Surrogates outside a pair after an 'a': high before ASCII, high at the end, lone low, and a reversed pair.
constexpr std::array<std::array<char16_t, 4>, 4> loneSurrogates{
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

// The sequences validateUtf8() rejects, the three a narrowed second-byte range catches among them.
constexpr std::array<const char *, 5> rejectedSequences{continuationByte, lonelyLead, overlongSymbol, surrogateSymbol,
                                                        aboveRange};

// A byte the conversion never writes, so a surviving one shows where the call stopped.
constexpr char sentinel = '#';

// The UTF-16 counterpart of sentinel, for buffers of char16_t.
constexpr char16_t utf16Sentinel = u'#';

} // namespace

// How many characters a UTF-8 string holds, against the bytes it occupies.
TEST_CASE("string_utils/utf8_len/counts_characters_not_bytes") {
  CHECK(utf8Len(ascii) == std::char_traits<char>::length(ascii));
  CHECK(utf8Len(cyrillic) == std::char_traits<char>::length(cyrillic) / 2);
  CHECK(utf8Len(cjk) == std::char_traits<char>::length(cjk) / 3);
  CHECK(utf8Len(emoji) == std::char_traits<char>::length(emoji) / 4);

  // A mixed string counts every character once, whatever width each one is stored in.
  CHECK(utf8Len(mixed) == utf8Len(ascii) + utf8Len(cyrillic) + utf8Len(cjk) + utf8Len(emoji) + 3);

  CHECK(utf8Len("") == 0);
}

// Which sequences the narrowed second-byte ranges still accept, at both ends of every narrowed range.
TEST_CASE("string_utils/utf8_len/counts_sequences_at_the_range_edges") {
  CHECK(utf8Len(lowestThreeByte) == 1);
  CHECK(utf8Len(beforeSurrogates) == 1);
  CHECK(utf8Len(lowestFourByte) == 1);
  CHECK(utf8Len(highestCodePoint) == 1);
}

// A surrogate pair counts as one character, so every sample holds as many characters as its UTF-8 form.
TEST_CASE("string_utils/utf16_len/counts_characters_not_units") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];

    CHECK(utf16Len(utf16) == utf8Len(utf8));
  }

  CHECK(utf16Len(u"🎮🎲") == std::char_traits<char16_t>::length(u"🎮🎲") / 2);
  CHECK(utf16Len(u"") == 0);
}

// A character at either edge of the surrogate range counts once, whether it takes one unit or a pair.
TEST_CASE("string_utils/utf16_len/counts_units_at_the_surrogate_edges") {
  CHECK(utf16Len(utf16BeforeSurrogates) == 1);
  CHECK(utf16Len(utf16AfterSurrogates) == 1);
  CHECK(utf16Len(utf16LowestPair) == 1);
  CHECK(utf16Len(utf16HighestCodePoint) == 1);
}

// Which strings the validator accepts: every encoding width and both ends of every narrowed range.
TEST_CASE("string_utils/validate_utf8/accepts_well_formed_sequences") {
  CHECK(validateUtf8(ascii));
  CHECK(validateUtf8(cyrillic));
  CHECK(validateUtf8(cjk));
  CHECK(validateUtf8(emoji));
  CHECK(validateUtf8(mixed));

  CHECK(validateUtf8(lowestThreeByte));
  CHECK(validateUtf8(beforeSurrogates));
  CHECK(validateUtf8(lowestFourByte));
  CHECK(validateUtf8(highestCodePoint));

  // A string holding no character holds no ill-formed one either.
  CHECK(validateUtf8(""));

  // The counted form accepts the same strings, measured by their length.
  CHECK(validateUtf8(mixed, std::char_traits<char>::length(mixed)));
  CHECK(validateUtf8(highestCodePoint, std::size(highestCodePoint) - 1));
  CHECK(validateUtf8(nullptr, 0));
}

// Which strings it rejects, among them the sequences a lead byte alone admits.
TEST_CASE("string_utils/validate_utf8/rejects_ill_formed_sequences") {
  // A null pointer names no string to check, which the validator answers rather than asserts.
  CHECK_FALSE(validateUtf8(nullptr));

  CHECK_FALSE(validateUtf8(continuationByte));
  CHECK_FALSE(validateUtf8(truncatedSymbol));

  // A trailing byte outside the 10xxxxxx form, whatever width the lead byte announced.
  CHECK_FALSE(validateUtf8(lonelyLead));

  // The three a narrowed second-byte range rejects: overlong, surrogate, and past U+10FFFF.
  CHECK_FALSE(validateUtf8(overlongSymbol));
  CHECK_FALSE(validateUtf8(surrogateSymbol));
  CHECK_FALSE(validateUtf8(aboveRange));

  // The counted form rejects the same sequences, and a null pointer that names bytes.
  CHECK_FALSE(validateUtf8(continuationByte, 1));
  CHECK_FALSE(validateUtf8(lonelyLead, std::size(lonelyLead) - 1));
  CHECK_FALSE(validateUtf8(overlongSymbol, std::size(overlongSymbol) - 1));
  CHECK_FALSE(validateUtf8(surrogateSymbol, std::size(surrogateSymbol) - 1));
  CHECK_FALSE(validateUtf8(aboveRange, std::size(aboveRange) - 1));
  CHECK_FALSE(validateUtf8(nullptr, 1));
}

// A counted range ends at its length: bytes past it go unread, and a terminator inside it is an ordinary byte.
TEST_CASE("string_utils/validate_utf8/reads_only_the_counted_range") {
  // The ill-formed tail sits past the length, so the range holds the leading 'a' alone.
  CHECK(validateUtf8(truncatedSymbol, 1));

  // A length ending inside a sequence cuts it short, even where the bytes after it would complete it.
  CHECK_FALSE(validateUtf8(cyrillic, 1));
  CHECK(validateUtf8(cyrillic, 2));

  // A terminator inside the range is U+0000, a well-formed one-byte character.
  CHECK(validateUtf8(innerTerminator, std::size(innerTerminator)));
}

// Which strings the validator accepts: both widths and both ends of the surrogate range.
TEST_CASE("string_utils/validate_utf16/accepts_well_formed_strings") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const char16_t * const utf16 = utf16Samples[index].utf16;

    CHECK(validateUtf16(utf16));
    CHECK(validateUtf16(utf16, std::char_traits<char16_t>::length(utf16)));
  }

  CHECK(validateUtf16(utf16BeforeSurrogates));
  CHECK(validateUtf16(utf16AfterSurrogates));
  CHECK(validateUtf16(utf16LowestPair));
  CHECK(validateUtf16(utf16HighestCodePoint));

  // A string holding no character holds no ill-formed one either.
  CHECK(validateUtf16(u""));
  CHECK(validateUtf16(nullptr, 0));
}

// Which strings it rejects: a surrogate outside a pair, wherever it sits, and a null pointer.
TEST_CASE("string_utils/validate_utf16/rejects_ill_formed_strings") {
  // A null pointer names no string to check, which the validator answers rather than asserts.
  CHECK_FALSE(validateUtf16(nullptr));

  for (size_t index = 0; index < loneSurrogates.size(); ++index) {
    INFO("index ", index);
    const char16_t * const units = loneSurrogates[index].data();

    CHECK_FALSE(validateUtf16(units));
    CHECK_FALSE(validateUtf16(units, std::char_traits<char16_t>::length(units)));
  }

  // The counted form rejects a null pointer that names units.
  CHECK_FALSE(validateUtf16(nullptr, 1));
}

// A counted range ends at its length: units past it go unread, and a terminator inside it is an ordinary unit.
TEST_CASE("string_utils/validate_utf16/reads_only_the_counted_range") {
  // The high surrogate sits past the length, so the range holds the leading 'a' alone.
  CHECK(validateUtf16(loneSurrogates[1].data(), 1));

  // A length ending between the halves of a pair cuts it short, even where the unit after it would complete it.
  CHECK_FALSE(validateUtf16(u"a🎮", 2));
  CHECK(validateUtf16(u"a🎮", 3));

  // A terminator inside the range is U+0000, a well-formed one-unit character.
  CHECK(validateUtf16(utf16InnerTerminator, std::size(utf16InnerTerminator)));
}

// Every width converts to the bytes of the matching UTF-8 literal, and the call hands back the buffer it wrote.
TEST_CASE("string_utils/utf16_to_utf8/encodes_every_width") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];
    const auto length          = std::char_traits<char>::length(utf8);

    std::array<char, 64> buffer{};
    CHECK(utf16ToUtf8(buffer.data(), buffer.size(), utf16, std::char_traits<char16_t>::length(utf16)) == buffer.data());
    CHECK(std::char_traits<char>::length(buffer.data()) == length);
    CHECK(std::char_traits<char>::compare(buffer.data(), utf8, length) == 0);
  }
}

// The unit count bounds the input: a terminator inside it is encoded, and a pair it cuts short ends the call.
TEST_CASE("string_utils/utf16_to_utf8/reads_only_the_counted_units") {
  std::array<char, 8> inner{sentinel, sentinel, sentinel, sentinel, sentinel};
  utf16ToUtf8(inner.data(), inner.size(), utf16InnerTerminator, std::size(utf16InnerTerminator));
  CHECK(inner[0] == 'a');
  CHECK(inner[1] == '\0');
  CHECK(inner[2] == 'b');
  CHECK(inner[3] == '\0');

  // Three units of a two-byte script encode three characters and leave the rest unread.
  const auto prefixBytes = std::char_traits<char>::length("при");

  std::array<char, 16> prefix{};
  utf16ToUtf8(prefix.data(), prefix.size(), u"привет", 3);
  CHECK(std::char_traits<char>::length(prefix.data()) == prefixBytes);
  CHECK(std::char_traits<char>::compare(prefix.data(), "при", prefixBytes) == 0);

  // A count ending between the halves of a pair keeps what precedes the pair and stops there.
  std::array<char, 16> cut{sentinel, sentinel, sentinel, sentinel};
  utf16ToUtf8(cut.data(), cut.size(), u"a🎮", 2);
  CHECK(cut[0] == 'a');
  CHECK(cut[1] == '\0');
}

// A character that does not fit whole stays out, and the terminator always fits.
TEST_CASE("string_utils/utf16_to_utf8/stops_before_a_character_that_does_not_fit") {
  const auto length = std::char_traits<char>::length(cyrillic);
  const auto units  = std::char_traits<char16_t>::length(u"привет");

  // A buffer of exactly the encoded length plus the terminator holds the whole string.
  std::array<char, 32> exact{};
  utf16ToUtf8(exact.data(), length + 1, u"привет", units);
  CHECK(std::char_traits<char>::length(exact.data()) == length);

  // One byte less leaves out the whole last two-byte character.
  std::array<char, 32> shorter{};
  utf16ToUtf8(shorter.data(), length, u"привет", units);
  CHECK(std::char_traits<char>::length(shorter.data()) == length - 2);
  CHECK(std::char_traits<char>::compare(shorter.data(), cyrillic, length - 2) == 0);

  // ASCII and a three-byte script each take their own branch of the encoder and stop at a full buffer the same way.
  const auto asciiLength = std::char_traits<char>::length(ascii);

  std::array<char, 16> asciiShorter{};
  utf16ToUtf8(asciiShorter.data(), asciiLength, u"player", std::char_traits<char16_t>::length(u"player"));
  CHECK(std::char_traits<char>::length(asciiShorter.data()) == asciiLength - 1);
  CHECK(std::char_traits<char>::compare(asciiShorter.data(), ascii, asciiLength - 1) == 0);

  const auto cjkLength    = std::char_traits<char>::length(cjk);
  const auto cjkCharBytes = cjkLength / utf8Len(cjk);

  std::array<char, 16> cjkShorter{};
  utf16ToUtf8(cjkShorter.data(), cjkLength, u"日本語", std::char_traits<char16_t>::length(u"日本語"));
  CHECK(std::char_traits<char>::length(cjkShorter.data()) == cjkLength - cjkCharBytes);
  CHECK(std::char_traits<char>::compare(cjkShorter.data(), cjk, cjkLength - cjkCharBytes) == 0);

  // A single byte holds the terminator alone, and nothing past the size is written.
  std::array<char, 4> single{sentinel, sentinel, sentinel, sentinel};
  utf16ToUtf8(single.data(), 1, u"привет", units);
  CHECK(single[0] == '\0');
  CHECK(single[1] == sentinel);
}

// A surrogate pair goes in as one four-byte sequence or not at all.
TEST_CASE("string_utils/utf16_to_utf8/writes_a_surrogate_pair_whole_or_not_at_all") {
  const auto pairBytes = std::char_traits<char>::length("🎮");
  const auto pairUnits = std::char_traits<char16_t>::length(u"🎮");

  std::array<char, 8> pairShort{sentinel, sentinel, sentinel, sentinel};
  utf16ToUtf8(pairShort.data(), pairBytes, u"🎮", pairUnits);
  CHECK(pairShort[0] == '\0');

  std::array<char, 8> pairWhole{};
  utf16ToUtf8(pairWhole.data(), pairBytes + 1, u"🎮", pairUnits);
  CHECK(std::char_traits<char>::compare(pairWhole.data(), "🎮", pairBytes + 1) == 0);
}

// A surrogate outside a pair encodes no code point, so the call keeps what precedes it and stops there.
TEST_CASE("string_utils/utf16_to_utf8/stops_before_a_lone_surrogate") {
  for (size_t index = 0; index < loneSurrogates.size(); ++index) {
    INFO("index ", index);

    const char16_t * const units = loneSurrogates[index].data();

    std::array<char, 16> buffer{};
    utf16ToUtf8(buffer.data(), buffer.size(), units, std::char_traits<char16_t>::length(units));
    CHECK(std::char_traits<char>::length(buffer.data()) == 1);
    CHECK(buffer[0] == 'a');
  }
}

// A null or empty source converts as an empty string; a buffer that cannot hold the terminator gets no write at all.
TEST_CASE("string_utils/utf16_to_utf8/handles_null_and_empty_arguments") {
  std::array<char, 4> untouched{sentinel, sentinel, sentinel, sentinel};
  CHECK(utf16ToUtf8(nullptr, untouched.size(), u"player", 1) == nullptr);
  CHECK(utf16ToUtf8(untouched.data(), 0, u"player", 1) == nullptr);
  CHECK(untouched[0] == sentinel);

  std::array<char, 4> fromNull{sentinel, sentinel, sentinel, sentinel};
  CHECK(utf16ToUtf8(fromNull.data(), fromNull.size(), nullptr, 4) == fromNull.data());
  CHECK(fromNull[0] == '\0');

  std::array<char, 4> fromEmpty{sentinel, sentinel, sentinel, sentinel};
  CHECK(utf16ToUtf8(fromEmpty.data(), fromEmpty.size(), u"player", 0) == fromEmpty.data());
  CHECK(fromEmpty[0] == '\0');
}

// Every width measures the byte length of the matching UTF-8 literal.
TEST_CASE("string_utils/utf16_in_utf8_size/matches_every_width") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];

    CHECK(utf16InUtf8Size(utf16, std::char_traits<char16_t>::length(utf16)) == std::char_traits<char>::length(utf8));
  }
}

// Each width boundary counts on its side: U+007F one byte, U+0080 and U+07FF two, U+0800 three, a pair four.
TEST_CASE("string_utils/utf16_in_utf8_size/counts_the_width_edges") {
  constexpr char16_t widthEdges[] = {0x7F, 0x80, 0x7FF, 0x800};

  CHECK(utf16InUtf8Size(&widthEdges[0], 1) == 1);
  CHECK(utf16InUtf8Size(&widthEdges[1], 1) == 2);
  CHECK(utf16InUtf8Size(&widthEdges[2], 1) == 2);
  CHECK(utf16InUtf8Size(&widthEdges[3], 1) == 3);
  CHECK(utf16InUtf8Size(utf16BeforeSurrogates, 1) == 3);
  CHECK(utf16InUtf8Size(utf16AfterSurrogates, 1) == 3);
  CHECK(utf16InUtf8Size(utf16LowestPair, 2) == 4);
  CHECK(utf16InUtf8Size(utf16HighestCodePoint, 2) == 4);
}

// The unit count bounds the input: a terminator inside it counts one byte, and a pair it cuts short ends the count.
TEST_CASE("string_utils/utf16_in_utf8_size/reads_only_the_counted_units") {
  CHECK(utf16InUtf8Size(utf16InnerTerminator, std::size(utf16InnerTerminator)) == 3);
  CHECK(utf16InUtf8Size(u"привет", 3) == std::char_traits<char>::length("при"));
  CHECK(utf16InUtf8Size(u"a🎮", 2) == 1);
}

// A surrogate outside a pair ends the count where utf16ToUtf8() stops encoding.
TEST_CASE("string_utils/utf16_in_utf8_size/stops_before_a_lone_surrogate") {
  for (size_t index = 0; index < loneSurrogates.size(); ++index) {
    INFO("index ", index);

    const char16_t * const units = loneSurrogates[index].data();
    const auto             count = std::char_traits<char16_t>::length(units);

    std::array<char, 16> buffer{};
    utf16ToUtf8(buffer.data(), buffer.size(), units, count);
    CHECK(utf16InUtf8Size(units, count) == std::char_traits<char>::length(buffer.data()));
    CHECK(utf16InUtf8Size(units, count) == 1);
  }
}

// Over the units a decode wrote, the count gives the bytes the decode consumed before an ill-formed sequence.
TEST_CASE("string_utils/utf16_in_utf8_size/measures_the_bytes_a_decode_consumed") {
  for (size_t index = 0; index < rejectedSequences.size(); ++index) {
    INFO("index ", index);
    const char * const rejected = rejectedSequences[index];

    std::array<char, 64> input{};
    const auto           prefixLength = std::char_traits<char>::length(mixed);
    std::char_traits<char>::copy(input.data(), mixed, prefixLength);
    std::char_traits<char>::copy(input.data() + prefixLength, rejected, std::char_traits<char>::length(rejected));

    std::array<char16_t, 64> decoded{};
    utf8ToUtf16(decoded.data(), decoded.size(), input.data(), std::char_traits<char>::length(input.data()));
    CHECK(utf16InUtf8Size(decoded.data(), std::char_traits<char16_t>::length(decoded.data())) == prefixLength);
  }
}

// A null or empty source measures nothing, whatever the count.
TEST_CASE("string_utils/utf16_in_utf8_size/handles_null_and_empty_arguments") {
  CHECK(utf16InUtf8Size(nullptr, 0) == 0);
  CHECK(utf16InUtf8Size(nullptr, 4) == 0);
  CHECK(utf16InUtf8Size(u"player", 0) == 0);
}

// Every width decodes to the matching UTF-16 literal, a pair for four bytes, and the call hands back its buffer.
TEST_CASE("string_utils/utf8_to_utf16/decodes_every_width") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];
    const auto length          = std::char_traits<char16_t>::length(utf16);

    std::array<char16_t, 64> buffer{};
    CHECK(utf8ToUtf16(buffer.data(), buffer.size(), utf8, std::char_traits<char>::length(utf8)) == buffer.data());
    CHECK(std::char_traits<char16_t>::length(buffer.data()) == length);
    CHECK(std::char_traits<char16_t>::compare(buffer.data(), utf16, length) == 0);
  }
}

// The byte count bounds the input: a terminator inside it is decoded, and a sequence it cuts short ends the call.
TEST_CASE("string_utils/utf8_to_utf16/reads_only_the_counted_bytes") {
  std::array<char16_t, 8> inner{};
  utf8ToUtf16(inner.data(), inner.size(), innerTerminator, std::size(innerTerminator));
  CHECK(inner[0] == u'a');
  CHECK(inner[1] == u'\0');
  CHECK(inner[2] == u'b');
  CHECK(inner[3] == u'\0');

  // Three bytes of a two-byte script hold one whole character and the lead byte of the next.
  std::array<char16_t, 8> cut{};
  utf8ToUtf16(cut.data(), cut.size(), cyrillic, 3);
  CHECK(std::char_traits<char16_t>::length(cut.data()) == 1);
  CHECK(cut[0] == u'п');
}

// A sequence the validator rejects ends the call, and the characters before it stay.
TEST_CASE("string_utils/utf8_to_utf16/stops_at_a_sequence_it_cannot_decode") {
  for (size_t index = 0; index < rejectedSequences.size(); ++index) {
    INFO("index ", index);
    const char * const sequence = rejectedSequences[index];

    std::array<char16_t, 4> buffer{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
    utf8ToUtf16(buffer.data(), buffer.size(), sequence, std::char_traits<char>::length(sequence));
    CHECK(buffer[0] == u'\0');
  }

  std::array<char16_t, 4> prefix{};
  utf8ToUtf16(prefix.data(), prefix.size(), truncatedSymbol, std::size(truncatedSymbol) - 1);
  CHECK(prefix[0] == u'a');
  CHECK(prefix[1] == u'\0');
}

// A character that does not fit stays out, and the terminator always fits.
TEST_CASE("string_utils/utf8_to_utf16/stops_when_the_buffer_is_full") {
  const auto bytes  = std::char_traits<char>::length(cyrillic);
  const auto length = utf8Len(cyrillic);

  // A buffer of exactly the character count plus the terminator holds the whole string.
  std::array<char16_t, 16> exact{};
  utf8ToUtf16(exact.data(), length + 1, cyrillic, bytes);
  CHECK(std::char_traits<char16_t>::compare(exact.data(), u"привет", length) == 0);
  CHECK(exact[length] == u'\0');

  // One element less leaves out the last character.
  std::array<char16_t, 16> shorter{};
  utf8ToUtf16(shorter.data(), length, cyrillic, bytes);
  CHECK(std::char_traits<char16_t>::length(shorter.data()) == length - 1);
  CHECK(std::char_traits<char16_t>::compare(shorter.data(), u"привет", length - 1) == 0);

  // ASCII takes a shorter path through the decoder and stops at a full buffer the same way.
  const auto asciiLength = std::char_traits<char>::length(ascii);

  std::array<char16_t, 16> asciiShorter{};
  utf8ToUtf16(asciiShorter.data(), asciiLength, ascii, asciiLength);
  CHECK(std::char_traits<char16_t>::length(asciiShorter.data()) == asciiLength - 1);
  CHECK(std::char_traits<char16_t>::compare(asciiShorter.data(), u"player", asciiLength - 1) == 0);

  // A single element holds the terminator alone, and nothing past the size is written.
  std::array<char16_t, 4> single{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  utf8ToUtf16(single.data(), 1, cyrillic, bytes);
  CHECK(single[0] == u'\0');
  CHECK(single[1] == utf16Sentinel);
}

// A four-byte character goes in as a whole surrogate pair or not at all.
TEST_CASE("string_utils/utf8_to_utf16/writes_a_surrogate_pair_whole_or_not_at_all") {
  const auto pairBytes = std::char_traits<char>::length("🎮");
  const auto pairUnits = std::char_traits<char16_t>::length(u"🎮");

  std::array<char16_t, 4> pairShort{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  utf8ToUtf16(pairShort.data(), pairUnits, "🎮", pairBytes);
  CHECK(pairShort[0] == u'\0');

  std::array<char16_t, 4> pairWhole{};
  utf8ToUtf16(pairWhole.data(), pairUnits + 1, "🎮", pairBytes);
  CHECK(std::char_traits<char16_t>::compare(pairWhole.data(), u"🎮", pairUnits + 1) == 0);
}

// A null or empty source decodes as an empty string; a buffer that cannot hold the terminator gets no write at all.
TEST_CASE("string_utils/utf8_to_utf16/handles_null_and_empty_arguments") {
  std::array<char16_t, 4> untouched{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  CHECK(utf8ToUtf16(nullptr, untouched.size(), ascii, 1) == nullptr);
  CHECK(utf8ToUtf16(untouched.data(), 0, ascii, 1) == nullptr);
  CHECK(untouched[0] == utf16Sentinel);

  std::array<char16_t, 4> fromNull{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  CHECK(utf8ToUtf16(fromNull.data(), fromNull.size(), nullptr, 4) == fromNull.data());
  CHECK(fromNull[0] == u'\0');

  std::array<char16_t, 4> fromEmpty{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  CHECK(utf8ToUtf16(fromEmpty.data(), fromEmpty.size(), ascii, 0) == fromEmpty.data());
  CHECK(fromEmpty[0] == u'\0');
}

} // namespace toy::string_utils
