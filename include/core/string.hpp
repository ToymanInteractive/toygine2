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

/// String class template.
template <StringStorage storageType>
class String {
public:
  /// Character operations the string uses to measure its characters
  using traits_type            = std::char_traits<char>;
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

    Allocates only if the default constructor of the storage does; \ref toy::FixedStringStorage does not, and writes
    only the terminator.

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

    \note Declared only where the standard library defines \c __cpp_lib_ranges_to_container; a freestanding libstdc++,
          as on Sega MD, does not.

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
    \pre The storage accepts \a count, checked by assert_message in debug builds; for \ref toy::FixedString, \a count is
         below the buffer size.

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

    \post size() returns the smaller of \a count and the length of \a string minus \a pos, and c_str() reads those bytes
          followed by \c '\\0'.

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
    that storage does. \ref toy::FixedStringStorage copies the whole object up to
    \ref toy::platform::c_inlineCopyMaxBytes and only the characters in use above it.

    \param other String to copy.

    \post size() returns other.size(), and c_str() reads the same bytes as other.c_str() from a buffer of its own.

    \sa String(String &&)
  */
  constexpr String(const String & other) noexcept = default;

  /*!
    \brief Builds a string that takes over the storage of \a other.

    Moves the storage through its move constructor, so a storage that owns heap memory hands over its buffer instead of
    allocating. \ref toy::FixedStringStorage has no buffer to hand over and copies as the copy constructor does.

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
    \brief Replaces the contents with a copy of \a other.

    Assigns the storage through its copy assignment, so the storage decides what the copy costs:
    \ref toy::FixedStringStorage copies the whole object up to \ref toy::platform::c_inlineCopyMaxBytes and only the
    characters in use above it.

    \param other String to copy; may be this string.

    \return Reference to this string.

    \post size() returns other.size(), and c_str() reads the same characters.

    \sa assign(const String &)
  */
  constexpr String & operator=(const String & other) noexcept = default;

  /*!
    \brief Replaces the contents with those of \a other, taking over its storage where the storage allows.

    Assigns the storage through its move assignment. A storage that owns heap memory hands over its buffer;
    \ref toy::FixedStringStorage has nothing to hand over and copies as the copy assignment does.

    \param other String to take the contents from; may be this string.

    \return Reference to this string.

    \post size() and c_str() read what \a other held before the call.
    \post \a other holds whatever the move assignment of the storage leaves; over \ref toy::FixedStringStorage, its
          original characters.

    \sa assign(String &&)
  */
  constexpr String & operator=(String && other) noexcept = default;

  /*!
    \brief Replaces the contents with a copy of the null-terminated byte string \a string.

    \param string Null-terminated byte string to copy; may point into this string.

    \return Reference to this string.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre The storage accepts the length of \a string, as assign(const char *, size_t) requires.

    \post size() returns the length of \a string, and c_str() reads the same characters.

    \sa assign(const char *)
  */
  constexpr String & operator=(const value_type * string) noexcept;

  /*!
    \brief Replaces the contents with the single character \a ch.

    \param ch Character the string holds after the call.

    \return Reference to this string.

    \pre The storage accepts a length of \c 1, as assign(size_t, char) requires.

    \post size() returns \c 1, and c_str() reads \a ch followed by \c '\\0'.

    \sa assign(size_t, char)
  */
  constexpr String & operator=(value_type ch) noexcept;

  /*!
    \brief Replaces the contents with a copy of the characters in \a list.

    \param list Characters to copy.

    \return Reference to this string.

    \pre The storage accepts list.size(), as assign(const char *, size_t) requires.

    \post size() returns list.size(), and c_str() reads the characters of \a list followed by \c '\\0'.

    \sa assign(std::initializer_list<char>)
  */
  constexpr String & operator=(std::initializer_list<value_type> list) noexcept;

  /*!
    \brief Replaces the contents with a copy of the characters of \a string.

    Accepts the source implicitly, as the matching \c std::basic_string operator does, although the constructor from
    the same type is explicit.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param string View or string to copy, such as a \ref toy::StringView or a \ref toy::String over another storage;
                  may view this string.

    \return Reference to this string.

    \pre The storage accepts the length of \a string, as assign(const char *, size_t) requires.

    \post size() returns the length of \a string, and c_str() reads the same bytes followed by \c '\\0'.

    \sa assign(const StringType &)
  */
  template <StringLike StringType>
  constexpr String<storageType> & operator=(const StringType & string) noexcept;

  /*!
    \brief Deleted: a null pointer names no string to copy.

    Rejects a literal \c nullptr during compilation, ahead of the debug check the pointer assignment performs.

    \sa operator=(const char *)
  */
  String & operator=(nullptr_t) = delete;

  /*!
    \brief Replaces the contents with \a count copies of \a ch.

    \param count Number of characters to write.
    \param ch    Character written at every position.

    \return Reference to this string.

    \pre The storage accepts \a count: its reserve() returns \c true, checked by assert_message in debug builds.

    \post size() returns \a count, and every character before the terminator equals \a ch.

    \warning A shipping build skips the capacity check and leaves the string unchanged when the storage rejects
             \a count, where \c std::basic_string throws \c std::length_error.

    \sa operator=(char)
  */
  constexpr String & assign(size_type count, value_type ch) noexcept;

  /*!
    \brief Replaces the contents with a copy of \a other, as the copy assignment operator does.

    \param other String to copy; may be this string.

    \return Reference to this string.

    \post size() returns other.size(), and c_str() reads the same characters.

    \sa operator=(const String &)
  */
  constexpr String & assign(const String & other) noexcept;

  /*!
    \brief Replaces the contents with those of \a other, as the move assignment operator does.

    \param other String to take the contents from; may be this string.

    \return Reference to this string.

    \post size() and c_str() read what \a other held before the call.

    \sa operator=(String &&)
  */
  constexpr String & assign(String && other) noexcept;

  /*!
    \brief Replaces the contents with a copy of \a count bytes starting at \a string.

    Copies exactly \a count bytes, so the source needs no terminator. The source may lie inside this string, as when a
    string is assigned part of itself: the call then moves the bytes within the buffer instead of copying them.

    \param string First byte to copy.
    \param count  Number of bytes to copy.

    \return Reference to this string.

    \pre \a string is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a string are readable.
    \pre The storage accepts \a count, checked by assert_message in debug builds.

    \post size() returns \a count, and c_str() reads the copied bytes followed by \c '\\0'.

    \warning A shipping build skips the capacity check and leaves the string unchanged when the storage rejects
             \a count, where \c std::basic_string throws \c std::length_error.

    \sa assign(const char *)
  */
  constexpr String & assign(const value_type * string, size_type count) noexcept;

  /*!
    \brief Replaces the contents with a copy of the null-terminated byte string \a string.

    Measures \a string, then copies it as assign(const char *, size_t) does.

    \param string Null-terminated byte string to copy; may point into this string.

    \return Reference to this string.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre The storage accepts the length of \a string, as assign(const char *, size_t) requires.

    \post size() returns the length of \a string, and c_str() reads the same characters.

    \sa assign(const char *, size_t)
  */
  constexpr String & assign(const value_type * string) noexcept;

  /*!
    \brief Replaces the contents with a copy of the characters in [\a first, \a last).

    A contiguous range goes through assign(const char *, size_t), so it may lie inside this string. A forward range is
    measured first and copied in one step; a single-pass range is read one character at a time and checked against the
    storage whenever the buffer fills.

    \tparam InputIterator Iterator type with \c char as its value type; satisfies \c std::input_iterator and is its own
                          \c std::sentinel_for.

    \param first Iterator to the first character to copy.
    \param last  Iterator past the last character to copy.

    \return Reference to this string.

    \pre \a last is reachable from \a first.
    \pre A range that is not contiguous does not reference the characters of this string.
    \pre The storage accepts the length of the range, checked by assert_message in debug builds.

    \post size() returns the length of the range, and c_str() reads its characters followed by \c '\\0'.

    \warning A shipping build skips the capacity check when the storage rejects the length of the range: a single-pass
             range leaves the string empty, having overwritten it before the length was known, and any other range
             leaves it unchanged.

    \sa assign_range()
  */
  template <std::input_iterator InputIterator>
    requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
  constexpr String<storageType> & assign(InputIterator first, InputIterator last) noexcept;

  /*!
    \brief Replaces the contents with a copy of the characters in \a list.

    \param list Characters to copy.

    \return Reference to this string.

    \pre The storage accepts list.size(), as assign(const char *, size_t) requires.

    \post size() returns list.size(), and c_str() reads the characters of \a list followed by \c '\\0'.

    \sa operator=(std::initializer_list<char>)
  */
  constexpr String & assign(std::initializer_list<value_type> list) noexcept;

