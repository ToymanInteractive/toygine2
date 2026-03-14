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
  \file   vector2d.hpp
  \brief  2D vector class for interactive game objects and physics calculations.
*/

#ifndef INCLUDE_MATH_VECTOR2D_HPP_
#define INCLUDE_MATH_VECTOR2D_HPP_

namespace toy::math {

/*!
  \concept Vector2DComponent
  \brief Concept satisfied when \a T is a component type allowed as \ref toy::math::Vector2D template parameter.

  Use to constrain the component type of \ref toy::math::Vector2D to floating-point or fixed-point types only.

  \section requirements Requirements

  A type \a T satisfies Vector2DComponent if and only if at least one of the following holds:
  - \a T satisfies \ref toy::math::floating_point.
  - \a T satisfies \ref toy::math::fixed_point.

  \sa toy::math::Vector2D
*/
template <typename T>
concept Vector2DComponent = floating_point<T> || fixed_point<T>;

/*!
  \class Vector2D
  \brief 2D vector with floating-point or fixed-point components for positions, movement, and physics.

  Represents a 2D vector with \a x and \a y components. Suited for world/screen coordinates, velocity, forces, and
  transformations where sub-pixel or fractional precision is needed. For integer pixel coordinates use
  \ref toy::math::Point.

  \section features Key Features

  - **Precision**: Component type \a T is \ref toy::math::Vector2DComponent (float, double, or fixed-point).
  - **Constexpr**: Most operations are constexpr.
  - **Exception safety**: All operations are noexcept.
  - **Small size**: Two components; stack-friendly.

  \section usage Usage Example

  \code
  #include "math.hpp"

  toy::math::Vector2D pos(100.5f, 200.3f);
  toy::math::Vector2D velocity(50.0f, -25.0f);
  auto deltaTime = 0.5f;
  pos += velocity * deltaTime;

  toy::math::Vector2D force(10.0f, 0.0f);
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
template <Vector2DComponent T>
class Vector2D {
public:
  /// X component of the vector.
  T x{constants::zero_v<T>};

  /// Y component of the vector.
  T y{constants::zero_v<T>};

  /*!
    \brief Default constructor.

    \post \a x and \a y are zero.
  */
  constexpr Vector2D() noexcept = default;

  /*!
    \brief Constructs a vector with the given components.

    \param x X component.
    \param y Y component.
  */
  constexpr Vector2D(const T & x, const T & y) noexcept;

  /*!
    \brief Constructs a vector from an array.

    \param values Pointer to at least two elements; \a values[0] is \a x, \a values[1] is \a y.

    \pre \a values != nullptr; array has at least two elements.

    \post \a x == \a values[0], \a y == \a values[1].
  */
  explicit constexpr Vector2D(const T * values) noexcept;

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
  constexpr Vector2D & operator+=(const Vector2D & vector) noexcept;

  /*!
    \brief Subtracts \a vector from this vector.

    \param vector Vector to subtract.

    \return Reference to \c *this.

    \post Components are the difference of the previous values and \a vector.
  */
  constexpr Vector2D & operator-=(const Vector2D & vector) noexcept;

  /*!
    \brief Multiplies both components by \a scalar.

    \param scalar Scale factor.

    \return Reference to \c *this.
  */
  constexpr Vector2D & operator*=(const T & scalar) noexcept;

  /*!
    \brief Divides both components by \a scalar.

    \param scalar Divisor.

    \return Reference to \c *this.

    \pre \a scalar != \c 0.
  */
  constexpr Vector2D & operator/=(const T & scalar) noexcept;

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
  [[nodiscard]] bool isEqual(const Vector2D & vector, T absEpsilon = 8 * numeric_limits<T>::epsilon(),
                             T relEpsilon = 64 * numeric_limits<T>::epsilon()) const noexcept;
};

/*!
  \brief Deduction guide: enables \c Vector2D(x, y) without an explicit template argument when both arguments have the
         same \ref toy::math::Vector2DComponent type.

  \tparam T Component type; must satisfy \ref toy::math::Vector2DComponent.
*/
template <Vector2DComponent T>
Vector2D(const T &, const T &) -> Vector2D<T>;

/*!
  \brief Unary minus: negated components.

  \param vector Operand.

  \return \ref toy::math::Vector2D with (-vector.x, -vector.y).
*/
template <Vector2DComponent T>
[[nodiscard]] constexpr Vector2D<T> operator-(const Vector2D<T> & vector) noexcept;

/*!
  \brief Addition of two vectors.

  \param left  First operand.
  \param right Second operand.

  \return \ref toy::math::Vector2D with (left.x + right.x, left.y + right.y).
*/
template <Vector2DComponent T>
[[nodiscard]] constexpr Vector2D<T> operator+(const Vector2D<T> & left, const Vector2D<T> & right) noexcept;

/*!
  \brief Subtraction of two vectors.

  \param left  First operand.
  \param right Second operand.

  \return \ref toy::math::Vector2D with (left.x - right.x, left.y - right.y).
*/
template <Vector2DComponent T>
[[nodiscard]] constexpr Vector2D<T> operator-(const Vector2D<T> & left, const Vector2D<T> & right) noexcept;

/*!
  \brief Vector scaled by scalar.

  \tparam T Component type satisfy \ref toy::math::Vector2DComponent.

  \param left  Vector operand.
  \param right Scale factor.

  \return \ref toy::math::Vector2D with components multiplied by \a scalar.
*/
template <Vector2DComponent T>
[[nodiscard]] constexpr Vector2D<T> operator*(const Vector2D<T> & left, const T & right) noexcept;

/*!
  \brief Vector scaled by scalar (scalar on the left).

  \tparam T Component type satisfy \ref toy::math::Vector2DComponent.

  \param left  Scale factor.
  \param right Vector operand.

  \return \ref toy::math::Vector2D with components multiplied by \a scalar.
*/
template <Vector2DComponent T>
[[nodiscard]] constexpr Vector2D<T> operator*(const T & left, const Vector2D<T> & right) noexcept;

/*!
  \brief Vector divided by scalar.

  \tparam T Component type satisfy \ref toy::math::Vector2DComponent.

  \param left  Vector operand.
  \param right Divisor.

  \return \ref toy::math::Vector2D with components divided by \a scalar.

  \pre \a right != \c 0.
*/
template <Vector2DComponent T>
[[nodiscard]] constexpr Vector2D<T> operator/(const Vector2D<T> & left, const T & right) noexcept;

/*!
  \brief Exact equality of two vectors.

  \param left  First operand.
  \param right Second operand.

  \return \c true if components are equal within default tolerances, \c false otherwise.

  \sa isEqual() for custom tolerances.
*/
template <Vector2DComponent T>
[[nodiscard]] constexpr bool operator==(const Vector2D<T> & left, const Vector2D<T> & right) noexcept;

} // namespace toy::math

#endif // INCLUDE_MATH_VECTOR2D_HPP_
