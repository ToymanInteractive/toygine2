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
  \file   point.hpp
  \brief  TODO
*/

#ifndef INCLUDE_MATHEMATICS_POINT_HPP_
#define INCLUDE_MATHEMATICS_POINT_HPP_

namespace toy::mathematics {

class Point {
public:
  using value_type = std::int32_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

  value_type x;
  value_type y;

  /*!
    \brief Default constructor.

    This constructor initializes a Point with zero coordinates.

    \post The point is initialized with x = 0 and y = 0.
  */
  constexpr Point() noexcept;

  /*!
    \brief Constructs a Point with specified coordinates.

    This constructor initializes a Point with the provided \a x and \a y coordinates.

    \param x The x-coordinate of the point.
    \param y The y-coordinate of the point.
  */
  constexpr Point(const_reference x, const_reference y) noexcept;

  /*!
    \brief Constructs a Point from an array of \a values.

    This constructor initializes a Point with coordinates from the provided array. The array must contain at least two
    elements where values[0] is x and values[1] is y.

    \param values Pointer to an array containing the x and y coordinates.

    \pre The \a values pointer must not be null.
    \pre The array must contain at least two elements.

    \post The point is initialized with x = values[0] and y = values[1].
  */
  explicit constexpr Point(const_pointer values) noexcept;

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
  [[nodiscard]] constexpr pointer c_arr() noexcept;

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
  [[nodiscard]] constexpr const_pointer c_arr() const noexcept;

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
  constexpr Point & operator*=(value_type scalar) noexcept;

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
  constexpr Point & operator/=(value_type scalar) noexcept;

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
  [[nodiscard]] constexpr value_type sqrMagnitude() const noexcept;

  /*!
    \brief Sets this point to zero coordinates.

    This method sets both x and y coordinates of this point to zero.

    \post This point's coordinates are set to x = 0 and y = 0.
  */
  constexpr void setZero() noexcept;

  /*!
    \brief Checks if this point is at the origin.

    This method checks if both coordinates of this point are zero.

    \return true if both coordinates are zero, false otherwise.

    \note This is equivalent to checking if the point is at the origin (0, 0).
  */
  [[nodiscard]] constexpr bool isZero() const noexcept;

  /*!
    \brief Checks if this point is equal to another point within a \a tolerance.

    This method checks if this point is equal to another point within the specified \a tolerance. Two points are
    considered equal if the absolute difference between their coordinates is less than or equal to \a tolerance.

    \param point     The point to compare with.
    \param tolerance The tolerance for equality comparison (default: 0).

    \return true if the points are equal within the \a tolerance, false otherwise.

    \pre The \a tolerance must be non-negative.

    \note When tolerance is 0, this performs exact equality comparison.
    \note When tolerance is greater than 0, this performs approximate equality comparison.
  */
  [[nodiscard]] constexpr bool isEqual(const Point & point, value_type tolerance = 0) const noexcept;
};

} // namespace toy::mathematics

#endif // INCLUDE_MATHEMATICS_POINT_HPP_
