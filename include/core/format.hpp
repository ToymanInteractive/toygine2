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
  \file   format.hpp
  \brief  Format functions for substituting arguments into \c {} and \c {N} placeholders.

  Defines toy::format(), toy::formatTo(), toy::vformat(), and toy::vformatTo() for building formatted strings.
  toy::format() and toy::formatTo() use compile-time validated \ref toy::FormatString patterns; toy::vformat() and
  toy::vformatTo() accept runtime patterns and type-erase arguments into arrays internally.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_HPP_
#define INCLUDE_CORE_FORMAT_HPP_

#include "format_string.hpp"
#include "o_string_stream.hpp"

/*!
  \defgroup format Format
  \ingroup String
  \brief Functions for substituting arguments into \c {} and \c {N} placeholders in a format pattern.
*/

namespace toy {

/*!
  \ingroup format

  \brief Formats arguments into a new \ref toy::FixedString using a compile-time validated pattern.

  Iterates \a fmt, copies literal text verbatim, and substitutes each \c {} or \c {N} placeholder with the corresponding
  argument formatted via \c operator<< on \ref toy::OStringStream.

  \tparam BufferSize Capacity of the returned \ref toy::FixedString in bytes.
  \tparam Args       Types of format arguments; deduced from \a args.

  \param fmt  Compile-time validated format pattern.
  \param args Values to substitute at each placeholder.

  \return A \ref toy::FixedString containing the formatted result.

  \pre Each argument type supports \c operator<< on \ref toy::OStringStream.
  \pre \a fmt is a valid \ref toy::FormatString; validation occurs at compile time.

  \note \c {{ and \c }} in the pattern emit a literal \c { and \c } respectively.
  \note When the formatted result exceeds \a BufferSize characters the output is silently truncated per
        \ref toy::FixedString capacity semantics.

  \sa toy::formatTo()
*/
template <size_t BufferSize, typename... Args>
[[nodiscard]] constexpr FixedString<BufferSize> format(type_identity_t<FormatString<Args...>> fmt,
                                                       const Args &... args) noexcept;

/*!
  \ingroup format

  \brief Formats arguments into \a output using a compile-time validated pattern.

  Replaces the content of \a output with the formatted result of \a fmt applied to \a args. Literal text is copied
  verbatim; \c {} and \c {N} placeholders are replaced by the corresponding argument formatted via \c operator<< on
  \ref toy::OStringStream.

  \tparam BackendType Type of the output string. Must satisfy the \ref toy::OStringStreamBackend concept.
  \tparam Args        Types of format arguments; deduced from \a args.

  \param output Destination string; its previous content is replaced.
  \param fmt    Compile-time validated format pattern.
  \param args   Values to substitute at each placeholder.

  \pre Each argument type supports \c operator<< on \ref toy::OStringStream.
  \pre \a fmt is a valid \ref toy::FormatString; validation occurs at compile time.

  \post \a output holds the formatted result, subject to \a BackendType capacity/overflow semantics.

  \note \c {{ and \c }} in the pattern emit a literal \c { and \c } respectively.
  \note Overflow semantics match those of \a BackendType.

  \sa toy::format()
*/
template <OStringStreamBackend BackendType, typename... Args>
constexpr void formatTo(BackendType & output, type_identity_t<FormatString<Args...>> fmt,
                        const Args &... args) noexcept;

/*!
  \ingroup format

  \brief Formats arguments into a new \ref toy::FixedString using a runtime pattern.

  Delegates to toy::vformatTo(), which type-erases \a args into an array internally.

  \tparam BufferSize Capacity of the returned \ref toy::FixedString in bytes.
  \tparam Args       Types of format arguments; deduced from \a args.

  \param pattern Runtime format pattern.
  \param args    Values to substitute at each placeholder.

  \return A \ref toy::FixedString containing the formatted result.

  \pre \a pattern must be a valid format pattern consistent with \c sizeof...(Args).

  \sa toy::vformatTo()
*/
template <size_t BufferSize, typename... Args>
[[nodiscard]] FixedString<BufferSize> vformat(CStringView pattern, const Args &... args) noexcept;

/*!
  \ingroup format

  \brief Formats arguments into \a output using a runtime pattern, type-erasing them automatically.

  Type-erases each argument, then iterates \a pattern, copies literal text to \a output verbatim, and substitutes each
  \c {} or \c {N} placeholder with the corresponding type-erased argument. The pattern is validated at runtime; an
  invalid pattern triggers a debug assertion.

  \tparam BackendType Type of the output string. Must satisfy the \ref toy::OStringStreamBackend concept.
  \tparam Args        Types of format arguments; deduced from \a args.

  \param output  Destination string; its previous content is replaced.
  \param pattern Runtime format pattern; may be a variable or computed string.
  \param args    Values to substitute at each placeholder.

  \pre \a pattern must be a valid format pattern consistent with \c sizeof...(Args) (same rules as
       \ref toy::FormatString).
  \pre Each argument in \a args must remain valid for the duration of the call.

  \post \a output holds the formatted result, subject to \a BackendType capacity semantics.

  \note \c {{ and \c }} in the pattern emit a literal \c { and \c } respectively.
  \note Pattern validity is checked at runtime via \c assert_message with toy::validateFormatPattern(); use
        toy::formatTo() for compile-time checks.

  \sa toy::vformat()
*/
template <OStringStreamBackend BackendType, typename... Args>
void vformatTo(BackendType & output, CStringView pattern, const Args &... args) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_HPP_
