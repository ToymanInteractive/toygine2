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
  \file   console_md.cpp
  \brief  Definitions for toy::console::initialize() and toy::console::write() over the Mega Drive VDP plane B.
*/

#include <font8x8_basic.h>

#include "console.hpp"
#include "platforms/platform_config.hpp"

namespace toy::console {

namespace {

namespace vdp = ClownMDSDK::MainCPU::VDP;

/// The vertical interrupt is level 6; mask 5 lets it through.
constexpr unsigned int c_interruptMask = 5;

/// Plane B name table at VRAM 0xE000; the 64x32 plane spans 0xE000..0xEFFF.
constexpr unsigned int c_planeBAddress = 0xE000;

/// A plane row holds 64 entries of 2 bytes, whatever the visible window shows.
constexpr unsigned int c_planeStrideBytes = 64 * 2;

/// Font tiles live at tile index 0x300, VRAM 0x6000, below the name table.
constexpr unsigned int c_fontTileBase = 0x300;
/// VRAM byte address of the first font tile.
constexpr unsigned int c_fontAddress  = c_fontTileBase * vdp::VRAM::TILE_SIZE_IN_BYTES_NORMAL;

/// Printable glyphs the font carries, U+0020..U+007E.
constexpr size_t c_glyphCount = 0x7E - 0x20 + 1;

/*!
  \brief Screen backend over the VDP plane B: geometry, name-table cells, and the CPU upload.
*/
struct MdBackend final {
  /// Name-table word the plane reads.
  using Cell = uint16_t;

  /// Buffer columns: the H40 maximum.
  static constexpr size_t capacityColumns = platform::detail::consoleColumns;
  /// Buffer rows: the maximum region height, so one buffer serves both regions.
  static constexpr size_t capacityRows    = max(platform::detail::consoleNtscRows, platform::detail::consolePalRows);
  /// Active columns; the backend selects H40.
  static size_t           columns;
  /// Active rows by console region; fixed by initialize().
  static size_t           rows;

  /*!
    \brief Returns the cell the buffer clears to.

    \return Name-table word for the space glyph.
  */
  static Cell blankCell() noexcept {
    return encodeCharacter(' ');
  }

  /*!
    \brief Encodes a printable byte as a name-table word.

    \param character Byte in \c 0x20..\c 0x7E.

    \return Name-table word pointing at the glyph's tile.
  */
  static Cell encodeCharacter(char character) noexcept {
    const vdp::VRAM::TileMetadata
      metadata{.priority     = false,
               .palette_line = 0,
               .y_flip       = false,
               .x_flip       = false,
               .tile_index   = c_fontTileBase
                               + (static_cast<unsigned int>(static_cast<unsigned char>(character)) - 0x20)};

    return bit_cast<Cell>(metadata);
  }

  /*!
    \brief Prepares the VDP, uploads the font, and fixes the active rows by console region.
  */
  static void initialize() noexcept;

