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
  switch (const auto error = _validateFormat(string, sizeof...(Args)); error) {
  case ValidationError::unmatchedBrace:
    _compileTimeError("invalid format string: unmatched or incomplete braces");
    break;
  case ValidationError::invalidContent:
    _compileTimeError("invalid format string: invalid content inside braces (expected } after index)");
    break;
  case ValidationError::mixedPlaceholders:
    _compileTimeError("invalid format string: cannot mix auto {} and positional {N} placeholders");
    break;
  case ValidationError::argCountMismatch:
    _compileTimeError("invalid format string: placeholder count does not match the number of arguments");
    break;
  case ValidationError::indexOutOfRange:
    _compileTimeError("invalid format string: positional index out of range");
    break;
  case ValidationError::none:
  default:
    break;
  }
}

template <class... Args>
constexpr CStringView FormatString<Args...>::get() const noexcept {
  return _string;
}

template <class... Args>
constexpr typename FormatString<Args...>::ValidationError FormatString<Args...>::_validateFormat(
  const CStringView & string, size_t argCount) noexcept {
  auto mode = PlaceholderMode::none;
  size_t autoCount = 0;
  size_t position = 0;
  const auto length = string.size();

  while (position < length) {
    const char c = string.at(position);
    if (c == '{') {
      if (_consumeEscapedBrace(string, length, position, '{'))
        continue;

      const auto err = _parseOpeningBrace(string, length, position, autoCount, mode, argCount);
      if (err != ValidationError::none)
        return err;

      continue;
    }

    if (c == '}') {
      if (_consumeEscapedBrace(string, length, position, '}'))
        continue;

      return ValidationError::unmatchedBrace;
    }

    ++position;
  }

  return _reconcilePlaceholderMode(mode, autoCount, argCount);
}

template <class... Args>
constexpr bool FormatString<Args...>::_consumeEscapedBrace(const CStringView & string, size_t length, size_t & position,
                                                           char brace) noexcept {
  if (position + 1 < length && string.at(position) == brace && string.at(position + 1) == brace) {
    position += 2;

    return true;
  }

  return false;
}

template <class... Args>
constexpr typename FormatString<Args...>::ValidationError FormatString<Args...>::_parseOpeningBrace(
  const CStringView & string, size_t length, size_t & position, size_t & autoCount, PlaceholderMode & mode,
  size_t argCount) noexcept {
  if (position + 1 >= length)
    return ValidationError::unmatchedBrace;

  const char next = string.at(position + 1);
  if (next == '}') {
    if (mode == PlaceholderMode::positional)
      return ValidationError::mixedPlaceholders;

    mode = PlaceholderMode::autoIndex;
    ++autoCount;
    position += 2;

    return ValidationError::none;
  }

  if (next >= '0' && next <= '9') {
    if (mode == PlaceholderMode::autoIndex)
      return ValidationError::mixedPlaceholders;

    mode = PlaceholderMode::positional;
    ++position;

    return _parsePositionalIndex(string, length, position, argCount);
  }

  return ValidationError::unmatchedBrace;
}

template <class... Args>
constexpr typename FormatString<Args...>::ValidationError FormatString<Args...>::_parsePositionalIndex(
  const CStringView & string, size_t length, size_t & position, size_t argCount) noexcept {
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

  return ValidationError::none;
}

template <class... Args>
constexpr typename FormatString<Args...>::ValidationError FormatString<Args...>::_reconcilePlaceholderMode(
  PlaceholderMode mode, size_t autoCount, size_t argCount) noexcept {
  if (mode == PlaceholderMode::none) {
    if (argCount != 0)
      return ValidationError::argCountMismatch;

    return ValidationError::none;
  }

  if (mode == PlaceholderMode::autoIndex) {
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
