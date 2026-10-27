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
  \brief  Character search and comparison over a pointer and a length, plus UTF-8 and UTF-16 counting, validation, and
          conversion.

  Defines the \ref toy::string_utils functions behind the matching members of \ref toy::StringView and
  \ref toy::String, and the conversions for text that crosses a platform API taking UTF-16. Conversions write into
  caller-supplied buffers and allocate nothing.

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

  \param data First byte of the searched range.
  \param size Number of bytes in the searched range.
  \param ch   Character to look for.
  \param pos  Offset the search starts from.

  \return Offset of the first \a ch at or after \a pos, or \ref toy::string_utils::npos when there is none; a \a pos
          at or past \a size always gives \ref toy::string_utils::npos.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.

  \sa toy::string_utils::rfind(const char *, size_t, char, size_t)
*/
[[nodiscard]] constexpr size_t find(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first position at or after \a pos where the \a count bytes starting at \a pattern start.

  Matches the bytes of \a pattern in order and next to each other; toy::string_utils::findFirstOf() matches any one of
  them. Reads exactly \a count bytes of the pattern, so it needs no terminator and may hold \c '\\0'.

  \param data    First byte of the searched range.
  \param size    Number of bytes in the searched range.
  \param pattern First byte of the pattern.
  \param pos     Offset the search starts from.
  \param count   Number of bytes in the pattern.

  \return Offset where the first match starts, or \ref toy::string_utils::npos when there is none. An empty pattern
          matches at \a pos while \a pos is not greater than \a size.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a pattern is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a pattern are readable.

  \sa toy::string_utils::rfind(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t find(const char * data, size_t size, const char * pattern, size_t pos,
                                    size_t count) noexcept;

/*!
  \brief Finds the last occurrence of \a ch at or before \a pos.

  \param data First byte of the searched range.
  \param size Number of bytes in the searched range.
  \param ch   Character to look for.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last \a ch at or before \a pos, or \ref toy::string_utils::npos when there is none.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.

  \sa toy::string_utils::find(const char *, size_t, char, size_t)
*/
[[nodiscard]] constexpr size_t rfind(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last position at or before \a pos where the \a count bytes starting at \a pattern start.

  Matches the bytes of \a pattern in order and next to each other; toy::string_utils::findLastOf() matches any one of
  them. Reads exactly \a count bytes of the pattern, so it needs no terminator and may hold \c '\\0'.

  \param data    First byte of the searched range.
  \param size    Number of bytes in the searched range.
  \param pattern First byte of the pattern.
  \param pos     Last offset a match may start at; any offset past the range covers all of it.
  \param count   Number of bytes in the pattern.

  \return Offset where the last match starts, or \ref toy::string_utils::npos when there is none. An empty pattern
          matches at the smaller of \a pos and \a size.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a pattern is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a pattern are readable.

  \sa toy::string_utils::find(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t rfind(const char * data, size_t size, const char * pattern, size_t pos,
                                     size_t count) noexcept;

/*!
  \brief Finds the first character at or after \a pos that belongs to the \a count bytes starting at \a set.

  Reads the bytes of \a set as a set, so their order carries no meaning; toy::string_utils::find() matches them as a
  sequence.

  \param data  First byte of the searched range.
  \param size  Number of bytes in the searched range.
  \param set   First byte of the set.
  \param pos   Offset the search starts from.
  \param count Number of bytes in the set.

  \return Offset of the first character that belongs to the set, or \ref toy::string_utils::npos when there is none.
          An empty set matches nothing.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a set are readable.

  \sa toy::string_utils::findLastOf(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t findFirstOf(const char * data, size_t size, const char * set, size_t pos,
                                           size_t count) noexcept;

/*!
  \brief Finds the last character at or before \a pos that belongs to the \a count bytes starting at \a set.

  Reads the bytes of \a set as a set, so their order carries no meaning; toy::string_utils::rfind() matches them as a
  sequence.

  \param data  First byte of the searched range.
  \param size  Number of bytes in the searched range.
  \param set   First byte of the set.
  \param pos   Last offset the search considers; any offset past the range covers all of it.
  \param count Number of bytes in the set.

  \return Offset of the last character that belongs to the set, or \ref toy::string_utils::npos when there is none. An
          empty set matches nothing.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a set are readable.

  \sa toy::string_utils::findFirstOf(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t findLastOf(const char * data, size_t size, const char * set, size_t pos,
                                          size_t count) noexcept;

/*!
  \brief Finds the first character at or after \a pos that differs from \a ch.

  \param data First byte of the searched range.
  \param size Number of bytes in the searched range.
  \param ch   Character to skip.
  \param pos  Offset the search starts from.

  \return Offset of the first character other than \a ch at or after \a pos, or \ref toy::string_utils::npos when
          there is none.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.

  \sa toy::string_utils::findLastNotOf(const char *, size_t, char, size_t)
*/
[[nodiscard]] constexpr size_t findFirstNotOf(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first character at or after \a pos that does not belong to the \a count bytes starting at \a set.

  \param data  First byte of the searched range.
  \param size  Number of bytes in the searched range.
  \param set   First byte of the set.
  \param pos   Offset the search starts from.
  \param count Number of bytes in the set.

  \return Offset of the first character that does not belong to the set, or \ref toy::string_utils::npos when there is
          none. No character belongs to an empty set, so the result is then \a pos while \a pos is less than \a size.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a set are readable.

  \sa toy::string_utils::findLastNotOf(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t findFirstNotOf(const char * data, size_t size, const char * set, size_t pos,
                                              size_t count) noexcept;

/*!
  \brief Finds the last character at or before \a pos that differs from \a ch.

  \param data First byte of the searched range.
  \param size Number of bytes in the searched range.
  \param ch   Character to skip.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last character other than \a ch at or before \a pos, or \ref toy::string_utils::npos when
          there is none.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.

  \sa toy::string_utils::findFirstNotOf(const char *, size_t, char, size_t)
*/
[[nodiscard]] constexpr size_t findLastNotOf(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last character at or before \a pos that does not belong to the \a count bytes starting at \a set.

  \param data  First byte of the searched range.
  \param size  Number of bytes in the searched range.
  \param set   First byte of the set.
  \param pos   Last offset the search considers; any offset past the range covers all of it.
  \param count Number of bytes in the set.

  \return Offset of the last character that does not belong to the set, or \ref toy::string_utils::npos when there is
          none. No character belongs to an empty set, so the result is then \a pos, or \a size minus one when \a pos is
          past that, or \ref toy::string_utils::npos for an empty range.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a set are readable.

  \sa toy::string_utils::findFirstNotOf(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t findLastNotOf(const char * data, size_t size, const char * set, size_t pos,
                                             size_t count) noexcept;

/*!
  \brief Compares the \a lhsSize bytes starting at \a lhs with the \a rhsSize bytes starting at \a rhs in lexicographic
         order.

  Compares bytes as \c unsigned \c char up to the shorter length; when one range is a prefix of the other, the shorter
  one orders first. A \c '\\0' inside either range compares like any other byte.

  \param lhs     First byte of the left range.
  \param lhsSize Number of bytes in the left range.
  \param rhs     First byte of the right range.
  \param rhsSize Number of bytes in the right range.

  \return A negative value when the left range orders first, \c 0 when both hold the same characters, a positive value
          when the right range orders first.

  \pre \a lhs is non-null or \a lhsSize is \c 0, checked by assert_message in debug builds.
  \pre The \a lhsSize bytes starting at \a lhs are readable.
  \pre \a rhs is non-null or \a rhsSize is \c 0, checked by assert_message in debug builds.
  \pre The \a rhsSize bytes starting at \a rhs are readable.

  \note Only the sign carries meaning; the magnitude is whatever the byte comparison produced.

  \sa toy::string_utils::find(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr int compare(const char * lhs, size_t lhsSize, const char * rhs, size_t rhsSize) noexcept;

/*!
  \brief Counts the characters the \a size bytes starting at \a data encode in UTF-8.

  Counts the bytes outside the 10xxxxxx continuation form, so a character counts once whatever width it is stored in.
  Reads exactly \a size bytes, so it needs no terminator and counts a \c '\\0' inside the range as a character.

  \param data First byte of the range.
  \param size Number of bytes in the range.

  \return Count of encoded characters, \c 0 for an empty range.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data hold well-formed UTF-8, checked with
       toy::string_utils::validateUtf8(const char *, size_t) by assert_message in debug builds outside constant
       evaluation. A shipping build and a constant expression skip that check and count every byte outside the
       continuation form, a stray lead byte included.

  \note The count matches \a size only where every character is ASCII.

  \sa toy::string_utils::compare(const char *, size_t, const char *, size_t)
*/
[[nodiscard]] constexpr size_t utf8Size(const char * data, size_t size) noexcept;

/*!
  \brief Largest number of UTF-8 bytes one UTF-16 code unit converts to.

  Sizes buffers for converting UTF-16 text: a unit outside the surrogate range takes at most three bytes, and a
  surrogate pair takes four for its two units.
*/
constexpr size_t utf16InUtf8MaxSize = 3;

/*!
  \brief Converts a range of UTF-8 bytes to a null-terminated UTF-16 string in a caller-supplied buffer.

  Decodes a sequence of one to three bytes into one \c char16_t unit and a four-byte sequence into a surrogate pair.
  Only \a count bounds the input, so a \c '\\0' inside the range comes out as \c u'\\0' in \a dest. Allocates nothing;
  the result is always null-terminated.

  \param dest     Buffer that receives the UTF-16 string.
  \param destSize Size of \a dest in \c char16_t units, the terminator included.
  \param src      UTF-8 bytes to convert, or \c nullptr for an empty result.
  \param count    Number of bytes to read from \a src.

  \return \a dest, or \c nullptr when \a dest is \c nullptr or \a destSize is \c 0; \a dest is then left untouched.

  \pre The \a count bytes starting at \a src are readable.

  \post \a dest holds the characters decoded from the start of \a src, followed by \c u'\\0'. Decoding stops at the end
        of the range, before a character that does not fit whole, and before the first sequence
        toy::string_utils::validateUtf8() rejects, among them an overlong encoding, an encoded surrogate, and a code
        point past U+10FFFF.

  \note Truncation and ill-formed input go unreported: a buffer of \a count + 1 units holds every character the call
        decodes.
  \note The conversion is a runtime one: the definition lives in a translation unit, out of reach of a constant
        expression.

  \sa toy::string_utils::utf16ToUtf8(), toy::string_utils::validateUtf8()
*/
char16_t * utf8ToUtf16(char16_t * dest, size_t destSize, const char * src, size_t count) noexcept;

/*!
  \brief Converts a range of UTF-16 units to a null-terminated UTF-8 string in a caller-supplied buffer.

  Encodes a unit outside the surrogate range as one to three bytes and a surrogate pair as one four-byte sequence, so
  the result is well-formed UTF-8. Only \a count bounds the input, so a \c u'\\0' inside the range comes out as
  \c '\\0' in \a dest. Allocates nothing; the result is always null-terminated.

  \param dest     Buffer that receives the UTF-8 string.
  \param destSize Size of \a dest in bytes, the terminator included.
  \param src      UTF-16 units to convert, or \c nullptr for an empty result.
  \param count    Number of units to read from \a src.

  \return \a dest, or \c nullptr when \a dest is \c nullptr or \a destSize is \c 0; \a dest is then left untouched.

  \pre The \a count units starting at \a src are readable.

  \post \a dest holds the encoded characters from the start of \a src, followed by \c '\\0'. Encoding stops at the end
        of the range, before a character that does not fit whole in \a destSize - 1 bytes, and before a surrogate
        whose pair does not lie whole inside the range.

  \note Truncation and lone surrogates go unreported: a buffer of \ref toy::string_utils::utf16InUtf8MaxSize bytes per
        unit of the range, plus one for the terminator, holds every character the call encodes.
  \note The conversion is a runtime one: the definition lives in a translation unit, out of reach of a constant
        expression.

  \sa toy::string_utils::utf8ToUtf16(), toy::string_utils::validateUtf8()
*/
char * utf16ToUtf8(char * dest, size_t destSize, const char16_t * src, size_t count) noexcept;

/*!
  \brief Counts the bytes the UTF-8 form of a range of UTF-16 units takes.

  Measures what toy::string_utils::utf16ToUtf8() writes for the same range into a buffer with room for all of it: one
  to three bytes for a unit outside the surrogate range, four for a surrogate pair. For units that
  toy::string_utils::utf8ToUtf16() decoded, the count equals the bytes that call consumed. Allocates nothing.

  \param src   UTF-16 units to measure, or \c nullptr for an empty range.
  \param count Number of units to read from \a src.

  \return Byte count of the UTF-8 form, the terminator excluded; \c 0 for an empty range.

  \pre The \a count units starting at \a src are readable.

  \note The count stops before a surrogate whose pair does not lie whole inside the range, where
        toy::string_utils::utf16ToUtf8() stops encoding.
  \note The count is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.

  \sa toy::string_utils::utf16ToUtf8(), toy::string_utils::utf8ToUtf16()
*/
[[nodiscard]] size_t utf16InUtf8Size(const char16_t * src, size_t count) noexcept;

/*!
  \brief Counts the characters a UTF-8 string encodes.

  Walks the string one lead byte at a time and steps over the sequence that byte announces, so a character stored in
  two, three, or four bytes counts once. The walk trusts the width the lead byte announces;
  toy::string_utils::validateUtf8() checks the bytes behind it.

  \param str Null-terminated UTF-8 string to measure.

  \return Count of encoded characters, \c 0 over an empty string, and \c 0 where the walk stops early.

  \pre \a str is non-null, checked by assert_message in debug builds; a shipping build counts \c 0 instead.
  \pre \a str holds well-formed UTF-8, checked with toy::string_utils::validateUtf8() by assert_message in debug builds.
       A shipping build skips that check: a byte starting no sequence and a sequence the terminator cuts short each end
       the count at \c 0, while an overlong, a surrogate, and a code point past U+10FFFF each count as one character.

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

  \note Asserts nothing, so it fits as the condition of an assert_message check, which is how utf8Len() states its
        precondition.
  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in the length of the string, one pass, so a caller checking before a walk pays for two.

  \sa toy::string_utils::utf8Len(), toy::string_utils::validateUtf16()
*/
[[nodiscard]] bool validateUtf8(const char * str) noexcept;

/*!
  \brief Reports whether the \a size bytes starting at \a str are well-formed UTF-8.

  Runs the checks of toy::string_utils::validateUtf8(const char *) over a range that \a size bounds instead of a
  terminator. No byte past \a size is read, and a \c '\\0' inside the range counts as a one-byte character.

  \param str  First byte of the range.
  \param size Number of bytes in the range.

  \return \c true when every sequence is well-formed and ends inside the range, \c false at the first that is not. An
          empty range is well-formed, a null \a str included; a null \a str with a non-zero \a size gives \c false.

  \pre The \a size bytes starting at \a str are readable.

  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in \a size, one pass.

  \sa toy::string_utils::validateUtf8(const char *)
*/
[[nodiscard]] bool validateUtf8(const char * str, size_t size) noexcept;

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
  \brief Reports whether the \a size units starting at \a str are well-formed UTF-16.

  Runs the checks of toy::string_utils::validateUtf16(const char16_t *) over a range that \a size bounds instead of a
  terminator. No unit past \a size is read, and a \c u'\\0' inside the range counts as a one-unit character.

  \param str  First unit of the range.
  \param size Number of units in the range.

  \return \c true when every surrogate is half of a pair inside the range, \c false at the first that is not. An empty
          range is well-formed, a null \a str included; a null \a str with a non-zero \a size gives \c false.

  \pre The \a size units starting at \a str are readable.

  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in \a size, one pass.

  \sa toy::string_utils::validateUtf16(const char16_t *)
*/
[[nodiscard]] bool validateUtf16(const char16_t * str, size_t size) noexcept;

/*!
  \brief Reverses the order of the \a count bytes starting at \a str in place.

  Treats the range as bytes: a multi-byte UTF-8 character comes out with its bytes reversed, which leaves the range
  ill-formed UTF-8 wherever it holds one. Reads and writes exactly \a count bytes, so it needs no terminator and moves a
  \c '\\0' inside the range like any other byte. Allocates nothing.

  \param str   First byte of the range.
  \param count Number of bytes in the range.

  \return \a str.

  \pre \a str is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a str are readable and writable.

  \post The \a count bytes starting at \a str hold their previous values in reverse order; the bytes past the range keep
        theirs.

  \note Works in a constant expression.

  \sa toy::string_utils::validateUtf8(const char *, size_t)
*/
constexpr char * reverseString(char * str, size_t count) noexcept;

} // namespace toy::string_utils

#endif // INCLUDE_CORE_STRING_UTILS_HPP_
