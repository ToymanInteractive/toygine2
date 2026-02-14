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

// Point has fixed size and contiguous x,y layout.
TEST_CASE("math/point/object_structure") {
  constexpr Point point(111, 222);

  static_assert(sizeof(point) == sizeof(int32_t) * 2, "Point must have size of two int32_t");

  const auto * arr = point.c_arr();

  REQUIRE(arr == &point.x);
  REQUIRE(arr + 1 == &point.y);
  REQUIRE(reinterpret_cast<const std::byte *>(arr + 1) - reinterpret_cast<const std::byte *>(arr) == sizeof(int32_t));
}

// Default, coordinate, and array constructors.
TEST_CASE("math/point/constructors") {
  // Default constructor yields zero x, y.
  SUBCASE("default_constructor") {
    constexpr Point point;

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    static_assert(point.x == 0, "default-constructed x must be 0");
    static_assert(point.y == 0, "default-constructed y must be 0");
  }

  // Constructor with positive x, y.
  SUBCASE("constructor_with_positive_coordinates") {
    constexpr Point point(10, 20);

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);

    static_assert(point.x == 10, "x must match constructor argument");
    static_assert(point.y == 20, "y must match constructor argument");
  }

  // Constructor with negative x, y.
  SUBCASE("constructor_with_negative_coordinates") {
    constexpr Point point(-5, -15);

    REQUIRE(point.x == -5);
    REQUIRE(point.y == -15);

    static_assert(point.x == -5, "x must match constructor argument");
    static_assert(point.y == -15, "y must match constructor argument");
  }

  // Constructor with mixed-sign x, y.
  SUBCASE("constructor_with_mixed_coordinates") {
    constexpr Point point(-100, 200);

    REQUIRE(point.x == -100);
    REQUIRE(point.y == 200);

    static_assert(point.x == -100, "x must match constructor argument");
    static_assert(point.y == 200, "y must match constructor argument");
  }

  // Constructor with zero x, y.
  SUBCASE("constructor_with_zero_coordinates") {
    constexpr Point point(0, 0);

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    static_assert(point.x == 0, "x must be 0");
    static_assert(point.y == 0, "y must be 0");
  }

  // Constructor from pointer to two-element array.
  SUBCASE("constructor_from_pointer_to_array") {
    constexpr array<int32_t, 2> arr{42, -17};
    constexpr Point point(arr.data());

    REQUIRE(point.x == 42);
    REQUIRE(point.y == -17);

    static_assert(point.x == 42, "x must match array element");
    static_assert(point.y == -17, "y must match array element");
  }

  // Constructor from array with positive values.
  SUBCASE("constructor_from_pointer_to_array_with_positive_values") {
    constexpr array<int32_t, 2> arr{100, 200};
    constexpr Point point(arr.data());

    REQUIRE(point.x == 100);
    REQUIRE(point.y == 200);

    static_assert(point.x == 100, "x must match array element");
    static_assert(point.y == 200, "y must match array element");
  }

  // Constructor from array with negative values.
  SUBCASE("constructor_from_pointer_to_array_with_negative_values") {
    constexpr array<int32_t, 2> arr{-50, -75};
    constexpr Point point(arr.data());

    REQUIRE(point.x == -50);
    REQUIRE(point.y == -75);

    static_assert(point.x == -50, "x must match array element");
    static_assert(point.y == -75, "y must match array element");
  }

  // Constructor from array with mixed-sign values.
  SUBCASE("constructor_from_pointer_to_array_with_mixed_values") {
    constexpr array<int32_t, 2> arr{-300, 400};
    constexpr Point point(arr.data());

    REQUIRE(point.x == -300);
    REQUIRE(point.y == 400);

    static_assert(point.x == -300, "x must match array element");
    static_assert(point.y == 400, "y must match array element");
  }

  // Constructor from array with zero values.
  SUBCASE("constructor_from_pointer_to_array_with_zero_values") {
    constexpr array<int32_t, 2> arr{0, 0};
    constexpr Point point(arr.data());

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    static_assert(point.x == 0, "x must be 0");
    static_assert(point.y == 0, "y must be 0");
  }

  // Runtime constructor behavior.
  SUBCASE("runtime_constructor_tests") {
    Point defaultPoint;
    REQUIRE(defaultPoint.x == 0);
    REQUIRE(defaultPoint.y == 0);

    Point coordPoint(123, -456);
    REQUIRE(coordPoint.x == 123);
    REQUIRE(coordPoint.y == -456);

    constexpr array<int32_t, 2> arr{789, -321};
    Point arrayPoint(arr.data());
    REQUIRE(arrayPoint.x == 789);
    REQUIRE(arrayPoint.y == -321);
  }
}

