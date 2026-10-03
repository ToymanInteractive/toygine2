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
  \file   string_utils.hpp
  \brief  Character search over a range given as a pointer and a length.

  Defines the \ref toy::string_utils functions that the search methods of \ref toy::StringView and \ref toy::String
  delegate to. The caller passes the length it already knows, so no search measures a terminator.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_UTILS_HPP_
#define INCLUDE_CORE_STRING_UTILS_HPP_

namespace toy::string_utils {

/// Character operations every search compares and scans with.
using traits_type = std::char_traits<char>;

/// Offset a search returns when nothing matches.
inline constexpr size_t c_npos = static_cast<size_t>(-1);

/*!
  \brief Finds the first occurrence of a character at or after an offset.

  \param data  First character of the searched range.
  \param size  Number of characters in the searched range.
  \param ch    Character to look for.
  \param pos   Offset the search starts from.

  \return Offset of the first \a ch at or after \a pos, or \ref toy::string_utils::c_npos when there is none; a \a pos
          at or past \a size always gives \ref toy::string_utils::c_npos.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.

  \sa toy::string_utils::rfind()
  \sa toy::string_utils::findFirstNotOf()
*/
[[nodiscard]] constexpr size_t find(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first position at or after an offset where a character sequence starts.

  \param data     First character of the searched range.
  \param size     Number of characters in the searched range.
  \param pattern  First character of the sequence to look for.
  \param pos      Offset the search starts from.
  \param count    Number of characters in \a pattern.

  \return Offset where the first match starts, or \ref toy::string_utils::c_npos when there is none. An empty
          \a pattern matches at \a pos while \a pos does not exceed \a size.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.
  \pre \a pattern is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count characters starting at \a pattern are readable.

  \note Worst case O(\a size * \a count) character comparisons.

  \sa toy::string_utils::rfind()
*/
[[nodiscard]] constexpr size_t find(const char * data, size_t size, const char * pattern, size_t pos,
                                    size_t count) noexcept;

/*!
  \brief Finds the last occurrence of a character at or before an offset.

  \param data  First character of the searched range.
  \param size  Number of characters in the searched range.
  \param ch    Character to look for.
  \param pos   Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last \a ch at or before \a pos, or \ref toy::string_utils::c_npos when there is none.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.

  \sa toy::string_utils::find()
  \sa toy::string_utils::findLastNotOf()
*/
[[nodiscard]] constexpr size_t rfind(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last position at or before an offset where a character sequence starts.

  \param data     First character of the searched range.
  \param size     Number of characters in the searched range.
  \param pattern  First character of the sequence to look for.
  \param pos      Last offset a match may start at; any offset past the range covers all of it.
  \param count    Number of characters in \a pattern.

  \return Offset where the last match starts, or \ref toy::string_utils::c_npos when there is none. An empty
          \a pattern matches at the smaller of \a pos and \a size.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.
  \pre \a pattern is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count characters starting at \a pattern are readable.

  \note Worst case O(\a size * \a count) character comparisons.

  \sa toy::string_utils::find()
*/
[[nodiscard]] constexpr size_t rfind(const char * data, size_t size, const char * pattern, size_t pos,
                                     size_t count) noexcept;

/*!
  \brief Finds the first character at or after an offset that belongs to a set.

  \param data   First character of the searched range.
  \param size   Number of characters in the searched range.
  \param set    First character of the set.
  \param pos    Offset the search starts from.
  \param count  Number of characters in \a set.

  \return Offset of the first character found in \a set, or \ref toy::string_utils::c_npos when there is none. An
          empty \a set matches nothing.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count characters starting at \a set are readable.

  \note Worst case O(\a size * \a count) character comparisons.

  \sa toy::string_utils::findLastOf()
  \sa toy::string_utils::findFirstNotOf()
*/
[[nodiscard]] constexpr size_t findFirstOf(const char * data, size_t size, const char * set, size_t pos,
                                           size_t count) noexcept;

/*!
  \brief Finds the last character at or before an offset that belongs to a set.

  \param data   First character of the searched range.
  \param size   Number of characters in the searched range.
  \param set    First character of the set.
  \param pos    Last offset the search considers; any offset past the range covers all of it.
  \param count  Number of characters in \a set.

  \return Offset of the last character found in \a set, or \ref toy::string_utils::c_npos when there is none. An
          empty \a set matches nothing.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count characters starting at \a set are readable.

  \note Worst case O(\a size * \a count) character comparisons.

  \sa toy::string_utils::findFirstOf()
  \sa toy::string_utils::findLastNotOf()
*/
[[nodiscard]] constexpr size_t findLastOf(const char * data, size_t size, const char * set, size_t pos,
                                          size_t count) noexcept;

/*!
  \brief Finds the first character at or after an offset that differs from a given one.

  \param data  First character of the searched range.
  \param size  Number of characters in the searched range.
  \param ch    Character to skip.
  \param pos   Offset the search starts from.

  \return Offset of the first character other than \a ch, or \ref toy::string_utils::c_npos when there is none.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.

  \sa toy::string_utils::find()
  \sa toy::string_utils::findLastNotOf()
*/
[[nodiscard]] constexpr size_t findFirstNotOf(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first character at or after an offset that does not belong to a set.

  \param data   First character of the searched range.
  \param size   Number of characters in the searched range.
  \param set    First character of the set.
  \param pos    Offset the search starts from.
  \param count  Number of characters in \a set.

  \return Offset of the first character missing from \a set, or \ref toy::string_utils::c_npos when there is none. No
          character belongs to an empty \a set, so the result is then \a pos while \a pos is less than \a size.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count characters starting at \a set are readable.

  \note Worst case O(\a size * \a count) character comparisons.

  \sa toy::string_utils::findFirstOf()
  \sa toy::string_utils::findLastNotOf()
*/
[[nodiscard]] constexpr size_t findFirstNotOf(const char * data, size_t size, const char * set, size_t pos,
                                              size_t count) noexcept;

/*!
  \brief Finds the last character at or before an offset that differs from a given one.

  \param data  First character of the searched range.
  \param size  Number of characters in the searched range.
  \param ch    Character to skip.
  \param pos   Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last character other than \a ch, or \ref toy::string_utils::c_npos when there is none.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.

  \sa toy::string_utils::rfind()
  \sa toy::string_utils::findFirstNotOf()
*/
[[nodiscard]] constexpr size_t findLastNotOf(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last character at or before an offset that does not belong to a set.

  \param data   First character of the searched range.
  \param size   Number of characters in the searched range.
  \param set    First character of the set.
  \param pos    Last offset the search considers; any offset past the range covers all of it.
  \param count  Number of characters in \a set.

  \return Offset of the last character missing from \a set, or \ref toy::string_utils::c_npos when there is none. No
          character belongs to an empty \a set, so the result is then the smaller of \a pos and the last offset, or
          \ref toy::string_utils::c_npos for an empty range.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size characters starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count characters starting at \a set are readable.

  \note Worst case O(\a size * \a count) character comparisons.

  \sa toy::string_utils::findLastOf()
  \sa toy::string_utils::findFirstNotOf()
*/
[[nodiscard]] constexpr size_t findLastNotOf(const char * data, size_t size, const char * set, size_t pos,
                                             size_t count) noexcept;

/*!
  \brief Compares two character ranges in lexicographic order.

  Compares bytes as \c unsigned \c char up to the shorter length; when one range is a prefix of the other, the shorter
  one orders first.

  \param lhs     First character of the left range.
  \param lhsSize Number of characters in the left range.
  \param rhs     First character of the right range.
  \param rhsSize Number of characters in the right range.

  \return A negative value when the left range orders first, \c 0 when both hold the same characters, a positive value
          when the right range orders first. Only the sign carries meaning.

  \pre \a lhs is non-null or \a lhsSize is \c 0, checked by assert_message in debug builds.
  \pre \a rhs is non-null or \a rhsSize is \c 0, checked by assert_message in debug builds.
  \pre The \a lhsSize characters starting at \a lhs and the \a rhsSize characters starting at \a rhs are readable.

  \sa toy::string_utils::find()
*/
[[nodiscard]] constexpr int compare(const char * lhs, size_t lhsSize, const char * rhs, size_t rhsSize) noexcept;

} // namespace toy::string_utils

#endif // INCLUDE_CORE_STRING_UTILS_HPP_
