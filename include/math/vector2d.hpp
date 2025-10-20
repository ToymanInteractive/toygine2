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
  \file   vector2d.hpp
  \brief  TODO
*/

#ifndef INCLUDE_MATH_VECTOR2D_HPP_
#define INCLUDE_MATH_VECTOR2D_HPP_

namespace toy::math {

class Vector2D {
public:
  /// X coordinate.
  real_t x;

  /// Y coordinate.
  real_t y;

  /*!
    \brief Default constructor.

    This constructor initializes a Vector2D with zero coordinates.

    \post The vector is initialized with x = 0 and y = 0.
  */
  constexpr Vector2D() noexcept;

  /*!
    \brief Constructs a Vector2D with specified coordinates.

    This constructor initializes a Vector2D with the provided \a x and \a y coordinates.

    \param x The x-coordinate of the vector.
    \param y The y-coordinate of the vector.
  */
  constexpr Vector2D(const real_t & x, const real_t & y) noexcept;

  /*!
    \brief Constructs a Vector2D from an array of \a values.

    This constructor initializes a Vector2D with coordinates from the provided array. The array must contain at least
    two elements where values[0] is x and values[1] is y.

    \param values Pointer to an array containing the x and y coordinates.

    \pre The \a values pointer must not be null.
    \pre The array must contain at least two elements.

    \post The vector is initialized with x = values[0] and y = values[1].
  */
  explicit constexpr Vector2D(const real_t * values) noexcept;

  /*!
    \brief Destructor for the vector.

    \note Since the Vector2D does not manage dynamic memory, no special cleanup is required.
    */
  constexpr ~Vector2D() noexcept = default;

  /*!
    \brief Returns a pointer to the array representation of this vector.

    This method provides direct access to the internal array representation of the vector coordinates. The returned
    pointer points to the x coordinate, and the y coordinate follows immediately after.

    \return A pointer to the array representation of this vector.

    \note The returned pointer points to the x coordinate.
    \note The y coordinate can be accessed at pointer + 1.
    \note The returned pointer allows modification of the vector coordinates.
    \note Use const version for read-only access.
    */
  [[nodiscard]] constexpr real_t * c_arr() noexcept;

  /*!
    \brief Returns a constant pointer to the array representation of this vector.

    This method provides read-only access to the internal array representation of the vector coordinates. The returned
    pointer points to the x coordinate, and the y coordinate follows immediately after.

    \return A constant pointer to the array representation of this vector.

    \note The returned pointer points to the x coordinate.
    \note The y coordinate can be accessed at pointer + 1.
    \note The returned pointer is read-only and cannot modify the vector coordinates.
    \note Use the non-const overload to allow modification.
  */
  [[nodiscard]] constexpr const real_t * c_arr() const noexcept;
};

} // namespace toy::math

#endif // INCLUDE_MATH_VECTOR2D_HPP_
