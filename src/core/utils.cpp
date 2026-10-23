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
  \file   utils.cpp
  \brief  Implementations for the UTF-8 and UTF-16 utilities declared in utils.hpp.

  The validator and the UTF-8 decoder share one well-formedness check, so they accept the same sequences.
*/

#include "core.hpp"

namespace toy {

namespace {

/*!
  \brief UTF-8 character size lookup table for efficient UTF-8 parsing.

  This lookup table maps each of the 256 possible byte values to the length of a valid UTF-8 sequence that byte may
  begin, or to 0x00 when the byte can never start one. It gives O(1) lead-byte classification instead of O(n) bit-by-bit
  analysis.

  \note Values: 0x01 = 1-byte ASCII, 0x02-0x04 = length of a multi-byte sequence this lead byte may begin, 0x00 = byte
        that can never start a valid sequence — continuation bytes (0x80-0xBF), overlong 2-byte leads (0xC0-0xC1), and
        out-of-range leads (0xF5-0xFF).
  \note Range and overlong checks that depend on continuation bytes (e.g. for 0xE0, 0xED, 0xF0, 0xF4) are left to
        toy::validateUtf8(); this table validates only what the lead byte alone determines.
*/
constexpr std::array<uint8_t, 256> c_utf8CharSizeTable{
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

   0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,

   0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,

   0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   }
};

/*!
  \brief Inclusive range of byte values a trailing byte may take.
*/
struct ByteRange {
  uint8_t lowest;  ///< Smallest accepted value
  uint8_t highest; ///< Largest accepted value
};

/// Values every trailing byte carries outside the sequences the lead byte constrains: the 10xxxxxx form
constexpr ByteRange c_continuationRange{0x80, 0xBF};

/*!
  \brief Returns the range the byte after \a leadByte may take.

  Four lead bytes narrow that range, which is what rejects the ill-formed sequences the lead byte alone admits
  (Unicode table 3-7). Read through wellFormedLength() by toy::validateUtf8() and toy::utf8ToUtf16(); the counting walk
  does no range check.

  \param leadByte First byte of the sequence.

  \return Range of the second byte, \ref c_continuationRange for a lead byte that narrows nothing.
*/
[[nodiscard]] constexpr ByteRange secondByteRange(uint8_t leadByte) noexcept {
  switch (leadByte) {
    case 0xE0:
      return {0xA0, 0xBF}; // an overlong three-byte encoding of a code point below U+0800
    case 0xED:
      return {0x80, 0x9F}; // a surrogate code point, U+D800 through U+DFFF
    case 0xF0:
      return {0x90, 0xBF}; // an overlong four-byte encoding of a code point below U+10000
    case 0xF4:
      return {0x80, 0x8F}; // a code point past U+10FFFF
    default:
      return c_continuationRange;
  }
}

/*!
  \brief Measures the well-formed sequence that starts at \a str.

  Checks the lead byte, the narrowed range of the byte after it, and the 10xxxxxx form of the rest. A terminator inside
  the sequence falls outside every range, so a walk over a null-terminated string stops at it.

  \param str       First byte of the sequence.
  \param available Number of readable bytes starting at \a str.

  \return Length of the sequence in bytes, or \c 0 when it is ill-formed or does not fit in \a available bytes.

  \pre \a available is at least \c 1.
*/
[[nodiscard]] uint8_t wellFormedLength(const char * str, size_t available) noexcept {
  const auto leadByte     = std::bit_cast<uint8_t>(*str);
  const auto symbolLength = c_utf8CharSizeTable[leadByte];
  if (symbolLength == 0 || symbolLength > available)
    return 0;

  // The lead byte constrains the byte after it; the rest carry the plain 10xxxxxx form.
  auto range = secondByteRange(leadByte);
  for (uint8_t offset = 1; offset < symbolLength; ++offset) {
    if (const auto trailingByte = std::bit_cast<uint8_t>(str[offset]);
        trailingByte < range.lowest || trailingByte > range.highest)
      return 0;

    range = c_continuationRange;
  }

  return symbolLength;
}

/*!
  \brief Measures the well-formed UTF-16 character that starts at \a str.

  A terminator is never a low surrogate, so a walk over a null-terminated string stops at a high surrogate before it.

  \param str       First unit of the character.
  \param available Number of readable units starting at \a str.

  \return \c 1 for a unit outside the surrogate range, \c 2 for a surrogate pair, and \c 0 for a surrogate that is not
          half of a pair inside \a available units.

  \pre \a available is at least \c 1.
*/
[[nodiscard]] uint8_t wellFormedLength(const char16_t * str, size_t available) noexcept {
  const auto unit = *str;
  if (unit < 0xD800 || unit > 0xDFFF)
    return 1;

  // Only a high surrogate followed by a low one encodes a code point.
  if (unit > 0xDBFF || available < 2)
    return 0;

  const auto low = str[1];

  return low >= 0xDC00 && low <= 0xDFFF ? 2 : 0;
}

} // namespace

