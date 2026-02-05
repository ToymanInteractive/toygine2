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

    static_assert(abs(0) == 0, "abs(0) must be 0");
    static_assert(abs(1) == 1, "abs(positive) must be unchanged");
    static_assert(abs(-1) == 1, "abs(negative) must yield positive");
    static_assert(abs(42) == 42, "abs(positive) must be unchanged");
    static_assert(abs(-42) == 42, "abs(negative) must yield positive");
  }

  // All supported signed integral widths: abs of negative equals positive.
  SUBCASE("signed_integral_all_supported_widths") {
    REQUIRE(abs(int8_t(-1)) == 1);
    REQUIRE(abs(int16_t(-10)) == 10);
    REQUIRE(abs(int32_t(-100)) == 100);
    REQUIRE(abs(int64_t(-10000)) == 10000);

    static_assert(abs(int8_t(-1)) == 1, "abs(int8_t(-1)) must be 1");
    static_assert(abs(int16_t(-10)) == 10, "abs(int16_t(-10)) must be 10");
    static_assert(abs(int32_t(-100)) == 100, "abs(int32_t(-100)) must be 100");
    static_assert(abs(int64_t(-10000)) == 10000, "abs(int64_t(-10000)) must be 10000");
  }

  // Float and double: non-negative unchanged, negative yields positive (within epsilon).
  SUBCASE("floating_point") {
    REQUIRE(abs(0.0f) == doctest::Approx(0.0f));
    REQUIRE(abs(1.5f) == doctest::Approx(1.5f));
    REQUIRE(abs(-1.5f) == doctest::Approx(1.5f));
    REQUIRE(abs(0.0) == doctest::Approx(0.0));
    REQUIRE(abs(1.5) == doctest::Approx(1.5));
    REQUIRE(abs(-1.5) == doctest::Approx(1.5));

    static_assert(abs(0.0f) <= std::numeric_limits<float>::epsilon(), "abs(0.0f) must be zero within epsilon");
    static_assert(abs(1.5f) - 1.5f <= std::numeric_limits<float>::epsilon(), "abs(positive float) must be unchanged");
    static_assert(abs(-1.5f) - 1.5f <= std::numeric_limits<float>::epsilon(), "abs(negative float) must yield positive");
    static_assert(abs(0.0) <= std::numeric_limits<double>::epsilon(), "abs(0.0) must be zero within epsilon");
    static_assert(abs(1.5) - 1.5 <= std::numeric_limits<double>::epsilon(), "abs(positive double) must be unchanged");
    static_assert(abs(-1.5) - 1.5 <= std::numeric_limits<double>::epsilon(), "abs(negative double) must yield positive");
  }
}

} // namespace toy::math
