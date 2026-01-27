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

TEST_CASE("Point object structure", "[math][point]") {
  constexpr Point point(111, 222);

  static_assert(sizeof(point) == sizeof(int32_t) * 2);

  const auto * arr = point.c_arr();

  // Verify memory layout
  REQUIRE(arr == &point.x);
  REQUIRE(arr + 1 == &point.y);

  // Verify contiguous memory
  REQUIRE(reinterpret_cast<const std::byte *>(arr + 1) - reinterpret_cast<const std::byte *>(arr) == sizeof(int32_t));
}

TEST_CASE("Point constructors", "[math][point]") {
  SECTION("Default constructor") {
    const Point point;
    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    // Compile-time checks
    constexpr Point constexprPoint;
    static_assert(constexprPoint.x == 0);
    static_assert(constexprPoint.y == 0);
  }

  SECTION("Constructor with positive coordinates") {
    const Point point(10, 20);
    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);

    // Compile-time checks
    constexpr Point constexprPoint(10, 20);
    static_assert(constexprPoint.x == 10);
    static_assert(constexprPoint.y == 20);
  }

  SECTION("Constructor with negative coordinates") {
    const Point point(-5, -15);
    REQUIRE(point.x == -5);
    REQUIRE(point.y == -15);

    // Compile-time checks
    constexpr Point constexprPoint(-5, -15);
    static_assert(constexprPoint.x == -5);
    static_assert(constexprPoint.y == -15);
  }

  SECTION("Constructor with mixed coordinates") {
    const Point point(-100, 200);
    REQUIRE(point.x == -100);
    REQUIRE(point.y == 200);

    // Compile-time checks
    constexpr Point constexprPoint(-100, 200);
    static_assert(constexprPoint.x == -100);
    static_assert(constexprPoint.y == 200);
  }

  SECTION("Constructor with zero coordinates") {
    const Point point(0, 0);
    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    // Compile-time checks
    constexpr Point constexprPoint(0, 0);
    static_assert(constexprPoint.x == 0);
    static_assert(constexprPoint.y == 0);
  }

  SECTION("Constructor from pointer to array") {
    constexpr std::array<int32_t, 2> arr{42, -17};

    const Point point(arr.data());
    REQUIRE(point.x == 42);
    REQUIRE(point.y == -17);

    // Compile-time checks
    constexpr Point constexprPoint(arr.data());
    static_assert(constexprPoint.x == 42);
    static_assert(constexprPoint.y == -17);
  }

  SECTION("Constructor from pointer to array with positive values") {
    constexpr std::array<int32_t, 2> arr{100, 200};
    constexpr Point point(arr.data());

    static_assert(point.x == 100);
    static_assert(point.y == 200);
  }

  SECTION("Constructor from pointer to array with negative values") {
    constexpr std::array<int32_t, 2> arr{-50, -75};
    constexpr Point point(arr.data());

    static_assert(point.x == -50);
    static_assert(point.y == -75);
  }

  SECTION("Constructor from pointer to array with mixed values") {
    constexpr std::array<int32_t, 2> arr{-300, 400};
    constexpr Point point(arr.data());

    static_assert(point.x == -300);
    static_assert(point.y == 400);
  }

  SECTION("Constructor from pointer to array with zero values") {
    constexpr std::array<int32_t, 2> arr{0, 0};
    constexpr Point point(arr.data());

    static_assert(point.x == 0);
    static_assert(point.y == 0);
  }

  SECTION("Runtime constructor tests") {
    // Test default constructor at runtime
    Point defaultPoint;
    REQUIRE(defaultPoint.x == 0);
    REQUIRE(defaultPoint.y == 0);

    // Test coordinate constructor at runtime
    Point coordPoint(123, -456);
    REQUIRE(coordPoint.x == 123);
    REQUIRE(coordPoint.y == -456);

    // Test array constructor at runtime
    constexpr std::array<int32_t, 2> arr{789, -321};
    Point arrayPoint(arr.data());
    REQUIRE(arrayPoint.x == 789);
    REQUIRE(arrayPoint.y == -321);
  }
}

