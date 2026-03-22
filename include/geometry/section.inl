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

  Included by geometry.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_GEOMETRY_SECTION_INL_
#define INCLUDE_GEOMETRY_SECTION_INL_

namespace toy::geometry {

template <SectionEndpoint T>
constexpr Section<T>::Section() noexcept
  : start()
  , end() {
#ifdef _DEBUG
  if constexpr (math::floating_point<T>) {
    start = numeric_limits<T>::signaling_NaN();
    end = numeric_limits<T>::signaling_NaN();
  }
#endif
}

template <SectionEndpoint T>
constexpr Section<T>::Section(const T & s, const T & e) noexcept
  : start(s)
  , end(e) {
  assert_message(isValid(), "Section bounds must satisfy start <= end");
}

template <SectionEndpoint T>
constexpr T Section<T>::midpoint() const noexcept {
  assert_message(isValid(), "midpoint() requires a valid section");

  return start + length() / 2;
}

template <SectionEndpoint T>
constexpr T Section<T>::length() const noexcept {
  assert_message(isValid(), "length() requires a valid section");

  return end - start;
}

template <SectionEndpoint T>
constexpr void Section<T>::reset() noexcept {
  start = numeric_limits<T>::max();
  end = numeric_limits<T>::lowest();
}

template <SectionEndpoint T>
constexpr void Section<T>::expand(const T & value) noexcept {
  if (value < start)
    start = value;
  if (value > end)
    end = value;
}

template <SectionEndpoint T>
constexpr void Section<T>::expand(const Section<T> & section) noexcept {
  assert_message(section.isValid(), "Section argument to expand() must be valid (start <= end)");

  if (section.start < start)
    start = section.start;
  if (section.end > end)
    end = section.end;
}

template <SectionEndpoint T>
constexpr bool Section<T>::isReset() const noexcept {
  return start > end;
}

template <SectionEndpoint T>
constexpr bool Section<T>::isValid() const noexcept {
  return start <= end;
}

template <SectionEndpoint T>
constexpr bool Section<T>::isContains(const T & value) const noexcept {
  return start <= value && value <= end;
}

template <SectionEndpoint T>
constexpr bool operator==(const Section<T> & left, const Section<T> & right) noexcept {
  if constexpr (math::floating_point<T>) {
    return math::isEqual(left.start, right.start) && math::isEqual(left.end, right.end);
  } else {
    return left.start == right.start && left.end == right.end;
  }
}

template <SectionEndpoint T>
constexpr bool operator!=(const Section<T> & left, const Section<T> & right) noexcept {
  return !(left == right);
}

} // namespace toy::geometry

#endif // INCLUDE_GEOMETRY_SECTION_INL_
