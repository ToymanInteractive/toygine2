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
  \file   approx.test.cpp
  \brief  Unit tests for \ref toy::test::Approx, its deduction guides and its tolerant comparison.
*/

#include <limits>
#include <type_traits>

#include <doctest/doctest.h>

#include "approx.hpp"

// Approx compares equal within its tolerance and unequal outside it.
TEST_CASE("test/approx/comparison_honours_tolerance") {
  REQUIRE(1.0 == toy::test::Approx(1.0));
  REQUIRE(toy::test::Approx(1.0) == 1.0);
  REQUIRE_FALSE(1.0 == toy::test::Approx(2.0));

  static_assert(1.0 == toy::test::Approx(1.0), "a value must compare equal to itself");
}

// A widened epsilon accepts a difference the default tolerance rejects.
TEST_CASE("test/approx/comparison_epsilon_widens_tolerance") {
  REQUIRE_FALSE(1.0 == toy::test::Approx(1.5));
  REQUIRE(1.0 == toy::test::Approx(1.5).epsilon(0.5));

  static_assert(1.0 == toy::test::Approx(1.5).epsilon(0.5), "epsilon 0.5 must accept a difference of 0.5");
}

// Tolerance scales with magnitude, so large values keep the same relative precision.
TEST_CASE("test/approx/comparison_scales_with_magnitude") {
  REQUIRE(1000000.0 == toy::test::Approx(1000000.001));
  REQUIRE_FALSE(1.0 == toy::test::Approx(1.001));
}

