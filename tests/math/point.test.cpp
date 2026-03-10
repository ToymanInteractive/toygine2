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

// Point has fixed size and contiguous x,y layout.
TEST_CASE("math/point/object_structure") {
  constexpr Point point(111, 222);

  static_assert(sizeof(point) == sizeof(int32_t) * 2, "Point must have size of two int32_t");

  const auto * arr = point.c_arr();

  REQUIRE(arr == &point.x);
  REQUIRE(arr + 1 == &point.y);
  REQUIRE(reinterpret_cast<const byte *>(arr + 1) - reinterpret_cast<const byte *>(arr) == sizeof(int32_t));
}

// Default, coordinate, and array constructors.
TEST_CASE("math/point/constructors") {
  // Default constructor yields zero x, y.
  SUBCASE("default_constructor") {
    constexpr Point point;

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    static_assert(Point().x == 0, "default-constructed x must be 0");
    static_assert(Point().y == 0, "default-constructed y must be 0");
  }

  // Constructor with coordinates.
  SUBCASE("constructor_with_coordinates") {
    constexpr Point point(10, 20);

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);

    static_assert(Point(-5, -15).x == -5, "x must match constructor argument");
    static_assert(Point(-5, -15).y == -15, "y must match constructor argument");
  }

  // Constructor from pointer to two-element array.
  SUBCASE("constructor_from_pointer_to_array") {
    constexpr array<int32_t, 2> arr{{42, -17}};
    constexpr Point point(arr.data());

    REQUIRE(point.x == 42);
    REQUIRE(point.y == -17);

    static_assert(point.x == 42, "x must match array element");
    static_assert(point.y == -17, "y must match array element");
  }

  // Runtime constructor behavior.
  SUBCASE("runtime_constructor_tests") {
    Point defaultPoint;
    REQUIRE(defaultPoint.x == 0);
    REQUIRE(defaultPoint.y == 0);

    Point coordPoint(123, -456);
    REQUIRE(coordPoint.x == 123);
    REQUIRE(coordPoint.y == -456);

    constexpr array<int32_t, 2> arr{{789, -321}};
    Point arrayPoint(arr.data());
    REQUIRE(arrayPoint.x == 789);
    REQUIRE(arrayPoint.y == -321);
  }
}

