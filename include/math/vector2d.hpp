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
  \file   vector2d.hpp
  \brief  2D floating-point vector class for interactive game objects and physics calculations.
*/

#ifndef INCLUDE_MATH_VECTOR2D_HPP_
#define INCLUDE_MATH_VECTOR2D_HPP_

namespace toy::math {

/*!
  \class Vector2D
  \brief 2D floating-point vector class for interactive game objects and physics calculations.

  Vector2D represents a 2D vector with floating-point coordinates, designed for interactive game objects, sprite
  positioning, character movement, particle systems, physics calculations, and world coordinate transformations where
  sub-pixel precision is required.

  \section features Key Features

  - ⚙️ **Floating-Point Precision**: Uses \a real_t (float) for sub-pixel positioning and smooth movement
  - 🔧 **ConstExpr Support**: Most operations can be evaluated at compile time
  - 🛡️ **Exception Safety**: All operations are noexcept
  - 🎯 **Game Optimized**: Designed specifically for interactive game objects and physics
  - 🧬 **Type Safety**: Strong typing with clear vector semantics

  \section usage Usage Example

  \code
  #include "math/vector2d.hpp"

  // Sprite positioning
  toy::math::Vector2D spritePos(100.5f, 200.3f);

  // Character movement
  toy::math::Vector2D velocity(50.0f, -25.0f);
  spritePos += velocity * deltaTime;

  // Physics calculations
  toy::math::Vector2D force(10.0f, 0.0f);
  toy::math::Vector2D acceleration = force / mass;

  // Particle system
  std::vector<toy::math::Vector2D> particles;
  for (auto& particle : particles) {
      particle += velocity * deltaTime;
  }
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(1) constant time
  - 📝 **Assignment**: O(1) constant time
  - 🔗 **Arithmetic Operations**: O(1) constant time
  - ⚖️ **Comparison Operations**: O(1) constant time
  - 💾 **Memory Usage**: 8 bytes (2 * sizeof(real_t))
  - ⚡ **Cache Performance**: Excellent due to small size and stack allocation
  - 📋 **Copy Performance**: Fast due to simple floating-point copying

  \section safety Safety Guarantees

  - 🛡️ **Contracts & Debug Checks**: Division by zero is asserted in debug
  - 🔒 **Bounds Safety**: All operations are bounds-safe
  - 📐 **Type Safety**: Strong typing prevents coordinate mixing
  - ⚠️ **Exception Safety**: All operations are noexcept, no exceptions thrown

  \note This class is specifically designed for interactive game objects and physics calculations.
  \note For UI positioning and pixel-perfect coordinates, consider using Point with integer coordinates.

  \sa toy::math::Point
*/
class Vector2D {
public:
  /// X coordinate.
  real_t x;

  /// Y coordinate.
  real_t y;

  /*!
    \brief Default constructor.

    This constructor initializes a Vector2D with zero coordinates.

    \post The vector is initialized with x = \c 0 and y = \c 0.
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

  /*!
    \brief Adds another \a vector to this vector.

    This operator adds the coordinates of another \a vector to this vector's coordinates.

    \param vector The vector to add to this vector.

    \return A reference to this vector after addition.

    \post This vector's coordinates are the sum of the original coordinates and the other vector's coordinates.
  */
  constexpr Vector2D & operator+=(const Vector2D & vector) noexcept;

  /*!
    \brief Subtracts another \a vector from this vector.

    This operator subtracts the coordinates of another \a vector from this vector's coordinates.

    \param vector The vector to subtract from this vector.

    \return A reference to this vector after subtraction.

    \post This vector's coordinates are the difference of the original coordinates and the other vector's coordinates.
  */
  constexpr Vector2D & operator-=(const Vector2D & vector) noexcept;

  /*!
    \brief Multiplies this vector by a real \a scalar.

    This operator multiplies both coordinates of this vector by the specified real \a scalar.

    \param scalar The real scalar to multiply by.

    \return A reference to this vector after multiplication.

    \post This vector's coordinates are multiplied by the \a scalar.
  */
  constexpr Vector2D & operator*=(real_t scalar) noexcept;

  /*!
    \brief Divides this vector by a real \a scalar.

    This operator divides both coordinates of this vector by the specified real \a scalar.

    \param scalar The real scalar to divide by.

    \return A reference to this vector after division.

    \pre The \a scalar must not be zero.

    \post This vector's coordinates are divided by the \a scalar.
  */
  constexpr Vector2D & operator/=(real_t scalar) noexcept;

