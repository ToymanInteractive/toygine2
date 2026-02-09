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
  \file   point.inl
  \brief  Inline implementations for 2D integer point class.
*/

#ifndef INCLUDE_MATH_POINT_INL_
#define INCLUDE_MATH_POINT_INL_

namespace toy::math {

static_assert(sizeof(Point) == 2 * sizeof(int32_t), "Point must be tightly packed");
static_assert(offsetof(Point, y) == sizeof(int32_t), "y must follow x contiguously");

constexpr Point::Point() noexcept
  : x()
  , y() {}

constexpr Point::Point(const int32_t & x_, const int32_t & y_) noexcept
  : x(x_)
  , y(y_) {}

constexpr Point::Point(const int32_t * values) noexcept
  : x()
  , y() {
  assert_message(values != nullptr, "values cannot be null");

  x = values[0];
  y = values[1];
}

constexpr int32_t * Point::c_arr() noexcept {
  return &x;
}

constexpr const int32_t * Point::c_arr() const noexcept {
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

constexpr Point & Point::operator*=(int32_t scalar) noexcept {
  x *= scalar;
  y *= scalar;

  return *this;
}

constexpr Point & Point::operator*=(real_t scalar) noexcept {
  x = static_cast<int32_t>(x * scalar);
  y = static_cast<int32_t>(y * scalar);

  return *this;
}

constexpr Point & Point::operator/=(int32_t scalar) noexcept {
  assert_message(scalar != 0, "scalar must be non-zero");

  x /= scalar;
  y /= scalar;

  return *this;
}

constexpr Point & Point::operator/=(real_t scalar) noexcept {
  assert_message(scalar > 0 || scalar < 0, "scalar must be non-zero");

  x = static_cast<int32_t>(x / scalar);
  y = static_cast<int32_t>(y / scalar);

  return *this;
}

constexpr int32_t Point::sqrMagnitude() const noexcept {
  return x * x + y * y;
}

constexpr void Point::setZero() noexcept {
  x = y = 0;
}

constexpr bool Point::isZero() const noexcept {
  return x == 0 && y == 0;
}

inline bool Point::isEqual(const Point & point, int32_t epsilon) const noexcept {
  assert_message(epsilon >= 0, "epsilon must be non-negative");

  return abs(x - point.x) <= epsilon && abs(y - point.y) <= epsilon;
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

constexpr Point operator*(const Point & point, int32_t scalar) noexcept {
  return Point(point.x * scalar, point.y * scalar);
}

constexpr Point operator*(int32_t scalar, const Point & point) noexcept {
  return point * scalar;
}

constexpr Point operator*(const Point & point, real_t scalar) noexcept {
  return Point(static_cast<int32_t>(point.x * scalar), static_cast<int32_t>(point.y * scalar));
}

constexpr Point operator*(real_t scalar, const Point & point) noexcept {
  return point * scalar;
}

constexpr Point operator/(const Point & point, int32_t scalar) noexcept {
  assert_message(scalar != 0, "scalar must be non-zero");

  return Point(point.x / scalar, point.y / scalar);
}

constexpr Point operator/(const Point & point, real_t scalar) noexcept {
  assert_message(scalar > 0 || scalar < 0, "scalar must be non-zero");

  return Point(static_cast<int32_t>(point.x / scalar), static_cast<int32_t>(point.y / scalar));
}

constexpr bool operator==(const Point & left, const Point & right) noexcept {
  return left.x == right.x && left.y == right.y;
}

} // namespace toy::math

#endif // INCLUDE_MATH_POINT_INL_
