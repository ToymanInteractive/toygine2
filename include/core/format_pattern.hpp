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
  \brief  Shared compile-time validation for format patterns using \c {} and \c {N} placeholders.

  Declares \ref toy::FormatPatternValidationError and \ref toy::validateFormatPattern used by \ref toy::FormatString and
  runtime formatting APIs.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_PATTERN_HPP_
#define INCLUDE_CORE_FORMAT_PATTERN_HPP_

namespace toy {

/*!
  \enum FormatPatternValidationError
  \ingroup String
  \brief Result of validating a format pattern against an expected argument count.
*/
enum class FormatPatternValidationError {
  /// Pattern is valid for the given argument count.
  none,

  /// Stray or incomplete \c { / \c } (not \c {{, \c }}, \c {}, or \c {N}).
  unmatchedBrace,

  /// Positional \c {N…} not closed by \c }, or disallowed characters after the index.
  invalidContent,

  /// Mixed auto \c {} and positional \c {N} in one pattern.
  mixedPlaceholders,

  /// Placeholder count vs expected argument count mismatch (including empty pattern with arguments).
  argCountMismatch,

  /// Positional index out of range or overflow while parsing digits.
  indexOutOfRange
};

/*!
  \ingroup String

  \brief Validates auto-indexed \c {} and positional \c {N} placeholders for \a argCount arguments.

  \param string   Pattern to scan.
  \param argCount Number of format arguments.

  \return Error code; \c FormatPatternValidationError::none when the pattern is consistent with \a argCount.
*/
[[nodiscard]] constexpr FormatPatternValidationError validateFormatPattern(const CStringView & string,
                                                                           size_t argCount) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_PATTERN_HPP_
