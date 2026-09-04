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
  \brief  Non-owning view over a null-terminated byte string that carries its length.

  Defines \ref toy::CStringView, a read-only view that holds the pointer to a null-terminated byte string and the
  length measured when the view was built. Used where a call site hands the string to a C interface and still needs its
  length without a second scan for the terminator.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_C_STRING_VIEW_HPP_
#define INCLUDE_CORE_C_STRING_VIEW_HPP_

#include "string_like.hpp"

namespace toy {

/*!
  \class CStringView
  \brief Read-only view over a null-terminated byte string that keeps the pointer and its measured length.

  Stores the pointer the caller supplies and copies neither the characters nor the terminator, so the viewed string has
  to outlive the view. Null-termination is part of the contract, unlike \c std::string_view: the pointer stays valid
  input for a C interface, and the length needs no second scan.

  \section c_string_view_features Key Features

  * **Null-terminated**: every viewed string ends in a null character, so its pointer reaches a C interface unchanged.
  * **Length measured once**: the pointer constructor scans for the terminator, and the view holds the result.
  * **Constexpr support**: every operation evaluates in a constant expression.
  * **No allocation**: the view holds a pointer and a length, and owns no characters.
  * **Range access**: forward and reverse iterator pairs, so a range-based \c for and the standard algorithms read the
    view.
  * **Search set**: substring, character, and character-set searches in both directions, each matching the
    \c std::string_view contract.
  * **Ordered comparison**: equality and a three-way order over the bytes, against another view or a null-terminated
    byte string.
  * **Type safety**: construction from \c nullptr is deleted, so the null case fails to compile.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section c_string_view_usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::CStringView name("player");
  constexpr toy::CStringView alias(name);

  size_t letters = 0;
  for (const char character : alias) {
    letters += static_cast<size_t>(character != ' ');
  }

  const char last = *name.rbegin();

  constexpr toy::CStringView path("assets/player.png");

  const size_t extension = path.rfind('.');
  const bool   named     = path.contains(name);
  const bool   isPlayer  = name == "player";
  \endcode

  \section c_string_view_performance Performance Characteristics

  * **Construction from a pointer**: O(n) in the length of the string, one scan for the terminator.
  * **Default construction and copying**: O(1).
  * **Iterator access**: O(1); each iterator is a pointer into the viewed string, or a reverse adaptor over one.
  * **Substring search**: O(n * m) in the length of the view and the length of the needle; nothing is indexed or
    cached between calls.
  * **Character-set search**: O(n * m) in the length of the view and the size of the set.
  * **Comparison**: O(n) in the shorter of the two lengths; equality stops on a length mismatch before reading a
    character.
  * **Memory usage**: one pointer and one length, 16 bytes on a 64-bit target and 8 bytes on a 32-bit one.

  \section c_string_view_safety Safety Guarantees

  * **Contracts**: the pointer constructor checks its argument against \c nullptr with assert_message in debug builds.
    A shipping build skips the check and stores the null pointer with a length of \c 0.
  * **Lifetime**: the characters belong to whoever created them; the view neither owns them nor extends their
    lifetime.
  * **Type safety**: the deleted \c nullptr_t constructor rejects a literal \c nullptr during compilation.
  * **Iterator validity**: an iterator stays valid while the string it points into lives. Assigning to the view repoints
    the view alone and leaves an outstanding iterator on the previous string.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section c_string_view_compatibility Compatibility

  * Requires C++20 for the conditional \c explicit specifier on the pointer constructor.
  * Header-only, and no operation allocates or calls into the platform, so the type suits embedded and retro targets.

  \note The length counts bytes, not characters; under a multi-byte encoding the two differ.
  \note A default-constructed view holds a null pointer, not a pointer to an empty string.

  \warning A view built over a temporary dangles as soon as that temporary dies at the end of the full expression.

  \sa \ref toy::StringLike
*/
class CStringView {
public:
  /// Character operations the view uses to measure and compare
  using traits_type            = char_traits<char>;
  /// Type of the viewed characters
  using value_type             = char;
  /// Mutable character pointer, present for parity with \c std::string_view; the view exposes no mutable access
  using pointer                = char *;
  /// Pointer to a viewed character
  using const_pointer          = const char *;
  /// Mutable character reference, present for parity with \c std::string_view; the view exposes no mutable access
  using reference              = char &;
  /// Reference to a viewed character
  using const_reference        = const char &;
  /// Iterator over the viewed characters
  using const_iterator         = const_pointer;
  /// Read-only iterator alias; repeats \ref toy::CStringView::const_iterator
  using iterator               = const_iterator;
  /// Iterator walking the viewed characters back to front
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  /// Read-only reverse iterator alias; repeats \ref toy::CStringView::const_reverse_iterator
  using reverse_iterator       = const_reverse_iterator;
  /// Unsigned type the length is measured in
  using size_type              = size_t;
  /// Signed type for the distance between two iterators
  using difference_type        = ptrdiff_t;

