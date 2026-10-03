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
constexpr const char * c_repeated               = "abracadabra";
constexpr size_t       c_repeatedLength         = std::char_traits<char>::length(c_repeated);
// The same literal with more after it, longer than the range the first one spans.
constexpr const char * c_repeatedLonger         = "abracadabra and then some";
constexpr size_t       c_repeatedLongerLength   = std::char_traits<char>::length(c_repeatedLonger);
// The same literal with its last character replaced, which only a comparison reading to the end can reject.
constexpr const char * c_repeatedLastDiffers    = "abracadabrx";
// The same again, differing in its first character and in one byte of its middle.
constexpr const char * c_repeatedFirstDiffers   = "xbracadabra";
constexpr const char * c_repeatedMiddleDiffers  = "abXacadabra";
// And again, differing in the byte just before the last, which a middle comparison short by one would skip.
constexpr const char * c_repeatedNearEndDiffers = "abracadabXa";
// A leading part of the literal that ends before its second "abra", 'b', and 'r'.
constexpr size_t       c_leadingLength          = 7;

// One character three times, then another just past the counted range.
constexpr const char * c_runOfA       = "aaab";
constexpr size_t       c_runOfALength = 3;

// A byte above the ASCII range inside a string, so a character-set search meets a value negative as a plain char.
constexpr const char * c_highByteInside       = "a\xff"
                                                "b";
constexpr size_t       c_highByteInsideLength = std::char_traits<char>::length(c_highByteInside);

// The literal's characters without a terminator; reading past them fails constant evaluation.
constexpr char c_unterminated[] = {'a', 'b', 'r', 'a', 'c', 'a', 'd', 'a', 'b', 'r', 'a'};

// A terminator inside the range, which a measured length would stop at.
constexpr char c_embeddedNull[] = {'p', 'l', '\0', 'a', 'y'};

} // namespace

// Where a character first occurs, counting from the offset the search starts at.
TEST_CASE("string_utils/find/first_character_at_or_after_offset") {
  CHECK(find(c_repeated, c_repeatedLength, 'a', 0) == 0);
  CHECK(find(c_repeated, c_repeatedLength, 'a', 1) == 3);
  CHECK(find(c_repeated, c_repeatedLength, 'd', 0) == 6);
  CHECK(find(c_repeated, c_repeatedLength, 'z', 0) == c_npos);

  // A start at or past the end matches nowhere, and so does any start in an empty range.
  CHECK(find(c_repeated, c_repeatedLength, 'a', c_repeatedLength) == c_npos);
  CHECK(find(c_repeated, c_repeatedLength, 'a', c_npos) == c_npos);
  CHECK(find(c_repeated, 0, 'a', 0) == c_npos);

  static_assert(find(c_repeated, c_repeatedLength, 'a', 0) == 0, "the first occurrence wins");
  static_assert(find(c_repeated, c_repeatedLength, 'a', 1) == 3, "the offset skips earlier matches");
  static_assert(find(c_repeated, c_repeatedLength, 'z', 0) == c_npos, "an absent character reports npos");
  static_assert(find(c_repeated, c_repeatedLength, 'a', c_repeatedLength) == c_npos,
                "a start at the end matches nowhere");
}

