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
  \brief  Nanobench benchmarks for toy::makeVFormatArguments() and toy::vformatTo().
*/

#include "../benchmark_factory.hpp"
#include "core.hpp"

namespace toy {

void formatCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  bench.run("makeVFormatArguments single int", [] {
    const int x    = 42;
    auto      args = makeVFormatArguments(x);

    doNotOptimize(args);
  });

  bench.run("makeVFormatArguments single c-string", [] {
    const char * msg  = "hello";
    auto         args = makeVFormatArguments(msg);

    doNotOptimize(args);
  });

  bench.run("makeVFormatArguments single FixedString", [] {
    FixedString<16> str("engine");
    auto            args = makeVFormatArguments(str);

    doNotOptimize(args);
  });

  bench.run("makeVFormatArguments 3 args mixed", [] {
    const int    a    = 42;
    const char * b    = "world";
    const bool   c    = true;
    auto         args = makeVFormatArguments(a, b, c);

    doNotOptimize(args);
  });

  bench.run("makeVFormatArguments 5 args int", [] {
    const int a    = 1;
    const int b    = 2;
    const int c    = 3;
    const int d    = 4;
    const int e    = 5;
    auto      args = makeVFormatArguments(a, b, c, d, e);

    doNotOptimize(args);
  });

  bench.run("makeVFormatArguments + formatFn int", [] {
    const int x    = 99;
    auto      args = makeVFormatArguments(x);

    FixedString<32> buf;
    FormatContext   ctx{static_cast<void *>(&buf), [](void * c, const char * data, size_t n) noexcept {
                        static_cast<FixedString<32> *>(c)->append(data, n);
                        }};

    args[0].formatFn(args[0].value, ctx);

    doNotOptimize(buf);
  });

  bench.run("makeVFormatArguments + formatFn c-string", [] {
    const char * msg  = "benchmark";
    auto         args = makeVFormatArguments(msg);

    FixedString<32> buf;
    FormatContext   ctx{static_cast<void *>(&buf), [](void * c, const char * data, size_t n) noexcept {
                        static_cast<FixedString<32> *>(c)->append(data, n);
                        }};

    args[0].formatFn(args[0].value, ctx);

    doNotOptimize(buf);
  });

  bench.run("makeVFormatArguments + formatFn FixedString", [] {
    FixedString<32> str("benchmark");
    auto            args = makeVFormatArguments(str);

    FixedString<32> buf;
    FormatContext   ctx{static_cast<void *>(&buf), [](void * c, const char * data, size_t n) noexcept {
                        static_cast<FixedString<32> *>(c)->append(data, n);
                        }};

    args[0].formatFn(args[0].value, ctx);

    doNotOptimize(buf);
  });

  // ----- vformatTo (array overload) -----

  bench.run("vformatTo no placeholders", [] {
    FixedString<64> output;
    auto            args = makeVFormatArguments();

    vformatTo(output, CStringView("Hello World"), args);

    doNotOptimize(output);
  });

  bench.run("vformatTo single int", [] {
    FixedString<64> output;
    const int       x    = 42;
    auto            args = makeVFormatArguments(x);

    vformatTo(output, CStringView("value: {}"), args);

    doNotOptimize(output);
  });

  bench.run("vformatTo single c-string", [] {
    FixedString<64> output;
    const char *    msg  = "hello";
    auto            args = makeVFormatArguments(msg);

    vformatTo(output, CStringView("say: {}"), args);

    doNotOptimize(output);
  });

  bench.run("vformatTo single FixedString", [] {
    FixedString<64> output;
    FixedString<16> str("engine");
    auto            args = makeVFormatArguments(str);

    vformatTo(output, CStringView("toy {}"), args);

    doNotOptimize(output);
  });

  bench.run("vformatTo 3 args mixed", [] {
    FixedString<64> output;
    const int       a    = 42;
    const char *    b    = "world";
    const bool      c    = true;
    auto            args = makeVFormatArguments(a, b, c);

    vformatTo(output, CStringView("{} {} {}"), args);

    doNotOptimize(output);
  });

  bench.run("vformatTo positional reorder", [] {
    FixedString<64> output;
    const int       a    = 10;
    const int       b    = 20;
    auto            args = makeVFormatArguments(a, b);

    vformatTo(output, CStringView("{1} before {0}"), args);

    doNotOptimize(output);
  });

  bench.run("vformatTo escaped braces", [] {
    FixedString<64> output;
    const int       x    = 42;
    auto            args = makeVFormatArguments(x);

    vformatTo(output, CStringView("{{{}}}"), args);

    doNotOptimize(output);
  });

  bench.run("vformatTo 5 args int", [] {
    FixedString<128> output;
    const int        a    = 1;
    const int        b    = 2;
    const int        c    = 3;
    const int        d    = 4;
    const int        e    = 5;
    auto             args = makeVFormatArguments(a, b, c, d, e);

    vformatTo(output, CStringView("{} {} {} {} {}"), args);

    doNotOptimize(output);
  });
}

} // namespace toy
