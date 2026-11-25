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
  \file   format.hpp
  \brief  Format entry points that render into a fixed-size string.

  Declares toy::format() and toy::vformat(). Both pack arguments into a stack array of \ref toy::detail::FormatArg and
  render through a fixed-capacity sink into a \ref toy::FixedString returned by value. Use them when formatting needs a
  fixed-size result owned by the caller.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_HPP_
#define INCLUDE_CORE_FORMAT_HPP_

#include "detail/format_string.hpp"
#include "string.hpp"

namespace toy {

/*!
  \brief Formats the arguments into a fixed-size string of the given capacity.

  The pattern literal builds a detail::FormatString<Args...> at compile time, so a pattern invalid for the argument
  types fails the build. Each argument packs to its tagged slot and the walk renders it in placeholder order; the output
  clamps byte by byte at the string's capacity.

  \tparam BufferSize Storage size of the returned FixedString; its capacity() is the output budget.
  \tparam Args       Argument types; each one packs through toy::detail::packArg().

  \param fmt   Pattern to walk; a literal or another source FormatString accepts.
  \param args  Arguments the placeholders name.

  \return The filled string, with size() set to the number of bytes written.

  \pre The pattern is valid for sizeof...(Args), checked when the detail::FormatString builds; an invalid one is a
       compile error.
  \pre The output fits capacity() of the returned string; a longer output clamps byte by byte, and a cut may split a
       UTF-8 sequence.

  \sa toy::vformat()
*/
template <size_t BufferSize, typename... Args>
[[nodiscard]] constexpr FixedString<BufferSize> format(std::type_identity_t<detail::FormatString<Args...>> fmt,
                                                       const Args &... args) noexcept
  requires(BufferSize >= 1);

/*!
  \brief Formats the arguments into a fixed-size string from a pattern read at runtime.

  Walks the same engine as toy::format() with a pattern that did not pass a compile-time check.

  \tparam BufferSize Storage size of the returned FixedString; its capacity() is the output budget.
  \tparam Args       Argument types; each one packs through toy::detail::packArg().

  \param pattern Pattern to walk.
  \param args    Arguments the placeholders name.

  \return The filled string, with size() set to the number of bytes written.

  \pre \a pattern is valid for sizeof...(Args); an out-of-range or unparsable placeholder writes nothing and a stray
       brace writes as a literal.
  \pre The output fits capacity() of the returned string; a longer output clamps byte by byte, and a cut may split a
       UTF-8 sequence.

  \sa toy::format()
*/
template <size_t BufferSize, typename... Args>
[[nodiscard]] constexpr FixedString<BufferSize> vformat(StringView pattern, const Args &... args) noexcept
  requires(BufferSize >= 1);

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_HPP_
