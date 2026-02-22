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
using Fixed4 = fixed<int32_t, int64_t, 4>;
using Fixed16 = fixed<int32_t, int64_t, 16>;

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
    constexpr Fixed f_round_up_neg(-0.502f); // rounds to -129
    constexpr Fixed f_round_down_neg(-0.498f); // rounds to -127

    constexpr FixedNoRounding f_trunc(0.502f); // 0.502 * 256 = 128.512, truncates to 128
    constexpr FixedNoRounding f_trunc_neg(-0.502f); // truncates to -128

    REQUIRE(f_half.rawValue() == 128);
    REQUIRE(f_neg_half.rawValue() == -128);
    REQUIRE(f_round_up.rawValue() == 129);
    REQUIRE(f_round_down.rawValue() == 127);
    REQUIRE(f_round_up_neg.rawValue() == -129);
    REQUIRE(f_round_down_neg.rawValue() == -127);

    REQUIRE(f_trunc.rawValue() == 128);
    REQUIRE(f_trunc_neg.rawValue() == -128);

    static_assert(f_half.rawValue() == 128, "fixed(0.5) raw value must be 128");
    static_assert(f_neg_half.rawValue() == -128, "fixed(-0.5) raw value must be -128");
    static_assert(f_round_up.rawValue() == 129, "rounding must be applied correctly");
    static_assert(f_round_down.rawValue() == 127, "rounding must be applied correctly");
    static_assert(f_round_up_neg.rawValue() == -129, "rounding must be applied correctly for negative");
    static_assert(f_round_down_neg.rawValue() == -127, "rounding must be applied correctly for negative");

    static_assert(f_trunc.rawValue() == 128, "truncation must be applied when rounding is disabled");
    static_assert(f_trunc_neg.rawValue() == -128, "truncation must be applied for negative when rounding is disabled");
  }

  // Construct from another fixed type with same fraction bits (raw value preserved).
  SUBCASE("from_fixed_same_fraction_bits") {
    constexpr Fixed src1(3);
    constexpr Fixed src2(-3);
    constexpr Fixed f1(src1);
    constexpr Fixed f2(src2);

    REQUIRE(f1.rawValue() == 3 * 256);
    REQUIRE(f2.rawValue() == -3 * 256);
    REQUIRE(static_cast<int>(f1) == 3);
    REQUIRE(static_cast<int>(f2) == -3);

    static_assert(f1.rawValue() == 3 * 256, "fixed(fixed same type) must preserve value");
    static_assert(f2.rawValue() == -3 * 256, "fixed(fixed same type) must preserve value");
    static_assert(static_cast<int>(f1) == 3, "fixed(fixed same type) must preserve value");
    static_assert(static_cast<int>(f2) == -3, "fixed(fixed same type) must preserve value");
  }

  // Construct from fixed with fewer fraction bits (scale up).
  SUBCASE("from_fixed_fewer_fraction_bits") {
    constexpr Fixed4 src1(1);
    constexpr Fixed4 src2(-1);
    constexpr Fixed f1(src1);
    constexpr Fixed f2(src2);

    REQUIRE(f1.rawValue() == 1 * 256);
    REQUIRE(f2.rawValue() == -1 * 256);
    REQUIRE(static_cast<int>(f1) == 1);
    REQUIRE(static_cast<int>(f2) == -1);

    static_assert(f1.rawValue() == 1 * 256, "fixed from 4-bit must scale up to 8-bit");
    static_assert(f2.rawValue() == -1 * 256, "fixed from 4-bit must scale up to 8-bit");
    static_assert(static_cast<int>(f1) == 1, "fixed from 4-bit must scale up to 8-bit");
    static_assert(static_cast<int>(f2) == -1, "fixed from 4-bit must scale up to 8-bit");
  }

  // Construct from fixed with more fraction bits (scale down, rounding when enabled).
  SUBCASE("from_fixed_more_fraction_bits") {
    constexpr Fixed16 src1(1);
    constexpr Fixed16 src2(-1);
    constexpr Fixed f1(src1);
    constexpr Fixed f2(src2);

    REQUIRE(f1.rawValue() == 1 * 256);
    REQUIRE(f2.rawValue() == -1 * 256);
    REQUIRE(static_cast<int>(f1) == 1);
    REQUIRE(static_cast<int>(f2) == -1);

    static_assert(f1.rawValue() == 1 * 256, "fixed from 16-bit must scale down to 8-bit");
    static_assert(f2.rawValue() == -1 * 256, "fixed from 16-bit must scale down to 8-bit");
    static_assert(static_cast<int>(f1) == 1, "fixed from 16-bit must scale down to 8-bit");
    static_assert(static_cast<int>(f2) == -1, "fixed from 16-bit must scale down to 8-bit");
  }
}

