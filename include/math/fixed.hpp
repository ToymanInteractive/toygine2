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
template <typename Base, typename Intermediate, unsigned Fraction>
concept ValidFixedPointTypes
  = std::integral<Base> && (Fraction > 0) && (Fraction <= std::numeric_limits<Base>::digits)
    && (sizeof(Intermediate) >= sizeof(Base)) && (std::is_signed_v<Base> == std::is_signed_v<Intermediate>);

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
template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding = true>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
class fixed {
  /// Internal tag for constructing from raw storage.
  class RawConstructorTag {};

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
    \brief Constructs by converting from another \ref toy::math::fixed type.

    The source may have different \a BaseType, \a IntermediateType, fraction bits, or rounding policy. The value is
    converted to this instance's \a FractionBits using fromFixedPoint; when the source has more fraction bits, rounding
    is applied if \a EnableRounding is \c true.

    \tparam B Storage type of the source fixed.
    \tparam I Intermediate type of the source fixed.
    \tparam F Number of fraction bits of the source fixed.
    \tparam R Rounding policy of the source fixed.

    \param value Source fixed-point value.
  */
  template <typename B, typename I, unsigned F, bool R>
  constexpr explicit fixed(fixed<B, I, F, R> value) noexcept;

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

  /*!
    \brief Builds a \ref toy::math::fixed from a raw value with a different number of fraction bits.

    When \a NumFractionBits is greater than \a FractionBits, the value is scaled down; if \a EnableRounding is \c true,
    the result is rounded to the nearest representable value. When \a NumFractionBits is less than or equal to \a
    FractionBits, the value is scaled up (no rounding).

    \tparam NumFractionBits Number of fraction bits of the source raw value.
    \tparam T Integral type of the source raw value.

    \param value Raw fixed-point value in \a NumFractionBits fractional bits.

    \return Fixed-point value with this instance's \a FractionBits and \a EnableRounding.
  */
  template <unsigned NumFractionBits, typename T>
    requires(NumFractionBits > FractionBits)
  [[nodiscard]] static constexpr fixed fromFixedPoint(T value) noexcept;

  /*!
    \brief Builds a \ref toy::math::fixed from a raw value with a different number of fraction bits.

    When \a NumFractionBits less or equal than \a FractionBits, the value is scaled down; if \a EnableRounding is
    \c true, the result is rounded to the nearest representable value. When \a NumFractionBits is less than or equal to
    \a FractionBits, the value is scaled up (no rounding).

    \tparam NumFractionBits Number of fraction bits of the source raw value.
    \tparam T Integral type of the source raw value.

    \param value Raw fixed-point value in \a NumFractionBits fractional bits.

    \return Fixed-point value with this instance's \a FractionBits and \a EnableRounding.
  */
  template <unsigned NumFractionBits, typename T>
    requires(NumFractionBits <= FractionBits)
  [[nodiscard]] static constexpr fixed fromFixedPoint(T value) noexcept;

  /*!
    \brief Adds another \ref toy::math::fixed value in place.

    Accepts any \ref toy::math::fixed with the same \a BaseType, \a IntermediateType, and \a FractionBits, regardless of
    \a EnableRounding. The operation uses raw storage; rounding policy of \a other does not affect the result.

    \tparam OtherRounding \a EnableRounding of the right-hand side type (may differ from this instance).

    \param other Value to add.

    \return Reference to \c *this.
  */
  template <bool OtherRounding>
  constexpr fixed & operator+=(const fixed<BaseType, IntermediateType, FractionBits, OtherRounding> & other) noexcept;

  /*!
    \brief Adds an integral value in place.

    \a other is scaled by 2^\a FractionBits and added to the raw storage.

    \tparam T Integral type.

    \param other Value to add (interpreted as fixed-point whole units).

    \return Reference to \c *this.
  */
  template <std::integral T>
  constexpr fixed & operator+=(T other) noexcept;

