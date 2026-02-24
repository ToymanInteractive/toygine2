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

  \tparam Base Storage type; must satisfy \c integral.
  \tparam Intermediate Wider type for intermediate calculations; size must be \c >= sizeof(Base), signedness must match
  \a Base.
  \tparam Fraction Number of fractional bits; must be in (0, numeric_limits<Base>::digits].
*/
template <typename Base, typename Intermediate, unsigned Fraction>
concept ValidFixedPointTypes
  = integral<Base> && (Fraction > 0) && (Fraction <= numeric_limits<Base>::digits)
    && (sizeof(Intermediate) >= sizeof(Base)) && (is_signed_v<Base> == is_signed_v<Intermediate>);

/*!
  \class fixed
  \brief Fixed-point numeric type with configurable storage and fractional precision.

  Stores values as scaled integers using \a Fraction fractional bits. Conversions from integral and floating-point types
  are explicit.

  \tparam Base Storage type for raw fixed-point value.
  \tparam Intermediate Wider type used for intermediate calculations.
  \tparam Fraction Number of fractional bits.
  \tparam Rounding If \c true, floating-point construction rounds to nearest value.

  \section features Key Features

  - **Constexpr**: All operations and constructors are \c constexpr; usable at compile time.
  - **Configurable precision**: \a Base and \a Fraction choose storage size and fractional bits.
  - **No exceptions**: All operations are \c noexcept; no dynamic allocation.
  - **Type safety**: \ref toy::math::ValidFixedPointTypes concept constrains template parameters.
  - **Rounding policy**: Optional rounding when converting from floating-point or scaling down fraction bits.

  \section usage Usage Example

  \code
  #include "math.hpp"

  using Fixed = toy::math::fixed<int32_t, int64_t, 8>;
  constexpr Fixed a(2);
  constexpr Fixed b(3);
  constexpr auto sum = a + b;
  constexpr auto raw = Fixed::fromRawValue(256);
  \endcode

  \section performance Performance Characteristics

  - **Construction, conversion, arithmetic**: O(1). All operations are integer-based.
  - **Memory**: sizeof(\a Base); no heap allocation.

  \section safety Safety Guarantees

  - **Contracts**: Division by zero is checked in debug builds via \c assert_message; undefined behavior if divisor is
  zero in release.
  - **Exception safety**: All operations are \c noexcept; no exceptions thrown.
  - **Type safety**: Explicit conversions only; no implicit narrowing.

  \section compatibility Compatibility

  - **C++ standard**: C++20 (concepts, \c constexpr, \c consteval).
  - **Embedded**: No heap allocation; suitable for constrained environments.

  \sa \ref toy::math::ValidFixedPointTypes
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding = true>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
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
  template <integral T>
  constexpr explicit fixed(const T & value) noexcept;

  /*!
    \brief Constructs from a floating-point value.

    \param value Source floating-point value.
  */
  template <floating_point T>
  constexpr explicit fixed(const T & value) noexcept;

  /*!
    \brief Constructs by converting from another \ref toy::math::fixed type.

    The source may have different \a Base, \a Intermediate, fraction bits, or rounding policy. The value is converted to
    this instance's \a Fraction using fromFixedPoint; when the source has more fraction bits, rounding is applied if
    \a Rounding is \c true.

    \tparam OtherBase Storage type of the source fixed.
    \tparam OtherIntermediate Intermediate type of the source fixed.
    \tparam OtherFraction Number of fraction bits of the source fixed.
    \tparam OtherRounding Rounding policy of the source fixed.

    \param value Source fixed-point value.
  */
  template <typename OtherBase, typename OtherIntermediate, unsigned OtherFraction, bool OtherRounding>
  constexpr explicit fixed(const fixed<OtherBase, OtherIntermediate, OtherFraction, OtherRounding> & value) noexcept;

  /*!
    \brief Converts to an integral type.

    \return Value converted to \a T.
  */
  template <integral T>
  constexpr explicit operator T() const noexcept;

  /*!
    \brief Converts to a floating-point type.

    \return Value converted to \a T.
  */
  template <floating_point T>
  constexpr explicit operator T() const noexcept;

  /*!
    \brief Returns raw fixed-point storage value.

    \return Raw scaled integer (storage value).
  */
  [[nodiscard]] constexpr Base rawValue() const noexcept;

  /*!
    \brief Creates a fixed-point value from raw storage bits.

    \param value Raw fixed-point value.

    \return Fixed-point value with raw storage set to \a value.
  */
  [[nodiscard]] static constexpr fixed fromRawValue(const Base & value) noexcept;

  /*!
    \brief Builds a \ref toy::math::fixed from a raw value with a different
    number of fraction bits.

    When \a NumFractionBits is greater than \a Fraction, the value is scaled down (right-shifted). If \a Rounding is
    \c true, the result is rounded to the nearest representable value, otherwise the result is truncated.

    \tparam NumFractionBits Number of fraction bits of the source raw value.
    \tparam T Integral type of the source raw value.

    \param value Raw fixed-point value in \a NumFractionBits fractional bits.

    \return Fixed-point value with this instance's \a Fraction and \a Rounding.
  */
  template <unsigned NumFractionBits, integral T>
    requires(NumFractionBits > Fraction)
  [[nodiscard]] static constexpr fixed fromFixedPoint(const T & value) noexcept;

  /*!
    \brief Builds a \ref toy::math::fixed from a raw value with fewer or equal fraction bits.

    When \a NumFractionBits is less than \a Fraction, the value is scaled up (left-shifted); no rounding is needed.
    When \a NumFractionBits equals \a Fraction, the raw value is used as-is.

    \tparam NumFractionBits Number of fraction bits of the source raw value.
    \tparam T Integral type of the source raw value.

    \param value Raw fixed-point value in \a NumFractionBits fractional bits.

    \return Fixed-point value with this instance's \a Fraction and \a Rounding.
  */
  template <unsigned NumFractionBits, integral T>
    requires(NumFractionBits <= Fraction)
  [[nodiscard]] static constexpr fixed fromFixedPoint(const T & value) noexcept;

  /*!
    \brief Adds another \ref toy::math::fixed value in place.

    Accepts any \ref toy::math::fixed with the same \a Base, \a Intermediate, and \a Fraction, regardless of
    \a Rounding. The operation uses raw storage; rounding policy of \a other does not affect the result.

    \tparam OtherRounding \a Rounding of the right-hand side type (may differ from this instance).

    \param other Value to add.

    \return Reference to \c *this.
  */
  template <bool OtherRounding>
  constexpr fixed & operator+=(const fixed<Base, Intermediate, Fraction, OtherRounding> & other) noexcept;

  /*!
    \brief Adds an integral value in place.

    \a other is scaled by 2^\a Fraction and added to the raw storage.

    \tparam T Integral type.

    \param other Value to add (interpreted as fixed-point whole units).

    \return Reference to \c *this.
  */
  template <integral T>
  constexpr fixed & operator+=(const T & other) noexcept;

  /*!
    \brief Subtracts another \ref toy::math::fixed value in place.

    Accepts any \ref toy::math::fixed with the same \a Base, \a Intermediate, and \a Fraction, regardless of
    \a Rounding. The operation uses raw storage; rounding policy of \a other does not affect the result.

    \tparam OtherRounding \a Rounding of the right-hand side type (may differ from this instance).

    \param other Value to subtract.

    \return Reference to \c *this.
  */
  template <bool OtherRounding>
  constexpr fixed & operator-=(const fixed<Base, Intermediate, Fraction, OtherRounding> & other) noexcept;

  /*!
    \brief Subtracts an integral value in place.

    \a other is scaled by 2^\a Fraction and subtracted from the raw storage.

    \tparam T Integral type.

    \param other Value to subtract (interpreted as fixed-point whole units).

    \return Reference to \c *this.
  */
  template <integral T>
  constexpr fixed & operator-=(const T & other) noexcept;

  /*!
    \brief Multiplies by another \ref toy::math::fixed value in place.

    Accepts any \ref toy::math::fixed with the same \a Base, \a Intermediate, and \a Fraction, regardless of
    \a Rounding. Uses raw storage; when \a Rounding is \c true, the result is rounded to the nearest
    representable value.

    \tparam OtherRounding \a Rounding of the right-hand side type (may differ from this instance).

    \param other Value to multiply by.

    \return Reference to \c *this.
  */
  template <bool OtherRounding>
  constexpr fixed & operator*=(const fixed<Base, Intermediate, Fraction, OtherRounding> & other) noexcept;

  /*!
    \brief Multiplies by an integral value in place.

    \a other scales the fixed-point value (whole units). Raw storage is multiplied by \a other directly.

    \tparam T Integral type.

    \param other Multiplier (fixed-point whole units).

    \return Reference to \c *this.
  */
  template <integral T>
  constexpr fixed & operator*=(const T & other) noexcept;

  /*!
    \brief Divides by another \ref toy::math::fixed value in place.

    Accepts any \ref toy::math::fixed with the same \a Base, \a Intermediate, and \a Fraction, regardless of
    \a Rounding. Uses raw storage; rounding policy of \a other does not affect the result. Behavior is undefined
    if \a other is zero.

    \tparam OtherRounding \a Rounding of the right-hand side type (may differ from this instance).

    \param other Divisor (must not be zero).

    \return Reference to \c *this.
  */
  template <bool OtherRounding>
  constexpr fixed & operator/=(const fixed<Base, Intermediate, Fraction, OtherRounding> & other) noexcept;

  /*!
    \brief Divides by an integral value in place.

    \a other is the divisor (fixed-point whole units). Behavior is undefined if \a other is zero.

    \tparam T Integral type.

    \param other Divisor (must not be zero).

    \return Reference to \c *this.
  */
  template <integral T>
  constexpr fixed & operator/=(const T & other) noexcept;

