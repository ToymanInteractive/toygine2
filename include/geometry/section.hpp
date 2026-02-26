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
  \file   section.hpp
  \brief  \ref toy::geometry::Section and \ref toy::geometry::SectionScalar declarations.
*/

#ifndef INCLUDE_GEOMETRY_SECTION_HPP_
#define INCLUDE_GEOMETRY_SECTION_HPP_

namespace toy {
namespace geometry {

/*!
  \brief Concept restricting \ref toy::geometry::Section scalar type to integral, floating-point, or \ref
  toy::math::fixed.

  \tparam T Type to check.
*/
template <typename T>
concept SectionScalar = std::integral<T> || std::floating_point<T> || math::fixed_point<T>;

/*!
  \class Section
  \brief One-dimensional interval [minimum, maximum] over a numeric scalar type.

  Represents a closed interval on a numeric type. Default-constructed section is in reset (empty) state:
  \a minimum > \a maximum, isValid() returns \c false. Use reset() to clear, or construct with (min, max)
  for a valid interval.

  \tparam T Scalar type; must satisfy \ref toy::geometry::SectionScalar (integral, floating-point, or \ref
          toy::math::fixed).

  \section features Key Features

  - **Numeric scalar only**: \a T constrained by \ref toy::geometry::SectionScalar.
  - **Reset state**: Empty interval represented by minimum > maximum; reset() restores it.
  - **Expand**: expand() extends bounds to include a value or another section (union).
  - **Constexpr**: All operations are constexpr and noexcept where applicable.

  \section usage Usage Example

  \code
  #include "geometry.hpp"

  toy::geometry::Section<int> s(10, 20);

  s.expand(5);   // now [5, 20]
  s.expand(toy::geometry::Section<int>(25, 30)); // now [5, 30]
  \endcode

  \section performance Performance Characteristics

  **Operations**: O(1). No allocations.

  \section safety Safety Guarantees

  **Preconditions**: expand(const Section<T>&) requires the argument to satisfy isValid() (assert in debug).
  **Exception safety**: All operations are noexcept.

  \sa operator==(), operator!=()
*/
template <typename T>
  requires SectionScalar<T>
class Section {
public:
  T minimum{numeric_limits<T>::max()};
  T maximum{numeric_limits<T>::lowest()};

public:
  constexpr Section() noexcept = default;

  /*!
    \brief Constructs a section with the given bounds.

    \param min Lower bound.
    \param max Upper bound.
  */
  constexpr Section(const T & min, const T & max) noexcept;

  /*!
    \brief Returns the midpoint of the interval.

    \return (minimum + maximum) / 2.
  */
  [[nodiscard]] constexpr T midpoint() const noexcept;

  /*!
    \brief Returns the length of the interval.

    \return maximum - minimum.
  */
  [[nodiscard]] constexpr T length() const noexcept;

  /*!
    \brief Puts the section in reset (empty) state.

    \post isReset() is \c true, isValid() is \c false.
  */
  constexpr void reset() noexcept;

  /*!
    \brief Extends the interval to include \a value if outside current bounds.

    \param value Value to include.
  */
  constexpr void expand(const T & value) noexcept;

  /*!
    \brief Merges \a section into this interval (union of two intervals).

    \pre \a section.isValid() is \c true.

    \param section Section to merge.

    \post This section contains the union of the previous interval and \a section.
  */
  constexpr void expand(const Section<T> & section) noexcept;

  /*!
    \brief Returns \c true if the section is in reset (empty) state.

    \return \c true when minimum > maximum.
  */
  [[nodiscard]] constexpr bool isReset() const noexcept;

  /*!
    \brief Returns \c true if the section represents a valid non-empty interval.

    \return \c true when minimum <= maximum.
  */
  [[nodiscard]] constexpr bool isValid() const noexcept;

  /*!
    \brief Returns \c true if \a value lies inside [minimum, maximum] (inclusive).

    \param value Value to test.

    \return \c true when minimum <= value <= maximum.
  */
  [[nodiscard]] constexpr bool isContains(const T & value) const noexcept;
};

/*!
  \brief Equality for integral and fixed-point sections: exact comparison of bounds.

  \param a First section.
  \param b Second section.

  \return \c true if a.minimum == b.minimum and a.maximum == b.maximum.

  \sa operator!=()
*/
template <typename T>
  requires SectionScalar<T> && (std::integral<T> || math::fixed_point<T>)
[[nodiscard]] constexpr bool operator==(const Section<T> & a, const Section<T> & b) noexcept;

/*!
  \brief Equality for floating-point sections: approximate comparison of bounds via math::isEqual.

  \param a First section.
  \param b Second section.

  \return \c true if both bounds are equal within the default epsilon (math::isEqual).

  \sa operator!=()
*/
template <typename T>
  requires SectionScalar<T> && std::floating_point<T>
[[nodiscard]] constexpr bool operator==(const Section<T> & a, const Section<T> & b) noexcept;

/*!
  \brief Inequality: different bounds.

  \param a First section.
  \param b Second section.

  \return \c true if \c !(a == b).

  \sa operator==()
*/
template <typename T>
  requires SectionScalar<T>
[[nodiscard]] constexpr bool operator!=(const Section<T> & a, const Section<T> & b) noexcept;

} // namespace geometry
} // namespace toy

#endif // INCLUDE_GEOMETRY_SECTION_HPP_
