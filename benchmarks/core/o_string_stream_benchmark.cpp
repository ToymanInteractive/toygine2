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
  \file   o_string_stream_benchmark.cpp
  \brief  Nanobench benchmarks for output string stream from the core module.
*/

#include "../benchmark_factory.hpp"
#include "core.hpp"

namespace toy {

// OStringStream benchmarks (FixedString backend)
void oStringStreamCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  bench.run("OStringStream<FixedString<64>> default construct", [] {
    toy::OStringStream<toy::FixedString<64>> s;
    doNotOptimize(s.str().data());
  });

  bench.run("OStringStream<FixedString<64>> literal and int", [] {
    toy::OStringStream<toy::FixedString<64>> s;
    s << "n=" << 12345;
    doNotOptimize(s.str().data());
  });

  bench.run("OStringStream<FixedString<64>> chained mixed", [] {
    toy::OStringStream<toy::FixedString<64>> s;
    s << "v=" << -42 << ' ' << 3.14f << toy::CStringView(" end");
    doNotOptimize(s.str().data());
  });
}

} // namespace toy
