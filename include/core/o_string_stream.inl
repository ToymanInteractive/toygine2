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
  \file   o_string_stream.inl
  \brief  Implementation of OStringStream template methods.
*/

#ifndef INCLUDE_CORE_O_STRING_STREAM_INL_
#define INCLUDE_CORE_O_STRING_STREAM_INL_

namespace toy {

template <typename StringType>
template <StringLike SourceStringType>
constexpr OStringStream<StringType>::OStringStream(const SourceStringType & string) noexcept
  : _string(string) {}

template <typename StringType>
constexpr void OStringStream<StringType>::swap(OStringStream & other) noexcept {
  std::swap(_string, other._string);
  std::swap(_precision, other._precision);
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(bool value) noexcept {
  _string.append(value ? "true" : "false");

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(long value) noexcept {
  char buffer[21];

  if constexpr (sizeof(value) == sizeof(int32_t)) {
    itoa(buffer, size(buffer), static_cast<int32_t>(value));
  } else if constexpr (sizeof(value) == sizeof(int64_t)) {
    itoa(buffer, size(buffer), static_cast<int64_t>(value));
  } else {
    static_assert(sizeof(value) == sizeof(int32_t) || sizeof(value) == sizeof(int64_t), "Unsupported value size");
  }

  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(unsigned long value) noexcept {
  char buffer[21];

  if constexpr (sizeof(value) == sizeof(uint32_t)) {
    utoa(buffer, size(buffer), static_cast<uint32_t>(value));
  } else if constexpr (sizeof(value) == sizeof(uint64_t)) {
    utoa(buffer, size(buffer), static_cast<uint64_t>(value));
  } else {
    static_assert(sizeof(value) == sizeof(uint32_t) || sizeof(value) == sizeof(uint64_t), "Unsupported value size");
  }

  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(long long value) noexcept {
  char buffer[21];

  static_assert(sizeof(value) == sizeof(int64_t), "Unsupported value size");
  itoa(buffer, size(buffer), static_cast<int64_t>(value));
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(unsigned long long value) noexcept {
  char buffer[21];

  static_assert(sizeof(value) == sizeof(uint64_t), "Unsupported value size");
  utoa(buffer, size(buffer), static_cast<uint64_t>(value));
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(double value) noexcept {
  char buffer[128];

  static_assert(sizeof(value) == 8, "Unsupported value size");
  ftoa(buffer, size(buffer), value, _precision);
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(const void * value) noexcept {
  if (value == nullptr)
    return *this << nullptr;

  char buffer[17];

  if constexpr (sizeof(value) == 4) {
    utoa(buffer, size(buffer), static_cast<uint32_t>(reinterpret_cast<uintptr_t>(value)), 16);
  } else if constexpr (sizeof(value) == 8) {
    utoa(buffer, size(buffer), static_cast<uint64_t>(reinterpret_cast<uintptr_t>(value)), 16);
  } else {
    static_assert(sizeof(value) == 4 || sizeof(value) == 8, "Unsupported pointer size");
  }

  _string.append("0x");

  const auto bufferDataLen = char_traits<char>::length(buffer);
  const int leadingZeros = static_cast<int>(sizeof(value)) * 2 - static_cast<int>(bufferDataLen);
  if (leadingZeros > 0)
    _string.append(leadingZeros, '0');

  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(nullptr_t) noexcept {
  _string.append("nullptr");

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(short value) noexcept {
  char buffer[7];

  static_assert(sizeof(value) == sizeof(int16_t), "Unsupported value size");
  itoa(buffer, size(buffer), static_cast<int16_t>(value));
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(int value) noexcept {
  char buffer[12];

  static_assert(sizeof(value) == sizeof(int32_t), "Unsupported value size");
  itoa(buffer, size(buffer), static_cast<int32_t>(value));
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(unsigned short value) noexcept {
  char buffer[7];

  static_assert(sizeof(value) == sizeof(uint16_t), "Unsupported value size");
  utoa(buffer, size(buffer), static_cast<uint16_t>(value));
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(unsigned int value) noexcept {
  char buffer[11];

  static_assert(sizeof(value) == sizeof(uint32_t), "Unsupported value size");
  utoa(buffer, size(buffer), static_cast<uint32_t>(value));
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(float value) noexcept {
  char buffer[128];

  static_assert(sizeof(value) == 4, "Unsupported value size");
  ftoa(buffer, size(buffer), value, _precision);
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(signed char value) noexcept {
  char buffer[5];

  static_assert(sizeof(value) == sizeof(int8_t), "Unsupported value size");
  itoa(buffer, size(buffer), static_cast<int8_t>(value));
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(unsigned char value) noexcept {
  char buffer[4];

  static_assert(sizeof(value) == sizeof(uint8_t), "Unsupported value size");
  utoa(buffer, size(buffer), static_cast<uint8_t>(value));
  _string.append(buffer);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(char_type value) noexcept {
  _string.push_back(value);

  return *this;
}

template <typename StringType>
template <StringLike SourceStringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(const SourceStringType & value) noexcept {
  _string.append(value);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::operator<<(const char_type * value) noexcept {
  assert_message(value != nullptr, "String pointer must not be null.");

  _string.append(value);

  return *this;
}

template <typename StringType>
constexpr const StringType & OStringStream<StringType>::str() const noexcept {
  return _string;
}

template <typename StringType>
template <StringLike SourceStringType>
constexpr void OStringStream<StringType>::str(const SourceStringType & string) noexcept {
  _string = string;
}

template <typename StringType>
constexpr CStringView OStringStream<StringType>::view() const noexcept {
  return CStringView(_string.c_str());
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::put(char_type character) noexcept {
  assert_message(character != '\0', "Character must not be null.");

  _string.push_back(character);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType> & OStringStream<StringType>::write(const char_type * string,
                                                                       size_t count) noexcept {
  assert_message(string != nullptr, "Source string must not be null.");
  if (string != nullptr && count > 0)
    _string.append(string, count);

  return *this;
}

template <typename StringType>
constexpr OStringStream<StringType>::pos_type OStringStream<StringType>::tellp() const noexcept {
  return _string.size();
}

template <typename StringType>
constexpr int OStringStream<StringType>::precision() const noexcept {
  return _precision;
}

template <typename StringType>
constexpr int OStringStream<StringType>::precision(int newPrecision) noexcept {
  assert_message(newPrecision >= 0, "Precision must be non-negative.");

  const auto oldPrecision = _precision;

  _precision = newPrecision;

  return oldPrecision;
}

} // namespace toy

#endif // INCLUDE_CORE_O_STRING_STREAM_INL_
