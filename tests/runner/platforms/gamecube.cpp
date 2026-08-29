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
  \file   gamecube.cpp
  \brief  Platform layer for Nintendo GameCube: console output and the entry point.
*/

#include <algorithm>
#include <cstddef>
#include <cstdio>

#include <gccore.h>

#include "report.hpp"

namespace {

// Columns one row of the console holds. Unknown until the video mode is picked, so main() fills it in before the
// first report line is written; the value here only keeps the writer safe if that ever stops being true.
std::size_t consoleWidth = 0;

// The report's writer seam carries caller data stdout has no use for. A line wider than one row is cut to it:
// printing the tail would cost further rows and push the summary off the top of the screen.
void writeLine(const char * text, std::size_t length, [[maybe_unused]] const void * writerData) noexcept {
  const auto count = std::min(length, consoleWidth);

  printf("%.*s", static_cast<int>(count), text);
}

} // namespace

int main() {
  VIDEO_Init();

  auto * rmode = VIDEO_GetPreferredMode(nullptr);

  auto * framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
  console_init(framebuffer, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

  VIDEO_Configure(rmode);
  VIDEO_SetNextFramebuffer(framebuffer);
  VIDEO_SetBlack(FALSE);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  if (rmode->viTVMode & VI_NON_INTERLACE)
    VIDEO_WaitVSync();

  int columns = 0;
  int rows    = 0;
  CON_GetMetrics(&columns, &rows);

  consoleWidth = static_cast<std::size_t>(columns);

  const int code = ::toy::test::writeReport(&writeLine, nullptr, ::toy::test::detail::caseListHead);

  while (SYS_MainLoop())
    VIDEO_WaitVSync();

  return code;
}
