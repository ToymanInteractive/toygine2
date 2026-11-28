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
  \file   format_to.hpp
  \brief  The pattern walk and the per-type formatters it dispatches to.

  Defines toy::detail::formatTo(), which walks a pattern and writes its arguments through a
  \ref toy::detail::FormatContext. Also defines toy::detail::formatChar(), toy::detail::formatBool(), and sink-based
  variants for reuse by callers and the format entry points.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_TO_HPP_
#define INCLUDE_CORE_DETAIL_FORMAT_TO_HPP_

#include "core/detail/format_arg.hpp"
#include "core/detail/format_context.hpp"
#include "core/detail/format_pattern.hpp"

namespace toy::detail {

/*!
  \brief Writes a truth value as the word \c true or \c false through \a sink.

  \tparam SinkT Sink type the helper writes through.

  \param sink  Sink the word goes to.
  \param value Value to write.

  \return Count of bytes handed to \a sink.
*/
template <typename SinkT>
[[nodiscard]] constexpr size_t formatBoolTo(SinkT & sink, bool value) noexcept;

/*!
  \brief Writes one character through \a sink.

  \tparam SinkT Sink type the helper writes through.

  \param sink      Sink the character goes to.
  \param character Byte to write.

  \return Count of bytes handed to \a sink.
*/
template <typename SinkT>
[[nodiscard]] constexpr size_t formatCharTo(SinkT & sink, char character) noexcept;

/*!
  \brief Writes one packed argument through the formatter its tag selects.

  \tparam SinkT Sink type the rendered argument goes to.

  \param sink Sink the rendered argument goes to.
  \param arg  Packed argument to render.

  \return Count of bytes handed to \a sink.
*/
template <typename SinkT>
[[nodiscard]] constexpr size_t dispatchArgument(SinkT & sink, const FormatArg & arg) noexcept;

/*!
  \brief Walks a format pattern and writes its literals and arguments through the sink.

  \tparam SinkT Sink type the walk writes through; a FormatContext or any type with write() and put().

  \param sink    Sink the output goes to.
  \param pattern Pattern to walk.
  \param args    Arguments the placeholders name.

  \return Count of bytes handed to \a sink.

  \note Both formatTo() and the fixed-string entry points use the same walk, so they share one grammar.
*/
template <typename SinkT>
constexpr size_t walk(SinkT & sink, StringView pattern, span<const FormatArg> args) noexcept;

/*!
  \brief Writes one character through the sink.

  \param sink      Sink the character goes to.
  \param character Byte to write.

  \post The sink received \a character.
*/
constexpr void formatChar(FormatContext sink, char character) noexcept;

/*!
  \brief Writes a truth value through the sink as the word \c true or \c false.

  \param sink  Sink the word goes to.
  \param value Value to write.

  \post The sink received \c "true" for a \c true \a value and \c "false" otherwise.
*/
constexpr void formatBool(FormatContext sink, bool value) noexcept;

/*!
  \brief Walks a format pattern and writes its literals and arguments through the sink.

  Writes the pattern's literal characters, collapses a doubled brace to a single one in the output, and replaces each
  placeholder with the argument it names: an auto placeholder takes the next argument in order, a positional one indexes
  the pack. Each argument renders through the formatter its tag selects.

  \param sink    Sink the output goes to.
  \param pattern Pattern to walk.
  \param args    Arguments the placeholders name.

  \return Count of bytes handed to \a sink.

  \pre \a pattern is valid for \a args.size() per toy::detail::validateFormatPattern(); an out-of-range or unparsable
       placeholder writes nothing and a stray brace writes as a literal.

  \post The sink received the pattern's literals, each doubled brace collapsed to one brace, and the rendered
        arguments in placeholder order.

  \sa toy::detail::validateFormatPattern(const StringType &, size_t)
*/
constexpr size_t formatTo(FormatContext sink, StringView pattern, span<const FormatArg> args) noexcept;

} // namespace toy::detail

#include "format_to.inl"

#endif // INCLUDE_CORE_DETAIL_FORMAT_TO_HPP_
