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
/*
  \file   core_inline.inl
  \brief  collection of core inline functions
*/

#ifndef INCLUDE_CORE_INLINE_INL_
#define INCLUDE_CORE_INLINE_INL_

namespace toygine {
//------------------------------------------------------------------------------

inline wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, char const * src) {
  assert_message(src != nullptr, "The source must not be null.");

  return src != nullptr ? utf8toWChar(dest, destSize, src, std::strlen(src)) : nullptr;
}

template <typename stringType>
inline wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, stringType const & src) {
  return utf8toWChar(dest, destSize, src.c_str(), src.size());
}

inline char * reverseString(char * string, std::size_t stringLength) {
  assert_message(string != nullptr, "The source string must not be null.");
  if (string == nullptr)
    return nullptr;

  if (stringLength == 0)
    stringLength = std::strlen(string);

  if (stringLength != 0) {
    for (std::size_t i = 0, j = stringLength - 1; i < j; ++i, --j) {
      std::swap(string[i], string[j]);
    }
  }

  return string;
}

//------------------------------------------------------------------------------

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString() noexcept
  : _size(0)
  , _data{'\0'} {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::~FixString() noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString(const FixString<allocatedSize> & string) noexcept
  : _size(string._size) {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");

  std::memcpy(_data, string._data, _size + 1);
}

template <std::size_t allocatedSize>
template <std::size_t allocatedSize2>
constexpr inline FixString<allocatedSize>::FixString(const FixString<allocatedSize2> & string) noexcept
  : _size(string.size()) {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  static_assert(allocatedSize2 > 0, "FixString capacity must be greater than zero.");
  assert(_size < allocatedSize);

  if (_size >= allocatedSize)
    _size = allocatedSize - 1;

  std::memcpy(_data, string.c_str(), _size);
  _data[_size] = '\0';
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString(const char * string) noexcept
  : _size(0) {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(string != nullptr);

  const std::size_t inputLen = string ? std::strlen(string) : 0;
  _size = inputLen >= allocatedSize ? allocatedSize - 1 : inputLen;
  std::memcpy(_data, string, _size);
  _data[_size] = '\0';
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize>::FixString(char symbol, std::size_t count) noexcept
  : _size(count) {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(count < allocatedSize);

  if (_size >= allocatedSize)
    _size = allocatedSize - 1;

  std::memset(_data, symbol, _size);
  _data[_size] = '\0';
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(
  const FixString<allocatedSize> & string) noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  if (this == &string)
    return *this;

  _size = string._size;
  std::memcpy(_data, string._data, _size + 1);

  return *this;
}

template <std::size_t allocatedSize>
template <std::size_t allocatedSize2>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(
  const FixString<allocatedSize2> & string) noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  static_assert(allocatedSize2 > 0, "FixString capacity must be greater than zero.");
  assert(reinterpret_cast<const void *>(this) != reinterpret_cast<const void *>(&string));
  assert(string.size() < allocatedSize);

  _size = string.size();
  std::memcpy(_data, string.c_str(), _size + 1);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(const char * string) noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(string != nullptr);
  if (_data == string)
    return *this;

  _size = std::strlen(string);
  assert(_size < allocatedSize);

  std::memcpy(_data, string, _size + 1);

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator=(char symbol) noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");

  _size = 1;
  _data[0] = symbol;
  _data[1] = '\0';

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(
  const FixString<allocatedSize> & string) noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(_size + string._size < allocatedSize);

  std::memcpy(_data + _size, string._data, string._size + 1);
  _size += string._size;

  return *this;
}

template <std::size_t allocatedSize>
template <std::size_t allocatedSize2>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(
  const FixString<allocatedSize2> & string) noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  static_assert(allocatedSize2 > 0, "FixString capacity must be greater than zero.");
  assert(_size + string.size() < allocatedSize);

  std::memcpy(_data + _size, string.c_str(), string.size() + 1);
  _size += string.size();

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(const char * string) noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(string != nullptr);

  const auto stringSize = std::strlen(string);
  assert(_size + stringSize < allocatedSize);

  std::memcpy(_data + _size, string, stringSize + 1);
  _size += stringSize;

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> & FixString<allocatedSize>::operator+=(char symbol) noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(_size + 1 < allocatedSize);

  _data[_size++] = symbol;
  _data[_size] = '\0';

  return *this;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> FixString<allocatedSize>::operator+(
  const FixString<allocatedSize> & string) const noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(_size + string._size < allocatedSize);

  FixString<allocatedSize> value(*this);
  value += string;
  return value;
}

template <std::size_t allocatedSize>
template <std::size_t allocatedSize2>
constexpr inline FixString<allocatedSize> FixString<allocatedSize>::operator+(
  const FixString<allocatedSize2> & string) const noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  static_assert(allocatedSize2 > 0, "FixString capacity must be greater than zero.");
  assert(_size + string.size() < allocatedSize);

  FixString<allocatedSize> value(*this);
  value += string;
  return value;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> FixString<allocatedSize>::operator+(const char * string) const noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(string != nullptr);

  FixString<allocatedSize> value(*this);
  value += string;
  return value;
}

template <std::size_t allocatedSize>
constexpr inline FixString<allocatedSize> FixString<allocatedSize>::operator+(char symbol) const noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");

  FixString<allocatedSize> value(*this);
  value += symbol;
  return value;
}

template <std::size_t allocatedSize>
constexpr inline char & FixString<allocatedSize>::operator[](std::size_t offset) noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(offset < _size);

  return _data[offset];
}

template <std::size_t allocatedSize>
constexpr inline char FixString<allocatedSize>::operator[](std::size_t offset) const noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");
  assert(offset < _size);

  return _data[offset];
}

template <std::size_t allocatedSize>
constexpr inline const char * FixString<allocatedSize>::c_str() const noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");

  return _data;
}

template <std::size_t allocatedSize>
constexpr inline std::size_t FixString<allocatedSize>::size() const noexcept {
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");

  return _size;
}

} // namespace toygine

#endif // INCLUDE_CORE_INLINE_INL_
