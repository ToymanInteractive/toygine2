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
  \file   format_fixed_sink.inl
  \brief  Inline implementations for the write() and put() methods of \ref toy::detail::FormatFixedSink.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_FIXED_SINK_INL_
#define INCLUDE_CORE_DETAIL_FORMAT_FIXED_SINK_INL_

namespace toy::detail {

constexpr void FormatFixedSink::write(span<const char> data) noexcept {
  const size_t remaining  = capacity - size;
  const size_t clamped    = min(data.size(), remaining);
  overflow               |= clamped != data.size();

  for (size_t index = 0; index < clamped; ++index)
    out[size++] = data[index];
}

constexpr void FormatFixedSink::put(char character) noexcept {
  write(span{&character, 1});
}

} // namespace toy::detail

#endif // INCLUDE_CORE_DETAIL_FORMAT_FIXED_SINK_INL_
