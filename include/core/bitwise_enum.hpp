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
  \file   bitwise_enum.hpp
  \brief  Opt-in bitwise operators for scoped enumerations used as bit-flag sets.

  Defines \ref toy::EnableBitwiseOperators, the trait admitting one enumeration to the operator overloads declared here,
  \ref toy::BitwiseEnum, the concept those overloads constrain on, and the \c ENABLE_BITWISE_OPERATORS macro that writes
  the specialization and makes the operators visible to argument-dependent lookup. Used where a flag set is spelled as
  an \c enum class and callers combine, mask, and test its enumerators without casting to the underlying integer.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_BITWISE_ENUM_HPP_
#define INCLUDE_CORE_BITWISE_ENUM_HPP_

namespace toy {

/*!
  \struct EnableBitwiseOperators
  \brief Marks \a T as a bit-flag enumeration the operators in this header accept; denies every type by default.

  The primary template leaves \c enable at \c false, so no operator applies to an arbitrary type. A full specialization
  setting it to \c true, written by hand or through \c ENABLE_BITWISE_OPERATORS, opts one enumeration in. Every overload
  constrains on \ref toy::BitwiseEnum, which reads this value, so an enumeration that never opted in reports an unmet
  requirement instead of silently converting.

  \tparam T Scoped enumeration over an unsigned underlying type other than \c bool, whose enumerators name single bits.
            The primary template accepts any type; a specialization grants operators only to a type that
            \ref toy::BitwiseEnum also accepts.

  \section features Key Features

  * **Opt-in per type**: One specialization enables the whole operator set for one enumeration.
  * **Any named namespace**: \c ENABLE_BITWISE_OPERATORS makes the operators reachable by argument-dependent lookup, so
    call sites need no using-directive.
  * **Type safety**: Operands and results keep type \a T; the underlying integer never reaches the call site.
  * **Constexpr support**: Every operator evaluates in a constant expression.
  * **No allocation**: Each operator forwards to one integer operation on the underlying type.

  \section usage Usage Example

  \code
  #include "core.hpp"

  namespace game {

  enum class Flags : unsigned { none = 0u, a = 1u, b = 2u, c = 4u };

  } // namespace game

  ENABLE_BITWISE_OPERATORS(game, Flags)

  constexpr game::Flags combined = game::Flags::a | game::Flags::b;
  constexpr bool carriesA = (combined & game::Flags::a) != game::Flags::none;
  \endcode

  \section performance Performance Characteristics

  * **Trait**: resolved during compilation; it holds one \c static \c constexpr member and no state.
  * **Each operator**: O(1), one integer operation after inlining.

  \section safety Safety Guarantees

  * **Contracts**: none checked. The trait does not test whether the enumerators of \a T occupy disjoint bits, so
    overlapping values stay a logic error no diagnostic reports.
  * **Type safety**: participation is a \c requires clause, so an enumeration without a specialization, or over a signed
    or \c bool underlying type, fails to compile at the call site.
  * **Value range**: a result may hold a bit pattern no enumerator names. That is well defined, because \a T fixes its
    underlying type.
  * **Exception safety**: no operation throws; every operator is \c noexcept, and exceptions are off in the build.

  \warning The operators live in namespace \ref toy, and \c ENABLE_BITWISE_OPERATORS adds using-declarations of them to
           the namespace of the enumeration, where argument-dependent lookup finds them. A specialization written by
           hand adds none, so for an enumeration outside \ref toy the call site needs \c using \c namespace \c toy; or
           a using-declaration per operator.

  \sa \ref toy::BitwiseEnum, ENABLE_BITWISE_OPERATORS
*/
template <typename T>
struct EnableBitwiseOperators {
  /// \c true where the operators of this header accept \a T; the primary template leaves it \c false.
  static constexpr bool enable = false;
};

/*!
  \concept BitwiseEnum
  \brief Concept satisfied when \a T is an opted-in scoped enumeration over an unsigned underlying type.

  Constrains every operator below. Splitting the scoped-enum requirement from the opt-in keeps an unscoped enumeration
  out of the overload set, where the template would otherwise beat the built-in operator and quietly change the result
  type. The unsigned requirement exists for \c ~: over a signed type its result is negative, over \c bool every
  complement is \c true.

  \section bitwise_enum_requirements Requirements

  A type satisfies BitwiseEnum if and only if:
  * It is a scoped enumeration, as reported by \c std::is_scoped_enum_v.
  * Its underlying type satisfies \c std::unsigned_integral and is not \c bool.
  * A specialization of \ref toy::EnableBitwiseOperators sets its \c enable to \c true.

  \sa \ref toy::EnableBitwiseOperators
*/
template <typename T>
concept BitwiseEnum = std::is_scoped_enum_v<T> && std::unsigned_integral<std::underlying_type_t<T>>
                      && !std::same_as<std::underlying_type_t<T>, bool> && EnableBitwiseOperators<T>::enable;

/*!
  \brief Unites two flag sets.

  \tparam T Type satisfying \ref toy::BitwiseEnum.

  \param lhs Left operand.
  \param rhs Right operand.

  \return Value of \a T carrying every bit set in \a lhs, in \a rhs, or in both.

  \sa toy::operator|=(), toy::operator&()
*/
template <typename T>
  requires BitwiseEnum<T>
[[nodiscard]] constexpr T operator|(T lhs, T rhs) noexcept;

/*!
  \brief Intersects two flag sets; a membership test masks with this operator.

  \tparam T Type satisfying \ref toy::BitwiseEnum.

  \param lhs Left operand.
  \param rhs Right operand.

  \return Value of \a T carrying the bits set in \a lhs and in \a rhs alike.

  \note An empty intersection equals \c T{}, so a membership test compares against it or an enumerator whose value is
        zero.

  \sa toy::operator&=(), toy::operator|()
*/
template <typename T>
  requires BitwiseEnum<T>
[[nodiscard]] constexpr T operator&(T lhs, T rhs) noexcept;

/*!
  \brief Toggles the flags of \a rhs in \a lhs.

  \tparam T Type satisfying \ref toy::BitwiseEnum.

  \param lhs Left operand.
  \param rhs Right operand.

  \return Value of \a T carrying the bits set in exactly one of \a lhs and \a rhs.

  \sa toy::operator^=(), toy::operator|()
*/
template <typename T>
  requires BitwiseEnum<T>
[[nodiscard]] constexpr T operator^(T lhs, T rhs) noexcept;

/*!
  \brief Complements a flag set over the full width of the underlying type.

  \tparam T Type satisfying \ref toy::BitwiseEnum.

  \param val Operand.

  \return Value of \a T carrying every bit \a val leaves clear.

  \warning The result covers the whole underlying type, bits no enumerator names included. Mask it against the union of
           the declared flags before comparing it to one.

  \sa toy::operator&(), toy::operator^()
*/
template <typename T>
  requires BitwiseEnum<T>
[[nodiscard]] constexpr T operator~(T val) noexcept;

/*!
  \brief Adds the flags of \a rhs to \a lhs.

  \tparam T Type satisfying \ref toy::BitwiseEnum.

  \param lhs Value updated in place.
  \param rhs Flags to add.

  \return Reference to \a lhs.

  \post \a lhs carries every bit it held before the call plus every bit of \a rhs.

  \sa toy::operator|()
*/
template <typename T>
  requires BitwiseEnum<T>
constexpr T & operator|=(T & lhs, T rhs) noexcept;

/*!
  \brief Restricts \a lhs to the flags it shares with \a rhs.

  \tparam T Type satisfying \ref toy::BitwiseEnum.

  \param lhs Value updated in place.
  \param rhs Mask to apply.

  \return Reference to \a lhs.

  \post \a lhs carries the bits it held before the call that \a rhs also sets; the rest are clear.

  \sa toy::operator&()
*/
template <typename T>
  requires BitwiseEnum<T>
constexpr T & operator&=(T & lhs, T rhs) noexcept;

/*!
  \brief Toggles the flags of \a rhs in \a lhs.

  \tparam T Type satisfying \ref toy::BitwiseEnum.

  \param lhs Value updated in place.
  \param rhs Flags to toggle.

  \return Reference to \a lhs.

  \post \a lhs carries the bits set in exactly one of its previous value and \a rhs.

  \sa toy::operator^()
*/
template <typename T>
  requires BitwiseEnum<T>
constexpr T & operator^=(T & lhs, T rhs) noexcept;

} // namespace toy

