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
  \file   c_string.inl
  \brief  TODO
*/

#ifndef INCLUDE_CORE_C_STRING_INL_
#define INCLUDE_CORE_C_STRING_INL_

namespace toy {

constexpr CString::CString() noexcept
  : _data(_emptyString) {};

constexpr CString::CString(const CString & string) noexcept
  : _data(string._data) {}

constexpr CString::CString(const char * string) noexcept
  : _data(string) {
  assert_message(string != nullptr, "String pointer must not be null");
}

constexpr CString & CString::operator=(const CString & string) noexcept {
  _data = string._data;

  return *this;
}

constexpr CString & CString::operator=(const char * string) noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  _data = string;

  return *this;
}

constexpr CString & CString::assign(const CString & string) noexcept {
  _data = string._data;

  return *this;
}

constexpr CString & CString::assign(const char * string) noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  _data = string;

  return *this;
}

constexpr const char & CString::at(std::size_t offset) const noexcept {
  assert_message(offset < size() || (offset == 0 && size() == 0), "Offset must be within bounds");

  return _data[offset];
}

constexpr const char & CString::operator[](std::size_t offset) const noexcept {
  assert_message(offset < size() || (offset == 0 && size() == 0), "Offset must be within bounds");

  return _data[offset];
}

constexpr const char & CString::front() const noexcept {
  return _data[0];
}

constexpr const char & CString::back() const noexcept {
  const auto dataSize = size();

  assert_message(dataSize > 0, "String must not be empty");

  return _data[dataSize - 1];
}

constexpr const char * CString::data() const noexcept {
  return _data;
}

constexpr const char * CString::c_str() const noexcept {
  return _data;
}

constexpr std::size_t CString::size() const noexcept {
  if consteval {
    return std::char_traits<char>::length(_data);
  } else {
    return std::strlen(_data);
  }
}

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_INL_
