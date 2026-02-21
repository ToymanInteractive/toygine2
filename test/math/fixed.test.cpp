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

using Fixed = fixed<int32_t, int64_t, 8>;
using FixedNoRounding = fixed<int32_t, int64_t, 8, false>;

// Default constructor and constructor from integer.
TEST_CASE("math/fixed/constructors") {
  // Default constructor yields zero raw value.
  SUBCASE("default_constructor") {
    constexpr Fixed f;

    REQUIRE(f.rawValue() == 0);

    static_assert(f.rawValue() == 0, "default-constructed raw value must be 0");
  }

  // Constructor from integer: value scaled by 2^FractionBits.
  SUBCASE("from_integer") {
    constexpr Fixed f0(0);
    constexpr Fixed f1(1);
    constexpr Fixed f2(-1);

    REQUIRE(f0.rawValue() == 0);
    REQUIRE(f1.rawValue() == 256);
    REQUIRE(f2.rawValue() == -256);

    static_assert(f0.rawValue() == 0, "fixed(0) raw value must be 0");
    static_assert(f1.rawValue() == 256, "fixed(1) raw value must be 256 for 8 fraction bits");
    static_assert(f2.rawValue() == -256, "fixed(-1) raw value must be -256 for 8 fraction bits");
  }

  // Constructor from floating-point: value scaled and rounded.
  SUBCASE("from_float") {
    constexpr Fixed f_half(0.5f);
    constexpr Fixed f_neg_half(-0.5f);
    constexpr Fixed f_round_up(0.502f); // 0.502 * 256 = 128.512, rounds to 129
    constexpr Fixed f_round_down(0.498f); // 0.498 * 256 = 127.488, rounds to 127

    constexpr FixedNoRounding f_trunc(0.502f); // 0.502 * 256 = 128.512, truncates to 128

    REQUIRE(f_half.rawValue() == 128);
    REQUIRE(f_neg_half.rawValue() == -128);
    REQUIRE(f_round_up.rawValue() == 129);
    REQUIRE(f_round_down.rawValue() == 127);

    REQUIRE(f_trunc.rawValue() == 128);

    static_assert(f_half.rawValue() == 128, "fixed(0.5) raw value must be 128");
    static_assert(f_neg_half.rawValue() == -128, "fixed(-0.5) raw value must be -128");
    static_assert(f_round_up.rawValue() == 129, "rounding must be applied correctly");
    static_assert(f_round_down.rawValue() == 127, "rounding must be applied correctly");

    static_assert(f_trunc.rawValue() == 128, "truncation must be applied when rounding is disabled");
  }
}

// Conversion to integral and floating-point.
TEST_CASE("math/fixed/conversion_operators") {
  // Cast to int: truncates toward zero.
  SUBCASE("operator_int") {
    constexpr Fixed f1(1);
    constexpr Fixed f5(5);

    REQUIRE(static_cast<int>(f1) == 1);
    REQUIRE(static_cast<int>(f5) == 5);

    static_assert(static_cast<int>(f1) == 1, "cast fixed(1) to int must be 1");
    static_assert(static_cast<int>(f5) == 5, "cast fixed(5) to int must be 5");
  }

  // Cast to float: raw value divided by 2^FractionBits.
  SUBCASE("operator_float") {
    constexpr Fixed f1(1);

    REQUIRE(isEqual(static_cast<float>(f1), 1.0f));

    static_assert(static_cast<float>(f1) >= 0.99f && static_cast<float>(f1) <= 1.01f,
                  "cast fixed(1) to float must be approximately 1.0");
  }
}

// rawValue and fromRawValue roundtrip.
TEST_CASE("math/fixed/raw_value") {
  // rawValue returns internal representation.
  SUBCASE("raw_value") {
    constexpr Fixed f(3);

    REQUIRE(f.rawValue() == 3 * 256);

    static_assert(f.rawValue() == 768, "fixed(3) raw value must be 768");
  }

  // fromRawValue roundtrip: fromRawValue(x).rawValue() == x.
  SUBCASE("from_raw_value_roundtrip") {
    constexpr auto raw = 512;
    constexpr auto f = Fixed::fromRawValue(raw);

    REQUIRE(f.rawValue() == raw);

    static_assert(f.rawValue() == 512, "fromRawValue roundtrip must preserve raw value");
  }
}

// Unary operator-: negates the fixed-point value.
TEST_CASE("math/fixed/operator_minus") {
  SUBCASE("negate_zero") {
    constexpr Fixed f(0);
    constexpr auto neg = -f;

    REQUIRE(neg.rawValue() == 0);

    static_assert(neg.rawValue() == 0, "operator- of zero must be zero");
  }

  SUBCASE("negate_positive") {
    constexpr Fixed f(5);
    constexpr auto neg = -f;

    REQUIRE(neg.rawValue() == -5 * 256);
    REQUIRE(static_cast<int>(neg) == -5);

    static_assert(neg.rawValue() == -5 * 256, "operator- of positive must yield negative raw");
    static_assert(static_cast<int>(neg) == -5, "operator- of positive must yield negative int");
  }

  SUBCASE("negate_negative") {
    constexpr Fixed f(-3);
    constexpr auto neg = -f;

    REQUIRE(neg.rawValue() == 3 * 256);
    REQUIRE(static_cast<int>(neg) == 3);

    static_assert(neg.rawValue() == 3 * 256, "operator- of negative must yield positive raw");
    static_assert(static_cast<int>(neg) == 3, "operator- of negative must yield positive int");
  }

  SUBCASE("operand_unchanged") {
    Fixed f(7);
    auto neg = -f;

    REQUIRE(f.rawValue() == 7 * 256);
    REQUIRE(neg.rawValue() == -7 * 256);
  }
}

