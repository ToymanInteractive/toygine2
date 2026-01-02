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
  \brief  Umbrella header for the engine mathematics module
*/

#ifndef INCLUDE_MATH_HPP_
#define INCLUDE_MATH_HPP_

#include "core.hpp"

/// @namespace toy::math
/// @brief Contains all public mathematical types, constants, and utility functions of engine.
namespace toy::math {

/// Floating‑point scalar type.
using real_t = float;

} // namespace toy::math

#include "math/point.hpp"
#include "math/vector2d.hpp"

//----------------------------------------------------------------------------------------------------------------------

#include "math/point.inl"
#include "math/vector2d.inl"

#endif // INCLUDE_MATH_HPP_
