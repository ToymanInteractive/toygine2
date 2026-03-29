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
  \file   section.test.cpp
  \brief  Unit tests for \ref toy::geometry::Section.
*/

#include <doctest/doctest.h>

#include "geometry.hpp"

namespace toy::geometry {

using fixed_type = math::fixed<int32_t, int32_t, 24>;

static_assert(SectionEndpoint<int>, "int must satisfy SectionEndpoint");
static_assert(SectionEndpoint<float>, "float must satisfy SectionEndpoint");
static_assert(SectionEndpoint<fixed_type>, "fixed_type must satisfy SectionEndpoint");
static_assert(!SectionEndpoint<unsigned int>, "unsigned endpoints must remain rejected");

// Section has fixed size and contiguous layout.
TEST_CASE("geometry/section/object_structure") {
  static_assert(sizeof(Section<int>) == sizeof(int) * 2, "Section size must be 2× component size");
  static_assert(sizeof(Section<float>) == sizeof(float) * 2, "Section size must be 2× component size");
  static_assert(sizeof(Section<fixed_type>) == sizeof(fixed_type) * 2, "Section size must be 2× component size");

  static_assert(!std::is_trivial_v<Section<int>>, "Section<int> must not be trivial");
  static_assert(!std::is_trivial_v<Section<float>>, "Section<float> must not be trivial");
  static_assert(!std::is_trivial_v<Section<fixed_type>>, "Section<fixed_type> must not be trivial");
  static_assert(std::is_trivially_copyable_v<Section<int>>, "Section<int> must be trivially copyable");
  static_assert(std::is_trivially_copyable_v<Section<float>>, "Section<float> must be trivially copyable");
  static_assert(std::is_trivially_copyable_v<Section<fixed_type>>, "Section<fixed_type> must be trivially copyable");
  static_assert(std::is_standard_layout_v<Section<int>>, "Section<int> must have standard layout");
  static_assert(std::is_standard_layout_v<Section<float>>, "Section<float> must have standard layout");
  static_assert(std::is_standard_layout_v<Section<fixed_type>>, "Section<fixed_type> must have standard layout");
}

// Section(start, end) stores bounds; valid when start <= end.
TEST_CASE("geometry/section/constructor_bounds") {
  // Integer endpoint type.
  SUBCASE("int") {
    constexpr Section s(10, 20);

    REQUIRE(s.start == 10);
    REQUIRE(s.end == 20);
    REQUIRE(s.isValid());
    REQUIRE(!s.isReset());

    static_assert(s.start == 10, "constructor must store start");
    static_assert(s.end == 20, "constructor must store end");
    static_assert(s.isValid(), "Section(10,20) must be valid");
    static_assert(!s.isReset(), "Section(10,20) must not be reset");
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    constexpr Section s(10.0f, 20.0f);

    REQUIRE(math::isEqual(s.start, 10.0f));
    REQUIRE(math::isEqual(s.end, 20.0f));
    REQUIRE(s.isValid());
    REQUIRE(!s.isReset());

    static_assert(math::isEqual(s.start, 10.0f), "constructor must store start");
    static_assert(math::isEqual(s.end, 20.0f), "constructor must store end");
    static_assert(s.isValid(), "Section(10,20) float must be valid");
    static_assert(!s.isReset(), "Section(10,20) float must not be reset");
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    constexpr Section s(fixed_type(10), fixed_type(20));

    REQUIRE(s.start == fixed_type(10));
    REQUIRE(s.end == fixed_type(20));
    REQUIRE(s.isValid());
    REQUIRE(!s.isReset());

    static_assert(s.start == fixed_type(10), "constructor must store start");
    static_assert(s.end == fixed_type(20), "constructor must store end");
    static_assert(s.isValid(), "Section(10,20) fixed must be valid");
    static_assert(!s.isReset(), "Section(10,20) fixed must not be reset");
  }
}

// midpoint() returns (start + end) / 2.
TEST_CASE("geometry/section/midpoint") {
  // Integer endpoint type.
  SUBCASE("int") {
    constexpr Section s(10, 20);

    REQUIRE(s.midpoint() == 15);

    static_assert(s.midpoint() == 15, "midpoint of [10,20] must be 15");
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    constexpr Section s(1.0f, 3.0f);

    REQUIRE(math::isEqual(s.midpoint(), 2.0f));

    static_assert(math::isEqual(s.midpoint(), 2.0f), "midpoint of [1,3] float must be 2");
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    constexpr Section s(fixed_type(1), fixed_type(3));

    REQUIRE(s.midpoint() == fixed_type(2));

    static_assert(s.midpoint() == fixed_type(2), "midpoint of [1,3] must be 2");
  }
}

// length() returns end - start.
TEST_CASE("geometry/section/length") {
  // Integer endpoint type.
  SUBCASE("int") {
    constexpr Section s(10, 20);

    REQUIRE(s.length() == 10);

    static_assert(s.length() == 10, "length of [10,20] must be 10");
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    constexpr Section s(7.0f, 7.0f);

    REQUIRE(math::isEqual(s.length(), 0.0f));

    static_assert(math::isEqual(s.length(), 0.0f), "length of point section float must be 0");
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    constexpr Section s(fixed_type(1), fixed_type(3));

    REQUIRE(s.length() == fixed_type(2));

    static_assert(s.length() == fixed_type(2), "length of [1,3] must be 2");
  }
}

// reset() puts section in empty state (start/end extremes, isReset true).
TEST_CASE("geometry/section/reset") {
  // Integer endpoint type.
  SUBCASE("int") {
    Section s(1, 10);

    s.reset();

    REQUIRE(s.isReset());
    REQUIRE(!s.isValid());
    REQUIRE(s.start == numeric_limits<int>::max());
    REQUIRE(s.end == numeric_limits<int>::lowest());
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    Section s(1.0f, 10.0f);

    s.reset();

    REQUIRE(s.isReset());
    REQUIRE(!s.isValid());
    REQUIRE(math::isEqual(s.start, numeric_limits<float>::max()));
    REQUIRE(math::isEqual(s.end, numeric_limits<float>::lowest()));
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    Section s(fixed_type(1), fixed_type(10));

    s.reset();

    REQUIRE(s.isReset());
    REQUIRE(!s.isValid());
    REQUIRE(s.start == numeric_limits<fixed_type>::max());
    REQUIRE(s.end == numeric_limits<fixed_type>::lowest());
  }
}

// expand(T value) extends interval to include value.
TEST_CASE("geometry/section/expand_value") {
  // Integer endpoint type.
  SUBCASE("int") {
    Section s(10, 20);

    s.expand(5);

    REQUIRE(s.start == 5);
    REQUIRE(s.end == 20);

    s.expand(25);

    REQUIRE(s.start == 5);
    REQUIRE(s.end == 25);

    s.expand(15);

    REQUIRE(s.start == 5);
    REQUIRE(s.end == 25);
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    Section s(10.0f, 20.0f);

    s.expand(5.0f);

    REQUIRE(math::isEqual(s.start, 5.0f));
    REQUIRE(math::isEqual(s.end, 20.0f));

    s.expand(25.0f);

    REQUIRE(math::isEqual(s.start, 5.0f));
    REQUIRE(math::isEqual(s.end, 25.0f));

    s.expand(15.0f);

    REQUIRE(math::isEqual(s.start, 5.0f));
    REQUIRE(math::isEqual(s.end, 25.0f));
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    Section s(fixed_type(10), fixed_type(20));

    s.expand(fixed_type(5));

    REQUIRE(s.start == fixed_type(5));
    REQUIRE(s.end == fixed_type(20));

    s.expand(fixed_type(25));

    REQUIRE(s.start == fixed_type(5));
    REQUIRE(s.end == fixed_type(25));

    s.expand(fixed_type(15));

    REQUIRE(s.start == fixed_type(5));
    REQUIRE(s.end == fixed_type(25));
  }
}

// expand(Section) merges the other interval (union).
TEST_CASE("geometry/section/expand_section") {
  // Integer endpoint type.
  SUBCASE("int") {
    Section s(5, 10);
    Section t(5, 15);

    s.expand(Section(20, 30));
    t.expand(Section(1, 3));

    REQUIRE(s.start == 5);
    REQUIRE(s.end == 30);
    REQUIRE(t.start == 1);
    REQUIRE(t.end == 15);
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    Section s(5.0f, 10.0f);
    Section t(5.0f, 15.0f);

    s.expand(Section(20.0f, 30.0f));
    t.expand(Section(1.0f, 3.0f));

    REQUIRE(math::isEqual(s.start, 5.0f));
    REQUIRE(math::isEqual(s.end, 30.0f));
    REQUIRE(math::isEqual(t.start, 1.0f));
    REQUIRE(math::isEqual(t.end, 15.0f));
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    Section s(fixed_type(5), fixed_type(10));
    Section t(fixed_type(5), fixed_type(15));

    s.expand(Section(fixed_type(20), fixed_type(30)));
    t.expand(Section(fixed_type(1), fixed_type(3)));

    REQUIRE(s.start == fixed_type(5));
    REQUIRE(s.end == fixed_type(30));
    REQUIRE(t.start == fixed_type(1));
    REQUIRE(t.end == fixed_type(15));
  }
}

// isReset() true after reset, false when start <= end.
TEST_CASE("geometry/section/is_reset") {
  // Integer endpoint type.
  SUBCASE("int") {
    Section<int> empty;
    constexpr Section valid(1, 2);

    empty.reset();

    REQUIRE(empty.isReset());
    REQUIRE(!valid.isReset());

    static_assert(!valid.isReset(), "valid Section<int> must not be reset");
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    Section<float> empty;
    constexpr Section valid(1.0f, 2.0f);

    empty.reset();

    REQUIRE(empty.isReset());
    REQUIRE(!valid.isReset());

    static_assert(!valid.isReset(), "valid Section<float> must not be reset");
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    Section<fixed_type> empty;
    constexpr Section valid(fixed_type(1), fixed_type(2));

    empty.reset();

    REQUIRE(empty.isReset());
    REQUIRE(!valid.isReset());

    static_assert(!valid.isReset(), "valid Section<fixed_type> must not be reset");
  }
}

// isValid() true when start <= end.
TEST_CASE("geometry/section/is_valid") {
  // Integer endpoint type.
  SUBCASE("int") {
    Section<int> empty;
    constexpr Section valid(1, 2);

    empty.reset();

    REQUIRE(!empty.isValid());
    REQUIRE(valid.isValid());

    static_assert(valid.isValid(), "Section(1,2) must be valid");
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    Section<float> empty;
    constexpr Section valid(1.0f, 2.0f);

    empty.reset();

    REQUIRE(!empty.isValid());
    REQUIRE(valid.isValid());

    static_assert(valid.isValid(), "Section(1,2) float must be valid");
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    Section<fixed_type> empty;
    constexpr Section valid(fixed_type(1), fixed_type(2));

    empty.reset();

    REQUIRE(!empty.isValid());
    REQUIRE(valid.isValid());

    static_assert(valid.isValid(), "Section(1,2) fixed must be valid");
  }
}

// isContains(T value) true when value in [start, end] inclusive.
TEST_CASE("geometry/section/is_contains") {
  // Integer endpoint type.
  SUBCASE("int") {
    constexpr Section s(10, 20);

    REQUIRE(s.isContains(10));
    REQUIRE(s.isContains(15));
    REQUIRE(s.isContains(20));
    REQUIRE(!s.isContains(9));
    REQUIRE(!s.isContains(21));

    static_assert(s.isContains(10), "10 must be inside [10,20]");
    static_assert(s.isContains(15), "15 must be inside [10,20]");
    static_assert(s.isContains(20), "20 must be inside [10,20]");
    static_assert(!s.isContains(9), "9 must be outside [10,20]");
    static_assert(!s.isContains(21), "21 must be outside [10,20]");
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    constexpr Section s(10.0f, 20.0f);

    REQUIRE(s.isContains(10.0f));
    REQUIRE(s.isContains(15.0f));
    REQUIRE(s.isContains(20.0f));
    REQUIRE(!s.isContains(9.0f));
    REQUIRE(!s.isContains(21.0f));

    static_assert(s.isContains(10.0f), "10 must be inside [10,20] float");
    static_assert(s.isContains(15.0f), "15 must be inside [10,20] float");
    static_assert(s.isContains(20.0f), "20 must be inside [10,20] float");
    static_assert(!s.isContains(9.0f), "9 must be outside [10,20] float");
    static_assert(!s.isContains(21.0f), "21 must be outside [10,20] float");
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    constexpr Section s(fixed_type(10), fixed_type(20));

    REQUIRE(s.isContains(fixed_type(10)));
    REQUIRE(s.isContains(fixed_type(15)));
    REQUIRE(s.isContains(fixed_type(20)));
    REQUIRE(!s.isContains(fixed_type(9)));
    REQUIRE(!s.isContains(fixed_type(21)));

    static_assert(s.isContains(fixed_type(10)), "10 must be inside [10,20] fixed");
    static_assert(s.isContains(fixed_type(15)), "fixed_type(15) must be inside [10,20]");
    static_assert(s.isContains(fixed_type(20)), "20 must be inside [10,20] fixed");
    static_assert(!s.isContains(fixed_type(9)), "9 must be outside [10,20] fixed");
    static_assert(!s.isContains(fixed_type(21)), "21 must be outside [10,20] fixed");
  }
}

// operator== true when both bounds match (exact for int/fixed, tolerance for float).
TEST_CASE("geometry/section/operator_eq") {
  // Integer endpoint type.
  SUBCASE("int") {
    constexpr Section a(1, 2);
    constexpr Section b(1, 2);
    constexpr Section c(1, 3);

    REQUIRE(a == b);
    REQUIRE(!(a != b));
    REQUIRE(a != c);
    REQUIRE(!(a == c));

    static_assert(a == b, "equal sections must compare equal");
    static_assert(!(a != b), "equal sections must not be unequal");
    static_assert(a != c, "different sections must compare unequal");
    static_assert(!(a == c), "different sections must not be equal");
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    constexpr Section a(1.0f, 2.0f);
    constexpr Section b(1.0f, 2.0f);
    constexpr Section c(1.0f, 3.0f);

    REQUIRE(a == b);
    REQUIRE(!(a != b));
    REQUIRE(a != c);
    REQUIRE(!(a == c));

    static_assert(a == b, "equal float sections must compare equal");
    static_assert(!(a != b), "equal float sections must not be unequal");
    static_assert(a != c, "different float sections must compare unequal");
    static_assert(!(a == c), "different float sections must not be equal");
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    constexpr Section a(fixed_type(1), fixed_type(2));
    constexpr Section b(fixed_type(1), fixed_type(2));
    constexpr Section c(fixed_type(1), fixed_type(3));

    REQUIRE(a == b);
    REQUIRE(!(a != b));
    REQUIRE(a != c);
    REQUIRE(!(a == c));

    static_assert(a == b, "equal sections must compare equal");
    static_assert(!(a != b), "equal fixed sections must not be unequal");
    static_assert(a != c, "different fixed sections must compare unequal");
    static_assert(!(a == c), "different fixed sections must not be equal");
  }
}

// operator!= true when bounds differ; consistent with operator==.
TEST_CASE("geometry/section/operator_ne") {
  // Integer endpoint type.
  SUBCASE("int") {
    constexpr Section a(1, 2);
    constexpr Section b(1, 3);

    REQUIRE(a != b);
    REQUIRE(!(a == b));
    REQUIRE(a == a);

    static_assert(a != b, "operator!= must reflect inequality");
    static_assert(!(a == b), "unequal sections must not be equal");
    static_assert(a == a, "same section must be equal to self");
  }

  // Floating-point endpoint type.
  SUBCASE("float") {
    constexpr Section a(1.0f, 2.0f);
    constexpr Section b(1.0f, 3.0f);

    REQUIRE(a != b);
    REQUIRE(!(a == b));
    REQUIRE(a == a);

    static_assert(a != b, "operator!= float must reflect inequality");
    static_assert(!(a == b), "unequal float sections must not be equal");
    static_assert(a == a, "same float section must be equal to self");
  }

  // Fixed-point endpoint type.
  SUBCASE("fixed") {
    constexpr Section a(fixed_type(1), fixed_type(2));
    constexpr Section b(fixed_type(1), fixed_type(3));

    REQUIRE(a != b);
    REQUIRE(!(a == b));
    REQUIRE(a == a);

    static_assert(a != b, "operator!= must reflect inequality");
    static_assert(!(a == b), "unequal fixed sections must not be equal");
    static_assert(a == a, "same fixed section must be equal to self");
  }
}

} // namespace toy::geometry
