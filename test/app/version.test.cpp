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

#include "app.hpp"

using namespace toy::app;

TEST_CASE("Version constructors", "[app][version]") {
  SECTION("Default constructor") {
    constexpr Version version;

    REQUIRE(version.major == 0);
    REQUIRE(version.minor == 0);
    REQUIRE(version.maintenance == 0);
    REQUIRE(version.revision == 0);

    // Compile-time checks
    STATIC_REQUIRE(version.major == 0);
    STATIC_REQUIRE(version.minor == 0);
    STATIC_REQUIRE(version.maintenance == 0);
    STATIC_REQUIRE(version.revision == 0);
  }

  SECTION("Aggregate initialization") {
    constexpr Version version{5, 10, 15, 20};

    REQUIRE(version.major == 5);
    REQUIRE(version.minor == 10);
    REQUIRE(version.maintenance == 15);
    REQUIRE(version.revision == 20);

    // Compile-time checks
    STATIC_REQUIRE(version.major == 5);
    STATIC_REQUIRE(version.minor == 10);
    STATIC_REQUIRE(version.maintenance == 15);
    STATIC_REQUIRE(version.revision == 20);
  }

  SECTION("Partial initialization") {
    constexpr Version version{1, 2};

    REQUIRE(version.major == 1);
    REQUIRE(version.minor == 2);
    REQUIRE(version.maintenance == 0);
    REQUIRE(version.revision == 0);

    // Compile-time checks
    STATIC_REQUIRE(version.major == 1);
    STATIC_REQUIRE(version.minor == 2);
    STATIC_REQUIRE(version.maintenance == 0);
    STATIC_REQUIRE(version.revision == 0);
  }
}

TEST_CASE("Version equality operator", "[app][version]") {
  SECTION("Identical versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 4};
    constexpr bool result = v1 == v2;

    REQUIRE(result == true);

    // Compile-time checks
    STATIC_REQUIRE(result == true);
  }

  SECTION("Different major versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{2, 2, 3, 4};
    constexpr bool result = v1 == v2;

    REQUIRE(result == false);

    // Compile-time checks
    STATIC_REQUIRE(result == false);
  }

  SECTION("Different minor versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 3, 3, 4};
    constexpr bool result = v1 == v2;

    REQUIRE(result == false);

    // Compile-time checks
    STATIC_REQUIRE(result == false);
  }

  SECTION("Different maintenance versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 4, 4};
    constexpr bool result = v1 == v2;

    REQUIRE(result == false);

    // Compile-time checks
    STATIC_REQUIRE(result == false);
  }

  SECTION("Different revision versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};
    constexpr bool result = v1 == v2;

    REQUIRE(result == false);

    // Compile-time checks
    STATIC_REQUIRE(result == false);
  }

  SECTION("Zero versions") {
    constexpr Version v1{0, 0, 0, 0};
    constexpr Version v2{0, 0, 0, 0};
    constexpr bool result = v1 == v2;

    REQUIRE(result == true);

    // Compile-time checks
    STATIC_REQUIRE(result == true);
  }

  SECTION("Mixed zero and non-zero") {
    constexpr Version v1{0, 0, 0, 0};
    constexpr Version v2{0, 0, 0, 1};
    constexpr bool result = v1 == v2;

    REQUIRE(result == false);

    // Compile-time checks
    STATIC_REQUIRE(result == false);
  }
}

