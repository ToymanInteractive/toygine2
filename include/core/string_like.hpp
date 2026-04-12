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
  \file   string_like.hpp
  \brief  Concept for read-only C-string views with explicit length.

  Defines \ref toy::StringLike.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_LIKE_HPP_
#define INCLUDE_CORE_STRING_LIKE_HPP_

namespace toy {

/*!
  \concept StringLike

  \ingroup String

  \brief Concept satisfied when \a T exposes \c size() as \c size_t and \c c_str() as a pointer to null-terminated
         characters.

  This concept defines the interface that any type must satisfy to be considered string-like. It requires the type to
  have a \c size() method that returns a value convertible to \c size_t, and a \c c_str() method that returns a value
  convertible to \c const \c char*.

  \section requirements Requirements

  A type \a T satisfies StringLike if and only if:
  - \c T::size() returns \c size_t (\c std::same_as<size_t>).
  - \c T::c_str() returns \c std::convertible_to<const char *>.

  \section usage Usage Example

  \code
  #include "core.hpp"

  std::string stdStr = "Hello";
  static_assert(StringLike<decltype(stdStr)>);

  constexpr toy::CStringView view("Hi");
  static_assert(StringLike<decltype(view)>);

  toy::FixedString<32> fixedStr = "World";
  static_assert(StringLike<decltype(fixedStr)>);

  toy::FixedString<64> result;
  result.assign(stdStr);
  result += fixedStr;
  \endcode

  \note \c std::string, \ref toy::CStringView, and \ref toy::FixedString are typical representatives; other types
        qualify if they match the expression requirements above.

  \sa toy::CStringView, toy::FixedString
*/
template <typename T>
concept StringLike = requires(const T & str) {
  { str.size() } -> std::same_as<size_t>;
  { str.c_str() } -> std::convertible_to<const char *>;
};

} // namespace toy

#endif // INCLUDE_CORE_STRING_LIKE_HPP_
