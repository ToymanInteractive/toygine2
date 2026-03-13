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
  \file   point.hpp
  \brief  2D integer point for UI and input coordinates.
*/

#ifndef INCLUDE_MATH_POINT_HPP_
#define INCLUDE_MATH_POINT_HPP_

namespace toy::math {

/*!
  \concept PointScalar
  \brief Concept satisfied when \a T is a signed integral, floating-point, or fixed-point type.

  Used to constrain scalar arguments for \ref toy::math::Point multiplication and division operators. Accepts signed
  integral types only (e.g. \c int32_t; unsigned integral is excluded), standard floating-point types ( \c float,
  \c double, \c long \c double), and fixed-point types.

  \section requirements Requirements

  - \a T satisfies \ref toy::math::signed_integral, or
  - \a T satisfies \ref toy::math::floating_point, or
  - \a T satisfies \ref toy::math::fixed_point.

  \sa toy::math::Point, toy::math::Vector2D
*/
template <typename T>
concept PointScalar = signed_integral<T> || floating_point<T> || fixed_point<T>;

/*!
  \class Point
  \brief 2D integer point for UI and input coordinates.

  Lightweight value type with \c x and \c y. Suitable for pixel positions, mouse coordinates, and layout.
  Prefer \ref toy::math::Vector2D for floating-point or world coordinates.

  \section features Key Features

  - **Constexpr support**: Most operations are constexpr.
  - **Exception safety**: All operations are noexcept.
  - **UI-oriented**: Integer coordinates for pixel-perfect layout.
  - **Type safety**: Distinct type; coordinates are named \c x and \c y.

  \section usage Usage Example

  \code
  #include "math.hpp"

  toy::math::Point mousePos(100, 200);
  toy::math::Point windowPos(50, 75);
  toy::math::Point buttonPos(10, 10);
  buttonPos += Point(5, 5);

  constexpr auto origin = toy::math::Point(0, 0);
  constexpr auto center = toy::math::Point(640, 480);
  \endcode

  \section performance Performance Characteristics

  - **Construction, assignment, arithmetic, comparison**: O(1).
  - **Memory**: 8 bytes; stack-friendly.

  \section safety Safety Guarantees

  - **Contracts**: Division by zero and null \a values are asserted in debug.
  - **Exception safety**: All operations are noexcept.

  \note For approximate equality use isEqual() with a tolerance.

  \sa toy::math::Vector2D
*/
class Point {
public:
  /// X coordinate.
  int32_t x = 0;

  /// Y coordinate.
  int32_t y = 0;

  /*!
    \brief Default constructor.

    \post \a x and \a y are zero.
  */
  constexpr Point() noexcept = default;

  /*!
    \brief Constructs a point with the given coordinates.

    \param x X coordinate.
    \param y Y coordinate.
  */
  constexpr Point(const int32_t & x, const int32_t & y) noexcept;

  /*!
    \brief Constructs a point from an array.

    \param values Array of at least two elements; values[0] is \a x, values[1] is \a y.

    \pre \a values != nullptr; array has at least two elements.

    \post \a x == values[0], \a y == values[1].
  */
  explicit constexpr Point(const int32_t * values) noexcept;

  /*!
    \brief Pointer to the coordinate array (\a x, then \a y).

    \return Pointer to \a x; \a y follows contiguously.
  */
  [[nodiscard]] constexpr int32_t * c_arr() noexcept;

  /*!
    \brief Read-only pointer to the coordinate array (\a x, then \a y).

    \return Pointer to \a x; \a y follows contiguously.
  */
  [[nodiscard]] constexpr const int32_t * c_arr() const noexcept;

  /*!
    \brief Adds \a point to this point.

    \param point Point to add.

    \return Reference to \c *this.

    \post Coordinates are the sum of the previous values and \a point.
  */
  constexpr Point & operator+=(const Point & point) noexcept;

  /*!
    \brief Subtracts \a point from this point.

    \param point Point to subtract.

    \return Reference to \c *this.

    \post Coordinates are the difference of the previous values and \a point.
  */
  constexpr Point & operator-=(const Point & point) noexcept;