  /*!
    \brief Replaces the contents with a copy of the characters of \a string.

    Copies size() bytes starting at c_str() of \a string, as assign(const char *, size_t) does.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param string View or string to copy; may view this string.

    \return Reference to this string.

    \pre The storage accepts the length of \a string, as assign(const char *, size_t) requires.

    \post size() returns the length of \a string, and c_str() reads the same bytes followed by \c '\\0'.

    \sa assign(const StringType &, size_t, size_t)
  */
  template <StringLike StringType>
  constexpr String<storageType> & assign(const StringType & string) noexcept;

  /*!
    \brief Replaces the contents with a copy of the substring of \a string that starts at \a pos.

    Takes at most \a count bytes and stops at the end of \a string. Covers the \c std::basic_string substring overloads
    for a string and for a string-view-like type.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param string View or string to copy from; may view this string.
    \param pos    Offset of the first byte to copy.
    \param count  Most bytes to copy (default: \ref toy::String::npos, which copies to the end of \a string).

    \return Reference to this string.

    \pre \a pos is not greater than the length of \a string, checked by assert_message in debug builds.
    \pre The storage accepts the length of the substring, as assign(const char *, size_t) requires.

    \post size() returns the smaller of \a count and the length of \a string minus \a pos, and c_str() reads those bytes
          followed by \c '\\0'.

    \warning A shipping build skips the checks and leaves the string unchanged when \a pos is past the end of \a string
             or the storage rejects the substring, where \c std::basic_string throws \c std::out_of_range or
             \c std::length_error.

    \sa assign(const StringType &)
  */
  template <StringLike StringType>
  constexpr String<storageType> & assign(const StringType & string, size_type pos, size_type count = npos) noexcept;

  /*!
    \brief Replaces the contents with a copy of the characters of \a range.

    Reads the range as assign(InputIterator, InputIterator) reads an iterator pair, except that the end of the range
    may have a type of its own, such as a sentinel that stops at a terminator.

    \tparam Range Source range with \c char as its value type; satisfies \c std::ranges::input_range.

    \param range Characters to copy.

    \return Reference to this string.

    \pre A range that is not contiguous does not reference the characters of this string.
    \pre The storage accepts the length of \a range, checked by assert_message in debug builds.

    \post size() returns the length of \a range, and c_str() reads its characters followed by \c '\\0'.

    \warning A shipping build skips the capacity check when the storage rejects the length of \a range: a single-pass
             range leaves the string empty, having overwritten it before the length was known, and any other range
             leaves it unchanged.

    \sa assign(InputIterator, InputIterator)
  */
  template <std::ranges::input_range Range>
    requires std::same_as<std::ranges::range_value_t<Range>, char>
  constexpr String<storageType> & assign_range(Range && range) noexcept;

  /*!
    \brief Returns the character at an offset for writing.

    Where \c std::basic_string throws \c std::out_of_range, an offset past the characters fails a debug check.

    \param pos Zero-based offset of the character.

    \return Reference to the character at \a pos.

    \pre \a pos is less than size(), checked by assert_message in debug builds.

    \warning A shipping build skips the check, and an offset past the characters reads or writes outside them.

    \sa operator[](size_t)
  */
  [[nodiscard]] constexpr reference at(size_type pos) noexcept;

  /*!
    \brief Returns the character at an offset.

    \param pos Zero-based offset of the character.

    \return Reference to the character at \a pos.

    \pre \a pos is less than size(), checked by assert_message in debug builds.

    \sa at(size_t)
  */
  [[nodiscard]] constexpr const_reference at(size_type pos) const noexcept;

  /*!
    \brief Returns the character at an offset for writing, the terminator included.

    Accepts one offset more than at(): \a pos equal to size() names the terminator, as in \c std::basic_string.

    \param pos Zero-based offset of the character.

    \return Reference to the character at \a pos; at size(), to the terminator.

    \pre \a pos is not greater than size(), checked by assert_message in debug builds.

    \warning Writing anything but \c '\\0' to the terminator leaves c_str() unterminated.
    \warning A shipping build skips the check, and an offset past the terminator reads or writes outside the characters.

    \sa at(size_t)
  */
  [[nodiscard]] constexpr reference operator[](size_type pos) noexcept;

