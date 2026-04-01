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
  \brief  Inline implementations for toy::validateFormatPattern().

  Scanning helpers live in an unnamed namespace so they are not named \c toy sub-entities in the include graph.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_PATTERN_INL_
#define INCLUDE_CORE_FORMAT_PATTERN_INL_

namespace toy {

namespace {

/// Auto vs positional placeholder mode while scanning; used only inside \c validateFormatImpl().
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

  \note Internal helper for \c validateFormatImpl; not part of the public API.
*/
[[nodiscard]] constexpr bool consumeEscapedBrace(const CStringView & string, size_t length, size_t & position,
                                                 char brace) noexcept {
  const bool pair = position + 1 < length && string.at(position) == brace && string.at(position + 1) == brace;
  if (pair)
    position += 2;

  return pair;
}

/*!
  \brief Reads the decimal index and closing \c } for a positional placeholder.

  \param string   Pattern being scanned.
  \param length   Byte length of \a string.
  \param position Index of the first digit inside \c {…}; updated past the closing \c } on success.
  \param argCount Upper bound for valid indices (\c N must be less than \a argCount).

  \return \c FormatPatternValidationError::none on success; \c FormatPatternValidationError::invalidContent when digits
          or closing \c } are missing; \c FormatPatternValidationError::indexOutOfRange when digit overflow occurs or
          the index is not less than \a argCount.

  \pre \a position refers to a decimal digit in \a string.

  \note Internal helper for \c parseOpeningBrace; not part of the public API.
*/
[[nodiscard]] constexpr FormatPatternValidationError parsePositionalIndex(const CStringView & string, size_t length,
                                                                          size_t & position, size_t argCount) noexcept {
  using enum FormatPatternValidationError;

  size_t index = 0;
  bool anyDigit = false;
  while (position < length) {
    const char d = string.at(position);
    if (d < '0' || d > '9')
      break;

    anyDigit = true;
    const auto digit = static_cast<unsigned>(d - '0');
    if (index > (SIZE_MAX - digit) / 10U)
      return indexOutOfRange;

    index = index * 10U + digit;
    ++position;
  }

  if (!anyDigit)
    return invalidContent;
  else if (position >= length || string.at(position) != '}')
    return invalidContent;
  else if (index >= argCount)
    return indexOutOfRange;

  ++position;

  return none;
}

/*!
  \brief Parses a non-escaped opening brace: auto \c {}, positional \c {N}, or a structural error.

  \param string   Pattern being scanned.
  \param length   Byte length of \a string.
  \param position Index of \c {; updated past the placeholder on success.
  \param autoCount Incremented for each auto \c {} placeholder.
  \param mode       Updated when the pattern uses auto or positional placeholders.
  \param argCount   Upper bound for valid positional indices (\c N must be less than \a argCount).

  \return First \c FormatPatternValidationError encountered, or \c FormatPatternValidationError::none when the
          placeholder is well-formed.

  \pre \a string at \a position is \c { and not the start of \c {{.

  \note Internal helper for \c validateFormatImpl; not part of the public API.
*/
[[nodiscard]] constexpr FormatPatternValidationError parseOpeningBrace(const CStringView & string, size_t length,
                                                                       size_t & position, size_t & autoCount,
                                                                       PlaceholderMode & mode,
                                                                       size_t argCount) noexcept {
  using enum FormatPatternValidationError;

  if (position + 1 >= length)
    return unmatchedBrace;

  const char next = string.at(position + 1);
  if (next == '}') {
    if (mode == PlaceholderMode::positional)
      return mixedPlaceholders;

    mode = PlaceholderMode::autoIndex;
    ++autoCount;
    position += 2;

    return none;
  }

  if (next >= '0' && next <= '9') {
    if (mode == PlaceholderMode::autoIndex)
      return mixedPlaceholders;

    mode = PlaceholderMode::positional;
    ++position;

    return parsePositionalIndex(string, length, position, argCount);
  }

  return unmatchedBrace;
}

/*!
  \brief Verifies placeholder counts against \a argCount after the whole pattern has been scanned.

  \param mode       Whether the pattern used no placeholders, auto \c {}, or positional \c {N}.
  \param autoCount  Number of \c {} placeholders when in auto mode.
  \param argCount   Expected \c sizeof...(Args).

  \return \c FormatPatternValidationError::none or \c FormatPatternValidationError::argCountMismatch.

  \note Internal helper for \c validateFormatImpl; not part of the public API.
*/
[[nodiscard]] constexpr FormatPatternValidationError reconcilePlaceholderMode(PlaceholderMode mode, size_t autoCount,
                                                                              size_t argCount) noexcept {
  using enum FormatPatternValidationError;

  if (mode == PlaceholderMode::none)
    return argCount != 0 ? argCountMismatch : none;
  else if (mode == PlaceholderMode::autoIndex)
    return autoCount != argCount ? argCountMismatch : none;

  return none;
}

/*!
  \ingroup String

  \brief Validates auto-indexed \c {} and positional \c {N} placeholders for \a argCount arguments.

  \param string   Pattern to scan.
  \param argCount Number of format arguments.

  \return \c FormatPatternValidationError::none when the pattern is valid; otherwise the first validation error
          encountered.

  \note Internal implementation for \c validateFormatPattern; not part of the public API.
*/
[[nodiscard]] constexpr FormatPatternValidationError validateFormatImpl(const CStringView & string,
                                                                        size_t argCount) noexcept {
  using enum FormatPatternValidationError;

  auto mode = PlaceholderMode::none;
  size_t autoCount = 0;
  size_t position = 0;
  const auto length = string.size();

  while (position < length) {
    const char c = string.at(position);
    if (c == '{') {
      if (consumeEscapedBrace(string, length, position, '{'))
        continue;

      if (const auto err = parseOpeningBrace(string, length, position, autoCount, mode, argCount); err != none)
        return err;

      continue;
    }

    if (c == '}') {
      if (consumeEscapedBrace(string, length, position, '}'))
        continue;

      return unmatchedBrace;
    }

    ++position;
  }

  return reconcilePlaceholderMode(mode, autoCount, argCount);
}

} // namespace

constexpr FormatPatternValidationError validateFormatPattern(const CStringView & string, size_t argCount) noexcept {
  return validateFormatImpl(string, argCount);
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_PATTERN_INL_
