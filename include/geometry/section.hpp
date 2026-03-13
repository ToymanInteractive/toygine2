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
  \brief  \ref toy::geometry::Section and \ref toy::geometry::SectionEndpoint declarations.
*/

#ifndef INCLUDE_GEOMETRY_SECTION_HPP_
#define INCLUDE_GEOMETRY_SECTION_HPP_

namespace toy {
namespace geometry {

/*!
  \concept SectionEndpoint
  \brief Concept satisfied when \a T is an endpoint type allowed as \ref toy::geometry::Section template parameter.

  Use to constrain the endpoint type of \ref toy::geometry::Section to signed-integral, floating-point, or fixed-point
  types only.

  \section requirements Requirements

  A type \a T satisfies SectionEndpoint if and only if at least one of the following holds:
  - \a T satisfies \ref toy::math::signed_integral.
  - \a T satisfies \ref toy::math::floating_point.
  - \a T satisfies \ref toy::math::fixed_point.

  \sa toy::geometry::Section
*/
template <typename T>
concept SectionEndpoint = math::signed_integral<T> || math::floating_point<T> || math::fixed_point<T>;

/*!
  \class Section
  \brief One-dimensional interval [start, end] over a numeric endpoint type.

  Represents a closed interval on a numeric type. Default-constructed section is in reset (empty) state:
  \a start > \a end, isValid() returns \c false. Use reset() to clear, or construct with (min, max) for a valid
  interval.

  \tparam T Endpoint type; must satisfy \ref toy::geometry::SectionEndpoint (signed integral, floating-point, or
            fixed-point).

  \section features Key Features

  - **Numeric endpoint only**: \a T constrained by \ref toy::geometry::SectionEndpoint.
  - **Reset state**: Empty interval represented by start > end; reset() restores it.
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
template <SectionEndpoint T>
class Section {
public:
  /// Lower bound of the interval. Default-constructed: \c numeric_limits<T>::max() (reset state).
  T start{numeric_limits<T>::max()};
  /// Upper bound of the interval. Default-constructed: \c numeric_limits<T>::lowest() (reset state).
  T end{numeric_limits<T>::lowest()};

public:
  /*!
    \brief Default constructor; section is in reset (empty) state.

    \post isReset() is \c true, isValid() is \c false, \a start is \c numeric_limits<T>::max(), \a end is
          \c numeric_limits<T>::lowest().
  */
  constexpr Section() noexcept = default;

  /*!
    \brief Constructs a section with the given bounds.

    \param min Lower bound.
    \param max Upper bound.
  */
  constexpr Section(const T & min, const T & max) noexcept;

  /*!
    \brief Returns the midpoint of the interval.

    \return (start + end) / 2.
  */
  [[nodiscard]] constexpr T midpoint() const noexcept;

  /*!
    \brief Returns the length of the interval.

    \return end - start.
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

    \return \c true when start > end.
  */
  [[nodiscard]] constexpr bool isReset() const noexcept;

  /*!
    \brief Returns \c true if the section represents a valid non-empty interval.

    \return \c true when start <= end.
  */
  [[nodiscard]] constexpr bool isValid() const noexcept;

  /*!
    \brief Returns \c true if \a value lies inside [start, end] (inclusive).

    \param value Value to test.

    \return \c true when start <= value <= end.
  */
  [[nodiscard]] constexpr bool isContains(const T & value) const noexcept;
};

/*!
  \brief Deduction guide for \ref toy::geometry::Section: enables \c Section(min, max) without an explicit template
         argument when both arguments have the same \ref toy::geometry::SectionEndpoint type.

  \tparam T Endpoint type; must satisfy \ref toy::geometry::SectionEndpoint.
*/
template <SectionEndpoint T>
Section(const T &, const T &) -> Section<T>;

/*!
  \brief Equality for signed-integral and fixed-point sections: exact comparison of bounds.

  \param a First section.
  \param b Second section.

  \return \c true if a.start == b.start and a.end == b.end.

  \sa operator!=()
*/
template <SectionEndpoint T>
  requires(math::signed_integral<T> || math::fixed_point<T>)
[[nodiscard]] constexpr bool operator==(const Section<T> & a, const Section<T> & b) noexcept;

/*!
  \brief Equality for floating-point sections: approximate comparison of bounds via math::isEqual.

  \param a First section.
  \param b Second section.

  \return \c true if both bounds are equal within the default epsilon (math::isEqual).

  \sa operator!=()
*/
template <SectionEndpoint T>
  requires math::floating_point<T>
[[nodiscard]] constexpr bool operator==(const Section<T> & a, const Section<T> & b) noexcept;

/*!
  \brief Inequality: different bounds.

  \param a First section.
  \param b Second section.

  \return \c true if \c !(a == b).

  \sa operator==()
*/
template <SectionEndpoint T>
[[nodiscard]] constexpr bool operator!=(const Section<T> & a, const Section<T> & b) noexcept;

} // namespace geometry
} // namespace toy

#endif // INCLUDE_GEOMETRY_SECTION_HPP_
