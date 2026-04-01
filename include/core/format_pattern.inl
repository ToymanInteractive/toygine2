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
  \file   format_pattern.inl
  \brief  Inline implementations for \ref toy::validateFormatPattern.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_PATTERN_INL_
#define INCLUDE_CORE_FORMAT_PATTERN_INL_

namespace toy {

namespace format_pattern_detail {

/// Auto vs positional placeholder mode while scanning; internal to \c _validateFormat.
enum class PlaceholderMode {
  /// No \c {} or \c {N} placeholder has been seen yet.
  none,

  /// Only auto \c {} placeholders appear in the pattern.
  autoIndex,

  /// Only positional \c {N} placeholders appear in the pattern.
  positional
};

/*!
  \brief Consumes a literal \c {{ or \c }} pair at \a position when present.

  \param string   Pattern being scanned.
  \param length   Byte length of \a string (same value as \c string.size()).
  \param position Current index.
  \param brace    \c '{' to match \c {{, or \c '}' to match \c }}.

  \return \c true if a pair was consumed; \c false if \a position does not start a doubled brace.

  \note Helper for \c validateFormatImpl; not part of the public API.
*/
[[nodiscard]] constexpr bool consumeEscapedBrace(const CStringView & string, size_t length, size_t position,
                                                 char brace) noexcept {
  return position + 1 < length && string.at(position) == brace && string.at(position + 1) == brace;
}

[[nodiscard]] constexpr FormatPatternValidationError parsePositionalIndex(const CStringView & string, size_t length,
                                                                          size_t & position, size_t argCount) noexcept {
  size_t index = 0;
  bool anyDigit = false;
  while (position < length) {
    const char d = string.at(position);
    if (d < '0' || d > '9')
      break;

    anyDigit = true;
    const auto digit = static_cast<unsigned>(d - '0');
    if (index > (SIZE_MAX - digit) / 10U)
      return FormatPatternValidationError::indexOutOfRange;

    index = index * 10U + digit;
    ++position;
  }

  if (!anyDigit)
    return FormatPatternValidationError::invalidContent;

  if (position >= length || string.at(position) != '}')
    return FormatPatternValidationError::invalidContent;

  if (index >= argCount)
    return FormatPatternValidationError::indexOutOfRange;

  ++position;

  return FormatPatternValidationError::none;
}

[[nodiscard]] constexpr FormatPatternValidationError parseOpeningBrace(const CStringView & string, size_t length,
                                                                       size_t & position, size_t & autoCount,
                                                                       PlaceholderMode & mode,
                                                                       size_t argCount) noexcept {
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

    return parsePositionalIndex(string, length, position, argCount);
  }

  return FormatPatternValidationError::unmatchedBrace;
}

[[nodiscard]] constexpr FormatPatternValidationError reconcilePlaceholderMode(PlaceholderMode mode, size_t autoCount,
                                                                              size_t argCount) noexcept {
  if (mode == PlaceholderMode::none)
    return argCount != 0 ? FormatPatternValidationError ::argCountMismatch : FormatPatternValidationError::none;

  if (mode == PlaceholderMode::autoIndex)
    return autoCount != argCount ? FormatPatternValidationError::argCountMismatch : FormatPatternValidationError::none;

  return FormatPatternValidationError::none;
}

[[nodiscard]] constexpr FormatPatternValidationError validateFormatImpl(const CStringView & string,
                                                                        size_t argCount) noexcept {
  auto mode = PlaceholderMode::none;
  size_t autoCount = 0;
  size_t position = 0;
  const auto length = string.size();

  while (position < length) {
    const char c = string.at(position);
    if (c == '{') {
      if (consumeEscapedBrace(string, length, position, '{')) {
        position += 2;
        continue;
      }

      if (const auto err = parseOpeningBrace(string, length, position, autoCount, mode, argCount);
          err != FormatPatternValidationError::none)
        return err;

      continue;
    }

    if (c == '}') {
      if (consumeEscapedBrace(string, length, position, '}')) {
        position += 2;
        continue;
      }

      return FormatPatternValidationError::unmatchedBrace;
    }

    ++position;
  }

  return reconcilePlaceholderMode(mode, autoCount, argCount);
}

} // namespace format_pattern_detail

constexpr FormatPatternValidationError validateFormatPattern(const CStringView & string, size_t argCount) noexcept {
  return format_pattern_detail::validateFormatImpl(string, argCount);
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_PATTERN_INL_
