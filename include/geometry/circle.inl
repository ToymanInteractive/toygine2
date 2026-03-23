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
  \file   circle.inl
  \brief  Inline implementations for \ref toy::geometry::Circle.

  \note Included by geometry.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_GEOMETRY_CIRCLE_INL_
#define INCLUDE_GEOMETRY_CIRCLE_INL_

namespace toy::geometry {

template <CircleComponent T>
constexpr Circle<T>::Circle() noexcept
  : center()
  , radius() {
#ifdef _DEBUG
  if constexpr (math::floating_point<T>) {
    radius = numeric_limits<T>::signaling_NaN();
  }
#endif
}

template <CircleComponent T>
constexpr Circle<T>::Circle(const math::Vector2<T> & c, const T & r) noexcept
  : center(c)
  , radius(r) {
  assert_message(isValid(), "Circle radius must satisfy radius > 0");
}

template <CircleComponent T>
constexpr T Circle<T>::area() const noexcept {
  return math::constants::pi_v<T> * (radius * radius);
}

template <CircleComponent T>
constexpr void Circle<T>::reset() noexcept {
  center.setZero();
  radius = T{0};
}

template <CircleComponent T>
constexpr bool Circle<T>::isReset() const noexcept {
  if (!center.isZero())
    return false;

  if constexpr (math::floating_point<T>) {
    return math::isEqual(radius, T{0});
  } else if constexpr (math::fixed_point<T>) {
    return radius.rawValue() == 0;
  }
}

template <CircleComponent T>
constexpr bool Circle<T>::isValid() const noexcept {
  if constexpr (math::floating_point<T>) {
    return radius > T{0};
  } else if constexpr (math::fixed_point<T>) {
    return radius.rawValue() > 0;
  }
}

template <CircleComponent T>
constexpr bool Circle<T>::isContain(const math::Vector2<T> & vector) const noexcept {
  const auto distanceSqr = (vector - center).sqrMagnitude();

  return distanceSqr <= (radius * radius);
}

template <CircleComponent T>
constexpr bool operator==(const Circle<T> & left, const Circle<T> & right) noexcept {
  if (left.center != right.center)
    return false;

  if constexpr (math::floating_point<T>) {
    return math::isEqual(left.radius, right.radius);
  } else if constexpr (math::fixed_point<T>) {
    return left.radius == right.radius;
  }
}

template <CircleComponent T>
constexpr bool operator!=(const Circle<T> & left, const Circle<T> & right) noexcept {
  return !(left == right);
}

} // namespace toy::geometry

#endif // INCLUDE_GEOMETRY_CIRCLE_INL_
