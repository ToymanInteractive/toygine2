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
  \file   utils.hpp
  \brief  General math utilities.
*/

#ifndef INCLUDE_MATH_UTILS_HPP_
#define INCLUDE_MATH_UTILS_HPP_

namespace toy::math {

/*!
  \brief Returns the absolute value of a signed integer.

  This function computes the absolute value without branches. Usable in constant expressions.

  \param value The signed integer value.

  \return The absolute value of \a value (unchanged if non-negative, negated if negative).
*/
template <std::signed_integral T>
[[nodiscard]] constexpr T abs(T value) noexcept;

/*!
  \brief Returns the absolute value of a floating-point number.

  This function computes the absolute value without branches for \c float and \c double; usable in constant expressions.

  \param value The floating-point value.

  \return The absolute value of \a value (unchanged if non-negative, negated if negative).
*/
template <std::floating_point T>
[[nodiscard]] constexpr T abs(T value) noexcept;

/*!
  \brief Compares two single-precision floats for approximate equality.

  This function uses a combined absolute and relative epsilon test. Small values are compared by absolute difference (\a
  absEpsilon), large values by relative difference (\a relEpsilon) scaled by max(|a|, |b|).

  \param a          The first value.
  \param b          The second value.
  \param absEpsilon The maximum absolute difference treated as equal (default: 8× machine epsilon).
  \param relEpsilon The maximum relative difference (default: 64× machine epsilon).

  \return \c true if \a a and \a b are considered equal under the chosen tolerances, \c false otherwise.
*/
[[nodiscard]] constexpr bool isEqual(float a, float b, float absEpsilon = 8.0f * numeric_limits<float>::epsilon(),
                                     float relEpsilon = 64.0f * numeric_limits<float>::epsilon()) noexcept;

} // namespace toy::math

#endif // INCLUDE_MATH_UTILS_HPP_
