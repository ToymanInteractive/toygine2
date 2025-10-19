//
// Copyright (c) 2025 by Toyman Interactive
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

#include "core.hpp"
#include "math.hpp"

using namespace toy::math;

TEST_CASE("Point object structure", "[math][point]") {
  constexpr Point point(111, 222);

  STATIC_REQUIRE(sizeof(point) == sizeof(Point::value_type) * 2);

  const auto * arr = point.c_arr();

  // Verify memory layout
  REQUIRE(arr == &point.x);
  REQUIRE(arr + 1 == &point.y);

  // Verify contiguous memory
  REQUIRE(reinterpret_cast<const char *>(arr + 1) - reinterpret_cast<const char *>(arr) == sizeof(Point::value_type));
}

TEST_CASE("Point constructors", "[math][point]") {
  SECTION("Default constructor") {
    constexpr Point point;

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    // Compile-time checks
    STATIC_REQUIRE(point.x == 0);
    STATIC_REQUIRE(point.y == 0);
  }

  SECTION("Constructor with positive coordinates") {
    constexpr Point point(10, 20);

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);

    // Compile-time checks
    STATIC_REQUIRE(point.x == 10);
    STATIC_REQUIRE(point.y == 20);
  }

  SECTION("Constructor with negative coordinates") {
    constexpr Point point(-5, -15);

    REQUIRE(point.x == -5);
    REQUIRE(point.y == -15);

    // Compile-time checks
    STATIC_REQUIRE(point.x == -5);
    STATIC_REQUIRE(point.y == -15);
  }

  SECTION("Constructor with mixed coordinates") {
    constexpr Point point(-100, 200);

    REQUIRE(point.x == -100);
    REQUIRE(point.y == 200);

    // Compile-time checks
    STATIC_REQUIRE(point.x == -100);
    STATIC_REQUIRE(point.y == 200);
  }

  SECTION("Constructor with zero coordinates") {
    constexpr Point point(0, 0);

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    // Compile-time checks
    STATIC_REQUIRE(point.x == 0);
    STATIC_REQUIRE(point.y == 0);
  }

  SECTION("Constructor from array") {
    constexpr Point point({42, -17});

    REQUIRE(point.x == 42);
    REQUIRE(point.y == -17);

    // Compile-time checks
    STATIC_REQUIRE(point.x == 42);
    STATIC_REQUIRE(point.y == -17);
  }

  SECTION("Constructor from array with positive values") {
    constexpr Point point({100, 200});

    REQUIRE(point.x == 100);
    REQUIRE(point.y == 200);

    // Compile-time checks
    STATIC_REQUIRE(point.x == 100);
    STATIC_REQUIRE(point.y == 200);
  }

  SECTION("Constructor from array with negative values") {
    constexpr Point point({-50, -75});

    REQUIRE(point.x == -50);
    REQUIRE(point.y == -75);

    // Compile-time checks
    STATIC_REQUIRE(point.x == -50);
    STATIC_REQUIRE(point.y == -75);
  }

  SECTION("Constructor from array with mixed values") {
    constexpr Point point({-300, 400});

    REQUIRE(point.x == -300);
    REQUIRE(point.y == 400);

    // Compile-time checks
    STATIC_REQUIRE(point.x == -300);
    STATIC_REQUIRE(point.y == 400);
  }

  SECTION("Constructor from array with zero values") {
    constexpr Point point({0, 0});

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    // Compile-time checks
    STATIC_REQUIRE(point.x == 0);
    STATIC_REQUIRE(point.y == 0);
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
    Point arrayPoint({789, -321});
    REQUIRE(arrayPoint.x == 789);
    REQUIRE(arrayPoint.y == -321);
  }

  SECTION("Edge cases") {
    // Test with maximum positive values
    constexpr Point maxPoint(2147483647, 2147483647);
    REQUIRE(maxPoint.x == 2147483647);
    REQUIRE(maxPoint.y == 2147483647);

    // Test with minimum negative values
    constexpr Point minPoint(-2147483648, -2147483648);
    REQUIRE(minPoint.x == -2147483648);
    REQUIRE(minPoint.y == -2147483648);

    // Test with mixed extreme values
    constexpr Point mixedPoint(2147483647, -2147483648);
    REQUIRE(mixedPoint.x == 2147483647);
    REQUIRE(mixedPoint.y == -2147483648);
  }

  SECTION("Constexpr operations") {
    // Test constexpr operations with constructors
    constexpr Point p1(10, 20);
    constexpr Point p2(-5, -15);
    constexpr Point p3({100, -200});

    // Verify all points are properly constructed
    STATIC_REQUIRE(p1.x == 10);
    STATIC_REQUIRE(p1.y == 20);
    STATIC_REQUIRE(p2.x == -5);
    STATIC_REQUIRE(p2.y == -15);
    STATIC_REQUIRE(p3.x == 100);
    STATIC_REQUIRE(p3.y == -200);
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
    const Point point(123, -456);
    const auto * arr = point.c_arr();

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
    STATIC_REQUIRE(point.c_arr()[0] == 0);
  }

  SECTION("c_arr with coordinate constructor") {
    constexpr Point point(10, 20);

    REQUIRE(point.c_arr()[0] == 10);
    REQUIRE(point.c_arr()[1] == 20);

    // Compile-time checks
    STATIC_REQUIRE(point.c_arr()[0] == 10);
  }

  SECTION("c_arr with array constructor") {
    constexpr Point point({-50, 75});

    REQUIRE(point.c_arr()[0] == -50);
    REQUIRE(point.c_arr()[1] == 75);

    // Compile-time checks
    STATIC_REQUIRE(point.c_arr()[0] == -50);
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
    constexpr Point::value_type scalar = 3;

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
    constexpr Point::value_type scalar = 3;

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
    const Point offset(5, 10);

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
    STATIC_REQUIRE(magnitude == 25);
  }

  SECTION("sqrMagnitude with negative coordinates") {
    constexpr Point point(-3, -4);
    constexpr auto magnitude = point.sqrMagnitude();

    REQUIRE(magnitude == 25);
    STATIC_REQUIRE(magnitude == 25);
  }

  SECTION("sqrMagnitude with zero coordinates") {
    constexpr Point point(0, 0);
    constexpr auto magnitude = point.sqrMagnitude();

    REQUIRE(magnitude == 0);
    STATIC_REQUIRE(magnitude == 0);
  }

  SECTION("sqrMagnitude with large coordinates") {
    constexpr Point point(1000, 2000);
    constexpr auto magnitude = point.sqrMagnitude();

    REQUIRE(magnitude == 5000000);
    STATIC_REQUIRE(magnitude == 5000000);
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

    REQUIRE(result == true);
    STATIC_REQUIRE(result == true);
  }

  SECTION("isZero with non-zero point") {
    constexpr Point point(1, 0);
    constexpr bool result = point.isZero();

    REQUIRE(result == false);
    STATIC_REQUIRE(result == false);
  }

  SECTION("isZero with negative coordinates") {
    constexpr Point point(-1, -1);
    constexpr bool result = point.isZero();

    REQUIRE(result == false);
    STATIC_REQUIRE(result == false);
  }

  SECTION("isZero after setZero") {
    Point point(100, 200);

    REQUIRE(point.isZero() == false);

    point.setZero();

    REQUIRE(point.isZero() == true);
  }

  SECTION("isEqual with exact match") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    const auto result = point1.isEqual(point2);

    REQUIRE(result == true);
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
    constexpr Point::value_type tolerance = 3;

    const bool result = point1.isEqual(point2, tolerance);

    REQUIRE(result == true); // |10-12| <= 3 && |20-18| <= 3
  }

  SECTION("isEqual with tolerance too small") {
    constexpr Point point1(10, 20);
    constexpr Point point2(15, 25);
    constexpr Point::value_type tolerance = 2;

    const bool result = point1.isEqual(point2, tolerance);

    REQUIRE(result == false); // |10-15| > 2 || |20-25| > 2
  }

  SECTION("isEqual with zero tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 21);
    constexpr Point::value_type tolerance = 0;

    const bool result = point1.isEqual(point2, tolerance);

    REQUIRE(result == false); // Exact match required
  }

  SECTION("isEqual with default tolerance") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);

    const bool result = point1.isEqual(point2);

    REQUIRE(result == true); // Default tolerance is 0, exact match
  }

  SECTION("isEqual with large tolerance") {
    constexpr Point point1(0, 0);
    constexpr Point point2(1000, 1000);
    constexpr Point::value_type tolerance = 2000;

    const bool result = point1.isEqual(point2, tolerance);

    REQUIRE(result == true); // |0-1000| <= 2000 && |0-1000| <= 2000
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
    REQUIRE(point.isZero() == true);
    REQUIRE(point.sqrMagnitude() == 0);

    // Test isEqual at runtime
    constexpr Point testPoint(5, 5);
    REQUIRE(point.isEqual(testPoint) == false);
    REQUIRE(point.isEqual(testPoint, 10) == true);
  }
}
