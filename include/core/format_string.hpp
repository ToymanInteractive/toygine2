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
  \brief  Compile-time validated format-string wrapper for auto \c {} and positional \c {N} placeholders.

  Defines \ref toy::FormatString for checking brace pairing, placeholder mode (auto vs positional), and consistency with
  \c sizeof...(Args) at compile time.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_STRING_HPP_
#define INCLUDE_CORE_FORMAT_STRING_HPP_

#include "string_like.hpp"

namespace toy {

/*!
  \class FormatString

  \ingroup Format

  \brief Wrapper around a \ref toy::CStringView pattern with \c consteval validation of placeholders.

  Two modes are supported, not mixed in one pattern: **auto-indexed** placeholders are exactly \c {}; **positional**
  placeholders are \c {0}, \c {1}, … (decimal digits only). \c {{ and \c }} are literal braces. In auto mode the number
  of \c {} must equal \c sizeof...(Args). In positional mode each index must be \c < sizeof...(Args); repeating an index
  is allowed and not every index need appear. Unmatched \c { or \c } makes construction ill-formed.

  \tparam Args Template parameter pack representing the types of arguments that will be formatted.

  \section features Key Features

  - **Compile-time checks**: \c consteval constructor; invalid patterns fail at compile time.
  - **Auto and positional modes**: \c {} vs \c {N}; mixing both in one string is ill-formed.
  - **Storage**: Holds a \ref toy::CStringView to the pattern; no allocation.
  - **get()**: Returns the same view; \c constexpr \c noexcept.

  \section usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::FormatString<int, float> autoFmt(toy::CStringView("Value: {}, Float: {}"));
  constexpr toy::FormatString<int, float> posFmt(toy::CStringView("Float: {1}, Value: {0}"));

  // Ill-formed examples (do not compile):
  // toy::FormatString<int, int>(toy::CStringView("{} {0}")); // mixed auto and positional
  // toy::FormatString<int>(toy::CStringView("{"));           // unmatched brace
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

  \sa \ref toy::CStringView
*/
template <class... Args>
class FormatString {
public:
  /*!
    \brief Validates \a string at compile time and stores it as a \ref toy::CStringView.

    \param string Format pattern to validate and store.

    \pre Braces in \a string are balanced; \c {{ and \c }} are literals.
    \pre Either: auto mode — number of \c {} equals \c sizeof...(Args); or positional mode — each \c {N} has \c N \c <
         \c sizeof...(Args), and the pattern does not mix \c {} with \c {N}.

    \post The pattern is stored and can be read with get().

    \note \c consteval; ill-formed patterns make the program ill-formed.
    \note Escaped \c {{ and \c }} are not placeholders.

    \sa FormatString(const char *)
    \sa get()
  */
  consteval explicit(false) FormatString(const CStringView & string) noexcept;

  /*!
    \brief Validates \a string at compile time and stores it as a \ref toy::CStringView.

    Convenience overload accepting a null-terminated C string directly, avoiding the two-step implicit conversion
    through \ref toy::CStringView that would otherwise be needed at call sites.

    \param string Null-terminated format pattern to validate and store.

    \pre \a string is not null and is a valid null-terminated character sequence.
    \pre Braces in \a string are balanced; \c {{ and \c }} are literals.
    \pre Either: auto mode — number of \c {} equals \c sizeof...(Args); or positional mode — each \c {N} has \c N \c <
         \c sizeof...(Args), and the pattern does not mix \c {} with \c {N}.

    \post The pattern is stored and can be read with get().

    \note \c consteval; ill-formed patterns make the program ill-formed.
    \note Escaped \c {{ and \c }} are not placeholders.

    \sa FormatString(const CStringView &)
    \sa get()
  */
  consteval explicit(false) FormatString(const char * string) noexcept;

  /*!
    \brief Default copy constructor.

    \param other FormatString to copy.
  */
  FormatString(const FormatString & other) noexcept = default;

  /// Copy assignment is deleted.
  FormatString & operator=(const FormatString &) = delete;

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
    \brief Stub used when validation fails; the program should be ill-formed before a call is needed.

    \param message Diagnostic text for implementations that surface it.

    \note Intended only from the \c consteval constructor path.

    \sa FormatString(const CStringView &)
  */
  static void _compileTimeError(const char * message) noexcept;
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_STRING_HPP_
