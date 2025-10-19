//
// Copyright (c) 2025 by Toyman Interactive
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
  \file   point.inl
  \brief  Inline implementations for 2D integer point class.
*/

#ifndef INCLUDE_MATH_POINT_INL_
#define INCLUDE_MATH_POINT_INL_

namespace toy::math {

static_assert(sizeof(Point) == 2 * sizeof(Point::value_type), "Point must be tightly packed");
static_assert(offsetof(Point, y) == sizeof(Point::value_type), "y must follow x contiguously");

constexpr Point::Point() noexcept
  : x()
  , y() {}

constexpr Point::Point(const_reference x, const_reference y) noexcept
  : x(x)
  , y(y) {}

constexpr Point::Point(const_pointer values) noexcept
  : x()
  , y() {
  assert_message(values != nullptr, "values cannot be null");

  x = values[0];
  y = values[1];
}

constexpr Point::pointer Point::c_arr() noexcept {
  return &x;
}

constexpr Point::const_pointer Point::c_arr() const noexcept {
  return &x;
}

constexpr Point & Point::operator+=(const Point & point) noexcept {
  x += point.x;
  y += point.y;

  return *this;
}

constexpr Point & Point::operator-=(const Point & point) noexcept {
  x -= point.x;
  y -= point.y;

  return *this;
}

constexpr Point & Point::operator*=(value_type scalar) noexcept {
  x *= scalar;
  y *= scalar;

  return *this;
}

constexpr Point & Point::operator*=(real_t scalar) noexcept {
  x = static_cast<value_type>(x * scalar);
  y = static_cast<value_type>(y * scalar);

  return *this;
}

constexpr Point & Point::operator/=(value_type scalar) noexcept {
  assert_message(scalar != 0, "scalar must be non-zero");

  x /= scalar;
  y /= scalar;

  return *this;
}

constexpr Point & Point::operator/=(real_t scalar) noexcept {
  assert_message(scalar > 0 || scalar < 0, "scalar must be non-zero");

  x = static_cast<value_type>(x / scalar);
  y = static_cast<value_type>(y / scalar);

  return *this;
}

constexpr Point::value_type Point::sqrMagnitude() const noexcept {
  return x * x + y * y;
}

constexpr void Point::setZero() noexcept {
  x = y = 0;
}

constexpr bool Point::isZero() const noexcept {
  return x == 0 && y == 0;
}

inline bool Point::isEqual(const Point & point, value_type tolerance) const noexcept {
  assert_message(tolerance >= 0, "tolerance must be non-negative");

  return std::abs(x - point.x) <= tolerance && std::abs(y - point.y) <= tolerance;
}

constexpr Point operator-(const Point & point) noexcept {
  return Point(-point.x, -point.y);
}

constexpr Point operator+(const Point & left, const Point & right) noexcept {
  return Point(left.x + right.x, left.y + right.y);
}

constexpr Point operator-(const Point & left, const Point & right) noexcept {
  return Point(left.x - right.x, left.y - right.y);
}

constexpr Point operator*(const Point & left, Point::value_type right) noexcept {
  return Point(left.x * right, left.y * right);
}

constexpr Point operator*(Point::value_type left, const Point & right) noexcept {
  return right * left;
}

constexpr Point operator*(const Point & left, real_t right) noexcept {
  return Point(left.x * right, left.y * right);
}

constexpr Point operator*(real_t left, const Point & right) noexcept {
  return right * left;
}

constexpr Point operator/(const Point & left, Point::value_type right) noexcept {
  assert_message(right != 0, "right must be non-zero");

  return Point(left.x / right, left.y / right);
}

constexpr Point operator/(const Point & left, real_t right) noexcept {
  assert_message(right > 0 || right < 0, "right must be non-zero");

  return Point(left.x / right, left.y / right);
}

constexpr bool operator==(const Point & left, const Point & right) noexcept {
  return left.x == right.x && left.y == right.y;
}

} // namespace toy::math

#endif // INCLUDE_MATH_POINT_INL_
