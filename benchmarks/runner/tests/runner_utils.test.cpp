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
  \file   runner_utils.test.cpp
  \brief  Unit tests for the benchmark runner's name ordering and integer formatting.
*/

#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <span>
#include <string_view>

#include <doctest/doctest.h>

#include "runner_utils.hpp"

namespace {

// The helpers write through a span, so a returned buffer gives each static_assert one call to inspect.
struct Formatted final {
  std::array<char, 32> buffer;
  std::size_t          length;

  [[nodiscard]] constexpr std::string_view view() const noexcept {
    return {buffer.data(), length};
  }
};

[[nodiscard]] constexpr Formatted formatText(std::size_t capacity, const char * text) noexcept {
  Formatted result{};

  result.length = toy::benchmark::detail::appendText(std::span<char>{result.buffer}.first(capacity), 0, text);

  return result;
}

[[nodiscard]] constexpr Formatted formatDecimal(std::uint64_t value) noexcept {
  Formatted result{};

  result.length = toy::benchmark::detail::appendDecimal(result.buffer, 0, value);

  return result;
}

} // namespace

// Names order by their first differing byte. A prefix comes before its extension, and a high byte after ASCII.
TEST_CASE("benchmark::detail/compare_names/orders_by_unsigned_bytes") {
  CHECK(toy::benchmark::detail::compareNames("core/a", "core/b") == -1);
  CHECK(toy::benchmark::detail::compareNames("core/b", "core/a") == 1);
  CHECK(toy::benchmark::detail::compareNames("core/a", "core/a") == 0);
  CHECK(toy::benchmark::detail::compareNames("core", "core/a") == -1);
  CHECK(toy::benchmark::detail::compareNames("\xFF", "z") == 1);

  static_assert(toy::benchmark::detail::compareNames("core/a", "core/b") == -1,
                "a name orders before one with a greater byte at the first difference");
  static_assert(toy::benchmark::detail::compareNames("core/b", "core/a") == 1,
                "a name orders after one with a smaller byte at the first difference");
  static_assert(toy::benchmark::detail::compareNames("core/a", "core/a") == 0, "equal names compare equal");
  static_assert(toy::benchmark::detail::compareNames("core", "core/a") == -1, "a prefix orders before its extension");
  static_assert(toy::benchmark::detail::compareNames("\x{FF}", "z") == 1, "bytes compare as unsigned");
}

// The copy stops where the buffer ends, and the returned offset stops with it.
TEST_CASE("benchmark::detail/append_text/stops_at_the_buffer_end") {
  CHECK(formatText(8, "search").view() == "search");
  CHECK(formatText(3, "search").view() == "sea");

  static_assert(formatText(8, "search").view() == "search", "text shorter than the buffer is copied whole");
  static_assert(formatText(3, "search").view() == "sea", "text is cut where the buffer ends");
}

// Digits come out most significant first. Zero is one digit, and the largest 64-bit value fits whole.
TEST_CASE("benchmark::detail/append_decimal/writes_the_digits") {
  CHECK(formatDecimal(63695246).view() == "63695246");
  CHECK(formatDecimal(0).view() == "0");
  CHECK(formatDecimal(18446744073709551615ULL).view() == "18446744073709551615");

  static_assert(formatDecimal(63695246).view() == "63695246", "digits are written most significant first");
  static_assert(formatDecimal(0).view() == "0", "zero is written as a single digit");
  static_assert(formatDecimal(18446744073709551615ULL).view() == "18446744073709551615",
                "all twenty digits of the largest 64-bit value are written");
}
