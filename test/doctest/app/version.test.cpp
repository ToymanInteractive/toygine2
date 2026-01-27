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

#include "app.hpp"

using namespace toy;
using namespace toy::app;

// Version constructors and initialization variants.
TEST_CASE("app/version/constructors") {
  // Default constructed version is zeroed.
  SUBCASE("default") {
    constexpr Version version;

    static_assert(version.major == 0);
    static_assert(version.minor == 0);
    static_assert(version.maintenance == 0);
    static_assert(version.revision == 0);
  }

  // Aggregate initialization sets all fields.
  SUBCASE("aggregate_initialization") {
    constexpr Version version{5, 10, 15, 20};

    static_assert(version.major == 5);
    static_assert(version.minor == 10);
    static_assert(version.maintenance == 15);
    static_assert(version.revision == 20);
  }

  // Partial initialization leaves missing fields at zero.
  SUBCASE("partial_initialization") {
    constexpr Version version{1, 2};

    static_assert(version.major == 1);
    static_assert(version.minor == 2);
    static_assert(version.maintenance == 0);
    static_assert(version.revision == 0);
  }
}

// Equality operator comparisons across version fields.
TEST_CASE("app/version/equality_operator") {
  // Identical versions compare equal.
  SUBCASE("identical") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 4};
    constexpr bool result = v1 == v2;

    static_assert(result);
  }

  // Major version differences compare not equal.
  SUBCASE("different_major_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{2, 2, 3, 4};
    constexpr bool result = v1 == v2;

    static_assert(!result);
  }

  // Minor version differences compare not equal.
  SUBCASE("different_minor_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 3, 3, 4};
    constexpr bool result = v1 == v2;

    static_assert(!result);
  }

  // Maintenance version differences compare not equal.
  SUBCASE("different_maintenance_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 4, 4};
    constexpr bool result = v1 == v2;

    static_assert(!result);
  }

  // Revision version differences compare not equal.
  SUBCASE("different_revision_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};
    constexpr bool result = v1 == v2;

    static_assert(!result);
  }

  // Zero versions compare equal.
  SUBCASE("zero_versions") {
    constexpr Version v1{0, 0, 0, 0};
    constexpr Version v2{0, 0, 0, 0};
    constexpr bool result = v1 == v2;

    static_assert(result);
  }

  // Zero and non-zero versions compare not equal.
  SUBCASE("mixed_zero_and_non_zero") {
    constexpr Version v1{0, 0, 0, 0};
    constexpr Version v2{0, 0, 0, 1};
    constexpr bool result = v1 == v2;

    static_assert(!result);
  }
}

// Three-way comparison ordering behavior.
TEST_CASE("app/version/three_way_comparison_operator") {
  // Equal versions yield strong_ordering::equal.
  SUBCASE("equal_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 4};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::equal);
  }

  // Major version drives ordering first.
  SUBCASE("major_version_comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{2, 1, 1, 1};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::less);

    constexpr Version v3{2, 1, 1, 1};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    static_assert(result2 == strong_ordering::greater);
  }

  // Minor version drives ordering after major.
  SUBCASE("minor_version_comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 3, 1, 1};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::less);

    constexpr Version v3{1, 3, 1, 1};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    static_assert(result2 == strong_ordering::greater);
  }

  // Maintenance version drives ordering after minor.
  SUBCASE("maintenance_version_comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 4, 1};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::less);

    constexpr Version v3{1, 2, 4, 1};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    static_assert(result2 == strong_ordering::greater);
  }

  // Revision version drives ordering last.
  SUBCASE("revision_version_comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::less);

    constexpr Version v3{1, 2, 3, 5};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    static_assert(result2 == strong_ordering::greater);
  }

  // Zero versions compare as equal in ordering.
  SUBCASE("zero_versions") {
    constexpr Version v1{0, 0, 0, 0};
    constexpr Version v2{0, 0, 0, 0};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::equal);
  }
}

