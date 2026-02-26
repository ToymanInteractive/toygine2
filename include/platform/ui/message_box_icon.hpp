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
  \file   message_box_icon.hpp
  \brief  Icon type for message box / dialog.

  Defines \ref toy::platform::ui::MessageBoxIcon: which icon is displayed in a message box (none, error, question,
  warning, information). Used by platform UI APIs that show dialogs.
*/

#ifndef INCLUDE_PLATFORM_UI_MESSAGE_BOX_ICON_HPP_
#define INCLUDE_PLATFORM_UI_MESSAGE_BOX_ICON_HPP_

namespace toy::platform::ui {

/*!
  \enum MessageBoxIcon
  \brief Icon shown in a message box or dialog.

  Used when displaying a native message box to indicate severity or kind of message. Exact appearance is
  platform-dependent.
*/
enum class MessageBoxIcon {
  /// No icon.
  None,
  /// Error icon (e.g. stop/critical).
  Error,
  /// Question icon (e.g. help/inquiry).
  Question,
  /// Warning icon (e.g. caution).
  Warning,
  /// Information icon (e.g. info).
  Information,
};

} // namespace toy::platform::ui

#endif // INCLUDE_PLATFORM_UI_MESSAGE_BOX_ICON_HPP_
