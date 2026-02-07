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

#include <limits>

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
    REQUIRE(abs(0.0f) == doctest::Approx(0.0f));
    REQUIRE(abs(1.5f) == doctest::Approx(1.5f));
    REQUIRE(abs(-1.5f) == doctest::Approx(1.5f));
    REQUIRE(abs(0.0) == doctest::Approx(0.0));
    REQUIRE(abs(1.5) == doctest::Approx(1.5));
    REQUIRE(abs(-1.5) == doctest::Approx(1.5));

    static_assert(abs(0.0f) <= numeric_limits<float>::epsilon() && abs(0.0f) >= -numeric_limits<float>::epsilon(),
                  "abs of zero float must be zero within machine epsilon");
    static_assert(abs(1.5f) - 1.5f <= numeric_limits<float>::epsilon()
                    && abs(1.5f) - 1.5f >= -numeric_limits<float>::epsilon(),
                  "abs of positive float must be unchanged within epsilon");
    static_assert(abs(-1.5f) - 1.5f <= numeric_limits<float>::epsilon()
                    && abs(-1.5f) - 1.5f >= -numeric_limits<float>::epsilon(),
                  "abs of negative float must yield positive within epsilon");
    static_assert(abs(0.0) <= numeric_limits<double>::epsilon() && abs(0.0) >= -numeric_limits<double>::epsilon(),
                  "abs of zero double must be zero within machine epsilon");
    static_assert(abs(1.5) - 1.5 <= numeric_limits<double>::epsilon()
                    && abs(1.5) - 1.5 >= -numeric_limits<double>::epsilon(),
                  "abs of positive double must be unchanged within epsilon");
    static_assert(abs(-1.5) - 1.5 <= numeric_limits<double>::epsilon()
                    && abs(-1.5) - 1.5 >= -numeric_limits<double>::epsilon(),
                  "abs of negative double must yield positive within epsilon");
  }
}

} // namespace toy::math
