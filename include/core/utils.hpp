//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \brief  Core utilities: string encoding conversion, length, reversal, and number formatting.

  UTF-8 ↔ wide conversion, utf8Len, reverseString, itoa, utoa, ftoa, formatNumberString. Implementations may be in
  \c core/utils.cpp or inline. All functions write into caller-provided buffers; no dynamic allocation.
*/

#ifndef INCLUDE_CORE_UTILS_HPP_
#define INCLUDE_CORE_UTILS_HPP_

/*!
  \defgroup TextFunctions Common text processing functions
  \brief String encoding conversion, length and reversal.

  - **utf8toWChar**: UTF-8 C string or \ref toy::StringLike to wide-character string (with optional character limit).
  - **wcharToUtf8**: Wide-character C string to UTF-8.
  - **utf8Len**: Unicode code point count in a UTF-8 C string.
  - **reverseString**: In-place reversal of a C string.

  Related: \ref toy::StringLike, \ref toy::FixedString; constant \c WCHAR_IN_UTF8_MAX_SIZE.

  \section features Key Features

  - **UTF-8 / wide conversion**: BMP-only; invalid UTF-8 skipped; destination null-terminated on success.
  - **No allocation**: All functions write into caller-provided buffers.
  - **Constexpr**: utf8toWChar overloads and reverseString where applicable.
  - **Exception safety**: All operations are \c noexcept.
*/

