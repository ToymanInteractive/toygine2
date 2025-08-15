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
/*
  \file   utils.hpp
  \brief  toygine core part united header
*/

#ifndef INCLUDE_CORE_UTILS_HPP_
#define INCLUDE_CORE_UTILS_HPP_

namespace toygine {

/// Maximum UTF-8 bytes for BMP characters (only BMP supported by design)
inline constexpr std::size_t wcharInUtf8MaxSize = 3;

/*!
  \brief Converts a Unicode UTF-8 encoded string to a wide character string.

  This function translates a UTF-8 encoded source string into a wide character string stored in the destination buffer.
  The conversion stops when the specified number of characters have been converted or the destination buffer is filled.
  The destination string is null-terminated.

  \param dest     A pointer to the destination buffer where the converted wide character string will be stored.
  \param destSize The size of the destination buffer.
  \param src      A pointer to the source UTF-8 encoded string.
  \param count    The number of characters to convert from the source string.

  \return A pointer to the destination wide character string, or nullptr if the destination buffer is invalid.
*/
wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, char const * src, std::size_t count);

/*!
  \brief Converts a Unicode UTF-8 encoded string to a wide character string.

  This function translates a UTF-8 encoded source string into a wide character string stored in the destination buffer.
  The conversion stops when the specified number of characters have been converted or the destination buffer is filled.
  The destination string is null-terminated.

  \param dest     A pointer to the destination buffer where the converted wide character string will be stored.
  \param destSize The size of the destination buffer.
  \param src      A pointer to the source UTF-8 encoded string.

  \return A pointer to the destination wide character string, or nullptr if the destination buffer is invalid.
*/
inline wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, char const * src);

/*!
  \brief Converts a Unicode UTF-8 encoded string to a wide character string.

  This function translates a UTF-8 encoded source string into a wide character string stored in the destination buffer.
  The conversion stops when the specified number of characters have been converted or the destination buffer is filled.
  The destination string is null-terminated.

  \param dest     A pointer to the destination buffer where the converted wide character string will be stored.
  \param destSize The size of the destination buffer.
  \param src      A reference to a class with null-terminated UTF-8 encoded string.

  \return A pointer to the destination wide character string, or nullptr if the destination buffer is invalid.
*/
template <typename stringType>
inline wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, stringType const & src);

/*!
  \brief Converts a Unicode wide character string to a UTF-8 encoded string.

  This function translates a source wide character string into a UTF-8 encoded string stored in the destination buffer.
  The conversion stops when the source string is fully converted, or the destination buffer is filled. The destination
  string is null-terminated.

  \param dest     A pointer to the destination buffer where the converted UTF-8 encoded string will be stored.
  \param destSize The size of the destination buffer.
  \param src      A pointer to the source wide character string.

  \return A pointer to the destination UTF-8 encoded string, or nullptr if the destination buffer is invalid.
*/
char * wcharToUtf8(char * dest, std::size_t destSize, wchar_t const * src);

/*!
  \brief Returns the number of Unicode characters in a UTF-8 encoded string.

  This function counts the number of Unicode characters in a UTF-8 encoded string. It stops counting when the null
  character is encountered or the end of the string is reached.

  \param str A pointer to the UTF-8 encoded string.

  \return The number of Unicode characters in the string, or 0 if the string is invalid.
*/
std::size_t utf8len(const char * str);

/*!
  \brief Reverses a given string in-place.

  This function will reverse a given string in-place. It can be used to reverse a string of a given length, or to
  reverse a null-terminated string.

  \param str   The string to reverse.
  \param count The length of the string to reverse.

  \return A pointer to the reversed string.
*/
char * reverseString(char * str, std::size_t count = 0);

/*!
  \brief Converts an integer value to a string representation.

  This function converts a given integer value into its string representation, storing the result in the provided
  destination buffer.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
        destination buffer size is 1, only a null terminator is written. The function reverses the string in-place.
*/
char * itoa(char * dest, std::size_t destSize, std::int8_t value);

/*!
  \brief Converts an integer value to a string representation.

  This function converts a given integer value into its string representation, storing the result in the provided
  destination buffer.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
        destination buffer size is 1, only a null terminator is written. The function reverses the string in-place.
*/
char * itoa(char * dest, std::size_t destSize, std::int16_t value);

/*!
  \brief Converts an integer value to a string representation.

  This function converts a given integer value into its string representation, storing the result in the provided
  destination buffer.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
        destination buffer size is 1, only a null terminator is written. The function reverses the string in-place.
*/
char * itoa(char * dest, std::size_t destSize, std::int32_t value);

/*!
  \brief Converts an integer value to a string representation.

  This function converts a given integer value into its string representation, storing the result in the provided
  destination buffer.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
        destination buffer size is 1, only a null terminator is written. The function reverses the string in-place.
*/
char * itoa(char * dest, std::size_t destSize, std::int64_t value);

/*!
  \brief Converts an unsigned integer to a string representation in the specified base.

  This function converts a given unsigned integer value into its string representation in the specified base,
  storing the result in the provided destination buffer.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The unsigned integer value to be converted.
  \param base     The numerical base for the conversion, e.g., 10 for decimal, 16 for hexadecimal.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
        destination buffer size is 1, only a null terminator is written. The base must be between 2 and 36 inclusive.
*/
char * itoa(char * dest, std::size_t destSize, std::uint8_t value, unsigned base);

/*!
  \brief Converts an unsigned integer to a string representation in the specified base.

  This function converts a given unsigned integer value into its string representation in the specified base,
  storing the result in the provided destination buffer.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The unsigned integer value to be converted.
  \param base     The numerical base for the conversion, e.g., 10 for decimal, 16 for hexadecimal.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
        destination buffer size is 1, only a null terminator is written. The base must be between 2 and 36 inclusive.
*/
char * itoa(char * dest, std::size_t destSize, std::uint16_t value, unsigned base);

/*!
  \brief Converts an unsigned integer to a string representation in the specified base.

  This function converts a given unsigned integer value into its string representation in the specified base,
  storing the result in the provided destination buffer.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The unsigned integer value to be converted.
  \param base     The numerical base for the conversion, e.g., 10 for decimal, 16 for hexadecimal.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
        destination buffer size is 1, only a null terminator is written. The base must be between 2 and 36 inclusive.
*/
char * itoa(char * dest, std::size_t destSize, std::uint32_t value, unsigned base);

/*!
  \brief Converts an unsigned integer to a string representation in the specified base.

  This function converts a given unsigned integer value into its string representation in the specified base,
  storing the result in the provided destination buffer.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The unsigned integer value to be converted.
  \param base     The numerical base for the conversion, e.g., 10 for decimal, 16 for hexadecimal.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
        destination buffer size is 1, only a null terminator is written. The base must be between 2 and 36 inclusive.
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

  \note The function does not check if the destination buffer is large enough to hold the modified string. If the buffer
        is too small, the function will not insert any separators and will return without modifying the buffer.
*/
void formatNumberString(char * buffer, std::size_t bufferSize, char const * groupingSeparator);

} // namespace toygine

#endif // INCLUDE_CORE_UTILS_HPP_
