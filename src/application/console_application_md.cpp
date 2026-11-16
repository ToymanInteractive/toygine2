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

namespace {

/// The vertical interrupt is level 6; mask 5 lets it through.
constexpr unsigned int c_interruptMask = 5;

} // namespace

bool ConsoleApplication::initializeSystemConsole() noexcept {
  toy::console::initialize();

  return true;
}

void ConsoleApplication::writeToSystemConsole(StringView text, [[maybe_unused]] bool isError, bool newLine) noexcept {
  if (text.empty() && !newLine)
    return;

  if (!text.empty())
    toy::console::write(text);

  if (newLine)
    toy::console::write(toy::StringView{"\n"});

  return;
}

void ConsoleApplication::idleSystemConsole() noexcept {
  // WaitForInterrupt's "i" constraint fails at -O0 (parameter, not immediate); issue the same stop instruction here.
  for (;;)
    asm volatile("stop %0" : : "i"(0x2000 | (c_interruptMask << 8)) : "cc");
}

} // namespace toy::application
