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
  \brief  Character search, comparison, and UTF-8 counting over a range given as a pointer and a length.

  Defines the \ref toy::string_utils functions behind the matching members of \ref toy::StringView and
  \ref toy::String. The caller passes the length it already knows, so no function measures a terminator.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_UTILS_HPP_
#define INCLUDE_CORE_STRING_UTILS_HPP_

namespace toy::string_utils {

/// Character operations every search compares and scans with.
using traits_type = std::char_traits<char>;

/// Offset a search returns when nothing matches.
inline constexpr size_t npos = static_cast<size_t>(-1);

/*!
  \brief Finds the first occurrence of \a ch at or after \a pos.

  \param data Characters to search.
  \param ch   Character to look for.
  \param pos  Offset the search starts from.

  \return Offset of the first \a ch at or after \a pos, or \ref toy::string_utils::npos when there is none; a \a pos
          at or past the range length always gives \ref toy::string_utils::npos.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::rfind(std::span<const char>, char, size_t)
*/
[[nodiscard]] constexpr size_t find(std::span<const char> data, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first position at or after \a pos where \a pattern starts.

  Matches the characters of \a pattern in order and next to each other; toy::string_utils::findFirstOf() matches any
  one of them. Reads the whole of \a pattern, so it needs no terminator and may hold \c '\\0'.

  \param data    Characters to search.
  \param pattern Characters to look for, in order and next to each other.
  \param pos     Offset the search starts from.

  \return Offset where the first match starts, or \ref toy::string_utils::npos when there is none. An empty pattern
          matches at \a pos while \a pos is not greater than the range length.

  \pre Every character of \a data and of \a pattern is readable.
  \pre A null \a data or \a pattern comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::rfind(std::span<const char>, std::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t find(std::span<const char> data, std::span<const char> pattern, size_t pos) noexcept;

/*!
  \brief Finds the last occurrence of \a ch at or before \a pos.

  \param data Characters to search.
  \param ch   Character to look for.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last \a ch at or before \a pos, or \ref toy::string_utils::npos when there is none.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::find(std::span<const char>, char, size_t)
*/
[[nodiscard]] constexpr size_t rfind(std::span<const char> data, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last position at or before \a pos where \a pattern starts.

  Matches the characters of \a pattern in order and next to each other; toy::string_utils::findLastOf() matches any
  one of them. Reads the whole of \a pattern, so it needs no terminator and may hold \c '\\0'.

  \param data    Characters to search.
  \param pattern Characters to look for, in order and next to each other.
  \param pos     Last offset a match may start at; any offset past the range covers all of it.

  \return Offset where the last match starts, or \ref toy::string_utils::npos when there is none. An empty pattern
          matches at the smaller of \a pos and the range length.

  \pre Every character of \a data and of \a pattern is readable.
  \pre A null \a data or \a pattern comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::find(std::span<const char>, std::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t rfind(std::span<const char> data, std::span<const char> pattern, size_t pos) noexcept;

/*!
  \brief Finds the first character at or after \a pos that belongs to \a set.

  Reads \a set as a set, so the order of its characters carries no meaning; toy::string_utils::find() matches them as a
  sequence.

  \param data Characters to search.
  \param set  Characters a match may equal.
  \param pos  Offset the search starts from.

  \return Offset of the first character that belongs to the set, or \ref toy::string_utils::npos when there is none.
          An empty set matches nothing.

  \pre Every character of \a data and of \a set is readable.
  \pre A null \a data or \a set comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findLastOf(std::span<const char>, std::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t findFirstOf(std::span<const char> data, std::span<const char> set, size_t pos) noexcept;

/*!
  \brief Finds the last character at or before \a pos that belongs to \a set.

  Reads \a set as a set, so the order of its characters carries no meaning; toy::string_utils::rfind() matches them as a
  sequence.

  \param data Characters to search.
  \param set  Characters a match may equal.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last character that belongs to the set, or \ref toy::string_utils::npos when there is none. An
          empty set matches nothing.

  \pre Every character of \a data and of \a set is readable.
  \pre A null \a data or \a set comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findFirstOf(std::span<const char>, std::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t findLastOf(std::span<const char> data, std::span<const char> set, size_t pos) noexcept;

/*!
  \brief Finds the first character at or after \a pos that differs from \a ch.

  \param data Characters to search.
  \param ch   Character to skip.
  \param pos  Offset the search starts from.

  \return Offset of the first character other than \a ch at or after \a pos, or \ref toy::string_utils::npos when
          there is none.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findLastNotOf(std::span<const char>, char, size_t)
*/
[[nodiscard]] constexpr size_t findFirstNotOf(std::span<const char> data, char ch, size_t pos) noexcept;

/*!
  \brief Finds the first character at or after \a pos that does not belong to \a set.

  \param data Characters to search.
  \param set  Characters to exclude.
  \param pos  Offset the search starts from.

  \return Offset of the first character that does not belong to the set, or \ref toy::string_utils::npos when there is
          none. No character belongs to an empty set, so the result is then \a pos while \a pos is less than the range
          length.

  \pre Every character of \a data and of \a set is readable.
  \pre A null \a data or \a set comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findLastNotOf(std::span<const char>, std::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t findFirstNotOf(std::span<const char> data, std::span<const char> set,
                                              size_t pos) noexcept;

/*!
  \brief Finds the last character at or before \a pos that differs from \a ch.

  \param data Characters to search.
  \param ch   Character to skip.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last character other than \a ch at or before \a pos, or \ref toy::string_utils::npos when
          there is none.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findFirstNotOf(std::span<const char>, char, size_t)
*/
[[nodiscard]] constexpr size_t findLastNotOf(std::span<const char> data, char ch, size_t pos) noexcept;

/*!
  \brief Finds the last character at or before \a pos that does not belong to \a set.

  \param data Characters to search.
  \param set  Characters to exclude.
  \param pos  Last offset the search considers; any offset past the range covers all of it.

  \return Offset of the last character that does not belong to the set, or \ref toy::string_utils::npos when there is
          none. No character belongs to an empty set, so the result is then \a pos, or one before the range end when
          \a pos is past that, or \ref toy::string_utils::npos for an empty range.

  \pre Every character of \a data and of \a set is readable.
  \pre A null \a data or \a set comes with an empty range, checked by assert_message in debug builds.

  \sa toy::string_utils::findFirstNotOf(std::span<const char>, std::span<const char>, size_t)
*/
[[nodiscard]] constexpr size_t findLastNotOf(std::span<const char> data, std::span<const char> set,
                                             size_t pos) noexcept;

/*!
  \brief Compares \a lhs with \a rhs in lexicographic order.

  Compares bytes as \c unsigned \c char up to the shorter length; when one range is a prefix of the other, the shorter
  one orders first. A \c '\\0' inside either range compares like any other byte.

  \param lhs Left range.
  \param rhs Right range.

  \return A negative value when the left range orders first, \c 0 when both hold the same characters, a positive value
          when the right range orders first.

  \pre Every character of \a lhs and of \a rhs is readable.
  \pre A null \a lhs or \a rhs comes with an empty range, checked by assert_message in debug builds.

  \note Only the sign carries meaning; the magnitude is whatever the byte comparison produced.

  \sa toy::string_utils::find(std::span<const char>, std::span<const char>, size_t)
*/
[[nodiscard]] constexpr int compare(std::span<const char> lhs, std::span<const char> rhs) noexcept;

/*!
  \brief Counts the characters \a data encodes in UTF-8.

  Counts the bytes outside the 10xxxxxx continuation form, so a character counts once whatever width it is stored in.
  Reads the whole of \a data, so it needs no terminator and counts a \c '\\0' inside the range as a character.

  \param data Range to count.

  \return Count of encoded characters, \c 0 for an empty range.

  \pre Every character of \a data is readable.
  \pre A null \a data comes with an empty range, checked by assert_message in debug builds.
  \pre \a data holds well-formed UTF-8, checked with toy::validateUtf8(std::span<const char>) by assert_message in
       debug builds outside constant evaluation. A shipping build and a constant expression skip that check and count
       every byte outside the continuation form, a stray lead byte included.

  \note The count matches the range length only where every character is ASCII.

  \sa toy::string_utils::compare(std::span<const char>, std::span<const char>)
*/
[[nodiscard]] constexpr size_t utf8Size(std::span<const char> data) noexcept;

} // namespace toy::string_utils

#endif // INCLUDE_CORE_STRING_UTILS_HPP_
