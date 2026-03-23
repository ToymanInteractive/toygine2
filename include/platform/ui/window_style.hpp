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
  \file   window_style.hpp
  \brief  Bitmask enumeration for window decoration and mode.

  Defines \ref toy::platform::ui::WindowStyle: flags for title bar buttons, popup/topmost/fullscreen behavior, and
  resize state. Values can be combined with operator&, operator|, operator^.

  \note Included by platform/ui.hpp; do not include this file directly.
*/

#ifndef INCLUDE_PLATFORM_UI_WINDOW_STYLE_HPP_
#define INCLUDE_PLATFORM_UI_WINDOW_STYLE_HPP_

namespace toy::platform::ui {

/*!
  \enum WindowStyle
  \brief Bitmask flags for window decoration, modality, and display mode.

  Values can be combined (e.g. \c Close | \c MinMax for a standard title bar). Used when creating or configuring
  platform windows.

  \sa operator&, operator|, operator^
*/
enum class WindowStyle : uint8_t {
  /// No decoration (plain window).
  Simple = 0x00,

  /// Close button on title bar.
  Close = 0x01,

  /// Minimize and maximize buttons.
  MinMax = 0x02,

  /// Popup/modal behavior.
  Popup = 0x04,

  /// Window stays above others.
  TopMost = 0x08,

  /// Full-screen mode.
  FullScreen = 0x10,

  /// Window has been resized.
  Resized = 0x20
};

} // namespace toy::platform::ui

ENABLE_BITWISE_OPERATORS(toy::platform::ui::WindowStyle)

#endif // INCLUDE_PLATFORM_UI_WINDOW_STYLE_HPP_
