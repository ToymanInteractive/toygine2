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
  \brief  Platform layer for Nintendo GBA: console output and the entry point.
*/

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <gba_console.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

#include "report.hpp"

namespace {

// Columns of the console consoleDemoInit() sets up: the 240-pixel screen over the 8-pixel tiles it draws with.
constexpr std::size_t c_consoleWidth = 30;

// Level 3 is mGBA's "info"; bit 8 is what makes it take the buffer and print an entry.
constexpr std::uint16_t c_debugLevelInfo = 3;
constexpr std::uint16_t c_debugSend      = 0x100;

constexpr std::size_t c_debugStringCapacity = 256;

// Asks the debug interface whether it is there, which is the only way a ROM can tell the emulator from hardware.
bool mGBAProbe() noexcept {
  auto & mGBAEnableReg = *reinterpret_cast<volatile std::uint16_t *>(0x04FFF780);

  mGBAEnableReg = 0xC0DE;

  return mGBAEnableReg == 0x1DEA;
}

// Unlike the screen, the log takes the line whole, minus the trailing newline: mGBA ends each entry itself.
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

// SWI 3 (Stop) ends emulation and mgba-rom-test -S 3 -R r0 takes the exit code from r0.
// The call is written by hand because libgba's Stop() clobbers r0.
[[noreturn]] void mGBASwiExit(int code) noexcept {
#ifdef __thumb__
  asm volatile("mov r0, %0\n\tswi 3" : : "r"(code) : "r0", "r1", "r2", "r3", "memory");
#else
  asm volatile("mov r0, %0\n\tswi 3 << 16" : : "r"(code) : "r0", "r1", "r2", "r3", "memory");
#endif

  __builtin_unreachable();
}

// The report's writer seam carries the second destination the line has. A line wider than one row is cut to it:
// printing the tail would cost further rows and push the summary off the top of the screen.
void writeLine(const char * text, std::size_t length, const void * writerData) noexcept {
  const auto count = std::min(length, c_consoleWidth);

  printf("%.*s", static_cast<int>(count), text);

  const bool mGBADetected = *static_cast<const bool *>(writerData);
  if (mGBADetected)
    mGBAWriteToDebugLog(text, length);
}

} // namespace

int main() {
  irqInit();
  irqEnable(IRQ_VBLANK);

  consoleDemoInit();

  const bool mGBADetected = mGBAProbe();

  const int code = ::toy::test::writeReport(&writeLine, &mGBADetected, ::toy::test::detail::caseListHead);

  if (mGBADetected)
    mGBASwiExit(code);

  while (true)
    VBlankIntrWait();

  return 0;
}
