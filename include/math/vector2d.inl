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
  \file   vector2d.inl
  \brief  Inline implementations for \ref toy::math::Vector2D.
*/

#ifndef INCLUDE_MATH_VECTOR2D_INL_
#define INCLUDE_MATH_VECTOR2D_INL_

namespace toy::math {

template <Vector2DComponent T>
constexpr Vector2D<T>::Vector2D() noexcept
  : x()
  , y() {
#ifdef _DEBUG
  if constexpr (floating_point<T>) {
    x = numeric_limits<T>::signaling_NaN();
    y = numeric_limits<T>::signaling_NaN();
  }
#endif
}

template <Vector2DComponent T>
constexpr Vector2D<T>::Vector2D(const T & _x, const T & _y) noexcept
  : x(_x)
  , y(_y) {}

template <Vector2DComponent T>
constexpr Vector2D<T>::Vector2D(const T * values) noexcept
  : x()
  , y() {
  assert_message(values != nullptr, "values cannot be null");

  x = values[0];
  y = values[1];
}

template <Vector2DComponent T>
constexpr T * Vector2D<T>::c_arr() noexcept {
  return &x;
}

template <Vector2DComponent T>
constexpr const T * Vector2D<T>::c_arr() const noexcept {
  return &x;
}

template <Vector2DComponent T>
constexpr Vector2D<T> & Vector2D<T>::operator+=(const Vector2D<T> & vector) noexcept {
  x += vector.x;
  y += vector.y;

  return *this;
}

template <Vector2DComponent T>
constexpr Vector2D<T> & Vector2D<T>::operator-=(const Vector2D<T> & vector) noexcept {
  x -= vector.x;
  y -= vector.y;

  return *this;
}

template <Vector2DComponent T>
constexpr Vector2D<T> & Vector2D<T>::operator*=(const T & scalar) noexcept {
  x *= scalar;
  y *= scalar;

  return *this;
}

template <Vector2DComponent T>
constexpr Vector2D<T> & Vector2D<T>::operator/=(const T & scalar) noexcept {
  assert_message(scalar > 0 || scalar < 0, "scalar must be non-zero");

  constexpr T one{1};
  const T invScalar = one / scalar;

  *this *= invScalar;

  return *this;
}

template <Vector2DComponent T>
constexpr T Vector2D<T>::sqrMagnitude() const noexcept {
  return x * x + y * y;
}

template <Vector2DComponent T>
constexpr void Vector2D<T>::setZero() noexcept {
  x = y = T{0};
}

template <Vector2DComponent T>
constexpr bool Vector2D<T>::isZero() const noexcept {
  if constexpr (floating_point<T>) {
    return math::isEqual(x, T{0}) && math::isEqual(y, T{0});
  } else if constexpr (fixed_point<T>) {
    return x.rawValue() == 0 && y.rawValue() == 0;
  }
}

template <Vector2DComponent T>
constexpr bool Vector2D<T>::isEqual(const Vector2D<T> & vector, T absEpsilon, T relEpsilon) const noexcept {
  return math::isEqual(x, vector.x, absEpsilon, relEpsilon) && math::isEqual(y, vector.y, absEpsilon, relEpsilon);
}

template <Vector2DComponent T>
constexpr Vector2D<T> operator-(const Vector2D<T> & vector) noexcept {
  return Vector2D(-vector.x, -vector.y);
}

template <Vector2DComponent T>
constexpr Vector2D<T> operator+(const Vector2D<T> & left, const Vector2D<T> & right) noexcept {
  return Vector2D(left.x + right.x, left.y + right.y);
}

template <Vector2DComponent T>
constexpr Vector2D<T> operator-(const Vector2D<T> & left, const Vector2D<T> & right) noexcept {
  return Vector2D(left.x - right.x, left.y - right.y);
}

template <Vector2DComponent T>
constexpr Vector2D<T> operator*(const Vector2D<T> & left, const T & right) noexcept {
  return Vector2D(left.x * right, left.y * right);
}

template <Vector2DComponent T>
constexpr Vector2D<T> operator*(const T & left, const Vector2D<T> & right) noexcept {
  return right * left;
}

template <Vector2DComponent T>
constexpr T operator*(const Vector2D<T> & left, const Vector2D<T> & right) noexcept {
  return left.x * right.x + left.y * right.y;
}

template <Vector2DComponent T>
constexpr Vector2D<T> operator/(const Vector2D<T> & left, const T & right) noexcept {
  assert_message(right > 0 || right < 0, "right must be non-zero");

  constexpr T one{1};
  const T invScalar = one / right;

  return Vector2D(left.x * invScalar, left.y * invScalar);
}

template <Vector2DComponent T>
constexpr bool operator==(const Vector2D<T> & left, const Vector2D<T> & right) noexcept {
  if constexpr (floating_point<T>) {
    return left.isEqual(right);
  } else if constexpr (fixed_point<T>) {
    return left.x.rawValue() == right.x.rawValue() && left.y.rawValue() == right.y.rawValue();
  }
}

template <Vector2DComponent T>
constexpr bool operator!=(const Vector2D<T> & left, const Vector2D<T> & right) noexcept {
  return !(left == right);
}

template <Vector2DComponent T>
constexpr T cross(const Vector2D<T> & left, const Vector2D<T> & right) noexcept {
  return left.x * right.y - left.y * right.x;
}

} // namespace toy::math

#endif // INCLUDE_MATH_VECTOR2D_INL_
