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
  \file   format.inl
  \brief  Inline implementations for FormatString class.
*/

#ifndef INCLUDE_CORE_FORMAT_INL_
#define INCLUDE_CORE_FORMAT_INL_

namespace toy {

template <class... Args>
consteval FormatString<Args...>::FormatString(const CStringView & string) noexcept
  : _string(string) {
  const auto placeholderCount = _countFormatPlaceholders(string);
  if (placeholderCount == CStringView::npos)
    _compileTimeError("Invalid format string: unmatched braces");

  constexpr auto expectedArgs = sizeof...(Args);
  if (placeholderCount != expectedArgs)
    _compileTimeError("Format string placeholder count does not match argument count");
}

template <class... Args>
constexpr CStringView FormatString<Args...>::get() const noexcept {
  return _string;
}

template <class... Args>
constexpr size_t FormatString<Args...>::_countFormatPlaceholders(const CStringView & string) noexcept {
  size_t placeholderCount = 0;
  size_t position = 0;
  const auto length = string.size();

  while (position < length) {
    if (string.at(position) == '{') {
      if (position + 1 < length && string.at(position + 1) == '{') {
        // Escaped opening brace `{{`
        position += 2;
        continue;
      }
      if (position + 1 < length && string.at(position + 1) == '}') {
        // Placeholder `{}`
        ++placeholderCount;
        position += 2;
        continue;
      }

      // Unmatched opening brace `{` - invalid format
      return CStringView::npos;
    } else if (string.at(position) == '}') {
      if (position + 1 < length && string.at(position + 1) == '}') {
        // Escaped closing brace `}}`
        position += 2;
        continue;
      }

      // Unmatched closing brace `}` - invalid format
      return CStringView::npos;
    }

    ++position;
  }

  return placeholderCount;
}

inline void _compileTimeError(const char *) noexcept {
  // Intentionally cause a compile-time error
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_INL_
