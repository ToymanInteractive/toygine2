//
// Copyright (c) 2025 by Toyman Interactive
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
  \file   utils.inl
  \brief  Inline implementations of core utility functions for string manipulation and encoding conversion
*/

#ifndef INCLUDE_CORE_UTILS_INL_
#define INCLUDE_CORE_UTILS_INL_

namespace toygine {

constexpr inline wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, const char * const src) noexcept {
  assert_message(src != nullptr, "The source must not be null.");

  return src != nullptr ? utf8toWChar(dest, destSize, src, std::strlen(src)) : nullptr;
}

template <StringLike stringType>
constexpr inline wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, const stringType & src) noexcept {
  return utf8toWChar(dest, destSize, src.c_str(), src.size());
}

constexpr inline char * reverseString(char * string, std::size_t stringLength) noexcept {
  assert_message(string != nullptr, "The source string must not be null.");
  if (string == nullptr)
    return nullptr;

  if (stringLength == 0)
    stringLength = std::strlen(string);

  if (stringLength != 0) {
    for (std::size_t i = 0, j = stringLength - 1; i < j; ++i, --j) {
      std::swap(string[i], string[j]);
    }
  }

  return string;
}

} // namespace toygine

#endif // INCLUDE_CORE_UTILS_INL_
