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
  \brief  Core utility functions for string manipulation, encoding conversion, and number formatting
*/

#ifndef INCLUDE_CORE_UTILS_HPP_
#define INCLUDE_CORE_UTILS_HPP_

namespace toygine {

/// Maximum UTF-8 bytes required for BMP characters.
inline constexpr std::size_t wcharInUtf8MaxSize = 3;

/*!
  \brief Converts a Unicode UTF-8 encoded string to a wide character string with character count limit.

  This function translates a UTF-8 encoded source string into a wide character string stored in the destination buffer.
  The conversion stops when the specified number of characters have been converted, the destination buffer is filled, or
  the source string ends.

  \param dest     A pointer to the destination buffer where the converted wide character string will be stored.
  \param destSize The size of the destination buffer in wide characters (not bytes).
  \param src      A pointer to the source UTF-8 encoded string.
  \param count    The maximum number of characters to convert from the source string.

  \return A pointer to the destination wide character string, or nullptr if the destination buffer is invalid.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The source string must be a valid UTF-8 encoded string.
  \pre The count parameter must be reasonable (typically ≤ source string length).

  \post The destination string is null-terminated.
  \post The function returns nullptr on buffer overflow or invalid input.

  \note Only BMP (≤ 0xFFFF) characters are supported by design; 4-byte UTF-8 sequences are not produced.
  \note The function handles UTF-8 validation and skips invalid sequences.
  \note If count exceeds the available characters, conversion stops at the end of the source string.
  \note The function is thread-safe for read operations.
  \note Performance is optimized for common UTF-8 sequences.
*/
wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, const char * const src, std::size_t count);

/*!
  \brief Converts a Unicode UTF-8 encoded string to a wide character string.

  This function translates a null-terminated UTF-8 encoded source string into a wide character string. The conversion
  stops when the source string ends or the destination buffer is filled.

  \param dest     A pointer to the destination buffer where the converted wide character string will be stored.
  \param destSize The size of the destination buffer in wide characters (not bytes).
  \param src      A pointer to the null-terminated source UTF-8 encoded string.

  \return A pointer to the destination wide character string, or nullptr if the destination buffer is invalid.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The source string must be a valid null-terminated UTF-8 encoded string.

  \post The destination string is null-terminated.
  \post The function returns nullptr on buffer overflow or invalid input.

  \note Only BMP (≤ 0xFFFF) characters are supported by design; 4-byte UTF-8 sequences are not produced.
  \note This is an inline wrapper around the main utf8toWChar function.
  \note The function automatically determines the source string length.
  \note Performance is optimized for common UTF-8 sequences.
*/
inline wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, const char * const src);

/*!
  \brief Converts a Unicode UTF-8 encoded string to a wide character string.

  This template function translates a UTF-8 encoded source string from any string-like type into a wide character
  string. The conversion stops when the source string ends or the destination buffer is filled.

  \tparam stringType The type of the source string. Must have a c_str() and size() methods.

  \param dest     A pointer to the destination buffer where the converted wide character string will be stored.
  \param destSize The size of the destination buffer in wide characters (not bytes).
  \param src      A reference to a string-like object with UTF-8 encoded content.

  \return A pointer to the destination wide character string, or nullptr if the destination buffer is invalid.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The source object must provide UTF-8 encoded string data via c_str().

  \post The destination string is null-terminated.
  \post The function returns nullptr on buffer overflow or invalid input.

  \note Only BMP (≤ 0xFFFF) characters are supported by design; 4-byte UTF-8 sequences are not produced.
  \note This template works with std::string, FixString, and other string-like types.
  \note The function automatically determines the source string length.
  \note Performance is optimized for common UTF-8 sequences.
*/
template <typename stringType>
inline wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, const stringType & src);

/*!
  \brief Converts a Unicode wide character string to a UTF-8 encoded string.

  This function translates a source wide character string into a UTF-8 encoded string stored in the destination buffer.
  The conversion stops when the source string ends or the destination buffer is filled.

  \param dest     A pointer to the destination buffer where the converted UTF-8 encoded string will be stored.
  \param destSize The size of the destination buffer in bytes (not wide characters).
  \param src      A pointer to the source wide character string.

  \return A pointer to the destination UTF-8 encoded string, or nullptr if the destination buffer is invalid.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The source string must be a valid wide character string.
  \pre The destination buffer size should account for potential UTF-8 expansion.

  \post The destination string is null-terminated.
  \post The function returns nullptr on buffer overflow or invalid input.

  \note The function handles wide character to UTF-8 conversion efficiently.
  \note UTF-8 sequences may require 1-3 bytes per wide character.
  \note The function is thread-safe for read operations.
  \note Performance is optimized for common wide character ranges.
*/
char * wcharToUtf8(char * dest, std::size_t destSize, wchar_t const * src);

