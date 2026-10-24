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

} // namespace toy::string_utils
