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
  \file   gba.cpp
  \brief  Platform layer of the benchmark runner for Nintendo GBA: timer, output channels and the entry point.
*/

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_timers.h>
#include <gba_video.h>

#include "benchmark_build_id.hpp"
#include "console_report.hpp"
#include "sram_writer.hpp"

namespace {

// Level 3 is mGBA's "info"; bit 8 makes it take the buffer and print an entry.
constexpr std::uint16_t c_debugLevelInfo      = 3;
constexpr std::uint16_t c_debugSend           = 0x100;
constexpr std::size_t   c_debugStringCapacity = 256;

// One tick of Timer 2 at prescaler 0: 1 / 16.777216 MHz.
constexpr std::uint64_t c_picosecondsPerTick = 59'605;
constexpr std::uint32_t c_minEpochTicks      = 1UL << 14;
constexpr std::uint32_t c_maxIterations      = 1UL << 31;

constexpr std::uint16_t c_colorRunning = RGB5(31, 31, 0);
constexpr std::uint16_t c_colorDone    = RGB5(0, 31, 0);
constexpr std::uint16_t c_colorFailed  = RGB5(31, 0, 0);

// Flash carts and emulators pick the save type by this tag, which they look for on a 4-byte boundary.
alignas(4) constinit const char c_saveTypeTag[] = "SRAM_V113";

// main() stores the tag here once; the volatile write keeps the linker from discarding it.
constinit const char * volatile saveTypeReference = nullptr;

struct Channels final {
  bool                                 mGBADetected;
  toy::benchmark::detail::SramWriter * sram;
};

// Probes the mGBA debug interface, the only way a ROM can tell the emulator from hardware.
bool mGBAProbe() noexcept {
  auto & mGBAEnableReg = *reinterpret_cast<volatile std::uint16_t *>(0x04FFF780);

  mGBAEnableReg = 0xC0DE;

  return mGBAEnableReg == 0x1DEA;
}

// The log takes the line whole, minus the trailing newline: mGBA ends each entry itself.
void mGBAWriteToDebugLog(const char * text, std::size_t length) noexcept {
  if (length > 0 && text[length - 1] == '\n')
    --length;

  auto * const debugString = reinterpret_cast<volatile char *>(0x04FFF600);
  const auto   count       = std::min(length, c_debugStringCapacity);

  for (std::size_t index = 0; index < count; ++index)
    debugString[index] = text[index];

  if (count < c_debugStringCapacity)
    debugString[count] = '\0';

  *reinterpret_cast<volatile std::uint16_t *>(0x04FFF700) = c_debugLevelInfo | c_debugSend;
}

// SWI 3 ends emulation and mgba-headless reads the exit code from r0, which libgba's Stop() would clobber.
[[noreturn]] void mGBASwiExit(int code) noexcept {
#ifdef __thumb__
  asm volatile("mov r0, %0\n\tswi 3" : : "r"(code) : "r0", "r1", "r2", "r3", "memory");
#else
  asm volatile("mov r0, %0\n\tswi 3 << 16" : : "r"(code) : "r0", "r1", "r2", "r3", "memory");
#endif

  __builtin_unreachable();
}

void startTimer() noexcept {
  REG_TM2CNT_H = 0;
  REG_TM3CNT_H = 0;
  REG_TM2CNT_L = 0;
  REG_TM3CNT_L = 0;

  // Timer 3 counts the overflows of Timer 2, which runs at the full 16.777216 MHz.
  REG_TM3CNT_H = TIMER_COUNT | TIMER_START;
  REG_TM2CNT_H = TIMER_START;
}

std::uint32_t readTimer([[maybe_unused]] const void * clockData) noexcept {
  for (;;) {
    const std::uint16_t high = REG_TM3CNT_L;
    const std::uint16_t low  = REG_TM2CNT_L;

    // An overflow between the two reads moves the high half, so the loop reads the pair again.
    if (REG_TM3CNT_L == high)
      return static_cast<std::uint32_t>(high) << 16 | low;
  }
}

void writeLine(std::span<const char> text, const void * writerData) noexcept {
  const auto & channels = *static_cast<const Channels *>(writerData);

  if (channels.mGBADetected)
    mGBAWriteToDebugLog(text.data(), text.size()); // The emulator debug channel is a C API: pointer and length apart.

  channels.sram->write(text);
}

} // namespace

int main() {
  irqInit();
  irqEnable(IRQ_VBLANK);

  saveTypeReference = c_saveTypeTag;

  // No layer is enabled, so the whole screen shows the backdrop color.
  REG_DISPCNT   = MODE_0;
  BG_PALETTE[0] = c_colorRunning;

  const std::span<volatile std::uint8_t> sram{reinterpret_cast<volatile std::uint8_t *>(0x0E000000), 0x8000};
  toy::benchmark::detail::SramWriter     sramWriter{sram, 1};
  const Channels                         channels{.mGBADetected = mGBAProbe(), .sram = &sramWriter};

  const std::array<const char *, 3> heading{TOYGINE_BENCHMARKS_PLATFORM, TOYGINE_BENCHMARKS_BUILD_TYPE,
                                            TOYGINE_BENCHMARKS_BUILD_ID};

  startTimer();

  // No output channel needs an interrupt, and one taken inside an epoch would count toward the operation.
  REG_IME = 0;

  const int code = toy::benchmark::writeReport({.now                = &readTimer,
                                                .clockData          = nullptr,
                                                .picosecondsPerTick = c_picosecondsPerTick,
                                                .minEpochTicks      = c_minEpochTicks,
                                                .maxIterations      = c_maxIterations},
                                               {.write = &writeLine, .writerData = &channels}, heading,
                                               toy::benchmark::detail::caseListHead);

  REG_IME = 1;

  BG_PALETTE[0] = code == 0 ? c_colorDone : c_colorFailed;

  if (channels.mGBADetected)
    mGBASwiExit(code);

  while (true)
    VBlankIntrWait();
}
