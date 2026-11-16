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
  \file   terminal.hpp
  \brief  Platform-independent terminal core of the console module.

  Defines \ref toy::console::Terminal, the cell buffer and cursor logic every platform shares, and
  \ref toy::console::ConsoleBackend, the contract a screen backend satisfies. Used by the platform backend
  translation units and by the desktop unit tests.

  \note Included by console.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CONSOLE_TERMINAL_HPP_
#define INCLUDE_CONSOLE_TERMINAL_HPP_

#include <core.hpp>

namespace toy::console {

/*!
  \concept ConsoleBackend
  \brief Concept satisfied when \a Backend describes a screen the terminal can draw on.

  Constrains the \ref toy::console::Terminal template parameter. The backend owns the geometry, the cell encoding,
  and the upload, so the terminal core stays platform-independent.

  \section console_backend_requirements Requirements

  \a Backend satisfies ConsoleBackend if and only if:
  * \a Backend::Cell names the screen cell type.
  * \a Backend::columns converts to \c const \c size_t and gives the active column count, fixed before the first
    write().
  * \a Backend::rows converts to \c const \c size_t and gives the active row count, fixed before the first write().
  * \a Backend::columns and \a Backend::rows are positive and fit \a Backend::capacityColumns and
    \a Backend::capacityRows; initialize() checks this with assert_message in debug builds.
  * \a Backend::capacityColumns is a constant expression convertible to \c const \c size_t, at least
    \a Backend::columns; it bounds the buffer width.
  * \a Backend::capacityRows is a constant expression convertible to \c const \c size_t, at least \a Backend::rows;
    it bounds the buffer height.
  * \a Backend::initialize() returns \c void and prepares the screen.
  * \a Backend::blankCell() returns the empty \a Backend::Cell.
  * \a Backend::encodeCharacter(char) returns the \a Backend::Cell for a byte in \c 0x20..\c 0x7E.
  * \a Backend::encodeErrorCharacter(char) returns the error \a Backend::Cell for a byte in \c 0x20..\c 0x7E;
    the backend decides how errors look.
  * \a Backend::upload(span<const Backend::Cell>) returns \c void and draws a row-major buffer of rows * columns cells.

  All four calls are static.

  \sa \ref toy::console::Terminal
*/
template <typename Backend>
concept ConsoleBackend = requires {
  typename Backend::Cell;
  { Backend::columns } -> std::convertible_to<const size_t>;
  { Backend::rows } -> std::convertible_to<const size_t>;
  { Backend::capacityColumns } -> std::convertible_to<const size_t>;
  { Backend::capacityRows } -> std::convertible_to<const size_t>;
} && requires(span<const typename Backend::Cell> cells, char character) {
  { Backend::initialize() } -> std::same_as<void>;
  { Backend::blankCell() } -> std::same_as<typename Backend::Cell>;
  { Backend::encodeCharacter(character) } -> std::same_as<typename Backend::Cell>;
  { Backend::encodeErrorCharacter(character) } -> std::same_as<typename Backend::Cell>;
  { Backend::upload(cells) } -> std::same_as<void>;
};

/*!
  \class Terminal
  \brief Platform-independent TTY terminal over a screen backend.

  Keeps a fixed-capacity cell buffer, a cursor, and the wrap, scroll, and control-byte logic, so every platform
  shares one implementation. The backend supplies the geometry, the cell encoding, and the upload; the core knows
  nothing about the hardware. Two terminals can coexist in one process.

  \tparam Backend Screen backend; satisfies \ref toy::console::ConsoleBackend.

  \section terminal_features Key Features

  * **Fixed capacity**: capacityRows * capacityColumns cells live inside the object; no operation allocates.
  * **Active geometry**: wrap, scroll, and upload follow the backend's active rows and columns, which may be below the
    capacity.
  * **Deferred wrap**: the cursor wraps on the next character, so a trailing newline does not blank a row.
  * **Scroll**: a full last row shifts the buffer up one row and blanks the bottom row.
  * **Control bytes**: \c '\\b', \c '\\n', \c '\\r', \c '\\t' with configurable stops (default 2 columns), and \c '\\v'.
  * **Immediate upload**: every write() draws the whole buffer through the backend.

  \section terminal_usage Usage Example

  \code
  #include "console.hpp"

  struct ScreenBackend final {
    using Cell = uint16_t;

    static constexpr size_t capacityColumns = 40;
    static constexpr size_t capacityRows    = 30;
    static constexpr size_t columns         = 40;
    static constexpr size_t rows            = 28;

    static void initialize() noexcept {}
    static Cell blankCell() noexcept { return 0; }
    static Cell encodeCharacter(char character) noexcept { return static_cast<Cell>(character); }
    static Cell encodeErrorCharacter(char character) noexcept { return static_cast<Cell>(character); }
    static void upload(span<const Cell> cells) noexcept { static_cast<void>(cells); }
  };

  toy::console::Terminal<ScreenBackend> terminal;
  terminal.initialize();
  terminal.write("Hello\n");
  \endcode

  \section terminal_performance Performance Characteristics

  * **initialize()**: O(capacityRows * capacityColumns) to blank the buffer.
  * **write()**: O(n + rows * columns) in the bytes of \a text and the active buffer size.
  * **Memory usage**: capacityRows * capacityColumns cells, two cursor indices, and a flag.

  \section terminal_safety Safety Guarantees

  * **Type safety**: uses C++23 concepts; a backend that misses the contract fails to compile.
  * **Exception safety**: No operation throws; exceptions are off in the build.

  \section terminal_compatibility Compatibility

  * Allocates nothing and calls no platform API, so the core suits embedded and retro targets; the backend sets the
    platform reach.

  \sa \ref toy::console::ConsoleBackend
*/
template <ConsoleBackend Backend>
class Terminal final {
public:
  /// Screen cell type the backend draws.
  using Cell                              = typename Backend::Cell;
  /// Buffer column capacity; at least the active column count.
  static constexpr size_t capacityColumns = Backend::capacityColumns;
  /// Buffer row capacity; at least the active row count.
  static constexpr size_t capacityRows    = Backend::capacityRows;

