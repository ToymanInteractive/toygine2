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
/*!
  \file   circle.test.cpp
  \brief  Unit tests for \ref toy::geometry::Circle.
*/

#include <doctest/doctest.h>

#include "geometry.hpp"

namespace toy::geometry {

using fixed_type = math::fixed<int32_t, int64_t, 20>;

static_assert(CircleComponent<float>, "float must satisfy CircleComponent");
static_assert(CircleComponent<fixed_type>, "math::fixed must satisfy CircleComponent");
static_assert(!CircleComponent<int>, "int must not satisfy CircleComponent");

// Circle has fixed size and contiguous layout.
TEST_CASE("geometry/circle/object_structure") {
  static_assert(sizeof(Circle<float>) == sizeof(float) * 3, "Circle size must be 3× component size");
  static_assert(sizeof(Circle<fixed_type>) == sizeof(fixed_type) * 3, "Circle size must be 3× component size");

  static_assert(!std::is_trivial_v<Circle<float>>, "Circle<float> must not be trivial");
  static_assert(!std::is_trivial_v<Circle<fixed_type>>, "Circle<fixed_type> must not be trivial");
  static_assert(std::is_trivially_copyable_v<Circle<float>>, "Circle<float> must be trivially copyable");
  static_assert(std::is_trivially_copyable_v<Circle<fixed_type>>, "Circle<fixed_type> must be trivially copyable");
  static_assert(std::is_trivially_destructible_v<Circle<float>>, "Circle<float> must be trivially destructible");
  static_assert(std::is_trivially_destructible_v<Circle<fixed_type>>,
                "Circle<fixed_type> must be trivially destructible");
  static_assert(std::is_standard_layout_v<Circle<float>>, "Circle<float> must have standard layout");
  static_assert(std::is_standard_layout_v<Circle<fixed_type>>, "Circle<fixed_type> must have standard layout");
}

// Constructor (center, radius) stores center and radius; valid when radius > 0.
TEST_CASE("geometry/circle/constructor_center_radius") {
  // Floating-point component type.
  SUBCASE("float") {
    constexpr Circle c(math::Vector2(5.0f, 10.0f), 3.0f);

    REQUIRE(c.center.x == doctest::Approx(5.0f));
    REQUIRE(c.center.y == doctest::Approx(10.0f));
    REQUIRE(c.radius == doctest::Approx(3.0f));
    REQUIRE(c.isValid());
    REQUIRE(!c.isReset());

    static_assert(math::isEqual(c.center.x, 5.0f), "constructor must store center.x");
    static_assert(math::isEqual(c.center.y, 10.0f), "constructor must store center.y");
    static_assert(math::isEqual(c.radius, 3.0f), "constructor must store radius");
    static_assert(c.isValid(), "circle with positive radius must be valid");
    static_assert(!c.isReset(), "circle with positive radius must not be reset");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    constexpr Circle c(math::Vector2(fixed_type(5), fixed_type(10)), fixed_type(3));

    REQUIRE(c.center.x == 5);
    REQUIRE(c.center.y == 10);
    REQUIRE(c.radius == 3);
    REQUIRE(c.isValid());
    REQUIRE(!c.isReset());

    static_assert(c.center.x == 5, "constructor must store center.x");
    static_assert(c.center.y == 10, "constructor must store center.y");
    static_assert(c.radius == 3, "constructor must store radius");
    static_assert(c.isValid(), "circle with positive radius must be valid");
    static_assert(!c.isReset(), "circle with positive radius must not be reset");
  }
}

// area() returns π * radius².
TEST_CASE("geometry/circle/area") {
  // Floating-point component type.
  SUBCASE("float") {
    constexpr Circle c(math::Vector2(0.0f, 0.0f), 10.0f);

    REQUIRE(c.area() == doctest::Approx(314.159265f));

    static_assert(math::isEqual(c.area(), 314.159265f), "area of radius 10 must be π*100");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    constexpr Circle c(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(10));

    REQUIRE(math::isEqual(c.area(), fixed_type(314.159265f)));

    static_assert(math::isEqual(c.area(), fixed_type(314.159265f)), "area of radius 10 must be π*100");
  }
}

// reset() sets center to zero and radius to zero.
TEST_CASE("geometry/circle/reset") {
  // Floating-point component type.
  SUBCASE("float") {
    Circle c(math::Vector2(10.0f, 20.0f), 5.0f);

    c.reset();

    REQUIRE(c.center.isZero());
    REQUIRE(math::isEqual(c.radius, 0.0f));
    REQUIRE(c.isReset());
    REQUIRE(!c.isValid());
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    Circle c(math::Vector2(fixed_type(10), fixed_type(20)), fixed_type(5));

    c.reset();

    REQUIRE(c.center.isZero());
    REQUIRE(c.radius == 0);
    REQUIRE(c.isReset());
    REQUIRE(!c.isValid());
  }
}

// isReset() true after reset, false when radius > 0.
TEST_CASE("geometry/circle/is_reset") {
  // Floating-point component type.
  SUBCASE("float") {
    Circle<float>    cZero;
    constexpr Circle cPos(math::Vector2(0.0f, 0.0f), 1.0f);

    cZero.reset();

    REQUIRE(cZero.isReset());
    REQUIRE(!cPos.isReset());

    static_assert(!cPos.isReset(), "circle with positive radius must not be reset");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    Circle<fixed_type> cZero;
    constexpr Circle   cPos(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(1));

    cZero.reset();

    REQUIRE(cZero.isReset());
    REQUIRE(!cPos.isReset());

    static_assert(!cPos.isReset(), "circle with positive radius must not be reset");
  }
}

// isValid() true when radius > 0.
TEST_CASE("geometry/circle/is_valid") {
  // Floating-point component type.
  SUBCASE("float") {
    Circle<float>    cZero;
    constexpr Circle cPos(math::Vector2(0.0f, 0.0f), 1.0f);

    cZero.reset();

    REQUIRE(!cZero.isValid());
    REQUIRE(cPos.isValid());

    static_assert(cPos.isValid(), "circle with positive radius must be valid");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    Circle<fixed_type> cZero;
    constexpr Circle   cPos(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(1));

    cZero.reset();

    REQUIRE(!cZero.isValid());
    REQUIRE(cPos.isValid());

    static_assert(cPos.isValid(), "circle with positive radius must be valid");
  }
}

// isContain() true when point inside or on circle, false when outside.
TEST_CASE("geometry/circle/is_contain") {
  // Floating-point component type.
  SUBCASE("float") {
    constexpr Circle c(math::Vector2(0.0f, 0.0f), 10.0f);

    REQUIRE(c.isContain(math::Vector2(0.0f, 0.0f)));
    REQUIRE(c.isContain(math::Vector2(5.0f, 0.0f)));
    REQUIRE(c.isContain(math::Vector2(10.0f, 0.0f)));
    REQUIRE(!c.isContain(math::Vector2(10.01f, 0.0f)));

    static_assert(c.isContain(math::Vector2(0.0f, 0.0f)), "center must be contained");
    static_assert(c.isContain(math::Vector2(5.0f, 0.0f)), "point inside must be contained");
    static_assert(c.isContain(math::Vector2(10.0f, 0.0f)), "point on boundary must be contained");
    static_assert(!c.isContain(math::Vector2(10.01f, 0.0f)), "point outside must not be contained");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    constexpr Circle c(math::Vector2(fixed_type(0), fixed_type(0)), fixed_type(10));

    REQUIRE(c.isContain(math::Vector2(fixed_type(0), fixed_type(0))));
    REQUIRE(c.isContain(math::Vector2(fixed_type(5), fixed_type(0))));
    REQUIRE(c.isContain(math::Vector2(fixed_type(10), fixed_type(0))));
    REQUIRE(!c.isContain(math::Vector2(fixed_type(11), fixed_type(0))));

    static_assert(c.isContain(math::Vector2(fixed_type(0), fixed_type(0))), "center must be contained");
    static_assert(c.isContain(math::Vector2(fixed_type(5), fixed_type(0))), "point inside must be contained");
    static_assert(c.isContain(math::Vector2(fixed_type(10), fixed_type(0))), "point on boundary must be contained");
    static_assert(!c.isContain(math::Vector2(fixed_type(11), fixed_type(0))), "point outside must not be contained");
  }
}

// operator== and operator!= compare center and radius.
TEST_CASE("geometry/circle/operator_equality") {
  // Identical center and radius yield equality.
  SUBCASE("equal") {
    constexpr Circle a(math::Vector2(1.0f, 2.0f), 3.0f);
    constexpr Circle b(math::Vector2(1.0f, 2.0f), 3.0f);
    constexpr Circle af(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
    constexpr Circle bf(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));

    REQUIRE(a == b);
    REQUIRE(!(a != b));
    REQUIRE(af == bf);
    REQUIRE(!(af != bf));

    static_assert(a == b, "identical circles must compare equal");
    static_assert(!(a != b), "identical circles must not be unequal");
    static_assert(af == bf, "equal fixed circles must compare equal");
    static_assert(!(af != bf), "equal fixed circles must not be unequal");
  }

  // Different center or radius yield inequality.
  SUBCASE("not_equal") {
    constexpr Circle a(math::Vector2(1.0f, 2.0f), 3.0f);
    constexpr Circle b(math::Vector2(1.0f, 2.0f), 4.0f);
    constexpr Circle c(math::Vector2(1.0f, 3.0f), 3.0f);
    constexpr Circle af(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(3));
    constexpr Circle bf(math::Vector2(fixed_type(1), fixed_type(2)), fixed_type(4));
    constexpr Circle cf(math::Vector2(fixed_type(1), fixed_type(3)), fixed_type(3));

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(!(a == b));
    REQUIRE(!(a == c));
    REQUIRE(af != bf);
    REQUIRE(af != cf);
    REQUIRE(!(af == bf));
    REQUIRE(!(af == cf));

    static_assert(a != b, "circles with different radius must be unequal");
    static_assert(a != c, "circles with different center must be unequal");
    static_assert(!(a == b), "unequal circles must not compare equal");
    static_assert(!(a == c), "unequal circles must not compare equal");
    static_assert(af != bf, "fixed circles with different radius must be unequal");
    static_assert(af != cf, "fixed circles with different center must be unequal");
    static_assert(!(af == bf), "unequal fixed circles must not compare equal");
    static_assert(!(af == cf), "unequal fixed circles must not compare equal");
  }
}

} // namespace toy::geometry
