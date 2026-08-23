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
  \file   utils.test.cpp
  \brief  Unit tests for the runner's magnitude, name-ordering and buffer-formatting helpers.
*/

#include <array>
#include <cstddef>

#include <doctest/doctest.h>

#include "toy_test.hpp"

namespace {

// The append helpers write through a pointer, which a constant expression cannot hand them directly.
// Wrapping the buffer in a return value gives every static_assert below one constexpr call to inspect.
template <std::size_t Capacity>
struct AppendResult final {
  std::array<char, Capacity> buffer;
  std::size_t                written;
};

template <std::size_t Capacity>
[[nodiscard]] constexpr AppendResult<Capacity> appendTextInto(const char * text) noexcept {
  AppendResult<Capacity> result{};
  result.written = toy::test::detail::appendText(result.buffer.data(), Capacity, 0, text);

  return result;
}

template <std::size_t Capacity>
[[nodiscard]] constexpr AppendResult<Capacity> appendIntegerInto(long long value) noexcept {
  AppendResult<Capacity> result{};
  result.written = toy::test::detail::appendInteger(result.buffer.data(), Capacity, 0, value);

  return result;
}

// Appending at a full buffer needs the offset as a parameter, which the two helpers above fix at zero.
template <std::size_t Capacity>
[[nodiscard]] constexpr AppendResult<Capacity> appendTextAt(std::size_t offset, const char * text) noexcept {
  AppendResult<Capacity> result{};
  result.written = toy::test::detail::appendText(result.buffer.data(), Capacity, offset, text);

  return result;
}

template <std::size_t Capacity>
[[nodiscard]] constexpr AppendResult<Capacity> appendIntegerAt(std::size_t offset, long long value) noexcept {
  AppendResult<Capacity> result{};
  result.written = toy::test::detail::appendInteger(result.buffer.data(), Capacity, offset, value);

  return result;
}

} // namespace

// absoluteValue drops the sign and leaves a positive value alone.
TEST_CASE("test::detail/absolute_value/drops_the_sign") {
  REQUIRE(toy::test::detail::absoluteValue(-1.5) == 1.5);
  REQUIRE(toy::test::detail::absoluteValue(1.5) == 1.5);
  REQUIRE(toy::test::detail::absoluteValue(0.0) == 0.0);

  static_assert(toy::test::detail::absoluteValue(-1.5) == 1.5, "a negative value must lose its sign");
  static_assert(toy::test::detail::absoluteValue(1.5) == 1.5, "a positive value must survive unchanged");
  static_assert(toy::test::detail::absoluteValue(-1.5F) == 1.5F, "float must lose its sign the same way");
  static_assert(toy::test::detail::absoluteValue(-1.5L) == 1.5L, "long double must lose its sign the same way");
}

// compareNames orders by the first differing byte and reports equality as zero.
TEST_CASE("test::detail/compare_names/orders_lexicographically") {
  REQUIRE(toy::test::detail::compareNames("abc", "abd") < 0);
  REQUIRE(toy::test::detail::compareNames("abd", "abc") > 0);
  REQUIRE(toy::test::detail::compareNames("abc", "abc") == 0);

  static_assert(toy::test::detail::compareNames("abc", "abd") < 0, "abc must order before abd");
  static_assert(toy::test::detail::compareNames("abc", "abc") == 0, "equal names must compare equal");
}

// A prefix orders before the longer name that contains it.
TEST_CASE("test::detail/compare_names/prefix_orders_first") {
  REQUIRE(toy::test::detail::compareNames("core", "core/utils") < 0);
  REQUIRE(toy::test::detail::compareNames("core/utils", "core") > 0);

  static_assert(toy::test::detail::compareNames("core", "core/utils") < 0,
                "a prefix must order before the longer name");
}

// Bytes above 127 compare as unsigned, so ordering stays total on every target.
TEST_CASE("test::detail/compare_names/compares_bytes_as_unsigned") {
  REQUIRE(toy::test::detail::compareNames("\x{7f}", "\x{80}") < 0);

  static_assert(toy::test::detail::compareNames("\x{7f}", "\x{80}") < 0, "byte 0x80 must order after byte 0x7f");
}

