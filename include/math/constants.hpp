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

  \defgroup MathConstants Mathematical constants

  | Constant        | Description |
  |-----------------|-------------|
  | \c e_v          | The mathematical constant \a e |
  | \c pi_v         | The mathematical constant π |
  | \c log2e_v      | \f$ \log_2 e \f$ |
  | \c log10e_v     | \f$ \log_{10} e \f$ |
  | \c sqrt2_v      | \f$ \sqrt{2} \f$ |
  | \c sqrt3_v      | \f$ \sqrt{3} \f$ |
  | \c inv_pi_v     | \f$ 1/\pi \f$ |
  | \c inv_sqrtpi_v | \f$ 1/\sqrt{\pi} \f$ |
  | \c ln2_v        | \f$ \ln 2 \f$ |
  | \c ln10_v       | \f$ \ln 10 \f$ |
  | \c egamma_v     | Euler–Mascheroni constant γ |
  | \c phi_v        | Golden ratio Φ |
  | \c rad_per_deg_v| \f$ \pi/180 \f$ (radians per degree, for deg2rad) |
  | \c deg_per_rad_v| \f$ 180/\pi \f$ (degrees per radian, for rad2deg) |

  \sa https://en.cppreference.com/w/cpp/numeric/constants
*/

#ifndef INCLUDE_MATH_CONSTANTS_HPP_
#define INCLUDE_MATH_CONSTANTS_HPP_

#include "math/fixed.hpp"
#include "math/fixed_std_extension.hpp"

namespace toy::math {

/*!
  \concept NumericConstantType
  \brief Type that can be used with \ref toy::math::constants variable templates.

  Satisfied for \c std::floating_point types (\c float, \c double, \c long \c double) and \ref toy::math::fixed_point
  types (\ref toy::math::fixed).
*/
template <typename T>
concept NumericConstantType = std::floating_point<T> || fixed_point<T>;

namespace constants {

/// The mathematical constant \a e.
template <NumericConstantType T>
constexpr T e_v = std::numbers::e_v<T>;

/// The mathematical constant π.
template <NumericConstantType T>
constexpr T pi_v = std::numbers::pi_v<T>;

/// \f$ \log_2 e \f$
template <NumericConstantType T>
constexpr T log2e_v = std::numbers::log2e_v<T>;

/// \f$ \log_{10} e \f$
template <NumericConstantType T>
constexpr T log10e_v = std::numbers::log10e_v<T>;

/// \f$ \sqrt{2} \f$
template <NumericConstantType T>
constexpr T sqrt2_v = std::numbers::sqrt2_v<T>;

/// \f$ \sqrt{3} \f$
template <NumericConstantType T>
constexpr T sqrt3_v = std::numbers::sqrt3_v<T>;

/// \f$ 1/\pi \f$
template <NumericConstantType T>
constexpr T inv_pi_v = std::numbers::inv_pi_v<T>;

/// \f$ 1/\sqrt{\pi} \f$
template <NumericConstantType T>
constexpr T inv_sqrtpi_v = std::numbers::inv_sqrtpi_v<T>;

/// \f$ \ln 2 \f$
template <NumericConstantType T>
constexpr T ln2_v = std::numbers::ln2_v<T>;

/// \f$ \ln 10 \f$
template <NumericConstantType T>
constexpr T ln10_v = std::numbers::ln10_v<T>;

/// Euler–Mascheroni constant γ.
template <NumericConstantType T>
constexpr T egamma_v = std::numbers::egamma_v<T>;

/// Golden ratio Φ.
template <NumericConstantType T>
constexpr T phi_v = std::numbers::phi_v<T>;

} // namespace constants

} // namespace toy::math

#endif // INCLUDE_MATH_CONSTANTS_HPP_
