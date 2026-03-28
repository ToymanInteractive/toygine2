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
  \file   ellipse.test.cpp
  \brief  Unit tests for \ref toy::geometry::Ellipse.
*/

#include <doctest/doctest.h>

#include "geometry.hpp"

namespace toy::geometry {

using Fixed = math::fixed<int32_t, int64_t, 20>;

static_assert(EllipseComponent<float>, "float must satisfy EllipseComponent");
static_assert(EllipseComponent<Fixed>, "Fixed must satisfy EllipseComponent");
static_assert(!EllipseComponent<int>, "int must not satisfy EllipseComponent");

// Ellipse has fixed size and contiguous layout (center + radiuses = two Vector2).
TEST_CASE("geometry/ellipse/object_structure") {
  static_assert(sizeof(Ellipse<float>) == sizeof(float) * 4, "Ellipse size must be 4× component size");
  static_assert(sizeof(Ellipse<Fixed>) == sizeof(Fixed) * 4, "Ellipse size must be 4× component size");

  static_assert(!std::is_trivial_v<Ellipse<float>>, "Ellipse<float> must not be trivial");
  static_assert(!std::is_trivial_v<Ellipse<Fixed>>, "Ellipse<Fixed> must not be trivial");
  static_assert(std::is_trivially_copyable_v<Ellipse<float>>, "Ellipse<float> must be trivially copyable");
  static_assert(std::is_trivially_copyable_v<Ellipse<Fixed>>, "Ellipse<Fixed> must be trivially copyable");
  static_assert(std::is_standard_layout_v<Ellipse<float>>, "Ellipse<float> must have standard layout");
  static_assert(std::is_standard_layout_v<Ellipse<Fixed>>, "Ellipse<Fixed> must have standard layout");
}

// Constructor (center, radiuses) stores center and radiuses; valid when both semi-axes > 0.
TEST_CASE("geometry/ellipse/constructor_center_radiuses") {
  // Floating-point component type.
  SUBCASE("float") {
    constexpr Ellipse e(math::Vector2(5.0f, 10.0f), math::Vector2(3.0f, 4.0f));

    REQUIRE(math::isEqual(e.center.x, 5.0f));
    REQUIRE(math::isEqual(e.center.y, 10.0f));
    REQUIRE(math::isEqual(e.radiuses.x, 3.0f));
    REQUIRE(math::isEqual(e.radiuses.y, 4.0f));
    REQUIRE(e.isValid());
    REQUIRE(!e.isReset());

    static_assert(math::isEqual(e.center.x, 5.0f), "constructor must store center.x");
    static_assert(math::isEqual(e.center.y, 10.0f), "constructor must store center.y");
    static_assert(math::isEqual(e.radiuses.x, 3.0f), "constructor must store radiuses.x");
    static_assert(math::isEqual(e.radiuses.y, 4.0f), "constructor must store radiuses.y");
    static_assert(e.isValid(), "ellipse with positive semi-axes must be valid");
    static_assert(!e.isReset(), "ellipse with non-zero radiuses must not be reset");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    constexpr Ellipse e(math::Vector2(Fixed(5), Fixed(10)), math::Vector2(Fixed(3), Fixed(4)));

    REQUIRE(e.center.x == 5);
    REQUIRE(e.center.y == 10);
    REQUIRE(e.radiuses.x == 3);
    REQUIRE(e.radiuses.y == 4);
    REQUIRE(e.isValid());
    REQUIRE(!e.isReset());

    static_assert(e.center.x == 5, "constructor must store center.x");
    static_assert(e.center.y == 10, "constructor must store center.y");
    static_assert(e.radiuses.x == 3, "constructor must store radiuses.x");
    static_assert(e.radiuses.y == 4, "constructor must store radiuses.y");
    static_assert(e.isValid(), "ellipse with positive semi-axes must be valid");
    static_assert(!e.isReset(), "ellipse with non-zero radiuses must not be reset");
  }
}

// area() returns π * radiuses.x * radiuses.y.
TEST_CASE("geometry/ellipse/area") {
  // Floating-point component type.
  SUBCASE("float") {
    constexpr Ellipse e(math::Vector2(0.0f, 0.0f), math::Vector2(10.0f, 5.0f));

    REQUIRE(e.area() == doctest::Approx(157.07963268f));

    static_assert(math::isEqual(e.area(), 157.07963268f), "area with semi-axes 10 and 5 must equal π×10×5");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    constexpr Ellipse e(math::Vector2(Fixed(0), Fixed(0)), math::Vector2(Fixed(10), Fixed(5)));

    REQUIRE(math::isEqual(e.area(), Fixed(157.07963268f)));

    static_assert(math::isEqual(e.area(), Fixed(157.07963268f)), "area with semi-axes 10 and 5 must equal π×10×5");
  }
}

// reset() sets center and radiuses to zero.
TEST_CASE("geometry/ellipse/reset") {
  // Floating-point component type.
  SUBCASE("float") {
    Ellipse e(math::Vector2(10.0f, 20.0f), math::Vector2(5.0f, 3.0f));

    e.reset();

    REQUIRE(e.center.isZero());
    REQUIRE(e.radiuses.isZero());
    REQUIRE(e.isReset());
    REQUIRE(!e.isValid());
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    Ellipse e(math::Vector2(Fixed(10), Fixed(20)), math::Vector2(Fixed(5), Fixed(3)));

    e.reset();

    REQUIRE(e.center.isZero());
    REQUIRE(e.radiuses.isZero());
    REQUIRE(e.isReset());
    REQUIRE(!e.isValid());
  }
}

// isReset() true after reset, false when both semi-axes > 0.
TEST_CASE("geometry/ellipse/is_reset") {
  // Floating-point component type.
  SUBCASE("float") {
    Ellipse<float> eZero;
    constexpr Ellipse ePos(math::Vector2(0.0f, 0.0f), math::Vector2(1.0f, 2.0f));

    eZero.reset();

    REQUIRE(eZero.isReset());
    REQUIRE(!ePos.isReset());

    static_assert(!ePos.isReset(), "ellipse with positive semi-axes must not be reset");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    Ellipse<Fixed> eZero;
    constexpr Ellipse ePos(math::Vector2(Fixed(0), Fixed(0)), math::Vector2(Fixed(1), Fixed(2)));

    eZero.reset();

    REQUIRE(eZero.isReset());
    REQUIRE(!ePos.isReset());

    static_assert(!ePos.isReset(), "ellipse with positive semi-axes must not be reset");
  }
}

// isValid() true when both semi-axes > 0.
TEST_CASE("geometry/ellipse/is_valid") {
  // Floating-point component type.
  SUBCASE("float") {
    Ellipse<float> eZero;
    constexpr Ellipse ePos(math::Vector2(0.0f, 0.0f), math::Vector2(1.0f, 2.0f));

    eZero.reset();

    REQUIRE(!eZero.isValid());
    REQUIRE(ePos.isValid());

    static_assert(ePos.isValid(), "ellipse with positive semi-axes must be valid");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    Ellipse<Fixed> eZero;
    constexpr Ellipse ePos(math::Vector2(Fixed(0), Fixed(0)), math::Vector2(Fixed(1), Fixed(2)));

    eZero.reset();

    REQUIRE(!eZero.isValid());
    REQUIRE(ePos.isValid());

    static_assert(ePos.isValid(), "ellipse with positive semi-axes must be valid");
  }
}

// isContain() true when point inside or on ellipse, false when outside.
TEST_CASE("geometry/ellipse/is_contain") {
  // Floating-point: center (0,0), semi-axes (10, 5). Center inside; (10,0) and (0,5) on boundary; (11,0) outside.
  SUBCASE("float") {
    constexpr Ellipse e(math::Vector2(0.0f, 0.0f), math::Vector2(10.0f, 5.0f));

    REQUIRE(e.isContain(math::Vector2(0.0f, 0.0f)));
    REQUIRE(e.isContain(math::Vector2(5.0f, 0.0f)));
    REQUIRE(e.isContain(math::Vector2(10.0f, 0.0f)));
    REQUIRE(e.isContain(math::Vector2(0.0f, 5.0f)));
    REQUIRE(!e.isContain(math::Vector2(11.0f, 0.0f)));
    REQUIRE(!e.isContain(math::Vector2(0.0f, 6.0f)));

    static_assert(e.isContain(math::Vector2(0.0f, 0.0f)), "center must be contained");
    static_assert(e.isContain(math::Vector2(5.0f, 0.0f)), "point inside must be contained");
    static_assert(e.isContain(math::Vector2(10.0f, 0.0f)), "point on boundary (x-axis) must be contained");
    static_assert(e.isContain(math::Vector2(0.0f, 5.0f)), "point on boundary (y-axis) must be contained");
    static_assert(!e.isContain(math::Vector2(11.0f, 0.0f)), "point outside must not be contained");
    static_assert(!e.isContain(math::Vector2(0.0f, 6.0f)), "point outside must not be contained");
  }

  // Fixed-point component type.
  SUBCASE("fixed") {
    constexpr Ellipse e(math::Vector2(Fixed(0), Fixed(0)), math::Vector2(Fixed(10), Fixed(5)));

    REQUIRE(e.isContain(math::Vector2(Fixed(0), Fixed(0))));
    REQUIRE(e.isContain(math::Vector2(Fixed(5), Fixed(0))));
    REQUIRE(e.isContain(math::Vector2(Fixed(10), Fixed(0))));
    REQUIRE(e.isContain(math::Vector2(Fixed(0), Fixed(5))));
    REQUIRE(!e.isContain(math::Vector2(Fixed(11), Fixed(0))));
    REQUIRE(!e.isContain(math::Vector2(Fixed(0), Fixed(6))));

    static_assert(e.isContain(math::Vector2(Fixed(0), Fixed(0))), "center must be contained");
    static_assert(e.isContain(math::Vector2(Fixed(5), Fixed(0))), "point inside must be contained");
    static_assert(e.isContain(math::Vector2(Fixed(10), Fixed(0))), "point on boundary (x-axis) must be contained");
    static_assert(e.isContain(math::Vector2(Fixed(0), Fixed(5))), "point on boundary (y-axis) must be contained");
    static_assert(!e.isContain(math::Vector2(Fixed(11), Fixed(0))), "point outside must not be contained");
    static_assert(!e.isContain(math::Vector2(Fixed(0), Fixed(6))), "point outside must not be contained");
  }
}

// operator== and operator!= compare center and radiuses.
TEST_CASE("geometry/ellipse/operator_equality") {
  // Identical center and radiuses yield equality.
  SUBCASE("equal") {
    constexpr Ellipse a(math::Vector2(1.0f, 2.0f), math::Vector2(3.0f, 4.0f));
    constexpr Ellipse b(math::Vector2(1.0f, 2.0f), math::Vector2(3.0f, 4.0f));
    constexpr Ellipse af(math::Vector2(Fixed(1), Fixed(2)), math::Vector2(Fixed(3), Fixed(4)));
    constexpr Ellipse bf(math::Vector2(Fixed(1), Fixed(2)), math::Vector2(Fixed(3), Fixed(4)));

    REQUIRE(a == b);
    REQUIRE(!(a != b));
    REQUIRE(af == bf);
    REQUIRE(!(af != bf));

    static_assert(a == b, "identical ellipses must compare equal");
    static_assert(!(a != b), "identical ellipses must not be unequal");
    static_assert(af == bf, "equal fixed ellipses must compare equal");
    static_assert(!(af != bf), "equal fixed ellipses must not be unequal");
  }

  // Different center or radiuses yield inequality.
  SUBCASE("not_equal") {
    constexpr Ellipse a(math::Vector2(1.0f, 2.0f), math::Vector2(3.0f, 4.0f));
    constexpr Ellipse b(math::Vector2(1.0f, 2.0f), math::Vector2(3.0f, 5.0f));
    constexpr Ellipse c(math::Vector2(1.0f, 3.0f), math::Vector2(3.0f, 4.0f));
    constexpr Ellipse af(math::Vector2(Fixed(1), Fixed(2)), math::Vector2(Fixed(3), Fixed(4)));
    constexpr Ellipse bf(math::Vector2(Fixed(1), Fixed(2)), math::Vector2(Fixed(3), Fixed(5)));
    constexpr Ellipse cf(math::Vector2(Fixed(1), Fixed(3)), math::Vector2(Fixed(3), Fixed(4)));

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(!(a == b));
    REQUIRE(!(a == c));
    REQUIRE(af != bf);
    REQUIRE(af != cf);
    REQUIRE(!(af == bf));
    REQUIRE(!(af == cf));

    static_assert(a != b, "ellipses with different radiuses must be unequal");
    static_assert(a != c, "ellipses with different center must be unequal");
    static_assert(!(a == b), "unequal ellipses must not compare equal");
    static_assert(!(a == c), "unequal ellipses must not compare equal");
    static_assert(af != bf, "fixed ellipses with different radiuses must be unequal");
    static_assert(af != cf, "fixed ellipses with different center must be unequal");
    static_assert(!(af == bf), "unequal fixed ellipses must not compare equal");
    static_assert(!(af == cf), "unequal fixed ellipses must not compare equal");
  }
}

} // namespace toy::geometry
