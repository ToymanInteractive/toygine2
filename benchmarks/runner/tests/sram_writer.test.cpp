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
  \file   sram_writer.test.cpp
  \brief  Unit tests for the report copy written to cartridge RAM.
*/

#include <array>
#include <cstdint>
// doctest prints a failing std::string_view through operator<<, which needs the stream type defined, not just declared.
#include <ostream>
#include <string_view>

#include <doctest/doctest.h>

#include "sram_writer.hpp"

namespace {

constexpr std::uint8_t c_untouched = 0xEE;

} // namespace

// Characters land one per byte, and a zero follows the last one.
TEST_CASE("benchmark::detail/sram_writer/appends_lines_and_terminates") {
  std::array<std::uint8_t, 32> memory{};
  memory.fill(c_untouched);

  toy::benchmark::detail::SramWriter writer{memory, 1};

  writer.write("ab\n", 3);
  writer.write("c\n", 2);

  CHECK(std::string_view{reinterpret_cast<const char *>(memory.data())} == "ab\nc\n");
}

// On every-other-byte RAM the bytes between characters stay as they were.
TEST_CASE("benchmark::detail/sram_writer/stride_skips_bytes_between_characters") {
  std::array<std::uint8_t, 32> memory{};
  memory.fill(c_untouched);

  toy::benchmark::detail::SramWriter writer{memory, 2};

  writer.write("ab\n", 3);

  CHECK(memory[0] == 'a');
  CHECK(memory[1] == c_untouched);
  CHECK(memory[2] == 'b');
  CHECK(memory[3] == c_untouched);
  CHECK(memory[4] == '\n');
  CHECK(memory[6] == 0);
}

// A line that would crowd out the truncation comment is replaced by it, and later lines are dropped.
TEST_CASE("benchmark::detail/sram_writer/full_window_ends_with_truncation_comment") {
  std::array<std::uint8_t, 24> memory{};
  memory.fill(c_untouched);

  toy::benchmark::detail::SramWriter writer{memory, 1};

  writer.write("abc\n", 4);
  writer.write("defghij\n", 8);
  writer.write("x\n", 2);

  CHECK(std::string_view{reinterpret_cast<const char *>(memory.data())} == "abc\n# truncated\n");
}
