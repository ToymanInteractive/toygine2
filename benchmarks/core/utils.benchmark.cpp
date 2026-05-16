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
  \file   utils.benchmark.cpp
  \brief  Implementations for utility function nanobench benchmarks in the core module.
*/

#include "../utils.hpp"

#include "core.hpp"

namespace toy {

// itoa benchmarks
void itoaCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  constexpr size_t bufSize = 32;

  bench.run("itoa int8_t", [&] {
    char buf[bufSize];

    char * r = toy::itoa(buf, bufSize, int8_t{42});

    doNotOptimize(r);
  });

  bench.run("itoa int16_t", [&] {
    char buf[bufSize];

    char * r = toy::itoa(buf, bufSize, int16_t{-1234});

    doNotOptimize(r);
  });

  bench.run("itoa int32_t", [&] {
    char buf[bufSize];

    char * r = toy::itoa(buf, bufSize, int32_t{12345678});

    doNotOptimize(r);
  });

  bench.run("itoa int64_t", [&] {
    char buf[bufSize];

    char * r = toy::itoa(buf, bufSize, int64_t{-9223372036854775807LL});

    doNotOptimize(r);
  });
}

// utoa benchmarks
void utoaCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  constexpr size_t bufSize = 32;

  bench.run("utoa uint8_t base 10", [&] {
    char buf[bufSize];

    char * r = toy::utoa(buf, bufSize, uint8_t{200}, 10u);

    doNotOptimize(r);
  });

  bench.run("utoa uint16_t base 10", [&] {
    char buf[bufSize];

    char * r = toy::utoa(buf, bufSize, uint16_t{65535}, 10u);

    doNotOptimize(r);
  });

  bench.run("utoa uint32_t base 10", [&] {
    char buf[bufSize];

    char * r = toy::utoa(buf, bufSize, uint32_t{4000000000u}, 10);

    doNotOptimize(r);
  });

  bench.run("utoa uint64_t base 10", [&] {
    char buf[bufSize];

    char * r = toy::utoa(buf, bufSize, uint64_t{18446744073709551615ULL}, 10);

    doNotOptimize(r);
  });

  bench.run("utoa uint32_t base 16", [&] {
    char buf[bufSize];

    char * r = toy::utoa(buf, bufSize, uint32_t{0xDEADBEEFu}, 16);

    doNotOptimize(r);
  });

  bench.run("utoa uint64_t base 16", [&] {
    char buf[bufSize];

    char * r = toy::utoa(buf, bufSize, uint64_t{0xFFFFFFFFFFFFFFFFULL}, 16);

    doNotOptimize(r);
  });
}

// ftoa benchmarks
void ftoaCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  constexpr size_t bufSize = 64;

  bench.run("ftoa float default precision", [&] {
    char buf[bufSize];

    char * r = toy::ftoa(buf, bufSize, 3.14159265f);

    doNotOptimize(r);
  });

  bench.run("ftoa float custom precision", [&] {
    char buf[bufSize];

    char * r = toy::ftoa(buf, bufSize, -123.456f, 4);

    doNotOptimize(r);
  });

  bench.run("ftoa double default precision", [&] {
    char buf[bufSize];

    char * r = toy::ftoa(buf, bufSize, 3.14159265358979);

    doNotOptimize(r);
  });

  bench.run("ftoa double custom precision", [&] {
    char buf[bufSize];

    char * r = toy::ftoa(buf, bufSize, -12345.6789012345, 8);

    doNotOptimize(r);
  });
}

// formatNumberString benchmarks
void formatNumberStringCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  static constexpr auto initialBuf = []() consteval noexcept {
    const char * src = "123456789012345";

    std::array<char, 32> arr{};
    size_t               i = 0;
    while (src[i] != '\0') {
      arr[i] = src[i];
      ++i;
    }

    return arr;
  }();

  bench.run("formatNumberString long comma", [] {
    auto buf = initialBuf;

    toy::formatNumberString(buf.data(), buf.size(), ",");

    doNotOptimize(buf);
  });
}

// highestBit benchmarks
void highestBitCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  bench.run("highestBit high bits", [] {
    auto r = toy::highestBit(0x8000000000000000ULL);

    doNotOptimize(r);
  });
}

} // namespace toy
