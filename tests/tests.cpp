//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \file   tests.cpp
  \brief  Test runner entry point: doctest \c main, assertion hooks, and optional GBA console routing.
*/

#define DOCTEST_CONFIG_IMPLEMENT

#include <doctest/doctest.h>

#include "core.hpp"

[[noreturn]] static bool assertionCallback(const char * message) {
  throw std::runtime_error(message);
}

static void stackWalkCallback([[maybe_unused]] const char * info) noexcept {}

#if defined(__GBA__)

#include <gba_console.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_video.h>
#include <stdio.h>

#include "gba_filtering_stream_buf.hpp"

/*!
  \brief Ends the mgba-headless run, propagating \a code as the process exit status.

  Loads \a code into \c r0 and issues SWI \c 0x60, which mgba-headless traps (run with \c -S \c 0x60 \c -R \c r0) to
  stop emulation and return \c r0 as the process exit code. The immediate sits outside the GBA BIOS SWI range so it
  never collides with a real BIOS call. On real hardware the SWI is a no-op, so the function falls through to an
  idle loop.

  \param code Process exit code (doctest result; \c 0 on success).
*/
[[noreturn]] static void mGbaExit(int code) noexcept {
  asm volatile("mov r0, %0\n\tswi 0x60" : : "r"(code) : "r0", "memory");

  for (;;) {}
}

int main() noexcept {
  irqInit();
  irqEnable(IRQ_VBLANK);

  consoleDemoInit();

  toy::assertion::initialize();

  toy::assertion::setCallbacks(assertionCallback, stackWalkCallback);

  GbaFilteringStreamBuf gbaStreamBuf;
  auto                  gbaStream = std::ostream(&gbaStreamBuf);

  doctest::Context context;
  context.setCout(&gbaStream);

  int res = context.run(); // run doctest

  toy::assertion::deInitialize();

  // GBA crt0 never returns to the OS; signal mgba-headless to exit and hand back the doctest result.
  mGbaExit(res);
}

#else

int main(int argc, char ** argv) noexcept {
  toy::assertion::initialize();

  toy::assertion::setCallbacks(assertionCallback, stackWalkCallback);

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int res = context.run(); // run doctest

  toy::assertion::deInitialize();

  // important - query flags (and --exit) rely on the user doing this
  if (context.shouldExit()) {
    // propagate the result of the tests
    return res;
  }

  return res;
}

#endif