private:
  /*!
    \brief Constructs directly from raw storage value.

    \param val Raw fixed-point storage value.
  */
  constexpr fixed(const Base & val, RawConstructorTag) noexcept;

  /// Raw fixed-point storage value (scaled by 2^Fraction).
  Base _value{0};

  /// Compile-time scaling factor 2^Fraction.
  static consteval Intermediate _fractionMult() noexcept;
};

/*!
  \struct is_fixed_point
  \brief Unary type trait: \c true if \a T is an instantiation of \ref toy::math::fixed, \c false otherwise.

  \tparam T Type to check.
*/
template <typename T>
struct is_fixed_point : false_type {};

/*!
  \struct is_fixed_point
  \brief Specialization for \ref toy::math::fixed: always \c true_type.

  \tparam Base Storage type for raw fixed-point value.
  \tparam Intermediate Intermediate type for calculations.
  \tparam Fraction Number of fractional bits.
  \tparam Rounding Rounding policy.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
struct is_fixed_point<fixed<Base, Intermediate, Fraction, Rounding>> : true_type {};

/// Helper variable template: \c true if \a T is \ref toy::math::fixed, \c false otherwise.
template <typename T>
inline constexpr bool is_fixed_point_v = is_fixed_point<T>::value;

/*!
  \brief Concept: \a T is an instantiation of \ref toy::math::fixed.

  Use to constrain templates to fixed-point types only (analogous to \c floating_point for built-in floats).

  \tparam T Type to check.
*/
template <typename T>
concept fixed_point = is_fixed_point_v<T>;