// c_arr returns pointer to contiguous x,y.
TEST_CASE("math/point/c_arr_methods") {
  // Non-const c_arr() returns writable pointer to x,y.
  SUBCASE("non_const_c_arr_method") {
    Point point(42, -17);
    auto * arr = point.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 42);
    REQUIRE(arr[1] == -17);
    REQUIRE(arr == &point.x);

    arr[0] = 100;
    arr[1] = -200;

    REQUIRE(point.x == 100);
    REQUIRE(point.y == -200);
  }

  // Const c_arr() returns pointer to x,y.
  SUBCASE("const_c_arr_method") {
    constexpr Point point(123, -456);
    const auto * arr = point.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 123);
    REQUIRE(arr[1] == -456);
    REQUIRE(arr == &point.x);
    REQUIRE(point.x == 123);
    REQUIRE(point.y == -456);
  }

  // c_arr() with default-constructed point.
  SUBCASE("c_arr_with_default_constructor") {
    constexpr Point point;

    REQUIRE(point.c_arr()[0] == 0);
    REQUIRE(point.c_arr()[1] == 0);

    static_assert(point.c_arr()[0] == 0, "first element must be 0 for default-constructed point");
  }

  // c_arr() with coordinate constructor.
  SUBCASE("c_arr_with_coordinate_constructor") {
    constexpr Point point(10, 20);

    REQUIRE(point.c_arr()[0] == 10);
    REQUIRE(point.c_arr()[1] == 20);

    static_assert(point.c_arr()[0] == 10, "first element must match x");
  }

  // c_arr() with array constructor.
  SUBCASE("c_arr_with_array_constructor") {
    constexpr Point point({-50, 75});

    REQUIRE(point.c_arr()[0] == -50);
    REQUIRE(point.c_arr()[1] == 75);

    static_assert(point.c_arr()[0] == -50, "first element must match x");
  }

  // Modifying via c_arr() updates point.
  SUBCASE("c_arr_modification_test") {
    Point point(1, 2);
    auto * arr = point.c_arr();

    arr[0] = 10;
    arr[1] = 20;

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);

    *arr = 30;
    *(arr + 1) = 40;

    REQUIRE(point.x == 30);
    REQUIRE(point.y == 40);

    arr[0] = 50;
    arr[1] = 60;

    REQUIRE(point.x == 50);
    REQUIRE(point.y == 60);
  }

  // Runtime c_arr() behavior.
  SUBCASE("c_arr_runtime_tests") {
    Point runtimePoint(500, -600);
    auto * runtimeArr = runtimePoint.c_arr();

    REQUIRE(runtimeArr != nullptr);
    REQUIRE(runtimeArr[0] == 500);
    REQUIRE(runtimeArr[1] == -600);

    runtimeArr[0] = 700;
    runtimeArr[1] = -800;

    REQUIRE(runtimePoint.x == 700);
    REQUIRE(runtimePoint.y == -800);

    const Point constRuntimePoint(900, -1000);
    const auto * constRuntimeArr = constRuntimePoint.c_arr();

    REQUIRE(constRuntimeArr != nullptr);
    REQUIRE(constRuntimeArr[0] == 900);
    REQUIRE(constRuntimeArr[1] == -1000);
  }
}

