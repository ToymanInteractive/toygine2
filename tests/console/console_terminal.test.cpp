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
  \file   console_terminal.test.cpp
  \brief  Unit tests for \ref toy::console::Terminal.
*/

#include "console.hpp"
#include "toy_test.hpp"

namespace toy::console {

namespace {

// Width 16 leaves room for a tab stop: at width 8 every tab clamps to the last column.
struct FakeBackend final {
  using Cell = uint16_t;

  static constexpr size_t capacityColumns = 16;
  static constexpr size_t capacityRows    = 6;
  static inline size_t    columns         = 16;
  static inline size_t    rows            = 4;
  static inline size_t    pendingColumns  = 16;
  static inline size_t    pendingRows     = 4;

  // The backend fixes the active geometry during initialize(), as MdBackend does from the console region.
  static void initialize() noexcept {
    ++initializeCount;
    columns = pendingColumns;
    rows    = pendingRows;
  }

  static Cell blankCell() noexcept {
    return 0;
  }

  // The identity: the uploaded cell reads back as the character code.
  static Cell encodeCharacter(char character) noexcept {
    return static_cast<Cell>(static_cast<unsigned char>(character));
  }

  // Error cells carry bit 15 so the cases can tell the two encodings apart.
  static Cell encodeErrorCharacter(char character) noexcept {
    return static_cast<Cell>(static_cast<unsigned char>(character) | 0x8000);
  }

  static void upload(span<const Cell> cells) noexcept {
    ++uploadCount;
    lastUploadSize = cells.size();
    std::ranges::copy(cells, lastUpload.begin());
  }

  static inline size_t                                      initializeCount = 0;
  static inline size_t                                      uploadCount     = 0;
  static inline size_t                                      lastUploadSize  = 0;
  static inline array<Cell, capacityRows * capacityColumns> lastUpload{};

  static void reset() noexcept {
    initializeCount = 0;
    uploadCount     = 0;
    lastUploadSize  = 0;
    columns         = 16;
    rows            = 4;
    pendingColumns  = 16;
    pendingRows     = 4;
    lastUpload.fill(0);
  }
};

using TestTerminal = toy::console::Terminal<FakeBackend>;

static_assert(toy::console::ConsoleBackend<FakeBackend>, "FakeBackend must satisfy the backend contract");

struct alignas(8) OverAlignedCell {
  uint16_t value;
};

struct AlignedBackend final {
  using Cell = OverAlignedCell;

  [[maybe_unused]] static constexpr size_t capacityColumns = 4;
  [[maybe_unused]] static constexpr size_t capacityRows    = 2;
  static constexpr size_t                  columns         = 4;
  static constexpr size_t                  rows            = 2;

  static void initialize() noexcept {}

  static Cell blankCell() noexcept {
    return Cell{0};
  }

  static Cell encodeCharacter(char character) noexcept {
    return Cell{static_cast<uint16_t>(character)};
  }

  static Cell encodeErrorCharacter(char character) noexcept {
    return Cell{static_cast<uint16_t>(character)};
  }

  static void upload(span<const Cell> cells) noexcept {
    static_cast<void>(cells);
  }
};

static_assert(alignof(toy::console::Terminal<AlignedBackend>) >= alignof(OverAlignedCell),
              "the buffer must keep the alignment the cell requires");

struct NonTrivialCell {
  uint16_t value;

  NonTrivialCell & operator=(const NonTrivialCell & other) noexcept {
    value = other.value;
    return *this;
  }
};

struct NonTrivialBackend final {
  using Cell = NonTrivialCell;

  [[maybe_unused]] static constexpr size_t capacityColumns = 4;
  [[maybe_unused]] static constexpr size_t capacityRows    = 2;
  [[maybe_unused]] static constexpr size_t columns         = 4;
  [[maybe_unused]] static constexpr size_t rows            = 2;

  static void initialize() noexcept {}

  static Cell blankCell() noexcept {
    return Cell{0};
  }

  static Cell encodeCharacter(char character) noexcept {
    return Cell{static_cast<uint16_t>(character)};
  }

  static Cell encodeErrorCharacter(char character) noexcept {
    return Cell{static_cast<uint16_t>(character)};
  }

