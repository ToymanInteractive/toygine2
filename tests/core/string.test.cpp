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
  \brief  Unit tests for \ref toy::String constructors.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// Buffer size every case builds on, the terminator included.
constexpr size_t c_allocatedSize = 16;

using Fixed = FixedString<c_allocatedSize>;

// A string over a larger buffer, the source for the cross-storage cases.
using Wide = FixedString<c_allocatedSize * 2>;

// Most characters a Fixed string holds.
constexpr size_t c_capacity = c_allocatedSize - 1;

// The literal most cases copy; its length is measured from the literal.
constexpr const char * c_sample       = "player";
constexpr size_t       c_sampleLength = char_traits<char>::length(c_sample);

// Byte count a counted construction takes, short enough to leave sample characters out.
constexpr size_t c_prefixLength = 4;

// Offset and byte count the substring cases take, so the substring stops short of both ends of the sample.
constexpr size_t c_substringOffset = 2;
constexpr size_t c_substringLength = 3;

// Bytes from the substring offset to the end of the sample.
constexpr size_t c_tailLength = c_sampleLength - c_substringOffset;

// Range with a null byte inside; its length comes from the array, since a terminator scan stops at the first null.
constexpr char   c_embeddedNull[]     = "ab\0cd";
constexpr size_t c_embeddedNullLength = sizeof(c_embeddedNull) - 1;

// Literal longer than the buffer, so a counted construction can fill the buffer exactly.
constexpr const char * c_long       = "abcdefghijklmnopqrstuvwxyz";
constexpr size_t       c_longLength = char_traits<char>::length(c_long);

// Character and count the fill cases write, and the same fill spelled out.
constexpr char         c_fillCharacter = 'x';
constexpr const char * c_filled        = "xxxxx";
constexpr size_t       c_fillCount     = char_traits<char>::length(c_filled);

// The characters the initializer-list cases write between braces, spelled as a literal.
constexpr const char * c_listed       = "abc";
constexpr size_t       c_listedLength = char_traits<char>::length(c_listed);

// Index of the first byte where the string differs from the count bytes of expected followed by a terminator; count + 1
// when the size and every byte match.
template <typename StringType>
[[nodiscard]] constexpr size_t firstDifference(const StringType & string, const char * expected,
                                               size_t count) noexcept {
  if (string.size() != count)
    return 0;

  size_t index = 0;
  while (index < count && string.c_str()[index] == expected[index])
    ++index;

  if (index == count && string.c_str()[count] == '\0')
    ++index;

  return index;
}

// Iterator over a character array that models input_iterator and nothing stronger, so a range of it is single-pass.
class SinglePassIterator {
public:
  using value_type       = char;
  using difference_type  = ptrdiff_t;
  using iterator_concept = std::input_iterator_tag;

  constexpr SinglePassIterator() noexcept = default;

  constexpr explicit SinglePassIterator(const char * position) noexcept
    : _position(position) {}

  [[nodiscard]] constexpr char operator*() const noexcept {
    return *_position;
  }

  constexpr SinglePassIterator & operator++() noexcept {
    ++_position;
    return *this;
  }

  constexpr void operator++(int) noexcept {
    ++_position;
  }

  [[nodiscard]] constexpr bool operator==(const SinglePassIterator & other) const noexcept = default;

private:
  const char * _position{nullptr};
};

static_assert(std::input_iterator<SinglePassIterator> && !std::forward_iterator<SinglePassIterator>,
              "the helper must be single-pass, so the construction takes its one-character-at-a-time path");

// Input iterator with no equality comparison, so a pair of it marks no range.
class UncomparableIterator {
public:
  using value_type      = char;
  using difference_type = ptrdiff_t;

  [[nodiscard]] char operator*() const noexcept {
    return '\0';
  }

  UncomparableIterator & operator++() noexcept {
    return *this;
  }

  void operator++(int) noexcept {}
};

static_assert(std::input_iterator<UncomparableIterator>
                && !std::sentinel_for<UncomparableIterator, UncomparableIterator>,
              "the helper must read like an iterator yet give no end to compare against");

// Where a string built from a single-pass range over the sample first differs from the sample.
[[nodiscard]] constexpr size_t singlePassDifference() noexcept {
  const Fixed string(SinglePassIterator(c_sample), SinglePassIterator(c_sample + c_sampleLength));

  return firstDifference(string, c_sample, c_sampleLength);
}

// Where a copy of the sample string first differs from the sample.
[[nodiscard]] constexpr size_t copiedDifference() noexcept {
  const Fixed source(c_sample);
  const Fixed copy(source);

  return firstDifference(copy, c_sample, c_sampleLength);
}

