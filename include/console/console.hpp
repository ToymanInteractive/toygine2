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
  \file   console.hpp
  \brief  Public entry points of the engine console module.

  Declares the screen TTY API: toy::console::initialize(), toy::console::write(), and toy::console::setTabSize().
  The definitions come from the target platform's backend; a platform without a backend leaves the symbols
  undefined at link time.
*/

#ifndef INCLUDE_CONSOLE_CONSOLE_HPP_
#define INCLUDE_CONSOLE_CONSOLE_HPP_

#include <core.hpp>

namespace toy::console {

/*!
  \brief Brings the screen to the console state and clears it.

  \post The screen is empty and the cursor is at the origin; a repeated call is equivalent to clearing the screen.

  \sa toy::console::write()
*/
void initialize() noexcept;

/*!
  \brief Prints \a text to the screen, interpreting the terminal control bytes.

  Handles \c '\\b', \c '\\n', \c '\\r', \c '\\t', and \c '\\v', wraps at the last column, and scrolls when the last
  row fills. Bytes \c 0x20..\c 0x7E render as glyphs, bytes at or above \c 0x80 render as \c '?', and other control
  bytes are dropped. The screen reflects the text when the call returns.

  \param text Bytes to print; need not be null-terminated.

  \pre initialize() has run; checked by assert_message in debug builds.
  \post The screen reflects \a text.

  \sa toy::console::initialize()
*/
void write(StringView text) noexcept;

/*!
  \brief Sets the tab stop interval for subsequent write() calls.

  \param tabSize Columns between tab stops; \c 2 by default.

  \post Tab stops sit at multiples of \a tabSize, clamped to the last column.

  \sa toy::console::write()
*/
void setTabSize(size_t tabSize) noexcept;

} // namespace toy::console

#endif // INCLUDE_CONSOLE_CONSOLE_HPP_
