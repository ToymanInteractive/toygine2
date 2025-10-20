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
    const Point constRuntimeVector(900, -1000);
    const auto * constRuntimeArr = constRuntimeVector.c_arr();

    REQUIRE(constRuntimeArr != nullptr);
    REQUIRE(constRuntimeArr[0] == 900);
    REQUIRE(constRuntimeArr[1] == -1000);
  }
}
