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
  \file   format_string.hpp
  \brief  Compile-time validated format-string wrapper for \c {} placeholders.

  Defines \ref toy::FormatString for checking brace pairing and placeholder count against the template argument pack at
  compile time.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_STRING_HPP_
#define INCLUDE_CORE_FORMAT_STRING_HPP_

#include "string_like.hpp"

namespace toy {

/*!
  \class FormatString
  \ingroup String
  \brief Wrapper around a \ref toy::CStringView pattern with \c consteval validation of \c `{}` placeholders.

  Placeholders are exactly the two-character sequence \c {}; \c {{ and \c }} are literals and do not increase the
  placeholder count. The number of placeholders must equal \c sizeof...(Args). Unmatched \c { or \c } makes construction
  ill-formed. This type does not perform value formatting; it only stores and returns the pattern.

  \tparam Args Template parameter pack representing the types of arguments that will be formatted.

  \section features Key Features

  - **Compile-time checks**: \c consteval constructor; mismatch or bad braces fail at compile time.
  - **Storage**: Holds a \ref toy::CStringView to the pattern; no allocation.
  - **get()**: Returns the same view; \c constexpr \c noexcept.
  - **Alignment with \c std::format rules**: Placeholder and escape rules mirror the subset described above.

  \section usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::FormatString<int, float> format(toy::CStringView("Value: {}, Float: {}"));
  constexpr toy::CStringView str = format.get();

  // Ill-formed examples (do not compile):
  // toy::FormatString<int> bad(toy::CStringView("{}, {}")); // too many placeholders
  // toy::FormatString<> bad2(toy::CStringView("{"));        // unmatched brace
  \endcode

  \section performance Performance Characteristics

  - **Construction**: Work is done at compile time (\c consteval).
  - **get()**: O(1); returns the stored \ref toy::CStringView.

  \section safety Safety Guarantees

  - **Invalid patterns**: Diagnosed at compile time when the \c consteval constructor runs.
  - **Exception safety**: All operations are \c noexcept; this type does not throw.

  \section compatibility Compatibility

  - **C++23**: \c consteval constructor and \c constexpr \c get() as implemented.
  - **CStringView**: Pattern must be reachable as a \c const \ref toy::CStringView at the call site.

  \note Value substitution is out of scope; pair with formatting utilities that consume the pattern separately.

  \sa CStringView
*/
template <class... Args>
class FormatString {
public:
  /*!
    \brief Validates \a string at compile time and stores it as a \ref toy::CStringView.

    \param string Format pattern to validate and store.

    \pre Braces in \a string are balanced; \c {{ and \c }} are literals.
    \pre Placeholder count (pairs \c {}) equals \c sizeof...(Args).

    \post The pattern is stored and can be read with get().

    \note \c consteval; ill-formed patterns make the program ill-formed.
    \note Escaped \c {{ and \c }} do not count as placeholders.

    \sa get()
  */
  consteval explicit(false) FormatString(const CStringView & string) noexcept;

  /*!
    \brief Returns the stored format pattern as a \ref toy::CStringView.

    \return The same view stored at construction time.

    \note \c constexpr; safe in constant evaluation.

    \sa FormatString(const CStringView &)
  */
  [[nodiscard]] constexpr CStringView get() const noexcept;

private:
  /// The stored format string.
  const CStringView _string;

  /*!
    \brief Counts \c {} placeholders in \a string (escaped \c {{ / \c }} are literals).

    \param string Pattern to scan.

    \return Placeholder count, or \ref toy::CStringView::npos if braces are unmatched.

    \sa CStringView::npos
  */
  [[nodiscard]] static constexpr size_t _countFormatPlaceholders(const CStringView & string) noexcept;

  /*!
    \brief Stub used when validation fails; the program should be ill-formed before a call is needed.

    \param message Diagnostic text for implementations that surface it.

    \note Intended only from the \c consteval constructor path.

    \sa FormatString(const CStringView &)
  */
  static void _compileTimeError(const char * message) noexcept;
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_STRING_HPP_