// Explicit conversion to integral and floating-point types.
TEST_CASE("math/fixed/conversion_operators") {
  // Cast to int truncates toward zero.
  SUBCASE("operator_int") {
    constexpr Fixed f1(5);
    constexpr Fixed f2(-5);

    REQUIRE(static_cast<int>(f1) == 5);
    REQUIRE(static_cast<int>(f2) == -5);

    static_assert(static_cast<int>(f1) == 5, "cast fixed(f) to int must be 5");
    static_assert(static_cast<int>(f2) == -5, "cast fixed(-5) to int must be -5");
  }

  // Cast to float divides raw value by 2^FractionBits.
  SUBCASE("operator_float") {
    constexpr Fixed f1(1);
    constexpr Fixed f2(-1);

    REQUIRE(isEqual(static_cast<float>(f1), 1.0f));
    REQUIRE(isEqual(static_cast<float>(f2), -1.0f));

    static_assert(static_cast<float>(f1) >= 0.99f && static_cast<float>(f1) <= 1.01f,
                  "cast fixed(1) to float must be approximately 1.0");
    static_assert(static_cast<float>(f2) >= -1.01f && static_cast<float>(f2) <= -0.99f,
                  "cast fixed(-1) to float must be approximately -1.0");
  }
}

// rawValue access and fromRawValue roundtrip.
TEST_CASE("math/fixed/raw_value") {
  // rawValue must equal the scaled integer representation.
  SUBCASE("raw_value") {
    constexpr Fixed f(3);
    constexpr Fixed fNeg(-3);

    REQUIRE(f.rawValue() == 3 * 256);
    REQUIRE(fNeg.rawValue() == -3 * 256);

    static_assert(f.rawValue() == 768, "fixed(3) raw value must be 768");
    static_assert(fNeg.rawValue() == -768, "fixed(-3) raw value must be -768");
  }

  // fromRawValue(x).rawValue() must equal x.
  SUBCASE("from_raw_value_roundtrip") {
    constexpr auto raw = 512;
    constexpr auto rawNeg = -512;
    constexpr auto f = Fixed::fromRawValue(raw);
    constexpr auto fNeg = Fixed::fromRawValue(rawNeg);

    REQUIRE(f.rawValue() == raw);
    REQUIRE(fNeg.rawValue() == rawNeg);

    static_assert(f.rawValue() == raw, "fromRawValue roundtrip must preserve raw value");
    static_assert(fNeg.rawValue() == rawNeg, "fromRawValue roundtrip must preserve negative raw value");
  }
}

