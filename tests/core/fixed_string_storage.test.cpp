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
  \file   fixed_string_storage.test.cpp
  \brief  Unit tests for \ref toy::FixedStringStorage.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// Buffer size every case builds on, wide enough to leave bytes past the sample.
constexpr size_t c_allocatedSize = 16;

using Storage = FixedStringStorage<c_allocatedSize>;

// The literal the cases write. Its length is measured from the literal, not written out as a constant.
constexpr const char * c_sample       = "player";
constexpr size_t       c_sampleLength = char_traits<char>::length(c_sample);

// Length a shortened storage keeps, short enough to leave sample characters behind the new terminator.
constexpr size_t c_shortLength = 2;

// Byte no operation writes, so whatever setSize() puts down shows up against it.
constexpr char c_sentinel = '#';

// Storage holding the sample, the state a constant expression reads back.
[[nodiscard]] constexpr Storage sampleStorage() noexcept {
  Storage storage;

  for (size_t index = 0; index < c_sampleLength; ++index)
    storage.data()[index] = c_sample[index];
  storage.setSize(c_sampleLength);

  return storage;
}

// Storage whose every writable byte carries the sentinel, so a terminator reads against a known character.
[[nodiscard]] constexpr Storage sentinelStorage() noexcept {
  Storage storage;

  for (size_t index = 0; index < storage.capacity(); ++index)
    storage.data()[index] = c_sentinel;

  return storage;
}

// Index of the first byte where two buffers differ, c_allocatedSize when they hold the same bytes throughout.
[[nodiscard]] constexpr size_t firstDifference(const Storage & left, const Storage & right) noexcept {
  size_t index = 0;
  while (index < c_allocatedSize && left.data()[index] == right.data()[index])
    ++index;

  return index;
}

// What a reserve() call answered, and where the buffer first differs from its state before the call.
struct ReserveResult {
  bool   answer;
  size_t difference;
};

// Both halves of one reserve() call, so a constant expression reads the answer and the bytes from the same call.
[[nodiscard]] constexpr ReserveResult reserveResult(size_t newCapacity) noexcept {
  Storage       storage = sampleStorage();
  const Storage before  = storage;

  return {storage.reserve(newCapacity), firstDifference(storage, before)};
}

// Character left at the length setSize() was given, over a buffer that held no null character there.
[[nodiscard]] constexpr char terminatorAfter(size_t newSize) noexcept {
  Storage storage = sentinelStorage();
  storage.setSize(newSize);

  return storage.data()[newSize];
}

// Byte at an index past a shortened length, which setSize() has no business clearing.
[[nodiscard]] constexpr char byteAfterShrink(size_t index) noexcept {
  Storage storage = sampleStorage();
  storage.setSize(c_shortLength);

  return storage.data()[index];
}

// Storage the compile-time assertions read, so a constant expression names one instead of rebuilding it each time.
constexpr Storage c_sampleStorage = sampleStorage();

} // namespace

// What a copy and a scope exit cost the caller, the initializer form the call site may write, and what a
// declaration without one leaves behind.
TEST_CASE("fixed_string_storage/value_semantics") {
  static_assert(std::is_trivially_copyable_v<Storage>, "the buffer must copy as bytes");
  static_assert(std::is_trivially_destructible_v<Storage>, "leaving scope must run no destructor");
  static_assert(std::is_standard_layout_v<Storage>, "the layout must stay readable from a C interface");

  static_assert(!std::is_aggregate_v<Storage>,
                "the bytes and the length stay private, so no call site may brace-initialize them");
  static_assert(!std::is_trivially_default_constructible_v<Storage>,
                "a default-constructed buffer fills itself with null characters, which is not a trivial default");
}

// What a storage reports before anything is written into it.
TEST_CASE("fixed_string_storage/default_state") {
  const Storage storage;

  CHECK(storage.size() == 0);
  CHECK(storage.data() != nullptr);
  CHECK(storage.data()[0] == '\0');

  // Every byte starts as a null character, not only the one the terminator sits on.
  size_t firstNonNull = 0;
  while (firstNonNull <= storage.capacity() && storage.data()[firstNonNull] == '\0')
    ++firstNonNull;
  CHECK(firstNonNull == storage.capacity() + 1);

  static_assert(Storage().size() == 0, "a default-constructed storage holds no character");
  static_assert(Storage().data()[0] == '\0', "the buffer must read as an empty string before the first setSize()");
}