// appendText copies until the terminator and returns the new offset.
TEST_CASE("test::detail/append_text/copies_and_advances_offset") {
  char              buffer[8] = {};
  const std::size_t written   = toy::test::detail::appendText(buffer, sizeof(buffer), 0, "ok");

  REQUIRE(written == 2);
  REQUIRE(buffer[0] == 'o');
  REQUIRE(buffer[1] == 'k');

  constexpr auto copied = appendTextInto<8>("ok");
  static_assert(copied.written == 2, "appendText must return the offset past the two bytes it wrote");
  static_assert(copied.buffer[0] == 'o', "the first byte of the text must land at the offset");
  static_assert(copied.buffer[1] == 'k', "the second byte of the text must follow the first");
}

// appendText stops at capacity instead of running past the buffer.
TEST_CASE("test::detail/append_text/truncates_at_capacity") {
  char              buffer[3] = {};
  const std::size_t written   = toy::test::detail::appendText(buffer, sizeof(buffer), 0, "abcdef");

  REQUIRE(written == 3);
  REQUIRE(buffer[2] == 'c');

  constexpr auto truncated = appendTextInto<3>("abcdef");
  static_assert(truncated.written == 3, "a truncated append must return the capacity");
  static_assert(truncated.buffer[2] == 'c', "the last byte that fits must be the third of the text");
}

// A buffer already at capacity takes nothing, and the offset comes back unchanged.
TEST_CASE("test::detail/append_text/writes_nothing_at_a_full_buffer") {
  char              buffer[4] = {};
  const std::size_t written   = toy::test::detail::appendText(buffer, sizeof(buffer), sizeof(buffer), "abc");

  REQUIRE(written == sizeof(buffer));
  REQUIRE(buffer[0] == '\0');

  constexpr auto full = appendTextAt<4>(4, "abc");
  static_assert(full.written == 4, "an append at capacity must return the capacity");
  static_assert(full.buffer[0] == '\0', "an append at capacity must leave the buffer untouched");
}

// appendInteger writes the decimal digits in order, most significant first.
TEST_CASE("test::detail/append_integer/writes_decimal_digits") {
  char              buffer[8] = {};
  const std::size_t written   = toy::test::detail::appendInteger(buffer, sizeof(buffer), 0, 1024);

  REQUIRE(written == 4);
  REQUIRE(buffer[0] == '1');
  REQUIRE(buffer[3] == '4');

  constexpr auto formatted = appendIntegerInto<8>(1024);
  static_assert(formatted.written == 4, "1024 must format as four digits");
  static_assert(formatted.buffer[0] == '1', "the most significant digit must come first");
  static_assert(formatted.buffer[3] == '4', "the least significant digit must come last");
}

// Zero is one digit, not an empty string.
TEST_CASE("test::detail/append_integer/writes_zero_as_single_digit") {
  char              buffer[4] = {};
  const std::size_t written   = toy::test::detail::appendInteger(buffer, sizeof(buffer), 0, 0);

  REQUIRE(written == 1);
  REQUIRE(buffer[0] == '0');

  constexpr auto zero = appendIntegerInto<4>(0);
  static_assert(zero.written == 1, "zero must format as one digit");
  static_assert(zero.buffer[0] == '0', "zero must format as the digit zero");
}

// The most negative value has no positive counterpart, so the magnitude is taken in unsigned arithmetic.
TEST_CASE("test::detail/append_integer/writes_most_negative_value") {
  char              buffer[24] = {};
  const std::size_t written = toy::test::detail::appendInteger(buffer, sizeof(buffer), 0, -9223372036854775807LL - 1LL);

  REQUIRE(written == 20);
  REQUIRE(buffer[0] == '-');
  REQUIRE(buffer[1] == '9');
  REQUIRE(buffer[19] == '8');

  constexpr auto mostNegative = appendIntegerInto<24>(-9223372036854775807LL - 1LL);
  static_assert(mostNegative.written == 20, "the most negative value must format as a sign and nineteen digits");
  static_assert(mostNegative.buffer[0] == '-', "a negative value must open with a sign");
  static_assert(mostNegative.buffer[19] == '8', "the magnitude must survive the unsigned negation");
}

// A buffer already at capacity takes no digits, not even a sign.
TEST_CASE("test::detail/append_integer/writes_nothing_at_a_full_buffer") {
  char              buffer[4] = {};
  const std::size_t written   = toy::test::detail::appendInteger(buffer, sizeof(buffer), sizeof(buffer), -42);

  REQUIRE(written == sizeof(buffer));
  REQUIRE(buffer[0] == '\0');

  constexpr auto full = appendIntegerAt<4>(4, -42);
  static_assert(full.written == 4, "an append at capacity must return the capacity");
  static_assert(full.buffer[0] == '\0', "an append at capacity must leave the buffer untouched");
}
