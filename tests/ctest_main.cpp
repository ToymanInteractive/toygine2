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
  \file   ctest_main.cpp
  \brief  Entry point of the doctest build: applies the command line and runs the registered cases.
*/

#define DOCTEST_CONFIG_IMPLEMENT

#include <doctest/doctest.h>

#include "core.hpp"

// Throwing here would terminate instead of reaching doctest: both callback types are noexcept.
static bool assertionCallback(const char * message) noexcept {
  ADD_FAIL_CHECK_AT(__FILE__, __LINE__, message);

  // Stops the caller at the failing check, the same thing an unregistered handler does.
  return true;
}

// One frame of a trace, so it reports a message rather than a second failure.
static void stackWalkCallback(const char * info) noexcept {
  ADD_MESSAGE_AT(__FILE__, __LINE__, info);
}

int main(int argc, char ** argv) noexcept {
  toy::assertion::initialize();

  toy::assertion::setCallbacks(assertionCallback, stackWalkCallback);

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int res = context.run();

  toy::assertion::deInitialize();

  if (context.shouldExit())
    exit(res);

  return res;
}
