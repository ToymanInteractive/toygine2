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
  \file   utils.cpp
  \brief  Definitions for string, encoding, and number utilities declared in \c core/utils.hpp.
*/

#include "core.hpp"

namespace toy {

/*!
  \brief UTF-8 character size lookup table for efficient UTF-8 parsing.

  This lookup table contains precomputed UTF-8 character sizes for all possible byte values (0-255). The table maps each
  byte to the number of bytes required to represent a complete UTF-8 character sequence. Using this table allows for
  O(1) UTF-8 character size determination instead of O(n) bit-by-bit analysis.

  \note Values: 0x01 = 1-byte ASCII character, 0x02-0x04 = multi-byte UTF-8 sequence, 0x00 = invalid/incomplete.
*/
static constexpr array<uint8_t, 256> _utf8CharSizeTable{
  {
   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
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

   0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x07, 0x08,
   }
};

/*!
  \brief Precomputed exponent lookup table for efficient binary-to-decimal floating-point conversion.

  This lookup table contains 32 precomputed values used to convert IEEE-754 binary floating-point exponents to decimal
  exponents during float-to-string conversion. The table maps groups of 8 binary exponent values to precomputed
  multipliers that approximate the conversion from binary to decimal representation.

  \note The table is indexed by exponent / 8, where exponent is the 8-bit IEEE-754 exponent field (0-255).
        Each entry is a 32-bit fixed-point multiplier used to compute the decimal mantissa efficiently.
  \note This table enables O(1) lookup instead of expensive runtime power-of-10 calculations, significantly improving
        the performance of floating-point number formatting.
*/
constexpr array<uint32_t, 32> _exponentTable{
  {
   0xF0BDC21A, 0x3DA137D5, 0x9DC5ADA8, 0x2863C1F5, 0x6765C793, 0x1A784379, 0x43C33C19, 0xAD78EBC5,
   0x2C68AF0B, 0x71AFD498, 0x1D1A94A2, 0x4A817C80, 0xBEBC2000, 0x30D40000, 0x7D000000, 0x20000000,
   0x51EB851E, 0xD1B71758, 0x35AFE535, 0x89705F41, 0x232F3302, 0x5A126E1A, 0xE69594BE, 0x3B07929F,
   0x971DA050, 0x26AF8533, 0x63090312, 0xFD87B5F2, 0x40E75996, 0xA6274BBD, 0x2A890926, 0x6CE3EE76,
   }
};

/*!
  \struct _divmod10

  \brief Result of an unsigned division by 10, carrying quotient and remainder.

  Used by _divModU10() to return both values from a single branchless division step.

  \section features Key Features

  - **Aggregate layout**: Two-field POD suitable for constexpr return.
  - **Compact remainder**: Remainder stored as \c uint8_t (always in \c [0, 9]).
  - **No allocation**: Stack-only value type.
  - **noexcept**: Trivially constructible and copyable.

  \section performance Performance Characteristics

  - **Construction**: O(1) constant time.
  - **Memory usage**: 8 bytes (4-byte quotient + 1-byte remainder + padding).

  \section safety Safety Guarantees

  - **Value range**: \a rem is always in \c [0, 9] when produced by _divModU10().
  - **Exception safety**: Trivial type; all operations are noexcept.

  \sa _divModU10()
*/
struct _divmod10 {
  /// Quotient of the division by 10.
  uint32_t quot;

  /// Remainder of the division by 10 (always in \c [0, 9]).
  uint8_t rem;
};

/*!
  \brief Divides a given 32-bit unsigned integer by 10 and returns the quotient and remainder.

  The function approximates division by 10 using a sequence of shift-and-add operations to compute the quotient, then
  derives the remainder and corrects it if needed. It returns both quotient and remainder.

  \param value The 32-bit unsigned integer to divide by 10.

  \return A struct containing the quotient and remainder of the division.
*/
constexpr _divmod10 _divModU10(uint32_t value) noexcept {
  _divmod10 res;

  res.quot  = value >> 1;
  res.quot += res.quot >> 1;
  res.quot += res.quot >> 4;
  res.quot += res.quot >> 8;
  res.quot += res.quot >> 16;

  const auto qq   = res.quot;
  res.quot      >>= 3;

  res.rem = static_cast<uint8_t>(value - ((res.quot << 1) + (qq & 0xFFFFFFF8U)));
  if (res.rem > 9) {
    res.rem -= 10;
    ++res.quot;
  }

  return res;
}

/*!
  \brief Converts a floating-point number to its string representation in a specified precision. The output is always
         sign-prefixed ('+' or '-') and normalized as "+0.<digits>" or "-0.<digits>".

  This function converts a given floating-point number into its string representation, storing the result in the
  provided destination buffer. The function rounds the result to the given precision and stores the exponent in the
  return value.

  \param value     The floating-point number to be converted.
  \param buffer    The destination buffer where the converted string is stored.
  \param precision The precision (digits after the decimal point). For IEEE-754 f32, practical precision is ~7–9 digits.

  \return The exponent of the converted number in the given precision. Returns 0xFF for zero, subnormals (unsupported),
          NaN, and INF.

  \note The function assumes that the destination buffer is large enough to hold the converted string. The function does
        not support subnormals.
*/
constexpr int32_t _ftoa32Engine(char * buffer, float value, size_t precision) noexcept {
  const auto uvalue   = bit_cast<uint32_t>(value);
  const auto exponent = static_cast<uint8_t>((uvalue >> 23) & 0xff);
  if (exponent == 0) { // don't care about a subnormals
    buffer[0] = '0';
    buffer[1] = '\0';
    return 0xFF;
  }

  char * pointer = buffer;
  if (uvalue & 0x80000000)
    *pointer++ = '-';
  else
    *pointer++ = '+';

  const uint32_t fraction = (uvalue & 0x007FFFFF) | 0x00800000;
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

    pointer[3] = '\0';

    return 0xFF;
  }

