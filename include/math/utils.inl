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
  \file   utils.inl
  \brief  Inline implementations for General math utilities.
*/

#ifndef INCLUDE_MATH_UTILS_INL_
#define INCLUDE_MATH_UTILS_INL_

namespace toy::math {

template <std::signed_integral T>
constexpr T abs(T value) noexcept {
  assert_message(value != numeric_limits<T>::min(), "abs() of the minimum signed integer is not representable");

  T mask = value >> numeric_limits<T>::digits;

  return (value + mask) ^ mask;
}

template <std::floating_point T>
constexpr T abs(T value) noexcept {
  if constexpr (std::same_as<T, float>) {
    // Branch-free: clear IEEE 754 sign bit via bit_cast (float is 32-bit on supported platforms).
    auto bits = bit_cast<uint32_t>(value);
    bits &= 0x7FFFFFFF;

    return bit_cast<float>(bits);
  } else if constexpr (std::same_as<T, double>) {
    // Branch-free: clear IEEE 754 sign bit via bit_cast (double is 64-bit on supported platforms).
    auto bits = bit_cast<uint64_t>(value);
    bits &= 0x7FFFFFFFFFFFFFFF;

    return bit_cast<double>(bits);
  } else {
    // Long double: platform-dependent representation; bit_cast not viable, use conditional.
    return value < T(0) ? -value : value;
  }
}

template <std::floating_point T>
constexpr bool isEqual(T a, T b, T absEpsilon, T relEpsilon) noexcept {
  assert_message(absEpsilon >= 0 && relEpsilon >= 0, "absolute and relative epsilon must be non-negative");
  if !consteval {
    assert_message(!isnan(a) && !isnan(b), "isEqual() does not support NaN values");
  }

  const auto diff = abs(a - b);
  if (diff <= absEpsilon)
    return true;

  return diff <= max(abs(a), abs(b)) * relEpsilon;
}

} // namespace toy::math

#endif // INCLUDE_MATH_UTILS_INL_
