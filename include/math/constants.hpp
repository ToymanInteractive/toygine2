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
// OTHERWISE, ARISING FROM OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
/*!
  \file   constants.hpp
  \brief  Mathematical constants for floating-point and \ref toy::math::fixed_point types.

  Variable templates analogous to \c std::numbers (C++20). Supported types: \c float, \c double, \c long \c double, and
  \ref toy::math::fixed. Values for built-in floating-point types come from the standard library; values for
  \ref toy::math::fixed come from \c std::numbers specializations in \c math/fixed_std_extension.hpp.

  | Constant         | Description                        |
  |------------------|------------------------------------|
  | \c e_v           | The mathematical constant e.       |
  | \c pi_v          | The mathematical constant π.       |
  | \c log2e_v       | Base-2 logarithm of e.             |
  | \c log10e_v      | Base-10 logarithm of e.            |
  | \c sqrt2_v       | Square root of \c 2                |
  | \c sqrt3_v       | Square root of \c 3.               |
  | \c inv_pi_v      | 1 over π.                          |
  | \c inv_sqrtpi_v  | 1 over square root of π.           |
  | \c ln2_v         | Natural logarithm of \c 2.         |
  | \c ln10_v        | Natural logarithm of \c 10.        |
  | \c egamma_v      | Euler–Mascheroni constant γ.       |
  | \c phi_v         | Golden ratio Φ.                    |
  | \c rad_per_deg_v | π over \c 180 (radians per degree) |
  | \c deg_per_rad_v | \c 180 over π (degrees per radian) |

  \note Included by math.hpp only; do not include this file directly.

  \sa https://en.cppreference.com/w/cpp/numeric/constants
*/

#ifndef INCLUDE_MATH_CONSTANTS_HPP_
#define INCLUDE_MATH_CONSTANTS_HPP_

#include "math/fixed.hpp"
#include "math/fixed_std_extension.hpp"

namespace toy::math {

/*!
  \concept FractionalConstantType
  \brief Type that can be used with \ref toy::math::constants variable templates.

  Satisfied for \ref toy::math::floating_point types (\c float, \c double, \c long \c double) and
  \ref toy::math::fixed_point types (\ref toy::math::fixed).

  \section requirements Requirements

  A type \a T satisfies FractionalConstantType if and only if at least one of the following holds:
  - \a T satisfies \ref toy::math::floating_point.
  - \a T satisfies \ref toy::math::fixed_point.
*/
template <typename T>
concept FractionalConstantType = floating_point<T> || fixed_point<T>;

/*!
  \namespace toy::math::constants
  \brief Variable templates for mathematical constants.

  Single API for \ref toy::math::FractionalConstantType: \c float, \c double, \c long \c double, and
  \ref toy::math::fixed. Values forward to \c std::numbers for built-in floating-point types and to \c std::numbers
  specializations for \ref toy::math::fixed (see \c math/fixed_std_extension.hpp).
*/
namespace constants {

/// Mathematical constant e.
template <FractionalConstantType T>
constexpr T e_v = std::numbers::e_v<T>;

/// Mathematical constant π.
template <FractionalConstantType T>
constexpr T pi_v = std::numbers::pi_v<T>;

/// Base-2 logarithm of e.
template <FractionalConstantType T>
constexpr T log2e_v = std::numbers::log2e_v<T>;

/// Base-10 logarithm of e.
template <FractionalConstantType T>
constexpr T log10e_v = std::numbers::log10e_v<T>;

/// Square root of 2.
template <FractionalConstantType T>
constexpr T sqrt2_v = std::numbers::sqrt2_v<T>;

/// Square root of 3.
template <FractionalConstantType T>
constexpr T sqrt3_v = std::numbers::sqrt3_v<T>;

/// 1 over π.
template <FractionalConstantType T>
constexpr T inv_pi_v = std::numbers::inv_pi_v<T>;

/// 1 over square root of π.
template <FractionalConstantType T>
constexpr T inv_sqrtpi_v = std::numbers::inv_sqrtpi_v<T>;

/// Natural logarithm of 2.
template <FractionalConstantType T>
constexpr T ln2_v = std::numbers::ln2_v<T>;

/// Natural logarithm of 10.
template <FractionalConstantType T>
constexpr T ln10_v = std::numbers::ln10_v<T>;

/// Euler–Mascheroni constant γ.
template <FractionalConstantType T>
constexpr T egamma_v = std::numbers::egamma_v<T>;

/// Golden ratio Φ.
template <FractionalConstantType T>
constexpr T phi_v = std::numbers::phi_v<T>;

/// π over 180 (radians per degree).
template <FractionalConstantType T>
constexpr T rad_per_deg_v = static_cast<T>(std::numbers::pi_v<long double> / 180.0L);

/// 180 over π (degrees per radian).
template <FractionalConstantType T>
constexpr T deg_per_rad_v = static_cast<T>(180.0L / std::numbers::pi_v<long double>);

} // namespace constants

} // namespace toy::math

#endif // INCLUDE_MATH_CONSTANTS_HPP_
