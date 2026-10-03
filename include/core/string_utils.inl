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
  \file   string_utils.inl
  \brief  Inline implementations for the \ref toy::string_utils search functions.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_UTILS_INL_
#define INCLUDE_CORE_STRING_UTILS_INL_

namespace toy::string_utils {

constexpr size_t find(const char * data, size_t size, char ch, size_t pos) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");

  if (pos >= size)
    return c_npos;

  // memchr at runtime, a plain scan under constant evaluation; neither reads a byte before pos.
  const char * found = traits_type::find(data + pos, size - pos, ch);

  return found != nullptr ? static_cast<size_t>(found - data) : c_npos;
}

constexpr size_t find(const char * data, size_t size, const char * pattern, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(pattern != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return pos <= size ? pos : c_npos;
  else if (count > size)
    return c_npos;

  const size_t last = size - count;
  for (size_t index = pos; index <= last;) {
    // memchr at runtime: skips to the next position that can start a match instead of comparing at every one.
    const char * candidate = traits_type::find(data + index, last - index + 1, pattern[0]);
    if (candidate == nullptr)
      return c_npos;

    index = static_cast<size_t>(candidate - data);

    // The first character matched already, so the comparison starts behind it.
    if (traits_type::compare(candidate + 1, pattern + 1, count - 1) == 0)
      return index;

    ++index;
  }

  return c_npos;
}

constexpr size_t rfind(const char * data, size_t size, char ch, size_t pos) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");

  for (size_t remaining = pos < size ? pos + 1 : size; remaining > 0; --remaining)
    if (traits_type::eq(data[remaining - 1], ch))
      return remaining - 1;

  return c_npos;
}

constexpr size_t rfind(const char * data, size_t size, const char * pattern, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(pattern != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return std::min(pos, size);
  else if (count > size)
    return c_npos;

  for (size_t remaining = std::min(pos, size - count) + 1; remaining > 0; --remaining) {
    const size_t index = remaining - 1;

    // Ends reject most positions before compare(); the backward walk keeps the early exit a forward scan loses.
    if (!traits_type::eq(data[index], pattern[0]))
      continue;
    if (count > 1 && !traits_type::eq(data[index + count - 1], pattern[count - 1]))
      continue;
    if (count <= 2 || traits_type::compare(data + index + 1, pattern + 1, count - 2) == 0)
      return index;
  }

  return c_npos;
}

constexpr size_t findFirstOf(const char * data, size_t size, const char * set, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(set != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return c_npos;

  for (size_t index = pos; index < size; ++index)
    if (traits_type::find(set, count, data[index]) != nullptr)
      return index;

  return c_npos;
}

constexpr size_t findLastOf(const char * data, size_t size, const char * set, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(set != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return c_npos;

  for (size_t remaining = pos < size ? pos + 1 : size; remaining > 0; --remaining)
    if (traits_type::find(set, count, data[remaining - 1]) != nullptr)
      return remaining - 1;

  return c_npos;
}

constexpr size_t findFirstNotOf(const char * data, size_t size, char ch, size_t pos) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");

  for (size_t index = pos; index < size; ++index)
    if (!traits_type::eq(data[index], ch))
      return index;

  return c_npos;
}

constexpr size_t findFirstNotOf(const char * data, size_t size, const char * set, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(set != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return pos < size ? pos : c_npos;

  for (size_t index = pos; index < size; ++index)
    if (traits_type::find(set, count, data[index]) == nullptr)
      return index;

  return c_npos;
}

constexpr size_t findLastNotOf(const char * data, size_t size, char ch, size_t pos) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");

  for (size_t remaining = pos < size ? pos + 1 : size; remaining > 0; --remaining)
    if (!traits_type::eq(data[remaining - 1], ch))
      return remaining - 1;

  return c_npos;
}

constexpr size_t findLastNotOf(const char * data, size_t size, const char * set, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(set != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return size == 0 ? c_npos : std::min(pos, size - 1);

  for (size_t remaining = pos < size ? pos + 1 : size; remaining > 0; --remaining)
    if (traits_type::find(set, count, data[remaining - 1]) == nullptr)
      return remaining - 1;

  return c_npos;
}

constexpr int compare(const char * lhs, size_t lhsSize, const char * rhs, size_t rhsSize) noexcept {
  assert_message(lhs != nullptr || lhsSize == 0, "a null pointer names no characters to compare");
  assert_message(rhs != nullptr || rhsSize == 0, "a null pointer names no characters to compare with");

  // memcmp behind std::char_traits::compare takes no null pointer, even for an empty range.
  const size_t common = std::min(lhsSize, rhsSize);
  if (const int result = common != 0 ? traits_type::compare(lhs, rhs, common) : 0; result != 0)
    return result;

  return lhsSize == rhsSize ? 0 : (lhsSize < rhsSize ? -1 : 1);
}

} // namespace toy::string_utils

#endif // INCLUDE_CORE_STRING_UTILS_INL_
