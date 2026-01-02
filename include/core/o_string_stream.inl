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
  \file   o_string_stream.inl
  \brief  TODO
*/

#ifndef INCLUDE_CORE_O_STRING_STREAM_INL_
#define INCLUDE_CORE_O_STRING_STREAM_INL_

namespace toy {

template <typename StringType>
template <StringLike SourceStringType>
constexpr OStringStream<StringType>::OStringStream(const SourceStringType & string) noexcept
  : _string(string) {}

template <typename StringType>
constexpr void OStringStream<StringType>::swap(OStringStream & other) noexcept {
  if (this != std::addressof(other))
    std::swap(_string, other._string);
}

template <typename StringType>
constexpr const StringType & OStringStream<StringType>::str() const noexcept {
  return _string;
}

} // namespace toy

#endif // INCLUDE_CORE_O_STRING_STREAM_INL_
