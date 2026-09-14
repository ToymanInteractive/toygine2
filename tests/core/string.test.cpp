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
  \file   string.test.cpp
  \brief  Unit tests for \ref toy::String.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// Buffer size every case builds on.
constexpr size_t c_allocatedSize = 16;

using Fixed = FixedString<c_allocatedSize>;

// Length a default-constructed string reports, read in a constant expression.
[[nodiscard]] constexpr size_t defaultSize() noexcept {
  const Fixed string;

  return string.size();
}

// Character c_str() of a default-constructed string points at, read in a constant expression.
[[nodiscard]] constexpr char defaultFirstCharacter() noexcept {
  const Fixed string;

  return *string.c_str();
}

// Length a copy of a default-constructed string reports, read in a constant expression.
[[nodiscard]] constexpr size_t copiedSize() noexcept {
  const Fixed source;
  const Fixed copy(source);

  return copy.size();
}

} // namespace

// What a default-constructed string holds: no characters and a terminator behind c_str().
TEST_CASE("string/construction") {
  const Fixed string;

  CHECK(string.size() == 0);
  REQUIRE(string.c_str() != nullptr);
  CHECK(*string.c_str() == '\0');

  static_assert(defaultSize() == 0, "a default-constructed string must hold no characters");
  static_assert(defaultFirstCharacter() == '\0', "c_str() of a default-constructed string must read as empty");
}

// What a copy holds and that its characters live in its own buffer, not in the source's.
TEST_CASE("string/construction_copy") {
  const Fixed source;
  const Fixed copy(source);

  CHECK(copy.size() == source.size());
  CHECK(copy.c_str() != source.c_str());
  CHECK(*copy.c_str() == '\0');

  static_assert(copiedSize() == 0, "a copy of an empty string must hold no characters");
}

// Which constructors a call site may name, and which ones the type system rejects or keeps noexcept.
TEST_CASE("string/construction_signatures") {
  static_assert(std::is_nothrow_default_constructible_v<Fixed>, "an empty string must be buildable without throwing");
  static_assert(std::is_nothrow_copy_constructible_v<Fixed>, "copying a string must not throw");
  static_assert(std::is_nothrow_move_constructible_v<Fixed>, "moving a string must not throw");

  static_assert(std::is_convertible_v<const char *, Fixed>,
                "a const char * argument must become a string without a cast");
  static_assert(!std::is_constructible_v<Fixed, nullptr_t>,
                "the deleted overload must reject a literal nullptr during compilation");
}

} // namespace toy
