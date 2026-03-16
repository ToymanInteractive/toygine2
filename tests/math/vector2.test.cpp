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

using Fixed = fixed<int32_t, int64_t, 24>;

// Vector2 has fixed size and contiguous x, y layout (two components).
TEST_CASE("math/vector2/object_structure") {
  static_assert(sizeof(Vector2<float>) == sizeof(float) * 2, "Vector2 size must be 2× component size");
  static_assert(sizeof(Vector2<Fixed>) == sizeof(Fixed) * 2, "Vector2 size must be 2× component size");

  constexpr Vector2<float> vectorFloat;
  constexpr Vector2<Fixed> vectorFixed;

  const auto * arrFloat = vectorFloat.c_arr();
  const auto * arrFixed = vectorFixed.c_arr();

  REQUIRE(arrFloat == &vectorFloat.x);
  REQUIRE(arrFloat + 1 == &vectorFloat.y);
  REQUIRE(reinterpret_cast<const byte *>(arrFloat + 1) - reinterpret_cast<const byte *>(arrFloat) == sizeof(float));
  REQUIRE(arrFixed == &vectorFixed.x);
  REQUIRE(arrFixed + 1 == &vectorFixed.y);
  REQUIRE(reinterpret_cast<const byte *>(arrFixed + 1) - reinterpret_cast<const byte *>(arrFixed) == sizeof(Fixed));

  static_assert(!std::is_trivial_v<Vector2<float>>, "Vector2<float> must not be trivial");
  static_assert(!std::is_trivial_v<Vector2<Fixed>>, "Vector2<Fixed> must not be trivial");
  static_assert(std::is_trivially_copyable_v<Vector2<float>>, "Vector2<float> must be trivially copyable");
  static_assert(std::is_trivially_copyable_v<Vector2<Fixed>>, "Vector2<Fixed> must be trivially copyable");
  static_assert(std::is_standard_layout_v<Vector2<float>>, "Vector2<float> must have standard layout");
  static_assert(std::is_standard_layout_v<Vector2<Fixed>>, "Vector2<Fixed> must have standard layout");
}

