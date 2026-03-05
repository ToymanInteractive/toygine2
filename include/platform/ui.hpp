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
  \file   ui.hpp
  \brief  Umbrella header for platform UI types.

  Provides \ref toy::platform::ui::Orientation, \ref toy::platform::ui::WindowStyle,
  \ref toy::platform::ui::MessageBoxIcon, \ref toy::platform::ui::MessageBoxButtons,
  \ref toy::platform::ui::MessageBoxReturn, and related UI. Include this header only; do not include internal headers
  (e.g. \c ui/orientation.hpp, \c ui/window_style.hpp) directly.
*/

#ifndef INCLUDE_PLATFORM_UI_HPP_
#define INCLUDE_PLATFORM_UI_HPP_

#include "../core.hpp"

/*!
  \namespace toy::platform::ui
  \brief Platform UI: orientation, window style flags, message box (icon, buttons, return value), and related types.

  \sa toy::platform::ui::Orientation, toy::platform::ui::WindowStyle, toy::platform::ui::MessageBoxIcon,
      toy::platform::ui::MessageBoxButtons, toy::platform::ui::MessageBoxReturn
*/

#include "ui/message_box_buttons.hpp"
#include "ui/message_box_icon.hpp"
#include "ui/message_box_return.hpp"
#include "ui/orientation.hpp"
#include "ui/window_style.hpp"

//----------------------------------------------------------------------------------------------------------------------

#include "ui/orientation.inl"
#include "ui/window_style.inl"

#endif // INCLUDE_PLATFORM_UI_HPP_