  *pointer++ = '0';

  int32_t  exp10 = (((exponent >> 3) * 77 + 63) >> 5) - 38;
  uint32_t t = static_cast<uint32_t>((static_cast<uint64_t>(fraction << 8) * _exponentTable[exponent / 8U]) >> 32) + 1;
  t >>= (7 - (exponent & 7));

  uint8_t digit = t >> 28;
  while (digit == 0) {
    t     &= 0x0fffffff;
    t     *= 10;
    digit  = t >> 28;
    --exp10;
  }

  for (size_t iter = precision + 1; iter > 0; --iter) {
    digit       = t >> 28;
    *pointer++  = digit + '0';
    t          &= 0x0fffffff;
    t          *= 10;
  }

  // roundup
  if (buffer[precision + 2] >= '5')
    buffer[precision + 1]++;

  pointer[-1]  = '\0';
  pointer     -= 2;

  for (size_t index = precision + 1; index > 1; --index) {
    if (buffer[index] > '9') {
      buffer[index] -= 10;
      ++buffer[index - 1];
    }
  }

  // If carry propagated into the integer digit ('0' at buffer[1]), adjust it and bump the exponent.
  if (buffer[1] > '9') {
    buffer[1] = '1';
    ++exp10;
  }

  while (pointer > buffer + 1 && pointer[0] == '0')
    *pointer-- = '\0';

  return exp10;
}

/*!
  \brief Converts a 64-bit floating-point number to its string representation with specified precision. The output is
         always sign-prefixed ('+' or '-') and normalized as "+0.<digits>" or "-0.<digits>".

  This function converts a given 64-bit floating-point number into its string representation, storing the result
  in the provided buffer. The conversion includes handling special cases such as subnormals, NaN, and infinity.
  The output is formatted according to the specified precision.

  \param buffer    The destination buffer where the converted string is stored.
  \param value     The 64-bit floating-point number to be converted.
  \param precision The number of decimal places to include in the representation.

  \return The exponent of the converted number in the given precision. Returns 0x7FF for zero, subnormals (unsupported),
          NaN, and INF.

  \note The function assumes that the buffer is large enough to hold the converted string. The buffer will contain
        the string representation in the form "+d.dd...e±dd" for normalized numbers.
*/
constexpr int32_t _ftoa64Engine(char * buffer, double value, size_t precision) noexcept {
  const auto uvalue   = bit_cast<uint64_t>(value);
  const auto exponent = static_cast<uint32_t>(uvalue >> 52) & 0x07FF;
  if (exponent == 0) { // don't care about a subnormals
    buffer[0] = '0';
    buffer[1] = '\0';
    return 0x7FF;
  }

  if (exponent == 0x07FF) {
    char * pointer = buffer;
    if (uvalue & 0x8000000000000000ULL)
      *pointer++ = '-';
    else
      *pointer++ = '+';

    if ((uvalue & 0x000FFFFFFFFFFFFFULL) != 0) {
      pointer[0] = 'N';
      pointer[1] = 'A';
      pointer[2] = 'N';
    } else {
      pointer[0] = 'I';
      pointer[1] = 'N';
      pointer[2] = 'F';
    }

    pointer[3] = '\0';

    return 0x7FF;
  }

  return _ftoa32Engine(buffer, static_cast<float>(value), precision);
}

