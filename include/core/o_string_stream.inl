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
  std::swap(_string, other._string);
}

template <typename StringType>
constexpr const StringType & OStringStream<StringType>::str() const noexcept {
  return _string;
}

template <typename StringType>
template <StringLike SourceStringType>
constexpr void OStringStream<StringType>::str(const SourceStringType & string) noexcept {
  _string = string;
}

template <typename StringType>
constexpr CStringView OStringStream<StringType>::view() const noexcept {
  return CStringView(_string.c_str());
}

template <typename StringType>
inline OStringStream<StringType> & OStringStream<StringType>::put(char_type character) noexcept {
  _string.push_back(character);

  return *this;
}

template <typename StringType>
inline OStringStream<StringType> & OStringStream<StringType>::write(const char_type * string, size_t count) noexcept {
  assert_message(string != nullptr, "Source string must not be null.");
  if (string == nullptr || count == 0)
    return *this;

  for (size_t i = 0; i < count; ++i)
    _string.push_back(string[i]);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType>::pos_type OStringStream<StringType>::tellp() const noexcept {
  return _string.size();
}

} // namespace toy

#endif // INCLUDE_CORE_O_STRING_STREAM_INL_
