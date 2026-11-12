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
  \file   console_application_ogc.cpp
  \brief  Implementations of the toy::application::ConsoleApplication system-console calls for GameCube and Wii.
*/

#include <gccore.h>
#include <stdio.h>

#include "application.hpp"

namespace toy::application {

bool ConsoleApplication::initializeSystemConsole() noexcept {
  ::VIDEO_Init();

  auto * const console = ::consoleInit(nullptr);

  return console != nullptr && console->consoleInitialised;
}

void ConsoleApplication::writeToSystemConsole(StringView text, bool isError, bool newLine) noexcept {
  if (text.empty() && !newLine)
    return;

  FILE * const stream = isError ? stderr : stdout;

  if (isError)
    ::fputs(CONSOLE_RED, stream);

  if (!text.empty())
    ::fwrite(text.data(), 1, text.size(), stream);

  if (newLine)
    ::fputc('\n', stream);

  if (isError)
    ::fputs(CONSOLE_RESET, stream);
}

// libogc resets or powers off the console in its exit handler, which is why leaving the loop is not a return.
void ConsoleApplication::idleSystemConsole() noexcept {
  while (::SYS_MainLoop())
    ::VIDEO_WaitVSync();

  ::exit(0);
}

} // namespace toy::application