/*!
  \brief Processes a string representation of a floating-point number, adjusting the exponent, and stripping trailing
         zeros.

  This function processes a string representation of a floating-point number, adjusting the exponent according to the
  given precision, and stripping trailing zeros. The function also handles the case of negative zero.

  \param dest       The destination buffer where the processed string is stored.
  \param srcBuffer  The source buffer containing the string representation of a floating-point number.
  \param bufferSize The size of the source buffer.
  \param exp10      The exponent of the floating-point number in the given precision.
  \param precision  The number of decimal places to include in the representation.

  \note The function assumes that the destination buffer is large enough to hold the processed string. The buffer will
        contain the string representation in the form "+d.dd...e±dd" for normalized numbers.
*/
void _floatPostProcess(char * dest, char * srcBuffer, size_t bufferSize, int32_t exp10, size_t precision) noexcept {
  const char * strBegin = srcBuffer + 2;
  if (srcBuffer[1] != '0') {
    // Carry propagated into the integer position at [1] (e.g., 0.999.. -> 1.000..).
    // Include that '1' in the mantissa by shifting strBegin left, and bump exp10 to keep
    // the mantissa/exponent product invariant.
    ++exp10;
    --strBegin;
  }

  const auto digits       = char_traits<char>::length(strBegin);
  size_t     intDigits    = 0;
  size_t     leadingZeros = 0;
  if (static_cast<size_t>(std::abs(exp10)) >= precision) {
    intDigits = 1;
  } else if (exp10 >= 0) {
    intDigits = static_cast<size_t>(exp10 + 1);
    exp10     = 0;
  } else {
    intDigits    = 0;
    leadingZeros = static_cast<size_t>(-exp10 - 1);
    exp10        = 0;
  }

  char * outputPointer = dest;
  if (*srcBuffer == '-') // copy sign if negative
    *outputPointer++ = '-';

  size_t fractionDigits = digits > intDigits ? digits - intDigits : 0;
  if (intDigits > 0) {
    auto count = std::min(intDigits, digits);
    while (count--)
      *outputPointer++ = *strBegin++;

    auto trailingZeros = static_cast<int32_t>(intDigits) - static_cast<int32_t>(digits);
    while (trailingZeros-- > 0)
      *outputPointer++ = '0';
  } else {
    *outputPointer++ = '0';
  }

  if (fractionDigits > 0) {
    *outputPointer++ = '.';
    while (leadingZeros-- > 0)
      *outputPointer++ = '0';

    while (fractionDigits-- > 0)
      *outputPointer++ = *strBegin++;
  }

  if (exp10 != 0) {
    *outputPointer++ = 'e';
    uint32_t upow10;
    if (exp10 < 0) {
      *outputPointer++ = '-';
      upow10           = static_cast<uint32_t>(-exp10);
    } else {
      *outputPointer++ = '+';
      upow10           = static_cast<uint32_t>(exp10);
    }

    char * bufferEndPointer = srcBuffer + bufferSize - 1;
    *bufferEndPointer       = '\0';

    _divmod10 res;
    res.quot = upow10;
    do {
      res                 = _divModU10(res.quot);
      *--bufferEndPointer = res.rem + '0';
    } while (res.quot != 0);

    while (bufferEndPointer < srcBuffer + bufferSize)
      *outputPointer++ = *bufferEndPointer++;
  }

  *outputPointer = '\0';
}

wchar_t * utf8toWChar(wchar_t * dest, size_t destSize, const char * src, size_t count) noexcept {
  if (dest == nullptr || destSize == 0)
    return nullptr;

  wchar_t * destPointer = dest;
  if (count == 0 || src == nullptr) {
    *destPointer = L'\0';

    return dest;
  }

  const wchar_t * const unicodeEndPos = dest + (destSize - 1);

  size_t srcIterator = 0;
  while (srcIterator < count && destPointer < unicodeEndPos) {
    const auto lead   = static_cast<byte>(src[srcIterator++]);
    const auto seqLen = _utf8CharSizeTable[std::to_integer<size_t>(lead)];
    if (seqLen == 0 || seqLen > 3 || srcIterator + (seqLen - 1) > count) {
      break;
    } else if (seqLen == 1) {
      *destPointer++ = std::to_integer<wchar_t>(lead);
      continue;
    }

    const auto leadMask  = seqLen == 2 ? byte{0x1F} : byte{0x0F};
    wchar_t    codePoint = std::to_integer<wchar_t>(lead & leadMask);

    for (size_t i = 1; i < seqLen; ++i) {
      const auto cont = static_cast<std::byte>(src[srcIterator++]);
      if ((cont & std::byte{0xC0}) != std::byte{0x80}) {
        *destPointer = L'\0';

        return dest;
      }

      codePoint = static_cast<wchar_t>((codePoint << 6) | std::to_integer<wchar_t>(cont & std::byte{0x3F}));
    }

    *destPointer++ = codePoint;
  }

  *destPointer = L'\0';
  return dest;
}

