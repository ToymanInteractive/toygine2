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
  \file   toy_benchmark.inl
  \brief  Inline implementation of the registry's duplicate-name scan.

  \note Included by toy_benchmark.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_TOY_BENCHMARK_INL_
#define INCLUDE_BENCHMARKS_RUNNER_TOY_BENCHMARK_INL_

#include <cstring>

namespace toy::benchmark::detail {

inline const CaseRegistrar * findDuplicateName(const CaseRegistrar * head) noexcept {
  for (const CaseRegistrar * node = head; node != nullptr && node->next() != nullptr; node = node->next())
    if (std::strcmp(node->name(), node->next()->name()) == 0)
      return node;

  return nullptr;
}

} // namespace toy::benchmark::detail

#endif // INCLUDE_BENCHMARKS_RUNNER_TOY_BENCHMARK_INL_