  /*!
    \brief Subtracts another \ref toy::math::fixed value in place.

    Accepts any \ref toy::math::fixed with the same \a BaseType, \a IntermediateType, and \a FractionBits, regardless of
    \a EnableRounding. The operation uses raw storage; rounding policy of \a other does not affect the result.

    \tparam OtherRounding \a EnableRounding of the right-hand side type (may differ from this instance).

    \param other Value to subtract.

    \return Reference to \c *this.
  */
  template <bool OtherRounding>
  constexpr fixed & operator-=(const fixed<BaseType, IntermediateType, FractionBits, OtherRounding> & other) noexcept;

  /*!
    \brief Subtracts an integral value in place.

    \a other is scaled by 2^\a FractionBits and subtracted from the raw storage.

    \tparam T Integral type.

    \param other Value to subtract (interpreted as fixed-point whole units).

    \return Reference to \c *this.
  */
  template <std::integral T>
  constexpr fixed & operator-=(T other) noexcept;

  /*!
    \brief Multiplies by another \ref toy::math::fixed value in place.

    Accepts any \ref toy::math::fixed with the same \a BaseType, \a IntermediateType, and \a FractionBits, regardless of
    \a EnableRounding. Uses raw storage; when \a EnableRounding is \c true, the result is rounded to the nearest
    representable value.

    \tparam OtherRounding \a EnableRounding of the right-hand side type (may differ from this instance).

    \param other Value to multiply by.

    \return Reference to \c *this.
  */
  template <bool OtherRounding>
  constexpr fixed & operator*=(const fixed<BaseType, IntermediateType, FractionBits, OtherRounding> & other) noexcept;

  /*!
    \brief Multiplies by an integral value in place.

    \a other scales the fixed-point value (whole units). Raw storage is multiplied by \a other directly.

    \tparam T Integral type.

    \param other Multiplier (fixed-point whole units).

    \return Reference to \c *this.
  */
  template <std::integral T>
  constexpr fixed & operator*=(T other) noexcept;

  /*!
    \brief Divides by another \ref toy::math::fixed value in place.

    Accepts any \ref toy::math::fixed with the same \a BaseType, \a IntermediateType, and \a FractionBits, regardless of
    \a EnableRounding. Uses raw storage; rounding policy of \a other does not affect the result. Behavior is undefined
    if \a other is zero.

    \tparam OtherRounding \a EnableRounding of the right-hand side type (may differ from this instance).

    \param other Divisor (must not be zero).

    \return Reference to \c *this.
  */
  template <bool OtherRounding>
  constexpr fixed & operator/=(const fixed<BaseType, IntermediateType, FractionBits, OtherRounding> & other) noexcept;

  /*!
    \brief Divides by an integral value in place.

    \a other is the divisor (fixed-point whole units). Behavior is undefined if \a other is zero.

    \tparam T Integral type.

    \param other Divisor (must not be zero).

    \return Reference to \c *this.
  */
  template <std::integral T>
  constexpr fixed & operator/=(T other) noexcept;

private:
  /*!
    \brief Constructs directly from raw storage value.

    \param val Raw fixed-point storage value.
  */
  constexpr fixed(BaseType val, RawConstructorTag) noexcept;

  /// Raw fixed-point storage value (scaled by 2^FractionBits).
  BaseType _value{0};

  /// Compile-time scaling factor 2^FractionBits.
  static consteval IntermediateType _fractionMult() noexcept;
};

/*!
  \brief Unary minus: returns the negation of a \ref toy::math::fixed value.

  The result has the same raw storage as \a value with sign flipped. The operand is not modified.

  \param value Fixed-point value to negate.

  \return A new \ref toy::math::fixed instance representing \c -value.
*/
template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits> && std::signed_integral<BaseType>
[[nodiscard]] constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> operator-(
  const fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & value) noexcept;

} // namespace toy::math

#endif // INCLUDE_MATH_FIXED_HPP_
