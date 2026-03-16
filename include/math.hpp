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
  \file   math.hpp
  \brief  Umbrella header for the math module.

  Provides \ref toy::math::Point, \ref toy::math::Vector2, \ref toy::math::Fixed, constants, and utilities. Include this
  header only; do not include internal headers (e.g. \c math/point.hpp, \c math/vector2.hpp) directly.
*/

#ifndef INCLUDE_MATH_HPP_
#define INCLUDE_MATH_HPP_

#include <numbers>

#include "core.hpp"

/*!
  \namespace toy::math
  \brief Mathematical types, constants, and utilities: fixed-point, vectors, point.

  \sa toy::math::Point, toy::math::Vector2, toy::math::Fixed
*/
namespace toy::math {

// C++ Standard Library headers https://en.cppreference.com/w/cpp/headers.html

// https://en.cppreference.com/w/cpp/header/type_traits.html
using std::true_type;

// https://en.cppreference.com/w/cpp/header/type_traits.html
using std::false_type;

// Metaprogramming library https://en.cppreference.com/w/cpp/meta.html

// https://en.cppreference.com/w/cpp/types/is_signed.html
using std::is_signed_v;

// Algorithms library https://en.cppreference.com/w/cpp/algorithm.html

// https://en.cppreference.com/w/cpp/algorithm/max.html
using std::max;

// Concepts library https://en.cppreference.com/w/cpp/concepts.html

// https://en.cppreference.com/w/cpp/concepts/integral.html
using std::integral;

// https://en.cppreference.com/w/cpp/concepts/signed_integral.html
using std::signed_integral;

// https://en.cppreference.com/w/cpp/concepts/floating_point.html
using std::floating_point;

//              Numerics library https://en.cppreference.com/w/cpp/numeric
// Common mathematical functions https://en.cppreference.com/w/cpp/numeric/math

// https://en.cppreference.com/w/cpp/numeric/math/isnan
using std::isnan;

/// Floating‑point scalar type.
using real_t = float;

} // namespace toy::math

//--------------------------------------------------------------------------------------------------------------------

#include "math/constants.hpp"
#include "math/fixed.hpp"
#include "math/fixed_std_extension.hpp"
#include "math/point.hpp"
#include "math/utils.hpp"
#include "math/vector2.hpp"

//----------------------------------------------------------------------------------------------------------------------

#include "math/fixed.inl"
#include "math/point.inl"
#include "math/utils.inl"
#include "math/vector2.inl"

#endif // INCLUDE_MATH_HPP_
