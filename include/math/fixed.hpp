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
  \file   fixed.hpp
  \brief  Fixed-point numeric type template.
*/

#ifndef INCLUDE_MATH_FIXED_HPP_
#define INCLUDE_MATH_FIXED_HPP_

namespace toy::math {

/*!
  \brief Compile-time constraints for \ref toy::math::fixed template parameters.
*/
template <typename Base, typename Intermediate, unsigned int Fraction>
concept ValidFixedPointTypes
  = std::integral<Base> && (Fraction > 0) && (Fraction <= std::numeric_limits<Base>::digits)
    && (sizeof(Intermediate) >= sizeof(Base)) && (std::signed_integral<Base> == std::signed_integral<Intermediate>);

/*!
  \class fixed
  \brief Fixed-point numeric type with configurable storage and fractional precision.

  Stores values as scaled integers using \a FractionBits fractional bits. Conversions from integral and floating-point
  types are explicit.

  \tparam BaseType Storage type for raw fixed-point value.
  \tparam IntermediateType Wider type used for intermediate calculations.
  \tparam FractionBits Number of fractional bits.
  \tparam EnableRounding If \c true, floating-point construction rounds to nearest value.
*/
template <typename BaseType, typename IntermediateType, unsigned int FractionBits, bool EnableRounding = true>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
class fixed {
  class raw_constructor_tag {};

public:
  /*!
    \brief Default constructor.

    Initializes value to zero.
  */
  constexpr fixed() noexcept = default;

  /*!
    \brief Constructs from an integral value.

    \param value Source integral value.
  */
  template <std::integral T>
  constexpr explicit fixed(T value) noexcept;

  /*!
    \brief Constructs from a floating-point value.

    \param value Source floating-point value.
  */
  template <std::floating_point T>
  constexpr explicit fixed(T value) noexcept;

  /*!
    \brief Converts to an integral type.

    \return Value converted to \a T.
  */
  template <std::integral T>
  constexpr explicit operator T() const noexcept;

  /*!
    \brief Converts to a floating-point type.

    \return Value converted to \a T.
  */
  template <std::floating_point T>
  constexpr explicit operator T() const noexcept;

  /*!
    \brief Returns raw fixed-point storage value.
  */
  [[nodiscard]] constexpr BaseType rawValue() const noexcept;

  /*!
    \brief Creates a fixed-point value from raw storage bits.

    \param value Raw fixed-point value.
    \return Fixed-point value with raw storage set to \a value.
  */
  [[nodiscard]] static constexpr fixed fromRawValue(BaseType value) noexcept;

private:
  constexpr fixed(BaseType val, raw_constructor_tag) noexcept;

  BaseType _value{0};

  static consteval IntermediateType _fractionMult() noexcept;
};

} // namespace toy::math

#endif // INCLUDE_MATH_FIXED_HPP_
