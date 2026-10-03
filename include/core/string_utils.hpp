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
  \brief Finds the first occurrence of \a ch at or after \a pos.

  \param data First byte of the searched range.
  \param size Number of bytes in the searched range.
  \param ch   Character to look for.
  \param pos  Offset the search starts from.

  \return Offset of the first \a ch at or after \a pos, or \ref toy::string_utils::c_npos when there is none; a \a pos
          at or past \a size always gives \ref toy::string_utils::c_npos.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.

  \sa toy::string_utils::rfind(const char *, size_t, char, size_t)
*/
[[nodiscard]] constexpr size_t find(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first position at or after \a pos where the \a count bytes starting at \a pattern start.

  Matches the bytes of \a pattern in order and next to each other; toy::string_utils::findFirstOf() matches any one of
  them. Reads exactly \a count bytes of the pattern, so it needs no terminator and may hold \c '\\0'.

  \param data    First byte of the searched range.
  \param size    Number of bytes in the searched range.
  \param pattern First byte of the pattern.
  \param pos     Offset the search starts from.
  \param count   Number of bytes in the pattern.

  \return Offset where the first match starts, or \ref toy::string_utils::c_npos when there is none. An empty pattern
          matches at \a pos while \a pos is not greater than \a size.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a pattern is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a pattern are readable.

  \sa toy::string_utils::rfind(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t find(const char * data, size_t size, const char * pattern, size_t pos,
                                    size_t count) noexcept;

/*!
  \brief Finds the last occurrence of \a ch at or before \a pos.

  \param data First byte of the searched range.
  \param size Number of bytes in the searched range.
  \param ch   Character to look for.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last \a ch at or before \a pos, or \ref toy::string_utils::c_npos when there is none.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.

  \sa toy::string_utils::find(const char *, size_t, char, size_t)
*/
[[nodiscard]] constexpr size_t rfind(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last position at or before \a pos where the \a count bytes starting at \a pattern start.

  Matches the bytes of \a pattern in order and next to each other; toy::string_utils::findLastOf() matches any one of
  them. Reads exactly \a count bytes of the pattern, so it needs no terminator and may hold \c '\\0'.

  \param data    First byte of the searched range.
  \param size    Number of bytes in the searched range.
  \param pattern First byte of the pattern.
  \param pos     Last offset a match may start at; any offset past the range covers all of it.
  \param count   Number of bytes in the pattern.

  \return Offset where the last match starts, or \ref toy::string_utils::c_npos when there is none. An empty pattern
          matches at the smaller of \a pos and \a size.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a pattern is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a pattern are readable.

  \sa toy::string_utils::find(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t rfind(const char * data, size_t size, const char * pattern, size_t pos,
                                     size_t count) noexcept;

/*!
  \brief Finds the first character at or after \a pos that belongs to the \a count bytes starting at \a set.

  Reads the bytes of \a set as a set, so their order carries no meaning; toy::string_utils::find() matches them as a
  sequence.

  \param data  First byte of the searched range.
  \param size  Number of bytes in the searched range.
  \param set   First byte of the set.
  \param pos   Offset the search starts from.
  \param count Number of bytes in the set.

  \return Offset of the first character that belongs to the set, or \ref toy::string_utils::c_npos when there is none.
          An empty set matches nothing.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a set are readable.

  \sa toy::string_utils::findLastOf(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t findFirstOf(const char * data, size_t size, const char * set, size_t pos,
                                           size_t count) noexcept;

/*!
  \brief Finds the last character at or before \a pos that belongs to the \a count bytes starting at \a set.

  Reads the bytes of \a set as a set, so their order carries no meaning; toy::string_utils::rfind() matches them as a
  sequence.

  \param data  First byte of the searched range.
  \param size  Number of bytes in the searched range.
  \param set   First byte of the set.
  \param pos   Last offset the search considers; any offset past the range covers all of it.
  \param count Number of bytes in the set.

  \return Offset of the last character that belongs to the set, or \ref toy::string_utils::c_npos when there is none. An
          empty set matches nothing.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a set are readable.

  \sa toy::string_utils::findFirstOf(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t findLastOf(const char * data, size_t size, const char * set, size_t pos,
                                          size_t count) noexcept;

/*!
  \brief Finds the first character at or after \a pos that differs from \a ch.

  \param data First byte of the searched range.
  \param size Number of bytes in the searched range.
  \param ch   Character to skip.
  \param pos  Offset the search starts from.

  \return Offset of the first character other than \a ch at or after \a pos, or \ref toy::string_utils::c_npos when
          there is none.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.

  \sa toy::string_utils::findLastNotOf(const char *, size_t, char, size_t)
*/
[[nodiscard]] constexpr size_t findFirstNotOf(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first character at or after \a pos that does not belong to the \a count bytes starting at \a set.

  \param data  First byte of the searched range.
  \param size  Number of bytes in the searched range.
  \param set   First byte of the set.
  \param pos   Offset the search starts from.
  \param count Number of bytes in the set.

  \return Offset of the first character that does not belong to the set, or \ref toy::string_utils::c_npos when there is
          none. No character belongs to an empty set, so the result is then \a pos while \a pos is less than \a size.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a set are readable.

  \sa toy::string_utils::findLastNotOf(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t findFirstNotOf(const char * data, size_t size, const char * set, size_t pos,
                                              size_t count) noexcept;

/*!
  \brief Finds the last character at or before \a pos that differs from \a ch.

  \param data First byte of the searched range.
  \param size Number of bytes in the searched range.
  \param ch   Character to skip.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last character other than \a ch at or before \a pos, or \ref toy::string_utils::c_npos when
          there is none.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.

  \sa toy::string_utils::findFirstNotOf(const char *, size_t, char, size_t)
*/
[[nodiscard]] constexpr size_t findLastNotOf(const char * data, size_t size, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last character at or before \a pos that does not belong to the \a count bytes starting at \a set.

  \param data  First byte of the searched range.
  \param size  Number of bytes in the searched range.
  \param set   First byte of the set.
  \param pos   Last offset the search considers; any offset past the range covers all of it.
  \param count Number of bytes in the set.

  \return Offset of the last character that does not belong to the set, or \ref toy::string_utils::c_npos when there is
          none. No character belongs to an empty set, so the result is then the smaller of \a pos and \a size minus one,
          or \ref toy::string_utils::c_npos for an empty range.

  \pre \a data is non-null or \a size is \c 0, checked by assert_message in debug builds.
  \pre The \a size bytes starting at \a data are readable.
  \pre \a set is non-null or \a count is \c 0, checked by assert_message in debug builds.
  \pre The \a count bytes starting at \a set are readable.

  \sa toy::string_utils::findFirstNotOf(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr size_t findLastNotOf(const char * data, size_t size, const char * set, size_t pos,
                                             size_t count) noexcept;

/*!
  \brief Compares the \a lhsSize bytes starting at \a lhs with the \a rhsSize bytes starting at \a rhs in lexicographic
         order.

  Compares bytes as \c unsigned \c char up to the shorter length; when one range is a prefix of the other, the shorter
  one orders first. A \c '\\0' inside either range compares like any other byte.

  \param lhs     First byte of the left range.
  \param lhsSize Number of bytes in the left range.
  \param rhs     First byte of the right range.
  \param rhsSize Number of bytes in the right range.

  \return A negative value when the left range orders first, \c 0 when both hold the same characters, a positive value
          when the right range orders first.

  \pre \a lhs is non-null or \a lhsSize is \c 0, checked by assert_message in debug builds.
  \pre The \a lhsSize bytes starting at \a lhs are readable.
  \pre \a rhs is non-null or \a rhsSize is \c 0, checked by assert_message in debug builds.
  \pre The \a rhsSize bytes starting at \a rhs are readable.

  \note Only the sign carries meaning; the magnitude is whatever the byte comparison produced.

  \sa toy::string_utils::find(const char *, size_t, const char *, size_t, size_t)
*/
[[nodiscard]] constexpr int compare(const char * lhs, size_t lhsSize, const char * rhs, size_t rhsSize) noexcept;

} // namespace toy::string_utils

#endif // INCLUDE_CORE_STRING_UTILS_HPP_
