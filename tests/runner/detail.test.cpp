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
  \file   detail.test.cpp
  \brief  Unit tests for \ref toy::test::Approx and the runner's name and number helpers.
*/

#include <limits>
#include <type_traits>

#include <doctest/doctest.h>

#include "toy_test.hpp"

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
  char         buffer[8] = {};
  const size_t written   = toy::test::detail::appendText(buffer, sizeof(buffer), 0, "ok");

  REQUIRE(written == 2);
  REQUIRE(buffer[0] == 'o');
  REQUIRE(buffer[1] == 'k');
}

// appendText stops at capacity instead of running past the buffer.
TEST_CASE("test::detail/append_text/truncates_at_capacity") {
  char         buffer[3] = {};
  const size_t written   = toy::test::detail::appendText(buffer, sizeof(buffer), 0, "abcdef");

  REQUIRE(written == 3);
  REQUIRE(buffer[2] == 'c');
}

// appendInteger writes the decimal digits in order, most significant first.
TEST_CASE("test::detail/append_integer/writes_decimal_digits") {
  char         buffer[8] = {};
  const size_t written   = toy::test::detail::appendInteger(buffer, sizeof(buffer), 0, 1024);

  REQUIRE(written == 4);
  REQUIRE(buffer[0] == '1');
  REQUIRE(buffer[3] == '4');
}

// Zero is one digit, not an empty string.
TEST_CASE("test::detail/append_integer/writes_zero_as_single_digit") {
  char         buffer[4] = {};
  const size_t written   = toy::test::detail::appendInteger(buffer, sizeof(buffer), 0, 0);

  REQUIRE(written == 1);
  REQUIRE(buffer[0] == '0');
}

// The most negative value has no positive counterpart, so the magnitude is taken in unsigned arithmetic.
TEST_CASE("test::detail/append_integer/writes_most_negative_value") {
  char         buffer[24] = {};
  const size_t written    = toy::test::detail::appendInteger(buffer, sizeof(buffer), 0, -9223372036854775807LL - 1LL);

  REQUIRE(written == 20);
  REQUIRE(buffer[0] == '-');
  REQUIRE(buffer[1] == '9');
  REQUIRE(buffer[19] == '8');
}

// Approx compares equal within its tolerance and unequal outside it.
TEST_CASE("test::approx/comparison/honours_tolerance") {
  REQUIRE(1.0 == toy::test::Approx(1.0));
  REQUIRE(toy::test::Approx(1.0) == 1.0);
  REQUIRE_FALSE(1.0 == toy::test::Approx(2.0));

  static_assert(1.0 == toy::test::Approx(1.0), "a value must compare equal to itself");
}

// A widened epsilon accepts a difference the default tolerance rejects.
TEST_CASE("test::approx/comparison/epsilon_widens_tolerance") {
  REQUIRE_FALSE(1.0 == toy::test::Approx(1.5));
  REQUIRE(1.0 == toy::test::Approx(1.5).epsilon(0.5));

  static_assert(1.0 == toy::test::Approx(1.5).epsilon(0.5), "epsilon 0.5 must accept a difference of 0.5");
}

// Tolerance scales with magnitude, so large values keep the same relative precision.
TEST_CASE("test::approx/comparison/scales_with_magnitude") {
  REQUIRE(1000000.0 == toy::test::Approx(1000000.001));
  REQUIRE_FALSE(1.0 == toy::test::Approx(1.001));
}

// Every standard floating-point type satisfies the constraint and compares within its own precision.
TEST_CASE("test::approx/comparison/accepts_every_floating_point_type") {
  SUBCASE("float") {
    REQUIRE(1.0F == toy::test::Approx(1.0F));
    REQUIRE_FALSE(1.0F == toy::test::Approx(2.0F));

    static_assert(1.0F == toy::test::Approx(1.0F), "float must compare equal to itself");
  }

  SUBCASE("double") {
    REQUIRE(1.0 == toy::test::Approx(1.0));

    static_assert(1.0 == toy::test::Approx(1.0), "double must compare equal to itself");
  }

  SUBCASE("long double") {
    REQUIRE(1.0L == toy::test::Approx(1.0L));

    static_assert(1.0L == toy::test::Approx(1.0L), "long double must compare equal to itself");
  }
}

// The class template deduces its parameter from the constructor argument.
TEST_CASE("test::approx/deduction/infers_type_from_argument") {
  static_assert(std::is_same_v<decltype(toy::test::Approx(1.0F)), toy::test::Approx<float>>,
                "a float argument must deduce Approx<float>");
  static_assert(std::is_same_v<decltype(toy::test::Approx(1.0)), toy::test::Approx<double>>,
                "a double argument must deduce Approx<double>");
  static_assert(std::is_same_v<decltype(toy::test::Approx(1.0L)), toy::test::Approx<long double>>,
                "a long double argument must deduce Approx<long double>");

  REQUIRE(sizeof(toy::test::Approx<float>) == 2 * sizeof(float));
}

// Mixing precisions compares in the common type instead of narrowing either side.
TEST_CASE("test::approx/comparison/mixes_precisions") {
  REQUIRE(1.0 == toy::test::Approx(1.0F));
  REQUIRE(1.0F == toy::test::Approx(1.0));
  REQUIRE(1.0L == toy::test::Approx(1.0));

  static_assert(1.0 == toy::test::Approx(1.0F), "a double must compare equal to an Approx<float> of one");
}

// The default tolerance is single-precision epsilon scaled by a hundred, the same value doctest applies, and it
// does not change with the value's type.
TEST_CASE("test::approx/tolerance/defaults_to_doctest_value") {
  constexpr double expected = static_cast<double>(std::numeric_limits<float>::epsilon()) * 100.0;

  REQUIRE(static_cast<double>(toy::test::Approx(1.0).tolerance()) == expected);

  static_assert(static_cast<double>(toy::test::Approx(1.0).tolerance()) == expected,
                "double must carry doctest's default tolerance");
  static_assert(static_cast<double>(toy::test::Approx(1.0L).tolerance()) == expected,
                "long double must carry the same tolerance as double, not a narrower one");
}

// The tolerance keeps the type of the value it belongs to, whatever type the caller passes.
TEST_CASE("test::approx/epsilon/converts_to_the_value_type") {
  constexpr auto tolerant = toy::test::Approx(1.5F).epsilon(0.5);

  static_assert(std::is_same_v<decltype(tolerant.tolerance()), float>, "the tolerance must keep the value's type");
  static_assert(1.0F == tolerant, "epsilon 0.5 must accept a difference of 0.5");

  REQUIRE(1.0F == tolerant);
}
