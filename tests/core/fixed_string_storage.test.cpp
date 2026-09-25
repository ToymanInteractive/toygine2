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
constexpr size_t       c_sampleLength = std::char_traits<char>::length(c_sample);

// Length a shortened storage keeps, short enough to leave sample characters behind the new terminator.
constexpr size_t c_shortLength = 2;

// Byte no operation writes, so whatever setSize() puts down shows up against it.
constexpr char c_sentinel = '#';

// Buffer size above every target's c_inlineCopyMaxBytes, so assignment copies only the characters in use.
constexpr size_t c_largeAllocatedSize = 256;

using LargeStorage = FixedStringStorage<c_largeAllocatedSize>;

static_assert(sizeof(Storage) <= platform::c_inlineCopyMaxBytes,
              "Storage must copy its whole object on every target, so the assignment case reaches that path");
static_assert(sizeof(LargeStorage) > platform::c_inlineCopyMaxBytes,
              "LargeStorage must copy by length on every target, so the assignment case reaches that path");

// Writes the sentinel into every byte of the buffer, so no byte a test compares is left indeterminate.
template <typename StorageType>
constexpr void fillWithSentinel(StorageType & storage) noexcept {
  for (size_t index = 0; index <= storage.capacity(); ++index)
    storage.data()[index] = c_sentinel;
}

// Storage holding the sample over a sentinel-filled buffer, the state a constant expression reads back.
template <typename StorageType = Storage>
[[nodiscard]] constexpr StorageType sampleStorage() noexcept {
  StorageType storage;
  fillWithSentinel(storage);

  for (size_t index = 0; index < c_sampleLength; ++index)
    storage.data()[index] = c_sample[index];
  storage.setSize(c_sampleLength);

  return storage;
}

// Storage whose every writable byte carries the sentinel; a length-based copy of it would drop them, so it stays small.
[[nodiscard]] constexpr Storage sentinelStorage() noexcept {
  Storage storage;
  fillWithSentinel(storage);

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

// Whether every byte of a default-constructed buffer is a null character, as constant evaluation guarantees.
[[nodiscard]] constexpr bool defaultBufferIsNull() noexcept {
  const Storage storage;

  return std::ranges::all_of(storage.data(), storage.data() + c_allocatedSize, [](char character) {
    return character == '\0';
  });
}

// First index where a storage differs from the sample and its terminator; c_sampleLength + 1 on a full match.
template <typename StorageType>
[[nodiscard]] constexpr size_t sampleDifference(const StorageType & storage) noexcept {
  if (storage.size() != c_sampleLength)
    return 0;

  size_t index = 0;
  while (index < c_sampleLength && storage.data()[index] == c_sample[index])
    ++index;

  if (index == c_sampleLength && storage.data()[c_sampleLength] == '\0')
    ++index;

  return index;
}

// Sentinel-filled storage after a copy assignment of the sample.
template <typename StorageType>
[[nodiscard]] constexpr StorageType copyAssigned() noexcept {
  const StorageType source = sampleStorage<StorageType>();
  StorageType       target;
  fillWithSentinel(target);
  target = source;

  return target;
}

// Sentinel-filled storage after a move assignment of the sample.
template <typename StorageType>
[[nodiscard]] constexpr StorageType moveAssigned() noexcept {
  StorageType source = sampleStorage<StorageType>();
  StorageType target;
  fillWithSentinel(target);
  target = std::move(source);

  return target;
}

// Sample storage after an assignment to itself through an alias.
template <typename StorageType>
[[nodiscard]] constexpr StorageType selfAssigned() noexcept {
  StorageType         storage = sampleStorage<StorageType>();
  const StorageType & alias   = storage;
  storage                     = alias;

  return storage;
}

// Storage filled to capacity with an alphabet cycle, long enough that a copy of it takes the length-based path.
template <typename StorageType>
constexpr void fillToCapacity(StorageType & storage) noexcept {
  for (size_t index = 0; index < storage.capacity(); ++index)
    storage.data()[index] = static_cast<char>('a' + (index % 26));
  storage.setSize(storage.capacity());
}

// Index of the first byte where a storage differs from the alphabet cycle at capacity, capacity() + 1 on a full match.
template <typename StorageType>
[[nodiscard]] constexpr size_t fullDifference(const StorageType & storage) noexcept {
  if (storage.size() != storage.capacity())
    return 0;

  size_t index = 0;
  while (index < storage.capacity() && storage.data()[index] == static_cast<char>('a' + (index % 26)))
    ++index;

  if (index == storage.capacity() && storage.data()[index] == '\0')
    ++index;

  return index;
}

// Byte past the sample's terminator in a sentinel-filled storage after a copy assignment of the sample.
template <typename StorageType>
[[nodiscard]] constexpr char byteAfterCopyAssignment() noexcept {
  const StorageType source = sampleStorage<StorageType>();
  StorageType       target;
  fillWithSentinel(target);
  target = source;

  return target.data()[c_sampleLength + 1];
}

// Where a full storage, copy-assigned over a shorter one, first differs from the alphabet cycle.
template <typename StorageType>
[[nodiscard]] constexpr size_t fullCopyAssignedDifference() noexcept {
  StorageType source;
  fillToCapacity(source);

  StorageType target = sampleStorage<StorageType>();
  target             = source;

  return fullDifference(target);
}

// Where a copy of a full storage first differs from the alphabet cycle.
template <typename StorageType>
[[nodiscard]] constexpr size_t fullCopyConstructedDifference() noexcept {
  StorageType source;
  fillToCapacity(source);

  const StorageType copy(source);

  return fullDifference(copy);
}

// Where a copy of the sample storage first differs from the sample.
template <typename StorageType>
[[nodiscard]] constexpr size_t copyConstructedDifference() noexcept {
  const StorageType source = sampleStorage<StorageType>();
  const StorageType copy(source);

  return sampleDifference(copy);
}

// Where a storage moved from the sample storage first differs from the sample.
template <typename StorageType>
[[nodiscard]] constexpr size_t moveConstructedDifference() noexcept {
  StorageType       source = sampleStorage<StorageType>();
  const StorageType moved(std::move(source));

  return sampleDifference(moved);
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
                "a default-constructed buffer writes its terminator, which is not a trivial default");

  static_assert(!std::is_trivially_copyable_v<LargeStorage>,
                "a large buffer must copy only the characters in use on assignment, which is not a trivial copy");
  static_assert(!std::is_trivially_copy_constructible_v<LargeStorage>,
                "a large buffer must copy only the characters in use on construction as well");
  static_assert(std::is_nothrow_copy_constructible_v<LargeStorage>
                  && std::is_nothrow_move_constructible_v<LargeStorage>,
                "a length-based copy construction must not throw");
  static_assert(std::is_nothrow_copy_assignable_v<LargeStorage> && std::is_nothrow_move_assignable_v<LargeStorage>,
                "a length-based assignment must not throw");
}