/*!
  \brief Unary minus: returns the negation of a \ref toy::math::fixed value.

  The result has the same raw storage as \a value with sign flipped. The operand is not modified.

  \param value Fixed-point value to negate.

  \return A new \ref toy::math::fixed instance representing \c -value.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires signed_integral<Base>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator-(
  const fixed<Base, Intermediate, Fraction, Rounding> & value) noexcept;

/*!
  \brief Returns \a a + \a b as a new \ref toy::math::fixed (same type as left operand).

  \a b may be a \ref toy::math::fixed with different rounding (\a OtherRounding) or an integral;
  the result uses \a Rounding of the \ref toy::math::fixed operand.

  \param a Left operand (fixed-point value).
  \param b Right operand (fixed-point or integral whole units).

  \return Sum as \ref toy::math::fixed with same type as \a a.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator+(
  const fixed<Base, Intermediate, Fraction, Rounding> & a,
  const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept;

/*!
  \brief Returns \a a + \a b as a new \ref toy::math::fixed (fixed plus integral whole units).

  \param a Fixed-point value.
  \param b Integral value (whole units).

  \return Sum as \ref toy::math::fixed.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator+(
  const fixed<Base, Intermediate, Fraction, Rounding> & a, const T & b) noexcept;

/*!
  \brief Returns \a a + \a b as a new \ref toy::math::fixed (integral whole units plus fixed).

  \param a Integral value (whole units).
  \param b Fixed-point value.

  \return Sum as \ref toy::math::fixed.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator+(
  const T & a, const fixed<Base, Intermediate, Fraction, Rounding> & b) noexcept;

/*!
  \brief Returns \a a - \a b as a new \ref toy::math::fixed (same type as left operand).

  \param a Left operand (fixed-point value).
  \param b Right operand (fixed-point value).

  \return Difference as \ref toy::math::fixed with same type as \a a.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator-(
  const fixed<Base, Intermediate, Fraction, Rounding> & a,
  const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept;

/*!
  \brief Returns \a a - \a b as a new \ref toy::math::fixed (fixed minus integral whole units).

  \param a Fixed-point value.
  \param b Integral value (whole units).

  \return Difference as \ref toy::math::fixed.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator-(
  const fixed<Base, Intermediate, Fraction, Rounding> & a, const T & b) noexcept;

/*!
  \brief Returns \a a - \a b as a new \ref toy::math::fixed (integral whole units minus fixed).

  \param a Integral value (whole units).
  \param b Fixed-point value.

  \return Difference as \ref toy::math::fixed.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator-(
  const T & a, const fixed<Base, Intermediate, Fraction, Rounding> & b) noexcept;

/*!
  \brief Returns \a a * \a b as a new \ref toy::math::fixed (same type as left \ref toy::math::fixed operand).

  \param a Left operand (fixed-point value).
  \param b Right operand (fixed-point value).

  \return Product as \ref toy::math::fixed with same type as \a a.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator*(
  const fixed<Base, Intermediate, Fraction, Rounding> & a,
  const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept;

/*!
  \brief Returns \a a * \a b as a new \ref toy::math::fixed (fixed times integral whole units).

  \param a Fixed-point value.
  \param b Integral value (whole units).

  \return Product as \ref toy::math::fixed.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator*(
  const fixed<Base, Intermediate, Fraction, Rounding> & a, const T & b) noexcept;

/*!
  \brief Returns \a a * \a b as a new \ref toy::math::fixed (integral whole units times fixed).

  \param a Integral value (whole units).
  \param b Fixed-point value.

  \return Product as \ref toy::math::fixed.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator*(
  const T & a, const fixed<Base, Intermediate, Fraction, Rounding> & b) noexcept;

/*!
  \brief Returns \a a / \a b as a new \ref toy::math::fixed.

  Behavior is undefined if \a b is zero.

  \param a Dividend (fixed-point value).
  \param b Divisor (fixed-point value, must not be zero).

  \return Quotient as \ref toy::math::fixed.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator/(
  const fixed<Base, Intermediate, Fraction, Rounding> & a,
  const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept;

/*!
  \brief Returns \a a / \a b as a new \ref toy::math::fixed (fixed divided by integral whole units).

  Behavior is undefined if \a b is zero.

  \param a Dividend (fixed-point value).
  \param b Divisor (integral, must not be zero).

  \return Quotient as \ref toy::math::fixed.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator/(
  const fixed<Base, Intermediate, Fraction, Rounding> & a, const T & b) noexcept;

/*!
  \brief Returns \a a / \a b as a new \ref toy::math::fixed (integral whole units divided by fixed).

  Behavior is undefined if \a b is zero.

  \param a Dividend (integral value).
  \param b Divisor (fixed-point value, must not be zero).

  \return Quotient as \ref toy::math::fixed.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
[[nodiscard]] constexpr fixed<Base, Intermediate, Fraction, Rounding> operator/(
  const T & a, const fixed<Base, Intermediate, Fraction, Rounding> & b) noexcept;

/*!
  \brief Compares two \ref toy::math::fixed values for equality.

  Compares raw storage values; \a Rounding of either operand does not affect the result. Two values are equal if and
  only if their raw values are equal.

  \tparam Base Storage type (shared by both operands).
  \tparam Intermediate Intermediate type (shared by both operands).
  \tparam Fraction Number of fractional bits (shared by both operands).
  \tparam Rounding Rounding policy of the left operand.
  \tparam OtherRounding Rounding policy of the right operand.

  \param a Left operand.
  \param b Right operand.

  \return \c true if \a a and \a b represent the same value, \c false otherwise.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
[[nodiscard]] constexpr bool operator==(const fixed<Base, Intermediate, Fraction, Rounding> & a,
                                        const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept;

/*!
  \brief Three-way comparison of two \ref toy::math::fixed values.

  Compares raw storage values; \a Rounding of either operand does not affect the result. Enables \c <, \c <=, \c >,
  \c >=, and \c != via the compiler-generated operators.

  \tparam Base Storage type (shared by both operands).
  \tparam Intermediate Intermediate type (shared by both operands).
  \tparam Fraction Number of fractional bits (shared by both operands).
  \tparam Rounding Rounding policy of the left operand.
  \tparam OtherRounding Rounding policy of the right operand.

  \param a Left operand.
  \param b Right operand.

  \return \c strong_ordering::less if \a a is less than \a b, \c strong_ordering::equal if equal,
          \c strong_ordering::greater if \a a is greater than \a b.
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
[[nodiscard]] constexpr strong_ordering operator<=>(
  const fixed<Base, Intermediate, Fraction, Rounding> & a,
  const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept;

} // namespace toy::math

namespace std {

/*!
  \brief Specialization of \c std::numeric_limits for \c toy::math::fixed.

  \see https://en.cppreference.com/w/cpp/types/numeric_limits
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
class numeric_limits<toy::math::fixed<Base, Intermediate, Fraction, Rounding>> {
private:
  /// Helper: ceil(bits * log10(2)) using fixed-point; log10(2) in 8.24 format is 5050445.
  static consteval int _calcMaxDigits10(int bits) {
    using T = long long;

    return static_cast<int>((T{bits} * 5050445 + (T{1} << 24) - 1) >> 24);
  }

  /// Helper: floor(bits * log10(2)) using fixed-point; log10(2) in 8.24 format is 5050445.
  static consteval int _calcDigits10(int bits) {
    using T = long long;

    return static_cast<int>((T{bits} * 5050445) >> 24);
  }

public:
  /// true (this specialization is provided).
  static constexpr bool is_specialized = true;
  /// Same as \c numeric_limits<Base>::is_signed.
  static constexpr bool is_signed = numeric_limits<Base>::is_signed;
  /// false (fixed-point is not an integer type).
  static constexpr bool is_integer = false;
  /// true (no rounding error for representable values).
  static constexpr bool is_exact = true;

  /// false (fixed-point has no infinity).
  static constexpr bool has_infinity = false;
  /// false (fixed-point has no NaN).
  static constexpr bool has_quiet_NaN = false;
  /// false (fixed-point has no NaN).
  static constexpr bool has_signaling_NaN = false;
  /// false (no denormalization).
  static constexpr bool has_denorm = false;
  /// false.
  static constexpr bool has_denorm_loss = false;

  /// \c round_to_nearest (when Rounding is true).
  static constexpr float_round_style round_style = round_to_nearest;

  /// false (not IEEE 754).
  static constexpr bool is_iec559 = false;
  /// true (finite range).
  static constexpr bool is_bounded = true;
  /// Same as \c numeric_limits<Base>::is_modulo.
  static constexpr bool is_modulo = numeric_limits<Base>::is_modulo;

  /// Number of radix digits in \a Base (same as \c numeric_limits<Base>::digits).
  static constexpr int digits = numeric_limits<Base>::digits;
  /// Guaranteed decimal digits (at least 1).
  static constexpr int digits10 = 1;
  /// Max decimal digits needed to represent any value (integer + fractional part).
  static constexpr int max_digits10
    = _calcMaxDigits10(numeric_limits<Base>::digits - Fraction) + _calcMaxDigits10(Fraction);

  /// 2 (binary radix).
  static constexpr int radix = 2;
  /// Minimum exponent in radix (1 - Fraction).
  static constexpr int min_exponent = 1 - Fraction;
  /// Minimum decimal exponent (fractional part).
  static constexpr int min_exponent10 = _calcDigits10(Fraction);
  /// Maximum exponent in radix (integer part bits).
  static constexpr int max_exponent = numeric_limits<Base>::digits - Fraction;
  /// Maximum decimal exponent (integer part).
  static constexpr int max_exponent10 = _calcDigits10(numeric_limits<Base>::digits - Fraction);

  /// true (arithmetic can trap).
  static constexpr bool traps = true;
  /// false.
  static constexpr bool tinyness_before = false;

  /// Returns minimum positive value (1 LSB).
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> min() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(1);
  }
  /// Returns lowest (most negative) value.
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> lowest() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(numeric_limits<Base>::lowest());
  }
  /// Returns maximum finite value.
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> max() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(numeric_limits<Base>::max());
  }
  /// Returns 1 LSB (smallest representable positive).
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> epsilon() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(1);
  }
  /// Returns 0.5 (maximum rounding error).
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> round_error() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>(1) / 2;
  }
  /// Returns min() (no denormals).
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> denorm_min() noexcept {
    return min();
  }

  /// Returns zero (fixed-point has no infinity).
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> infinity() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>(0);
  }
  /// Returns zero (fixed-point has no NaN).
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> quiet_NaN() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>(0);
  }
  /// Returns zero (fixed-point has no NaN).
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> signaling_NaN() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>(0);
  }
};

} // namespace std

#endif // INCLUDE_MATH_FIXED_HPP_