// Where the first match of a character sequence starts, counting from the offset the search starts at.
TEST_CASE("string_utils/find/first_sequence_at_or_after_offset") {
  CHECK(find(c_repeated, c_repeatedLength, "abra", 0, 4) == 0);
  CHECK(find(c_repeated, c_repeatedLength, "abra", 1, 4) == 7);
  CHECK(find(c_repeated, c_repeatedLength, "abra", 8, 4) == c_npos);
  CHECK(find(c_repeated, c_repeatedLength, "cad", 0, 3) == 4);
  CHECK(find(c_repeated, c_repeatedLength, "xyz", 0, 3) == c_npos);

  // A pattern longer than the range matches nowhere, decided on the lengths before any character is read.
  CHECK(find(c_repeated, c_repeatedLength, c_repeatedLonger, 0, c_repeatedLongerLength) == c_npos);

  // An empty pattern matches at the start offset, up to the end of the range and no further.
  CHECK(find(c_repeated, c_repeatedLength, "", 0, 0) == 0);
  CHECK(find(c_repeated, c_repeatedLength, "", 3, 0) == 3);
  CHECK(find(c_repeated, c_repeatedLength, "", c_repeatedLength, 0) == c_repeatedLength);
  CHECK(find(c_repeated, c_repeatedLength, "", c_repeatedLength + 1, 0) == c_npos);

  // The count bounds how much of the pattern is read.
  CHECK(find(c_repeated, c_repeatedLength, "abrasive", 0, 4) == 0);
  CHECK(find(c_repeated, c_repeatedLength, "abrasive", 1, 4) == 7);

  // A count of one matches at the last position, where the comparison behind the matched character reads nothing.
  CHECK(find(c_repeated, c_repeatedLength, "a", c_repeatedLength - 1, 1) == c_repeatedLength - 1);

  // A candidate sharing only its first characters must not end the scan.
  CHECK(find(c_repeated, c_repeatedLength, "abz", 0, 3) == c_npos);

  // A pattern the size of the range leaves a single candidate, which its last character decides.
  CHECK(find(c_repeated, c_repeatedLength, c_repeated, 0, c_repeatedLength) == 0);
  CHECK(find(c_repeated, c_repeatedLength, c_repeatedLastDiffers, 0, c_repeatedLength) == c_npos);

  static_assert(find(c_repeated, c_repeatedLength, "abra", 0, 4) == 0, "the first match wins");
  static_assert(find(c_repeated, c_repeatedLength, "abra", 1, 4) == 7, "the offset skips earlier matches");
  static_assert(find(c_repeated, c_repeatedLength, "xyz", 0, 3) == c_npos, "an absent pattern reports npos");
  static_assert(find(c_repeated, c_repeatedLength, c_repeatedLonger, 0, c_repeatedLongerLength) == c_npos,
                "a pattern longer than the range reports npos");
  static_assert(find(c_repeated, c_repeatedLength, "", 3, 0) == 3, "an empty pattern matches at the start offset");
  static_assert(find(c_repeated, c_repeatedLength, "", c_repeatedLength + 1, 0) == c_npos,
                "an empty pattern matches nowhere past the end");
  static_assert(find(c_repeated, c_repeatedLength, "abrasive", 1, 4) == 7, "the count bounds what is read");
  static_assert(find(c_repeated, c_repeatedLength, "a", c_repeatedLength - 1, 1) == c_repeatedLength - 1,
                "a count of one matches at the last position");
  static_assert(find(c_repeated, c_repeatedLength, "abz", 0, 3) == c_npos,
                "a candidate sharing only its first characters is rejected");
  static_assert(find(c_repeated, c_repeatedLength, c_repeated, 0, c_repeatedLength) == 0,
                "a pattern the size of the range matches at the front");
  static_assert(find(c_repeated, c_repeatedLength, c_repeatedLastDiffers, 0, c_repeatedLength) == c_npos,
                "a pattern differing in its last character matches nowhere");
}

// Where a character last occurs, at or before the offset the search starts at.
TEST_CASE("string_utils/rfind/last_character_at_or_before_offset") {
  CHECK(rfind(c_repeated, c_repeatedLength, 'a', c_npos) == c_repeatedLength - 1);
  CHECK(rfind(c_repeated, c_repeatedLength, 'a', 9) == 7);
  CHECK(rfind(c_repeated, c_repeatedLength, 'd', c_npos) == 6);
  CHECK(rfind(c_repeated, c_repeatedLength, 'd', 5) == c_npos);
  CHECK(rfind(c_repeated, c_repeatedLength, 'z', c_npos) == c_npos);

  // An offset of zero leaves the first character as the only candidate.
  CHECK(rfind(c_repeated, c_repeatedLength, 'b', 0) == c_npos);
  CHECK(rfind(c_repeated, c_repeatedLength, 'a', 0) == 0);

  // Any offset past the range covers all of it, and an empty range holds no candidate.
  CHECK(rfind(c_repeated, c_repeatedLength, 'a', c_repeatedLength + 5) == c_repeatedLength - 1);
  CHECK(rfind(c_repeated, 0, 'a', c_npos) == c_npos);

  static_assert(rfind(c_repeated, c_repeatedLength, 'a', c_npos) == c_repeatedLength - 1, "the last occurrence wins");
  static_assert(rfind(c_repeated, c_repeatedLength, 'a', 9) == 7, "the offset caps how far back a match may sit");
  static_assert(rfind(c_repeated, c_repeatedLength, 'z', c_npos) == c_npos, "an absent character reports npos");
  static_assert(rfind(c_repeated, c_repeatedLength, 'b', 0) == c_npos,
                "an offset of zero leaves only the first character");
}

