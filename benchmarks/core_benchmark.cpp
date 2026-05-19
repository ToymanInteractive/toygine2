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
  \file   core_benchmark.cpp
  \brief  Implementation of nanobench benchmarks for the core module.
*/

#include <nanobench.h>

#include "core.hpp"

namespace toy {

void ftoaCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void hashesCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void itoaCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void oStringStreamCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void utoaCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void formatNumberStringCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void highestBitCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;

} // namespace toy

void runCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  toy::ftoaCoreBenchmarks(bench);
  toy::hashesCoreBenchmarks(bench);
  toy::itoaCoreBenchmarks(bench);
  toy::oStringStreamCoreBenchmarks(bench);
  toy::utoaCoreBenchmarks(bench);
  toy::formatNumberStringCoreBenchmarks(bench);
  toy::highestBitCoreBenchmarks(bench);
}