TEST_CASE("Version three-way comparison operator", "[app][version]") {
  SECTION("Equal versions") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 4};
    constexpr auto result = v1 <=> v2;

    REQUIRE(result == std::strong_ordering::equal);

    // Compile-time checks
    STATIC_REQUIRE(result == std::strong_ordering::equal);
  }

  SECTION("Major version comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{2, 1, 1, 1};
    constexpr auto result = v1 <=> v2;

    REQUIRE(result == std::strong_ordering::less);

    // Compile-time checks
    STATIC_REQUIRE(result == std::strong_ordering::less);

    constexpr Version v3{2, 1, 1, 1};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    REQUIRE(result2 == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE(result2 == std::strong_ordering::greater);
  }

  SECTION("Minor version comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 3, 1, 1};
    constexpr auto result = v1 <=> v2;

    REQUIRE(result == std::strong_ordering::less);

    // Compile-time checks
    STATIC_REQUIRE(result == std::strong_ordering::less);

    constexpr Version v3{1, 3, 1, 1};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    REQUIRE(result2 == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE(result2 == std::strong_ordering::greater);
  }

  SECTION("Maintenance version comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 4, 1};
    constexpr auto result = v1 <=> v2;

    REQUIRE(result == std::strong_ordering::less);

    // Compile-time checks
    STATIC_REQUIRE(result == std::strong_ordering::less);

    constexpr Version v3{1, 2, 4, 1};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    REQUIRE(result2 == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE(result2 == std::strong_ordering::greater);
  }

  SECTION("Revision version comparison") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};
    constexpr auto result = v1 <=> v2;

    REQUIRE(result == std::strong_ordering::less);

    // Compile-time checks
    STATIC_REQUIRE(result == std::strong_ordering::less);

    constexpr Version v3{1, 2, 3, 5};
    constexpr Version v4{1, 2, 3, 4};
    constexpr auto result2 = v3 <=> v4;

    REQUIRE(result2 == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE(result2 == std::strong_ordering::greater);
  }

  SECTION("Zero versions") {
    constexpr Version v1{0, 0, 0, 0};
    constexpr Version v2{0, 0, 0, 0};
    constexpr auto result = v1 <=> v2;

    REQUIRE(result == std::strong_ordering::equal);

    // Compile-time checks
    STATIC_REQUIRE(result == std::strong_ordering::equal);
  }
}

TEST_CASE("Version comparison operators", "[app][version]") {
  SECTION("Less than operator") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};

    REQUIRE(v1 < v2);
    REQUIRE(!(v2 < v1));
    REQUIRE(!(v1 < v1));

    // Compile-time checks
    STATIC_REQUIRE(v1 < v2);
    STATIC_REQUIRE(!(v2 < v1));
    STATIC_REQUIRE(!(v1 < v1));
  }

  SECTION("Less than or equal operator") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};
    constexpr Version v3{1, 2, 3, 4};

    REQUIRE(v1 <= v2);
    REQUIRE(v1 <= v3);
    REQUIRE(!(v2 <= v1));

    // Compile-time checks
    STATIC_REQUIRE(v1 <= v2);
    STATIC_REQUIRE(v1 <= v3);
    STATIC_REQUIRE(!(v2 <= v1));
  }

  SECTION("Greater than operator") {
    constexpr Version v1{1, 2, 3, 5};
    constexpr Version v2{1, 2, 3, 4};

    REQUIRE(v1 > v2);
    REQUIRE(!(v2 > v1));
    REQUIRE(!(v1 > v1));

    // Compile-time checks
    STATIC_REQUIRE(v1 > v2);
    STATIC_REQUIRE(!(v2 > v1));
    STATIC_REQUIRE(!(v1 > v1));
  }

  SECTION("Greater than or equal operator") {
    constexpr Version v1{1, 2, 3, 5};
    constexpr Version v2{1, 2, 3, 4};
    constexpr Version v3{1, 2, 3, 5};

    REQUIRE(v1 >= v2);
    REQUIRE(v1 >= v3);
    REQUIRE(!(v2 >= v1));

    // Compile-time checks
    STATIC_REQUIRE(v1 >= v2);
    STATIC_REQUIRE(v1 >= v3);
    STATIC_REQUIRE(!(v2 >= v1));
  }

  SECTION("Not equal operator") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 3, 5};

    REQUIRE(v1 != v2);
    REQUIRE(!(v1 != v1));

    // Compile-time checks
    STATIC_REQUIRE(v1 != v2);
    STATIC_REQUIRE(!(v1 != v1));
  }
}

TEST_CASE("Version semantic versioning scenarios", "[app][version]") {
  SECTION("Major version precedence") {
    constexpr Version v1{1, 0, 0, 0};
    constexpr Version v2{2, 0, 0, 0};

    REQUIRE(v1 < v2);
    REQUIRE(v2 > v1);
    REQUIRE(v1 != v2);

    // Compile-time checks
    STATIC_REQUIRE(v1 < v2);
    STATIC_REQUIRE(v2 > v1);
    STATIC_REQUIRE(v1 != v2);
  }

  SECTION("Minor version precedence") {
    constexpr Version v1{1, 1, 0, 0};
    constexpr Version v2{1, 2, 0, 0};

    REQUIRE(v1 < v2);
    REQUIRE(v2 > v1);
    REQUIRE(v1 != v2);

    // Compile-time checks
    STATIC_REQUIRE(v1 < v2);
    STATIC_REQUIRE(v2 > v1);
    STATIC_REQUIRE(v1 != v2);
  }

  SECTION("Maintenance version precedence") {
    constexpr Version v1{1, 1, 1, 0};
    constexpr Version v2{1, 1, 2, 0};

    REQUIRE(v1 < v2);
    REQUIRE(v2 > v1);
    REQUIRE(v1 != v2);

    // Compile-time checks
    STATIC_REQUIRE(v1 < v2);
    STATIC_REQUIRE(v2 > v1);
    STATIC_REQUIRE(v1 != v2);
  }

  SECTION("Revision version precedence") {
    constexpr Version v1{1, 1, 1, 1};
    constexpr Version v2{1, 1, 1, 2};

    REQUIRE(v1 < v2);
    REQUIRE(v2 > v1);
    REQUIRE(v1 != v2);

    // Compile-time checks
    STATIC_REQUIRE(v1 < v2);
    STATIC_REQUIRE(v2 > v1);
    STATIC_REQUIRE(v1 != v2);
  }

  SECTION("Complex version comparisons") {
    constexpr Version v1{1, 2, 3, 4};
    constexpr Version v2{1, 2, 4, 0};
    constexpr Version v3{1, 3, 0, 0};
    constexpr Version v4{2, 0, 0, 0};

    REQUIRE(v1 < v2);
    REQUIRE(v2 < v3);
    REQUIRE(v3 < v4);

    // Compile-time checks
    STATIC_REQUIRE(v1 < v2);
    STATIC_REQUIRE(v2 < v3);
    STATIC_REQUIRE(v3 < v4);
  }
}

TEST_CASE("Version runtime tests", "[app][version]") {
  SECTION("Runtime version creation and comparison") {
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
}
