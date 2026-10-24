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
  \file   utils.hpp
  \brief  Text utilities the core module shares across its types.

  Counts and validates UTF-8 and UTF-16 text, and converts between the two in caller-supplied buffers without
  allocating. Used where a byte length has to become a character count and where text crosses a platform API that takes
  UTF-16.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_UTILS_HPP_
#define INCLUDE_CORE_UTILS_HPP_

namespace toy {

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
        of the range, before a character that does not fit whole, and before the first sequence toy::validateUtf8()
        rejects, among them an overlong encoding, an encoded surrogate, and a code point past U+10FFFF.

  \note Truncation and ill-formed input go unreported: a buffer of \a count + 1 units holds every character the call
        decodes.
  \note The conversion is a runtime one: the definition lives in a translation unit, out of reach of a constant
        expression.

  \sa toy::utf16ToUtf8(), toy::validateUtf8()
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

  \note Truncation and lone surrogates go unreported: a buffer of \ref toy::utf16InUtf8MaxSize bytes per unit of the
        range, plus one for the terminator, holds every character the call encodes.
  \note The conversion is a runtime one: the definition lives in a translation unit, out of reach of a constant
        expression.

  \sa toy::utf8ToUtf16(), toy::validateUtf8()
*/
char * utf16ToUtf8(char * dest, size_t destSize, const char16_t * src, size_t count) noexcept;

/*!
  \brief Counts the bytes the UTF-8 form of a range of UTF-16 units takes.

  Measures what toy::utf16ToUtf8() writes for the same range into a buffer with room for all of it: one to three bytes
  for a unit outside the surrogate range, four for a surrogate pair. For units that toy::utf8ToUtf16() decoded, the
  count equals the bytes that call consumed. Allocates nothing.

  \param src   UTF-16 units to measure, or \c nullptr for an empty range.
  \param count Number of units to read from \a src.

  \return Byte count of the UTF-8 form, the terminator excluded; \c 0 for an empty range.

  \pre The \a count units starting at \a src are readable.

  \note The count stops before a surrogate whose pair does not lie whole inside the range, where toy::utf16ToUtf8()
        stops encoding.
  \note The count is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.

  \sa toy::utf16ToUtf8(), toy::utf8ToUtf16()
*/
[[nodiscard]] size_t utf16InUtf8Size(const char16_t * src, size_t count) noexcept;

/*!
  \brief Counts the characters a UTF-8 string encodes.

  Walks the string one lead byte at a time and steps over the sequence that byte announces, so a character stored in
  two, three, or four bytes counts once. The walk trusts the width the lead byte announces; toy::validateUtf8() is what
  checks the bytes behind it.

  \param str Null-terminated UTF-8 string to measure.

  \return Count of encoded characters, \c 0 over an empty string, and \c 0 where the walk stops early.

  \pre \a str is non-null, checked by assert_message in debug builds; a shipping build counts \c 0 instead.
  \pre \a str holds well-formed UTF-8, checked with toy::validateUtf8() by assert_message in debug builds. A shipping
       build skips that check: a byte starting no sequence and a sequence the terminator cuts short each end the count
       at \c 0, while an overlong, a surrogate, and a code point past U+10FFFF each count as one character.

  \note The count is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note The walk stops at the terminator and never reads past it, whatever width a lead byte announces.

  \sa toy::utf16Len(), toy::validateUtf8()
*/
[[nodiscard]] size_t utf8Len(const char * str) noexcept;

/*!
  \brief Counts the characters a UTF-16 string encodes.

  Walks the string one unit at a time and steps over a surrogate pair as a whole, so a character stored in one or two
  units counts once. The walk checks each surrogate against the unit after it, so it needs no separate pass of
  toy::validateUtf16().

  \param str Null-terminated UTF-16 string to measure.

  \return Count of encoded characters, \c 0 over an empty string, and \c 0 where the walk stops early.

  \pre \a str is non-null, checked by assert_message in debug builds; a shipping build counts \c 0 instead.
  \pre \a str holds well-formed UTF-16, checked by assert_message in debug builds. A shipping build skips that check:
       a surrogate that is not half of a pair ends the count at \c 0.

  \note The count is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note The walk stops at the terminator and never reads past it, even after a high surrogate that ends the string.

  \sa toy::utf8Len(), toy::validateUtf16()
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

  \note Asserts nothing, so it reads as the condition of an assert_message check — which is how utf8Len() states its
        precondition.
  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in the length of the string, one pass, so a caller checking before a walk pays for two.

  \sa toy::utf8Len(), toy::validateUtf16()
*/
[[nodiscard]] bool validateUtf8(const char * str) noexcept;

/*!
  \brief Reports whether the \a size bytes starting at \a str are well-formed UTF-8.

  Runs the checks of toy::validateUtf8(const char *) over a range that \a size bounds instead of a terminator. No byte
  past \a size is read, and a \c '\\0' inside the range counts as a one-byte character.

  \param str  First byte of the range.
  \param size Number of bytes in the range.

  \return \c true when every sequence is well-formed and ends inside the range, \c false at the first that is not. An
          empty range is well-formed, a null \a str included; a null \a str with a non-zero \a size gives \c false.

  \pre The \a size bytes starting at \a str are readable.

  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in \a size, one pass.

  \sa toy::validateUtf8(const char *)
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

  \sa toy::utf16Len(), toy::validateUtf8()
*/
[[nodiscard]] bool validateUtf16(const char16_t * str) noexcept;

/*!
  \brief Reports whether the \a size units starting at \a str are well-formed UTF-16.

  Runs the checks of toy::validateUtf16(const char16_t *) over a range that \a size bounds instead of a terminator. No
  unit past \a size is read, and a \c u'\\0' inside the range counts as a one-unit character.

  \param str  First unit of the range.
  \param size Number of units in the range.

  \return \c true when every surrogate is half of a pair inside the range, \c false at the first that is not. An empty
          range is well-formed, a null \a str included; a null \a str with a non-zero \a size gives \c false.

  \pre The \a size units starting at \a str are readable.

  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in \a size, one pass.

  \sa toy::validateUtf16(const char16_t *)
*/
[[nodiscard]] bool validateUtf16(const char16_t * str, size_t size) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_UTILS_HPP_
