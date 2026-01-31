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

// Vector2D has fixed size and contiguous x,y layout.
TEST_CASE("math/vector2d/vector2d_object_structure") {
  constexpr Vector2D vector(111, 222);

  static_assert(sizeof(vector) == sizeof(real_t) * 2, "Vector2D must have size of two real_t");

  const auto * arr = vector.c_arr();

  REQUIRE(arr == &vector.x);
  REQUIRE(arr + 1 == &vector.y);
  REQUIRE(reinterpret_cast<const std::byte *>(arr + 1) - reinterpret_cast<const std::byte *>(arr) == sizeof(real_t));
}

// Default, coordinate, and array constructors.
TEST_CASE("math/vector2d/vector2d_constructors") {
  SECTION("Default constructor") {
    constexpr Vector2D vector;

    REQUIRE(vector.x == 0.0);
    REQUIRE(vector.y == 0.0);

    static_assert(vector.x == 0.0, "default-constructed x must be 0");
    static_assert(vector.y == 0.0, "default-constructed y must be 0");
  }

  SECTION("Constructor with positive coordinates") {
    constexpr Vector2D vector(12, 23);

    REQUIRE(vector.x == 12);
    REQUIRE(vector.y == 23);

    static_assert(vector.x == 12, "x must match constructor argument");
    static_assert(vector.y == 23, "y must match constructor argument");
  }

  SECTION("Constructor with negative coordinates") {
    constexpr Vector2D vector(-5, -15);

    REQUIRE(vector.x == -5);
    REQUIRE(vector.y == -15);

    static_assert(vector.x == -5, "x must match constructor argument");
    static_assert(vector.y == -15, "y must match constructor argument");
  }

  SECTION("Constructor with mixed coordinates") {
    constexpr Vector2D vector(-100, 200);

    REQUIRE(vector.x == -100);
    REQUIRE(vector.y == 200);

    static_assert(vector.x == -100, "x must match constructor argument");
    static_assert(vector.y == 200, "y must match constructor argument");
  }

  SECTION("Constructor with zero coordinates") {
    constexpr Vector2D vector(0, 0);

    REQUIRE(vector.x == 0);
    REQUIRE(vector.y == 0);

    static_assert(vector.x == 0, "x must be 0");
    static_assert(vector.y == 0, "y must be 0");
  }

  SECTION("Constructor from pointer to array") {
    constexpr array<real_t, 2> arr{42, -17};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == 42);
    REQUIRE(vector.y == -17);

    static_assert(vector.x == 42, "x must match array element");
    static_assert(vector.y == -17, "y must match array element");
  }

  SECTION("Constructor from pointer to array with positive values") {
    constexpr array<real_t, 2> arr{100, 200};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == 100);
    REQUIRE(vector.y == 200);

    static_assert(vector.x == 100, "x must match array element");
    static_assert(vector.y == 200, "y must match array element");
  }

  SECTION("Constructor from pointer to array with negative values") {
    constexpr array<real_t, 2> arr{-50, -75};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == -50);
    REQUIRE(vector.y == -75);

    static_assert(vector.x == -50, "x must match array element");
    static_assert(vector.y == -75, "y must match array element");
  }

  SECTION("Constructor from pointer to array with mixed values") {
    constexpr array<real_t, 2> arr{-300, 400};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == -300);
    REQUIRE(vector.y == 400);

    static_assert(vector.x == -300, "x must match array element");
    static_assert(vector.y == 400, "y must match array element");
  }

  SECTION("Constructor from pointer to array with zero values") {
    constexpr array<real_t, 2> arr{0, 0};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == 0);
    REQUIRE(vector.y == 0);

    static_assert(vector.x == 0, "x must be 0");
    static_assert(vector.y == 0, "y must be 0");
  }

  SECTION("Runtime constructor tests") {
    Vector2D defaultVector;
    REQUIRE(defaultVector.x == 0);
    REQUIRE(defaultVector.y == 0);

    Vector2D coordVector(123, -456);
    REQUIRE(coordVector.x == 123);
    REQUIRE(coordVector.y == -456);

    constexpr array<real_t, 2> arr{789, -321};
    Vector2D arrayVector(arr.data());
    REQUIRE(arrayVector.x == 789);
    REQUIRE(arrayVector.y == -321);
  }
}

