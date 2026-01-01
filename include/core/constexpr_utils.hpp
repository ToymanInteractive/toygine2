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
  \file   constexpr_utils.hpp
  \brief  Constexpr utility functions for compile-time operations.
*/

#ifndef INCLUDE_CORE_CONSTEXPR_UTILS_HPP_
#define INCLUDE_CORE_CONSTEXPR_UTILS_HPP_

namespace toy {

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
  \note Return values are -1, 0, or 1 (differs from std::strcmp which returns arithmetic difference).
  \note This function is constexpr-compatible and can be used in compile-time contexts.

  \sa std::strcmp
*/
[[nodiscard]] constexpr int cstrcmp(const char * lhs, const char * rhs) noexcept;

/*!
  \brief Finds the first occurrence of a character in a C string.

  This function searches for the first occurrence of the character \a ch within the string \a str. The search is
  performed character by character from the beginning of the string and returns a pointer to the first occurrence, or
  nullptr if the character is not found. This function is designed to be constexpr-compatible and provides the same
  behavior as std::strchr but can be evaluated at compile time.

  \param str The C string to search in.
  \param ch  The character to search for.

  \return A pointer to the first occurrence of \a ch in \a str, or nullptr if not found.

  \pre The \a str pointer must not be null.

  \note The search is case-sensitive.
  \note The function searches from the beginning of \a str and returns the first match.
  \note This function is constexpr-compatible and can be used in compile-time contexts.

  \sa std::strchr
*/
[[nodiscard]] constexpr const char * cstrchr(const char * str, int ch) noexcept;

/*!
  \brief Finds the first occurrence of any character from a set in a C string.

  This function searches for the first occurrence of any character from the \a accept string within the \a str string.
  The search is performed character by character from the beginning and returns a pointer to the first character found
  that matches any character in the accept set, or nullptr if no character from the accept set is found. This function
  is designed to be constexpr-compatible and provides the same behavior as std::strpbrk but can be evaluated at compile
  time.

  \param str    The C string to search in.
  \param accept The C string containing the set of characters to search for.

  \return A pointer to the first character in \a str that matches any character in \a accept, or nullptr if no
          character from the accept set is found.

  \pre The \a str pointer must not be null.
  \pre The \a accept pointer must not be null.
  \pre The \a str must be null-terminated.
  \pre The \a accept must be null-terminated.

  \note The search is case-sensitive.
  \note The function searches from the beginning of \a str and returns the first match.
  \note This function is constexpr-compatible and can be used in compile-time contexts.

  \sa std::strpbrk
*/
[[nodiscard]] constexpr const char * cstrpbrk(const char * str, const char * accept) noexcept;

/*!
  \brief Finds the first occurrence of a substring in a C string.

  This function searches for the first occurrence of the \a needle string within the \a haystack string. The search is
  performed character by character from the beginning and returns a pointer to the beginning of the first occurrence, or
  nullptr if the substring is not found. This function is designed to be constexpr-compatible and provides the same
  behavior as std::strstr but can be evaluated at compile time.

  \param haystack The C string to search in.
  \param needle   The C string to search for.

  \return A pointer to the first occurrence of \a needle in \a haystack, or nullptr if not found. If \a needle is an
          empty string, returns \a haystack.

  \pre The \a haystack pointer must not be null.
  \pre The \a needle pointer must not be null.

  \note The search is case-sensitive.
  \note If \a needle is an empty string, the function returns \a haystack immediately.
  \note The function searches from the beginning of \a haystack and returns the first match.
  \note This function is constexpr-compatible and can be used in compile-time contexts.

  \sa std::strstr
*/
[[nodiscard]] constexpr const char * cstrstr(const char * haystack, const char * needle) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_CONSTEXPR_UTILS_HPP_