/*!
  \brief Returns the number of Unicode characters in a UTF-8 encoded string.

  This function counts the number of Unicode characters in a UTF-8 encoded string by parsing UTF-8 sequences. It stops
  counting when the null character is encountered.

  \param str A pointer to the UTF-8 encoded string to count characters in.

  \return The number of Unicode characters in the string, or 0 if the string is invalid or null.

  \pre The input string must be a valid UTF-8 encoded string.
  \pre The string must be null-terminated.

  \post The function returns 0 for null pointers or invalid UTF-8 sequences.
  \post The function correctly counts multi-byte UTF-8 sequences as single characters.

  \note The function validates UTF-8 sequences during counting.
  \note Multi-byte sequences (2-3 bytes) are counted as single Unicode characters.
  \note The function is thread-safe for read operations.
  \note Performance is optimized for common UTF-8 patterns.
  \note Invalid UTF-8 sequences cause the function to return 0.
*/
std::size_t utf8len(const char * str);

/*!
  \brief Reverses a given string in-place.

  This function reverses a given string in-place by swapping characters from both ends towards the center. It can be
  used to reverse a string of a specified length, or to reverse a null-terminated string when count is 0.

  \param str   A pointer to the string to reverse.
  \param count The length of the string to reverse. If 0, the function determines the length automatically.

  \return A pointer to the reversed string (same as input pointer).

  \pre The input string pointer must be valid.
  \pre If count > 0, the string must have at least count characters.
  \pre If count = 0, the string must be null-terminated.

  \post The string is modified in-place with characters in reverse order.
  \post The function returns the same pointer that was passed in.

  \note The function modifies the original string directly.
  \note When count = 0, the function calls strlen() to determine the string length.
  \note The function is efficient with O(n/2) character swaps.
  \note The function is thread-safe for single-string operations.
  \note Performance is optimized for common string lengths.
*/
char * reverseString(char * str, std::size_t count = 0);

/*!
  \brief Converts an 8-bit signed integer value to a string representation.

  This function converts a given 8-bit signed integer value into its decimal string representation, storing the result
  in the provided destination buffer.

  \param dest     A pointer to the destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 8-bit signed integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \pre The destination buffer must be valid and have sufficient capacity (at least 5 characters for worst case -128).
  \pre The buffer should account for the sign, digits, and null terminator.

  \post The destination string is null-terminated.
  \post Negative values are properly prefixed with a minus sign.

  \note The function handles negative values correctly.
  \note The function is thread-safe for single-buffer operations.
  \note Performance is optimized for common integer ranges.
*/
char * itoa(char * dest, std::size_t destSize, std::int8_t value);

/*!
  \brief Converts a 16-bit signed integer value to a string representation.

  This function converts a given 16-bit signed integer value into its decimal string representation, storing the result
  in the provided destination buffer.

  \param dest     A pointer to the destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 16-bit signed integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \pre The destination buffer must be valid and have sufficient capacity (at least 7 characters for worst case -32768).
  \pre The buffer should account for the sign, digits, and null terminator.

  \post The destination string is null-terminated.
  \post Negative values are properly prefixed with a minus sign.

  \note The function handles negative values correctly.
  \note The function is thread-safe for single-buffer operations.
  \note Performance is optimized for common integer ranges.
*/
char * itoa(char * dest, std::size_t destSize, std::int16_t value);

/*!
  \brief Converts a 32-bit signed integer value to a string representation.

  This function converts a given 32-bit signed integer value into its decimal string representation, storing the result
  in the provided destination buffer.

  \param dest     A pointer to the destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 32-bit signed integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \pre The destination buffer must be valid and have sufficient capacity (at least 12 characters for worst case
       -2147483648).
  \pre The buffer should account for the sign, digits, and null terminator.

  \post The destination string is null-terminated.
  \post Negative values are properly prefixed with a minus sign.

  \note The function handles negative values correctly.
  \note The function is thread-safe for single-buffer operations.
  \note Performance is optimized for common integer ranges.
*/
char * itoa(char * dest, std::size_t destSize, std::int32_t value);

/*!
  \brief Converts a 64-bit signed integer value to a string representation.

  This function converts a given 64-bit signed integer value into its decimal string representation, storing the result
  in the provided destination buffer.

  \param dest     A pointer to the destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 64-bit signed integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \pre The destination buffer must be valid and have sufficient capacity (at least 21 characters for worst case
       -9223372036854775808).
  \pre The buffer should account for the sign, digits, and null terminator.

  \post The destination string is null-terminated.
  \post Negative values are properly prefixed with a minus sign.

  \note The function handles negative values correctly.
  \note The function is thread-safe for single-buffer operations.
  \note Performance is optimized for common integer ranges.
*/
char * itoa(char * dest, std::size_t destSize, std::int64_t value);

/*!
  \brief Converts an 8-bit unsigned integer to a string representation in the specified base.

  This function converts a given 8-bit unsigned integer value into its string representation in the specified numerical
  base, storing the result in the provided destination buffer. Supports bases from 2 to 36.

  \param dest     A pointer to the destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 8-bit unsigned integer value to be converted.
  \param base     The numerical base for the conversion (2-36). Common values: 2 (binary), 10 (decimal), 16 (hex).

  \return A pointer to the destination buffer containing the converted string.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The base must be between 2 and 36 inclusive.

  \post The destination string is null-terminated.
  \post Hexadecimal values use uppercase letters (A-F).

  \note The function supports bases 2-36 with digits 0-9 and letters A-Z.
  \note The function is thread-safe for single-buffer operations.
*/
char * itoa(char * dest, std::size_t destSize, std::uint8_t value, unsigned base);

