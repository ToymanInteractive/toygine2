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
  switch (const auto error = validateFormatPattern(string, sizeof...(Args)); error) {
  case FormatPatternValidationError::unmatchedBrace:
    _compileTimeError("invalid format string: unmatched or incomplete braces");
    break;
  case FormatPatternValidationError::invalidContent:
    _compileTimeError("invalid format string: invalid content inside braces (expected } after index)");
    break;
  case FormatPatternValidationError::mixedPlaceholders:
    _compileTimeError("invalid format string: cannot mix auto {} and positional {N} placeholders");
    break;
  case FormatPatternValidationError::argCountMismatch:
    _compileTimeError("invalid format string: placeholder count does not match the number of arguments");
    break;
  case FormatPatternValidationError::indexOutOfRange:
    _compileTimeError("invalid format string: positional index out of range");
    break;
  case FormatPatternValidationError::none:
  default:
    break;
  }
}

template <class... Args>
constexpr CStringView FormatString<Args...>::get() const noexcept {
  return _string;
}

template <class... Args>
inline void FormatString<Args...>::_compileTimeError([[maybe_unused]] const char * message) noexcept {
  // Intentionally cause a compile-time error
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_STRING_INL_
