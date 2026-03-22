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
  \file   utils.inl
  \brief  Inline implementations for utilities declared in \c utils.hpp.

  Included by \ref core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_UTILS_INL_
#define INCLUDE_CORE_UTILS_INL_

namespace toy {

constexpr wchar_t * utf8toWChar(wchar_t * dest, size_t destSize, const char * src) noexcept {
  assert_message(src != nullptr, "The source must not be null.");

  return src != nullptr ? utf8toWChar(dest, destSize, src, char_traits<char>::length(src)) : nullptr;
}

template <StringLike T>
constexpr wchar_t * utf8toWChar(wchar_t * dest, size_t destSize, const T & src) noexcept {
  return utf8toWChar(dest, destSize, src.c_str(), src.size());
}

constexpr char * reverseString(char * string, size_t stringLength) noexcept {
  assert_message(string != nullptr, "The source string must not be null.");

  if (stringLength == 0)
    stringLength = char_traits<char>::length(string);

  if (stringLength != 0) {
    for (size_t i = 0, j = stringLength - 1; i < j; ++i, --j) {
      std::swap(string[i], string[j]);
    }
  }

  return string;
}

template <std::signed_integral T>
constexpr char * itoa(char * dest, size_t destSize, T value) noexcept {
  assert_message(dest != nullptr, "The destination buffer must not be null.");
  assert_message(destSize >= numeric_limits<T>::digits10 + 2,
                 "The destination buffer is too small for the given type.");

  if (dest == nullptr || destSize == 0)
    return dest;
  else if (destSize == 1) {
    *dest = '\0';

    return dest;
  }

  if (value < 0) {
    *dest = '-';
    utoa(dest + 1, destSize - 1, static_cast<std::make_unsigned_t<T>>(-(value + 1)) + 1U, 10);
  } else {
    utoa(dest, destSize, static_cast<std::make_unsigned_t<T>>(value), 10);
  }

  return dest;
}

template <std::unsigned_integral T>
constexpr char * utoa(char * dest, size_t destSize, T value, unsigned base) noexcept {
  assert_message(dest != nullptr, "The destination buffer must not be null.");
  assert_message(destSize > 0, "The destination buffer size must be greater than zero.");

  if (dest == nullptr || destSize == 0)
    return dest;
  else if (destSize == 1) {
    *dest = '\0';

    return dest;
  }

  // ANSI digit lookup table for base conversion.
  static constexpr array<char, 36> ansiDigits{{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  }};

  assert_message(base >= 2 && base <= ansiDigits.size(), "The base must be between 2 and 36 inclusive.");

  // decrease dest size for '\0' symbol
  --destSize;

  size_t symbols = 0;

  do {
    dest[symbols++] = ansiDigits[value % base];
  } while ((value /= static_cast<T>(base)) > 0 && symbols < destSize);

  dest[symbols] = '\0';
  reverseString(dest, symbols);

  return dest;
}

[[nodiscard]] inline size_t highestBit(uint64_t value) noexcept {
  assert_message(value != 0, "highestBit requires non-zero value (no highest set bit when value is zero)");

#if defined(_MSC_VER)
  unsigned long index;
#if defined(_WIN64)
  _BitScanReverse64(&index, value);
#else
  if (_BitScanReverse(&index, static_cast<unsigned long>(value >> 32)) != 0) {
    index += 32;
  } else {
    _BitScanReverse(&index, static_cast<unsigned long>(value & 0xfffffffflu));
  }
#endif
  return index;
#elif defined(__GNUC__) || defined(__clang__)
  return sizeof(value) * 8 - 1 - static_cast<size_t>(__builtin_clzll(value));
#else
#error "Platform does not support highestBit()"
#endif
}

} // namespace toy

#endif // INCLUDE_CORE_UTILS_INL_
