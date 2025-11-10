//
// Copyright (c) 2025 Toyman Interactive
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
  \brief  Inline implementations for CStringView class.
*/

#ifndef INCLUDE_CORE_C_STRING_VIEW_INL_
#define INCLUDE_CORE_C_STRING_VIEW_INL_

namespace toy {

constexpr CStringView::CStringView() noexcept
  : _data(_emptyString) {};

constexpr CStringView::CStringView(const char * string) noexcept
  : _data(string) {
  assert_message(string != nullptr, "C string must not be null");
}

constexpr CStringView & CStringView::operator=(const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  _data = string;

  return *this;
}

constexpr CStringView & CStringView::assign(const CStringView & string) noexcept {
  _data = string._data;

  return *this;
}

constexpr CStringView & CStringView::assign(const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  _data = string;

  return *this;
}

constexpr const char & CStringView::at(size_t offset) const noexcept {
  assert_message(offset < size() || (offset == 0 && empty()), "Offset must be within bounds");

  return _data[offset];
}

constexpr const char & CStringView::operator[](size_t offset) const noexcept {
  assert_message(offset < size() || (offset == 0 && empty()), "Offset must be within bounds");

  return _data[offset];
}

constexpr const char & CStringView::front() const noexcept {
  return _data[0];
}

constexpr const char & CStringView::back() const noexcept {
  assert_message(!empty(), "String view must not be empty");

  return _data[size() - 1];
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

constexpr size_t CStringView::size() const noexcept {
  if consteval {
    return std::char_traits<char>::length(_data);
  } else {
    return std::strlen(_data);
  }
}

inline size_t CStringView::utf8_size() const noexcept {
  return utf8Len(_data);
}

constexpr size_t CStringView::length() const noexcept {
  return size();
}

constexpr size_t CStringView::max_size() const noexcept {
  return size();
}

constexpr size_t CStringView::capacity() const noexcept {
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
constexpr size_t CStringView::find(const stringType & string, size_t position) const noexcept {
  return _find_raw(position, string.c_str(), string.size());
}

constexpr size_t CStringView::find(const char * string, size_t position) const noexcept {
  return find(CStringView(string), position);
}

constexpr size_t CStringView::find(char character, size_t position) const noexcept {
  return _find_raw(position, &character, 1);
}

template <StringLike stringType>
constexpr size_t CStringView::rfind(const stringType & string, size_t position) const noexcept {
  return _rfind_raw(position, string.c_str(), string.size());
}

constexpr size_t CStringView::rfind(const char * string, size_t position) const noexcept {
  return rfind(CStringView(string), position);
}

constexpr size_t CStringView::rfind(char character, size_t position) const noexcept {
  return _rfind_raw(position, &character, 1);
}

template <StringLike stringType>
constexpr size_t CStringView::find_first_of(const stringType & string, size_t position) const noexcept {
  return _find_first_of_raw(position, string.c_str(), string.size());
}

constexpr size_t CStringView::find_first_of(const char * string, size_t position) const noexcept {
  return find_first_of(CStringView(string), position);
}

constexpr size_t CStringView::find_first_of(char character, size_t position) const noexcept {
  return _find_first_of_raw(position, &character, 1);
}

template <StringLike stringType>
constexpr size_t CStringView::find_first_not_of(const stringType & string, size_t position) const noexcept {
  return _find_first_not_of_raw(position, string.c_str(), string.size());
}

constexpr size_t CStringView::find_first_not_of(const char * string, size_t position) const noexcept {
  return find_first_not_of(CStringView(string), position);
}

constexpr size_t CStringView::find_first_not_of(char character, size_t position) const noexcept {
  return _find_first_not_of_raw(position, &character, 1);
}

template <StringLike stringType>
constexpr size_t CStringView::find_last_of(const stringType & string, size_t position) const noexcept {
  return _find_last_of_raw(position, string.c_str(), string.size());
}

constexpr size_t CStringView::find_last_of(const char * string, size_t position) const noexcept {
  return find_last_of(CStringView(string), position);
}

constexpr size_t CStringView::find_last_of(char character, size_t position) const noexcept {
  return _find_last_of_raw(position, &character, 1);
}

template <StringLike stringType>
constexpr size_t CStringView::find_last_not_of(const stringType & string, size_t position) const noexcept {
  return _find_last_not_of_raw(position, string.c_str(), string.size());
}

constexpr size_t CStringView::find_last_not_of(const char * string, size_t position) const noexcept {
  return find_last_not_of(CStringView(string), position);
}

constexpr size_t CStringView::find_last_not_of(char character, size_t position) const noexcept {
  return _find_last_not_of_raw(position, &character, 1);
}

template <StringLike stringType>
constexpr int CStringView::compare(const stringType & string) const noexcept {
  if consteval {
    return cstrcmp(_data, string.c_str());
  } else {
    return std::strcmp(_data, string.c_str());
  }
}

constexpr int CStringView::compare(const char * string) const noexcept {
  return compare(CStringView(string));
}

template <StringLike stringType>
constexpr bool CStringView::starts_with(const stringType & string) const noexcept {
  const auto stringSize = string.size();
  if (size() < stringSize)
    return false;

  if consteval {
    return std::equal(_data, _data + stringSize, string.c_str());
  } else {
    return std::memcmp(_data, string.c_str(), stringSize) == 0;
  }
}

constexpr bool CStringView::starts_with(const char * string) const noexcept {
  return starts_with(CStringView(string));
}

constexpr bool CStringView::starts_with(char character) const noexcept {
  return !empty() && _data[0] == character;
}

template <StringLike stringType>
constexpr bool CStringView::ends_with(const stringType & string) const noexcept {
  const auto stringSize = string.size();
  const auto thisSize = size();
  if (thisSize < stringSize)
    return false;

  if consteval {
    return std::equal(_data + thisSize - stringSize, _data + thisSize, string.c_str());
  } else {
    return std::memcmp(_data + thisSize - stringSize, string.c_str(), stringSize) == 0;
  }
}

constexpr bool CStringView::ends_with(const char * string) const noexcept {
  return ends_with(CStringView(string));
}

constexpr bool CStringView::ends_with(char character) const noexcept {
  return !empty() && _data[size() - 1] == character;
}

template <StringLike stringType>
constexpr bool CStringView::contains(const stringType & string) const noexcept {
  if consteval {
    return cstrstr(_data, string.c_str()) != nullptr;
  } else {
    return std::strstr(_data, string.c_str()) != nullptr;
  }
}

constexpr bool CStringView::contains(const char * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  if consteval {
    return cstrstr(_data, string) != nullptr;
  } else {
    return std::strstr(_data, string) != nullptr;
  }
}

constexpr bool CStringView::contains(char character) const noexcept {
  if (empty())
    return false;

  if consteval {
    return cstrchr(_data, character) != nullptr;
  } else {
    return std::strchr(_data, character) != nullptr;
  }
}

constexpr size_t CStringView::_find_raw(size_t position, const char * data, size_t dataSize) const noexcept {
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

  return occurrence != nullptr ? static_cast<size_t>(occurrence - _data) : npos;
}

constexpr size_t CStringView::_rfind_raw(size_t position, const char * data, size_t dataSize) const noexcept {
  const auto stringViewSize = size();

  if (dataSize == 0)
    return std::min(position, stringViewSize);
  else if (dataSize > stringViewSize)
    return npos;

  if (position == npos)
    position = stringViewSize - dataSize;
  else if (position > stringViewSize - dataSize)
    return npos;

  for (size_t i = 0; i <= position; ++i) {
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

constexpr size_t CStringView::_find_first_of_raw(size_t position, const char * data, size_t dataSize) const noexcept {
  const auto stringViewSize = size();

  if (position >= stringViewSize || dataSize == 0)
    return npos;

  const char * occurrence;

  if consteval {
    occurrence = dataSize == 1 ? cstrchr(_data + position, data[0]) : cstrpbrk(_data + position, data);
  } else {
    occurrence = dataSize == 1 ? std::strchr(_data + position, data[0]) : std::strpbrk(_data + position, data);
  }

  return occurrence != nullptr ? static_cast<size_t>(occurrence - _data) : npos;
}

constexpr size_t CStringView::_find_first_not_of_raw(size_t position, const char * data,
                                                     size_t dataSize) const noexcept {
  const auto stringViewSize = size();

  if (position >= stringViewSize)
    return npos;

  if (dataSize == 0)
    return position;

  if (dataSize == 1) {
    const auto exclude = data[0];
    for (auto i = position; i < stringViewSize; ++i) {
      if (_data[i] != exclude)
        return i;
    }
  } else {
    std::array<bool, 256> excludedChars{};

    for (size_t i = 0; i < dataSize; ++i) {
      excludedChars[static_cast<unsigned char>(data[i])] = true;
    }

    for (auto i = position; i < stringViewSize; ++i) {
      if (!excludedChars[static_cast<unsigned char>(_data[i])])
        return i;
    }
  }

  return npos;
}

constexpr size_t CStringView::_find_last_of_raw(size_t position, const char * data, size_t dataSize) const noexcept {
  if (dataSize == 0 || empty())
    return npos;

  const auto stringViewSize = size();

  if (position == npos)
    position = stringViewSize - 1;
  else if (position >= stringViewSize)
    return npos;

  if (dataSize == 1) {
    const auto target = data[0];
    for (size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (_data[scanIndex] == target)
        return scanIndex;
    }
  } else {
    std::array<bool, 256> targetChars{};

    for (size_t i = 0; i < dataSize; ++i) {
      targetChars[static_cast<unsigned char>(data[i])] = true;
    }

    for (size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (targetChars[static_cast<unsigned char>(_data[scanIndex])])
        return scanIndex;
    }
  }

  return npos;
}

constexpr size_t CStringView::_find_last_not_of_raw(size_t position, const char * data,
                                                    size_t dataSize) const noexcept {
  if (empty())
    return npos;

  const auto stringViewSize = size();

  if (position == npos)
    position = stringViewSize - 1;
  else if (position >= stringViewSize)
    return npos;

  if (dataSize == 0)
    return position;

  if (dataSize == 1) {
    const auto exclude = data[0];
    for (size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (_data[scanIndex] != exclude)
        return scanIndex;
    }
  } else {
    std::array<bool, 256> excludedChars{};

    for (size_t i = 0; i < dataSize; ++i) {
      excludedChars[static_cast<unsigned char>(data[i])] = true;
    }

    for (size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (!excludedChars[static_cast<unsigned char>(_data[scanIndex])])
        return scanIndex;
    }
  }

  return npos;
}

constexpr bool operator==(const CStringView & lhs, const CStringView & rhs) noexcept {
  if (std::addressof(lhs) == std::addressof(rhs) || (lhs.empty() && rhs.empty()))
    return true;

  if consteval {
    return cstrcmp(lhs.c_str(), rhs.c_str()) == 0;
  } else {
    return std::strcmp(lhs.c_str(), rhs.c_str()) == 0;
  }
}

template <StringLike stringType>
constexpr bool operator==(const CStringView & lhs, const stringType & rhs) noexcept {
  if (lhs.c_str() == rhs.c_str() || (lhs.empty() && rhs.size() == 0))
    return true;

  if consteval {
    return cstrcmp(lhs.c_str(), rhs.c_str()) == 0;
  } else {
    return std::strcmp(lhs.c_str(), rhs.c_str()) == 0;
  }
}

template <StringLike stringType>
constexpr bool operator==(const stringType & lhs, const CStringView & rhs) noexcept {
  return rhs == lhs;
}

constexpr bool operator==(const CStringView & lhs, const char * rhs) noexcept {
  assert_message(rhs != nullptr, "C string must not be null");

  return lhs == CStringView(rhs);
}

constexpr bool operator==(const char * lhs, const CStringView & rhs) noexcept {
  assert_message(lhs != nullptr, "C string must not be null");

  return CStringView(lhs) == rhs;
}

constexpr std::strong_ordering operator<=>(const CStringView & lhs, const CStringView & rhs) noexcept {
  if (std::addressof(lhs) == std::addressof(rhs))
    return std::strong_ordering::equal;

  if (lhs.empty() && rhs.empty())
    return std::strong_ordering::equal;
  else if (lhs.empty())
    return std::strong_ordering::less;
  else if (rhs.empty())
    return std::strong_ordering::greater;

  if consteval {
    return cstrcmp(lhs.c_str(), rhs.c_str()) <=> 0;
  } else {
    return std::strcmp(lhs.c_str(), rhs.c_str()) <=> 0;
  }
}

template <StringLike stringType>
constexpr std::strong_ordering operator<=>(const CStringView & lhs, const stringType & rhs) noexcept {
  if (lhs.empty() && rhs.size() == 0)
    return std::strong_ordering::equal;
  else if (lhs.empty())
    return std::strong_ordering::less;
  else if (rhs.size() == 0)
    return std::strong_ordering::greater;

  if consteval {
    return cstrcmp(lhs.c_str(), rhs.c_str()) <=> 0;
  } else {
    return std::strcmp(lhs.c_str(), rhs.c_str()) <=> 0;
  }
}

template <StringLike stringType>
constexpr std::strong_ordering operator<=>(const stringType & lhs, const CStringView & rhs) noexcept {
  return 0 <=> (rhs <=> lhs);
}

constexpr std::strong_ordering operator<=>(const CStringView & lhs, const char * rhs) noexcept {
  return lhs <=> CStringView(rhs);
}

constexpr std::strong_ordering operator<=>(const char * lhs, const CStringView & rhs) noexcept {
  return CStringView(lhs) <=> rhs;
}

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_INL_
