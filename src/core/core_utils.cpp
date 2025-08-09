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
#include <array>

#include "core.hpp"
#include "core_utils_internal.inl"

namespace {
constexpr std::array<std::uint8_t, 256> sc_utf8CharSizeTable{
  {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,

   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,

   0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,

   0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x07, 0x08}};

constexpr std::array<std::uint32_t, 32> const exponentTable{
  {0xF0BDC21A, 0x3DA137D5, 0x9DC5ADA8, 0x2863C1F5, 0x6765C793, 0x1A784379, 0x43C33C19, 0xAD78EBC5,
   0x2C68AF0B, 0x71AFD498, 0x1D1A94A2, 0x4A817C80, 0xBEBC2000, 0x30D40000, 0x7D000000, 0x20000000,
   0x51EB851E, 0xD1B71758, 0x35AFE535, 0x89705F41, 0x232F3302, 0x5A126E1A, 0xE69594BE, 0x3B07929F,
   0x971DA050, 0x26AF8533, 0x63090312, 0xFD87B5F2, 0x40E75996, 0xA6274BBD, 0x2A890926, 0x6CE3EE76}};

/*!
  \brief Divides a given 32-bit unsigned integer by 10 and returns the quotient and remainder.

  The function approximates division by 10 using a sequence of shift-and-add operations to compute the quotient, then
  derives the remainder and corrects it if needed. It returns both quotient and remainder.

  \param value The 32-bit unsigned integer to divide by 10.

  \return A struct containing the quotient and remainder of the division.
*/

toygine::divmod10 divModU10(std::uint32_t value) noexcept {
  toygine::divmod10 res;

  res.quot = value >> 1;
  res.quot += res.quot >> 1;
  res.quot += res.quot >> 4;
  res.quot += res.quot >> 8;
  res.quot += res.quot >> 16;

  const auto qq = res.quot;
  res.quot >>= 3;

  constexpr std::uint32_t mask32 = 0xFFFFFFF8;
  res.rem = static_cast<std::uint8_t>(value - ((res.quot << 1) + (qq & mask32)));
  if (res.rem > 9) {
    res.rem -= 10;
    ++res.quot;
  }

  return res;
}

} // namespace

namespace toygine {

wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, char const * src, std::size_t count) {
  if (dest == nullptr || destSize == 0)
    return nullptr;

  wchar_t * destPointer = dest;
  if (count > 0 && src != nullptr) {
    wchar_t const * unicodeEndPos = dest + (destSize - 1);
    std::size_t srcIterator = 0;

    while (srcIterator < count && destPointer < unicodeEndPos) {
      std::uint8_t symbol = static_cast<std::uint8_t>(src[srcIterator++]);
      if (symbol <= 0x7F) {
        *destPointer = symbol;
      } else {
        std::size_t charBytes = 0;
        while (symbol & 0x80) {
          ++charBytes;
          symbol <<= 1;
        }

        wchar_t unicodeChar = static_cast<wchar_t>(symbol >> charBytes);
        while (charBytes-- > 1) {
          unicodeChar <<= 6;
          unicodeChar |= src[srcIterator++] & 0x3F;
        }

        *destPointer = unicodeChar;
      }

      ++destPointer;
    }
  }

  *destPointer = L'\0';

  return dest;
}

char * wcharToUtf8(char * dest, std::size_t destSize, wchar_t const * src) {
  if (dest == nullptr || destSize == 0)
    return nullptr;

  char * destPointer = dest;
  if (src != nullptr) {
    char const * utf8EndPos = dest + (destSize - 1);

    while (*src != L'\0' && destPointer < utf8EndPos) {
      std::uint32_t symbol = static_cast<std::uint32_t>(*src++);
      if (symbol <= 0x7F) {
        *destPointer = static_cast<char>(symbol);
      } else {
        if (symbol <= 0x7FF) {
          *destPointer = static_cast<char>(((symbol & 0x07C0) >> 6) | 0xC0);
        } else {
          *destPointer = static_cast<char>(((symbol & 0xF000) >> 12) | 0xE0);
          ++destPointer;
          *destPointer = static_cast<char>(((symbol & 0x0FC0) >> 6) | 0x80);
        }

        ++destPointer;
        *destPointer = static_cast<char>((symbol & 0x003F) | 0x80);
      }
      ++destPointer;
    }
  }

  *destPointer = '\0';

  return dest;
}

std::size_t utf8len(const char * str) {
  assert(str != nullptr);
  if (str == nullptr)
    return 0;

  std::size_t size = 0;
  while (*str != '\0') {
    const auto symbolLength = sc_utf8CharSizeTable[static_cast<std::uint8_t>(*str)];
    assert_message(symbolLength != 0, "Invalid UTF-8 symbol");
    str += symbolLength;
    ++size;
  }

  return size;
}