// operator+= (fixed and integral).
TEST_CASE("math/fixed/operator_plus_assign") {
  SUBCASE("plus_assign_same_type") {
    Fixed a(2);
    a += Fixed(3);

    REQUIRE(a.rawValue() == 5 * 256);
    REQUIRE(static_cast<int>(a) == 5);
  }

  SUBCASE("plus_assign_different_rounding") {
    Fixed a(1);
    a += FixedNoRounding(2);

    REQUIRE(a.rawValue() == 3 * 256);

    FixedNoRounding b(10);
    b += Fixed(5);

    REQUIRE(b.rawValue() == 15 * 256);
  }

  SUBCASE("plus_assign_integral") {
    Fixed a(2);
    a += 3;

    REQUIRE(a.rawValue() == 5 * 256);
    REQUIRE(static_cast<int>(a) == 5);
  }

  SUBCASE("plus_assign_constexpr") {
    constexpr Fixed a = []() {
      Fixed x(1);
      x += Fixed(2);
      x += 3;
      return x;
    }();

    REQUIRE(a.rawValue() == 6 * 256);

    static_assert(a.rawValue() == 6 * 256, "operator+= must be constexpr");
  }
}

// operator-= (fixed and integral).
TEST_CASE("math/fixed/operator_minus_assign") {
  SUBCASE("minus_assign_same_type") {
    Fixed a(5);
    a -= Fixed(2);

    REQUIRE(a.rawValue() == 3 * 256);
    REQUIRE(static_cast<int>(a) == 3);
  }

  SUBCASE("minus_assign_different_rounding") {
    Fixed a(10);
    a -= FixedNoRounding(3);

    REQUIRE(a.rawValue() == 7 * 256);

    FixedNoRounding b(8);
    b -= Fixed(2);

    REQUIRE(b.rawValue() == 6 * 256);
  }

  SUBCASE("minus_assign_integral") {
    Fixed a(5);
    a -= 2;

    REQUIRE(a.rawValue() == 3 * 256);
    REQUIRE(static_cast<int>(a) == 3);
  }

  SUBCASE("minus_assign_constexpr") {
    constexpr Fixed a = []() {
      Fixed x(10);
      x -= Fixed(3);
      x -= 2;
      return x;
    }();

    REQUIRE(a.rawValue() == 5 * 256);
    static_assert(a.rawValue() == 5 * 256, "operator-= must be constexpr");
  }
}

// operator*= (fixed and integral).
TEST_CASE("math/fixed/operator_mul_assign") {
  SUBCASE("mul_assign_same_type") {
    Fixed a(2);
    a *= Fixed(3);

    REQUIRE(a.rawValue() == 6 * 256);
    REQUIRE(static_cast<int>(a) == 6);
  }

  SUBCASE("mul_assign_different_rounding") {
    Fixed a(2);
    a *= FixedNoRounding(4);

    REQUIRE(static_cast<int>(a) == 8);

    FixedNoRounding b(3);
    b *= Fixed(2);

    REQUIRE(static_cast<int>(b) == 6);
  }

  SUBCASE("mul_assign_integral") {
    Fixed a(5);
    a *= 2;

    REQUIRE(a.rawValue() == 10 * 256);
    REQUIRE(static_cast<int>(a) == 10);
  }

  SUBCASE("mul_assign_constexpr") {
    constexpr Fixed a = []() {
      Fixed x(3);
      x *= 2;
      return x;
    }();

    REQUIRE(a.rawValue() == 6 * 256);
    static_assert(a.rawValue() == 6 * 256, "operator*=(T) must be constexpr");
  }
}

// operator/= (fixed and integral).
TEST_CASE("math/fixed/operator_div_assign") {
  SUBCASE("div_assign_same_type") {
    Fixed a(8);
    a /= Fixed(2);

    REQUIRE(a.rawValue() == 4 * 256);
    REQUIRE(static_cast<int>(a) == 4);
  }

  SUBCASE("div_assign_different_rounding") {
    Fixed a(6);
    a /= FixedNoRounding(2);

    REQUIRE(static_cast<int>(a) == 3);

    FixedNoRounding b(9);
    b /= Fixed(3);

    REQUIRE(static_cast<int>(b) == 3);
  }

  SUBCASE("div_assign_integral") {
    Fixed a(10);
    a /= 2;

    REQUIRE(a.rawValue() == 5 * 256);
    REQUIRE(static_cast<int>(a) == 5);
  }

  SUBCASE("div_assign_constexpr") {
    constexpr Fixed a = []() {
      Fixed x(12);
      x /= 3;
      return x;
    }();

    REQUIRE(a.rawValue() == 4 * 256);
    static_assert(a.rawValue() == 4 * 256, "operator/=(T) must be constexpr");
  }
}

} // namespace toy::math
