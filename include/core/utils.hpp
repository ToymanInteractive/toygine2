//
// Copyright (c) 2025 by Toyman Interactive
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
  \brief  Core utility functions for string manipulation, encoding conversion, and number formatting.
*/

#ifndef INCLUDE_CORE_UTILS_HPP_
#define INCLUDE_CORE_UTILS_HPP_

namespace toy {

/// Maximum UTF-8 bytes required for BMP characters.
constexpr std::size_t wcharInUtf8MaxSize = 3;

/*!
  \brief Converts a Unicode UTF-8 encoded C string to a wide character string with character count limit.

  This function translates a UTF-8 encoded C string into a wide character string stored in the destination buffer. The
  conversion stops when the specified number of characters have been converted, the destination buffer is filled, or the
  source string ends.

  \param dest     A pointer to the destination buffer where the converted wide character string will be stored.
  \param destSize The size of the destination buffer in wide characters (not bytes).
  \param src      A pointer to the source UTF-8 encoded C string.
  \param count    The maximum number of characters to convert from the source string.

  \return A pointer to the destination wide character string, or nullptr if the destination buffer is invalid.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The source C string must be a valid UTF-8 encoded string.
  \pre The count parameter must be reasonable (typically ≤ source string length).

  \post The destination string is null-terminated.
  \post The function returns \c nullptr on buffer overflow or invalid input.

  \note Only BMP (≤ 0xFFFF) characters are supported by design; 4-byte UTF-8 sequences are not produced.
  \note The function handles UTF-8 validation and skips invalid sequences.
  \note If count exceeds the available characters, conversion stops at the end of the source string.
*/
wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, const char * const src, std::size_t count) noexcept;

/*!
  \brief Converts a Unicode UTF-8 encoded C string to a wide character string.

  This function translates a UTF-8 encoded C string into a wide character string. The conversion stops when the source
  string ends or the destination buffer is filled.

  \param dest     A pointer to the destination buffer where the converted wide character string will be stored.
  \param destSize The size of the destination buffer in wide characters (not bytes).
  \param src      A pointer to the source UTF-8 encoded C string.

  \return A pointer to the destination wide character string, or nullptr if the destination buffer is invalid.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The source C string must be a valid UTF-8 encoded string.

  \post The destination string is null-terminated.
  \post The function returns \c nullptr on buffer overflow or invalid input.

  \note Only BMP (≤ 0xFFFF) characters are supported by design; 4-byte UTF-8 sequences are not produced.
  \note This is an inline wrapper around the main utf8toWChar function.
  \note The function automatically determines the source string length.
*/
constexpr wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, const char * const src) noexcept;

/*!
  \brief Converts a Unicode UTF-8 encoded StringLike object to a wide character string.

  This template function translates a UTF-8 encoded StringLike object into a wide character string. The conversion stops
  when the source string ends or the destination buffer is filled.

  \tparam stringType The type of the source string. Must satisfy the StringLike concept.

  \param dest     A pointer to the destination buffer where the converted wide character string will be stored.
  \param destSize The size of the destination buffer in wide characters (not bytes).
  \param src      A reference to a StringLike object with UTF-8 encoded content.

  \return A pointer to the destination wide character string, or nullptr if the destination buffer is invalid.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The source StringLike object must provide UTF-8 encoded string data via c_str().

  \post The destination string is null-terminated.
  \post The function returns \c nullptr on buffer overflow or invalid input.

  \note Only BMP (≤ 0xFFFF) characters are supported by design; 4-byte UTF-8 sequences are not produced.
  \note This template works with std::string, FixedString, and other string-like types.
*/
template <StringLike stringType>
constexpr wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, const stringType & src) noexcept;

