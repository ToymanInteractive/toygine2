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
  \file   ellipse.inl
  \brief  Inline implementations for \ref toy::geometry::Ellipse.
*/

#ifndef INCLUDE_GEOMETRY_ELLIPSE_INL_
#define INCLUDE_GEOMETRY_ELLIPSE_INL_

namespace toy::geometry {

template <EllipseComponent T>
constexpr Ellipse<T>::Ellipse(const math::Vector2<T> & c, const math::Vector2<T> & r) noexcept
  : center(c)
  , radiuses(r) {
  assert_message(isValid(), "Ellipse radiuses must satisfy radiuses.x > 0 and radiuses.y > 0");
}

template <EllipseComponent T>
constexpr T Ellipse<T>::area() const noexcept {
  return math::constants::pi_v<T> * radiuses.x * radiuses.y;
}

template <EllipseComponent T>
constexpr void Ellipse<T>::reset() noexcept {
  center.setZero();
  radiuses.setZero();
}

template <EllipseComponent T>
constexpr bool Ellipse<T>::isReset() const noexcept {
  return center.isZero() && radiuses.isZero();
}

template <EllipseComponent T>
constexpr bool Ellipse<T>::isValid() const noexcept {
  if constexpr (math::floating_point<T>) {
    constexpr T zero{0};

    return radiuses.x > zero && radiuses.y > zero;
  } else if constexpr (math::fixed_point<T>) {
    return radiuses.x.rawValue() > 0 && radiuses.y.rawValue() > 0;
  }
}

template <EllipseComponent T>
constexpr bool Ellipse<T>::isContain(const math::Vector2<T> & vector) const noexcept {
  assert_message(isValid(), "isContain requires a valid ellipse (both semi-axes positive)");

  const auto normalized = vector - center;

  return ((normalized.x * normalized.x) / (radiuses.x * radiuses.x)
          + (normalized.y * normalized.y) / (radiuses.y * radiuses.y))
         <= T{1};
}

template <EllipseComponent T>
constexpr bool operator==(const Ellipse<T> & left, const Ellipse<T> & right) noexcept {
  return left.center == right.center && left.radiuses == right.radiuses;
}

template <EllipseComponent T>
constexpr bool operator!=(const Ellipse<T> & left, const Ellipse<T> & right) noexcept {
  return !(left == right);
}

} // namespace toy::geometry

#endif // INCLUDE_GEOMETRY_ELLIPSE_INL_
