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
  \file   console_application_md.cpp
  \brief  Implementations of the toy::application::ConsoleApplication system-console calls for Sega Mega Drive.
*/

#include "application.hpp"
#include "console.hpp"

namespace toy::application {

bool ConsoleApplication::initializeSystemConsole() noexcept {
  toy::console::initialize();

  return true;
}

void ConsoleApplication::writeToSystemConsole(StringView text, bool isError, bool newLine) noexcept {
  if (text.empty() && !newLine)
    return;

  if (newLine)
    toy::console::writeLine(text, isError);
  else if (isError)
    toy::console::writeError(text);
  else
    toy::console::write(text);
}

void ConsoleApplication::idleSystemConsole() noexcept {
  // WaitForInterrupt's "i" constraint fails at -O0 (parameter, not immediate); issue the same stop instruction here.
  for (;;)
    asm volatile("stop %0" : : "i"(0x2000 | (platform::detail::consoleInterruptMask << 8)) : "cc");
}

} // namespace toy::application