// Relational and equality operators consistency checks.
TEST_CASE("app/version/comparison_operators") {
  // Less-than operator orders by version fields.
  SUBCASE("less_than_operator") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};

    static_assert(v1 < v2);
    static_assert(!(v2 < v1));
    static_assert(!(v1 < v1));
  }

  // Less-than-or-equal matches ordering semantics.
  SUBCASE("less_than_or_equal_operator") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};
    constexpr Version v3{1, 2, 3, 4};

    static_assert(v1 <= v2);
    static_assert(v1 <= v3);
    static_assert(!(v2 <= v1));
  }

  // Greater-than operator orders by version fields.
  SUBCASE("greater_than_operator") {
    constexpr Version v1{1, 2, 3, 5};
    constexpr Version v2{1, 2, 3, 4};

    static_assert(v1 > v2);
    static_assert(!(v2 > v1));
    static_assert(!(v1 > v1));
  }

  // Greater-than-or-equal matches ordering semantics.
  SUBCASE("greater_than_or_equal_operator") {
    constexpr Version v1{1, 2, 3, 5};
    constexpr Version v2{1, 2, 3, 4};
    constexpr Version v3{1, 2, 3, 5};

    static_assert(v1 >= v2);
    static_assert(v1 >= v3);
    static_assert(!(v2 >= v1));
  }

  // Not-equal operator matches equality semantics.
  SUBCASE("not_equal_operator") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};

    static_assert(v1 != v2);
    static_assert(!(v1 != v1));
  }
}

// Semantic versioning precedence scenarios.
TEST_CASE("app/version/semantic_scenarios") {
  // Major version has highest precedence.
  SUBCASE("major_version_precedence") {
    constexpr Version v1{1, 0, 0, 0};
    constexpr Version v2{2, 0, 0, 0};

    static_assert(v1 < v2);
    static_assert(v2 > v1);
    static_assert(v1 != v2);
  }

  // Minor version has precedence after major.
  SUBCASE("minor_version_precedence") {
    constexpr Version v1{1, 1, 0, 0};
    constexpr Version v2{1, 2, 0, 0};

    static_assert(v1 < v2);
    static_assert(v2 > v1);
    static_assert(v1 != v2);
  }

  // Maintenance version has precedence after minor.
  SUBCASE("maintenance_version_precedence") {
    constexpr Version v1{1, 1, 1, 0};
    constexpr Version v2{1, 1, 2, 0};

    static_assert(v1 < v2);
    static_assert(v2 > v1);
    static_assert(v1 != v2);
  }

  // Revision version has the lowest precedence.
  SUBCASE("revision_version_precedence") {
    constexpr Version v1{1, 1, 1, 1};
    constexpr Version v2{1, 1, 1, 2};

    static_assert(v1 < v2);
    static_assert(v2 > v1);
    static_assert(v1 != v2);
  }

  // Complex ordering chains across fields.
  SUBCASE("complex_version_comparisons") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 4, 0};
    constexpr Version v3{1, 3, 0, 0};
    constexpr Version v4{2, 0, 0, 0};

    static_assert(v1 < v2);
    static_assert(v2 < v3);
    static_assert(v3 < v4);
  }
}

// Runtime-only comparisons that cannot be constexpr.
TEST_CASE("app/version/runtime_tests") {
  Version v1{1, 0, 0, 0};
  Version v2{1, 1, 0, 0};
  Version v3{1, 1, 1, 0};
  Version v4{1, 1, 1, 1};

  REQUIRE(v1 < v2);
  REQUIRE(v2 < v3);
  REQUIRE(v3 < v4);

  REQUIRE(v4 > v3);
  REQUIRE(v3 > v2);
  REQUIRE(v2 > v1);

  REQUIRE(v1 <= v2);
  REQUIRE(v2 <= v3);
  REQUIRE(v3 <= v4);

  REQUIRE(v4 >= v3);
  REQUIRE(v3 >= v2);
  REQUIRE(v2 >= v1);

  REQUIRE(v1 != v2);
  REQUIRE(v2 != v3);
  REQUIRE(v3 != v4);

  // Test equality
  Version v5{1, 0, 0, 0};
  REQUIRE(v1 == v5);
  REQUIRE(!(v1 == v2));
}
