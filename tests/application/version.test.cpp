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
  \file   version.test.cpp
  \brief  Unit tests for \ref toy::application::Version.
*/

#include <doctest/doctest.h>

#include "application.hpp"

namespace toy::application {

// Version is trivially copyable and standard-layout; not trivially default-constructible due to member initializers.
TEST_CASE("app/version/type_traits") {
  static_assert(!std::is_trivial_v<Version>,
                "Version must not be trivial (member initializers suppress trivial default construction)");
  static_assert(std::is_trivially_copyable_v<Version>, "Version must be trivially copyable");
  static_assert(std::is_trivially_destructible_v<Version>, "Version must be trivially destructible");
  static_assert(std::is_standard_layout_v<Version>, "Version must have standard layout");
}

// Initialization sets each field; unspecified fields default to zero or empty.
TEST_CASE("app/version/initialization") {
  // Default-constructed version has all numeric fields zero and an empty revision.
  SUBCASE("default") {
    constexpr Version v;

    REQUIRE(v.major == 0);
    REQUIRE(v.minor == 0);
    REQUIRE(v.patch == 0);
    REQUIRE(v.revision.empty());

    static_assert(v.major == 0, "default major must be zero");
    static_assert(v.minor == 0, "default minor must be zero");
    static_assert(v.patch == 0, "default patch must be zero");
    static_assert(v.revision.empty(), "default revision must be empty");
  }

  // Aggregate initialization sets all four fields exactly.
  SUBCASE("aggregate") {
    constexpr Version v{1, 2, 3, "346ca09"};

    REQUIRE(v.major == 1);
    REQUIRE(v.minor == 2);
    REQUIRE(v.patch == 3);
    REQUIRE(v.revision == "346ca09");

    static_assert(v.major == 1, "major must match aggregate value");
    static_assert(v.minor == 2, "minor must match aggregate value");
    static_assert(v.patch == 3, "patch must match aggregate value");
    static_assert(v.revision == "346ca09", "revision must match aggregate value");
  }

  // Unspecified tail fields default to zero or empty.
  SUBCASE("partial") {
    constexpr Version v{2, 5};

    REQUIRE(v.major == 2);
    REQUIRE(v.minor == 5);
    REQUIRE(v.patch == 0);
    REQUIRE(v.revision.empty());

    static_assert(v.major == 2, "major must match aggregate value");
    static_assert(v.minor == 5, "minor must match aggregate value");
    static_assert(v.patch == 0, "unspecified patch must be zero");
    static_assert(v.revision.empty(), "unspecified revision must be empty");
  }
}

// operator== requires all four fields to match; any difference makes versions not equal.
TEST_CASE("app/version/equality") {
  // All fields identical.
  SUBCASE("equal") {
    constexpr Version v1{1, 2, 3, "346ca09"};
    constexpr Version v2{1, 2, 3, "346ca09"};

    REQUIRE(v1 == v2);

    static_assert(v1 == v2, "versions with identical fields must compare equal");
  }

  // Each field independently breaks equality.
  SUBCASE("different_major") {
    constexpr Version v1{1, 2, 3, "346ca09"};
    constexpr Version v2{9, 2, 3, "346ca09"};

    REQUIRE(v1 != v2);

    static_assert(v1 != v2, "differing major must yield not equal");
  }

  SUBCASE("different_minor") {
    constexpr Version v1{1, 2, 3, "abc"};
    constexpr Version v2{1, 9, 3, "abc"};

    REQUIRE(v1 != v2);

    static_assert(v1 != v2, "differing minor must yield not equal");
  }

  SUBCASE("different_patch") {
    constexpr Version v1{1, 2, 3, "abc"};
    constexpr Version v2{1, 2, 9, "abc"};

    REQUIRE(v1 != v2);

    static_assert(v1 != v2, "differing patch must yield not equal");
  }

  SUBCASE("different_revision") {
    constexpr Version v1{1, 2, 3, "abc"};
    constexpr Version v2{1, 2, 3, "xyz"};

    REQUIRE(v1 != v2);
    static_assert(v1 != v2, "differing revision must yield not equal");
  }

  // Empty revision is distinct from any non-empty revision.
  SUBCASE("empty_vs_nonempty_revision") {
    constexpr Version v1{1, 2, 3, {}};
    constexpr Version v2{1, 2, 3, "abc"};

    REQUIRE(v1 != v2);
    static_assert(v1 != v2, "empty revision must differ from non-empty revision");
  }
}

// operator<=> applies field priority: major > minor > patch > revision (lexicographic).
TEST_CASE("app/version/ordering") {
  // Identical versions produce strong_ordering::equal.
  SUBCASE("equal") {
    constexpr Version v1{1, 2, 3, "abc"};
    constexpr Version v2{1, 2, 3, "abc"};

    REQUIRE((v1 <=> v2) == strong_ordering::equal);

    static_assert((v1 <=> v2) == strong_ordering::equal, "identical versions must order equal");
  }

  // Major field has the highest priority; all other fields are ignored when major differs.
  SUBCASE("major_dominates") {
    constexpr Version lo{1, 9, 9, "zzz"};
    constexpr Version hi{2, 0, 0, {}};

    REQUIRE((lo <=> hi) == strong_ordering::less);
    REQUIRE((hi <=> lo) == strong_ordering::greater);

    static_assert((lo <=> hi) == strong_ordering::less, "lesser major must order less regardless of other fields");
    static_assert((hi <=> lo) == strong_ordering::greater,
                  "greater major must order greater regardless of other fields");
  }

  // Minor field decides when major is equal.
  SUBCASE("minor_dominates") {
    constexpr Version lo{1, 1, 9, "zzz"};
    constexpr Version hi{1, 2, 0, {}};

    REQUIRE((lo <=> hi) == strong_ordering::less);
    REQUIRE((hi <=> lo) == strong_ordering::greater);

    static_assert((lo <=> hi) == strong_ordering::less, "lesser minor must order less when major is equal");
    static_assert((hi <=> lo) == strong_ordering::greater, "greater minor must order greater when major is equal");
  }

  // patch field decides when major and minor are equal.
  SUBCASE("patch_dominates") {
    constexpr Version lo{1, 1, 1, "zzz"};
    constexpr Version hi{1, 1, 2, {}};

    REQUIRE((lo <=> hi) == strong_ordering::less);
    REQUIRE((hi <=> lo) == strong_ordering::greater);

    static_assert((lo <=> hi) == strong_ordering::less, "lesser patch must order less when major and minor are equal");
    static_assert((hi <=> lo) == strong_ordering::greater,
                  "greater patch must order greater when major and minor are equal");
  }

  // Revision is compared last using lexicographic order.
  SUBCASE("revision_last") {
    constexpr Version lo{1, 1, 1, "abc"};
    constexpr Version hi{1, 1, 1, "xyz"};

    REQUIRE((lo <=> hi) == strong_ordering::less);
    REQUIRE((hi <=> lo) == strong_ordering::greater);

    static_assert((lo <=> hi) == strong_ordering::less, "lexicographically lesser revision must order less");
    static_assert((hi <=> lo) == strong_ordering::greater, "lexicographically greater revision must order greater");
  }

  // Empty revision orders before any non-empty revision.
  SUBCASE("empty_revision_before_nonempty") {
    constexpr Version lo{1, 1, 1, {}};
    constexpr Version hi{1, 1, 1, "a"};

    REQUIRE((lo <=> hi) == strong_ordering::less);

    static_assert((lo <=> hi) == strong_ordering::less, "empty revision must order before any non-empty revision");
  }
}

// <, <=, >, >=, != are consistent with operator<=> and operator==.
TEST_CASE("app/version/comparison_operators") {
  // Less-than is strict and irreflexive.
  SUBCASE("less_than") {
    constexpr Version lo{1, 2, 3, "aaa"};
    constexpr Version hi{1, 2, 3, "zzz"};

    REQUIRE(lo < hi);
    REQUIRE(!(hi < lo));
    REQUIRE(!(lo < lo));

    static_assert(lo < hi, "lesser version must be less");
    static_assert(!(hi < lo), "greater must not be less");
    static_assert(!(lo < lo), "less-than must be irreflexive");
  }

  // Less-than-or-equal holds for strictly lesser and equal versions.
  SUBCASE("less_than_or_equal") {
    constexpr Version lo{1, 2, 3, "aaa"};
    constexpr Version hi{1, 2, 3, "zzz"};
    constexpr Version eq{1, 2, 3, "aaa"};

    REQUIRE(lo <= hi);
    REQUIRE(lo <= eq);
    REQUIRE(!(hi <= lo));

    static_assert(lo <= hi, "lesser version must satisfy less-or-equal");
    static_assert(lo <= eq, "equal version must satisfy less-or-equal");
    static_assert(!(hi <= lo), "greater must not satisfy less-or-equal");
  }

  // Greater-than is strict and irreflexive.
  SUBCASE("greater_than") {
    constexpr Version lo{1, 2, 3, "aaa"};
    constexpr Version hi{1, 2, 3, "zzz"};

    REQUIRE(hi > lo);
    REQUIRE(!(lo > hi));
    REQUIRE(!(hi > hi));

    static_assert(hi > lo, "greater version must be greater");
    static_assert(!(lo > hi), "lesser must not be greater");
    static_assert(!(hi > hi), "greater-than must be irreflexive");
  }

  // Greater-than-or-equal holds for strictly greater and equal versions.
  SUBCASE("greater_than_or_equal") {
    constexpr Version lo{1, 2, 3, "aaa"};
    constexpr Version hi{1, 2, 3, "zzz"};
    constexpr Version eq{1, 2, 3, "zzz"};

    REQUIRE(hi >= lo);
    REQUIRE(hi >= eq);
    REQUIRE(!(lo >= hi));

    static_assert(hi >= lo, "greater version must satisfy greater-or-equal");
    static_assert(hi >= eq, "equal version must satisfy greater-or-equal");
    static_assert(!(lo >= hi), "lesser must not satisfy greater-or-equal");
  }
}

// Runtime ordering chain across all four fields (non-constexpr path).
TEST_CASE("app/version/runtime_ordering_chain") {
  const Version v1{1, 0, 0, {}};
  const Version v2{1, 1, 0, {}};
  const Version v3{1, 1, 1, {}};
  const Version v4{1, 1, 1, "abc"};
  const Version v5{2, 0, 0, {}};

  REQUIRE(v1 < v2);
  REQUIRE(v2 < v3);
  REQUIRE(v3 < v4);
  REQUIRE(v4 < v5);

  REQUIRE(v5 > v4);
  REQUIRE(v4 > v3);
  REQUIRE(v3 > v2);
  REQUIRE(v2 > v1);

  REQUIRE(v1 == Version{1, 0, 0, {}});
  REQUIRE(v4 == Version{1, 1, 1, "abc"});
  REQUIRE(v1 != v5);
}

} // namespace toy::application
