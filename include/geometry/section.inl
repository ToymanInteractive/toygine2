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
  \file   section.inl
  \brief  Inline implementations for \ref toy::geometry::Section.

  Implements constructors, midpoint(), length(), reset(), expand(), isReset(), isValid(), isContains(),
  operator==(), operator!=(). Included by the geometry umbrella header; do not include directly.
*/

#ifndef INCLUDE_GEOMETRY_SECTION_INL_
#define INCLUDE_GEOMETRY_SECTION_INL_

namespace toy::geometry {

template <typename T>
  requires SectionScalar<T>
constexpr Section<T>::Section(const T & min, const T & max) noexcept
  : minimum(min)
  , maximum(max) {
  assert_message(isValid(), "Section bounds must satisfy minimum <= maximum");
}

template <typename T>
  requires SectionScalar<T>
constexpr T Section<T>::midpoint() const noexcept {
  assert_message(isValid(), "midpoint() requires a valid section");

  return minimum + (maximum - minimum) / 2;
}

template <typename T>
  requires SectionScalar<T>
constexpr T Section<T>::length() const noexcept {
  assert_message(isValid(), "length() requires a valid section");

  return maximum - minimum;
}

template <typename T>
  requires SectionScalar<T>
constexpr void Section<T>::reset() noexcept {
  minimum = numeric_limits<T>::max();
  maximum = numeric_limits<T>::lowest();
}

template <typename T>
  requires SectionScalar<T>
constexpr void Section<T>::expand(const T & value) noexcept {
  if (value < minimum)
    minimum = value;
  if (value > maximum)
    maximum = value;
}

template <typename T>
  requires SectionScalar<T>
constexpr void Section<T>::expand(const Section<T> & section) noexcept {
  assert_message(section.isValid(), "Section argument to expand() must be valid (minimum <= maximum)");

  if (section.minimum < minimum)
    minimum = section.minimum;
  if (section.maximum > maximum)
    maximum = section.maximum;
}

template <typename T>
  requires SectionScalar<T>
constexpr bool Section<T>::isReset() const noexcept {
  return minimum > maximum;
}

template <typename T>
  requires SectionScalar<T>
constexpr bool Section<T>::isValid() const noexcept {
  return minimum <= maximum;
}

template <typename T>
  requires SectionScalar<T>
constexpr bool Section<T>::isContains(const T & value) const noexcept {
  return minimum <= value && value <= maximum;
}

// int and fixed: exact comparison via operator==.
template <typename T>
  requires SectionScalar<T> && (std::integral<T> || math::fixed_point<T>)
constexpr bool operator==(const Section<T> & a, const Section<T> & b) noexcept {
  return a.minimum == b.minimum && a.maximum == b.maximum;
}

// float, double, long double: approximate comparison via math::isEqual.
template <typename T>
  requires SectionScalar<T> && std::floating_point<T>
constexpr bool operator==(const Section<T> & a, const Section<T> & b) noexcept {
  return math::isEqual(a.minimum, b.minimum) && math::isEqual(a.maximum, b.maximum);
}

template <typename T>
  requires SectionScalar<T>
constexpr bool operator!=(const Section<T> & a, const Section<T> & b) noexcept {
  return !(a == b);
}

} // namespace toy::geometry

#endif // INCLUDE_GEOMETRY_SECTION_INL_
