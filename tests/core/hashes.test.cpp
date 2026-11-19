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
  \file   hashes.test.cpp
  \brief  Unit tests for the toy::hash::crc8(), toy::hash::crc16() and toy::hash::crc32() checksums.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy::hash {

namespace {

// The check sequence the CRC catalog defines; the sums below are the fixed catalog values over it.
constexpr const char * checkData = "123456789";
constexpr size_t       checkSize = std::char_traits<char>::length(checkData);

// Bytes stepping five apart wrap the whole byte range, so the sums meet both extremes, 0x00 and 0xFF.
constexpr size_t chunkSize = 64;

constexpr toy::array<uint8_t, chunkSize> chunkData = [] {
  toy::array<uint8_t, chunkSize> bytes{};
  for (size_t index = 0; index < chunkSize; ++index)
    bytes[index] = static_cast<uint8_t>(index * 5);
  return bytes;
}();

} // namespace

// The sums live in another translation unit, so the compile-time contract is only their exception specification.
static_assert(noexcept(crc8(checkData, 0)), "crc8 must stay noexcept");
static_assert(noexcept(crc16(checkData, 0)), "crc16 must stay noexcept");
static_assert(noexcept(crc32(checkData, 0)), "crc32 must stay noexcept");

// CRC-8 over the reflected polynomial 0x31, the Dallas one-wire sum; the CRC catalog fixes its check value at 0xA1.
TEST_CASE("hash/crc8/computes_the_catalog_check_value") {
  CHECK(crc8(checkData, checkSize) == 0xA1);
  CHECK(crc8(checkData, checkSize, 0x00) == 0xA1);
}

// CRC-16 over the reflected polynomial 0x8005, the ARC sum; the CRC catalog fixes its check value at 0xBB3D.
TEST_CASE("hash/crc16/computes_the_catalog_check_value") {
  CHECK(crc16(checkData, checkSize) == 0xBB3D);
  CHECK(crc16(checkData, checkSize, 0x0000) == 0xBB3D);
}

// CRC-32 over the reflected polynomial 0xEDB88320, the zlib sum; the CRC catalog fixes its check value at 0xCBF43926.
TEST_CASE("hash/crc32/computes_the_catalog_check_value") {
  CHECK(crc32(checkData, checkSize) == 0xCBF43926);
  CHECK(crc32(checkData, checkSize, 0x00000000) == 0xCBF43926);
}

// An empty range must not disturb a running sum, so a tail of length zero leaves the caller's value alone.
TEST_CASE("hash/crc8/returns_the_initial_value_for_an_empty_range") {
  CHECK(crc8(checkData, 0) == 0x00);
  CHECK(crc8(checkData, 0, 0x5A) == 0x5A);
}

// No complement here: the running value stays as the caller handed it when no byte arrives.
TEST_CASE("hash/crc16/returns_the_initial_value_for_an_empty_range") {
  CHECK(crc16(checkData, 0) == 0x0000);
  CHECK(crc16(checkData, 0, 0x5A5A) == 0x5A5A);
}

// The complemented running state cancels over no bytes, so the value round-trips untouched.
TEST_CASE("hash/crc32/returns_the_initial_value_for_an_empty_range") {
  CHECK(crc32(checkData, 0) == 0x00000000);
  CHECK(crc32(checkData, 0, 0x5A5A5A5A) == 0x5A5A5A5A);
}

// Feeding the pattern in two chunks must land on the whole-pattern sum at every split point, the empty ones included.
TEST_CASE("hash/crc8/continues_a_checksum_across_chunks") {
  const uint8_t whole = crc8(chunkData.data(), chunkSize);

  for (size_t split = 0; split <= chunkSize; ++split) {
    INFO("split at ", split);
    const uint8_t head = crc8(chunkData.data(), split);
    CHECK(crc8(chunkData.data() + split, chunkSize - split, head) == whole);
  }
}

// The wider running state composes the same: the chained calls must land on the whole-pattern sum at every split.
TEST_CASE("hash/crc16/continues_a_checksum_across_chunks") {
  const uint16_t whole = crc16(chunkData.data(), chunkSize);

  for (size_t split = 0; split <= chunkSize; ++split) {
    INFO("split at ", split);
    const uint16_t head = crc16(chunkData.data(), split);
    CHECK(crc16(chunkData.data() + split, chunkSize - split, head) == whole);
  }
}

// The complemented running state composes the same: chained calls must land on the whole-pattern sum at every split.
TEST_CASE("hash/crc32/continues_a_checksum_across_chunks") {
  const uint32_t whole = crc32(chunkData.data(), chunkSize);

  for (size_t split = 0; split <= chunkSize; ++split) {
    INFO("split at ", split);
    const uint32_t head = crc32(chunkData.data(), split);
    CHECK(crc32(chunkData.data() + split, chunkSize - split, head) == whole);
  }
}

} // namespace toy::hash
