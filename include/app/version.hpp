//
// Copyright (c) 2025 Toyman Interactive
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
  \brief  Application version management utilities.
*/

#ifndef INCLUDE_APP_VERSION_HPP_
#define INCLUDE_APP_VERSION_HPP_

namespace toy::app {

/*!
  \struct Version
  \brief Application version information structure.

  Version represents a semantic version number following the major.minor.maintenance.revision format. This structure is
  designed for compile-time version management and provides a simple way to track application versions throughout the
  development lifecycle.

  \section features Key Features

  - ⚙️ **Semantic Versioning**: Follows major.minor.maintenance.revision format
  - 🔧 **ConstExpr Support**: All operations can be evaluated at compile time
  - 🛡️ **Exception Safety**: All operations are noexcept
  - 🔗 **Simple Structure**: Plain struct with public members for easy access
  - 🎯 **Version Management**: Designed for application version tracking
  - 🧬 **Type Safety**: Uses uint32_t for all version components
  - 🌍 **Standard Compliance**: Follows semantic versioning standards
  - 📏 **Immutable Design**: Structure designed for compile-time version constants

  \section usage Usage Example

  \code
  #include "app.hpp"

  // Default version (0.0.0.0)
  constexpr toy::app::Version version;

  // Custom version
  constexpr toy::app::Version customVersion{1, 2, 3, 4};

  // Access version components
  std::cout << "Version: " << customVersion.major << "."
            << customVersion.minor << "."
            << customVersion.maintenance << "."
            << customVersion.revision << std::endl;
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(1) constant time
  - 📝 **Access**: O(1) constant time for all members
  - 📝 **Assignment**: O(1) constant time for member assignment
  - 🔗 **Comparison**: O(1) constant time for version comparison
  - 💾 **Memory Usage**: 16 bytes (4 * uint32_t)
  - ⚡ **Cache Performance**: Excellent due to small size and contiguous layout
  - 📋 **Copy Performance**: Fast due to simple integer copying
  - 🎯 **Version Operations**: Optimized for compile-time evaluation

  \section safety Safety Guarantees

  - 🛡️ **Bounds Safety**: All operations are bounds-safe
  - 📐 **Type Safety**: Strong typing with uint32_t components
  - ⚠️ **Exception Safety**: All operations are noexcept, no exceptions thrown
  - 🔒 **Memory Safety**: No dynamic allocation, stack-only structure

  \section compatibility Compatibility

  - 🌐 **Cross-Platform**: Works on all platforms supported by the compiler
  - 🔧 **Embedded Systems**: Suitable for resource-constrained environments
  - 📱 **Mobile Platforms**: Lightweight and efficient for mobile applications

  \note This structure is designed for compile-time version management.
  \note For runtime version parsing from strings, consider implementing additional utility functions.

  \sa Semantic Versioning (https://semver.org/)
*/
struct Version {
  /*!
     \brief Major version number.
     \details Indicates incompatible API changes.
  */
  uint32_t major = 0;

  /*!
   \brief Minor version number.
   \details Indicates backward-compatible functionality additions.
  */
  uint32_t minor = 0;

  /*!
   \brief Maintenance version number.
   \details Indicates backward-compatible bug fixes.
  */
  uint32_t maintenance = 0;

  /*!
   \brief Revision number.
   \details Indicates build number or patch level.
  */
  uint32_t revision = 0;
};

/*!
  \brief Equality operator for two versions.

  Compares two versions for exact equality of all components.

  \param lhs The left-hand side version.
  \param rhs The right-hand side version.

  \return true if all version components are identical, false otherwise.

  \note This performs exact equality comparison. For semantic versioning compatibility, consider using the three-way
        comparison operator.
*/
[[nodiscard]] constexpr bool operator==(const Version & lhs, const Version & rhs) noexcept;

/*!
  \brief Three-way comparison operator for two versions.

  Compares two versions using lexicographical ordering following semantic versioning principles. The comparison
  prioritizes major version over minor, minor over maintenance, and maintenance over revision.

  \param lhs The left-hand side version.
  \param rhs The right-hand side version.

  \return \c strong_ordering::less if lhs < rhs, \c strong_ordering::greater if lhs > rhs, \c strong_ordering::equal if
          lhs == rhs.
*/
[[nodiscard]] constexpr strong_ordering operator<=>(const Version & lhs, const Version & rhs) noexcept;

} // namespace toy::app

#endif // INCLUDE_APP_VERSION_HPP_
