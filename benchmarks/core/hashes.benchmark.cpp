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
  \file   hashes.benchmark.cpp
  \brief  Implementations for hashes nanobench benchmarks in the core module.
*/

#include "../benchmark_factory.hpp"
#include "core.hpp"

namespace toy {

// Hashes benchmarks (toy::crc8, crc16, crc32)
void hashesCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  static unsigned char crcPayload512[512];
  static const bool    crcPayload512Init = []() noexcept {
    for (size_t i = 0; i < 512; ++i) {
      crcPayload512[i] = static_cast<unsigned char>(i ^ 0xA5U);
    }
    return true;
  }();
  (void)crcPayload512Init;

  bench.run("crc8 short string", [] {
    static const char s[] = "Hello";
    auto              r   = toy::crc8(s, sizeof(s) - 1);
    doNotOptimize(r);
  });

  bench.run("crc8 medium string", [] {
    static const char s[] = "Toygine2 - Free 2D/3D game engine.";
    auto              r   = toy::crc8(s, sizeof(s) - 1);
    doNotOptimize(r);
  });

  bench.run("crc16 medium string", [] {
    static const char s[] = "Toygine2 - Free 2D/3D game engine.";
    auto              r   = toy::crc16(s, sizeof(s) - 1);
    doNotOptimize(r);
  });

  bench.run("crc32 medium string", [] {
    static const char s[] = "Toygine2 - Free 2D/3D game engine.";
    auto              r   = toy::crc32(s, sizeof(s) - 1);
    doNotOptimize(r);
  });

  bench.run("crc32 512 byte buffer", [] {
    auto r = toy::crc32(crcPayload512, sizeof(crcPayload512));
    doNotOptimize(r);
  });
}

} // namespace toy
