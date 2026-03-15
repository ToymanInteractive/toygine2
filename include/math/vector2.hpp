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
  \file   vector2.hpp
  \brief  2D vector class for interactive game objects and physics calculations.
*/

#ifndef INCLUDE_MATH_VECTOR2_HPP_
#define INCLUDE_MATH_VECTOR2_HPP_

namespace toy::math {

/*!
  \concept Vector2Component
  \brief Concept satisfied when \a T is a component type allowed as \ref toy::math::Vector2 template parameter.

  Use to constrain the component type of \ref toy::math::Vector2 to floating-point or fixed-point types only.

  \section requirements Requirements

  A type \a T satisfies Vector2Component if and only if at least one of the following holds:
  - \a T satisfies \ref toy::math::floating_point.
  - \a T satisfies \ref toy::math::fixed_point.

  \sa toy::math::Vector2
*/
template <typename T>
concept Vector2Component = floating_point<T> || fixed_point<T>;

/*!
  \class Vector2
  \brief 2D vector with floating-point or fixed-point components for positions, movement, and physics.

  Represents a 2D vector with \a x and \a y components. Suited for world/screen coordinates, velocity, forces, and
  transformations where sub-pixel or fractional precision is needed. For integer pixel coordinates use
  \ref toy::math::Point.

  \section features Key Features

  - **Precision**: Component type \a T is \ref toy::math::Vector2Component (float, double, or fixed-point).
  - **Constexpr**: Most operations are constexpr.
  - **Exception safety**: All operations are noexcept.
  - **Small size**: Two components; stack-friendly.

  \section usage Usage Example

  \code
  #include "math.hpp"

  toy::math::Vector2 pos(100.5f, 200.3f);
  toy::math::Vector2 velocity(50.0f, -25.0f);
  auto deltaTime = 0.5f;
  pos += velocity * deltaTime;

  toy::math::Vector2 force(10.0f, 0.0f);
  auto mass = 2.0f;
  auto acceleration = force / mass;
  \endcode

  \section performance Performance Characteristics

  - **Construction, assignment, arithmetic, comparison**: O(1).
  - **Memory**: 2 × sizeof(\a T).

  \section safety Safety Guarantees

  - **Contracts**: Division by zero is asserted in debug.
  - **Exception safety**: noexcept; no exceptions thrown.

  \note For pixel-aligned UI coordinates use \ref toy::math::Point.

  \sa toy::math::Point
*/
template <Vector2Component T>
class Vector2 {
public:
  /// X component of the vector.
  T x;

  /// Y component of the vector.
  T y;

public:
  /// Default constructor.
  constexpr Vector2() noexcept;

  /*!
    \brief Constructs a vector with the given components.

    \param x X component.
    \param y Y component.
  */
  constexpr Vector2(const T & x, const T & y) noexcept;

  /*!
    \brief Constructs a vector from an array.

    \param values Pointer to at least two elements; \a values[0] is \a x, \a values[1] is \a y.

    \pre \a values != nullptr; array has at least two elements.

    \post \a x == \a values[0], \a y == \a values[1].
  */
  explicit constexpr Vector2(const T * values) noexcept;

  /*!
    \brief Pointer to the component array (\a x, then \a y).

    \return Pointer to \a x; \a y follows contiguously.
  */
  [[nodiscard]] constexpr T * c_arr() noexcept;

  /*!
    \brief Read-only pointer to the component array (\a x, then \a y).

    \return Pointer to \a x; \a y follows contiguously.
  */
  [[nodiscard]] constexpr const T * c_arr() const noexcept;

  /*!
    \brief Adds \a vector to this vector.

    \param vector Vector to add.

    \return Reference to \c *this.

    \post Components are the sum of the previous values and \a vector.
  */
  constexpr Vector2 & operator+=(const Vector2 & vector) noexcept;

  /*!
    \brief Subtracts \a vector from this vector.

    \param vector Vector to subtract.

    \return Reference to \c *this.

    \post Components are the difference of the previous values and \a vector.
  */
  constexpr Vector2 & operator-=(const Vector2 & vector) noexcept;

  /*!
    \brief Multiplies both components by \a scalar.

    \param scalar Scale factor.

    \return Reference to \c *this.
  */
  constexpr Vector2 & operator*=(const T & scalar) noexcept;

  /*!
    \brief Divides both components by \a scalar.

    \param scalar Divisor.

    \return Reference to \c *this.

    \pre \a scalar != \c 0.
  */
  constexpr Vector2 & operator/=(const T & scalar) noexcept;

  /*!
    \brief Squared Euclidean length (x² + y²).

    \return Squared magnitude; avoids sqrt for comparisons.
  */
  [[nodiscard]] constexpr T sqrMagnitude() const noexcept;

  /*!
    \brief Sets \a x and \a y to zero.

    \post Vector is at the origin.
  */
  constexpr void setZero() noexcept;

  /*!
    \brief Returns whether both components are zero.

    \return \c true if \a x and \a y are zero, \c false otherwise.
  */
  [[nodiscard]] constexpr bool isZero() const noexcept;

