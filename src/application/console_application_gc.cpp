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
  \file   console_application_gc.cpp
  \brief  Implementations of the toy::application::ConsoleApplication system-console calls for GameCube.
*/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>

#include "application.hpp"

/*!
  \brief Idles a console application after run() returns, waiting for vertical blanking until the console is reset.

  IMPLEMENT_CONSOLE_APP calls it after run(), so the console stays on screen.

  \warning Never returns: leaving the loop hands the console to libogc, which resets or powers it off.
*/
[[noreturn]] void idleSystemConsole() noexcept;

namespace toy::application {

bool ConsoleApplication::initializeSystemConsole() noexcept {
  ::VIDEO_Init();

  ::consoleInit(nullptr);

  return true;
}

void ConsoleApplication::writeToSystemConsole(StringView text, bool isError, bool newLine) noexcept {
  if (text.empty() && !newLine)
    return;

  FILE * const stream = isError ? stderr : stdout;

  if (!text.empty())
    ::fwrite(text.data(), 1, text.size(), stream);

  if (newLine)
    ::fputc('\n', stream);
}

} // namespace toy::application

[[noreturn]] void idleSystemConsole() noexcept {
  while (::SYS_MainLoop())
    ::VIDEO_WaitVSync();

  // libogc resets or powers off the console in its exit handler, which is why leaving the loop is not a return.
  ::exit(0);
}
