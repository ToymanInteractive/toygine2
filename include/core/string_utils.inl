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
  \brief  Inline implementations for the \ref toy::string_utils search and integer formatting functions.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_UTILS_INL_
#define INCLUDE_CORE_STRING_UTILS_INL_

namespace toy::string_utils {

namespace detail {

/*!
  \brief Builds the membership table of \a set.

  \param set Characters to mark.

  \return Table holding \c true at every byte value found in the set and \c false elsewhere.

  \pre The characters of \a set are readable; a null \a set comes with an empty range.
*/
constexpr array<bool, 256> membershipTable(span<const char> set) noexcept {
  array<bool, 256> table{};

  for (const char ch : set)
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
constexpr bool isMember(const array<bool, 256> & table, char ch) noexcept {
  return table[static_cast<unsigned char>(ch)];
}

/// The two decimal digits of every value below 100, so a pair reads as two adjacent bytes.
inline constexpr array<char, 200> digitPairs = [] {
  array<char, 200> pairs{};

  for (size_t number = 0; number < 100; ++number) {
    pairs[number * 2]     = static_cast<char>('0' + number / 10);
    pairs[number * 2 + 1] = static_cast<char>('0' + number % 10);
  }

  return pairs;
}();

/// Sixteen digits of the hexadecimal alphabet, indexed by nibble.
inline constexpr array hexDigits{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

} // namespace detail

constexpr size_t find(span<const char> data, char ch, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");

  if (pos >= data.size())
    return npos;

  // memchr at runtime, a plain scan under constant evaluation; neither reads a byte before pos.
  const char * found = traits_type::find(data.data() + pos, data.size() - pos, ch);

  return found != nullptr ? static_cast<size_t>(found - data.data()) : npos;
}

constexpr size_t find(span<const char> data, span<const char> pattern, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");
  assert_message(pattern.data() != nullptr || pattern.empty(), "a null pointer names no characters to search for");

  const size_t size  = data.size();
  const size_t count = pattern.size();

  if (count == 0)
    return pos <= size ? pos : npos;
  else if (count > size)
    return npos;

  const size_t last = size - count;
  for (size_t index = pos; index <= last;) {
    // memchr at runtime: skips to the next position that can start a match instead of comparing at every one.
    const char * candidate = traits_type::find(data.data() + index, last - index + 1, pattern[0]);
    if (candidate == nullptr)
      return npos;

    index = static_cast<size_t>(candidate - data.data());

    // The first character matched already, so the comparison starts behind it.
    if (traits_type::compare(candidate + 1, pattern.data() + 1, count - 1) == 0)
      return index;

    ++index;
  }

  return npos;
}

constexpr size_t rfind(span<const char> data, char ch, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");

  for (size_t remaining = pos < data.size() ? pos + 1 : data.size(); remaining > 0; --remaining)
    if (traits_type::eq(data[remaining - 1], ch))
      return remaining - 1;

  return npos;
}

constexpr size_t rfind(span<const char> data, span<const char> pattern, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");
  assert_message(pattern.data() != nullptr || pattern.empty(), "a null pointer names no characters to search for");

  const size_t size  = data.size();
  const size_t count = pattern.size();

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
    if (count <= 2 || traits_type::compare(data.data() + index + 1, pattern.data() + 1, count - 2) == 0)
      return index;
  }

  return npos;
}

constexpr size_t findFirstOf(span<const char> data, span<const char> set, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");
  assert_message(set.data() != nullptr || set.empty(), "a null pointer names no characters to search for");

  if (set.empty())
    return npos;

  const auto members = detail::membershipTable(set);
  for (size_t index = pos; index < data.size(); ++index)
    if (detail::isMember(members, data[index]))
      return index;

  return npos;
}

constexpr size_t findLastOf(span<const char> data, span<const char> set, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");
  assert_message(set.data() != nullptr || set.empty(), "a null pointer names no characters to search for");

  if (set.empty())
    return npos;

  const auto members = detail::membershipTable(set);
  for (size_t remaining = pos < data.size() ? pos + 1 : data.size(); remaining > 0; --remaining)
    if (detail::isMember(members, data[remaining - 1]))
      return remaining - 1;

  return npos;
}

constexpr size_t findFirstNotOf(span<const char> data, char ch, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");

  for (size_t index = pos; index < data.size(); ++index)
    if (!traits_type::eq(data[index], ch))
      return index;

  return npos;
}

constexpr size_t findFirstNotOf(span<const char> data, span<const char> set, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");
  assert_message(set.data() != nullptr || set.empty(), "a null pointer names no characters to search for");

  if (set.empty())
    return pos < data.size() ? pos : npos;

  const auto members = detail::membershipTable(set);
  for (size_t index = pos; index < data.size(); ++index)
    if (!detail::isMember(members, data[index]))
      return index;

  return npos;
}

constexpr size_t findLastNotOf(span<const char> data, char ch, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");

  for (size_t remaining = pos < data.size() ? pos + 1 : data.size(); remaining > 0; --remaining)
    if (!traits_type::eq(data[remaining - 1], ch))
      return remaining - 1;

  return npos;
}

constexpr size_t findLastNotOf(span<const char> data, span<const char> set, size_t pos) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to search");
  assert_message(set.data() != nullptr || set.empty(), "a null pointer names no characters to search for");

  if (set.empty())
    return data.empty() ? npos : std::min(pos, data.size() - 1);

