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
  \file   pixel_format.test.cpp
  \brief  Unit tests for \ref toy::render::PixelFormat name mapping.
*/

#include <doctest/doctest.h>

#include "render.hpp"

namespace toy::render {

// pixelFormatName returns canonical string for each format.
TEST_CASE("render/pixel_format/format_name") {
  // known colour format
  SUBCASE("colour_format") {
    REQUIRE(pixelFormatName(PixelFormat::A8R8G8B8) == "A8R8G8B8");
    REQUIRE(pixelFormatName(PixelFormat::R5G6B5) == "R5G6B5");
    REQUIRE(pixelFormatName(PixelFormat::A8) == "A8");
  }

  // compressed formats
  SUBCASE("compressed_formats") {
    REQUIRE(pixelFormatName(PixelFormat::S3TC1) == "S3TC1");
    REQUIRE(pixelFormatName(PixelFormat::PVRTC4) == "PVRTC4");
  }

  // depth/stencil formats
  SUBCASE("depth_stencil_formats") {
    REQUIRE(pixelFormatName(PixelFormat::D24S8) == "D24S8");
    REQUIRE(pixelFormatName(PixelFormat::D16) == "D16");
  }

  // Unknown yields "Unknown"
  SUBCASE("unknown_format") {
    REQUIRE(pixelFormatName(PixelFormat::Unknown) == "Unknown");
  }
}

// pixelFormatFromName parses name and returns matching format.
TEST_CASE("render/pixel_format/format_from_name") {
  // known names return correct format
  SUBCASE("known_names") {
    REQUIRE_EQ(pixelFormatFromName("A8R8G8B8"), PixelFormat::A8R8G8B8);
    REQUIRE_EQ(pixelFormatFromName("D24S8"), PixelFormat::D24S8);
    REQUIRE_EQ(pixelFormatFromName("S3TC5"), PixelFormat::S3TC5);
  }

  // "Unknown" string returns Unknown enum
  SUBCASE("unknown_string") {
    REQUIRE(pixelFormatFromName("Unknown") == PixelFormat::Unknown);
  }

  // empty or invalid name returns Unknown
  SUBCASE("empty_or_invalid") {
    REQUIRE(pixelFormatFromName(CStringView()) == PixelFormat::Unknown);
    REQUIRE(pixelFormatFromName("") == PixelFormat::Unknown);
    REQUIRE(pixelFormatFromName("InvalidFormat") == PixelFormat::Unknown);
  }

  // case-sensitive: wrong case yields Unknown
  SUBCASE("case_sensitive") {
    REQUIRE(pixelFormatFromName("a8r8g8b8") == PixelFormat::Unknown);
    REQUIRE(pixelFormatFromName("D24s8") == PixelFormat::Unknown);
  }
}

// round-trip: name(format) then fromName(name) yields same format.
TEST_CASE("render/pixel_format/round_trip") {
  // colour and depth formats round-trip
  SUBCASE("colour_and_depth") {
    REQUIRE(pixelFormatFromName(pixelFormatName(PixelFormat::X8R8G8B8)) == PixelFormat::X8R8G8B8);
    REQUIRE(pixelFormatFromName(pixelFormatName(PixelFormat::D24X8)) == PixelFormat::D24X8);
  }

  // compressed formats round-trip
  SUBCASE("compressed") {
    REQUIRE(pixelFormatFromName(pixelFormatName(PixelFormat::S3TC1)) == PixelFormat::S3TC1);
    REQUIRE(pixelFormatFromName(pixelFormatName(PixelFormat::PVRTC2)) == PixelFormat::PVRTC2);
  }

  // Unknown round-trips to Unknown
  SUBCASE("unknown") {
    REQUIRE(pixelFormatFromName(pixelFormatName(PixelFormat::Unknown)) == PixelFormat::Unknown);
  }
}

} // namespace toy::render
