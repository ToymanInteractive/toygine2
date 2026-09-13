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
  \file   line_buffer.inl
  \brief  Inline implementations for \ref toy::benchmark::detail::LineBuffer.

  \note Included by line_buffer.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_LINE_BUFFER_INL_
#define INCLUDE_BENCHMARKS_RUNNER_LINE_BUFFER_INL_

namespace toy::benchmark::detail {

inline LineBuffer::LineBuffer(const Output & output) noexcept
  : _output{output} {}

inline void LineBuffer::addText(const char * text) noexcept {
  _length = appendText(_buffer, _length, text);
}

inline void LineBuffer::addField(const char * text) noexcept {
  bool quoted = false;

  for (const char * character = text; *character != '\0' && !quoted; ++character)
    quoted = *character == ',' || *character == '"' || *character == '\t';

  if (quoted)
    _length = appendText(_buffer, _length, "\"");

  for (const char * character = text; *character != '\0'; ++character) {
    // A line break would split the row across two callbacks, and the host parses one line at a time.
    const char escaped[2] = {*character == '\n' || *character == '\r' ? ' ' : *character, '\0'};

    if (escaped[0] == '"')
      _length = appendText(_buffer, _length, "\"");

    _length = appendText(_buffer, _length, escaped);
  }

  if (quoted)
    _length = appendText(_buffer, _length, "\"");
}

inline void LineBuffer::addDecimal(std::uint64_t value) noexcept {
  _length = appendDecimal(_buffer, _length, value);
}

inline void LineBuffer::flush() noexcept {
  // The newline owns the last byte, so a cut line still ends where the receiver expects one.
  const auto newline = _length < c_capacity ? _length : c_capacity - 1;

  _buffer[newline] = '\n';
  _output.write(_buffer.data(), newline + 1, _output.writerData);
  _length = 0;
}

} // namespace toy::benchmark::detail

#endif // INCLUDE_BENCHMARKS_RUNNER_LINE_BUFFER_INL_
