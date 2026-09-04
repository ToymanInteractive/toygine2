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
  \brief  Implementation details for the core string and UTF-8 utilities.

  Defines toy::utf8Len(), toy::validateUtf8(), and the internal UTF-8 character-size lookup table they read for O(1)
  lead-byte classification; further utility definitions are added here as the API grows.
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
constexpr array<uint8_t, 256> c_utf8CharSizeTable{
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
  (Unicode table 3-7). Read by toy::validateUtf8() alone; the counting walk does no range check.

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

} // namespace

size_t utf8Len(const char * str) noexcept {
  assert_message(str != nullptr, "C string must not be null");
  if (str == nullptr)
    return 0;

  assert_message(validateUtf8(str), "C string must hold well-formed UTF-8");

  size_t size = 0;
  while (*str != '\0') {
    const auto symbolLength = c_utf8CharSizeTable[bit_cast<uint8_t>(*str)];
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

bool validateUtf8(const char * str) noexcept {
  if (str == nullptr)
    return false;

  while (*str != '\0') {
    const auto leadByte     = bit_cast<uint8_t>(*str);
    const auto symbolLength = c_utf8CharSizeTable[leadByte];
    if (symbolLength == 0)
      return false;

    // The lead byte constrains the byte after it; the rest carry the plain 10xxxxxx form. A terminator inside the
    // sequence falls outside either range, which is what stops the walk at the end of a string cut short.
    ByteRange range = secondByteRange(leadByte);
    for (uint8_t offset = 1; offset < symbolLength; ++offset) {
      if (const auto trailingByte = bit_cast<uint8_t>(str[offset]);
          trailingByte < range.lowest || trailingByte > range.highest)
        return false;

      range = c_continuationRange;
    }

    str += symbolLength;
  }

  return true;
}

} // namespace toy