// The bound a length may not cross, and the byte the terminator claims out of the buffer.
TEST_CASE("fixed_string_storage/capacity") {
  const Storage storage;

  CHECK(storage.capacity() == c_allocatedSize - 1);

  static_assert(Storage().capacity() == c_allocatedSize - 1, "capacity must be the buffer size less the terminator");
  static_assert(FixedStringStorage<1>().capacity() == 0, "the smallest buffer holds the terminator alone");
  static_assert(FixedStringStorage<256>().capacity() == 255, "capacity must follow AllocatedSize");
}

// Which bytes the two pointer overloads reach, and what a caller reads back after writing through the mutable one.
TEST_CASE("fixed_string_storage/data_access") {
  Storage         storage;
  const Storage & constStorage = storage;

  CHECK(constStorage.data() == storage.data());

  storage.data()[0] = c_sample[0];
  CHECK(constStorage.data()[0] == c_sample[0]);

  // Writing characters leaves the length where it was: the two move apart.
  CHECK(storage.size() == 0);

  static_assert(c_sampleStorage.data()[0] == c_sample[0], "the const overload must read the characters written in");
  static_assert(c_sampleStorage.data()[c_sampleLength - 1] == c_sample[c_sampleLength - 1],
                "every written character must read back");
}

// Which lengths the buffer admits, and what asking leaves behind.
TEST_CASE("fixed_string_storage/reserve") {
  Storage       storage = sampleStorage();
  const Storage before  = storage;

  CHECK(storage.reserve(c_allocatedSize - 1));
  CHECK(firstDifference(storage, before) == c_allocatedSize);

  CHECK_FALSE(storage.reserve(c_allocatedSize));
  CHECK(firstDifference(storage, before) == c_allocatedSize);

  // Asking changes nothing: the buffer is fixed, so neither answer moves the length or the bound.
  CHECK(storage.size() == c_sampleLength);
  CHECK(storage.capacity() == c_allocatedSize - 1);

  static_assert(reserveResult(0).answer, "an empty string must fit any buffer");
  static_assert(reserveResult(c_allocatedSize - 1).answer, "a length at capacity must fit");
  static_assert(!reserveResult(c_allocatedSize).answer, "a length past capacity must be refused, terminator included");

  static_assert(reserveResult(c_allocatedSize - 1).difference == c_allocatedSize,
                "an accepted length must leave every byte of the buffer as it was");
  static_assert(reserveResult(c_allocatedSize).difference == c_allocatedSize,
                "a refused length must leave every byte of the buffer as it was");
}

// What setSize() records, and where it puts the terminator.
TEST_CASE("fixed_string_storage/set_size") {
  Storage storage = sampleStorage();

  CHECK(storage.size() == c_sampleLength);
  CHECK(storage.data()[c_sampleLength] == '\0');

  // The length may reach capacity, where the terminator takes the last byte of the buffer.
  storage.setSize(storage.capacity());
  CHECK(storage.size() == c_allocatedSize - 1);
  CHECK(storage.data()[c_allocatedSize - 1] == '\0');

  static_assert(sampleStorage().size() == c_sampleLength, "setSize() must record the length it was given");
  static_assert(terminatorAfter(0) == '\0', "a zero length must terminate the buffer at its first byte");
  static_assert(terminatorAfter(c_allocatedSize - 1) == '\0', "a length at capacity must terminate the last byte");
}

// Which characters a shorter length leaves behind it.
TEST_CASE("fixed_string_storage/set_size_shrink") {
  Storage storage = sampleStorage();

  storage.setSize(c_shortLength);
  CHECK(storage.size() == c_shortLength);
  CHECK(storage.data()[c_shortLength] == '\0');

  // The characters past the new terminator stay as they were: the call clears nothing.
  CHECK(storage.data()[c_shortLength + 1] == c_sample[c_shortLength + 1]);
  CHECK(storage.data()[c_sampleLength - 1] == c_sample[c_sampleLength - 1]);

  static_assert(byteAfterShrink(c_shortLength) == '\0', "the terminator must move to the new length");
  static_assert(byteAfterShrink(c_shortLength + 1) == c_sample[c_shortLength + 1],
                "a character past the new length must survive the call");
}

} // namespace toy
