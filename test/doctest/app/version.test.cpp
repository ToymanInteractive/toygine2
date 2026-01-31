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

    static_assert(version.major == 0, "default version major must be zero");
    static_assert(version.minor == 0, "default version minor must be zero");
    static_assert(version.maintenance == 0, "default version maintenance must be zero");
    static_assert(version.revision == 0, "default version revision must be zero");
  }

  // Aggregate initialization sets all fields.
  SUBCASE("aggregate_initialization") {
    constexpr Version version{5, 10, 15, 20};

    static_assert(version.major == 5, "aggregate-initialized major must match");
    static_assert(version.minor == 10, "aggregate-initialized minor must match");
    static_assert(version.maintenance == 15, "aggregate-initialized maintenance must match");
    static_assert(version.revision == 20, "aggregate-initialized revision must match");
  }

  // Partial initialization leaves missing fields at zero.
  SUBCASE("partial_initialization") {
    constexpr Version version{1, 2};

    static_assert(version.major == 1, "partial-init major must match");
    static_assert(version.minor == 2, "partial-init minor must match");
    static_assert(version.maintenance == 0, "partial-init remainder must be zero");
    static_assert(version.revision == 0, "partial-init remainder must be zero");
  }
}

// Equality operator comparisons across version fields.
TEST_CASE("app/version/equality_operator") {
  // Identical versions compare equal.
  SUBCASE("identical") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 4};
    constexpr bool result = v1 == v2;

    static_assert(result, "identical versions must compare equal");
  }

  // Major version differences compare not equal.
  SUBCASE("different_major_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{2, 2, 3, 4};
    constexpr bool result = v1 == v2;

    static_assert(!result, "different major must yield not equal");
  }

  // Minor version differences compare not equal.
  SUBCASE("different_minor_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 3, 3, 4};
    constexpr bool result = v1 == v2;

    static_assert(!result, "different minor must yield not equal");
  }

  // Maintenance version differences compare not equal.
  SUBCASE("different_maintenance_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 4, 4};
    constexpr bool result = v1 == v2;

    static_assert(!result, "different maintenance must yield not equal");
  }

  // Revision version differences compare not equal.
  SUBCASE("different_revision_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};
    constexpr bool result = v1 == v2;

    static_assert(!result, "different revision must yield not equal");
  }

  // Zero versions compare equal.
  SUBCASE("zero_versions") {
    constexpr Version v1{0, 0, 0, 0};
    constexpr Version v2{0, 0, 0, 0};
    constexpr bool result = v1 == v2;

    static_assert(result, "both zero must compare equal");
  }

  // Zero and non-zero versions compare not equal.
  SUBCASE("mixed_zero_and_non_zero") {
    constexpr Version v1{0, 0, 0, 0};
    constexpr Version v2{0, 0, 0, 1};
    constexpr bool result = v1 == v2;

    static_assert(!result, "zero vs non-zero must be not equal");
  }
}

// Three-way comparison ordering behavior.
TEST_CASE("app/version/three_way_comparison_operator") {
  // Equal versions yield strong_ordering::equal.
  SUBCASE("equal_versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 4};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::equal, "identical versions must order equal");
  }

  // Major version drives ordering first.
  SUBCASE("major_version_comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{2, 1, 1, 1};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::less, "lesser major must order less");

    constexpr Version v3{2, 1, 1, 1};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    static_assert(result2 == strong_ordering::greater, "greater major must order greater");
  }

  // Minor version drives ordering after major.
  SUBCASE("minor_version_comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 3, 1, 1};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::less, "lesser minor must order less");

    constexpr Version v3{1, 3, 1, 1};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    static_assert(result2 == strong_ordering::greater, "greater minor must order greater");
  }

  // Maintenance version drives ordering after minor.
  SUBCASE("maintenance_version_comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 4, 1};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::less, "lesser maintenance must order less");

    constexpr Version v3{1, 2, 4, 1};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    static_assert(result2 == strong_ordering::greater, "greater maintenance must order greater");
  }

  // Revision version drives ordering last.
  SUBCASE("revision_version_comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::less, "lesser revision must order less");

    constexpr Version v3{1, 2, 3, 5};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    static_assert(result2 == strong_ordering::greater, "greater revision must order greater");
  }

  // Zero versions compare as equal in ordering.
  SUBCASE("zero_versions") {
    constexpr Version v1{0, 0, 0, 0};
    constexpr Version v2{0, 0, 0, 0};
    constexpr auto result = v1 <=> v2;

    static_assert(result == strong_ordering::equal, "both zero must order equal");
  }
}

