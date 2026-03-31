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

#include "benchmark_factory.hpp"
#include "core.hpp"

namespace toy {

void hashesCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void oStringStreamCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;
void stringFixedStorageCoreBenchmarks(ankerl::nanobench::Bench &) noexcept;

} // namespace toy

void runCoreBenchmarks() noexcept {
  auto bench = createBench("Core module");

  toy::hashesCoreBenchmarks(bench);
  toy::oStringStreamCoreBenchmarks(bench);
  toy::stringFixedStorageCoreBenchmarks(bench);

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

  // ftoa benchmarks
  {
    constexpr size_t bufSize = 64;
    char buf[bufSize];

    bench.run("ftoa float default precision", [&] {
      char * r = toy::ftoa(buf, bufSize, 3.14159265f);
      doNotOptimize(r);
    });

    bench.run("ftoa float custom precision", [&] {
      char * r = toy::ftoa(buf, bufSize, -123.456f, 4);
      doNotOptimize(r);
    });

    bench.run("ftoa double default precision", [&] {
      char * r = toy::ftoa(buf, bufSize, 3.14159265358979);
      doNotOptimize(r);
    });

    bench.run("ftoa double custom precision", [&] {
      char * r = toy::ftoa(buf, bufSize, -12345.6789012345, 8);
      doNotOptimize(r);
    });
  }

  // formatNumberString benchmarks
  {
    constexpr size_t bufSize = 32;
    char buf[bufSize];

    bench.run("formatNumberString long comma", [&] {
      static const char digits[] = "123456789012345";
      size_t i = 0;
      while (digits[i] != '\0') {
        buf[i] = digits[i];
        ++i;
      }
      buf[i] = '\0';
      toy::formatNumberString(buf, bufSize, ",");
      doNotOptimize(buf);
    });
  }

  // highestBit benchmarks
  {
    bench.run("highestBit high bits", [] {
      auto r = toy::highestBit(0x8000000000000000ULL);
      doNotOptimize(r);
    });
  }
}