  /*!
    \brief Equality within absolute and relative epsilon.

    \param vector     Other vector.
    \param absEpsilon Maximum absolute difference per component (default: 8× \c numeric_limits<T>::epsilon()).
    \param relEpsilon Maximum relative difference (default: 64× \c numeric_limits<T>::epsilon()).

    \return \c true if both components are equal within tolerances, \c false otherwise.

    \pre \a absEpsilon ≥ \c 0 and \a relEpsilon ≥ \c 0.
  */
  [[nodiscard]] constexpr bool isEqual(const Vector2 & vector, T absEpsilon = 8 * numeric_limits<T>::epsilon(),
                                       T relEpsilon = 64 * numeric_limits<T>::epsilon()) const noexcept;
};

/*!
  \brief Deduction guide: enables \c Vector2(x, y) without an explicit template argument when both arguments have the
         same \ref toy::math::Vector2Component type.

  \tparam T Component type; must satisfy \ref toy::math::Vector2Component.
*/
template <Vector2Component T>
Vector2(const T &, const T &) -> Vector2<T>;

/*!
  \brief Unary minus: negated components.

  \param vector Operand.

  \return \ref toy::math::Vector2 with (-vector.x, -vector.y).
*/
template <Vector2Component T>
[[nodiscard]] constexpr Vector2<T> operator-(const Vector2<T> & vector) noexcept;

/*!
  \brief Addition of two vectors.

  \param left  First operand.
  \param right Second operand.

  \return \ref toy::math::Vector2 with (left.x + right.x, left.y + right.y).
*/
template <Vector2Component T>
[[nodiscard]] constexpr Vector2<T> operator+(const Vector2<T> & left, const Vector2<T> & right) noexcept;

/*!
  \brief Subtraction of two vectors.

  \param left  First operand.
  \param right Second operand.

  \return \ref toy::math::Vector2 with (left.x - right.x, left.y - right.y).
*/
template <Vector2Component T>
[[nodiscard]] constexpr Vector2<T> operator-(const Vector2<T> & left, const Vector2<T> & right) noexcept;

/*!
  \brief Vector scaled by scalar.

  \tparam T Component type satisfy \ref toy::math::Vector2Component.

  \param left  Vector operand.
  \param right Scale factor.

  \return \ref toy::math::Vector2 with components multiplied by \a scalar.
*/
template <Vector2Component T>
[[nodiscard]] constexpr Vector2<T> operator*(const Vector2<T> & left, const T & right) noexcept;

/*!
  \brief Vector scaled by scalar (scalar on the left).

  \tparam T Component type satisfy \ref toy::math::Vector2Component.

  \param left  Scale factor.
  \param right Vector operand.

  \return \ref toy::math::Vector2 with components multiplied by \a scalar.
*/
template <Vector2Component T>
[[nodiscard]] constexpr Vector2<T> operator*(const T & left, const Vector2<T> & right) noexcept;

/*!
  \brief Dot product of two vectors.

  \param left  First operand.
  \param right Second operand.

  \return \a left.x * \a right.x + \a left.y * \a right.y.
*/
template <Vector2Component T>
[[nodiscard]] constexpr T operator*(const Vector2<T> & left, const Vector2<T> & right) noexcept;

/*!
  \brief Vector divided by scalar.

  \tparam T Component type satisfy \ref toy::math::Vector2Component.

  \param left  Vector operand.
  \param right Divisor.

  \return \ref toy::math::Vector2 with components divided by \a scalar.

  \pre \a right != \c 0.
*/
template <Vector2Component T>
[[nodiscard]] constexpr Vector2<T> operator/(const Vector2<T> & left, const T & right) noexcept;

/*!
  \brief Exact equality of two vectors.

  For floating-point \a T, delegates to isEqual() with default epsilons. For fixed-point \a T, compares raw storage of
  both components.

  \param left  First operand.
  \param right Second operand.

  \return For floating-point \a T: \c true if isEqual(\a right) with default tolerances. For fixed-point \a T: \c true
          if \a left and \a right have equal raw component values.

  \sa isEqual() for custom tolerances (floating-point).
*/
template <Vector2Component T>
[[nodiscard]] constexpr bool operator==(const Vector2<T> & left, const Vector2<T> & right) noexcept;

/*!
  \brief Inequality of two vectors.

  \param left  First operand.
  \param right Second operand.

  \return \c true if \c !(left == right).
*/
template <Vector2Component T>
[[nodiscard]] constexpr bool operator!=(const Vector2<T> & left, const Vector2<T> & right) noexcept;

/*!
  \brief 2D cross product (scalar result).

  \param left  First operand.
  \param right Second operand.

  \return \a left.x * \a right.y - \a left.y * \a right.x.
*/
template <Vector2Component T>
[[nodiscard]] constexpr T cross(const Vector2<T> & left, const Vector2<T> & right) noexcept;

} // namespace toy::math

#endif // INCLUDE_MATH_VECTOR2_HPP_
