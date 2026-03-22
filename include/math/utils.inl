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
  \brief  Inline implementations for utilities declared in \c math/utils.hpp.

  Included by \ref math.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_MATH_UTILS_INL_
#define INCLUDE_MATH_UTILS_INL_

namespace toy::math {

template <typename T>
  requires(signed_integral<T> || floating_point<T> || fixed_point<T>)
constexpr T abs(const T & value) noexcept {
  if constexpr (signed_integral<T>) {
    assert_message(value != numeric_limits<T>::min(), "abs() of the minimum signed integer is not representable");
    T mask = value >> numeric_limits<T>::digits;
    return (value + mask) ^ mask;
  } else if constexpr (floating_point<T>) {
    if constexpr (std::same_as<T, float>) {
      // Branch-free: clear sign bit (float has fixed 32-bit representation on supported platforms).
      auto bits = bit_cast<uint32_t>(value);
      bits &= 0x7FFFFFFF;
      return bit_cast<float>(bits);
    } else if constexpr (std::same_as<T, double>) {
      // Branch-free: clear sign bit (double has fixed 64-bit representation on supported platforms).
      auto bits = bit_cast<uint64_t>(value);
      bits &= 0x7FFFFFFFFFFFFFFF;
      return bit_cast<double>(bits);
    } else {
      // long double layout is platform-dependent; bit_cast not portable.
      return value < T(0) ? -value : value;
    }
  } else if constexpr (fixed_point<T>) {
    using Base = decltype(value.rawValue());
    constexpr int digits = numeric_limits<Base>::digits;
    auto raw = value.rawValue();
    assert_message(raw != numeric_limits<Base>::min(), "abs() of the minimum fixed-point value is not representable");
    Base mask = raw >> digits;
    return T::fromRawValue(static_cast<Base>((raw + mask) ^ mask));
  }
}

template <typename T>
  requires(floating_point<T> || fixed_point<T>)
constexpr bool isEqual(const T & a, const T & b, T absEpsilon, T relEpsilon) noexcept {
  assert_message(absEpsilon >= T{0} && relEpsilon >= T{0}, "absolute and relative epsilon must be non-negative");
  if constexpr (floating_point<T>) {
    if !consteval {
      assert_message(!isnan(a) && !isnan(b), "isEqual() does not support NaN values");
    }
  }

  const T diff = abs(a - b);
  if (diff <= absEpsilon)
    return true;

  return diff <= max(abs(a), abs(b)) * relEpsilon;
}

template <typename T>
  requires(floating_point<T> || fixed_point<T>)
constexpr T deg2rad(const T & angle) noexcept {
  return angle * constants::rad_per_deg_v<T>;
}

template <typename T>
  requires(floating_point<T> || fixed_point<T>)
constexpr T rad2deg(const T & angle) noexcept {
  return angle * constants::deg_per_rad_v<T>;
}

} // namespace toy::math

#endif // INCLUDE_MATH_UTILS_INL_