// c_arr returns pointer to contiguous x,y.
TEST_CASE("math/vector2d/vector2d_c_arr_methods") {
  SECTION("Non-const c_arr method") {
    Vector2D vector(42, -17);
    auto * arr = vector.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 42);
    REQUIRE(arr[1] == -17);
    REQUIRE(arr == &vector.x);

    arr[0] = 100;
    arr[1] = -200;

    REQUIRE(vector.x == 100);
    REQUIRE(vector.y == -200);
  }

  SECTION("Const c_arr method") {
    constexpr Vector2D vector(123, -456);
    const auto * arr = vector.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 123);
    REQUIRE(arr[1] == -456);
    REQUIRE(arr == &vector.x);
    REQUIRE(vector.x == 123);
    REQUIRE(vector.y == -456);
  }

  SECTION("c_arr with default constructor") {
    constexpr Vector2D vector;

    REQUIRE(vector.c_arr()[0] == 0);
    REQUIRE(vector.c_arr()[1] == 0);

    static_assert(vector.c_arr()[0] == 0, "first element must be 0 for default-constructed vector");
  }

  SECTION("c_arr with coordinate constructor") {
    constexpr Vector2D vector(10, 20);

    REQUIRE(vector.c_arr()[0] == 10);
    REQUIRE(vector.c_arr()[1] == 20);

    static_assert(vector.c_arr()[0] == 10, "first element must match x");
  }

  SECTION("c_arr with array constructor") {
    constexpr Vector2D vector({-50, 75});

    REQUIRE(vector.c_arr()[0] == -50);
    REQUIRE(vector.c_arr()[1] == 75);

    static_assert(vector.c_arr()[0] == -50, "first element must match x");
  }

  SECTION("c_arr modification test") {
    Vector2D vector(1, 2);
    auto * arr = vector.c_arr();

    arr[0] = 10;
    arr[1] = 20;

    REQUIRE(vector.x == 10);
    REQUIRE(vector.y == 20);

    *arr = 30;
    *(arr + 1) = 40;

    REQUIRE(vector.x == 30);
    REQUIRE(vector.y == 40);

    arr[0] = 50;
    arr[1] = 60;

    REQUIRE(vector.x == 50);
    REQUIRE(vector.y == 60);
  }

  SECTION("c_arr runtime tests") {
    Vector2D runtimeVector(500, -600);
    auto * runtimeArr = runtimeVector.c_arr();

    REQUIRE(runtimeArr != nullptr);
    REQUIRE(runtimeArr[0] == 500);
    REQUIRE(runtimeArr[1] == -600);

    runtimeArr[0] = 700;
    runtimeArr[1] = -800;

    REQUIRE(runtimeVector.x == 700);
    REQUIRE(runtimeVector.y == -800);

    const Vector2D constRuntimeVector(900, -1000);
    const auto * constRuntimeArr = constRuntimeVector.c_arr();

    REQUIRE(constRuntimeArr != nullptr);
    REQUIRE(constRuntimeArr[0] == 900);
    REQUIRE(constRuntimeArr[1] == -1000);
  }
}

// +=, -=, *=, /= and chaining.
TEST_CASE("math/vector2d/vector2d_operators") {
  SECTION("operator+=") {
    Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, -10);

    vector1 += vector2;

    REQUIRE(vector1.x == 15);
    REQUIRE(vector1.y == 10);
  }

  SECTION("operator-=") {
    Vector2D vector1(15, 25);
    constexpr Vector2D vector2(5, 10);

    vector1 -= vector2;

    REQUIRE(vector1.x == 10);
    REQUIRE(vector1.y == 15);
  }

  SECTION("operator*=") {
    Vector2D vector(10, 20);
    constexpr real_t scalar = 2.5;

    vector *= scalar;

    REQUIRE(vector.x == 25);
    REQUIRE(vector.y == 50);
  }

  SECTION("operator/=") {
    Vector2D vector(20, 45);
    constexpr real_t scalar = 2.5;

    vector /= scalar;

    REQUIRE(vector.x == 8);
    REQUIRE(vector.y == 18);
  }

  SECTION("Chained operations") {
    Vector2D vector(10, 20);
    constexpr Vector2D offset(5, 10);

    vector += offset;
    vector *= 2;
    vector -= offset;

    REQUIRE(vector.x == 25);
    REQUIRE(vector.y == 50);
  }
}

