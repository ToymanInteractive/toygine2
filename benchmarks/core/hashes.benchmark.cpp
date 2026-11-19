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

// Hands a view through an opaque barrier, so a measured call reads the buffer instead of a folded constant.
[[nodiscard]] toy::span<const std::byte> opaque(toy::span<const std::byte> data) noexcept {
  toy::benchmark::doNotOptimizeAway(data);

  return data;
}

// The bitwise loop toy::hash::crc8() tabulates: the reflected polynomial 0x8C applied eight times per byte.
[[nodiscard]] uint8_t bitwiseCrc8(toy::span<const std::byte> data, uint8_t crc = 0x00) noexcept {
  const std::byte * unit = data.data();
  size_t            size = data.size();

  while (size > 0) {
    --size;
    crc ^= std::to_integer<uint8_t>(*unit++);

    for (unsigned bit = 0; bit < 8; ++bit)
      crc = static_cast<uint8_t>((crc >> 1) ^ ((crc & 1U) != 0U ? 0x8CU : 0U));
  }

  return crc;
}

// The bitwise loop toy::hash::crc16() tabulates: the reflected polynomial 0xA001 applied eight times per byte.
[[nodiscard]] uint16_t bitwiseCrc16(toy::span<const std::byte> data, uint16_t crc = 0x0000) noexcept {
  const std::byte * unit = data.data();
  size_t            size = data.size();

  while (size > 0) {
    --size;
    crc = static_cast<uint16_t>(crc ^ std::to_integer<uint8_t>(*unit++));

    for (unsigned bit = 0; bit < 8; ++bit)
      crc = static_cast<uint16_t>((crc >> 1) ^ ((crc & 1U) != 0U ? 0xA001U : 0U));
  }

  return crc;
}

// The bitwise loop toy::hash::crc32() tabulates: the reflected polynomial 0xEDB88320 behind a complemented state.
[[nodiscard]] uint32_t bitwiseCrc32(toy::span<const std::byte> data, uint32_t crc = 0x00000000) noexcept {
  const std::byte * unit = data.data();
  size_t            size = data.size();

  crc = ~crc;

  while (size > 0) {
    --size;
    crc ^= std::to_integer<uint8_t>(*unit++);

    for (unsigned bit = 0; bit < 8; ++bit)
      crc = (crc >> 1) ^ ((crc & 1U) != 0U ? 0xEDB88320U : 0U);
  }

  return ~crc;
}

} // namespace

BENCHMARK_CASE("core/hash/crc8/compute") {
  const toy::span<const std::byte> data = opaque(std::as_bytes(toy::span{buffer})).first(smallSize);

  bench.unit("checksum");

  bench.run("toy::hash::crc8", [&] {
    toy::benchmark::doNotOptimizeAway(toy::hash::crc8(data));
  });

  bench.run("bitwiseCrc8", [&] {
    toy::benchmark::doNotOptimizeAway(bitwiseCrc8(data));
  });
}

BENCHMARK_CASE("core/hash/crc8/compute_large") {
  const toy::span<const std::byte> data = opaque(std::as_bytes(toy::span{buffer}));

  bench.unit("checksum");

  bench.run("toy::hash::crc8", [&] {
    toy::benchmark::doNotOptimizeAway(toy::hash::crc8(data));
  });

  bench.run("bitwiseCrc8", [&] {
    toy::benchmark::doNotOptimizeAway(bitwiseCrc8(data));
  });
}

BENCHMARK_CASE("core/hash/crc16/compute") {
  const toy::span<const std::byte> data = opaque(std::as_bytes(toy::span{buffer})).first(smallSize);

  bench.unit("checksum");

  bench.run("toy::hash::crc16", [&] {
    toy::benchmark::doNotOptimizeAway(toy::hash::crc16(data));
  });

  bench.run("bitwiseCrc16", [&] {
    toy::benchmark::doNotOptimizeAway(bitwiseCrc16(data));
  });
}

BENCHMARK_CASE("core/hash/crc16/compute_large") {
  const toy::span<const std::byte> data = opaque(std::as_bytes(toy::span{buffer}));

  bench.unit("checksum");

  bench.run("toy::hash::crc16", [&] {
    toy::benchmark::doNotOptimizeAway(toy::hash::crc16(data));
  });

  bench.run("bitwiseCrc16", [&] {
    toy::benchmark::doNotOptimizeAway(bitwiseCrc16(data));
  });
}

BENCHMARK_CASE("core/hash/crc32/compute") {
  const toy::span<const std::byte> data = opaque(std::as_bytes(toy::span{buffer})).first(smallSize);

  bench.unit("checksum");

  bench.run("toy::hash::crc32", [&] {
    toy::benchmark::doNotOptimizeAway(toy::hash::crc32(data));
  });

  bench.run("bitwiseCrc32", [&] {
    toy::benchmark::doNotOptimizeAway(bitwiseCrc32(data));
  });
}

BENCHMARK_CASE("core/hash/crc32/compute_large") {
  const toy::span<const std::byte> data = opaque(std::as_bytes(toy::span{buffer}));

  bench.unit("checksum");

  bench.run("toy::hash::crc32", [&] {
    toy::benchmark::doNotOptimizeAway(toy::hash::crc32(data));
  });

  bench.run("bitwiseCrc32", [&] {
    toy::benchmark::doNotOptimizeAway(bitwiseCrc32(data));
  });
}

} // namespace toy
