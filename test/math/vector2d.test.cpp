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

using namespace toy::math;

TEST_CASE("Vector2D object structure", "[math][vector2d]") {
  constexpr Vector2D vector(111, 222);

  STATIC_REQUIRE(sizeof(vector) == sizeof(real_t) * 2);

  const auto * arr = vector.c_arr();

  // Verify memory layout
  REQUIRE(arr == &vector.x);
  REQUIRE(arr + 1 == &vector.y);

  // Verify contiguous memory
  REQUIRE(reinterpret_cast<const std::byte *>(arr + 1) - reinterpret_cast<const std::byte *>(arr) == sizeof(real_t));
}

TEST_CASE("Vector2D constructors", "[math][vector2d]") {
  SECTION("Default constructor") {
    const Vector2D vector;
    REQUIRE(vector.x == 0.0);
    REQUIRE(vector.y == 0.0);

    // Compile-time checks
    constexpr Vector2D constexprVector;
    STATIC_REQUIRE(constexprVector.x == 0.0);
    STATIC_REQUIRE(constexprVector.y == 0.0);
  }

  SECTION("Constructor with positive coordinates") {
    const Vector2D vector(12, 23);
    REQUIRE(vector.x == 12);
    REQUIRE(vector.y == 23);

    // Compile-time checks
    constexpr Vector2D constexprVector(12, 23);
    STATIC_REQUIRE(constexprVector.x == 12);
    STATIC_REQUIRE(constexprVector.y == 23);
  }

  SECTION("Constructor with negative coordinates") {
    const Vector2D vector(-5, -15);
    REQUIRE(vector.x == -5);
    REQUIRE(vector.y == -15);

    // Compile-time checks
    constexpr Vector2D constexprVector(-5, -15);
    STATIC_REQUIRE(constexprVector.x == -5);
    STATIC_REQUIRE(constexprVector.y == -15);
  }

  SECTION("Constructor with mixed coordinates") {
    const Vector2D vector(-100, 200);
    REQUIRE(vector.x == -100);
    REQUIRE(vector.y == 200);

    // Compile-time checks
    constexpr Vector2D constexprVector(-100, 200);
    STATIC_REQUIRE(constexprVector.x == -100);
    STATIC_REQUIRE(constexprVector.y == 200);
  }

  SECTION("Constructor with zero coordinates") {
    const Vector2D vector(0, 0);
    REQUIRE(vector.x == 0);
    REQUIRE(vector.y == 0);

    // Compile-time checks
    constexpr Vector2D constexprVector(0, 0);
    STATIC_REQUIRE(constexprVector.x == 0);
    STATIC_REQUIRE(constexprVector.y == 0);
  }

  SECTION("Constructor from pointer to array") {
    constexpr std::array<real_t, 2> arr{42, -17};

    const Vector2D vector(arr.data());
    REQUIRE(vector.x == 42);
    REQUIRE(vector.y == -17);

    // Compile-time checks
    constexpr Vector2D constexprVector(arr.data());
    STATIC_REQUIRE(constexprVector.x == 42);
    STATIC_REQUIRE(constexprVector.y == -17);
  }

  SECTION("Constructor from pointer to array with positive values") {
    constexpr std::array<real_t, 2> arr{100, 200};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == 100);
    REQUIRE(vector.y == 200);

    // Compile-time checks
    STATIC_REQUIRE(vector.x == 100);
    STATIC_REQUIRE(vector.y == 200);
  }

  SECTION("Constructor from pointer to array with negative values") {
    constexpr std::array<real_t, 2> arr{-50, -75};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == -50);
    REQUIRE(vector.y == -75);

    // Compile-time checks
    STATIC_REQUIRE(vector.x == -50);
    STATIC_REQUIRE(vector.y == -75);
  }

  SECTION("Constructor from pointer to array with mixed values") {
    constexpr std::array<real_t, 2> arr{-300, 400};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == -300);
    REQUIRE(vector.y == 400);

    // Compile-time checks
    STATIC_REQUIRE(vector.x == -300);
    STATIC_REQUIRE(vector.y == 400);
  }

  SECTION("Constructor from pointer to array with zero values") {
    constexpr std::array<real_t, 2> arr{0, 0};
    constexpr Vector2D vector(arr.data());

    REQUIRE(vector.x == 0);
    REQUIRE(vector.y == 0);

    // Compile-time checks
    STATIC_REQUIRE(vector.x == 0);
    STATIC_REQUIRE(vector.y == 0);
  }

  SECTION("Runtime constructor tests") {
    // Test default constructor at runtime
    Vector2D defaultVector;
    REQUIRE(defaultVector.x == 0);
    REQUIRE(defaultVector.y == 0);

    // Test coordinate constructor at runtime
    Vector2D coordVector(123, -456);
    REQUIRE(coordVector.x == 123);
    REQUIRE(coordVector.y == -456);

    // Test array constructor at runtime
    constexpr std::array<real_t, 2> arr{789, -321};
    Vector2D arrayVector(arr.data());
    REQUIRE(arrayVector.x == 789);
    REQUIRE(arrayVector.y == -321);
  }
}

