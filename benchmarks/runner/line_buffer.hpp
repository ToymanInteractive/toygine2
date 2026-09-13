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
  \file   line_buffer.hpp
  \brief  Line output of the console benchmark report.

  Defines \ref toy::benchmark::Output, the destination of finished report lines, and
  \ref toy::benchmark::detail::LineBuffer, the fixed buffer a CSV row or a comment is built in first.

  \note Reached through toy_benchmark.hpp on consoles; included directly only by the runner's own headers and by its
        unit test.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_LINE_BUFFER_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_LINE_BUFFER_HPP_

#include <array>
#include <cstddef>
#include <cstdint>

#include "runner_utils.hpp"

namespace toy::benchmark {

/*!
  \brief Function receiving the report one finished line at a time.

  \param text        Line bytes, ending in a newline and not null-terminated.
  \param length      Number of bytes.
  \param writerData  Pointer stored beside the function in \ref toy::benchmark::Output.
*/
using write_function_type = void (*)(const char * text, std::size_t length, const void * writerData) noexcept;

/// Destination of report lines: the receiving function and the data handed back to it.
struct Output final {
  write_function_type write;      ///< Receives every finished line.
  const void *        writerData; ///< Passed to \a write unchanged; owned by the caller.
};

namespace detail {

/*!
  \class LineBuffer
  \brief Fixed buffer one report line is built in.

  Collects text and unsigned integers, then hands the line with its newline to a \ref toy::benchmark::Output. The
  capacity matches the longest message the mGBA log carries, so a row of seven epochs reaches the log whole.

  \section features Key Features

  * **Allocation-free**: one 256-byte buffer inside the object
  * **Freestanding**: formats through the runner's own helpers, without \c <cstdio>
  * **Line-shaped output**: every flush ends in exactly one newline
  * **Truncating**: text past the capacity is dropped, never written out of bounds

  \section usage Usage Example

  \code
  toy::benchmark::detail::LineBuffer line{output};

  line.addText("# done");
  line.flush();
  \endcode

  \section performance Performance Characteristics

  * **Appends**: O(n) in the characters written
  * **Memory usage**: 256-byte buffer, the output and a length

  \section safety Safety Guarantees

  * **Bounds safety**: every append stops at the capacity
  * **Memory safety**: no ownership of the output's data
  * **Exception safety**: No operation throws; exceptions are off in the build

  \warning A line cut at the capacity loses fields, and the host parser then rejects the row; see
           \ref toy::benchmark::Bench for the epoch count this capacity allows.

  \sa \ref toy::benchmark::Output
*/
class LineBuffer final {
public:
  /// Capacity of one line in bytes, newline included; the mGBA log carries no more in one message.
  static constexpr std::size_t c_capacity = 256;

  /*!
    \brief Builds an empty line that flushes into \a output.

    \param output  Destination of finished lines; its data must outlive the buffer.

    \post The line is empty.
  */
  explicit LineBuffer(const Output & output) noexcept;

  ~LineBuffer() noexcept                     = default;
  LineBuffer(const LineBuffer &)             = delete;
  LineBuffer & operator=(const LineBuffer &) = delete;
  LineBuffer(LineBuffer &&)                  = delete;
  LineBuffer & operator=(LineBuffer &&)      = delete;

  /*!
    \brief Appends null-terminated text.

    \param text  Text to append.

    \post The line grows by \a text, or stops at the capacity.

    \sa addDecimal()
  */
  void addText(const char * text) noexcept;

  /*!
    \brief Appends an unsigned integer as decimal digits.

    \param value  Value to append.

    \post The line grows by the digits, or stops at the capacity.
  */
  void addDecimal(std::uint64_t value) noexcept;

  /*!
    \brief Hands the line with a newline to the output and starts a new one.

    \post The output received one call, and the line is empty.

    \note A full line gives its last byte to the newline.
  */
  void flush() noexcept;

private:
  Output                       _output;
  std::array<char, c_capacity> _buffer{};
  std::size_t                  _length{0};
};

} // namespace detail

} // namespace toy::benchmark

#include "line_buffer.inl"

#endif // INCLUDE_BENCHMARKS_RUNNER_LINE_BUFFER_HPP_
