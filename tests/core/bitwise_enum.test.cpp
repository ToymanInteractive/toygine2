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

// Opted in by hand below, because the macro rejects a signed underlying type at the invocation.
enum class SignedFlags : int8_t {
  None = 0,
  A    = 1,
};

// Opted in by hand below for the same reason; over bool the complement of every value is true.
enum class BoolFlags : bool {
  None = false,
  A    = true,
};

// Opted in by hand below; an enum class without a fixed underlying type takes int, which is signed.
enum class DefaultFlags {
  None = 0,
  A    = 1,
};

template <>
struct EnableBitwiseOperators<SignedFlags> {
  static constexpr bool enable = true;
};

template <>
struct EnableBitwiseOperators<BoolFlags> {
  static constexpr bool enable = true;
};

template <>
struct EnableBitwiseOperators<DefaultFlags> {
  static constexpr bool enable = true;
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

// Outside namespace toy, as a gameplay flag set would be; argument-dependent lookup has to find the operators here.
namespace outside {

enum class OutsideFlags : uint8_t {
  None = 0,
  A    = 1,
  B    = 2,
};

} // namespace outside

ENABLE_BITWISE_OPERATORS(toy, TestFlags)
ENABLE_BITWISE_OPERATORS(toy, TemplatedHolder<int, long>::Flags)
ENABLE_BITWISE_OPERATORS(outside, OutsideFlags)

namespace toy {

namespace {

// Detects operator| through a dependent expression; a non-dependent one is a hard error, not a false requirement.
template <typename T>
concept HasBitwiseOperators = requires(T lhs, T rhs) { lhs | rhs; };

// True when any binary operator accepts the pair, so a negative check catches each of the three.
template <typename Left, typename Right>
concept HasMixedOperator = requires(Left lhs, Right rhs) { lhs | rhs; } || requires(Left lhs, Right rhs) { lhs & rhs; }
                           || requires(Left lhs, Right rhs) { lhs ^ rhs; };

// Detects the membership helpers through a dependent call, as HasBitwiseOperators does for operator|.
template <typename T>
concept HasMembershipTests = requires(T flags, T mask) {
  hasAny(flags, mask);
  hasAll(flags, mask);
};

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
    static_assert(!HasMembershipTests<UnregisteredFlags>,
                  "hasAny() and hasAll() must be unavailable without the opt-in");
  }

  // An opt-in does not admit a signed or bool underlying type, whether spelled out or left to the int default.
  SUBCASE("disabled_for_signed_or_bool_underlying_type") {
    REQUIRE(EnableBitwiseOperators<SignedFlags>::enable == true);
    REQUIRE(HasBitwiseOperators<SignedFlags> == false);
    REQUIRE(HasBitwiseOperators<BoolFlags> == false);
    REQUIRE(HasBitwiseOperators<DefaultFlags> == false);

    static_assert(!BitwiseEnum<SignedFlags>, "a signed underlying type must not satisfy BitwiseEnum");
    static_assert(!BitwiseEnum<BoolFlags>, "a bool underlying type must not satisfy BitwiseEnum");
    static_assert(!BitwiseEnum<DefaultFlags>, "the default int underlying type must not satisfy BitwiseEnum");
  }

