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
  \file   utils.inl
  \brief  Inline implementations for the runner's magnitude, name-ordering and buffer-formatting helpers.

  \note Included by utils.hpp only; do not include this file directly.
*/

namespace toy::test::detail {

template <std::floating_point T>
constexpr T absoluteValue(T value) noexcept {
  return value < T{0} ? -value : value;
}

constexpr int compareNames(const char * lhs, const char * rhs) noexcept {
  while (*lhs != '\0' && *lhs == *rhs) {
    ++lhs;
    ++rhs;
  }

  const auto left  = static_cast<unsigned char>(*lhs);
  const auto right = static_cast<unsigned char>(*rhs);

  if (left < right)
    return -1;

  return left > right ? 1 : 0;
}

constexpr std::size_t appendText(char * buffer, std::size_t capacity, std::size_t offset, const char * text) noexcept {
  TOY_TEST_ASSERT(offset <= capacity, "offset must not exceed the buffer capacity");

  while (*text != '\0' && offset < capacity) {
    buffer[offset] = *text;
    ++offset;
    ++text;
  }

  return offset;
}

constexpr std::size_t appendEscaped(char * buffer, std::size_t capacity, std::size_t offset,
                                    const char * text) noexcept {
  TOY_TEST_ASSERT(offset <= capacity, "offset must not exceed the buffer capacity");

  while (*text != '\0' && offset < capacity) {
    if (*text == '#' || *text == '\\') {
      // The pair goes in whole or not at all: a line ending in a lone backslash would escape its own line break.
      if (offset + 1 >= capacity)
        break;

      buffer[offset] = '\\';
      ++offset;
    }

    buffer[offset] = *text;
    ++offset;
    ++text;
  }

  return offset;
}

constexpr std::size_t appendQuoted(char * buffer, std::size_t capacity, std::size_t offset,
                                   const char * text) noexcept {
  TOY_TEST_ASSERT(offset <= capacity, "offset must not exceed the buffer capacity");

  // A scalar is two quotes at the very least; anything shorter would leave the block with an unterminated one.
  if (capacity - offset < 2)
    return offset;

  buffer[offset] = '\'';
  ++offset;

  while (*text != '\0') {
    const std::size_t width = *text == '\'' ? 2 : 1;

    // The closing quote owns the last byte, and a doubled apostrophe never splits across it.
    if (offset + width >= capacity)
      break;

    if (width == 2) {
      buffer[offset] = '\'';
      ++offset;
    }

    buffer[offset] = *text;
    ++offset;
    ++text;
  }

  buffer[offset] = '\'';
  ++offset;

  return offset;
}

constexpr std::size_t appendInteger(char * buffer, std::size_t capacity, std::size_t offset, long long value) noexcept {
  TOY_TEST_ASSERT(offset <= capacity, "offset must not exceed the buffer capacity");

  // Taking the magnitude in unsigned arithmetic keeps the most negative value representable.
  unsigned long long magnitude = value < 0 ? 0ULL - static_cast<unsigned long long>(value)
                                           : static_cast<unsigned long long>(value);

  std::array<char, 20> digits     = {};
  std::size_t          digitCount = 0;

  do {
    digits[digitCount] = static_cast<char>('0' + static_cast<char>(magnitude % 10ULL));
    ++digitCount;
    magnitude /= 10ULL;
  } while (magnitude != 0ULL);

  if (value < 0 && offset < capacity) {
    buffer[offset] = '-';
    ++offset;
  }

  while (digitCount > 0 && offset < capacity) {
    --digitCount;
    buffer[offset] = digits[digitCount];
    ++offset;
  }

  return offset;
}

} // namespace toy::test::detail
