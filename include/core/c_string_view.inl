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

constexpr void CStringView::swap(CStringView & string) noexcept {
  if (this != std::addressof(string))
    std::swap(_data, string._data);
}

template <StringLike stringType>
constexpr std::size_t CStringView::find(const stringType & string, std::size_t position) const noexcept {
  return _find_raw(position, string.c_str(), string.size());
}

constexpr std::size_t CStringView::find(const char * string, std::size_t position) const noexcept {
  return find(CStringView(string), position);
}

constexpr std::size_t CStringView::find(char character, std::size_t position) const noexcept {
  return _find_raw(position, &character, 1);
}

template <StringLike stringType>
constexpr std::size_t CStringView::rfind(const stringType & string, std::size_t position) const noexcept {
  return _rfind_raw(position, string.c_str(), string.size());
}

constexpr std::size_t CStringView::rfind(const char * string, std::size_t position) const noexcept {
  return rfind(CStringView(string), position);
}

constexpr std::size_t CStringView::rfind(char character, std::size_t position) const noexcept {
  return _rfind_raw(position, &character, 1);
}

template <StringLike stringType>
constexpr std::size_t CStringView::find_first_of(const stringType & string, std::size_t position) const noexcept {
  return _find_first_of_raw(position, string.c_str(), string.size());
}

constexpr std::size_t CStringView::find_first_of(const char * string, std::size_t position) const noexcept {
  return find_first_of(CStringView(string), position);
}

constexpr std::size_t CStringView::find_first_of(char character, std::size_t position) const noexcept {
  return _find_first_of_raw(position, &character, 1);
}

constexpr std::size_t CStringView::_find_raw(std::size_t position, const char * data,
                                             std::size_t dataSize) const noexcept {
  const auto stringViewSize = size();

  if (position > stringViewSize)
    return npos;

  if (dataSize == 0)
    return position;
  else if (dataSize > stringViewSize - position)
    return npos;

  const char * occurrence;

  if consteval {
    occurrence = dataSize == 1 ? cstrchr(_data + position, data[0]) : cstrstr(_data + position, data);
  } else {
    occurrence = dataSize == 1
                   ? static_cast<const char *>(std::memchr(_data + position, data[0], stringViewSize - position))
                   : std::strstr(_data + position, data);
  }

  return occurrence != nullptr ? static_cast<std::size_t>(occurrence - _data) : npos;
}

constexpr std::size_t CStringView::_rfind_raw(std::size_t position, const char * data,
                                              std::size_t dataSize) const noexcept {
  const auto stringViewSize = size();

  if (dataSize == 0)
    return std::min(position, stringViewSize);
  else if (dataSize > stringViewSize)
    return npos;

  if (position == npos)
    position = stringViewSize - dataSize;
  else if (position > stringViewSize - dataSize)
    return npos;

  for (std::size_t i = 0; i <= position; ++i) {
    const auto offset = position - i;

    bool found;

    if consteval {
      found = std::equal(_data + offset, _data + offset + dataSize, data);
    } else {
      found = std::memcmp(_data + offset, data, dataSize) == 0;
    }

    if (found)
      return offset;
  }

  return npos;
}

constexpr std::size_t CStringView::_find_first_of_raw(std::size_t position, const char * data,
                                                      std::size_t dataSize) const noexcept {
  const auto stringViewSize = size();

  if (position >= stringViewSize || dataSize == 0)
    return npos;

  const char * occurrence;

  if consteval {
    occurrence = dataSize == 1 ? cstrchr(_data + position, data[0]) : cstrpbrk(_data + position, data);
  } else {
    occurrence = dataSize == 1 ? std::strchr(_data + position, data[0]) : std::strpbrk(_data + position, data);
  }

  return occurrence != nullptr ? static_cast<std::size_t>(occurrence - _data) : npos;
}

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_INL_
