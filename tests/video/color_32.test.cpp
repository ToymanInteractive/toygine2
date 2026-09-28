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
  \file   color_32.test.cpp
  \brief  Unit tests for \ref toy::video::Color32.
*/

#include "toy_test.hpp"
#include "video.hpp"

namespace toy::video {

// The size, alignment and byte offsets an upload relies on, and what creating or copying a color costs.
TEST_CASE("video/color32/layout") {
  static_assert(sizeof(Color32) == 4, "Color32 must occupy exactly four bytes, one per channel");
  static_assert(alignof(Color32) == 4, "Color32 must align to four bytes so a copy or compare is one 32-bit access");
  static_assert(std::is_trivially_default_constructible_v<Color32>,
                "Color32 must not zero itself, so a pixel or vertex array costs nothing to create");
  static_assert(std::is_trivially_copyable_v<Color32>, "Color32 must copy as four bytes");
  static_assert(std::is_standard_layout_v<Color32>, "Color32 must have a layout offsetof can describe");
  static_assert(offsetof(Color32, r) == 0 && offsetof(Color32, g) == 1 && offsetof(Color32, b) == 2
                  && offsetof(Color32, a) == 3,
                "Color32 must store R, G, B, A at byte offsets 0, 1, 2, 3");
}

// Bytes R, G, B, A sit at increasing addresses whatever the byte order of the CPU.
TEST_CASE("video/color32/memory_byte_order") {
  constexpr Color32 color{.r = 0x11, .g = 0x22, .b = 0x33, .a = 0x44};

  const auto bytes = std::bit_cast<std::array<uint8_t, 4>>(color);

  CHECK(bytes[0] == 0x11);
  CHECK(bytes[1] == 0x22);
  CHECK(bytes[2] == 0x33);
  CHECK(bytes[3] == 0x44);

  static_assert(std::bit_cast<std::array<uint8_t, 4>>(color) == std::array<uint8_t, 4>{0x11, 0x22, 0x33, 0x44},
                "Color32 must store R, G, B, A at increasing addresses");
}

// Value initialization zeroes every channel, and designated initializers set the channel they name.
TEST_CASE("video/color32/initialization") {
  constexpr Color32 zero{};
  constexpr Color32 named{.r = 1, .g = 2, .b = 3, .a = 4};

  CHECK(zero.r == 0);
  CHECK(zero.g == 0);
  CHECK(zero.b == 0);
  CHECK(zero.a == 0);
  CHECK(named.r == 1);
  CHECK(named.g == 2);
  CHECK(named.b == 3);
  CHECK(named.a == 4);

  static_assert(zero.r == 0 && zero.g == 0 && zero.b == 0 && zero.a == 0,
                "value initialization must zero every channel");
  static_assert(named.r == 1 && named.g == 2 && named.b == 3 && named.a == 4,
                "designated initializers must set the channel they name");
}

// Two colors are equal only when all four channels match.
TEST_CASE("video/color32/equality") {
  constexpr Color32 base{.r = 10, .g = 20, .b = 30, .a = 40};
  constexpr Color32 same{.r = 10, .g = 20, .b = 30, .a = 40};
  constexpr Color32 otherRed{.r = 11, .g = 20, .b = 30, .a = 40};
  constexpr Color32 otherGreen{.r = 10, .g = 21, .b = 30, .a = 40};
  constexpr Color32 otherBlue{.r = 10, .g = 20, .b = 31, .a = 40};
  constexpr Color32 otherAlpha{.r = 10, .g = 20, .b = 30, .a = 41};

  CHECK(base == same);
  CHECK(base != otherRed);
  CHECK(base != otherGreen);
  CHECK(base != otherBlue);
  CHECK(base != otherAlpha);

  static_assert(base == same, "equal channels must compare equal");
  static_assert(base != otherAlpha, "a different alpha must compare unequal");
}

// The named colors hold the documented channel values.
TEST_CASE("video/color32/named_colors") {
  constexpr Color32 opaqueWhite{.r = 255, .g = 255, .b = 255, .a = 255};
  constexpr Color32 opaqueBlack{.r = 0, .g = 0, .b = 0, .a = 255};

  CHECK(c_white == opaqueWhite);
  CHECK(c_black == opaqueBlack);
  CHECK(c_transparent == Color32{});

  static_assert(c_white == opaqueWhite, "white must be opaque full intensity");
  static_assert(c_black == opaqueBlack, "black must be opaque zero intensity");
  static_assert(c_transparent == Color32{}, "transparent must equal a value-initialized color");
}

} // namespace toy::video
