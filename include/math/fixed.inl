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
  \file   fixed.inl
  \brief  Inline implementations for \ref toy::math::fixed.
*/

#ifndef INCLUDE_MATH_FIXED_INL_
#define INCLUDE_MATH_FIXED_INL_

namespace toy::math {

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <integral T>
constexpr fixed<Base, Intermediate, Fraction, Rounding>::fixed(const T & value) noexcept
  : _value(static_cast<Base>(value * _fractionMult())) {}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <floating_point T>
constexpr fixed<Base, Intermediate, Fraction, Rounding>::fixed(const T & value) noexcept
  : _value(static_cast<Base>(Rounding ? (value * static_cast<T>(_fractionMult())
                                         + ((value >= T{0}) ? static_cast<T>(0.5) : static_cast<T>(-0.5)))
                                      : (value * static_cast<T>(_fractionMult())))) {}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <typename OtherBase, typename OtherIntermediate, unsigned OtherFraction, bool OtherRounding>
constexpr fixed<Base, Intermediate, Fraction, Rounding>::fixed(
  const fixed<OtherBase, OtherIntermediate, OtherFraction, OtherRounding> & val) noexcept
  : _value(fromFixedPoint<OtherFraction>(val.rawValue()).rawValue()) {}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <integral T>
constexpr fixed<Base, Intermediate, Fraction, Rounding>::operator T() const noexcept {
  return static_cast<T>(_value / _fractionMult());
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <floating_point T>
constexpr fixed<Base, Intermediate, Fraction, Rounding>::operator T() const noexcept {
  return static_cast<T>(_value) / _fractionMult();
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
constexpr Base fixed<Base, Intermediate, Fraction, Rounding>::rawValue() const noexcept {
  return _value;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
constexpr fixed<Base, Intermediate, Fraction, Rounding> fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(
  const Base & value) noexcept {
  return fixed(value, RawConstructorTag{});
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <unsigned NumFractionBits, integral T>
  requires(NumFractionBits > Fraction)
constexpr fixed<Base, Intermediate, Fraction, Rounding> fixed<Base, Intermediate, Fraction, Rounding>::fromFixedPoint(
  const T & value) noexcept {
  Base rawValue;

  if constexpr (Rounding) {
    // To correctly round the last bit, add the LSB of the double-scale division.
    const auto halfScale = value / (T(1) << (NumFractionBits - Fraction - 1));
    rawValue = static_cast<Base>((value / (T(1) << (NumFractionBits - Fraction))) + (halfScale % 2));
  } else {
    rawValue = static_cast<Base>(value / (T(1) << (NumFractionBits - Fraction)));
  }

  return fixed(rawValue, RawConstructorTag{});
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <unsigned NumFractionBits, integral T>
  requires(NumFractionBits <= Fraction)
constexpr fixed<Base, Intermediate, Fraction, Rounding> fixed<Base, Intermediate, Fraction, Rounding>::fromFixedPoint(
  const T & value) noexcept {
  return fixed(static_cast<Base>(value * (T(1) << (Fraction - NumFractionBits))), RawConstructorTag{});
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <bool OtherRounding>
constexpr fixed<Base, Intermediate, Fraction, Rounding> & fixed<Base, Intermediate, Fraction, Rounding>::operator+=(
  const fixed<Base, Intermediate, Fraction, OtherRounding> & other) noexcept {
  _value += other.rawValue();

  return *this;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <integral T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> & fixed<Base, Intermediate, Fraction, Rounding>::operator+=(
  const T & other) noexcept {
  _value = static_cast<Base>(static_cast<Intermediate>(_value) + other * _fractionMult());

  return *this;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <bool OtherRounding>
constexpr fixed<Base, Intermediate, Fraction, Rounding> & fixed<Base, Intermediate, Fraction, Rounding>::operator-=(
  const fixed<Base, Intermediate, Fraction, OtherRounding> & other) noexcept {
  _value -= other.rawValue();

  return *this;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <integral T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> & fixed<Base, Intermediate, Fraction, Rounding>::operator-=(
  const T & other) noexcept {
  _value = static_cast<Base>(static_cast<Intermediate>(_value) - other * _fractionMult());

  return *this;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <bool OtherRounding>
constexpr fixed<Base, Intermediate, Fraction, Rounding> & fixed<Base, Intermediate, Fraction, Rounding>::operator*=(
  const fixed<Base, Intermediate, Fraction, OtherRounding> & other) noexcept {
  const auto otherRaw = other.rawValue();

  if constexpr (Rounding) {
    // To correctly round the last bit in the result, we need one more bit of information.
    // We do this by multiplying by two before dividing and adding the LSB to the real result.
    auto value = (static_cast<Intermediate>(_value) * otherRaw) / (_fractionMult() / 2);
    _value = static_cast<Base>((value / 2) + (value % 2));
  } else {
    auto value = (static_cast<Intermediate>(_value) * otherRaw) / _fractionMult();
    _value = static_cast<Base>(value);
  }

  return *this;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <integral T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> & fixed<Base, Intermediate, Fraction, Rounding>::operator*=(
  const T & other) noexcept {
  _value *= other;

  return *this;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <bool OtherRounding>
constexpr fixed<Base, Intermediate, Fraction, Rounding> & fixed<Base, Intermediate, Fraction, Rounding>::operator/=(
  const fixed<Base, Intermediate, Fraction, OtherRounding> & other) noexcept {
  const auto divisor = other.rawValue();
  assert_message(divisor != 0, "fixed operator/=: divisor must not be zero");

  if constexpr (Rounding) {
    // To correctly round the last bit in the result, we need one more bit of information.
    // We do this by multiplying by two before dividing and adding the LSB to the real result.
    auto value = (static_cast<Intermediate>(_value) * _fractionMult() * 2) / divisor;
    _value = static_cast<Base>((value / 2) + (value % 2));
  } else {
    auto value = (static_cast<Intermediate>(_value) * _fractionMult()) / divisor;
    _value = static_cast<Base>(value);
  }

  return *this;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
template <integral T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> & fixed<Base, Intermediate, Fraction, Rounding>::operator/=(
  const T & other) noexcept {
  assert_message(other != 0, "fixed operator/=(T): integral divisor must not be zero");

  _value /= other;

  return *this;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
constexpr fixed<Base, Intermediate, Fraction, Rounding>::fixed(const Base & val, RawConstructorTag) noexcept
  : _value(val) {}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires ValidFixedPointTypes<Base, Intermediate, Fraction>
consteval Intermediate fixed<Base, Intermediate, Fraction, Rounding>::_fractionMult() noexcept {
  return Intermediate(1) << Fraction;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding>
  requires signed_integral<Base>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator-(
  const fixed<Base, Intermediate, Fraction, Rounding> & value) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>::fromRawValue(-value.rawValue());
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator+(
  const fixed<Base, Intermediate, Fraction, Rounding> & a,
  const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>(a) += b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator+(
  const fixed<Base, Intermediate, Fraction, Rounding> & a, const T & b) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>(a) += b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator+(
  const T & a, const fixed<Base, Intermediate, Fraction, Rounding> & b) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>(a) += b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator-(
  const fixed<Base, Intermediate, Fraction, Rounding> & a,
  const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>(a) -= b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator-(
  const fixed<Base, Intermediate, Fraction, Rounding> & a, const T & b) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>(a) -= b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator-(
  const T & a, const fixed<Base, Intermediate, Fraction, Rounding> & b) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>(a) -= b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator*(
  const fixed<Base, Intermediate, Fraction, Rounding> & a,
  const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>(a) *= b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator*(
  const fixed<Base, Intermediate, Fraction, Rounding> & a, const T & b) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>(a) *= b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator*(
  const T & a, const fixed<Base, Intermediate, Fraction, Rounding> & b) noexcept {
  return fixed<Base, Intermediate, Fraction, Rounding>(a) *= b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator/(
  const fixed<Base, Intermediate, Fraction, Rounding> & a,
  const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept {
  assert_message(b.rawValue() != 0, "operator/(fixed, fixed): divisor must not be zero");

  return fixed<Base, Intermediate, Fraction, Rounding>(a) /= b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator/(
  const fixed<Base, Intermediate, Fraction, Rounding> & a, const T & b) noexcept {
  assert_message(b != 0, "operator/(fixed, T): integral divisor must not be zero");

  return fixed<Base, Intermediate, Fraction, Rounding>(a) /= b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, typename T>
  requires integral<T>
constexpr fixed<Base, Intermediate, Fraction, Rounding> operator/(
  const T & a, const fixed<Base, Intermediate, Fraction, Rounding> & b) noexcept {
  assert_message(b.rawValue() != 0, "operator/(T, fixed): divisor must not be zero");

  return fixed<Base, Intermediate, Fraction, Rounding>(a) /= b;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
constexpr bool operator==(const fixed<Base, Intermediate, Fraction, Rounding> & a,
                          const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept {
  return a.rawValue() == b.rawValue();
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, integral T>
constexpr bool operator==(const fixed<Base, Intermediate, Fraction, Rounding> & a, const T & b) noexcept {
  return a.rawValue() == fixed<Base, Intermediate, Fraction, Rounding>(b).rawValue();
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, integral T>
constexpr bool operator==(const T & a, const fixed<Base, Intermediate, Fraction, Rounding> & b) noexcept {
  return b == a;
}

template <typename Base, typename Intermediate, unsigned Fraction, bool Rounding, bool OtherRounding>
constexpr strong_ordering operator<=>(const fixed<Base, Intermediate, Fraction, Rounding> & a,
                                      const fixed<Base, Intermediate, Fraction, OtherRounding> & b) noexcept {
  return a.rawValue() <=> b.rawValue();
}

} // namespace toy::math

#endif // INCLUDE_MATH_FIXED_INL_
