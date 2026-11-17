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
  \file   format_string.hpp
  \brief  Format patterns validated at compile time against their argument counts.

  Defines \ref toy::FormatString, a view over a pattern whose placeholders are checked when the pattern is built, so a
  broken one fails the build instead of the first formatted call. Used wherever a formatter receives a pattern from a
  call site.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_STRING_HPP_
#define INCLUDE_CORE_FORMAT_STRING_HPP_

#include "string_like.hpp"
#include "string_view.hpp"

namespace toy {

/*!
  \class FormatString
  \brief A format pattern validated at compile time against its argument type list.

  Construction is consteval: a pattern that breaks any rule of toy::validateFormatPattern() fails the build, and a valid
  one survives as a \ref toy::StringView over the source. The object holds only the pointer and the length, so the
  source has to outlive it.

  \tparam Args Types the pattern's placeholders bind. Only the count takes part in the check; no requirement is placed
               on the types themselves.

  \section format_string_features Key Features

  * **Compile-time validation**: an invalid pattern fails the build, and no runtime check remains.
  * **Any string-like source**: a literal through the C-string constructor, any type satisfying \ref toy::StringLike
    through the template constructor.
  * **Constexpr support**: construction and get() evaluate in a constant expression, and the object lives at runtime.
  * **Minimal footprint**: one \ref toy::StringView per object; nothing of the check reaches the binary.

  \section format_string_usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::FormatString<> plain{"no placeholders"};
  toy::FormatString<int, int> positional{"{0} then {1}"};
  const toy::StringView view = positional.get();
  \endcode

  \section format_string_performance Performance Characteristics

  * **Construction**: walks the pattern once, linear in its length, at compile time.
  * **get()**: returns the stored view, constant time.
  * **Memory usage**: one \ref toy::StringView per object; the pattern stays where the caller put it.

  \section format_string_safety Safety Guarantees

  * **Contracts**: a pattern toy::validateFormatPattern() rejects fails the build; a source that violates a constructor
    precondition fails under assert_message in debug builds.
  * **Type safety**: sources are constrained by \ref toy::StringLike or taken as a null-terminated pointer; the
    placeholder types are counted, not constrained.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \note The source must be readable in a constant expression because the constructor is consteval; a string built at
        runtime cannot feed it.

  \sa \ref toy::FormatPatternValidationError
  \sa \ref toy::StringLike
  \sa toy::validateFormatPattern()
*/
template <class... Args>
class FormatString {
public:
  /*!
    \brief Builds the pattern from a null-terminated string, the length taken from its terminator.

    Validates the pattern against the argument count and stores a view over its characters without copying them.

    \param string Null-terminated pattern to validate and view.

    \pre \a string is non-null, checked by assert_message in debug builds.
    \pre \a string outlives the object and keeps its terminator in place.

    \sa FormatString(const StringType &)
  */
  consteval explicit(false) FormatString(const char * string) noexcept;

  /*!
    \brief Builds the pattern from a string-like object, the length taken from its size().

    Validates the pattern against the argument count and stores a view over the characters c_str() points to; the source
    is not copied.

    \tparam StringType Source type; satisfies \ref toy::StringLike.

    \param string Pattern to validate and view.

    \pre \a string returns a non-null c_str() with \c '\\0' at offset size(), checked by assert_message in debug builds.
    \pre \a string outlives the object and keeps its characters in place.

    \sa FormatString(const char *)
  */
  template <StringLike StringType>
  consteval explicit(false) FormatString(const StringType & string) noexcept;

  /*!
    \brief Builds a pattern reading the same source as \a other.

    \param other Pattern to copy the pointer and the length from.

    \post Both objects read the same characters; neither owns them.
  */
  FormatString(const FormatString & other) noexcept = default;

  /*!
    \brief Deleted: the pattern is fixed when the object is built.

    Construct a fresh object to read another pattern.

    \sa FormatString(const FormatString &)
  */
  FormatString & operator=(const FormatString &) = delete;

  /*!
    \brief Returns the pattern as written, escaped braces included.

    \return A view over the source; the same characters in a constant expression and at runtime.
  */
  [[nodiscard]] constexpr StringView get() const noexcept;

private:
  /// A view over the source the constructors validated.
  const StringView _source;

  /*!
    \brief Aborts the constant evaluation when the pattern check fails, which rejects the pattern at compile time.

    \param message Rule the pattern violates.
  */
  static void _compileTimeError(const char * message) noexcept;
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_STRING_HPP_