// Constructors set x, y from coordinates or from pointer to two-element array.
TEST_CASE("math/vector2/constructors") {
  // Constructor with positive x, y.
  SUBCASE("constructor_with_positive_coordinates") {
    constexpr Vector2 vectorFloat(12.0f, 23.0f);
    constexpr Vector2 vectorFixed(Fixed(12), Fixed(23));

    REQUIRE(vectorFloat.x == doctest::Approx(12));
    REQUIRE(vectorFloat.y == doctest::Approx(23));
    REQUIRE(vectorFixed.x == 12);
    REQUIRE(vectorFixed.y == 23);

    static_assert(isEqual(vectorFloat.x, 12.0f), "x must match constructor argument");
    static_assert(isEqual(vectorFloat.y, 23.0f), "y must match constructor argument");
    static_assert(vectorFixed.x == 12, "x must match constructor argument");
    static_assert(vectorFixed.y == 23, "y must match constructor argument");
  }

  // Constructor with negative x, y.
  SUBCASE("constructor_with_negative_coordinates") {
    constexpr Vector2 vectorFloat(-5.0f, -15.0f);
    constexpr Vector2 vectorFixed(Fixed(-5), Fixed(-15));

    REQUIRE(vectorFloat.x == doctest::Approx(-5));
    REQUIRE(vectorFloat.y == doctest::Approx(-15));
    REQUIRE(vectorFixed.x == -5);
    REQUIRE(vectorFixed.y == -15);

    static_assert(isEqual(vectorFloat.x, -5.0f), "x must match constructor argument");
    static_assert(isEqual(vectorFloat.y, -15.0f), "y must match constructor argument");
    static_assert(vectorFixed.x == -5, "x must match constructor argument");
    static_assert(vectorFixed.y == -15, "y must match constructor argument");
  }

  // Constructor with mixed-sign x, y.
  SUBCASE("constructor_with_mixed_coordinates") {
    constexpr Vector2 vectorFloat(-100.0f, 200.0f);
    constexpr Vector2 vectorFixed(Fixed(-100), Fixed(200));

    REQUIRE(vectorFloat.x == doctest::Approx(-100));
    REQUIRE(vectorFloat.y == doctest::Approx(200));
    REQUIRE(vectorFixed.x == -100);
    REQUIRE(vectorFixed.y == 200);

    static_assert(isEqual(vectorFloat.x, -100.0f), "x must match constructor argument");
    static_assert(isEqual(vectorFloat.y, 200.0f), "y must match constructor argument");
    static_assert(vectorFixed.x == -100, "x must match constructor argument");
    static_assert(vectorFixed.y == 200, "y must match constructor argument");
  }

  // Constructor with zero x, y.
  SUBCASE("constructor_with_zero_coordinates") {
    constexpr Vector2 vectorFloat(0.0f, 0.0f);
    constexpr Vector2 vectorFixed(Fixed(0), Fixed(0));

    REQUIRE(vectorFloat.x == doctest::Approx(0));
    REQUIRE(vectorFloat.y == doctest::Approx(0));
    REQUIRE(vectorFixed.x == 0);
    REQUIRE(vectorFixed.y == 0);

    static_assert(isEqual(vectorFloat.x, 0.0f), "x must be 0");
    static_assert(isEqual(vectorFloat.y, 0.0f), "y must be 0");
    static_assert(vectorFixed.x == 0, "x must be 0");
    static_assert(vectorFixed.y == 0, "y must be 0");
  }

  // Constructor from pointer to two-element array.
  SUBCASE("constructor_from_pointer_to_array") {
    constexpr array<float, 2> arrFloat{{42.0f, -17.0f}};
    constexpr array<Fixed, 2> arrFixed{Fixed(42), Fixed(-17)};
    constexpr Vector2 vectorFloat(arrFloat.data());
    constexpr Vector2 vectorFixed(arrFixed.data());

    REQUIRE(vectorFloat.x == doctest::Approx(42));
    REQUIRE(vectorFloat.y == doctest::Approx(-17));
    REQUIRE(vectorFixed.x == 42);
    REQUIRE(vectorFixed.y == -17);

    static_assert(isEqual(vectorFloat.x, 42.0f), "x must match array element");
    static_assert(isEqual(vectorFloat.y, -17.0f), "y must match array element");
    static_assert(vectorFixed.x == 42, "x must match array element");
    static_assert(vectorFixed.y == -17, "y must match array element");
  }

  // Runtime constructor behavior for float and Fixed.
  SUBCASE("runtime_constructors") {
    Vector2 vectorFloat(123.0f, -456.0f);
    Vector2 vectorFixed(Fixed(123), Fixed(-456));
    REQUIRE(vectorFloat.x == doctest::Approx(123));
    REQUIRE(vectorFloat.y == doctest::Approx(-456));
    REQUIRE(vectorFixed.x == 123);
    REQUIRE(vectorFixed.y == -456);

    constexpr array<float, 2> arrFloat{{789.0f, -321.0f}};
    constexpr array<Fixed, 2> arrFixed{Fixed(789), Fixed(-321)};
    Vector2 arrayFloat(arrFloat.data());
    Vector2 arrayFixed(arrFixed.data());
    REQUIRE(arrayFloat.x == doctest::Approx(789));
    REQUIRE(arrayFloat.y == doctest::Approx(-321));
    REQUIRE(arrayFixed.x == 789);
    REQUIRE(arrayFixed.y == -321);
  }
}

// c_arr() returns pointer to contiguous x, y; layout matches struct order.
TEST_CASE("math/vector2/c_arr_methods") {
  // Non-const c_arr() returns writable pointer to x,y (float and Fixed).
  SUBCASE("non_const_c_arr") {
    Vector2 vectorFloat(42.0f, -17.0f);
    Vector2 vectorFixed(Fixed(42), Fixed(-17));
    auto * arrFloat = vectorFloat.c_arr();
    auto * arrFixed = vectorFixed.c_arr();

    REQUIRE(arrFloat == &vectorFloat.x);
    REQUIRE(arrFloat[0] == doctest::Approx(42));
    REQUIRE(arrFloat[1] == doctest::Approx(-17));
    REQUIRE(arrFixed == &vectorFixed.x);
    REQUIRE(arrFixed[0] == 42);
    REQUIRE(arrFixed[1] == -17);

    arrFloat[0] = 100.0f;
    arrFloat[1] = -200.0f;
    arrFixed[0] = Fixed(100);
    arrFixed[1] = Fixed(-200);
    REQUIRE(vectorFloat.x == doctest::Approx(100));
    REQUIRE(vectorFloat.y == doctest::Approx(-200));
    REQUIRE(vectorFixed.x == 100);
    REQUIRE(vectorFixed.y == -200);
  }

  // Const c_arr() returns pointer to x,y.
  SUBCASE("const_c_arr") {
    constexpr Vector2 vectorFloat(123.0f, -456.0f);
    constexpr Vector2 vectorFixed(Fixed(123), Fixed(-456));
    const auto * arrFloat = vectorFloat.c_arr();
    const auto * arrFixed = vectorFixed.c_arr();

    REQUIRE(arrFloat == &vectorFloat.x);
    REQUIRE(arrFloat[0] == doctest::Approx(123));
    REQUIRE(arrFloat[1] == doctest::Approx(-456));
    REQUIRE(arrFixed[0] == 123);
    REQUIRE(arrFixed[1] == -456);
  }
}

