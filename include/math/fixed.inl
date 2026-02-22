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

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::integral T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::fixed(T value) noexcept
  : _value(static_cast<BaseType>(value * _fractionMult())) {}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::floating_point T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::fixed(T value) noexcept
  : _value(static_cast<BaseType>(EnableRounding
                                   ? (value * static_cast<T>(_fractionMult()) + ((value >= T{0}) ? T{0.5} : T{-0.5}))
                                   : (value * static_cast<T>(_fractionMult())))) {}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <typename B, typename I, unsigned F, bool R>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::fixed(fixed<B, I, F, R> val) noexcept
  : _value(fromFixedPoint<F>(val.rawValue()).rawValue()) {}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::integral T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::operator T() const noexcept {
  return static_cast<T>(_value / _fractionMult());
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::floating_point T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::operator T() const noexcept {
  return static_cast<T>(_value) / _fractionMult();
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
constexpr BaseType fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::rawValue() const noexcept {
  return _value;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> fixed<
  BaseType, IntermediateType, FractionBits, EnableRounding>::fromRawValue(BaseType value) noexcept {
  return fixed(value, RawConstructorTag{});
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <unsigned NumFractionBits, typename T>
  requires(NumFractionBits > FractionBits)
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> fixed<
  BaseType, IntermediateType, FractionBits, EnableRounding>::fromFixedPoint(T value) noexcept {
  // To correctly round the last bit in the result, we need one more bit of information.
  // We do this by multiplying by two before dividing and adding the LSB to the real result.
  return EnableRounding
           ? fixed(static_cast<BaseType>(value / (T(1) << (NumFractionBits - FractionBits))
                                         + (value / (T(1) << (NumFractionBits - FractionBits - 1)) % 2)),
                   RawConstructorTag{})
           : fixed(static_cast<BaseType>(value / (T(1) << (NumFractionBits - FractionBits))), RawConstructorTag{});
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <unsigned NumFractionBits, typename T>
  requires(NumFractionBits <= FractionBits)
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> fixed<
  BaseType, IntermediateType, FractionBits, EnableRounding>::fromFixedPoint(T value) noexcept {
  return fixed(static_cast<BaseType>(value * (T(1) << (FractionBits - NumFractionBits))), RawConstructorTag{});
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <bool OtherRounding>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & fixed<
  BaseType, IntermediateType, FractionBits,
  EnableRounding>::operator+=(const fixed<BaseType, IntermediateType, FractionBits, OtherRounding> & other) noexcept {
  _value += other.rawValue();

  return *this;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::integral T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & fixed<
  BaseType, IntermediateType, FractionBits, EnableRounding>::operator+=(T other) noexcept {
  _value = static_cast<BaseType>(static_cast<IntermediateType>(_value) + other * _fractionMult());

  return *this;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <bool OtherRounding>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & fixed<
  BaseType, IntermediateType, FractionBits,
  EnableRounding>::operator-=(const fixed<BaseType, IntermediateType, FractionBits, OtherRounding> & other) noexcept {
  _value -= other.rawValue();

  return *this;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::integral T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & fixed<
  BaseType, IntermediateType, FractionBits, EnableRounding>::operator-=(T other) noexcept {
  _value = static_cast<BaseType>(static_cast<IntermediateType>(_value) - other * _fractionMult());

  return *this;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <bool OtherRounding>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & fixed<
  BaseType, IntermediateType, FractionBits,
  EnableRounding>::operator*=(const fixed<BaseType, IntermediateType, FractionBits, OtherRounding> & other) noexcept {
  auto const otherRaw = other.rawValue();

  if constexpr (EnableRounding) {
    // To correctly round the last bit in the result, we need one more bit of information.
    // We do this by multiplying by two before dividing and adding the LSB to the real result.
    auto value = (static_cast<IntermediateType>(_value) * otherRaw) / (_fractionMult() / 2);
    _value = static_cast<BaseType>((value / 2) + (value % 2));
  } else {
    auto value = (static_cast<IntermediateType>(_value) * otherRaw) / _fractionMult();
    _value = static_cast<BaseType>(value);
  }

  return *this;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::integral T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & fixed<
  BaseType, IntermediateType, FractionBits, EnableRounding>::operator*=(T other) noexcept {
  _value *= other;

  return *this;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <bool OtherRounding>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & fixed<
  BaseType, IntermediateType, FractionBits,
  EnableRounding>::operator/=(const fixed<BaseType, IntermediateType, FractionBits, OtherRounding> & other) noexcept {
  auto const divisor = other.rawValue();
  assert_message(divisor != 0, "fixed operator/=: divisor must not be zero");

  if constexpr (EnableRounding) {
    // To correctly round the last bit in the result, we need one more bit of information.
    // We do this by multiplying by two before dividing and adding the LSB to the real result.
    auto value = (static_cast<IntermediateType>(_value) * _fractionMult() * 2) / divisor;
    _value = static_cast<BaseType>((value / 2) + (value % 2));
  } else {
    auto value = (static_cast<IntermediateType>(_value) * _fractionMult()) / divisor;
    _value = static_cast<BaseType>(value);
  }

  return *this;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::integral T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & fixed<
  BaseType, IntermediateType, FractionBits, EnableRounding>::operator/=(T other) noexcept {
  assert_message(other != 0, "fixed operator/=(T): integral divisor must not be zero");

  _value /= other;

  return *this;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::fixed(BaseType val,
                                                                                 RawConstructorTag) noexcept
  : _value(val) {}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
consteval IntermediateType fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::_fractionMult() noexcept {
  return IntermediateType(1) << FractionBits;
}

template <typename BaseType, typename IntermediateType, unsigned FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits> && std::signed_integral<BaseType>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> operator-(
  const fixed<BaseType, IntermediateType, FractionBits, EnableRounding> & value) noexcept {
  return fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::fromRawValue(-value.rawValue());
}

} // namespace toy::math

#endif // INCLUDE_MATH_FIXED_INL_
