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
  \file   string_view.hpp
  \brief  Non-owning view over a null-terminated byte string that carries its length.

  Defines \ref toy::StringView, a read-only view that holds the pointer to a null-terminated byte string and the length
  measured when the view was built. Used where a call site hands the string to a C interface and still needs its length
  without a second scan for the terminator.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_VIEW_HPP_
#define INCLUDE_CORE_STRING_VIEW_HPP_

#include "string_like.hpp"

namespace toy {

/*!
  \class StringView
  \brief Read-only view over a null-terminated byte string that keeps the pointer and its measured length.

  Stores the pointer the caller supplies and copies neither the characters nor the terminator, so the viewed string has
  to outlive the view. Null-termination is part of the contract, unlike \c std::string_view: the pointer stays valid
  input for a C interface, and the length needs no second scan.

  \section string_view_features Key Features

  * **Null-terminated**: every viewed string ends in a null character, so its pointer reaches a C interface unchanged.
  * **Length measured once**: the pointer constructor scans for the terminator, and the view holds the result.
  * **Constexpr support**: every operation except utf8_size() evaluates in a constant expression.
  * **No allocation**: the view holds a pointer and a length, and owns no characters.
  * **Range access**: forward and reverse iterator pairs, so a range-based \c for and the standard algorithms read the
    view.
  * **Search set**: substring, character, and character-set searches in both directions, each matching the
    \c std::string_view contract.
  * **Ordered comparison**: equality and a three-way order over the bytes, against another view or a null-terminated
    byte string.
  * **Type safety**: construction from \c nullptr is deleted, so the null case fails to compile.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section string_view_usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::StringView name("player");
  constexpr toy::StringView alias(name);

  size_t letters = 0;
  for (const char character : alias) {
    letters += static_cast<size_t>(character != ' ');
  }

  const char last = *name.rbegin();

  constexpr toy::StringView path("assets/player.png");

  const size_t extension = path.rfind('.');
  const bool   named     = path.contains(name);
  const bool   isPlayer  = name == "player";
  \endcode

  \section string_view_performance Performance Characteristics

  * **Construction from a pointer**: O(n) in the length of the string, one scan for the terminator.
  * **Default construction and copying**: O(1).
  * **Iterator access**: O(1); each iterator is a pointer into the viewed string, or a reverse adaptor over one.
  * **Substring search**: O(n * m) in the length of the view and the length of the needle; nothing is indexed or
    cached between calls.
  * **Character-set search**: O(n * m) in the length of the view and the size of the set.
  * **Comparison**: O(n) in the shorter of the two lengths; equality stops on a length mismatch before reading a
    character.
  * **UTF-8 character count**: O(n) in the length of the string, one walk over the lead bytes; nothing is cached
    between calls.
  * **Memory usage**: one pointer and one length, 16 bytes on a 64-bit target and 8 bytes on a 32-bit one.

  \section string_view_safety Safety Guarantees

  * **Contracts**: the pointer constructor checks its argument against \c nullptr with assert_message in debug builds.
    A shipping build skips the check and stores the null pointer with a length of \c 0.
  * **Lifetime**: the characters belong to whoever created them; the view neither owns them nor extends their
    lifetime.
  * **Type safety**: the deleted \c nullptr_t constructor rejects a literal \c nullptr during compilation.
  * **Iterator validity**: an iterator stays valid while the string it points into lives. Assigning to the view repoints
    the view alone and leaves an outstanding iterator on the previous string.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section string_view_compatibility Compatibility

  * No operation allocates or calls into the platform, so the type suits embedded and retro targets.

  \note The length counts bytes, not characters; under a multi-byte encoding the two differ.
  \note A default-constructed view holds a null pointer, not a pointer to an empty string.

  \warning A view built over a temporary dangles as soon as that temporary dies at the end of the full expression.

  \sa \ref toy::StringLike
*/
class StringView {
public:
  /// Character operations the view uses to measure and compare
  using traits_type            = std::char_traits<char>;
  /// Type of the viewed characters
  using value_type             = char;
  /// Mutable character pointer, present for parity with \c std::string_view; the view exposes no mutable access
  using pointer                = char *;
  /// Pointer to a viewed character
  using const_pointer          = const char *;
  /// Mutable character reference, present for parity with \c std::string_view; the view exposes no mutable access
  using reference              = value_type &;
  /// Reference to a viewed character
  using const_reference        = const value_type &;
  /// Iterator over the viewed characters
  using const_iterator         = const_pointer;
  /// Read-only iterator alias; repeats \ref toy::StringView::const_iterator
  using iterator               = const_iterator;
  /// Iterator walking the viewed characters back to front
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  /// Read-only reverse iterator alias; repeats \ref toy::StringView::const_reverse_iterator
  using reverse_iterator       = const_reverse_iterator;
  /// Unsigned type the length is measured in
  using size_type              = size_t;
  /// Signed type for the distance between two iterators
  using difference_type        = ptrdiff_t;

