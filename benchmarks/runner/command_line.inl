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
  \file   command_line.inl
  \brief  Inline implementations of toy::benchmark::detail::parseOptions() and selected().

  \note Included by command_line.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_COMMAND_LINE_INL_
#define INCLUDE_BENCHMARKS_RUNNER_COMMAND_LINE_INL_

#include <cstring>

namespace toy::benchmark::detail {

inline Options parseOptions(int argc, char ** argv) noexcept {
  Options options;

  while (options.firstPattern < argc) {
    if (const char * argument = argv[options.firstPattern]; std::strcmp(argument, "--list") == 0)
      options.listOnly = true;
    else if (std::strcmp(argument, "--csv") == 0)
      options.csv = true;
    else if (std::strcmp(argument, "--no-table") == 0)
      options.noTable = true;
    else
      break;

    ++options.firstPattern;
  }

  return options;
}

inline bool selected(std::string_view name, int count, char ** patterns) noexcept {
  if (count <= 0)
    return true;

  for (int index = 0; index < count; ++index)
    if (name.contains(patterns[index]))
      return true;

  return false;
}

} // namespace toy::benchmark::detail

#endif // INCLUDE_BENCHMARKS_RUNNER_COMMAND_LINE_INL_