// operator+=, -=, *=, /= and chaining.
TEST_CASE("math/vector2/operators") {
  // operator+= adds vector (float and Fixed).
  SUBCASE("operator_plus_assign") {
    Vector2 vectorFloat(10.0f, 20.0f);
    Vector2 vectorFixed(Fixed(10), Fixed(20));
    constexpr Vector2 addFloat(5.0f, -10.0f);
    constexpr Vector2 addFixed(Fixed(5), Fixed(-10));

    vectorFloat += addFloat;
    vectorFixed += addFixed;

    REQUIRE(vectorFloat.x == doctest::Approx(15));
    REQUIRE(vectorFloat.y == doctest::Approx(10));
    REQUIRE(vectorFixed.x == 15);
    REQUIRE(vectorFixed.y == 10);
  }

  // operator-= subtracts vector.
  SUBCASE("operator_minus_assign") {
    Vector2 vectorFloat(15.0f, 25.0f);
    Vector2 vectorFixed(Fixed(15), Fixed(25));
    constexpr Vector2 subFloat(5.0f, 10.0f);
    constexpr Vector2 subFixed(Fixed(5), Fixed(10));

    vectorFloat -= subFloat;
    vectorFixed -= subFixed;

    REQUIRE(vectorFloat.x == doctest::Approx(10));
    REQUIRE(vectorFloat.y == doctest::Approx(15));
    REQUIRE(vectorFixed.x == 10);
    REQUIRE(vectorFixed.y == 15);
  }

  // operator*= multiplies by scalar (float and Fixed).
  SUBCASE("operator_times_assign") {
    Vector2 vectorFloat(10.0f, 20.0f);
    Vector2 vectorFixed(Fixed(10), Fixed(20));
    constexpr float scalarFloat = 2.5f;
    constexpr Fixed scalarFixed(2.5f);

    vectorFloat *= scalarFloat;
    vectorFixed *= scalarFixed;

    REQUIRE(vectorFloat.x == doctest::Approx(25));
    REQUIRE(vectorFloat.y == doctest::Approx(50));
    REQUIRE(vectorFixed.x == 25);
    REQUIRE(vectorFixed.y == 50);
  }

  // operator/= divides by scalar.
  SUBCASE("operator_div_assign") {
    Vector2 vectorFloat(20.0f, 45.0f);
    Vector2 vectorFixed(Fixed(20), Fixed(45));
    constexpr float scalarFloat = 2.5f;
    constexpr Fixed scalarFixed(2.5f);

    vectorFloat /= scalarFloat;
    vectorFixed /= scalarFixed;

    REQUIRE(vectorFloat.x == doctest::Approx(8));
    REQUIRE(vectorFloat.y == doctest::Approx(18));
    REQUIRE(isEqual(vectorFixed.x, Fixed(8)));
    REQUIRE(isEqual(vectorFixed.y, Fixed(18)));
  }

  // Chained compound assignments.
  SUBCASE("chained_compound_assign") {
    Vector2 vectorFloat(10.0f, 20.0f);
    Vector2 vectorFixed(Fixed(10), Fixed(20));
    constexpr Vector2 offsetFloat(5.0f, 10.0f);
    constexpr Vector2 offsetFixed(Fixed(5), Fixed(10));

    vectorFloat += offsetFloat;
    vectorFloat *= 2.0f;
    vectorFloat -= offsetFloat;
    vectorFixed += offsetFixed;
    vectorFixed *= Fixed(2);
    vectorFixed -= offsetFixed;

    REQUIRE(vectorFloat.x == doctest::Approx(25));
    REQUIRE(vectorFloat.y == doctest::Approx(50));
    REQUIRE(vectorFixed.x == 25);
    REQUIRE(vectorFixed.y == 50);
  }
}

