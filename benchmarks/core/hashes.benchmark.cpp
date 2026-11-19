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
  \file   hashes.benchmark.cpp
  \brief  Benchmarks for toy::hash::crc8(), toy::hash::crc16() and toy::hash::crc32(), each beside its bitwise loop.
*/

#include "core.hpp"
#include "toy_benchmark.hpp"

namespace toy {

namespace {

// One asset page, inside the first cache level, so a case measures checksumming and not the memory system.
constexpr size_t largeSize = 4096;

// A record checksum, short enough that the call overhead is visible beside the per-byte work.
constexpr size_t smallSize = 64;

// Bytes stepping five apart wrap the whole byte range, so every sum meets both extremes, 0x00 and 0xFF.
constexpr array<uint8_t, largeSize> buffer = [] {
  array<uint8_t, largeSize> bytes{};
  for (size_t index = 0; index < largeSize; ++index)
    bytes[index] = static_cast<uint8_t>(index * 5);
  return bytes;
}();

// Hands a pointer through an opaque barrier, so a measured call reads the buffer instead of a folded constant.
[[nodiscard]] const uint8_t * opaque(const uint8_t * data) noexcept {
  benchmark::doNotOptimizeAway(data);

  return data;
}

// The bitwise loop hash::crc8() tabulates: the reflected polynomial 0x8C applied eight times per byte.
[[nodiscard]] uint8_t bitwiseCrc8(const uint8_t * data, size_t size, uint8_t crc = 0x00) noexcept {
  while (size > 0) {
    --size;
    crc ^= *data++;

    for (unsigned bit = 0; bit < 8; ++bit)
      crc = static_cast<uint8_t>((crc >> 1) ^ ((crc & 1U) != 0U ? 0x8CU : 0U));
  }

  return crc;
}

// The bitwise loop hash::crc16() tabulates: the reflected polynomial 0xA001 applied eight times per byte.
[[nodiscard]] uint16_t bitwiseCrc16(const uint8_t * data, size_t size, uint16_t crc = 0x0000) noexcept {
  while (size > 0) {
    --size;
    crc = static_cast<uint16_t>(crc ^ *data++);

    for (unsigned bit = 0; bit < 8; ++bit)
      crc = static_cast<uint16_t>((crc >> 1) ^ ((crc & 1U) != 0U ? 0xA001U : 0U));
  }

  return crc;
}

// The bitwise loop hash::crc32() tabulates: the reflected polynomial 0xEDB88320 behind a complemented state.
[[nodiscard]] uint32_t bitwiseCrc32(const uint8_t * data, size_t size, uint32_t crc = 0x00000000) noexcept {
  crc = ~crc;

  while (size > 0) {
    --size;
    crc ^= *data++;

    for (unsigned bit = 0; bit < 8; ++bit)
      crc = (crc >> 1) ^ ((crc & 1U) != 0U ? 0xEDB88320U : 0U);
  }

  return ~crc;
}

} // namespace

BENCHMARK_CASE("core/hash/crc8/compute") {
  const uint8_t * data = opaque(buffer.data());

  bench.unit("checksum");

  bench.run("hash::crc8", [&] {
    benchmark::doNotOptimizeAway(hash::crc8(data, smallSize));
  });

  bench.run("bitwiseCrc8", [&] {
    benchmark::doNotOptimizeAway(bitwiseCrc8(data, smallSize));
  });
}

BENCHMARK_CASE("core/hash/crc8/compute_large") {
  const uint8_t * data = opaque(buffer.data());

  bench.unit("checksum");

  bench.run("hash::crc8", [&] {
    benchmark::doNotOptimizeAway(hash::crc8(data, largeSize));
  });

  bench.run("bitwiseCrc8", [&] {
    benchmark::doNotOptimizeAway(bitwiseCrc8(data, largeSize));
  });
}

BENCHMARK_CASE("core/hash/crc16/compute") {
  const uint8_t * data = opaque(buffer.data());

  bench.unit("checksum");

  bench.run("hash::crc16", [&] {
    benchmark::doNotOptimizeAway(hash::crc16(data, smallSize));
  });

  bench.run("bitwiseCrc16", [&] {
    benchmark::doNotOptimizeAway(bitwiseCrc16(data, smallSize));
  });
}

BENCHMARK_CASE("core/hash/crc16/compute_large") {
  const uint8_t * data = opaque(buffer.data());

  bench.unit("checksum");

  bench.run("hash::crc16", [&] {
    benchmark::doNotOptimizeAway(hash::crc16(data, largeSize));
  });

  bench.run("bitwiseCrc16", [&] {
    benchmark::doNotOptimizeAway(bitwiseCrc16(data, largeSize));
  });
}

BENCHMARK_CASE("core/hash/crc32/compute") {
  const uint8_t * data = opaque(buffer.data());

  bench.unit("checksum");

  bench.run("hash::crc32", [&] {
    benchmark::doNotOptimizeAway(hash::crc32(data, smallSize));
  });

  bench.run("bitwiseCrc32", [&] {
    benchmark::doNotOptimizeAway(bitwiseCrc32(data, smallSize));
  });
}

BENCHMARK_CASE("core/hash/crc32/compute_large") {
  const uint8_t * data = opaque(buffer.data());

  bench.unit("checksum");

  bench.run("hash::crc32", [&] {
    benchmark::doNotOptimizeAway(hash::crc32(data, largeSize));
  });

  bench.run("bitwiseCrc32", [&] {
    benchmark::doNotOptimizeAway(bitwiseCrc32(data, largeSize));
  });
}

} // namespace toy