TEST_CASE("Point c_arr methods", "[math][point]") {
  SECTION("Non-const c_arr method") {
    Point point(42, -17);
    auto * arr = point.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 42);
    REQUIRE(arr[1] == -17);
    REQUIRE(arr == &point.x);

    // Test modification through pointer
    arr[0] = 100;
    arr[1] = -200;
    REQUIRE(point.x == 100);
    REQUIRE(point.y == -200);
  }

  SECTION("Const c_arr method") {
    constexpr Point point(123, -456);
    const int32_t * arr = point.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 123);
    REQUIRE(arr[1] == -456);
    REQUIRE(arr == &point.x);

    // Verify read-only access
    REQUIRE(point.x == 123);
    REQUIRE(point.y == -456);
  }

  SECTION("c_arr with default constructor") {
    constexpr Point point;

    REQUIRE(point.c_arr()[0] == 0);
    REQUIRE(point.c_arr()[1] == 0);

    // Compile-time checks
    static_assert(point.c_arr()[0] == 0);
  }

  SECTION("c_arr with coordinate constructor") {
    constexpr Point point(10, 20);

    REQUIRE(point.c_arr()[0] == 10);
    REQUIRE(point.c_arr()[1] == 20);

    // Compile-time checks
    static_assert(point.c_arr()[0] == 10);
  }

  SECTION("c_arr with array constructor") {
    constexpr Point point({-50, 75});

    REQUIRE(point.c_arr()[0] == -50);
    REQUIRE(point.c_arr()[1] == 75);

    // Compile-time checks
    static_assert(point.c_arr()[0] == -50);
  }

  SECTION("c_arr modification test") {
    Point point(1, 2);
    auto * arr = point.c_arr();

    // Modify through array access
    arr[0] = 10;
    arr[1] = 20;
    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);

    // Modify through pointer arithmetic
    *arr = 30;
    *(arr + 1) = 40;
    REQUIRE(point.x == 30);
    REQUIRE(point.y == 40);

    // Modify through direct assignment
    arr[0] = 50;
    arr[1] = 60;
    REQUIRE(point.x == 50);
    REQUIRE(point.y == 60);
  }

  SECTION("c_arr runtime tests") {
    // Test non-const version at runtime
    Point runtimePoint(500, -600);
    auto * runtimeArr = runtimePoint.c_arr();

    REQUIRE(runtimeArr != nullptr);
    REQUIRE(runtimeArr[0] == 500);
    REQUIRE(runtimeArr[1] == -600);

    // Modify at runtime
    runtimeArr[0] = 700;
    runtimeArr[1] = -800;
    REQUIRE(runtimePoint.x == 700);
    REQUIRE(runtimePoint.y == -800);

    // Test const version at runtime
    const Point constRuntimePoint(900, -1000);
    const auto * constRuntimeArr = constRuntimePoint.c_arr();

    REQUIRE(constRuntimeArr != nullptr);
    REQUIRE(constRuntimeArr[0] == 900);
    REQUIRE(constRuntimeArr[1] == -1000);
  }
}

