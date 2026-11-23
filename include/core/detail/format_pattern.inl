//
// Copyright (c) 2026 Toyman Interactive
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
  \file   format_pattern.inl
  \brief  Inline implementations for the toy::detail pattern grammar helpers and validateFormatPattern() overloads.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_PATTERN_INL_
#define INCLUDE_CORE_DETAIL_FORMAT_PATTERN_INL_

namespace toy::detail {

constexpr bool consumeEscapedBrace(StringView string, size_t length, size_t & position, char brace) noexcept {
  const bool pair = position + 1 < length && string.at(position) == brace && string.at(position + 1) == brace;
  if (pair)
    position += 2;

  return pair;
}

constexpr FormatPatternValidationError parsePositionalIndex(StringView string, size_t length, size_t & position,
                                                            size_t argCount, size_t * index) noexcept {
  size_t parsed   = 0;
  bool   anyDigit = false;
  while (position < length) {
    const char d = string.at(position);
    if (d < '0' || d > '9')
      break;

    anyDigit         = true;
    const auto digit = static_cast<unsigned>(d - '0');
    if (parsed > (SIZE_MAX - digit) / 10U)
      return FormatPatternValidationError::indexOutOfRange;

    parsed = parsed * 10U + digit;
    ++position;
  }

  if (!anyDigit)
    return FormatPatternValidationError::invalidContent;
  else if (position >= length || string.at(position) != '}')
    return FormatPatternValidationError::invalidContent;
  else if (parsed >= argCount)
    return FormatPatternValidationError::indexOutOfRange;

  ++position;

  if (index != nullptr)
    *index = parsed;

  return FormatPatternValidationError::none;
}

constexpr FormatPatternValidationError parseOpeningBrace(StringView string, size_t length, size_t & position,
                                                         size_t & autoCount, PlaceholderMode & mode, size_t argCount,
                                                         size_t * index) noexcept {
  if (position + 1 >= length)
    return FormatPatternValidationError::unmatchedBrace;

  const char next = string.at(position + 1);
  if (next == '}') {
    if (mode == PlaceholderMode::positional)
      return FormatPatternValidationError::mixedPlaceholders;

    mode = PlaceholderMode::autoIndex;
    ++autoCount;
    position += 2;

    return FormatPatternValidationError::none;
  }

  if (next >= '0' && next <= '9') {
    if (mode == PlaceholderMode::autoIndex)
      return FormatPatternValidationError::mixedPlaceholders;

    mode = PlaceholderMode::positional;
    ++position;

    return parsePositionalIndex(string, length, position, argCount, index);
  }

  return FormatPatternValidationError::unmatchedBrace;
}

constexpr FormatPatternValidationError reconcilePlaceholderMode(PlaceholderMode mode, size_t autoCount,
                                                                size_t argCount) noexcept {
  if (mode == PlaceholderMode::none)
    return argCount != 0 ? FormatPatternValidationError::argCountMismatch : FormatPatternValidationError::none;
  else if (mode == PlaceholderMode::autoIndex)
    return autoCount != argCount ? FormatPatternValidationError::argCountMismatch : FormatPatternValidationError::none;

  return FormatPatternValidationError::none;
}

constexpr FormatPatternValidationError validateFormatImpl(StringView string, size_t argCount) noexcept {
  auto       mode      = PlaceholderMode::none;
  size_t     autoCount = 0;
  size_t     position  = 0;
  const auto length    = string.size();

  while (position < length) {
    const char c = string.at(position);
    if (c == '{') {
      if (consumeEscapedBrace(string, length, position, '{'))
        continue;

      if (const auto err = parseOpeningBrace(string, length, position, autoCount, mode, argCount);
          err != FormatPatternValidationError::none)
        return err;

      continue;
    }

    if (c == '}') {
      if (consumeEscapedBrace(string, length, position, '}'))
        continue;

      return FormatPatternValidationError::unmatchedBrace;
    }

    ++position;
  }

  return reconcilePlaceholderMode(mode, autoCount, argCount);
}

constexpr FormatPatternValidationError validateFormatPattern(const char * string, size_t argCount) noexcept {
  return validateFormatImpl(StringView{string}, argCount);
}

template <StringLike StringType>
constexpr FormatPatternValidationError validateFormatPattern(StringType string, size_t argCount) noexcept {
  return validateFormatImpl(StringView{string}, argCount);
}

} // namespace toy::detail

#endif // INCLUDE_CORE_DETAIL_FORMAT_PATTERN_INL_