// Where the last match of a character sequence starts, at or before the offset the search starts at.
TEST_CASE("string_utils/rfind/last_sequence_at_or_before_offset") {
  CHECK(rfind(c_repeated, c_repeatedLength, "abra", c_npos, 4) == 7);
  CHECK(rfind(c_repeated, c_repeatedLength, "abra", 7, 4) == 7);
  CHECK(rfind(c_repeated, c_repeatedLength, "abra", 6, 4) == 0);
  CHECK(rfind(c_repeated, c_repeatedLength, "abra", 0, 4) == 0);
  CHECK(rfind(c_repeated, c_repeatedLength, "cad", c_npos, 3) == 4);
  CHECK(rfind(c_repeated, c_repeatedLength, "cad", 3, 3) == c_npos);
  CHECK(rfind(c_repeated, c_repeatedLength, "xyz", c_npos, 3) == c_npos);
  CHECK(rfind(c_repeated, c_repeatedLength, c_repeatedLonger, c_npos, c_repeatedLongerLength) == c_npos);

  // An empty pattern matches at the offset, capped at the end of the range.
  CHECK(rfind(c_repeated, c_repeatedLength, "", 5, 0) == 5);
  CHECK(rfind(c_repeated, c_repeatedLength, "", c_npos, 0) == c_repeatedLength);

  // The count bounds how much of the pattern is read.
  CHECK(rfind(c_repeated, c_repeatedLength, "abrasive", c_npos, 4) == 7);

  // A pattern the size of the range leaves one candidate, which any of its three parts can reject.
  CHECK(rfind(c_repeated, c_repeatedLength, c_repeated, c_npos, c_repeatedLength) == 0);
  CHECK(rfind(c_repeated, c_repeatedLength, c_repeatedFirstDiffers, c_npos, c_repeatedLength) == c_npos);
  CHECK(rfind(c_repeated, c_repeatedLength, c_repeatedMiddleDiffers, c_npos, c_repeatedLength) == c_npos);
  CHECK(rfind(c_repeated, c_repeatedLength, c_repeatedNearEndDiffers, c_npos, c_repeatedLength) == c_npos);
  CHECK(rfind(c_repeated, c_repeatedLength, c_repeatedLastDiffers, c_npos, c_repeatedLength) == c_npos);

  // Counts of one and two, where the comparison behind the ends has nothing left to read.
  CHECK(rfind(c_repeated, c_repeatedLength, "a", c_npos, 1) == c_repeatedLength - 1);
  CHECK(rfind(c_repeated, c_repeatedLength, "ab", c_npos, 2) == 7);

  static_assert(rfind(c_repeated, c_repeatedLength, "abra", c_npos, 4) == 7, "the last match wins");
  static_assert(rfind(c_repeated, c_repeatedLength, "abra", 6, 4) == 0, "the offset caps where a match starts");
  static_assert(rfind(c_repeated, c_repeatedLength, "xyz", c_npos, 3) == c_npos, "an absent pattern reports npos");
  static_assert(rfind(c_repeated, c_repeatedLength, c_repeatedLonger, c_npos, c_repeatedLongerLength) == c_npos,
                "a pattern longer than the range reports npos");
  static_assert(rfind(c_repeated, c_repeatedLength, "", c_npos, 0) == c_repeatedLength,
                "an empty pattern matches at the end of the range");
  static_assert(rfind(c_repeated, c_repeatedLength, "abrasive", c_npos, 4) == 7, "the count bounds what is read");
  static_assert(rfind(c_repeated, c_repeatedLength, c_repeated, c_npos, c_repeatedLength) == 0,
                "a pattern the size of the range matches at the front");
  static_assert(rfind(c_repeated, c_repeatedLength, c_repeatedFirstDiffers, c_npos, c_repeatedLength) == c_npos,
                "a pattern differing in its first character matches nowhere");
  static_assert(rfind(c_repeated, c_repeatedLength, c_repeatedMiddleDiffers, c_npos, c_repeatedLength) == c_npos,
                "a pattern differing in its middle matches nowhere");
  static_assert(rfind(c_repeated, c_repeatedLength, c_repeatedNearEndDiffers, c_npos, c_repeatedLength) == c_npos,
                "a pattern differing just before its last character matches nowhere");
  static_assert(rfind(c_repeated, c_repeatedLength, c_repeatedLastDiffers, c_npos, c_repeatedLength) == c_npos,
                "a pattern differing in its last character matches nowhere");
  static_assert(rfind(c_repeated, c_repeatedLength, "ab", c_npos, 2) == 7, "a count of two is decided by its ends");
}

