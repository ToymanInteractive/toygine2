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
  \file   string.hpp
  \brief  Owning string over a character buffer chosen by a template argument.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_HPP_
#define INCLUDE_CORE_STRING_HPP_

#include "fixed_string_storage.hpp"
#include "string_like.hpp"
#include "string_storage.hpp"
#include "string_view.hpp"

namespace toy {

template <StringStorage storageType>
class String {
public:
  /// Character operations the string uses to measure its characters
  using traits_type            = char_traits<char>;
  /// Type of the stored characters
  using value_type             = char;
  /// Unsigned type the length is measured in
  using size_type              = size_t;
  /// Signed type for the distance between two iterators
  using difference_type        = ptrdiff_t;
  /// Reference to a stored character that allows writing
  using reference              = value_type &;
  /// Reference to a stored character that allows reading only
  using const_reference        = const value_type &;
  /// Pointer to a stored character that allows writing
  using pointer                = char *;
  /// Pointer to a stored character that allows reading only
  using const_pointer          = const char *;
  /// Iterator over the stored characters that allows writing
  using iterator               = pointer;
  /// Iterator over the stored characters that allows reading only
  using const_iterator         = const_pointer;
  /// Back-to-front iterator over the stored characters that allows writing
  using reverse_iterator       = std::reverse_iterator<iterator>;
  /// Back-to-front iterator over the stored characters that allows reading only
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /*!
    \brief Builds an empty string.

    Allocates only if the default constructor of the storage does; \ref toy::FixedStringStorage does not, and zeroes its
    buffer instead.

    \post size() returns \c 0, and c_str() points at \c '\\0'.

    \sa String(const char *)
  */
  constexpr String() noexcept = default;

  /*!
    \brief Builds a string of \a count copies of \a ch.

    \param count Number of characters to write.
    \param ch    Character written at every position.

    \pre The storage accepts \a count: its reserve() returns \c true, checked by assert_message in debug builds.

    \post size() returns \a count, and every character before the terminator equals \a ch.

    \warning A shipping build skips the capacity check and leaves the string empty when the storage rejects \a count.

    \sa String(std::initializer_list<char>)
  */
  constexpr String(size_type count, value_type ch) noexcept;

  /*!
    \brief Builds a string from a copy of the characters in [\a first, \a last).

    A forward range is measured first and copied in one step, so the storage is asked for the whole length once; a pair
    of pointers is measured by subtraction and copied as one block. A single-pass range is read one character at a time,
    each checked against the storage before it is written.

    \tparam InputIterator Iterator type with \c char as its value type; satisfies \c std::input_iterator and is its
                          own \c std::sentinel_for.

    \param first Iterator to the first character to copy.
    \param last  Iterator past the last character to copy.

    \pre \a last is reachable from \a first.
    \pre The storage accepts the length of the range, checked by assert_message in debug builds.

    \post size() returns the length of the range, and c_str() reads its characters followed by \c '\\0'.

    \note A forward range without subtraction, such as a linked list, is traversed twice: once to measure, once to copy.

    \warning A shipping build skips the capacity check and leaves the string empty when the storage rejects the length
             of the range.

    \sa String(std::from_range_t, Range &&)
  */
  template <std::input_iterator InputIterator>
    requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
  constexpr String(InputIterator first, InputIterator last) noexcept;

#ifdef __cpp_lib_ranges_to_container
  /*!
    \brief Builds a string from a copy of the characters of \a range.

    Selected by the \c std::from_range tag, as in \c std::basic_string. Reads the range the way
    String(InputIterator, InputIterator) reads an iterator pair, except that the end of the range may have a type of its
    own, such as a sentinel that stops at a terminator.

    \tparam Range Source range with \c char as its value type; satisfies \c std::ranges::input_range.

    \param range Characters to copy.

    \pre The storage accepts the length of \a range, checked by assert_message in debug builds.

    \post size() returns the length of \a range, and c_str() reads its characters followed by \c '\\0'.

    \note Declared only where the standard library defines \c __cpp_lib_ranges_to_container; a freestanding
          libstdc++, as on Sega MD, does not.

    \warning A shipping build skips the capacity check and leaves the string empty when the storage rejects the length
             of \a range.

    \sa String(InputIterator, InputIterator)
  */
  template <std::ranges::input_range Range>
    requires std::same_as<std::ranges::range_value_t<Range>, char>
  constexpr String(std::from_range_t, Range && range) noexcept;
#endif // __cpp_lib_ranges_to_container

