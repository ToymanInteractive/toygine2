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

template <typename BaseType, typename IntermediateType, unsigned int FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::integral T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::fixed(T value) noexcept
  : _value(static_cast<BaseType>(value * _fractionMult())) {}

template <typename BaseType, typename IntermediateType, unsigned int FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::floating_point T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::fixed(T value) noexcept
  : _value(static_cast<BaseType>(EnableRounding ? (value * _fractionMult() + ((value >= 0.0) ? T{0.5} : T{-0.5}))
                                                : (value * _fractionMult()))) {}

template <typename BaseType, typename IntermediateType, unsigned int FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::integral T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::operator T() const noexcept {
  return static_cast<T>(_value / _fractionMult());
}

template <typename BaseType, typename IntermediateType, unsigned int FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
template <std::floating_point T>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::operator T() const noexcept {
  return static_cast<T>(_value) / _fractionMult();
}

template <typename BaseType, typename IntermediateType, unsigned int FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
constexpr BaseType fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::rawValue() const noexcept {
  return _value;
}

template <typename BaseType, typename IntermediateType, unsigned int FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding> fixed<
  BaseType, IntermediateType, FractionBits, EnableRounding>::fromRawValue(BaseType value) noexcept {
  return fixed(value, raw_constructor_tag{});
}

template <typename BaseType, typename IntermediateType, unsigned int FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
constexpr fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::fixed(BaseType val,
                                                                                 raw_constructor_tag) noexcept
  : _value(val) {}

template <typename BaseType, typename IntermediateType, unsigned int FractionBits, bool EnableRounding>
  requires ValidFixedPointTypes<BaseType, IntermediateType, FractionBits>
consteval IntermediateType fixed<BaseType, IntermediateType, FractionBits, EnableRounding>::_fractionMult() noexcept {
  return IntermediateType(1) << FractionBits;
}

} // namespace toy::math

#endif // INCLUDE_MATH_FIXED_INL_