  /*!
    \brief Draws the active rows into the plane B name table.

    \param cells Row-major buffer of rows * columns cells.
  */
  static void upload(span<const Cell> cells) noexcept;
};

// H40 on both regions.
size_t MdBackend::columns = platform::detail::consoleColumns;

// NTSC until initialize() reads the console region.
size_t MdBackend::rows = platform::detail::consoleNtscRows;

/// Static staging for the converted tiles: 3040 bytes exceed the stack budget this target keeps.
alignas(4) array<byte, c_glyphCount * vdp::VRAM::TILE_SIZE_IN_BYTES_NORMAL> c_fontTiles;

/*!
  \brief Converts the 1bpp font to 4bpp tiles and uploads them to VRAM.
*/
void uploadFontTiles() noexcept {
  // Pixel n of a row reads (byte >> n) & 1, n = 0 leftmost; a tile byte keeps the left pixel in the high nibble.
  for (size_t glyph = 0; glyph < c_glyphCount; ++glyph) {
    for (size_t row = 0; row < 8; ++row) {
      const auto pixels = static_cast<uint8_t>(font8x8_basic[glyph][row]);

      for (size_t pair = 0; pair < 4; ++pair) {
        const auto left                                                            = (pixels >> (pair * 2)) & 1;
        const auto right                                                           = (pixels >> (pair * 2 + 1)) & 1;
        c_fontTiles[glyph * vdp::VRAM::TILE_SIZE_IN_BYTES_NORMAL + row * 4 + pair] = static_cast<byte>(left << 4
                                                                                                       | right);
      }
    }
  }

  vdp::CopyWordsWithoutDMA(vdp::RAM::VRAM, c_fontAddress, c_fontTiles.data(),
                           static_cast<unsigned int>(c_glyphCount * vdp::VRAM::TILE_SIZE_IN_BYTES_NORMAL / 2));
}

void MdBackend::initialize() noexcept {
  const bool isPAL = ClownMDSDK::MainCPU::Unsafe::IsPAL();
  rows             = isPAL ? platform::detail::consolePalRows : platform::detail::consoleNtscRows;

  // Every register write lists all bitfields: the boot state of the registers is indeterminate.
  vdp::Write(vdp::Register00{.id                          = 0x80,
                             .bit7                        = false,
                             .bit6                        = false,
                             .blank_leftmode_8_pixels     = false,
                             .enable_horizontal_interrupt = false,
                             .bit3                        = false,
                             .bit2                        = true,
                             .lock_hv_counter             = false,
                             .bit0                        = false});
  vdp::VRAM::SetPlaneALocation(0x0000);
  vdp::VRAM::SetPlaneBLocation(c_planeBAddress);
  vdp::VRAM::SetSpriteTableLocation(0x0000);
  vdp::Write(vdp::Register07{.id = 0x87, .bit7 = false, .bit6 = false, .palette_line = 0, .palette_index = 0});
  vdp::Write(vdp::Register0B{.id                                = 0x8B,
                             .bit7                              = false,
                             .bit6                              = false,
                             .bit5                              = false,
                             .bit4                              = false,
                             .enable_external_interrupt_level_2 = false,
                             .vertical_scroll_mode              = 0,
                             .horizontal_scroll_mode            = 0});
  vdp::Write(vdp::Register0C{.id                           = 0x8C,
                             .enable_h40_cell_mode_1       = true,
                             .bit6                         = false,
                             .bit5                         = false,
                             .bit4                         = false,
                             .enable_shadow_highlight_mode = false,
                             .interlace_mode               = 0,
                             .enable_h40_cell_mode_2       = true});
  vdp::Write(vdp::Register0F{.id = 0x8F, .increment = 2});
  vdp::Write(vdp::Register10{.id              = 0x90,
                             .bit7            = false,
                             .bit6            = false,
                             .vertical_size   = 0,
                             .bit3            = false,
                             .bit2            = false,
                             .horizontal_size = 0});
  vdp::Write(vdp::Register11{.id = 0x91, .window_align_right = false, .bit6 = false, .bit5 = false, .window_width = 0});
  vdp::Write(
    vdp::Register12{.id = 0x92, .window_align_bottom = false, .bit6 = false, .bit5 = false, .window_height = 0}
  );

  // Zeros at VRAM 0x0000 keep the scroll and sprite tables the VDP reads from carrying garbage.
  vdp::VRAM::FillWordsWithoutDMA(0x0000, 0x100, 0);
  vdp::VSRAM::FillWordsWithoutDMA(0, vdp::VSRAM::total_words, 0);

  uploadFontTiles();

  vdp::CRAM::Set(0, 0, vdp::CRAM::Colour(0, 0, 0));
  vdp::CRAM::Set(0, 1, vdp::CRAM::Colour(7, 7, 7));

  vdp::VRAM::FillWordsWithoutDMA(c_planeBAddress, 0x800, blankCell());

  vdp::Write(vdp::Register01{.id                        = 0x81,
                             .bit7                      = false,
                             .enable_display            = true,
                             .enable_vertical_interrupt = true,
                             .enable_dma_transfer       = false,
                             .enable_v30_cell_mode      = isPAL,
                             .enable_mega_drive_mode    = true,
                             .bit1                      = false,
                             .bit0                      = false});
  static_cast<void>(ClownMDSDK::MainCPU::M68k::SetInterruptMask(c_interruptMask));
}

void MdBackend::upload(span<const Cell> cells) noexcept {
  for (size_t row = 0; row < rows; ++row)
    vdp::CopyWordsWithoutDMA(vdp::RAM::VRAM, c_planeBAddress + row * c_planeStrideBytes, cells.data() + row * columns,
                             static_cast<unsigned int>(columns));
}

/// The terminal instance the public API forwards to.
Terminal<MdBackend> terminal;

} // namespace

void initialize() noexcept {
  terminal.initialize();
}

void write(StringView text) noexcept {
  terminal.write(text);
}

void setTabSize(size_t tabSize) noexcept {
  terminal.setTabSize(tabSize);
}

} // namespace toy::console
