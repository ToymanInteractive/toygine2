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
  \file   message_box_buttons.hpp
  \brief  Button set configuration for message boxes / dialogs.

  Defines \ref toy::platform::ui::MessageBoxButtons: which combination of buttons to show (e.g. OK only, OK/Cancel,
  Yes/No). The user’s choice is reported as \ref toy::platform::ui::MessageBoxReturn. Used by platform UI APIs that show
  dialogs.

  Included by \ref platform/ui.hpp; do not include this file directly.
*/

#ifndef INCLUDE_PLATFORM_UI_MESSAGE_BOX_BUTTONS_HPP_
#define INCLUDE_PLATFORM_UI_MESSAGE_BOX_BUTTONS_HPP_

namespace toy::platform::ui {

/*!
  \enum MessageBoxButtons
  \brief Which buttons to show on a message box or dialog.

  Platform APIs take a value to configure the button set. Exact layout and labels are platform-dependent; use for
  branching after showing a message box together with \ref toy::platform::ui::MessageBoxReturn.
*/
enum class MessageBoxButtons {
  /// Single OK button.
  Ok,
  /// OK and Cancel.
  OkCancel,
  /// Abort, Retry, and Ignore.
  AbortRetryIgnore,
  /// Yes, No, and Cancel.
  YesNoCancel,
  /// Yes and No.
  YesNo,
  /// Retry and Cancel.
  RetryCancel,
  /// Cancel, Try Again, and Continue.
  CancelTryContinue,
  /// OK and Help.
  OkHelp,
};

} // namespace toy::platform::ui

#endif // INCLUDE_PLATFORM_UI_MESSAGE_BOX_BUTTONS_HPP_
