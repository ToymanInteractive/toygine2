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
  \file   vector2.inl
  \brief  Inline implementations for \ref toy::math::Vector2.
*/

#ifndef INCLUDE_MATH_VECTOR2_INL_
#define INCLUDE_MATH_VECTOR2_INL_

namespace toy::math {

template <Vector2Component T>
constexpr Vector2<T>::Vector2() noexcept
  : x()
  , y() {
#ifdef _DEBUG
  if constexpr (floating_point<T>) {
    x = numeric_limits<T>::signaling_NaN();
    y = numeric_limits<T>::signaling_NaN();
  }
#endif
}

template <Vector2Component T>
constexpr Vector2<T>::Vector2(const T & _x, const T & _y) noexcept
  : x(_x)
  , y(_y) {}

template <Vector2Component T>
constexpr Vector2<T>::Vector2(const T * values) noexcept
  : x()
  , y() {
  assert_message(values != nullptr, "values cannot be null");

  x = values[0];
  y = values[1];
}

template <Vector2Component T>
constexpr T * Vector2<T>::c_arr() noexcept {
  return &x;
}

template <Vector2Component T>
constexpr const T * Vector2<T>::c_arr() const noexcept {
  return &x;
}

template <Vector2Component T>
constexpr Vector2<T> & Vector2<T>::operator+=(const Vector2<T> & vector) noexcept {
  x += vector.x;
  y += vector.y;

  return *this;
}

template <Vector2Component T>
constexpr Vector2<T> & Vector2<T>::operator-=(const Vector2<T> & vector) noexcept {
  x -= vector.x;
  y -= vector.y;

  return *this;
}

template <Vector2Component T>
constexpr Vector2<T> & Vector2<T>::operator*=(const T & scalar) noexcept {
  x *= scalar;
  y *= scalar;

  return *this;
}

template <Vector2Component T>
constexpr Vector2<T> & Vector2<T>::operator/=(const T & scalar) noexcept {
  assert_message(scalar > 0 || scalar < 0, "scalar must be non-zero");

  x /= scalar;
  y /= scalar;

  return *this;
}

template <Vector2Component T>
constexpr T Vector2<T>::sqrMagnitude() const noexcept {
  return x * x + y * y;
}

template <Vector2Component T>
constexpr void Vector2<T>::setZero() noexcept {
  x = y = T{0};
}

template <Vector2Component T>
constexpr bool Vector2<T>::isZero() const noexcept {
  if constexpr (floating_point<T>) {
    return math::isEqual(x, T{0}) && math::isEqual(y, T{0});
  } else if constexpr (fixed_point<T>) {
    return x.rawValue() == 0 && y.rawValue() == 0;
  } else {
    static_assert(floating_point<T> || fixed_point<T>, "unsupported component type");
  }
}

template <Vector2Component T>
constexpr bool Vector2<T>::isEqual(const Vector2<T> & vector, T absEpsilon, T relEpsilon) const noexcept {
  assert_message(absEpsilon >= T{0}, "absEpsilon must be non-negative");
  assert_message(relEpsilon >= T{0}, "relEpsilon must be non-negative");

  return math::isEqual(x, vector.x, absEpsilon, relEpsilon) && math::isEqual(y, vector.y, absEpsilon, relEpsilon);
}

template <Vector2Component T>
constexpr Vector2<T> operator-(const Vector2<T> & vector) noexcept {
  return Vector2(-vector.x, -vector.y);
}

template <Vector2Component T>
constexpr Vector2<T> operator+(const Vector2<T> & left, const Vector2<T> & right) noexcept {
  return Vector2(left.x + right.x, left.y + right.y);
}

template <Vector2Component T>
constexpr Vector2<T> operator-(const Vector2<T> & left, const Vector2<T> & right) noexcept {
  return Vector2(left.x - right.x, left.y - right.y);
}

template <Vector2Component T>
constexpr Vector2<T> operator*(const Vector2<T> & left, const T & right) noexcept {
  return Vector2(left.x * right, left.y * right);
}

template <Vector2Component T>
constexpr Vector2<T> operator*(const T & left, const Vector2<T> & right) noexcept {
  return right * left;
}

template <Vector2Component T>
constexpr T operator*(const Vector2<T> & left, const Vector2<T> & right) noexcept {
  return left.x * right.x + left.y * right.y;
}

template <Vector2Component T>
constexpr Vector2<T> operator/(const Vector2<T> & left, const T & right) noexcept {
  assert_message(right > 0 || right < 0, "right must be non-zero");

  return Vector2<T>(left.x / right, left.y / right);
}

template <Vector2Component T>
constexpr bool operator==(const Vector2<T> & left, const Vector2<T> & right) noexcept {
  if constexpr (floating_point<T>) {
    return left.isEqual(right);
  } else if constexpr (fixed_point<T>) {
    return left.x == right.x && left.y == right.y;
  } else {
    static_assert(floating_point<T> || fixed_point<T>, "unsupported component type");
  }
}

template <Vector2Component T>
constexpr bool operator!=(const Vector2<T> & left, const Vector2<T> & right) noexcept {
  return !(left == right);
}

template <Vector2Component T>
constexpr T cross(const Vector2<T> & left, const Vector2<T> & right) noexcept {
  return left.x * right.y - left.y * right.x;
}

} // namespace toy::math

#endif // INCLUDE_MATH_VECTOR2_INL_
