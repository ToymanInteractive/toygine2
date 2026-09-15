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

  Defines \ref toy::String, which keeps its characters in a storage type modeling \ref toy::StringStorage, and
  \ref toy::FixedString, the same string over a buffer of a compile-time size. Used where one string type has to work
  both over memory inside the object and over memory taken from a heap.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_HPP_
#define INCLUDE_CORE_STRING_HPP_

#include "fixed_string_storage.hpp"
#include "string_storage.hpp"
#include "string_view.hpp"

namespace toy {

/*!
  \class String
  \brief Null-terminated byte string that owns its characters and keeps them in a storage chosen by a template argument.

  Holds one storage object and reads the characters and the length through it, so the storage type decides where the
  bytes live and how many fit. The string itself adds no data member and allocates nothing; any allocation belongs to
  the storage. Unlike \ref toy::StringView, the string owns the characters, and copying the string copies its storage.

  \tparam storageType Buffer the characters live in; satisfies \ref toy::StringStorage.

  \section string_features Key Features

  * **Storage chosen at compile time**: the template argument picks the buffer, and no call goes through a virtual
    function.
  * **Null-terminated**: c_str() returns a pointer that goes to a C interface unchanged.
  * **Value semantics**: a copy is as independent as the storage's own copy; over \ref toy::FixedStringStorage it holds
    its own bytes.
  * **Constexpr support**: every constructor, size(), and c_str() evaluate in a constant expression when the storage
    supports that.
  * **Counted construction**: a pointer and a byte count build a string from a range with no terminator, the way
    \c std::basic_string does.
  * **Type safety**: construction from \c nullptr is deleted, so a literal null pointer fails to compile.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section string_usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::FixedString<16> name("player");
  constexpr toy::FixedString<16> prefix("player", 4);

  const size_t       length = name.size();     // 6
  const char * const text   = prefix.c_str();  // reads "play"
  \endcode

  \section string_performance Performance Characteristics

  * **Default construction**: the cost of default-constructing the storage, O(n) in the buffer size for
    \ref toy::FixedStringStorage.
  * **Construction from a pointer and a count**: default construction plus one copy, O(n) in \a count.
  * **Construction from a pointer**: one scan for the terminator, then the counted construction.
  * **Length and pointer access**: one call into the storage, O(1) for \ref toy::FixedStringStorage.
  * **Memory usage**: the storage object and nothing else.

  \section string_safety Safety Guarantees

  * **Contracts**: the constructors check the source pointer against \c nullptr and the length against the storage
    with assert_message in debug builds. A shipping build skips the checks and builds an empty string when the
    source does not fit.
  * **Type safety**: \a storageType is checked against \ref toy::StringStorage, and the deleted \c nullptr_t
    constructor rejects a literal \c nullptr during compilation.
  * **Lifetime**: c_str() points into the string's own storage and dangles once the string is destroyed.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \note The length counts bytes, not characters; under a multi-byte encoding the two differ.

  \sa \ref toy::StringView
  \sa \ref toy::FixedStringStorage
*/
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

    Default-constructs the storage and allocates only if that storage does.

    \post size() returns \c 0, and c_str() points at a \c '\\0' character.
  */
  constexpr String() noexcept = default;

  /*!
    \brief Builds a string holding a copy of \a count bytes starting at \a string.

    Copies exactly \a count bytes, so the source needs no terminator, and a \c '\\0' inside the range is copied like any
    other byte. The contract follows \c std::basic_string(const CharT *, size_type): a null \a string with a \a count of
    \c 0 names an empty range and builds an empty string.

    \param string First byte of the range to copy.
    \param count  Number of bytes to copy.

    \pre \a string is non-null or \a count is \c 0, checked by assert_message in debug builds.
    \pre The \a count bytes starting at \a string are readable; a shorter range is undefined behavior.
    \pre The storage accepts \a count: its reserve() answers \c true, checked by assert_message in debug builds. For
         \ref toy::FixedString that means \a count is below the buffer size.

    \post size() returns \a count, and c_str() reads the copied bytes followed by \c '\\0'.

    \warning A shipping build skips the capacity check and leaves the string empty when the storage rejects \a count.
             \c std::basic_string reports the same case by throwing \c std::length_error, which leaves no object behind.

    \sa String(const char *)
  */
  constexpr String(const value_type * string, size_type count) noexcept;

  /*!
    \brief Builds a string holding a copy of a null-terminated byte string.

    Converts implicitly, so a string literal or a \c const \c char \c * argument becomes a string at the call site.
    Measures \a string and then builds the string the way String(const char *, size_t) does, so capacity handling is
    the same. The characters are copied into the storage, so \a string may be destroyed once the constructor returns.

    \param string Null-terminated byte string to copy.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre The storage accepts the length of \a string, as String(const char *, size_t) requires.

    \post size() returns the length of \a string in bytes, the terminator excluded, and c_str() reads the same
          characters as \a string.

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

private:
  /// Buffer holding the characters, the terminator, and the length
  storageType _storage;
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
