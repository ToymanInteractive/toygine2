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

#include <cstring>

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
template <StringLikeForFixString stringType>
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
constexpr inline FixString<allocatedSize>::FixString(char symbol, std::size_t count) noexcept
  : _size(count) {
  assert_message(count < allocatedSize, "Count must not exceed capacity");

  std::memset(_data, symbol, _size);
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
template <StringLikeForFixString stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(const stringType & string) noexcept {
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
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(char symbol) noexcept {
  static_assert(allocatedSize > 1, "FixString capacity must be greater than one.");

  _size = 1;
  _data[0] = symbol;
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
template <StringLikeForFixString stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::assign(const stringType & string) noexcept {
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
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::assign(char symbol, std::size_t count) noexcept {
  assert_message(count < allocatedSize, "Count must not exceed capacity");

  _size = count;
  std::memset(_data, symbol, _size);
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
constexpr inline std::size_t FixString<allocatedSize>::utf8Size() const noexcept {
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
template <StringLikeForFixString stringType>
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
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::insert(std::size_t index, char symbol,
                                                                             std::size_t count) noexcept {
  assert_message(index <= _size, "Index must not exceed string size");
  assert_message(_size + count < allocatedSize, "Inserted characters must fit in capacity");

  std::memmove(_data + index + count, _data + index, _size - index + 1);
  std::memset(_data + index, symbol, count);

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
constexpr inline void FixString<allocatedSize>::push_back(char symbol) noexcept {
  assert_message(_size + 1 < allocatedSize, "String must have space for new character");

  _data[_size++] = symbol;
  _data[_size] = '\0';
}

template <std::size_t allocatedSize>
constexpr inline void FixString<allocatedSize>::pop_back() noexcept {
  assert_message(_size > 0, "String must not be empty for pop_back");

  if (_size > 0)
    _data[--_size] = '\0';
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
template <StringLikeForFixString stringType>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::append(const stringType & string) noexcept {
  const auto sourceSize = string.size();

  assert_message(_data != string.c_str(), "Cannot append string into itself");
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
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::append(char symbol, std::size_t count) noexcept {
  assert_message(_size + count < allocatedSize, "Appended string must fit in capacity");

  std::memset(_data + _size, symbol, count);
  _size += count;
  _data[_size] = '\0';

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(
  const FixString<allocatedSize> & string) noexcept {
  assert(_size + string._size < allocatedSize);

  std::memcpy(_data + _size, string._data, string._size + 1);
  _size += string._size;

  return *this;
}

template <std::size_t allocatedSize>
template <std::size_t allocatedSize2>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(
  const FixString<allocatedSize2> & string) noexcept {
  static_assert(allocatedSize2 > 0, "FixString capacity must be greater than zero.");
  assert(_size + string.size() < allocatedSize);

  std::memcpy(_data + _size, string.c_str(), string.size() + 1);
  _size += string.size();

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(const char * string) noexcept {
  assert(string != nullptr);

  const auto stringSize = std::strlen(string);
  assert(_size + stringSize < allocatedSize);

  std::memcpy(_data + _size, string, stringSize + 1);
  _size += stringSize;

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(char symbol) noexcept {
  assert(_size + 1 < allocatedSize);

  _data[_size++] = symbol;
  _data[_size] = '\0';

  return *this;
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

} // namespace toygine

#endif // INCLUDE_CORE_FIX_STRING_INL_