  /*!
    \brief Builds an empty view.

    \post The view holds a null pointer and a length of \c 0.
  */
  constexpr StringView() noexcept = default;

  /*!
    \brief Builds a view over the same string as \a other.

    \param other View to copy the pointer and the length from.

    \post Both views read the same characters; neither owns them.
  */
  constexpr StringView(const StringView & other) noexcept = default;

  /*!
    \brief Builds a view over a null-terminated byte string and measures its length.

    Converts implicitly, so a string literal or a \c const \c char \c * argument becomes a view at the call site. Only
    the pointer and the length are stored; the characters stay where the caller put them.

    \param string Null-terminated byte string to view.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre \a string outlives the view and keeps its terminator in place.

    \post The view holds \a string and its length in bytes, the terminator excluded.

    \sa StringView()
  */
  constexpr explicit(false) StringView(const value_type * string) noexcept;

  /*!
    \brief Deleted: a null pointer names no string to view.

    Rejects a literal \c nullptr during compilation, ahead of the debug check the pointer constructor performs.

    \sa StringView(const char *)
  */
  StringView(nullptr_t) = delete;

  /*!
    \brief Makes this view read the same string as \a view.

    \param view View to copy the pointer and the length from.

    \return Reference to this view.

    \post Both views read the same characters. The string this view held before stays untouched, since the view owns
          nothing.

    \sa StringView(const StringView &)
  */
  constexpr StringView & operator=(const StringView & view) noexcept = default;

  /*!
    \brief Returns an iterator to the first character.

    \return Iterator to the first character, equal to end() when the view holds no character.

    \note A view holding no string yields a null pointer, which still compares equal to end().

    \sa end()
    \sa cbegin()
  */
  [[nodiscard]] constexpr const_iterator begin() const noexcept;

  /*!
    \brief Returns an iterator to the first character.

    Repeats begin(): the view exposes only const iterators.

    \return Iterator to the first character, equal to cend() when the view holds no character.

    \sa begin()
    \sa cend()
  */
  [[nodiscard]] constexpr const_iterator cbegin() const noexcept;

  /*!
    \brief Returns an iterator one past the last character.

    \return Iterator to the position after the last character.

    \note Over a viewed string that position holds the terminator.
    \note A view holding no string yields a null pointer.

    \sa begin()
    \sa cend()
  */
  [[nodiscard]] constexpr const_iterator end() const noexcept;

  /*!
    \brief Returns an iterator one past the last character.

    Repeats end(): the view exposes only const iterators.

    \return Iterator to the position after the last character.

    \sa end()
    \sa cbegin()
  */
  [[nodiscard]] constexpr const_iterator cend() const noexcept;

  /*!
    \brief Returns a reverse iterator to the last character.

    Starts at the last character; the walk runs back to front and stops before the first.

    \return Reverse iterator over end(), equal to rend() when the view holds no character.

    \sa rend()
    \sa crbegin()
  */
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept;