  static void upload(span<const Cell> cells) noexcept {
    static_cast<void>(cells);
  }
};

static_assert(!toy::console::ConsoleBackend<NonTrivialBackend>,
              "the scroll moves cells bytewise, so non-trivially-copyable cells must not satisfy the contract");

} // namespace

// Printable bytes reach the buffer through encodeCharacter, and one write uploads once.
TEST_CASE("console/terminal/writes_printable_characters") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("AB");

  CHECK(FakeBackend::lastUpload[0] == 'A');
  CHECK(FakeBackend::lastUpload[1] == 'B');
  CHECK(FakeBackend::lastUpload[2] == 0);
  CHECK(FakeBackend::uploadCount == 1);
}

// The wrap waits for the next character: a full row leaves the cursor past the last column.
TEST_CASE("console/terminal/wraps_at_last_column_then_continues") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("ABCDEFGHIJKLMNOP");
  terminal.write("Q");

  CHECK(FakeBackend::lastUpload[0] == 'A');
  CHECK(FakeBackend::lastUpload[16] == 'Q');
}

// A newline after a full row advances once: no blank row and no scroll.
TEST_CASE("console/terminal/trailing_newline_after_full_line_does_not_blank_a_row") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("ABCDEFGHIJKLMNOP\n");
  terminal.write("R");

  CHECK(FakeBackend::lastUpload[0] == 'A');
  CHECK(FakeBackend::lastUpload[15] == 'P');
  CHECK(FakeBackend::lastUpload[16] == 'R');
  CHECK(FakeBackend::lastUpload[48] == 0);
}

// Five rows into a four-row buffer scroll in one write: the first row drops, the last stays whole.
TEST_CASE("console/terminal/scrolls_when_last_row_fills") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("AAAAAAAAAAAAAAAA\nBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCC\nDDDDDDDDDDDDDDDD\nEEEEEEEEEEEEEEEE");

  CHECK(FakeBackend::lastUpload[0] == 'B');
  CHECK(FakeBackend::lastUpload[16] == 'C');
  CHECK(FakeBackend::lastUpload[32] == 'D');
  CHECK(FakeBackend::lastUpload[48] == 'E');
  CHECK(FakeBackend::lastUpload[63] == 'E');
}

// Backspace moves the cursor left, so the next character overwrites the previous one.
TEST_CASE("console/terminal/backspace_moves_cursor_left") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("AB\bC");

  CHECK(FakeBackend::lastUpload[0] == 'A');
  CHECK(FakeBackend::lastUpload[1] == 'C');
}

// Backspace at column zero stays at column zero.
TEST_CASE("console/terminal/backspace_at_column_zero_is_ignored") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("\bA");

  CHECK(FakeBackend::lastUpload[0] == 'A');
}

// Carriage return restarts the line without advancing the row.
TEST_CASE("console/terminal/carriage_return_restarts_line") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("AB\rCD");

  CHECK(FakeBackend::lastUpload[0] == 'C');
  CHECK(FakeBackend::lastUpload[1] == 'D');
}

// A tab jumps to the next multiple of the tab size (2 by default).
TEST_CASE("console/terminal/tab_advances_to_next_multiple_of_tab_size") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("AB\tC");

  CHECK(FakeBackend::lastUpload[4] == 'C');
}

// At the last column the tab clamps instead of wrapping.
TEST_CASE("console/terminal/tab_clamps_at_last_column") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("ABCDEFGHIJKLMNO\tX");

  CHECK(FakeBackend::lastUpload[14] == 'O');
  CHECK(FakeBackend::lastUpload[15] == 'X');
}

// setTabSize changes the stop interval for subsequent writes.
TEST_CASE("console/terminal/tab_size_is_configurable") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();
  terminal.setTabSize(8);

  terminal.write("AB\tC");

  CHECK(FakeBackend::lastUpload[8] == 'C');

  FakeBackend::reset();
  TestTerminal threeTerminal;
  threeTerminal.initialize();
  threeTerminal.setTabSize(3);

  threeTerminal.write("A\tB");

  CHECK(FakeBackend::lastUpload[3] == 'B');
}

// Vertical tab advances the row and keeps the column.
TEST_CASE("console/terminal/vertical_tab_advances_row_only") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("AB\vC");

  CHECK(FakeBackend::lastUpload[18] == 'C');
}

