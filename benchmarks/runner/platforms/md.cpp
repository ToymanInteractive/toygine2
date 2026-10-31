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
  \file   md.cpp
  \brief  Platform layer of the benchmark runner for Sega MD: scanline timer, output channels and the entry point.
*/

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

#include <clownmdsdk.h>

#include "benchmark_build_id.hpp"
#include "console_report.hpp"
#include "md_clock.hpp"
#include "sram_writer.hpp"

namespace {

namespace vdp = ClownMDSDK::MainCPU::VDP;

constexpr std::uint32_t c_minEpochLines = 2048;
constexpr std::uint32_t c_maxIterations = 1UL << 31;

// The vertical interrupt is level 6, so mask 5 lets it through; the horizontal one stays off at the VDP.
constexpr unsigned int c_interruptMask = 5;

// Frames counted by the vertical interrupt; nowLines() keeps interrupts on and rereads when one lands mid-read.
constinit volatile std::uint32_t frameCount = 0;

struct Channels final {
  toy::benchmark::detail::SramWriter * sram;
};

void setUpVideo() noexcept {
  vdp::Write(
    vdp::Register00{.blank_leftmode_8_pixels = false, .enable_horizontal_interrupt = false, .lock_hv_counter = false}
  );
  vdp::Write(vdp::Register0C{.enable_h40_cell_mode_1       = false,
                             .enable_shadow_highlight_mode = false,
                             .interlace_mode               = 0,
                             .enable_h40_cell_mode_2       = false});
  vdp::Write(vdp::Register07{.palette_line = 0, .palette_index = 0});

  // md_clock.hpp assumes 224-line mode with the display and the vertical interrupt on.
  vdp::Write(vdp::Register01{.enable_display            = true,
                             .enable_vertical_interrupt = true,
                             .enable_dma_transfer       = false,
                             .enable_v30_cell_mode      = false,
                             .enable_mega_drive_mode    = true});
}

std::uint32_t nowLines(const void * clockData) noexcept {
  const std::uint32_t linesPerFrame = *static_cast<const std::uint32_t *>(clockData);

  for (;;) {
    const std::uint32_t framesBefore  = frameCount;
    const bool          verticalBlank = vdp::ReadStatus().vertical_blanking;
    const std::uint8_t  vCounter      = vdp::v_counter;

    // An interrupt between the reads would pair a frame count with a counter from another frame.
    if (frameCount != framesBefore)
      continue;

    const toy::benchmark::detail::VdpSample sample{.frames        = framesBefore,
                                                   .vCounter      = vCounter,
                                                   .verticalBlank = verticalBlank};

    if (!toy::benchmark::detail::needsSettling(sample))
      return toy::benchmark::detail::lineTimestamp(sample, linesPerFrame);

    // The interrupt raised on this line may not have run yet; by the next line it has.
    while (vdp::v_counter == toy::benchmark::detail::verticalInterruptLine) {}
  }
}

void writeLine(std::span<const char> text, const void * writerData) noexcept {
  // The debug port ends a line on a null byte, so the newline goes out as one.
  for (std::size_t index = 0; index < text.size(); ++index)
    ClownMDSDK::MainCPU::Debug::Print(text[index] == '\n' ? '\0' : text[index]);

  static_cast<const Channels *>(writerData)->sram->write(text);
}

} // namespace

// Declared by clownmdsdk.h with C linkage and the interrupt attribute; the definition only counts frames.
void _VerticalInterruptHandler() {
  frameCount = frameCount + 1;
}

void _EntryPoint() {
  const bool          isPal         = ClownMDSDK::MainCPU::Unsafe::IsPAL();
  const std::uint32_t linesPerFrame = isPal ? toy::benchmark::detail::palLinesPerFrame
                                            : toy::benchmark::detail::ntscLinesPerFrame;

  vdp::CRAM::Set(0, 0, vdp::CRAM::Colour{7, 7, 0});
  setUpVideo();
  static_cast<void>(ClownMDSDK::MainCPU::M68k::SetInterruptMask(c_interruptMask));

  // 8-bit cartridge RAM on odd addresses from 0x200001, as the patched ROM header declares it.
  const std::span<volatile std::uint8_t> sram{reinterpret_cast<volatile std::uint8_t *>(0x200001), 0xFFFF};
  toy::benchmark::detail::SramWriter     sramWriter{sram, 2};
  const Channels                         channels{.sram = &sramWriter};

  const std::array<const char *, 4> heading{TOYGINE_BENCHMARKS_PLATFORM, TOYGINE_BENCHMARKS_BUILD_TYPE,
                                            isPal ? "PAL" : "NTSC", TOYGINE_BENCHMARKS_BUILD_ID};

  const int
    code = toy::benchmark::writeReport({.now                = &nowLines,
                                        .clockData          = &linesPerFrame,
                                        .picosecondsPerTick = isPal ? toy::benchmark::detail::palPicosecondsPerLine
                                                                    : toy::benchmark::detail::ntscPicosecondsPerLine,
                                        .minEpochTicks      = c_minEpochLines,
                                        .maxIterations      = c_maxIterations},
                                       {.write = &writeLine, .writerData = &channels}, heading,
                                       toy::benchmark::detail::caseListHead);

  vdp::CRAM::Set(0, 0, code == 0 ? vdp::CRAM::Colour{0, 7, 0} : vdp::CRAM::Colour{7, 0, 0});

  // Nothing to return to; the backdrop and the last report line carry the verdict.
  for (;;) {}
}
