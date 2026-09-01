//
// Copyright (c) 2026 Toyman Interactive
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
  \file   c_string_view.inl
  \brief  Inline implementations for \ref toy::CStringView.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_C_STRING_VIEW_INL_
#define INCLUDE_CORE_C_STRING_VIEW_INL_

namespace toy {

constexpr CStringView::CStringView(const char * string) noexcept
  : _data(string)
  , _size(string ? char_traits<char>::length(string) : 0) {
  assert_message(string != nullptr, "C string must not be null");
}

constexpr CStringView::const_iterator CStringView::begin() const noexcept {
  return _data;
}

constexpr CStringView::const_iterator CStringView::cbegin() const noexcept {
  return begin();
}

constexpr CStringView::const_iterator CStringView::end() const noexcept {
  return _data + _size;
}

constexpr CStringView::const_iterator CStringView::cend() const noexcept {
  return end();
}

constexpr CStringView::const_reverse_iterator CStringView::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

constexpr CStringView::const_reverse_iterator CStringView::crbegin() const noexcept {
  return rbegin();
}

constexpr CStringView::const_reverse_iterator CStringView::rend() const noexcept {
  return const_reverse_iterator(begin());
}

constexpr CStringView::const_reverse_iterator CStringView::crend() const noexcept {
  return rend();
}

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_INL_