char * wcharToUtf8(char * dest, size_t destSize, const wchar_t * src) noexcept {
  if (dest == nullptr || destSize == 0)
    return nullptr;

  if (src == nullptr) {
    *dest = '\0';

    return dest;
  }

  char *             destPointer = dest;
  const char * const utf8EndPos  = dest + (destSize - 1);

  while (*src != L'\0' && destPointer < utf8EndPos) {
    if (const auto symbol = static_cast<uint32_t>(*src++); symbol <= 0x7F) {
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

  *destPointer = '\0';

  return dest;
}

size_t utf8Len(const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");
  if (string == nullptr)
    return 0;

  size_t size = 0;
  while (*string != '\0') {
    const auto symbolLength = _utf8CharSizeTable[static_cast<uint8_t>(*string)];
    assert_message(symbolLength != 0, "Invalid UTF-8 symbol");
    if (symbolLength == 0)
      return 0;

    string += symbolLength;
    ++size;
  }

  return size;
}

char * ftoa(char * dest, size_t destSize, float value, size_t precision) noexcept {
  assert_message(dest != nullptr && destSize > 0, "The destination buffer must not be null.");
  if (dest == nullptr || destSize == 0)
    return dest;

  *dest = '\0';
  if (destSize == 1)
    return dest;

  const size_t bufferSize = 128;
  char         buffer[bufferSize + 1];

  if (const auto exp10 = _ftoa32Engine(buffer, value, precision); exp10 == 0xFF) {
#if defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
    strcpy_s(dest, destSize, buffer);
#else  // defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
    strncpy(dest, buffer, destSize - 1);
    dest[destSize - 1] = '\0';
#endif // defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
  } else {
    _floatPostProcess(dest, buffer, bufferSize, exp10, precision);
  }

  return dest;
}

char * ftoa(char * dest, size_t destSize, double value, size_t precision) noexcept {
  assert_message(dest != nullptr && destSize > 0, "The destination buffer must not be null.");
  if (dest == nullptr || destSize == 0)
    return dest;

  *dest = '\0';
  if (destSize == 1)
    return dest;

  const size_t bufferSize = 128;
  char         buffer[bufferSize + 1];

  if (const auto exp10 = _ftoa64Engine(buffer, value, precision); exp10 == 0x7FF) {
#if defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
    strcpy_s(dest, destSize, buffer);
#else  // defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
    strncpy(dest, buffer, destSize - 1);
    dest[destSize - 1] = '\0';
#endif // defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
  } else {
    _floatPostProcess(dest, buffer, bufferSize, exp10, precision);
  }

  return dest;
}

/*!
  \brief Number of decimal digits per group when \c formatNumberString() inserts the caller-supplied separator.

  Fixed at three so grouping matches common thousands-style splitting from the right.
*/
constexpr size_t _decimalDigitsPerGroup = 3;

void formatNumberString(char * buffer, size_t bufferSize, const char * separator) noexcept {
  assert_message(buffer != nullptr && bufferSize > 0, "The destination buffer must not be null.");
  assert_message(separator != nullptr, "The grouping separator must not be null.");
  if (separator == nullptr)
    return;

  const auto separatorLen = char_traits<char>::length(separator);
  assert_message(separatorLen <= 8, "The grouping separator must not exceed 8 characters.");
  if (separatorLen == 0)
    return;

  if (*buffer == '-' || *buffer == '+') {
    ++buffer;
    --bufferSize;
  }

  size_t digitsCount = 0;
  while (buffer[digitsCount] >= '0' && buffer[digitsCount] <= '9')
    ++digitsCount;

  if (digitsCount <= _decimalDigitsPerGroup) // Nothing to format.
    return;

  auto       groupsCount        = (digitsCount - 1U) / _decimalDigitsPerGroup;
  const auto ansiStringLen      = char_traits<char>::length(buffer);
  const auto requiredBufferSize = ansiStringLen + groupsCount * separatorLen;
  assert_message(requiredBufferSize < bufferSize, "Buffer size is too low.");
  if (requiredBufferSize >= bufferSize)
    return;

  buffer[ansiStringLen + groupsCount * separatorLen] = '\0';

  if (digitsCount != (ansiStringLen - 1))
    std::memmove(buffer + (digitsCount + groupsCount * separatorLen), buffer + digitsCount,
                 ansiStringLen - digitsCount);

  auto scanChars = digitsCount;
  while (groupsCount > 0) {
    std::memmove(buffer + (scanChars + groupsCount * separatorLen - _decimalDigitsPerGroup),
                 buffer + (scanChars - _decimalDigitsPerGroup), _decimalDigitsPerGroup);
    const auto destBufferShift = scanChars + (groupsCount - 1) * separatorLen - _decimalDigitsPerGroup;
    char_traits<char>::copy(buffer + destBufferShift, separator, separatorLen);
    scanChars -= _decimalDigitsPerGroup;
    --groupsCount;
  }
}

} // namespace toy
