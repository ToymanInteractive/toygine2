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
  \file   string_like.hpp
  \brief  C++ concept for string-like types with size() and c_str() methods.
*/

#ifndef INCLUDE_CORE_STRING_LIKE_HPP_
#define INCLUDE_CORE_STRING_LIKE_HPP_

namespace toy {

/*!
  \concept StringLike
  \brief Concept defining the requirements for string-like types.

  This concept defines the interface that any type must satisfy to be considered string-like. It requires the type to
  have a \c size() method that returns a value convertible to \c size_t, and a \c c_str() method that returns a value
  convertible to \c const \c char*.

  \section requirements Requirements

  A type \a T satisfies StringLike if and only if:
  - \c T::size() returns \c std::same_as<size_t>
  - \c T::c_str() returns \c std::convertible_to<const char*>

  \section usage Usage Example

  \code
  #include "core.hpp"

  // std::string satisfies StringLike
  std::string stdStr = "Hello";
  static_assert(StringLike<decltype(stdStr)>);

  // FixedString satisfies StringLike
  toy::FixedString<32> fixedStr = "World";
  static_assert(StringLike<decltype(fixedStr)>);

  // Use with FixedString methods
  toy::FixedString<64> result;
  result.assign(stdStr);  // Works with std::string
  result += fixedStr;     // Works with FixedString
  \endcode

  \note The concept ensures type safety and provides a compile-time guarantee that the required methods exist and return
        compatible types.
  \note Common types that satisfy this concept include \c std::string, \ref toy::CStringView, \ref toy::FixedString, and
        custom string types that provide the required interface.

  \sa toy::CStringView
  \sa toy::FixedString
*/
template <typename T>
concept StringLike = requires(const T & str) {
  { str.size() } -> std::same_as<size_t>;
  { str.c_str() } -> std::convertible_to<const char *>;
};

} // namespace toy

#endif // INCLUDE_CORE_STRING_LIKE_HPP_