// Relational and equality operators consistency checks.
TEST_CASE("app/version/comparison_operators") {
  // Less-than operator orders by version fields.
  SUBCASE("less_than_operator") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};

    static_assert(v1 < v2, "lesser version must be less");
    static_assert(!(v2 < v1), "greater must not be less");
    static_assert(!(v1 < v1), "less-than must be irreflexive");
  }

  // Less-than-or-equal matches ordering semantics.
  SUBCASE("less_than_or_equal_operator") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};
    constexpr Version v3{1, 2, 3, 4};

    static_assert(v1 <= v2, "less or equal must hold for lesser");
    static_assert(v1 <= v3, "equal must satisfy less-or-equal");
    static_assert(!(v2 <= v1), "greater must not satisfy less-or-equal");
  }

  // Greater-than operator orders by version fields.
  SUBCASE("greater_than_operator") {
    constexpr Version v1{1, 2, 3, 5};
    constexpr Version v2{1, 2, 3, 4};

    static_assert(v1 > v2, "greater version must be greater");
    static_assert(!(v2 > v1), "lesser must not be greater");
    static_assert(!(v1 > v1), "greater-than must be irreflexive");
  }

  // Greater-than-or-equal matches ordering semantics.
  SUBCASE("greater_than_or_equal_operator") {
    constexpr Version v1{1, 2, 3, 5};
    constexpr Version v2{1, 2, 3, 4};
    constexpr Version v3{1, 2, 3, 5};

    static_assert(v1 >= v2, "greater or equal must hold for greater");
    static_assert(v1 >= v3, "equal must satisfy greater-or-equal");
    static_assert(!(v2 >= v1), "lesser must not satisfy greater-or-equal");
  }

  // Not-equal operator matches equality semantics.
  SUBCASE("not_equal_operator") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};

    static_assert(v1 != v2, "different versions must be not equal");
    static_assert(!(v1 != v1), "same version must not be not equal");
  }
}

// Semantic versioning precedence scenarios.
TEST_CASE("app/version/semantic_scenarios") {
  // Major version has highest precedence.
  SUBCASE("major_version_precedence") {
    constexpr Version v1{1, 0, 0, 0};
    constexpr Version v2{2, 0, 0, 0};

    static_assert(v1 < v2, "major precedence: lesser major must be less");
    static_assert(v2 > v1, "major precedence: greater major must be greater");
    static_assert(v1 != v2, "major precedence: different major must be not equal");
  }

  // Minor version has precedence after major.
  SUBCASE("minor_version_precedence") {
    constexpr Version v1{1, 1, 0, 0};
    constexpr Version v2{1, 2, 0, 0};

    static_assert(v1 < v2, "minor precedence: lesser minor must be less");
    static_assert(v2 > v1, "minor precedence: greater minor must be greater");
    static_assert(v1 != v2, "minor precedence: different minor must be not equal");
  }

  // Maintenance version has precedence after minor.
  SUBCASE("maintenance_version_precedence") {
    constexpr Version v1{1, 1, 1, 0};
    constexpr Version v2{1, 1, 2, 0};

    static_assert(v1 < v2, "maintenance precedence: lesser must be less");
    static_assert(v2 > v1, "maintenance precedence: greater must be greater");
    static_assert(v1 != v2, "maintenance precedence: different must be not equal");
  }

  // Revision version has the lowest precedence.
  SUBCASE("revision_version_precedence") {
    constexpr Version v1{1, 1, 1, 1};
    constexpr Version v2{1, 1, 1, 2};

    static_assert(v1 < v2, "revision precedence: lesser revision must be less");
    static_assert(v2 > v1, "revision precedence: greater revision must be greater");
    static_assert(v1 != v2, "revision precedence: different revision must be not equal");
  }

  // Complex ordering chains across fields.
  SUBCASE("complex_version_comparisons") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 4, 0};
    constexpr Version v3{1, 3, 0, 0};
    constexpr Version v4{2, 0, 0, 0};

    static_assert(v1 < v2, "chain: maintenance difference must order less");
    static_assert(v2 < v3, "chain: minor difference must order less");
    static_assert(v3 < v4, "chain: major difference must order less");
  }
}

// Runtime-only comparisons that cannot be constexpr.
TEST_CASE("app/version/runtime_tests") {
  Version v1{1, 0, 0, 0};
  Version v2{1, 1, 0, 0};
  Version v3{1, 1, 1, 0};
  Version v4{1, 1, 1, 1};
  Version v5{2, 0, 0, 0};

  REQUIRE(v1 < v2);
  REQUIRE(v2 < v3);
  REQUIRE(v3 < v4);
  REQUIRE(v4 < v5);

  REQUIRE(v5 > v4);
  REQUIRE(v4 > v3);
  REQUIRE(v3 > v2);
  REQUIRE(v2 > v1);

  REQUIRE(v1 <= v2);
  REQUIRE(v2 <= v3);
  REQUIRE(v3 <= v4);
  REQUIRE(v4 <= v5);

  REQUIRE(v5 >= v4);
  REQUIRE(v4 >= v3);
  REQUIRE(v3 >= v2);
  REQUIRE(v2 >= v1);

  REQUIRE(v1 != v2);
  REQUIRE(v2 != v3);
  REQUIRE(v3 != v4);
  REQUIRE(v4 != v5);

  // Test equality
  Version v6{1, 0, 0, 0};
  REQUIRE(v1 == v6);
  REQUIRE(!(v1 == v2));
}
