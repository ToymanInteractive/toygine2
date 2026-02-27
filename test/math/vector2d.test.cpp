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

// Vector2D has fixed size and contiguous x,y layout.
TEST_CASE("math/vector2d/object_structure") {
  constexpr Vector2D vector(111, 222);

  static_assert(sizeof(vector) == sizeof(real_t) * 2, "Vector2D must have size of two real_t");

  const auto * arr = vector.c_arr();

  REQUIRE(arr == &vector.x);
  REQUIRE(arr + 1 == &vector.y);
  REQUIRE(reinterpret_cast<const std::byte *>(arr + 1) - reinterpret_cast<const std::byte *>(arr) == sizeof(real_t));
}

// Default, coordinate, and array constructors.
TEST_CASE("math/vector2d/constructors") {
  // Default constructor yields zero x, y.
  SUBCASE("default_constructor") {
    constexpr Vector2D vector;

    REQUIRE(vector.x == doctest::Approx(0.0));
    REQUIRE(vector.y == doctest::Approx(0.0));

    static_assert(vector.x == 0.0, "default-constructed x must be 0");
    static_assert(vector.y == 0.0, "default-constructed y must be 0");
  }

  // Constructor with positive x, y.
  SUBCASE("constructor_with_positive_coordinates") {
    constexpr Vector2D vector(12, 23);

    REQUIRE(vector.x == doctest::Approx(12));
    REQUIRE(vector.y == doctest::Approx(23));

    static_assert(vector.x == 12, "x must match constructor argument");
    static_assert(vector.y == 23, "y must match constructor argument");
  }

  // Constructor with negative x, y.
  SUBCASE("constructor_with_negative_coordinates") {
    constexpr Vector2D vector(-5, -15);

    REQUIRE(vector.x == doctest::Approx(-5));
    REQUIRE(vector.y == doctest::Approx(-15));

    static_assert(vector.x == -5, "x must match constructor argument");
    static_assert(vector.y == -15, "y must match constructor argument");
  }

  // Constructor with mixed-sign x, y.
  SUBCASE("constructor_with_mixed_coordinates") {
    constexpr Vector2D vector(-100, 200);

    REQUIRE(vector.x == doctest::Approx(-100));
    REQUIRE(vector.y == doctest::Approx(200));

    static_assert(vector.x == -100, "x must match constructor argument");
    static_assert(vector.y == 200, "y must match constructor argument");
  }

  // Constructor with zero x, y.
  SUBCASE("constructor_with_zero_coordinates") {
    constexpr Vector2D vector(0, 0);

    REQUIRE(vector.x == doctest::Approx(0));
    REQUIRE(vector.y == doctest::Approx(0));

    static_assert(vector.x == 0, "x must be 0");
    static_assert(vector.y == 0, "y must be 0");
  }

  // Constructor from pointer to two-element array.
  SUBCASE("constructor_from_pointer_to_array") {
    constexpr array<real_t, 2> arr{42, -17};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == doctest::Approx(42));
    REQUIRE(vector.y == doctest::Approx(-17));

    static_assert(vector.x == 42, "x must match array element");
    static_assert(vector.y == -17, "y must match array element");
  }

  // Constructor from array with positive values.
  SUBCASE("constructor_from_pointer_to_array_with_positive_values") {
    constexpr array<real_t, 2> arr{100, 200};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == doctest::Approx(100));
    REQUIRE(vector.y == doctest::Approx(200));

    static_assert(vector.x == 100, "x must match array element");
    static_assert(vector.y == 200, "y must match array element");
  }

  // Constructor from array with negative values.
  SUBCASE("constructor_from_pointer_to_array_with_negative_values") {
    constexpr array<real_t, 2> arr{-50, -75};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == doctest::Approx(-50));
    REQUIRE(vector.y == doctest::Approx(-75));

    static_assert(vector.x == -50, "x must match array element");
    static_assert(vector.y == -75, "y must match array element");
  }

  // Constructor from array with mixed-sign values.
  SUBCASE("constructor_from_pointer_to_array_with_mixed_values") {
    constexpr array<real_t, 2> arr{-300, 400};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == doctest::Approx(-300));
    REQUIRE(vector.y == doctest::Approx(400));

    static_assert(vector.x == -300, "x must match array element");
    static_assert(vector.y == 400, "y must match array element");
  }

  // Constructor from array with zero values.
  SUBCASE("constructor_from_pointer_to_array_with_zero_values") {
    constexpr array<real_t, 2> arr{0, 0};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == doctest::Approx(0));
    REQUIRE(vector.y == doctest::Approx(0));

    static_assert(vector.x == 0, "x must be 0");
    static_assert(vector.y == 0, "y must be 0");
  }

  // Runtime constructor behavior.
  SUBCASE("runtime_constructor_tests") {
    Vector2D defaultVector;
    REQUIRE(defaultVector.x == doctest::Approx(0));
    REQUIRE(defaultVector.y == doctest::Approx(0));

    Vector2D coordVector(123, -456);
    REQUIRE(coordVector.x == doctest::Approx(123));
    REQUIRE(coordVector.y == doctest::Approx(-456));

    constexpr array<real_t, 2> arr{789, -321};
    Vector2D arrayVector(arr.data());
    REQUIRE(arrayVector.x == doctest::Approx(789));
    REQUIRE(arrayVector.y == doctest::Approx(-321));
  }
}

