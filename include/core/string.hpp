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
  * **Constexpr support**: the default constructor, size(), and c_str() evaluate in a constant expression when the
    storage supports that.
  * **Type safety**: construction from \c nullptr is deleted, so a literal null pointer fails to compile.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section string_usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::FixedString<16> name;

  const size_t       length = name.size();   // 0
  const char * const text   = name.c_str();  // reads ""
  \endcode

  \section string_performance Performance Characteristics

  * **Default construction**: the cost of default-constructing the storage, O(n) in the buffer size for
    \ref toy::FixedStringStorage.
  * **Length and pointer access**: one call into the storage, O(1) for \ref toy::FixedStringStorage.
  * **Memory usage**: the storage object and nothing else.

  \section string_safety Safety Guarantees

  * **Contracts**: the pointer constructor checks its argument against \c nullptr with assert_message in debug builds.
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
    \brief Builds a string holding a copy of a null-terminated byte string.

    Converts implicitly, so a string literal or a \c const \c char \c * argument becomes a string at the call site. The
    characters are copied into the storage, so \a string may be destroyed once the constructor returns.

    \param string Null-terminated byte string to copy.

    \pre \a string is non-null, checked by assert_message in debug builds.

    \post size() returns the length of \a string in bytes, the terminator excluded, and c_str() reads the same
          characters as \a string.

    \sa String()
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