namespace toy {

/*!
  \brief Maximum UTF-8 byte length for one BMP character.
  \ingroup TextFunctions

  One wide character in the BMP (≤ 0xFFFF) encodes to at most 3 UTF-8 bytes.
*/
constexpr size_t WCHAR_IN_UTF8_MAX_SIZE = 3;

/*!
  \brief Converts a UTF-8 C string to a wide-character string with a character count limit.
  \ingroup TextFunctions

  Writes the converted wide-character string into \a dest. Stops when \a count source bytes have been processed, \a dest
  is full, or the source ends. Only BMP (≤ 0xFFFF) is supported.

  \param dest     Destination buffer for the wide-character string.
  \param destSize Size of \a dest in wide characters (not bytes).
  \param src      Source UTF-8 encoded C string.
  \param count    Maximum number of source bytes to process.

  \return Pointer to \a dest, or \c nullptr if \a dest or \a destSize is invalid.

  \pre \a dest points to a valid buffer with capacity \a destSize; \a src is valid UTF-8.
  \pre \a count is at most the number of code points in \a src (if bounded).

  \post On success, \a dest is null-terminated. On overflow or invalid input, returns \c nullptr.
*/
wchar_t * utf8toWChar(wchar_t * dest, size_t destSize, const char * src, size_t count) noexcept;

/*!
  \brief Converts a UTF-8 C string to a wide-character string (full source length).
  \ingroup TextFunctions

  Same as utf8toWChar(dest, destSize, src, count) with \a count set to the length of \a src. Stops when the source ends
  or \a dest is full. BMP-only; invalid UTF-8 sequences are skipped.

  \param dest     Destination buffer for the wide-character string.
  \param destSize Size of \a dest in wide characters (not bytes).
  \param src      Source UTF-8 encoded C string.

  \return Pointer to \a dest, or \c nullptr if \a dest or \a destSize is invalid.

  \pre \a dest points to a valid buffer with capacity \a destSize; \a src is valid UTF-8.

  \post On success, \a dest is null-terminated. On overflow or invalid input, returns \c nullptr.

  \sa utf8toWChar(dest, destSize, src, count)
*/
constexpr wchar_t * utf8toWChar(wchar_t * dest, size_t destSize, const char * src) noexcept;

/*!
  \brief Converts a UTF-8 \ref toy::StringLike object to a wide-character string.
  \ingroup TextFunctions

  Converts the UTF-8 content of \a src (via c_str()) into \a dest. Stops when the source ends or \a dest is full.
  BMP-only; invalid UTF-8 sequences are skipped.

  \tparam T Type satisfying \ref toy::StringLike (e.g. \ref toy::FixedString, \c std::string).

  \param dest     Destination buffer for the wide-character string.
  \param destSize Size of \a dest in wide characters (not bytes).
  \param src      Source object with UTF-8 encoded content.

  \return Pointer to \a dest, or \c nullptr if \a dest or \a destSize is invalid.

  \pre \a dest points to a valid buffer with capacity \a destSize; \a src.c_str() returns valid UTF-8.

  \post On success, \a dest is null-terminated. On overflow or invalid input, returns \c nullptr.

  \sa utf8toWChar (C string overloads)
*/
template <StringLike T>
constexpr wchar_t * utf8toWChar(wchar_t * dest, size_t destSize, const T & src) noexcept;

/*!
  \brief Converts a wide-character C string to UTF-8.
  \ingroup TextFunctions

  Writes the UTF-8 encoding of \a src into \a dest. Stops when the source ends or \a dest is full. Each wide character
  may produce 1–3 UTF-8 bytes.

  \param dest     Destination buffer for the UTF-8 string.
  \param destSize Size of \a dest in bytes (not wide characters).
  \param src      Source wide-character C string.

  \return Pointer to \a dest, or \c nullptr if \a dest or \a destSize is invalid.

  \pre \a dest points to a valid buffer with capacity \a destSize; \a src is a valid wide-character string.
  \pre \a destSize accounts for possible UTF-8 expansion (e.g. 3× wide length for BMP).

  \post On success, \a dest is null-terminated. On overflow or invalid input, returns \c nullptr.

  \sa utf8toWChar
*/
char * wcharToUtf8(char * dest, size_t destSize, const wchar_t * src) noexcept;

/*!
  \brief Returns the number of Unicode code points in a UTF-8 encoded C string.
  \ingroup TextFunctions

  Parses UTF-8 sequences in \a string and counts code points. Stops at the first null byte. Invalid sequences cause the
  function to return \c 0.

  \param string Source UTF-8 encoded C string (may be null).

  \return Number of Unicode code points, or \c 0 if \a string is \c nullptr or contains invalid UTF-8.

  \note Multi-byte sequences (2–3 bytes) count as one code point. BMP only.
*/
[[nodiscard]] size_t utf8Len(const char * string) noexcept;

/*!
  \brief Reverses a C string in-place.
  \ingroup TextFunctions

  Swaps characters from both ends toward the center. If \a count is \c 0, the length is obtained via strlen(\a str).

  \param str   C string to reverse (must be writable).
  \param count Number of characters to reverse (default: \c 0 for null-terminated length).

  \return Pointer to \a str (the reversed string).

  \pre If \a count > 0, \a str has at least \a count valid characters. If \a count is \c 0, \a str is null-terminated.

  \post Characters in the reversed range are swapped in place; \a str is still null-terminated when \a count was \c 0.
*/
constexpr char * reverseString(char * str, size_t count = 0) noexcept;

/*!
  \brief Converts a signed integer to its decimal C string representation.

  Writes the decimal representation of \a value into \a dest, null-terminated. Negative values are prefixed with a minus
  sign.

  \tparam T Signed integer type; must satisfy \c std::signed_integral (e.g. \c int8_t, \c int32_t, \c int64_t).

  \param dest     Destination buffer for the result.
  \param destSize Size of \a dest in characters.
  \param value    Value to convert.

  \return Pointer to \a dest.

  \pre \a dest is not null and \a destSize is sufficient (e.g. at least 5 for \c int8_t, 22 for \c int64_t worst case).
*/
template <std::signed_integral T>
constexpr char * itoa(char * dest, size_t destSize, T value) noexcept;

/*!
  \brief Converts an unsigned integer to its C string representation in the given base.

  Writes the representation of \a value in base \a base into \a dest, null-terminated. Digits above 9 use uppercase
  letters A–Z.

  \tparam T Unsigned integer type; must satisfy \c std::unsigned_integral (e.g. \c uint8_t, \c uint32_t, \c uint64_t).

  \param dest     Destination buffer for the result.
  \param destSize Size of \a dest in characters.
  \param value    Value to convert.
  \param base     Numerical base (default: \c 10). Supported range \c 2–\c 36.

  \return Pointer to \a dest.

  \pre \a dest is not null; \a destSize is sufficient for the output; \a base is between \c 2 and \c 36.
*/
template <std::unsigned_integral T>
constexpr char * utoa(char * dest, size_t destSize, T value, unsigned base = 10) noexcept;

/*!
  \brief Converts a 32-bit floating-point number to its C string representation with specified precision.

  This function converts a given 32-bit floating-point number into its decimal C string representation, storing the
  result in the provided destination buffer. The conversion supports configurable precision and handles special values.

  \param dest      A pointer to the destination buffer where the converted C string is stored.
  \param destSize  The size of the destination buffer in characters.
  \param value     The 32-bit floating-point number to be converted.
  \param precision The precision (digits after the decimal point). Default is 7, practical limit is ~7-9 digits.

  \return A pointer to the destination buffer containing the converted C string.

  \note The function handles special IEEE-754 values (infinity, NaN).
  \note The function does not support subnormal numbers.
  \note Precision beyond ~7-9 digits may not be meaningful for float.
  \note The function uses efficient bit manipulation for conversion.
*/
char * ftoa(char * dest, size_t destSize, float value, size_t precision = 7) noexcept;

/*!
  \brief Converts a 64-bit floating-point number to its C string representation with specified precision.

  This function converts a given 64-bit floating-point number into its decimal C string representation, storing the
  result in the provided destination buffer. The conversion supports configurable precision and handles special values.

  \param dest      A pointer to the destination buffer where the converted C string is stored.
  \param destSize  The size of the destination buffer in characters.
  \param value     The 64-bit floating-point number to be converted.
  \param precision The precision (digits after the decimal point). Default is 15, practical limit is ~15–17 digits.

  \return A pointer to the destination buffer containing the converted C string.

  \note The function handles special IEEE-754 values (infinity, NaN).
  \note The function does not support subnormal numbers.
  \note Precision beyond ~15–17 digits may not be meaningful for double.
  \note The function uses efficient bit manipulation for conversion.
*/
char * ftoa(char * dest, size_t destSize, double value, size_t precision = 15) noexcept;

/*!
  \brief Formats a number C string by inserting grouping separators.

  This function inserts a grouping \a separator (e.g., comma, space, or dot) into a number C string every three digits,
  starting from the right. This is commonly used for formatting large numbers to improve readability (e.g.,
  "1,234,567").

  \param buffer     A pointer to the buffer where the formatted C string is stored.
  \param bufferSize The size of the buffer in characters.
  \param separator  A pointer to the grouping separator C string to insert (e.g., ",", " ", ".").

  \note The function modifies the \a buffer in-place.
  \note Grouping separators are inserted every three digits from the right.
  \note The function handles edge cases gracefully (empty string, single digits, etc.).
  \note The function does not validate that the input is purely numeric.
*/
void formatNumberString(char * buffer, size_t bufferSize, const char * separator) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_UTILS_HPP_