/*!
  \brief Converts a Unicode wide character C string to a UTF-8 encoded string.

  This function translates a wide character C string into a UTF-8 encoded string stored in the destination buffer. The
  conversion stops when the source string ends or the destination buffer is filled.

  \param dest     A pointer to the destination buffer where the converted UTF-8 encoded string will be stored.
  \param destSize The size of the destination buffer in bytes (not wide characters).
  \param src      A pointer to the source wide character C string.

  \return A pointer to the destination UTF-8 encoded string, or nullptr if the destination buffer is invalid.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The source C string must be a valid wide character string.
  \pre The destination buffer size should account for potential UTF-8 expansion.

  \post The destination string is null-terminated.
  \post The function returns \c nullptr on buffer overflow or invalid input.

  \note The function handles wide character to UTF-8 conversion efficiently.
  \note UTF-8 sequences may require 1-3 bytes per wide character.
*/
char * wcharToUtf8(char * dest, std::size_t destSize, const wchar_t * src) noexcept;

/*!
  \brief Returns the number of Unicode characters in a UTF-8 encoded C \a string.

  This function counts the number of Unicode characters in a UTF-8 encoded C \a string by parsing UTF-8 sequences. It
  stops counting when the null character is encountered.

  \param string A pointer to the source UTF-8 encoded C string.

  \return The number of Unicode characters in the C \a string, or 0 if the \a string is invalid or null.

  \note The function validates UTF-8 sequences during counting.
  \note Multi-byte sequences (2-3 bytes) are counted as single Unicode characters.
  \note Invalid UTF-8 sequences cause the function to return 0.
*/
std::size_t utf8Len(const char * string) noexcept;

/*!
  \brief Reverses a given C string in-place.

  This function reverses a given C string in-place by swapping characters from both ends towards the center. It can be
  used to reverse a string of a specified length, or to reverse a null-terminated string when count is \c 0.

  \param str   A pointer to the C string to reverse.
  \param count The length of the C string to reverse (default: \c 0).

  \return A pointer to the reversed C string (same as input pointer).

  \note The function modifies the original string directly.
  \note When count = \c 0, the function calls strlen() to determine the string length.
*/
constexpr char * reverseString(char * str, std::size_t count = 0) noexcept;

/*!
  \brief Converts an 8-bit signed integer value to a C string representation.

  This function converts a given 8-bit signed integer value into its decimal C string representation, storing the result
  in the provided destination buffer.

  \param dest     A pointer to the destination buffer where the converted C string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 8-bit signed integer value to be converted.

  \return A pointer to the destination buffer containing the converted C string.

  \note The function handles negative values correctly.
  \note The destination buffer must have sufficient capacity (at least 5 characters for worst case -128).
*/
char * itoa(char * dest, std::size_t destSize, std::int8_t value) noexcept;

/*!
  \brief Converts a 16-bit signed integer value to a C string representation.

  This function converts a given 16-bit signed integer value into its decimal C string representation, storing the
  result in the provided destination buffer.

  \param dest     A pointer to the destination buffer where the converted C string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 16-bit signed integer value to be converted.

  \return A pointer to the destination buffer containing the converted C string.

  \note The function handles negative values correctly.
  \note The destination buffer must have sufficient capacity (at least 7 characters for worst case -32768).
*/
char * itoa(char * dest, std::size_t destSize, std::int16_t value) noexcept;

/*!
  \brief Converts a 32-bit signed integer value to a C string representation.

  This function converts a given 32-bit signed integer value into its decimal C string representation, storing the
  result in the provided destination buffer.

  \param dest     A pointer to the destination buffer where the converted C string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 32-bit signed integer value to be converted.

  \return A pointer to the destination buffer containing the converted C string.

  \note The function handles negative values correctly.
  \note The destination buffer must have sufficient capacity (at least 12 characters for worst case -2147483648).
*/
char * itoa(char * dest, std::size_t destSize, std::int32_t value) noexcept;

