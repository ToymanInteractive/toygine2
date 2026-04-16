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

  Defines toy::format() and toy::formatTo() for building formatted strings using compile-time validated
  \ref toy::FormatString patterns (auto-indexed \c {} and positional \c {N} modes; escaped \c {{ and \c }} emit
  literal braces). Also defines toy::makeVFormatArguments() which type-erases a parameter pack into a array of
  \ref toy::FormatArgument for use with toy::vformat() and toy::vformatTo().

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_HPP_
#define INCLUDE_CORE_FORMAT_HPP_

#include "format_argument.hpp"
#include "format_string.hpp"
#include "o_string_stream.hpp"

namespace toy {

/*!
  \ingroup String

  \brief Formats arguments into a new \ref toy::FixedString using a compile-time validated pattern.

  Iterates \a fmt, copies literal text verbatim, and substitutes each \c {} or \c {N} placeholder with the corresponding
  argument formatted via \c operator<< on \ref toy::OStringStream.

  \tparam BufferSize Capacity of the returned \ref toy::FixedString in bytes.
  \tparam Args       Types of format arguments; deduced from \a args.

  \param fmt  Compile-time validated format pattern.
  \param args Values to substitute at each placeholder.

  \return A \ref toy::FixedString<BufferSize> containing the formatted result.

  \pre Each argument type supports \c operator<< on \ref toy::OStringStream.
  \pre \a fmt is a valid \ref toy::FormatString; validation occurs at compile time.

  \note \c {{ and \c }} in the pattern emit a literal \c { and \c } respectively.
  \note When the formatted result exceeds \a BufferSize characters the output is silently truncated per
        \ref toy::FixedString capacity semantics.

  \sa formatTo()
*/
template <size_t BufferSize, typename... Args>
[[nodiscard]] constexpr FixedString<BufferSize> format(type_identity_t<FormatString<Args...>> fmt,
                                                       const Args &... args) noexcept;

/*!
  \ingroup String

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

  \sa format()
*/
template <OStringStreamBackend BackendType, typename... Args>
constexpr void formatTo(BackendType & output, type_identity_t<FormatString<Args...>> fmt,
                        const Args &... args) noexcept;

/*!
  \ingroup String

  \brief Formats arguments into \a output using a runtime pattern, type-erasing them automatically.

  Iterates \a pattern, copies literal text to \a output verbatim, and substitutes each \c {} or \c {N} placeholder with
  the corresponding argument. Each argument is type-erased via toy::makeVFormatArguments() and forwarded to the array
  overload of toy::vformatTo(). The pattern is validated at runtime; an invalid pattern triggers a debug assertion.

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
  \note Pattern validity is checked at runtime via \c assert_message with validateFormatPattern(); use toy::formatTo()
        for compile-time checks.

  \sa toy::vformatTo(BackendType &, CStringView, const array<FormatArgument, MaxArgs> &), toy::makeVFormatArguments()
*/
template <OStringStreamBackend BackendType, typename... Args>
void vformatTo(BackendType & output, CStringView pattern, const Args &... args) noexcept;

/*!
  \ingroup String

  \brief Formats type-erased arguments into \a output using a runtime pattern.

  Iterates \a pattern, copies literal text to \a output verbatim, and substitutes each \c {} or \c {N} placeholder by
  calling the corresponding entry in \a args. Output is written through a \ref toy::FormatContext backed by \a output,
  so the formatting loop is decoupled from the concrete backend type. The pattern is validated at runtime; an invalid
  pattern triggers a debug assertion.

  \tparam BackendType Type of the output string. Must satisfy the \ref toy::OStringStreamBackend concept.
  \tparam MaximumArgs Number of type-erased arguments; deduced from \a args.

  \param output  Destination string; its previous content is replaced.
  \param pattern Runtime format pattern; may be a variable or computed string.
  \param args    Type-erased arguments produced by toy::makeVFormatArguments().

  \pre \a pattern must be a valid format pattern consistent with \a MaximumArgs (same rules as \ref toy::FormatString).
  \pre Each \ref toy::FormatArgument in \a args must remain valid for the duration of the call.

  \post \a output holds the formatted result, subject to \a BackendType capacity semantics.

  \note \c {{ and \c }} in the pattern emit a literal \c { and \c } respectively.
  \note Pattern validity is checked at runtime via \c assert_message with validateFormatPattern(); use toy::formatTo()
        for compile-time checks.

  \sa toy::vformat(), toy::vformatTo(BackendType &, CStringView, const Args &...), toy::makeVFormatArguments()
*/
template <OStringStreamBackend BackendType, size_t MaximumArgs>
void vformatTo(BackendType & output, CStringView pattern, const array<FormatArgument, MaximumArgs> & args) noexcept;

/*!
  \ingroup String

  \brief Constructs an array of \ref toy::FormatArgument from a pack of arguments by type-erasing each one.

  For each argument a \ref toy::FormatArgument is created that stores a pointer to the original value and a stateless
  formatter callback that knows the concrete type. Three formatting paths are selected at compile time:
  \ref toy::StringLike arguments write \c c_str() / \c size() directly, null-terminated \c char pointers write via
  \c std::char_traits<char>::length(), and all other types are serialized through a temporary \ref toy::OStringStream
  backed by a \ref toy::FixedString<128>. The returned array borrows references to the original arguments and must not
  outlive them.

  \tparam Args Types of the format arguments; deduced from \a args.

  \param args Values to type-erase.

  \return A \c std::array<FormatArgument, \c sizeof...(Args)> with one entry per argument, ready to be passed to
          toy::vformat() or toy::vformatTo().

  \pre Each type in \a Args must be formattable via \c operator<< on \ref toy::OStringStream, or must satisfy
       \ref toy::StringLike, or must be a null-terminated \c char pointer.
  \pre The returned object must not be used after any of the original \a args go out of scope.

  \note When called with an empty argument pack the returned array has zero elements and the function body is a no-op.
  \note A null \c char pointer argument produces no output when the corresponding \a formatFn is invoked.

  \sa FormatArgument, FormatContext, toy::makeVFormatArgs()
*/
template <typename... Args>
[[nodiscard]] array<FormatArgument, sizeof...(Args)> makeVFormatArguments(const Args &... args) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_HPP_
