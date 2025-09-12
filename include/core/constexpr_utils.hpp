//
// Copyright (c) 2025 by Toyman Interactive
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
  \file   constexpr_utils.hpp
  \brief  Constexpr utility functions for compile-time operations.
*/

#ifndef INCLUDE_CORE_CONSTEXPR_UTILS_HPP_
#define INCLUDE_CORE_CONSTEXPR_UTILS_HPP_

namespace toygine {

/*!
  \brief Compares two C strings lexicographically.

  This function performs a lexicographic comparison between two C strings. The comparison is performed character by
  character using the character's numeric value. This function is designed to be constexpr-compatible and provides the
  same behavior as std::strcmp but can be evaluated at compile time.

  \param lhs The left-hand side C string to compare.
  \param rhs The right-hand side C string to compare.

  \return A negative value if \a lhs is lexicographically less than \a rhs, zero if they are equal,
          or a positive value if \a lhs is lexicographically greater than \a rhs.

  \pre The \a lhs pointer must not be null.
  \pre The \a rhs pointer must not be null.

  \note The comparison is case-sensitive.
  \note The comparison stops at the first character that differs between the strings.
  \note If one string is a prefix of another, the shorter string is considered lexicographically smaller.
  \note This function is constexpr-compatible and can be used in compile-time contexts.

  \example
  \code
  constexpr const char* str1 = "Hello";
  constexpr const char* str2 = "World";
  constexpr int result = cstrcmp(str1, str2);  // result < 0
  constexpr int equal = cstrcmp("Test", "Test");  // equal == 0
  \endcode

  \see std::strcmp
*/
[[nodiscard]] constexpr int cstrcmp(const char * lhs, const char * rhs) noexcept;

} // namespace toygine

#endif // INCLUDE_CORE_CONSTEXPR_UTILS_HPP_
