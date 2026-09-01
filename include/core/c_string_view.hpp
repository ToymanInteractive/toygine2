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
  * **Constexpr support**: construction and copying evaluate in a constant expression.
  * **No allocation**: the view holds a pointer and a length, and owns no characters.
  * **Type safety**: construction from \c nullptr is deleted, so the null case fails to compile.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section c_string_view_usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::CStringView name("player");

  constexpr toy::CStringView alias(name);
  \endcode

  \section c_string_view_performance Performance Characteristics

  * **Construction from a pointer**: O(n) in the length of the string, one scan for the terminator.
  * **Default construction and copying**: O(1).
  * **Memory usage**: one pointer and one length, 16 bytes on a 64-bit target and 8 bytes on a 32-bit one.

  \section c_string_view_safety Safety Guarantees

  * **Contracts**: the pointer constructor checks its argument against \c nullptr with assert_message in debug builds.
    A shipping build skips the check and stores the null pointer with a length of \c 0.
  * **Lifetime**: the characters belong to whoever created them; the view neither owns them nor extends their
    lifetime.
  * **Type safety**: the deleted \c nullptr_t constructor rejects a literal \c nullptr during compilation.
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

  [[nodiscard]] constexpr const_iterator begin() const noexcept;

  [[nodiscard]] constexpr const_iterator cbegin() const noexcept;

  [[nodiscard]] constexpr const_iterator end() const noexcept;

  [[nodiscard]] constexpr const_iterator cend() const noexcept;

  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept;

  [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept;

  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept;

  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept;

private:
  /// First character of the viewed string, \c nullptr while the view holds none
  const char * _data{nullptr};

  /// Length of the viewed string in bytes, the terminator excluded
  size_t _size{0};
};

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_HPP_