  /*!
    \brief Builds an empty view.

    \post The view holds a null pointer and a length of \c 0.
  */
  constexpr CStringView() noexcept = default;

  /*!
    \brief Builds a view over the same string as \a other.

    \param other View to copy the pointer and the length from.

    \post Both views read the same characters; neither owns them.
  */
  constexpr CStringView(const CStringView & other) noexcept = default;

  /*!
    \brief Builds a view over a null-terminated byte string and measures its length.

    Converts implicitly, so a string literal or a \c const \c char \c * argument becomes a view at the call site. Only
    the pointer and the length are stored; the characters stay where the caller put them.

    \param string Null-terminated byte string to view.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre \a string outlives the view and keeps its terminator in place.

    \post The view holds \a string and its length in bytes, the terminator excluded.

    \sa CStringView()
  */
  constexpr explicit(false) CStringView(const char * string) noexcept;

  /*!
    \brief Deleted: a null pointer names no string to view.

    Rejects a literal \c nullptr during compilation, ahead of the debug check the pointer constructor performs.

    \sa CStringView(const char *)
  */
  CStringView(nullptr_t) = delete;

  /*!
    \brief Makes this view read the same string as \a view.

    \param view View to copy the pointer and the length from.

    \return Reference to this view.

    \post Both views read the same characters. The string this view held before stays untouched, since the view owns
          nothing.

    \sa CStringView(const CStringView &)
  */
  constexpr CStringView & operator=(const CStringView & view) noexcept = default;

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
  constexpr void swap(CStringView & v) noexcept;

  /*!
    \brief Copies characters into caller storage.

    \param dest  Start of the buffer the characters are written to.
    \param count Greatest count of characters to write.
    \param pos   Offset in the viewed string to start reading at (default: \c 0).

    \return Count of characters written: \a count, or what is left after \a pos, whichever is smaller, and
            \c 0 when \a pos is size().

    \pre \a pos is at most size(), checked by assert_message in debug builds.
    \pre \a dest addresses at least as many characters as the call returns.

    \note The call writes no terminator; whether the copy needs one is the caller's decision.
  */
  constexpr size_type copy(value_type * dest, size_type count, size_type pos = 0) const noexcept;

  /*!
    \brief Orders this view against another.

    Reads the characters in order and settles a tie on the lengths, so a string that starts another one orders before
    it.

    \param v View to order against.

    \return \c 0 when both hold the same characters, a negative value when this view orders first, a positive value when
            \a v does.

    \note Only the sign carries meaning; the magnitude is whatever the character comparison produced.

    \sa starts_with()
    \sa ends_with()
  */
  [[nodiscard]] constexpr int compare(CStringView v) const noexcept;

  /*!
    \brief Orders a part of this view against another view.

    \param pos1   Offset in this view the part starts at.
    \param count1 Count of characters the part covers.
    \param v      View to order the part against.

    \return \c 0 when the part and \a v hold the same characters, a negative value when the part orders first, a
            positive value when \a v does.

    \pre \a pos1 and \a count1 name a range inside this view, checked by assert_message in debug builds.

    \sa compare(CStringView)
  */
  [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, CStringView v) const noexcept;

