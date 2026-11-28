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
  \file   format_pattern.hpp
  \brief  The format pattern check and its grammar helpers.

  Defines the grammar helpers that read escapes and placeholders, and toy::detail::validateFormatPattern(), which
  matches a pattern against an argument count. Used by the consteval constructor of \ref toy::detail::FormatString and
  by any caller that wants the same verdict at runtime.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_PATTERN_HPP_
#define INCLUDE_CORE_DETAIL_FORMAT_PATTERN_HPP_

#include "core/string_like.hpp"
#include "core/string_view.hpp"
#include "format_pattern_validation_error.hpp"
#include "format_placeholder_mode.hpp"

namespace toy::detail {

/*!
  \brief Consumes a doubled brace at the position and reports whether one was there.

  \param string   Pattern being walked.
  \param length   Length of \a string in bytes.
  \param position Walk position; advanced past the pair when one is consumed.
  \param brace    Brace the pair must repeat.

  \return \c true when the position holds two \a brace characters in a row.
*/
[[nodiscard]] constexpr bool consumeEscapedBrace(StringView string, size_t length, size_t & position,
                                                 char brace) noexcept;

/*!
  \brief Reads a positional index at the position and checks it against the argument count.

  \param string   Pattern being walked.
  \param length   Length of \a string in bytes.
  \param position Walk position at the first index digit; left past the closing brace on success.
  \param argCount Arguments the index has to fit.
  \param index    Out-pointer that receives the parsed index on success; untouched otherwise
                  (default: \c nullptr, no output).

  \return \ref toy::detail::FormatPatternValidationError::none when the index fits \a argCount,
          \ref toy::detail::FormatPatternValidationError::invalidContent when the closing brace is missing, and
          \ref toy::detail::FormatPatternValidationError::indexOutOfRange when the index names no argument or
          overflows size_t.
*/
[[nodiscard]] constexpr FormatPatternValidationError parsePositionalIndex(StringView string, size_t length,
                                                                          size_t & position, size_t argCount,
                                                                          size_t * index = nullptr) noexcept;

/*!
  \brief Parses the contents after an opening brace, which an escape has already ruled out.

  \param string    Pattern being walked.
  \param length    Length of \a string in bytes.
  \param position  Walk position at the opening brace; left past the placeholder on success.
  \param autoCount Count of auto placeholders seen so far; incremented for an auto one.
  \param mode      Placeholder style seen so far; set to the style this brace contributes.
  \param argCount  Arguments an index has to fit.
  \param index     Out-pointer that receives the parsed index of a positional placeholder on success; untouched
                   otherwise (default: \c nullptr, no output).

  \return \ref toy::detail::FormatPatternValidationError::none when the brace opens a well-formed placeholder,
          otherwise the value naming the first violated rule.
*/
[[nodiscard]] constexpr FormatPatternValidationError parseOpeningBrace(StringView string, size_t length,
                                                                       size_t & position, size_t & autoCount,
                                                                       PlaceholderMode & mode, size_t argCount,
                                                                       size_t * index = nullptr) noexcept;

/*!
  \brief Checks the placeholder style the walk ended with against the argument count.

  \param mode      Placeholder style the walk reached.
  \param autoCount Auto placeholders the pattern carries.
  \param argCount  Arguments the pattern has to fit.

  \return \ref toy::detail::FormatPatternValidationError::none when the style fits \a argCount, otherwise
          \ref toy::detail::FormatPatternValidationError::argCountMismatch.
*/
[[nodiscard]] constexpr FormatPatternValidationError reconcilePlaceholderMode(PlaceholderMode mode, size_t autoCount,
                                                                              size_t argCount) noexcept;

/*!
  \brief Walks the pattern once and reports the first violated rule.

  \param string   Pattern to check.
  \param argCount Arguments the pattern has to fit.

  \return \ref toy::detail::FormatPatternValidationError::none when the pattern fits \a argCount, otherwise the value
          naming the first violated rule.
*/
[[nodiscard]] constexpr FormatPatternValidationError validateFormatImpl(StringView string, size_t argCount) noexcept;

/*!
  \brief Checks a null-terminated format pattern against an argument count.

  Walks the pattern once, reading the doubled-brace escapes and the auto and positional placeholders. An escape binds no
  argument, a positional placeholder may repeat an index, and unused arguments are not an error.

  \param string   Null-terminated pattern to check; the length is the offset of its terminator.
  \param argCount Arguments the pattern has to fit.

  \return \ref toy::detail::FormatPatternValidationError::none when the pattern fits \a argCount, otherwise the value
          naming the first violated rule.

  \pre \a string is non-null, checked by assert_message in debug builds.

  \sa toy::detail::validateFormatPattern(const StringType &)
*/
[[nodiscard]] constexpr FormatPatternValidationError validateFormatPattern(const char * string,
                                                                           size_t       argCount) noexcept;

/*!
  \brief Checks a format pattern read from a string-like object against an argument count.

  Same walk as the C-string overload; the length is taken from size() of \a string and the characters from c_str(), so
  no terminator is searched for.

  \tparam StringType Pattern type; satisfies \ref toy::StringLike.

  \param string   Pattern to check.
  \param argCount Arguments the pattern has to fit.

  \return \ref toy::detail::FormatPatternValidationError::none when the pattern fits \a argCount, otherwise the value
          naming the first violated rule.

  \pre \a string returns a non-null c_str() with \c '\\0' at offset size(), checked by assert_message in debug builds.

  \sa toy::detail::validateFormatPattern(const char *)
*/
template <StringLike StringType>
[[nodiscard]] constexpr FormatPatternValidationError validateFormatPattern(const StringType & string,
                                                                           size_t             argCount) noexcept;

} // namespace toy::detail

#include "format_pattern.inl"

#endif // INCLUDE_CORE_DETAIL_FORMAT_PATTERN_HPP_