  /*!
    \brief Returns the character at an offset, the terminator included.

    \param pos Zero-based offset of the character.

    \return Reference to the character at \a pos; at size(), to the terminator.

    \pre \a pos is not greater than size(), checked by assert_message in debug builds.

    \sa operator[](size_t)
  */
  [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept;

  /*!
    \brief Returns the first character for writing.

    \return Reference to the character at offset \c 0.

    \pre The string is not empty, checked by assert_message in debug builds.

    \sa back()
  */
  [[nodiscard]] constexpr reference front() noexcept;

  /*!
    \brief Returns the first character.

    \return Reference to the character at offset \c 0.

    \pre The string is not empty, checked by assert_message in debug builds.

    \sa front()
  */
  [[nodiscard]] constexpr const_reference front() const noexcept;

  /*!
    \brief Returns the last character for writing.

    \return Reference to the character before the terminator.

    \pre The string is not empty, checked by assert_message in debug builds.

    \warning A shipping build skips the check, and an empty string yields a reference outside the buffer.

    \sa front()
  */
  [[nodiscard]] constexpr reference back() noexcept;

  /*!
    \brief Returns the last character.

    \return Reference to the character before the terminator.

    \pre The string is not empty, checked by assert_message in debug builds.

    \sa back()
  */
  [[nodiscard]] constexpr const_reference back() const noexcept;

  /*!
    \brief Returns the pointer to the characters for writing.

    A write through the pointer changes characters, never size().

    \return Pointer to the first character, never \c nullptr for a storage meeting the \ref toy::StringStorage
            guarantees.

    \pre A write stays below offset size(), or writes \c '\\0' to the terminator at size().

    \note The pointer stays valid as long as the one c_str() returns.

    \sa c_str()
  */
  [[nodiscard]] constexpr pointer data() noexcept;

  /*!
    \brief Returns the pointer to the null-terminated characters.

    \return The pointer c_str() returns.

    \sa data()
  */
  [[nodiscard]] constexpr const_pointer data() const noexcept;

  /*!
    \brief Returns the pointer to the null-terminated characters.

    \return Pointer to the first character of the storage, never \c nullptr for a storage meeting the
            \ref toy::StringStorage guarantees.

    \note The pointer stays valid while the string lives and its storage keeps the same buffer.

    \sa data()
    \sa size()
  */
  [[nodiscard]] constexpr const_pointer c_str() const noexcept;

  /*!
    \brief Returns a view over the characters of the string.

    Converts implicitly, as the \c std::basic_string conversion to \c std::basic_string_view does, so a string passes
    where a \ref toy::StringView is taken. The view measures c_str() again, which costs a scan of the characters.

    \return View over c_str() up to its first \c '\\0'.

    \note The view owns nothing and stays valid as long as the pointer c_str() returns.

    \warning A string holding \c '\\0' among its characters yields a view that stops there, shorter than size().

    \sa c_str()
  */
  constexpr explicit(false) operator StringView() const noexcept;

  /*!
    \brief Returns an iterator to the first character for writing.

    \return Iterator to the first character, equal to end() when the string is empty.

    \note Every iterator stays valid as long as the pointer data() returns.

    \sa end()
    \sa cbegin()
  */
  [[nodiscard]] constexpr iterator begin() noexcept;

  /*!
    \brief Returns an iterator to the first character.

    \return Iterator to the first character, equal to end() when the string is empty.

    \sa begin()
    \sa cbegin()
  */
  [[nodiscard]] constexpr const_iterator begin() const noexcept;

  /*!
    \brief Returns a read-only iterator to the first character.

    \return The iterator the const overload of begin() returns.

    \sa begin()
    \sa cend()
  */
  [[nodiscard]] constexpr const_iterator cbegin() const noexcept;

  /*!
    \brief Returns an iterator one past the last character.

    \return Iterator to the position after the last character, where the terminator sits.

    \warning Writing anything but \c '\\0' through it leaves c_str() unterminated.

    \sa begin()
    \sa cend()
  */
  [[nodiscard]] constexpr iterator end() noexcept;

  /*!
    \brief Returns an iterator one past the last character.

    \return Iterator to the position after the last character, where the terminator sits.

    \sa end()
    \sa cend()
  */
  [[nodiscard]] constexpr const_iterator end() const noexcept;

  /*!
    \brief Returns a read-only iterator one past the last character.

    \return The iterator the const overload of end() returns.

    \sa end()
    \sa cbegin()
  */
  [[nodiscard]] constexpr const_iterator cend() const noexcept;

  /*!
    \brief Returns a reverse iterator to the last character for writing.

    Starts at the last character; the walk runs back to front and stops before the first.

    \return Reverse iterator over end(), equal to rend() when the string is empty.

    \sa rend()
    \sa crbegin()
  */
  [[nodiscard]] constexpr reverse_iterator rbegin() noexcept;

  /*!
    \brief Returns a reverse iterator to the last character.

    \return Reverse iterator over end(), equal to rend() when the string is empty.

    \sa rbegin()
    \sa crbegin()
  */
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept;

  /*!
    \brief Returns a read-only reverse iterator to the last character.

    \return The iterator the const overload of rbegin() returns.

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
  [[nodiscard]] constexpr reverse_iterator rend() noexcept;

  /*!
    \brief Returns a reverse iterator to the position before the first character.

    \return Reverse iterator over begin(), the end of a back-to-front walk.

    \sa rend()
    \sa crend()
  */
  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept;

  /*!
    \brief Returns a read-only reverse iterator to the position before the first character.

    \return The iterator the const overload of rend() returns.

    \sa rend()
    \sa crbegin()
  */
  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept;

  /*!
    \brief Reports whether the string holds no characters.

    \return \c true when size() returns \c 0, \c false otherwise.

    \sa size()
  */
  [[nodiscard]] constexpr bool empty() const noexcept;

  /*!
    \brief Returns the length of the string.

    \return Count of bytes before the terminator, \c 0 for an empty string.

    \sa length()
    \sa c_str()
  */
  [[nodiscard]] constexpr size_type size() const noexcept;

  /*!
    \brief Returns the length of the string, as size() does.

    \return The value size() returns.

    \sa size()
  */
  [[nodiscard]] constexpr size_type length() const noexcept;

  /*!
    \brief Returns the longest length the storage can hold.

    Never changes and is never less than capacity(); over \ref toy::FixedStringStorage the two are equal, since the
    buffer never grows.

    \return Largest value size() may take, as the storage reports it.

    \sa capacity()
  */
  [[nodiscard]] constexpr size_type max_size() const noexcept;

  /*!
    \brief Prepares the storage to hold \a newCapacity characters without growing again.

    Passes the request to the storage: \ref toy::FixedStringStorage checks it against its buffer and allocates nothing,
    while a storage that owns heap memory may allocate here. A request no greater than capacity() keeps the buffer
    where it is, and no request shrinks it, as in \c std::basic_string since C++20.

    \param newCapacity Count of characters the string must hold, the terminator excluded.

    \pre The storage accepts \a newCapacity: its reserve() returns \c true, checked by assert_message in debug builds.

    \post size() and the characters are unchanged, and capacity() is at least \a newCapacity.

    \note A request above capacity() may move the buffer, so pointers and iterators into the string must be taken again.

    \warning A shipping build skips the check and leaves the string unchanged when the storage rejects \a newCapacity,
             where \c std::basic_string throws \c std::length_error.

    \sa capacity()
    \sa shrink_to_fit()
  */
  constexpr void reserve(size_type newCapacity) noexcept;

  /*!
    \brief Returns how many characters fit before the storage has to grow.

    \return Count of characters the buffer takes, the terminator excluded; never less than size().

    \sa reserve()
    \sa max_size()
  */
  [[nodiscard]] constexpr size_type capacity() const noexcept;

  /*!
    \brief Asks the storage to release the capacity the characters do not use.

    The request is non-binding, as in \c std::basic_string: \ref toy::FixedStringStorage keeps its buffer as it is.

    \post size() and the characters are unchanged, and capacity() is no less than size().

    \note A storage that releases memory may move the buffer, so pointers and iterators into the string must be taken
          again.

    \sa reserve()
  */
  constexpr void shrink_to_fit() noexcept;

  /*!
    \brief Removes every character and keeps the buffer.

    \post size() returns \c 0, c_str() points at \c '\\0', and capacity() is unchanged.

    \sa erase(size_t, size_t)
  */
  constexpr void clear() noexcept;

  /*!
    \brief Inserts \a count copies of \a ch before the character at \a index.

    \param index Offset the first copy takes; size() appends.
    \param count Number of characters to insert.
    \param ch    Character written at every inserted position.

    \return Reference to this string.

    \pre \a index is not greater than size(), checked by assert_message in debug builds.
    \pre The storage accepts size() plus \a count, checked by assert_message in debug builds.

    \post size() grows by \a count, the characters before \a index stay where they were, and the rest follow the copies.

    \note Pointers and iterators from \a index on read other characters after the call, and a storage that grows may
          move the buffer, after which none of them is valid.

    \warning A shipping build skips the checks and leaves the string unchanged when \a index is past the end or the
             storage rejects the new length, where \c std::basic_string throws \c std::out_of_range or
             \c std::length_error.

    \sa insert(PositionIterator, size_t, char)
  */
  constexpr String & insert(size_type index, size_type count, value_type ch) noexcept;

  /*!
    \brief Inserts a copy of the null-terminated byte string \a string before the character at \a index.

    Measures \a string, then inserts it as insert(size_t, const char *, size_t) does.

    \param index  Offset the first inserted byte takes; size() appends.
    \param string Null-terminated byte string to insert; may point into this string.

    \return Reference to this string.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre \a index and the length of \a string meet the preconditions of insert(size_t, const char *, size_t).

    \post size() grows by the length of \a string, which now starts at \a index.

    \sa insert(size_t, const char *, size_t)
  */
  constexpr String & insert(size_type index, const value_type * string) noexcept;

  /*!
    \brief Inserts a copy of \a count bytes starting at \a string before the character at \a index.

    Copies exactly \a count bytes, so the source needs no terminator. The source may lie inside this string, before
    \a index, after it, or across it; the inserted bytes are the ones it held before the call, even when the storage
    moves the buffer to grow.

    \param index  Offset the first inserted byte takes; size() appends.
    \param string First byte to insert.
    \param count  Number of bytes to insert.

    \return Reference to this string.

    \pre \a index is not greater than size(), checked by assert_message in debug builds.
    \pre \a string is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a string are readable.
    \pre The storage accepts size() plus \a count, checked by assert_message in debug builds.

    \post size() grows by \a count, the bytes before \a index stay where they were, the copy starts at \a index, and the
          rest follow it.

    \note Pointers and iterators from \a index on read other characters after the call, and a storage that grows may
          move the buffer, after which none of them is valid.

    \warning A shipping build skips the checks and leaves the string unchanged when \a index is past the end or the
             storage rejects the new length, where \c std::basic_string throws \c std::out_of_range or
             \c std::length_error.

    \sa insert(size_t, const char *)
  */
  constexpr String & insert(size_type index, const value_type * string, size_type count) noexcept;

  /*!
    \brief Inserts a copy of the characters of \a string before the character at \a index.

    Inserts size() bytes starting at c_str() of \a string, as insert(size_t, const char *, size_t) does.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param index  Offset the first inserted byte takes; size() appends.
    \param string View or string to insert; may view this string.

    \return Reference to this string.

    \pre \a index and the length of \a string meet the preconditions of insert(size_t, const char *, size_t).

    \post size() grows by the length of \a string, whose bytes now start at \a index.

    \sa insert(size_t, const StringType &, size_t, size_t)
  */
  template <StringLike StringType>
  constexpr String<storageType> & insert(size_type index, const StringType & string) noexcept;

  /*!
    \brief Inserts a copy of the substring of \a string that starts at \a pos before the character at \a index.

    Takes at most \a count bytes and stops at the end of \a string. Covers the \c std::basic_string substring overloads
    for a string and for a string-view-like type.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param index  Offset the first inserted byte takes; size() appends.
    \param string View or string to insert from; may view this string.
    \param pos    Offset of the first byte to insert.
    \param count  Most bytes to insert (default: \ref toy::String::npos, which inserts to the end of \a string).

    \return Reference to this string.

    \pre \a pos is not greater than the length of \a string, checked by assert_message in debug builds.
    \pre \a index and the length of the substring meet the preconditions of insert(size_t, const char *, size_t).

    \post size() grows by the smaller of \a count and the length of \a string minus \a pos, and those bytes now start at
          \a index.

    \warning A shipping build skips the checks and leaves the string unchanged when \a pos is past the end of \a string,
             where \c std::basic_string throws \c std::out_of_range.

    \sa insert(size_t, const StringType &)
  */
  template <StringLike StringType>
  constexpr String<storageType> & insert(size_type index, const StringType & string, size_type pos,
                                         size_type count = npos) noexcept;

  /*!
    \brief Inserts \a ch before the character \a pos points at.

    \tparam PositionIterator Type of \a pos; satisfies \c std::convertible_to with \c const \c char \c *, as
                             \ref toy::String::iterator and \ref toy::String::const_iterator do. An integer type does
    not, so a literal \c 0 selects the overload that takes an index.

    \param pos Iterator to the character the new one goes before; end() appends.
    \param ch  Character to insert.

    \return Iterator to the inserted character, or one at the offset of \a pos when the call is rejected.

    \pre \a pos and a count of \c 1 meet the preconditions of insert(PositionIterator, size_t, char).

    \post size() grows by \c 1, and \a ch stands where \a pos pointed.

    \sa insert(PositionIterator, size_t, char)
  */
  template <std::convertible_to<const char *> PositionIterator>
  constexpr iterator insert(PositionIterator pos, value_type ch) noexcept;

  /*!
    \brief Inserts \a count copies of \a ch before the character \a pos points at.

    Converts \a pos to an offset, then inserts as insert(size_t, size_t, char) does.

    \tparam PositionIterator Type of \a pos; satisfies \c std::convertible_to with \c const \c char \c *. An integer
    type does not, so a literal \c 0 selects the overload that takes an index.

    \param pos   Iterator to the character the copies go before; end() appends.
    \param count Number of characters to insert.
    \param ch    Character written at every inserted position.

    \return Iterator to the first inserted character, or one at the offset of \a pos when \a count is \c 0 or the call
    is rejected.

    \pre \a pos lies in [begin(), end()], checked by assert_message in debug builds.
    \pre The storage accepts size() plus \a count, checked by assert_message in debug builds.

    \post size() grows by \a count, and the copies start where \a pos pointed.

    \warning A shipping build skips the checks and leaves the string unchanged when \a pos lies outside the string or
    the storage rejects the new length.

    \sa insert(size_t, size_t, char)
  */
  template <std::convertible_to<const char *> PositionIterator>
  constexpr iterator insert(PositionIterator pos, size_type count, value_type ch) noexcept;

  /*!
    \brief Inserts a copy of the characters in [\a first, \a last) before the character \a pos points at.

    A contiguous range goes through insert(size_t, const char *, size_t), so it may lie inside this string. A forward
    range is measured first and copied in one step. A single-pass range is appended one character at a time, checked
    against the storage whenever the buffer fills, and then rotated into place.

    \tparam PositionIterator Type of \a pos; satisfies \c std::convertible_to with \c const \c char \c *. An integer
    type does not, so a literal \c 0 selects the overload that takes an index.
    \tparam InputIterator    Iterator type with \c char as its value type; satisfies \c std::input_iterator and is its
                             own \c std::sentinel_for.

    \param pos   Iterator to the character the range goes before; end() appends.
    \param first Iterator to the first character to insert.
    \param last  Iterator past the last character to insert.

    \return Iterator to the first inserted character, or one at the offset of \a pos when the range is empty or the call
            is rejected.

    \pre \a pos lies in [begin(), end()], checked by assert_message in debug builds.
    \pre \a last is reachable from \a first.
    \pre A range that is not contiguous does not reference the characters of this string.
    \pre The storage accepts size() plus the length of the range, checked by assert_message in debug builds.

    \post size() grows by the length of the range, whose characters now start where \a pos pointed.

    \warning A shipping build skips the checks and leaves the string unchanged when \a pos lies outside the string or
    the storage rejects the new length, a single-pass range included.

    \sa insert_range()
  */
  template <std::convertible_to<const char *> PositionIterator, std::input_iterator InputIterator>
    requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
  constexpr iterator insert(PositionIterator pos, InputIterator first, InputIterator last) noexcept;

  /*!
    \brief Inserts a copy of the characters in \a list before the character \a pos points at.

    \tparam PositionIterator Type of \a pos; satisfies \c std::convertible_to with \c const \c char \c *.

    \param pos  Iterator to the character the list goes before; end() appends.
    \param list Characters to insert.

    \return Iterator to the first inserted character, or one at the offset of \a pos when \a list is empty or the call
    is rejected.

    \pre \a pos and list.size() meet the preconditions of insert(PositionIterator, InputIterator, InputIterator).

    \post size() grows by list.size(), and the characters of \a list start where \a pos pointed.

    \sa insert(PositionIterator, InputIterator, InputIterator)
  */
  template <std::convertible_to<const char *> PositionIterator>
  constexpr iterator insert(PositionIterator pos, std::initializer_list<value_type> list) noexcept;

  /*!
    \brief Inserts a copy of the characters of \a range before the character \a pos points at.

    Reads the range as insert(PositionIterator, InputIterator, InputIterator) reads an iterator pair, except that the
    end of the range may have a type of its own, such as a sentinel that stops at a terminator.

    \tparam PositionIterator Type of \a pos; satisfies \c std::convertible_to with \c const \c char \c *.
    \tparam Range            Source range with \c char as its value type; satisfies \c std::ranges::input_range.

    \param pos   Iterator to the character the range goes before; end() appends.
    \param range Characters to insert.

    \return Iterator to the first inserted character, or one at the offset of \a pos when \a range is empty or the call
            is rejected.

    \pre \a pos and the length of \a range meet the preconditions of
         insert(PositionIterator, InputIterator, InputIterator).
    \pre A range that is not contiguous does not reference the characters of this string.

    \post size() grows by the length of \a range, whose characters now start where \a pos pointed.

    \sa insert(PositionIterator, InputIterator, InputIterator)
  */
  template <std::convertible_to<const char *> PositionIterator, std::ranges::input_range Range>
    requires std::same_as<std::ranges::range_value_t<Range>, char>
  constexpr iterator insert_range(PositionIterator pos, Range && range) noexcept;

  /*!
    \brief Removes up to \a count characters starting at \a index.

    Stops at the end of the string, so the defaults remove every character, as clear() does.

    \param index Offset of the first character to remove (default: \c 0).
    \param count Most characters to remove (default: \ref toy::String::npos, which removes to the end).

    \return Reference to this string.

    \pre \a index is not greater than size(), checked by assert_message in debug builds.

    \post size() shrinks by the smaller of \a count and size() minus \a index, and the characters after the removed ones
          now start at \a index.

    \note Pointers and iterators from \a index on read other characters after the call; the buffer stays where it is.

    \warning A shipping build skips the check and leaves the string unchanged when \a index is past the end, where
             \c std::basic_string throws \c std::out_of_range.

    \sa clear()
    \sa erase(PositionIterator, const_iterator)
  */
  constexpr String & erase(size_type index = 0, size_type count = npos) noexcept;

  /*!
    \brief Removes the character \a pos points at.

    \tparam PositionIterator Type of \a pos; satisfies \c std::convertible_to with \c const \c char \c *. An integer
    type does not, so a literal \c 0 selects the overload that takes an index.

    \param pos Iterator to the character to remove.

    \return Iterator to the character that followed the removed one, or end() when it was the last.

    \pre \a pos lies in [begin(), end()), checked by assert_message in debug builds.

    \post size() shrinks by \c 1.

    \warning A shipping build skips the check and leaves the string unchanged when \a pos is end() or lies outside the
             string.

    \sa erase(size_t, size_t)
  */
  template <std::convertible_to<const char *> PositionIterator>
  constexpr iterator erase(PositionIterator pos) noexcept;

  /*!
    \brief Removes the characters in [\a first, \a last).

    Only \a first takes a deduced type, so the pair may mix \ref toy::String::iterator and
    \ref toy::String::const_iterator, and a pair of integers still selects erase(size_t, size_t).

    \tparam PositionIterator Type of \a first; satisfies \c std::convertible_to with \c const \c char \c *.

    \param first Iterator to the first character to remove.
    \param last  Iterator past the last character to remove.

    \return Iterator to the character that followed the removed ones, at the offset \a first had.

    \pre \a first and \a last lie in [begin(), end()], checked by assert_message in debug builds.
    \pre \a first does not follow \a last, checked by assert_message in debug builds.

    \post size() shrinks by the length of the range.

    \warning A shipping build skips the checks and leaves the string unchanged when \a last precedes \a first or either
             lies outside the string.

    \sa erase(size_t, size_t)
  */
  template <std::convertible_to<const char *> PositionIterator>
  constexpr iterator erase(PositionIterator first, const_iterator last) noexcept;

  /*!
    \brief Appends \a ch after the last character.

    \param ch Character to append.

    \pre The storage accepts size() plus \c 1, checked by assert_message in debug builds.

    \post size() grows by \c 1, and back() returns \a ch.

    \note A storage that grows may move the buffer, after which no pointer or iterator into the string is valid.

    \warning A shipping build skips the check and leaves the string unchanged when the storage rejects the new length,
             where \c std::basic_string throws \c std::length_error.

    \sa pop_back()
    \sa append(size_t, char)
  */
  constexpr void push_back(value_type ch) noexcept;

  /*!
    \brief Removes the last character.

    \pre The string is not empty, checked by assert_message in debug builds.

    \post size() shrinks by \c 1, and \c '\\0' takes the place of the removed character.

    \note The buffer stays where it is, so pointers and iterators to the remaining characters stay valid.

    \warning A shipping build skips the check and leaves an empty string unchanged, where \c std::basic_string has
             undefined behavior.

    \sa push_back()
  */
  constexpr void pop_back() noexcept;

  /*!
    \brief Appends \a count copies of \a ch.

    \param count Number of characters to append.
    \param ch    Character written at every appended position.

    \return Reference to this string.

    \pre The storage accepts size() plus \a count, checked by assert_message in debug builds.

    \post size() grows by \a count, and the copies follow the old characters.

    \note A storage that grows may move the buffer, after which no pointer or iterator into the string is valid.

    \warning A shipping build skips the check and leaves the string unchanged when the storage rejects the new length,
             where \c std::basic_string throws \c std::length_error.

    \sa insert(size_t, size_t, char)
    \sa resize(size_t, char)
  */
  constexpr String & append(size_type count, value_type ch) noexcept;

  /*!
    \brief Appends a copy of \a count bytes starting at \a string.

    Copies exactly \a count bytes, so the source needs no terminator. The source may lie inside this string; the
    appended bytes are the ones it held before the call, even when the storage moves the buffer to grow.

    \param string First byte to append.
    \param count  Number of bytes to append.

    \return Reference to this string.

    \pre \a string is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a string are readable.
    \pre The storage accepts size() plus \a count, checked by assert_message in debug builds.

    \post size() grows by \a count, and the copy follows the old characters.

    \note A storage that grows may move the buffer, after which no pointer or iterator into the string is valid.

    \warning A shipping build skips the capacity check and leaves the string unchanged when the storage rejects the new
             length, where \c std::basic_string throws \c std::length_error.

    \sa append(const char *)
  */
  constexpr String & append(const value_type * string, size_type count) noexcept;

  /*!
    \brief Appends a copy of the null-terminated byte string \a string.

    Measures \a string, then appends it as append(const char *, size_t) does.

    \param string Null-terminated byte string to append; may point into this string.

    \return Reference to this string.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre The length of \a string meets the preconditions of append(const char *, size_t).

    \post size() grows by the length of \a string, and the copy follows the old characters.

    \sa append(const char *, size_t)
    \sa operator+=(const char *)
  */
  constexpr String & append(const value_type * string) noexcept;

  /*!
    \brief Appends a copy of the characters of \a string.

    Appends size() bytes starting at c_str() of \a string, as append(const char *, size_t) does.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param string View or string to append; may view this string.

    \return Reference to this string.

    \pre The length of \a string meets the preconditions of append(const char *, size_t).

    \post size() grows by the length of \a string, whose bytes follow the old characters.

    \sa append(const StringType &, size_t, size_t)
    \sa operator+=(const StringType &)
  */
  template <StringLike StringType>
  constexpr String<storageType> & append(const StringType & string) noexcept;

  /*!
    \brief Appends a copy of the substring of \a string that starts at \a pos.

    Takes at most \a count bytes and stops at the end of \a string. Covers the \c std::basic_string substring overloads
    for a string and for a string-view-like type.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param string View or string to append from; may view this string.
    \param pos    Offset of the first byte to append.
    \param count  Most bytes to append (default: \ref toy::String::npos, which appends to the end of \a string).

    \return Reference to this string.

    \pre \a pos is not greater than the length of \a string, checked by assert_message in debug builds.
    \pre The length of the substring meets the preconditions of append(const char *, size_t).

    \post size() grows by the smaller of \a count and the length of \a string minus \a pos, and those bytes follow the
          old characters.

    \warning A shipping build skips the checks and leaves the string unchanged when \a pos is past the end of \a string,
             where \c std::basic_string throws \c std::out_of_range.

    \sa append(const StringType &)
  */
  template <StringLike StringType>
  constexpr String<storageType> & append(const StringType & string, size_type pos, size_type count = npos) noexcept;

  /*!
    \brief Appends a copy of the characters in [\a first, \a last).

    A contiguous range goes through append(const char *, size_t), so it may lie inside this string. A forward range is
    measured first and copied in one step; a single-pass range is appended one character at a time and checked against
    the storage whenever the buffer fills.

    \tparam InputIterator Iterator type with \c char as its value type; satisfies \c std::input_iterator and is its own
                          \c std::sentinel_for.

    \param first Iterator to the first character to append.
    \param last  Iterator past the last character to append.

    \return Reference to this string.

    \pre \a last is reachable from \a first.
    \pre A range that is not contiguous does not reference the characters of this string.
    \pre The storage accepts size() plus the length of the range, checked by assert_message in debug builds.

    \post size() grows by the length of the range, whose characters follow the old ones.

    \note A storage that grows may move the buffer, after which no pointer or iterator into the string is valid.

    \warning A shipping build skips the check and leaves the string unchanged when the storage rejects the new length,
             a single-pass range included.

    \sa append_range()
  */
  template <std::input_iterator InputIterator>
    requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
  constexpr String<storageType> & append(InputIterator first, InputIterator last) noexcept;

  /*!
    \brief Appends a copy of the characters in \a list.

    \param list Characters to append.

    \return Reference to this string.

    \pre list.size() meets the preconditions of append(const char *, size_t).

    \post size() grows by list.size(), and the characters of \a list follow the old ones.

    \sa operator+=(std::initializer_list<char>)
  */
  constexpr String & append(std::initializer_list<value_type> list) noexcept;

  /*!
    \brief Appends a copy of the characters of \a range.

    Reads the range as append(InputIterator, InputIterator) reads an iterator pair, except that the end of the range may
    have a type of its own, such as a sentinel that stops at a terminator.

    \tparam Range Source range with \c char as its value type; satisfies \c std::ranges::input_range.

    \param range Characters to append.

    \return Reference to this string.

    \pre A range that is not contiguous does not reference the characters of this string.
    \pre The storage accepts size() plus the length of \a range, checked by assert_message in debug builds.

    \post size() grows by the length of \a range, whose characters follow the old ones.

    \warning A shipping build skips the check and leaves the string unchanged when the storage rejects the new length,
             a single-pass range included.

    \sa append(InputIterator, InputIterator)
  */
  template <std::ranges::input_range Range>
    requires std::same_as<std::ranges::range_value_t<Range>, char>
  constexpr String<storageType> & append_range(Range && range) noexcept;

  /*!
    \brief Appends \a ch, as push_back() does.

    \param ch Character to append.

    \return Reference to this string.

    \pre The storage accepts size() plus \c 1, checked by assert_message in debug builds.

    \post size() grows by \c 1, and back() returns \a ch.

    \warning A shipping build skips the check and leaves the string unchanged when the storage rejects the new length.

    \sa push_back()
  */
  constexpr String & operator+=(value_type ch) noexcept;

  /*!
    \brief Appends a copy of the null-terminated byte string \a string, as append(const char *) does.

    \param string Null-terminated byte string to append; may point into this string.

    \return Reference to this string.

    \pre \a string meets the preconditions of append(const char *).

    \post size() grows by the length of \a string, and the copy follows the old characters.

    \sa append(const char *)
  */
  constexpr String & operator+=(const value_type * string) noexcept;

  /*!
    \brief Appends a copy of the characters in \a list, as append(std::initializer_list<char>) does.

    \param list Characters to append.

    \return Reference to this string.

    \pre list.size() meets the preconditions of append(const char *, size_t).

    \post size() grows by list.size(), and the characters of \a list follow the old ones.

    \sa append(std::initializer_list<char>)
  */
  constexpr String & operator+=(std::initializer_list<value_type> list) noexcept;

  /*!
    \brief Appends a copy of the characters of \a string, as append(const StringType &) does.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param string View or string to append; may view this string.

    \return Reference to this string.

    \pre The length of \a string meets the preconditions of append(const char *, size_t).

    \post size() grows by the length of \a string, whose bytes follow the old characters.

    \sa append(const StringType &)
  */
  template <StringLike StringType>
  constexpr String<storageType> & operator+=(const StringType & string) noexcept;

  /*!
    \brief Replaces up to \a count characters starting at \a pos with a copy of \a count2 bytes starting at \a string.

    Removes the smaller of \a count and size() minus \a pos and puts the copy in their place, so the two lengths may
    differ. The source needs no terminator and may lie anywhere inside this string, the replaced run included: the copy
    holds the bytes the source had before the call, even when the storage moves the buffer to grow.

    \param pos    Offset of the first character to replace; size() appends.
    \param count  Most characters to replace.
    \param string First byte of the replacement.
    \param count2 Number of bytes in the replacement.

    \return Reference to this string.

    \pre \a pos is not greater than size(), checked by assert_message in debug builds.
    \pre \a string is non-null or \a count2 is \c 0, checked by assert_message in debug builds.
    \pre The \a count2 bytes starting at \a string are readable.
    \pre The storage accepts the new length, checked by assert_message in debug builds.

    \post The characters before \a pos stay where they were, the copy starts at \a pos, and the characters that followed
          the replaced run follow the copy.

    \note Pointers and iterators from \a pos on read other characters after the call, and a storage that grows may
          move the buffer, after which none of them is valid.

    \warning A shipping build skips the checks and leaves the string unchanged when \a pos is past the end or the
             storage rejects the new length, where \c std::basic_string throws \c std::out_of_range or
             \c std::length_error.

    \sa replace(size_t, size_t, const char *)
    \sa replace(PositionIterator, const_iterator, const char *, size_t)
  */
  constexpr String & replace(size_type pos, size_type count, const value_type * string, size_type count2) noexcept;

  /*!
    \brief Replaces up to \a count characters at \a pos with a copy of the null-terminated byte string \a string.

    Measures \a string, then replaces as replace(size_t, size_t, const char *, size_t) does.

    \param pos    Offset of the first character to replace; size() appends.
    \param count  Most characters to replace.
    \param string Null-terminated byte string to put in their place; may point into this string.

    \return Reference to this string.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre \a pos, \a count, and the length of \a string meet the preconditions of
         replace(size_t, size_t, const char *, size_t).

    \post The copy of \a string starts at \a pos, and the characters that followed the replaced run follow it.

    \sa replace(size_t, size_t, const char *, size_t)
  */
  constexpr String & replace(size_type pos, size_type count, const value_type * string) noexcept;

  /*!
    \brief Replaces up to \a count characters starting at \a pos with a copy of the characters of \a string.

    Puts size() bytes starting at c_str() of \a string in their place, as replace(size_t, size_t, const char *, size_t)
    does.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param pos    Offset of the first character to replace; size() appends.
    \param count  Most characters to replace.
    \param string View or string to put in their place; may view this string.

    \return Reference to this string.

    \pre \a pos, \a count, and the length of \a string meet the preconditions of
         replace(size_t, size_t, const char *, size_t).

    \post The bytes of \a string start at \a pos, and the characters that followed the replaced run follow them.

    \sa replace(size_t, size_t, const StringType &, size_t, size_t)
  */
  template <StringLike StringType>
  constexpr String<storageType> & replace(size_type pos, size_type count, const StringType & string) noexcept;

  /*!
    \brief Replaces up to \a count characters at \a pos with the substring of \a string that starts at \a pos2.

    Takes at most \a count2 bytes and stops at the end of \a string. Covers the \c std::basic_string substring overloads
    for a string and for a string-view-like type.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param pos    Offset of the first character to replace; size() appends.
    \param count  Most characters to replace.
    \param string View or string to take the replacement from; may view this string.
    \param pos2   Offset of the first byte of the replacement in \a string.
    \param count2 Most bytes in the replacement (default: \ref toy::String::npos, which takes \a string to its end).

    \return Reference to this string.

    \pre \a pos2 is not greater than the length of \a string, checked by assert_message in debug builds.
    \pre \a pos, \a count, and the length of the substring meet the preconditions of
         replace(size_t, size_t, const char *, size_t).

    \post The substring starts at \a pos, and the characters that followed the replaced run follow it.

    \warning A shipping build skips the checks and leaves the string unchanged when \a pos2 is past the end of
             \a string, where \c std::basic_string throws \c std::out_of_range.

    \sa replace(size_t, size_t, const StringType &)
  */
  template <StringLike StringType>
  constexpr String<storageType> & replace(size_type pos, size_type count, const StringType & string, size_type pos2,
                                          size_type count2 = npos) noexcept;

  /*!
    \brief Replaces up to \a count characters starting at \a pos with \a count2 copies of \a ch.

    Removes the smaller of \a count and size() minus \a pos and writes the copies in their place.

    \param pos    Offset of the first character to replace; size() appends.
    \param count  Most characters to replace.
    \param count2 Number of copies to write.
    \param ch     Character written at every position of the replacement.

    \return Reference to this string.

    \pre \a pos is not greater than size(), checked by assert_message in debug builds.
    \pre The storage accepts the new length, checked by assert_message in debug builds.

    \post The copies start at \a pos, and the characters that followed the replaced run follow them.

    \note Pointers and iterators from \a pos on read other characters after the call, and a storage that grows may
          move the buffer, after which none of them is valid.

    \warning A shipping build skips the checks and leaves the string unchanged when \a pos is past the end or the
             storage rejects the new length, where \c std::basic_string throws \c std::out_of_range or
             \c std::length_error.

    \sa replace(PositionIterator, const_iterator, size_t, char)
  */
  constexpr String & replace(size_type pos, size_type count, size_type count2, value_type ch) noexcept;

  /*!
    \brief Replaces the characters in [\a first, \a last) with a copy of \a count2 bytes starting at \a string.

    Converts the range to an offset and a length, then replaces as replace(size_t, size_t, const char *, size_t) does.
    Only \a first takes a deduced type, so the pair may mix \ref toy::String::iterator and
    \ref toy::String::const_iterator, and integers still select the overloads that take an offset.

    \tparam PositionIterator Type of \a first; satisfies \c std::convertible_to with \c const \c char \c *.

    \param first  Iterator to the first character to replace.
    \param last   Iterator past the last character to replace.
    \param string First byte of the replacement; may point into this string.
    \param count2 Number of bytes in the replacement.

    \return Reference to this string.

    \pre \a first and \a last lie in [begin(), end()], checked by assert_message in debug builds.
    \pre \a first does not follow \a last, checked by assert_message in debug builds.
    \pre \a string and \a count2 meet the preconditions of replace(size_t, size_t, const char *, size_t).

    \post The copy starts where \a first pointed, and the characters that followed the range follow it.

    \warning A shipping build skips the checks and leaves the string unchanged when \a last precedes \a first or either
             lies outside the string.

    \sa replace(size_t, size_t, const char *, size_t)
  */
  template <std::convertible_to<const char *> PositionIterator>
  constexpr String<storageType> & replace(PositionIterator first, const_iterator last, const value_type * string,
                                          size_type count2) noexcept;

  /*!
    \brief Replaces the characters in [\a first, \a last) with a copy of the null-terminated byte string \a string.

    Measures \a string, then replaces as replace(PositionIterator, const_iterator, const char *, size_t) does.

    \tparam PositionIterator Type of \a first; satisfies \c std::convertible_to with \c const \c char \c *.

    \param first  Iterator to the first character to replace.
    \param last   Iterator past the last character to replace.
    \param string Null-terminated byte string to put in their place; may point into this string.

    \return Reference to this string.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre \a first, \a last, and the length of \a string meet the preconditions of
         replace(PositionIterator, const_iterator, const char *, size_t).

    \post The copy of \a string starts where \a first pointed, and the characters that followed the range follow it.

    \sa replace(size_t, size_t, const char *)
  */
  template <std::convertible_to<const char *> PositionIterator>
  constexpr String<storageType> & replace(PositionIterator first, const_iterator last,
                                          const value_type * string) noexcept;

  /*!
    \brief Replaces the characters in [\a first, \a last) with a copy of the characters of \a string.

    Puts size() bytes starting at c_str() of \a string in their place, as
    replace(PositionIterator, const_iterator, const char *, size_t) does.

    \tparam PositionIterator Type of \a first; satisfies \c std::convertible_to with \c const \c char \c *.
    \tparam StringType       Source type; satisfies \ref toy::StringLike.

    \param first  Iterator to the first character to replace.
    \param last   Iterator past the last character to replace.
    \param string View or string to put in their place; may view this string.

    \return Reference to this string.

    \pre \a first, \a last, and the length of \a string meet the preconditions of
         replace(PositionIterator, const_iterator, const char *, size_t).

    \post The bytes of \a string start where \a first pointed, and the characters that followed the range follow them.

    \sa replace(size_t, size_t, const StringType &)
  */
  template <std::convertible_to<const char *> PositionIterator, StringLike StringType>
  constexpr String<storageType> & replace(PositionIterator first, const_iterator last,
                                          const StringType & string) noexcept;

  /*!
    \brief Replaces the characters in [\a first, \a last) with \a count2 copies of \a ch.

    Converts the range to an offset and a length, then replaces as replace(size_t, size_t, size_t, char) does.

    \tparam PositionIterator Type of \a first; satisfies \c std::convertible_to with \c const \c char \c *.

    \param first  Iterator to the first character to replace.
    \param last   Iterator past the last character to replace.
    \param count2 Number of copies to write.
    \param ch     Character written at every position of the replacement.

    \return Reference to this string.

    \pre \a first and \a last lie in [begin(), end()], checked by assert_message in debug builds.
    \pre \a first does not follow \a last, checked by assert_message in debug builds.
    \pre The storage accepts the new length, checked by assert_message in debug builds.

    \post The copies start where \a first pointed, and the characters that followed the range follow them.

    \warning A shipping build skips the checks and leaves the string unchanged when \a last precedes \a first, either
             lies outside the string, or the storage rejects the new length.

    \sa replace(size_t, size_t, size_t, char)
  */
  template <std::convertible_to<const char *> PositionIterator>
  constexpr String<storageType> & replace(PositionIterator first, const_iterator last, size_type count2,
                                          value_type ch) noexcept;

  /*!
    \brief Replaces the characters in [\a first, \a last) with a copy of the characters in [\a first2, \a last2).

    A contiguous source goes through replace(size_t, size_t, const char *, size_t), so it may lie inside this string. A
    forward source is measured first and copied in one step. A single-pass source overwrites the replaced characters
    first; what is left of it is appended one character at a time, checked against the storage whenever the buffer
    fills, and rotated into place, so the string never grows past its final length.

    \tparam PositionIterator Type of \a first; satisfies \c std::convertible_to with \c const \c char \c *.
    \tparam InputIterator    Iterator type with \c char as its value type; satisfies \c std::input_iterator and is its
                             own \c std::sentinel_for.

    \param first  Iterator to the first character to replace.
    \param last   Iterator past the last character to replace.
    \param first2 Iterator to the first character of the replacement.
    \param last2  Iterator past the last character of the replacement.

    \return Reference to this string.

    \pre \a first and \a last lie in [begin(), end()], checked by assert_message in debug builds.
    \pre \a first does not follow \a last, checked by assert_message in debug builds.
    \pre \a last2 is reachable from \a first2.
    \pre A source that is not contiguous does not reference the characters of this string.
    \pre The storage accepts the new length, checked by assert_message in debug builds.

    \post The characters of the source start where \a first pointed, and the characters that followed the range follow
          them.

    \warning A shipping build skips the checks and leaves the string unchanged when \a last precedes \a first, either
             lies outside the string, or the storage rejects the new length. A single-pass source is the exception to
             the last case: the string keeps its old length, but the replaced characters stay overwritten by the first
             characters of the source.

    \sa replace_with_range()
  */
  template <std::convertible_to<const char *> PositionIterator, std::input_iterator InputIterator>
    requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
  constexpr String<storageType> & replace(PositionIterator first, const_iterator last, InputIterator first2,
                                          InputIterator last2) noexcept;

  /*!
    \brief Replaces the characters in [\a first, \a last) with a copy of the characters in \a list.

    \tparam PositionIterator Type of \a first; satisfies \c std::convertible_to with \c const \c char \c *.

    \param first Iterator to the first character to replace.
    \param last  Iterator past the last character to replace.
    \param list  Characters to put in their place.

    \return Reference to this string.

    \pre \a first, \a last, and list.size() meet the preconditions of
         replace(PositionIterator, const_iterator, const char *, size_t).

    \post The characters of \a list start where \a first pointed, and the characters that followed the range follow
          them.

    \sa replace(PositionIterator, const_iterator, InputIterator, InputIterator)
  */
  template <std::convertible_to<const char *> PositionIterator>
  constexpr String<storageType> & replace(PositionIterator first, const_iterator last,
                                          std::initializer_list<value_type> list) noexcept;

  /*!
    \brief Replaces the characters in [\a first, \a last) with a copy of the characters of \a range.

    Reads the range as replace(PositionIterator, const_iterator, InputIterator, InputIterator) reads an iterator pair,
    except that the end of the range may have a type of its own, such as a sentinel that stops at a terminator.

    \tparam PositionIterator Type of \a first; satisfies \c std::convertible_to with \c const \c char \c *.
    \tparam Range            Source range with \c char as its value type; satisfies \c std::ranges::input_range.

    \param first Iterator to the first character to replace.
    \param last  Iterator past the last character to replace.
    \param range Characters to put in their place.

    \return Reference to this string.

    \pre \a first, \a last, and the length of \a range meet the preconditions of
         replace(PositionIterator, const_iterator, InputIterator, InputIterator).
    \pre A range that is not contiguous does not reference the characters of this string.

    \post The characters of \a range start where \a first pointed, and the characters that followed the range follow
          them.

    \warning A shipping build that rejects the new length handles a single-pass range as
             replace(PositionIterator, const_iterator, InputIterator, InputIterator) handles a single-pass source.

    \sa replace(PositionIterator, const_iterator, InputIterator, InputIterator)
  */
  template <std::convertible_to<const char *> PositionIterator, std::ranges::input_range Range>
    requires std::same_as<std::ranges::range_value_t<Range>, char>
  constexpr String<storageType> & replace_with_range(PositionIterator first, const_iterator last,
                                                     Range && range) noexcept;

  /*!
    \brief Copies up to \a count characters starting at \a pos into \a destination.

    Stops at the end of the string and writes no terminator.

    \param destination First byte of the buffer to write into.
    \param count       Most characters to copy.
    \param pos         Offset of the first character to copy (default: \c 0).

    \return Number of characters written: the smaller of \a count and size() minus \a pos.

    \pre \a pos is not greater than size(), checked by assert_message in debug builds.
    \pre \a destination is non-null or the call writes nothing, checked by assert_message in debug builds.
    \pre \a destination has room for the characters written and does not overlap this string.

    \warning A shipping build skips the checks, writes nothing, and returns \c 0 when \a pos is past the end, where
             \c std::basic_string throws \c std::out_of_range.

    \sa c_str()
  */
  constexpr size_type copy(value_type * destination, size_type count, size_type pos = 0) const noexcept;

  /*!
    \brief Sets the length to \a count, appending null characters when it grows.

    Resizes as resize(size_t, char) does, with \c '\\0' as the appended character.

    \param count New number of characters.

    \pre \a count meets the preconditions of resize(size_t, char).

    \post size() returns \a count, and every appended character is \c '\\0'.

    \sa resize(size_t, char)
  */
  constexpr void resize(size_type count) noexcept;

  /*!
    \brief Sets the length to \a count, removing the characters past it or appending copies of \a ch.

    \param count New number of characters.
    \param ch    Character written at every appended position.

    \pre The storage accepts \a count when it exceeds size(), checked by assert_message in debug builds.

    \post size() returns \a count, the characters before the smaller of \a count and the old size() stay as they were,
          and every appended one equals \a ch.

    \note Shrinking keeps the buffer where it is; growing may move it, after which no pointer or iterator into the
          string is valid.

    \warning A shipping build skips the check and leaves the string unchanged when the storage rejects \a count, where
             \c std::basic_string throws \c std::length_error.

    \sa resize(size_t)
    \sa resize_and_overwrite()
  */
  constexpr void resize(size_type count, value_type ch) noexcept;

  /*!
    \brief Makes room for \a count characters, lets \a operation write them, and takes the length it returns.

    Calls \a operation once, as an rvalue, with data() and \a count. The buffer still holds the old characters, and the
    operation may overwrite any of the first \a count positions; the string then keeps as many of them as the returned
    value says and writes a terminator after them.

    \tparam Operation Callable type invocable with \c char \c * and \c size_t; its result satisfies \c std::integral
                      and is none of \c bool, \c char, \c wchar_t, \c char8_t, \c char16_t, and \c char32_t.

    \param count     Number of characters the operation may write.
    \param operation Callable that writes the characters and returns the new length.

    \pre The storage accepts \a count, checked by assert_message in debug builds.
    \pre \a operation returns a value in [\c 0, \a count], checked by assert_message in debug builds.
    \pre \a operation writes nothing past the first \a count characters of the buffer.

    \post size() returns the value \a operation returned, and c_str() reads that many characters of the buffer followed
          by \c '\\0'.

    \note A storage that grows to \a count may move the buffer before the call, so the operation gets the new address
          and no pointer or iterator taken earlier is valid.

    \warning A shipping build skips the checks. It leaves the string unchanged without calling \a operation when the
             storage rejects \a count, and keeps the old size() when the returned value falls outside [\c 0, \a count],
             though any characters the operation wrote below that size stay written.

    \sa resize(size_t, char)
  */
  template <typename Operation>
    requires std::integral<std::invoke_result_t<Operation, char *, size_t>>
  constexpr void resize_and_overwrite(size_type count, Operation operation) noexcept;

  /*!
    \brief Exchanges the contents of this string and \a other.

    Swaps the two storages through \c std::ranges::swap: a free \c swap found for the storage type by argument-dependent
    lookup, or else a move construction and two move assignments. \ref toy::FixedStringStorage has no such \c swap, so
    its characters are copied both ways.

    \param other String to exchange contents with; may be this string.

    \post This string holds what \a other held before the call, and \a other holds what this string held.

    \note With \ref toy::FixedStringStorage, pointers and iterators keep their addresses and read the other string's
          characters after the call.

    \sa assign(String &&)
  */
  constexpr void swap(String & other) noexcept;

  /// Count that stands for every character up to the end of the source or of the string
  static constexpr const size_type npos = -1;

private:
  /// Buffer holding the characters, the terminator, and the length
  storageType _storage;

  /*!
    \brief Reports whether a source starting at \a pointer may share bytes with the storage.

    Tells an assignment whether it needs memmove() or can use memcpy(). Constant evaluation cannot compare pointers into
    unrelated objects, so it answers \c true there, and the caller takes the copy that tolerates overlap.

    \param pointer First byte of the source.

    \return \c true when \a pointer lies within the buffer, its terminator included, or during constant evaluation;
            \c false otherwise.

    \note Only the first byte is checked; a source that starts before the buffer and runs into it is not a valid string.

    \sa assign(const char *, size_t)
  */
  [[nodiscard]] constexpr bool _mayOverlap(const_pointer pointer) const noexcept;

  /*!
    \brief Replaces the characters in the storage with a copy of the characters in [\a first, \a last).

    A contiguous range may lie inside the storage, as when a string is assigned part of itself; any other range goes to
    _copyExternalRange().

    \tparam InputIterator Iterator type; satisfies \c std::input_iterator.
    \tparam Sentinel      End marker; satisfies \c std::sentinel_for with \a InputIterator.

    \param first Iterator to the first character to copy.
    \param last  Sentinel past the last character to copy.

    \pre The storage accepts the length of the range, checked by assert_message in debug builds.
    \pre A range that is not contiguous shares no bytes with the storage.

    \post size() returns the length of the range. When the storage rejects it, a single-pass range leaves the storage
          empty and any other range leaves it as it was.

    \sa _copyExternalRange()
  */
  template <std::input_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
  constexpr void _copyRange(InputIterator first, Sentinel last) noexcept;

  /*!
    \brief Replaces the characters in the storage with a copy of the characters in [\a first, \a last), which lie
           outside it.

    Copies a contiguous range with memcpy() rather than memmove(), which some targets implement byte by byte. Called
    where the source cannot belong to this string, such as a constructor.

    \tparam InputIterator Iterator type; satisfies \c std::input_iterator.
    \tparam Sentinel      End marker; satisfies \c std::sentinel_for with \a InputIterator.

    \param first Iterator to the first character to copy.
    \param last  Sentinel past the last character to copy.

    \pre The storage accepts the length of the range, checked by assert_message in debug builds.
    \pre The range shares no bytes with the storage.

    \post size() returns the length of the range. When the storage rejects it, a single-pass range leaves the storage
          empty and any other range leaves it as it was.

    \sa _copyRange()
  */
  template <std::input_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
  constexpr void _copyExternalRange(InputIterator first, Sentinel last) noexcept;

  /*!
    \brief Converts an iterator into this string to the offset of the character it points at.

    \param pos Iterator to convert.

    \return Distance from begin() to \a pos.

    \pre \a pos lies in [begin(), end()], checked by assert_message in debug builds; a shipping build returns an offset
         past size(), which the callers treat as a rejected call.
  */
  [[nodiscard]] constexpr size_type _indexOf(const_iterator pos) const noexcept;

  /*!
    \brief Finds the offset of \a pointer among the characters of this string.

    Lets an edit that may move the buffer find a source inside this string again afterwards. Constant evaluation cannot
    order pointers into unrelated objects, so there the search compares \a pointer for equality with the address of
    each character in turn.

    \param pointer Address to look for.

    \return Offset of the character \a pointer points at, or \ref toy::String::npos when it points at none of the size()
            characters.

    \sa _mayOverlap()
  */
  [[nodiscard]] constexpr size_type _offsetOf(const_pointer pointer) const noexcept;

  /*!
    \brief Replaces \a removed characters at \a pos with \a count unwritten ones.

    Shifts the characters after the removed ones to follow the new slots and records the new length; the caller writes
    the slots afterwards.

    \param pos     Offset of the first character to replace.
    \param removed Number of characters to replace.
    \param count   Number of slots to open in their place.

    \return \c true when the storage accepts the new length; \c false when it rejects it, and the string is unchanged.

    \pre \a pos plus \a removed is not greater than size().
    \pre The storage accepts size() minus \a removed plus \a count, checked by assert_message in debug builds.

    \post size() returns its old value minus \a removed plus \a count, and [\a pos, \a pos + \a count) holds
          unspecified characters.

    \note A storage that grows may move the buffer; a shrinking or same-size gap keeps it where it is.
  */
  [[nodiscard]] constexpr bool _openGap(size_type pos, size_type removed, size_type count) noexcept;

  /*!
    \brief Replaces \a removed characters at \a pos with a copy of \a count bytes starting at \a source.

    Insertion and erasure go through this call. A source inside this string is located by its offset before the buffer
    may move, then read from wherever its bytes lie once the gap is open.

    \param pos     Offset of the first character to replace.
    \param removed Number of characters to replace.
    \param source  First byte to copy; may point into this string.
    \param count   Number of bytes to copy.

    \pre \a pos plus \a removed is not greater than size().
    \pre \a source is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The storage accepts size() minus \a removed plus \a count, checked by assert_message in debug builds.

    \post The characters before \a pos are unchanged, the copy starts at \a pos, and the characters that followed the
          replaced ones follow it. When the storage rejects the new length, the string is unchanged.

    \sa _openGap()
  */
  constexpr void _replace(size_type pos, size_type removed, const_pointer source, size_type count) noexcept;

  /*!
    \brief Replaces \a removed characters at \a pos with a copy of the characters in [\a first, \a last).

    A contiguous range goes to _replace(), so it may lie inside this string. A forward range is measured and copied into
    an open gap. A single-pass range overwrites the replaced characters first, so the string never grows past its final
    length; what is left of the range is appended past the end and rotated into place.

    \tparam InputIterator Iterator type; satisfies \c std::input_iterator.
    \tparam Sentinel      End marker; satisfies \c std::sentinel_for with \a InputIterator.

    \param pos     Offset of the first character to replace.
    \param removed Number of characters to replace.
    \param first   Iterator to the first character to copy.
    \param last    Sentinel past the last character to copy.

    \pre \a pos plus \a removed is not greater than size().
    \pre A range that is not contiguous shares no bytes with the storage.
    \pre The storage accepts the resulting length, checked by assert_message in debug builds.

    \post The characters before \a pos are unchanged, the range starts at \a pos, and the characters that followed the
          replaced ones follow it. When the storage rejects the resulting length, the string keeps its old length: a
          single-pass range leaves the replaced characters overwritten by its first ones, and any other range leaves
          the string unchanged.

    \sa _replace()
  */
  template <std::input_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
  constexpr void _replaceRange(size_type pos, size_type removed, InputIterator first, Sentinel last) noexcept;
};

/*!
  \brief String whose characters live in a buffer of a compile-time size inside the object.

  Allocates nothing, so the type works on targets without a heap.

  \tparam Capacity Size of the buffer in bytes, the terminator included; the string holds at most \a Capacity \c - \c 1
                   characters. Must be greater than \c 0, checked by a static assertion in \ref toy::FixedStringStorage.

  \sa \ref toy::String
*/
template <size_t Capacity>
using FixedString = String<FixedStringStorage<Capacity>>;

} // namespace toy

#endif // INCLUDE_CORE_STRING_HPP_
