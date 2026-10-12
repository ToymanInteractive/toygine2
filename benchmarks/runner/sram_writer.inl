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
  \file   sram_writer.inl
  \brief  Inline implementations for \ref toy::benchmark::detail::SramWriter.

  \note Included by sram_writer.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_SRAM_WRITER_INL_
#define INCLUDE_BENCHMARKS_RUNNER_SRAM_WRITER_INL_

namespace toy::benchmark::detail {

inline SramWriter::SramWriter(std::span<volatile std::uint8_t> memory, std::size_t stride) noexcept
  : _memory{memory}
  , _stride{stride}
  , _capacity{stride == 0 ? 0 : (memory.size() + stride - 1) / stride} {
  assert_message(stride != 0, "a zero stride would put every character into the same byte");
  assert_message(_capacity > _truncatedLength, "the window must hold the truncation comment and its zero");

  _put(0, '\0');
}

inline void SramWriter::write(const char * text, std::size_t length) noexcept {
  if (_truncated)
    return;

  // Every line leaves room for the truncation comment, so the report can always end on it.
  if (_length + length + _truncatedLength + 1 > _capacity) {
    text       = _truncatedLine;
    length     = _truncatedLength;
    _truncated = true;
  }

  for (std::size_t index = 0; index < length; ++index)
    _put(_length + index, text[index]);

  _length += length;
  _put(_length, '\0');
}

inline void SramWriter::_put(std::size_t index, char character) noexcept {
  _memory[index * _stride] = static_cast<std::uint8_t>(character);
}

} // namespace toy::benchmark::detail

#endif // INCLUDE_BENCHMARKS_RUNNER_SRAM_WRITER_INL_
