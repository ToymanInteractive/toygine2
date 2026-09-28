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
  \file   color_32.hpp
  \brief  Color with four 8-bit channels in the byte order R, G, B, A.

  Defines \ref toy::video::Color32 and the named colors built on it. Used for vertex colors, texels and tints the CPU
  writes, and as the source the packed console formats are converted from.

  \note Included by video.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_VIDEO_COLOR_32_HPP_
#define INCLUDE_VIDEO_COLOR_32_HPP_

namespace toy::video {

/*!
  \class Color32
  \brief Color with four 8-bit channels, stored in memory as R, G, B, A on every target.

  An array copies as is into a texture or vertex buffer whose format stores 8-bit channels in the same order; any other
  channel order or width needs a conversion first. The channels hold the values as given; the resource format decides
  whether they read as linear or sRGB-encoded.

  \section color32_features Key Features

  * **Fixed layout**: bytes R, G, B, A at offsets 0 to 3 on little- and big-endian CPUs alike.
  * **Aggregate**: designated initializers name each channel at the call site.
  * **Trivial**: an array of pixels or vertices is not zeroed on construction and copies as bytes.
  * **Constexpr support**: construction and comparison evaluate in a constant expression.

  \section color32_usage Usage Example

  \code
  #include "video.hpp"

  constexpr toy::video::Color32 orange{.r = 255, .g = 128, .b = 0, .a = 255};
  static_assert(orange != toy::video::c_white);
  \endcode

  \section color32_performance Performance Characteristics

  * **Construction**: O(1); \c Color32{} writes four zero bytes, a declaration without braces writes nothing.
  * **Comparison**: O(1); the 4-byte alignment lets the compiler compare one 32-bit word.
  * **Memory usage**: 4 bytes, aligned to 4.

  \section color32_safety Safety Guarantees

  * **Initialization**: a declaration without braces leaves the channels indeterminate, as for \c int.
  * **Memory safety**: holds no pointer and allocates nothing.
  * **Exception safety**: No operation throws; exceptions are off in the build.

  \note A vertex format places a \ref toy::video::Color32 only at an offset that is a multiple of 4.
*/
class alignas(4) Color32 {
public:
  uint8_t r; ///< Red channel.
  uint8_t g; ///< Green channel.
  uint8_t b; ///< Blue channel.
  uint8_t a; ///< Alpha channel; \c 255 is opaque, \c 0 transparent.

  /*!
    \brief Compares all four channels.

    \param other Color to compare with.

    \return \c true when every channel of \a other equals the matching channel of this color.
  */
  [[nodiscard]] constexpr bool operator==(const Color32 & other) const noexcept = default;
};

/// Opaque white.
inline constexpr Color32 c_white{.r = 255, .g = 255, .b = 255, .a = 255};

/// Opaque black.
inline constexpr Color32 c_black{.r = 0, .g = 0, .b = 0, .a = 255};

/// Transparent black; equal to \c Color32{}.
inline constexpr Color32 c_transparent{.r = 0, .g = 0, .b = 0, .a = 0};

} // namespace toy::video

#endif // INCLUDE_VIDEO_COLOR_32_HPP_
