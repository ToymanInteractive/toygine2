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
  \file   format_pattern.hpp
  \brief  Format pattern validation diagnostics and the check itself.

  Defines \ref toy::FormatPatternValidationError, the values a pattern check reports, and declares
  toy::validateFormatPattern(), which walks a pattern and matches its placeholders against an argument count. Used by
  the consteval constructor of \ref toy::FormatString and by any caller that wants the same verdict at runtime.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_PATTERN_HPP_
#define INCLUDE_CORE_FORMAT_PATTERN_HPP_

#include "string_like.hpp"

namespace toy {

/*!
  \enum  FormatPatternValidationError
  \brief The values a format pattern check can report.

  The check stops at the first violation and reports it, so a pattern with several faults yields one value per call.
*/
enum class FormatPatternValidationError {
  none,              ///< The pattern fits the argument count.
  unmatchedBrace,    ///< A brace opens no placeholder or closes nothing.
  invalidContent,    ///< An index inside braces lacks its closing brace.
  mixedPlaceholders, ///< Auto and positional placeholders share one pattern.
  argCountMismatch,  ///< Arguments are given without placeholders, or the auto placeholder count differs.
  indexOutOfRange,   ///< A positional index names no argument or overflows size_t.
};

/*!
  \brief Checks a null-terminated format pattern against an argument count.

  Walks the pattern once, reading the doubled-brace escapes and the auto and positional placeholders. An escape binds no
  argument, a positional placeholder may repeat an index, and unused arguments are not an error.

  \param string   Null-terminated pattern to check; the length is the offset of its terminator.
  \param argCount Arguments the pattern has to fit.

  \return \ref toy::FormatPatternValidationError::none when the pattern fits \a argCount, otherwise the value naming the
          first violated rule.
  \pre \a string is non-null, checked by assert_message in debug builds.

  \sa toy::validateFormatPattern(const StringType &)
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

  \return \ref toy::FormatPatternValidationError::none when the pattern fits \a argCount, otherwise the value naming the
          first violated rule.

  \pre \a string returns a non-null c_str() with \c '\\0' at offset size(), checked by assert_message in debug builds.

  \sa toy::validateFormatPattern(const char *)
*/
template <StringLike StringType>
[[nodiscard]] constexpr FormatPatternValidationError validateFormatPattern(const StringType & string,
                                                                           size_t             argCount) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_PATTERN_HPP_
