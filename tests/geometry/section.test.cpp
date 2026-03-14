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

#include "geometry.hpp"
namespace toy::geometry {

using Fixed = math::fixed<int32_t, int32_t, 24>;

static_assert(SectionEndpoint<int>, "int must satisfy SectionEndpoint");
static_assert(SectionEndpoint<float>, "float must satisfy SectionEndpoint");
static_assert(SectionEndpoint<Fixed>, "Fixed must satisfy SectionEndpoint");
static_assert(!SectionEndpoint<unsigned int>, "unsigned endpoints must remain rejected");

// Default-constructed section is in reset state; bounds are numeric_limits extremes.
TEST_CASE("geometry/section/default_constructor") {
  // integer scalar
  SUBCASE("int") {
    constexpr Section<int> s;

    REQUIRE(s.isReset());
    REQUIRE(!s.isValid());
    REQUIRE(s.start == numeric_limits<int>::max());
    REQUIRE(s.end == numeric_limits<int>::lowest());

    static_assert(s.isReset(), "default Section<int> must be reset");
    static_assert(!s.isValid(), "default Section<int> must be invalid");
    static_assert(s.start == numeric_limits<int>::max(), "default start must be max()");
    static_assert(s.end == numeric_limits<int>::lowest(), "default end must be lowest()");
  }

  // float scalar
  SUBCASE("float") {
    constexpr Section<float> s;

    REQUIRE(s.isReset());
    REQUIRE(!s.isValid());
    REQUIRE(math::isEqual(s.start, numeric_limits<float>::max()));
    REQUIRE(math::isEqual(s.end, numeric_limits<float>::lowest()));

    static_assert(s.isReset(), "default Section<float> must be reset");
    static_assert(!s.isValid(), "default Section<float> must be invalid");
    static_assert(math::isEqual(s.start, numeric_limits<float>::max()), "default float start must be max()");
    static_assert(math::isEqual(s.end, numeric_limits<float>::lowest()), "default float end must be lowest()");
  }

  // fixed-point scalar
  SUBCASE("fixed") {
    constexpr Section<Fixed> s;

    REQUIRE(s.isReset());
    REQUIRE(!s.isValid());
    REQUIRE(s.start == numeric_limits<Fixed>::max());
    REQUIRE(s.end == numeric_limits<Fixed>::lowest());

    static_assert(s.isReset(), "default Section<Fixed> must be reset");
    static_assert(!s.isValid(), "default Section<Fixed> must be invalid");
    static_assert(s.start == numeric_limits<Fixed>::max(), "default fixed start must be max()");
    static_assert(s.end == numeric_limits<Fixed>::lowest(), "default fixed end must be lowest()");
  }
}

// Section(min, max) stores bounds; valid when min <= max.
TEST_CASE("geometry/section/constructor_bounds") {
  // integer scalar
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

  // float scalar
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

  // fixed-point scalar
  SUBCASE("fixed") {
    constexpr Section s(Fixed(10), Fixed(20));

    REQUIRE(s.start == Fixed(10));
    REQUIRE(s.end == Fixed(20));
    REQUIRE(s.isValid());
    REQUIRE(!s.isReset());

    static_assert(s.start == Fixed(10), "constructor must store start");
    static_assert(s.end == Fixed(20), "constructor must store end");
    static_assert(s.isValid(), "Section(10,20) fixed must be valid");
    static_assert(!s.isReset(), "Section(10,20) fixed must not be reset");
  }
}

// midpoint() returns (start + end) / 2.
TEST_CASE("geometry/section/midpoint") {
  // integer scalar
  SUBCASE("int") {
    constexpr Section s(10, 20);

    REQUIRE(s.midpoint() == 15);

    static_assert(s.midpoint() == 15, "midpoint of [10,20] must be 15");
  }

  // float scalar
  SUBCASE("float") {
    constexpr Section s(1.0f, 3.0f);

    REQUIRE(math::isEqual(s.midpoint(), 2.0f));

    static_assert(math::isEqual(s.midpoint(), 2.0f), "midpoint of [1,3] float must be 2");
  }

  // fixed-point scalar
  SUBCASE("fixed") {
    constexpr Section s(Fixed(1), Fixed(3));

    REQUIRE(s.midpoint() == Fixed(2));

    static_assert(s.midpoint() == Fixed(2), "midpoint of [1,3] must be 2");
  }
}

// length() returns end - start.
TEST_CASE("geometry/section/length") {
  // integer scalar
  SUBCASE("int") {
    constexpr Section s(10, 20);

    REQUIRE(s.length() == 10);

    static_assert(s.length() == 10, "length of [10,20] must be 10");
  }

  // float scalar
  SUBCASE("float") {
    constexpr Section s(7.0f, 7.0f);

    REQUIRE(math::isEqual(s.length(), 0.0f));

    static_assert(math::isEqual(s.length(), 0.0f), "length of point section float must be 0");
  }

  // fixed-point scalar
  SUBCASE("fixed") {
    constexpr Section s(Fixed(1), Fixed(3));

    REQUIRE(s.length() == Fixed(2));

    static_assert(s.length() == Fixed(2), "length of [1,3] must be 2");
  }
}

// reset() puts section in empty state (min/max extremes, isReset true).
TEST_CASE("geometry/section/reset") {
  // integer scalar
  SUBCASE("int") {
    Section s(1, 10);

    s.reset();

    REQUIRE(s.isReset());
    REQUIRE(!s.isValid());
    REQUIRE(s.start == numeric_limits<int>::max());
    REQUIRE(s.end == numeric_limits<int>::lowest());
  }

  // float scalar
  SUBCASE("float") {
    Section s(1.0f, 10.0f);

    s.reset();

    REQUIRE(s.isReset());
    REQUIRE(!s.isValid());
    REQUIRE(math::isEqual(s.start, numeric_limits<float>::max()));
    REQUIRE(math::isEqual(s.end, numeric_limits<float>::lowest()));
  }

  // fixed-point scalar
  SUBCASE("fixed") {
    Section s(Fixed(1), Fixed(10));

    s.reset();

    REQUIRE(s.isReset());
    REQUIRE(!s.isValid());
    REQUIRE(s.start == numeric_limits<Fixed>::max());
    REQUIRE(s.end == numeric_limits<Fixed>::lowest());
  }
}

// expand(T value) extends interval to include value.
TEST_CASE("geometry/section/expand_value") {
  // integer scalar
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

  // float scalar
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

  // fixed-point scalar
  SUBCASE("fixed") {
    Section s(Fixed(10), Fixed(20));

    s.expand(Fixed(5));

    REQUIRE(s.start == Fixed(5));
    REQUIRE(s.end == Fixed(20));

    s.expand(Fixed(25));

    REQUIRE(s.start == Fixed(5));
    REQUIRE(s.end == Fixed(25));

    s.expand(Fixed(15));

    REQUIRE(s.start == Fixed(5));
    REQUIRE(s.end == Fixed(25));
  }
}

// expand(Section) merges the other interval (union).
TEST_CASE("geometry/section/expand_section") {
  // integer scalar
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

  // float scalar
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

  // fixed-point scalar
  SUBCASE("fixed") {
    Section s(Fixed(5), Fixed(10));
    Section t(Fixed(5), Fixed(15));

    s.expand(Section(Fixed(20), Fixed(30)));
    t.expand(Section(Fixed(1), Fixed(3)));

    REQUIRE(s.start == Fixed(5));
    REQUIRE(s.end == Fixed(30));
    REQUIRE(t.start == Fixed(1));
    REQUIRE(t.end == Fixed(15));
  }
}

// isReset() is true when start > end.
TEST_CASE("geometry/section/is_reset") {
  // integer scalar
  SUBCASE("int") {
    constexpr Section<int> empty;
    constexpr Section valid(1, 2);

    REQUIRE(empty.isReset());
    REQUIRE(!valid.isReset());

    static_assert(empty.isReset(), "default Section<int> must be reset");
    static_assert(!valid.isReset(), "valid Section<int> must not be reset");
  }

  // float scalar
  SUBCASE("float") {
    constexpr Section<float> empty;
    constexpr Section valid(1.0f, 2.0f);

    REQUIRE(empty.isReset());
    REQUIRE(!valid.isReset());

    static_assert(empty.isReset(), "default Section<float> must be reset");
    static_assert(!valid.isReset(), "valid Section<float> must not be reset");
  }

  // fixed-point scalar
  SUBCASE("fixed") {
    constexpr Section<Fixed> empty;
    constexpr Section valid(Fixed(1), Fixed(2));

    REQUIRE(empty.isReset());
    REQUIRE(!valid.isReset());

    static_assert(empty.isReset(), "default Section<Fixed> must be reset");
    static_assert(!valid.isReset(), "valid Section<Fixed> must not be reset");
  }
}

// isValid() is true when start <= end.
TEST_CASE("geometry/section/is_valid") {
  // integer scalar
  SUBCASE("int") {
    constexpr Section<int> empty;
    constexpr Section valid(1, 2);

    REQUIRE(!empty.isValid());
    REQUIRE(valid.isValid());

    static_assert(!empty.isValid(), "default Section<int> must be invalid");
    static_assert(valid.isValid(), "Section(1,2) must be valid");
  }

  // float scalar
  SUBCASE("float") {
    constexpr Section<float> empty;
    constexpr Section valid(1.0f, 2.0f);

    REQUIRE(!empty.isValid());
    REQUIRE(valid.isValid());

    static_assert(!empty.isValid(), "default Section<float> must be invalid");
    static_assert(valid.isValid(), "Section(1,2) float must be valid");
  }

  // fixed-point scalar
  SUBCASE("fixed") {
    constexpr Section<Fixed> empty;
    constexpr Section valid(Fixed(1), Fixed(2));

    REQUIRE(!empty.isValid());
    REQUIRE(valid.isValid());

    static_assert(!empty.isValid(), "default Section<Fixed> must be invalid");
    static_assert(valid.isValid(), "Section(1,2) fixed must be valid");
  }
}

// isContains(T value) is true when value is in [start, end] inclusive.
TEST_CASE("geometry/section/is_contains") {
  // integer scalar
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

  // float scalar
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

  // fixed-point scalar
  SUBCASE("fixed") {
    constexpr Section s(Fixed(10), Fixed(20));

    REQUIRE(s.isContains(Fixed(10)));
    REQUIRE(s.isContains(Fixed(15)));
    REQUIRE(s.isContains(Fixed(20)));
    REQUIRE(!s.isContains(Fixed(9)));
    REQUIRE(!s.isContains(Fixed(21)));

    static_assert(s.isContains(Fixed(10)), "10 must be inside [10,20] fixed");
    static_assert(s.isContains(Fixed(15)), "Fixed(15) must be inside [10,20]");
    static_assert(s.isContains(Fixed(20)), "20 must be inside [10,20] fixed");
    static_assert(!s.isContains(Fixed(9)), "9 must be outside [10,20] fixed");
    static_assert(!s.isContains(Fixed(21)), "21 must be outside [10,20] fixed");
  }
}

// operator== returns true when both bounds match (exact for int/fixed, isEqual for float).
TEST_CASE("geometry/section/operator_eq") {
  // integer scalar
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

  // float scalar
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

  // fixed-point scalar
  SUBCASE("fixed") {
    constexpr Section a(Fixed(1), Fixed(2));
    constexpr Section b(Fixed(1), Fixed(2));
    constexpr Section c(Fixed(1), Fixed(3));

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

// operator!= returns true when bounds differ; consistent with operator==.
TEST_CASE("geometry/section/operator_ne") {
  // integer scalar
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

  // float scalar
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

  // fixed-point scalar
  SUBCASE("fixed") {
    constexpr Section a(Fixed(1), Fixed(2));
    constexpr Section b(Fixed(1), Fixed(3));

    REQUIRE(a != b);
    REQUIRE(!(a == b));
    REQUIRE(a == a);

    static_assert(a != b, "operator!= must reflect inequality");
    static_assert(!(a == b), "unequal fixed sections must not be equal");
    static_assert(a == a, "same fixed section must be equal to self");
  }
}

} // namespace toy::geometry