// What a storage reports before anything is written into it.
TEST_CASE("fixed_string_storage/default_state") {
  const Storage storage;

  CHECK(storage.size() == 0);
  CHECK(storage.data() != nullptr);
  CHECK(storage.data()[0] == '\0');

  static_assert(Storage().size() == 0, "a default-constructed storage holds no character");
  static_assert(Storage().data()[0] == '\0', "the buffer must read as an empty string before the first setSize()");
  static_assert(defaultBufferIsNull(), "constant evaluation must leave no byte of the buffer indeterminate");
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

// Assignments reproduce the source over both copy paths; the length-based one keeps the bytes past the terminator.
TEST_CASE("fixed_string_storage/assignment") {
  CHECK(sampleDifference(copyAssigned<Storage>()) == c_sampleLength + 1);
  CHECK(sampleDifference(copyAssigned<LargeStorage>()) == c_sampleLength + 1);
  CHECK(sampleDifference(moveAssigned<Storage>()) == c_sampleLength + 1);
  CHECK(sampleDifference(moveAssigned<LargeStorage>()) == c_sampleLength + 1);
  CHECK(sampleDifference(selfAssigned<Storage>()) == c_sampleLength + 1);
  CHECK(sampleDifference(selfAssigned<LargeStorage>()) == c_sampleLength + 1);

  CHECK(fullCopyAssignedDifference<LargeStorage>() == c_largeAllocatedSize);
  CHECK(byteAfterCopyAssignment<LargeStorage>() == c_sentinel);

  static_assert(sampleDifference(copyAssigned<Storage>()) == c_sampleLength + 1,
                "a whole-object copy assignment must reproduce the source");
  static_assert(sampleDifference(copyAssigned<LargeStorage>()) == c_sampleLength + 1,
                "a length-based copy assignment must reproduce the source and its terminator");
  static_assert(sampleDifference(moveAssigned<LargeStorage>()) == c_sampleLength + 1,
                "a length-based move assignment must reproduce the source");
  static_assert(sampleDifference(selfAssigned<LargeStorage>()) == c_sampleLength + 1,
                "a length-based self-assignment must keep the contents");
  static_assert(fullCopyAssignedDifference<LargeStorage>() == c_largeAllocatedSize,
                "a full string must be copied by its length, terminator included");
  static_assert(byteAfterCopyAssignment<LargeStorage>() == c_sentinel,
                "a length-based copy assignment must leave the bytes past the terminator as they were");
}

// Copy and move construction reproduce the source's characters, over a buffer copied whole and one copied by length.
TEST_CASE("fixed_string_storage/copy_construction") {
  CHECK(copyConstructedDifference<Storage>() == c_sampleLength + 1);
  CHECK(copyConstructedDifference<LargeStorage>() == c_sampleLength + 1);
  CHECK(moveConstructedDifference<Storage>() == c_sampleLength + 1);
  CHECK(moveConstructedDifference<LargeStorage>() == c_sampleLength + 1);
  CHECK(fullCopyConstructedDifference<LargeStorage>() == c_largeAllocatedSize);

  static_assert(copyConstructedDifference<Storage>() == c_sampleLength + 1,
                "a whole-object copy construction must reproduce the source");
  static_assert(copyConstructedDifference<LargeStorage>() == c_sampleLength + 1,
                "a length-based copy construction must reproduce the source and its terminator");
  static_assert(moveConstructedDifference<LargeStorage>() == c_sampleLength + 1,
                "a length-based move construction must reproduce the source");
  static_assert(fullCopyConstructedDifference<LargeStorage>() == c_largeAllocatedSize,
                "a full string must be copied by its length, terminator included");
}

// The storage contract a string constrains on, and which buffers meet it.
TEST_CASE("fixed_string_storage/concept") {
  static_assert(StringStorage<Storage>, "the buffer must satisfy the storage concept a string constrains on");
  static_assert(StringStorage<Storage &>, "a reference is stripped from T, so a reference must satisfy it too");
  static_assert(StringStorage<FixedStringStorage<1>>, "the smallest buffer must satisfy the concept as well");
}

} // namespace toy