// Which character first belongs to a set, counting from the offset the search starts at.
TEST_CASE("string_utils/find_first_of/first_member_of_set") {
  CHECK(findFirstOf(c_repeated, c_repeatedLength, "rc", 0, 2) == 2);
  CHECK(findFirstOf(c_repeated, c_repeatedLength, "rc", 3, 2) == 4);
  CHECK(findFirstOf(c_repeated, c_repeatedLength, "rc", 10, 2) == c_npos);
  CHECK(findFirstOf(c_repeated, c_repeatedLength, "xyz", 0, 3) == c_npos);

  // The count bounds how much of the set is read.
  CHECK(findFirstOf(c_repeated, c_repeatedLength, "rcxyz", 0, 2) == 2);
  CHECK(findFirstOf(c_repeated, c_repeatedLength, "rcxyz", 3, 2) == 4);

  // An empty set holds no character to match, unlike an empty pattern in a sequence search.
  CHECK(findFirstOf(c_repeated, c_repeatedLength, "", 0, 0) == c_npos);

  // Neither does a start past the end, nor an empty range.
  CHECK(findFirstOf(c_repeated, c_repeatedLength, "a", c_repeatedLength, 1) == c_npos);
  CHECK(findFirstOf(c_repeated, 0, "a", 0, 1) == c_npos);

  static_assert(findFirstOf(c_repeated, c_repeatedLength, "rc", 0, 2) == 2, "the first member of the set wins");
  static_assert(findFirstOf(c_repeated, c_repeatedLength, "rc", 3, 2) == 4, "the offset skips earlier matches");
  static_assert(findFirstOf(c_repeated, c_repeatedLength, "rcxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(findFirstOf(c_repeated, c_repeatedLength, "", 0, 0) == c_npos, "an empty set matches nothing");
}

// Which character last belongs to a set, at or before the offset the search starts at.
TEST_CASE("string_utils/find_last_of/last_member_of_set") {
  CHECK(findLastOf(c_repeated, c_repeatedLength, "rc", c_npos, 2) == 9);
  CHECK(findLastOf(c_repeated, c_repeatedLength, "rc", 8, 2) == 4);
  CHECK(findLastOf(c_repeated, c_repeatedLength, "rc", 1, 2) == c_npos);
  CHECK(findLastOf(c_repeated, c_repeatedLength, "xyz", c_npos, 3) == c_npos);

  // The count bounds how much of the set is read.
  CHECK(findLastOf(c_repeated, c_repeatedLength, "rcxyz", c_npos, 2) == 9);
  CHECK(findLastOf(c_repeated, c_repeatedLength, "rcxyz", 8, 2) == 4);

  // An empty set matches nothing, and an empty range holds nothing to match.
  CHECK(findLastOf(c_repeated, c_repeatedLength, "", c_npos, 0) == c_npos);
  CHECK(findLastOf(c_repeated, 0, "a", c_npos, 1) == c_npos);

  static_assert(findLastOf(c_repeated, c_repeatedLength, "rc", c_npos, 2) == 9, "the last member of the set wins");
  static_assert(findLastOf(c_repeated, c_repeatedLength, "rc", 8, 2) == 4,
                "the offset caps how far back a match may sit");
  static_assert(findLastOf(c_repeated, c_repeatedLength, "rcxyz", 8, 2) == 4, "the count bounds what is read");
  static_assert(findLastOf(c_repeated, c_repeatedLength, "", c_npos, 0) == c_npos, "an empty set matches nothing");
}

// Which character first falls outside a single character or a set, counting from the start offset.
TEST_CASE("string_utils/find_first_not_of/first_character_outside") {
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, 'a', 0) == 1);
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, 'a', 3) == 4);
  CHECK(findFirstNotOf(c_runOfA, c_runOfALength, 'a', 0) == c_npos);

  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "ab", 0, 2) == 2);
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "ab", 3, 2) == 4);
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "abr", 0, 3) == 4);
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "ab", 10, 2) == c_npos);

  // A set holding every character the range uses leaves nothing to report.
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "abcdr", 0, 5) == c_npos);

  // The count bounds how much of the set is read.
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "abxyz", 0, 2) == 2);
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "abxyz", 3, 2) == 4);

  // No character belongs to an empty set, so the start offset matches while it lies inside the range.
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "", 0, 0) == 0);
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "", 2, 0) == 2);
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, "", c_repeatedLength, 0) == c_npos);
  CHECK(findFirstNotOf(c_repeated, 0, "", 0, 0) == c_npos);

  static_assert(findFirstNotOf(c_repeated, c_repeatedLength, 'a', 0) == 1,
                "the first character other than the skipped one wins");
  static_assert(findFirstNotOf(c_runOfA, c_runOfALength, 'a', 0) == c_npos,
                "a range of the skipped character reports npos");
  static_assert(findFirstNotOf(c_repeated, c_repeatedLength, "ab", 3, 2) == 4, "the offset skips earlier characters");
  static_assert(findFirstNotOf(c_repeated, c_repeatedLength, "abcdr", 0, 5) == c_npos,
                "a set covering every character reports npos");
  static_assert(findFirstNotOf(c_repeated, c_repeatedLength, "abxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(findFirstNotOf(c_repeated, c_repeatedLength, "", 2, 0) == 2, "an empty set excludes nothing");
}

// Which character last falls outside a single character or a set, at or before the start offset.
TEST_CASE("string_utils/find_last_not_of/last_character_outside") {
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, 'a', c_npos) == 9);
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, 'a', 8) == 8);
  CHECK(findLastNotOf(c_runOfA, c_runOfALength, 'a', c_npos) == c_npos);

  CHECK(findLastNotOf(c_repeated, c_repeatedLength, "ab", c_npos, 2) == 9);
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, "ab", 8, 2) == 6);
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, "ab", 1, 2) == c_npos);
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, "abcdr", c_npos, 5) == c_npos);

  // The count bounds how much of the set is read.
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, "abxyz", c_npos, 2) == 9);
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, "abxyz", 8, 2) == 6);

  // No character belongs to an empty set, so the offset matches, capped at the last character of the range.
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, "", c_npos, 0) == c_repeatedLength - 1);
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, "", 2, 0) == 2);
  CHECK(findLastNotOf(c_repeated, 0, "", c_npos, 0) == c_npos);

  static_assert(findLastNotOf(c_repeated, c_repeatedLength, 'a', c_npos) == 9,
                "the last character other than the skipped one wins");
  static_assert(findLastNotOf(c_repeated, c_repeatedLength, "ab", 8, 2) == 6,
                "the offset caps how far back a match may sit");
  static_assert(findLastNotOf(c_repeated, c_repeatedLength, "abcdr", c_npos, 5) == c_npos,
                "a set covering every character reports npos");
  static_assert(findLastNotOf(c_repeated, c_repeatedLength, "abxyz", 8, 2) == 6, "the count bounds what is read");
  static_assert(findLastNotOf(c_repeated, c_repeatedLength, "", c_npos, 0) == c_repeatedLength - 1,
                "an empty set excludes nothing");
  static_assert(findLastNotOf(c_repeated, 0, "", c_npos, 0) == c_npos, "an empty range has no last character");
}