  /*!
    \brief Builds a string from a copy of \a count bytes starting at \a string.

    Copies exactly \a count bytes, so the source needs no terminator and a \c '\\0' inside the range is copied like any
    other byte. A null \a string with a \a count of \c 0 names an empty range, as in \c std::basic_string.

    \param string First byte to copy.
    \param count  Number of bytes to copy.

    \pre \a string is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a string are readable.
    \pre The storage accepts \a count, checked by assert_message in debug builds; for \ref toy::FixedString,
         \a count is below the buffer size.

    \post size() returns \a count, and c_str() reads the copied bytes followed by \c '\\0'.

    \warning A shipping build skips the capacity check and leaves the string empty when the storage rejects \a count,
             where \c std::basic_string throws \c std::length_error.

    \sa String(const char *)
  */
  constexpr String(const value_type * string, size_type count) noexcept;

  /*!
    \brief Builds a string from a copy of the null-terminated byte string \a string.

    Converts implicitly, so a string literal or a \c const \c char \c * becomes a string at the call site. Measures
    \a string, then copies it as String(const char *, size_t) does; the source may be destroyed once the constructor
    returns.

    \param string Null-terminated byte string to copy.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre The storage accepts the length of \a string, as String(const char *, size_t) requires.

    \post size() returns the length of \a string without its terminator, and c_str() reads the same characters.

    \sa String(const char *, size_t)
  */
  constexpr explicit(false) String(const value_type * string) noexcept;

  /*!
    \brief Deleted: a null pointer names no string to copy.

    Rejects a literal \c nullptr during compilation, ahead of the debug check the pointer constructor performs.

    \sa String(const char *)
  */
  String(nullptr_t) = delete;

  /*!
    \brief Builds a string from a copy of the characters of \a string.

    Copies size() bytes starting at c_str() of \a string. Explicit, like the \c std::basic_string constructor from a
    string-view-like type, because a copy into another storage may not fit. A string over the same storage type takes
    the copy constructor instead.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param string View or string to copy, such as a \ref toy::StringView or a \ref toy::String over another storage.

    \pre The storage accepts the length of \a string, as String(const char *, size_t) requires.

    \post size() returns the length of \a string, and c_str() reads the same bytes followed by \c '\\0'.

    \sa String(const StringType &, size_t, size_t)
  */
  template <StringLike StringType>
  constexpr explicit String(const StringType & string) noexcept;

  /*!
    \brief Builds a string from a copy of the substring of \a string that starts at \a pos.

    Takes at most \a count bytes and stops at the end of \a string. Covers the \c std::basic_string substring
    constructors from a string and from a string-view-like type, a \ref toy::String of any storage included. With a
    pointer the same call shape means a prefix: \c String("player", \c 2) holds \c "pl", while
    \c String(StringView("player"), \c 2) holds \c "ayer".

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param string View or string to copy from.
    \param pos    Offset of the first byte to copy.
    \param count  Most bytes to copy (default: \ref toy::String::npos, which copies to the end of \a string).

    \pre \a pos is not greater than the length of \a string, checked by assert_message in debug builds.
    \pre The storage accepts the length of the substring, as String(const char *, size_t) requires.

    \post size() returns the smaller of \a count and the length of \a string minus \a pos, and c_str() reads those
          bytes followed by \c '\\0'.

    \warning A shipping build skips the checks and leaves the string empty when \a pos is past the end of \a string or
             the storage rejects the substring, where \c std::basic_string throws \c std::out_of_range or
             \c std::length_error.

    \sa String(String &&, size_t, size_t)
  */
  template <StringLike StringType>
  constexpr String(const StringType & string, size_type pos, size_type count = npos) noexcept;

  /*!
    \brief Builds a string from a copy of \a other.

    Copies the storage through its copy constructor, so the two strings share no buffer and the copy allocates only if
    that storage does. Over \ref toy::FixedStringStorage the whole buffer is copied, not only the characters in use.

    \param other String to copy.

    \post size() returns other.size(), and c_str() reads the same bytes as other.c_str() from a buffer of its own.

    \sa String(String &&)
  */
  constexpr String(const String & other) noexcept = default;