// Bytes at or above 0x80 render as the replacement glyph.
TEST_CASE("console/terminal/high_bytes_render_as_question_marks") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("\x{C3}\x{A9}");

  CHECK(FakeBackend::lastUpload[0] == '?');
  CHECK(FakeBackend::lastUpload[1] == '?');
}

// Unhandled control bytes drop out; the explicit count keeps the embedded NUL and DEL in the text.
TEST_CASE("console/terminal/unhandled_control_bytes_are_ignored") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write(toy::FixedString<8>{"A\x{01}\x{00}\x{7F}B", 5});

  CHECK(FakeBackend::lastUpload[0] == 'A');
  CHECK(FakeBackend::lastUpload[1] == 'B');
}

// A second initialize clears the buffer, resets the cursor, and re-runs the backend initialize.
TEST_CASE("console/terminal/reinitialization_clears_buffer_and_resets_cursor") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("AB");
  terminal.initialize();
  terminal.write("C");

  CHECK(FakeBackend::lastUpload[0] == 'C');
  CHECK(FakeBackend::lastUpload[1] == 0);
  CHECK(FakeBackend::lastUpload[63] == 0);
  CHECK(FakeBackend::initializeCount == 2);
}

// Every write uploads the whole buffer, an empty one included.
TEST_CASE("console/terminal/upload_follows_every_write") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("A");
  terminal.write("");
  terminal.write("B");

  CHECK(FakeBackend::uploadCount == 3);
  CHECK(FakeBackend::lastUpload[0] == 'A');
  CHECK(FakeBackend::lastUpload[1] == 'B');
}

// Scroll and upload follow the active rows; the capacity tail stays blank and undrawn.
TEST_CASE("console/terminal/scrolls_at_active_rows_not_capacity") {
  FakeBackend::reset();
  FakeBackend::pendingRows = 3;
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("AAAAAAAAAAAAAAAA\nBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCC\nDDDDDDDDDDDDDDDD");

  CHECK(terminal.rows() == 3);
  CHECK(FakeBackend::lastUploadSize == 48);
  CHECK(FakeBackend::lastUpload[0] == 'B');
  CHECK(FakeBackend::lastUpload[16] == 'C');
  CHECK(FakeBackend::lastUpload[32] == 'D');
  CHECK(FakeBackend::lastUpload[48] == 0);
  CHECK(FakeBackend::lastUpload[95] == 0);
}

// Wrap and upload follow the active columns; the capacity tail stays blank and undrawn.
TEST_CASE("console/terminal/wraps_at_active_columns_not_capacity") {
  FakeBackend::reset();
  FakeBackend::pendingColumns = 8;
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("ABCDEFGHI");

  CHECK(terminal.columns() == 8);
  CHECK(FakeBackend::lastUploadSize == 32);
  CHECK(FakeBackend::lastUpload[0] == 'A');
  CHECK(FakeBackend::lastUpload[7] == 'H');
  CHECK(FakeBackend::lastUpload[8] == 'I');
  CHECK(FakeBackend::lastUpload[32] == 0);
}

// The terminal reads the geometry the backend sets during initialize(), not the values before it.
TEST_CASE("console/terminal/geometry_comes_from_backend_initialize") {
  FakeBackend::reset();
  FakeBackend::pendingRows    = 5;
  FakeBackend::pendingColumns = 10;
  TestTerminal terminal;
  terminal.initialize();

  CHECK(terminal.rows() == 5);
  CHECK(terminal.columns() == 10);

  terminal.write("A");

  CHECK(FakeBackend::lastUploadSize == 50);
}

// Error text uses the backend's error encoding; normal text keeps the regular one.
TEST_CASE("console/terminal/write_error_uses_error_encoding") {
  FakeBackend::reset();
  TestTerminal terminal;
  terminal.initialize();

  terminal.write("A");
  terminal.writeError("B\x{C3}");

  CHECK(FakeBackend::lastUpload[0] == 'A');
  CHECK(FakeBackend::lastUpload[1] == ('B' | 0x8000));
  CHECK(FakeBackend::lastUpload[2] == ('?' | 0x8000));
  CHECK(FakeBackend::uploadCount == 2);
}

} // namespace toy::console
