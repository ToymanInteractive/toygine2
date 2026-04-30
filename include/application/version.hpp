//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \file   version.hpp
  \brief  Application version type and comparison operators.

  Defines \ref toy::application::Version and declares comparison operators.

  \note Included by application.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_APPLICATION_VERSION_HPP_
#define INCLUDE_APPLICATION_VERSION_HPP_

namespace toy::application {

/*!
  \struct Version
  \brief Application version information structure.

  Version represents a semantic version number following the major.minor.patch.revision format. This structure
  is designed for compile-time version management and provides a simple way to track application versions throughout the
  development lifecycle. The \c revision field holds a non-owning string view (e.g. a git commit hash) whose lifetime
  must exceed that of the \c Version value.

  \section features Key Features

  - **Semantic versioning**: major.minor.patch.revision format.
  - **Constexpr**: Usable in constexpr contexts; comparison operators are constexpr.
  - **Exception safety**: All operations are \c noexcept.
  - **Non-owning revision**: \c revision is a \ref toy::CStringView; no allocation, zero copy cost.

  \section usage Usage Example

  \code
  #include "application.hpp"

  constexpr toy::application::Version version;
  constexpr toy::application::Version customVersion{1, 2, 3, "abc1234d"};

  if (customVersion >= toy::application::Version{1, 0, 0, {}}) {
    // API 1.x or later
  }
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1).
  - **Access**: O(1) for all members.
  - **Comparison**: O(n) for \c operator== and \c operator<=>, where \a n is the revision string length.
  - **Memory**: 3 × \c uint32_t + one pointer (typically 24 bytes on 64-bit targets due to alignment padding).

  \section safety Safety Guarantees

  - **Exception safety**: All operations are \c noexcept; no dynamic allocation.
  - **Thread safety**: Trivially copyable; safe to copy across threads.
  - **Revision lifetime**: The string referenced by \c revision must outlive the \c Version value.

  \section compatibility Compatibility

  - **Semantic versioning**: Aligns with semver.org (major.minor.patch); \c revision carries a build identifier such as
    a git commit hash.
  - **ABI**: Layout includes a pointer-sized field; size is platform-dependent.

  \note For runtime parsing from strings, use separate utility functions.

  \warning The string referenced by \c revision must outlive the \c Version value. Storing a \c Version that points to a
           temporary or stack-allocated buffer beyond that buffer's lifetime causes undefined behavior. Use a string
           literal or storage with static lifetime.

  \sa https://semver.org/
*/
struct Version {
  /// Major version; incompatible API changes.
  uint32_t major{0};

  /// Minor version; backward-compatible additions.
  uint32_t minor{0};

  /// Patch version; backward-compatible fixes.
  uint32_t patch{0};

  /// Git revision string (e.g. commit hash). Must point to storage that outlives this \c Version value.
  CStringView revision{};

  /*!
    \brief Equality of two \ref toy::application::Version values.

    \param lhs Left-hand side version.
    \param rhs Right-hand side version.

    \return \c true if all components are equal, \c false otherwise.
  */
  [[nodiscard]] friend constexpr bool operator==(const Version & lhs, const Version & rhs) noexcept {
    return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch && lhs.revision == rhs.revision;
  }

  /*!
    \brief Three-way comparison of two \ref toy::application::Version values (lexicographic order).

    Compares major, then minor, then patch, then revision.

    \param lhs Left-hand side version.
    \param rhs Right-hand side version.

    \return \c strong_ordering::less, \c strong_ordering::equal, or \c strong_ordering::greater.
  */
  [[nodiscard]] friend constexpr strong_ordering operator<=>(const Version & lhs, const Version & rhs) noexcept {
    if (const auto cmp = lhs.major <=> rhs.major; cmp != strong_ordering::equal)
      return cmp;

    if (const auto cmp = lhs.minor <=> rhs.minor; cmp != strong_ordering::equal)
      return cmp;

    if (const auto cmp = lhs.patch <=> rhs.patch; cmp != strong_ordering::equal)
      return cmp;

    return lhs.revision <=> rhs.revision;
  }
};

} // namespace toy::application

#endif // INCLUDE_APPLICATION_VERSION_HPP_
