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
  \brief  General math utilities: abs, isEqual, deg2rad, rad2deg.

  Declares overloads in namespace \ref toy::math for signed integers, floating-point, and fixed-point types.

  \note Included by math.hpp; do not include this file directly.
*/

#ifndef INCLUDE_MATH_UTILS_HPP_
#define INCLUDE_MATH_UTILS_HPP_

/*!
  \defgroup MathFunctions Common mathematical functions
  \brief Math utilities, fixed-point arithmetic, and related types.

  - **abs**: Absolute value for signed integers, floating-point (\c float, \c double, \c long \c double), and
             fixed-point; branch-free for integral and common float types where applicable.
  - **isEqual**: Approximate equality with configurable absolute and relative epsilon (default 8× and 64×
                 \c numeric_limits<T>::epsilon()); overloads for standard floating-point and fixed-point types.
  - **deg2rad**, **rad2deg**: Angle conversion (degrees ↔ radians) for floating-point and fixed-point.

  \section features Key Features

  - **Constexpr support**: Available where logically feasible.
  - **Exception safety**: Uses \c noexcept operations.
  - **Type safety**: Enforced through C++20 concepts.
  - **Allocation-Free Core Types**: No dynamic allocation in core math types.
*/

namespace toy::math {

/*!
  \brief Returns the absolute value of \a value.
  \ingroup MathFunctions

  Supports signed integers, floating-point, and fixed-point. Branch-free for signed integrals and fixed-point, for
  floating-point uses a conditional. Usable in constant expressions.

  \tparam T Must satisfy \ref toy::math::signed_integral, \ref toy::math::floating_point, or
            \ref toy::math::fixed_point.

  \param value The value (signed integer, floating-point, or fixed-point).

  \return The absolute value of \a value (unchanged if non-negative, negated if negative).
*/
template <typename T>
  requires(signed_integral<T> || floating_point<T> || fixed_point<T>)
[[nodiscard]] constexpr T abs(const T & value) noexcept;

/*!
  \brief Compares two values for approximate equality.
  \ingroup MathFunctions

  Combined absolute and relative epsilon test: small values by \a absEpsilon, large values by \a relEpsilon scaled by
  max(|a|, |b|). Supports floating-point and fixed-point. For floating-point, does not support NaN inputs (assert in
  debug).

  \tparam T Must satisfy \ref toy::math::floating_point or \ref toy::math::fixed_point.

  \param a          The first value.
  \param b          The second value.
  \param absEpsilon The maximum absolute difference treated as equal (default: 8× \c numeric_limits<T>::epsilon()).
  \param relEpsilon The maximum relative difference (default: 64× \c numeric_limits<T>::epsilon()).

  \return \c true if \a a and \a b are considered equal under the chosen tolerances, \c false otherwise.
*/
template <typename T>
  requires(floating_point<T> || fixed_point<T>)
[[nodiscard]] constexpr bool isEqual(const T & a, const T & b, T absEpsilon = 8 * numeric_limits<T>::epsilon(),
                                     T relEpsilon = 64 * numeric_limits<T>::epsilon()) noexcept;

/*!
  \brief Converts angle from degrees to radians.
  \ingroup MathFunctions

  \tparam T Scalar type; must satisfy \ref toy::math::floating_point or \ref toy::math::fixed_point.

  \param angle Angle in degrees.

  \return Angle in radians (\a angle × π / 180). Supports \c float, \c double, \c long \c double, and
          \ref toy::math::fixed.

  \sa rad2deg
*/
template <typename T>
  requires(floating_point<T> || fixed_point<T>)
[[nodiscard]] constexpr T deg2rad(const T & angle) noexcept;

/*!
  \brief Converts angle from radians to degrees.
  \ingroup MathFunctions

  \tparam T Scalar type; must satisfy \ref toy::math::floating_point or \ref toy::math::fixed_point.

  \param angle Angle in radians.

  \return Angle in degrees (\a angle × 180 / π). Supports \c float, \c double, \c long \c double, and
          \ref toy::math::fixed.

  \sa deg2rad
*/
template <typename T>
  requires(floating_point<T> || fixed_point<T>)
[[nodiscard]] constexpr T rad2deg(const T & angle) noexcept;

} // namespace toy::math

#endif // INCLUDE_MATH_UTILS_HPP_
