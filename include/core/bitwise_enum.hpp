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

  Defines \ref toy::EnableBitwiseOperators, the trait admitting one enumeration to the operator overloads declared
  here, together with those overloads and the \c ENABLE_BITWISE_OPERATORS macro that writes the specialization. Used
  where a flag set is spelled as an \c enum class and callers combine, mask, and test its enumerators without casting
  to the underlying integer.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_BITWISE_ENUM_HPP_
#define INCLUDE_CORE_BITWISE_ENUM_HPP_

namespace toy {

/*!
  \class EnableBitwiseOperators
  \brief Marks \a T as a bit-flag enumeration the operators in this header accept; denies every type by default.

  The primary template leaves \c enable at \c false, so no operator applies to an arbitrary type. A full
  specialization setting it to \c true, written by hand or through \c ENABLE_BITWISE_OPERATORS, opts one enumeration
  in. Each overload constrains on that value, so an enumeration that never opted in reports an unmet requirement
  instead of silently converting.

  \tparam T Scoped enumeration whose enumerators name single bits. The primary template accepts any type; only a
            specialization grants operators.

  \section features Key Features

  * **Opt-in per type**: One specialization enables the whole operator set for one enumeration.
  * **Constexpr support**: Every operator evaluates in a constant expression.
  * **Type safety**: Operands and results keep type \a T; the underlying integer never reaches the call site.
  * **Exception safety**: Every operator is \c noexcept.
  * **No allocation**: Each operator forwards to one integer operation on the underlying type.

  \section usage Usage Example

  \code
  #include "core.hpp"

  namespace toy {

  enum class MyFlags : unsigned { none = 0u, a = 1u, b = 2u, c = 4u };

  } // namespace toy

  ENABLE_BITWISE_OPERATORS(toy::MyFlags)

  constexpr toy::MyFlags combined = toy::MyFlags::a | toy::MyFlags::b;
  constexpr bool carriesA = (combined & toy::MyFlags::a) != toy::MyFlags::none;
  \endcode

  \section performance Performance Characteristics

  * **Trait**: resolved during compilation; it declares no non-static member and adds nothing to the size of \a T.
  * **Each operator**: O(1), one integer operation after inlining.

  \section safety Safety Guarantees

  * **Contracts**: none checked. Whether the enumerators of \a T occupy disjoint bits is beyond the reach of the
    trait, and overlapping values stay a logic error no diagnostic reports.
  * **Type safety**: participation is a \c requires clause, so an enumeration without a specialization fails to
    compile at the call site.
  * **Value range**: results carry the bit pattern the operation produces, which for \a T with a fixed underlying type
    is well defined even where no enumerator names it.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section compatibility Compatibility

  * Requires C++23 for \c std::to_underlying and the constraint on each overload.
  * Header-only and free of allocation, platform calls, and standard-library containers, so it holds on embedded and
    retro targets.

  \note Give a flag enumeration an unsigned underlying type. Over a signed one, \c ~ yields a negative value whose
  meaning as a flag set depends on the width of the type.

  \warning The operators live in namespace \ref toy, so argument-dependent lookup finds them only for an enumeration
  declared directly in \ref toy. One declared elsewhere, a nested namespace such as \c toy::gfx included, needs
  \c using \c namespace \c toy; or a using-declaration per operator in scope at the call site.

  \sa toy::operator|(), ENABLE_BITWISE_OPERATORS
*/
template <typename T>
class EnableBitwiseOperators {
public:
  /// \c true where the operators of this header accept \a T; the primary template leaves it \c false.
  static constexpr bool enable = false;
};

/*!
  \brief Unites two flag sets.

  \tparam T Scoped enumeration whose \ref toy::EnableBitwiseOperators specialization sets \c enable to \c true.

  \param lhs Left operand.
  \param rhs Right operand.

  \return Value of \a T carrying every bit set in \a lhs, in \a rhs, or in both.

  \sa toy::operator|=(), toy::operator&()
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator|(T lhs, T rhs) noexcept;

/*!
  \brief Intersects two flag sets, the operation a membership test masks with.

  \tparam T Scoped enumeration whose \ref toy::EnableBitwiseOperators specialization sets \c enable to \c true.

  \param lhs Left operand.
  \param rhs Right operand.

  \return Value of \a T carrying the bits set in \a lhs and in \a rhs alike.

  \note An empty intersection equals the enumerator naming zero, which is what a test compares against; \a T has to
  declare one.

  \sa toy::operator&=(), toy::operator|()
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator&(T lhs, T rhs) noexcept;

/*!
  \brief Toggles the flags of \a rhs in \a lhs.

  \tparam T Scoped enumeration whose \ref toy::EnableBitwiseOperators specialization sets \c enable to \c true.

  \param lhs Left operand.
  \param rhs Right operand.

  \return Value of \a T carrying the bits set in exactly one of \a lhs and \a rhs.

  \sa toy::operator^=(), toy::operator|()
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator^(T lhs, T rhs) noexcept;

/*!
  \brief Complements a flag set over the full width of the underlying type.

  \tparam T Scoped enumeration whose \ref toy::EnableBitwiseOperators specialization sets \c enable to \c true.

  \param lhs Operand.

  \return Value of \a T carrying every bit \a lhs leaves clear.

  \warning The result covers the whole underlying type, bits no enumerator names included. Mask it against the union
  of the declared flags before comparing it to one.

  \sa toy::operator&(), toy::operator^()
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator~(T lhs) noexcept;

/*!
  \brief Adds the flags of \a rhs to \a lhs.

  \tparam T Scoped enumeration whose \ref toy::EnableBitwiseOperators specialization sets \c enable to \c true.

  \param lhs Value updated in place.
  \param rhs Flags to add.

  \return Reference to \a lhs.

  \post \a lhs carries every bit it held before the call plus every bit of \a rhs.

  \sa toy::operator|()
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
constexpr T & operator|=(T & lhs, T rhs) noexcept;

/*!
  \brief Restricts \a lhs to the flags it shares with \a rhs.

  \tparam T Scoped enumeration whose \ref toy::EnableBitwiseOperators specialization sets \c enable to \c true.

  \param lhs Value updated in place.
  \param rhs Mask to apply.

  \return Reference to \a lhs.

  \post \a lhs carries the bits it held before the call that \a rhs also sets; the rest are clear.

  \sa toy::operator&()
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
constexpr T & operator&=(T & lhs, T rhs) noexcept;

/*!
  \brief Toggles the flags of \a rhs in \a lhs.

  \tparam T Scoped enumeration whose \ref toy::EnableBitwiseOperators specialization sets \c enable to \c true.

  \param lhs Value updated in place.
  \param rhs Flags to toggle.

  \return Reference to \a lhs.

  \post \a lhs carries the bits set in exactly one of its previous value and \a rhs.

  \sa toy::operator^()
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
constexpr T & operator^=(T & lhs, T rhs) noexcept;

} // namespace toy

/*!
  \def ENABLE_BITWISE_OPERATORS
  \brief Opts the scoped enumeration \a T into the bitwise operators of this header.

  Expands to a full specialization of \ref toy::EnableBitwiseOperators, qualified with namespace \ref toy, whose
  \c enable is \c true. The expansion carries its own semicolon, so the invocation needs no terminator.

  \param T Scoped enumeration to opt in, spelled with its namespace qualification.

  \pre \a T names a complete \c enum class.
  \pre The invocation stands at global namespace scope, outside \ref toy, after the definition of \a T.
  \pre No other specialization of \ref toy::EnableBitwiseOperators for \a T precedes it.

  \note Put the invocation in the header that defines \a T, right below it. Combining flags of \a T where the
  specialization is not visible either fails to compile or, across translation units that disagree, is ill-formed
  with no diagnostic required.

  \sa \ref toy::EnableBitwiseOperators
*/
#define ENABLE_BITWISE_OPERATORS(T)                                                                                    \
  template <>                                                                                                          \
  struct toy::EnableBitwiseOperators<T> {                                                                              \
    static constexpr bool enable = true;                                                                               \
  };

#endif // INCLUDE_CORE_BITWISE_ENUM_HPP_