/*!
  \def ENABLE_BITWISE_OPERATORS
  \brief Opts a scoped enumeration into the bitwise operators of this header.

  Expands to a full specialization of \ref toy::EnableBitwiseOperators whose \c enable is \c true, a \c static_assert
  that \ref toy::BitwiseEnum then accepts the enumeration, and using-declarations of the seven operators inside
  \a namespaceName. Argument-dependent lookup searches the namespace of the enumeration, so the operators resolve at any
  call site without a using-directive. The expansion carries its own semicolons, so the invocation needs no terminator.

  \param namespaceName Innermost namespace enclosing the enumeration, as in \c toy::gfx.
  \param ...           Enumeration named relative to \a namespaceName, through its enclosing classes for a member
                       enumeration. Taken variadically so a nested template argument list keeps the commas the
                       preprocessor would otherwise read as argument separators, as in
                       \c ENABLE_BITWISE_OPERATORS(toy, Flags<int, long>::Type).

  \pre The arguments name one complete \c enum class over an unsigned underlying type other than \c bool. Anything else
       fails the \c static_assert at the invocation, including an \c enum class without a fixed type, which takes
       \c int.
  \pre \a namespaceName is the innermost namespace enclosing the enumeration, the one argument-dependent lookup
       searches. Naming an outer namespace still compiles, but call sites then miss the operators; naming a class
       fails as a redefinition of the class as a namespace.
  \pre That namespace is named; an enumeration in an unnamed namespace cannot be opted in.
  \pre The invocation stands at global namespace scope, after the definition of the enumeration.
  \pre No other specialization of \ref toy::EnableBitwiseOperators for that enumeration precedes it.

  \note Put the invocation in the header that defines the enumeration, right below it. Combining its flags where the
        specialization is not visible either fails to compile or, across translation units that disagree, is ill-formed
        with no diagnostic required.
  \note Several enumerations of one namespace may each invoke it: namespace scope allows a repeated using-declaration.

  \warning A specialization written by hand skips this check and adds no using-declarations. The operators still
           require \ref toy::BitwiseEnum. An unscoped enumeration that fails it silently keeps the built-in operators,
           and any other fails to compile at the call site.

  \sa \ref toy::EnableBitwiseOperators, \ref toy::BitwiseEnum
*/
#define ENABLE_BITWISE_OPERATORS(namespaceName, ...)                                                                   \
  template <>                                                                                                          \
  struct toy::EnableBitwiseOperators<namespaceName::__VA_ARGS__> {                                                     \
    static constexpr bool enable = true;                                                                               \
  };                                                                                                                   \
  static_assert(toy::BitwiseEnum<namespaceName::__VA_ARGS__>,                                                          \
                "ENABLE_BITWISE_OPERATORS requires an enum class with an unsigned underlying type other than bool");   \
  namespace namespaceName {                                                                                            \
                                                                                                                       \
  using ::toy::operator|;                                                                                              \
  using ::toy::operator&;                                                                                              \
  using ::toy::operator^;                                                                                              \
  using ::toy::operator~;                                                                                              \
  using ::toy::operator|=;                                                                                             \
  using ::toy::operator&=;                                                                                             \
  using ::toy::operator^=;                                                                                             \
                                                                                                                       \
  }

#endif // INCLUDE_CORE_BITWISE_ENUM_HPP_