  /*!
    \brief Calculates the squared magnitude of this vector.

    This method calculates the squared magnitude (length) of the vector represented by this vector.
    The squared magnitude is calculated as x² + y².

    \return The squared magnitude of this vector.

    \note This method avoids the expensive square root operation by returning the squared value.
    \note Use this method when comparing magnitudes for performance reasons.
  */
  [[nodiscard]] constexpr real_t sqrMagnitude() const noexcept;

  /*!
    \brief Sets this vector to zero coordinates.

    This method sets both x and y coordinates of this vector to zero.

    \post This vector's coordinates are set to x = \c 0 and y = \c 0.
    \post The vector is at the origin.
  */
  constexpr void setZero() noexcept;

  /*!
    \brief Checks if this vector is at the origin.

    This method checks if both coordinates of this vector are zero.

    \return \c true if both coordinates are zero, \c false otherwise.

    \note This is equivalent to checking if the vector is at the origin (\c 0, \c 0).
  */
  [[nodiscard]] constexpr bool isZero() const noexcept;

  /*!
    \brief Checks if this vector is equal to another vector within a \a tolerance.

    This method checks if this vector is equal to another vector within the specified \a tolerance. Two vectors are
    considered equal if the absolute difference between their coordinates is less than or equal to \a tolerance.

    \param vector    The vector to compare with.
    \param tolerance The tolerance for equality comparison (default: \c 0).

    \return \c true if the vector are equal within the \a tolerance, \c false otherwise.

    \pre The \a tolerance must be non-negative.

    \note When tolerance is \c 0, this performs exact equality comparison.
    \note When tolerance is greater than \c 0, this performs approximate equality comparison.
  */
  [[nodiscard]] bool isEqual(const Vector2D & vector, real_t tolerance = 0) const noexcept;
};

/*!
  \brief Unary minus operator.

  Returns a \a vector with negated coordinates.

  \param vector The vector to negate.

  \return A new vector with negated x and y coordinates.
*/
[[nodiscard]] constexpr Vector2D operator-(const Vector2D & vector) noexcept;

/*!
  \brief Addition operator for two vectors.

  Adds the coordinates of two vectors together.

  \param left  The first vector.
  \param right The second vector.

  \return A new vector with coordinates equal to the sum of the input vectors.
*/
[[nodiscard]] constexpr Vector2D operator+(const Vector2D & left, const Vector2D & right) noexcept;

/*!
  \brief Subtraction operator for two vectors.

  Subtracts the coordinates of the second vector from the first vector.

  \param left  The first vector.
  \param right The second vector.

  \return A new vector with coordinates equal to the difference of the input vectors.
*/
[[nodiscard]] constexpr Vector2D operator-(const Vector2D & left, const Vector2D & right) noexcept;

/*!
  \brief Multiplication operator for vector and scalar.

  Multiplies each coordinate of the vector by the scalar value.

  \param left  The vector to multiply.
  \param right The scalar value.

  \return A new vector with coordinates multiplied by the scalar.
*/
[[nodiscard]] constexpr Vector2D operator*(const Vector2D & left, real_t right) noexcept;

/*!
  \brief Multiplication operator for scalar and vector.

  Multiplies each coordinate of the vector by the scalar value.

  \param left  The scalar value.
  \param right The vector to multiply.

  \return A new vector with coordinates multiplied by the scalar.
*/
[[nodiscard]] constexpr Vector2D operator*(real_t left, const Vector2D & right) noexcept;

/*!
  \brief Division operator for vector and scalar.

  Divides each coordinate of the vector by the scalar value.

  \param left  The vector to divide.
  \param right The scalar value.

  \return A new vector with coordinates divided by the scalar.

  \pre The \a right scalar must not be zero.

  \note Division by zero will trigger an assertion in debug mode.
*/
[[nodiscard]] constexpr Vector2D operator/(const Vector2D & left, real_t right) noexcept;

/*!
  \brief Equality operator for two vectors.

  Compares two vectors for exact equality.

  \param left  The first vector.
  \param right The second vector.

  \return \c true if both vectors have identical coordinates, \c false otherwise.

  \note This performs exact equality comparison. For approximate comparison with tolerance, use the \a isEqual method.
*/
[[nodiscard]] constexpr bool operator==(const Vector2D & left, const Vector2D & right) noexcept;

} // namespace toy::math

#endif // INCLUDE_MATH_VECTOR2D_HPP_