TEST_CASE("Vector2D c_arr methods", "[math][vector2d]") {
  SECTION("Non-const c_arr method") {
    Vector2D vector(42, -17);
    auto * arr = vector.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 42);
    REQUIRE(arr[1] == -17);
    REQUIRE(arr == &vector.x);

    // Test modification through pointer
    arr[0] = 100;
    arr[1] = -200;
    REQUIRE(vector.x == 100);
    REQUIRE(vector.y == -200);
  }

  SECTION("Const c_arr method") {
    constexpr Vector2D vector(123, -456);
    const real_t * arr = vector.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 123);
    REQUIRE(arr[1] == -456);
    REQUIRE(arr == &vector.x);

    // Verify read-only access
    REQUIRE(vector.x == 123);
    REQUIRE(vector.y == -456);
  }

  SECTION("c_arr with default constructor") {
    constexpr Vector2D vector;

    REQUIRE(vector.c_arr()[0] == 0);
    REQUIRE(vector.c_arr()[1] == 0);

    // Compile-time checks
    STATIC_REQUIRE(vector.c_arr()[0] == 0);
  }

  SECTION("c_arr with coordinate constructor") {
    constexpr Vector2D vector(10, 20);

    REQUIRE(vector.c_arr()[0] == 10);
    REQUIRE(vector.c_arr()[1] == 20);

    // Compile-time checks
    STATIC_REQUIRE(vector.c_arr()[0] == 10);
  }

  SECTION("c_arr with array constructor") {
    constexpr Vector2D vector({-50, 75});

    REQUIRE(vector.c_arr()[0] == -50);
    REQUIRE(vector.c_arr()[1] == 75);

    // Compile-time checks
    STATIC_REQUIRE(vector.c_arr()[0] == -50);
  }

  SECTION("c_arr modification test") {
    Vector2D vector(1, 2);
    auto * arr = vector.c_arr();

    // Modify through array access
    arr[0] = 10;
    arr[1] = 20;
    REQUIRE(vector.x == 10);
    REQUIRE(vector.y == 20);

    // Modify through pointer arithmetic
    *arr = 30;
    *(arr + 1) = 40;
    REQUIRE(vector.x == 30);
    REQUIRE(vector.y == 40);

    // Modify through direct assignment
    arr[0] = 50;
    arr[1] = 60;
    REQUIRE(vector.x == 50);
    REQUIRE(vector.y == 60);
  }

  SECTION("c_arr runtime tests") {
    // Test non-const version at runtime
    Vector2D runtimeVector(500, -600);
    auto * runtimeArr = runtimeVector.c_arr();

    REQUIRE(runtimeArr != nullptr);
    REQUIRE(runtimeArr[0] == 500);
    REQUIRE(runtimeArr[1] == -600);

    // Modify at runtime
    runtimeArr[0] = 700;
    runtimeArr[1] = -800;
    REQUIRE(runtimeVector.x == 700);
    REQUIRE(runtimeVector.y == -800);

    // Test const version at runtime
    const Vector2D constRuntimeVector(900, -1000);
    const auto * constRuntimeArr = constRuntimeVector.c_arr();

    REQUIRE(constRuntimeArr != nullptr);
    REQUIRE(constRuntimeArr[0] == 900);
    REQUIRE(constRuntimeArr[1] == -1000);
  }
}

