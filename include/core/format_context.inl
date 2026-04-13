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
  \file   format_context.inl
  \brief  Inline implementations for \ref toy::FormatContext.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_CONTEXT_INL_
#define INCLUDE_CORE_FORMAT_CONTEXT_INL_

namespace toy {

template <OStringStreamBackend BackendType>
constexpr FormatContext<BackendType>::FormatContext(BackendType & output) noexcept
  : _output(output) {}

template <OStringStreamBackend BackendType>
constexpr void FormatContext<BackendType>::write(const char * data, size_t count) noexcept {
  assert_message((data != nullptr) || (count == 0), "FormatContext::write expects non-null data when count > 0");
  if (count == 0U)
    return;

  _output.append(data, count);
}

template <OStringStreamBackend BackendType>
constexpr void FormatContext<BackendType>::put(char character) noexcept {
  _output.push_back(character);
}

template <OStringStreamBackend BackendType>
constexpr BackendType & FormatContext<BackendType>::backend() noexcept {
  return _output;
}

template <OStringStreamBackend BackendType>
constexpr const BackendType & FormatContext<BackendType>::backend() const noexcept {
  return _output;
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_CONTEXT_INL_
