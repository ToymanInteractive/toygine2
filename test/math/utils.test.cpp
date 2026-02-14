//
// Copyright (c) 2025-2026 Toyman Interactive
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
/*
#include <doctest/doctest.h>

#include "math.hpp"

namespace toy::math {

// Absolute value of integers and floating-point numbers: non-negative unchanged, negative yields positive.
TEST_CASE("math/utils/abs") {
  // Zero and positive integers unchanged; negative integers yield positive.
  SUBCASE("signed_integral") {
    REQUIRE(abs(0) == 0);
    REQUIRE(abs(1) == 1);
    REQUIRE(abs(-1) == 1);
    REQUIRE(abs(42) == 42);
    REQUIRE(abs(-42) == 42);

    static_assert(abs(0) == 0, "abs of zero must remain zero");
    static_assert(abs(1) == 1, "abs of positive integer must be unchanged");
    static_assert(abs(-1) == 1, "abs of negative integer must yield positive");
    static_assert(abs(42) == 42, "abs of positive integer must be unchanged");
    static_assert(abs(-42) == 42, "abs of negative integer must yield positive");
  }

  // All supported signed integral widths: abs of negative equals positive.
  SUBCASE("signed_integral_all_supported_widths") {
    REQUIRE(abs(int8_t(numeric_limits<int8_t>::min() + 1)) == numeric_limits<int8_t>::max());
    REQUIRE(abs(int16_t(numeric_limits<int16_t>::min() + 1)) == numeric_limits<int16_t>::max());
    REQUIRE(abs(int32_t(numeric_limits<int32_t>::min() + 1)) == numeric_limits<int32_t>::max());
    REQUIRE(abs(int64_t(numeric_limits<int64_t>::min() + 1)) == numeric_limits<int64_t>::max());

    static_assert(abs(int8_t(numeric_limits<int8_t>::min() + 1)) == numeric_limits<int8_t>::max(),
                  "abs of int8_t(min+1) must equal int8_t max");
    static_assert(abs(int16_t(numeric_limits<int16_t>::min() + 1)) == numeric_limits<int16_t>::max(),
                  "abs of int16_t(min+1) must equal int16_t max");
    static_assert(abs(int32_t(numeric_limits<int32_t>::min() + 1)) == numeric_limits<int32_t>::max(),
                  "abs of int32_t(min+1) must equal int32_t max");
    static_assert(abs(int64_t(numeric_limits<int64_t>::min() + 1)) == numeric_limits<int64_t>::max(),
                  "abs of int64_t(min+1) must equal int64_t max");
  }

  // Float and double: non-negative unchanged, negative yields positive (within epsilon).
  SUBCASE("floating_point") {
    REQUIRE(isEqual(abs(0.0f), 0.0f));
    REQUIRE(isEqual(abs(1.5f), 1.5f));
    REQUIRE(isEqual(abs(-1.5f), 1.5f));
    REQUIRE(isEqual(abs(0.0), 0.0));
    REQUIRE(isEqual(abs(1.5), 1.5));
    REQUIRE(isEqual(abs(-1.5), 1.5));
    REQUIRE(isEqual(abs(0.0L), 0.0L));
    REQUIRE(isEqual(abs(1.5L), 1.5L));
    REQUIRE(isEqual(abs(-1.5L), 1.5L));

    static_assert(isEqual(abs(0.0f), 0.0f), "abs of zero float must be zero within machine epsilon");
    static_assert(isEqual(abs(1.5f), 1.5f), "abs of positive float must be unchanged within epsilon");
    static_assert(isEqual(abs(-1.5f), 1.5f), "abs of negative float must yield positive within epsilon");
    static_assert(isEqual(abs(0.0), 0.0), "abs of zero double must be zero within machine epsilon");
    static_assert(isEqual(abs(1.5), 1.5), "abs of positive double must be unchanged within epsilon");
    static_assert(isEqual(abs(-1.5), 1.5), "abs of negative double must yield positive within epsilon");
    static_assert(isEqual(abs(0.0L), 0.0L), "abs of zero long double must be zero within machine epsilon");
    static_assert(isEqual(abs(1.5L), 1.5L), "abs of positive long double must be unchanged within epsilon");
    static_assert(isEqual(abs(-1.5L), 1.5L), "abs of negative long double must yield positive within epsilon");
  }
}

// Approximate equality of two floats: absolute and relative epsilon.
TEST_CASE("math/utils/is_equal") {
  // Identical values are equal.
  SUBCASE("identical_values") {
    REQUIRE(isEqual(0.0f, 0.0f));
    REQUIRE(isEqual(1.0f, 1.0f));

    static_assert(isEqual(0.0f, 0.0f), "identical values must be equal");
    static_assert(isEqual(1.0f, 1.0f), "identical values must be equal");
  }

  // Within default absolute epsilon: treated as equal; beyond it (with relEpsilon zero) treated as not equal.
  SUBCASE("within_absolute_epsilon") {
    constexpr float eps = 8.0f * numeric_limits<float>::epsilon();

    REQUIRE(isEqual(0.0f, eps));
    REQUIRE(isEqual(0.0f, -eps));
    REQUIRE(isEqual(1.0f, 1.0f + eps * 0.5f));
    REQUIRE(!isEqual(0.0f, eps * 1.5f, eps, 0.0f));
    REQUIRE(!isEqual(0.0f, -eps * 1.5f, eps, 0.0f));

    static_assert(isEqual(0.0f, eps), "values within absolute epsilon must be equal");
    static_assert(isEqual(0.0f, -eps), "values within absolute epsilon must be equal");
    static_assert(isEqual(1.0f, 1.0f + eps * 0.5f), "values within absolute epsilon must be equal");
    static_assert(!isEqual(0.0f, eps * 1.5f, eps, 0.0f), "values beyond absolute epsilon must not be equal");
    static_assert(!isEqual(0.0f, -eps * 1.5f, eps, 0.0f), "values beyond absolute epsilon must not be equal");
  }

  // Within default relative epsilon for large values: treated as equal.
  SUBCASE("within_relative_epsilon") {
    constexpr float big = 1e6f;
    constexpr float relEps = 64.0f * numeric_limits<float>::epsilon();

    REQUIRE(isEqual(big, big * (1.0f + relEps * 0.5f)));

    static_assert(isEqual(big, big * (1.0f + relEps * 0.5f)), "large values within relative epsilon must be equal");
  }

  // Clearly different values: not equal.
  SUBCASE("clearly_different") {
    REQUIRE(!isEqual(0.0f, 1.0f));
    REQUIRE(!isEqual(1.0f, 2.0f));
    REQUIRE(!isEqual(-1.0f, 1.0f));

    static_assert(!isEqual(0.0f, 1.0f), "clearly different values must not be equal");
    static_assert(!isEqual(1.0f, 2.0f), "clearly different values must not be equal");
    static_assert(!isEqual(-1.0f, 1.0f), "clearly different values must not be equal");
  }

  // Custom epsilons: absolute only.
  SUBCASE("custom_epsilons") {
    constexpr float absEps = 0.01f;

    REQUIRE(isEqual(0.0f, 0.005f, absEps, 0.0f));
    REQUIRE(!isEqual(0.0f, 0.02f, absEps, 0.0f));

    static_assert(isEqual(0.0f, 0.005f, absEps, 0.0f), "values within custom absolute epsilon must be equal");
    static_assert(!isEqual(0.0f, 0.02f, absEps, 0.0f), "values beyond custom absolute epsilon must not be equal");
  }
}

} // namespace toy::math
*/
