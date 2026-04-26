//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \file   console_application.cpp
  \brief  Implementations for \ref toy::application::ConsoleApplication.
*/

#include <print>

#include "application.hpp"

namespace toy::application {

static bool consoleAssert(const char * message) {
  std::print("--------------------------------------------\n");
  std::print(" Assert Failed!\n");
  std::print("--------------------------------------------\n");
  if (message != nullptr) {
    std::print("{}\n", message);
    std::print("--------------------------------------------\n");
  }

  char answer = '\0';
  while (answer != 'c' && answer != 'd') {
    std::print(" press 'c' - to continue, 'd' - to break in debug mode\n");
    answer = static_cast<char>(getchar());

    char temp = '\0';
    while (temp != EOF && temp != '\n')
      temp = static_cast<char>(getchar());
  }

  std::print("--------------------------------------------\n");

  return (answer == 'c');
}

static void consoleStackWalker(const char * message) {
  std::print("--------------------------------------------\n");
  std::print(" Call Stack:\n");
  std::print("--------------------------------------------\n");
  if (message != nullptr) {
    std::print("{}\n", message);
    std::print("--------------------------------------------\n");
  }
}

ConsoleApplication::ConsoleApplication() noexcept
  : CoreApplication(consoleAssert, consoleStackWalker) {}

ConsoleApplication::~ConsoleApplication() noexcept = default;

} // namespace toy::application