// c_arr returns pointer to contiguous x,y.
TEST_CASE("math/vector2d/c_arr_methods") {
  // Non-const c_arr() returns writable pointer to x,y.
  SUBCASE("non_const_c_arr_method") {
    Vector2D vector(42, -17);
    auto * arr = vector.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == doctest::Approx(42));
    REQUIRE(arr[1] == doctest::Approx(-17));
    REQUIRE(arr == &vector.x);

    arr[0] = 100;
    arr[1] = -200;

    REQUIRE(vector.x == doctest::Approx(100));
    REQUIRE(vector.y == doctest::Approx(-200));
  }

  // Const c_arr() returns pointer to x,y.
  SUBCASE("const_c_arr_method") {
    constexpr Vector2D vector(123, -456);
    const auto * arr = vector.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == doctest::Approx(123));
    REQUIRE(arr[1] == doctest::Approx(-456));
    REQUIRE(arr == &vector.x);
    REQUIRE(vector.x == doctest::Approx(123));
    REQUIRE(vector.y == doctest::Approx(-456));
  }

  // c_arr() with default-constructed vector.
  SUBCASE("c_arr_with_default_constructor") {
    constexpr Vector2D vector;

    REQUIRE(vector.c_arr()[0] == doctest::Approx(0));
    REQUIRE(vector.c_arr()[1] == doctest::Approx(0));

    static_assert(vector.c_arr()[0] == 0, "first element must be 0 for default-constructed vector");
  }

  // c_arr() with coordinate constructor.
  SUBCASE("c_arr_with_coordinate_constructor") {
    constexpr Vector2D vector(10, 20);

    REQUIRE(vector.c_arr()[0] == doctest::Approx(10));
    REQUIRE(vector.c_arr()[1] == doctest::Approx(20));

    static_assert(vector.c_arr()[0] == 10, "first element must match x");
  }

  // c_arr() with array constructor.
  SUBCASE("c_arr_with_array_constructor") {
    constexpr Vector2D vector({-50, 75});

    REQUIRE(vector.c_arr()[0] == doctest::Approx(-50));
    REQUIRE(vector.c_arr()[1] == doctest::Approx(75));

    static_assert(vector.c_arr()[0] == -50, "first element must match x");
  }

  SUBCASE("c_arr_modification_test") {
    Vector2D vector(1, 2);
    auto * arr = vector.c_arr();

    arr[0] = 10;
    arr[1] = 20;

    REQUIRE(vector.x == doctest::Approx(10));
    REQUIRE(vector.y == doctest::Approx(20));

    *arr = 30;
    *(arr + 1) = 40;

    REQUIRE(vector.x == doctest::Approx(30));
    REQUIRE(vector.y == doctest::Approx(40));

    arr[0] = 50;
    arr[1] = 60;

    REQUIRE(vector.x == doctest::Approx(50));
    REQUIRE(vector.y == doctest::Approx(60));
  }

  // Runtime c_arr() behavior.
  SUBCASE("c_arr_runtime_tests") {
    Vector2D runtimeVector(500, -600);
    auto * runtimeArr = runtimeVector.c_arr();

    REQUIRE(runtimeArr != nullptr);
    REQUIRE(runtimeArr[0] == doctest::Approx(500));
    REQUIRE(runtimeArr[1] == doctest::Approx(-600));

    runtimeArr[0] = 700;
    runtimeArr[1] = -800;

    REQUIRE(runtimeVector.x == doctest::Approx(700));
    REQUIRE(runtimeVector.y == doctest::Approx(-800));

    const Vector2D constRuntimeVector(900, -1000);
    const auto * constRuntimeArr = constRuntimeVector.c_arr();

    REQUIRE(constRuntimeArr != nullptr);
    REQUIRE(constRuntimeArr[0] == doctest::Approx(900));
    REQUIRE(constRuntimeArr[1] == doctest::Approx(-1000));
  }
}

