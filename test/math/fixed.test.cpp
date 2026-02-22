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

// Construction from default, integer, and floating-point.
TEST_CASE("math/fixed/constructors") {
  // Default-constructed value must have zero raw storage.
  SUBCASE("default_constructor") {
    constexpr Fixed f;

    REQUIRE(f.rawValue() == 0);

    static_assert(f.rawValue() == 0, "default-constructed raw value must be 0");
  }

  // Integer constructor scales by 2^FractionBits.
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

  // Float constructor scales and rounds (or truncates when rounding disabled).
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

// Explicit conversion to integral and floating-point types.
TEST_CASE("math/fixed/conversion_operators") {
  // Cast to int truncates toward zero.
  SUBCASE("operator_int") {
    constexpr Fixed f1(1);
    constexpr Fixed f5(5);

    REQUIRE(static_cast<int>(f1) == 1);
    REQUIRE(static_cast<int>(f5) == 5);

    static_assert(static_cast<int>(f1) == 1, "cast fixed(1) to int must be 1");
    static_assert(static_cast<int>(f5) == 5, "cast fixed(5) to int must be 5");
  }

  // Cast to float divides raw value by 2^FractionBits.
  SUBCASE("operator_float") {
    constexpr Fixed f1(1);

    REQUIRE(isEqual(static_cast<float>(f1), 1.0f));

    static_assert(static_cast<float>(f1) >= 0.99f && static_cast<float>(f1) <= 1.01f,
                  "cast fixed(1) to float must be approximately 1.0");
  }
}

// rawValue access and fromRawValue roundtrip.
TEST_CASE("math/fixed/raw_value") {
  // rawValue must equal the scaled integer representation.
  SUBCASE("raw_value") {
    constexpr Fixed f(3);

    REQUIRE(f.rawValue() == 3 * 256);

    static_assert(f.rawValue() == 768, "fixed(3) raw value must be 768");
  }

  // fromRawValue(x).rawValue() must equal x.
  SUBCASE("from_raw_value_roundtrip") {
    constexpr auto raw = 512;
    constexpr auto f = Fixed::fromRawValue(raw);

    REQUIRE(f.rawValue() == raw);

    static_assert(f.rawValue() == raw, "fromRawValue roundtrip must preserve raw value");
  }
}

// Unary operator- (negation).
TEST_CASE("math/fixed/operator_minus") {
  // Negation of zero yields zero.
  SUBCASE("negate_zero") {
    constexpr Fixed f(0);
    constexpr auto neg = -f;

    REQUIRE(neg.rawValue() == 0);

    static_assert(neg.rawValue() == 0, "operator- of zero must be zero");
  }

  // Negation of a positive value yields negative.
  SUBCASE("negate_positive") {
    constexpr Fixed f(5);
    constexpr auto neg = -f;

    REQUIRE(neg.rawValue() == -5 * 256);
    REQUIRE(static_cast<int>(neg) == -5);

    static_assert(neg.rawValue() == -5 * 256, "operator- of positive must yield negative raw");
    static_assert(static_cast<int>(neg) == -5, "operator- of positive must yield negative int");
  }

  // Negation of a negative value yields positive.
  SUBCASE("negate_negative") {
    constexpr Fixed f(-3);
    constexpr auto neg = -f;

    REQUIRE(neg.rawValue() == 3 * 256);
    REQUIRE(static_cast<int>(neg) == 3);

    static_assert(neg.rawValue() == 3 * 256, "operator- of negative must yield positive raw");
    static_assert(static_cast<int>(neg) == 3, "operator- of negative must yield positive int");
  }

  // Original operand remains unchanged after unary minus.
  SUBCASE("operand_unchanged") {
    constexpr Fixed f(7);
    constexpr auto neg = -f;

    REQUIRE(f.rawValue() == 7 * 256);
    REQUIRE(neg.rawValue() == -7 * 256);

    static_assert(f.rawValue() == 7 * 256, "operand must be unchanged after operator-");
    static_assert(neg.rawValue() == -7 * 256, "operator- result must negate raw value");
  }
}

// operator+= (in-place addition with fixed or integral).
TEST_CASE("math/fixed/operator_plus_assign") {
  // Add another fixed of the same type in place.
  SUBCASE("plus_assign_same_type") {
    Fixed a(2);
    a += Fixed(3);

    REQUIRE(a.rawValue() == 5 * 256);
    REQUIRE(static_cast<int>(a) == 5);
  }

  // Add fixed with different rounding policy (raw storage is compatible).
  SUBCASE("plus_assign_different_rounding") {
    Fixed a(1);
    a += FixedNoRounding(2);

    REQUIRE(a.rawValue() == 3 * 256);

    FixedNoRounding b(10);
    b += Fixed(5);

    REQUIRE(b.rawValue() == 15 * 256);
  }

  // Add integral value (whole units) in place.
  SUBCASE("plus_assign_integral") {
    Fixed a(2);
    a += 3;

    REQUIRE(a.rawValue() == 5 * 256);
    REQUIRE(static_cast<int>(a) == 5);
  }

  // operator+= is usable in constant expressions.
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

// operator-= (in-place subtraction with fixed or integral).
TEST_CASE("math/fixed/operator_minus_assign") {
  // Subtract another fixed of the same type in place.
  SUBCASE("minus_assign_same_type") {
    Fixed a(5);
    a -= Fixed(2);

    REQUIRE(a.rawValue() == 3 * 256);
    REQUIRE(static_cast<int>(a) == 3);
  }

  // Subtract fixed with different rounding policy in place.
  SUBCASE("minus_assign_different_rounding") {
    Fixed a(10);
    a -= FixedNoRounding(3);

    REQUIRE(a.rawValue() == 7 * 256);

    FixedNoRounding b(8);
    b -= Fixed(2);

    REQUIRE(b.rawValue() == 6 * 256);
  }

  // Subtract integral value (whole units) in place.
  SUBCASE("minus_assign_integral") {
    Fixed a(5);
    a -= 2;

    REQUIRE(a.rawValue() == 3 * 256);
    REQUIRE(static_cast<int>(a) == 3);
  }

  // operator-= is usable in constant expressions.
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

// operator*= (in-place multiplication with fixed or integral).
TEST_CASE("math/fixed/operator_mul_assign") {
  // Multiply by another fixed of the same type in place.
  SUBCASE("mul_assign_same_type") {
    Fixed a(2);
    a *= Fixed(3);

    REQUIRE(a.rawValue() == 6 * 256);
    REQUIRE(static_cast<int>(a) == 6);

    constexpr Fixed a_ct = []() {
      Fixed x(2);
      x *= Fixed(3);
      return x;
    }();
    static_assert(a_ct.rawValue() == 6 * 256, "operator*=(fixed) result must be 6 for 2*3");
  }

  // Multiply by fixed with different rounding policy in place.
  SUBCASE("mul_assign_different_rounding") {
    Fixed a(2);
    a *= FixedNoRounding(4);

    REQUIRE(static_cast<int>(a) == 8);

    FixedNoRounding b(3);
    b *= Fixed(2);

    REQUIRE(static_cast<int>(b) == 6);
  }

  // Multiply by integral (whole units) in place.
  SUBCASE("mul_assign_integral") {
    Fixed a(5);
    a *= 2;

    REQUIRE(a.rawValue() == 10 * 256);
    REQUIRE(static_cast<int>(a) == 10);
  }

  // operator*= is usable in constant expressions.
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

// operator/= (in-place division by fixed or integral).
TEST_CASE("math/fixed/operator_div_assign") {
  // Divide by another fixed of the same type in place.
  SUBCASE("div_assign_same_type") {
    Fixed a(8);
    a /= Fixed(2);

    REQUIRE(a.rawValue() == 4 * 256);
    REQUIRE(static_cast<int>(a) == 4);

    constexpr Fixed a_ct = []() {
      Fixed x(8);
      x /= Fixed(2);
      return x;
    }();
    static_assert(a_ct.rawValue() == 4 * 256, "operator/=(fixed) result must be 4 for 8/2");
  }

  // Divide by fixed with different rounding policy in place.
  SUBCASE("div_assign_different_rounding") {
    Fixed a(6);
    a /= FixedNoRounding(2);

    REQUIRE(static_cast<int>(a) == 3);

    FixedNoRounding b(9);
    b /= Fixed(3);

    REQUIRE(static_cast<int>(b) == 3);
  }

  // Divide by integral (whole units) in place.
  SUBCASE("div_assign_integral") {
    Fixed a(10);
    a /= 2;

    REQUIRE(a.rawValue() == 5 * 256);
    REQUIRE(static_cast<int>(a) == 5);
  }

  // operator/= is usable in constant expressions.
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
