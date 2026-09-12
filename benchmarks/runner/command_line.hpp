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
  \file   command_line.hpp
  \brief  Command line of the benchmark binary: the options it accepts and how a pattern selects a benchmark.

  Defines \ref toy::benchmark::detail::Options: what the leading options of a run asked for, and where the patterns
  begin in \c argv. The functions beside it read the arguments and match a name against those patterns.

  \note Included by main.cpp and by the runner's unit tests; a benchmark body needs toy_benchmark.hpp instead.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_COMMAND_LINE_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_COMMAND_LINE_HPP_

#include <string_view>

namespace toy::benchmark::detail {

/// What the leading options asked for; the arguments from \ref firstPattern on are patterns.
struct Options final {
  bool listOnly     = false; ///< Print each selected name with its file and line instead of running it.
  bool csv          = false; ///< Print one CSV line per timed epoch beside the tables.
  bool noTable      = false; ///< Leave the tables unprinted.
  int  firstPattern = 1;     ///< Index of the first pattern in \c argv; equals \c argc when none was given.
};

/*!
  \brief Reads the leading \c --list, \c --csv and \c --no-table in any order.

  \param argc  Argument count as \c main() received it.
  \param argv  Argument vector as \c main() received it; read, never written.

  \return Options the arguments asked for, and the index where the patterns start.

  \note Parsing stops at the first argument that is not one of the options, so an option after a pattern stays a
        pattern.

  \sa selected()
*/
[[nodiscard]] Options parseOptions(int argc, char ** argv) noexcept;

/*!
  \brief Whether a benchmark name carries any of the patterns as a substring.

  \param name      Benchmark name.
  \param count     Number of patterns; \c 0 or less selects every name.
  \param patterns  Null-terminated patterns; read only when \a count is positive.

  \return \c true when the name was selected.

  \sa parseOptions()
*/
[[nodiscard]] bool selected(std::string_view name, int count, char ** patterns) noexcept;

} // namespace toy::benchmark::detail

#include "command_line.inl"

#endif // INCLUDE_BENCHMARKS_RUNNER_COMMAND_LINE_HPP_
