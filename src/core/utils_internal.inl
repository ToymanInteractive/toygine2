//
// Copyright (c) 2025 Toyman Interactive
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
  \file   utils_internal.inl
  \brief  Internal utilities: inline helpers used by the core utils implementation.
*/

#ifndef SRC_CORE_UTILS_INTERNAL_INL_
#define SRC_CORE_UTILS_INTERNAL_INL_

namespace toy {

/*!
  \brief ANSI digit lookup table for base conversion.

  This lookup table contains the characters used for representing digits in different numerical bases. It supports bases
  from 2 to 36, using digits '0'-'9' and 'A'-'Z'.

  \note The table contains 36 characters: '0'-'9' and 'A'-'Z'.
*/
static constexpr std::array<char, 36> _ansiDigits{{
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
}};

/*!
  \brief Converts an integer value to a string representation in a specified base.

  This function converts a given integer value into its string representation in the specified base, storing the result
  in the provided destination buffer.

  \tparam type The type of the integer value to be converted.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The integer value to be converted.
  \param base     The numerical base for the conversion, e.g., 10 for decimal, 16 for hexadecimal.

  \return The number of characters written to the destination buffer.

  \note The function assumes that the destination buffer is large enough to hold the converted string.
  \note The function does not null-terminate the string.
  \note The base must be between 2 and 36 inclusive.
*/
template <typename type>
constexpr size_t integerToSymbols(char * dest, size_t destSize, type value, unsigned base) {
  static_assert(std::is_integral_v<type>, "integerToSymbols requires an integral type");
  assert_message(dest != nullptr && destSize > 0, "The destination buffer must not be null.");
  assert_message(base >= 2 && base <= _ansiDigits.size(), "The base must be between 2 and 36 inclusive.");

  size_t index = 0;

  do {
    dest[index++] = _ansiDigits[value % base];
  } while ((value /= static_cast<type>(base)) > 0 && index < destSize);

  return index;
}

/*!
  \brief Converts an integer value to a string representation.

  This function converts a given integer value into its string representation, storing the result in the provided
  destination buffer.

  \tparam type The type of the integer value to be converted.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string.
  \note If the destination buffer size is 1, only a null terminator is written.
  \note The function reverses the string in-place.
*/
template <typename type>
inline char * itoaImplementation(char * dest, size_t destSize, type value) {
  static_assert(std::is_integral_v<type> && std::is_signed_v<type>,
                "itoaImplementation requires a signed integral type");
  assert_message(dest != nullptr && destSize > 0, "The destination buffer must not be null.");
  if (destSize == 1) {
    *dest = '\0';

    return dest;
  }

  // decrease dest size for '\0' symbol
  --destSize;

  size_t symbols = 0;
  const bool valueNegative = (value < 0);
  using unsigned_type = std::make_unsigned_t<type>;
  if (valueNegative) {
    symbols = integerToSymbols(dest, destSize, static_cast<unsigned_type>(-(value + 1)) + 1U, 10);
  } else {
    symbols = integerToSymbols(dest, destSize, static_cast<unsigned_type>(value), 10);
  }

  if (valueNegative && symbols < destSize)
    dest[symbols++] = '-';

  dest[symbols] = '\0';
  reverseString(dest, symbols);

  return dest;
}

/*!
  \brief Converts an unsigned integer value to a string representation in a specified base.

  This function converts a given unsigned integer value into its string representation in the specified numerical base,
  storing the result in the provided destination buffer. The resulting string is reversed in place and null-terminated.

  \tparam type The type of the unsigned integer value to be converted.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The unsigned integer value to be converted.
  \param base     The numerical base for the conversion, e.g., 10 for decimal, 16 for hexadecimal.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string.
  \note If the destination buffer size is 1, only a null terminator is written.
  \note The base must be between 2 and 36 inclusive.
*/
template <typename type>
inline char * utoaImplementation(char * dest, size_t destSize, type value, unsigned base) {
  static_assert(std::is_unsigned_v<type>, "utoaImplementation requires an unsigned integer type");
  assert_message(dest != nullptr && destSize > 0, "The destination buffer must not be null.");
  if (destSize == 1) {
    *dest = '\0';

    return dest;
  }

  // decrease dest size for '\0' symbol
  --destSize;

  auto symbols = integerToSymbols(dest, destSize, value, base);

  dest[symbols] = '\0';
  reverseString(dest, symbols);

  return dest;
}

} // namespace toy

#endif // SRC_CORE_UTILS_INTERNAL_INL_
