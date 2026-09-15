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

// Buffer size every case builds on, the terminator included.
constexpr size_t c_allocatedSize = 16;

using Fixed = FixedString<c_allocatedSize>;

// Most characters a Fixed string holds.
constexpr size_t c_capacity = c_allocatedSize - 1;

// The literal most cases copy. Its length is measured from the literal, not written out as a constant.
constexpr const char * c_sample       = "player";
constexpr size_t       c_sampleLength = char_traits<char>::length(c_sample);

// Count of sample bytes a counted construction takes, short enough to leave sample characters out.
constexpr size_t c_prefixLength = 4;

// Range with a null byte inside; its length comes from the array, since a terminator scan stops at the first null.
constexpr char   c_embeddedNull[]     = "ab\0cd";
constexpr size_t c_embeddedNullLength = sizeof(c_embeddedNull) - 1;

// Literal longer than the buffer, so a counted construction can fill the buffer exactly.
constexpr const char * c_long       = "abcdefghijklmnopqrstuvwxyz";
constexpr size_t       c_longLength = char_traits<char>::length(c_long);

// Index of the first byte where the string differs from the count bytes of expected followed by a terminator; count + 1
// when the size and every byte match.
[[nodiscard]] constexpr size_t firstDifference(const Fixed & string, const char * expected, size_t count) noexcept {
  if (string.size() != count)
    return 0;

  size_t index = 0;
  while (index < count && string.c_str()[index] == expected[index])
    ++index;

  if (index == count && string.c_str()[count] == '\0')
    ++index;

  return index;
}

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

// Where a copy of the sample string first differs from the sample, read in a constant expression.
[[nodiscard]] constexpr size_t copiedDifference() noexcept {
  const Fixed source(c_sample);
  const Fixed copy(source);

  return firstDifference(copy, c_sample, c_sampleLength);
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

// A null-terminated source is measured and copied into the string's own buffer, the terminator included.
TEST_CASE("string/construction_from_pointer") {
  const Fixed string(c_sample);

  CHECK(string.size() == c_sampleLength);
  CHECK(firstDifference(string, c_sample, c_sampleLength) == c_sampleLength + 1);
  CHECK(string.c_str() != c_sample);

  static_assert(Fixed(c_sample).size() == c_sampleLength,
                "the pointer constructor must measure the literal's byte count");
  static_assert(firstDifference(Fixed(c_sample), c_sample, c_sampleLength) == c_sampleLength + 1,
                "the pointer constructor must copy every byte and terminate the copy");
}

// A counted construction copies exactly count bytes: it stops short of the source's terminator and copies a null inside
// the range like any other byte.
TEST_CASE("string/construction_from_range") {
  const Fixed prefix(c_sample, c_prefixLength);
  CHECK(prefix.size() == c_prefixLength);
  CHECK(firstDifference(prefix, c_sample, c_prefixLength) == c_prefixLength + 1);

  const Fixed embedded(c_embeddedNull, c_embeddedNullLength);
  CHECK(embedded.size() == c_embeddedNullLength);
  CHECK(firstDifference(embedded, c_embeddedNull, c_embeddedNullLength) == c_embeddedNullLength + 1);

  static_assert(firstDifference(Fixed(c_sample, c_prefixLength), c_sample, c_prefixLength) == c_prefixLength + 1,
                "a counted construction must copy the prefix and terminate it");
  static_assert(firstDifference(Fixed(c_embeddedNull, c_embeddedNullLength), c_embeddedNull, c_embeddedNullLength)
                  == c_embeddedNullLength + 1,
                "a null byte inside the range must not end the copy");
}

// An empty range builds an empty string, a null pointer with a count of zero included.
TEST_CASE("string/construction_from_empty_range") {
  const Fixed fromNull(nullptr, 0);
  CHECK(fromNull.size() == 0);
  CHECK(*fromNull.c_str() == '\0');

  const Fixed fromSample(c_sample, 0);
  CHECK(fromSample.size() == 0);
  CHECK(*fromSample.c_str() == '\0');

  static_assert(Fixed(nullptr, 0).size() == 0, "a null pointer with a count of zero must build an empty string");
  static_assert(*Fixed(c_sample, 0).c_str() == '\0', "a count of zero must copy no byte");
}

// A range as long as the capacity fills the buffer and still ends in a terminator.
TEST_CASE("string/construction_full_capacity") {
  static_assert(c_longLength > c_capacity, "the long literal must not fit, so the count alone bounds the copy");

  const Fixed string(c_long, c_capacity);

  CHECK(string.size() == c_capacity);
  CHECK(firstDifference(string, c_long, c_capacity) == c_capacity + 1);

  static_assert(firstDifference(Fixed(c_long, c_capacity), c_long, c_capacity) == c_capacity + 1,
                "a range as long as the capacity must fit with its terminator");
}

// A copy holds the same characters as its source in a buffer of its own.
TEST_CASE("string/construction_copy") {
  const Fixed source(c_sample);
  const Fixed copy(source);

  CHECK(copy.c_str() != source.c_str());
  CHECK(firstDifference(copy, c_sample, c_sampleLength) == c_sampleLength + 1);

  static_assert(copiedDifference() == c_sampleLength + 1, "a copy must hold every byte of its source");
}

// Which constructors a call site may name, and which ones the type system rejects or keeps noexcept.
TEST_CASE("string/construction_signatures") {
  static_assert(std::is_nothrow_default_constructible_v<Fixed>, "an empty string must be buildable without throwing");
  static_assert(std::is_nothrow_constructible_v<Fixed, const char *, size_t>, "a counted construction must not throw");
  static_assert(std::is_nothrow_copy_constructible_v<Fixed>, "copying a string must not throw");
  static_assert(std::is_nothrow_move_constructible_v<Fixed>, "moving a string must not throw");

  static_assert(std::is_convertible_v<const char *, Fixed>,
                "a const char * argument must become a string without a cast");
  static_assert(!std::is_constructible_v<Fixed, nullptr_t>,
                "the deleted overload must reject a literal nullptr during compilation");
}

} // namespace toy