// +=, -=, *=, /= and chaining.
TEST_CASE("math/vector2d/operators") {
  // operator+= adds vector.
  SUBCASE("operator_plus_assign") {
    Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, -10);

    vector1 += vector2;

    REQUIRE(vector1.x == doctest::Approx(15));
    REQUIRE(vector1.y == doctest::Approx(10));
  }

  // operator-= subtracts vector.
  SUBCASE("operator_minus_assign") {
    Vector2D vector1(15, 25);
    constexpr Vector2D vector2(5, 10);

    vector1 -= vector2;

    REQUIRE(vector1.x == doctest::Approx(10));
    REQUIRE(vector1.y == doctest::Approx(15));
  }

  // operator*= multiplies by scalar.
  SUBCASE("operator_times_assign") {
    Vector2D vector(10, 20);
    constexpr real_t scalar = 2.5;

    vector *= scalar;

    REQUIRE(vector.x == doctest::Approx(25));
    REQUIRE(vector.y == doctest::Approx(50));
  }

  // operator/= divides by scalar.
  SUBCASE("operator_div_assign") {
    Vector2D vector(20, 45);
    constexpr real_t scalar = 2.5;

    vector /= scalar;

    REQUIRE(vector.x == doctest::Approx(8));
    REQUIRE(vector.y == doctest::Approx(18));
  }

  // Chained compound assignments.
  SUBCASE("chained_operations") {
    Vector2D vector(10, 20);
    constexpr Vector2D offset(5, 10);

    vector += offset;
    vector *= 2;
    vector -= offset;

    REQUIRE(vector.x == doctest::Approx(25));
    REQUIRE(vector.y == doctest::Approx(50));
  }
}

