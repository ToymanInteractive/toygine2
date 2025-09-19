//
// Copyright (c) 2025 by Toyman Interactive
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
  \file   c_string_view.inl
  \brief  TODO
*/

#ifndef INCLUDE_CORE_C_STRING_VIEW_INL_
#define INCLUDE_CORE_C_STRING_VIEW_INL_

namespace toy {

constexpr CStringView::CStringView() noexcept
  : _data(_emptyString) {};

constexpr CStringView::CStringView(const char * string) noexcept
  : _data(string) {
  assert_message(string != nullptr, "String pointer must not be null");
}

constexpr CStringView & CStringView::operator=(const char * string) noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  _data = string;

  return *this;
}

constexpr CStringView & CStringView::assign(const CStringView & string) noexcept {
  _data = string._data;

  return *this;
}

constexpr CStringView & CStringView::assign(const char * string) noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  _data = string;

  return *this;
}

constexpr const char & CStringView::at(std::size_t offset) const noexcept {
  assert_message(offset < size() || (offset == 0 && empty()), "Offset must be within bounds");

  return _data[offset];
}

constexpr const char & CStringView::operator[](std::size_t offset) const noexcept {
  assert_message(offset < size() || (offset == 0 && empty()), "Offset must be within bounds");

  return _data[offset];
}

constexpr const char & CStringView::front() const noexcept {
  return _data[0];
}

constexpr const char & CStringView::back() const noexcept {
  const auto dataSize = size();

  assert_message(dataSize > 0, "String must not be empty");

  return _data[dataSize - 1];
}

constexpr const char * CStringView::data() const noexcept {
  return _data;
}

constexpr const char * CStringView::c_str() const noexcept {
  return _data;
}

constexpr bool CStringView::empty() const noexcept {
  return *_data == '\0';
}

constexpr std::size_t CStringView::size() const noexcept {
  if consteval {
    return std::char_traits<char>::length(_data);
  } else {
    return std::strlen(_data);
  }
}

inline std::size_t CStringView::utf8_size() const noexcept {
  return utf8Len(_data);
}

constexpr std::size_t CStringView::length() const noexcept {
  return size();
}

constexpr std::size_t CStringView::max_size() const noexcept {
  return size();
}

constexpr std::size_t CStringView::capacity() const noexcept {
  return size();
}

constexpr void CStringView::clear() noexcept {
  _data = _emptyString;
}

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_INL_