// Where a string moved from the sample string first differs from the sample.
[[nodiscard]] constexpr size_t movedDifference() noexcept {
  Fixed       source(c_sample);
  const Fixed moved(std::move(source));

  return firstDifference(moved, c_sample, c_sampleLength);
}

// Where a substring of an lvalue sample string, taken with an offset alone, first differs from the sample's tail.
[[nodiscard]] constexpr size_t lvalueTailDifference() noexcept {
  const Fixed source(c_sample);
  const Fixed tail(source, c_substringOffset);

  return firstDifference(tail, c_sample + c_substringOffset, c_tailLength);
}

} // namespace

// What a default-constructed string holds: no characters and a terminator behind c_str().
TEST_CASE("string/construction") {
  const Fixed string;

  CHECK(string.size() == 0);
  REQUIRE(string.c_str() != nullptr);
  CHECK(*string.c_str() == '\0');

  static_assert(Fixed().size() == 0, "a default-constructed string must hold no characters");
  static_assert(*Fixed().c_str() == '\0', "c_str() of a default-constructed string must read as empty");
}

// A fill construction writes count copies of one character, and a count of zero builds an empty string.
TEST_CASE("string/construction_fill") {
  const Fixed filled(c_fillCount, c_fillCharacter);
  CHECK(firstDifference(filled, c_filled, c_fillCount) == c_fillCount + 1);

  const Fixed empty(0, c_fillCharacter);
  CHECK(empty.size() == 0);
  CHECK(*empty.c_str() == '\0');

  static_assert(firstDifference(Fixed(c_fillCount, c_fillCharacter), c_filled, c_fillCount) == c_fillCount + 1,
                "a fill construction must write count copies of the character and terminate them");
  static_assert(Fixed(0, c_fillCharacter).size() == 0, "a fill count of zero must build an empty string");
}

// An iterator range is copied whole, whether the iterators allow several passes or one.
TEST_CASE("string/construction_from_iterators") {
  const Fixed fromPointers(c_sample, c_sample + c_sampleLength);
  CHECK(firstDifference(fromPointers, c_sample, c_sampleLength) == c_sampleLength + 1);

  const Fixed fromEmpty(c_sample, c_sample);
  CHECK(fromEmpty.size() == 0);

  CHECK(singlePassDifference() == c_sampleLength + 1);

  static_assert(firstDifference(Fixed(c_sample, c_sample + c_sampleLength), c_sample, c_sampleLength)
                  == c_sampleLength + 1,
                "a forward range must be copied whole");
  static_assert(singlePassDifference() == c_sampleLength + 1, "a single-pass range must be copied whole");
}

#if defined(__cpp_lib_ranges_to_container)
// A range tagged with std::from_range is copied whole.
TEST_CASE("string/construction_from_range_tag") {
  const Fixed string(std::from_range, StringView{c_sample});
  CHECK(firstDifference(string, c_sample, c_sampleLength) == c_sampleLength + 1);

  static_assert(firstDifference(Fixed(std::from_range, StringView{c_sample}), c_sample, c_sampleLength)
                  == c_sampleLength + 1,
                "a tagged range must be copied whole");
}
#endif // __cpp_lib_ranges_to_container

// A counted construction copies exactly count bytes: it stops short of the source's terminator and copies a null inside
// the range like any other byte.
TEST_CASE("string/construction_from_counted_pointer") {
  const Fixed prefix(c_sample, c_prefixLength);
  CHECK(firstDifference(prefix, c_sample, c_prefixLength) == c_prefixLength + 1);

  const Fixed embedded(c_embeddedNull, c_embeddedNullLength);
  CHECK(firstDifference(embedded, c_embeddedNull, c_embeddedNullLength) == c_embeddedNullLength + 1);

  static_assert(firstDifference(Fixed(c_sample, c_prefixLength), c_sample, c_prefixLength) == c_prefixLength + 1,
                "a counted construction must copy the prefix and terminate it");
  static_assert(firstDifference(Fixed(c_embeddedNull, c_embeddedNullLength), c_embeddedNull, c_embeddedNullLength)
                  == c_embeddedNullLength + 1,
                "a null byte inside the range must not end the copy");
}