// sqrMagnitude, setZero, isZero, isEqual.
TEST_CASE("math/vector2d/methods") {
  // sqrMagnitude() for positive coordinates.
  SUBCASE("sqr_magnitude") {
    constexpr Vector2D vector(3, 4);

    REQUIRE(vector.sqrMagnitude() == doctest::Approx(25));

    static_assert(vector.sqrMagnitude() == 25, "sqrMagnitude of (3,4) must be 25");
  }

  // sqrMagnitude() with negative coordinates.
  SUBCASE("sqr_magnitude_with_negative_coordinates") {
    constexpr Vector2D vector(-3, -4);

    REQUIRE(vector.sqrMagnitude() == doctest::Approx(25));

    static_assert(vector.sqrMagnitude() == 25, "sqrMagnitude of (-3,-4) must be 25");
  }

  // sqrMagnitude() with zero coordinates.
  SUBCASE("sqr_magnitude_with_zero_coordinates") {
    constexpr Vector2D vector(0, 0);

    REQUIRE(vector.sqrMagnitude() == doctest::Approx(0));

    static_assert(vector.sqrMagnitude() == 0, "sqrMagnitude of origin must be 0");
  }

  // sqrMagnitude() with large coordinates.
  SUBCASE("sqr_magnitude_with_large_coordinates") {
    constexpr Vector2D vector(1000, 2000);

    REQUIRE(vector.sqrMagnitude() == doctest::Approx(5000000));

    static_assert(vector.sqrMagnitude() == 5000000, "sqrMagnitude must equal x² + y²");
  }

  // setZero() sets x, y to zero.
  SUBCASE("set_zero") {
    Vector2D vector(100, 200);

    vector.setZero();

    REQUIRE(vector.x == doctest::Approx(0));
    REQUIRE(vector.y == doctest::Approx(0));
  }

  // isZero() true for zero vector.
  SUBCASE("is_zero_with_zero_vector") {
    constexpr Vector2D vector(0, 0);

    REQUIRE(vector.isZero());

    static_assert(vector.isZero(), "origin must be zero");
  }

  // isZero() false for non-zero vector.
  SUBCASE("is_zero_with_non_zero_vector") {
    constexpr Vector2D vector(1, 0);

    REQUIRE(!vector.isZero());

    static_assert(!vector.isZero(), "non-zero vector must not be zero");
  }

  // isZero() false for negative coordinates.
  SUBCASE("is_zero_with_negative_coordinates") {
    constexpr Vector2D vector(-1, -1);

    REQUIRE(!vector.isZero());

    static_assert(!vector.isZero(), "non-zero vector must not be zero");
  }

  // isZero() true after setZero().
  SUBCASE("is_zero_after_set_zero") {
    Vector2D vector(100, 200);

    REQUIRE(!vector.isZero());

    vector.setZero();

    REQUIRE(vector.isZero());
  }

  // isEqual() with exact match.
  SUBCASE("is_equal_with_exact_match") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 20);

    REQUIRE(vector1.isEqual(vector2));
  }

  // isEqual() with different vectors.
  SUBCASE("is_equal_with_different_vectors") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(11, 20);

    REQUIRE(!vector1.isEqual(vector2));
  }

  // isEqual() with tolerance.
  SUBCASE("is_equal_with_tolerance") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(12, 18);
    constexpr real_t tolerance = 3;

    REQUIRE(vector1.isEqual(vector2, tolerance));
  }

  // isEqual() with tolerance too small.
  SUBCASE("is_equal_with_tolerance_too_small") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(15, 25);
    constexpr real_t tolerance = 2;

    REQUIRE(!vector1.isEqual(vector2, tolerance));
  }

  // isEqual() with zero tolerance.
  SUBCASE("is_equal_with_zero_tolerance") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 21);
    constexpr real_t tolerance = 0;

    REQUIRE(!vector1.isEqual(vector2, tolerance));
  }

  // isEqual() with default tolerance.
  SUBCASE("is_equal_with_default_tolerance") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 20);

    REQUIRE(vector1.isEqual(vector2));
  }

  // isEqual() with large tolerance.
  SUBCASE("is_equal_with_large_tolerance") {
    constexpr Vector2D vector1(0, 0);
    constexpr Vector2D vector2(1000, 1000);
    constexpr real_t tolerance = 2000;

    REQUIRE(vector1.isEqual(vector2, tolerance));
  }

  // Runtime vector methods.
  SUBCASE("runtime_tests") {
    Vector2D vector(10, 20);
    constexpr Vector2D offset(5, -10);

    vector += offset;
    REQUIRE(vector.x == doctest::Approx(15));
    REQUIRE(vector.y == doctest::Approx(10));

    vector *= 2;
    REQUIRE(vector.x == doctest::Approx(30));
    REQUIRE(vector.y == doctest::Approx(20));

    vector /= 4;
    REQUIRE(vector.x == doctest::Approx(7.5));
    REQUIRE(vector.y == doctest::Approx(5));

    REQUIRE(vector.sqrMagnitude() == doctest::Approx(81.25));
    REQUIRE(!vector.isZero());

    vector.setZero();
    REQUIRE(vector.isZero());
    REQUIRE(vector.sqrMagnitude() == doctest::Approx(0));

    constexpr Vector2D testVector(5, 5);
    REQUIRE(!vector.isEqual(testVector));
    REQUIRE(vector.isEqual(testVector, 10));
  }
}

