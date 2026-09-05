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
  \file   utils.hpp
  \brief  Freestanding helpers the runner needs where a hosted header is unavailable.

  Declares toy::test::detail::absoluteValue(), toy::test::detail::compareNames(), toy::test::detail::appendText(),
  toy::test::detail::appendEscaped() and toy::test::detail::appendInteger(): what \c <cmath>, \c <cstring> and
  \c <charconv> would provide, written so the runner builds on targets whose standard library ships none of them.

  \note Reached through toy_test.hpp; included directly only by the runner's own headers and by the unit test
        for this type.
*/

#ifndef INCLUDE_TESTS_RUNNER_UTILS_HPP_
#define INCLUDE_TESTS_RUNNER_UTILS_HPP_

#include <array>
#include <concepts>
#include <cstddef>

#include "assertion.hpp"

namespace toy::test::detail {

/*!
  \brief Returns the magnitude of a floating-point value.

  \tparam T      Floating-point type of the value.
  \param  value  Value to take the magnitude of.

  \return \a value without its sign.

  \note Exists because \c <cmath> is a hosted header and the runner must build freestanding.
*/
template <std::floating_point T>
[[nodiscard]] constexpr T absoluteValue(T value) noexcept;

/*!
  \brief Orders two null-terminated names by their first differing byte.

  \param lhs  Left name.
  \param rhs  Right name.

  \return \c -1 when \a lhs orders first, \c 1 when \a rhs orders first, \c 0 when the names are equal.

  \note Bytes compare as unsigned, so the order is total on targets where \c char is signed.
*/
[[nodiscard]] constexpr int compareNames(const char * lhs, const char * rhs) noexcept;

/*!
  \brief Copies text into a buffer, stopping at its capacity.

  \param buffer    Destination buffer.
  \param capacity  Size of \a buffer in bytes.
  \param offset    Position to write from.
  \param text      Null-terminated source text.

  \return Offset past the last byte written; equals \a capacity when the text was truncated.

  \pre \a offset does not exceed \a capacity.

  \note No terminating zero is written; the caller tracks the length.
*/
[[nodiscard]] constexpr std::size_t appendText(char * buffer, std::size_t capacity, std::size_t offset,
                                               const char * text) noexcept;

/*!
  \brief Copies text into a buffer with the backslash and the hash escaped, stopping at its capacity.

  \param buffer    Destination buffer.
  \param capacity  Size of \a buffer in bytes.
  \param offset    Position to write from.
  \param text      Null-terminated source text.

  \return Offset past the last byte written; equals \a capacity when the text was truncated.

  \pre \a offset does not exceed \a capacity.

  \note A backslash arrives as \c "\\\\" and a hash as \c "\\#", the two escapes TAP defines.

  \note An escape pair that does not fit is left out whole, so a truncated line never ends in a lone backslash.

  \sa appendText()
*/
[[nodiscard]] constexpr std::size_t appendEscaped(char * buffer, std::size_t capacity, std::size_t offset,
                                                  const char * text) noexcept;

/*!
  \brief Writes a signed decimal representation into a buffer, stopping at its capacity.

  \param buffer    Destination buffer.
  \param capacity  Size of \a buffer in bytes.
  \param offset    Position to write from.
  \param value     Value to format.

  \return Offset past the last byte written; equals \a capacity when the digits were truncated.

  \pre \a offset does not exceed \a capacity.

  \note The magnitude is taken in unsigned arithmetic, so the most negative value formats correctly.
*/
[[nodiscard]] constexpr std::size_t appendInteger(char * buffer, std::size_t capacity, std::size_t offset,
                                                  long long value) noexcept;

} // namespace toy::test::detail

#include "utils.inl"

#endif // INCLUDE_TESTS_RUNNER_UTILS_HPP_
