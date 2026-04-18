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
  \file   format.benchmark.cpp
  \brief  Nanobench benchmarks for toy::vformat() and toy::vformatTo().
*/

#include "../benchmark_factory.hpp"
#include "core.hpp"

namespace toy {

void formatCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  // ----- vformatTo -----

  bench.run("vformatTo no placeholders", [] {
    FixedString<64> output;

    vformatTo(output, CStringView("Hello World"));

    doNotOptimize(output);
  });

  bench.run("vformatTo single int", [] {
    FixedString<64> output;

    vformatTo(output, CStringView("value: {}"), 42);

    doNotOptimize(output);
  });

  bench.run("vformatTo single c-string", [] {
    FixedString<64> output;
    const char *    msg = "hello";

    vformatTo(output, CStringView("say: {}"), msg);

    doNotOptimize(output);
  });

  bench.run("vformatTo single FixedString", [] {
    FixedString<64> output;
    FixedString<16> name("engine");

    vformatTo(output, CStringView("toy {}"), name);

    doNotOptimize(output);
  });

  bench.run("vformatTo single CStringView", [] {
    FixedString<64> output;
    CStringView     sv("engine");

    vformatTo(output, CStringView("toy {}"), sv);

    doNotOptimize(output);
  });

  bench.run("vformatTo 3 args mixed", [] {
    FixedString<64> output;

    vformatTo(output, CStringView("{} {} {}"), 42, "world", true);

    doNotOptimize(output);
  });

  bench.run("vformatTo 5 args int", [] {
    FixedString<128> output;

    vformatTo(output, CStringView("{} {} {} {} {}"), 1, 2, 3, 4, 5);

    doNotOptimize(output);
  });

  bench.run("vformatTo positional reorder", [] {
    FixedString<64> output;

    vformatTo(output, CStringView("{1} before {0}"), 10, 20);

    doNotOptimize(output);
  });

  bench.run("vformatTo escaped braces", [] {
    FixedString<64> output;

    vformatTo(output, CStringView("{{{}}}: {{done}}"), 42);

    doNotOptimize(output);
  });

  // ----- vformat -----

  bench.run("vformat single int", [] {
    auto result = vformat<64>(CStringView("value: {}"), 42);

    doNotOptimize(result);
  });

  bench.run("vformat 3 args mixed", [] {
    auto result = vformat<64>(CStringView("{} {} {}"), 42, "world", true);

    doNotOptimize(result);
  });

  bench.run("vformat 5 args int", [] {
    auto result = vformat<128>(CStringView("{} {} {} {} {}"), 1, 2, 3, 4, 5);

    doNotOptimize(result);
  });
}

} // namespace toy
