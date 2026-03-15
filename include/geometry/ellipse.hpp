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
  \file   ellipse.hpp
  \brief  Declarations for \ref toy::geometry::Ellipse and \ref toy::geometry::EllipseComponent.
*/

#ifndef INCLUDE_GEOMETRY_ELLIPSE_HPP_
#define INCLUDE_GEOMETRY_ELLIPSE_HPP_

namespace toy::geometry {

/*!
  \concept EllipseComponent
  \brief Concept satisfied when \a T is a component type allowed as \ref toy::geometry::Ellipse template parameter.

  Use to constrain the component type of \ref toy::geometry::Ellipse to floating-point or fixed-point types only.

  \section requirements Requirements

  A type \a T satisfies EllipseComponent if and only if at least one of the following holds:
  - \a T satisfies \ref toy::math::floating_point.
  - \a T satisfies \ref toy::math::fixed_point.

  \sa toy::geometry::Ellipse
*/
template <typename T>
concept EllipseComponent = math::floating_point<T> || math::fixed_point<T>;

/*!
  \class Ellipse
  \brief Axis-aligned ellipse in 2D with center and semi-axes.

  Represents an ellipse by center (\ref toy::math::Vector2D) and \a radiuses (semi-axes: \a radiuses.x along x,
  \a radiuses.y along y). Default-constructed ellipse is in reset state; use reset() to clear or construct with (center,
  radiuses) for a valid ellipse. operator== compares center and radiuses (tolerance-based for float, exact for
  fixed-point).

  \tparam T Component type; must satisfy \ref toy::geometry::EllipseComponent.

  \section features Key Features

  - **Center and semi-axes**: \a center and \a radiuses are public; \a radiuses.x and \a radiuses.y must be positive
    for a valid ellipse.
  - **Reset state**: reset() sets center and radiuses to zero; isReset() detects it.
  - **Validity**: isValid() is \c true when both \a radiuses.x and \a radiuses.y are \c > \c 0; constructor asserts in
    debug.
  - **Containment**: isContain() tests whether a point lies inside or on the ellipse using the normalized equation
    \f$ (x/a)^2 + (y/b)^2 \le 1 \f$.
  - **Constexpr**: All operations are constexpr and noexcept.

  \section usage Usage Example

  \code
  #include "geometry.hpp"

  toy::geometry::Ellipse<float> e(toy::math::Vector2D(0.0f, 0.0f), toy::math::Vector2D(10.0f, 5.0f));
  auto a = e.area();
  bool inside = e.isContain(toy::math::Vector2D(3.0f, 2.0f));
  \endcode

  \section performance Performance Characteristics

  Operations are O(1). No allocations.

  \section safety Safety Guarantees

  - **Preconditions**: Constructor (center, radiuses) requires both \a radiuses.x and \a radiuses.y \c > \c 0 (assert in
    debug).
  - **Exception safety**: All operations are noexcept.

  \sa EllipseComponent
  \sa toy::math::Vector2D
*/
template <EllipseComponent T>
class Ellipse {
public:
  /// Center of the ellipse.
  math::Vector2D<T> center;
  /// Semi-axes of the ellipse (\a radiuses.x along x, \a radiuses.y along y).
  math::Vector2D<T> radiuses;

public:
  /// Default constructor.
  constexpr Ellipse() noexcept = default;

  /*!
    \brief Constructs an ellipse with the given center and semi-axes.

    \param center   Center of the ellipse.
    \param radiuses Semi-axes; both components must be \c > \c 0 (asserted in debug).

    \pre \a radiuses.x \c > \c 0 and \a radiuses.y \c > \c 0.
  */
  constexpr Ellipse(const math::Vector2D<T> & center, const math::Vector2D<T> & radiuses) noexcept;

  /*!
    \brief Returns the area (π × radiuses.x × radiuses.y).

    \return Area of the ellipse.
  */
  [[nodiscard]] constexpr T area() const noexcept;

  /*!
    \brief Puts the ellipse in reset state (center zero, radiuses zero).

    \post isReset() is \c true.
  */
  constexpr void reset() noexcept;

  /*!
    \brief Returns \c true if the ellipse is in reset state (center zero, radiuses zero).

    \return \c true when \a center and \a radiuses are both zero.
  */
  [[nodiscard]] constexpr bool isReset() const noexcept;

  /*!
    \brief Returns \c true if the ellipse has valid state (both semi-axes positive).

    \return \c true when \a radiuses.x \c > \c 0 and \a radiuses.y \c > \c 0.
  */
  [[nodiscard]] constexpr bool isValid() const noexcept;

  /*!
    \brief Returns \c true if \a vector lies inside or on the ellipse.

    Uses the normalized ellipse equation: \f$ ((x-c_x)/a)^2 + ((y-c_y)/b)^2 \le 1 \f$ where \a center is
    \f$ (c_x, c_y) \f$ and \a radiuses is \f$ (a, b) \f$.

    \param vector Point to test.

    \return \c true when the point satisfies the ellipse inequality.
  */
  [[nodiscard]] constexpr bool isContain(const math::Vector2D<T> & vector) const noexcept;
};

/*!
  \brief Deduction guide: enables \c Ellipse(center, radiuses) without an explicit template argument when both arguments
         have the same \ref toy::geometry::EllipseComponent type.

  \tparam T Component type; must satisfy \ref toy::geometry::EllipseComponent.
*/
template <EllipseComponent T>
Ellipse(const math::Vector2D<T> &, const math::Vector2D<T> &) -> Ellipse<T>;

/*!
  \brief Equality of two ellipses: same center and radiuses (tolerance-based for float, exact for fixed-point).

  \tparam T Component type; must satisfy \ref toy::geometry::EllipseComponent.

  \param left  First ellipse.
  \param right Second ellipse.

  \return \c true if \a left and \a right have equal center and radiuses.

  \sa operator!=()
*/
template <EllipseComponent T>
[[nodiscard]] constexpr bool operator==(const Ellipse<T> & left, const Ellipse<T> & right) noexcept;

/*!
  \brief Inequality of two ellipses.

  \tparam T Component type; must satisfy \ref toy::geometry::EllipseComponent.

  \param left  First ellipse.
  \param right Second ellipse.

  \return \c true if \a left and \a right differ, i.e. \c !(\a left == \a right).

  \sa operator==()
*/
template <EllipseComponent T>
[[nodiscard]] constexpr bool operator!=(const Ellipse<T> & left, const Ellipse<T> & right) noexcept;

} // namespace toy::geometry

#endif // INCLUDE_GEOMETRY_ELLIPSE_HPP_