  /*!
    \brief Orders a part of this view against a part of another view.

    \param pos1   Offset in this view the first part starts at.
    \param count1 Count of characters the first part covers.
    \param v      View the second part is taken from.
    \param pos2   Offset in \a v the second part starts at.
    \param count2 Count of characters the second part covers.

    \return \c 0 when both parts hold the same characters, a negative value when the first orders before the second,
            a positive value when it orders after.

    \pre \a pos1 and \a count1 name a range inside this view, checked by assert_message in debug builds.
    \pre \a pos2 and \a count2 name a range inside \a v, checked by assert_message in debug builds.

    \sa compare(CStringView)
  */
  [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, CStringView v, size_type pos2,
                                      size_type count2) const noexcept;

  /*!
    \brief Orders this view against a null-terminated byte string.

    Measures \a s and orders it the way compare(CStringView) does.

    \param s Null-terminated byte string to order against.

    \return \c 0 when both hold the same characters, a negative value when this view orders first, a positive value when
            \a s does.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa compare(CStringView)
  */
  [[nodiscard]] constexpr int compare(const value_type * s) const noexcept;

  /*!
    \brief Orders a part of this view against a null-terminated byte string.

    \param pos1   Offset in this view the part starts at.
    \param count1 Count of characters the part covers.
    \param s      Null-terminated byte string to order the part against.

    \return \c 0 when the part and \a s hold the same characters, a negative value when the part orders first,
            a positive value when \a s does.

    \pre \a pos1 and \a count1 name a range inside this view, checked by assert_message in debug builds.
    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa compare(size_type, size_type, CStringView)
  */
  [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const value_type * s) const noexcept;

  /*!
    \brief Orders a part of this view against a counted part of a byte string.

    Reads \a count2 characters from \a s instead of scanning it for a terminator, so \a s may hold null characters and
    need not end in one.

    \param pos1   Offset in this view the part starts at.
    \param count1 Count of characters the part covers.
    \param s      Byte string the compared characters are read from.
    \param count2 Count of characters to read from \a s.

    \return \c 0 when both parts hold the same characters, a negative value when the part of this view orders first,
            a positive value when the part of \a s does.

    \pre \a pos1 and \a count1 name a range inside this view, checked by assert_message in debug builds.
    \pre \a s is non-null, checked by assert_message in debug builds.
    \pre \a s addresses at least \a count2 characters.

    \sa compare(size_type, size_type, CStringView, size_type, size_type)
  */
  [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const value_type * s,
                                      size_type count2) const noexcept;

  /*!
    \brief Reports whether the viewed string opens with given characters.

    \param sv Characters the viewed string has to open with.

    \return \c true when the leading characters match \a sv, \c false when they differ or when \a sv is the longer
            string.

    \note An empty \a sv matches every string.

    \sa ends_with()
    \sa compare(size_type, size_type, CStringView)
  */
  [[nodiscard]] constexpr bool starts_with(CStringView sv) const noexcept;

  /*!
    \brief Reports whether the viewed string opens with a given character.

    \param ch Character the viewed string has to open with.

    \return \c true when the first character is \a ch, \c false when it differs or when the view holds no character.

    \sa front()
    \sa ends_with()
  */
  [[nodiscard]] constexpr bool starts_with(value_type ch) const noexcept;

  /*!
    \brief Reports whether the viewed string opens with given characters.

    Measures \a s and matches it the way starts_with(CStringView) does.

    \param s Null-terminated byte string the viewed string has to open with.

    \return \c true when the leading characters match \a s, \c false when they differ or when \a s is the longer string.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa starts_with(CStringView)
  */
  [[nodiscard]] constexpr bool starts_with(const value_type * s) const noexcept;

  /*!
    \brief Reports whether the viewed string closes with given characters.

    \param sv Characters the viewed string has to close with.

    \return \c true when the trailing characters match \a sv, \c false when they differ or when \a sv is the longer
            string.

    \note An empty \a sv matches every string.

    \sa starts_with()
    \sa compare(size_type, size_type, CStringView)
  */
  [[nodiscard]] constexpr bool ends_with(CStringView sv) const noexcept;

