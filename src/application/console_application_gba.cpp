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
  \brief  Implementation of toy::application::ConsoleApplication::writeToSystemConsole() for Game Boy Advance.
*/

#include <gba_console.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <stdio.h>

#include "application.hpp"

/*!
  \brief Sets up the interrupt controller and the default text console for a console application.

  IMPLEMENT_CONSOLE_APP calls it before run(), so the write calls print onto the console.

  \post The VBlank interrupt is enabled, and the console receives everything stdout writes.
*/
void initializeSystemConsole() noexcept;

/*!
  \brief Idles a console application after run() returns, waiting for vertical blanking forever.

  \warning Never returns: the Game Boy Advance has nothing to exit into.
*/
[[noreturn]] void idleSystemConsole() noexcept;

namespace toy::application {

void ConsoleApplication::writeToSystemConsole(StringView text, [[maybe_unused]] bool isError, bool newLine) noexcept {
  if (text.empty() && !newLine)
    return;

  if (!text.empty())
    ::fwrite(text.data(), 1, text.size(), stdout);

  if (newLine)
    ::fputc('\n', stdout);
}

} // namespace toy::application

void initializeSystemConsole() noexcept {
  ::irqInit();
  ::irqEnable(IRQ_VBLANK);

  ::consoleDemoInit();
}

[[noreturn]] void idleSystemConsole() noexcept {
  for (;;)
    ::VBlankIntrWait();
}
