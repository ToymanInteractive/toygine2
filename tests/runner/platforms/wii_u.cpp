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
  \file   wii_u.cpp
  \brief  Platform layer for Nintendo Wii U: console output and the entry point.
*/

#include <algorithm>
#include <cstddef>

#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

#include "report.hpp"

namespace {

// Columns one row of the WHB log console holds. The WHB API reports no width, so the figure is fixed here.
constexpr std::size_t c_consoleWidth = 80;

// The report's writer seam carries caller data the log console has no use for. One call is one log entry, so the
// trailing newline goes, and a line wider than the screen is cut to what one row holds.
void writeToLog(const char * text, std::size_t length, [[maybe_unused]] void * writerData) noexcept {
  const auto lineLength = (length != 0 && text[length - 1] == '\n') ? length - 1 : length;
  const auto count      = std::min(lineLength, c_consoleWidth);

  WHBLogPrintf("%.*s", static_cast<int>(count), text);
}

} // namespace

int main() {
  WHBProcInit();
  WHBLogConsoleInit();

  const int code = ::toy::test::writeReport(&writeToLog, nullptr, ::toy::test::detail::caseListHead);

  while (WHBProcIsRunning()) {
    WHBLogConsoleDraw();

    OSSleepTicks(OSMillisecondsToTicks(100));
  }

  WHBLogConsoleFree();
  WHBProcShutdown();

  return code;
}