// An empty counted range builds an empty string, a null pointer with a count of zero included.
TEST_CASE("string/construction_from_counted_pointer_empty") {
  const Fixed fromNull(nullptr, 0);
  CHECK(fromNull.size() == 0);
  CHECK(*fromNull.c_str() == '\0');

  const Fixed fromSample(c_sample, 0);
  CHECK(fromSample.size() == 0);

  static_assert(Fixed(nullptr, 0).size() == 0, "a null pointer with a count of zero must build an empty string");
  static_assert(*Fixed(c_sample, 0).c_str() == '\0', "a count of zero must copy no byte");
}

// A counted range as long as the capacity fills the buffer and still ends in a terminator.
TEST_CASE("string/construction_from_counted_pointer_full_capacity") {
  static_assert(c_longLength > c_capacity, "the long literal must not fit, so the count alone bounds the copy");

  const Fixed string(c_long, c_capacity);
  CHECK(firstDifference(string, c_long, c_capacity) == c_capacity + 1);

  static_assert(firstDifference(Fixed(c_long, c_capacity), c_long, c_capacity) == c_capacity + 1,
                "a range as long as the capacity must fit with its terminator");
}

// A null-terminated source is measured and copied into the string's own buffer.
TEST_CASE("string/construction_from_pointer") {
  const Fixed string(c_sample);

  CHECK(firstDifference(string, c_sample, c_sampleLength) == c_sampleLength + 1);
  CHECK(string.c_str() != c_sample);

  static_assert(firstDifference(Fixed(c_sample), c_sample, c_sampleLength) == c_sampleLength + 1,
                "the pointer constructor must copy every byte and terminate the copy");
}

// A string-like source, a view or a string over another storage, is copied whole.
TEST_CASE("string/construction_from_string_like") {
  const Fixed fromView(StringView{c_sample});
  CHECK(firstDifference(fromView, c_sample, c_sampleLength) == c_sampleLength + 1);

  const Wide  wide(c_sample);
  const Fixed fromWide(wide);
  CHECK(firstDifference(fromWide, c_sample, c_sampleLength) == c_sampleLength + 1);

  static_assert(firstDifference(Fixed(StringView{c_sample}), c_sample, c_sampleLength) == c_sampleLength + 1,
                "a view must be copied whole");
  static_assert(firstDifference(Fixed(Wide(c_sample)), c_sample, c_sampleLength) == c_sampleLength + 1,
                "a string over another storage must be copied whole");
}

// A substring starts at the offset, takes at most count bytes, and stops at the end of the source.
TEST_CASE("string/construction_substring") {
  const StringView view{c_sample};

  const Fixed bounded(view, c_substringOffset, c_substringLength);
  CHECK(firstDifference(bounded, c_sample + c_substringOffset, c_substringLength) == c_substringLength + 1);

  const Fixed tail(view, c_substringOffset);
  CHECK(firstDifference(tail, c_sample + c_substringOffset, c_tailLength) == c_tailLength + 1);

  const Fixed atEnd(view, c_sampleLength);
  CHECK(atEnd.size() == 0);

  static_assert(firstDifference(Fixed(StringView{c_sample}, c_substringOffset, c_substringLength),
                                c_sample + c_substringOffset, c_substringLength)
                  == c_substringLength + 1,
                "a substring must take count bytes from the offset");
  static_assert(Fixed(StringView{c_sample}, c_substringOffset, Fixed::npos).size() == c_tailLength,
                "a count past the end must stop at the end of the source");
  static_assert(Fixed(StringView{c_sample}, c_sampleLength).size() == 0,
                "an offset at the end of the source must build an empty string");
}

// A substring of an lvalue string, the std::basic_string(const basic_string &, pos[, count]) shape, same storage or
// not.
TEST_CASE("string/construction_substring_of_string") {
  const Fixed source(c_sample);

  const Fixed tail(source, c_substringOffset);
  CHECK(firstDifference(tail, c_sample + c_substringOffset, c_tailLength) == c_tailLength + 1);

  const Fixed bounded(source, c_substringOffset, c_substringLength);
  CHECK(firstDifference(bounded, c_sample + c_substringOffset, c_substringLength) == c_substringLength + 1);

  const Wide  wide(c_sample);
  const Fixed fromWide(wide, c_substringOffset);
  CHECK(firstDifference(fromWide, c_sample + c_substringOffset, c_tailLength) == c_tailLength + 1);

  static_assert(lvalueTailDifference() == c_tailLength + 1, "an offset alone must take the rest of the string");
}

// A copy holds the same characters as its source in a buffer of its own.
TEST_CASE("string/construction_copy") {
  const Fixed source(c_sample);
  const Fixed copy(source);

  CHECK(copy.c_str() != source.c_str());
  CHECK(firstDifference(copy, c_sample, c_sampleLength) == c_sampleLength + 1);

  static_assert(copiedDifference() == c_sampleLength + 1, "a copy must hold every byte of its source");
}

