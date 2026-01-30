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

#include <catch2/catch_test_macros.hpp>

#include "math.hpp"

using namespace toy;
using namespace toy::math;

// Point has fixed size and contiguous x,y layout.
TEST_CASE("math/point/point_object_structure") {
  constexpr Point point(111, 222);

  static_assert(sizeof(point) == sizeof(int32_t) * 2, "Point must have size of two int32_t");

  const auto * arr = point.c_arr();

  REQUIRE(arr == &point.x);
  REQUIRE(arr + 1 == &point.y);
  REQUIRE(reinterpret_cast<const std::byte *>(arr + 1) - reinterpret_cast<const std::byte *>(arr) == sizeof(int32_t));
}

// Default, coordinate, and array constructors.
TEST_CASE("math/point/point_constructors") {
  SECTION("Default constructor") {
    constexpr Point point;

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    static_assert(point.x == 0, "default-constructed x must be 0");
    static_assert(point.y == 0, "default-constructed y must be 0");
  }

  SECTION("Constructor with positive coordinates") {
    constexpr Point point(10, 20);

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);

    static_assert(point.x == 10, "x must match constructor argument");
    static_assert(point.y == 20, "y must match constructor argument");
  }

  SECTION("Constructor with negative coordinates") {
    constexpr Point point(-5, -15);

    REQUIRE(point.x == -5);
    REQUIRE(point.y == -15);

    static_assert(point.x == -5, "x must match constructor argument");
    static_assert(point.y == -15, "y must match constructor argument");
  }

  SECTION("Constructor with mixed coordinates") {
    constexpr Point point(-100, 200);

    REQUIRE(point.x == -100);
    REQUIRE(point.y == 200);

    static_assert(point.x == -100, "x must match constructor argument");
    static_assert(point.y == 200, "y must match constructor argument");
  }

  SECTION("Constructor with zero coordinates") {
    constexpr Point point(0, 0);

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    static_assert(point.x == 0, "x must be 0");
    static_assert(point.y == 0, "y must be 0");
  }

  SECTION("Constructor from pointer to array") {
    constexpr array<int32_t, 2> arr{42, -17};
    constexpr Point point(arr.data());

    REQUIRE(point.x == 42);
    REQUIRE(point.y == -17);

    static_assert(point.x == 42, "x must match array element");
    static_assert(point.y == -17, "y must match array element");
  }

  SECTION("Constructor from pointer to array with positive values") {
    constexpr array<int32_t, 2> arr{100, 200};
    constexpr Point point(arr.data());

    REQUIRE(point.x == 100);
    REQUIRE(point.y == 200);

    static_assert(point.x == 100, "x must match array element");
    static_assert(point.y == 200, "y must match array element");
  }

  SECTION("Constructor from pointer to array with negative values") {
    constexpr array<int32_t, 2> arr{-50, -75};
    constexpr Point point(arr.data());

    REQUIRE(point.x == -50);
    REQUIRE(point.y == -75);

    static_assert(point.x == -50, "x must match array element");
    static_assert(point.y == -75, "y must match array element");
  }

  SECTION("Constructor from pointer to array with mixed values") {
    constexpr array<int32_t, 2> arr{-300, 400};
    constexpr Point point(arr.data());

    REQUIRE(point.x == -300);
    REQUIRE(point.y == 400);

    static_assert(point.x == -300, "x must match array element");
    static_assert(point.y == 400, "y must match array element");
  }

  SECTION("Constructor from pointer to array with zero values") {
    constexpr array<int32_t, 2> arr{0, 0};
    constexpr Point point(arr.data());

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    static_assert(point.x == 0, "x must be 0");
    static_assert(point.y == 0, "y must be 0");
  }

  SECTION("Runtime constructor tests") {
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
TEST_CASE("math/point/point_c_arr_methods") {
  SECTION("Non-const c_arr method") {
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

  SECTION("Const c_arr method") {
    constexpr Point point(123, -456);
    const auto * arr = point.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 123);
    REQUIRE(arr[1] == -456);
    REQUIRE(arr == &point.x);
    REQUIRE(point.x == 123);
    REQUIRE(point.y == -456);
  }

  SECTION("c_arr with default constructor") {
    constexpr Point point;

    REQUIRE(point.c_arr()[0] == 0);
    REQUIRE(point.c_arr()[1] == 0);

    static_assert(point.c_arr()[0] == 0, "first element must be 0 for default-constructed point");
  }

  SECTION("c_arr with coordinate constructor") {
    constexpr Point point(10, 20);

    REQUIRE(point.c_arr()[0] == 10);
    REQUIRE(point.c_arr()[1] == 20);

    static_assert(point.c_arr()[0] == 10, "first element must match x");
  }

  SECTION("c_arr with array constructor") {
    constexpr Point point({-50, 75});

    REQUIRE(point.c_arr()[0] == -50);
    REQUIRE(point.c_arr()[1] == 75);

    static_assert(point.c_arr()[0] == -50, "first element must match x");
  }

  SECTION("c_arr modification test") {
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

  SECTION("c_arr runtime tests") {
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
TEST_CASE("math/point/point_operators") {
  SECTION("operator+=") {
    Point point1(10, 20);
    constexpr Point point2(5, -10);

    point1 += point2;

    REQUIRE(point1.x == 15);
    REQUIRE(point1.y == 10);
  }

  SECTION("operator-=") {
    Point point1(15, 25);
    constexpr Point point2(5, 10);

    point1 -= point2;

    REQUIRE(point1.x == 10);
    REQUIRE(point1.y == 15);
  }

  SECTION("operator*= with integer") {
    Point point(10, 20);
    constexpr int32_t scalar = 3;

    point *= scalar;

    REQUIRE(point.x == 30);
    REQUIRE(point.y == 60);
  }

  SECTION("operator*= with real") {
    Point point(10, 20);
    constexpr real_t scalar = 2.5;

    point *= scalar;

    REQUIRE(point.x == 25);
    REQUIRE(point.y == 50);
  }

  SECTION("operator/= with integer") {
    Point point(30, 60);
    constexpr int32_t scalar = 3;

    point /= scalar;

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);
  }

  SECTION("operator/= with real") {
    Point point(20, 45);
    constexpr real_t scalar = 2.5;

    point /= scalar;

    REQUIRE(point.x == 8);
    REQUIRE(point.y == 18);
  }

  SECTION("Chained operations") {
    Point point(10, 20);
    constexpr Point offset(5, 10);

    point += offset;
    point *= 2;
    point -= offset;

    REQUIRE(point.x == 25);
    REQUIRE(point.y == 50);
  }
}

// sqrMagnitude, setZero, isZero, isEqual.
TEST_CASE("math/point/point_methods") {
  SECTION("sqrMagnitude") {
    constexpr Point point(3, 4);

    REQUIRE(point.sqrMagnitude() == 25);

    static_assert(point.sqrMagnitude() == 25, "sqrMagnitude of (3,4) must be 25");
  }

  SECTION("sqrMagnitude with negative coordinates") {
    constexpr Point point(-3, -4);

    REQUIRE(point.sqrMagnitude() == 25);

    static_assert(point.sqrMagnitude() == 25, "sqrMagnitude of (-3,-4) must be 25");
  }

  SECTION("sqrMagnitude with zero coordinates") {
    constexpr Point point(0, 0);

    REQUIRE(point.sqrMagnitude() == 0);

    static_assert(point.sqrMagnitude() == 0, "sqrMagnitude of origin must be 0");
  }

  SECTION("sqrMagnitude with large coordinates") {
    constexpr Point point(1000, 2000);

    REQUIRE(point.sqrMagnitude() == 5000000);

    static_assert(point.sqrMagnitude() == 5000000, "sqrMagnitude must equal x² + y²");
  }

  SECTION("setZero") {
    Point point(100, 200);

    point.setZero();

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);
  }

  SECTION("isZero with zero point") {
    constexpr Point point(0, 0);

    REQUIRE(point.isZero());

    static_assert(point.isZero(), "origin must be zero");
  }

  SECTION("isZero with non-zero point") {
    constexpr Point point(1, 0);

    REQUIRE(!point.isZero());

    static_assert(!point.isZero(), "non-zero vector must not be zero");
  }

  SECTION("isZero with negative coordinates") {
    constexpr Point point(-1, -1);

    REQUIRE(!point.isZero());

    static_assert(!point.isZero(), "non-zero vector must not be zero");
  }

  SECTION("isZero after setZero") {
    Point point(100, 200);

    REQUIRE(!point.isZero());

    point.setZero();

    REQUIRE(point.isZero());
  }

  SECTION("isEqual with exact match") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    REQUIRE(point1.isEqual(point2));
  }

  SECTION("isEqual with different points") {
    constexpr Point point1(10, 20);
    constexpr Point point2(11, 20);

    REQUIRE(!point1.isEqual(point2));
  }

  SECTION("isEqual with tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(12, 18);
    constexpr int32_t tolerance = 3;

    REQUIRE(point1.isEqual(point2, tolerance));
  }

  SECTION("isEqual with tolerance too small") {
    constexpr Point point1(10, 20);
    constexpr Point point2(15, 25);
    constexpr int32_t tolerance = 2;

    REQUIRE(!point1.isEqual(point2, tolerance));
  }

  SECTION("isEqual with zero tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 21);
    constexpr int32_t tolerance = 0;

    REQUIRE(!point1.isEqual(point2, tolerance));
  }

  SECTION("isEqual with default tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    REQUIRE(point1.isEqual(point2));
  }

  SECTION("isEqual with large tolerance") {
    constexpr Point point1(0, 0);
    constexpr Point point2(1000, 1000);
    constexpr int32_t tolerance = 2000;

    REQUIRE(point1.isEqual(point2, tolerance));
  }

  SECTION("Runtime tests") {
    Point point(10, 20);
    constexpr Point offset(5, -10);

    point += offset;
    REQUIRE(point.x == 15);
    REQUIRE(point.y == 10);

    point *= 2;
    REQUIRE(point.x == 30);
    REQUIRE(point.y == 20);

    point /= 3;
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
TEST_CASE("math/point/point_binary_operators") {
  SECTION("Unary minus operator") {
    constexpr Point point(10, -20);
    constexpr auto result = -point;

    REQUIRE(result.x == -10);
    REQUIRE(result.y == 20);

    static_assert(result.x == -10, "unary minus must negate x");
    static_assert(result.y == 20, "unary minus must negate y");
  }

  SECTION("Unary minus with zero coordinates") {
    constexpr Point point(0, 0);
    constexpr auto result = -point;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    static_assert(result.x == 0, "unary minus of origin must remain 0");
    static_assert(result.y == 0, "unary minus of origin must remain 0");
  }

  SECTION("Unary minus with negative coordinates") {
    constexpr Point point(-5, -15);
    constexpr auto result = -point;

    REQUIRE(result.x == 5);
    REQUIRE(result.y == 15);

    static_assert(result.x == 5, "unary minus must negate x");
    static_assert(result.y == 15, "unary minus must negate y");
  }

  SECTION("Addition operator") {
    constexpr Point point1(10, 20);
    constexpr Point point2(5, -10);

    constexpr auto result = point1 + point2;

    REQUIRE(result.x == 15);
    REQUIRE(result.y == 10);

    static_assert(result.x == 15, "addition x must be sum of x components");
    static_assert(result.y == 10, "addition y must be sum of y components");
  }

  SECTION("Addition with zero coordinates") {
    constexpr Point point1(10, 20);
    constexpr Point point2(0, 0);

    constexpr auto result = point1 + point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "adding origin must preserve x");
    static_assert(result.y == 20, "adding origin must preserve y");
  }

  SECTION("Addition with negative coordinates") {
    constexpr Point point1(-10, -20);
    constexpr Point point2(-5, -15);

    constexpr auto result = point1 + point2;

    REQUIRE(result.x == -15);
    REQUIRE(result.y == -35);

    static_assert(result.x == -15, "addition x must be sum of x components");
    static_assert(result.y == -35, "addition y must be sum of y components");
  }

  SECTION("Subtraction operator") {
    constexpr Point point1(15, 25);
    constexpr Point point2(5, 10);

    constexpr auto result = point1 - point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 15);

    static_assert(result.x == 10, "subtraction x must be difference of x components");
    static_assert(result.y == 15, "subtraction y must be difference of y components");
  }

  SECTION("Subtraction with zero coordinates") {
    constexpr Point point1(10, 20);
    constexpr Point point2(0, 0);

    constexpr auto result = point1 - point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "subtracting origin must preserve x");
    static_assert(result.y == 20, "subtracting origin must preserve y");
  }

  SECTION("Subtraction with negative coordinates") {
    constexpr Point point1(-10, -20);
    constexpr Point point2(-5, -15);

    constexpr auto result = point1 - point2;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -5);

    static_assert(result.x == -5, "subtraction x must be difference of x components");
    static_assert(result.y == -5, "subtraction y must be difference of y components");
  }

  SECTION("Multiplication with integer scalar (point * scalar)") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = 3;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == 30);
    REQUIRE(result.y == 60);

    static_assert(result.x == 30, "point * scalar must scale x");
    static_assert(result.y == 60, "point * scalar must scale y");
  }

  SECTION("Multiplication with integer scalar (scalar * point)") {
    constexpr int32_t scalar = 4;
    constexpr Point point(5, 15);

    constexpr auto result = scalar * point;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 60);

    static_assert(result.x == 20, "scalar * point must scale x");
    static_assert(result.y == 60, "scalar * point must scale y");
  }

  SECTION("Multiplication with zero scalar") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = 0;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    static_assert(result.x == 0, "point * 0 must yield 0");
    static_assert(result.y == 0, "point * 0 must yield 0");
  }

  SECTION("Multiplication with negative scalar") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = -2;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == -20);
    REQUIRE(result.y == -40);

    static_assert(result.x == -20, "point * negative scalar must negate and scale x");
    static_assert(result.y == -40, "point * negative scalar must negate and scale y");
  }

  SECTION("Multiplication with real scalar (point * real)") {
    constexpr Point point(10, 20);
    constexpr real_t scalar = 2.5;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == 25);
    REQUIRE(result.y == 50);

    static_assert(result.x == 25, "point * real must scale x");
    static_assert(result.y == 50, "point * real must scale y");
  }

  SECTION("Multiplication with real scalar (real * point)") {
    constexpr real_t scalar = 1.5;
    constexpr Point point(20, 30);

    constexpr auto result = scalar * point;

    REQUIRE(result.x == 30);
    REQUIRE(result.y == 45);

    static_assert(result.x == 30, "real * point must scale x");
    static_assert(result.y == 45, "real * point must scale y");
  }

  SECTION("Multiplication with real scalar zero") {
    constexpr Point point(10, 20);
    constexpr real_t scalar = 0.0;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    static_assert(result.x == 0, "point * 0 must yield 0");
    static_assert(result.y == 0, "point * 0 must yield 0");
  }

  SECTION("Multiplication with real scalar negative") {
    constexpr Point point(10, 20);
    constexpr real_t scalar = -0.5;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -10);

    static_assert(result.x == -5, "point * negative real must scale x");
    static_assert(result.y == -10, "point * negative real must scale y");
  }

  SECTION("Division with integer scalar") {
    constexpr Point point(30, 60);
    constexpr int32_t scalar = 3;

    constexpr auto result = point / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "point / scalar must divide x");
    static_assert(result.y == 20, "point / scalar must divide y");
  }

  SECTION("Division with negative scalar") {
    constexpr Point point(-20, -40);
    constexpr int32_t scalar = -2;

    constexpr auto result = point / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "point / negative scalar must divide x");
    static_assert(result.y == 20, "point / negative scalar must divide y");
  }

  SECTION("Division with real scalar") {
    constexpr Point point(25, 50);
    constexpr real_t scalar = 2.5;

    constexpr auto result = point / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "point / real must divide x");
    static_assert(result.y == 20, "point / real must divide y");
  }

  SECTION("Division with real scalar negative") {
    constexpr Point point(-30, -60);
    constexpr real_t scalar = -1.5;

    constexpr auto result = point / scalar;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 40);

    static_assert(result.x == 20, "point / negative real must divide x");
    static_assert(result.y == 40, "point / negative real must divide y");
  }

  SECTION("Equality operator with identical points") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    REQUIRE(point1 == point2);

    static_assert(point1 == point2, "identical points must compare equal");
  }

  SECTION("Equality operator with different points") {
    constexpr Point point1(10, 20);
    constexpr Point point2(11, 20);

    REQUIRE(!(point1 == point2));

    static_assert(!(point1 == point2), "different points must not compare equal");
  }

  SECTION("Equality operator with zero coordinates") {
    constexpr Point point1(0, 0);
    constexpr Point point2(0, 0);

    REQUIRE(point1 == point2);

    static_assert(point1 == point2, "identical origins must compare equal");
  }

  SECTION("Equality operator with negative coordinates") {
    constexpr Point point1(-10, -20);
    constexpr Point point2(-10, -20);

    REQUIRE(point1 == point2);

    static_assert(point1 == point2, "identical points must compare equal");
  }

  SECTION("Equality operator with mixed coordinates") {
    constexpr Point point1(10, -20);
    constexpr Point point2(10, -20);

    REQUIRE(point1 == point2);

    static_assert(point1 == point2, "identical points must compare equal");
  }

  SECTION("Chained binary operations") {
    constexpr Point point1(10, 20);
    constexpr Point point2(5, 10);
    constexpr int32_t scalar = 2;

    constexpr auto result = (point1 + point2) * scalar - point1;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 40);

    static_assert(result.x == 20, "chained (p1+p2)*s-p1 must yield correct x");
    static_assert(result.y == 40, "chained (p1+p2)*s-p1 must yield correct y");
  }

  SECTION("Complex chained operations") {
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

  SECTION("Chained tests") {
    constexpr Point point1(10, 20);
    constexpr Point point2(5, -10);

    constexpr auto negated = -point1;
    constexpr auto sum = negated + point2;
    constexpr auto diff = sum - point2;
    constexpr auto multInt = diff * 3;
    constexpr auto multReal = multInt * 2.5f;
    constexpr auto divInt = multReal / 3;
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
