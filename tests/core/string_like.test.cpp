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
  \file   string_like.test.cpp
  \brief  Unit tests for \ref toy::StringLike.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// The types below exist for the concept checks alone, so their members are declared and never defined.

// Minimal type that meets both requirements on a const reference.
struct FakeString {
  size_t       size() const noexcept;
  const char * c_str() const noexcept;
};

// c_str() returns char *, which converts to const char *.
struct MutableBufferString {
  size_t size() const noexcept;
  char * c_str() const noexcept;
};

// Has the length, but no pointer to the characters.
struct NoCStrString {
  size_t size() const noexcept;
};

// Has the pointer, but no length.
struct NoSizeString {
  const char * c_str() const noexcept;
};

// Length as int: converts to size_t, but is not size_t.
struct IntSizeString {
  int          size() const noexcept;
  const char * c_str() const noexcept;
};

// Both members present, neither callable on a const object.
struct NonConstString {
  size_t       size() noexcept;
  const char * c_str() noexcept;
};

} // namespace

// Concept StringLike: which types the requirement expression accepts and which it leaves out.
TEST_CASE("string_like/requirements") {
  static_assert(StringLike<FakeString>,
                "a const size() returning size_t and c_str() returning const char * satisfy StringLike");
  static_assert(StringLike<MutableBufferString>,
                "c_str() returning char * must convert to const char * and satisfy StringLike");

  static_assert(!StringLike<NoCStrString>, "a type without c_str() must not satisfy StringLike");
  static_assert(!StringLike<NoSizeString>, "a type without size() must not satisfy StringLike");

  static_assert(!StringLike<IntSizeString>, "size() returning int must not satisfy StringLike");
  static_assert(!StringLike<NonConstString>, "members callable only on a non-const object must not satisfy StringLike");

  static_assert(!StringLike<const char *>, "const char * carries no length and must not satisfy StringLike");
  static_assert(!StringLike<int>, "a non-class type must not satisfy StringLike");
}

} // namespace toy
