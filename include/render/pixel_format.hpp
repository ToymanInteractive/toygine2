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
  \file   pixel_format.hpp
  \brief  Pixel format enumeration for render targets and textures.

  Defines \ref toy::render::PixelFormat: colour, compressed, and depth/stencil formats
  used by the rendering API. Used when creating surfaces or specifying buffer layout.
*/

#ifndef INCLUDE_RENDER_PIXEL_FORMAT_HPP_
#define INCLUDE_RENDER_PIXEL_FORMAT_HPP_

namespace toy::render {

/*!
  \enum PixelFormat
  \brief Pixel or surface format for colour, compression, or depth/stencil.

  Covers common formats: 32-bit ARGB/XRGB, 16-bit, alpha-only, block-compressed (S3TC, PVRTC), and depth/stencil. Exact
  interpretation is backend-dependent.
*/
enum class PixelFormat : uint8_t {
  /// 32-bit ARGB (8 bits per channel, alpha premultiplied or straight).
  A8R8G8B8,
  /// 32-bit XRGB (8 bits per channel; alpha unused).
  X8R8G8B8,

  /// 16-bit ARGB (4 bits per channel).
  A4R4G4B4,
  /// 16-bit RGB (5-6-5).
  R5G6B5,

  /// 8-bit alpha only.
  A8,

  /// S3TC/BC1 block-compressed (e.g. DXT1).
  S3TC1,
  /// S3TC/BC3 block-compressed (e.g. DXT5).
  S3TC5,
  /// PVRTC 2 bpp.
  PVRTC2,
  /// PVRTC 4 bpp.
  PVRTC4,

  /// 16-bit lockable depth.
  D16Lockable,
  /// 32-bit depth.
  D32,
  /// 15-bit depth, 1-bit stencil.
  D15S1,
  /// 24-bit depth, 8-bit stencil.
  D24S8,
  /// 24-bit depth (stencil unused).
  D24X8,
  /// 24-bit depth, 4-bit stencil.
  D24X4S4,
  /// 16-bit depth.
  D16,

  /// Format not specified or unknown.
  Unknown,
};

/*!
  \brief Returns the canonical string name for a pixel format.

  \param format The format to name.

  \return A non-null view of the format name (e.g. "A8R8G8B8", "D24S8"). For \ref toy::render::PixelFormat::Unknown
          returns "Unknown".
*/
[[nodiscard]] CStringView pixelFormatName(PixelFormat format) noexcept;

/*!
  \brief Parses a format name and returns the corresponding \ref toy::render::PixelFormat.

  \param name The format name (case-sensitive). Empty or unknown names yield \ref toy::render::PixelFormat::Unknown.

  \return The matching format, or \ref toy::render::PixelFormat::Unknown if no format matches.
*/
[[nodiscard]] PixelFormat pixelFormatFromName(CStringView name) noexcept;

} // namespace toy::render

#endif // INCLUDE_RENDER_PIXEL_FORMAT_HPP_
