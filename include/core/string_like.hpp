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
  \file   string_like.hpp
  \brief  Concept for read-only C-string views with explicit length.

  Defines \ref toy::StringLike, the requirement a string type meets to be read as a C string of known length. Used to
  constrain a template that takes any such string, std::string included.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_LIKE_HPP_
#define INCLUDE_CORE_STRING_LIKE_HPP_

namespace toy {

/*!
  \concept StringLike
  \brief Concept satisfied when \a T exposes its length and a C-string pointer on a \c const object.

  Constrains a template that reads a string it neither owns nor copies. The requirement expression uses a \c const
  reference, so the constrained template takes the string by \c const \c & and cannot modify the caller's.

  \section string_like_requirements Requirements

  A type satisfies StringLike if and only if, for an lvalue \a str of type \c const \a T:
  * \c str.size() is well-formed and its type is exactly \c std::size_t.
  * \c str.c_str() is well-formed and its result converts to \c const \c char \c *.

  \section string_like_usage Usage Example

  \code
  #include "core.hpp"

  template <toy::StringLike T>
  [[nodiscard]] constexpr bool isEmpty(const T & str) noexcept {
    return str.size() == 0;
  }
  \endcode

  \note The length counts bytes, not characters; under a multi-byte encoding the two differ.

  \note size() has to return \c std::size_t itself. A length reported as \c int, or as a proxy that converts to
        \c std::size_t, fails the requirement.

  \note Null-termination, ownership, and pointer lifetime stay the type's own contract; the requirement expression
        checks none of them.
*/
template <typename T>
concept StringLike = requires(const T & str) {
  { str.size() } -> std::same_as<size_t>;
  { str.c_str() } -> std::convertible_to<const char *>;
};

} // namespace toy

#endif // INCLUDE_CORE_STRING_LIKE_HPP_