// sqrMagnitude(), setZero(), isZero(), isEqual() behavior.
TEST_CASE("math/vector2/methods") {
  // sqrMagnitude() returns x² + y² (float and Fixed).
  SUBCASE("sqr_magnitude") {
    constexpr Vector2 vectorFloat(3.0f, 4.0f);
    constexpr Vector2 vectorFixed(Fixed(3), Fixed(4));

    REQUIRE(vectorFloat.sqrMagnitude() == doctest::Approx(25));
    REQUIRE(vectorFixed.sqrMagnitude() == 25);

    static_assert(isEqual(vectorFloat.sqrMagnitude(), 25.0f), "sqrMagnitude of (3,4) must be 25");
    static_assert(vectorFixed.sqrMagnitude() == 25, "sqrMagnitude of (3,4) must be 25");
  }

  // sqrMagnitude() is zero for origin.
  SUBCASE("sqr_magnitude_zero") {
    constexpr Vector2 vectorFloat(0.0f, 0.0f);
    constexpr Vector2 vectorFixed(Fixed(0), Fixed(0));

    REQUIRE(vectorFloat.sqrMagnitude() == doctest::Approx(0));
    REQUIRE(vectorFixed.sqrMagnitude() == 0);

    static_assert(vectorFixed.sqrMagnitude() == 0, "sqrMagnitude of origin must be 0");
  }

  // setZero() sets x, y to zero.
  SUBCASE("set_zero") {
    Vector2 vectorFloat(100.0f, 200.0f);
    Vector2 vectorFixed(Fixed(100), Fixed(200));

    vectorFloat.setZero();
    vectorFixed.setZero();

    REQUIRE(vectorFloat.x == doctest::Approx(0));
    REQUIRE(vectorFloat.y == doctest::Approx(0));
    REQUIRE(vectorFixed.x == 0);
    REQUIRE(vectorFixed.y == 0);
  }

  // isZero() true for zero vector, false otherwise; true after setZero().
  SUBCASE("is_zero") {
    constexpr Vector2 vectorFloatZero(0.0f, 0.0f);
    constexpr Vector2 vectorFixedZero(Fixed(0), Fixed(0));
    constexpr Vector2 vectorFloatNonZero(1.0f, 0.0f);
    constexpr Vector2 vectorFixedNonZero(Fixed(1), Fixed(0));

    REQUIRE(vectorFloatZero.isZero());
    REQUIRE(vectorFixedZero.isZero());
    REQUIRE(!vectorFloatNonZero.isZero());
    REQUIRE(!vectorFixedNonZero.isZero());

    static_assert(vectorFloatZero.isZero(), "origin must be zero");
    static_assert(vectorFixedZero.isZero(), "origin must be zero");
    static_assert(!vectorFloatNonZero.isZero(), "non-zero vector must not be zero");
    static_assert(!vectorFixedNonZero.isZero(), "non-zero vector must not be zero");

    Vector2 v(100.0f, 200.0f);
    REQUIRE(!v.isZero());
    v.setZero();
    REQUIRE(v.isZero());
  }

  // isEqual() exact match and different vectors (float and Fixed).
  SUBCASE("is_equal_exact_and_different") {
    constexpr Vector2 aFloat(10.0f, 20.0f);
    constexpr Vector2 bFloat(10.0f, 20.0f);
    constexpr Vector2 cFloat(11.0f, 20.0f);
    constexpr Vector2 aFixed(Fixed(10), Fixed(20));
    constexpr Vector2 bFixed(Fixed(10), Fixed(20));
    constexpr Vector2 cFixed(Fixed(11), Fixed(20));

    REQUIRE(aFloat.isEqual(bFloat));
    REQUIRE(!aFloat.isEqual(cFloat));
    REQUIRE(aFixed.isEqual(bFixed));
    REQUIRE(!aFixed.isEqual(cFixed));

    static_assert(aFloat.isEqual(bFloat), "identical float vectors must compare equal with isEqual");
    static_assert(!aFloat.isEqual(cFloat), "different float vectors must not compare equal with isEqual");
    static_assert(aFixed.isEqual(bFixed), "identical fixed vectors must compare equal with isEqual");
    static_assert(!aFixed.isEqual(cFixed), "different fixed vectors must not compare equal with isEqual");
  }

  // isEqual() with tolerance: within tolerance returns true, outside returns false.
  SUBCASE("is_equal_with_tolerance") {
    constexpr Vector2 v1Float(10.0f, 20.0f);
    constexpr Vector2 v2Float(12.0f, 18.0f);
    constexpr float toleranceOk = 3.0f;
    constexpr Vector2 v3Float(15.0f, 25.0f);
    constexpr float toleranceTooSmall = 2.0f;

    REQUIRE(v1Float.isEqual(v2Float, toleranceOk));
    REQUIRE(!v1Float.isEqual(v3Float, toleranceTooSmall));

    static_assert(v1Float.isEqual(v2Float, toleranceOk), "isEqual with sufficient tolerance must return true");
    static_assert(!v1Float.isEqual(v3Float, toleranceTooSmall), "isEqual with too small tolerance must be false");
  }

  // isEqual() with zero tolerance requires exact match.
  SUBCASE("is_equal_zero_tolerance") {
    constexpr Vector2 v1(10.0f, 20.0f);
    constexpr Vector2 v2(10.0f, 21.0f);
    constexpr float tol = 0.0f;

    REQUIRE(!v1.isEqual(v2, tol));

    static_assert(!v1.isEqual(v2, tol), "isEqual with zero tolerance must require exact component match");
  }
}