/*!
  \brief Converts a 64-bit signed integer value to a C string representation.

  This function converts a given 64-bit signed integer value into its decimal C string representation, storing the
  result in the provided destination buffer.

  \param dest     A pointer to the destination buffer where the converted C string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 64-bit signed integer value to be converted.

  \return A pointer to the destination buffer containing the converted C string.

  \note The function handles negative values correctly.
  \note The destination buffer must have sufficient capacity (at least 21 characters for worst case
  -9223372036854775808).
*/
char * itoa(char * dest, std::size_t destSize, std::int64_t value) noexcept;

/*!
  \brief Converts an 8-bit unsigned integer to a C string representation in the specified base.

  This function converts a given 8-bit unsigned integer value into its C string representation in the specified
  numerical base, storing the result in the provided destination buffer. Supports bases from 2 to 36.

  \param dest     A pointer to the destination buffer where the converted C string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 8-bit unsigned integer value to be converted.
  \param base     The numerical base for the conversion (2-36). Common values: 2 (binary), 10 (decimal), 16 (hex).

  \return A pointer to the destination buffer containing the converted C string.

  \note The function supports bases 2-36 with digits 0-9 and letters A-Z.
  \note Hexadecimal values use uppercase letters (A-F).
*/
char * itoa(char * dest, std::size_t destSize, std::uint8_t value, unsigned base) noexcept;

/*!
  \brief Converts a 16-bit unsigned integer to a C string representation in the specified base.

  This function converts a given 16-bit unsigned integer value into its C string representation in the specified
  numerical base, storing the result in the provided destination buffer. Supports bases from 2 to 36.

  \param dest     A pointer to the destination buffer where the converted C string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 16-bit unsigned integer value to be converted.
  \param base     The numerical base for the conversion (2-36). Common values: 2 (binary), 10 (decimal), 16 (hex).

  \return A pointer to the destination buffer containing the converted C string.

  \note The function supports bases 2-36 with digits 0-9 and letters A-Z.
  \note Hexadecimal values use uppercase letters (A-F).
*/
char * itoa(char * dest, std::size_t destSize, std::uint16_t value, unsigned base) noexcept;

/*!
  \brief Converts a 32-bit unsigned integer to a C string representation in the specified base.

  This function converts a given 32-bit unsigned integer value into its C string representation in the specified
  numerical base, storing the result in the provided destination buffer. Supports bases from 2 to 36.

  \param dest     A pointer to the destination buffer where the converted C string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 32-bit unsigned integer value to be converted.
  \param base     The numerical base for the conversion (2-36). Common values: 2 (binary), 10 (decimal), 16 (hex).

  \return A pointer to the destination buffer containing the converted C string.

  \note The function supports bases 2-36 with digits 0-9 and letters A-Z.
  \note Hexadecimal values use uppercase letters (A-F).
*/
char * itoa(char * dest, std::size_t destSize, std::uint32_t value, unsigned base) noexcept;

/*!
  \brief Converts a 64-bit unsigned integer to a C string representation in the specified base.

  This function converts a given 64-bit unsigned integer value into its C string representation in the specified
  numerical base, storing the result in the provided destination buffer. Supports bases from 2 to 36.

  \param dest     A pointer to the destination buffer where the converted C string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 64-bit unsigned integer value to be converted.
  \param base     The numerical base for the conversion (2-36). Common values: 2 (binary), 10 (decimal), 16 (hex).

  \return A pointer to the destination buffer containing the converted C string.

  \note The function supports bases 2-36 with digits 0-9 and letters A-Z.
  \note Hexadecimal values use uppercase letters (A-F).
*/
char * itoa(char * dest, std::size_t destSize, std::uint64_t value, unsigned base) noexcept;

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
char * ftoa(char * dest, std::size_t destSize, float value, std::size_t precision = 7) noexcept;

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
char * ftoa(char * dest, std::size_t destSize, double value, std::size_t precision = 15) noexcept;

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
void formatNumberString(char * buffer, std::size_t bufferSize, const char * separator) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_UTILS_HPP_