TEST_CASE("Vector2D operators", "[math][vector2d]") {
  SECTION("operator+=") {
    Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, -10);

    vector1 += vector2;

    REQUIRE(vector1.x == 15);
    REQUIRE(vector1.y == 10);
  }

  SECTION("operator-=") {
    Vector2D vector1(15, 25);
    const Vector2D vector2(5, 10);

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

TEST_CASE("Vector2D methods", "[math][vector2d]") {
  SECTION("sqrMagnitude") {
    constexpr Vector2D vector(3, 4);
    constexpr auto magnitude = vector.sqrMagnitude();

    REQUIRE(magnitude == 25);
    STATIC_REQUIRE(magnitude == 25);
  }

  SECTION("sqrMagnitude with negative coordinates") {
    constexpr Vector2D vector(-3, -4);
    constexpr auto magnitude = vector.sqrMagnitude();

    REQUIRE(magnitude == 25);

    // Compile-time checks
    STATIC_REQUIRE(magnitude == 25);
  }

  SECTION("sqrMagnitude with zero coordinates") {
    constexpr Vector2D vector(0, 0);
    constexpr auto magnitude = vector.sqrMagnitude();

    REQUIRE(magnitude == 0);

    // Compile-time checks
    STATIC_REQUIRE(magnitude == 0);
  }

  SECTION("sqrMagnitude with large coordinates") {
    constexpr Vector2D vector(1000, 2000);
    constexpr auto magnitude = vector.sqrMagnitude();

    REQUIRE(magnitude == 5000000);

    // Compile-time checks
    STATIC_REQUIRE(magnitude == 5000000);
  }

  SECTION("setZero") {
    Vector2D vector(100, 200);

    vector.setZero();

    REQUIRE(vector.x == 0);
    REQUIRE(vector.y == 0);
  }

  SECTION("isZero with zero vector") {
    constexpr Vector2D vector(0, 0);
    constexpr bool result = vector.isZero();

    REQUIRE(result == true);

    // Compile-time checks
    STATIC_REQUIRE(result == true);
  }

  SECTION("isZero with non-zero vector") {
    constexpr Vector2D vector(1, 0);
    constexpr bool result = vector.isZero();

    REQUIRE(result == false);

    // Compile-time checks
    STATIC_REQUIRE(result == false);
  }

  SECTION("isZero with negative coordinates") {
    constexpr Vector2D vector(-1, -1);
    constexpr bool result = vector.isZero();

    REQUIRE(result == false);

    // Compile-time checks
    STATIC_REQUIRE(result == false);
  }

  SECTION("isZero after setZero") {
    Vector2D vector(100, 200);

    REQUIRE(vector.isZero() == false);

    vector.setZero();

    REQUIRE(vector.isZero() == true);
  }

  SECTION("isEqual with exact match") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 20);

    const auto result = vector1.isEqual(vector2);

    REQUIRE(result == true);
  }

  SECTION("isEqual with different vectors") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(11, 20);

    const bool result = vector1.isEqual(vector2);

    REQUIRE(result == false);
  }

  SECTION("isEqual with tolerance") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(12, 18);
    constexpr real_t tolerance = 3;

    const bool result = vector1.isEqual(vector2, tolerance);

    REQUIRE(result == true); // |10-12| <= 3 && |20-18| <= 3
  }

  SECTION("isEqual with tolerance too small") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(15, 25);
    constexpr real_t tolerance = 2;

    const bool result = vector1.isEqual(vector2, tolerance);

    REQUIRE(result == false); // |10-15| > 2 || |20-25| > 2
  }

  SECTION("isEqual with zero tolerance") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 21);
    constexpr real_t tolerance = 0;

    const bool result = vector1.isEqual(vector2, tolerance);

    REQUIRE(result == false); // Exact match required
  }

  SECTION("isEqual with default tolerance") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 20);

    const bool result = vector1.isEqual(vector2);

    REQUIRE(result == true); // Default tolerance is 0, exact match
  }

  SECTION("isEqual with large tolerance") {
    constexpr Vector2D vector1(0, 0);
    constexpr Vector2D vector2(1000, 1000);
    constexpr real_t tolerance = 2000;

    const bool result = vector1.isEqual(vector2, tolerance);

    REQUIRE(result == true); // |0-1000| <= 2000 && |0-1000| <= 2000
  }

  SECTION("Runtime tests") {
    // Test operators at runtime
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

    // Test methods at runtime
    REQUIRE(vector.sqrMagnitude() == 81.25); // 7.5² + 5² = 56.25 + 25 = 81.25
    REQUIRE(vector.isZero() == false);

    vector.setZero();
    REQUIRE(vector.isZero() == true);
    REQUIRE(vector.sqrMagnitude() == 0);

    // Test isEqual at runtime
    constexpr Vector2D testVector(5, 5);
    REQUIRE(vector.isEqual(testVector) == false);
    REQUIRE(vector.isEqual(testVector, 10) == true);
  }
}