  /*!
    \brief Reports whether the viewed string closes with a given character.

    \param ch Character the viewed string has to close with.

    \return \c true when the last character is \a ch, \c false when it differs or when the view holds no character.

    \sa back()
    \sa starts_with()
  */
  [[nodiscard]] constexpr bool ends_with(value_type ch) const noexcept;

  /*!
    \brief Reports whether the viewed string closes with given characters.

    Measures \a s and matches it the way ends_with(CStringView) does.

    \param s Null-terminated byte string the viewed string has to close with.

    \return \c true when the trailing characters match \a s, \c false when they differ or when \a s is the longer
            string.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa ends_with(CStringView)
  */
  [[nodiscard]] constexpr bool ends_with(const value_type * s) const noexcept;

  /*!
    \brief Reports whether given characters appear anywhere in the viewed string.

    \param sv Characters to look for, in order and next to each other.

    \return \c true when find() locates \a sv, \c false when it does not.

    \note An empty \a sv appears in every string.

    \sa find(CStringView, size_type)
  */
  [[nodiscard]] constexpr bool contains(CStringView sv) const noexcept;

  /*!
    \brief Reports whether a given character appears anywhere in the viewed string.

    \param ch Character to look for.

    \return \c true when find() locates \a ch, \c false when it does not.

    \sa find(value_type, size_type)
  */
  [[nodiscard]] constexpr bool contains(value_type ch) const noexcept;

  /*!
    \brief Reports whether given characters appear anywhere in the viewed string.

    Measures \a s and looks for it the way contains(CStringView) does.

    \param s Null-terminated byte string to look for.

    \return \c true when find() locates \a s, \c false when it does not.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \sa contains(CStringView)
  */
  [[nodiscard]] constexpr bool contains(const value_type * s) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds given characters.

    Matches the characters of \a v in order and next to each other. find_first_of() instead matches any single character
    \a v holds.

    \param v   Characters to match, in order.
    \param pos Offset to start the search at (default: \c 0).

    \return Offset of the first match at or after \a pos, \ref toy::CStringView::npos when the string holds none.

    \note An empty \a v matches at \a pos, and matches nothing once \a pos is past the length.

    \sa rfind(CStringView, size_type)
    \sa find_first_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find(CStringView v, size_type pos = 0) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds a given character.

    \param ch  Character to match.
    \param pos Offset to start the search at (default: \c 0).

    \return Offset of the first \a ch at or after \a pos, \ref toy::CStringView::npos when the string holds none.

    \sa rfind(value_type, size_type)
    \sa contains(value_type)
  */
  [[nodiscard]] constexpr size_type find(value_type ch, size_type pos = 0) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds given characters.

    Reads \a count characters from \a s instead of scanning it for a terminator, so \a s may hold null characters and
    need not end in one.

    \param s     Characters to match, in order.
    \param pos   Offset to start the search at.
    \param count Count of characters to read from \a s.

    \return Offset of the first match at or after \a pos, \ref toy::CStringView::npos when the string holds none.

    \pre \a s addresses at least \a count characters.

    \note A \a count of \c 0 matches at \a pos, and matches nothing once \a pos is past the length.

