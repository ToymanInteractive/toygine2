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
  \file   console_report.inl
  \brief  Inline implementation of toy::benchmark::writeReport().

  \note Included by console_report.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_CONSOLE_REPORT_INL_
#define INCLUDE_BENCHMARKS_RUNNER_CONSOLE_REPORT_INL_

#include <cstdint>
#include <initializer_list>

namespace toy::benchmark {

inline int writeReport(const Clock & clock, const Output & output, std::span<const char * const> heading,
                       const CaseRegistrar * head) noexcept {
  detail::LineBuffer line{output};

  line.addText("# toygine2 benchmarks:");

  for (const char * word : heading) {
    line.addText(" ");
    line.addText(word);
  }

  line.flush();

  // tools/builder/bmf_from_csv.py would merge the rows of two benchmarks sharing a name into one series.
  if (const auto * duplicate = detail::findDuplicateName(head); duplicate != nullptr) {
    line.addText("# duplicate benchmark name: ");
    line.addText(duplicate->name());
    line.flush();

    for (const auto * declaration : {duplicate, duplicate->next()}) {
      line.addText("#   ");
      line.addText(declaration->file());
      line.addText(":");
      line.addDecimal(static_cast<std::uint64_t>(declaration->line()));
      line.flush();
    }

    return 1;
  }

  // bmf_from_csv.py rejects the whole log unless this line matches its HEADER byte for byte.
  line.addText("#csv case,row,unit,ps_per_tick,epochs,iters,ticks");
  line.flush();

  for (const auto * benchmarkCase = head; benchmarkCase != nullptr; benchmarkCase = benchmarkCase->next()) {
    Bench bench{clock, output, benchmarkCase->name()};

    benchmarkCase->body()(bench);
  }

  line.addText("# done");
  line.flush();

  return 0;
}

} // namespace toy::benchmark

#endif // INCLUDE_BENCHMARKS_RUNNER_CONSOLE_REPORT_INL_
