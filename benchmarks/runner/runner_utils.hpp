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
  \file   runner_utils.hpp
  \brief  Freestanding name ordering and integer formatting for the benchmark runner.

  The console toolchains here ship neither \c <cstring> nor \c <charconv>, so the runner compares names and writes
  numbers on its own.

  \note Reached through toy_benchmark.hpp; included directly only by the runner's own headers and by its unit test.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_RUNNER_UTILS_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_RUNNER_UTILS_HPP_

#include <cstddef>
#include <cstdint>
#include <span>

#include "core.hpp"

namespace toy::benchmark::detail {

/*!
  \brief Orders two null-terminated names by their first differing byte.

  \param lhs  Left name.
  \param rhs  Right name.

  \return \c -1 when \a lhs orders first, \c 1 when \a rhs orders first, \c 0 when the names are equal.

  \note Bytes compare as unsigned, so the order is the same on targets where \c char is signed.
*/
[[nodiscard]] constexpr int compareNames(const char * lhs, const char * rhs) noexcept;

/*!
  \brief Copies null-terminated text into \a buffer from \a offset.

  \param buffer  Destination.
  \param offset  Position to write from.
  \param text    Null-terminated source text.

  \return Offset past the last byte written; equals the size of \a buffer when the text was cut.

  \pre \a offset does not exceed the size of \a buffer.

  \note Writes no terminating zero; the caller tracks the length.

  \sa appendDecimal()
*/
[[nodiscard]] constexpr std::size_t appendText(std::span<char> buffer, std::size_t offset, const char * text) noexcept;

/*!
  \brief Writes an unsigned integer as decimal digits into \a buffer from \a offset.

  \param buffer  Destination.
  \param offset  Position to write from.
  \param value   Value to write.

  \return Offset past the last digit written; equals the size of \a buffer when the digits were cut.

  \pre \a offset does not exceed the size of \a buffer.

  \note Zero writes one digit, and the digits carry no sign: every field of a report line is unsigned.
*/
[[nodiscard]] constexpr std::size_t appendDecimal(std::span<char> buffer, std::size_t offset,
                                                  std::uint64_t value) noexcept;

} // namespace toy::benchmark::detail

#include "runner_utils.inl"

#endif // INCLUDE_BENCHMARKS_RUNNER_RUNNER_UTILS_HPP_