// A byte above the ASCII range reaches a character-set search, where a signed index would read outside the table.
TEST_CASE("string_utils/search/matches_high_byte_in_set") {
  CHECK(findFirstOf(c_highByteInside, c_highByteInsideLength, "\xff", 0, 1) == 1);
  CHECK(findLastOf(c_highByteInside, c_highByteInsideLength, "\xff", c_npos, 1) == 1);
  // Temporary: Clang 23 on x86_64 miscompiles memchr over a constant set; the static_asserts below still cover both.
  // Upstream report: https://github.com/llvm/llvm-project/issues/228213
#if !defined(__clang__) || defined(__apple_build_version__) || __clang_major__ != 23 || !defined(__x86_64__)
  CHECK(findFirstNotOf(c_highByteInside, c_highByteInsideLength, "a\xff", 0, 2) == 2);
  CHECK(findLastNotOf(c_highByteInside, c_highByteInsideLength, "b\xff", c_npos, 2) == 0);
#endif

  static_assert(findFirstOf(c_highByteInside, c_highByteInsideLength, "\xff", 0, 1) == 1,
                "a high byte is found in a set");
  static_assert(findLastOf(c_highByteInside, c_highByteInsideLength, "\xff", c_npos, 1) == 1,
                "a high byte is found scanning backwards");
  static_assert(findFirstNotOf(c_highByteInside, c_highByteInsideLength, "a\xff", 0, 2) == 2,
                "a high byte counts as a member of the set");
  static_assert(findLastNotOf(c_highByteInside, c_highByteInsideLength, "b\xff", c_npos, 2) == 0,
                "a high byte counts as a member scanning backwards");
}

