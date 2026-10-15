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
  \file   string_storage.test.cpp
  \brief  Unit tests for \ref toy::StringStorage.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// The types below exist for the concept checks alone, so their members are declared and never defined.

// Minimal type that meets every requirement.
struct FakeStorage {
  char *       data() noexcept;
  const char * data() const noexcept;
  size_t       size() const noexcept;
  size_t       capacity() const noexcept;
  size_t       max_size() const noexcept;
  bool         reserve(size_t newCapacity) noexcept;
  void         shrink_to_fit() noexcept;
  void         setSize(size_t newSize) noexcept;
};

// Every member present, but built only from a buffer size.
struct NoDefaultStorage : FakeStorage {
  explicit NoDefaultStorage(size_t bufferSize) noexcept;
};

// Every member present, but neither copyable nor movable.
struct NonMovableStorage : FakeStorage {
  NonMovableStorage() noexcept                                   = default;
  NonMovableStorage(const NonMovableStorage & other)             = delete;
  NonMovableStorage(NonMovableStorage && other)                  = delete;
  NonMovableStorage & operator=(const NonMovableStorage & other) = delete;
  NonMovableStorage & operator=(NonMovableStorage && other)      = delete;
  ~NonMovableStorage() noexcept                                  = default;
};

// Every member present and movable, but not copyable.
struct MoveOnlyStorage : FakeStorage {
  MoveOnlyStorage() noexcept                                     = default;
  MoveOnlyStorage(const MoveOnlyStorage & other)                 = delete;
  MoveOnlyStorage(MoveOnlyStorage && other) noexcept             = default;
  MoveOnlyStorage & operator=(const MoveOnlyStorage & other)     = delete;
  MoveOnlyStorage & operator=(MoveOnlyStorage && other) noexcept = default;
  ~MoveOnlyStorage() noexcept                                    = default;
};

} // namespace

// Concept StringStorage: the construction and move support it asks for beyond the eight members.
TEST_CASE("string_storage/object_requirements") {
  static_assert(StringStorage<FakeStorage>,
                "the eight members on a default-constructible, movable type must satisfy it");
  static_assert(StringStorage<FakeStorage &>, "a reference is stripped from T, so a reference must satisfy it too");

  static_assert(!StringStorage<NoDefaultStorage>, "a storage without a default constructor must not satisfy it");
  static_assert(!StringStorage<NonMovableStorage>, "a storage that cannot be moved must not satisfy it");

  static_assert(StringStorage<MoveOnlyStorage>, "copying is not required, so a move-only storage must satisfy it");
  static_assert(!std::copy_constructible<String<MoveOnlyStorage>>,
                "a string over a storage that cannot be copied must not be copyable either");
  static_assert(std::move_constructible<String<MoveOnlyStorage>>,
                "a string over a move-only storage must still be movable");
}

} // namespace toy