// fromFixedPoint: convert raw value from another fraction-bit width.
TEST_CASE("math/fixed/from_fixed_point") {
  // Source has fewer fraction bits: scale up (no rounding).
  SUBCASE("from_fixed_point_scale_up") {
    constexpr auto raw4 = 16; // 1.0 in 4-bit
    constexpr auto raw4Neg = -16;
    constexpr auto f = Fixed::fromFixedPoint<4>(raw4);
    constexpr auto fNeg = Fixed::fromFixedPoint<4>(raw4Neg);

    REQUIRE(f.rawValue() == 1 * 256);
    REQUIRE(fNeg.rawValue() == -1 * 256);
    REQUIRE(static_cast<int>(f) == 1);
    REQUIRE(static_cast<int>(fNeg) == -1);

    static_assert(f.rawValue() == 1 * 256, "fromFixedPoint<4> must scale up to 8-bit");
    static_assert(Fixed::fromFixedPoint<4>(-16).rawValue() == -256, "fromFixedPoint<4> must scale up negative");
  }

  // Source has more fraction bits: scale down (rounding when EnableRounding).
  SUBCASE("from_fixed_point_scale_down") {
    constexpr auto raw16 = 65536; // 1.0 in 16-bit
    constexpr auto raw16Neg = -65536;
    constexpr auto f = Fixed::fromFixedPoint<16>(raw16);
    constexpr auto fNeg = Fixed::fromFixedPoint<16>(raw16Neg);

    REQUIRE(f.rawValue() == 1 * 256);
    REQUIRE(fNeg.rawValue() == -1 * 256);
    REQUIRE(static_cast<int>(f) == 1);
    REQUIRE(static_cast<int>(fNeg) == -1);

    static_assert(f.rawValue() == 1 * 256, "fromFixedPoint<16> must scale down to 8-bit");
    static_assert(Fixed::fromFixedPoint<16>(-65536).rawValue() == -256, "fromFixedPoint<16> must scale down negative");
  }

  // Same fraction bits: raw value used as-is.
  SUBCASE("from_fixed_point_same_bits") {
    constexpr auto raw8 = 512;
    constexpr auto raw8Neg = -512;
    constexpr auto f = Fixed::fromFixedPoint<8>(raw8);
    constexpr auto fNeg = Fixed::fromFixedPoint<8>(raw8Neg);

    REQUIRE(f.rawValue() == 512);
    REQUIRE(fNeg.rawValue() == -512);

    static_assert(f.rawValue() == 512, "fromFixedPoint<8> same bits must preserve raw");
    static_assert(Fixed::fromFixedPoint<8>(-512).rawValue() == -512,
                  "fromFixedPoint<8> same bits must preserve negative raw");
  }

  // Scale down with rounding: the discarded bit rounds the result up.
  SUBCASE("from_fixed_point_scale_down_rounding") {
    // raw = 0x180 in 16-bit = 384 → 384 / 256 = 1, rounding bit = (384 / 128) % 2 = 3 % 2 = 1 → result = 2
    constexpr auto f = Fixed::fromFixedPoint<16>(384);
    constexpr auto fNeg = Fixed::fromFixedPoint<16>(-384);

    REQUIRE(f.rawValue() == 2);
    REQUIRE(fNeg.rawValue() == -2);

    static_assert(f.rawValue() == 2, "fromFixedPoint<16> must round up when rounding bit is set");
    static_assert(fNeg.rawValue() == -2, "fromFixedPoint<16> must round negative");
  }

  // Scale down without rounding: verify truncation when rounding is disabled.
  SUBCASE("from_fixed_point_scale_down_no_rounding") {
    constexpr auto f = FixedNoRounding::fromFixedPoint<16>(384);
    constexpr auto fNeg = FixedNoRounding::fromFixedPoint<16>(-384);

    REQUIRE(f.rawValue() == 1);
    REQUIRE(fNeg.rawValue() == -1);

    static_assert(f.rawValue() == 1, "fromFixedPoint<16> must truncate when rounding disabled");
    static_assert(fNeg.rawValue() == -1, "fromFixedPoint<16> must truncate negative");
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
    constexpr Fixed fNeg(-4);
    constexpr auto neg = -f;
    constexpr auto negNeg = -fNeg;

    REQUIRE(f.rawValue() == 7 * 256);
    REQUIRE(neg.rawValue() == -7 * 256);
    REQUIRE(fNeg.rawValue() == -4 * 256);
    REQUIRE(negNeg.rawValue() == 4 * 256);

    static_assert(f.rawValue() == 7 * 256, "operand must be unchanged after operator-");
    static_assert(neg.rawValue() == -7 * 256, "operator- result must negate raw value");
    static_assert((-Fixed(-4)).rawValue() == 4 * 256, "operand unchanged and negation for negative");
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

    Fixed aNeg(-2);
    aNeg += Fixed(-3);
    REQUIRE(aNeg.rawValue() == -5 * 256);
    REQUIRE(static_cast<int>(aNeg) == -5);
  }

  // Add fixed with different rounding policy (raw storage is compatible).
  SUBCASE("plus_assign_different_rounding") {
    Fixed a(1);
    a += FixedNoRounding(2);

    REQUIRE(a.rawValue() == 3 * 256);

    Fixed aNeg(-1);
    aNeg += FixedNoRounding(-2);
    REQUIRE(aNeg.rawValue() == -3 * 256);

    FixedNoRounding b(10);
    b += Fixed(5);
    REQUIRE(b.rawValue() == 15 * 256);

    FixedNoRounding bNeg(-10);
    bNeg += Fixed(-5);
    REQUIRE(bNeg.rawValue() == -15 * 256);
  }

  // Add integral value (whole units) in place.
  SUBCASE("plus_assign_integral") {
    Fixed a(2);
    a += 3;

    REQUIRE(a.rawValue() == 5 * 256);
    REQUIRE(static_cast<int>(a) == 5);

    Fixed aNeg(-2);
    aNeg += -3;
    REQUIRE(aNeg.rawValue() == -5 * 256);
    REQUIRE(static_cast<int>(aNeg) == -5);
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

    constexpr Fixed aNeg = []() {
      Fixed x(-1);
      x += Fixed(-2);
      x += -3;
      return x;
    }();
    REQUIRE(aNeg.rawValue() == -6 * 256);

    static_assert(a.rawValue() == 6 * 256, "operator+= must be constexpr");
    static_assert(
      []() {
        Fixed x(-1);
        x += Fixed(-2);
        return x.rawValue();
      }()
        == -3 * 256,
      "operator+= negative must be constexpr");
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

    Fixed aNeg(-5);
    aNeg -= Fixed(-2);
    REQUIRE(aNeg.rawValue() == -3 * 256);
    REQUIRE(static_cast<int>(aNeg) == -3);
  }

  // Subtract fixed with different rounding policy in place.
  SUBCASE("minus_assign_different_rounding") {
    Fixed a(10);
    a -= FixedNoRounding(3);

    REQUIRE(a.rawValue() == 7 * 256);

    Fixed aNeg(-10);
    aNeg -= FixedNoRounding(-3);
    REQUIRE(aNeg.rawValue() == -7 * 256);

    FixedNoRounding b(8);
    b -= Fixed(2);
    REQUIRE(b.rawValue() == 6 * 256);

    FixedNoRounding bNeg(-8);
    bNeg -= Fixed(-2);
    REQUIRE(bNeg.rawValue() == -6 * 256);
  }

  // Subtract integral value (whole units) in place.
  SUBCASE("minus_assign_integral") {
    Fixed a(5);
    a -= 2;

    REQUIRE(a.rawValue() == 3 * 256);
    REQUIRE(static_cast<int>(a) == 3);

    Fixed aNeg(-5);
    aNeg -= -2;
    REQUIRE(aNeg.rawValue() == -3 * 256);
    REQUIRE(static_cast<int>(aNeg) == -3);
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

    constexpr Fixed aNeg = []() {
      Fixed x(-10);
      x -= Fixed(-3);
      x -= -2;
      return x;
    }();
    REQUIRE(aNeg.rawValue() == -5 * 256);

    static_assert(a.rawValue() == 5 * 256, "operator-= must be constexpr");
    static_assert(
      []() {
        Fixed x(-10);
        x -= Fixed(-3);
        return x.rawValue();
      }()
        == -7 * 256,
      "operator-= negative must be constexpr");
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

    Fixed aNeg(-2);
    aNeg *= Fixed(3);
    REQUIRE(aNeg.rawValue() == -6 * 256);
    REQUIRE(static_cast<int>(aNeg) == -6);

    constexpr Fixed a_ct = []() {
      Fixed x(2);
      x *= Fixed(3);
      return x;
    }();
    static_assert(a_ct.rawValue() == 6 * 256, "operator*=(fixed) result must be 6 for 2*3");
    static_assert(
      []() {
        Fixed x(-2);
        x *= Fixed(3);
        return x.rawValue();
      }()
        == -6 * 256,
      "operator*=(fixed) negative");
  }

  // Multiply by fixed with different rounding policy in place.
  SUBCASE("mul_assign_different_rounding") {
    Fixed a(2);
    a *= FixedNoRounding(4);

    REQUIRE(static_cast<int>(a) == 8);

    Fixed aNeg(-2);
    aNeg *= FixedNoRounding(4);
    REQUIRE(static_cast<int>(aNeg) == -8);

    FixedNoRounding b(3);
    b *= Fixed(2);
    REQUIRE(static_cast<int>(b) == 6);

    FixedNoRounding bNeg(-3);
    bNeg *= Fixed(2);
    REQUIRE(static_cast<int>(bNeg) == -6);
  }

  // Multiply by integral (whole units) in place.
  SUBCASE("mul_assign_integral") {
    Fixed a(5);
    a *= 2;

    REQUIRE(a.rawValue() == 10 * 256);
    REQUIRE(static_cast<int>(a) == 10);

    Fixed aNeg(-5);
    aNeg *= 2;
    REQUIRE(aNeg.rawValue() == -10 * 256);
    REQUIRE(static_cast<int>(aNeg) == -10);
  }

  // operator*= is usable in constant expressions.
  SUBCASE("mul_assign_constexpr") {
    constexpr Fixed a = []() {
      Fixed x(3);
      x *= 2;
      return x;
    }();

    REQUIRE(a.rawValue() == 6 * 256);

    constexpr Fixed aNeg = []() {
      Fixed x(-3);
      x *= 2;
      return x;
    }();
    REQUIRE(aNeg.rawValue() == -6 * 256);

    static_assert(a.rawValue() == 6 * 256, "operator*=(T) must be constexpr");
    static_assert(
      []() {
        Fixed x(-3);
        x *= 2;
        return x.rawValue();
      }()
        == -6 * 256,
      "operator*=(T) negative must be constexpr");
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

    Fixed aNeg(-8);
    aNeg /= Fixed(2);
    REQUIRE(aNeg.rawValue() == -4 * 256);
    REQUIRE(static_cast<int>(aNeg) == -4);

    constexpr Fixed a_ct = []() {
      Fixed x(8);
      x /= Fixed(2);
      return x;
    }();
    static_assert(a_ct.rawValue() == 4 * 256, "operator/=(fixed) result must be 4 for 8/2");
    static_assert(
      []() {
        Fixed x(-8);
        x /= Fixed(2);
        return x.rawValue();
      }()
        == -4 * 256,
      "operator/=(fixed) negative");
  }

  // Divide by fixed with different rounding policy in place.
  SUBCASE("div_assign_different_rounding") {
    Fixed a(6);
    a /= FixedNoRounding(2);

    REQUIRE(static_cast<int>(a) == 3);

    Fixed aNeg(-6);
    aNeg /= FixedNoRounding(2);
    REQUIRE(static_cast<int>(aNeg) == -3);

    FixedNoRounding b(9);
    b /= Fixed(3);
    REQUIRE(static_cast<int>(b) == 3);

    FixedNoRounding bNeg(-9);
    bNeg /= Fixed(3);
    REQUIRE(static_cast<int>(bNeg) == -3);
  }

  // Divide by integral (whole units) in place.
  SUBCASE("div_assign_integral") {
    Fixed a(10);
    a /= 2;

    REQUIRE(a.rawValue() == 5 * 256);
    REQUIRE(static_cast<int>(a) == 5);

    Fixed aNeg(-10);
    aNeg /= 2;
    REQUIRE(aNeg.rawValue() == -5 * 256);
    REQUIRE(static_cast<int>(aNeg) == -5);
  }

  // operator/= is usable in constant expressions.
  SUBCASE("div_assign_constexpr") {
    constexpr Fixed a = []() {
      Fixed x(12);
      x /= 3;
      return x;
    }();

    REQUIRE(a.rawValue() == 4 * 256);

    constexpr Fixed aNeg = []() {
      Fixed x(-12);
      x /= 3;
      return x;
    }();
    REQUIRE(aNeg.rawValue() == -4 * 256);

    static_assert(a.rawValue() == 4 * 256, "operator/=(T) must be constexpr");
    static_assert(
      []() {
        Fixed x(-12);
        x /= 3;
        return x.rawValue();
      }()
        == -4 * 256,
      "operator/=(T) negative must be constexpr");
  }
}

} // namespace toy::math
