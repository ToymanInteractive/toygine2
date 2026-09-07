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
  \file   version.hpp
  \brief  Version of a build, and the order two versions compare in.

  Defines \ref toy::application::Version the major, minor, and patch numbers of a build plus the revision it was built
  from. Used where an application names its build to a log line or checks it against saved data.

  \note Included by application.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_APPLICATION_VERSION_HPP_
#define INCLUDE_APPLICATION_VERSION_HPP_

namespace toy::application {

/*!
  \struct Version
  \brief  Major, minor, and patch numbers of a build, with the revision it was built from.

  Aggregate the caller fills field by field; every member has a default, so an omitted number reads \c 0 and an omitted
  revision holds no characters. The three numbers rank the release. \a revision names the source of the build, a commit
  hash or a tag, and settles the order when the numbers match.

  \section version_features Key Features

  * **Aggregate initialization**: no constructor, so designated initializers name the fields at the call site.
  * **Constexpr support**: construction and both comparisons evaluate in a constant expression.
  * **Ordered**: equality over all four fields, and a three-way order that reads the numbers first and the revision
    last.
  * **No allocation**: three integers and a view; the revision characters belong to whoever created them.
  * **Determinism**: the order consults no locale and holds across runs and targets.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section version_usage Usage Example

  \code
  #include "application.hpp"

  const toy::application::Version build{1, 4, 2, "9f3c1ab"};
  constexpr toy::application::Version minimum{.major = 1, .minor = 4};

  const bool supported = build >= minimum;
  \endcode

  \section version_performance Performance Characteristics

  * **Construction**: O(1) for the numbers; a revision built from a pointer costs one scan for the terminator.
  * **Access**: O(1); every field is read directly.
  * **Equality**: O(1) on the numbers, plus O(n) in the length of the revision when all three numbers match.
  * **Ordering**: O(1) until the first number that differs, plus O(n) in the shorter revision when none does.
  * **Memory usage**: three 32-bit numbers, then the pointer and length of the view; 32 bytes on a 64-bit target.

  \section version_safety Safety Guarantees

  * **Lifetime**: \a revision views characters it does not own, so the string has to outlive the version.
  * **Contracts**: none of its own; \a revision inherits what \ref toy::StringView checks.
  * **Memory safety**: nothing allocates, and the numbers live in the object itself.
  * **Type safety**: a \c nullptr revision meets the deleted constructor of \ref toy::StringView and fails to compile.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \note A version built without a revision compares equal only to another one that carries none.

  \sa \ref toy::StringView
*/
struct Version {
  uint32_t major{0};     ///< Major number, compared first

  uint32_t minor{0};     ///< Minor number, compared when the major numbers match

  uint32_t patch{0};     ///< Patch number, compared when the major and minor numbers match

  StringView revision{}; ///< Source the build came from, compared last; empty when the build names none

  /*!
    \brief Reports whether two versions name the same build.

    Compares all four fields, the revision by its bytes.

    \param lhs Version on the left of the operator.
    \param rhs Version on the right of the operator.

    \return \c true when the three numbers and the revision all match, \c false otherwise.

    \note The compiler synthesizes \c != from this operator.

    \sa operator<=>()
  */
  [[nodiscard]] friend constexpr bool operator==(const Version & lhs, const Version & rhs) noexcept = default;

  /*!
    \brief Orders two versions by number, and by revision when every number matches.

    Reads the major, minor, and patch numbers in that order and returns at the first one that differs. Three equal
    numbers hand the decision to the revision, which orders lexicographically by byte.

    \param lhs Version on the left of the operator.
    \param rhs Version on the right of the operator.

    \return \c strong_ordering::less when \a lhs orders first, \c strong_ordering::equal when both hold the same numbers
            and the same revision, \c strong_ordering::greater when \a rhs orders first.

    \note The compiler synthesizes \c <, \c <=, \c >, and \c >= from this operator.
    \note The order consults no locale, which makes it identical across runs and targets.

    \warning An empty revision orders before every other, so a build that names one orders after the same numbers that
             name none. Semantic versioning ranks a pre-release the other way.

    \sa operator==()
  */
  friend constexpr strong_ordering operator<=>(const Version & lhs, const Version & rhs) noexcept;
};

} // namespace toy::application

#endif // INCLUDE_APPLICATION_VERSION_HPP_
