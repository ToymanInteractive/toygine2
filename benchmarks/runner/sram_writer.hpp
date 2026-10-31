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
  \file   sram_writer.hpp
  \brief  Report copy kept in cartridge battery-backed RAM.

  Defines \ref toy::benchmark::detail::SramWriter, the output channel that puts the report into cartridge RAM, where a
  flash cart on real hardware saves it to a file.

  \note Included by a console platform file and by the runner's unit test.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_SRAM_WRITER_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_SRAM_WRITER_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

#include "core.hpp"

namespace toy::benchmark::detail {

/*!
  \class SramWriter
  \brief Appends report lines to cartridge RAM as null-terminated text.

  Writes each character to its own byte of the window, a stride apart: \c 1 on GBA, \c 2 on a Mega Drive cartridge
  whose 8-bit RAM answers on odd addresses. A zero follows the last character, so a longer report left by an earlier
  run never reads as a continuation.

  \section features Key Features

  * **Whole lines**: a line that does not fit is replaced by \c "# truncated", and nothing is written after it
  * **Always terminated**: the text ends in a zero after every write
  * **Allocation-free**: writes into the window it is given
  * **Stride-aware**: one class for byte-per-character and every-other-byte RAM

  \section usage Usage Example

  \code
  const std::span<volatile std::uint8_t> sram{reinterpret_cast<volatile std::uint8_t *>(0x0E000000), 0x8000};
  toy::benchmark::detail::SramWriter    writer{sram, 1};

  writer.write(std::span<const char>{"# done\n", 7});
  \endcode

  \section performance Performance Characteristics

  * **write()**: O(n) in the line length
  * **Memory usage**: keeps no copy of the text, only the window and the write position

  \section safety Safety Guarantees

  * **Contracts**: debug builds assert a non-zero stride and room for the truncation comment
  * **Bounds safety**: no byte is written past the window
  * **Exception safety**: No operation throws; exceptions are off in the build

  \warning The window must be the cartridge RAM the ROM header declares; the writer cannot check this and writes
           wherever the span points.
*/
class SramWriter final {
public:
  /*!
    \brief Starts an empty report in \a memory.

    \param memory  Window over the cartridge RAM; must outlive the writer.
    \param stride  Bytes from one character to the next.

    \pre \a stride is not \c 0.
    \pre The window holds more than 12 characters at \a stride: room for the \c "# truncated" line with its newline
         and the zero after it.

    \post The first character is a zero.
  */
  SramWriter(std::span<volatile std::uint8_t> memory, std::size_t stride) noexcept;

  ~SramWriter() noexcept                     = default;
  SramWriter(const SramWriter &)             = delete;
  SramWriter & operator=(const SramWriter &) = delete;
  SramWriter(SramWriter &&)                  = delete;
  SramWriter & operator=(SramWriter &&)      = delete;

  /*!
    \brief Appends one report line.

    \param text  Line bytes, ending in a newline.

    \post The window ends in the line and a zero when the line leaves room for the truncation comment. Otherwise it
          ends in \c "# truncated" and a zero, and later calls write nothing.
  */
  void write(std::span<const char> text) noexcept;

private:
  static constexpr auto        _truncatedLine   = std::to_array("# truncated\n");
  static constexpr std::size_t _truncatedLength = _truncatedLine.size() - 1;

  void _put(std::size_t index, char character) noexcept;

  std::span<volatile std::uint8_t> _memory;
  std::size_t                      _stride;
  std::size_t                      _capacity;
  std::size_t                      _length{0};
  bool                             _truncated{false};
};

} // namespace toy::benchmark::detail

#include "sram_writer.inl"

#endif // INCLUDE_BENCHMARKS_RUNNER_SRAM_WRITER_HPP_
