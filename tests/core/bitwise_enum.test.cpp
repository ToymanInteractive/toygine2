//
// Copyright (c) 2026 Toyman Interactive
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
  \file   bitwise_enum.test.cpp
  \brief  Unit tests for \ref toy::EnableBitwiseOperators and scoped-enum bitwise operators.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

enum class TestFlags : uint8_t {
  None = 0,
  A    = 1,
  B    = 2,
  C    = 4,
};

// Scoped enum deliberately left without ENABLE_BITWISE_OPERATORS.
enum class UnregisteredFlags : uint8_t {
  None = 0,
  A    = 1,
};

// Nested enum reached through a template argument list, so its spelling carries a comma into the macro.
template <typename First, typename Second>
struct TemplatedHolder {
  enum class Flags : uint8_t {
    None = 0,
    A    = 1,
    B    = 2,
  };
};

} // namespace toy

ENABLE_BITWISE_OPERATORS(toy::TestFlags)
ENABLE_BITWISE_OPERATORS(toy::TemplatedHolder<int, long>::Flags)

namespace toy {

namespace {

// Detects operator| through a dependent expression; a non-dependent one is a hard error, not a false requirement.
template <typename T>
concept HasBitwiseOperators = requires(T lhs, T rhs) { lhs | rhs; };

} // namespace

// Trait EnableBitwiseOperators: enabled for specialized enum, disabled otherwise.
TEST_CASE("enable_bitwise_operators/trait") {
  // Specialized enum has enable == true and carries the operators.
  SUBCASE("enabled_for_specialized_enum") {
    REQUIRE(EnableBitwiseOperators<TestFlags>::enable == true);
    REQUIRE(HasBitwiseOperators<TestFlags> == true);

    static_assert(EnableBitwiseOperators<TestFlags>::enable == true,
                  "EnableBitwiseOperators<TestFlags>::enable must be true after ENABLE_BITWISE_OPERATORS");
    static_assert(BitwiseEnum<TestFlags>, "TestFlags must satisfy BitwiseEnum after ENABLE_BITWISE_OPERATORS");
  }

  // Non-specialized type has enable == false.
  SUBCASE("disabled_for_other_types") {
    struct S {};

    REQUIRE(EnableBitwiseOperators<S>::enable == false);

    static_assert(EnableBitwiseOperators<S>::enable == false,
                  "EnableBitwiseOperators for non-specialized type must be false");
    static_assert(!BitwiseEnum<S>, "a non-enumeration type must not satisfy BitwiseEnum");
  }

  // Scoped enum without the opt-in keeps the operators out of the overload set.
  SUBCASE("disabled_for_scoped_enum_without_opt_in") {
    REQUIRE(EnableBitwiseOperators<UnregisteredFlags>::enable == false);
    REQUIRE(HasBitwiseOperators<UnregisteredFlags> == false);

    static_assert(!BitwiseEnum<UnregisteredFlags>, "an enum without the opt-in must not satisfy BitwiseEnum");
    static_assert(!HasBitwiseOperators<UnregisteredFlags>, "operator| must be unavailable without the opt-in");
  }
}

// ENABLE_BITWISE_OPERATORS takes its argument variadically, so a template argument list survives the preprocessor.
TEST_CASE("enable_bitwise_operators/opt_in_through_template_argument_list") {
  using flags_type = TemplatedHolder<int, long>::Flags;

  REQUIRE(EnableBitwiseOperators<flags_type>::enable == true);
  REQUIRE(toy::to_underlying(flags_type::A | flags_type::B) == 4);

  static_assert(BitwiseEnum<flags_type>, "an enum spelled with a template argument list must satisfy BitwiseEnum");
  static_assert(toy::to_underlying(flags_type::A | flags_type::B) == 3,
                "flags_type::A | flags_type::B must equal 3 through the macro-generated specialization");
}

// Bitwise OR.
TEST_CASE("enable_bitwise_operators/operator_or") {
  // TestFlags::A | TestFlags::B combines bits.
  SUBCASE("or_combines_bits") {
    REQUIRE(toy::to_underlying(TestFlags::A | TestFlags::B) == 3);

    static_assert(toy::to_underlying(TestFlags::A | TestFlags::B) == 3, "TestFlags::A | TestFlags::B must equal 3");
  }

  // TestFlags::None | TestFlags::A yields TestFlags::A (TestFlags::None is zero).
  SUBCASE("or_with_none_preserves_other") {
    REQUIRE((TestFlags::None | TestFlags::A) == TestFlags::A);

    static_assert((TestFlags::None | TestFlags::A) == TestFlags::A,
                  "TestFlags::None | TestFlags::A must equal TestFlags::A");
  }
}

// Bitwise AND.
TEST_CASE("enable_bitwise_operators/operator_and") {
  // (TestFlags::A | TestFlags::B) & TestFlags::A yields TestFlags::A.
  SUBCASE("and_intersection") {
    REQUIRE(((TestFlags::A | TestFlags::B) & TestFlags::A) == TestFlags::A);

    static_assert(((TestFlags::A | TestFlags::B) & TestFlags::A) == TestFlags::A,
                  "(TestFlags::A|TestFlags::B) & TestFlags::A must equal TestFlags::A");
  }

  // TestFlags::A & TestFlags::B has no common bits; underlying value is zero.
  SUBCASE("and_disjoint_bits") {
    REQUIRE(toy::to_underlying(TestFlags::A & TestFlags::B) == 0);

    static_assert(toy::to_underlying(TestFlags::A & TestFlags::B) == 0, "TestFlags::A & TestFlags::B must be zero");
  }
}

// Bitwise XOR.
TEST_CASE("enable_bitwise_operators/operator_xor") {
  // TestFlags::A ^ TestFlags::B yields bits in exactly one operand.
  SUBCASE("xor_symmetric_difference") {
    REQUIRE((TestFlags::A ^ TestFlags::B) == (TestFlags::A | TestFlags::B));

    static_assert((TestFlags::A ^ TestFlags::B) == (TestFlags::A | TestFlags::B),
                  "TestFlags::A ^ TestFlags::B must equal TestFlags::A | TestFlags::B");
  }

  // Same ^ same yields zero.
  SUBCASE("xor_same_yields_zero") {
    REQUIRE(toy::to_underlying(TestFlags::C ^ TestFlags::C) == 0);

    static_assert(toy::to_underlying(TestFlags::C ^ TestFlags::C) == 0, "TestFlags::C ^ TestFlags::C must be zero");
  }
}

// Bitwise NOT.
TEST_CASE("enable_bitwise_operators/operator_not") {
  // ~TestFlags::None has all bits set for underlying type.
  REQUIRE(toy::to_underlying(~TestFlags::None) == 0xFF);

  static_assert(toy::to_underlying(~TestFlags::None) == 0xFF, "~TestFlags::None must have all bits set for uint8_t");
}

// Compound assignment operators.
TEST_CASE("enable_bitwise_operators/compound_assignment") {
  // operator|= assigns lhs | rhs to lhs and returns reference.
  SUBCASE("or_assign") {
    auto   f   = TestFlags::A;
    auto & ref = (f |= TestFlags::B);

    REQUIRE(f == (TestFlags::A | TestFlags::B));
    REQUIRE(&ref == &f);
  }

  // operator&= assigns lhs & rhs to lhs.
  SUBCASE("and_assign") {
    auto f  = TestFlags::A | TestFlags::B;
    f      &= TestFlags::A;

    REQUIRE(f == TestFlags::A);
  }

  // operator^= assigns lhs ^ rhs to lhs.
  SUBCASE("xor_assign") {
    auto f  = TestFlags::A | TestFlags::B;
    f      ^= TestFlags::B;

    REQUIRE(f == TestFlags::A);
  }
}

} // namespace toy