// Binary +, -, *, /, unary minus, operator==, operator!=, cross().
TEST_CASE("math/vector2/binary_operators") {
  // Unary minus negates x, y (float and Fixed).
  SUBCASE("unary_minus") {
    constexpr Vector2 vectorFloat(10.0f, -20.0f);
    constexpr Vector2 vectorFixed(Fixed(10), Fixed(-20));
    constexpr auto resultFloat = -vectorFloat;
    constexpr auto resultFixed = -vectorFixed;

    REQUIRE(resultFloat.x == doctest::Approx(-10));
    REQUIRE(resultFloat.y == doctest::Approx(20));
    REQUIRE(resultFixed.x == -10);
    REQUIRE(resultFixed.y == 20);

    static_assert(isEqual(resultFloat.x, -10.0f), "unary minus must negate x");
    static_assert(isEqual(resultFloat.y, 20.0f), "unary minus must negate y");
    static_assert(resultFixed.x == -10, "unary minus must negate x");
    static_assert(resultFixed.y == 20, "unary minus must negate y");
  }

  // operator+ adds vectors component-wise.
  SUBCASE("addition") {
    constexpr Vector2 aFloat(10.0f, 20.0f);
    constexpr Vector2 bFloat(5.0f, -10.0f);
    constexpr Vector2 aFixed(Fixed(10), Fixed(20));
    constexpr Vector2 bFixed(Fixed(5), Fixed(-10));
    constexpr auto resultFloat = aFloat + bFloat;
    constexpr auto resultFixed = aFixed + bFixed;

    REQUIRE(resultFloat.x == doctest::Approx(15));
    REQUIRE(resultFloat.y == doctest::Approx(10));
    REQUIRE(resultFixed.x == 15);
    REQUIRE(resultFixed.y == 10);

    static_assert(isEqual(resultFloat.x, 15.0f), "addition x must be sum of x components");
    static_assert(isEqual(resultFloat.y, 10.0f), "addition y must be sum of y components");
    static_assert(resultFixed.x == 15, "addition x must be sum of x components");
    static_assert(resultFixed.y == 10, "addition y must be sum of y components");
  }

  // operator- subtracts vectors component-wise.
  SUBCASE("subtraction") {
    constexpr Vector2 aFloat(15.0f, 25.0f);
    constexpr Vector2 bFloat(5.0f, 10.0f);
    constexpr Vector2 aFixed(Fixed(15), Fixed(25));
    constexpr Vector2 bFixed(Fixed(5), Fixed(10));
    constexpr auto resultFloat = aFloat - bFloat;
    constexpr auto resultFixed = aFixed - bFixed;

    REQUIRE(resultFloat.x == doctest::Approx(10));
    REQUIRE(resultFloat.y == doctest::Approx(15));
    REQUIRE(resultFixed.x == 10);
    REQUIRE(resultFixed.y == 15);

    static_assert(isEqual(resultFloat.x, 10.0f), "subtraction x must be difference of x components");
    static_assert(isEqual(resultFloat.y, 15.0f), "subtraction y must be difference of y components");
    static_assert(resultFixed.x == 10, "subtraction x must be difference of x components");
    static_assert(resultFixed.y == 15, "subtraction y must be difference of y components");
  }

  // Vector * scalar and scalar * vector (float and Fixed).
  SUBCASE("multiplication_scalar") {
    constexpr Vector2 vectorFloat(10.0f, 20.0f);
    constexpr Vector2 vectorFixed(Fixed(10), Fixed(20));
    constexpr float scalarFloat = 2.5f;
    constexpr Fixed scalarFixed(2.5f);

    constexpr auto resultFloat = vectorFloat * scalarFloat;
    constexpr auto resultFixed = vectorFixed * scalarFixed;
    REQUIRE(resultFloat.x == doctest::Approx(25));
    REQUIRE(resultFloat.y == doctest::Approx(50));
    REQUIRE(resultFixed.x == 25);
    REQUIRE(resultFixed.y == 50);

    static_assert(isEqual(resultFloat.x, 25.0f), "vector * scalar must scale x component");
    static_assert(isEqual(resultFloat.y, 50.0f), "vector * scalar must scale y component");
    static_assert(resultFixed.x == 25, "vector * scalar must scale x component for fixed");
    static_assert(resultFixed.y == 50, "vector * scalar must scale y component for fixed");

    constexpr auto swappedFloat = scalarFloat * vectorFloat;
    constexpr auto swappedFixed = scalarFixed * vectorFixed;
    REQUIRE(swappedFloat.x == doctest::Approx(25));
    REQUIRE(swappedFloat.y == doctest::Approx(50));
    REQUIRE(swappedFixed.x == 25);
    REQUIRE(swappedFixed.y == 50);

    static_assert(isEqual(swappedFloat.x, 25.0f), "scalar * vector must scale x component");
    static_assert(isEqual(swappedFloat.y, 50.0f), "scalar * vector must scale y component");
    static_assert(swappedFixed.x == 25, "scalar * vector must scale x component for fixed");
    static_assert(swappedFixed.y == 50, "scalar * vector must scale y component for fixed");
  }

  // Dot product: left.x*right.x + left.y*right.y.
  SUBCASE("dot_product") {
    constexpr Vector2 aFloat(10.0f, 20.0f);
    constexpr Vector2 bFloat(5.0f, 10.0f);
    constexpr Vector2 aFixed(Fixed(10), Fixed(20));
    constexpr Vector2 bFixed(Fixed(5), Fixed(10));

    REQUIRE(aFloat * bFloat == doctest::Approx(250.0f));
    REQUIRE(aFixed * bFixed == 250);

    static_assert(aFixed * bFixed == 250, "dot product (10,20)·(5,10) must be 250");
  }

  // Division by scalar.
  SUBCASE("division_scalar") {
    constexpr Vector2 vectorFloat(25.0f, 50.0f);
    constexpr Vector2 vectorFixed(Fixed(25), Fixed(50));
    constexpr float scalarFloat = 2.5f;
    constexpr Fixed scalarFixed(2.5f);

    constexpr auto resultFloat = vectorFloat / scalarFloat;
    constexpr auto resultFixed = vectorFixed / scalarFixed;

    REQUIRE(resultFloat.x == doctest::Approx(10));
    REQUIRE(resultFloat.y == doctest::Approx(20));
    REQUIRE(isEqual(resultFixed.x, Fixed(10)));
    REQUIRE(isEqual(resultFixed.y, Fixed(20)));

    static_assert(isEqual(resultFloat.x, 10.0f), "vector / scalar must divide x component");
    static_assert(isEqual(resultFloat.y, 20.0f), "vector / scalar must divide y component");
    static_assert(isEqual(resultFixed.x, Fixed(10)), "vector / scalar must divide x component for fixed");
    static_assert(isEqual(resultFixed.y, Fixed(20)), "vector / scalar must divide y component for fixed");
  }

  // operator== and operator!= (float and Fixed).
  SUBCASE("equality") {
    constexpr Vector2 aFloat(10.0f, 20.0f);
    constexpr Vector2 bFloat(10.0f, 20.0f);
    constexpr Vector2 cFloat(11.0f, 20.0f);
    constexpr Vector2 aFixed(Fixed(10), Fixed(20));
    constexpr Vector2 bFixed(Fixed(10), Fixed(20));
    constexpr Vector2 cFixed(Fixed(11), Fixed(20));

    REQUIRE(aFloat == bFloat);
    REQUIRE(aFloat != cFloat);
    REQUIRE(aFixed == bFixed);
    REQUIRE(aFixed != cFixed);

    static_assert(aFloat == bFloat, "identical vectors must compare equal");
    static_assert(aFloat != cFloat, "different vectors must not compare equal");
    static_assert(aFixed == bFixed, "identical vectors must compare equal");
    static_assert(aFixed != cFixed, "different vectors must not compare equal");
  }

  // operator== for float uses default tolerances (same as isEqual with defaults); near-equal vectors compare equal.
  SUBCASE("equality_float_within_default_tolerance") {
    constexpr Vector2 a(10.0f, 20.0f);
    constexpr Vector2 b(10.0f + 1e-6f, 20.0f - 1e-6f);

    REQUIRE(a == b);

    static_assert(a == b, "operator== must treat near-equal float vectors as equal");
  }

  // operator!= is the negation of operator==.
  SUBCASE("inequality") {
    constexpr Vector2 aFloat(10.0f, 20.0f);
    constexpr Vector2 bFloat(10.0f, 20.0f);
    constexpr Vector2 cFloat(11.0f, 20.0f);
    constexpr Vector2 aFixed(Fixed(10), Fixed(20));
    constexpr Vector2 bFixed(Fixed(10), Fixed(20));
    constexpr Vector2 cFixed(Fixed(11), Fixed(20));

    REQUIRE(!(aFloat != bFloat));
    REQUIRE(aFloat != cFloat);
    REQUIRE(!(aFixed != bFixed));
    REQUIRE(aFixed != cFixed);

    static_assert(!(aFloat != bFloat), "identical vectors must not be unequal");
    static_assert(aFloat != cFloat, "different vectors must be unequal");
    static_assert(!(aFixed != bFixed), "identical vectors must not be unequal");
    static_assert(aFixed != cFixed, "different vectors must be unequal");
  }

  // cross(left, right) = left.x*right.y - left.y*right.x.
  SUBCASE("cross") {
    constexpr Vector2 aFloat(3.0f, 4.0f);
    constexpr Vector2 bFloat(1.0f, 0.0f);
    constexpr Vector2 aFixed(Fixed(3), Fixed(4));
    constexpr Vector2 bFixed(Fixed(1), Fixed(0));

    REQUIRE(cross(aFloat, bFloat) == doctest::Approx(-4.0f));
    REQUIRE(cross(aFixed, bFixed) == -4);

    static_assert(cross(aFixed, bFixed) == -4, "cross (3,4)×(1,0) must be -4");
  }

  // Chained binary operations.
  SUBCASE("chained") {
    constexpr Vector2 v1Float(10.0f, 20.0f);
    constexpr Vector2 v2Float(5.0f, 10.0f);
    constexpr float sFloat = 2.0f;
    constexpr auto resultFloat = (v1Float + v2Float) * sFloat - v1Float;

    REQUIRE(resultFloat.x == doctest::Approx(20));
    REQUIRE(resultFloat.y == doctest::Approx(40));

    static_assert(isEqual(resultFloat.x, 20.0f), "chained (v1+v2)*s-v1 must yield correct x");
    static_assert(isEqual(resultFloat.y, 40.0f), "chained (v1+v2)*s-v1 must yield correct y");

    constexpr Vector2 v1Fixed(Fixed(10), Fixed(20));
    constexpr Vector2 v2Fixed(Fixed(5), Fixed(10));
    constexpr Fixed sFixed(2);
    constexpr auto resultFixed = (v1Fixed + v2Fixed) * sFixed - v1Fixed;

    REQUIRE(resultFixed.x == 20);
    REQUIRE(resultFixed.y == 40);

    static_assert(resultFixed.x == 20, "chained (v1+v2)*s-v1 must yield correct x");
    static_assert(resultFixed.y == 40, "chained (v1+v2)*s-v1 must yield correct y");
  }
}

} // namespace toy::math