// sqrMagnitude, setZero, isZero, isEqual.
TEST_CASE("math/vector2d/vector2d_methods") {
  SECTION("sqrMagnitude") {
    constexpr Vector2D vector(3, 4);

    REQUIRE(vector.sqrMagnitude() == 25);

    static_assert(vector.sqrMagnitude() == 25, "sqrMagnitude of (3,4) must be 25");
  }

  SECTION("sqrMagnitude with negative coordinates") {
    constexpr Vector2D vector(-3, -4);

    REQUIRE(vector.sqrMagnitude() == 25);

    static_assert(vector.sqrMagnitude() == 25, "sqrMagnitude of (-3,-4) must be 25");
  }

  SECTION("sqrMagnitude with zero coordinates") {
    constexpr Vector2D vector(0, 0);

    REQUIRE(vector.sqrMagnitude() == 0);

    static_assert(vector.sqrMagnitude() == 0, "sqrMagnitude of origin must be 0");
  }

  SECTION("sqrMagnitude with large coordinates") {
    constexpr Vector2D vector(1000, 2000);

    REQUIRE(vector.sqrMagnitude() == 5000000);

    static_assert(vector.sqrMagnitude() == 5000000, "sqrMagnitude must equal x² + y²");
  }

  SECTION("setZero") {
    Vector2D vector(100, 200);

    vector.setZero();

    REQUIRE(vector.x == 0);
    REQUIRE(vector.y == 0);
  }

  SECTION("isZero with zero vector") {
    constexpr Vector2D vector(0, 0);

    REQUIRE(vector.isZero());

    static_assert(vector.isZero(), "origin must be zero");
  }

  SECTION("isZero with non-zero vector") {
    constexpr Vector2D vector(1, 0);

    REQUIRE(!vector.isZero());

    static_assert(!vector.isZero(), "non-zero vector must not be zero");
  }

  SECTION("isZero with negative coordinates") {
    constexpr Vector2D vector(-1, -1);

    REQUIRE(!vector.isZero());

    static_assert(!vector.isZero(), "non-zero vector must not be zero");
  }

  SECTION("isZero after setZero") {
    Vector2D vector(100, 200);

    REQUIRE(!vector.isZero());

    vector.setZero();

    REQUIRE(vector.isZero());
  }

  SECTION("isEqual with exact match") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 20);

    REQUIRE(vector1.isEqual(vector2));
  }

  SECTION("isEqual with different vectors") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(11, 20);

    REQUIRE(!vector1.isEqual(vector2));
  }

  SECTION("isEqual with tolerance") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(12, 18);
    constexpr real_t tolerance = 3;

    REQUIRE(vector1.isEqual(vector2, tolerance));
  }

  SECTION("isEqual with tolerance too small") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(15, 25);
    constexpr real_t tolerance = 2;

    REQUIRE(!vector1.isEqual(vector2, tolerance));
  }

  SECTION("isEqual with zero tolerance") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 21);
    constexpr real_t tolerance = 0;

    REQUIRE(!vector1.isEqual(vector2, tolerance));
  }

  SECTION("isEqual with default tolerance") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 20);

    REQUIRE(vector1.isEqual(vector2));
  }

  SECTION("isEqual with large tolerance") {
    constexpr Vector2D vector1(0, 0);
    constexpr Vector2D vector2(1000, 1000);
    constexpr real_t tolerance = 2000;

    REQUIRE(vector1.isEqual(vector2, tolerance));
  }

  SECTION("Runtime tests") {
    Vector2D vector(10, 20);
    constexpr Vector2D offset(5, -10);

    vector += offset;
    REQUIRE(vector.x == 15);
    REQUIRE(vector.y == 10);

    vector *= 2;
    REQUIRE(vector.x == 30);
    REQUIRE(vector.y == 20);

    vector /= 4;
    REQUIRE(vector.x == 7.5);
    REQUIRE(vector.y == 5);

    REQUIRE(vector.sqrMagnitude() == 81.25);
    REQUIRE(!vector.isZero());

    vector.setZero();
    REQUIRE(vector.isZero());
    REQUIRE(vector.sqrMagnitude() == 0);

    constexpr Vector2D testVector(5, 5);
    REQUIRE(!vector.isEqual(testVector));
    REQUIRE(vector.isEqual(testVector, 10));
  }
}

