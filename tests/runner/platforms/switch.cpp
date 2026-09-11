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
  \file   switch.cpp
  \brief  Platform layer for Nintendo Switch: console output and the entry point.
*/

#include <algorithm>
#include <cstddef>
#include <cstdio>

#include <switch.h>

#include "report.hpp"

namespace {

// Columns per row after consoleInit(); libnx reports it in PrintConsole::consoleWidth but publishes no default.
constexpr std::size_t c_consoleWidth = 80;

// Caller data stdout has no use for; a line wider than one row is cut, so the summary stays on screen.
void writeLine(const char * text, std::size_t length, [[maybe_unused]] const void * writerData) noexcept {
  const auto count = std::min(length, c_consoleWidth);

  printf("%.*s", static_cast<int>(count), text);
}

} // namespace

int main() {
  consoleInit(nullptr);

  padConfigureInput(1, HidNpadStyleSet_NpadStandard);

  PadState pad;
  padInitializeDefault(&pad);

  const int code = ::toy::test::writeReport(&writeLine, nullptr, ::toy::test::detail::caseListHead);

  // Holds the screen until Plus is pressed; otherwise the applet closes and takes the last lines with it.
  while (appletMainLoop()) {
    padUpdate(&pad);

    if ((padGetButtonsDown(&pad) & HidNpadButton_Plus) != 0)
      break;

    consoleUpdate(nullptr);
  }

  consoleExit(nullptr);

  return code;
}
