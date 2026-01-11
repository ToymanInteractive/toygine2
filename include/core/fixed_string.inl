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
  \file   fixed_string.inl
  \brief  Inline implementations for FixedString template class.
*/

#ifndef INCLUDE_CORE_FIXED_STRING_INL_
#define INCLUDE_CORE_FIXED_STRING_INL_

namespace toy {

template <size_t allocatedSize>
constexpr FixedString<allocatedSize>::FixedString(size_type count, char character) noexcept {
  assert_message(count < allocatedSize, "Count must not exceed capacity");

  _storage.size = count;

  if consteval {
    std::fill_n(_storage.buffer, _storage.size, character);
  } else {
    std::memset(_storage.buffer, character, _storage.size);
  }
  _storage.buffer[_storage.size] = '\0';
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize>::FixedString(const stringType & string) noexcept {
  assert_message(string.size() < allocatedSize, "String size must not exceed capacity");

  _storage.size = string.size();

  char_traits<char>::move(_storage.buffer, string.c_str(), _storage.size + 1);
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize>::FixedString(const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  _storage.size = char_traits<char>::length(string);
  assert_message(_storage.size < allocatedSize, "String length must not exceed capacity");

  char_traits<char>::copy(_storage.buffer, string, _storage.size + 1);
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator=(
  const FixedString<allocatedSize> & string) noexcept {
  if (this == std::addressof(string))
    return *this;

  _storage.size = string._storage.size;
  char_traits<char>::move(_storage.buffer, string._storage.buffer, _storage.size + 1);

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator=(const stringType & string) noexcept {
  assert_message(string.size() < allocatedSize, "String size must not exceed capacity");

  _storage.size = string.size();
  char_traits<char>::move(_storage.buffer, string.c_str(), _storage.size + 1);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator=(const char * string) noexcept {
  if (_storage.buffer == string)
    return *this;

  assert_message(string != nullptr, "C string must not be null");

  _storage.size = char_traits<char>::length(string);
  assert_message(_storage.size < allocatedSize, "String length must not exceed capacity");

  char_traits<char>::copy(_storage.buffer, string, _storage.size + 1);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator=(char character) noexcept {
  static_assert(allocatedSize > 1, "FixedString capacity must be greater than one.");

  _storage.size = 1;
  _storage.buffer[0] = character;
  _storage.buffer[1] = '\0';

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::assign(
  const FixedString<allocatedSize> & string) noexcept {
  if (this == std::addressof(string))
    return *this;

  _storage.size = string.size();
  char_traits<char>::move(_storage.buffer, string.data(), _storage.size + 1);

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::assign(const stringType & string) noexcept {
  assert_message(string.size() < allocatedSize, "String size must not exceed capacity");

  _storage.size = string.size();
  char_traits<char>::move(_storage.buffer, string.c_str(), _storage.size + 1);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::assign(const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  if !consteval {
    if (_storage.buffer == string)
      return *this;
  }

  _storage.size = char_traits<char>::length(string);
  assert_message(_storage.size < allocatedSize, "String length must not exceed capacity");

  char_traits<char>::copy(_storage.buffer, string, _storage.size + 1);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::assign(char character, size_t count) noexcept {
  assert_message(count < allocatedSize, "Count must not exceed capacity");

  _storage.size = count;
  if consteval {
    std::fill_n(_storage.buffer, _storage.size, character);
  } else {
    std::memset(_storage.buffer, character, _storage.size);
  }
  _storage.buffer[_storage.size] = '\0';

  return *this;
}

template <size_t allocatedSize>
constexpr char & FixedString<allocatedSize>::at(size_t offset) noexcept {
  assert_message(offset < _storage.size, "Offset must be within bounds");

  return _storage.buffer[offset];
}

template <size_t allocatedSize>
constexpr const char & FixedString<allocatedSize>::at(size_t offset) const noexcept {
  assert_message(offset < _storage.size || (offset == 0 && empty()), "Offset must be within bounds");

  return _storage.buffer[offset];
}

template <size_t allocatedSize>
constexpr char & FixedString<allocatedSize>::operator[](size_t offset) noexcept {
  assert_message(offset < _storage.size, "Offset must be within bounds");

  return _storage.buffer[offset];
}

template <size_t allocatedSize>
constexpr const char & FixedString<allocatedSize>::operator[](size_t offset) const noexcept {
  assert_message(offset < _storage.size || (offset == 0 && empty()), "Offset must be within bounds");

  return _storage.buffer[offset];
}

template <size_t allocatedSize>
constexpr char & FixedString<allocatedSize>::front() noexcept {
  return _storage.buffer[0];
}

template <size_t allocatedSize>
constexpr const char & FixedString<allocatedSize>::front() const noexcept {
  return _storage.buffer[0];
}

template <size_t allocatedSize>
constexpr char & FixedString<allocatedSize>::back() noexcept {
  assert_message(!empty(), "String must not be empty");

  return _storage.buffer[_storage.size - 1];
}

template <size_t allocatedSize>
constexpr const char & FixedString<allocatedSize>::back() const noexcept {
  assert_message(!empty(), "String must not be empty");

  return _storage.buffer[_storage.size - 1];
}

template <size_t allocatedSize>
constexpr char * FixedString<allocatedSize>::data() noexcept {
  return _storage.buffer;
}

template <size_t allocatedSize>
constexpr const char * FixedString<allocatedSize>::data() const noexcept {
  return _storage.buffer;
}

template <size_t allocatedSize>
constexpr const char * FixedString<allocatedSize>::c_str() const noexcept {
  return _storage.buffer;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::empty() const noexcept {
  return _storage.size == 0;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::size() const noexcept {
  return _storage.size;
}

template <size_t allocatedSize>
inline size_t FixedString<allocatedSize>::utf8_size() const noexcept {
  return utf8Len(_storage.buffer);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::length() const noexcept {
  return _storage.size;
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
  _storage.size = 0;
  *_storage.buffer = '\0';
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

  const auto stringLen = char_traits<char>::length(string);

  _insert_raw(index, string, stringLen);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::insert(size_t index, char character,
                                                                          size_t count) noexcept {
  if (count == 0)
    return *this;

  assert_message(index <= _storage.size, "Index must not exceed string size");
  assert_message(_storage.size + count < allocatedSize, "Inserted characters must fit in capacity");

  // if inserting at the end, just append
  if (index == _storage.size) {
    if consteval {
      std::fill_n(_storage.buffer + index, count, character);
    } else {
      std::memset(_storage.buffer + index, character, count);
    }

    _storage.size += count;
    _storage.buffer[_storage.size] = '\0';
  } else {
    char_traits<char>::move(_storage.buffer + index + count, _storage.buffer + index, _storage.size - index + 1);

    if consteval {
      std::fill_n(_storage.buffer + index, count, character);
    } else {
      std::memset(_storage.buffer + index, character, count);
    }

    _storage.size += count;
  }

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::erase(size_t offset, size_t count) noexcept {
  assert_message(offset <= _storage.size, "Offset must not exceed string size");

  if (count == npos)
    count = _storage.size - offset;

  if (count == 0)
    return *this;

  assert_message(offset + count <= _storage.size, "Erase range must be within string bounds");

  _storage.size -= count;
  char_traits<char>::move(_storage.buffer + offset, _storage.buffer + offset + count, _storage.size - offset + 1);

  return *this;
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::push_back(char character) noexcept {
  assert_message(character != '\0', "Character must not be null.");
  if (character == '\0')
    return;

  assert_message(_storage.size + 1 < allocatedSize, "String must have space for new character");

  _storage.buffer[_storage.size++] = character;
  _storage.buffer[_storage.size] = '\0';
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::pop_back() noexcept {
  assert_message(!empty(), "String must not be empty for pop_back");

  if (_storage.size > 0)
    _storage.buffer[--_storage.size] = '\0';
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::utf8_pop_back() noexcept {
  assert_message(!empty(), "String must not be empty for utf8_pop_back");

  while (_storage.size > 0) {
    --_storage.size;
    const auto byte = static_cast<unsigned char>(_storage.buffer[_storage.size]);
    _storage.buffer[_storage.size] = '\0';
    if ((byte & 0xC0) != 0x80)
      break;
  }
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::append(size_type count, char character) noexcept {
  assert_message(character != '\0', "Character must not be null.");
  if (character == '\0' || count == 0)
    return *this;

  assert_message(_storage.size + count < allocatedSize, "Appended string must fit in capacity");

  if consteval {
    std::fill_n(_storage.buffer + _storage.size, count, character);
  } else {
    std::memset(_storage.buffer + _storage.size, character, count);
  }

  _storage.size += count;
  _storage.buffer[_storage.size] = '\0';

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::append(const char * string,
                                                                          size_type count) noexcept {
  if (count == 0)
    return *this;

  assert_message(string != nullptr, "C string must not be null");
  assert_message(char_traits<char>::find(string, count, '\0') == nullptr,
                 "C string must be at least 'count' characters long");
  assert_message(_storage.size + count < allocatedSize, "Appended data must fit in capacity");
  if !consteval {
    assert_message(((string + count) < _storage.buffer) || (string >= (_storage.buffer + allocatedSize)),
                   "Source data pointer must not point into _storage.buffer buffer");
  }

  char_traits<char>::move(_storage.buffer + _storage.size, string, count);
  _storage.size += count;
  _storage.buffer[_storage.size] = '\0';

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::append(const char * string) noexcept {
  assert_message(_storage.buffer != string, "Cannot append string into itself");
  assert_message(string != nullptr, "C string must not be null");

  _append_raw(string, char_traits<char>::length(string));

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::append(
  const FixedString<allocatedSize> & string) noexcept {
  assert_message(this != &string, "Cannot append string into itself");

  _append_raw(string._storage.buffer, string._storage.size);

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::append(const stringType & string) noexcept {
  assert_message(_storage.buffer != string.c_str(), "Cannot append string into itself");

  _append_raw(string.c_str(), string.size());

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator+=(
  const FixedString<allocatedSize> & string) noexcept {
  assert_message(this != &string, "Cannot append string into itself");

  _append_raw(string._storage.buffer, string._storage.size);

  return *this;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator+=(const stringType & string) noexcept {
  assert_message(_storage.buffer != string.c_str(), "Cannot append string into itself");

  _append_raw(string.c_str(), string.size());

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator+=(const char * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  if !consteval {
    assert_message(_storage.buffer != string, "Cannot append string into itself");
  }

  const auto stringLen = char_traits<char>::length(string);

  _append_raw(string, stringLen);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::operator+=(char character) noexcept {
  assert_message(character != '\0', "Character must not be null.");

  if (character == '\0')
    return *this;

  assert_message(_storage.size + 1 < allocatedSize, "Appended string must fit in capacity");

  _storage.buffer[_storage.size++] = character;
  _storage.buffer[_storage.size] = '\0';

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::replace(
  size_t pos, size_t count, const FixedString<allocatedSize> & string) noexcept {
  _replace_raw(pos, count, string._storage.buffer, string._storage.size);

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

  const auto stringLen = char_traits<char>::length(string);

  _replace_raw(pos, count, string, stringLen);

  return *this;
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> & FixedString<allocatedSize>::replace(size_t pos, size_t count, char character,
                                                                           size_t charactersCount) noexcept {
  if (count == 0 && charactersCount == 0)
    return *this;

  assert_message(pos <= _storage.size, "Position must be within string bounds");
  assert_message(pos + count <= _storage.size, "Replacement range must be within string bounds");

  const auto newSize = _storage.size - count + charactersCount;

  assert_message(newSize < allocatedSize, "Replacement result must fit in capacity");

  // If sizes are equal, no need to shift data
  if (count == charactersCount) {
    if consteval {
      std::fill_n(_storage.buffer + pos, charactersCount, character);
    } else {
      std::memset(_storage.buffer + pos, character, charactersCount);
    }
  } else {
    // If replacing at the end, no need to shift
    if (pos + count == _storage.size) {
      if consteval {
        std::fill_n(_storage.buffer + pos, charactersCount, character);
      } else {
        std::memset(_storage.buffer + pos, character, charactersCount);
      }
      _storage.buffer[pos + charactersCount] = '\0';
    } else {
      char_traits<char>::move(_storage.buffer + pos + charactersCount, _storage.buffer + pos + count,
                              _storage.size - pos - count + 1);

      if consteval {
        std::fill_n(_storage.buffer + pos, charactersCount, character);
      } else {
        std::memset(_storage.buffer + pos, character, charactersCount);
      }
    }

    _storage.size = newSize;
  }

  return *this;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::copy(char * dest, size_t count, size_t pos) const noexcept {
  assert_message(pos <= _storage.size, "Position must be within string bounds");
  assert_message(dest != nullptr, "Destination pointer must not be null");
  assert_message((dest < _storage.buffer) || (dest >= (_storage.buffer + allocatedSize)),
                 "Destination buffer must not overlap internal storage");

  if (count == npos || pos + count > _storage.size)
    count = _storage.size - pos;

  char_traits<char>::move(dest, _storage.buffer + pos, count);

  return count;
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::swap(FixedString<allocatedSize> & string) noexcept {
  if (this == std::addressof(string))
    return;

  char tempData[allocatedSize];

  char_traits<char>::move(tempData, _storage.buffer, _storage.size + 1);
  char_traits<char>::move(_storage.buffer, string._storage.buffer, string._storage.size + 1);
  char_traits<char>::move(string._storage.buffer, tempData, _storage.size + 1);

  std::swap(_storage.size, string._storage.size);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find(const FixedString<allocatedSize> & string,
                                                  size_t position) const noexcept {
  return _find_raw(position, string._storage.buffer, string._storage.size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::find(const stringType & string, size_t position) const noexcept {
  return _find_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  const auto stringLen = char_traits<char>::length(string);

  return _find_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find(char character, size_t position) const noexcept {
  return _find_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::rfind(const FixedString<allocatedSize> & string,
                                                   size_t position) const noexcept {
  return _rfind_raw(position, string._storage.buffer, string._storage.size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::rfind(const stringType & string, size_t position) const noexcept {
  return _rfind_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::rfind(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  const auto stringLen = char_traits<char>::length(string);

  return _rfind_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::rfind(char character, size_t position) const noexcept {
  return _rfind_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_of(const FixedString<allocatedSize> & string,
                                                           size_t position) const noexcept {
  return _find_first_of_raw(position, string._storage.buffer, string._storage.size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::find_first_of(const stringType & string, size_t position) const noexcept {
  return _find_first_of_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_of(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  const auto stringLen = char_traits<char>::length(string);

  return _find_first_of_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_of(char character, size_t position) const noexcept {
  return _find_first_of_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_not_of(const FixedString<allocatedSize> & string,
                                                               size_t position) const noexcept {
  return _find_first_not_of_raw(position, string._storage.buffer, string._storage.size);
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

  const auto stringLen = char_traits<char>::length(string);

  return _find_first_not_of_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_first_not_of(char character, size_t position) const noexcept {
  return _find_first_not_of_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_of(const FixedString<allocatedSize> & string,
                                                          size_t position) const noexcept {
  return _find_last_of_raw(position, string._storage.buffer, string._storage.size);
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr size_t FixedString<allocatedSize>::find_last_of(const stringType & string, size_t position) const noexcept {
  return _find_last_of_raw(position, string.c_str(), string.size());
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_of(const char * string, size_t position) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  const auto stringLen = char_traits<char>::length(string);

  return _find_last_of_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_of(char character, size_t position) const noexcept {
  return _find_last_of_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_not_of(const FixedString<allocatedSize> & string,
                                                              size_t position) const noexcept {
  return _find_last_not_of_raw(position, string._storage.buffer, string._storage.size);
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

  const auto stringLen = char_traits<char>::length(string);

  return _find_last_not_of_raw(position, string, stringLen);
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::find_last_not_of(char character, size_t position) const noexcept {
  return _find_last_not_of_raw(position, &character, 1);
}

template <size_t allocatedSize>
constexpr int FixedString<allocatedSize>::compare(const FixedString<allocatedSize> & string) const noexcept {
  if consteval {
    return cstrcmp(_storage.buffer, string._storage.buffer);
  } else {
    return std::strcmp(_storage.buffer, string._storage.buffer);
  }
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr int FixedString<allocatedSize>::compare(const stringType & string) const noexcept {
  if consteval {
    return cstrcmp(_storage.buffer, string.c_str());
  } else {
    return std::strcmp(_storage.buffer, string.c_str());
  }
}

template <size_t allocatedSize>
constexpr int FixedString<allocatedSize>::compare(const char * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  if consteval {
    return cstrcmp(_storage.buffer, string);
  } else {
    return std::strcmp(_storage.buffer, string);
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::starts_with(const FixedString<allocatedSize> & string) const noexcept {
  return _storage.size >= string._storage.size
         && char_traits<char>::compare(_storage.buffer, string._storage.buffer, string._storage.size) == 0;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr bool FixedString<allocatedSize>::starts_with(const stringType & string) const noexcept {
  const auto stringSize = string.size();
  if (size() < stringSize)
    return false;

  return char_traits<char>::compare(_storage.buffer, string.c_str(), stringSize) == 0;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::starts_with(const char * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  const auto needleSize = char_traits<char>::length(string);

  return _storage.size >= needleSize && char_traits<char>::compare(_storage.buffer, string, needleSize) == 0;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::starts_with(char character) const noexcept {
  return !empty() && _storage.buffer[0] == character;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::ends_with(const FixedString<allocatedSize> & string) const noexcept {
  return _storage.size >= string._storage.size
         && char_traits<char>::compare(_storage.buffer + _storage.size - string._storage.size, string._storage.buffer,
                                       string._storage.size)
              == 0;
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr bool FixedString<allocatedSize>::ends_with(const stringType & string) const noexcept {
  const auto stringSize = string.size();
  if (_storage.size < stringSize)
    return false;

  return char_traits<char>::compare(_storage.buffer + _storage.size - stringSize, string.c_str(), stringSize) == 0;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::ends_with(const char * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  const auto needleSize = char_traits<char>::length(string);
  if (_storage.size < needleSize)
    return false;

  return char_traits<char>::compare(_storage.buffer + _storage.size - needleSize, string, needleSize) == 0;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::ends_with(char character) const noexcept {
  return !empty() && _storage.buffer[_storage.size - 1] == character;
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::contains(const FixedString<allocatedSize> & string) const noexcept {
  if consteval {
    return _storage.size >= string._storage.size && cstrstr(_storage.buffer, string.c_str()) != nullptr;
  } else {
    return _storage.size >= string._storage.size && std::strstr(_storage.buffer, string.c_str()) != nullptr;
  }
}

template <size_t allocatedSize>
template <StringLike stringType>
constexpr bool FixedString<allocatedSize>::contains(const stringType & string) const noexcept {
  if consteval {
    return cstrstr(_storage.buffer, string.c_str()) != nullptr;
  } else {
    return std::strstr(_storage.buffer, string.c_str()) != nullptr;
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::contains(const char * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  if consteval {
    return cstrstr(_storage.buffer, string) != nullptr;
  } else {
    return std::strstr(_storage.buffer, string) != nullptr;
  }
}

template <size_t allocatedSize>
constexpr bool FixedString<allocatedSize>::contains(char character) const noexcept {
  if (empty())
    return false;

  if consteval {
    return cstrchr(_storage.buffer, character) != nullptr;
  } else {
    return std::memchr(_storage.buffer, character, _storage.size) != nullptr;
  }
}

template <size_t allocatedSize>
constexpr FixedString<allocatedSize> FixedString<allocatedSize>::substr(size_t position, size_t count) const noexcept {
  assert_message(position <= _storage.size, "Position must not exceed string size");

  const auto remaining = _storage.size - position;
  if (count == npos || count > remaining)
    count = remaining;

  assert_message(count <= remaining, "Substring range must be within string bounds");

  FixedString<allocatedSize> result;

  result._storage.size = count;

  char_traits<char>::move(result._storage.buffer, _storage.buffer + position, count);

  result._storage.buffer[count] = '\0';

  return result;
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::_insert_raw(size_t position, const char * data, size_t dataSize) noexcept {
  if (dataSize == 0)
    return;

  assert_message(((data + dataSize) < _storage.buffer) || (data >= (_storage.buffer + allocatedSize)),
                 "Source data pointer must not point into _storage.buffer buffer");

  assert_message(position <= _storage.size, "Index must not exceed string size");
  assert_message(_storage.size + dataSize < allocatedSize, "Inserted string must fit in capacity");

  // If inserting at the end, just append
  if (position == _storage.size) {
    char_traits<char>::move(_storage.buffer + _storage.size, data, dataSize + 1);
  } else {
    char_traits<char>::move(_storage.buffer + position + dataSize, _storage.buffer + position,
                            _storage.size - position + 1);

    char_traits<char>::move(_storage.buffer + position, data, dataSize);
  }

  _storage.size += dataSize;
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::_append_raw(const char * data, size_t dataSize) noexcept {
  if (dataSize == 0)
    return;

  assert_message(_storage.size + dataSize < allocatedSize, "Appended data must fit in capacity");
  if !consteval {
    assert_message(((data + dataSize) < _storage.buffer) || (data >= (_storage.buffer + allocatedSize)),
                   "Source data pointer must not point into _storage.buffer buffer");
  }

  char_traits<char>::move(_storage.buffer + _storage.size, data, dataSize + 1);
  _storage.size += dataSize;
}

template <size_t allocatedSize>
constexpr void FixedString<allocatedSize>::_replace_raw(size_t position, size_t oldCount, const char * data,
                                                        size_t dataSize) noexcept {
  if (oldCount == 0 && dataSize == 0)
    return;

  assert_message(((data + dataSize) < _storage.buffer) || (data >= (_storage.buffer + allocatedSize)),
                 "Source data pointer must not point into _storage.buffer buffer");

  assert_message(position <= _storage.size, "Position must be within string bounds");
  assert_message(position + oldCount <= _storage.size, "Replacement range must be within string bounds");

  // If sizes are equal, no need to shift data
  if (oldCount == dataSize) {
    char_traits<char>::move(_storage.buffer + position, data, dataSize);

    return;
  }

  assert_message((_storage.size - oldCount + dataSize) < allocatedSize, "Replacement result must fit in capacity");

  // If replacing at the end, no need to shift
  if (position + oldCount == _storage.size) {
    char_traits<char>::move(_storage.buffer + position, data, dataSize + 1);
  } else {
    char_traits<char>::move(_storage.buffer + position + dataSize, _storage.buffer + position + oldCount,
                            _storage.size - position - oldCount + 1);
    char_traits<char>::move(_storage.buffer + position, data, dataSize);
  }

  _storage.size = _storage.size - oldCount + dataSize;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_find_raw(size_t position, const char * data,
                                                       size_t dataSize) const noexcept {
  if (position > _storage.size)
    return npos;

  if (dataSize == 0)
    return position;
  else if (dataSize > _storage.size - position)
    return npos;

  const char * occurrence;

  if consteval {
    occurrence
      = dataSize == 1 ? cstrchr(_storage.buffer + position, data[0]) : cstrstr(_storage.buffer + position, data);
  } else {
    occurrence
      = dataSize == 1
          ? static_cast<const char *>(std::memchr(_storage.buffer + position, data[0], _storage.size - position))
          : std::strstr(_storage.buffer + position, data);
  }

  return occurrence != nullptr ? occurrence - _storage.buffer : npos;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_rfind_raw(size_t position, const char * data,
                                                        size_t dataSize) const noexcept {
  if (dataSize == 0)
    return std::min(position, _storage.size);
  else if (dataSize > _storage.size)
    return npos;

  if (position == npos)
    position = _storage.size - dataSize;
  else if (position > _storage.size - dataSize)
    return npos;

  for (size_t i = 0; i <= position; ++i) {
    const auto offset = position - i;

    const auto found = char_traits<char>::compare(_storage.buffer + offset, data, dataSize) == 0;
    if (found)
      return offset;
  }

  return npos;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_find_first_of_raw(size_t position, const char * data,
                                                                size_t dataSize) const noexcept {
  if (position >= _storage.size || dataSize == 0)
    return npos;

  const char * occurrence;

  if consteval {
    occurrence
      = dataSize == 1 ? cstrchr(_storage.buffer + position, data[0]) : cstrpbrk(_storage.buffer + position, data);
  } else {
    occurrence = dataSize == 1 ? std::strchr(_storage.buffer + position, data[0])
                               : std::strpbrk(_storage.buffer + position, data);
  }

  return occurrence != nullptr ? occurrence - _storage.buffer : npos;
}

template <size_t allocatedSize>
constexpr size_t FixedString<allocatedSize>::_find_first_not_of_raw(size_t position, const char * data,
                                                                    size_t dataSize) const noexcept {
  if (position >= _storage.size)
    return npos;

  if (dataSize == 0)
    return position;

  if (dataSize == 1) {
    const auto exclude = data[0];
    for (auto i = position; i < _storage.size; ++i) {
      if (_storage.buffer[i] != exclude)
        return i;
    }
  } else {
    std::array<bool, 256> excludedChars{};

    for (size_t i = 0; i < dataSize; ++i) {
      excludedChars[static_cast<unsigned char>(data[i])] = true;
    }

    for (auto i = position; i < _storage.size; ++i) {
      if (!excludedChars[static_cast<unsigned char>(_storage.buffer[i])])
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
    position = _storage.size - 1;
  else if (position >= _storage.size)
    return npos;

  if (dataSize == 1) {
    const auto target = data[0];
    for (size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (_storage.buffer[scanIndex] == target)
        return scanIndex;
    }
  } else {
    std::array<bool, 256> targetChars{};

    for (size_t i = 0; i < dataSize; ++i) {
      targetChars[static_cast<unsigned char>(data[i])] = true;
    }

    for (size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (targetChars[static_cast<unsigned char>(_storage.buffer[scanIndex])])
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
    position = _storage.size - 1;
  else if (position >= _storage.size)
    return npos;

  if (dataSize == 0)
    return position;

  if (dataSize == 1) {
    const auto exclude = data[0];
    for (size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (_storage.buffer[scanIndex] != exclude)
        return scanIndex;
    }
  } else {
    std::array<bool, 256> excludedChars{};

    for (size_t i = 0; i < dataSize; ++i) {
      excludedChars[static_cast<unsigned char>(data[i])] = true;
    }

    for (size_t i = 0; i <= position; ++i) {
      const auto scanIndex = position - i;
      if (!excludedChars[static_cast<unsigned char>(_storage.buffer[scanIndex])])
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

template <size_t allocatedSize, StringLike stringType>
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
  FixedString<allocatedSize> result(1, lhs);

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

  return char_traits<char>::compare(lhs.c_str(), rhs.c_str(), lhs.size()) == 0;
}

template <size_t allocatedSize, StringLike stringType>
constexpr bool operator==(const FixedString<allocatedSize> & lhs, const stringType & rhs) noexcept {
  if (lhs.size() != rhs.size())
    return false;
  else if (lhs.empty())
    return true;

  return char_traits<char>::compare(lhs.c_str(), rhs.c_str(), lhs.size()) == 0;
}

template <size_t allocatedSize, StringLike stringType>
constexpr bool operator==(const stringType & lhs, const FixedString<allocatedSize> & rhs) noexcept {
  return rhs == lhs;
}

template <size_t allocatedSize>
constexpr bool operator==(const FixedString<allocatedSize> & lhs, const char * rhs) noexcept {
  assert_message(rhs != nullptr, "C string must not be null");

  if (lhs.empty())
    return *rhs == '\0';

  return lhs.size() == char_traits<char>::length(rhs) && char_traits<char>::compare(lhs.c_str(), rhs, lhs.size()) == 0;
}

template <size_t allocatedSize>
constexpr bool operator==(const char * lhs, const FixedString<allocatedSize> & rhs) noexcept {
  return rhs == lhs;
}

template <size_t allocatedSize1, size_t allocatedSize2>
constexpr strong_ordering operator<=>(const FixedString<allocatedSize1> & lhs,
                                      const FixedString<allocatedSize2> & rhs) noexcept {
  if constexpr (allocatedSize1 == allocatedSize2) {
    if (std::addressof(lhs) == std::addressof(rhs))
      return strong_ordering::equal;
  }

  if (lhs.empty() && rhs.empty())
    return strong_ordering::equal;
  else if (lhs.empty())
    return strong_ordering::less;
  else if (rhs.empty())
    return strong_ordering::greater;

  if consteval {
    return cstrcmp(lhs.c_str(), rhs.c_str()) <=> 0;
  } else {
    const int result = std::memcmp(lhs.c_str(), rhs.c_str(), std::min(lhs.size(), rhs.size()));

    if (result < 0)
      return strong_ordering::less;
    else if (result > 0)
      return strong_ordering::greater;
    else {
      if (lhs.size() < rhs.size())
        return strong_ordering::less;
      else if (lhs.size() > rhs.size())
        return strong_ordering::greater;
      else
        return strong_ordering::equal;
    }
  }
}

template <size_t allocatedSize, StringLike stringType>
constexpr strong_ordering operator<=>(const FixedString<allocatedSize> & lhs, const stringType & rhs) noexcept {
  if (lhs.empty() && rhs.size() == 0)
    return strong_ordering::equal;
  else if (lhs.empty())
    return strong_ordering::less;
  else if (rhs.size() == 0)
    return strong_ordering::greater;

  if consteval {
    return cstrcmp(lhs.c_str(), rhs.c_str()) <=> 0;
  } else {
    const int result = std::memcmp(lhs.c_str(), rhs.c_str(), std::min(lhs.size(), rhs.size()));

    if (result < 0)
      return strong_ordering::less;
    else if (result > 0)
      return strong_ordering::greater;
    else {
      if (lhs.size() < rhs.size())
        return strong_ordering::less;
      else if (lhs.size() > rhs.size())
        return strong_ordering::greater;
      else
        return strong_ordering::equal;
    }
  }
}

template <size_t allocatedSize, StringLike stringType>
constexpr strong_ordering operator<=>(const stringType & lhs, const FixedString<allocatedSize> & rhs) noexcept {
  return 0 <=> (rhs <=> lhs);
}

template <size_t allocatedSize>
constexpr strong_ordering operator<=>(const FixedString<allocatedSize> & lhs, const char * rhs) noexcept {
  assert_message(rhs != nullptr, "C string must not be null");

  if (lhs.empty() && *rhs == '\0')
    return strong_ordering::equal;
  else if (lhs.empty())
    return strong_ordering::less;
  else if (*rhs == '\0')
    return strong_ordering::greater;

  const auto rhsLen = char_traits<char>::length(rhs);
  const int result = char_traits<char>::compare(lhs.c_str(), rhs, std::min(lhs.size(), rhsLen));
  if (result < 0)
    return strong_ordering::less;
  else if (result > 0)
    return strong_ordering::greater;
  else {
    if (lhs.size() < rhsLen)
      return strong_ordering::less;
    else if (lhs.size() > rhsLen)
      return strong_ordering::greater;
    else
      return strong_ordering::equal;
  }
}

template <size_t allocatedSize>
constexpr strong_ordering operator<=>(const char * lhs, const FixedString<allocatedSize> & rhs) noexcept {
  assert_message(lhs != nullptr, "C string must not be null");

  return 0 <=> (rhs <=> lhs);
}

} // namespace toy

#endif // INCLUDE_CORE_FIXED_STRING_INL_