// +=, -=, *=, /= and chaining.
TEST_CASE("math/point/operators") {
  // operator+= adds vector.
  SUBCASE("operator_plus_assign") {
    Point point1(10, 20);
    constexpr Point point2(5, -10);

    point1 += point2;

    REQUIRE(point1.x == 15);
    REQUIRE(point1.y == 10);
  }

  // operator-= subtracts vector.
  SUBCASE("operator_minus_assign") {
    Point point1(15, 25);
    constexpr Point point2(5, 10);

    point1 -= point2;

    REQUIRE(point1.x == 10);
    REQUIRE(point1.y == 15);
  }

  // operator*= with integer scalar.
  SUBCASE("operator_times_assign_with_integer") {
    Point point(10, 20);
    constexpr int32_t scalar = 3;

    point *= scalar;

    REQUIRE(point.x == 30);
    REQUIRE(point.y == 60);
  }

  // operator*= with real scalar.
  SUBCASE("operator_times_assign_with_real") {
    Point point(10, 20);
    constexpr real_t scalar = 2.5;

    point *= scalar;

    REQUIRE(point.x == 25);
    REQUIRE(point.y == 50);
  }

  // operator/= with integer scalar.
  SUBCASE("operator_div_assign_with_integer") {
    Point point(30, 60);
    constexpr int32_t scalar = 3;

    point /= scalar;

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);
  }

  // operator/= with real scalar.
  SUBCASE("operator_div_assign_with_real") {
    Point point(20, 45);
    constexpr real_t scalar = 2.5;

    point /= scalar;

    REQUIRE(point.x == 8);
    REQUIRE(point.y == 18);
  }

  // Chained compound assignments.
  SUBCASE("chained_operations") {
    Point point(10, 20);
    constexpr Point offset(5, 10);

    point += offset;
    point *= int32_t(2);
    point -= offset;

    REQUIRE(point.x == 25);
    REQUIRE(point.y == 50);
  }
}

// sqrMagnitude, setZero, isZero, isEqual.
TEST_CASE("math/point/point_methods") {
  // sqrMagnitude() for positive coordinates.
  SUBCASE("sqr_magnitude") {
    constexpr Point point(3, 4);

    REQUIRE(point.sqrMagnitude() == 25);

    static_assert(point.sqrMagnitude() == 25, "sqrMagnitude of (3,4) must be 25");
  }

  // sqrMagnitude() with negative coordinates.
  SUBCASE("sqr_magnitude_with_negative_coordinates") {
    constexpr Point point(-3, -4);

    REQUIRE(point.sqrMagnitude() == 25);

    static_assert(point.sqrMagnitude() == 25, "sqrMagnitude of (-3,-4) must be 25");
  }

  // sqrMagnitude() with zero coordinates.
  SUBCASE("sqr_magnitude_with_zero_coordinates") {
    constexpr Point point(0, 0);

    REQUIRE(point.sqrMagnitude() == 0);

    static_assert(point.sqrMagnitude() == 0, "sqrMagnitude of origin must be 0");
  }

  // sqrMagnitude() with large coordinates.
  SUBCASE("sqr_magnitude_with_large_coordinates") {
    constexpr Point point(1000, 2000);

    REQUIRE(point.sqrMagnitude() == 5000000);

    static_assert(point.sqrMagnitude() == 5000000, "sqrMagnitude must equal x² + y²");
  }

  // setZero() sets x, y to zero.
  SUBCASE("set_zero") {
    Point point(100, 200);

    point.setZero();

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);
  }

  // isZero() true for (0,0).
  SUBCASE("is_zero_with_zero_point") {
    constexpr Point point(0, 0);

    REQUIRE(point.isZero());

    static_assert(point.isZero(), "origin must be zero");
  }

  // isZero() false for non-zero point.
  SUBCASE("is_zero_with_non_zero_point") {
    constexpr Point point(1, 0);

    REQUIRE(!point.isZero());

    static_assert(!point.isZero(), "non-zero vector must not be zero");
  }

  // isZero() false for negative coordinates.
  SUBCASE("is_zero_with_negative_coordinates") {
    constexpr Point point(-1, -1);

    REQUIRE(!point.isZero());

    static_assert(!point.isZero(), "non-zero vector must not be zero");
  }

  // isZero() true after setZero().
  SUBCASE("is_zero_after_set_zero") {
    Point point(100, 200);

    REQUIRE(!point.isZero());

    point.setZero();

    REQUIRE(point.isZero());
  }

  // isEqual() with exact match.
  SUBCASE("is_equal_with_exact_match") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    REQUIRE(point1.isEqual(point2));
  }

  // isEqual() with different points.
  SUBCASE("is_equal_with_different_points") {
    constexpr Point point1(10, 20);
    constexpr Point point2(11, 20);

    REQUIRE(!point1.isEqual(point2));
  }

  // isEqual() with tolerance.
  SUBCASE("is_equal_with_tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(12, 18);
    constexpr int32_t tolerance = 3;

    REQUIRE(point1.isEqual(point2, tolerance));
  }

  // isEqual() with tolerance too small.
  SUBCASE("is_equal_with_tolerance_too_small") {
    constexpr Point point1(10, 20);
    constexpr Point point2(15, 25);
    constexpr int32_t tolerance = 2;

    REQUIRE(!point1.isEqual(point2, tolerance));
  }

  // isEqual() with zero tolerance.
  SUBCASE("is_equal_with_zero_tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 21);
    constexpr int32_t tolerance = 0;

    REQUIRE(!point1.isEqual(point2, tolerance));
  }

  // isEqual() with default tolerance.
  SUBCASE("is_equal_with_default_tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    REQUIRE(point1.isEqual(point2));
  }

  // isEqual() with large tolerance.
  SUBCASE("is_equal_with_large_tolerance") {
    constexpr Point point1(0, 0);
    constexpr Point point2(1000, 1000);
    constexpr int32_t tolerance = 2000;

    REQUIRE(point1.isEqual(point2, tolerance));
  }

  // Runtime point methods.
  SUBCASE("runtime_tests") {
    Point point(10, 20);
    constexpr Point offset(5, -10);

    point += offset;
    REQUIRE(point.x == 15);
    REQUIRE(point.y == 10);

    point *= int32_t(2);
    REQUIRE(point.x == 30);
    REQUIRE(point.y == 20);

    point /= int32_t(3);
    REQUIRE(point.x == 10);
    REQUIRE(point.y == 6);

    REQUIRE(point.sqrMagnitude() == 136);
    REQUIRE(!point.isZero());

    point.setZero();
    REQUIRE(point.isZero());
    REQUIRE(point.sqrMagnitude() == 0);

    constexpr Point testPoint(5, 5);
    REQUIRE(point.isEqual(testPoint) == false);
    REQUIRE(point.isEqual(testPoint, 10));
  }
}

