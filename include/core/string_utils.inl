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

namespace detail {

/// Table with one entry per byte value, \c true where that byte belongs to a set.
using membership_table_type = std::array<bool, 256>;

/*!
  \brief Builds the membership table of the \a count bytes starting at \a set.

  \param set   First byte of the set.
  \param count Number of bytes in the set.

  \return Table holding \c true at every byte value found in the set and \c false elsewhere.

  \pre The \a count bytes starting at \a set are readable; \a set may be \c nullptr only when \a count is \c 0.
*/
constexpr membership_table_type membershipTable(const char * set, size_t count) noexcept {
  membership_table_type table{};

  for (const char ch : std::views::counted(set, count))
    table[static_cast<unsigned char>(ch)] = true;

  return table;
}

/*!
  \brief Tests whether \a ch belongs to the set \a table was built from.

  Reads \a ch as \c unsigned \c char, so a byte above \c 0x7f indexes the table at 128 to 255.

  \param table Membership table from membershipTable().
  \param ch    Character to test.

  \return \c true when \a ch belongs to the set.
*/
constexpr bool isMember(const membership_table_type & table, char ch) noexcept {
  return table[static_cast<unsigned char>(ch)];
}

} // namespace detail

constexpr size_t find(const char * data, size_t size, char ch, size_t pos) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");

  if (pos >= size)
    return npos;

  // memchr at runtime, a plain scan under constant evaluation; neither reads a byte before pos.
  const char * found = traits_type::find(data + pos, size - pos, ch);

  return found != nullptr ? static_cast<size_t>(found - data) : npos;
}

constexpr size_t find(const char * data, size_t size, const char * pattern, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(pattern != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return pos <= size ? pos : npos;
  else if (count > size)
    return npos;

  const size_t last = size - count;
  for (size_t index = pos; index <= last;) {
    // memchr at runtime: skips to the next position that can start a match instead of comparing at every one.
    const char * candidate = traits_type::find(data + index, last - index + 1, pattern[0]);
    if (candidate == nullptr)
      return npos;

    index = static_cast<size_t>(candidate - data);

    // The first character matched already, so the comparison starts behind it.
    if (traits_type::compare(candidate + 1, pattern + 1, count - 1) == 0)
      return index;

    ++index;
  }

  return npos;
}

constexpr size_t rfind(const char * data, size_t size, char ch, size_t pos) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");

  if (size == 0)
    return npos;

  size_t index = std::min(pos, size - 1);
  do {
    if (traits_type::eq(data[index], ch))
      return index;
  } while (index-- != 0);

  return npos;
}

constexpr size_t rfind(const char * data, size_t size, const char * pattern, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(pattern != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return std::min(pos, size);
  else if (count > size)
    return npos;

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

  return npos;
}

constexpr size_t findFirstOf(const char * data, size_t size, const char * set, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(set != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return npos;

  const auto members = detail::membershipTable(set, count);
  for (size_t index = pos; index < size; ++index)
    if (detail::isMember(members, data[index]))
      return index;

  return npos;
}

constexpr size_t findLastOf(const char * data, size_t size, const char * set, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(set != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return npos;

  const auto members = detail::membershipTable(set, count);
  for (size_t remaining = pos < size ? pos + 1 : size; remaining > 0; --remaining)
    if (detail::isMember(members, data[remaining - 1]))
      return remaining - 1;

  return npos;
}

constexpr size_t findFirstNotOf(const char * data, size_t size, char ch, size_t pos) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");

  for (size_t index = pos; index < size; ++index)
    if (!traits_type::eq(data[index], ch))
      return index;

  return npos;
}

constexpr size_t findFirstNotOf(const char * data, size_t size, const char * set, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(set != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return pos < size ? pos : npos;

  const auto members = detail::membershipTable(set, count);
  for (size_t index = pos; index < size; ++index)
    if (!detail::isMember(members, data[index]))
      return index;

  return npos;
}

constexpr size_t findLastNotOf(const char * data, size_t size, char ch, size_t pos) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");

  if (size == 0)
    return npos;

  size_t index = std::min(pos, size - 1);
  do {
    if (!traits_type::eq(data[index], ch))
      return index;
  } while (index-- != 0);

  return npos;
}

constexpr size_t findLastNotOf(const char * data, size_t size, const char * set, size_t pos, size_t count) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to search");
  assert_message(set != nullptr || count == 0, "a null pointer names no characters to search for");

  if (count == 0)
    return size == 0 ? npos : std::min(pos, size - 1);

  const auto members = detail::membershipTable(set, count);
  for (size_t remaining = pos < size ? pos + 1 : size; remaining > 0; --remaining)
    if (!detail::isMember(members, data[remaining - 1]))
      return remaining - 1;

  return npos;
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

constexpr size_t utf8Size(const char * data, size_t size) noexcept {
  assert_message(data != nullptr || size == 0, "a null pointer names no characters to count");
  if !consteval {
    assert_message(validateUtf8(data, size), "the counted bytes must hold well-formed UTF-8");
  }

  return static_cast<size_t>(std::count_if(data, data + size, [](char byte) {
    return (static_cast<unsigned char>(byte) & 0xC0) != 0x80;
  }));
}

constexpr char * reverseString(char * str, size_t count) noexcept {
  if (count > 0) {
    assert_message(str != nullptr, "The source string must not be null.");

    if consteval {
      std::reverse(str, str + count);
    } else {
      auto * const bytes = reinterpret_cast<byte *>(str);
      std::reverse(bytes, bytes + count);
    }
  }

  return str;
}

} // namespace toy::string_utils

#endif // INCLUDE_CORE_STRING_UTILS_INL_