  /*!
    \brief Returns a reverse iterator to the last character.

    Repeats rbegin(): the view exposes only const iterators.

    \return Reverse iterator over cend(), equal to crend() when the view holds no character.

    \sa rbegin()
    \sa crend()
  */
  [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept;

  /*!
    \brief Returns a reverse iterator to the position before the first character.

    \return Reverse iterator over begin(), the end of a back-to-front walk.

    \note Dereferencing it reads before the first character; a walk ends by comparing against it instead.

    \sa rbegin()
    \sa crend()
  */
  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept;

  /*!
    \brief Returns a reverse iterator to the position before the first character.

    Repeats rend(): the view exposes only const iterators.

    \return Reverse iterator over cbegin(), the end of a back-to-front walk.

    \sa rend()
    \sa crbegin()
  */
  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept;

  /*!
    \brief Returns the character at an offset.

    \param pos Zero-based offset of the character to read.

    \return Reference to the character at \a pos.

    \pre \a pos is less than size(), checked by assert_message in debug builds.

    \sa at()
    \sa front()
  */
  [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept;

  /*!
    \brief Returns the character at an offset.

    Repeats operator[](): exceptions are off in the build, so a bad offset fails the same debug check instead of
    reporting an error of its own.

    \param pos Zero-based offset of the character to read.

    \return Reference to the character at \a pos.

    \pre \a pos is less than size(), checked by assert_message in debug builds.

    \sa operator[]()
  */
  [[nodiscard]] constexpr const_reference at(size_type pos) const noexcept;

  /*!
    \brief Returns the first character.

    \return Reference to the character at offset \c 0.

    \pre The view holds at least one character, checked by assert_message in debug builds.

    \sa back()
    \sa operator[]()
  */
  [[nodiscard]] constexpr const_reference front() const noexcept;

  /*!
    \brief Returns the last character.

    \return Reference to the character before the terminator.

    \pre The view holds at least one character, checked by assert_message in debug builds.

    \sa front()
    \sa operator[]()
  */
  [[nodiscard]] constexpr const_reference back() const noexcept;

  /*!
    \brief Returns the pointer to the viewed characters.

    \return Pointer to the first character, \c nullptr while the view holds no string.

    \note The characters end in a null one, so the pointer reaches a C interface unchanged.

    \sa c_str()
  */
  [[nodiscard]] constexpr const_pointer data() const noexcept;

  /*!
    \brief Returns the pointer to the viewed characters.

    Repeats data() under the name C interfaces expect.

    \return Pointer to the first character, \c nullptr while the view holds no string.

    \sa data()
  */
  [[nodiscard]] constexpr const_pointer c_str() const noexcept;

  /*!
    \brief Returns the length of the viewed string.

    \return Count of characters before the terminator, \c 0 while the view holds no string.

    \note The count is in bytes, not characters; under a multi-byte encoding the two differ.

    \sa length()
    \sa empty()
  */
  [[nodiscard]] constexpr size_type size() const noexcept;

  /*!
    \brief Returns the length of the viewed string.

    Repeats size() under the name string types use.

    \return Count of characters before the terminator, \c 0 while the view holds no string.

    \sa size()
  */
  [[nodiscard]] constexpr size_type length() const noexcept;

  /*!
    \brief Returns the longest length a view can report.

    \return Largest value size() may take, fixed by the length type.

    \note The view owns no characters, so the bound limits what it can measure rather than what it can hold.

    \sa size()
  */
  [[nodiscard]] constexpr size_type max_size() const noexcept;

  /*!
    \brief Counts the characters the viewed string encodes in UTF-8.

    Reads the viewed bytes as UTF-8 and counts a character once, whatever width it is stored in, where size() reports
    the bytes those characters occupy.

    \return Count of encoded characters, \c 0 over an empty string and \c 0 while the view holds no string.

    \pre The viewed string holds well-formed UTF-8.

    \note The count matches size() only where every character is ASCII.
    \note The count is a runtime one, unlike the rest of the view: it calls toy::utf8Len(), which is defined in a
          translation unit.

    \sa size()
  */
  [[nodiscard]] size_type utf8_size() const noexcept;

  /*!
    \brief Reports whether the view holds no character.

    \return \c true when size() is \c 0, which covers a view over an empty string and one over no string alike.

    \sa size()
  */
  [[nodiscard]] constexpr bool empty() const noexcept;

  /*!
    \brief Drops leading characters from the view.

    \param n Count of characters to drop from the front.

    \pre \a n is at most size(), checked by assert_message in debug builds.

    \post The view starts \a n characters later and reports a length shorter by \a n. The characters stay where they
          were.

    \note The terminator keeps its place, so the shortened view stays null-terminated and c_str() still reaches a C
          interface.
    \note Nothing drops trailing characters: that would leave the view without a terminator.

    \sa swap()
  */
  constexpr void remove_prefix(size_type n) noexcept;

  /*!
    \brief Exchanges the viewed strings of two views.

    \param v View to exchange the pointer and the length with.

    \post This view reads what \a v read, and \a v reads what this view read. Neither string is touched.

    \sa operator=()
  */
  constexpr void swap(StringView & v) noexcept;

  /*!
    \brief Copies up to \a count characters starting at \a pos into \a dest.

    Stops at the end of the view and writes no terminator.

    \param dest  First byte of the buffer to write into.
    \param count Most characters to copy.
    \param pos   Offset of the first character to copy (default: \c 0).

    \return Number of characters written: the smaller of \a count and size() minus \a pos.

    \pre \a pos is not greater than size(), checked by assert_message in debug builds.
    \pre \a dest has room for the characters written and does not overlap the viewed string.

    \warning A shipping build skips the check, writes nothing, and returns \c 0 when \a pos is past the end, where
             \c std::string_view throws \c std::out_of_range.

    \sa c_str()
  */
  constexpr size_type copy(value_type * dest, size_type count, size_type pos = 0) const noexcept;

  /*!
    \brief Compares this view with \a v in lexicographic order.

    Compares bytes as \c unsigned \c char up to the shorter length; when one view is a prefix of the other, the shorter
    one orders first. A \c '\\0' inside either view compares like any other byte.

    \param v View to compare with.

    \return A negative value when this view orders first, \c 0 when both hold the same characters, a positive value when
            \a v orders first.

    \note Only the sign carries meaning; the magnitude is whatever the byte comparison produced.

    \sa compare(size_t, size_t, StringView)
    \sa toy::operator<=>(StringView, StringView)
  */
  [[nodiscard]] constexpr int compare(StringView v) const noexcept;

  /*!
    \brief Compares the substring that starts at \a pos1 with \a v.

    The substring holds at most \a count1 characters and stops at the end of this view; the order is the one
    compare(StringView) defines.

    \param pos1   Offset of the first character of the substring.
    \param count1 Most characters in the substring.
    \param v      View to compare with.

    \return A negative value when the substring orders first, \c 0 when both hold the same characters, a positive value
            when \a v orders first.

    \pre \a pos1 is not greater than size(), checked by assert_message in debug builds.

    \warning A shipping build skips the check and compares an empty substring when \a pos1 is past the end, where
             \c std::string_view throws \c std::out_of_range.

    \sa compare(size_t, size_t, StringView, size_t, size_t)
  */
  [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, StringView v) const noexcept;

  /*!
    \brief Compares the substring that starts at \a pos1 with the substring of \a v that starts at \a pos2.

    Each substring stops at the end of its view; the order is the one compare(StringView) defines.

    \param pos1   Offset of the first character of this substring.
    \param count1 Most characters in this substring.
    \param v      View holding the other substring.
    \param pos2   Offset of the first character of the other substring.
    \param count2 Most characters in the other substring.

    \return A negative value when this substring orders first, \c 0 when both hold the same characters, a positive value
            when the other substring orders first.

    \pre \a pos1 is not greater than size(), checked by assert_message in debug builds.
    \pre \a pos2 is not greater than the length of \a v, checked by assert_message in debug builds.

    \warning A shipping build skips the checks and compares an empty substring for an offset past its end, where
             \c std::string_view throws \c std::out_of_range.

    \sa compare(size_t, size_t, StringView)
  */
  [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, StringView v, size_type pos2,
                                      size_type count2) const noexcept;

  /*!
    \brief Compares this view with the null-terminated byte string \a s.

    Measures \a s, then compares as compare(StringView) does.

    \param s Null-terminated byte string to compare with.

    \return A negative value when this view orders first, \c 0 when both hold the same characters, a positive value when
            \a s orders first.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa compare(size_t, size_t, const char *, size_t)
  */
  [[nodiscard]] constexpr int compare(const value_type * s) const noexcept;

  /*!
    \brief Compares the substring that starts at \a pos1 with the null-terminated byte string \a s.

    \param pos1   Offset of the first character of the substring.
    \param count1 Most characters in the substring.
    \param s      Null-terminated byte string to compare with.

    \return A negative value when the substring orders first, \c 0 when both hold the same characters, a positive value
            when \a s orders first.

    \pre \a pos1 is not greater than size(), checked by assert_message in debug builds.
    \pre \a s is non-null, checked by assert_message in debug builds.

    \warning A shipping build skips the position check and compares an empty substring when \a pos1 is past the end,
             where \c std::string_view throws \c std::out_of_range.

    \sa compare(size_t, size_t, const char *, size_t)
  */
  [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const value_type * s) const noexcept;

  /*!
    \brief Compares the substring that starts at \a pos1 with the \a count2 bytes starting at \a s.

    Reads exactly \a count2 bytes of the other side, so it needs no terminator and may hold \c '\\0'.

    \param pos1   Offset of the first character of the substring.
    \param count1 Most characters in the substring.
    \param s      First byte of the other side.
    \param count2 Number of bytes on the other side.

    \return A negative value when the substring orders first, \c 0 when both hold the same characters, a positive value
            when the other side orders first.

    \pre \a pos1 is not greater than size(), checked by assert_message in debug builds.
    \pre \a s is non-null or \a count2 is \c 0, checked by assert_message in debug builds.
    \pre The \a count2 bytes starting at \a s are readable.

    \warning A shipping build skips the position check and compares an empty substring when \a pos1 is past the end,
             where \c std::string_view throws \c std::out_of_range.

    \sa compare(const char *)
  */
  [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const value_type * s,
                                      size_type count2) const noexcept;

  /*!
    \brief Reports whether this view opens with the characters of \a sv.

    \param sv View to look for.

    \return \c true when \a sv is no longer than this view and matches its leading characters; \c true for an empty
            \a sv.

    \sa ends_with(StringView)
  */
  [[nodiscard]] constexpr bool starts_with(StringView sv) const noexcept;

  /*!
    \brief Reports whether the first character of this view is \a ch.

    \param ch Character to look for.

    \return \c true when the view is non-empty and its first character equals \a ch.

    \sa starts_with(StringView)
  */
  [[nodiscard]] constexpr bool starts_with(value_type ch) const noexcept;

  /*!
    \brief Reports whether this view opens with the null-terminated byte string \a s.

    \param s Null-terminated byte string to look for.

    \return \c true when this view opens with the characters of \a s; \c true for an empty one.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa starts_with(StringView)
  */
  [[nodiscard]] constexpr bool starts_with(const value_type * s) const noexcept;

  /*!
    \brief Reports whether this view closes with the characters of \a sv.

    \param sv View to look for.

    \return \c true when \a sv is no longer than this view and matches its trailing characters; \c true for an empty
            \a sv.

    \sa starts_with(StringView)
  */
  [[nodiscard]] constexpr bool ends_with(StringView sv) const noexcept;

  /*!
    \brief Reports whether the last character of this view is \a ch.

    \param ch Character to look for.

    \return \c true when the view is non-empty and its last character equals \a ch.

    \sa ends_with(StringView)
  */
  [[nodiscard]] constexpr bool ends_with(value_type ch) const noexcept;

  /*!
    \brief Reports whether this view closes with the null-terminated byte string \a s.

    \param s Null-terminated byte string to look for.

    \return \c true when this view closes with the characters of \a s; \c true for an empty one.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa ends_with(StringView)
  */
  [[nodiscard]] constexpr bool ends_with(const value_type * s) const noexcept;

  /*!
    \brief Reports whether the characters of \a sv occur anywhere in this view.

    \param sv View to look for.

    \return \c true when find(StringView, size_t) finds \a sv; \c true for an empty one.

    \sa find(StringView, size_t)
  */
  [[nodiscard]] constexpr bool contains(StringView sv) const noexcept;

  /*!
    \brief Reports whether \a ch occurs anywhere in this view.

    \param ch Character to look for.

    \return \c true when find(char, size_t) finds \a ch.

    \sa find(char, size_t)
  */
  [[nodiscard]] constexpr bool contains(value_type ch) const noexcept;

  /*!
    \brief Reports whether the null-terminated byte string \a s occurs anywhere in this view.

    \param s Null-terminated byte string to look for.

    \return \c true when find(const char *, size_t) finds \a s; \c true for an empty one.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa find(const char *, size_t)
  */
  [[nodiscard]] constexpr bool contains(const value_type * s) const noexcept;

  /*!
    \brief Finds the first position at or after \a pos where the characters of \a v start.

    Matches the characters of \a v in order and next to each other; find_first_of() matches any one of them.

    \param v   View to look for.
    \param pos Offset the search starts from (default: \c 0).

    \return Offset where the first match starts, or \ref toy::StringView::npos when there is none. An empty pattern
            matches at \a pos while \a pos is not greater than size().

    \sa find(const char *, size_t, size_t)
    \sa rfind(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type find(StringView v, size_type pos = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of \a ch at or after \a pos.

    \param ch  Character to look for.
    \param pos Offset the search starts from (default: \c 0).

    \return Offset of the first \a ch at or after \a pos, or \ref toy::StringView::npos when there is none; a \a pos at
            or past size() always gives \ref toy::StringView::npos.

    \sa rfind(char, size_t)
  */
  [[nodiscard]] constexpr size_type find(value_type ch, size_type pos = 0) const noexcept;

  /*!
    \brief Finds the first position at or after \a pos where the \a count bytes starting at \a s start.

    Reads exactly \a count bytes of the pattern, so it needs no terminator and may hold \c '\\0'.

    \param s     First byte of the pattern.
    \param pos   Offset the search starts from.
    \param count Number of bytes in the pattern.

    \return Offset where the first match starts, or \ref toy::StringView::npos when there is none. An empty pattern
            matches at \a pos while \a pos is not greater than size().

    \pre \a s is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a s are readable.

    \sa find(const char *, size_t)
  */
  [[nodiscard]] constexpr size_type find(const value_type * s, size_type pos, size_type count) const noexcept;

  /*!
    \brief Finds the first position at or after \a pos where the null-terminated byte string \a s starts.

    Measures \a s, then searches as find(const char *, size_t, size_t) does.

    \param s   Null-terminated byte string to look for.
    \param pos Offset the search starts from (default: \c 0).

    \return Offset where the first match starts, or \ref toy::StringView::npos when there is none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa find(const char *, size_t, size_t)
  */
  [[nodiscard]] constexpr size_type find(const value_type * s, size_type pos = 0) const noexcept;

  /*!
    \brief Finds the last position at or before \a pos where the characters of \a v start.

    Matches the characters of \a v in order and next to each other; find_last_of() matches any one of them.

    \param v   View to look for.
    \param pos Last offset a match may start at (default: \ref toy::StringView::npos, which searches the whole view).

    \return Offset where the last match starts, or \ref toy::StringView::npos when there is none. An empty pattern
            matches at the smaller of \a pos and size().

    \sa rfind(const char *, size_t, size_t)
    \sa find(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type rfind(StringView v, size_type pos = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of \a ch at or before \a pos.

    \param ch  Character to look for.
    \param pos Last offset a match may start at (default: \ref toy::StringView::npos, which searches the whole view).

    \return Offset of the last \a ch at or before \a pos, or \ref toy::StringView::npos when there is none.

    \sa find(char, size_t)
  */
  [[nodiscard]] constexpr size_type rfind(value_type ch, size_type pos = npos) const noexcept;

  /*!
    \brief Finds the last position at or before \a pos where the \a count bytes starting at \a s start.

    Reads exactly \a count bytes of the pattern, so it needs no terminator and may hold \c '\\0'.

    \param s     First byte of the pattern.
    \param pos   Last offset a match may start at; any offset past the view covers all of it.
    \param count Number of bytes in the pattern.

    \return Offset where the last match starts, or \ref toy::StringView::npos when there is none. An empty pattern
            matches at the smaller of \a pos and size().

    \pre \a s is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a s are readable.

    \sa rfind(const char *, size_t)
  */
  [[nodiscard]] constexpr size_type rfind(const value_type * s, size_type pos, size_type count) const noexcept;

  /*!
    \brief Finds the last position at or before \a pos where the null-terminated byte string \a s starts.

    Measures \a s, then searches as rfind(const char *, size_t, size_t) does.

    \param s   Null-terminated byte string to look for.
    \param pos Last offset a match may start at (default: \ref toy::StringView::npos, which searches the whole view).

    \return Offset where the last match starts, or \ref toy::StringView::npos when there is none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa rfind(const char *, size_t, size_t)
  */
  [[nodiscard]] constexpr size_type rfind(const value_type * s, size_type pos = npos) const noexcept;

  /*!
    \brief Finds the first character at or after \a pos that belongs to the characters of \a v.

    Reads the characters of \a v as a set, so their order carries no meaning; find() matches them as a sequence.

    \param v   View holding the set.
    \param pos Offset the search starts from (default: \c 0).

    \return Offset of the first character that belongs to the set, or \ref toy::StringView::npos when there is none. An
            empty set matches nothing.

    \sa find_first_of(const char *, size_t, size_t)
    \sa find_last_of(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type find_first_of(StringView v, size_type pos = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of \a ch at or after \a pos, as find(char, size_t) does.

    \param ch  Character to look for.
    \param pos Offset the search starts from (default: \c 0).

    \return Offset of the first \a ch at or after \a pos, or \ref toy::StringView::npos when there is none.

    \sa find_first_of(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type find_first_of(value_type ch, size_type pos = 0) const noexcept;

  /*!
    \brief Finds the first character at or after \a pos that belongs to the \a count bytes starting at \a s.

    \param s     First byte of the set.
    \param pos   Offset the search starts from.
    \param count Number of bytes in the set.

    \return Offset of the first character that belongs to the set, or \ref toy::StringView::npos when there is none. An
            empty set matches nothing.

    \pre \a s is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a s are readable.

    \sa find_first_of(const char *, size_t)
  */
  [[nodiscard]] constexpr size_type find_first_of(const value_type * s, size_type pos, size_type count) const noexcept;

  /*!
    \brief Finds the first character at or after \a pos that belongs to the null-terminated byte string \a s.

    Measures \a s, then searches as find_first_of(const char *, size_t, size_t) does.

    \param s   Null-terminated byte string holding the set.
    \param pos Offset the search starts from (default: \c 0).

    \return Offset of the first character that belongs to the set, or \ref toy::StringView::npos when there is none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa find_first_of(const char *, size_t, size_t)
  */
  [[nodiscard]] constexpr size_type find_first_of(const value_type * s, size_type pos = 0) const noexcept;

  /*!
    \brief Finds the last character at or before \a pos that belongs to the characters of \a v.

    Reads the characters of \a v as a set, so their order carries no meaning; rfind() matches them as a sequence.

    \param v   View holding the set.
    \param pos Last offset the search considers (default: \ref toy::StringView::npos, which searches the whole view).

    \return Offset of the last character that belongs to the set, or \ref toy::StringView::npos when there is none. An
            empty set matches nothing.

    \sa find_last_of(const char *, size_t, size_t)
    \sa find_first_of(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type find_last_of(StringView v, size_type pos = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of \a ch at or before \a pos, as rfind(char, size_t) does.

    \param ch  Character to look for.
    \param pos Last offset the search considers (default: \ref toy::StringView::npos, which searches the whole view).

    \return Offset of the last \a ch at or before \a pos, or \ref toy::StringView::npos when there is none.

    \sa find_last_of(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type find_last_of(value_type ch, size_type pos = npos) const noexcept;

  /*!
    \brief Finds the last character at or before \a pos that belongs to the \a count bytes starting at \a s.

    \param s     First byte of the set.
    \param pos   Last offset the search considers; any offset past the view covers all of it.
    \param count Number of bytes in the set.

    \return Offset of the last character that belongs to the set, or \ref toy::StringView::npos when there is none. An
            empty set matches nothing.

    \pre \a s is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a s are readable.

    \sa find_last_of(const char *, size_t)
  */
  [[nodiscard]] constexpr size_type find_last_of(const value_type * s, size_type pos, size_type count) const noexcept;

  /*!
    \brief Finds the last character at or before \a pos that belongs to the null-terminated byte string \a s.

    Measures \a s, then searches as find_last_of(const char *, size_t, size_t) does.

    \param s   Null-terminated byte string holding the set.
    \param pos Last offset the search considers (default: \ref toy::StringView::npos, which searches the whole view).

    \return Offset of the last character that belongs to the set, or \ref toy::StringView::npos when there is none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa find_last_of(const char *, size_t, size_t)
  */
  [[nodiscard]] constexpr size_type find_last_of(const value_type * s, size_type pos = npos) const noexcept;

  /*!
    \brief Finds the first character at or after \a pos that does not belong to the characters of \a v.

    \param v   View holding the set.
    \param pos Offset the search starts from (default: \c 0).

    \return Offset of the first character that does not belong to the set, or \ref toy::StringView::npos when there is
            none. No character belongs to an empty set, so the result is then \a pos while \a pos is less than size().

    \sa find_first_not_of(const char *, size_t, size_t)
    \sa find_last_not_of(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type find_first_not_of(StringView v, size_type pos = 0) const noexcept;

  /*!
    \brief Finds the first character at or after \a pos that differs from \a ch.

    \param ch  Character to skip.
    \param pos Offset the search starts from (default: \c 0).

    \return Offset of the first character other than \a ch at or after \a pos, or \ref toy::StringView::npos when there
            is none.

    \sa find_first_not_of(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type find_first_not_of(value_type ch, size_type pos = 0) const noexcept;

  /*!
    \brief Finds the first character at or after \a pos that does not belong to the \a count bytes starting at \a s.

    \param s     First byte of the set.
    \param pos   Offset the search starts from.
    \param count Number of bytes in the set.

    \return Offset of the first character that does not belong to the set, or \ref toy::StringView::npos when there is
            none. No character belongs to an empty set, so the result is then \a pos while \a pos is less than size().

    \pre \a s is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a s are readable.

    \sa find_first_not_of(const char *, size_t)
  */
  [[nodiscard]] constexpr size_type find_first_not_of(const value_type * s, size_type pos,
                                                      size_type count) const noexcept;

  /*!
    \brief Finds the first character at or after \a pos that does not belong to the null-terminated byte string \a s.

    Measures \a s, then searches as find_first_not_of(const char *, size_t, size_t) does.

    \param s   Null-terminated byte string holding the set.
    \param pos Offset the search starts from (default: \c 0).

    \return Offset of the first character that does not belong to the set, or \ref toy::StringView::npos when there is
            none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa find_first_not_of(const char *, size_t, size_t)
  */
  [[nodiscard]] constexpr size_type find_first_not_of(const value_type * s, size_type pos = 0) const noexcept;

  /*!
    \brief Finds the last character at or before \a pos that does not belong to the characters of \a v.

    \param v   View holding the set.
    \param pos Last offset the search considers (default: \ref toy::StringView::npos, which searches the whole view).

    \return Offset of the last character that does not belong to the set, or \ref toy::StringView::npos when there is
            none. No character belongs to an empty set, so the result is then the smaller of \a pos and size() minus
            one, or \ref toy::StringView::npos for an empty view.

    \sa find_last_not_of(const char *, size_t, size_t)
    \sa find_first_not_of(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type find_last_not_of(StringView v, size_type pos = npos) const noexcept;

  /*!
    \brief Finds the last character at or before \a pos that differs from \a ch.

    \param ch  Character to skip.
    \param pos Last offset the search considers (default: \ref toy::StringView::npos, which searches the whole view).

    \return Offset of the last character other than \a ch at or before \a pos, or \ref toy::StringView::npos when there
            is none.

    \sa find_last_not_of(StringView, size_t)
  */
  [[nodiscard]] constexpr size_type find_last_not_of(value_type ch, size_type pos = npos) const noexcept;

  /*!
    \brief Finds the last character at or before \a pos that does not belong to the \a count bytes starting at \a s.

    \param s     First byte of the set.
    \param pos   Last offset the search considers; any offset past the view covers all of it.
    \param count Number of bytes in the set.

    \return Offset of the last character that does not belong to the set, or \ref toy::StringView::npos when there is
            none. No character belongs to an empty set, so the result is then the smaller of \a pos and size() minus
            one, or \ref toy::StringView::npos for an empty view.

    \pre \a s is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a s are readable.

    \sa find_last_not_of(const char *, size_t)
  */
  [[nodiscard]] constexpr size_type find_last_not_of(const value_type * s, size_type pos,
                                                     size_type count) const noexcept;

  /*!
    \brief Finds the last character at or before \a pos that does not belong to the null-terminated byte string \a s.

    Measures \a s, then searches as find_last_not_of(const char *, size_t, size_t) does.

    \param s   Null-terminated byte string holding the set.
    \param pos Last offset the search considers (default: \ref toy::StringView::npos, which searches the whole view).

    \return Offset of the last character that does not belong to the set, or \ref toy::StringView::npos when there is
            none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa find_last_not_of(const char *, size_t, size_t)
  */
  [[nodiscard]] constexpr size_type find_last_not_of(const value_type * s, size_type pos = npos) const noexcept;

  /// Offset no character sits at, returned by every search that matches nothing
  static constexpr const size_type npos = -1;

  /*!
    \brief Reports whether two views read the same characters.

    Compares the lengths first and the bytes only when those match. Either side converts from a null-terminated byte
    string, so a literal compares against a view directly. A literal \c nullptr meets the deleted constructor and
    fails to compile.

    \param lhs View on the left of the operator.
    \param rhs View on the right of the operator.

    \return \c true when both views hold the same length and the same bytes, \c false otherwise.

    \note The compiler synthesizes \c != from this operator.
    \note A view over no string and a view over an empty string both report a length of \c 0, which makes them equal.

    \sa operator<=>()
    \sa compare(StringView)
  */
  friend constexpr bool operator==(StringView lhs, StringView rhs) noexcept;

  /*!
    \brief Orders two views lexicographically.

    Reads the bytes in order and settles a tie on the lengths, so a string that starts another one orders before it.
    Either side converts from a null-terminated byte string, as equality does.

    \param lhs View on the left of the operator.
    \param rhs View on the right of the operator.

    \return \c std::strong_ordering::less when \a lhs orders first, \c std::strong_ordering::equal when both hold the
            same characters, \c std::strong_ordering::greater when \a rhs orders first.

    \note The compiler synthesizes \c <, \c <=, \c >, and \c >= from this operator.
    \note Bytes order by their \c unsigned \c char value, so the order holds on a target whose plain \c char is signed.
    \note The order consults no locale, which makes it identical across runs and targets.

    \sa operator==()
    \sa compare(StringView)
  */
  friend constexpr traits_type::comparison_category operator<=>(StringView lhs, StringView rhs) noexcept;

private:
  /// First character of the viewed string, \c nullptr while the view holds none
  const value_type * _data{nullptr};

  /// Length of the viewed string in bytes, the terminator excluded
  size_type _size{0};
};

} // namespace toy

#endif // INCLUDE_CORE_STRING_VIEW_HPP_
