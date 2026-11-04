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
  \file   string_utils.hpp
  \brief  Text utilities the core module shares across its types.

  Defines the \ref toy::string_utils functions: character search and comparison over a toy::span, behind the matching
  members of \ref toy::StringView and \ref toy::String, and UTF-8 and UTF-16 conversion, counting, and validation over
  a span or a null-terminated string.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_UTILS_HPP_
#define INCLUDE_CORE_STRING_UTILS_HPP_

namespace toy::string_utils {

/// Character operations every search compares and scans with.
using traits_type = std::char_traits<char>;

/// Offset a search returns when nothing matches.
inline constexpr size_t npos = static_cast<size_t>(-1);

/*!
  \brief Finds the first occurrence of \a ch at or after \a pos.

  \param data Characters to search.
  \param ch   Character to look for.
  \param pos  Offset the search starts from.

  \return Offset of the first \a ch at or after \a pos, or \ref toy::string_utils::npos when there is none; a \a pos
          at or past the range length always gives \ref toy::string_utils::npos.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::rfind(toy::span<const char>, char, size_t)
*/
[[nodiscard]] constexpr size_t find(span<const char> data, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first position at or after \a pos where \a pattern starts.

  Matches the characters of \a pattern in order and next to each other; toy::string_utils::findFirstOf() matches any
  one of them. Reads the whole of \a pattern, so it needs no terminator and may hold \c '\\0'.

  \param data    Characters to search.
  \param pattern Characters to look for, in order and next to each other.
  \param pos     Offset the search starts from.

  \return Offset where the first match starts, or \ref toy::string_utils::npos when there is none. An empty pattern
          matches at \a pos while \a pos is not greater than the range length.

  \pre Every character of \a data and of \a pattern is readable.
  \pre A null \a data or \a pattern comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::rfind(toy::span<const char>, toy::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t find(span<const char> data, span<const char> pattern, size_t pos) noexcept;

/*!
  \brief Finds the last occurrence of \a ch at or before \a pos.

  \param data Characters to search.
  \param ch   Character to look for.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last \a ch at or before \a pos, or \ref toy::string_utils::npos when there is none.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::find(toy::span<const char>, char, size_t)
*/
[[nodiscard]] constexpr size_t rfind(span<const char> data, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last position at or before \a pos where \a pattern starts.

  Matches the characters of \a pattern in order and next to each other; toy::string_utils::findLastOf() matches any
  one of them. Reads the whole of \a pattern, so it needs no terminator and may hold \c '\\0'.

  \param data    Characters to search.
  \param pattern Characters to look for, in order and next to each other.
  \param pos     Last offset a match may start at; any offset past the range covers all of it.

  \return Offset where the last match starts, or \ref toy::string_utils::npos when there is none. An empty pattern
          matches at the smaller of \a pos and the range length.

  \pre Every character of \a data and of \a pattern is readable.
  \pre A null \a data or \a pattern comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::find(toy::span<const char>, toy::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t rfind(span<const char> data, span<const char> pattern, size_t pos) noexcept;

/*!
  \brief Finds the first character at or after \a pos that belongs to \a set.

  Reads \a set as a set, so the order of its characters carries no meaning; toy::string_utils::find() matches them as a
  sequence.

  \param data Characters to search.
  \param set  Characters a match may equal.
  \param pos  Offset the search starts from.

  \return Offset of the first character that belongs to the set, or \ref toy::string_utils::npos when there is none.
          An empty set matches nothing.

  \pre Every character of \a data and of \a set is readable.
  \pre A null \a data or \a set comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findLastOf(toy::span<const char>, toy::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t findFirstOf(span<const char> data, span<const char> set, size_t pos) noexcept;

/*!
  \brief Finds the last character at or before \a pos that belongs to \a set.

  Reads \a set as a set, so the order of its characters carries no meaning; toy::string_utils::rfind() matches them as a
  sequence.

  \param data Characters to search.
  \param set  Characters a match may equal.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last character that belongs to the set, or \ref toy::string_utils::npos when there is none. An
          empty set matches nothing.

  \pre Every character of \a data and of \a set is readable.
  \pre A null \a data or \a set comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findFirstOf(toy::span<const char>, toy::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t findLastOf(span<const char> data, span<const char> set, size_t pos) noexcept;

/*!
  \brief Finds the first character at or after \a pos that differs from \a ch.

  \param data Characters to search.
  \param ch   Character to skip.
  \param pos  Offset the search starts from.

  \return Offset of the first character other than \a ch at or after \a pos, or \ref toy::string_utils::npos when
          there is none.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findLastNotOf(toy::span<const char>, char, size_t)
*/
[[nodiscard]] constexpr size_t findFirstNotOf(span<const char> data, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first character at or after \a pos that does not belong to \a set.

  \param data Characters to search.
  \param set  Characters to exclude.
  \param pos  Offset the search starts from.

  \return Offset of the first character that does not belong to the set, or \ref toy::string_utils::npos when there is
          none. No character belongs to an empty set, so the result is then \a pos while \a pos is less than the range
          length.

  \pre Every character of \a data and of \a set is readable.
  \pre A null \a data or \a set comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findLastNotOf(toy::span<const char>, toy::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t findFirstNotOf(span<const char> data, span<const char> set, size_t pos) noexcept;

/*!
  \brief Finds the last character at or before \a pos that differs from \a ch.

  \param data Characters to search.
  \param ch   Character to skip.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last character other than \a ch at or before \a pos, or \ref toy::string_utils::npos when
          there is none.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findFirstNotOf(toy::span<const char>, char, size_t)
*/
[[nodiscard]] constexpr size_t findLastNotOf(span<const char> data, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last character at or before \a pos that does not belong to \a set.

  \param data Characters to search.
  \param set  Characters to exclude.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last character that does not belong to the set, or \ref toy::string_utils::npos when there is
          none. No character belongs to an empty set, so the result is then \a pos, or one before the range end when
          \a pos is past that, or \ref toy::string_utils::npos for an empty range.

  \pre Every character of \a data and of \a set is readable.
  \pre A null \a data or \a set comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findFirstNotOf(toy::span<const char>, toy::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t findLastNotOf(span<const char> data, span<const char> set, size_t pos) noexcept;

/*!
  \brief Compares \a lhs with \a rhs in lexicographic order.

  Compares bytes as \c unsigned \c char up to the shorter length; when one range is a prefix of the other, the shorter
  one orders first. A \c '\\0' inside either range compares like any other byte.

  \param lhs Left range.
  \param rhs Right range.

  \return A negative value when the left range orders first, \c 0 when both hold the same characters, a positive value
          when the right range orders first.

  \pre Every character of \a lhs and of \a rhs is readable.
  \pre A null \a lhs or \a rhs comes with an empty range, checked by assert_message in debug builds.

  \note Only the sign carries meaning; the magnitude is whatever the byte comparison produced.

  \sa toy::string_utils::find(toy::span<const char>, toy::span<const char>, size_t)
*/
[[nodiscard]] constexpr int compare(span<const char> lhs, span<const char> rhs) noexcept;

/*!
  \brief Counts the characters \a data encodes in UTF-8.

  Counts the bytes outside the 10xxxxxx continuation form, so a character counts once whatever width it is stored in.
  Reads the whole of \a data, so it needs no terminator and counts a \c '\\0' inside the range as a character.

  \param data Range to count.

  \return Count of encoded characters, \c 0 for an empty range.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.
  \pre \a data holds well-formed UTF-8, checked with toy::string_utils::validateUtf8(toy::span<const char>) by
       assert_message in debug builds outside constant evaluation. A shipping build and a constant expression skip
       that check and count every byte outside the continuation form, a stray lead byte included.

  \note The count matches the range length only where every character is ASCII.

  \sa toy::string_utils::compare(toy::span<const char>, toy::span<const char>)
*/
[[nodiscard]] constexpr size_t utf8Size(span<const char> data) noexcept;

/*!
  \brief Largest number of UTF-8 bytes one UTF-16 code unit converts to.

  Sizes buffers for converting UTF-16 text: a unit outside the surrogate range takes at most three bytes, and a
  surrogate pair takes four for its two units.
*/
constexpr size_t utf16InUtf8MaxSize = 3;

/*!
  \brief Converts a range of UTF-8 bytes to a null-terminated UTF-16 string in a caller-supplied buffer.

  Decodes a sequence of one to three bytes into one \c char16_t unit and a four-byte sequence into a surrogate pair.
  Only the length of \a src bounds the input, so a \c '\\0' inside the range comes out as \c u'\\0' in \a dest.
  Allocates nothing; the result is always null-terminated.

  \param dest Buffer that receives the UTF-16 string; its size is the capacity, terminator included.
  \param src  UTF-8 bytes to convert. An empty range, or one whose data pointer is \c nullptr, converts nothing.

  \return \a dest.data(), or \c nullptr when \a dest is empty or names no data; \a dest is then left untouched.

  \pre Every byte of \a src is readable.

  \post \a dest holds the characters decoded from the start of \a src, followed by \c u'\\0'. Decoding stops at the end
        of the range, before a character that does not fit whole, and before the first sequence
        toy::string_utils::validateUtf8() rejects, among them an overlong encoding, an encoded surrogate, and a code
        point past U+10FFFF.

  \note Truncation and ill-formed input go unreported: a buffer of \a src.size() + 1 units holds every character the
        call decodes.
  \note The conversion is a runtime one: the definition lives in a translation unit, out of reach of a constant
        expression.

  \sa toy::string_utils::utf16ToUtf8(), toy::string_utils::validateUtf8()
*/
char16_t * utf8ToUtf16(span<char16_t> dest, span<const char> src) noexcept;

/*!
  \brief Converts a range of UTF-16 units to a null-terminated UTF-8 string in a caller-supplied buffer.

  Encodes a unit outside the surrogate range as one to three bytes and a surrogate pair as one four-byte sequence, so
  the result is well-formed UTF-8. Only the length of \a src bounds the input, so a \c u'\\0' inside the range comes
  out as \c '\\0' in \a dest. Allocates nothing; the result is always null-terminated.

  \param dest Buffer that receives the UTF-8 string; its size is the capacity, terminator included.
  \param src  UTF-16 units to convert. An empty range, or one whose data pointer is \c nullptr, encodes nothing.

  \return \a dest.data(), or \c nullptr when \a dest is empty or names no data; \a dest is then left untouched.

  \pre Every code unit of \a src is readable.

  \post \a dest holds the encoded characters from the start of \a src, followed by \c '\\0'. Encoding stops at the end
        of the range, before a character that does not fit whole in one less than the capacity of \a dest, and before a
        surrogate whose pair does not lie whole inside the range.

  \note Truncation and lone surrogates go unreported: a buffer of \ref toy::string_utils::utf16InUtf8MaxSize bytes per
        unit of the range, plus one for the terminator, holds every character the call encodes.
  \note The conversion is a runtime one: the definition lives in a translation unit, out of reach of a constant
        expression.

  \sa toy::string_utils::utf8ToUtf16(), toy::string_utils::validateUtf8()
*/
char * utf16ToUtf8(span<char> dest, span<const char16_t> src) noexcept;

/*!
  \brief Counts the bytes the UTF-8 form of a range of UTF-16 units takes.

  Measures what toy::string_utils::utf16ToUtf8() writes for the same range into a buffer with room for all of it: one
  to three bytes for a unit outside the surrogate range, four for a surrogate pair. For units that
  toy::string_utils::utf8ToUtf16() decoded, the count equals the bytes that call consumed. Allocates nothing.

  \param units UTF-16 units to measure.

  \return Byte count of the UTF-8 form, the terminator excluded; \c 0 for an empty range.

  \pre Every code unit of \a units is readable.

  \note The count stops before a surrogate whose pair does not lie whole inside the range, where
        toy::string_utils::utf16ToUtf8() stops encoding.
  \note The count is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.

  \sa toy::string_utils::utf16ToUtf8(), toy::string_utils::utf8ToUtf16()
*/
[[nodiscard]] size_t utf16InUtf8Size(span<const char16_t> units) noexcept;

/*!
  \brief Counts the characters a UTF-8 string encodes.

  Walks the string one lead byte at a time and steps over the sequence that byte announces, so a character stored in
  two, three, or four bytes counts once. The walk trusts the width the lead byte announces;
  toy::string_utils::validateUtf8() is what checks the bytes behind it.

  \param str Null-terminated UTF-8 string to measure.

  \return Count of encoded characters, \c 0 over an empty string, and \c 0 where the walk stops early.

  \pre \a str is non-null, checked by assert_message in debug builds; a shipping build counts \c 0 instead.
  \pre \a str holds well-formed UTF-8, checked with toy::string_utils::validateUtf8() by assert_message in debug
       builds. A shipping build skips that check: a byte starting no sequence and a sequence the terminator cuts short
       each end the count at \c 0, while an overlong, a surrogate, and a code point past U+10FFFF each count as one
       character.

  \note The count is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note The walk stops at the terminator and never reads past it, whatever width a lead byte announces.

  \sa toy::string_utils::utf16Len(), toy::string_utils::validateUtf8()
*/
[[nodiscard]] size_t utf8Len(const char * str) noexcept;

/*!
  \brief Counts the characters a UTF-16 string encodes.

  Walks the string one unit at a time and steps over a surrogate pair as a whole, so a character stored in one or two
  units counts once. The walk checks each surrogate against the unit after it, so it needs no separate pass of
  toy::string_utils::validateUtf16().

  \param str Null-terminated UTF-16 string to measure.

  \return Count of encoded characters, \c 0 over an empty string, and \c 0 where the walk stops early.

  \pre \a str is non-null, checked by assert_message in debug builds; a shipping build counts \c 0 instead.
  \pre \a str holds well-formed UTF-16, checked by assert_message in debug builds. A shipping build skips that check:
       a surrogate that is not half of a pair ends the count at \c 0.

  \note The count is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note The walk stops at the terminator and never reads past it, even after a high surrogate that ends the string.

  \sa toy::string_utils::utf8Len(), toy::string_utils::validateUtf16()
*/
[[nodiscard]] size_t utf16Len(const char16_t * str) noexcept;

/*!
  \brief Reports whether a byte string is well-formed UTF-8.

  Checks every sequence against Unicode table 3-7: the lead byte, the narrowed range of the byte after it, and the
  10xxxxxx form of the rest. That rejects an overlong encoding, a surrogate, and a code point past U+10FFFF, each of
  which a lead byte alone admits.

  \param str Null-terminated byte string to check.

  \return \c true when every sequence is well-formed, \c false at the first that is not and \c false for a null \a str.
          An empty string is well-formed.

  \note Asserts nothing, so it reads as the condition of an assert_message check, which is how
        toy::string_utils::utf8Len() states its precondition.
  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in the length of the string, one pass, so a caller checking before a walk pays for two.

  \sa toy::string_utils::utf8Len(), toy::string_utils::validateUtf16()
*/
[[nodiscard]] bool validateUtf8(const char * str) noexcept;

/*!
  \brief Reports whether \a str holds well-formed UTF-8.

  Runs the checks of toy::string_utils::validateUtf8(const char *) over a range bounded by the span's length, not by a
  terminator. Reads no byte past the range, and a \c '\\0' inside it counts as a one-byte character.

  \param str Range of bytes to check.

  \return \c true when every sequence is well-formed and ends inside the range, \c false at the first that is not. An
          empty range is well-formed, a null \a str included.

  \pre Every byte of \a str is readable, and a null \a str comes with an empty range, checked by assert_message in
       debug builds.

  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in the range length, one pass.
  \warning A shipping build skips the null check and answers \c false to a null \a str that carries bytes.

  \sa toy::string_utils::validateUtf8(const char *)
*/
[[nodiscard]] bool validateUtf8(span<const char> str) noexcept;

/*!
  \brief Reports whether a UTF-16 string is well-formed.

  Checks that every surrogate is half of a pair, a high surrogate followed by a low one. A unit outside the surrogate
  range is a whole character, so a surrogate outside a pair is the only ill-formed input.

  \param str Null-terminated UTF-16 string to check.

  \return \c true when every surrogate is half of a pair, \c false at the first that is not and \c false for a null
          \a str. An empty string is well-formed.

  \note Asserts nothing, so it reads as the condition of an assert_message check.
  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in the length of the string, one pass.

  \sa toy::string_utils::utf16Len(), toy::string_utils::validateUtf8()
*/
[[nodiscard]] bool validateUtf16(const char16_t * str) noexcept;

/*!
  \brief Reports whether \a str holds well-formed UTF-16.

  Runs the checks of toy::string_utils::validateUtf16(const char16_t *) over a range bounded by the span's length, not
  by a terminator. Reads no unit past the range, and a \c u'\\0' inside it counts as a one-unit character.

  \param str Range of code units to check.

  \return \c true when every surrogate is half of a pair inside the range, \c false at the first that is not. An empty
          range is well-formed, a null \a str included.

  \pre Every code unit of \a str is readable, and a null \a str comes with an empty range, checked by assert_message in
       debug builds.

  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in the range length, one pass.
  \warning A shipping build skips the null check and answers \c false to a null \a str that carries units.

  \sa toy::string_utils::validateUtf16(const char16_t *)
*/
[[nodiscard]] bool validateUtf16(span<const char16_t> str) noexcept;

template <std::signed_integral T>
[[nodiscard]] constexpr char * itoa(span<char> dest, T value) noexcept;

template <std::unsigned_integral T>
[[nodiscard]] constexpr char * utoa(char * dest, size_t destSize, T value) noexcept;

template <std::unsigned_integral T>
[[nodiscard]] constexpr char * utoaHex(char * dest, size_t destSize, T value) noexcept;

} // namespace toy::string_utils

#endif // INCLUDE_CORE_STRING_UTILS_HPP_
