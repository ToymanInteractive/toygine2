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
  \brief  Inline implementations for \ref toy::OStringStream.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_O_STRING_STREAM_INL_
#define INCLUDE_CORE_O_STRING_STREAM_INL_

namespace toy {

template <OStringStreamBackend BackendType>
template <StringLike SourceStringType>
constexpr OStringStream<BackendType>::OStringStream(const SourceStringType & string) noexcept
  : _string(string) {}

template <OStringStreamBackend BackendType>
constexpr void OStringStream<BackendType>::swap(OStringStream & other) noexcept {
  std::swap(_string, other._string);
  std::swap(_precision, other._precision);
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(bool value) noexcept {
  _string.append(value ? "true" : "false");

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(long value) noexcept {
  char buffer[21];

  if constexpr (sizeof(value) == sizeof(int32_t))
    itoa(buffer, size(buffer), static_cast<int32_t>(value));
  else if constexpr (sizeof(value) == sizeof(int64_t))
    itoa(buffer, size(buffer), static_cast<int64_t>(value));
  else
    static_assert(sizeof(value) == sizeof(int32_t) || sizeof(value) == sizeof(int64_t), "Unsupported value size");

  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(unsigned long value) noexcept {
  char buffer[21];

  if constexpr (sizeof(value) == sizeof(uint32_t))
    utoa(buffer, size(buffer), static_cast<uint32_t>(value));
  else if constexpr (sizeof(value) == sizeof(uint64_t))
    utoa(buffer, size(buffer), static_cast<uint64_t>(value));
  else
    static_assert(sizeof(value) == sizeof(uint32_t) || sizeof(value) == sizeof(uint64_t), "Unsupported value size");

  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(long long value) noexcept {
  char buffer[21];

  static_assert(sizeof(value) == sizeof(int64_t), "Unsupported value size");
  itoa(buffer, size(buffer), static_cast<int64_t>(value));
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(unsigned long long value) noexcept {
  char buffer[21];

  static_assert(sizeof(value) == sizeof(uint64_t), "Unsupported value size");
  utoa(buffer, size(buffer), static_cast<uint64_t>(value));
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(double value) noexcept {
  char buffer[128];

  static_assert(sizeof(value) == 8, "Unsupported value size");
  ftoa(buffer, size(buffer), value, _precision);
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(const void * value) noexcept {
  if (value == nullptr)
    return *this << nullptr;

  char buffer[17];

  if constexpr (sizeof(value) == 4)
    utoa(buffer, size(buffer), reinterpret_cast<uintptr_t>(value), 16);
  else if constexpr (sizeof(value) == 8)
    utoa(buffer, size(buffer), reinterpret_cast<uintptr_t>(value), 16);
  else
    static_assert(sizeof(value) == 4 || sizeof(value) == 8, "Unsupported pointer size");

  _string.append("0x");

  const auto bufferDataLen = char_traits<char>::length(buffer);
  if (const int leadingZeros = static_cast<int>(sizeof(value)) * 2 - static_cast<int>(bufferDataLen); leadingZeros > 0)
    _string.append(static_cast<size_t>(leadingZeros), '0');

  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(nullptr_t) noexcept {
  _string.append("nullptr");

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(short value) noexcept {
  char buffer[7];

  static_assert(sizeof(value) == sizeof(int16_t), "Unsupported value size");
  itoa(buffer, size(buffer), static_cast<int16_t>(value));
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(int value) noexcept {
  char buffer[12];

  static_assert(sizeof(value) == sizeof(int32_t), "Unsupported value size");
  itoa(buffer, size(buffer), static_cast<int32_t>(value));
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(unsigned short value) noexcept {
  char buffer[7];

  static_assert(sizeof(value) == sizeof(uint16_t), "Unsupported value size");
  utoa(buffer, size(buffer), static_cast<uint16_t>(value));
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(unsigned int value) noexcept {
  char buffer[11];

  static_assert(sizeof(value) == sizeof(uint32_t), "Unsupported value size");
  utoa(buffer, size(buffer), static_cast<uint32_t>(value));
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(float value) noexcept {
  char buffer[128];

  static_assert(sizeof(value) == 4, "Unsupported value size");
  ftoa(buffer, size(buffer), value, _precision);
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(signed char value) noexcept {
  char buffer[5];

  static_assert(sizeof(value) == sizeof(int8_t), "Unsupported value size");
  itoa(buffer, size(buffer), static_cast<int8_t>(value));
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(unsigned char value) noexcept {
  char buffer[4];

  static_assert(sizeof(value) == sizeof(uint8_t), "Unsupported value size");
  utoa(buffer, size(buffer), static_cast<uint8_t>(value));
  _string.append(buffer);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(char_type value) noexcept {
  _string.push_back(value);

  return *this;
}

template <OStringStreamBackend BackendType>
template <StringLike SourceStringType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(const SourceStringType & value) noexcept {
  _string.append(value.c_str(), value.size());

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(const char_type * value) noexcept {
  assert_message(value != nullptr, "String pointer must not be null.");

  _string.append(value);

  return *this;
}

template <OStringStreamBackend BackendType>
template <typename Rep, typename Period>
  requires std::signed_integral<Rep>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(
  const chrono::Duration<Rep, Period> & value
) noexcept {
  auto count = static_cast<int64_t>(value.count());
  if (count < 0) {
    put('-');
    count = (count != std::numeric_limits<int64_t>::min()) ? -count : std::numeric_limits<int64_t>::max();
  }

  if constexpr (Period::num == 1 && Period::den > 1) {
    constexpr auto fracDigits = [](int64_t n) constexpr noexcept {
      int d = 0;
      while (n > 0) {
        n /= 10;
        ++d;
      }

      return d;
    };
    constexpr auto pow10 = [](int n) constexpr noexcept {
      int64_t r = 1;
      while (n-- > 0)
        r *= 10;

      return r;
    };

    constexpr auto den    = static_cast<int64_t>(Period::den);
    constexpr auto digits = fracDigits(den - 1);
    constexpr auto scale  = pow10(digits);
    *this << (count / den);
    put('.');
    _writeZeroPadded(count % den * scale / den, digits);
  } else {
    using seconds_type = std::chrono::duration<int64_t>;
    *this << chrono::duration_cast<seconds_type>(chrono::Duration<int64_t, Period>{count}).count();
  }

  return put('s');
}

template <OStringStreamBackend BackendType>
template <typename Rep, typename Period>
  requires std::signed_integral<Rep>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(
  const chrono::DurationFormat<Rep, Period> & value
) noexcept {
  auto duration = value.duration;
  if (duration.count() < 0) {
    put('-');
    duration = (duration.count() != numeric_limits<Rep>::min())
                 ? -duration
                 : chrono::Duration<Rep, Period>(numeric_limits<Rep>::max());
  }

  const int64_t totalMs = chrono::duration_cast<chrono::milliseconds>(duration).count();
  const int64_t h       = totalMs / 3600000LL;
  const int32_t m       = static_cast<int32_t>(totalMs % 3600000LL) / 60000;
  const int32_t s       = static_cast<int32_t>(totalMs % 60000LL) / 1000;
  const auto    z       = static_cast<int32_t>(totalMs % 1000LL);

  for (const char * p = value.pattern.c_str(); *p != '\0'; ++p) {
    if (*p == 'h' && p[1] == 'h') {
      _writeZeroPadded(h, 2);
      ++p;
    } else if (*p == 'h') {
      *this << h;
    } else if (*p == 'm' && p[1] == 'm') {
      _writeZeroPadded(m, 2);
      ++p;
    } else if (*p == 'm') {
      *this << m;
    } else if (*p == 's' && p[1] == 's') {
      _writeZeroPadded(s, 2);
      ++p;
    } else if (*p == 's') {
      *this << s;
    } else if (*p == 'z' && p[1] == 'z' && p[2] == 'z') {
      _writeZeroPadded(z, 3);
      p += 2;
    } else if (*p == 'z') {
      *this << z;
    } else {
      put(*p);
    }
  }

  return *this;
}

template <OStringStreamBackend BackendType>
template <typename Clock, typename Dur>
  requires std::signed_integral<typename Dur::rep>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(
  chrono::TimePoint<Clock, Dur> value
) noexcept {
  return *this << value.time_since_epoch();
}

template <OStringStreamBackend BackendType>
template <typename Clock, typename Dur>
  requires std::signed_integral<typename Dur::rep>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(
  chrono::TimePointFormat<Clock, Dur> value
) noexcept {
  return *this << chrono::DurationFormat<typename Dur::rep, typename Dur::period>{value.pattern,
                                                                                  value.timePoint.time_since_epoch()};
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(chrono::CalendarTime value) noexcept {
  _writeZeroPadded(static_cast<int32_t>(value.year), 4);
  put('-');
  _writeZeroPadded(static_cast<int32_t>(value.month), 2);
  put('-');
  _writeZeroPadded(static_cast<int32_t>(value.day), 2);
  put(' ');
  _writeZeroPadded(static_cast<int32_t>(value.hour), 2);
  put(':');
  _writeZeroPadded(static_cast<int32_t>(value.minute), 2);
  put(':');
  _writeZeroPadded(static_cast<int32_t>(value.second), 2);
  put('.');
  _writeZeroPadded(static_cast<int32_t>(value.millisecond), 3);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::operator<<(
  chrono::CalendarTimeFormat value
) noexcept {
  const auto year   = static_cast<int32_t>(value.calendarTime.year);
  const auto month  = static_cast<int32_t>(value.calendarTime.month);
  const auto day    = static_cast<int32_t>(value.calendarTime.day);
  const auto hour   = static_cast<int32_t>(value.calendarTime.hour);
  const auto minute = static_cast<int32_t>(value.calendarTime.minute);
  const auto second = static_cast<int32_t>(value.calendarTime.second);
  const auto ms     = static_cast<int32_t>(value.calendarTime.millisecond);

  for (const char * p = value.pattern.c_str(); *p != '\0'; ++p) {
    if (*p == 'y' && p[1] == 'y' && p[2] == 'y' && p[3] == 'y') {
      _writeZeroPadded(year, 4);
      p += 3;
    } else if (*p == 'y' && p[1] == 'y') {
      _writeZeroPadded(year % 100, 2);
      ++p;
    } else if (*p == 'y') {
      *this << year;
    } else if (*p == 'M' && p[1] == 'M') {
      _writeZeroPadded(month, 2);
      ++p;
    } else if (*p == 'M') {
      *this << month;
    } else if (*p == 'd' && p[1] == 'd') {
      _writeZeroPadded(day, 2);
      ++p;
    } else if (*p == 'd') {
      *this << day;
    } else if (*p == 'h' && p[1] == 'h') {
      _writeZeroPadded(hour, 2);
      ++p;
    } else if (*p == 'h') {
      *this << hour;
    } else if (*p == 'm' && p[1] == 'm') {
      _writeZeroPadded(minute, 2);
      ++p;
    } else if (*p == 'm') {
      *this << minute;
    } else if (*p == 's' && p[1] == 's') {
      _writeZeroPadded(second, 2);
      ++p;
    } else if (*p == 's') {
      *this << second;
    } else if (*p == 'z' && p[1] == 'z' && p[2] == 'z') {
      _writeZeroPadded(ms, 3);
      p += 2;
    } else if (*p == 'z') {
      *this << ms;
    } else {
      put(*p);
    }
  }

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr const BackendType & OStringStream<BackendType>::str() const noexcept {
  return _string;
}

template <OStringStreamBackend BackendType>
template <StringLike SourceStringType>
constexpr void OStringStream<BackendType>::str(const SourceStringType & string) noexcept {
  _string = string;
}

template <OStringStreamBackend BackendType>
constexpr CStringView OStringStream<BackendType>::view() const noexcept {
  return CStringView(_string.c_str());
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::put(char_type character) noexcept {
  assert_message(character != '\0', "Character must not be null.");

  _string.push_back(character);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType> & OStringStream<BackendType>::write(const char_type * string,
                                                                         size_t            count) noexcept {
  assert_message(string != nullptr, "Source string must not be null.");
  if (string != nullptr && count > 0)
    _string.append(string, count);

  return *this;
}

template <OStringStreamBackend BackendType>
constexpr OStringStream<BackendType>::pos_type OStringStream<BackendType>::tellp() const noexcept {
  return _string.size();
}

template <OStringStreamBackend BackendType>
constexpr size_t OStringStream<BackendType>::precision() const noexcept {
  return _precision;
}

template <OStringStreamBackend BackendType>
constexpr size_t OStringStream<BackendType>::setPrecision(size_t newPrecision) noexcept {
  assert_message(newPrecision <= numeric_limits<long double>::digits10,
                 "Precision must not exceed maximum supported digits for long double");

  const auto oldPrecision = _precision;

  _precision = newPrecision;

  return oldPrecision;
}

template <OStringStreamBackend BackendType>
void OStringStream<BackendType>::_writeZeroPadded(int64_t value, size_t width) noexcept {
  char buffer[22];
  utoa(buffer, size(buffer), static_cast<uint64_t>(value));

  const auto length = char_traits<char>::length(buffer);
  if (width > length)
    _string.append(width - length, '0');

  _string.append(buffer, length);
}

template <OStringStreamBackend BackendType>
void OStringStream<BackendType>::_writeZeroPadded(int32_t value, size_t width) noexcept {
  char buffer[12];
  utoa(buffer, size(buffer), static_cast<uint32_t>(value));

  const auto length = char_traits<char>::length(buffer);
  if (width > length)
    _string.append(width - length, '0');

  _string.append(buffer, length);
}

} // namespace toy

#endif // INCLUDE_CORE_O_STRING_STREAM_INL_
