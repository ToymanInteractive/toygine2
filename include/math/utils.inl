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
  constexpr int shift = sizeof(T) * 8 - 1;
  T mask = value >> shift;
  return (value + mask) ^ mask;
}

template <std::floating_point T>
constexpr T abs(T value) noexcept {
  if constexpr (std::same_as<T, float>) {
    auto bits = std::bit_cast<uint32_t>(value);

    bits &= 0x7FFFFFFF;

    return std::bit_cast<float>(bits);
  } else if constexpr (std::same_as<T, double>) {
    auto bits = std::bit_cast<uint64_t>(value);

    bits &= 0x7FFFFFFFFFFFFFFF;

    return std::bit_cast<double>(bits);
  } else { // long double
    return value < T(0) ? -value : value;
  }
}

} // namespace toy::math

#endif // INCLUDE_MATH_UTILS_INL_
