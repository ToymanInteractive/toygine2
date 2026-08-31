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
  \file   c_string_view.hpp
  \brief

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_C_STRING_VIEW_HPP_
#define INCLUDE_CORE_C_STRING_VIEW_HPP_

#include "string_like.hpp"

namespace toy {

class CStringView {
public:
  using traits_type            = char_traits<char>;
  using value_type             = char;
  using pointer                = char *;
  using const_pointer          = const char *;
  using reference              = char &;
  using const_reference        = const char &;
  using const_iterator         = const_pointer;
  using iterator               = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator       = const_reverse_iterator;
  using size_type              = size_t;
  using difference_type        = ptrdiff_t;

  constexpr CStringView() noexcept = default;

  constexpr CStringView(const CStringView & other) noexcept = default;

  constexpr explicit(false) CStringView(const char * s) noexcept;

  CStringView(nullptr_t) = delete;

private:
  const char * _data{nullptr};

  size_t _size{0};
};

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_HPP_