// +, -, *, /, unary minus, ==.
TEST_CASE("math/point/binary_operators") {
  // Unary minus negates x, y.
  SUBCASE("unary_minus_operator") {
    constexpr Point point(10, -20);
    constexpr auto result = -point;

    REQUIRE(result.x == -10);
    REQUIRE(result.y == 20);

    static_assert(result.x == -10, "unary minus must negate x");
    static_assert(result.y == 20, "unary minus must negate y");
  }

  // Unary minus with zero coordinates.
  SUBCASE("unary_minus_with_zero_coordinates") {
    constexpr Point point(0, 0);
    constexpr auto result = -point;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    static_assert(result.x == 0, "unary minus of origin must remain 0");
    static_assert(result.y == 0, "unary minus of origin must remain 0");
  }

  // Unary minus with negative coordinates.
  SUBCASE("unary_minus_with_negative_coordinates") {
    constexpr Point point(-5, -15);
    constexpr auto result = -point;

    REQUIRE(result.x == 5);
    REQUIRE(result.y == 15);

    static_assert(result.x == 5, "unary minus must negate x");
    static_assert(result.y == 15, "unary minus must negate y");
  }

  // operator+ adds vectors.
  SUBCASE("addition_operator") {
    constexpr Point point1(10, 20);
    constexpr Point point2(5, -10);

    constexpr auto result = point1 + point2;

    REQUIRE(result.x == 15);
    REQUIRE(result.y == 10);

    static_assert(result.x == 15, "addition x must be sum of x components");
    static_assert(result.y == 10, "addition y must be sum of y components");
  }

  // Addition with zero coordinates.
  SUBCASE("addition_with_zero_coordinates") {
    constexpr Point point1(10, 20);
    constexpr Point point2(0, 0);

    constexpr auto result = point1 + point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "adding origin must preserve x");
    static_assert(result.y == 20, "adding origin must preserve y");
  }

  // Addition with negative coordinates.
  SUBCASE("addition_with_negative_coordinates") {
    constexpr Point point1(-10, -20);
    constexpr Point point2(-5, -15);

    constexpr auto result = point1 + point2;

    REQUIRE(result.x == -15);
    REQUIRE(result.y == -35);

    static_assert(result.x == -15, "addition x must be sum of x components");
    static_assert(result.y == -35, "addition y must be sum of y components");
  }

  // operator- subtracts vectors.
  SUBCASE("subtraction_operator") {
    constexpr Point point1(15, 25);
    constexpr Point point2(5, 10);

    constexpr auto result = point1 - point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 15);

    static_assert(result.x == 10, "subtraction x must be difference of x components");
    static_assert(result.y == 15, "subtraction y must be difference of y components");
  }

  // Subtraction with zero coordinates.
  SUBCASE("subtraction_with_zero_coordinates") {
    constexpr Point point1(10, 20);
    constexpr Point point2(0, 0);

    constexpr auto result = point1 - point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "subtracting origin must preserve x");
    static_assert(result.y == 20, "subtracting origin must preserve y");
  }

  // Subtraction with negative coordinates.
  SUBCASE("subtraction_with_negative_coordinates") {
    constexpr Point point1(-10, -20);
    constexpr Point point2(-5, -15);

    constexpr auto result = point1 - point2;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -5);

    static_assert(result.x == -5, "subtraction x must be difference of x components");
    static_assert(result.y == -5, "subtraction y must be difference of y components");
  }

  // Point * integer scalar.
  SUBCASE("multiplication_with_integer_scalar_point_times_scalar") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = 3;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == 30);
    REQUIRE(result.y == 60);

    static_assert(result.x == 30, "point * scalar must scale x");
    static_assert(result.y == 60, "point * scalar must scale y");
  }

  // Integer scalar * point.
  SUBCASE("multiplication_with_integer_scalar_scalar_times_point") {
    constexpr int32_t scalar = 4;
    constexpr Point point(5, 15);

    constexpr auto result = scalar * point;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 60);

    static_assert(result.x == 20, "scalar * point must scale x");
    static_assert(result.y == 60, "scalar * point must scale y");
  }

  // Multiplication with zero scalar.
  SUBCASE("multiplication_with_zero_scalar") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = 0;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    static_assert(result.x == 0, "point * 0 must yield 0");
    static_assert(result.y == 0, "point * 0 must yield 0");
  }

  // Multiplication with negative scalar.
  SUBCASE("multiplication_with_negative_scalar") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = -2;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == -20);
    REQUIRE(result.y == -40);

    static_assert(result.x == -20, "point * negative scalar must negate and scale x");
    static_assert(result.y == -40, "point * negative scalar must negate and scale y");
  }

  // Point * real scalar.
  SUBCASE("multiplication_with_real_scalar_point_times_real") {
    constexpr Point point(10, 20);
    constexpr real_t scalar = 2.5;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == 25);
    REQUIRE(result.y == 50);

    static_assert(result.x == 25, "point * real must scale x");
    static_assert(result.y == 50, "point * real must scale y");
  }

  // Real scalar * point.
  SUBCASE("multiplication_with_real_scalar_real_times_point") {
    constexpr real_t scalar = 1.5;
    constexpr Point point(20, 30);

    constexpr auto result = scalar * point;

    REQUIRE(result.x == 30);
    REQUIRE(result.y == 45);

    static_assert(result.x == 30, "real * point must scale x");
    static_assert(result.y == 45, "real * point must scale y");
  }

  // Real scalar zero.
  SUBCASE("multiplication_with_real_scalar_zero") {
    constexpr Point point(10, 20);
    constexpr real_t scalar = 0.0;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    static_assert(result.x == 0, "point * 0 must yield 0");
    static_assert(result.y == 0, "point * 0 must yield 0");
  }

  // Real scalar negative.
  SUBCASE("multiplication_with_real_scalar_negative") {
    constexpr Point point(10, 20);
    constexpr real_t scalar = -0.5;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -10);

    static_assert(result.x == -5, "point * negative real must scale x");
    static_assert(result.y == -10, "point * negative real must scale y");
  }

  // Division by integer scalar.
  SUBCASE("division_with_integer_scalar") {
    constexpr Point point(30, 60);
    constexpr int32_t scalar = 3;

    constexpr auto result = point / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "point / scalar must divide x");
    static_assert(result.y == 20, "point / scalar must divide y");
  }

  // Division by negative scalar.
  SUBCASE("division_with_negative_scalar") {
    constexpr Point point(-20, -40);
    constexpr int32_t scalar = -2;

    constexpr auto result = point / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "point / negative scalar must divide x");
    static_assert(result.y == 20, "point / negative scalar must divide y");
  }

  // Division by real scalar.
  SUBCASE("division_with_real_scalar") {
    constexpr Point point(25, 50);
    constexpr real_t scalar = 2.5;

    constexpr auto result = point / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "point / real must divide x");
    static_assert(result.y == 20, "point / real must divide y");
  }

  // Division by negative real scalar.
  SUBCASE("division_with_real_scalar_negative") {
    constexpr Point point(-30, -60);
    constexpr real_t scalar = -1.5;

    constexpr auto result = point / scalar;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 40);

    static_assert(result.x == 20, "point / negative real must divide x");
    static_assert(result.y == 40, "point / negative real must divide y");
  }

  // operator== with identical points.
  SUBCASE("equality_operator_with_identical_points") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    REQUIRE(point1 == point2);

    static_assert(point1 == point2, "identical points must compare equal");
  }

  // operator== with different points.
  SUBCASE("equality_operator_with_different_points") {
    constexpr Point point1(10, 20);
    constexpr Point point2(11, 20);

    REQUIRE(point1 != point2);

    static_assert(point1 != point2, "different points must not compare equal");
  }

  // operator== with zero coordinates.
  SUBCASE("equality_operator_with_zero_coordinates") {
    constexpr Point point1(0, 0);
    constexpr Point point2(0, 0);

    REQUIRE(point1 == point2);

    static_assert(point1 == point2, "identical origins must compare equal");
  }

  // operator== with negative coordinates.
  SUBCASE("equality_operator_with_negative_coordinates") {
    constexpr Point point1(-10, -20);
    constexpr Point point2(-10, -20);

    REQUIRE(point1 == point2);

    static_assert(point1 == point2, "identical points must compare equal");
  }

  // operator== with mixed coordinates.
  SUBCASE("equality_operator_with_mixed_coordinates") {
    constexpr Point point1(10, -20);
    constexpr Point point2(10, -20);

    REQUIRE(point1 == point2);

    static_assert(point1 == point2, "identical points must compare equal");
  }

  // Chained binary operations.
  SUBCASE("chained_binary_operations") {
    constexpr Point point1(10, 20);
    constexpr Point point2(5, 10);
    constexpr int32_t scalar = 2;

    constexpr auto result = (point1 + point2) * scalar - point1;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 40);

    static_assert(result.x == 20, "chained (p1+p2)*s-p1 must yield correct x");
    static_assert(result.y == 40, "chained (p1+p2)*s-p1 must yield correct y");
  }

  // Complex chained binary operations.
  SUBCASE("complex_chained_operations") {
    constexpr Point point1(100, 200);
    constexpr Point point2(50, 75);
    constexpr real_t scalar1 = 1.5;
    constexpr int32_t scalar2 = 2;

    constexpr auto result = (point1 * scalar1 + point2) / scalar2;

    REQUIRE(result.x == 100);
    REQUIRE(result.y == 187);

    static_assert(result.x == 100, "chained (p1*s1+p2)/s2 must yield correct x");
    static_assert(result.y == 187, "chained (p1*s1+p2)/s2 must yield correct y");
  }

  // Runtime chained binary tests.
  SUBCASE("chained_tests") {
    constexpr Point point1(10, 20);
    constexpr Point point2(5, -10);

    constexpr auto negated = -point1;
    constexpr auto sum = negated + point2;
    constexpr auto diff = sum - point2;
    constexpr auto multInt = diff * int32_t(3);
    constexpr auto multReal = multInt * 2.5f;
    constexpr auto divInt = multReal / int32_t(3);
    constexpr auto divReal = divInt / 2.5f;

    REQUIRE(negated.x == -10);
    REQUIRE(negated.y == -20);
    REQUIRE(sum.x == -5);
    REQUIRE(sum.y == -30);
    REQUIRE(diff.x == -10);
    REQUIRE(diff.y == -20);
    REQUIRE(multInt.x == -30);
    REQUIRE(multInt.y == -60);
    REQUIRE(multReal.x == -75);
    REQUIRE(multReal.y == -150);
    REQUIRE(divInt.x == -25);
    REQUIRE(divInt.y == -50);
    REQUIRE(divReal.x == -10);
    REQUIRE(divReal.y == -20);

    static_assert(negated.x == -10, "negated x must be -10");
    static_assert(negated.y == -20, "negated y must be -20");
    static_assert(sum.x == -5, "sum x must be -5");
    static_assert(sum.y == -30, "sum y must be -30");
    static_assert(diff.x == -10, "diff x must be -10");
    static_assert(diff.y == -20, "diff y must be -20");
    static_assert(multInt.x == -30, "multInt x must be -30");
    static_assert(multInt.y == -60, "multInt y must be -60");
    static_assert(multReal.x == -75, "multReal x must be -75");
    static_assert(multReal.y == -150, "multReal y must be -150");
    static_assert(divInt.x == -25, "divInt x must be -25");
    static_assert(divInt.y == -50, "divInt y must be -50");
    static_assert(divReal.x == -10, "divReal x must be -10");
    static_assert(divReal.y == -20, "divReal y must be -20");
  }
}

} // namespace toy::math
*/