// c_arr returns pointer to contiguous x,y.
TEST_CASE("math/point/c_arr_methods") {
  // c_arr() returns pointer to x, y; layout is contiguous.
  SUBCASE("c_arr_layout") {
    constexpr Point point(10, 20);
    const auto * arr = point.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr == &point.x);
    REQUIRE(arr[0] == 10);
    REQUIRE(arr[1] == 20);

    static_assert(Point(42, -17).c_arr()[0] == 42, "first element must match x");
  }

  // Non-const c_arr() allows modification of x, y.
  SUBCASE("c_arr_modification") {
    Point point(1, 2);
    auto * arr = point.c_arr();

    REQUIRE(arr != nullptr);

    arr[0] = 10;
    arr[1] = 20;

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);
  }

  // Const c_arr() returns read-only pointer.
  SUBCASE("const_c_arr") {
    constexpr Point point(123, -456);
    const auto * arr = point.c_arr();

    REQUIRE(arr != nullptr);
    REQUIRE(arr[0] == 123);
    REQUIRE(arr[1] == -456);
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

  // operator*= with floating-point scalar (float, double, long double).
  SUBCASE("operator_times_assign_with_floating") {
    Point point1(10, 20);
    Point point2(10, 20);
    Point point3(10, 20);

    point1 *= 2.5f;
    point2 *= 2.5;
    point3 *= 2.5L;

    REQUIRE(point1.x == 25);
    REQUIRE(point1.y == 50);
    REQUIRE(point2.x == 25);
    REQUIRE(point2.y == 50);
    REQUIRE(point3.x == 25);
    REQUIRE(point3.y == 50);
  }

  // operator*= with fixed_point scalar.
  SUBCASE("operator_times_assign_with_fixed") {
    Point point(10, 20);
    constexpr Fixed scalar(2);

    point *= scalar;

    REQUIRE(point.x == 20);
    REQUIRE(point.y == 40);
  }

  // operator/= with integer scalar.
  SUBCASE("operator_div_assign_with_integer") {
    Point point(30, 60);
    constexpr int32_t scalar = 3;

    point /= scalar;

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);
  }

  // operator/= with floating-point scalar (float, double, long double).
  SUBCASE("operator_div_assign_with_floating") {
    Point point1(20, 45);
    Point point2(20, 45);
    Point point3(20, 45);

    point1 /= 2.5f;
    point2 /= 2.5;
    point3 /= 2.5L;

    REQUIRE(point1.x == 8);
    REQUIRE(point1.y == 18);
    REQUIRE(point2.x == 8);
    REQUIRE(point2.y == 18);
    REQUIRE(point3.x == 8);
    REQUIRE(point3.y == 18);
  }

  // operator/= with fixed_point scalar.
  SUBCASE("operator_div_assign_with_fixed") {
    Point point(20, 40);
    constexpr Fixed scalar(2);

    point /= scalar;

    REQUIRE(point.x == 10);
    REQUIRE(point.y == 20);
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
  // sqrMagnitude() returns x² + y².
  SUBCASE("sqr_magnitude") {
    constexpr Point point(3, 4);

    REQUIRE(point.sqrMagnitude() == 25);

    static_assert(point.sqrMagnitude() == 25, "sqrMagnitude of (3,4) must be 25");
    static_assert(Point().sqrMagnitude() == 0, "sqrMagnitude of origin must be 0");
  }

  // setZero() sets x, y to zero.
  SUBCASE("set_zero") {
    Point point(100, 200);

    point.setZero();

    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);
  }

  // isZero() true for (0,0), false otherwise.
  SUBCASE("is_zero") {
    constexpr Point origin(0, 0);
    constexpr Point nonZero(1, 0);

    REQUIRE(origin.isZero());
    REQUIRE(!nonZero.isZero());
    REQUIRE(Point().isZero());

    static_assert(origin.isZero(), "origin must be zero");
    static_assert(!nonZero.isZero(), "non-zero point must not be zero");
    static_assert(Point().isZero());

    Point p(10, 20);
    REQUIRE(!p.isZero());
    p.setZero();
    REQUIRE(p.isZero());
  }

  // isEqual() exact and with tolerance.
  SUBCASE("is_equal") {
    constexpr Point point1(10, 20);
    constexpr Point point2(10, 20);
    constexpr Point point3(11, 20);
    constexpr Point point4(12, 18);
    constexpr int32_t tolerance = 3;

    REQUIRE(point1.isEqual(point2));
    REQUIRE(!point1.isEqual(point3));
    REQUIRE(point1.isEqual(point4, tolerance));
    REQUIRE(!point1.isEqual(Point(15, 25), 2));

    static_assert(point1.isEqual(point2));
    static_assert(!point1.isEqual(point3));
    static_assert(point1.isEqual(point4, tolerance));
    static_assert(!point1.isEqual(Point(15, 25), 2));

    constexpr Point a(10, 20);
    constexpr Point b(10, 21);

    REQUIRE(!a.isEqual(b));

    static_assert(!a.isEqual(b));
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
  SUBCASE("unary_minus") {
    constexpr Point point(10, -20);
    constexpr auto result = -point;

    REQUIRE(result.x == -10);
    REQUIRE(result.y == 20);

    static_assert(result.x == -10, "unary minus must negate x");
    static_assert(result.y == 20, "unary minus must negate y");
    static_assert((-Point()).x == 0, "unary minus of origin must remain 0");
  }

  // operator+ adds vectors.
  SUBCASE("addition") {
    constexpr Point point1(10, 20);
    constexpr Point point2(5, -10);

    constexpr auto result = point1 + point2;

    REQUIRE(result.x == 15);
    REQUIRE(result.y == 10);

    static_assert(result.x == 15, "addition x must be sum of x components");
    static_assert(result.y == 10, "addition y must be sum of y components");
  }

  // operator- subtracts vectors.
  SUBCASE("subtraction") {
    constexpr Point point1(15, 25);
    constexpr Point point2(5, 10);

    constexpr auto result = point1 - point2;

    REQUIRE(result.x == 10);
    REQUIRE(result.y == 15);

    static_assert(result.x == 10, "subtraction x must be difference of x components");
    static_assert(result.y == 15, "subtraction y must be difference of y components");
  }

  // Point * integer scalar and integer * point.
  SUBCASE("multiplication_with_integer_scalar") {
    constexpr Point point(10, 20);
    constexpr int32_t scalar = 3;

    constexpr auto result = point * scalar;

    REQUIRE(result.x == 30);
    REQUIRE(result.y == 60);

    static_assert(result.x == 30, "point * scalar must scale x");
    static_assert(result.y == 60, "scalar * point must scale y");
  }

  // Multiplication with zero or negative scalar.
  SUBCASE("multiplication_zero_and_negative_scalar") {
    constexpr Point point(10, 20);

    constexpr auto zeroResult = point * 0;

    REQUIRE(zeroResult.x == 0);
    REQUIRE(zeroResult.y == 0);

    static_assert(zeroResult.x == 0);
    static_assert(zeroResult.y == 0);

    constexpr auto negResult = point * int32_t(-2);

    REQUIRE(negResult.x == -20);
    REQUIRE(negResult.y == -40);

    static_assert(negResult.x == -20);
    static_assert(negResult.y == -40);

    constexpr auto negFloatResult = point * (-0.5f);

    REQUIRE(negFloatResult.x == -5);
    REQUIRE(negFloatResult.y == -10);

    static_assert(negFloatResult.x == -5);
    static_assert(negFloatResult.y == -10);

    constexpr auto negDoubleResult = point * (-1.5);

    REQUIRE(negDoubleResult.x == -15);
    REQUIRE(negDoubleResult.y == -30);

    static_assert(negDoubleResult.x == -15);
    static_assert(negDoubleResult.y == -30);

    constexpr auto negLongDoubleResult = point * (-2.0L);

    REQUIRE(negLongDoubleResult.x == -20);
    REQUIRE(negLongDoubleResult.y == -40);

    static_assert(negLongDoubleResult.x == -20);
    static_assert(negLongDoubleResult.y == -40);

    constexpr auto negFixedResult = point * Fixed(-2);

    REQUIRE(negFixedResult.x == -20);
    REQUIRE(negFixedResult.y == -40);

    static_assert(negFixedResult.x == -20);
    static_assert(negFixedResult.y == -40);

    REQUIRE((-2 * point).x == -20);
    REQUIRE((-2 * point).y == -40);
    REQUIRE((-1.5f * point).x == -15);
    REQUIRE((-1.5f * point).y == -30);
    REQUIRE((Fixed(-3) * point).x == -30);
    REQUIRE((Fixed(-3) * point).y == -60);

    static_assert((-2 * point).x == -20);
    static_assert((-2 * point).y == -40);
    static_assert((-1.5f * point).x == -15);
    static_assert((-1.5f * point).y == -30);
    static_assert((Fixed(-3) * point).x == -30);
    static_assert((Fixed(-3) * point).y == -60);
  }

  // Point * float, double, long double and Fixed; scalar * point.
  SUBCASE("multiplication_point_times_floating_and_fixed") {
    constexpr Point p(10, 20);

    REQUIRE((p * 2.5f).x == 25);
    REQUIRE((p * 2.5f).y == 50);
    REQUIRE((p * 2.5).x == 25);
    REQUIRE((p * 2.5L).y == 50);

    static_assert((p * 2.5f).x == 25);
    static_assert((p * 2.5f).y == 50);
    static_assert((p * 2.5).x == 25);
    static_assert((p * 2.5L).y == 50);

    constexpr auto fixedResult = p * Fixed(2);

    REQUIRE(fixedResult.x == 20);
    REQUIRE(fixedResult.y == 40);

    static_assert(fixedResult.x == 20);
    static_assert(fixedResult.y == 40);

    REQUIRE((1.5f * Point(20, 30)).x == 30);
    REQUIRE((1.5 * Point(20, 40)).y == 60);
    REQUIRE((Fixed(3) * Point(5, 10)).x == 15);
    REQUIRE((Fixed(3) * Point(5, 10)).y == 30);

    static_assert((1.5f * Point(20, 30)).x == 30);
    static_assert((1.5 * Point(20, 40)).y == 60);
    static_assert((Fixed(3) * Point(5, 10)).x == 15);
    static_assert((Fixed(3) * Point(5, 10)).y == 30);
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

    constexpr Point negPoint(-20, -40);
    constexpr auto negDivResult = negPoint / int32_t(-2);

    REQUIRE(negDivResult.x == 10);
    REQUIRE(negDivResult.y == 20);

    static_assert(negDivResult.x == 10);
    static_assert(negDivResult.y == 20);
  }

  // Division by float, double, long double and Fixed.
  SUBCASE("division_by_floating_and_fixed") {
    constexpr Point point(25, 50);

    REQUIRE((point / 2.5f).x == 10);
    REQUIRE((point / 2.5f).y == 20);
    REQUIRE((point / 2.5).x == 10);
    REQUIRE((point / 2.5L).y == 20);

    static_assert((point / 2.5f).x == 10);
    static_assert((point / 2.5f).y == 20);
    static_assert((point / 2.5).x == 10);
    static_assert((point / 2.5L).y == 20);

    constexpr Point p2(30, 60);

    REQUIRE((p2 / Fixed(3)).x == 10);
    REQUIRE((p2 / Fixed(3)).y == 20);

    static_assert((p2 / Fixed(3)).x == 10);
    static_assert((p2 / Fixed(3)).y == 20);

    constexpr Point neg(-30, -60);

    REQUIRE((neg / -1.5f).x == 20);
    REQUIRE((neg / -1.5f).y == 40);

    static_assert((neg / -1.5f).x == 20);
    static_assert((neg / -1.5f).y == 40);

    constexpr Point pos(25, 50);

    REQUIRE((pos / -2.5f).x == -10);
    REQUIRE((pos / -2.5f).y == -20);

    static_assert((pos / -2.5f).x == -10);
    static_assert((pos / -2.5f).y == -20);

    REQUIRE((Point(30, 60) / -2.0).x == -15);
    REQUIRE((Point(30, 60) / -2.0).y == -30);
    REQUIRE((Point(-60, -90) / Fixed(-3)).x == 20);
    REQUIRE((Point(-60, -90) / Fixed(-3)).y == 30);

    static_assert((Point(30, 60) / -2.0).x == -15);
    static_assert((Point(30, 60) / -2.0).y == -30);
    static_assert((Point(-60, -90) / Fixed(-3)).x == 20);
    static_assert((Point(-60, -90) / Fixed(-3)).y == 30);
  }

  // operator== and operator!=.
  SUBCASE("equality_operator") {
    constexpr Point a(10, 20);
    constexpr Point b(10, 20);
    constexpr Point c(11, 20);

    REQUIRE(a == b);
    REQUIRE(a != c);

    static_assert(a == b);
    static_assert(a != c);
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

  // Complex chained with float and integer scalars.
  SUBCASE("complex_chained_operations") {
    constexpr Point point1(100, 200);
    constexpr Point point2(50, 75);
    constexpr float scalar1 = 1.5f;
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

    REQUIRE(divReal.x == -10);
    REQUIRE(divReal.y == -20);

    static_assert((-Point(10, 20) + Point(5, -10) - Point(5, -10)).x == -10, "chained ops x");
    static_assert((Point(-10, -20) * 3 * 2.5f / 3 / 2.5f).y == -20, "chained ops y");
  }
}

} // namespace toy::math
