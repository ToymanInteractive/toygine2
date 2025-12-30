//
// Copyright (c) 2025 Toyman Interactive
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
  \brief  2D integer point class for UI and input handling.
*/

#ifndef INCLUDE_MATH_POINT_HPP_
#define INCLUDE_MATH_POINT_HPP_

namespace toy::math {

/*!
  \class Point
  \brief 2D integer point class for UI and input handling.

  Point is a lightweight, high-performance 2D point class that uses integer coordinates. It is specifically designed for
  UI elements, mouse input handling, window positioning, and GUI layout calculations where pixel-perfect positioning is
  required.

  \section features Key Features

  - 🔧 **ConstExpr Support**: Most operations can be evaluated at compile time
  - 🛡️ **Exception Safety**: All operations are noexcept
  - 🎯 **UI Optimized**: Designed specifically for UI and input handling
  - 🧬 **Type Safety**: Strong typing with clear coordinate semantics

  \section usage Usage Example

  \code
  #include "math/point.hpp"

  // Mouse position handling
  toy::math::Point mousePos(100, 200);

  // Window positioning
  toy::math::Point windowPos(50, 75);

  // GUI element positioning
  toy::math::Point buttonPos(10, 10);
  buttonPos += Point(5, 5); // Move button

  // Use in constexpr context
  constexpr auto origin = toy::math::Point(0, 0);
  constexpr auto center = toy::math::Point(640, 480);
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(1) constant time
  - 📝 **Assignment**: O(1) constant time
  - 🔗 **Arithmetic Operations**: O(1) constant time
  - ⚖️ **Comparison Operations**: O(1) constant time
  - 💾 **Memory Usage**: 8 bytes
  - ⚡ **Cache Performance**: Excellent due to small size and stack allocation
  - 📋 **Copy Performance**: Fast due to simple integer copying
  - 🎯 **UI Operations**: Optimized for UI and input handling

  \section safety Safety Guarantees

  - 🛡️ **Contracts & Debug Checks**: Division by zero is asserted in debug
  - 🔒 **Bounds Safety**: All operations are bounds-safe
  - 📐 **Type Safety**: Strong typing prevents coordinate mixing
  - ⚠️ **Exception Safety**: All operations are noexcept, no exceptions thrown

  \note This class is specifically designed for UI and input handling.
  \note For sprite positioning and world coordinates, consider using Vector2D with floating-point precision.

  \sa toy::math::Vector2D
*/
class Point {
public:
  /// X coordinate.
  int32_t x;

  /// Y coordinate.
  int32_t y;

  /*!
    \brief Default constructor.

    This constructor initializes a Point with zero coordinates.

    \post The point is initialized with x = \c 0 and y = \c 0.
  */
  constexpr Point() noexcept;

  /*!
    \brief Constructs a Point with specified coordinates.

    This constructor initializes a Point with the provided \a x and \a y coordinates.

    \param x The x-coordinate of the point.
    \param y The y-coordinate of the point.
  */
  constexpr Point(const int32_t & x, const int32_t & y) noexcept;

  /*!
    \brief Constructs a Point from an array of \a values.

    This constructor initializes a Point with coordinates from the provided array. The array must contain at least two
    elements where values[0] is x and values[1] is y.

    \param values Pointer to an array containing the x and y coordinates.

    \pre The \a values pointer must not be null.
    \pre The array must contain at least two elements.

    \post The point is initialized with x = values[0] and y = values[1].
  */
  explicit constexpr Point(const int32_t * values) noexcept;

  /*!
    \brief Destructor for the point.

    \note Since the Point does not manage dynamic memory, no special cleanup is required.
  */
  constexpr ~Point() noexcept = default;

  /*!
    \brief Returns a pointer to the array representation of this point.

    This method provides direct access to the internal array representation of the point coordinates. The returned
    pointer points to the x coordinate, and the y coordinate follows immediately after.

    \return A pointer to the array representation of this point.

    \note The returned pointer points to the x coordinate.
    \note The y coordinate can be accessed at pointer + 1.
    \note The returned pointer allows modification of the point coordinates.
    \note Use const version for read-only access.
  */
  [[nodiscard]] constexpr int32_t * c_arr() noexcept;