TEST_CASE("Point operators", "[math][point]") {
  SECTION("operator+=") {
    Point point1(10, 20);
    constexpr Point point2(5, -10);

    point1 += point2;

    REQUIRE(point1.x == 15);
    REQUIRE(point1.y == 10);
  }

  SECTION("operator-=") {
    Point point1(15, 25);
    const Point point2(5, 10);

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

TEST_CASE("Point methods", "[math][point]") {
  SECTION("sqrMagnitude") {
    constexpr Point point(3, 4);
    constexpr auto magnitude = point.sqrMagnitude();

    REQUIRE(magnitude == 25);
    static_assert(magnitude == 25);
  }

  SECTION("sqrMagnitude with negative coordinates") {
    constexpr Point point(-3, -4);
    constexpr auto magnitude = point.sqrMagnitude();

    static_assert(magnitude == 25);
  }

  SECTION("sqrMagnitude with zero coordinates") {
    constexpr Point point(0, 0);
    constexpr auto magnitude = point.sqrMagnitude();

    static_assert(magnitude == 0);
  }

  SECTION("sqrMagnitude with large coordinates") {
    constexpr Point point(1000, 2000);
    constexpr auto magnitude = point.sqrMagnitude();

    static_assert(magnitude == 5000000);
  }

  SECTION("setZero") {
    Point point(100, 200);

    point.setZero();

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);
  }

  SECTION("isZero with zero point") {
    constexpr Point point(0, 0);
    constexpr bool result = point.isZero();

    static_assert(result);
  }

  SECTION("isZero with non-zero point") {
    constexpr Point point(1, 0);
    constexpr bool result = point.isZero();

    static_assert(!result);
  }

  SECTION("isZero with negative coordinates") {
    constexpr Point point(-1, -1);
    constexpr bool result = point.isZero();

    static_assert(!result);
  }

  SECTION("isZero after setZero") {
    Point point(100, 200);

    REQUIRE(point.isZero() == false);

    point.setZero();

    REQUIRE(point.isZero());
  }

  SECTION("isEqual with exact match") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    const auto result = point1.isEqual(point2);

    REQUIRE(result);
  }

  SECTION("isEqual with different points") {
    constexpr Point point1(10, 20);
    constexpr Point point2(11, 20);

    const bool result = point1.isEqual(point2);

    REQUIRE(result == false);
  }

  SECTION("isEqual with tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(12, 18);
    constexpr int32_t tolerance = 3;

    const bool result = point1.isEqual(point2, tolerance);

    REQUIRE(result); // |10-12| <= 3 && |20-18| <= 3
  }

  SECTION("isEqual with tolerance too small") {
    constexpr Point point1(10, 20);
    constexpr Point point2(15, 25);
    constexpr int32_t tolerance = 2;

    const bool result = point1.isEqual(point2, tolerance);

    REQUIRE(result == false); // |10-15| > 2 || |20-25| > 2
  }

  SECTION("isEqual with zero tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 21);
    constexpr int32_t tolerance = 0;

    const bool result = point1.isEqual(point2, tolerance);

    REQUIRE(result == false); // Exact match required
  }

  SECTION("isEqual with default tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    const bool result = point1.isEqual(point2);

    REQUIRE(result); // Default tolerance is 0, exact match
  }

  SECTION("isEqual with large tolerance") {
    constexpr Point point1(0, 0);
    constexpr Point point2(1000, 1000);
    constexpr int32_t tolerance = 2000;

    const bool result = point1.isEqual(point2, tolerance);

    REQUIRE(result); // |0-1000| <= 2000 && |0-1000| <= 2000
  }

  SECTION("Runtime tests") {
    // Test operators at runtime
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

    // Test methods at runtime
    REQUIRE(point.sqrMagnitude() == 136); // 10² + 6² = 100 + 36 = 136
    REQUIRE(point.isZero() == false);

    point.setZero();
    REQUIRE(point.isZero());
    REQUIRE(point.sqrMagnitude() == 0);

    // Test isEqual at runtime
    constexpr Point testPoint(5, 5);
    REQUIRE(point.isEqual(testPoint) == false);
    REQUIRE(point.isEqual(testPoint, 10));
  }
}

