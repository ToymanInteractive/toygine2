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
  \file   constexpr_utils.inl
  \brief  Inline implementations for constexpr utility functions.
*/

#ifndef INCLUDE_CORE_CONSTEXPR_UTILS_INL_
#define INCLUDE_CORE_CONSTEXPR_UTILS_INL_

namespace toy {

constexpr int cstrcmp(const char * lhs, const char * rhs) noexcept {
  assert_message(lhs != nullptr && rhs != nullptr, "C string pointers must not be null");

  while (*lhs && (*lhs == *rhs)) {
    ++lhs;
    ++rhs;
  }

  if (*lhs == *rhs)
    return 0;

  return (static_cast<unsigned char>(*lhs) < static_cast<unsigned char>(*rhs)) ? -1 : 1;
}

constexpr const char * cstrchr(const char * str, int ch) noexcept {
  assert_message(str != nullptr, "C string pointer must not be null");

  const auto target = static_cast<unsigned char>(ch);

  for (; *str; ++str) {
    if (static_cast<unsigned char>(*str) == target)
      return str;
  }

  return (static_cast<unsigned char>(*str) == target) ? str : nullptr;
}

constexpr const char * cstrpbrk(const char * str, const char * accept) noexcept {
  if (str == nullptr || accept == nullptr)
    return nullptr;

  for (; *str != '\0'; ++str) {
    for (const char * a = accept; *a != '\0'; ++a) {
      if (*str == *a)
        return str;
    }
  }

  return nullptr;
}

constexpr const char * cstrstr(const char * haystack, const char * needle) noexcept {
  assert_message(haystack != nullptr && needle != nullptr, "C string pointers must not be null");

  if (!*needle)
    return haystack;

  const char first = *needle;
  for (const char * h = haystack; *h; ++h) {
    if (*h != first)
      continue;

    const char * h_it = h;
    const char * n_it = needle;

    while (*h_it && *n_it && *h_it == *n_it) {
      ++h_it;
      ++n_it;
    }

    if (!*n_it)
      return h;
  }

  return nullptr;
}

} // namespace toy

#endif // INCLUDE_CORE_CONSTEXPR_UTILS_INL_