// A moved-to string holds the characters its source held.
TEST_CASE("string/construction_move") {
  Fixed       source(c_sample);
  const Fixed moved(std::move(source));

  CHECK(firstDifference(moved, c_sample, c_sampleLength) == c_sampleLength + 1);

  static_assert(movedDifference() == c_sampleLength + 1, "a moved-to string must hold every byte of its source");
}

// A substring of an rvalue string moves the kept bytes to the front of the storage it takes over.
TEST_CASE("string/construction_substring_move") {
  Fixed       source(c_sample);
  const Fixed bounded(std::move(source), c_substringOffset, c_substringLength);
  CHECK(firstDifference(bounded, c_sample + c_substringOffset, c_substringLength) == c_substringLength + 1);

  const Fixed whole(Fixed(c_sample), 0);
  CHECK(firstDifference(whole, c_sample, c_sampleLength) == c_sampleLength + 1);

  static_assert(firstDifference(Fixed(Fixed(c_sample), c_substringOffset, c_substringLength),
                                c_sample + c_substringOffset, c_substringLength)
                  == c_substringLength + 1,
                "a moved-from substring must keep count bytes from the offset");
  static_assert(firstDifference(Fixed(Fixed(c_sample), c_substringOffset), c_sample + c_substringOffset, c_tailLength)
                  == c_tailLength + 1,
                "an offset alone must keep the rest of the moved string");
  static_assert(Fixed(Fixed(c_sample), c_sampleLength).size() == 0,
                "an offset at the end of the moved string must build an empty string");
}

// Brace initialization copies the listed characters, so a count and a character read as two characters.
TEST_CASE("string/construction_from_initializer_list") {
  const Fixed string{'a', 'b', 'c'};
  CHECK(firstDifference(string, c_listed, c_listedLength) == c_listedLength + 1);

  const Fixed braced{static_cast<char>(3), 'a'};
  CHECK(braced.size() == 2);

  static_assert(firstDifference(Fixed{'a', 'b', 'c'}, c_listed, c_listedLength) == c_listedLength + 1,
                "a list must be copied character by character");
  static_assert(Fixed{static_cast<char>(3), 'a'}.size() == 2, "braces must select the list, not the fill constructor");
}

// Which constructors a call site may name, which ones it must spell out, and which the type system rejects.
TEST_CASE("string/construction_signatures") {
  static_assert(std::is_nothrow_default_constructible_v<Fixed>, "an empty string must be buildable without throwing");
  static_assert(std::is_nothrow_constructible_v<Fixed, size_t, char>, "a fill construction must not throw");
  static_assert(std::is_nothrow_constructible_v<Fixed, const char *, const char *>,
                "an iterator construction must not throw");
  static_assert(std::is_nothrow_constructible_v<Fixed, const char *, size_t>, "a counted construction must not throw");
  static_assert(std::is_nothrow_constructible_v<Fixed, StringView, size_t, size_t>,
                "a substring construction must not throw");
  static_assert(std::is_nothrow_copy_constructible_v<Fixed>, "copying a string must not throw");
  static_assert(std::is_nothrow_move_constructible_v<Fixed>, "moving a string must not throw");
  static_assert(std::is_nothrow_constructible_v<Fixed, Fixed &&, size_t, size_t>,
                "a moved-from substring construction must not throw");
  static_assert(std::is_nothrow_destructible_v<Fixed>, "destroying a string must not throw");

  static_assert(std::is_convertible_v<const char *, Fixed>,
                "a const char * argument must become a string without a cast");
  static_assert(std::is_convertible_v<std::initializer_list<char>, Fixed>, "a braced list must become a string");
  static_assert(!std::is_convertible_v<StringView, Fixed>, "a copy from a view must be spelled out at the call site");
  static_assert(!std::is_convertible_v<Wide, Fixed>,
                "a copy from another storage must be spelled out at the call site");

  static_assert(!std::is_constructible_v<Fixed, nullptr_t>,
                "the deleted overload must reject a literal nullptr during compilation");
  static_assert(!std::is_constructible_v<Fixed, const int *, const int *>,
                "an iterator range must hold characters, not values that merely convert to char");
  static_assert(!std::is_constructible_v<Fixed, UncomparableIterator, UncomparableIterator>,
                "an iterator pair must be comparable, so the constructor can find the end of the range");
}

} // namespace toy
