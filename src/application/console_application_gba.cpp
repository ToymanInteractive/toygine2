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
  \file   console_application_gba.cpp
  \brief  Implementations of the toy::application::ConsoleApplication system-console calls for Game Boy Advance.
*/

#include <gba_console.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <stdio.h>

#include "application.hpp"

namespace toy::application {

// The VBlank interrupt stays enabled because idleSystemConsole() waits on it after run() returns.
bool ConsoleApplication::initializeSystemConsole() noexcept {
  ::irqInit();
  ::irqEnable(IRQ_VBLANK);

  ::consoleDemoInit();

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

void ConsoleApplication::idleSystemConsole() noexcept {
  for (;;)
    ::VBlankIntrWait();
}

} // namespace toy::application