// What a search sees of a buffer the length cuts short: nothing past it, and no terminator required.
TEST_CASE("string_utils/search/reads_only_the_counted_range") {
  // Every match below exists in the whole literal, but only past the leading part the length names.
  CHECK(find(c_repeated, c_leadingLength, 'b', 2) == c_npos);
  CHECK(find(c_repeated, c_leadingLength, "abra", 1, 4) == c_npos);
  CHECK(rfind(c_repeated, c_leadingLength, 'a', c_npos) == 5);
  CHECK(rfind(c_repeated, c_leadingLength, "abra", c_npos, 4) == 0);
  CHECK(findFirstOf(c_repeated, c_leadingLength, "r", 3, 1) == c_npos);
  CHECK(findLastOf(c_repeated, c_leadingLength, "br", c_npos, 2) == 2);
  CHECK(findFirstNotOf(c_runOfA, c_runOfALength, "a", 0, 1) == c_npos);
  CHECK(findLastNotOf(c_runOfA, c_runOfALength, "a", c_npos, 1) == c_npos);

  // Constant evaluation rejects a read past the array, so each assertion proves the search stays inside it.
  static_assert(find(c_unterminated, std::size(c_unterminated), 'z', 0) == c_npos,
                "a full scan must stop at the end of the range");
  static_assert(find(c_unterminated, std::size(c_unterminated), "bra", 2, 3) == 8,
                "a sequence search must stop at the end of the range");
  static_assert(rfind(c_unterminated, std::size(c_unterminated), "bra", c_npos, 3) == 8,
                "a backward sequence search must start inside the range");
  static_assert(findFirstOf(c_unterminated, std::size(c_unterminated), "z", 0, 1) == c_npos,
                "a set search must stop at the end of the range");
  static_assert(findLastNotOf(c_unterminated, std::size(c_unterminated), 'a', c_npos) == 9,
                "a backward search must start at the last character");
}