TEST_CASE("Vector2D binary operators", "[math][vector2d]") {
  SECTION("Unary minus operator") {
    constexpr Vector2D vector(10, -20);
    constexpr Vector2D result = -vector;

    REQUIRE(result.x == -10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    STATIC_REQUIRE(result.x == -10);
    STATIC_REQUIRE(result.y == 20);
  }

  SECTION("Unary minus with zero coordinates") {
    constexpr Vector2D vector(0, 0);
    constexpr Vector2D result = -vector;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 0);
    STATIC_REQUIRE(result.y == 0);
  }

  SECTION("Unary minus with negative coordinates") {
    constexpr Vector2D vector(-5, -15);
    constexpr Vector2D result = -vector;

    REQUIRE(result.x == 5);
    REQUIRE(result.y == 15);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 5);
    STATIC_REQUIRE(result.y == 15);
  }

  SECTION("Addition operator") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, -10);

    constexpr Vector2D result = vector1 + vector2;

    REQUIRE(result.x == 15);
    REQUIRE(result.y == 10);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 15);
    STATIC_REQUIRE(result.y == 10);
  }

  SECTION("Addition with zero coordinates") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(0, 0);

    constexpr Vector2D result = vector1 + vector2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 10);
    STATIC_REQUIRE(result.y == 20);
  }

  SECTION("Addition with negative coordinates") {
    constexpr Vector2D vector1(-10, -20);
    constexpr Vector2D vector2(-5, -15);

    constexpr Vector2D result = vector1 + vector2;

    REQUIRE(result.x == -15);
    REQUIRE(result.y == -35);

    // Compile-time checks
    STATIC_REQUIRE(result.x == -15);
    STATIC_REQUIRE(result.y == -35);
  }

  SECTION("Subtraction operator") {
    constexpr Vector2D vector1(15, 25);
    constexpr Vector2D vector2(5, 10);

    constexpr Vector2D result = vector1 - vector2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 15);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 10);
    STATIC_REQUIRE(result.y == 15);
  }

  SECTION("Subtraction with zero coordinates") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(0, 0);

    constexpr Vector2D result = vector1 - vector2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 10);
    STATIC_REQUIRE(result.y == 20);
  }

  SECTION("Subtraction with negative coordinates") {
    constexpr Vector2D vector1(-10, -20);
    constexpr Vector2D vector2(-5, -15);

    constexpr Vector2D result = vector1 - vector2;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -5);

    // Compile-time checks
    STATIC_REQUIRE(result.x == -5);
    STATIC_REQUIRE(result.y == -5);
  }

  SECTION("Multiplication with scalar (vector * scalar)") {
    constexpr Vector2D vector(10, 20);
    constexpr real_t scalar = 2.5;

    constexpr Vector2D result = vector * scalar;

    REQUIRE(result.x == 25);
    REQUIRE(result.y == 50);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 25);
    STATIC_REQUIRE(result.y == 50);
  }

  SECTION("Multiplication with scalar (scalar * vector)") {
    constexpr real_t scalar = 1.5;
    constexpr Vector2D vector(20, 30);

    constexpr Vector2D result = scalar * vector;

    REQUIRE(result.x == 30);
    REQUIRE(result.y == 45);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 30);
    STATIC_REQUIRE(result.y == 45);
  }

  SECTION("Multiplication with scalar zero") {
    constexpr Vector2D vector(10, 20);
    constexpr real_t scalar = 0.0;

    constexpr Vector2D result = vector * scalar;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 0);
    STATIC_REQUIRE(result.y == 0);
  }

  SECTION("Multiplication with scalar negative") {
    constexpr Vector2D vector(10, 20);
    constexpr real_t scalar = -0.5;

    constexpr Vector2D result = vector * scalar;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -10);

    // Compile-time checks
    STATIC_REQUIRE(result.x == -5);
    STATIC_REQUIRE(result.y == -10);
  }

  SECTION("Division with scalar") {
    constexpr Vector2D vector(25, 50);
    constexpr real_t scalar = 2.5;

    constexpr Vector2D result = vector / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 10);
    STATIC_REQUIRE(result.y == 20);
  }

  SECTION("Division with scalar negative") {
    constexpr Vector2D vector(-30, -60);
    constexpr real_t scalar = -1.5;

    constexpr Vector2D result = vector / scalar;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 40);

    // Compile-time checks
    STATIC_REQUIRE(result.x == 20);
    STATIC_REQUIRE(result.y == 40);
  }

  SECTION("Equality operator with identical points") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(10, 20);

    constexpr bool result = vector1 == vector2;

    REQUIRE(result == true);

    // Compile-time checks
    STATIC_REQUIRE(result == true);
  }

  SECTION("Equality operator with different points") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(11, 20);

    constexpr bool result = vector1 == vector2;

    REQUIRE(result == false);

    // Compile-time checks
    STATIC_REQUIRE(result == false);
  }

  SECTION("Equality operator with zero coordinates") {
    constexpr Vector2D vector1(0, 0);
    constexpr Vector2D vector2(0, 0);

    constexpr bool result = vector1 == vector2;

    REQUIRE(result == true);

    // Compile-time checks
    STATIC_REQUIRE(result == true);
  }

  SECTION("Equality operator with negative coordinates") {
    constexpr Vector2D vector1(-10, -20);
    constexpr Vector2D vector2(-10, -20);

    constexpr bool result = vector1 == vector2;

    REQUIRE(result == true);

    // Compile-time checks
    STATIC_REQUIRE(result == true);
  }

  SECTION("Equality operator with mixed coordinates") {
    constexpr Vector2D vector1(10, -20);
    constexpr Vector2D vector2(10, -20);

    constexpr bool result = vector1 == vector2;

    REQUIRE(result == true);

    // Compile-time checks
    STATIC_REQUIRE(result == true);
  }

  SECTION("Chained binary operations") {
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, 10);
    constexpr real_t scalar = 2;

    constexpr Vector2D result = (vector1 + vector2) * scalar - vector1;

    REQUIRE(result.x == 20); // ((10+5)*2 - 10) = 30 - 10 = 20
    REQUIRE(result.y == 40); // ((20+10)*2 - 20) = 60 - 20 = 40

    // Compile-time checks
    STATIC_REQUIRE(result.x == 20);
    STATIC_REQUIRE(result.y == 40);
  }

  SECTION("Complex chained operations") {
    constexpr Vector2D vector1(100, 200);
    constexpr Vector2D vector2(50, 75);
    constexpr real_t scalar1 = 1.5;
    constexpr real_t scalar2 = 2;

    constexpr Vector2D result = (vector1 * scalar1 + vector2) / scalar2;

    REQUIRE(result.x == 100); // ((100*1.5 + 50) / 2) = (150 + 50) / 2 = 100
    REQUIRE(result.y == 187.5); // ((200*1.5 + 75) / 2) = (300 + 75) / 2 = 187.5

    // Compile-time checks
    STATIC_REQUIRE(result.x == 100);
    STATIC_REQUIRE(result.y == 187.5);
  }

  SECTION("Chained tests") {
    // Test all binary operators at runtime
    constexpr Vector2D vector1(10, 20);
    constexpr Vector2D vector2(5, -10);

    // Unary minus
    constexpr Vector2D negated = -vector1;
    REQUIRE(negated.x == -10);
    REQUIRE(negated.y == -20);

    // Addition
    constexpr Vector2D sum = negated + vector2;
    REQUIRE(sum.x == -5);
    REQUIRE(sum.y == -30);

    // Subtraction
    constexpr Vector2D diff = sum - vector2;
    REQUIRE(diff.x == -10);
    REQUIRE(diff.y == -20);

    // Multiplication
    constexpr Vector2D mult = diff * 2.5f;
    REQUIRE(mult.x == -25);
    REQUIRE(mult.y == -50);

    // Division
    constexpr Vector2D div = mult / 5;
    REQUIRE(div.x == -5);
    REQUIRE(div.y == -10);

    // Compile-time checks
    STATIC_REQUIRE(negated.x == -10);
    STATIC_REQUIRE(negated.y == -20);
    STATIC_REQUIRE(sum.x == -5);
    STATIC_REQUIRE(sum.y == -30);
    STATIC_REQUIRE(diff.x == -10);
    STATIC_REQUIRE(diff.y == -20);
    STATIC_REQUIRE(mult.x == -25);
    STATIC_REQUIRE(mult.y == -50);
    STATIC_REQUIRE(div.x == -5);
    STATIC_REQUIRE(div.y == -10);
  }
}