  /*!
    \brief Returns the active column count the terminal draws.

    \return Active columns, never above capacityColumns.
  */
  static size_t columns() noexcept;

  /*!
    \brief Returns the active row count the terminal draws.

    \return Active rows, never above capacityRows.
  */
  static size_t rows() noexcept;

  /*!
    \brief Sets the tab stop interval.

    \param tabSize Columns between tab stops.

    \post Tab stops sit at multiples of \a tabSize, clamped to the last column.
  */
  void setTabSize(size_t tabSize) noexcept;

  /*!
    \brief Clears the buffer, resets the cursor, and prepares the backend screen.

    \post The buffer holds blank cells, the cursor is at the origin, and the backend is initialized.
  */
  void initialize() noexcept;

  /*!
    \brief Prints \a text into the buffer and uploads the buffer through the backend.

    \param text Bytes to print; need not be null-terminated.

    \post The buffer reflects \a text and the backend has drawn it.

    \sa toy::console::write()
  */
  void write(StringView text) noexcept;

  /*!
    \brief Prints \a text into the buffer as errors and uploads the buffer through the backend.

    \param text Bytes to print; need not be null-terminated.

    \post The buffer reflects \a text in the backend's error encoding and the backend has drawn it.

    \sa toy::console::writeError()
  */
  void writeError(StringView text) noexcept;

private:
  /*!
    \brief Shared byte loop of write() and writeError(); \a isError picks the backend encoder.

    \param text    Bytes to print.
    \param isError \c true for the error encoding.
  */
  void writeText(StringView text, bool isError) noexcept;
  /*!
    \brief Writes one cell at the cursor, wrapping first when the cursor sits past the last column.

    \param cell Encoded cell to store.

    \post The buffer holds \a cell at the cursor position and the cursor has advanced one column.
  */
  void writeCharacter(Cell cell) noexcept;

  /*!
    \brief Advances the cursor one row, scrolling the buffer when the last row fills.

    \post The cursor sits on the next row; a scroll shifts the buffer up one row and blanks the bottom row.
  */
  void lineAdvance() noexcept;

  /// Row-major cells at the active stride, capacityRows * capacityColumns total, 4-byte aligned for longword uploads.
  alignas(4) Cell _buffer[capacityRows * capacityColumns];
  /// Cursor column; equals the active columns when the wrap is pending.
  size_t _cursorX     = 0;
  /// Cursor row.
  size_t _cursorY     = 0;
  /// Whether initialize() has run.
  bool   _initialized = false;
  /// Columns between tab stops; 2 by default.
  size_t _tabSize     = 2;
};

} // namespace toy::console

#endif // INCLUDE_CONSOLE_TERMINAL_HPP_
