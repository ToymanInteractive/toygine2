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
  \file   core_utils_internal.inl
  \brief  collection of core utils inline functions
*/

#ifndef SRC_CORE_CORE_UTILS_INTERNAL_INL_
#define SRC_CORE_CORE_UTILS_INTERNAL_INL_

#include <array>

#include "core.hpp"

namespace toygine {

static constexpr std::array<char, 36> sc_ansiDigits{{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
                                                     'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                                     'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}};

/*!
  \brief Converts an integer value to a string representation in a specified base.

  This function converts a given integer value into its string representation in the specified base, storing the result
  in the provided destination buffer.

  \tparam type    The type of the integer value to be converted.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The integer value to be converted.
  \param base     The numerical base for the conversion, e.g., 10 for decimal, 16 for hexadecimal.

  \return The number of characters written to the destination buffer.

  \note The function assumes that the destination buffer is large enough to hold the converted string. The function does
        not null-terminate the string. The base must be between 2 and 36 inclusive.
*/
template <typename type>
constexpr inline std::size_t integerToSymbols(char * dest, std::size_t destSize, type value, unsigned base) {
  assert_message(dest != nullptr && destSize > 0, "The destination buffer must not be null.");
  assert_message(base >= 2 && base <= sc_ansiDigits.size(), "The base must be between 2 and 36 inclusive.");

  std::size_t index = 0;

  do {
    dest[index++] = sc_ansiDigits[value % base];
  } while ((value /= static_cast<type>(base)) > 0 && index < destSize);

  return index;
}

/*!
  \brief Converts an integer value to a string representation.

  This function converts a given integer value into its string representation, storing the result in the provided
  destination buffer.

  \tparam type    The type of the integer value to be converted.

  \param dest     The destination buffer where the converted string is stored.
  \param destSize The size of the destination buffer.
  \param value    The integer value to be converted.

  \return A pointer to the destination buffer containing the converted string.

  \note The function assumes that the destination buffer is large enough to hold the converted string. If the
        destination buffer size is 1, only a null terminator is written. The function reverses the string in-place.
*/
template <typename type>
inline char * itoaImplementation(char * dest, size_t destSize, type value) {
  assert_message(dest != nullptr && destSize > 0, "The destination buffer must not be null.");
  if (destSize == 1) {
    *dest = '\0';

    return dest;
  }

  // decrease dest size for '\0' symbol
  --destSize;

  std::size_t symbols = 0;
  const bool valueNegative = (value < 0);
  if (valueNegative) {
    symbols = integerToSymbols(dest, destSize, static_cast<unsigned_type>(-(value + 1)), 10);
  } else {
    symbols = integerToSymbols(dest, destSize, valueNegative ? -value : value, 10);
  }

  if (valueNegative && symbols < destSize)
    dest[symbols++] = '-';

  dest[symbols] = '\0';
  reverseString(dest, symbols);

  return dest;
}

} // namespace toygine

#endif // SRC_CORE_CORE_UTILS_INTERNAL_INL_
