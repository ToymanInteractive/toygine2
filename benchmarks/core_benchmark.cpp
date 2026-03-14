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

#include "benchmark_factory.hpp"
#include "core.hpp"

void runCoreBenchmarks() noexcept {
  auto bench = createBench("Core module");

  // itoa benchmarks
  {
    constexpr size_t bufSize = 32;
    char buf[bufSize];

    bench.run("itoa int8_t", [&] {
      char * r = toy::itoa(buf, bufSize, int8_t{42});
      doNotOptimize(r);
    });

    bench.run("itoa int16_t", [&] {
      char * r = toy::itoa(buf, bufSize, int16_t{-1234});
      doNotOptimize(r);
    });

    bench.run("itoa int32_t", [&] {
      char * r = toy::itoa(buf, bufSize, int32_t{12345678});
      doNotOptimize(r);
    });

    bench.run("itoa int64_t", [&] {
      char * r = toy::itoa(buf, bufSize, int64_t{-9223372036854775807LL});
      doNotOptimize(r);
    });
  }

  // utoa benchmarks
  {
    constexpr size_t bufSize = 32;
    char buf[bufSize];

    bench.run("utoa uint8_t base 10", [&] {
      char * r = toy::utoa(buf, bufSize, uint8_t{200}, 10u);
      doNotOptimize(r);
    });

    bench.run("utoa uint16_t base 10", [&] {
      char * r = toy::utoa(buf, bufSize, uint16_t{65535}, 10u);
      doNotOptimize(r);
    });

    bench.run("utoa uint32_t base 10", [&] {
      char * r = toy::utoa(buf, bufSize, uint32_t{4000000000u}, 10);
      doNotOptimize(r);
    });

    bench.run("utoa uint64_t base 10", [&] {
      char * r = toy::utoa(buf, bufSize, uint64_t{18446744073709551615ULL}, 10);
      doNotOptimize(r);
    });

    bench.run("utoa uint32_t base 16", [&] {
      char * r = toy::utoa(buf, bufSize, uint32_t{0xDEADBEEFu}, 16);
      doNotOptimize(r);
    });

    bench.run("utoa uint64_t base 16", [&] {
      char * r = toy::utoa(buf, bufSize, uint64_t{0xFFFFFFFFFFFFFFFFULL}, 16);
      doNotOptimize(r);
    });
  }
}
