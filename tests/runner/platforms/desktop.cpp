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
  \file   desktop.cpp
  \brief  Platform layer for Windows, Linux and macOS: standard output, the process exit code, and the entry point.
*/

#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include "report.hpp"

namespace {

// The report's writer seam carries caller data stdout has no use for.
void writeToStdout(const char * text, std::size_t length, [[maybe_unused]] void * writerData) noexcept {
  std::fwrite(text, 1, length, stdout);
}

} // namespace

int main() {
  const int code = ::toy::test::writeReport(&writeToStdout, nullptr, ::toy::test::detail::caseListHead);

  std::fflush(stdout);

  std::exit(code);
}
