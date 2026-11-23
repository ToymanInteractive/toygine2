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
  \file   format_context.inl
  \brief  Inline implementations for \ref toy::detail::FormatContext constructor, write(), and put().

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_CONTEXT_INL_
#define INCLUDE_CORE_DETAIL_FORMAT_CONTEXT_INL_

namespace toy::detail {

constexpr FormatContext::FormatContext(void * context, WriteFunction writeFn) noexcept
  : _context(context)
  , _writeFn(writeFn) {
  assert_message(writeFn != nullptr, "FormatContext requires a non-null write callback");
}

constexpr void FormatContext::write(span<const char> data) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to write");

  if (data.empty())
    return;

  _writeFn(_context, data);
}

constexpr void FormatContext::put(char character) noexcept {
  _writeFn(_context, span{&character, 1});
}

} // namespace toy::detail

#endif // INCLUDE_CORE_DETAIL_FORMAT_CONTEXT_INL_