// How a terminator inside the range compares: like any other character, in a pattern, in a set, and as a skip.
TEST_CASE("string_utils/search/matches_embedded_null") {
  CHECK(find(c_embeddedNull, std::size(c_embeddedNull), '\0', 0) == 2);
  CHECK(find(c_embeddedNull, std::size(c_embeddedNull), 'a', 0) == 3);
  CHECK(rfind(c_embeddedNull, std::size(c_embeddedNull), "\0a", c_npos, 2) == 2);
  CHECK(findFirstOf(c_embeddedNull, std::size(c_embeddedNull), "y\0", 0, 2) == 2);
  CHECK(findFirstNotOf(c_embeddedNull, std::size(c_embeddedNull), "pl", 0, 2) == 2);
  CHECK(findLastNotOf(c_embeddedNull, std::size(c_embeddedNull), 'y', c_npos) == 3);

  static_assert(find(c_embeddedNull, std::size(c_embeddedNull), 'a', 0) == 3,
                "a search must continue past a terminator inside the range");
  static_assert(rfind(c_embeddedNull, std::size(c_embeddedNull), "\0a", c_npos, 2) == 2,
                "a terminator must match inside a sequence");
  static_assert(findFirstOf(c_embeddedNull, std::size(c_embeddedNull), "y\0", 0, 2) == 2,
                "a terminator must count as a member of a set");
}

// What a null pointer with a zero length yields: the empty-range or empty-set answer, with nothing read.
TEST_CASE("string_utils/search/accepts_null_with_zero_length") {
  CHECK(find(nullptr, 0, 'a', 0) == c_npos);
  CHECK(find(nullptr, 0, "a", 0, 1) == c_npos);
  CHECK(find(nullptr, 0, nullptr, 0, 0) == 0);
  CHECK(rfind(nullptr, 0, 'a', c_npos) == c_npos);
  CHECK(rfind(nullptr, 0, nullptr, c_npos, 0) == 0);
  CHECK(findLastOf(nullptr, 0, "a", c_npos, 1) == c_npos);
  CHECK(findFirstNotOf(nullptr, 0, 'a', 0) == c_npos);
  CHECK(findLastNotOf(nullptr, 0, nullptr, c_npos, 0) == c_npos);

  // A null pattern or set with a zero count, searched for in a non-empty range.
  CHECK(find(c_repeated, c_repeatedLength, nullptr, 4, 0) == 4);
  CHECK(findFirstOf(c_repeated, c_repeatedLength, nullptr, 0, 0) == c_npos);
  CHECK(findFirstNotOf(c_repeated, c_repeatedLength, nullptr, 2, 0) == 2);
  CHECK(findLastNotOf(c_repeated, c_repeatedLength, nullptr, c_npos, 0) == c_repeatedLength - 1);

  static_assert(find(nullptr, 0, nullptr, 0, 0) == 0, "an empty pattern matches at the start of an empty range");
  static_assert(rfind(nullptr, 0, 'a', c_npos) == c_npos, "an empty range holds no character");
  static_assert(findFirstNotOf(c_repeated, c_repeatedLength, nullptr, 2, 0) == 2,
                "a null set with a zero count excludes nothing");
}

// Two ranges holding the same characters compare equal, whichever buffers hold them.
TEST_CASE("string_utils/compare/equal_ranges") {
  CHECK(compare(c_repeated, c_repeatedLength, c_repeated, c_repeatedLength) == 0);
  CHECK(compare(c_repeated, c_repeatedLength, c_unterminated, std::size(c_unterminated)) == 0);

  // Two empty ranges compare equal, a null pointer with a zero length among them.
  CHECK(compare(c_repeated, 0, "", 0) == 0);
  CHECK(compare(nullptr, 0, nullptr, 0) == 0);

  static_assert(compare(c_repeated, c_repeatedLength, c_unterminated, std::size(c_unterminated)) == 0,
                "the same characters in another buffer must compare equal");
  static_assert(compare(nullptr, 0, nullptr, 0) == 0, "two empty ranges must compare equal");
}