  const auto members = detail::membershipTable(set);
  for (size_t remaining = pos < data.size() ? pos + 1 : data.size(); remaining > 0; --remaining)
    if (!detail::isMember(members, data[remaining - 1]))
      return remaining - 1;

  return npos;
}

constexpr int compare(span<const char> lhs, span<const char> rhs) noexcept {
  assert_message(lhs.data() != nullptr || lhs.empty(), "a null pointer names no characters to compare");
  assert_message(rhs.data() != nullptr || rhs.empty(), "a null pointer names no characters to compare with");

  // memcmp behind std::char_traits::compare takes no null pointer, even for an empty range.
  const size_t common = std::min(lhs.size(), rhs.size());
  if (const int result = common != 0 ? traits_type::compare(lhs.data(), rhs.data(), common) : 0; result != 0)
    return result;

  if (lhs.size() == rhs.size())
    return 0;

  return lhs.size() < rhs.size() ? -1 : 1;
}

constexpr size_t utf8Size(span<const char> data) noexcept {
  assert_message(data.data() != nullptr || data.empty(), "a null pointer names no characters to count");
  if !consteval {
    assert_message(validateUtf8(data), "the counted bytes must hold well-formed UTF-8");
  }

  return static_cast<size_t>(std::count_if(data.begin(), data.end(), [](char byte) {
    return (static_cast<unsigned char>(byte) & 0xC0) != 0x80;
  }));
}

template <std::signed_integral T>
constexpr char * itoa(span<char> dest, T value) noexcept {
  assert_message(dest.data() != nullptr, "a null pointer names no buffer to write into");
  assert_message(dest.size() >= std::numeric_limits<T>::digits10 + 3,
                 "the capacity must hold the widest value of the type");

  if (value >= 0)
    return utoa(dest, static_cast<std::make_unsigned_t<T>>(value));

  if (dest.data() == nullptr || dest.size() == 0)
    return dest.data();
  else if (dest.size() == 1) {
    *dest.data() = '\0';

    return dest.data();
  }

  // The sum is computed in unsigned int; the outer cast keeps utoa on the width of T, as the non-negative branch does.
  using Unsigned           = std::make_unsigned_t<T>;
  const Unsigned magnitude = static_cast<Unsigned>(static_cast<Unsigned>(-(value + 1)) + 1U);

  char * digits = utoa(span<char>(dest.data() + 1, dest.size() - 1), magnitude);
  *--digits     = '-';

  return digits;
}

template <std::unsigned_integral T>
constexpr char * utoa(span<char> dest, T value) noexcept {
  assert_message(dest.data() != nullptr, "a null pointer names no buffer to write into");
  assert_message(dest.size() > 0, "a zero capacity cannot hold even a terminator");

  if (dest.data() == nullptr || dest.size() == 0)
    return dest.data();

  char * const base = dest.data();

  // A single digit comes from the value with no division; the loop below divides once per digit.
  if (value < 10) {
    if (dest.size() < 2) {
      *base = '\0';

      return base;
    }

    base[dest.size() - 2] = static_cast<char>('0' + static_cast<uint8_t>(value));
    base[dest.size() - 1] = '\0';

    return base + dest.size() - 2;
  }

  // Two digits and the terminator need three bytes.
  if (value < 100 && dest.size() >= 3) {
    const char * const digits = &detail::digitPairs[static_cast<size_t>(value) * 2];
    char * const       start  = base + dest.size() - 3;
    start[0]                  = digits[0];
    start[1]                  = digits[1];
    start[2]                  = '\0';

    return start;
  }

  char * end = base + dest.size();
  *--end     = '\0';

  // A window that holds the widest value of T needs no room test per digit; a narrower one truncates.
  const bool fullWindow = dest.size() >= std::numeric_limits<T>::digits10 + 2;

  while (value >= 100 && (fullWindow || end - base >= 2)) {
    const T    quotient  = value / static_cast<T>(100);
    const auto remainder = static_cast<size_t>(value - quotient * static_cast<T>(100));
    value                = quotient;

    end    -= 2;
    end[0]  = detail::digitPairs[remainder * 2];
    end[1]  = detail::digitPairs[remainder * 2 + 1];
  }

  // Single digits finish the value: the leading one or two, or the single digit a narrow window has room for.
  do {
    if (!fullWindow && end == base)
      break;

    *--end = static_cast<char>('0' + static_cast<uint8_t>(value % 10));
  } while ((value /= static_cast<T>(10)) > 0);

  return end;
}

template <std::unsigned_integral T>
constexpr char * utoaHex(span<char> dest, T value) noexcept {
  assert_message(dest.data() != nullptr, "a null pointer names no buffer to write into");
  assert_message(dest.size() > 0, "a zero capacity cannot hold even a terminator");

  if (dest.data() == nullptr || dest.size() == 0)
    return dest.data();

  char * end = dest.data() + dest.size();
  *--end     = '\0';

  const size_t digits = sizeof(T) * 2;
  const auto   room   = static_cast<size_t>(end - dest.data());
  const auto   count  = std::min(digits, room);

  for (size_t index = count; index > 0; --index) {
    const auto nibble   = static_cast<unsigned>(value & 0x0FU);
    *--end              = detail::hexDigits[nibble];
    value             >>= 4;
  }

  return end;
}

} // namespace toy::string_utils

#endif // INCLUDE_CORE_STRING_UTILS_INL_