char * itoa(char * dest, std::size_t destSize, std::int8_t value) {
  return itoaImplementation(dest, destSize, value);
}

char * itoa(char * dest, std::size_t destSize, std::int16_t value) {
  return itoaImplementation(dest, destSize, value);
}

char * itoa(char * dest, std::size_t destSize, std::int32_t value) {
  return itoaImplementation(dest, destSize, value);
}

char * itoa(char * dest, std::size_t destSize, std::int64_t value) {
  return itoaImplementation(dest, destSize, value);
}

char * itoa(char * dest, std::size_t destSize, std::uint8_t value, unsigned base) {
  return utoaImplementation(dest, destSize, value, base);
}

char * itoa(char * dest, std::size_t destSize, std::uint16_t value, unsigned base) {
  return utoaImplementation(dest, destSize, value, base);
}

char * itoa(char * dest, std::size_t destSize, std::uint32_t value, unsigned base) {
  return utoaImplementation(dest, destSize, value, base);
}

char * itoa(char * dest, std::size_t destSize, std::uint64_t value, unsigned base) {
  return utoaImplementation(dest, destSize, value, base);
}

/*!
  \brief Converts a 32-bit unsigned integer to its decimal string representation in reverse order.

  This function divides the given 32-bit unsigned integer by 10 repeatedly to compute each digit of its decimal
  representation. The digits are stored in reverse order in the provided buffer, starting from the position
  just before the null-terminator. The buffer should be large enough to hold the entire string representation.

  \param value     The 32-bit unsigned integer to be converted.
  \param bufferEnd A pointer to the end of the buffer where the resulting string will be stored in reverse order,
                   with a null-terminator at the end.

  \return A pointer to the beginning of the string representation within the buffer.
*/
char * utoaFast(char * bufferEnd, std::uint32_t value) noexcept {
  *bufferEnd = '\0';

  divmod10 res;
  res.quot = value;
  do {
    res = divModU10(res.quot);
    *--bufferEnd = res.rem + '0';
  } while (res.quot != 0);

  return bufferEnd;
}

/*!
  \brief Converts a floating-point number to its string representation in a specified precision.

  This function converts a given floating-point number into its string representation, storing the result in the
  provided destination buffer. The function rounds the result to the given precision and stores the exponent in the
  return value.

  \param value     The floating-point number to be converted.
  \param buffer    The destination buffer where the converted string is stored.
  \param precision The precision of the conversion, i.e., the number of digits after the decimal point.

  \return The exponent of the converted number in the given precision.

  \note The function assumes that the destination buffer is large enough to hold the converted string. The function does
        not support subnormals.
*/
std::int32_t ftoa32Engine(char * buffer, float value, std::size_t precision) noexcept {
  const auto uvalue = std::bit_cast<uint32_t>(value);
  const auto exponent = static_cast<std::uint8_t>(uvalue >> 23);
  if (exponent == 0) { // don't care about a subnormals
    buffer[0] = '0';
    buffer[1] = '\0';
    return 0xff;
  }

  auto pointer = buffer;
  if (uvalue & 0x80000000)
    *pointer++ = '-';
  else
    *pointer++ = '+';

  const std::uint32_t fraction = (uvalue & 0x00FFFFFF) | 0x00800000;
  if (exponent == 0xFF) {
    if (fraction & 0x007FFFFF) {
      pointer[0] = 'N';
      pointer[1] = 'A';
      pointer[2] = 'N';
    } else {
      pointer[0] = 'I';
      pointer[1] = 'N';
      pointer[2] = 'F';
    }

    pointer[3] = 0;
    return 0xff;
  }

  *pointer++ = '0';

  std::int32_t exp10 = (((exponent >> 3) * 77 + 63) >> 5) - 38;
  std::uint32_t t
    = static_cast<std::uint32_t>((static_cast<std::uint64_t>(fraction << 8) * exponentTable[exponent / 8]) >> 32) + 1;
  t >>= (7 - (exponent & 7));

  std::uint8_t digit = t >> 28;
  while (digit == 0) {
    t &= 0x0fffffff;
    t *= 10;
    digit = t >> 28;
    --exp10;
  }

  for (std::size_t iter = precision + 1; iter > 0; --iter) {
    digit = t >> 28;
    *pointer++ = digit + '0';
    t &= 0x0fffffff;
    t *= 10;
  }

  // roundup
  if (buffer[precision + 2] >= '5')
    buffer[precision + 1]++;

  pointer[-1] = '\0';
  pointer -= 2;

  for (std::size_t index = precision + 1; index > 1; --index) {
    if (buffer[index] > '9') {
      buffer[index] -= 10;
      ++buffer[index - 1];
    }
  }

  while (pointer[0] == '0')
    *pointer-- = '\0';

  return exp10;
}

} // namespace toygine