// The first differing character decides the order, whatever the lengths, and swapping the ranges flips it.
TEST_CASE("string_utils/compare/first_difference_decides") {
  CHECK(compare(c_repeated, c_repeatedLength, c_repeatedFirstDiffers, c_repeatedLength) < 0);
  CHECK(compare(c_repeatedFirstDiffers, c_repeatedLength, c_repeated, c_repeatedLength) > 0);
  CHECK(compare(c_repeated, c_repeatedLength, c_repeatedMiddleDiffers, c_repeatedLength) > 0);
  CHECK(compare(c_repeated, c_repeatedLength, c_repeatedLastDiffers, c_repeatedLength) < 0);

  // A difference in the first character outweighs a shorter length.
  CHECK(compare("b", 1, c_repeated, c_repeatedLength) > 0);

  // Bytes order as unsigned char, so a byte above the ASCII range follows an ASCII one.
  CHECK(compare(c_highByteInside + 1, 1, c_highByteInside, 1) > 0);

  static_assert(compare(c_repeated, c_repeatedLength, c_repeatedFirstDiffers, c_repeatedLength) < 0,
                "a smaller first character must order first");
  static_assert(compare(c_repeatedFirstDiffers, c_repeatedLength, c_repeated, c_repeatedLength) > 0,
                "swapping the ranges must flip the order");
  static_assert(compare(c_repeated, c_repeatedLength, c_repeatedLastDiffers, c_repeatedLength) < 0,
                "a difference in the last character must still decide");
  static_assert(compare("b", 1, c_repeated, c_repeatedLength) > 0, "the first difference must outweigh the lengths");
  static_assert(compare(c_highByteInside + 1, 1, c_highByteInside, 1) > 0, "a high byte must order after an ASCII one");
}

// When one range is a prefix of the other, the shorter one orders first.
TEST_CASE("string_utils/compare/shorter_prefix_orders_first") {
  CHECK(compare(c_repeated, c_repeatedLength, c_repeatedLonger, c_repeatedLongerLength) < 0);
  CHECK(compare(c_repeatedLonger, c_repeatedLongerLength, c_repeated, c_repeatedLength) > 0);
  CHECK(compare(c_repeated, c_leadingLength, c_repeated, c_repeatedLength) < 0);

  // An empty range is a prefix of every other one.
  CHECK(compare(nullptr, 0, c_repeated, c_repeatedLength) < 0);
  CHECK(compare(c_repeated, c_repeatedLength, "", 0) > 0);

  static_assert(compare(c_repeated, c_repeatedLength, c_repeatedLonger, c_repeatedLongerLength) < 0,
                "a prefix must order before the longer range");
  static_assert(compare(c_repeatedLonger, c_repeatedLongerLength, c_repeated, c_repeatedLength) > 0,
                "the longer range must order after its prefix");
  static_assert(compare(nullptr, 0, c_repeated, c_repeatedLength) < 0, "an empty range must order first");
}

// What a comparison sees of its ranges: the counted characters only, a null byte among them included.
TEST_CASE("string_utils/compare/reads_only_the_counted_ranges") {
  // The lengths cut both literals before they part, so the parts compare equal.
  CHECK(compare(c_repeated, c_leadingLength, c_repeatedLastDiffers, c_leadingLength) == 0);

  // A null byte compares as a character ordering before any letter, where a terminator scan would stop.
  CHECK(compare(c_embeddedNull, std::size(c_embeddedNull), "pl", 2) > 0);
  CHECK(compare(c_embeddedNull, std::size(c_embeddedNull), "pla", 3) < 0);

  static_assert(compare(c_repeated, c_leadingLength, c_repeatedLastDiffers, c_leadingLength) == 0,
                "characters past the lengths must not take part");
  static_assert(compare(c_embeddedNull, std::size(c_embeddedNull), "pla", 3) < 0,
                "a null byte must compare as a character");

  // Constant evaluation rejects a read past the array, so the comparison stays inside both ranges.
  static_assert(compare(c_unterminated, std::size(c_unterminated), c_repeatedLonger, c_repeatedLongerLength) < 0,
                "the comparison must stop at the end of the shorter range");
}

} // namespace toy::string_utils