  /*!
    \brief Multiplies both coordinates by \a scalar and truncates to integer.

    \tparam T Type satisfying \ref toy::math::PointScalar (signed integral, floating-point, or fixed-point).

    \param scalar Scale factor.

    \return Reference to \c *this.
  */
  template <PointScalar T>
  constexpr Point & operator*=(const T & scalar) noexcept;

  /*!
    \brief Divides both coordinates by \a scalar and truncates to integer.

    \tparam T Type satisfying \ref toy::math::PointScalar (signed integral, floating-point, or fixed-point).

    \param scalar Divisor.

    \return Reference to \c *this.

    \pre \a scalar != \c 0.
  */
  template <PointScalar T>
  constexpr Point & operator/=(const T & scalar) noexcept;

  /*!
    \brief Squared Euclidean length (x² + y²).

    \return Squared magnitude; avoids sqrt for comparisons.
  */
  [[nodiscard]] constexpr int32_t sqrMagnitude() const noexcept;

  /*!
    \brief Sets \a x and \a y to zero.

    \post Point is at the origin.
  */
  constexpr void setZero() noexcept;

  /*!
    \brief Returns whether both coordinates are zero.

    \return \c true if \a x == \c 0 and \a y == \c 0, \c false otherwise.
  */
  [[nodiscard]] constexpr bool isZero() const noexcept;

  /*!
    \brief Equality within \a epsilon.

    \param point   Other point.
    \param epsilon Maximum allowed difference per coordinate (default: \c 0).

    \return \c true if |x − point.x| ≤ \a epsilon and |y − point.y| ≤ \a epsilon.

    \pre \a epsilon ≥ \c 0.
  */
  [[nodiscard]] constexpr bool isEqual(const Point & point, int32_t epsilon = 0) const noexcept;
};

/*!
  \brief Unary minus: negated coordinates.

  \param point Operand.

  \return \ref toy::math::Point with (-point.x, -point.y).
*/
[[nodiscard]] constexpr Point operator-(const Point & point) noexcept;

/*!
  \brief Addition of two points.

  \param left  First operand.
  \param right Second operand.

  \return \ref toy::math::Point with (left.x + right.x, left.y + right.y).
*/
[[nodiscard]] constexpr Point operator+(const Point & left, const Point & right) noexcept;

/*!
  \brief Subtraction of two points.

  \param left  First operand.
  \param right Second operand.

  \return \ref toy::math::Point with (left.x - right.x, left.y - right.y).
*/
[[nodiscard]] constexpr Point operator-(const Point & left, const Point & right) noexcept;

/*!
  \brief Point scaled by scalar and truncated to integer.

  \tparam T Type satisfying \ref toy::math::PointScalar (signed integral, floating-point, or fixed-point).

  \param point  Operand.
  \param scalar Scale factor.

  \return \ref toy::math::Point with coordinates multiplied by \a scalar then cast to \c int32_t.
*/
template <PointScalar T>
[[nodiscard]] constexpr Point operator*(const Point & point, const T & scalar) noexcept;

/*!
  \brief Point scaled by scalar (scalar on the left).

  \tparam T Type satisfying \ref toy::math::PointScalar (signed integral, floating-point, or fixed-point).

  \param scalar Scale factor.
  \param point  Operand.

  \return \ref toy::math::Point with coordinates multiplied by \a scalar then cast to \c int32_t.
*/
template <PointScalar T>
[[nodiscard]] constexpr Point operator*(const T & scalar, const Point & point) noexcept;

/*!
  \brief Point divided by scalar and truncated to integer.

  \tparam T Type satisfying \ref toy::math::PointScalar (signed integral, floating-point, or fixed-point).

  \param point  Operand.
  \param scalar Divisor.

  \return \ref toy::math::Point with coordinates divided by \a scalar then cast to \c int32_t.

  \pre \a scalar != \c 0.
*/
template <PointScalar T>
[[nodiscard]] constexpr Point operator/(const Point & point, const T & scalar) noexcept;

/*!
  \brief Exact equality of two points.

  \param left  First operand.
  \param right Second operand.

  \return \c true if left.x == right.x and left.y == right.y, \c false otherwise.

  \sa isEqual() for tolerance-based comparison.
*/
[[nodiscard]] constexpr bool operator==(const Point & left, const Point & right) noexcept;

} // namespace toy::math

#endif // INCLUDE_MATH_POINT_HPP_