// +, -, *, /, unary minus, ==.
TEST_CASE("math/vector2d/vector2d_binary_operators") {
  SECTION("Unary minus operator") {
    constexpr Vector2D vector(10, -20);
    constexpr auto result = -vector;

    REQUIRE(result.x == -10);
    REQUIRE(result.y == 20);

    static_assert(result.x == -10, "unary minus must negate x");
    static_assert(result.y == 20, "unary minus must negate y");
  }

  SECTION("Unary minus with zero coordinates") {
    constexpr Vector2D vector(0, 0);
    constexpr auto result = -vector;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    static_assert(result.x == 0, "unary minus of origin must remain 0");
    static_assert(result.y == 0, "unary minus of origin must remain 0");
  }

  SECTION("Unary minus with negative coordinates") {
    constexpr Vector2D vector(-5, -15);
    constexpr auto result = -vector;

    REQUIRE(result.x == 5);
    REQUIRE(result.y == 15);

    static_assert(result.x == 5, "unary minus must negate x");
    static_assert(result.y == 15, "unary minus must negate y");
  }

  SECTION("Addition operator") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, -10);

    constexpr auto result = vector1 + vector2;

    REQUIRE(result.x == 15);
    REQUIRE(result.y == 10);

    static_assert(result.x == 15, "addition x must be sum of x components");
    static_assert(result.y == 10, "addition y must be sum of y components");
  }

  SECTION("Addition with zero coordinates") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(0, 0);

    constexpr auto result = vector1 + vector2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "adding origin must preserve x");
    static_assert(result.y == 20, "adding origin must preserve y");
  }

  SECTION("Addition with negative coordinates") {
    constexpr Vector2D vector1(-10, -20);
    constexpr Vector2D vector2(-5, -15);

    constexpr auto result = vector1 + vector2;

    REQUIRE(result.x == -15);
    REQUIRE(result.y == -35);

    static_assert(result.x == -15, "addition x must be sum of x components");
    static_assert(result.y == -35, "addition y must be sum of y components");
  }

  SECTION("Subtraction operator") {
    constexpr Vector2D vector1(15, 25);
    constexpr Vector2D vector2(5, 10);

    constexpr auto result = vector1 - vector2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 15);

    static_assert(result.x == 10, "subtraction x must be difference of x components");
    static_assert(result.y == 15, "subtraction y must be difference of y components");
  }

  SECTION("Subtraction with zero coordinates") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(0, 0);

    constexpr auto result = vector1 - vector2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "subtracting origin must preserve x");
    static_assert(result.y == 20, "subtracting origin must preserve y");
  }

  SECTION("Subtraction with negative coordinates") {
    constexpr Vector2D vector1(-10, -20);
    constexpr Vector2D vector2(-5, -15);

    constexpr auto result = vector1 - vector2;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -5);

    static_assert(result.x == -5, "subtraction x must be difference of x components");
    static_assert(result.y == -5, "subtraction y must be difference of y components");
  }

  SECTION("Multiplication with scalar (vector * scalar)") {
    constexpr Vector2D vector(10, 20);
    constexpr real_t scalar = 2.5;

    constexpr auto result = vector * scalar;

    REQUIRE(result.x == 25);
    REQUIRE(result.y == 50);

    static_assert(result.x == 25, "vector * scalar must scale x");
    static_assert(result.y == 50, "vector * scalar must scale y");
  }

  SECTION("Multiplication with scalar (scalar * vector)") {
    constexpr real_t scalar = 1.5;
    constexpr Vector2D vector(20, 30);

    constexpr auto result = scalar * vector;

    REQUIRE(result.x == 30);
    REQUIRE(result.y == 45);

    static_assert(result.x == 30, "scalar * vector must scale x");
    static_assert(result.y == 45, "scalar * vector must scale y");
  }

  SECTION("Multiplication with scalar zero") {
    constexpr Vector2D vector(10, 20);
    constexpr real_t scalar = 0.0;

    constexpr auto result = vector * scalar;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    static_assert(result.x == 0, "vector * 0 must yield 0");
    static_assert(result.y == 0, "vector * 0 must yield 0");
  }

  SECTION("Multiplication with scalar negative") {
    constexpr Vector2D vector(10, 20);
    constexpr real_t scalar = -0.5;

    constexpr auto result = vector * scalar;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -10);

    static_assert(result.x == -5, "vector * negative scalar must scale x");
    static_assert(result.y == -10, "vector * negative scalar must scale y");
  }

  SECTION("Division with scalar") {
    constexpr Vector2D vector(25, 50);
    constexpr real_t scalar = 2.5;

    constexpr auto result = vector / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    static_assert(result.x == 10, "vector / scalar must divide x");
    static_assert(result.y == 20, "vector / scalar must divide y");
  }

  SECTION("Division with scalar negative") {
    constexpr Vector2D vector(-30, -60);
    constexpr real_t scalar = -1.5;

    constexpr auto result = vector / scalar;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 40);

    static_assert(result.x == 20, "vector / negative scalar must divide x");
    static_assert(result.y == 40, "vector / negative scalar must divide y");
  }

  SECTION("Equality operator with identical points") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 20);

    REQUIRE(vector1 == vector2);

    static_assert(vector1 == vector2, "identical vectors must compare equal");
  }

  SECTION("Equality operator with different points") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(11, 20);

    REQUIRE(!(vector1 == vector2));

    static_assert(!(vector1 == vector2), "different vectors must not compare equal");
  }

  SECTION("Equality operator with zero coordinates") {
    constexpr Vector2D vector1(0, 0);
    constexpr Vector2D vector2(0, 0);

    REQUIRE(vector1 == vector2);

    static_assert(vector1 == vector2, "identical origins must compare equal");
  }

  SECTION("Equality operator with negative coordinates") {
    constexpr Vector2D vector1(-10, -20);
    constexpr Vector2D vector2(-10, -20);

    REQUIRE(vector1 == vector2);

    static_assert(vector1 == vector2, "identical origins must compare equal");
  }

  SECTION("Equality operator with mixed coordinates") {
    constexpr Vector2D vector1(10, -20);
    constexpr Vector2D vector2(10, -20);

    REQUIRE(vector1 == vector2);

    static_assert(vector1 == vector2, "identical origins must compare equal");
  }

  SECTION("Chained binary operations") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, 10);
    constexpr real_t scalar = 2;

    constexpr auto result = (vector1 + vector2) * scalar - vector1;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 40);

    static_assert(result.x == 20, "chained (v1+v2)*s-v1 must yield correct x");
    static_assert(result.y == 40, "chained (v1+v2)*s-v1 must yield correct y");
  }

  SECTION("Complex chained operations") {
    constexpr Vector2D vector1(100, 200);
    constexpr Vector2D vector2(50, 75);
    constexpr real_t scalar1 = 1.5;
    constexpr real_t scalar2 = 2;

    constexpr auto result = (vector1 * scalar1 + vector2) / scalar2;

    REQUIRE(result.x == 100);
    REQUIRE(result.y == 187.5);

    static_assert(result.x == 100, "chained (v1*s1+v2)/s2 must yield correct x");
    static_assert(result.y == 187.5, "chained (v1*s1+v2)/s2 must yield correct y");
  }

  SECTION("Chained tests") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, -10);

    constexpr auto negated = -vector1;
    constexpr auto sum = negated + vector2;
    constexpr auto diff = sum - vector2;
    constexpr auto mult = diff * 2.5f;
    constexpr auto div = mult / 5;

    REQUIRE(negated.x == -10);
    REQUIRE(negated.y == -20);
    REQUIRE(sum.x == -5);
    REQUIRE(sum.y == -30);
    REQUIRE(diff.x == -10);
    REQUIRE(diff.y == -20);
    REQUIRE(mult.x == -25);
    REQUIRE(mult.y == -50);
    REQUIRE(div.x == -5);
    REQUIRE(div.y == -10);

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
