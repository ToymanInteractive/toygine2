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
  \file   fixed_std_extension.hpp
  \brief  Standard library extensions for \ref toy::math::fixed: \c numeric_limits.
*/

#ifndef INCLUDE_MATH_FIXED_STD_EXTENSION_HPP_
#define INCLUDE_MATH_FIXED_STD_EXTENSION_HPP_

namespace std {

/*!
  \class numeric_limits
  \brief Specialization of \c std::numeric_limits for \c toy::math::fixed.

  \tparam Base         Storage integral type.
  \tparam Intermediate Wider type for intermediate calculations.
  \tparam Fraction     Number of fractional bits.
  \tparam Rounding     If \c true, rounds to nearest; otherwise truncates toward zero.

  \sa https://en.cppreference.com/w/cpp/types/numeric_limits
*/
template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
class numeric_limits<toy::math::fixed<Base, Intermediate, Fraction, Rounding>> {
private:
  /// Helper: ceil(bits * log10(2)) using fixed-point; log10(2) in 8.24 format is 5050445.
  static consteval int _calcMaxDigits10(int bits) noexcept {
    using T = long long;

    return static_cast<int>((T{bits} * 5050445 + (T{1} << 24) - 1) >> 24);
  }

  /// Helper: floor(bits * log10(2)) using fixed-point; log10(2) in 8.24 format is 5050445.
  static consteval int _calcDigits10(int bits) noexcept {
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

  /// \c round_to_nearest when \a Rounding is \c true; \c round_toward_zero otherwise.
  static constexpr float_round_style round_style = Rounding ? round_to_nearest : round_toward_zero;

  /// false (not IEEE 754).
  static constexpr bool is_iec559 = false;
  /// true (finite range).
  static constexpr bool is_bounded = true;
  /// Same as \c numeric_limits<Base>::is_modulo.
  static constexpr bool is_modulo = numeric_limits<Base>::is_modulo;

  /// Number of radix digits in \a Base (same as \c numeric_limits<Base>::digits).
  static constexpr int digits = numeric_limits<Base>::digits;
  /// Guaranteed significant decimal digits for the full fixed-point value.
  static constexpr int digits10 = _calcDigits10(numeric_limits<Base>::digits);
  /// Max decimal digits needed to represent any value (integer + fractional part).
  static constexpr int max_digits10
    = _calcMaxDigits10(numeric_limits<Base>::digits - Fraction) + _calcMaxDigits10(Fraction);

  /// 2 (binary radix).
  static constexpr int radix = 2;
  /// Minimum exponent in radix (1 - Fraction).
  static constexpr int min_exponent = 1 - Fraction;
  /// Minimum decimal exponent; negative, reflecting the fractional resolution.
  static constexpr int min_exponent10 = -_calcDigits10(Fraction);
  /// Maximum exponent in radix (integer part bits).
  static constexpr int max_exponent = numeric_limits<Base>::digits - Fraction;
  /// Maximum decimal exponent (integer part).
  static constexpr int max_exponent10 = _calcDigits10(numeric_limits<Base>::digits - Fraction);

  /// true (arithmetic can trap).
  static constexpr bool traps = true;
  /// false.
  static constexpr bool tinyness_before = false;

  /// Returns minimum positive value (1 LSB).
  [[nodiscard]] static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> min() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(1);
  }
  /// Returns lowest (most negative) value.
  [[nodiscard]] static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> lowest() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(numeric_limits<Base>::lowest());
  }
  /// Returns maximum finite value.
  [[nodiscard]] static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> max() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(numeric_limits<Base>::max());
  }
  /// Returns 1 LSB (smallest representable positive).
  [[nodiscard]] static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> epsilon() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(1);
  }
  /// Returns 0.5 (maximum rounding error).
  [[nodiscard]] static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> round_error() noexcept {
    if constexpr (Rounding) {
      return toy::math::fixed<Base, Intermediate, Fraction, Rounding>(1) / 2; // 0.5
    } else {
      return toy::math::fixed<Base, Intermediate, Fraction, Rounding>(1); // 1.0
    }
  }
  /// Returns min() (no denormals).
  [[nodiscard]] static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> denorm_min() noexcept {
    return min();
  }

  /// Returns zero (fixed-point has no infinity).
  [[nodiscard]] static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> infinity() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>(0);
  }
  /// Returns zero (fixed-point has no NaN).
  static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> quiet_NaN() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>(0);
  }
  /// Returns zero (fixed-point has no NaN).
  [[nodiscard]] static constexpr toy::math::fixed<Base, Intermediate, Fraction, Rounding> signaling_NaN() noexcept {
    return toy::math::fixed<Base, Intermediate, Fraction, Rounding>(0);
  }
};

} // namespace std

#endif // INCLUDE_MATH_FIXED_STD_EXTENSION_HPP_
