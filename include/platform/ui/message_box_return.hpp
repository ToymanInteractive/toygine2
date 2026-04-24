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
  \file   message_box_return.hpp
  \brief  Return value for message box / dialog interactions.

  Defines \ref toy::platform::ui::MessageBoxReturn: which button or action the user chose when dismissing a message box.
  Used by platform UI APIs that show dialogs.

  \note Included by platform/ui.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_PLATFORM_UI_MESSAGE_BOX_RETURN_HPP_
#define INCLUDE_PLATFORM_UI_MESSAGE_BOX_RETURN_HPP_

namespace toy::platform::ui {

/*!
  \enum MessageBoxReturn
  \brief User choice when dismissing a message box or dialog.

  Maps to common dialog button results (OK, Cancel, Yes/No, Abort/Retry/Ignore, etc.). Exact semantics are
  platform-dependent; use for branching after showing a message box.
*/
enum class MessageBoxReturn {
  /// User confirmed (OK button).
  Ok,
  /// User cancelled the dialog.
  Cancel,
  /// User chose Abort (e.g. Abort/Retry/Ignore).
  Abort,
  /// User chose Retry.
  Retry,
  /// User chose Ignore.
  Ignore,
  /// User chose Yes.
  Yes,
  /// User chose No.
  No,
  /// User closed the dialog (e.g. window close).
  Close,
  /// User requested help.
  Help,
  /// User chose Try Again.
  TryAgain,
  /// User chose Continue.
  Continue,
};

} // namespace toy::platform::ui

#endif // INCLUDE_PLATFORM_UI_MESSAGE_BOX_RETURN_HPP_
