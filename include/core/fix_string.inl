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
inline FixString<allocatedSize>::~FixString() noexcept {}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString(const FixString<allocatedSize> & string) noexcept
  : _size(string.size()) {
  std::memcpy(_data, string.data(), _size + 1);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize>::FixString(const stringType & string) noexcept
  : _size(string.size()) {
  assert_message(_size < allocatedSize, "String size must not exceed capacity");

  std::memcpy(_data, string.c_str(), _size + 1);
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString(const char * string) noexcept
  : _size(0) {
  assert_message(string != nullptr, "String pointer must not be null");

  _size = std::strlen(string);
  assert_message(_size < allocatedSize, "String length must not exceed capacity");

  std::memcpy(_data, string, _size + 1);
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString(char character, std::size_t count) noexcept
  : _size(count) {
  assert_message(count < allocatedSize, "Count must not exceed capacity");

  std::memset(_data, character, _size);
  _data[_size] = '\0';
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(
  const FixString<allocatedSize> & string) noexcept {
  if (this == &string)
    return *this;

  _size = string.size();
  std::memcpy(_data, string.data(), _size + 1);

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(const stringType & string) noexcept {
  if (_data == string.c_str())
    return *this;

  assert_message(string.size() < allocatedSize, "String size must not exceed capacity");

  _size = string.size();
  std::memcpy(_data, string.c_str(), _size + 1);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(const char * string) noexcept {
  if (_data == string)
    return *this;

  assert_message(string != nullptr, "String pointer must not be null");

  _size = std::strlen(string);

  assert_message(_size < allocatedSize, "String length must not exceed capacity");

  std::memcpy(_data, string, _size + 1);

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
  std::memcpy(_data, string.data(), _size + 1);

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
  std::memcpy(_data, string.c_str(), _size + 1);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::assign(const char * string) noexcept {
  if (_data == string)
    return *this;

  assert_message(string != nullptr, "String pointer must not be null");

  _size = std::strlen(string);

  assert_message(_size < allocatedSize, "String length must not exceed capacity");

  std::memcpy(_data, string, _size + 1);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::assign(char character,
                                                                             std::size_t count) noexcept {
  assert_message(count < allocatedSize, "Count must not exceed capacity");

  _size = count;
  std::memset(_data, character, _size);
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
  assert_message(this != &string, "Cannot insert string into itself");
  assert_message(index <= _size, "Index must not exceed string size");
  assert_message(_size + string.size() < allocatedSize, "Inserted string must fit in capacity");

  std::memmove(_data + index + string.size(), _data + index, _size - index + 1);
  std::memcpy(_data + index, string.data(), string.size());

  _size += string.size();

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::insert(std::size_t index,
                                                                             const stringType & string) noexcept {
  assert_message(_data != string.c_str(), "Cannot insert string into itself");
  assert_message(index <= _size, "Index must not exceed string size");
  assert_message(_size + string.size() < allocatedSize, "Inserted string must fit in capacity");

  std::memmove(_data + index + string.size(), _data + index, _size - index + 1);
  std::memcpy(_data + index, string.c_str(), string.size());

  _size += string.size();

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::insert(std::size_t index,
                                                                             const char * string) noexcept {
  assert_message(_data != string, "Cannot insert string into itself");
  assert_message(string != nullptr, "String pointer must not be null");
  assert_message(index <= _size, "Index must not exceed string size");

  const auto stringSize = std::strlen(string);

  assert_message(_size + stringSize < allocatedSize, "Inserted string must fit in capacity");

  std::memmove(_data + index + stringSize, _data + index, _size - index + 1);
  std::memcpy(_data + index, string, stringSize);

  _size += stringSize;

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::insert(std::size_t index, char character,
                                                                             std::size_t count) noexcept {
  assert_message(index <= _size, "Index must not exceed string size");
  assert_message(_size + count < allocatedSize, "Inserted characters must fit in capacity");

  std::memmove(_data + index + count, _data + index, _size - index + 1);
  std::memset(_data + index, character, count);

  _size += count;

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
  assert_message(_size + string._size < allocatedSize, "Appended string must fit in capacity");

  std::memcpy(_data + _size, string._data, string._size + 1);
  _size += string._size;

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::append(const stringType & string) noexcept {
  assert_message(_data != string.c_str(), "Cannot append string into itself");

  const auto sourceSize = string.size();

  assert_message(_size + sourceSize < allocatedSize, "Appended string must fit in capacity");

  std::memcpy(_data + _size, string.c_str(), sourceSize + 1);
  _size += sourceSize;

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::append(const char * string) noexcept {
  assert_message(_data != string, "Cannot append string into itself");
  assert_message(string != nullptr, "String pointer must not be null");

  const auto stringSize = std::strlen(string);
  assert_message(_size + stringSize < allocatedSize, "Appended string must fit in capacity");

  std::memcpy(_data + _size, string, stringSize + 1);
  _size += stringSize;

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::append(char character,
                                                                             std::size_t count) noexcept {
  assert_message(_size + count < allocatedSize, "Appended string must fit in capacity");

  std::memset(_data + _size, character, count);
  _size += count;
  _data[_size] = '\0';

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(
  const FixString<allocatedSize> & string) noexcept {
  assert_message(this != &string, "Cannot append string into itself");
  assert_message(_size + string._size < allocatedSize, "Appended string must fit in capacity");

  std::memcpy(_data + _size, string._data, string._size + 1);
  _size += string._size;

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(const stringType & string) noexcept {
  assert_message(_data != string.c_str(), "Cannot append string into itself");

  const auto sourceSize = string.size();

  assert_message(_size + sourceSize < allocatedSize, "Appended string must fit in capacity");

  std::memcpy(_data + _size, string.c_str(), sourceSize + 1);
  _size += sourceSize;

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(const char * string) noexcept {
  assert_message(_data != string, "Cannot append string into itself");
  assert_message(string != nullptr, "String pointer must not be null");

  const auto stringSize = std::strlen(string);
  assert_message(_size + stringSize < allocatedSize, "Appended string must fit in capacity");

  std::memcpy(_data + _size, string, stringSize + 1);
  _size += stringSize;

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
  assert_message(this != &string, "Cannot replace string into itself");
  assert_message(pos <= _size, "Position must be within string bounds");
  assert_message(pos + count <= _size, "Replacement range must be within string bounds");

  const auto newSize = _size - count + string._size;

  assert_message(newSize < allocatedSize, "Replacement result must fit in capacity");

  if (count != string._size)
    std::memmove(_data + pos + string._size, _data + pos + count, _size - pos - count + 1);

  std::memcpy(_data + pos, string._data, string._size);

  _size = newSize;

  return *this;
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::replace(std::size_t pos, std::size_t count,
                                                                              const stringType & string) noexcept {
  assert_message(_data != string.c_str(), "Cannot replace string into itself");
  assert_message(pos <= _size, "Position must be within string bounds");
  assert_message(pos + count <= _size, "Replacement range must be within string bounds");

  const auto replaceSize = string.size();
  const auto newSize = _size - count + replaceSize;

  assert_message(newSize < allocatedSize, "Replacement result must fit in capacity");

  if (count != replaceSize)
    std::memmove(_data + pos + replaceSize, _data + pos + count, _size - pos - count + 1);

  std::memcpy(_data + pos, string.c_str(), replaceSize);
  _size = newSize;

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::replace(std::size_t pos, std::size_t count,
                                                                              const char * string) noexcept {
  assert_message(string != nullptr, "String pointer must not be null");
  assert_message((string < _data) || (string >= (_data + ArraySize(_data))),
                 "Source pointer must not point into _data buffer");
  assert_message(pos <= _size, "Position must be within string bounds");
  assert_message(pos + count <= _size, "Replacement range must be within string bounds");

  const auto replaceSize = std::strlen(string);
  const auto newSize = _size - count + replaceSize;

  assert_message(newSize < allocatedSize, "Replacement result must fit in capacity");

  if (count != replaceSize)
    std::memmove(_data + pos + replaceSize, _data + pos + count, _size - pos - count + 1);

  std::memcpy(_data + pos, string, replaceSize);
  _size = newSize;

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::replace(std::size_t pos, std::size_t count,
                                                                              char character,
                                                                              std::size_t charactersCount) noexcept {
  assert_message(pos <= _size, "Position must be within string bounds");
  assert_message(pos + count <= _size, "Replacement range must be within string bounds");

  const auto newSize = _size - count + charactersCount;

  assert_message(newSize < allocatedSize, "Replacement result must fit in capacity");

  if (count != charactersCount)
    std::memmove(_data + pos + charactersCount, _data + pos + count, _size - pos - count + 1);

  std::memset(_data + pos, character, charactersCount);
  _size = newSize;

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::copy(char * dest, std::size_t count,
                                                            std::size_t pos) const noexcept {
  assert_message(pos <= _size, "Position must be within string bounds");
  assert_message(dest != nullptr, "Destination pointer must not be null");
  assert_message((dest < _data) || (dest >= (_data + ArraySize(_data))),
                 "Destination buffer must not overlap internal storage");

  if (count == npos || pos + count > _size)
    count = _size - pos;

  std::memcpy(dest, _data + pos, count);

  return count;
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::swap(FixString<allocatedSize> & string) noexcept {
  if (this == &string)
    return;

  char tempData[allocatedSize];
  std::memcpy(tempData, _data, _size + 1);
  std::memcpy(_data, string._data, string._size + 1);
  std::memcpy(string._data, tempData, _size + 1);

  std::swap(_size, string._size);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find(const FixString<allocatedSize> & string,
                                                            std::size_t position) const noexcept {
  if (position > _size)
    return npos;
  if (string.empty())
    return position;
  if (string._size > _size - position)
    return npos;

  return find(string._data, position);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline std::size_t FixString<allocatedSize>::find(const stringType & string,
                                                            std::size_t position) const noexcept {
  if (position > _size)
    return npos;

  const auto needleSize = string.size();
  if (needleSize == 0)
    return position;
  if (needleSize > _size - position)
    return npos;

  return find(string.c_str(), position);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find(const char * string, std::size_t position) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  if (position >= _size)
    return npos;

  auto occurrence = std::strstr(_data + position, string);
  if (occurrence != nullptr)
    return occurrence - _data;

  return npos;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::find(char character, std::size_t position) const noexcept {
  if (position >= _size)
    return npos;

  auto occurrence = std::strchr(_data + position, character);
  if (occurrence != nullptr)
    return occurrence - _data;

  return npos;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::rfind(const FixString<allocatedSize> & string,
                                                             std::size_t position) const noexcept {
  return _rfind_raw(string._data, string._size, position);
}

template <std::size_t allocatedSize>
template <StringLike stringType>
constexpr inline std::size_t FixString<allocatedSize>::rfind(const stringType & string,
                                                             std::size_t position) const noexcept {
  return _rfind_raw(string.c_str(), string.size(), position);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::rfind(const char * string, std::size_t position) const noexcept {
  assert_message(string != nullptr, "String pointer must not be null");

  return _rfind_raw(string, std::strlen(string), position);
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::rfind(char character, std::size_t position) const noexcept {
  return _rfind_raw(&character, 1, position);
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
constexpr inline std::size_t FixString<allocatedSize>::_rfind_raw(const char * needle, std::size_t needleSize,
                                                                  std::size_t position) const noexcept {
  if (needleSize == 0)
    return position <= _size ? position : _size;

  if (needleSize > _size)
    return npos;
  if (position == npos)
    position = _size - needleSize;
  else if (position > _size - needleSize)
    return npos;

  for (auto i = 0U; i <= position; ++i) {
    const auto offset = position - i;
    if (std::memcmp(_data + offset, needle, needleSize) == 0)
      return offset;
  }

  return npos;
}

} // namespace toygine

#endif // INCLUDE_CORE_FIX_STRING_INL_
