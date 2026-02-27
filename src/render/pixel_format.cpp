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
  \file   pixel_format.cpp
  \brief  Implementation of pixelFormatName() and pixelFormatFromName().
*/

#include "render.hpp"

namespace toy::render {

static constexpr array<CStringView, std::to_underlying(PixelFormat::Unknown)> _formatNames{{
  CStringView("A8R8G8B8"),
  CStringView("X8R8G8B8"),

  CStringView("A4R4G4B4"),
  CStringView("R5G6B5"),

  CStringView("A8"),

  CStringView("S3TC1"),
  CStringView("S3TC5"),
  CStringView("PVRTC2"),
  CStringView("PVRTC4"),

  CStringView("D16Lockable"),
  CStringView("D32"),
  CStringView("D15S1"),
  CStringView("D24S8"),
  CStringView("D24X8"),
  CStringView("D24X4S4"),
  CStringView("D16"),
}};

CStringView pixelFormatName(PixelFormat format) noexcept {
  if (const auto index = std::to_underlying(format); index < _formatNames.size())
    return _formatNames[index];

  return "Unknown";
}

PixelFormat pixelFormatFromName(CStringView name) noexcept {
  for (size_t index = 0; index < _formatNames.size(); ++index) {
    if (_formatNames[index] == name)
      return static_cast<PixelFormat>(index);
  }

  return PixelFormat::Unknown;
}

} // namespace toy::render
