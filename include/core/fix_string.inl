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
  \file   fix_string.inl
  \brief  Inline implementations for FixString template class
*/

#ifndef INCLUDE_CORE_FIX_STRING_INL_
#define INCLUDE_CORE_FIX_STRING_INL_

namespace toygine {
template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString() noexcept
  : _size(0)
  , _data{'\0'} {}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::~FixString() noexcept {}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString(const FixString<allocatedSize> & string) noexcept
  : _size(string.size()) {
  if consteval {
    std::copy_n(string.data(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.data(), _size + 1);
  }
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize>::FixString(const stringType & string) noexcept
  : _size(string.size()) {
  assert_message(_size < allocatedSize, "String size must not exceed capacity");

  if consteval {
    std::copy_n(string.c_str(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.c_str(), _size + 1);
  }
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString(const char * string) noexcept
  : _size(0) {
  assert_message(string != nullptr, "String pointer must not be null");

  if consteval {
    _size = std::char_traits<char>::length(string);
  } else {
    _size = std::strlen(string);
  }

  assert_message(_size < allocatedSize, "String length must not exceed capacity");

  if consteval {
    std::copy_n(string, _size + 1, _data);
  } else {
    std::memcpy(_data, string, _size + 1);
  }
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString(char character, std::size_t count) noexcept
  : _size(count) {
  assert_message(count < allocatedSize, "Count must not exceed capacity");

  if consteval {
    std::fill_n(_data, _size, character);
  } else {
    std::memset(_data, character, _size);
  }
  _data[_size] = '\0';
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(
  const FixString<allocatedSize> & string) noexcept {
  if (this == &string)
    return *this;

  _size = string.size();
  if consteval {
    std::copy_n(string.data(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.data(), _size + 1);
  }

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(const stringType & string) noexcept {
  if (_data == string.c_str())
    return *this;

  assert_message(string.size() < allocatedSize, "String size must not exceed capacity");

  _size = string.size();
  if consteval {
    std::copy_n(string.c_str(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.c_str(), _size + 1);
  }

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(const char * string) noexcept {
  if (_data == string)
    return *this;

  assert_message(string != nullptr, "String pointer must not be null");

  if consteval {
    _size = std::char_traits<char>::length(string);
  } else {
    _size = std::strlen(string);
  }

  assert_message(_size < allocatedSize, "String length must not exceed capacity");

  if consteval {
    std::copy_n(string, _size + 1, _data);
  } else {
    std::memcpy(_data, string, _size + 1);
  }

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(char character) noexcept {
  static_assert(allocatedSize > 1, "FixString capacity must be greater than one.");

  _size = 1;
  _data[0] = character;
  _data[1] = '\0';

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::assign(
  const FixString<allocatedSize> & string) noexcept {
  if (this == &string)
    return *this;

  _size = string.size();
  if consteval {
    std::copy_n(string.data(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.data(), _size + 1);
  }

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::assign(const stringType & string) noexcept {
  if (_data == string.c_str()) {
    assert_message(_size == string.size(), "Aliased assign requires matching sizes");

    return *this;
  }

  assert_message(string.size() < allocatedSize, "String size must not exceed capacity");

  _size = string.size();
  if consteval {
    std::copy_n(string.c_str(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.c_str(), _size + 1);
  }

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::assign(const char * string) noexcept {
  if (_data == string)
    return *this;

  assert_message(string != nullptr, "String pointer must not be null");

  if consteval {
    _size = std::char_traits<char>::length(string);
  } else {
    _size = std::strlen(string);
  }

  assert_message(_size < allocatedSize, "String length must not exceed capacity");

  if consteval {
    std::copy_n(string, _size + 1, _data);
  } else {
    std::memcpy(_data, string, _size + 1);
  }

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::assign(char character,
                                                                             std::size_t count) noexcept {
  assert_message(count < allocatedSize, "Count must not exceed capacity");

  _size = count;
  if consteval {
    std::fill_n(_data, _size, character);
  } else {
    std::memset(_data, character, _size);
  }
  _data[_size] = '\0';

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline char & FixString<allocatedSize>::at(std::size_t offset) noexcept {
  assert_message(offset < _size, "Offset must be within bounds");

  return _data[offset];
}

template <std::size_t allocatedSize>
constexpr inline const char & FixString<allocatedSize>::at(std::size_t offset) const noexcept {
  assert_message(offset < _size, "Offset must be within bounds");

  return _data[offset];
}

template <std::size_t allocatedSize>
constexpr inline char & FixString<allocatedSize>::operator[](std::size_t offset) noexcept {
  assert_message(offset < _size, "Offset must be within bounds");

  return _data[offset];
}

template <std::size_t allocatedSize>
constexpr inline const char & FixString<allocatedSize>::operator[](std::size_t offset) const noexcept {
  assert_message(offset < _size, "Offset must be within bounds");

  return _data[offset];
}

template <std::size_t allocatedSize>
constexpr inline char & FixString<allocatedSize>::front() noexcept {
  assert_message(_size > 0, "String must not be empty");

  return _data[0];
}

template <std::size_t allocatedSize>
constexpr inline const char & FixString<allocatedSize>::front() const noexcept {
  assert_message(_size > 0, "String must not be empty");

  return _data[0];
}

template <std::size_t allocatedSize>
constexpr inline char & FixString<allocatedSize>::back() noexcept {
  assert_message(_size > 0, "String must not be empty");

  return _data[_size - 1];
}

template <std::size_t allocatedSize>
constexpr inline const char & FixString<allocatedSize>::back() const noexcept {
  assert_message(_size > 0, "String must not be empty");

  return _data[_size - 1];
}

template <std::size_t allocatedSize>
constexpr inline char * FixString<allocatedSize>::data() noexcept {
  return _data;
}

template <std::size_t allocatedSize>
constexpr inline const char * FixString<allocatedSize>::data() const noexcept {
  return _data;
}

template <std::size_t allocatedSize>
constexpr inline const char * FixString<allocatedSize>::c_str() const noexcept {
  return _data;
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::empty() const noexcept {
  return _size == 0;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::size() const noexcept {
  return _size;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::utf8_size() const noexcept {
  return utf8len(_data);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::length() const noexcept {
  return _size;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::max_size() const noexcept {
  return allocatedSize - 1;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::capacity() const noexcept {
  return allocatedSize - 1;
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::clear() noexcept {
  _size = 0;
  *_data = '\0';
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::insert(
  std::size_t index, const FixString<allocatedSize> & string) noexcept {
  _insert_raw(index, string.data(), string.size());

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::insert(std::size_t index,
                                                                             const stringType & string) noexcept {
  _insert_raw(index, string.c_str(), string.size());

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::insert(std::size_t index,
                                                                             const char * string) noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }
  _insert_raw(index, string, stringLen);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::insert(std::size_t index, char character,
                                                                             std::size_t count) noexcept {
  if (count == 0)
    return *this;

  assert_message(index <= _size, "Index must not exceed string size");
  assert_message(_size + count < allocatedSize, "Inserted characters must fit in capacity");

  // if inserting at the end, just append
  if (index == _size) {
    if consteval {
      std::fill_n(_data + index, count, character);
    } else {
      std::memset(_data + index, character, count);
    }
    _size += count;
    _data[_size] = '\0';
  } else {
    std::memmove(_data + index + count, _data + index, _size - index + 1);
    if consteval {
      std::fill_n(_data + index, count, character);
    } else {
      std::memset(_data + index, character, count);
    }
    _size += count;
  }

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::erase(std::size_t offset,
                                                                            std::size_t count) noexcept {
  assert_message(offset <= _size, "Offset must not exceed string size");

  if (count == npos)
    count = _size - offset;

  if (count == 0)
    return *this;

  assert_message(offset + count <= _size, "Erase range must be within string bounds");

  _size -= count;
  std::memmove(_data + offset, _data + offset + count, _size - offset + 1);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::push_back(char character) noexcept {
  assert_message(_size + 1 < allocatedSize, "String must have space for new character");

  _data[_size++] = character;
  _data[_size] = '\0';
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::pop_back() noexcept {
  assert_message(_size > 0, "String must not be empty for pop_back");

  if (_size > 0)
    _data[--_size] = '\0';
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::utf8_pop_back() noexcept {
  assert_message(_size > 0, "String must not be empty for utf8_pop_back");

  while (_size > 0) {
    --_size;
    const auto byte = static_cast<unsigned char>(_data[_size]);
    _data[_size] = '\0';
    if ((byte & 0xC0) != 0x80)
      break;
  }
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::append(
  const FixString<allocatedSize> & string) noexcept {
  assert_message(this != &string, "Cannot append string into itself");

  _append_raw(string._data, string._size);

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::append(const stringType & string) noexcept {
  assert_message(_data != string.c_str(), "Cannot append string into itself");

  _append_raw(string.c_str(), string.size());

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::append(const char * string) noexcept {
  assert_message(_data != string, "Cannot append string into itself");
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }
  _append_raw(string, stringLen);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::append(char character,
                                                                             std::size_t count) noexcept {
  if (count == 0)
    return *this;

  assert_message(_size + count < allocatedSize, "Appended string must fit in capacity");

  if consteval {
    std::fill_n(_data + _size, count, character);
  } else {
    std::memset(_data + _size, character, count);
  }
  _size += count;
  _data[_size] = '\0';

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(
  const FixString<allocatedSize> & string) noexcept {
  assert_message(this != &string, "Cannot append string into itself");

  _append_raw(string._data, string._size);

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(const stringType & string) noexcept {
  assert_message(_data != string.c_str(), "Cannot append string into itself");

  _append_raw(string.c_str(), string.size());

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(const char * string) noexcept {
  assert_message(_data != string, "Cannot append string into itself");
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }
  _append_raw(string, stringLen);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(char character) noexcept {
  assert_message(_size + 1 < allocatedSize, "Appended string must fit in capacity");

  _data[_size++] = character;
  _data[_size] = '\0';

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::replace(
  std::size_t pos, std::size_t count, const FixString<allocatedSize> & string) noexcept {
  _replace_raw(pos, count, string._data, string._size);

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::replace(std::size_t pos, std::size_t count,
                                                                              const stringType & string) noexcept {
  _replace_raw(pos, count, string.c_str(), string.size());

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::replace(std::size_t pos, std::size_t count,
                                                                              const char * string) noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }
  _replace_raw(pos, count, string, stringLen);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::replace(std::size_t pos, std::size_t count,
                                                                              char character,
                                                                              std::size_t charactersCount) noexcept {
  if (count == 0 && charactersCount == 0)
    return *this;

  assert_message(pos <= _size, "Position must be within string bounds");
  assert_message(pos + count <= _size, "Replacement range must be within string bounds");

  const auto newSize = _size - count + charactersCount;

  assert_message(newSize < allocatedSize, "Replacement result must fit in capacity");

  // If sizes are equal, no need to shift data
  if (count == charactersCount) {
    if consteval {
      std::fill_n(_data + pos, charactersCount, character);
    } else {
      std::memset(_data + pos, character, charactersCount);
    }
  } else {
    // If replacing at the end, no need to shift
    if (pos + count == _size) {
      if consteval {
        std::fill_n(_data + pos, charactersCount, character);
      } else {
        std::memset(_data + pos, character, charactersCount);
      }
      _data[pos + charactersCount] = '\0';
    } else {
      std::memmove(_data + pos + charactersCount, _data + pos + count, _size - pos - count + 1);
      if consteval {
        std::fill_n(_data + pos, charactersCount, character);
      } else {
        std::memset(_data + pos, character, charactersCount);
      }
    }

    _size = newSize;
  }

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::copy(char * dest, std::size_t count,
                                                            std::size_t pos) const noexcept {
  assert_message(pos <= _size, "Position must be within string bounds");
  assert_message(dest != nullptr, "Destination pointer must not be null");
  assert_message((dest < _data) || (dest >= (_data + allocatedSize)),
                 "Destination buffer must not overlap internal storage");

  if (count == npos || pos + count > _size)
    count = _size - pos;

  if consteval {
    std::copy_n(_data + pos, count, dest);
  } else {
    std::memcpy(dest, _data + pos, count);
  }

  return count;
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::swap(FixString<allocatedSize> & string) noexcept {
  if (this == &string)
    return;

  char tempData[allocatedSize];
  if consteval {
    std::copy_n(_data, _size + 1, tempData);
    std::copy_n(string._data, string._size + 1, _data);
    std::copy_n(tempData, _size + 1, string._data);
  } else {
    std::memcpy(tempData, _data, _size + 1);
    std::memcpy(_data, string._data, string._size + 1);
    std::memcpy(string._data, tempData, _size + 1);
  }

  std::swap(_size, string._size);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find(const FixString<allocatedSize> & string,
                                                            std::size_t position) const noexcept {
  return _find_raw(position, string._data, string._size);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline std::size_t FixString<allocatedSize>::find(const stringType & string,
                                                            std::size_t position) const noexcept {
  return _find_raw(position, string.c_str(), string.size());
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find(const char * string, std::size_t position) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_raw(position, string, stringLen);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find(char character, std::size_t position) const noexcept {
  return _find_raw(position, &character, 1);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::rfind(const FixString<allocatedSize> & string,
                                                             std::size_t position) const noexcept {
  return _rfind_raw(position, string._data, string._size);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline std::size_t FixString<allocatedSize>::rfind(const stringType & string,
                                                             std::size_t position) const noexcept {
  return _rfind_raw(position, string.c_str(), string.size());
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::rfind(const char * string, std::size_t position) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _rfind_raw(position, string, stringLen);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::rfind(char character, std::size_t position) const noexcept {
  return _rfind_raw(position, &character, 1);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_first_of(const FixString<allocatedSize> & string,
                                                                     std::size_t position) const noexcept {
  return _find_first_of_raw(position, string._data, string._size);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline std::size_t FixString<allocatedSize>::find_first_of(const stringType & string,
                                                                     std::size_t position) const noexcept {
  return _find_first_of_raw(position, string.c_str(), string.size());
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_first_of(const char * string,
                                                                     std::size_t position) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_first_of_raw(position, string, stringLen);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_first_of(char character,
                                                                     std::size_t position) const noexcept {
  return _find_first_of_raw(position, &character, 1);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_first_not_of(const FixString<allocatedSize> & string,
                                                                         std::size_t position) const noexcept {
  return _find_first_not_of_raw(position, string._data, string._size);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline std::size_t FixString<allocatedSize>::find_first_not_of(const stringType & string,
                                                                         std::size_t position) const noexcept {
  return _find_first_not_of_raw(position, string.c_str(), string.size());
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_first_not_of(const char * string,
                                                                         std::size_t position) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_first_not_of_raw(position, string, stringLen);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_first_not_of(char character,
                                                                         std::size_t position) const noexcept {
  return _find_first_not_of_raw(position, &character, 1);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_last_of(const FixString<allocatedSize> & string,
                                                                    std::size_t position) const noexcept {
  return _find_last_of_raw(position, string._data, string._size);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline std::size_t FixString<allocatedSize>::find_last_of(const stringType & string,
                                                                    std::size_t position) const noexcept {
  return _find_last_of_raw(position, string.c_str(), string.size());
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_last_of(const char * string,
                                                                    std::size_t position) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_last_of_raw(position, string, stringLen);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_last_of(char character,
                                                                    std::size_t position) const noexcept {
  return _find_last_of_raw(position, &character, 1);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_last_not_of(const FixString<allocatedSize> & string,
                                                                        std::size_t position) const noexcept {
  return _find_last_not_of_raw(position, string._data, string._size);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline std::size_t FixString<allocatedSize>::find_last_not_of(const stringType & string,
                                                                        std::size_t position) const noexcept {
  return _find_last_not_of_raw(position, string.c_str(), string.size());
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_last_not_of(const char * string,
                                                                        std::size_t position) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_last_not_of_raw(position, string, stringLen);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find_last_not_of(char character,
                                                                        std::size_t position) const noexcept {
  return _find_last_not_of_raw(position, &character, 1);
}

template <std::size_t allocatedSize>
constexpr inline int FixString<allocatedSize>::compare(const FixString<allocatedSize> & string) const noexcept {
  return std::strcmp(_data, string._data);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline int FixString<allocatedSize>::compare(const stringType & string) const noexcept {
  return std::strcmp(_data, string.c_str());
}

template <std::size_t allocatedSize>
constexpr inline int FixString<allocatedSize>::compare(const char * string) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  return std::strcmp(_data, string);
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::starts_with(const FixString<allocatedSize> & string) const noexcept {
  return _size >= string._size && std::memcmp(_data, string._data, string._size) == 0;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline bool FixString<allocatedSize>::starts_with(const stringType & string) const noexcept {
  return _size >= string.size() && std::memcmp(_data, string.c_str(), string.size()) == 0;
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::starts_with(const char * string) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t needleSize;
  if consteval {
    needleSize = std::char_traits<char>::length(string);
  } else {
    needleSize = std::strlen(string);
  }

  return _size >= needleSize && std::memcmp(_data, string, needleSize) == 0;
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::starts_with(char character) const noexcept {
  return _size > 0 && _data[0] == character;
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::ends_with(const FixString<allocatedSize> & string) const noexcept {
  return _size >= string._size && std::memcmp(_data + (_size - string._size), string._data, string._size) == 0;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline bool FixString<allocatedSize>::ends_with(const stringType & string) const noexcept {
  return _size >= string.size() && std::memcmp(_data + (_size - string.size()), string.c_str(), string.size()) == 0;
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::ends_with(const char * string) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t needleSize;
  if consteval {
    needleSize = std::char_traits<char>::length(string);
  } else {
    needleSize = std::strlen(string);
  }

  return _size >= needleSize && std::memcmp(_data + (_size - needleSize), string, needleSize) == 0;
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::ends_with(char character) const noexcept {
  return _size > 0 && _data[_size - 1] == character;
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::contains(const FixString<allocatedSize> & string) const noexcept {
  return _size >= string._size && std::strstr(_data, string._data) != nullptr;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline bool FixString<allocatedSize>::contains(const stringType & string) const noexcept {
  return _size >= string.size() && std::strstr(_data, string.c_str()) != nullptr;
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::contains(const char * string) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  std::size_t needleSize;
  if consteval {
    needleSize = std::char_traits<char>::length(string);
  } else {
    needleSize = std::strlen(string);
  }

  return _size >= needleSize && std::strstr(_data, string) != nullptr;
}

template <std::size_t allocatedSize>
constexpr inline bool FixString<allocatedSize>::contains(char character) const noexcept {
  return _size > 0 && std::strchr(_data, character) != nullptr;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> FixString<allocatedSize>::substr(std::size_t position,
                                                                           std::size_t count) const noexcept {
  assert_message(position <= _size, "Position must not exceed string size");

  const auto remaining = _size - position;
  if (count == npos || count > remaining)
    count = remaining;

  assert_message(count <= remaining, "Substring range must be within string bounds");

  FixString<allocatedSize> result;

  result._size = count;
  if consteval {
    std::copy_n(_data + position, count, result._data);
  } else {
    std::memcpy(result._data, _data + position, count);
  }
  result._data[count] = '\0';

  return result;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> FixString<allocatedSize>::operator+(
  const FixString<allocatedSize> & string) const noexcept {
  assert(_size + string._size < allocatedSize);

  FixString<allocatedSize> value(*this);
  value += string;

  return value;
}

template <std::size_t allocatedSize>
template <std::size_t allocatedSize2>
constexpr inline FixString<allocatedSize> FixString<allocatedSize>::operator+(
  const FixString<allocatedSize2> & string) const noexcept {
  static_assert(allocatedSize2 > 0, "FixString capacity must be greater than zero.");
  assert(_size + string.size() < allocatedSize);

  FixString<allocatedSize> value(*this);
  value += string;

  return value;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> FixString<allocatedSize>::operator+(const char * string) const noexcept {
  assert(string != nullptr);

  FixString<allocatedSize> value(*this);
  value += string;

  return value;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> FixString<allocatedSize>::operator+(char symbol) const noexcept {
  FixString<allocatedSize> value(*this);
  value += symbol;

  return value;
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::_insert_raw(std::size_t position, const char * data,
                                                            std::size_t dataSize) noexcept {
  if (dataSize == 0)
    return;

  assert_message(((data + dataSize) < _data) || (data >= (_data + allocatedSize)),
                 "Source data pointer must not point into _data buffer");

  assert_message(position <= _size, "Index must not exceed string size");
  assert_message(_size + dataSize < allocatedSize, "Inserted string must fit in capacity");

  // If inserting at the end, just append
  if (position == _size) {
    if consteval {
      std::copy_n(data, dataSize + 1, _data + _size);
    } else {
      std::memcpy(_data + _size, data, dataSize + 1);
    }
  } else {
    std::memmove(_data + position + dataSize, _data + position, _size - position + 1);
    if consteval {
      std::copy_n(data, dataSize, _data + position);
    } else {
      std::memcpy(_data + position, data, dataSize);
    }
  }

  _size += dataSize;
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::_append_raw(const char * data, std::size_t dataSize) noexcept {
  if (dataSize == 0)
    return;

  assert_message(((data + dataSize) < _data) || (data >= (_data + allocatedSize)),
                 "Source data pointer must not point into _data buffer");
  assert_message(_size + dataSize < allocatedSize, "Appended data must fit in capacity");

  if consteval {
    std::copy_n(data, dataSize + 1, _data + _size);
  } else {
    std::memcpy(_data + _size, data, dataSize + 1);
  }
  _size += dataSize;
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::_replace_raw(std::size_t position, std::size_t oldCount,
                                                             const char * data, std::size_t dataSize) noexcept {
  if (oldCount == 0 && dataSize == 0)
    return;

  assert_message(((data + dataSize) < _data) || (data >= (_data + allocatedSize)),
                 "Source data pointer must not point into _data buffer");

  assert_message(position <= _size, "Position must be within string bounds");
  assert_message(position + oldCount <= _size, "Replacement range must be within string bounds");

  // If sizes are equal, no need to shift data
  if (oldCount == dataSize) {
    if consteval {
      std::copy_n(data, dataSize, _data + position);
    } else {
      std::memcpy(_data + position, data, dataSize);
    }

    return;
  }

  assert_message((_size - oldCount + dataSize) < allocatedSize, "Replacement result must fit in capacity");

  // If replacing at the end, no need to shift
  if (position + oldCount == _size) {
    if consteval {
      std::copy_n(data, dataSize + 1, _data + position);
    } else {
      std::memcpy(_data + position, data, dataSize + 1);
    }
  } else {
    std::memmove(_data + position + dataSize, _data + position + oldCount, _size - position - oldCount + 1);
    if consteval {
      std::copy_n(data, dataSize, _data + position);
    } else {
      std::memcpy(_data + position, data, dataSize);
    }
  }

  _size = _size - oldCount + dataSize;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::_find_raw(std::size_t position, const char * data,
                                                                 std::size_t dataSize) const noexcept {
  if (position >= _size)
    return npos;

  if (dataSize == 0)
    return position;
  else if (dataSize > _size - position)
    return npos;

  const auto occurrence = dataSize == 1 ? std::strchr(_data + position, data[0]) : std::strstr(_data + position, data);

  return occurrence != nullptr ? occurrence - _data : npos;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::_rfind_raw(std::size_t position, const char * data,
                                                                  std::size_t dataSize) const noexcept {
  if (dataSize == 0)
    return std::min(position, _size);
  else if (dataSize > _size)
    return npos;

  if (position == npos)
    position = _size - dataSize;
  else if (position > _size - dataSize)
    return npos;

  for (std::size_t i = 0; i <= position; ++i) {
    const auto offset = position - i;
    if (std::memcmp(_data + offset, data, dataSize) == 0)
      return offset;
  }

  return npos;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::_find_first_of_raw(std::size_t position, const char * data,
                                                                          std::size_t dataSize) const noexcept {
  if (position >= _size || dataSize == 0)
    return npos;

  const auto occurrence = dataSize == 1 ? std::strchr(_data + position, data[0]) : std::strpbrk(_data + position, data);

  return occurrence != nullptr ? occurrence - _data : npos;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::_find_first_not_of_raw(std::size_t position, const char * data,
                                                                              std::size_t dataSize) const noexcept {
  if (position >= _size)
    return npos;

  if (dataSize == 0)
    return position;

  if (dataSize == 1) {
    const auto exclude = data[0];
    for (auto i = position; i < _size; ++i) {
      if (_data[i] != exclude)
        return i;
    }

    return npos;
  }

  std::array<bool, 256> excludedChars{};

  for (std::size_t i = 0; i < dataSize; ++i) {
    excludedChars[static_cast<unsigned char>(data[i])] = true;
  }

  for (auto i = position; i < _size; ++i) {
    if (!excludedChars[static_cast<unsigned char>(_data[i])])
      return i;
  }

  return npos;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::_find_last_of_raw(std::size_t position, const char * data,
                                                                         std::size_t dataSize) const noexcept {
  if (dataSize == 0 || _size == 0)
    return npos;

  if (position == npos)
    position = _size - 1;
  else if (position >= _size)
    return npos;

  if (dataSize == 1) {
    const auto target = data[0];
    for (std::size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (_data[scanIndex] == target)
        return scanIndex;
    }
  } else {
    std::array<bool, 256> targetChars{};

    for (std::size_t i = 0; i < dataSize; ++i) {
      targetChars[static_cast<unsigned char>(data[i])] = true;
    }

    for (std::size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (targetChars[static_cast<unsigned char>(_data[scanIndex])])
        return scanIndex;
    }
  }

  return npos;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::_find_last_not_of_raw(std::size_t position, const char * data,
                                                                             std::size_t dataSize) const noexcept {
  if (_size == 0)
    return npos;

  if (position == npos)
    position = _size - 1;
  else if (position >= _size)
    return npos;

  if (dataSize == 0)
    return position;

  if (dataSize == 1) {
    const auto exclude = data[0];
    for (std::size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (_data[scanIndex] != exclude)
        return scanIndex;
    }
  } else {
    std::array<bool, 256> excludedChars{};

    for (std::size_t i = 0; i < dataSize; ++i) {
      excludedChars[static_cast<unsigned char>(data[i])] = true;
    }

    for (std::size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (!excludedChars[static_cast<unsigned char>(_data[scanIndex])])
        return scanIndex;
    }
  }

  return npos;
}

} // namespace toygine

#endif // INCLUDE_CORE_FIX_STRING_INL_
