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

#include "core.hpp"

namespace toy {

enum class TestFlags : uint8_t { None = 0, A = 1, B = 2, C = 4 };

} // namespace toy

ENABLE_BITWISE_OPERATORS(toy::TestFlags)

namespace toy {

// Trait EnableBitwiseOperators: enabled for specialized enum, disabled otherwise.
TEST_CASE("core/bitwise_enum/trait") {
  // Specialized enum has enable == true.
  SUBCASE("enabled_for_specialized_enum") {
    REQUIRE(EnableBitwiseOperators<TestFlags>::enable == true);

    static_assert(EnableBitwiseOperators<TestFlags>::enable == true,
                  "EnableBitwiseOperators<TestFlags>::enable must be true after ENABLE_BITWISE_OPERATORS");
  }

  // Non-specialized type has enable == false.
  SUBCASE("disabled_for_other_types") {
    struct S {};

    REQUIRE(EnableBitwiseOperators<S>::enable == false);

    static_assert(EnableBitwiseOperators<S>::enable == false,
                  "EnableBitwiseOperators for non-specialized type must be false");
  }
}

// Bitwise OR.
TEST_CASE("core/bitwise_enum/operator_or") {
  // TestFlags::A | TestFlags::B combines bits.
  SUBCASE("or_combines_bits") {
    constexpr auto combined = TestFlags::A | TestFlags::B;

    REQUIRE(std::to_underlying(combined) == 3);

    static_assert(std::to_underlying(TestFlags::A | TestFlags::B) == 3, "TestFlags::A | TestFlags::B must equal 3");
  }

  // TestFlags::None | TestFlags::A yields TestFlags::A (TestFlags::None is zero).
  SUBCASE("or_with_TestFlags::None_preserves_other") {
    constexpr auto combined = TestFlags::None | TestFlags::A;

    REQUIRE(combined == TestFlags::A);

    static_assert(combined == TestFlags::A, "TestFlags::None | TestFlags::A must equal TestFlags::A");
  }
}

// Bitwise AND.
TEST_CASE("core/bitwise_enum/operator_and") {
  // (TestFlags::A | TestFlags::B) & TestFlags::A yields TestFlags::A.
  SUBCASE("and_intersection") {
    constexpr auto combined = TestFlags::A | TestFlags::B;

    REQUIRE((combined & TestFlags::A) == TestFlags::A);

    static_assert((combined & TestFlags::A) == TestFlags::A,
                  "(TestFlags::A|TestFlags::B) & TestFlags::A must equal TestFlags::A");
  }

  // TestFlags::A & TestFlags::B has no common bits; underlying value is zero.
  SUBCASE("and_disjoint_bits") {
    REQUIRE(std::to_underlying(TestFlags::A & TestFlags::B) == 0);

    static_assert(std::to_underlying(TestFlags::A & TestFlags::B) == 0, "TestFlags::A & TestFlags::B must be zero");
  }
}

// Bitwise XOR.
TEST_CASE("core/bitwise_enum/operator_xor") {
  // TestFlags::A ^ TestFlags::B yields bits in exactly one operand.
  SUBCASE("xor_symmetric_difference") {
    REQUIRE((TestFlags::A ^ TestFlags::B) == (TestFlags::A | TestFlags::B));

    static_assert((TestFlags::A ^ TestFlags::B) == (TestFlags::A | TestFlags::B),
                  "TestFlags::A ^ TestFlags::B must equal TestFlags::A | TestFlags::B");
  }

  // Same ^ same yields zero.
  SUBCASE("xor_same_yields_zero") {
    REQUIRE(std::to_underlying(TestFlags::C ^ TestFlags::C) == 0);

    static_assert(std::to_underlying(TestFlags::C ^ TestFlags::C) == 0, "TestFlags::C ^ TestFlags::C must be zero");
  }
}

// Bitwise NOT.
TEST_CASE("core/bitwise_enum/operator_not") {
  // ~TestFlags::None has all bits set for underlying type.
  REQUIRE(std::to_underlying(~TestFlags::None) == 0xFF);

  static_assert(std::to_underlying(~TestFlags::None) == 0xFF, "~TestFlags::None must have all bits set for uint8_t");
}

// Compound assignment operators.
TEST_CASE("core/bitwise_enum/compound_assignment") {
  // operator|= assigns lhs | rhs to lhs and returns reference.
  SUBCASE("or_assign") {
    TestFlags f = TestFlags::A;
    TestFlags & ref = (f |= TestFlags::B);

    REQUIRE(f == (TestFlags::A | TestFlags::B));
    REQUIRE(&ref == &f);
  }

  // operator&= assigns lhs & rhs to lhs.
  SUBCASE("and_assign") {
    TestFlags f = TestFlags::A | TestFlags::B;
    f &= TestFlags::A;

    REQUIRE(f == TestFlags::A);
  }

  // operator^= assigns lhs ^ rhs to lhs.
  SUBCASE("xor_assign") {
    TestFlags f = TestFlags::A | TestFlags::B;
    f ^= TestFlags::B;

    REQUIRE(f == TestFlags::A);
  }
}

} // namespace toy