  /*!
    \brief Returns a constant pointer to the array representation of this point.

    This method provides read-only access to the internal array representation of the point coordinates. The returned
    pointer points to the x coordinate, and the y coordinate follows immediately after.

    \return A constant pointer to the array representation of this point.

    \note The returned pointer points to the x coordinate.
    \note The y coordinate can be accessed at pointer + 1.
    \note The returned pointer is read-only and cannot modify the point coordinates.
    \note Use the non-const overload to allow modification.
  */
  [[nodiscard]] constexpr const int32_t * c_arr() const noexcept;

  /*!
    \brief Adds another \a point to this point.

    This operator adds the coordinates of another \a point to this point's coordinates.

    \param point The point to add to this point.

    \return A reference to this point after addition.

    \post This point's coordinates are the sum of the original coordinates and the other point's coordinates.
  */
  constexpr Point & operator+=(const Point & point) noexcept;

  /*!
    \brief Subtracts another \a point from this point.

    This operator subtracts the coordinates of another \a point from this point's coordinates.

    \param point The point to subtract from this point.

    \return A reference to this point after subtraction.

    \post This point's coordinates are the difference of the original coordinates and the other point's coordinates.
  */
  constexpr Point & operator-=(const Point & point) noexcept;

  /*!
    \brief Multiplies this point by an integer \a scalar.

    This operator multiplies both coordinates of this point by the specified integer \a scalar.

    \param scalar The integer scalar to multiply by.

    \return A reference to this point after multiplication.

    \post This point's coordinates are multiplied by the \a scalar.
  */
  constexpr Point & operator*=(int32_t scalar) noexcept;

  /*!
    \brief Multiplies this point by a real \a scalar.

    This operator multiplies both coordinates of this point by the specified real \a scalar.

    \param scalar The real scalar to multiply by.

    \return A reference to this point after multiplication.

    \post This point's coordinates are multiplied by the \a scalar and converted back to integer type.
  */
  constexpr Point & operator*=(real_t scalar) noexcept;

  /*!
    \brief Divides this point by an integer \a scalar.

    This operator divides both coordinates of this point by the specified integer \a scalar.

    \param scalar The integer scalar to divide by.

    \return A reference to this point after division.

    \pre The \a scalar must not be zero.

    \post This point's coordinates are divided by the \a scalar.
  */
  constexpr Point & operator/=(int32_t scalar) noexcept;

  /*!
    \brief Divides this point by a real \a scalar.

    This operator divides both coordinates of this point by the specified real \a scalar.

    \param scalar The real scalar to divide by.

    \return A reference to this point after division.

    \pre The \a scalar must not be zero.

    \post This point's coordinates are divided by the \a scalar and converted back to integer type.
  */
  constexpr Point & operator/=(real_t scalar) noexcept;

  /*!
    \brief Calculates the squared magnitude of this point.

    This method calculates the squared magnitude (length) of the vector represented by this point.
    The squared magnitude is calculated as x² + y².

    \return The squared magnitude of this point.

    \note This method avoids the expensive square root operation by returning the squared value.
    \note Use this method when comparing magnitudes for performance reasons.
  */
  [[nodiscard]] constexpr int32_t sqrMagnitude() const noexcept;

  /*!
    \brief Sets this point to zero coordinates.

    This method sets both x and y coordinates of this point to zero.

    \post This point's coordinates are set to x = \c 0 and y = \c 0.
    \post The point is at the origin.
  */
  constexpr void setZero() noexcept;

  /*!
    \brief Checks if this point is at the origin.

    This method checks if both coordinates of this point are zero.

    \return \c true if both coordinates are zero, \c false otherwise.

    \note This is equivalent to checking if the point is at the origin (\c 0, \c 0).
  */
  [[nodiscard]] constexpr bool isZero() const noexcept;

