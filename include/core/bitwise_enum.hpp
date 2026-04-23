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
  \file   bitwise_enum.hpp
  \brief  Opt-in bitwise operators for scoped enumerations used as flag sets.

  Defines \ref toy::EnableBitwiseOperators and constrained operator overloads.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_BITWISE_ENUM_HPP_
#define INCLUDE_CORE_BITWISE_ENUM_HPP_

namespace toy {

/*!
  \class EnableBitwiseOperators
  \brief Primary template: disables generic bitwise operators for \a T unless specialized.

  Specialize (manually or with \c ENABLE_BITWISE_OPERATORS(T)) so that \c enable is \c true for a given scoped enum. The
  generic operator overloads in this header are only instantiated when `EnableBitwiseOperators<T>::enable` is true.

  \tparam T Scoped enumeration type (`enum class`).

  \section usage Usage Example

  \code
  #include "core.hpp"

  namespace toy {

  enum class MyFlags : unsigned { a = 1u, b = 2u, c = 4u };

  } // namespace toy

  ENABLE_BITWISE_OPERATORS(toy::MyFlags)

  namespace toy {

  constexpr MyFlags combined = MyFlags::a | MyFlags::b;
  constexpr MyFlags masked = combined & MyFlags::a;
  constexpr MyFlags inverted = ~MyFlags::a;

  } // namespace toy
  \endcode

  \section features Key Features

  - ⚙️ **Opt-in**: No operators are added until you specialize the trait (typically via \c ENABLE_BITWISE_OPERATORS).
  - **Constrained overloads**: Participation is explicit (`requires EnableBitwiseOperators<T>::enable`).
  - **constexpr / noexcept**: Usable in constant evaluation when operands and the underlying type allow it.
  - **No allocation**: Pure forwarding to integral bitwise operations on the underlying type.

  \section performance Performance Characteristics

  Each operation is a short inline forwarding to one or two integer operations; complexity is O(1) per call with no heap
  use.

  \section safety Safety Guarantees

  - The trait does not check that enumerators are disjoint bits; mixing overlapping values is a logic error.
  - All declared operations are noexcept.
  - For signed underlying types, unary `~` follows the usual two's-complement bit pattern for the underlying width.

  \note Prefer an unsigned underlying type for flag enums to reduce surprises with `~` and with mixing signed literals.

  \sa ENABLE_BITWISE_OPERATORS
*/
template <typename T>
class EnableBitwiseOperators {
public:
  /// When \c true, the bitwise operator overloads in this header apply to \a T. The primary template sets \c false.
  static constexpr bool enable = false;
};

/*!
  \brief Computes bitwise OR on the underlying integers, then casts back to \a T.

  \tparam T Scoped enum for which \ref toy::EnableBitwiseOperators is specialized with \c enable == \c true.

  \param lhs First operand.
  \param rhs Second operand.

  \return \c static_cast<T>(std::to_underlying(lhs) | std::to_underlying(rhs)).
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator|(T lhs, T rhs) noexcept;

/*!
  \brief Computes bitwise AND on the underlying integers, then casts back to \a T.

  \tparam T Scoped enum for which \ref toy::EnableBitwiseOperators is specialized with \c enable == \c true.

  \param lhs First operand.
  \param rhs Second operand.

  \return \c static_cast<T>(std::to_underlying(lhs) & std::to_underlying(rhs)).
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator&(T lhs, T rhs) noexcept;

/*!
  \brief Computes bitwise XOR on the underlying integers, then casts back to \a T.

  \tparam T Scoped enum for which \ref toy::EnableBitwiseOperators is specialized with \c enable == \c true.

  \param lhs First operand.
  \param rhs Second operand.

  \return \c static_cast<T>(std::to_underlying(lhs) ^ std::to_underlying(rhs)).
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator^(T lhs, T rhs) noexcept;

/*!
  \brief Computes bitwise NOT of the underlying integer, then casts back to \a T.

  \tparam T Scoped enum for which \ref toy::EnableBitwiseOperators is specialized with \c enable == \c true.

  \param lhs Operand.

  \return \c static_cast<T>(~std::to_underlying(lhs)).

  \note Inverts every bit of the underlying representation; meaning as a flag set depends on width and signedness.
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator~(T lhs) noexcept;

/*!
  \brief Compound OR: \a lhs becomes \a lhs | \a rhs.

  \tparam T Scoped enum for which \ref toy::EnableBitwiseOperators is specialized with \c enable == \c true.

  \param lhs Reference to the left-hand value (updated).
  \param rhs Right-hand operand.

  \return Reference to \a lhs.
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
constexpr T & operator|=(T & lhs, T rhs) noexcept;

/*!
  \brief Compound AND: \a lhs becomes \a lhs & \a rhs.

  \tparam T Scoped enum for which \ref toy::EnableBitwiseOperators is specialized with \c enable == \c true.

  \param lhs Reference to the left-hand value (updated).
  \param rhs Right-hand operand.

  \return Reference to \a lhs.
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
constexpr T & operator&=(T & lhs, T rhs) noexcept;

/*!
  \brief Compound XOR: \a lhs becomes \a lhs ^ \a rhs.

  \tparam T Scoped enum for which \ref toy::EnableBitwiseOperators is specialized with \c enable == \c true.

  \param lhs Reference to the left-hand value (updated).
  \param rhs Right-hand operand.

  \return Reference to \a lhs.
*/
template <typename T>
  requires EnableBitwiseOperators<T>::enable
constexpr T & operator^=(T & lhs, T rhs) noexcept;

} // namespace toy

/*!
  \def ENABLE_BITWISE_OPERATORS
  \brief Specializes \ref toy::EnableBitwiseOperators so that \c enable is \c true for the scoped enum \a T.

  The macro expands to a full class template specialization in namespace \ref toy. Place it in the global namespace
  after \a T is complete so the specialization is visible when instantiating the generic operators.

  \param T The scoped enumeration type (may be qualified with its namespace).

  \pre \a T names a complete `enum class` type intended for use as a bit-flag set.

  \see EnableBitwiseOperators
*/
#define ENABLE_BITWISE_OPERATORS(T)                                                                                    \
  template <>                                                                                                          \
  struct toy::EnableBitwiseOperators<T> {                                                                              \
    static constexpr bool enable = true;                                                                               \
  };

#endif // INCLUDE_CORE_BITWISE_ENUM_HPP_
