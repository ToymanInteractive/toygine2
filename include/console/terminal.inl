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
  \file   terminal.inl
  \brief  Inline implementations for \ref toy::console::Terminal initialize, write, and the cursor helpers.

  \note Included by console.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CONSOLE_TERMINAL_INL_
#define INCLUDE_CONSOLE_TERMINAL_INL_

namespace toy::console {

template <ConsoleBackend Backend>
void Terminal<Backend>::initialize() noexcept {
  _cursorX = 0;
  _cursorY = 0;

  std::ranges::fill(_buffer, Backend::blankCell());

  _initialized = true;

  Backend::initialize();
}

template <ConsoleBackend Backend>
size_t Terminal<Backend>::columns() noexcept {
  return Backend::columns;
}

template <ConsoleBackend Backend>
size_t Terminal<Backend>::rows() noexcept {
  return Backend::rows;
}

template <ConsoleBackend Backend>
void Terminal<Backend>::setTabSize(size_t tabSize) noexcept {
  assert_message(tabSize > 0, "console::setTabSize: tab size must be positive");

  _tabSize = tabSize;
}

template <ConsoleBackend Backend>
void Terminal<Backend>::write(StringView text) noexcept {
  writeText(text, false);
}

template <ConsoleBackend Backend>
void Terminal<Backend>::writeError(StringView text) noexcept {
  writeText(text, true);
}

template <ConsoleBackend Backend>
void Terminal<Backend>::writeText(StringView text, bool isError) noexcept {
  assert_message(_initialized, "console::write: initialize() has not run");

  for (size_t index = 0; index < text.size(); ++index) {
    const auto character = static_cast<unsigned char>(text[index]);

    if (character == '\b')
      _cursorX = _cursorX > 0 ? _cursorX - 1 : 0;
    else if (character == '\n') {
      _cursorX = 0;
      lineAdvance();
    } else if (character == '\r')
      _cursorX = 0;
    else if (character == '\t')
      _cursorX = min((_cursorX + _tabSize) / _tabSize * _tabSize, Backend::columns - 1);
    else if (character == '\v')
      lineAdvance();
    else if (character >= 0x80)
      writeCharacter(isError ? Backend::encodeErrorCharacter('?') : Backend::encodeCharacter('?'));
    else if (character >= 0x20)
      writeCharacter(isError ? Backend::encodeErrorCharacter(static_cast<char>(character))
                             : Backend::encodeCharacter(static_cast<char>(character)));
    // Control bytes the terminal does not handle are dropped.
  }

  Backend::upload(span<const Cell>(_buffer, Backend::rows * Backend::columns));
}

template <ConsoleBackend Backend>
void Terminal<Backend>::writeCharacter(Cell cell) noexcept {
  if (_cursorX == Backend::columns) {
    _cursorX = 0;
    lineAdvance();
  }

  _buffer[_cursorY * Backend::columns + _cursorX] = cell;
  ++_cursorX;
}

template <ConsoleBackend Backend>
void Terminal<Backend>::lineAdvance() noexcept {
  ++_cursorY;

  if (_cursorY == Backend::rows) {
    std::memmove(_buffer, _buffer + Backend::columns, (Backend::rows - 1) * Backend::columns * sizeof(Cell));
    std::ranges::fill(_buffer + (Backend::rows - 1) * Backend::columns, _buffer + Backend::rows * Backend::columns,
                      Backend::blankCell());
    _cursorY = Backend::rows - 1;
  }
}

} // namespace toy::console

#endif // INCLUDE_CONSOLE_TERMINAL_INL_