  /*!
    \brief Checks if this point is equal to another point within a \a tolerance.

    This method checks if this point is equal to another point within the specified \a tolerance. Two points are
    considered equal if the absolute difference between their coordinates is less than or equal to \a tolerance.

    \param point     The point to compare with.
    \param tolerance The tolerance for equality comparison (default: \c 0).

    \return \c true if the points are equal within the \a tolerance, \c false otherwise.

    \pre The \a tolerance must be non-negative.

    \note When tolerance is \c 0, this performs exact equality comparison.
    \note When tolerance is greater than \c 0, this performs approximate equality comparison.
  */
  [[nodiscard]] bool isEqual(const Point & point, int32_t tolerance = 0) const noexcept;
};

/*!
  \brief Unary minus operator.

  Returns a \a point with negated coordinates.

  \param point The point to negate.

  \return A new point with negated x and y coordinates.
*/
[[nodiscard]] constexpr Point operator-(const Point & point) noexcept;

/*!
  \brief Addition operator for two points.

  Adds the coordinates of two points together.

  \param left  The first point.
  \param right The second point.

  \return A new point with coordinates equal to the sum of the input points.
*/
[[nodiscard]] constexpr Point operator+(const Point & left, const Point & right) noexcept;

/*!
  \brief Subtraction operator for two points.

  Subtracts the coordinates of the second point from the first point.

  \param left  The first point.
  \param right The second point.

  \return A new point with coordinates equal to the difference of the input points.
*/
[[nodiscard]] constexpr Point operator-(const Point & left, const Point & right) noexcept;

/*!
  \brief Multiplication operator for \a point and integer \a scalar.

  Multiplies each coordinate of the \a point by the \a scalar value.

  \param point  The point to multiply.
  \param scalar The integer scalar value.

  \return A new point with coordinates multiplied by the \a scalar.
*/
[[nodiscard]] constexpr Point operator*(const Point & point, int32_t scalar) noexcept;

/*!
  \brief Multiplication operator for integer \a scalar and \a point.

  Multiplies each coordinate of the \a point by the \a scalar value.

  \param scalar The integer scalar value.
  \param point  The point to multiply.

  \return A new point with coordinates multiplied by the \a scalar.
*/
[[nodiscard]] constexpr Point operator*(int32_t scalar, const Point & point) noexcept;

/*!
  \brief Multiplication operator for \a point and real \a scalar.

  Multiplies each coordinate of the \a point by the \a scalar value.

  \param point  The point to multiply.
  \param scalar The real scalar value.

  \return A new point with coordinates multiplied by the \a scalar.
*/
[[nodiscard]] constexpr Point operator*(const Point & point, real_t scalar) noexcept;

/*!
  \brief Multiplication operator for real \a scalar and \a point.

  Multiplies each coordinate of the \a point by the \a scalar value.

  \param scalar The real scalar value.
  \param point  The point to multiply.

  \return A new point with coordinates multiplied by the \a scalar.
*/
[[nodiscard]] constexpr Point operator*(real_t scalar, const Point & point) noexcept;

/*!
  \brief Division operator for \a point and integer \a scalar.

  Divides each coordinate of the \a point by the \a scalar value.

  \param point  The point to divide.
  \param scalar The integer scalar value.

  \return A new point with coordinates divided by the \a scalar.

  \pre The \a scalar must not be zero.
*/
[[nodiscard]] constexpr Point operator/(const Point & point, int32_t scalar) noexcept;

/*!
  \brief Division operator for \a point and real \a scalar.

  Divides each coordinate of the \a point by the \a scalar value.

  \param point  The point to divide.
  \param scalar The real scalar value.

  \return A new point with coordinates divided by the \a scalar.

  \pre The \a scalar must not be zero.
*/
[[nodiscard]] constexpr Point operator/(const Point & point, real_t scalar) noexcept;

/*!
  \brief Equality operator for two points.

  Compares two points for exact equality.

  \param left  The first point.
  \param right The second point.

  \return \c true if both points have identical coordinates, \c false otherwise.

  \note This performs exact equality comparison. For approximate comparison with tolerance, use the \a isEqual method.
*/
[[nodiscard]] constexpr bool operator==(const Point & left, const Point & right) noexcept;

} // namespace toy::math

#endif // INCLUDE_MATH_POINT_HPP_
