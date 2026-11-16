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
  \brief  Inline implementations for toy::validateFormatPattern(), the C-string and string-like overloads.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_PATTERN_INL_
#define INCLUDE_CORE_FORMAT_PATTERN_INL_

namespace toy {

namespace detail {

/// Tracks which placeholder style the pattern has used so far.
enum class PlaceholderMode {
  none,       ///< The pattern carries no placeholder yet.
  autoIndex,  ///< An auto placeholder has been seen.
  positional, ///< A positional placeholder has been seen.
};

/*!
  \brief Consumes a doubled brace at the position and reports whether one was there.

  \param string   Pattern being walked.
  \param length   Length of \a string in bytes.
  \param position Walk position; advanced past the pair when one is consumed.
  \param brace    Brace the pair must repeat.

  \return \c true when the position holds two \a brace characters in a row.
*/
[[nodiscard]] constexpr bool consumeEscapedBrace(const StringView & string, size_t length, size_t & position,
                                                 char brace) noexcept {
  const bool pair = position + 1 < length && string.at(position) == brace && string.at(position + 1) == brace;
  if (pair)
    position += 2;

  return pair;
}

/*!
  \brief Reads a positional index at the position and checks it against the argument count.

  \param string   Pattern being walked.
  \param length   Length of \a string in bytes.
  \param position Walk position at the first index digit; left past the closing brace on success.
  \param argCount Arguments the index has to fit.

  \return \ref toy::FormatPatternValidationError::none when the index fits \a argCount,
          \ref toy::FormatPatternValidationError::invalidContent when the closing brace is missing, and
          \ref toy::FormatPatternValidationError::indexOutOfRange when the index names no argument or overflows size_t.
*/
[[nodiscard]] constexpr FormatPatternValidationError parsePositionalIndex(const StringView & string, size_t length,
                                                                          size_t & position, size_t argCount) noexcept {
  size_t index    = 0;
  bool   anyDigit = false;
  while (position < length) {
    const char d = string.at(position);
    if (d < '0' || d > '9')
      break;

    anyDigit         = true;
    const auto digit = static_cast<unsigned>(d - '0');
    if (index > (SIZE_MAX - digit) / 10U)
      return FormatPatternValidationError::indexOutOfRange;

    index = index * 10U + digit;
    ++position;
  }

  if (!anyDigit)
    return FormatPatternValidationError::invalidContent;
  else if (position >= length || string.at(position) != '}')
    return FormatPatternValidationError::invalidContent;
  else if (index >= argCount)
    return FormatPatternValidationError::indexOutOfRange;

  ++position;

  return FormatPatternValidationError::none;
}

/*!
  \brief Parses the contents after an opening brace, which an escape has already ruled out.

  \param string    Pattern being walked.
  \param length    Length of \a string in bytes.
  \param position  Walk position at the opening brace; left past the placeholder on success.
  \param autoCount Count of auto placeholders seen so far; incremented for an auto one.
  \param mode      Placeholder style seen so far; set to the style this brace contributes.
  \param argCount  Arguments an index has to fit.

  \return \ref toy::FormatPatternValidationError::none when the brace opens a well-formed placeholder, otherwise the
          value naming the first violated rule.
*/
[[nodiscard]] constexpr FormatPatternValidationError parseOpeningBrace(const StringView & string, size_t length,
                                                                       size_t & position, size_t & autoCount,
                                                                       PlaceholderMode & mode,
                                                                       size_t            argCount) noexcept {
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

/*!
  \brief Checks the placeholder style the walk ended with against the argument count.

  \param mode      Placeholder style the walk reached.
  \param autoCount Auto placeholders the pattern carries.
  \param argCount  Arguments the pattern has to fit.

  \return \ref toy::FormatPatternValidationError::none when the style fits \a argCount, otherwise
          \ref toy::FormatPatternValidationError::argCountMismatch.
*/
[[nodiscard]] constexpr FormatPatternValidationError reconcilePlaceholderMode(PlaceholderMode mode, size_t autoCount,
                                                                              size_t argCount) noexcept {
  if (mode == PlaceholderMode::none)
    return argCount != 0 ? FormatPatternValidationError::argCountMismatch : FormatPatternValidationError::none;
  else if (mode == PlaceholderMode::autoIndex)
    return autoCount != argCount ? FormatPatternValidationError::argCountMismatch : FormatPatternValidationError::none;

  return FormatPatternValidationError::none;
}

/*!
  \brief Walks the pattern once and reports the first violated rule.

  \param string   Pattern to check.
  \param argCount Arguments the pattern has to fit.

  \return \ref toy::FormatPatternValidationError::none when the pattern fits \a argCount, otherwise the value naming the
          first violated rule.
*/
[[nodiscard]] constexpr FormatPatternValidationError validateFormatImpl(const StringView & string,
                                                                        size_t             argCount) noexcept {
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

} // namespace detail

constexpr FormatPatternValidationError validateFormatPattern(const char * string, size_t argCount) noexcept {
  return detail::validateFormatImpl(StringView{string}, argCount);
}

template <StringLike StringType>
constexpr FormatPatternValidationError validateFormatPattern(const StringType & string, size_t argCount) noexcept {
  return detail::validateFormatImpl(StringView{string}, argCount);
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_PATTERN_INL_
