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
  \file   circle.hpp
  \brief  Circle in 2D with center and radius.

  Defines \ref toy::geometry::Circle and \ref toy::geometry::CircleComponent.

  Included by \ref geometry.hpp; do not include this file directly.
*/

#ifndef INCLUDE_GEOMETRY_CIRCLE_HPP_
#define INCLUDE_GEOMETRY_CIRCLE_HPP_

namespace toy::geometry {

/*!
  \concept CircleComponent
  \brief Concept satisfied when \a T is a component type allowed as \ref toy::geometry::Circle template parameter.

  Use to constrain the component type of \ref toy::geometry::Circle to floating-point or fixed-point types only.

  \section requirements Requirements

  A type \a T satisfies CircleComponent if and only if at least one of the following holds:
  - \a T satisfies \ref toy::math::floating_point.
  - \a T satisfies \ref toy::math::fixed_point.

  \sa toy::geometry::Circle
*/
template <typename T>
concept CircleComponent = math::floating_point<T> || math::fixed_point<T>;

/*!
  \class Circle
  \brief Circle in 2D with center and radius.

  Represents a circle by center (\ref toy::math::Vector2) and radius. Default-constructed circle is in reset state; use
  reset() to clear or construct with (center, radius) for a valid circle. operator== compares center and radius
  (tolerance-based for float, exact for fixed-point).

  \tparam T Component type; must satisfy \ref toy::geometry::CircleComponent.

  \section features Key Features

  - **Center and radius**: \a center and \a radius are public; \a T is floating-point or fixed-point.
  - **Reset state**: reset() sets center to zero and radius to zero; isReset() detects it.
  - **Validity**: isValid() is \c true when \a radius > \c 0; constructor asserts \a radius > \c 0 in debug.
  - **Containment**: isContain() tests whether a point lies inside or on the circle (squared distance).
  - **Constexpr**: All operations are constexpr and noexcept.

  \section usage Usage Example

  \code
  #include "geometry.hpp"

  toy::geometry::Circle<float> c(toy::math::Vector2(0.0f, 0.0f), 10.0f);
  auto a = c.area();
  bool inside = c.isContain(toy::math::Vector2(5.0f, 0.0f));
  \endcode

  \section performance Performance Characteristics

  Operations are O(1). No allocations.

  \section safety Safety Guarantees

  - **Preconditions**: Constructor (center, radius) requires \a radius > \c 0 (assert in debug).
  - **Exception safety**: All operations are noexcept.

  \sa CircleComponent
  \sa toy::math::Vector2
*/
template <CircleComponent T>
class Circle {
public:
  /// Center of the circle.
  math::Vector2<T> center;
  /// Radius of the circle.
  T radius;

public:
  /// Default constructor.
  constexpr Circle() noexcept;

  /*!
    \brief Constructs a circle with the given center and radius.

    \param center Center of the circle.
    \param radius Radius; must be \c > \c 0 (asserted in debug).

    \pre \a radius > \c 0 (circle must be valid).
  */
  constexpr Circle(const math::Vector2<T> & center, const T & radius) noexcept;

  /*!
    \brief Returns the area (π * radius²).

    \return Area of the circle.
  */
  [[nodiscard]] constexpr T area() const noexcept;

  /*!
    \brief Puts the circle in reset state (center zero, radius zero).

    \post isReset() is \c true.
  */
  constexpr void reset() noexcept;

  /*!
    \brief Returns \c true if the circle is in reset state (center zero, radius zero).

    \return \c true when \a center is zero and \a radius is zero.
  */
  [[nodiscard]] constexpr bool isReset() const noexcept;

  /*!
    \brief Returns \c true if the circle has valid state (radius > 0).

    \return \c true when \a radius > \c 0.
  */
  [[nodiscard]] constexpr bool isValid() const noexcept;

  /*!
    \brief Returns \c true if \a vector lies inside or on the circle.

    \param vector Point to test.

    \return \c true when squared distance from \a vector to \a center is <= radius².
  */
  [[nodiscard]] constexpr bool isContain(const math::Vector2<T> & vector) const noexcept;
};

/*!
  \brief Deduction guide: enables \c Circle(center, radius) without an explicit template argument when both arguments
         have the same \ref toy::geometry::CircleComponent type.

  \tparam T Component type; must satisfy \ref toy::geometry::CircleComponent.
*/
template <CircleComponent T>
Circle(const math::Vector2<T> &, const T &) -> Circle<T>;

/*!
  \brief Equality of two circles: same center and radius (tolerance-based for float, exact for fixed-point).

  \tparam T Component type; must satisfy \ref toy::geometry::CircleComponent.

  \param left  First circle.
  \param right Second circle.

  \return \c true if \a left and \a right have equal center and radius.

  \sa operator!=()
*/
template <CircleComponent T>
[[nodiscard]] constexpr bool operator==(const Circle<T> & left, const Circle<T> & right) noexcept;

/*!
  \brief Inequality of two circles.

  \tparam T Component type; must satisfy \ref toy::geometry::CircleComponent.

  \param left  First circle.
  \param right Second circle.

  \return \c true if \a left and \a right differ, i.e. \c !(\a left == \a right).

  \sa operator==()
*/
template <CircleComponent T>
[[nodiscard]] constexpr bool operator!=(const Circle<T> & left, const Circle<T> & right) noexcept;

} // namespace toy::geometry

#endif // INCLUDE_GEOMETRY_CIRCLE_HPP_