  // Operands of two types, or of a flag type and an integer, find no operator.
  SUBCASE("disabled_for_mixed_operand_types") {
    REQUIRE(HasMixedOperator<TestFlags, outside::OutsideFlags> == false);
    REQUIRE(HasMixedOperator<TestFlags, uint8_t> == false);
    REQUIRE(HasMixedOperator<uint8_t, TestFlags> == false);

    static_assert(!HasMixedOperator<TestFlags, outside::OutsideFlags>, "two flag types must not combine");
    static_assert(!HasMixedOperator<TestFlags, uint8_t>, "a flag type must not combine with an integer");
    static_assert(!HasMixedOperator<uint8_t, TestFlags>, "an integer must not combine with a flag type");
  }
}

// ENABLE_BITWISE_OPERATORS takes its argument variadically, so a template argument list survives the preprocessor.
TEST_CASE("enable_bitwise_operators/opt_in_through_template_argument_list") {
  using flags_type = TemplatedHolder<int, long>::Flags;

  REQUIRE(EnableBitwiseOperators<flags_type>::enable == true);
  REQUIRE(std::to_underlying(flags_type::A | flags_type::B) == 3);

  static_assert(BitwiseEnum<flags_type>, "an enum spelled with a template argument list must satisfy BitwiseEnum");
  static_assert(std::to_underlying(flags_type::A | flags_type::B) == 3,
                "flags_type::A | flags_type::B must equal 3 through the macro-generated specialization");
}

// Bitwise OR.
TEST_CASE("enable_bitwise_operators/operator_or") {
  // TestFlags::A | TestFlags::B combines bits.
  SUBCASE("or_combines_bits") {
    REQUIRE(std::to_underlying(TestFlags::A | TestFlags::B) == 3);

    static_assert(std::to_underlying(TestFlags::A | TestFlags::B) == 3, "TestFlags::A | TestFlags::B must equal 3");
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
    REQUIRE(std::to_underlying(TestFlags::A & TestFlags::B) == 0);

    static_assert(std::to_underlying(TestFlags::A & TestFlags::B) == 0, "TestFlags::A & TestFlags::B must be zero");
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
    REQUIRE(std::to_underlying(TestFlags::C ^ TestFlags::C) == 0);

    static_assert(std::to_underlying(TestFlags::C ^ TestFlags::C) == 0, "TestFlags::C ^ TestFlags::C must be zero");
  }
}

// Bitwise NOT.
TEST_CASE("enable_bitwise_operators/operator_not") {
  // ~TestFlags::None has all bits set for underlying type.
  SUBCASE("not_none_sets_every_bit") {
    REQUIRE(std::to_underlying(~TestFlags::None) == 0xFF);

    static_assert(std::to_underlying(~TestFlags::None) == 0xFF, "~TestFlags::None must have all bits set for uint8_t");
  }

  // ~TestFlags::A clears bit A and sets every other bit, undeclared ones included.
  SUBCASE("not_clears_the_operand_bits") {
    REQUIRE(std::to_underlying(~TestFlags::A) == 0xFE);

    static_assert(std::to_underlying(~TestFlags::A) == 0xFE, "~TestFlags::A must clear bit A only");
  }

  // Masked against the declared flags, the complement keeps the flags the operand lacks.
  SUBCASE("not_masked_to_declared_flags") {
    constexpr auto declared = TestFlags::A | TestFlags::B | TestFlags::C;

    REQUIRE((~TestFlags::A & declared) == (TestFlags::B | TestFlags::C));

    static_assert((~TestFlags::A & declared) == (TestFlags::B | TestFlags::C),
                  "~TestFlags::A masked to the declared flags must equal TestFlags::B | TestFlags::C");
  }
}

// Compound assignment operators.
TEST_CASE("enable_bitwise_operators/compound_assignment") {
  // operator|= assigns lhs | rhs to lhs and returns reference.
  SUBCASE("or_assign") {
    auto   f   = TestFlags::A;
    auto & ref = (f |= TestFlags::B);

    REQUIRE(f == (TestFlags::A | TestFlags::B));
    REQUIRE(&ref == &f);

    static_assert(
      [] {
        auto flags  = TestFlags::A;
        flags      |= TestFlags::B;
        return flags;
      }()
        == (TestFlags::A | TestFlags::B),
      "TestFlags::A |= TestFlags::B must yield TestFlags::A | TestFlags::B in a constant expression"
    );
  }

  // operator&= assigns lhs & rhs to lhs and returns reference.
  SUBCASE("and_assign") {
    auto   f   = TestFlags::A | TestFlags::B;
    auto & ref = (f &= TestFlags::A);

    REQUIRE(f == TestFlags::A);
    REQUIRE(&ref == &f);

    static_assert(
      [] {
        auto flags  = TestFlags::A | TestFlags::B;
        flags      &= TestFlags::A;
        return flags;
      }()
        == TestFlags::A,
      "(TestFlags::A | TestFlags::B) &= TestFlags::A must yield TestFlags::A in a constant expression"
    );
  }

  // operator^= assigns lhs ^ rhs to lhs and returns reference.
  SUBCASE("xor_assign") {
    auto   f   = TestFlags::A | TestFlags::B;
    auto & ref = (f ^= TestFlags::B);

    REQUIRE(f == TestFlags::A);
    REQUIRE(&ref == &f);

    static_assert(
      [] {
        auto flags  = TestFlags::A | TestFlags::B;
        flags      ^= TestFlags::B;
        return flags;
      }()
        == TestFlags::A,
      "(TestFlags::A | TestFlags::B) ^= TestFlags::B must yield TestFlags::A in a constant expression"
    );
  }
}

// hasAny() reports whether the flags share at least one bit with the mask.
TEST_CASE("enable_bitwise_operators/has_any") {
  // One shared bit is enough, even when the mask carries others the flags lack.
  SUBCASE("true_for_a_shared_bit") {
    REQUIRE(hasAny(TestFlags::A | TestFlags::B, TestFlags::B | TestFlags::C) == true);

    static_assert(hasAny(TestFlags::A | TestFlags::B, TestFlags::B | TestFlags::C),
                  "flags sharing bit B with the mask must report a shared bit");
  }

  // Disjoint bits share nothing.
  SUBCASE("false_for_disjoint_bits") {
    REQUIRE(hasAny(TestFlags::A, TestFlags::B | TestFlags::C) == false);

    static_assert(!hasAny(TestFlags::A, TestFlags::B | TestFlags::C), "disjoint bits must report no shared bit");
  }

  // An empty mask has no bit to share.
  SUBCASE("false_for_an_empty_mask") {
    REQUIRE(hasAny(TestFlags::A | TestFlags::B, TestFlags::None) == false);

    static_assert(!hasAny(TestFlags::A | TestFlags::B, TestFlags::None), "an empty mask must report no shared bit");
  }
}

// hasAll() reports whether the flags carry every bit of the mask.
TEST_CASE("enable_bitwise_operators/has_all") {
  // Every bit of the mask is present, alongside bits the mask does not name.
  SUBCASE("true_when_every_mask_bit_is_set") {
    REQUIRE(hasAll(TestFlags::A | TestFlags::B | TestFlags::C, TestFlags::A | TestFlags::C) == true);

    static_assert(hasAll(TestFlags::A | TestFlags::B | TestFlags::C, TestFlags::A | TestFlags::C),
                  "flags carrying A and C must report the mask A | C as present");
  }

  // One missing bit is enough to fail.
  SUBCASE("false_when_a_mask_bit_is_missing") {
    REQUIRE(hasAll(TestFlags::A | TestFlags::B, TestFlags::A | TestFlags::C) == false);

    static_assert(!hasAll(TestFlags::A | TestFlags::B, TestFlags::A | TestFlags::C),
                  "flags lacking C must not report the mask A | C as present");
  }

  // An empty mask names no bit, so every value carries all of it.
  SUBCASE("true_for_an_empty_mask") {
    REQUIRE(hasAll(TestFlags::None, TestFlags::None) == true);

    static_assert(hasAll(TestFlags::None, TestFlags::None), "an empty mask must be present in every value");
  }
}

} // namespace toy

