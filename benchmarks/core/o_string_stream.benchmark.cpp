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
  \file   o_string_stream.benchmark.cpp
  \brief  Implementations for output string stream nanobench benchmarks in the core module.
*/

#include "../utils.hpp"
#include "core.hpp"

namespace toy {

// OStringStream benchmarks (FixedString backend)
void oStringStreamCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  // ----- primitive inserts -----

  bench.run("OStringStream<FixedString<64>> default construct", [] {
    OStringStream<FixedString<64>> s;

    doNotOptimize(s);
  });

  bench.run("OStringStream<FixedString<64>> literal and int", [] {
    OStringStream<FixedString<64>> s;

    s << "n=" << 12345;

    doNotOptimize(s);
  });

  bench.run("OStringStream<FixedString<64>> chained mixed", [] {
    OStringStream<FixedString<64>> s;

    s << "v=" << -42 << ' ' << 3.1415926f << CStringView(" end");

    doNotOptimize(s);
  });

  // ----- chrono::Duration -----

  bench.run("OStringStream Duration nanoseconds", [] {
    OStringStream<FixedString<32>> s;

    s << chrono::nanoseconds{16'000'000};

    doNotOptimize(s);
  });

  bench.run("OStringStream Duration milliseconds", [] {
    OStringStream<FixedString<32>> s;

    s << chrono::milliseconds{1'042};

    doNotOptimize(s);
  });

  bench.run("OStringStream Duration seconds", [] {
    OStringStream<FixedString<32>> s;

    s << chrono::seconds{65};

    doNotOptimize(s);
  });

  bench.run("OStringStream Duration negative milliseconds", [] {
    OStringStream<FixedString<32>> s;

    s << chrono::milliseconds{-500};

    doNotOptimize(s);
  });

  // ----- chrono::DurationFormat -----

  bench.run("OStringStream DurationFormat hh:mm:ss", [] {
    OStringStream<FixedString<32>> s;

    s << chrono::DurationFormat{"h:m:s:z", chrono::milliseconds{3'723'000}};

    doNotOptimize(s);
  });

  bench.run("OStringStream DurationFormat hh:mm:ss.zzz", [] {
    OStringStream<FixedString<32>> s;

    s << chrono::DurationFormat{"hh:mm:ss.zzz", chrono::milliseconds{3'723'042}};

    doNotOptimize(s);
  });

  // ----- chrono::TimePoint / TimePointFormat -----

  bench.run("OStringStream TimePoint milliseconds", [] {
    OStringStream<FixedString<32>>                                     s;
    const chrono::TimePoint<chrono::SteadyClock, chrono::milliseconds> tp{chrono::milliseconds{3'723'042}};

    s << tp;

    doNotOptimize(s);
  });

  bench.run("OStringStream TimePointFormat hh:mm:ss.zzz", [] {
    OStringStream<FixedString<32>>                                     s;
    const chrono::TimePoint<chrono::SteadyClock, chrono::milliseconds> tp{chrono::milliseconds{3'723'042}};

    s << chrono::TimePointFormat{"hh:mm:ss.zzz", tp};

    doNotOptimize(s);
  });
}

} // namespace toy