// Every standard floating-point type satisfies the constraint and compares within its own precision.
TEST_CASE("test/approx/comparison_accepts_every_floating_point_type") {
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
TEST_CASE("test/approx/deduction_infers_type_from_argument") {
  static_assert(std::is_same_v<decltype(toy::test::Approx(1.0F)), toy::test::Approx<float>>,
                "a float argument must deduce Approx<float>");
  static_assert(std::is_same_v<decltype(toy::test::Approx(1.0)), toy::test::Approx<double>>,
                "a double argument must deduce Approx<double>");
  static_assert(std::is_same_v<decltype(toy::test::Approx(1.0L)), toy::test::Approx<long double>>,
                "a long double argument must deduce Approx<long double>");
}

// An integral argument deduces Approx<double>, which is the only type doctest's Approx ever holds.
TEST_CASE("test/approx/deduction_widens_an_integral_argument") {
  static_assert(std::is_same_v<decltype(toy::test::Approx(1)), toy::test::Approx<double>>,
                "an int argument must deduce Approx<double>");
  static_assert(std::is_same_v<decltype(toy::test::Approx(1U)), toy::test::Approx<double>>,
                "an unsigned argument must deduce Approx<double>");
}

// An integral operand compares like doctest, which accepts anything a double is constructible from.
TEST_CASE("test/approx/comparison_accepts_an_integral_operand") {
  REQUIRE(1 == toy::test::Approx(1.0));
  REQUIRE(toy::test::Approx(1.0) == 1);
  REQUIRE_FALSE(2 == toy::test::Approx(1.0));

  REQUIRE(1 == toy::test::Approx(1));
  REQUIRE(1.0 == toy::test::Approx(1).epsilon(1));

  static_assert(1 == toy::test::Approx(1.0), "an int must compare equal to the same value");
  static_assert(1 == toy::test::Approx(1), "an int argument and an int operand must compare equal");
}

// Mixing precisions compares in the common type instead of narrowing either side.
TEST_CASE("test/approx/comparison_mixes_precisions") {
  REQUIRE(1.0 == toy::test::Approx(1.0F));
  REQUIRE(1.0F == toy::test::Approx(1.0));
  REQUIRE(1.0L == toy::test::Approx(1.0));

  static_assert(1.0 == toy::test::Approx(1.0F), "a double must compare equal to an Approx<float> of one");
}

// The default tolerance is single-precision epsilon scaled by a hundred, the same value doctest applies, and it
// does not change with the value's type.
TEST_CASE("test/approx/tolerance_defaults_to_doctest_value") {
  constexpr double expected = static_cast<double>(std::numeric_limits<float>::epsilon()) * 100.0;

  REQUIRE(static_cast<double>(toy::test::Approx(1.0).tolerance()) == expected);

  static_assert(static_cast<double>(toy::test::Approx(1.0).tolerance()) == expected,
                "double must carry doctest's default tolerance");
  static_assert(static_cast<double>(toy::test::Approx(1.0L).tolerance()) == expected,
                "long double must carry the same tolerance as double, not a narrower one");
}

// The tolerance keeps the type of the value it belongs to, whatever type the caller passes.
TEST_CASE("test/approx/epsilon_converts_to_the_value_type") {
  constexpr auto tolerant = toy::test::Approx(1.5F).epsilon(0.5);

  static_assert(std::is_same_v<decltype(tolerant.tolerance()), float>, "the tolerance must keep the value's type");
  static_assert(1.0F == tolerant, "epsilon 0.5 must accept a difference of 0.5");

  REQUIRE(1.0F == tolerant);
}

// The comparison reproduces doctest's formula, including its strict inequality. A difference of one and a half
// epsilons around one is the cheapest point that separates the two candidate formulas: doctest's threshold there
// is two epsilons, while a threshold of one epsilon would reject it.
TEST_CASE("test/approx/comparison_matches_the_doctest_formula") {
  // doctest 2.5.3: fabs(lhs - value) < epsilon * (scale + max(fabs(lhs), fabs(value))), with scale fixed at one.
  constexpr auto doctestEquals = [](double lhs, double value, double epsilon) {
    // The magnitude is taken locally rather than through the runner's helper: an oracle sharing code with the
    // code under test agrees with it even when both are wrong.
    const auto magnitude = [](double input) constexpr {
      return input < 0.0 ? -input : input;
    };

    const double difference = magnitude(lhs - value);
    const double left       = magnitude(lhs);
    const double right      = magnitude(value);

    return difference < epsilon * (1.0 + (left > right ? left : right));
  };

  constexpr double epsilon = static_cast<double>(std::numeric_limits<float>::epsilon()) * 100.0;

  SUBCASE("one and a half epsilons around one are within tolerance") {
    constexpr double value = 1.0 + 1.5 * epsilon;

    REQUIRE(doctestEquals(1.0, value, epsilon) == true);
    REQUIRE(1.0 == toy::test::Approx(value));

    static_assert(1.0 == toy::test::Approx(1.0 + 1.5 * epsilon),
                  "a difference of one and a half epsilons must stay within doctest's threshold of two");
  }

  SUBCASE("two and a half epsilons around one are outside tolerance") {
    constexpr double value = 1.0 + 2.5 * epsilon;

    REQUIRE(doctestEquals(1.0, value, epsilon) == false);
    REQUIRE_FALSE(1.0 == toy::test::Approx(value));
  }

  SUBCASE("the added one carries values below unit magnitude") {
    constexpr double value = 0.5 + 1.2 * epsilon;

    REQUIRE(doctestEquals(0.5, value, epsilon) == true);
    REQUIRE(0.5 == toy::test::Approx(value));
  }

  SUBCASE("an exact match compares equal under a strict inequality") {
    REQUIRE(doctestEquals(1.0, 1.0, epsilon) == true);
    REQUIRE(1.0 == toy::test::Approx(1.0));
  }

  // A zero tolerance collapses the threshold to zero, and a strict inequality rejects a difference of zero with
  // it. DocTest 2.5.3 behaves the same way, so the surprise is part of the contract rather than a defect: reading
  // it as one invites replacing the strict comparison and breaking the agreement between the two runners.
  SUBCASE("a zero tolerance rejects even an exact match") {
    REQUIRE(doctestEquals(1.0, 1.0, 0.0) == false);
    REQUIRE_FALSE(1.0 == toy::test::Approx(1.0).epsilon(0.0));

    static_assert(!(1.0 == toy::test::Approx(1.0).epsilon(0.0)),
                  "a zero tolerance must reject an exact match, as it does under doctest");
  }
}
