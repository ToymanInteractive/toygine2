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
  \file   runner_utils.inl
  \brief  Inline implementations for the benchmark runner's name ordering and integer formatting.

  \note Included by runner_utils.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_RUNNER_UTILS_INL_
#define INCLUDE_BENCHMARKS_RUNNER_RUNNER_UTILS_INL_

#include <array>

namespace toy::benchmark::detail {

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

constexpr std::size_t appendText(std::span<char> buffer, std::size_t offset, const char * text) noexcept {
  assert_message(offset <= buffer.size(), "offset must not exceed the buffer size");

  while (*text != '\0' && offset < buffer.size()) {
    buffer[offset] = *text;
    ++offset;
    ++text;
  }

  return offset;
}

constexpr std::size_t appendDecimal(std::span<char> buffer, std::size_t offset, std::uint64_t value) noexcept {
  assert_message(offset <= buffer.size(), "offset must not exceed the buffer size");

  // Digits come out least significant first; twenty of them hold the largest 64-bit value.
  std::array<char, 20> reversed{};
  std::size_t          length = 0;

  do {
    reversed[length] = static_cast<char>('0' + value % 10U);
    ++length;
    value /= 10U;
  } while (value != 0U);

  while (length > 0 && offset < buffer.size()) {
    --length;
    buffer[offset] = reversed[length];
    ++offset;
  }

  return offset;
}

} // namespace toy::benchmark::detail

#endif // INCLUDE_BENCHMARKS_RUNNER_RUNNER_UTILS_INL_
