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
  \file   console_report.hpp
  \brief  Console run report, printed in one pass over the benchmark registry.

  Declares toy::benchmark::writeReport(), which the Nintendo GBA and Sega MD entry points call with their timer and
  output channels. The report carries the CSV lines \c tools/builder/bmf_from_csv.py reads.

  \note Included by a console platform file and by the runner's unit test.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_CONSOLE_REPORT_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_CONSOLE_REPORT_HPP_

#include <span>

#include "toy_benchmark.hpp"

namespace toy::benchmark {

/*!
  \brief Runs every registered benchmark and prints the console report.

  Prints a heading comment, the CSV header, one CSV row per toy::benchmark::Bench::run() call with benchmarks in name
  order, and \c "# done". A repeated name stops the run before the first row with a comment naming both declarations.

  \param clock    Platform timer and calibration limits for every benchmark.
  \param output   Destination of the report lines.
  \param heading  Words printed after \c "# toygine2 benchmarks:", such as the platform and the build id.
  \param head     Registry head; may be \c nullptr.

  \return \c 0 when every benchmark ran, \c 1 when a repeated name stopped the run.
*/
[[nodiscard]] int writeReport(const Clock & clock, const Output & output, std::span<const char * const> heading,
                              const CaseRegistrar * head) noexcept;

} // namespace toy::benchmark

#include "console_report.inl"

#endif // INCLUDE_BENCHMARKS_RUNNER_CONSOLE_REPORT_HPP_