char16_t * utf8ToUtf16(char16_t * dest, size_t destSize, const char * src, size_t count) noexcept {
  if (dest == nullptr || destSize == 0)
    return nullptr;

  char16_t *             destPointer = dest;
  const char16_t * const utf16EndPos = dest + (destSize - 1);

  for (const char * const end = src != nullptr ? src + count : src; src != end;) {
    if (const auto lead = std::bit_cast<uint8_t>(*src); lead <= 0x7F) {
      if (destPointer == utf16EndPos)
        break;
      *destPointer++ = lead;
      ++src;
      continue;
    }

    const auto symbolLength = wellFormedLength(src, static_cast<size_t>(end - src));
    if (symbolLength == 0)
      break;

    // The mask keeps the zero bit that ends the length prefix, so one shift serves every width.
    auto codePoint = std::to_integer<uint32_t>(std::bit_cast<std::byte>(*src) & (std::byte{0xFF} >> symbolLength));
    for (uint8_t offset = 1; offset < symbolLength; ++offset)
      codePoint = (codePoint << 6) | std::to_integer<uint32_t>(std::bit_cast<std::byte>(src[offset]) & std::byte{0x3F});

    const auto room = static_cast<size_t>(utf16EndPos - destPointer);
    if (codePoint <= 0xFFFF) {
      if (room < 1)
        break;
      *destPointer++ = static_cast<char16_t>(codePoint);

    } else {
      if (room < 2)
        break;
      *destPointer++ = static_cast<char16_t>(0xD800 + ((codePoint - 0x10000) >> 10));
      *destPointer++ = static_cast<char16_t>(0xDC00 + ((codePoint - 0x10000) & 0x03FF));
    }

    src += symbolLength;
  }

  *destPointer = u'\0';

  return dest;
}

char * utf16ToUtf8(char * dest, size_t destSize, const char16_t * src, size_t count) noexcept {
  if (dest == nullptr || destSize == 0)
    return nullptr;

  char *             destPointer = dest;
  const char * const utf8EndPos  = dest + (destSize - 1);

  for (const char16_t * const end = src != nullptr ? src + count : src; src != end;) {
    const auto symbol = static_cast<uint32_t>(*src);
    const auto room   = static_cast<size_t>(utf8EndPos - destPointer);

    if (symbol <= 0x7F) {
      if (room < 1)
        break;
      *destPointer++ = static_cast<char>(symbol);

    } else if (symbol <= 0x7FF) {
      if (room < 2)
        break;
      *destPointer++ = static_cast<char>(((symbol & 0x07C0) >> 6) | 0xC0);
      *destPointer++ = static_cast<char>((symbol & 0x003F) | 0x80);

    } else if (symbol < 0xD800 || symbol > 0xDFFF) {
      if (room < 3)
        break;
      *destPointer++ = static_cast<char>(((symbol & 0xF000) >> 12) | 0xE0);
      *destPointer++ = static_cast<char>(((symbol & 0x0FC0) >> 6) | 0x80);
      *destPointer++ = static_cast<char>((symbol & 0x003F) | 0x80);

    } else {
      // A pair needs its low surrogate inside the range; the zero standing in past the range never matches.
      const auto low = src + 1 != end ? static_cast<uint32_t>(src[1]) : uint32_t{0};
      if (symbol > 0xDBFF || low < 0xDC00 || low > 0xDFFF || room < 4)
        break;

      const auto codePoint = 0x10000 + ((symbol - 0xD800) << 10) + (low - 0xDC00);
      *destPointer++       = static_cast<char>(((codePoint & 0x1C0000) >> 18) | 0xF0);
      *destPointer++       = static_cast<char>(((codePoint & 0x03F000) >> 12) | 0x80);
      *destPointer++       = static_cast<char>(((codePoint & 0x000FC0) >> 6) | 0x80);
      *destPointer++       = static_cast<char>((codePoint & 0x00003F) | 0x80);
      ++src;
    }

    ++src;
  }

  *destPointer = '\0';

  return dest;
}

