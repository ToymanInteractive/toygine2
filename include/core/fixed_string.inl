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
  \file   fixed_string.inl
  \brief  Inline implementations for FixedString template class.
*/

#ifndef INCLUDE_CORE_FIXED_STRING_INL_
#define INCLUDE_CORE_FIXED_STRING_INL_

namespace toy {

template <size_t allocatedSize>
constexpr FixedString<allocatedSize>::FixedString() noexcept
  : _data{'\0'}
  , _size(0) {}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize>::FixedString(const FixedString<allocatedSize> & string) noexcept
  : _data{}
  , _size(string.size()) {
  if consteval {
    std::copy_n(string.data(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.data(), _size + 1);
  }
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize>::FixedString(const stringType & string) noexcept
  : _data{}
  , _size(string.size()) {
  assert_message(_size < allocatedSize, "String size must not exceed capacity");

  if consteval {
    std::copy_n(string.c_str(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.c_str(), _size + 1);
  }
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize>::FixedString(const char * string) noexcept
  : _data{}
  , _size(0) {
  assert_message(string != nullptr, "C string must not be null");

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

template <size_t allocatedSize>
constexpr FixedString<allocatedSize>::FixedString(char character, size_t count) noexcept
  : _data{}
  , _size(count) {
  assert_message(count < allocatedSize, "Count must not exceed capacity");

  if consteval {
    std::fill_n(_data, _size, character);
  } else {
    std::memset(_data, character, _size);
  }
  _data[_size] = '\0';
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator=(
  const FixedString<allocatedSize> & string) noexcept {
  if (this == std::addressof(string))
    return *this;

  _size = string._size;
  std::memcpy(_data, string._data, _size + 1);

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator=(const stringType & string) noexcept {
  assert_message(string.size() < allocatedSize, "String size must not exceed capacity");

  _size = string.size();
  if consteval {
    std::copy_n(string.c_str(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.c_str(), _size + 1);
  }

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator=(const char * string) noexcept {
  if (_data == string)
    return *this;

  assert_message(string != nullptr, "C string must not be null");

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

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator=(char character) noexcept {
  static_assert(allocatedSize > 1, "FixedString capacity must be greater than one.");

  _size = 1;
  _data[0] = character;
  _data[1] = '\0';

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::assign(
  const FixedString<allocatedSize> & string) noexcept {
  if (this == std::addressof(string))
    return *this;

  _size = string.size();
  if consteval {
    std::copy_n(string.data(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.data(), _size + 1);
  }

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::assign(const stringType & string) noexcept {
  assert_message(string.size() < allocatedSize, "String size must not exceed capacity");

  _size = string.size();
  if consteval {
    std::copy_n(string.c_str(), _size + 1, _data);
  } else {
    std::memcpy(_data, string.c_str(), _size + 1);
  }

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::assign(const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  if consteval {
    _size = std::char_traits<char>::length(string);
  } else {
    if (_data == string)
      return *this;

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

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::assign(char character, size_t count) noexcept {
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

template <size_t allocatedSize>
constexpr char & FixedString<allocatedSize>::at(size_t offset) noexcept {
  assert_message(offset < _size, "Offset must be within bounds");

  return _data[offset];
}

template <size_t allocatedSize>
constexpr const char & FixedString<allocatedSize>::at(size_t offset) const noexcept {
  assert_message(offset < _size || (offset == 0 && empty()), "Offset must be within bounds");

  return _data[offset];
}

template <size_t allocatedSize>
constexpr char & FixedString<allocatedSize>::operator[](size_t offset) noexcept {
  assert_message(offset < _size, "Offset must be within bounds");

  return _data[offset];
}

template <size_t allocatedSize>
constexpr const char & FixedString<allocatedSize>::operator[](size_t offset) const noexcept {
  assert_message(offset < _size || (offset == 0 && empty()), "Offset must be within bounds");

  return _data[offset];
}

template <size_t allocatedSize>
constexpr char & FixedString<allocatedSize>::front() noexcept {
  return _data[0];
}

template <size_t allocatedSize>
constexpr const char & FixedString<allocatedSize>::front() const noexcept {
  return _data[0];
}

template <size_t allocatedSize>
constexpr char & FixedString<allocatedSize>::back() noexcept {
  assert_message(!empty(), "String must not be empty");

  return _data[_size - 1];
}

template <size_t allocatedSize>
constexpr const char & FixedString<allocatedSize>::back() const noexcept {
  assert_message(!empty(), "String must not be empty");

  return _data[_size - 1];
}

template <size_t allocatedSize>
constexpr char * FixedString<allocatedSize>::data() noexcept {
  return _data;
}

template <size_t allocatedSize>
constexpr const char * FixedString<allocatedSize>::data() const noexcept {
  return _data;
}

template <size_t allocatedSize>
constexpr const char * FixedString<allocatedSize>::c_str() const noexcept {
  return _data;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::empty() const noexcept {
  return _size == 0;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::size() const noexcept {
  return _size;
}

template <size_t allocatedSize>
inline size_t FixedString<allocatedSize>::utf8_size() const noexcept {
  return utf8Len(_data);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::length() const noexcept {
  return _size;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::max_size() const noexcept {
  return allocatedSize - 1;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::capacity() const noexcept {
  return allocatedSize - 1;
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::clear() noexcept {
  _size = 0;
  *_data = '\0';
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::insert(
  size_t index, const FixedString<allocatedSize> & string) noexcept {
  _insert_raw(index, string.data(), string.size());

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::insert(size_t index,
                                                                          const stringType & string) noexcept {
  _insert_raw(index, string.c_str(), string.size());

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::insert(size_t index, const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }
  _insert_raw(index, string, stringLen);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::insert(size_t index, char character,
                                                                          size_t count) noexcept {
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

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::erase(size_t offset, size_t count) noexcept {
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

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::push_back(char character) noexcept {
  assert_message(_size + 1 < allocatedSize, "String must have space for new character");

  _data[_size++] = character;
  _data[_size] = '\0';
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::pop_back() noexcept {
  assert_message(!empty(), "String must not be empty for pop_back");

  if (_size > 0)
    _data[--_size] = '\0';
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::utf8_pop_back() noexcept {
  assert_message(!empty(), "String must not be empty for utf8_pop_back");

  while (_size > 0) {
    --_size;
    const auto byte = static_cast<unsigned char>(_data[_size]);
    _data[_size] = '\0';
    if ((byte & 0xC0) != 0x80)
      break;
  }
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::append(
  const FixedString<allocatedSize> & string) noexcept {
  assert_message(this != &string, "Cannot append string into itself");

  _append_raw(string._data, string._size);

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::append(const stringType & string) noexcept {
  assert_message(_data != string.c_str(), "Cannot append string into itself");

  _append_raw(string.c_str(), string.size());

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::append(const char * string) noexcept {
  assert_message(_data != string, "Cannot append string into itself");
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }
  _append_raw(string, stringLen);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::append(char character, size_t count) noexcept {
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

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator+=(
  const FixedString<allocatedSize> & string) noexcept {
  assert_message(this != &string, "Cannot append string into itself");

  _append_raw(string._data, string._size);

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator+=(const stringType & string) noexcept {
  assert_message(_data != string.c_str(), "Cannot append string into itself");

  _append_raw(string.c_str(), string.size());

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator+=(const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    assert_message(_data != string, "Cannot append string into itself");

    stringLen = std::strlen(string);
  }
  _append_raw(string, stringLen);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator+=(char character) noexcept {
  assert_message(_size + 1 < allocatedSize, "Appended string must fit in capacity");

  _data[_size++] = character;
  _data[_size] = '\0';

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::replace(
  size_t pos, size_t count, const FixedString<allocatedSize> & string) noexcept {
  _replace_raw(pos, count, string._data, string._size);

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::replace(size_t pos, size_t count,
                                                                           const stringType & string) noexcept {
  _replace_raw(pos, count, string.c_str(), string.size());

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::replace(size_t pos, size_t count,
                                                                           const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }
  _replace_raw(pos, count, string, stringLen);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::replace(size_t pos, size_t count, char character,
                                                                           size_t charactersCount) noexcept {
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

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::copy(char * dest, size_t count, size_t pos) const noexcept {
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

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::swap(FixedString<allocatedSize> & string) noexcept {
  if (this == std::addressof(string))
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

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find(const FixedString<allocatedSize> & string,
                                                  size_t position) const noexcept {
  return _find_raw(position, string._data, string._size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::find(const stringType & string, size_t position) const noexcept {
  return _find_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find(char character, size_t position) const noexcept {
  return _find_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::rfind(const FixedString<allocatedSize> & string,
                                                   size_t position) const noexcept {
  return _rfind_raw(position, string._data, string._size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::rfind(const stringType & string, size_t position) const noexcept {
  return _rfind_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::rfind(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _rfind_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::rfind(char character, size_t position) const noexcept {
  return _rfind_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_of(const FixedString<allocatedSize> & string,
                                                           size_t position) const noexcept {
  return _find_first_of_raw(position, string._data, string._size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::find_first_of(const stringType & string, size_t position) const noexcept {
  return _find_first_of_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_of(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_first_of_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_of(char character, size_t position) const noexcept {
  return _find_first_of_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_not_of(const FixedString<allocatedSize> & string,
                                                               size_t position) const noexcept {
  return _find_first_not_of_raw(position, string._data, string._size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::find_first_not_of(const stringType & string,
                                                               size_t position) const noexcept {
  return _find_first_not_of_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_not_of(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_first_not_of_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_not_of(char character, size_t position) const noexcept {
  return _find_first_not_of_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_of(const FixedString<allocatedSize> & string,
                                                          size_t position) const noexcept {
  return _find_last_of_raw(position, string._data, string._size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::find_last_of(const stringType & string, size_t position) const noexcept {
  return _find_last_of_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_of(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_last_of_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_of(char character, size_t position) const noexcept {
  return _find_last_of_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_not_of(const FixedString<allocatedSize> & string,
                                                              size_t position) const noexcept {
  return _find_last_not_of_raw(position, string._data, string._size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::find_last_not_of(const stringType & string,
                                                              size_t position) const noexcept {
  return _find_last_not_of_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_not_of(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t stringLen;
  if consteval {
    stringLen = std::char_traits<char>::length(string);
  } else {
    stringLen = std::strlen(string);
  }

  return _find_last_not_of_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_not_of(char character, size_t position) const noexcept {
  return _find_last_not_of_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr int FixedString<allocatedSize>::compare(const FixedString<allocatedSize> & string) const noexcept {
  if consteval {
    return cstrcmp(_data, string._data);
  } else {
    return std::strcmp(_data, string._data);
  }
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr int FixedString<allocatedSize>::compare(const stringType & string) const noexcept {
  if consteval {
    return cstrcmp(_data, string.c_str());
  } else {
    return std::strcmp(_data, string.c_str());
  }
}

template <size_t allocatedSize>
constexpr int FixedString<allocatedSize>::compare(const char * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  if consteval {
    return cstrcmp(_data, string);
  } else {
    return std::strcmp(_data, string);
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::starts_with(const FixedString<allocatedSize> & string) const noexcept {
  if consteval {
    return _size >= string._size && std::equal(_data, _data + string._size, string._data);
  } else {
    return _size >= string._size && std::memcmp(_data, string._data, string._size) == 0;
  }
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr bool FixedString<allocatedSize>::starts_with(const stringType & string) const noexcept {
  const auto stringSize = string.size();
  if (size() < stringSize)
    return false;

  if consteval {
    return std::equal(_data, _data + stringSize, string.c_str());
  } else {
    return std::memcmp(_data, string.c_str(), stringSize) == 0;
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::starts_with(const char * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t needleSize;
  if consteval {
    needleSize = std::char_traits<char>::length(string);

    return _size >= needleSize && std::equal(_data, _data + needleSize, string);
  } else {
    needleSize = std::strlen(string);

    return _size >= needleSize && std::memcmp(_data, string, needleSize) == 0;
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::starts_with(char character) const noexcept {
  return !empty() && _data[0] == character;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::ends_with(const FixedString<allocatedSize> & string) const noexcept {
  if consteval {
    return _size >= string._size && std::equal(_data + _size - string._size, _data + _size, string._data);
  } else {
    return _size >= string._size && std::memcmp(_data + _size - string._size, string._data, string._size) == 0;
  }
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr bool FixedString<allocatedSize>::ends_with(const stringType & string) const noexcept {
  const auto stringSize = string.size();
  if (_size < stringSize)
    return false;

  if consteval {
    return std::equal(_data + _size - stringSize, _data + _size, string.c_str());
  } else {
    return std::memcmp(_data + _size - stringSize, string.c_str(), stringSize) == 0;
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::ends_with(const char * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  size_t needleSize;
  if consteval {
    needleSize = std::char_traits<char>::length(string);
  } else {
    needleSize = std::strlen(string);
  }

  if (_size < needleSize)
    return false;

  if consteval {
    return std::equal(_data + _size - needleSize, _data + _size, string);
  } else {
    return std::memcmp(_data + _size - needleSize, string, needleSize) == 0;
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::ends_with(char character) const noexcept {
  return !empty() && _data[_size - 1] == character;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::contains(const FixedString<allocatedSize> & string) const noexcept {
  if consteval {
    return _size >= string._size && cstrstr(_data, string.c_str()) != nullptr;
  } else {
    return _size >= string._size && std::strstr(_data, string.c_str()) != nullptr;
  }
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr bool FixedString<allocatedSize>::contains(const stringType & string) const noexcept {
  if consteval {
    return cstrstr(_data, string.c_str()) != nullptr;
  } else {
    return std::strstr(_data, string.c_str()) != nullptr;
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::contains(const char * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  if consteval {
    return cstrstr(_data, string) != nullptr;
  } else {
    return std::strstr(_data, string) != nullptr;
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::contains(char character) const noexcept {
  if (empty())
    return false;

  if consteval {
    return cstrchr(_data, character) != nullptr;
  } else {
    return std::memchr(_data, character, _size) != nullptr;
  }
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> FixedString<allocatedSize>::substr(size_t position, size_t count) const noexcept {
  assert_message(position <= _size, "Position must not exceed string size");

  const auto remaining = _size - position;
  if (count == npos || count > remaining)
    count = remaining;

  assert_message(count <= remaining, "Substring range must be within string bounds");

  FixedString<allocatedSize> result;

  result._size = count;
  if consteval {
    std::copy_n(_data + position, count, result._data);
  } else {
    std::memcpy(result._data, _data + position, count);
  }
  result._data[count] = '\0';

  return result;
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::_insert_raw(size_t position, const char * data, size_t dataSize) noexcept {
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

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::_append_raw(const char * data, size_t dataSize) noexcept {
  if (dataSize == 0)
    return;

  assert_message(_size + dataSize < allocatedSize, "Appended data must fit in capacity");

  if consteval {
    std::copy_n(data, dataSize + 1, _data + _size);
  } else {
    assert_message(((data + dataSize) < _data) || (data >= (_data + allocatedSize)),
                   "Source data pointer must not point into _data buffer");

    std::memcpy(_data + _size, data, dataSize + 1);
  }
  _size += dataSize;
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::_replace_raw(size_t position, size_t oldCount, const char * data,
                                                        size_t dataSize) noexcept {
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

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_find_raw(size_t position, const char * data,
                                                       size_t dataSize) const noexcept {
  if (position > _size)
    return npos;

  if (dataSize == 0)
    return position;
  else if (dataSize > _size - position)
    return npos;

  const char * occurrence;

  if consteval {
    occurrence = dataSize == 1 ? cstrchr(_data + position, data[0]) : cstrstr(_data + position, data);
  } else {
    occurrence = dataSize == 1 ? static_cast<const char *>(std::memchr(_data + position, data[0], _size - position))
                               : std::strstr(_data + position, data);
  }

  return occurrence != nullptr ? occurrence - _data : npos;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_rfind_raw(size_t position, const char * data,
                                                        size_t dataSize) const noexcept {
  if (dataSize == 0)
    return std::min(position, _size);
  else if (dataSize > _size)
    return npos;

  if (position == npos)
    position = _size - dataSize;
  else if (position > _size - dataSize)
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

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_find_first_of_raw(size_t position, const char * data,
                                                                size_t dataSize) const noexcept {
  if (position >= _size || dataSize == 0)
    return npos;

  const char * occurrence;

  if consteval {
    occurrence = dataSize == 1 ? cstrchr(_data + position, data[0]) : cstrpbrk(_data + position, data);
  } else {
    occurrence = dataSize == 1 ? std::strchr(_data + position, data[0]) : std::strpbrk(_data + position, data);
  }

  return occurrence != nullptr ? occurrence - _data : npos;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_find_first_not_of_raw(size_t position, const char * data,
                                                                    size_t dataSize) const noexcept {
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
  } else {
    std::array<bool, 256> excludedChars{};

    for (size_t i = 0; i < dataSize; ++i) {
      excludedChars[static_cast<unsigned char>(data[i])] = true;
    }

    for (auto i = position; i < _size; ++i) {
      if (!excludedChars[static_cast<unsigned char>(_data[i])])
        return i;
    }
  }

  return npos;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_find_last_of_raw(size_t position, const char * data,
                                                               size_t dataSize) const noexcept {
  if (dataSize == 0 || empty())
    return npos;

  if (position == npos)
    position = _size - 1;
  else if (position >= _size)
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

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_find_last_not_of_raw(size_t position, const char * data,
                                                                   size_t dataSize) const noexcept {
  if (empty())
    return npos;

  if (position == npos)
    position = _size - 1;
  else if (position >= _size)
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

template <size_t allocatedSize1, size_t allocatedSize2>
constexpr FixedString<allocatedSize1> operator+(const FixedString<allocatedSize1> & lhs,
                                                const FixedString<allocatedSize2> & rhs) noexcept {
  FixedString<allocatedSize1> result(lhs);

  result += rhs;

  return result;
}

template <size_t allocatedSize, StringLike stringType>
constexpr FixedString<allocatedSize> operator+(const FixedString<allocatedSize> & lhs,
                                               const stringType & rhs) noexcept {
  FixedString<allocatedSize> result(lhs);

  result += rhs;

  return result;
}

template <StringLike stringType, size_t allocatedSize>
constexpr FixedString<allocatedSize> operator+(const stringType & lhs,
                                               const FixedString<allocatedSize> & rhs) noexcept {
  FixedString<allocatedSize> result(lhs);

  result += rhs;

  return result;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> operator+(const FixedString<allocatedSize> & lhs, const char * rhs) noexcept {
  FixedString<allocatedSize> result(lhs);

  result += rhs;

  return result;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> operator+(const char * lhs, const FixedString<allocatedSize> & rhs) noexcept {
  FixedString<allocatedSize> result(lhs);

  result += rhs;

  return result;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> operator+(const FixedString<allocatedSize> & lhs, char rhs) noexcept {
  FixedString<allocatedSize> result(lhs);

  result += rhs;

  return result;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> operator+(char lhs, const FixedString<allocatedSize> & rhs) noexcept {
  FixedString<allocatedSize> result(lhs, 1);

  result += rhs;

  return result;
}

template <size_t allocatedSize1, size_t allocatedSize2>
constexpr bool operator==(const FixedString<allocatedSize1> & lhs, const FixedString<allocatedSize2> & rhs) noexcept {
  if constexpr (allocatedSize1 == allocatedSize2) {
    if (std::addressof(lhs) == std::addressof(rhs))
      return true;
  }

  if (lhs.size() != rhs.size())
    return false;
  else if (lhs.empty())
    return true;

  if consteval {
    return std::equal(lhs.c_str(), lhs.c_str() + lhs.size(), rhs.c_str());
  } else {
    return std::memcmp(lhs.c_str(), rhs.c_str(), lhs.size()) == 0;
  }
}

template <size_t allocatedSize, StringLike stringType>
constexpr bool operator==(const FixedString<allocatedSize> & lhs, const stringType & rhs) noexcept {
  if (lhs.size() != rhs.size())
    return false;
  else if (lhs.empty())
    return true;

  if consteval {
    return std::equal(lhs.c_str(), lhs.c_str() + lhs.size(), rhs.c_str());
  } else {
    return std::memcmp(lhs.c_str(), rhs.c_str(), lhs.size()) == 0;
  }
}

template <StringLike stringType, size_t allocatedSize>
constexpr bool operator==(const stringType & lhs, const FixedString<allocatedSize> & rhs) noexcept {
  return rhs == lhs;
}

template <size_t allocatedSize>
constexpr bool operator==(const FixedString<allocatedSize> & lhs, const char * rhs) noexcept {
  assert_message(rhs != nullptr, "C string must not be null");

  if (lhs.empty())
    return *rhs == '\0';

  if consteval {
    return lhs.size() == std::char_traits<char>::length(rhs) && std::equal(lhs.c_str(), lhs.c_str() + lhs.size(), rhs);
  } else {
    return lhs.size() == std::strlen(rhs) && std::memcmp(lhs.c_str(), rhs, lhs.size()) == 0;
  }
}

template <size_t allocatedSize>
constexpr bool operator==(const char * lhs, const FixedString<allocatedSize> & rhs) noexcept {
  return rhs == lhs;
}

template <size_t allocatedSize1, size_t allocatedSize2>
constexpr std::strong_ordering operator<=>(const FixedString<allocatedSize1> & lhs,
                                           const FixedString<allocatedSize2> & rhs) noexcept {
  if constexpr (allocatedSize1 == allocatedSize2) {
    if (std::addressof(lhs) == std::addressof(rhs))
      return std::strong_ordering::equal;
  }

  if (lhs.empty() && rhs.empty())
    return std::strong_ordering::equal;
  else if (lhs.empty())
    return std::strong_ordering::less;
  else if (rhs.empty())
    return std::strong_ordering::greater;

  if consteval {
    return cstrcmp(lhs.c_str(), rhs.c_str()) <=> 0;
  } else {
    const int result = std::memcmp(lhs.c_str(), rhs.c_str(), std::min(lhs.size(), rhs.size()));

    if (result < 0)
      return std::strong_ordering::less;
    else if (result > 0)
      return std::strong_ordering::greater;
    else {
      if (lhs.size() < rhs.size())
        return std::strong_ordering::less;
      else if (lhs.size() > rhs.size())
        return std::strong_ordering::greater;
      else
        return std::strong_ordering::equal;
    }
  }
}

template <size_t allocatedSize, StringLike stringType>
constexpr std::strong_ordering operator<=>(const FixedString<allocatedSize> & lhs, const stringType & rhs) noexcept {
  if (lhs.empty() && rhs.size() == 0)
    return std::strong_ordering::equal;
  else if (lhs.empty())
    return std::strong_ordering::less;
  else if (rhs.size() == 0)
    return std::strong_ordering::greater;

  if consteval {
    return cstrcmp(lhs.c_str(), rhs.c_str()) <=> 0;
  } else {
    const int result = std::memcmp(lhs.c_str(), rhs.c_str(), std::min(lhs.size(), rhs.size()));

    if (result < 0)
      return std::strong_ordering::less;
    else if (result > 0)
      return std::strong_ordering::greater;
    else {
      if (lhs.size() < rhs.size())
        return std::strong_ordering::less;
      else if (lhs.size() > rhs.size())
        return std::strong_ordering::greater;
      else
        return std::strong_ordering::equal;
    }
  }
}

template <StringLike stringType, size_t allocatedSize>
constexpr std::strong_ordering operator<=>(const stringType & lhs, const FixedString<allocatedSize> & rhs) noexcept {
  return 0 <=> (rhs <=> lhs);
}

template <size_t allocatedSize>
constexpr std::strong_ordering operator<=>(const FixedString<allocatedSize> & lhs, const char * rhs) noexcept {
  assert_message(rhs != nullptr, "C string must not be null");

  if (lhs.empty() && *rhs == '\0')
    return std::strong_ordering::equal;
  else if (lhs.empty())
    return std::strong_ordering::less;
  else if (*rhs == '\0')
    return std::strong_ordering::greater;

  if consteval {
    return cstrcmp(lhs.c_str(), rhs) <=> 0;
  } else {
    const auto rhsLen = std::strlen(rhs);
    const auto result = std::memcmp(lhs.c_str(), rhs, std::min(lhs.size(), rhsLen));

    if (result < 0)
      return std::strong_ordering::less;
    else if (result > 0)
      return std::strong_ordering::greater;
    else {
      if (lhs.size() < rhsLen)
        return std::strong_ordering::less;
      else if (lhs.size() > rhsLen)
        return std::strong_ordering::greater;
      else
        return std::strong_ordering::equal;
    }
  }
}

template <size_t allocatedSize>
constexpr std::strong_ordering operator<=>(const char * lhs, const FixedString<allocatedSize> & rhs) noexcept {
  assert_message(lhs != nullptr, "C string must not be null");

  return 0 <=> (rhs <=> lhs);
}

} // namespace toy

#endif // INCLUDE_CORE_FIXED_STRING_INL_