// +, -, *, /, unary minus, ==.
TEST_CASE("math/vector2d/binary_operators") {
  // Unary minus negates x, y.
  SUBCASE("unary_minus_operator") {
    constexpr Vector2D vector(10, -20);
    constexpr auto result = -vector;

    REQUIRE(result.x == doctest::Approx(-10));
    REQUIRE(result.y == doctest::Approx(20));

    static_assert(result.x == -10, "unary minus must negate x");
    static_assert(result.y == 20, "unary minus must negate y");
  }

  // Unary minus with zero coordinates.
  SUBCASE("unary_minus_with_zero_coordinates") {
    constexpr Vector2D vector(0, 0);
    constexpr auto result = -vector;

    REQUIRE(result.x == doctest::Approx(0));
    REQUIRE(result.y == doctest::Approx(0));

    static_assert(result.x == 0, "unary minus of origin must remain 0");
    static_assert(result.y == 0, "unary minus of origin must remain 0");
  }

  // Unary minus with negative coordinates.
  SUBCASE("unary_minus_with_negative_coordinates") {
    constexpr Vector2D vector(-5, -15);
    constexpr auto result = -vector;

    REQUIRE(result.x == doctest::Approx(5));
    REQUIRE(result.y == doctest::Approx(15));

    static_assert(result.x == 5, "unary minus must negate x");
    static_assert(result.y == 15, "unary minus must negate y");
  }

  // operator+ adds vectors.
  SUBCASE("addition_operator") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, -10);

    constexpr auto result = vector1 + vector2;

    REQUIRE(result.x == doctest::Approx(15));
    REQUIRE(result.y == doctest::Approx(10));

    static_assert(result.x == 15, "addition x must be sum of x components");
    static_assert(result.y == 10, "addition y must be sum of y components");
  }

  // Addition with zero coordinates.
  SUBCASE("addition_with_zero_coordinates") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(0, 0);

    constexpr auto result = vector1 + vector2;

    REQUIRE(result.x == doctest::Approx(10));
    REQUIRE(result.y == doctest::Approx(20));

    static_assert(result.x == 10, "adding origin must preserve x");
    static_assert(result.y == 20, "adding origin must preserve y");
  }

  // Addition with negative coordinates.
  SUBCASE("addition_with_negative_coordinates") {
    constexpr Vector2D vector1(-10, -20);
    constexpr Vector2D vector2(-5, -15);

    constexpr auto result = vector1 + vector2;

    REQUIRE(result.x == doctest::Approx(-15));
    REQUIRE(result.y == doctest::Approx(-35));

    static_assert(result.x == -15, "addition x must be sum of x components");
    static_assert(result.y == -35, "addition y must be sum of y components");
  }

  // operator- subtracts vectors.
  SUBCASE("subtraction_operator") {
    constexpr Vector2D vector1(15, 25);
    constexpr Vector2D vector2(5, 10);

    constexpr auto result = vector1 - vector2;

    REQUIRE(result.x == doctest::Approx(10));
    REQUIRE(result.y == doctest::Approx(15));

    static_assert(result.x == 10, "subtraction x must be difference of x components");
    static_assert(result.y == 15, "subtraction y must be difference of y components");
  }

  // Subtraction with zero coordinates.
  SUBCASE("subtraction_with_zero_coordinates") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(0, 0);

    constexpr auto result = vector1 - vector2;

    REQUIRE(result.x == doctest::Approx(10));
    REQUIRE(result.y == doctest::Approx(20));

    static_assert(result.x == 10, "subtracting origin must preserve x");
    static_assert(result.y == 20, "subtracting origin must preserve y");
  }

  // Subtraction with negative coordinates.
  SUBCASE("subtraction_with_negative_coordinates") {
    constexpr Vector2D vector1(-10, -20);
    constexpr Vector2D vector2(-5, -15);

    constexpr auto result = vector1 - vector2;

    REQUIRE(result.x == doctest::Approx(-5));
    REQUIRE(result.y == doctest::Approx(-5));

    static_assert(result.x == -5, "subtraction x must be difference of x components");
    static_assert(result.y == -5, "subtraction y must be difference of y components");
  }

  // Vector * scalar.
  SUBCASE("multiplication_with_scalar_vector_times_scalar") {
    constexpr Vector2D vector(10, 20);
    constexpr real_t scalar = 2.5;

    constexpr auto result = vector * scalar;

    REQUIRE(result.x == doctest::Approx(25));
    REQUIRE(result.y == doctest::Approx(50));

    static_assert(result.x == 25, "vector * scalar must scale x");
    static_assert(result.y == 50, "vector * scalar must scale y");
  }

  // Scalar * vector.
  SUBCASE("multiplication_with_scalar_scalar_times_vector") {
    constexpr real_t scalar = 1.5;
    constexpr Vector2D vector(20, 30);

    constexpr auto result = scalar * vector;

    REQUIRE(result.x == doctest::Approx(30));
    REQUIRE(result.y == doctest::Approx(45));

    static_assert(result.x == 30, "scalar * vector must scale x");
    static_assert(result.y == 45, "scalar * vector must scale y");
  }

  // Multiplication with zero scalar.
  SUBCASE("multiplication_with_scalar_zero") {
    constexpr Vector2D vector(10, 20);
    constexpr real_t scalar = 0.0;

    constexpr auto result = vector * scalar;

    REQUIRE(result.x == doctest::Approx(0));
    REQUIRE(result.y == doctest::Approx(0));

    static_assert(result.x == 0, "vector * 0 must yield 0");
    static_assert(result.y == 0, "vector * 0 must yield 0");
  }

  // Multiplication with negative scalar.
  SUBCASE("multiplication_with_scalar_negative") {
    constexpr Vector2D vector(10, 20);
    constexpr real_t scalar = -0.5;

    constexpr auto result = vector * scalar;

    REQUIRE(result.x == doctest::Approx(-5));
    REQUIRE(result.y == doctest::Approx(-10));

    static_assert(result.x == -5, "vector * negative scalar must scale x");
    static_assert(result.y == -10, "vector * negative scalar must scale y");
  }

  // Division by scalar.
  SUBCASE("division_with_scalar") {
    constexpr Vector2D vector(25, 50);
    constexpr real_t scalar = 2.5;

    constexpr auto result = vector / scalar;

    REQUIRE(result.x == doctest::Approx(10));
    REQUIRE(result.y == doctest::Approx(20));

    static_assert(result.x == 10, "vector / scalar must divide x");
    static_assert(result.y == 20, "vector / scalar must divide y");
  }

  // Division by negative scalar.
  SUBCASE("division_with_scalar_negative") {
    constexpr Vector2D vector(-30, -60);
    constexpr real_t scalar = -1.5;

    constexpr auto result = vector / scalar;

    REQUIRE(result.x == doctest::Approx(20));
    REQUIRE(result.y == doctest::Approx(40));

    static_assert(result.x == 20, "vector / negative scalar must divide x");
    static_assert(result.y == 40, "vector / negative scalar must divide y");
  }

  // operator== with identical vectors.
  SUBCASE("equality_operator_with_identical_points") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 20);

    REQUIRE(vector1 == vector2);

    static_assert(vector1 == vector2, "identical vectors must compare equal");
  }

  // operator== with different vectors.
  SUBCASE("equality_operator_with_different_points") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(11, 20);

    REQUIRE(vector1 != vector2);

    static_assert(vector1 != vector2, "different vectors must not compare equal");
  }

  // operator== with zero coordinates.
  SUBCASE("equality_operator_with_zero_coordinates") {
    constexpr Vector2D vector1(0, 0);
    constexpr Vector2D vector2(0, 0);

    REQUIRE(vector1 == vector2);

    static_assert(vector1 == vector2, "identical origins must compare equal");
  }

  // operator== with negative coordinates.
  SUBCASE("equality_operator_with_negative_coordinates") {
    constexpr Vector2D vector1(-10, -20);
    constexpr Vector2D vector2(-10, -20);

    REQUIRE(vector1 == vector2);

    static_assert(vector1 == vector2, "identical origins must compare equal");
  }

  // operator== with mixed coordinates.
  SUBCASE("equality_operator_with_mixed_coordinates") {
    constexpr Vector2D vector1(10, -20);
    constexpr Vector2D vector2(10, -20);

    REQUIRE(vector1 == vector2);

    static_assert(vector1 == vector2, "identical origins must compare equal");
  }

  // Chained binary operations.
  SUBCASE("chained_binary_operations") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, 10);
    constexpr real_t scalar = 2;

    constexpr auto result = (vector1 + vector2) * scalar - vector1;

    REQUIRE(result.x == doctest::Approx(20));
    REQUIRE(result.y == doctest::Approx(40));

    static_assert(result.x == 20, "chained (v1+v2)*s-v1 must yield correct x");
    static_assert(result.y == 40, "chained (v1+v2)*s-v1 must yield correct y");
  }

  SUBCASE("complex_chained_operations") {
    constexpr Vector2D vector1(100, 200);
    constexpr Vector2D vector2(50, 75);
    constexpr real_t scalar1 = 1.5;
    constexpr real_t scalar2 = 2;

    constexpr auto result = (vector1 * scalar1 + vector2) / scalar2;

    REQUIRE(result.x == doctest::Approx(100));
    REQUIRE(result.y == doctest::Approx(187.5));

    static_assert(result.x == 100, "chained (v1*s1+v2)/s2 must yield correct x");
    static_assert(result.y == 187.5, "chained (v1*s1+v2)/s2 must yield correct y");
  }

  // Runtime chained binary tests.
  SUBCASE("chained_tests") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, -10);

    constexpr auto negated = -vector1;
    constexpr auto sum = negated + vector2;
    constexpr auto diff = sum - vector2;
    constexpr auto mult = diff * 2.5f;
    constexpr auto div = mult / 5;

    REQUIRE(negated.x == doctest::Approx(-10));
    REQUIRE(negated.y == doctest::Approx(-20));
    REQUIRE(sum.x == doctest::Approx(-5));
    REQUIRE(sum.y == doctest::Approx(-30));
    REQUIRE(diff.x == doctest::Approx(-10));
    REQUIRE(diff.y == doctest::Approx(-20));
    REQUIRE(mult.x == doctest::Approx(-25));
    REQUIRE(mult.y == doctest::Approx(-50));
    REQUIRE(div.x == doctest::Approx(-5));
    REQUIRE(div.y == doctest::Approx(-10));

    static_assert(negated.x == -10, "negated x must be -10");
    static_assert(negated.y == -20, "negated y must be -20");
    static_assert(sum.x == -5, "sum x must be -5");
    static_assert(sum.y == -30, "sum y must be -30");
    static_assert(diff.x == -10, "diff x must be -10");
    static_assert(diff.y == -20, "diff y must be -20");
    static_assert(mult.x == -25, "mult x must be -25");
    static_assert(mult.y == -50, "mult y must be -50");
    static_assert(div.x == -5, "div x must be -5");
    static_assert(div.y == -10, "div y must be -10");
  }
}

} // namespace toy::math
*/
