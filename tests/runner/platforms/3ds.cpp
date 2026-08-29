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
  \file   3ds.cpp
  \brief  Platform layer for Nintendo 3DS: console output and the entry point.
*/

#include <algorithm>
#include <cstddef>
#include <cstdio>

// clang-format off
#include <3ds.h>
// clang-format on

#include "report.hpp"

namespace {

// Columns of the console consoleInit() sets up on the top screen: its 400 pixels over the 8-pixel tiles it draws with.
constexpr std::size_t c_consoleWidth = 50;

// The report's writer seam carries caller data stdout has no use for. A line wider than one row is cut to it:
// printing the tail would cost further rows and push the summary off the top of the screen.
void writeLine(const char * text, std::size_t length, [[maybe_unused]] const void * writerData) noexcept {
  const auto count = std::min(length, c_consoleWidth);

  printf("%.*s", static_cast<int>(count), text);
}

} // namespace

int main() {
  gfxInitDefault();

  consoleInit(GFX_TOP, nullptr);

  const int code = ::toy::test::writeReport(&writeLine, nullptr, ::toy::test::detail::caseListHead);

  while (aptMainLoop()) {
    gfxFlushBuffers();
    gfxSwapBuffers();

    gspWaitForVBlank();
  }

  gfxExit();

  return code;
}