  /*!
    \brief Builds a string that takes over the storage of \a other.

    Moves the storage through its move constructor, so a storage that owns heap memory hands over its buffer instead of
    allocating. \ref toy::FixedStringStorage has no buffer to hand over and copies it.

    \param other String to take the storage from.

    \post size() and c_str() read what \a other held before the call.
    \post \a other holds whatever the move constructor of the storage leaves; over \ref toy::FixedStringStorage, its
          original characters.

    \sa String(const String &)
  */
  constexpr String(String && other) noexcept = default;

  /*!
    \brief Builds a string from the substring of \a other that starts at \a pos, taking over its storage.

    Moves the storage of \a other, then shifts the substring to the front of the buffer, so a storage that owns heap
    memory reuses its buffer instead of allocating, as the \c std::basic_string constructor from an rvalue string and a
    position does.

    \param other String to take the storage from.
    \param pos   Offset of the first byte to keep.
    \param count Most bytes to keep (default: \ref toy::String::npos, which keeps everything to the end of \a other).

    \pre \a pos is not greater than other.size(), checked by assert_message in debug builds.

    \post size() returns the smaller of \a count and the length of \a other minus \a pos, and c_str() reads those bytes
          followed by \c '\\0'.
    \post \a other holds whatever the move constructor of the storage leaves; over \ref toy::FixedStringStorage, its
          original characters.

    \warning A shipping build skips the position check and leaves the string empty when \a pos is past the end of
             \a other.

    \sa String(const StringType &, size_t, size_t)
  */
  constexpr String(String && other, size_type pos, size_type count = npos) noexcept;

  /*!
    \brief Builds a string from a copy of the characters in \a list.

    Brace initialization prefers this constructor, so \c {3, \c 'a'} holds the two characters \c '\\3' and \c 'a' rather
    than three copies of \c 'a', as with \c std::basic_string.

    \param list Characters to copy.

    \pre The storage accepts the length of \a list, as String(const char *, size_t) requires.

    \post size() returns list.size(), and c_str() reads the characters of \a list followed by \c '\\0'.

    \sa String(size_t, char)
  */
  constexpr explicit(false) String(std::initializer_list<value_type> list) noexcept;

  /*!
    \brief Destroys the string and its storage.

    Releases what the destructor of the storage releases; \ref toy::FixedStringStorage releases nothing.

    \post Pointers returned by c_str() dangle.
  */
  constexpr ~String() noexcept = default;

  /*!
    \brief Returns the length of the string.

    \return Count of bytes before the terminator, \c 0 for an empty string.

    \sa c_str()
  */
  [[nodiscard]] constexpr size_type size() const noexcept;

  /*!
    \brief Returns the pointer to the null-terminated characters.

    \return Pointer to the first character of the storage, never \c nullptr for a storage meeting the
            \ref toy::StringStorage guarantees.

    \note The pointer stays valid while the string lives and its storage keeps the same buffer.

    \sa size()
  */
  [[nodiscard]] constexpr const_pointer c_str() const noexcept;

  /// Count that stands for every character up to the end of the source in the substring constructors
  static constexpr const size_type npos = -1;

private:
  /// Buffer holding the characters, the terminator, and the length
  storageType _storage;

  /*!
    \brief Copies the characters in [\a first, \a last) into the storage, which holds no characters yet.

    \tparam InputIterator Iterator type; satisfies \c std::input_iterator.
    \tparam Sentinel      End marker; satisfies \c std::sentinel_for with \a InputIterator.

    \param first Iterator to the first character to copy.
    \param last  Sentinel past the last character to copy.

    \pre size() is \c 0.
    \pre The storage accepts the length of the range, checked by assert_message in debug builds.

    \post size() returns the length of the range, or \c 0 when the storage rejects it.
  */
  template <std::input_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
  constexpr void _copyRange(InputIterator first, Sentinel last) noexcept;
};

/*!
  \brief String whose characters live in a buffer of a compile-time size inside the object.

  Allocates nothing, so the type works on targets without a heap.

  \tparam Capacity Size of the buffer in bytes, the terminator included; the string holds at most \a Capacity \c - \c 1
                   characters. Must be greater than \c 0, checked by a static assertion in
                   \ref toy::FixedStringStorage.

  \sa \ref toy::String
*/
template <size_t Capacity>
using FixedString = String<FixedStringStorage<Capacity>>;

} // namespace toy

#endif // INCLUDE_CORE_STRING_HPP_
