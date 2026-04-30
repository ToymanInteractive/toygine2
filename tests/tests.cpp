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

using vu16 = volatile unsigned short;

inline vu16 & mgba_enable_reg() {
  return *reinterpret_cast<vu16 *>(0x4FFF780);
}

inline vu16 & mgba_flags_reg() {
  return *reinterpret_cast<vu16 *>(0x4FFF700);
}

#define MGBA_LOG_FATAL 0
#define MGBA_LOG_ERROR 1
#define MGBA_LOG_WARN  2
#define MGBA_LOG_INFO  3
#define MGBA_LOG_DEBUG 4
#define MGBA_SEND      0x100

bool mgba_probe() {
  mgba_enable_reg() = 0xC0DE;
  return mgba_enable_reg() == 0x1DEA;
}

// Завершить эмулятор (exitCode — для CI: 0 = success)
static inline void mgba_exit(int exitCode) {
  if (!mgba_probe())
    return;
  // FATAL level вызывает выход из эмулятора
  // exitCode передаётся в старшем байте flags
  mgba_flags_reg() = MGBA_LOG_FATAL | MGBA_SEND | ((exitCode & 0xFF) << 8);
}

int main() {
  irqInit();
  irqEnable(IRQ_VBLANK);

  consoleDemoInit();

  toy::assertion::initialize();

  toy::assertion::setCallbacks(assertionCallback, stackWalkCallback);

  GbaFilteringStreamBuf gbaStreamBuf;
  auto                  gbaStream = std::ostream(&gbaStreamBuf);

  doctest::Context context;
  context.setCout(&gbaStream);

  //  context.applyCommandLine(argc, argv);

  int res = context.run(); // run doctest

  toy::assertion::deInitialize();

  // important - query flags (and --exit) rely on the user doing this
  if (context.shouldExit()) {
    // propagate the result of the tests
    mgba_exit(res);
  }

  mgba_exit(res);
  while (true) {} // hang on real device
}

#else

int main(int argc, char ** argv) {
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