/*!
  \brief Converts a 16-bit unsigned integer to a string representation in the specified base.

  This function converts a given 16-bit unsigned integer value into its string representation in the specified numerical
  base, storing the result in the provided destination buffer. Supports bases from 2 to 36.

  \param dest     A pointer to the destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 16-bit unsigned integer value to be converted.
  \param base     The numerical base for the conversion (2-36). Common values: 2 (binary), 10 (decimal), 16 (hex).

  \return A pointer to the destination buffer containing the converted string.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The base must be between 2 and 36 inclusive.

  \post The destination string is null-terminated.
  \post Hexadecimal values use uppercase letters (A-F).

  \note The function supports bases 2-36 with digits 0-9 and letters A-Z.
  \note The function is thread-safe for single-buffer operations.
*/
char * itoa(char * dest, std::size_t destSize, std::uint16_t value, unsigned base);

/*!
  \brief Converts a 32-bit unsigned integer to a string representation in the specified base.

  This function converts a given 32-bit unsigned integer value into its string representation in the specified numerical
  base, storing the result in the provided destination buffer. Supports bases from 2 to 36.

  \param dest     A pointer to the destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 32-bit unsigned integer value to be converted.
  \param base     The numerical base for the conversion (2-36). Common values: 2 (binary), 10 (decimal), 16 (hex).

  \return A pointer to the destination buffer containing the converted string.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The base must be between 2 and 36 inclusive.

  \post The destination string is null-terminated.
  \post Hexadecimal values use uppercase letters (A-F).

  \note The function supports bases 2-36 with digits 0-9 and letters A-Z.
  \note The function is thread-safe for single-buffer operations.
*/
char * itoa(char * dest, std::size_t destSize, std::uint32_t value, unsigned base);

/*!
  \brief Converts a 64-bit unsigned integer to a string representation in the specified base.

  This function converts a given 64-bit unsigned integer value into its string representation in the specified numerical
  base, storing the result in the provided destination buffer. Supports bases from 2 to 36.

  \param dest     A pointer to the destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer in characters.
  \param value    The 64-bit unsigned integer value to be converted.
  \param base     The numerical base for the conversion (2-36). Common values: 2 (binary), 10 (decimal), 16 (hex).

  \return A pointer to the destination buffer containing the converted string.

  \pre The destination buffer must be valid and have sufficient capacity.
  \pre The base must be between 2 and 36 inclusive.

  \post The destination string is null-terminated.
  \post Hexadecimal values use uppercase letters (A-F).

  \note The function supports bases 2-36 with digits 0-9 and letters A-Z.
  \note The function is thread-safe for single-buffer operations.
*/
char * itoa(char * dest, std::size_t destSize, std::uint64_t value, unsigned base);

/*!
  \brief Converts a floating-point number to its string representation in a specified precision.

  This function converts a given floating-point number into its string representation, storing the result in the
  provided destination buffer.

  \param dest      The destination buffer where the converted string is stored.
  \param destSize  The size of the destination buffer.
  \param value     The floating-point number to be converted.
  \param precision The precision (digits after the decimal point). For IEEE-754 f32, practical precision is ~7–9 digits.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
  destination buffer size is 1, only a null terminator is written. The function does not support subnormals.
*/
char * ftoa(char * dest, std::size_t destSize, float value, std::size_t precision = 7);

/*!
  \brief Converts a floating-point number to its string representation in a specified precision.

  This function converts a given floating-point number into its string representation, storing the result in the
  provided destination buffer.

  \param dest      The destination buffer where the converted string is stored.
  \param destSize  The size of the destination buffer.
  \param value     The floating-point number to be converted.
  \param precision The precision (digits after the decimal point). For IEEE-754 f64, practical precision is ~15–17
                   digits.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
  destination buffer size is 1, only a null terminator is written. The function does not support subnormals.
*/
char * ftoa(char * dest, std::size_t destSize, double value, std::size_t precision = 15);

/*!
  \brief Format number string.

  This function inserts a grouping separator (e.g., a comma or a space) into a number string. The separator is inserted
  every three digits, starting from the right. The function assumes that the destination buffer is large enough to hold
  the modified string.

  \param buffer            The destination buffer where the modified string is stored.
  \param bufferSize        The size of the destination buffer.
  \param groupingSeparator The grouping separator to be inserted into the string.

  \note The function validates available capacity and returns early without modification if the buffer is too small. The
        groupingSeparator must not be null and should not exceed 8 characters.
*/
void formatNumberString(char * buffer, std::size_t bufferSize, char const * groupingSeparator);

} // namespace toygine

#endif // INCLUDE_CORE_UTILS_HPP_
