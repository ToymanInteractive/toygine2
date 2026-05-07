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
  \file   string_fixed_storage.test.cpp
  \brief  Unit tests for \ref toy::StringFixedStorage.
*/

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

// Check object structure.
TEST_CASE("string_fixed_storage/object_structure") {
  static_assert(!std::is_trivial_v<StringFixedStorage<8>>,
                "StringFixedStorage must not be trivial (has non-trivial default init)");
  static_assert(std::is_trivially_copyable_v<StringFixedStorage<8>>, "StringFixedStorage must be trivially copyable");
  static_assert(std::is_trivially_destructible_v<StringFixedStorage<8>>,
                "StringFixedStorage must be trivially destructible");
  static_assert(std::is_standard_layout_v<StringFixedStorage<8>>, "StringFixedStorage must have standard layout");
}

// capacity() is allocatedSize minus one (room for null terminator).
TEST_CASE("string_fixed_storage/capacity") {
  REQUIRE(StringFixedStorage<1>::capacity() == 0);
  REQUIRE(StringFixedStorage<32>::capacity() == 31);
  REQUIRE(StringFixedStorage<256>::capacity() == 255);

  static_assert(StringFixedStorage<1>::capacity() == 0, "capacity of size-1 buffer must be zero characters");
  static_assert(StringFixedStorage<32>::capacity() == 31, "capacity must be allocatedSize minus one");
  static_assert(StringFixedStorage<256>::capacity() == 255, "capacity must be allocatedSize minus one");
}

// Default construction yields zero size and null-terminated empty buffer.
TEST_CASE("string_fixed_storage/default_state") {
  constexpr StringFixedStorage<8> s;

  REQUIRE(StringFixedStorage<8>::capacity() == 7);
  REQUIRE(s.size() == 0);
  REQUIRE(s.data()[0] == '\0');

  static_assert(StringFixedStorage<8>::capacity() == 7, "default capacity must be allocatedSize minus one");
  static_assert(s.size() == 0, "default size must be zero");
  static_assert(s.data()[0] == '\0', "default data must be null-terminated");
}

// setSize updates length and places null terminator at data()[size].
TEST_CASE("string_fixed_storage/set_size") {
  StringFixedStorage<8> s;

  s.data()[0] = 'a';
  s.data()[1] = 'b';
  s.data()[2] = 'c';
  s.setSize(3);

  REQUIRE(s.size() == 3);
  REQUIRE(std::strcmp(s.data(), "abc") == 0);
  REQUIRE(s.data()[3] == '\0');
}

// setSize(0) yields empty C string.
TEST_CASE("string_fixed_storage/set_size_empty") {
  StringFixedStorage<8> s;

  s.data()[0] = 'x';
  s.setSize(1);
  REQUIRE(s.size() == 1);
  REQUIRE(std::strcmp(s.data(), "x") == 0);

  s.setSize(0);
  REQUIRE(s.size() == 0);
  REQUIRE(std::strcmp(s.data(), "") == 0);
}

// data() is stable and supports filling up to capacity characters.
TEST_CASE("string_fixed_storage/fill_to_capacity") {
  constexpr size_t                         capacity = 4;
  StringFixedStorage<capacity + 1>         s;
  const StringFixedStorage<capacity + 1> & cs = s;

  for (size_t i = 0; i < capacity; ++i)
    s.data()[i] = static_cast<char>('a' + static_cast<int>(i));

  s.setSize(capacity);

  REQUIRE(s.size() == capacity);
  REQUIRE(cs.size() == capacity);
  REQUIRE(s.data()[capacity] == '\0');
  REQUIRE(cs.data()[capacity] == '\0');
  REQUIRE(std::strlen(s.data()) == capacity);
  REQUIRE(std::strlen(cs.data()) == capacity);
}

} // namespace toy