TEST_CASE("Point binary operators", "[math][point]") {
  SECTION("Unary minus operator") {
    constexpr Point point(10, -20);
    constexpr Point result = -point;

    REQUIRE(result.x == -10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    static_assert(result.x == -10);
    static_assert(result.y == 20);
  }

  SECTION("Unary minus with zero coordinates") {
    constexpr Point point(0, 0);
    constexpr Point result = -point;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    // Compile-time checks
    static_assert(result.x == 0);
    static_assert(result.y == 0);
  }

  SECTION("Unary minus with negative coordinates") {
    constexpr Point point(-5, -15);
    constexpr Point result = -point;

    REQUIRE(result.x == 5);
    REQUIRE(result.y == 15);

    // Compile-time checks
    static_assert(result.x == 5);
    static_assert(result.y == 15);
  }

  SECTION("Addition operator") {
    constexpr Point point1(10, 20);
    constexpr Point point2(5, -10);

    constexpr Point result = point1 + point2;

    REQUIRE(result.x == 15);
    REQUIRE(result.y == 10);

    // Compile-time checks
    static_assert(result.x == 15);
    static_assert(result.y == 10);
  }

  SECTION("Addition with zero coordinates") {
    constexpr Point point1(10, 20);
    constexpr Point point2(0, 0);

    constexpr Point result = point1 + point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    static_assert(result.x == 10);
    static_assert(result.y == 20);
  }

  SECTION("Addition with negative coordinates") {
    constexpr Point point1(-10, -20);
    constexpr Point point2(-5, -15);

    constexpr Point result = point1 + point2;

    REQUIRE(result.x == -15);
    REQUIRE(result.y == -35);

    // Compile-time checks
    static_assert(result.x == -15);
    static_assert(result.y == -35);
  }

  SECTION("Subtraction operator") {
    constexpr Point point1(15, 25);
    constexpr Point point2(5, 10);

    constexpr Point result = point1 - point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 15);

    // Compile-time checks
    static_assert(result.x == 10);
    static_assert(result.y == 15);
  }

  SECTION("Subtraction with zero coordinates") {
    constexpr Point point1(10, 20);
    constexpr Point point2(0, 0);

    constexpr Point result = point1 - point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    static_assert(result.x == 10);
    static_assert(result.y == 20);
  }

  SECTION("Subtraction with negative coordinates") {
    constexpr Point point1(-10, -20);
    constexpr Point point2(-5, -15);

    constexpr Point result = point1 - point2;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -5);

    // Compile-time checks
    static_assert(result.x == -5);
    static_assert(result.y == -5);
  }

  SECTION("Multiplication with integer scalar (point * scalar)") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = 3;

    constexpr Point result = point * scalar;

    REQUIRE(result.x == 30);
    REQUIRE(result.y == 60);

    // Compile-time checks
    static_assert(result.x == 30);
    static_assert(result.y == 60);
  }

  SECTION("Multiplication with integer scalar (scalar * point)") {
    constexpr int32_t scalar = 4;
    constexpr Point point(5, 15);

    constexpr Point result = scalar * point;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 60);

    // Compile-time checks
    static_assert(result.x == 20);
    static_assert(result.y == 60);
  }

  SECTION("Multiplication with zero scalar") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = 0;

    constexpr Point result = point * scalar;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    // Compile-time checks
    static_assert(result.x == 0);
    static_assert(result.y == 0);
  }

  SECTION("Multiplication with negative scalar") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = -2;

    constexpr Point result = point * scalar;

    REQUIRE(result.x == -20);
    REQUIRE(result.y == -40);

    // Compile-time checks
    static_assert(result.x == -20);
    static_assert(result.y == -40);
  }

  SECTION("Multiplication with real scalar (point * real)") {
    constexpr Point point(10, 20);
    constexpr real_t scalar = 2.5;

    constexpr Point result = point * scalar;

    REQUIRE(result.x == 25);
    REQUIRE(result.y == 50);

    // Compile-time checks
    static_assert(result.x == 25);
    static_assert(result.y == 50);
  }

  SECTION("Multiplication with real scalar (real * point)") {
    constexpr real_t scalar = 1.5;
    constexpr Point point(20, 30);

    constexpr Point result = scalar * point;

    REQUIRE(result.x == 30);
    REQUIRE(result.y == 45);

    // Compile-time checks
    static_assert(result.x == 30);
    static_assert(result.y == 45);
  }

  SECTION("Multiplication with real scalar zero") {
    constexpr Point point(10, 20);
    constexpr real_t scalar = 0.0;

    constexpr Point result = point * scalar;

    REQUIRE(result.x == 0);
    REQUIRE(result.y == 0);

    // Compile-time checks
    static_assert(result.x == 0);
    static_assert(result.y == 0);
  }

  SECTION("Multiplication with real scalar negative") {
    constexpr Point point(10, 20);
    constexpr real_t scalar = -0.5;

    constexpr Point result = point * scalar;

    REQUIRE(result.x == -5);
    REQUIRE(result.y == -10);

    // Compile-time checks
    static_assert(result.x == -5);
    static_assert(result.y == -10);
  }

  SECTION("Division with integer scalar") {
    constexpr Point point(30, 60);
    constexpr int32_t scalar = 3;

    constexpr Point result = point / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    static_assert(result.x == 10);
    static_assert(result.y == 20);
  }

  SECTION("Division with negative scalar") {
    constexpr Point point(-20, -40);
    constexpr int32_t scalar = -2;

    constexpr Point result = point / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    static_assert(result.x == 10);
    static_assert(result.y == 20);
  }

  SECTION("Division with real scalar") {
    constexpr Point point(25, 50);
    constexpr real_t scalar = 2.5;

    constexpr Point result = point / scalar;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 20);

    // Compile-time checks
    static_assert(result.x == 10);
    static_assert(result.y == 20);
  }

  SECTION("Division with real scalar negative") {
    constexpr Point point(-30, -60);
    constexpr real_t scalar = -1.5;

    constexpr Point result = point / scalar;

    REQUIRE(result.x == 20);
    REQUIRE(result.y == 40);

    // Compile-time checks
    static_assert(result.x == 20);
    static_assert(result.y == 40);
  }

  SECTION("Equality operator with identical points") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    constexpr bool result = point1 == point2;

    REQUIRE(result);

    // Compile-time checks
    static_assert(result);
  }

  SECTION("Equality operator with different points") {
    constexpr Point point1(10, 20);
    constexpr Point point2(11, 20);

    constexpr bool result = point1 == point2;

    REQUIRE(result == false);

    // Compile-time checks
    static_assert(result == false);
  }

  SECTION("Equality operator with zero coordinates") {
    constexpr Point point1(0, 0);
    constexpr Point point2(0, 0);

    constexpr bool result = point1 == point2;

    REQUIRE(result);

    // Compile-time checks
    static_assert(result);
  }

  SECTION("Equality operator with negative coordinates") {
    constexpr Point point1(-10, -20);
    constexpr Point point2(-10, -20);

    constexpr bool result = point1 == point2;

    REQUIRE(result);

    // Compile-time checks
    static_assert(result);
  }

  SECTION("Equality operator with mixed coordinates") {
    constexpr Point point1(10, -20);
    constexpr Point point2(10, -20);

    constexpr bool result = point1 == point2;

    REQUIRE(result);

    // Compile-time checks
    static_assert(result);
  }

  SECTION("Chained binary operations") {
    constexpr Point point1(10, 20);
    constexpr Point point2(5, 10);
    constexpr int32_t scalar = 2;

    constexpr Point result = (point1 + point2) * scalar - point1;

    REQUIRE(result.x == 20); // ((10+5)*2 - 10) = 30 - 10 = 20
    REQUIRE(result.y == 40); // ((20+10)*2 - 20) = 60 - 20 = 40

    // Compile-time checks
    static_assert(result.x == 20);
    static_assert(result.y == 40);
  }

  SECTION("Complex chained operations") {
    constexpr Point point1(100, 200);
    constexpr Point point2(50, 75);
    constexpr real_t scalar1 = 1.5;
    constexpr int32_t scalar2 = 2;

    constexpr Point result = (point1 * scalar1 + point2) / scalar2;

    REQUIRE(result.x == 100); // ((100*1.5 + 50) / 2) = (150 + 50) / 2 = 100
    REQUIRE(result.y == 187); // ((200*1.5 + 75) / 2) = (300 + 75) / 2 = 187

    // Compile-time checks
    static_assert(result.x == 100);
    static_assert(result.y == 187);
  }

  SECTION("Chained tests") {
    // Test all binary operators at runtime
    constexpr Point point1(10, 20);
    constexpr Point point2(5, -10);

    // Unary minus
    constexpr Point negated = -point1;
    REQUIRE(negated.x == -10);
    REQUIRE(negated.y == -20);

    // Addition
    constexpr Point sum = negated + point2;
    REQUIRE(sum.x == -5);
    REQUIRE(sum.y == -30);

    // Subtraction
    constexpr Point diff = sum - point2;
    REQUIRE(diff.x == -10);
    REQUIRE(diff.y == -20);

    // Multiplication with integer
    constexpr Point multInt = diff * 3;
    REQUIRE(multInt.x == -30);
    REQUIRE(multInt.y == -60);

    // Multiplication with real
    constexpr Point multReal = multInt * 2.5f;
    REQUIRE(multReal.x == -75);
    REQUIRE(multReal.y == -150);

    // Division with integer
    constexpr Point divInt = multReal / 3;
    REQUIRE(divInt.x == -25);
    REQUIRE(divInt.y == -50);

    // Division with real
    constexpr Point divReal = divInt / 2.5f;
    REQUIRE(divReal.x == -10);
    REQUIRE(divReal.y == -20);

    // Compile-time checks
    static_assert(negated.x == -10);
    static_assert(negated.y == -20);
    static_assert(sum.x == -5);
    static_assert(sum.y == -30);
    static_assert(diff.x == -10);
    static_assert(diff.y == -20);
    static_assert(multInt.x == -30);
    static_assert(multInt.y == -60);
    static_assert(multReal.x == -75);
    static_assert(multReal.y == -150);
    static_assert(divInt.x == -25);
    static_assert(divInt.y == -50);
    static_assert(divReal.x == -10);
    static_assert(divReal.y == -20);
  }
}