// Operators and membership helpers resolve for an enumeration outside namespace toy, called from the global namespace.
TEST_CASE("enable_bitwise_operators/operators_reach_an_enumeration_outside_toy") {
  using outside::OutsideFlags;

  auto flags  = OutsideFlags::A;
  flags      |= OutsideFlags::B;
  flags      &= OutsideFlags::A | OutsideFlags::B;
  flags      ^= OutsideFlags::B;

  REQUIRE(flags == OutsideFlags::A);
  REQUIRE(std::to_underlying(OutsideFlags::A | OutsideFlags::B) == 3);
  REQUIRE((OutsideFlags::A & OutsideFlags::B) == OutsideFlags::None);
  REQUIRE((OutsideFlags::A ^ OutsideFlags::A) == OutsideFlags::None);
  REQUIRE(std::to_underlying(~OutsideFlags::None) == 0xFF);
  REQUIRE(hasAny(OutsideFlags::A | OutsideFlags::B, OutsideFlags::B) == true);
  REQUIRE(hasAll(OutsideFlags::A | OutsideFlags::B, OutsideFlags::A | OutsideFlags::B) == true);

  static_assert(std::to_underlying(OutsideFlags::A | OutsideFlags::B) == 3,
                "operator| must resolve for an enumeration outside namespace toy");
  static_assert((OutsideFlags::A & OutsideFlags::B) == OutsideFlags::None,
                "operator& must resolve for an enumeration outside namespace toy");
  static_assert((OutsideFlags::A ^ OutsideFlags::A) == OutsideFlags::None,
                "operator^ must resolve for an enumeration outside namespace toy");
  static_assert(std::to_underlying(~OutsideFlags::None) == 0xFF,
                "operator~ must resolve for an enumeration outside namespace toy");
  static_assert(hasAny(OutsideFlags::A, OutsideFlags::A),
                "hasAny() must resolve for an enumeration outside namespace toy");
  static_assert(hasAll(OutsideFlags::A, OutsideFlags::A),
                "hasAll() must resolve for an enumeration outside namespace toy");
}
