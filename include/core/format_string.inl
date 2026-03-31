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
  \file   format_string.inl
  \brief  Inline implementations for \ref toy::FormatString.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_STRING_INL_
#define INCLUDE_CORE_FORMAT_STRING_INL_

namespace toy {

template <class... Args>
consteval FormatString<Args...>::FormatString(const CStringView & string) noexcept
  : _string(string) {
  const auto error = _validateFormat(string, sizeof...(Args));
  if (error != ValidationError::none)
    _compileTimeError("invalid format string: check braces, auto {} vs positional {N}, argument count, indices");
}

template <class... Args>
constexpr CStringView FormatString<Args...>::get() const noexcept {
  return _string;
}

template <class... Args>
constexpr typename FormatString<Args...>::ValidationError FormatString<Args...>::_validateFormat(
  const CStringView & string, size_t argCount) noexcept {
  enum class Mode { none, autoIndex, positional };

  Mode mode = Mode::none;
  size_t autoCount = 0;
  size_t position = 0;
  const auto length = string.size();

  while (position < length) {
    const char c = string.at(position);
    if (c == '{') {
      if (position + 1 < length && string.at(position + 1) == '{') {
        position += 2;
        continue;
      }

      if (position + 1 >= length)
        return ValidationError::unmatchedBrace;

      if (string.at(position + 1) == '}') {
        if (mode == Mode::positional)
          return ValidationError::mixedPlaceholders;

        mode = Mode::autoIndex;
        ++autoCount;
        position += 2;
        continue;
      }

      if (string.at(position + 1) >= '0' && string.at(position + 1) <= '9') {
        if (mode == Mode::autoIndex)
          return ValidationError::mixedPlaceholders;

        mode = Mode::positional;
        ++position;
        size_t index = 0;
        bool anyDigit = false;
        while (position < length) {
          const char d = string.at(position);
          if (d < '0' || d > '9')
            break;

          anyDigit = true;
          const auto digit = static_cast<unsigned>(d - '0');
          if (index > (SIZE_MAX - digit) / 10U)
            return ValidationError::indexOutOfRange;

          index = index * 10U + digit;
          ++position;
        }

        if (!anyDigit)
          return ValidationError::invalidContent;

        if (position >= length || string.at(position) != '}')
          return ValidationError::invalidContent;

        if (index >= argCount)
          return ValidationError::indexOutOfRange;

        ++position;
        continue;
      }

      return ValidationError::unmatchedBrace;
    }

    if (c == '}') {
      if (position + 1 < length && string.at(position + 1) == '}') {
        position += 2;
        continue;
      }

      return ValidationError::unmatchedBrace;
    }

    ++position;
  }

  if (mode == Mode::none) {
    if (argCount != 0)
      return ValidationError::argCountMismatch;

    return ValidationError::none;
  }

  if (mode == Mode::autoIndex) {
    if (autoCount != argCount)
      return ValidationError::argCountMismatch;

    return ValidationError::none;
  }

  return ValidationError::none;
}

template <class... Args>
inline void FormatString<Args...>::_compileTimeError([[maybe_unused]] const char * message) noexcept {
  // Intentionally cause a compile-time error
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_STRING_INL_
