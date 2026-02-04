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
  \brief  Inline implementations for 2D floating-point vector class.
*/

#ifndef INCLUDE_MATH_VECTOR2D_INL_
#define INCLUDE_MATH_VECTOR2D_INL_

namespace toy::math {

static_assert(sizeof(Vector2D) == 2 * sizeof(real_t), "Vector2D must be tightly packed");
static_assert(offsetof(Vector2D, y) == sizeof(real_t), "y must follow x contiguously");

constexpr Vector2D::Vector2D() noexcept
  : x()
  , y() {}

constexpr Vector2D::Vector2D(const real_t & x_, const real_t & y_) noexcept
  : x(x_)
  , y(y_) {}

constexpr Vector2D::Vector2D(const real_t * values) noexcept
  : x()
  , y() {
  assert_message(values != nullptr, "values cannot be null");

  x = values[0];
  y = values[1];
}

constexpr real_t * Vector2D::c_arr() noexcept {
  return &x;
}

constexpr const real_t * Vector2D::c_arr() const noexcept {
  return &x;
}

constexpr Vector2D & Vector2D::operator+=(const Vector2D & vector) noexcept {
  x += vector.x;
  y += vector.y;

  return *this;
}

constexpr Vector2D & Vector2D::operator-=(const Vector2D & vector) noexcept {
  x -= vector.x;
  y -= vector.y;

  return *this;
}

constexpr Vector2D & Vector2D::operator*=(real_t scalar) noexcept {
  x *= scalar;
  y *= scalar;

  return *this;
}

constexpr Vector2D & Vector2D::operator/=(real_t scalar) noexcept {
  assert_message(scalar > 0 || scalar < 0, "scalar must be non-zero");

  x /= scalar;
  y /= scalar;

  return *this;
}

constexpr real_t Vector2D::sqrMagnitude() const noexcept {
  return x * x + y * y;
}

constexpr void Vector2D::setZero() noexcept {
  x = y = 0;
}

constexpr bool Vector2D::isZero() const noexcept {
  return x == 0 && y == 0;
}

inline bool Vector2D::isEqual(const Vector2D & vector, real_t tolerance) const noexcept {
  assert_message(tolerance >= 0, "tolerance must be non-negative");

  return std::abs(x - vector.x) <= tolerance && std::abs(y - vector.y) <= tolerance;
}

constexpr Vector2D operator-(const Vector2D & vector) noexcept {
  return Vector2D(-vector.x, -vector.y);
}

constexpr Vector2D operator+(const Vector2D & left, const Vector2D & right) noexcept {
  return Vector2D(left.x + right.x, left.y + right.y);
}

constexpr Vector2D operator-(const Vector2D & left, const Vector2D & right) noexcept {
  return Vector2D(left.x - right.x, left.y - right.y);
}

constexpr Vector2D operator*(const Vector2D & left, real_t right) noexcept {
  return Vector2D(left.x * right, left.y * right);
}

constexpr Vector2D operator*(real_t left, const Vector2D & right) noexcept {
  return right * left;
}

constexpr Vector2D operator/(const Vector2D & left, real_t right) noexcept {
  assert_message(right > 0 || right < 0, "right must be non-zero");

  return Vector2D(left.x / right, left.y / right);
}

constexpr bool operator==(const Vector2D & left, const Vector2D & right) noexcept {
  return left.x == right.x && left.y == right.y;
}

} // namespace toy::math

#endif // INCLUDE_MATH_VECTOR2D_INL_