size_t utf8Len(const char * str) noexcept {
  assert_message(str != nullptr, "C string must not be null");
  if (str == nullptr)
    return 0;

  assert_message(validateUtf8(str), "C string must hold well-formed UTF-8");

  size_t size = 0;
  while (*str != '\0') {
    // ASCII steps by a constant, so the next read does not wait for a table lookup.
    if (std::bit_cast<uint8_t>(*str) <= 0x7F) {
      ++str;
      ++size;
      continue;
    }

    const auto symbolLength = c_utf8CharSizeTable[std::bit_cast<uint8_t>(*str)];
    assert_message(symbolLength != 0, "Invalid UTF-8 symbol");
    if (symbolLength == 0)
      return 0;

    // Stepping over the announced width would pass the terminator when a sequence is cut short of it.
    for (uint8_t offset = 1; offset < symbolLength; ++offset) {
      assert_message(str[offset] != '\0', "Truncated UTF-8 symbol");
      if (str[offset] == '\0')
        return 0;
    }

    str += symbolLength;
    ++size;
  }

  return size;
}

size_t utf16Len(const char16_t * str) noexcept {
  assert_message(str != nullptr, "UTF-16 string must not be null");
  if (str == nullptr)
    return 0;

  assert_message(validateUtf16(str), "UTF-16 string must hold well-formed UTF-16");

  size_t size = 0;
  while (*str != u'\0') {
    if (const auto unit = *str; unit >= 0xD800 && unit <= 0xDFFF) {
      // Only a high surrogate followed by a low one encodes a code point; the terminator is never a low surrogate.
      const auto low    = str[1];
      const auto isPair = unit <= 0xDBFF && low >= 0xDC00 && low <= 0xDFFF;
      assert_message(isPair, "UTF-16 string must hold no lone surrogate");
      if (!isPair)
        return 0;

      ++str;
    }

    ++str;
    ++size;
  }

  return size;
}

bool validateUtf8(const char * str) noexcept {
  if (str == nullptr)
    return false;

  // No length is known, so only the terminator bounds each sequence.
  while (*str != '\0') {
    const auto symbolLength = wellFormedLength(str, std::numeric_limits<size_t>::max());
    if (symbolLength == 0)
      return false;

    str += symbolLength;
  }

  return true;
}

bool validateUtf8(const char * str, size_t size) noexcept {
  if (str == nullptr)
    return size == 0;

  for (const char * const end = str + size; str != end;) {
    const auto symbolLength = wellFormedLength(str, static_cast<size_t>(end - str));
    if (symbolLength == 0)
      return false;

    str += symbolLength;
  }

  return true;
}

bool validateUtf16(const char16_t * str) noexcept {
  if (str == nullptr)
    return false;

  // No length is known, so only the terminator bounds each character.
  while (*str != u'\0') {
    const auto symbolLength = wellFormedLength(str, std::numeric_limits<size_t>::max());
    if (symbolLength == 0)
      return false;

    str += symbolLength;
  }

  return true;
}

bool validateUtf16(const char16_t * str, size_t size) noexcept {
  if (str == nullptr)
    return size == 0;

  for (const char16_t * const end = str + size; str != end;) {
    const auto symbolLength = wellFormedLength(str, static_cast<size_t>(end - str));
    if (symbolLength == 0)
      return false;

    str += symbolLength;
  }

  return true;
}

} // namespace toy
