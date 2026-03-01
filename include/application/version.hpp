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
*/

#ifndef INCLUDE_APPLICATION_VERSION_HPP_
#define INCLUDE_APPLICATION_VERSION_HPP_

namespace toy::application {

/*!
  \struct Version
  \brief Application version information structure.

  Version represents a semantic version number following the major.minor.maintenance.revision format. This structure
  is designed for compile-time version management and provides a simple way to track application versions throughout the
  development lifecycle.

  \section features Key Features

  - **Semantic versioning**: major.minor.maintenance.revision format.
  - **Constexpr**: Usable in constexpr contexts; comparison operators are constexpr.
  - **Exception safety**: All operations are \c noexcept.
  - **Type safety**: \c uint32_t components; no implicit conversions.

  \section usage Usage Example

  \code
  #include "application.hpp"

  constexpr toy::application::Version version;
  constexpr toy::application::Version customVersion{1, 2, 3, 4};

  if (customVersion >= toy::application::Version{1, 0, 0, 0}) {
    // API 1.x or later
  }
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1).
  - **Access**: O(1) for all members.
  - **Comparison**: O(1) for \c operator== and \c operator<=>.
  - **Memory**: 16 bytes (4 × \c uint32_t).

  \section safety Safety Guarantees

  - **Type safety**: Strong typing; no overflow in normal version ranges.
  - **Exception safety**: All operations are \c noexcept; no dynamic allocation.
  - **Thread safety**: Trivially copyable; safe to copy across threads.

  \section compatibility Compatibility

  - **Semantic versioning**: Aligns with semver.org (major.minor.patch); \c maintenance and \c revision map to
    patch/build.
  - **ABI**: Plain layout; safe to pass across translation units and use in stable ABIs.

  \note For runtime parsing from strings, use separate utility functions.

  \sa https://semver.org/
*/
struct Version {
  /// Major version; incompatible API changes.
  uint32_t major = 0;

  /// Minor version; backward-compatible additions.
  uint32_t minor = 0;

  /// Maintenance version; backward-compatible fixes.
  uint32_t maintenance = 0;

  /// Revision (e.g. build or patch level).
  uint32_t revision = 0;
};

/*!
  \brief Equality of two \ref toy::application::Version values.

  \param lhs Left-hand side version.
  \param rhs Right-hand side version.

  \return \c true if all components are equal, \c false otherwise.
*/
[[nodiscard]] constexpr bool operator==(const Version & lhs, const Version & rhs) noexcept;

/*!
  \brief Three-way comparison of two \ref toy::application::Version values (lexicographic order).

  Compares major, then minor, then maintenance, then revision.

  \param lhs Left-hand side version.
  \param rhs Right-hand side version.

  \return \c strong_ordering::less, \c strong_ordering::equal, or \c strong_ordering::greater.
*/
[[nodiscard]] constexpr strong_ordering operator<=>(const Version & lhs, const Version & rhs) noexcept;

} // namespace toy::application

#endif // INCLUDE_APPLICATION_VERSION_HPP_
