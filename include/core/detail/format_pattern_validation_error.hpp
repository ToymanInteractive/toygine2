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
  \file   format_pattern_validation_error.hpp
  \brief  The values a format pattern check can report.

  Defines \ref toy::detail::FormatPatternValidationError, the verdict a format pattern check reports. Internal to the
  format implementation.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_PATTERN_VALIDATION_ERROR_HPP_
#define INCLUDE_CORE_DETAIL_FORMAT_PATTERN_VALIDATION_ERROR_HPP_

namespace toy::detail {

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

} // namespace toy::detail

#endif // INCLUDE_CORE_DETAIL_FORMAT_PATTERN_VALIDATION_ERROR_HPP_
