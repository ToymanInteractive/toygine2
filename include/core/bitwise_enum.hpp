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
  \brief  Trait and generic bitwise operators for scoped enums.

  \ref toy::enable_bitwise_operators is a trait that gates bitwise operator overloads for an enum type.
  Use macro ENABLE_BITWISE_OPERATORS to enable operator|, operator&, operator^, operator~ and compound
  operator|=, operator&=, operator^= for a given enum. All operations are constexpr and noexcept.

  \sa toy::enable_bitwise_operators, ENABLE_BITWISE_OPERATORS
*/

#ifndef INCLUDE_CORE_BITWISE_ENUM_HPP_
#define INCLUDE_CORE_BITWISE_ENUM_HPP_

namespace toy {

/*!
  \brief Trait that enables bitwise operator overloads for an enum type \a T.

  Default specialization has \a enable equal to \c false. Specialize via ENABLE_BITWISE_OPERATORS(T) to set \a enable to
  \c true and enable operator|, operator&, operator^, operator~ and compound forms for \a T.

  \tparam T Scoped enum type.

  \sa ENABLE_BITWISE_OPERATORS
*/
template <typename T>
struct enable_bitwise_operators {
  /// Enables bitwise operators.
  static constexpr bool enable = false;
};

/*!
  \brief Bitwise OR of two enum values.

  \param lhs First operand.
  \param rhs Second operand.

  \return A value of type \a T with bits set in either \a lhs or \a rhs.
*/
template <typename T>
  requires enable_bitwise_operators<T>::enable
constexpr T operator|(T lhs, T rhs) noexcept {
  return static_cast<T>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

/*!
  \brief Bitwise AND of two enum values.

  \param lhs First operand.
  \param rhs Second operand.

  \return A value of type \a T with only the bits set in both \a lhs and \a rhs.
*/
template <typename T>
  requires enable_bitwise_operators<T>::enable
constexpr T operator&(T lhs, T rhs) noexcept {
  return static_cast<T>(std::to_underlying(lhs) & std::to_underlying(rhs));
}

/*!
  \brief Bitwise XOR of two enum values.

  \param lhs First operand.
  \param rhs Second operand.

  \return A value of type \a T with bits set in exactly one of \a lhs or \a rhs.
*/
template <typename T>
  requires enable_bitwise_operators<T>::enable
constexpr T operator^(T lhs, T rhs) noexcept {
  return static_cast<T>(std::to_underlying(lhs) ^ std::to_underlying(rhs));
}

/*!
  \brief Bitwise NOT (one's complement) of an enum value.

  \param lhs Operand.

  \return A value of type \a T with all bits of the underlying value inverted.
*/
template <typename T>
  requires enable_bitwise_operators<T>::enable
constexpr T operator~(T lhs) noexcept {
  return static_cast<T>(~std::to_underlying(lhs));
}

/*!
  \brief Compound bitwise OR assignment.

  \param lhs Left operand (modified).
  \param rhs Right operand.

  \return Reference to \a lhs.
*/
template <typename T>
  requires enable_bitwise_operators<T>::enable
constexpr T & operator|=(T & lhs, T rhs) noexcept {
  return lhs = lhs | rhs;
}

/*!
  \brief Compound bitwise AND assignment.

  \param lhs Left operand (modified).
  \param rhs Right operand.

  \return Reference to \a lhs.
*/
template <typename T>
  requires enable_bitwise_operators<T>::enable
constexpr T & operator&=(T & lhs, T rhs) noexcept {
  return lhs = lhs & rhs;
}

/*!
  \brief Compound bitwise XOR assignment.

  \param lhs Left operand (modified).
  \param rhs Right operand.

  \return Reference to \a lhs.
*/
template <typename T>
  requires enable_bitwise_operators<T>::enable
constexpr T & operator^=(T & lhs, T rhs) noexcept {
  return lhs = lhs ^ rhs;
}

} // namespace toy

/*!
  \def ENABLE_BITWISE_OPERATORS
  \brief Enables bitwise operators for the scoped enum type \a T.

  Expands to a full specialization of \ref toy::enable_bitwise_operators with \a enable set to \c true. Must be used in
  the global namespace after the enum definition. Include \c core/bitwise_enum.hpp so that the trait and operators are
  visible.

  \param T Scoped enum type (e.g. \ref toy::render::ClearFlags).

  \sa toy::enable_bitwise_operators
*/
#define ENABLE_BITWISE_OPERATORS(T)                                                                                    \
  template <>                                                                                                          \
  struct toy::enable_bitwise_operators<T> {                                                                            \
    static constexpr bool enable = true;                                                                               \
  };

#endif // INCLUDE_CORE_BITWISE_ENUM_HPP_