    \sa find(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find(const value_type * s, size_type pos, size_type count) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds given characters.

    Measures \a s and matches it the way find(CStringView, size_type) does.

    \param s   Characters to match, in order.
    \param pos Offset to start the search at (default: \c 0).

    \return Offset of the first match at or after \a pos, \ref toy::CStringView::npos when the string holds none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \note An empty \a s matches at \a pos, and matches nothing once \a pos is past the length.

    \sa find(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find(const value_type * s, size_type pos = 0) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds given characters.

    Matches the characters of \a v in order and next to each other, and reports the match that starts latest. The cap
    applies to where a match begins, not to where it ends.

    \param v   Characters to match, in order.
    \param pos Greatest offset a match may start at
               (default: \ref toy::CStringView::npos, which searches the whole string).

    \return Offset of the last match starting at or before \a pos, \ref toy::CStringView::npos when the string holds
            none.

    \note An empty \a v matches at \a pos, capped at the length.

    \sa find(CStringView, size_type)
    \sa find_last_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type rfind(CStringView v, size_type pos = npos) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds a given character.

    \param ch  Character to match.
    \param pos Greatest offset the character may sit at (default: \ref toy::CStringView::npos, which searches the whole
               string).

    \return Offset of the last \a ch at or before \a pos, \ref toy::CStringView::npos when the string holds none.

    \sa find(value_type, size_type)
    \sa find_last_of(value_type, size_type)
  */
  [[nodiscard]] constexpr size_type rfind(value_type ch, size_type pos = npos) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds given characters.

    Reads \a count characters from \a s instead of scanning it for a terminator, so \a s may hold null characters and
    need not end in one.

    \param s     Characters to match, in order.
    \param pos   Greatest offset a match may start at.
    \param count Count of characters to read from \a s.

    \return Offset of the last match starting at or before \a pos, \ref toy::CStringView::npos when the string holds
            none.

    \pre \a s addresses at least \a count characters.

    \note A \a count of \c 0 matches at \a pos, capped at the length.

    \sa rfind(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type rfind(const value_type * s, size_type pos, size_type count) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds given characters.

    Measures \a s and matches it the way rfind(CStringView, size_type) does.

    \param s   Characters to match, in order.
    \param pos Greatest offset a match may start at
               (default: \ref toy::CStringView::npos, which searches the whole string).

    \return Offset of the last match starting at or before \a pos, \ref toy::CStringView::npos when the string holds
            none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \note An empty \a s matches at \a pos, capped at the length.

    \sa rfind(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type rfind(const value_type * s, size_type pos = npos) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds any character of a given set.

    Reads \a v as a set, so the order of its characters and their adjacency carry no meaning. find() instead matches
    \a v as a sequence.

    \param v   Characters the match may be any one of.
    \param pos Offset to start the search at (default: \c 0).

    \return Offset of the first character at or after \a pos that \a v holds, \ref toy::CStringView::npos when there is
            none.

    \note An empty \a v holds no character to match, so the search reports nothing.

    \sa find_last_of(CStringView, size_type)
    \sa find_first_not_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_first_of(CStringView v, size_type pos = 0) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds a given character.

    Repeats find(value_type, size_type): a set of one character is that character.

    \param ch  Character to match.
    \param pos Offset to start the search at (default: \c 0).

    \return Offset of the first \a ch at or after \a pos, \ref toy::CStringView::npos when the string holds none.

    \sa find(value_type, size_type)
    \sa find_last_of(value_type, size_type)
  */
  [[nodiscard]] constexpr size_type find_first_of(value_type ch, size_type pos = 0) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds any character of a given set.

    Reads \a count characters from \a s instead of scanning it for a terminator, so \a s may hold null characters and
    need not end in one.

    \param s     Characters the match may be any one of.
    \param pos   Offset to start the search at.
    \param count Count of characters to read from \a s.

    \return Offset of the first character at or after \a pos that \a s holds, \ref toy::CStringView::npos when there is
            none.

    \pre \a s addresses at least \a count characters.

    \note A \a count of \c 0 holds no character to match, so the search reports nothing.

    \sa find_first_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_first_of(const value_type * s, size_type pos, size_type count) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds any character of a given set.

    Measures \a s and matches it the way find_first_of(CStringView, size_type) does.

    \param s   Characters the match may be any one of.
    \param pos Offset to start the search at (default: \c 0).

    \return Offset of the first character at or after \a pos that \a s holds, \ref toy::CStringView::npos when there is
            none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \note An empty \a s holds no character to match, so the search reports nothing.

    \sa find_first_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_first_of(const value_type * s, size_type pos = 0) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds any character of a given set.

    Reads \a v as a set, so the order of its characters and their adjacency carry no meaning. rfind() instead matches \a
    v as a sequence.

    \param v   Characters the match may be any one of.
    \param pos Greatest offset the match may sit at
               (default: \ref toy::CStringView::npos, which searches the whole string).

    \return Offset of the last character at or before \a pos that \a v holds, \ref toy::CStringView::npos when there is
            none.

    \note An empty \a v holds no character to match, so the search reports nothing.

    \sa find_first_of(CStringView, size_type)
    \sa find_last_not_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_last_of(CStringView v, size_type pos = npos) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds a given character.

    Repeats rfind(value_type, size_type): a set of one character is that character.

    \param ch  Character to match.
    \param pos Greatest offset the match may sit at
               (default: \ref toy::CStringView::npos, which searches the whole string).

    \return Offset of the last \a ch at or before \a pos, \ref toy::CStringView::npos when the string holds none.

    \sa rfind(value_type, size_type)
    \sa find_first_of(value_type, size_type)
  */
  [[nodiscard]] constexpr size_type find_last_of(value_type ch, size_type pos = npos) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds any character of a given set.

    Reads \a count characters from \a s instead of scanning it for a terminator, so \a s may hold null characters and
    need not end in one.

    \param s     Characters the match may be any one of.
    \param pos   Greatest offset the match may sit at.
    \param count Count of characters to read from \a s.

    \return Offset of the last character at or before \a pos that \a s holds, \ref toy::CStringView::npos when there is
            none.

    \pre \a s addresses at least \a count characters.

    \note A \a count of \c 0 holds no character to match, so the search reports nothing.

    \sa find_last_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_last_of(const value_type * s, size_type pos, size_type count) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds any character of a given set.

    Measures \a s and matches it the way find_last_of(CStringView, size_type) does.

    \param s   Characters the match may be any one of.
    \param pos Greatest offset the match may sit at
               (default: \ref toy::CStringView::npos, which searches the whole string).

    \return Offset of the last character at or before \a pos that \a s holds, \ref toy::CStringView::npos when there is
            none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \note An empty \a s holds no character to match, so the search reports nothing.

    \sa find_last_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_last_of(const value_type * s, size_type pos = npos) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds a character outside a given set.

    Reads \a v as a set and reports the first character the set leaves out, which is what trims a leading run of the
    characters \a v holds.

    \param v   Characters the match may not be any of.
    \param pos Offset to start the search at (default: \c 0).

    \return Offset of the first character at or after \a pos that \a v does not hold, \ref toy::CStringView::npos when
            there is none.

    \note An empty \a v leaves every character out, so the search reports \a pos while that offset is inside the string.

    \sa find_last_not_of(CStringView, size_type)
    \sa find_first_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_first_not_of(CStringView v, size_type pos = 0) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds a character other than a given one.

    \param ch  Character to match.
    \param pos Offset to start the search at (default: \c 0).

    \return Offset of the first character at or after \a pos that differs from \a ch, \ref toy::CStringView::npos when
            there is none.

    \sa find_last_not_of(value_type, size_type)
    \sa find_first_of(value_type, size_type)
  */
  [[nodiscard]] constexpr size_type find_first_not_of(value_type ch, size_type pos = 0) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds a character outside a given set.

    Reads \a count characters from \a s instead of scanning it for a terminator, so \a s may hold null characters and
    need not end in one.

    \param s     Characters the match may not be any of.
    \param pos   Offset to start the search at.
    \param count Count of characters to read from \a s.

    \return Offset of the first character at or after \a pos that \a s does not hold, \ref toy::CStringView::npos when
            there is none.

    \pre \a s addresses at least \a count characters.

    \note A \a count of \c 0 leaves every character out, so the search reports \a pos while that offset is inside the
          string.

    \sa find_first_not_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_first_not_of(const value_type * s, size_type pos,
                                                      size_type count) const noexcept;

  /*!
    \brief Returns the offset where the viewed string first holds a character outside a given set.

    Measures \a s and matches it the way find_first_not_of(CStringView, size_type) does.

    \param s   Characters the match may not be any of.
    \param pos Offset to start the search at (default: \c 0).

    \return Offset of the first character at or after \a pos that \a s does not hold, \ref toy::CStringView::npos when
            there is none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \note An empty \a s leaves every character out, so the search reports \a pos while that offset is inside the string.

    \sa find_first_not_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_first_not_of(const value_type * s, size_type pos = 0) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds a character outside a given set.

    Reads \a v as a set and reports the last character the set leaves out, which is what trims a trailing run of the
    characters \a v holds.

    \param v   Characters the match may not be any of.
    \param pos Greatest offset the match may sit at
               (default: \ref toy::CStringView::npos, which searches the whole string).

    \return Offset of the last character at or before \a pos that \a v does not hold, \ref toy::CStringView::npos when
            there is none.

    \note An empty \a v leaves every character out, so the search reports the last character.

    \sa find_first_not_of(CStringView, size_type)
    \sa find_last_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_last_not_of(CStringView v, size_type pos = npos) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds a character other than a given one.

    \param ch  Character to match.
    \param pos Greatest offset the match may sit at
               (default: \ref toy::CStringView::npos, which searches the whole string).

    \return Offset of the last character at or before \a pos that differs from \a ch, \ref toy::CStringView::npos when
            there is none.

    \sa find_first_not_of(value_type, size_type)
    \sa find_last_of(value_type, size_type)
  */
  [[nodiscard]] constexpr size_type find_last_not_of(value_type ch, size_type pos = npos) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds a character outside a given set.

    Reads \a count characters from \a s instead of scanning it for a terminator, so \a s may hold null characters and
    need not end in one.

    \param s     Characters the match may not be any of.
    \param pos   Greatest offset the match may sit at.
    \param count Count of characters to read from \a s.

    \return Offset of the last character at or before \a pos that \a s does not hold, \ref toy::CStringView::npos when
            there is none.

    \pre \a s addresses at least \a count characters.

    \note A \a count of \c 0 leaves every character out, so the search reports the last character.

    \sa find_last_not_of(CStringView, size_type)
  */
  [[nodiscard]] constexpr size_type find_last_not_of(const value_type * s, size_type pos,
                                                     size_type count) const noexcept;

  /*!
    \brief Returns the offset where the viewed string last holds a character outside a given set.

    Measures \a s and matches it the way find_last_not_of(CStringView, size_type) does.

    \param s   Characters the match may not be any of.
    \param pos Greatest offset the match may sit at
               (default: \ref toy::CStringView::npos, which searches the whole string).

    \return Offset of the last character at or before \a pos that \a s does not hold, \ref toy::CStringView::npos when
            there is none.

    \pre \a s is non-null, checked by assert_message in debug builds.

    \note An empty \a s leaves every character out, so the search reports the last character.

    \sa find_last_not_of(CStringView, size_type)
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
    \sa compare(CStringView)
  */
  friend constexpr bool operator==(CStringView lhs, CStringView rhs) noexcept;

  /*!
    \brief Orders two views lexicographically.

    Reads the bytes in order and settles a tie on the lengths, so a string that starts another one orders before it.
    Either side converts from a null-terminated byte string, as equality does.

    \param lhs View on the left of the operator.
    \param rhs View on the right of the operator.

    \return \c strong_ordering::less when \a lhs orders first, \c strong_ordering::equal when both hold the same
            characters, \c strong_ordering::greater when \a rhs orders first.

    \note The compiler synthesizes \c <, \c <=, \c >, and \c >= from this operator.
    \note Bytes order by their \c unsigned \c char value, so the order holds on a target whose plain \c char is signed.
    \note The order consults no locale, which makes it identical across runs and targets.

    \sa operator==()
    \sa compare(CStringView)
  */
  friend constexpr traits_type::comparison_category operator<=>(CStringView lhs, CStringView rhs) noexcept;

private:
  /// First character of the viewed string, \c nullptr while the view holds none
  const value_type * _data{nullptr};

  /// Length of the viewed string in bytes, the terminator excluded
  size_type _size{0};
};

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_HPP_
