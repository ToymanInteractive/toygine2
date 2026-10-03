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
  \brief  Unit tests for \ref toy::String constructors, assignment, and element access.
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
constexpr size_t       c_sampleLength = std::char_traits<char>::length(c_sample);

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
constexpr size_t       c_longLength = std::char_traits<char>::length(c_long);

// Character and count the fill cases write, and the same fill spelled out.
constexpr char         c_fillCharacter = 'x';
constexpr const char * c_filled        = "xxxxx";
constexpr size_t       c_fillCount     = std::char_traits<char>::length(c_filled);

// The characters the initializer-list cases write between braces, spelled as a literal.
constexpr const char * c_listed       = "abc";
constexpr size_t       c_listedLength = std::char_traits<char>::length(c_listed);

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
template <typename StringType = Fixed>
[[nodiscard]] constexpr size_t copiedDifference() noexcept {
  const StringType source(c_sample);
  const StringType copy(source);

  return firstDifference(copy, c_sample, c_sampleLength);
}

// Where a string moved from the sample string first differs from the sample.
template <typename StringType = Fixed>
[[nodiscard]] constexpr size_t movedDifference() noexcept {
  StringType       source(c_sample);
  const StringType moved(std::move(source));

  return firstDifference(moved, c_sample, c_sampleLength);
}

// Where a substring of an lvalue sample string, taken with an offset alone, first differs from the sample's tail.
[[nodiscard]] constexpr size_t lvalueTailDifference() noexcept {
  const Fixed source(c_sample);
  const Fixed tail(source, c_substringOffset);

  return firstDifference(tail, c_sample + c_substringOffset, c_tailLength);
}

// Buffer large enough that assignment copies only the characters in use, the path Fixed does not take.
constexpr size_t c_largeAllocatedSize = 256;

using Large = FixedString<c_largeAllocatedSize>;

static_assert(sizeof(Fixed) <= platform::c_inlineCopyMaxBytes,
              "Fixed must copy its whole object on every target, so the assignment cases reach that path");
static_assert(sizeof(Large) > platform::c_inlineCopyMaxBytes,
              "Large must copy by length on every target, so the assignment cases reach that path");

// String holding a value longer than the sample, so an assignment of the sample has to move the terminator.
template <typename StringType>
[[nodiscard]] constexpr StringType longerThanSample() noexcept {
  return StringType(c_long, c_capacity);
}

// Where a string holding a longer value, then copy-assigned the sample string, first differs from the sample.
template <typename StringType>
[[nodiscard]] constexpr size_t copyAssignedDifference() noexcept {
  const StringType source(c_sample);
  StringType       target = longerThanSample<StringType>();
  target                  = source;

  return firstDifference(target, c_sample, c_sampleLength);
}

// Where a string holding a longer value, then move-assigned the sample string, first differs from the sample.
template <typename StringType>
[[nodiscard]] constexpr size_t moveAssignedDifference() noexcept {
  StringType source(c_sample);
  StringType target = longerThanSample<StringType>();
  target            = std::move(source);

  return firstDifference(target, c_sample, c_sampleLength);
}

// Where the sample string, assigned to itself through an alias, first differs from the sample.
template <typename StringType>
[[nodiscard]] constexpr size_t selfAssignedDifference() noexcept {
  StringType         string(c_sample);
  const StringType & alias = string;
  string                   = alias;

  return firstDifference(string, c_sample, c_sampleLength);
}

// Where the sample string, assigned a substring of itself, first differs from that substring.
[[nodiscard]] constexpr size_t selfSubstringDifference() noexcept {
  Fixed string(c_sample);
  string.assign(string, c_substringOffset, c_substringLength);

  return firstDifference(string, c_sample + c_substringOffset, c_substringLength);
}

// Where the sample string, assigned counted bytes from inside its own buffer, first differs from those bytes.
[[nodiscard]] constexpr size_t selfPointerDifference() noexcept {
  Fixed string(c_sample);
  string.assign(string.c_str() + c_substringOffset, c_substringLength);

  return firstDifference(string, c_sample + c_substringOffset, c_substringLength);
}

// Where the sample string, assigned a view of its own tail as a range, first differs from that tail.
[[nodiscard]] constexpr size_t selfRangeDifference() noexcept {
  Fixed string(c_sample);
  string.assign_range(StringView{string.c_str() + c_substringOffset});

  return firstDifference(string, c_sample + c_substringOffset, c_tailLength);
}

// Where a string holding a longer value, then assigned a single-pass range over the sample, first differs from it.
[[nodiscard]] constexpr size_t singlePassAssignedDifference() noexcept {
  Fixed string = longerThanSample<Fixed>();
  string.assign(SinglePassIterator(c_sample), SinglePassIterator(c_sample + c_sampleLength));

  return firstDifference(string, c_sample, c_sampleLength);
}

// Where a string holding a longer value, then assigned a single-pass range with assign_range(), first differs from it.
[[nodiscard]] constexpr size_t singlePassRangeDifference() noexcept {
  const std::ranges::subrange range(SinglePassIterator(c_sample), SinglePassIterator(c_sample + c_sampleLength));

  Fixed string = longerThanSample<Fixed>();
  string.assign_range(range);

  return firstDifference(string, c_sample, c_sampleLength);
}

// The sample as a constant, so the element-access cases reach the const overloads in a constant expression.
constexpr Fixed c_sampleString(c_sample);

// The sample after front(), at(), operator[] and back() each write one character, offsets 0, 1, 2 and the last.
constexpr const char * c_overwritten = "abcyed";

// The sample after a write through data() replaces its first character with the fill character.
constexpr const char * c_overwrittenFirst = "xlayer";

// The sample string after front(), at(), operator[] and back() each write the matching character of c_overwritten.
[[nodiscard]] constexpr Fixed overwrittenThroughElements() noexcept {
  Fixed string(c_sample);
  string.front() = c_overwritten[0];
  string.at(1)   = c_overwritten[1];
  string[2]      = c_overwritten[2];
  string.back()  = c_overwritten[c_sampleLength - 1];

  return string;
}

// The sample string after a write through data() replaces its first character.
[[nodiscard]] constexpr Fixed overwrittenThroughData() noexcept {
  Fixed string(c_sample);
  string.data()[0] = c_fillCharacter;

  return string;
}

// A string holding no characters, kept at namespace scope so constant evaluation can compare its iterators.
constexpr Fixed c_emptyString{};

// The sample characters back to front, the order a reverse walk must yield.
constexpr const char * c_sampleReversed = "reyalp";

// The sample after writes through begin() and rbegin() replace its first and last characters.
constexpr const char * c_overwrittenEnds = "alayed";

// The sample string after begin() and rbegin() each write the matching character of c_overwrittenEnds.
[[nodiscard]] constexpr Fixed overwrittenThroughIterators() noexcept {
  Fixed string(c_sample);
  *string.begin()  = c_overwrittenEnds[0];
  *string.rbegin() = c_overwrittenEnds[c_sampleLength - 1];

  return string;
}

// The sample string after a reserve() of newCapacity, so a constant expression reads what the request left behind.
[[nodiscard]] constexpr Fixed reservedSample(size_t newCapacity) noexcept {
  Fixed string(c_sample);
  string.reserve(newCapacity);

  return string;
}

// The sample string after a shrink_to_fit() call.
[[nodiscard]] constexpr Fixed shrunkSample() noexcept {
  Fixed string(c_sample);
  string.shrink_to_fit();

  return string;
}

// Storage that moves its characters to a second buffer whenever it grows, as a storage that owns heap memory may.
template <size_t MaxCapacity>
class RelocatingStorage {
public:
  [[nodiscard]] constexpr char * data() noexcept {
    return _buffers[_active].data();
  }

  [[nodiscard]] constexpr const char * data() const noexcept {
    return _buffers[_active].data();
  }

  [[nodiscard]] constexpr size_t size() const noexcept {
    return _size;
  }

  [[nodiscard]] constexpr size_t capacity() const noexcept {
    return _capacity;
  }

  [[nodiscard]] constexpr size_t max_size() const noexcept {
    return MaxCapacity;
  }

  [[nodiscard]] constexpr bool reserve(size_t newCapacity) noexcept {
    if (newCapacity > MaxCapacity)
      return false;

    if (newCapacity > _capacity) {
      const size_t target = 1 - _active;
      std::ranges::copy_n(data(), static_cast<ptrdiff_t>(_size + 1), _buffers[target].data());
      _active   = target;
      _capacity = newCapacity;
    }

    return true;
  }

  constexpr void shrink_to_fit() noexcept {
    _capacity = _size;
  }

  constexpr void setSize(size_t newSize) noexcept {
    _size           = newSize;
    data()[newSize] = '\0';
  }

private:
  std::array<std::array<char, MaxCapacity + 1>, 2> _buffers{};
  size_t                                           _active{0};
  size_t                                           _size{0};
  size_t                                           _capacity{0};
};

static_assert(StringStorage<RelocatingStorage<c_capacity>>, "the relocating stub must back a string");

// String whose every growth moves the buffer, so an edit must find a source inside it again after the move.
using Relocating = String<RelocatingStorage<c_capacity>>;

// What firstDifference() returns when a string holds exactly the characters of expected.
[[nodiscard]] constexpr size_t fullMatch(const char * expected) noexcept {
  return std::char_traits<char>::length(expected) + 1;
}

// Where the sample string, after edit runs on it, first differs from the count bytes of expected.
template <typename StringType = Fixed, typename Edit>
[[nodiscard]] constexpr size_t editedDifference(Edit edit, const char * expected, size_t count) noexcept {
  StringType string(c_sample);
  edit(string);

  return firstDifference(string, expected, count);
}

// Where the sample string, after edit runs on it, first differs from the C string expected.
template <typename StringType = Fixed, typename Edit>
[[nodiscard]] constexpr size_t editedDifference(Edit edit, const char * expected) noexcept {
  return editedDifference<StringType>(edit, expected, std::char_traits<char>::length(expected));
}

// Offset of the iterator that edit returns after it runs on the sample string.
template <typename Edit>
[[nodiscard]] constexpr size_t returnedOffset(Edit edit) noexcept {
  Fixed string(c_sample);

  return static_cast<size_t>(edit(string) - string.begin());
}

// The sample with removed characters at position replaced by count characters from offset of the sample itself.
[[nodiscard]] constexpr std::array<char, c_allocatedSize> selfReplacedSample(size_t position, size_t removed,
                                                                             size_t offset, size_t count) noexcept {
  std::array<char, c_allocatedSize> expected{};

  auto output = std::ranges::copy_n(c_sample, static_cast<ptrdiff_t>(position), expected.begin()).out;
  output      = std::ranges::copy_n(c_sample + offset, static_cast<ptrdiff_t>(count), output).out;
  std::ranges::copy(c_sample + position + removed, c_sample + c_sampleLength, output);

  return expected;
}

// Inserts each slice of the sample into itself; report() gets the triple, the difference, and the full-match value.
template <typename StringType, typename Report>
constexpr void forEachSelfInsertion(Report report) noexcept {
  for (const size_t position : std::views::iota(size_t{0}, c_sampleLength + 1)) {
    for (const size_t offset : std::views::iota(size_t{0}, c_sampleLength)) {
      for (const size_t count : std::views::iota(size_t{1}, c_sampleLength - offset + 1)) {
        StringType string(c_sample);
        string.insert(position, string.c_str() + offset, count);

        const size_t length = c_sampleLength + count;
        report(position, offset, count,
               firstDifference(string, selfReplacedSample(position, 0, offset, count).data(), length), length + 1);
      }
    }
  }
}

// How many triples of forEachSelfInsertion() leave anything but selfInsertedSample().
template <typename StringType>
[[nodiscard]] constexpr size_t selfInsertionFailures() noexcept {
  size_t failures = 0;
  forEachSelfInsertion<StringType>([&failures](size_t, size_t, size_t, size_t difference, size_t match) {
    if (difference != match)
      ++failures;
  });

  return failures;
}

// Replaces each nonempty run of the sample with each slice of itself; insertion, where nothing is removed, has its own.
template <typename StringType, typename Report>
constexpr void forEachSelfReplacement(Report report) noexcept {
  for (const size_t position : std::views::iota(size_t{0}, c_sampleLength)) {
    for (const size_t removed : std::views::iota(size_t{1}, c_sampleLength - position + 1)) {
      for (const size_t offset : std::views::iota(size_t{0}, c_sampleLength)) {
        for (const size_t count : std::views::iota(size_t{1}, c_sampleLength - offset + 1)) {
          StringType string(c_sample);
          string.replace(position, removed, string.c_str() + offset, count);

          const size_t length = c_sampleLength - removed + count;
          report(position, removed, offset, count,
                 firstDifference(string, selfReplacedSample(position, removed, offset, count).data(), length),
                 length + 1);
        }
      }
    }
  }
}

// How many quadruples of forEachSelfReplacement() leave anything but selfReplacedSample().
template <typename StringType>
[[nodiscard]] constexpr size_t selfReplacementFailures() noexcept {
  size_t failures = 0;
  forEachSelfReplacement<StringType>([&failures](size_t, size_t, size_t, size_t, size_t difference, size_t match) {
    if (difference != match)
      ++failures;
  });

  return failures;
}

// Where each side of a swap between the sample string and the listed string first differs from what the other held.
template <typename StringType>
[[nodiscard]] constexpr std::array<size_t, 2> swappedDifferences() noexcept {
  StringType sample(c_sample);
  StringType listed(c_listed);
  sample.swap(listed);

  return {firstDifference(sample, c_listed, c_listedLength), firstDifference(listed, c_sample, c_sampleLength)};
}

// What copy() leaves in a buffer of fill characters, and the count it returns.
struct CopiedCharacters {
  size_t                            count;
  std::array<char, c_allocatedSize> buffer;
};

// Runs copy over a buffer of fill characters and keeps what it wrote.
template <typename Copy>
[[nodiscard]] constexpr CopiedCharacters copiedBy(Copy copy) noexcept {
  CopiedCharacters copied{.count = 0, .buffer = {}};
  copied.buffer.fill(c_fillCharacter);
  copied.count = copy(copied.buffer.data());

  return copied;
}

// The sample with its characters from c_substringOffset on removed.
constexpr const char * c_sampleHead = "pl";

// The sample with c_substringLength characters removed from c_substringOffset.
constexpr const char * c_sampleWithoutMiddle = "plr";

// The sample with the fill characters inserted at c_substringOffset.
constexpr const char * c_filledInside = "plxxxxxayer";

// The sample with the fill characters inserted before its first character.
constexpr const char * c_filledFront = "xxxxxplayer";

// The sample with the fill characters inserted after its last character.
constexpr const char * c_filledBack = "playerxxxxx";

// The sample with the listed characters inserted at c_substringOffset.
constexpr const char * c_listedInside = "plabcayer";

// The sample with the listed characters, back to front, inserted at c_substringOffset.
constexpr const char * c_listedReversedInside = "plcbaayer";

// The sample with a copy of itself inserted at c_substringOffset.
constexpr const char * c_sampleDoubledInside = "plplayerayer";

// The sample with c_embeddedNull inserted at c_substringOffset, the null byte included.
constexpr char   c_embeddedNullInside[]     = "plab\0cdayer";
constexpr size_t c_embeddedNullInsideLength = sizeof(c_embeddedNullInside) - 1;

// The sample with the listed characters after its last character.
constexpr const char * c_listedAppended = "playerabc";

// The sample with the listed characters, back to front, after its last character.
constexpr const char * c_listedReversedAppended = "playercba";

// The sample followed by a copy of itself.
constexpr const char * c_sampleDoubled = "playerplayer";

// The sample with c_embeddedNull after its last character, the null byte included.
constexpr char   c_embeddedNullAppended[]     = "playerab\0cd";
constexpr size_t c_embeddedNullAppendedLength = sizeof(c_embeddedNullAppended) - 1;

// The sample with its c_substringLength characters from c_substringOffset replaced by the listed characters.
constexpr const char * c_listedReplacing = "plabcr";

// The same run replaced by the listed characters back to front.
constexpr const char * c_listedReversedReplacing = "plcbar";

// The same run replaced by the fill characters.
constexpr const char * c_filledReplacing = "plxxxxxr";

// The same run replaced by c_embeddedNull, the null byte included.
constexpr char   c_embeddedNullReplacing[]     = "plab\0cdr";
constexpr size_t c_embeddedNullReplacingLength = sizeof(c_embeddedNullReplacing) - 1;

// The sample with its character at c_substringOffset replaced by a copy of the whole sample.
constexpr const char * c_sampleSpliced = "plplayeryer";

// The sample followed by two null bytes, what a resize() with the default character leaves.
constexpr char   c_sampleWithNulls[]     = "player\0\0";
constexpr size_t c_sampleWithNullsLength = sizeof(c_sampleWithNulls) - 1;

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
  CHECK(copiedDifference<Large>() == c_sampleLength + 1);

  static_assert(copiedDifference() == c_sampleLength + 1, "a copy must hold every byte of its source");
  static_assert(copiedDifference<Large>() == c_sampleLength + 1,
                "a copy that takes only the characters in use must still hold every one of them");
}

// A moved-to string holds the characters its source held.
TEST_CASE("string/construction_move") {
  Fixed       source(c_sample);
  const Fixed moved(std::move(source));

  CHECK(firstDifference(moved, c_sample, c_sampleLength) == c_sampleLength + 1);
  CHECK(movedDifference<Large>() == c_sampleLength + 1);

  static_assert(movedDifference() == c_sampleLength + 1, "a moved-to string must hold every byte of its source");
  static_assert(movedDifference<Large>() == c_sampleLength + 1,
                "a move that takes only the characters in use must still hold every one of them");
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
  static_assert(std::is_trivially_copy_constructible_v<Fixed>,
                "a small string must keep the trivial copy construction");
  static_assert(!std::is_trivially_copy_constructible_v<Large>,
                "a large string must copy only the characters in use on construction");
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

// A fill assignment writes count copies of a character; zero empties the string, and a single char is a fill of one.
TEST_CASE("string/assignment_fill") {
  Fixed filled = longerThanSample<Fixed>();
  filled.assign(c_fillCount, c_fillCharacter);
  CHECK(firstDifference(filled, c_filled, c_fillCount) == c_fillCount + 1);

  Fixed emptied = longerThanSample<Fixed>();
  emptied.assign(0, c_fillCharacter);
  CHECK(emptied.size() == 0);
  CHECK(*emptied.c_str() == '\0');

  Fixed single = longerThanSample<Fixed>();
  single       = c_fillCharacter;
  CHECK(firstDifference(single, c_filled, 1) == 2);

  static_assert(firstDifference(longerThanSample<Fixed>().assign(c_fillCount, c_fillCharacter), c_filled, c_fillCount)
                  == c_fillCount + 1,
                "a fill assignment must write count copies of the character and terminate them");
  static_assert(longerThanSample<Fixed>().assign(0, c_fillCharacter).size() == 0,
                "a fill count of zero must empty the string");
  static_assert(firstDifference(longerThanSample<Fixed>() = c_fillCharacter, c_filled, 1) == 2,
                "a character must assign as a fill of one");
}

// A pointer assignment copies a C string whole, or exactly count bytes of a counted one, embedded nulls included.
TEST_CASE("string/assignment_from_pointer") {
  Fixed measured = longerThanSample<Fixed>();
  measured       = c_sample;
  CHECK(firstDifference(measured, c_sample, c_sampleLength) == c_sampleLength + 1);
  CHECK(measured.c_str() != c_sample);

  Fixed prefix = longerThanSample<Fixed>();
  prefix.assign(c_sample, c_prefixLength);
  CHECK(firstDifference(prefix, c_sample, c_prefixLength) == c_prefixLength + 1);

  Fixed embedded = longerThanSample<Fixed>();
  embedded.assign(c_embeddedNull, c_embeddedNullLength);
  CHECK(firstDifference(embedded, c_embeddedNull, c_embeddedNullLength) == c_embeddedNullLength + 1);

  Fixed fromNull = longerThanSample<Fixed>();
  fromNull.assign(nullptr, 0);
  CHECK(fromNull.size() == 0);

  static_assert(firstDifference(longerThanSample<Fixed>() = c_sample, c_sample, c_sampleLength) == c_sampleLength + 1,
                "a null-terminated source must be copied whole");
  static_assert(firstDifference(longerThanSample<Fixed>().assign(c_sample), c_sample, c_sampleLength)
                  == c_sampleLength + 1,
                "assign() must copy a null-terminated source as operator= does");
  static_assert(firstDifference(longerThanSample<Fixed>().assign(c_sample, c_prefixLength), c_sample, c_prefixLength)
                  == c_prefixLength + 1,
                "a counted assignment must copy the prefix and terminate it");
  static_assert(firstDifference(longerThanSample<Fixed>().assign(c_embeddedNull, c_embeddedNullLength), c_embeddedNull,
                                c_embeddedNullLength)
                  == c_embeddedNullLength + 1,
                "a null byte inside the range must not end the copy");
  static_assert(longerThanSample<Fixed>().assign(nullptr, 0).size() == 0,
                "a null pointer with a count of zero must empty the string");
}

// A string-like source, a view or a string over another storage, is copied whole by operator= and assign() alike.
TEST_CASE("string/assignment_from_string_like") {
  Fixed fromView = longerThanSample<Fixed>();
  fromView       = StringView{c_sample};
  CHECK(firstDifference(fromView, c_sample, c_sampleLength) == c_sampleLength + 1);

  const Wide wide(c_sample);
  Fixed      fromWide = longerThanSample<Fixed>();
  fromWide.assign(wide);
  CHECK(firstDifference(fromWide, c_sample, c_sampleLength) == c_sampleLength + 1);

  static_assert(firstDifference(longerThanSample<Fixed>() = StringView{c_sample}, c_sample, c_sampleLength)
                  == c_sampleLength + 1,
                "a view must be copied whole");
  static_assert(firstDifference(longerThanSample<Fixed>().assign(Wide(c_sample)), c_sample, c_sampleLength)
                  == c_sampleLength + 1,
                "a string over another storage must be copied whole");
}

// A substring assignment starts at the offset, takes at most count bytes, and stops at the end of the source.
TEST_CASE("string/assignment_substring") {
  const StringView view{c_sample};

  Fixed bounded = longerThanSample<Fixed>();
  bounded.assign(view, c_substringOffset, c_substringLength);
  CHECK(firstDifference(bounded, c_sample + c_substringOffset, c_substringLength) == c_substringLength + 1);

  Fixed tail = longerThanSample<Fixed>();
  tail.assign(Wide(c_sample), c_substringOffset);
  CHECK(firstDifference(tail, c_sample + c_substringOffset, c_tailLength) == c_tailLength + 1);

  Fixed atEnd = longerThanSample<Fixed>();
  atEnd.assign(view, c_sampleLength);
  CHECK(atEnd.size() == 0);

  static_assert(firstDifference(longerThanSample<Fixed>().assign(StringView{c_sample}, c_substringOffset,
                                                                 c_substringLength),
                                c_sample + c_substringOffset, c_substringLength)
                  == c_substringLength + 1,
                "a substring must take count bytes from the offset");
  static_assert(longerThanSample<Fixed>().assign(StringView{c_sample}, c_substringOffset).size() == c_tailLength,
                "an offset alone must take the rest of the source");
  static_assert(longerThanSample<Fixed>().assign(StringView{c_sample}, c_sampleLength).size() == 0,
                "an offset at the end of the source must empty the string");
}

// An iterator range replaces the contents whole, whether the iterators allow several passes or one.
TEST_CASE("string/assignment_from_iterators") {
  Fixed fromPointers = longerThanSample<Fixed>();
  fromPointers.assign(c_sample, c_sample + c_sampleLength);
  CHECK(firstDifference(fromPointers, c_sample, c_sampleLength) == c_sampleLength + 1);

  Fixed fromEmpty = longerThanSample<Fixed>();
  fromEmpty.assign(c_sample, c_sample);
  CHECK(fromEmpty.size() == 0);

  CHECK(singlePassAssignedDifference() == c_sampleLength + 1);

  static_assert(firstDifference(longerThanSample<Fixed>().assign(c_sample, c_sample + c_sampleLength), c_sample,
                                c_sampleLength)
                  == c_sampleLength + 1,
                "a forward range must replace the contents whole");
  static_assert(singlePassAssignedDifference() == c_sampleLength + 1,
                "a single-pass range must replace the contents whole");
}

// assign_range() replaces the contents with a range, contiguous or single-pass.
TEST_CASE("string/assignment_from_range") {
  Fixed fromView = longerThanSample<Fixed>();
  fromView.assign_range(StringView{c_sample});
  CHECK(firstDifference(fromView, c_sample, c_sampleLength) == c_sampleLength + 1);

  CHECK(singlePassRangeDifference() == c_sampleLength + 1);

  static_assert(firstDifference(longerThanSample<Fixed>().assign_range(StringView{c_sample}), c_sample, c_sampleLength)
                  == c_sampleLength + 1,
                "a contiguous range must replace the contents whole");
  static_assert(singlePassRangeDifference() == c_sampleLength + 1,
                "a single-pass range must replace the contents whole");
}

// A braced list replaces the contents character by character, through operator= and assign() alike.
TEST_CASE("string/assignment_from_initializer_list") {
  Fixed string = longerThanSample<Fixed>();
  string       = {'a', 'b', 'c'};
  CHECK(firstDifference(string, c_listed, c_listedLength) == c_listedLength + 1);

  static_assert(firstDifference(longerThanSample<Fixed>() = {'a', 'b', 'c'}, c_listed, c_listedLength)
                  == c_listedLength + 1,
                "a list must be copied character by character");
  static_assert(firstDifference(longerThanSample<Fixed>().assign({'a', 'b', 'c'}), c_listed, c_listedLength)
                  == c_listedLength + 1,
                "assign() must copy a list as operator= does");
}

// A copy assignment reproduces the source, whether the storage copies its whole object or only the characters in use.
TEST_CASE("string/assignment_copy") {
  CHECK(copyAssignedDifference<Fixed>() == c_sampleLength + 1);
  CHECK(copyAssignedDifference<Large>() == c_sampleLength + 1);

  const Large source(c_sample);
  Large       target = longerThanSample<Large>();
  target.assign(source);
  CHECK(firstDifference(target, c_sample, c_sampleLength) == c_sampleLength + 1);
  CHECK(target.c_str() != source.c_str());

  static_assert(copyAssignedDifference<Fixed>() == c_sampleLength + 1,
                "a whole-object copy assignment must reproduce the source");
  static_assert(copyAssignedDifference<Large>() == c_sampleLength + 1,
                "a length-based copy assignment must reproduce the source and move the terminator");
}

// A move assignment leaves the target holding what the source held, over either copy path.
TEST_CASE("string/assignment_move") {
  CHECK(moveAssignedDifference<Fixed>() == c_sampleLength + 1);
  CHECK(moveAssignedDifference<Large>() == c_sampleLength + 1);

  Large source(c_sample);
  Large target = longerThanSample<Large>();
  target.assign(std::move(source));
  CHECK(firstDifference(target, c_sample, c_sampleLength) == c_sampleLength + 1);

  static_assert(moveAssignedDifference<Fixed>() == c_sampleLength + 1,
                "a whole-object move assignment must reproduce the source");
  static_assert(moveAssignedDifference<Large>() == c_sampleLength + 1,
                "a length-based move assignment must reproduce the source");
}

// A source inside the string itself, the whole string or a part of it, survives the assignment that reads it.
TEST_CASE("string/assignment_from_itself") {
  CHECK(selfAssignedDifference<Fixed>() == c_sampleLength + 1);
  CHECK(selfAssignedDifference<Large>() == c_sampleLength + 1);
  CHECK(selfSubstringDifference() == c_substringLength + 1);
  CHECK(selfPointerDifference() == c_substringLength + 1);
  CHECK(selfRangeDifference() == c_tailLength + 1);

  static_assert(selfAssignedDifference<Fixed>() == c_sampleLength + 1,
                "a whole-object self-assignment must keep the contents");
  static_assert(selfAssignedDifference<Large>() == c_sampleLength + 1,
                "a length-based self-assignment must keep the contents");
  static_assert(selfSubstringDifference() == c_substringLength + 1, "a substring of the string itself must survive");
  static_assert(selfPointerDifference() == c_substringLength + 1,
                "bytes read from the string's own buffer must survive");
  static_assert(selfRangeDifference() == c_tailLength + 1, "a view of the string's own tail must survive");
}

#if !defined(_DEBUG)
// Without the debug checks a rejected assignment keeps the old contents; a single-pass range, already written, empties.
TEST_CASE("string/assignment_rejected") {
  Fixed filled(c_sample);
  filled.assign(c_capacity + 1, c_fillCharacter);
  CHECK(firstDifference(filled, c_sample, c_sampleLength) == c_sampleLength + 1);

  Fixed counted(c_sample);
  counted.assign(c_long, c_longLength);
  CHECK(firstDifference(counted, c_sample, c_sampleLength) == c_sampleLength + 1);

  Fixed pastEnd(c_sample);
  pastEnd.assign(StringView{c_long}, c_longLength + 1);
  CHECK(firstDifference(pastEnd, c_sample, c_sampleLength) == c_sampleLength + 1);

  Fixed forward(c_sample);
  forward.assign(c_long, c_long + c_longLength);
  CHECK(firstDifference(forward, c_sample, c_sampleLength) == c_sampleLength + 1);

  Fixed singlePass(c_sample);
  singlePass.assign(SinglePassIterator(c_long), SinglePassIterator(c_long + c_longLength));
  CHECK(singlePass.size() == 0);

  static_assert(firstDifference(Fixed(c_sample).assign(c_capacity + 1, c_fillCharacter), c_sample, c_sampleLength)
                  == c_sampleLength + 1,
                "a rejected fill must keep the old contents");
  static_assert(firstDifference(Fixed(c_sample).assign(c_long, c_longLength), c_sample, c_sampleLength)
                  == c_sampleLength + 1,
                "a rejected counted copy must keep the old contents");
  static_assert(firstDifference(Fixed(c_sample).assign(StringView{c_long}, c_longLength + 1), c_sample, c_sampleLength)
                  == c_sampleLength + 1,
                "an offset past the end must keep the old contents");
}
#endif // !_DEBUG

// Which assignments a call site may write, which never throw, which stay trivial, and which the type system rejects.
TEST_CASE("string/assignment_signatures") {
  static_assert(std::is_nothrow_copy_assignable_v<Fixed> && std::is_nothrow_move_assignable_v<Fixed>,
                "copying or moving a string into another must not throw");
  static_assert(std::is_nothrow_copy_assignable_v<Large> && std::is_nothrow_move_assignable_v<Large>,
                "a length-based copy must not throw either");
  static_assert(std::is_nothrow_assignable_v<Fixed &, const char *>, "a pointer assignment must not throw");
  static_assert(std::is_nothrow_assignable_v<Fixed &, char>, "a character assignment must not throw");

  static_assert(std::is_trivially_copy_assignable_v<Fixed>, "a small string must keep the trivial copy assignment");
  static_assert(!std::is_trivially_copy_assignable_v<Large>, "a large string must copy only the characters in use");

  static_assert(std::is_assignable_v<Fixed &, StringView>,
                "a view must assign without a cast, as std::basic_string allows, although it cannot construct one");
  static_assert(std::is_assignable_v<Fixed &, Wide>, "a string over another storage must assign without a cast");
  static_assert(std::is_assignable_v<Fixed &, std::initializer_list<char>>, "a braced list must assign");

  static_assert(!std::is_assignable_v<Fixed &, nullptr_t>,
                "the deleted overload must reject a literal nullptr during compilation");
}

// The character each indexed access path yields, and where in the string it reads it.
TEST_CASE("string/element_access") {
  const Fixed string(c_sample);

  for (size_t index = 0; index < c_sampleLength; ++index) {
    INFO("index ", static_cast<long long>(index));
    CHECK(string[index] == c_sample[index]);
    CHECK(string.at(index) == c_sample[index]);
  }

  CHECK(string.front() == c_sample[0]);
  CHECK(string.back() == c_sample[c_sampleLength - 1]);

  // Each access hands back a reference into the string, not a copy of the character.
  CHECK(&string.front() == string.c_str());
  CHECK(&string.back() == string.c_str() + c_sampleLength - 1);

  static_assert(c_sampleString[0] == c_sample[0], "an indexed read must yield the literal's byte at that offset");
  static_assert(c_sampleString.at(c_sampleLength - 1) == c_sample[c_sampleLength - 1],
                "at() must read the same byte as operator[]");
  static_assert(c_sampleString.front() == c_sample[0], "the front character must be the literal's first byte");
  static_assert(c_sampleString.back() == c_sample[c_sampleLength - 1],
                "the back character must be the literal's last byte");
  static_assert(&c_sampleString.back() == c_sampleString.c_str() + c_sampleLength - 1,
                "back() must reference the character before the terminator");
}

// A write through a reference from front(), at(), operator[] or back() changes that character in place.
TEST_CASE("string/element_access_writes") {
  const Fixed string = overwrittenThroughElements();

  CHECK(firstDifference(string, c_overwritten, c_sampleLength) == c_sampleLength + 1);

  static_assert(firstDifference(overwrittenThroughElements(), c_overwritten, c_sampleLength) == c_sampleLength + 1,
                "each writable access must change the character it references and leave the length alone");
}

// operator[] reaches one offset past the characters, where it yields the terminator, on a writable string as well.
TEST_CASE("string/element_access_terminator") {
  Fixed         string(c_sample);
  const Fixed & constString = string;

  CHECK(constString[c_sampleLength] == '\0');
  CHECK(&string[c_sampleLength] == string.c_str() + c_sampleLength);

  const Fixed empty;
  CHECK(empty[0] == '\0');

  static_assert(c_sampleString[c_sampleLength] == '\0', "the offset size() must read the terminator");
  static_assert(Fixed()[0] == '\0', "the offset 0 of an empty string must read the terminator");
}

// data() and c_str() hand out the same buffer, and a write through data() changes a character but not the length.
TEST_CASE("string/pointer_access") {
  Fixed         string(c_sample);
  const Fixed & constString = string;

  CHECK(string.data() == string.c_str());
  CHECK(constString.data() == string.c_str());

  const Fixed written = overwrittenThroughData();
  CHECK(firstDifference(written, c_overwrittenFirst, c_sampleLength) == c_sampleLength + 1);

  static_assert(c_sampleString.data() == c_sampleString.c_str(), "data() must point where c_str() points");
  static_assert(firstDifference(overwrittenThroughData(), c_overwrittenFirst, c_sampleLength) == c_sampleLength + 1,
                "a write through data() must change the character and leave the length alone");
}

// A string passes where a view is taken, and the view reads the string's own characters.
TEST_CASE("string/conversion_to_string_view") {
  const Fixed      string(c_sample);
  const StringView view = string;

  CHECK(view.data() == string.c_str());
  CHECK(view.size() == string.size());

  static_assert(StringView(c_sampleString).data() == c_sampleString.c_str(),
                "the view must read the string's buffer, not a copy");
  static_assert(StringView(c_sampleString).size() == c_sampleLength, "the view must cover every character");
}

// The view measures up to the first null character, so a string holding one converts to a shorter view.
TEST_CASE("string/conversion_to_string_view_embedded_null") {
  constexpr size_t c_viewLength = std::char_traits<char>::length(c_embeddedNull);

  const Fixed      string(c_embeddedNull, c_embeddedNullLength);
  const StringView view = string;

  CHECK(string.size() == c_embeddedNullLength);
  CHECK(view.size() == c_viewLength);

  static_assert(StringView(Fixed(c_embeddedNull, c_embeddedNullLength)).size() == c_viewLength,
                "the view must stop at the first null character");
}

// Which reference and pointer each access returns on a writable and on a read-only string, and how a view is reached.
TEST_CASE("string/element_access_signatures") {
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>()[0]), char &>,
                "operator[] of a writable string must allow writing");
  static_assert(std::is_same_v<decltype(std::declval<const Fixed &>()[0]), const char &>,
                "operator[] of a read-only string must allow reading only");
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().at(0)), char &>,
                "at() of a writable string must allow writing");
  static_assert(std::is_same_v<decltype(std::declval<const Fixed &>().at(0)), const char &>,
                "at() of a read-only string must allow reading only");
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().front()), char &>
                  && std::is_same_v<decltype(std::declval<Fixed &>().back()), char &>,
                "front() and back() of a writable string must allow writing");
  static_assert(std::is_same_v<decltype(std::declval<const Fixed &>().front()), const char &>
                  && std::is_same_v<decltype(std::declval<const Fixed &>().back()), const char &>,
                "front() and back() of a read-only string must allow reading only");
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().data()), char *>,
                "data() of a writable string must allow writing");
  static_assert(std::is_same_v<decltype(std::declval<const Fixed &>().data()), const char *>,
                "data() of a read-only string must allow reading only");

  static_assert(std::is_nothrow_convertible_v<Fixed, StringView>,
                "a string must become a view without a cast, as std::basic_string becomes std::basic_string_view");
}

// A front-to-back walk spans the characters from c_str() up to the terminator, the same through the c overloads.
TEST_CASE("string/iterators") {
  const Fixed string(c_sample);

  CHECK(string.begin() == string.c_str());
  CHECK(string.end() == string.c_str() + c_sampleLength);
  CHECK(string.cbegin() == string.begin());
  CHECK(string.cend() == string.end());
  CHECK(firstDifference(Fixed(string.begin(), string.end()), c_sample, c_sampleLength) == c_sampleLength + 1);

  static_assert(c_sampleString.begin() == c_sampleString.c_str(), "begin() must point at the first character");
  static_assert(c_sampleString.begin() + c_sampleLength == c_sampleString.end(),
                "end() must sit one past the last character");
  static_assert(c_sampleString.cbegin() == c_sampleString.begin() && c_sampleString.cend() == c_sampleString.end(),
                "the c overloads must return what begin() and end() return");
}

// A back-to-front walk yields the characters in reverse and ends before the first one.
TEST_CASE("string/reverse_iterators") {
  const Fixed string(c_sample);

  CHECK(&*string.rbegin() == string.c_str() + c_sampleLength - 1);
  CHECK(string.rend().base() == string.begin());
  CHECK(string.crbegin() == string.rbegin());
  CHECK(string.crend() == string.rend());
  CHECK(firstDifference(Fixed(string.rbegin(), string.rend()), c_sampleReversed, c_sampleLength) == c_sampleLength + 1);

  static_assert(firstDifference(Fixed(c_sampleString.rbegin(), c_sampleString.rend()), c_sampleReversed, c_sampleLength)
                  == c_sampleLength + 1,
                "a reverse walk must yield the characters back to front");
  static_assert(c_sampleString.crbegin() == c_sampleString.rbegin() && c_sampleString.crend() == c_sampleString.rend(),
                "the c overloads must return what rbegin() and rend() return");
}

// An empty string yields an empty walk in both directions.
TEST_CASE("string/iterators_empty") {
  const Fixed empty;

  CHECK(empty.begin() == empty.end());
  CHECK(empty.rbegin() == empty.rend());

  static_assert(c_emptyString.begin() == c_emptyString.end(), "an empty string must have no characters to walk");
  static_assert(c_emptyString.rbegin() == c_emptyString.rend(), "an empty string must have no characters to walk back");
}

// A write through begin() or rbegin() changes that character in place and leaves the length alone.
TEST_CASE("string/iterator_writes") {
  const Fixed string = overwrittenThroughIterators();

  CHECK(firstDifference(string, c_overwrittenEnds, c_sampleLength) == c_sampleLength + 1);

  static_assert(firstDifference(overwrittenThroughIterators(), c_overwrittenEnds, c_sampleLength) == c_sampleLength + 1,
                "each writable iterator must change the character it points at");
}

// Which iterator each overload returns on a writable and on a read-only string, and the range the pair forms.
TEST_CASE("string/iterator_signatures") {
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().begin()), char *>
                  && std::is_same_v<decltype(std::declval<Fixed &>().end()), char *>,
                "begin() and end() of a writable string must allow writing");
  static_assert(std::is_same_v<decltype(std::declval<const Fixed &>().begin()), const char *>
                  && std::is_same_v<decltype(std::declval<const Fixed &>().end()), const char *>,
                "begin() and end() of a read-only string must allow reading only");
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().cbegin()), const char *>
                  && std::is_same_v<decltype(std::declval<Fixed &>().cend()), const char *>,
                "cbegin() and cend() must allow reading only, on a writable string as well");
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().rbegin()), std::reverse_iterator<char *>>
                  && std::is_same_v<decltype(std::declval<Fixed &>().rend()), std::reverse_iterator<char *>>,
                "rbegin() and rend() of a writable string must allow writing");
  static_assert(std::is_same_v<decltype(std::declval<const Fixed &>().rbegin()), std::reverse_iterator<const char *>>
                  && std::is_same_v<decltype(std::declval<const Fixed &>().rend()),
                                    std::reverse_iterator<const char *>>,
                "rbegin() and rend() of a read-only string must allow reading only");
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().crbegin()), std::reverse_iterator<const char *>>
                  && std::is_same_v<decltype(std::declval<Fixed &>().crend()), std::reverse_iterator<const char *>>,
                "crbegin() and crend() must allow reading only, on a writable string as well");

  static_assert(std::ranges::contiguous_range<Fixed> && std::ranges::sized_range<Fixed>,
                "a string must work with the contiguous range algorithms");
}

// A string is empty only while it holds no characters; a lone null character still counts as one.
TEST_CASE("string/empty") {
  CHECK(Fixed().empty());
  CHECK_FALSE(Fixed(c_sample).empty());
  CHECK_FALSE(Fixed(1, '\0').empty());

  static_assert(c_emptyString.empty(), "a default-constructed string must be empty");
  static_assert(!c_sampleString.empty(), "a string with characters must not be empty");
  static_assert(!Fixed(1, '\0').empty(), "a null character must count as a character");
}

// length() reports the same count as size(), embedded null characters included.
TEST_CASE("string/length") {
  const Fixed sample(c_sample);
  const Fixed embedded(c_embeddedNull, c_embeddedNullLength);

  CHECK(sample.length() == c_sampleLength);
  CHECK(embedded.length() == c_embeddedNullLength);
  CHECK(Fixed().length() == 0);

  static_assert(c_sampleString.length() == c_sampleLength, "length() must count the characters");
  static_assert(Fixed(c_embeddedNull, c_embeddedNullLength).length() == c_embeddedNullLength,
                "length() must count an embedded null like any other character");
  static_assert(c_emptyString.length() == 0, "an empty string must have a length of zero");
}

// Over a fixed buffer the longest length is the capacity, whatever the string holds now.
TEST_CASE("string/max_size") {
  CHECK(Fixed().max_size() == c_capacity);
  CHECK(Fixed(c_sample).max_size() == c_capacity);

  static_assert(c_sampleString.max_size() == c_capacity, "max_size() must equal the capacity of a fixed buffer");
  static_assert(FixedString<1>().max_size() == 0, "the smallest buffer must hold no characters at all");
}

// capacity() reports the characters the buffer takes, the terminator excluded, independent of the length.
TEST_CASE("string/capacity") {
  CHECK(Fixed().capacity() == c_capacity);
  CHECK(Fixed(c_sample).capacity() == c_capacity);
  CHECK(Wide().capacity() == c_allocatedSize * 2 - 1);

  static_assert(c_emptyString.capacity() == c_capacity, "capacity must be the buffer size less the terminator");
  static_assert(c_sampleString.capacity() == c_capacity, "capacity must not depend on the length");
}

// An accepted reserve() leaves the characters, the capacity and the buffer as they were.
TEST_CASE("string/reserve") {
  Fixed        string(c_sample);
  const char * buffer = string.c_str();

  string.reserve(0);
  CHECK(firstDifference(string, c_sample, c_sampleLength) == c_sampleLength + 1);

  string.reserve(c_capacity);
  CHECK(firstDifference(string, c_sample, c_sampleLength) == c_sampleLength + 1);
  CHECK(string.capacity() == c_capacity);
  CHECK(string.c_str() == buffer);

  static_assert(firstDifference(reservedSample(0), c_sample, c_sampleLength) == c_sampleLength + 1,
                "a request below the length must keep the characters");
  static_assert(firstDifference(reservedSample(c_capacity), c_sample, c_sampleLength) == c_sampleLength + 1,
                "a request at capacity must keep the characters");
  static_assert(reservedSample(c_capacity).capacity() == c_capacity, "a request at capacity must keep the capacity");
}

#if !defined(_DEBUG)
// Without the debug checks a reserve() the storage rejects leaves the string as it was.
TEST_CASE("string/reserve_rejected") {
  Fixed string(c_sample);
  string.reserve(c_capacity + 1);

  CHECK(firstDifference(string, c_sample, c_sampleLength) == c_sampleLength + 1);
  CHECK(string.capacity() == c_capacity);

  static_assert(firstDifference(reservedSample(c_capacity + 1), c_sample, c_sampleLength) == c_sampleLength + 1,
                "a rejected request must keep the characters");
}
#endif // !_DEBUG

// A shrink request over a fixed buffer keeps the characters, the capacity and the buffer itself.
TEST_CASE("string/shrink_to_fit") {
  Fixed        string(c_sample);
  const char * buffer = string.c_str();

  string.shrink_to_fit();

  CHECK(firstDifference(string, c_sample, c_sampleLength) == c_sampleLength + 1);
  CHECK(string.capacity() == c_capacity);
  CHECK(string.c_str() == buffer);

  static_assert(firstDifference(shrunkSample(), c_sample, c_sampleLength) == c_sampleLength + 1,
                "a shrink request must keep the characters");
  static_assert(shrunkSample().capacity() == c_capacity, "a fixed buffer must keep its capacity");
}

// Which capacity queries and requests never throw, and the type each query returns.
TEST_CASE("string/capacity_signatures") {
  static_assert(noexcept(std::declval<const Fixed &>().empty()) && noexcept(std::declval<const Fixed &>().length())
                  && noexcept(std::declval<const Fixed &>().max_size())
                  && noexcept(std::declval<const Fixed &>().capacity()),
                "the capacity queries must not throw");
  static_assert(noexcept(std::declval<Fixed &>().reserve(0)) && noexcept(std::declval<Fixed &>().shrink_to_fit()),
                "the capacity requests must not throw, where std::basic_string::reserve() throws std::length_error");

  static_assert(std::is_same_v<decltype(std::declval<const Fixed &>().empty()), bool>,
                "empty() must answer with a bool");
  static_assert(std::is_same_v<decltype(std::declval<const Fixed &>().length()), Fixed::size_type>
                  && std::is_same_v<decltype(std::declval<const Fixed &>().max_size()), Fixed::size_type>
                  && std::is_same_v<decltype(std::declval<const Fixed &>().capacity()), Fixed::size_type>,
                "the counting queries must return size_type, as size() does");
}

// clear() removes every character and keeps the buffer with its capacity.
TEST_CASE("string/clear") {
  constexpr auto clear = [](Fixed & string) {
    string.clear();
  };

  Fixed        string(c_sample);
  const char * buffer = string.c_str();
  clear(string);

  CHECK(firstDifference(string, "", 0) == 1);
  CHECK(string.capacity() == c_capacity);
  CHECK(string.c_str() == buffer);

  static_assert(editedDifference(clear, "") == 1, "clear() must leave no characters and a terminator");
}

// Copies of one character go in before the given offset, at the front, inside, or after the last character.
TEST_CASE("string/insertion_fill") {
  constexpr auto front = [](Fixed & string) {
    string.insert(0, c_fillCount, c_fillCharacter);
  };
  constexpr auto inside = [](Fixed & string) {
    string.insert(c_substringOffset, c_fillCount, c_fillCharacter);
  };
  constexpr auto back = [](Fixed & string) {
    string.insert(string.size(), c_fillCount, c_fillCharacter);
  };
  constexpr auto none = [](Fixed & string) {
    string.insert(c_substringOffset, 0, c_fillCharacter);
  };

  CHECK(editedDifference(front, c_filledFront) == fullMatch(c_filledFront));
  CHECK(editedDifference(inside, c_filledInside) == fullMatch(c_filledInside));
  CHECK(editedDifference(back, c_filledBack) == fullMatch(c_filledBack));
  CHECK(editedDifference(none, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(front, c_filledFront) == fullMatch(c_filledFront),
                "copies inserted at offset 0 must precede the old characters");
  static_assert(editedDifference(inside, c_filledInside) == fullMatch(c_filledInside),
                "copies inserted inside must split the old characters at the offset");
  static_assert(editedDifference(back, c_filledBack) == fullMatch(c_filledBack),
                "copies inserted at size() must follow the old characters");
  static_assert(editedDifference(none, c_sample) == fullMatch(c_sample), "a count of zero must change nothing");
}

// A C string goes in up to its terminator; a counted copy takes exactly its count, a null byte included.
TEST_CASE("string/insertion_from_pointer") {
  constexpr auto terminated = [](Fixed & string) {
    string.insert(c_substringOffset, c_listed);
  };
  constexpr auto counted = [](Fixed & string) {
    string.insert(c_substringOffset, c_embeddedNull, c_embeddedNullLength);
  };
  constexpr auto empty = [](Fixed & string) {
    string.insert(c_substringOffset, nullptr, 0);
  };

  CHECK(editedDifference(terminated, c_listedInside) == fullMatch(c_listedInside));
  CHECK(editedDifference(empty, c_sample) == fullMatch(c_sample));

  CHECK(editedDifference(counted, c_embeddedNullInside, c_embeddedNullInsideLength) == c_embeddedNullInsideLength + 1);

  static_assert(editedDifference(terminated, c_listedInside) == fullMatch(c_listedInside),
                "a C string must go in whole at the offset");
  static_assert(editedDifference(empty, c_sample) == fullMatch(c_sample),
                "a null pointer with a count of zero must change nothing");
  static_assert(editedDifference(counted, c_embeddedNullInside, c_embeddedNullInsideLength)
                  == c_embeddedNullInsideLength + 1,
                "a counted copy must take every byte, a null byte included");
}

// A view or a string over another storage goes in whole, or as the substring that starts at an offset into it.
TEST_CASE("string/insertion_from_string_like") {
  constexpr auto view = [](Fixed & string) {
    string.insert(c_substringOffset, StringView{c_listed});
  };
  constexpr auto wide = [](Fixed & string) {
    string.insert(c_substringOffset, Wide(c_listed));
  };
  constexpr auto substring = [](Fixed & string) {
    string.insert(c_substringOffset, StringView{c_sample}, 0, c_substringOffset);
  };
  constexpr auto toEnd = [](Fixed & string) {
    string.insert(string.size(), StringView{c_sample}, c_substringOffset);
  };

  CHECK(editedDifference(view, c_listedInside) == fullMatch(c_listedInside));
  CHECK(editedDifference(wide, c_listedInside) == fullMatch(c_listedInside));
  CHECK(editedDifference(substring, "plplayer") == fullMatch("plplayer"));
  CHECK(editedDifference(toEnd, "playerayer") == fullMatch("playerayer"));

  static_assert(editedDifference(view, c_listedInside) == fullMatch(c_listedInside), "a view must go in whole");
  static_assert(editedDifference(wide, c_listedInside) == fullMatch(c_listedInside),
                "a string over another storage must go in whole");
  static_assert(editedDifference(substring, "plplayer") == fullMatch("plplayer"),
                "a substring must take at most count bytes from the offset");
  static_assert(editedDifference(toEnd, "playerayer") == fullMatch("playerayer"),
                "the default count must take the source up to its end");
}

// Insertion at an iterator puts the characters where it pointed and returns an iterator to the first of them.
TEST_CASE("string/insertion_at_iterator") {
  constexpr auto single = [](Fixed & string) {
    return string.insert(string.begin() + 1, c_fillCharacter);
  };
  constexpr auto filled = [](Fixed & string) {
    return string.insert(string.cbegin() + c_substringOffset, c_fillCount, c_fillCharacter);
  };
  constexpr auto none = [](Fixed & string) {
    return string.insert(string.end(), 0, c_fillCharacter);
  };

  CHECK(editedDifference(single, "pxlayer") == fullMatch("pxlayer"));
  CHECK(returnedOffset(single) == 1);
  CHECK(editedDifference(filled, c_filledInside) == fullMatch(c_filledInside));
  CHECK(returnedOffset(filled) == c_substringOffset);
  CHECK(editedDifference(none, c_sample) == fullMatch(c_sample));
  CHECK(returnedOffset(none) == c_sampleLength);

  static_assert(editedDifference(single, "pxlayer") == fullMatch("pxlayer"),
                "one character must go in before the iterator");
  static_assert(returnedOffset(single) == 1, "the result must point at the inserted character");
  static_assert(editedDifference(filled, c_filledInside) == fullMatch(c_filledInside),
                "a const_iterator must take copies before it");
  static_assert(returnedOffset(filled) == c_substringOffset, "the result must point at the first copy");
  static_assert(returnedOffset(none) == c_sampleLength, "a count of zero must return the position it was given");
}

// An iterator range goes in whole, whether it is contiguous, forward only, or single-pass.
TEST_CASE("string/insertion_from_iterators") {
  constexpr auto contiguous = [](Fixed & string) {
    return string.insert(string.begin() + c_substringOffset, c_listed, c_listed + c_listedLength);
  };
  constexpr auto forward = [](Fixed & string) {
    return string.insert(string.begin() + c_substringOffset, std::reverse_iterator(c_listed + c_listedLength),
                         std::reverse_iterator(c_listed));
  };
  constexpr auto singlePass = [](Fixed & string) {
    return string.insert(string.begin() + c_substringOffset, SinglePassIterator(c_listed),
                         SinglePassIterator(c_listed + c_listedLength));
  };
  constexpr auto empty = [](Fixed & string) {
    return string.insert(string.begin() + c_substringOffset, c_listed, c_listed);
  };

  CHECK(editedDifference(contiguous, c_listedInside) == fullMatch(c_listedInside));
  CHECK(editedDifference(forward, c_listedReversedInside) == fullMatch(c_listedReversedInside));
  CHECK(editedDifference(singlePass, c_listedInside) == fullMatch(c_listedInside));
  CHECK(returnedOffset(singlePass) == c_substringOffset);
  CHECK(editedDifference(empty, c_sample) == fullMatch(c_sample));
  CHECK(returnedOffset(empty) == c_substringOffset);

  static_assert(editedDifference(contiguous, c_listedInside) == fullMatch(c_listedInside),
                "a pointer range must go in before the iterator");
  static_assert(editedDifference(forward, c_listedReversedInside) == fullMatch(c_listedReversedInside),
                "a forward range must go in in its own order");
  static_assert(editedDifference(singlePass, c_listedInside) == fullMatch(c_listedInside),
                "a single-pass range must be rotated into place");
  static_assert(returnedOffset(singlePass) == c_substringOffset, "the result must point at the first inserted one");
  static_assert(editedDifference(empty, c_sample) == fullMatch(c_sample), "an empty range must change nothing");
}

// A braced list goes in before the iterator, as the same characters in a pointer range would.
TEST_CASE("string/insertion_from_initializer_list") {
  constexpr auto listed = [](Fixed & string) {
    return string.insert(string.begin() + c_substringOffset, {'a', 'b', 'c'});
  };

  CHECK(editedDifference(listed, c_listedInside) == fullMatch(c_listedInside));
  CHECK(returnedOffset(listed) == c_substringOffset);

  static_assert(editedDifference(listed, c_listedInside) == fullMatch(c_listedInside),
                "a braced list must go in before the iterator");
  static_assert(returnedOffset(listed) == c_substringOffset, "the result must point at the first listed character");
}

// insert_range() takes a range whose end may have a type of its own, single-pass ranges included.
TEST_CASE("string/insertion_from_range") {
  constexpr auto view = [](Fixed & string) {
    return string.insert_range(string.begin() + c_substringOffset, StringView{c_listed});
  };
  constexpr auto singlePass = [](Fixed & string) {
    return string.insert_range(string.begin() + c_substringOffset,
                               std::ranges::subrange(SinglePassIterator(c_listed),
                                                     SinglePassIterator(c_listed + c_listedLength)));
  };

  CHECK(editedDifference(view, c_listedInside) == fullMatch(c_listedInside));
  CHECK(returnedOffset(view) == c_substringOffset);
  CHECK(editedDifference(singlePass, c_listedInside) == fullMatch(c_listedInside));

  static_assert(editedDifference(view, c_listedInside) == fullMatch(c_listedInside),
                "a contiguous range must go in before the iterator");
  static_assert(returnedOffset(view) == c_substringOffset, "the result must point at the first inserted character");
  static_assert(editedDifference(singlePass, c_listedInside) == fullMatch(c_listedInside),
                "a single-pass range must be rotated into place");
}

// Characters read from the string itself go in unchanged, wherever they lie and even when the buffer moves to grow.
TEST_CASE("string/insertion_from_itself") {
  constexpr auto whole = [](Relocating & string) {
    string.insert(c_substringOffset, string);
  };
  constexpr auto iterated = [](Relocating & string) {
    string.insert(string.begin() + c_substringOffset, string.begin(), string.end());
  };
  constexpr auto ranged = [](Relocating & string) {
    string.insert_range(string.begin() + c_substringOffset, StringView{string.c_str()});
  };

  Relocating   relocating(c_sample);
  const char * buffer = relocating.c_str();
  whole(relocating);
  REQUIRE(relocating.c_str() != buffer);
  CHECK(firstDifference(relocating, c_sampleDoubledInside, c_sampleLength * 2) == c_sampleLength * 2 + 1);

  // Names the triple that breaks, where the static_assert below only counts them.
  const auto checkSelfInsertion = [&](size_t position, size_t offset, size_t count, size_t difference, size_t match) {
    INFO("position ", static_cast<long long>(position));
    INFO("offset ", static_cast<long long>(offset));
    INFO("count ", static_cast<long long>(count));
    CHECK(difference == match);
  };

  forEachSelfInsertion<Fixed>(checkSelfInsertion);
  forEachSelfInsertion<Relocating>(checkSelfInsertion);
  CHECK(editedDifference<Relocating>(iterated, c_sampleDoubledInside) == fullMatch(c_sampleDoubledInside));
  CHECK(editedDifference<Relocating>(ranged, c_sampleDoubledInside) == fullMatch(c_sampleDoubledInside));

  static_assert(selfInsertionFailures<Fixed>() == 0, "every slice of the string must survive insertion into itself");
  static_assert(selfInsertionFailures<Relocating>() == 0, "every slice must survive a buffer that moves to grow");
  static_assert(editedDifference<Relocating>(whole, c_sampleDoubledInside) == fullMatch(c_sampleDoubledInside),
                "the string must go into itself whole");
  static_assert(editedDifference<Relocating>(iterated, c_sampleDoubledInside) == fullMatch(c_sampleDoubledInside),
                "an iterator range over the string must go into itself");
  static_assert(editedDifference<Relocating>(ranged, c_sampleDoubledInside) == fullMatch(c_sampleDoubledInside),
                "a view of the string must go into itself");
}

#if !defined(_DEBUG)
// Without the debug checks a rejected insertion keeps the old contents, a single-pass range included.
TEST_CASE("string/insertion_rejected") {
  constexpr auto pastEnd = [](Fixed & string) {
    string.insert(c_sampleLength + 1, c_listed);
  };
  constexpr auto filled = [](Fixed & string) {
    string.insert(0, c_capacity, c_fillCharacter);
  };
  constexpr auto counted = [](Fixed & string) {
    string.insert(0, c_long, c_longLength);
  };
  constexpr auto substring = [](Fixed & string) {
    string.insert(0, StringView{c_listed}, c_listedLength + 1);
  };
  constexpr auto forward = [](Fixed & string) {
    string.insert(string.begin(), std::reverse_iterator(c_long + c_longLength), std::reverse_iterator(c_long));
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.insert(string.begin(), SinglePassIterator(c_long), SinglePassIterator(c_long + c_longLength));
  };

  CHECK(editedDifference(pastEnd, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(filled, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(counted, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(substring, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(forward, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(singlePass, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(pastEnd, c_sample) == fullMatch(c_sample),
                "an offset past the end must keep the old contents");
  static_assert(editedDifference(filled, c_sample) == fullMatch(c_sample),
                "a rejected fill must keep the old contents");
  static_assert(editedDifference(counted, c_sample) == fullMatch(c_sample),
                "a rejected counted copy must keep the old contents");
  static_assert(editedDifference(substring, c_sample) == fullMatch(c_sample),
                "a substring offset past the end must keep the old contents");
  static_assert(editedDifference(forward, c_sample) == fullMatch(c_sample),
                "a rejected forward range must keep the old contents");
  static_assert(editedDifference(singlePass, c_sample) == fullMatch(c_sample),
                "a rejected single-pass range must roll back what it appended");
}
#endif // !_DEBUG

// erase() takes up to count characters from an offset and stops at the end; the defaults take everything.
TEST_CASE("string/erasure") {
  constexpr auto middle = [](Fixed & string) {
    string.erase(c_substringOffset, c_substringLength);
  };
  constexpr auto tail = [](Fixed & string) {
    string.erase(c_substringOffset);
  };
  constexpr auto beyond = [](Fixed & string) {
    string.erase(c_substringOffset, c_longLength);
  };
  constexpr auto atEnd = [](Fixed & string) {
    string.erase(string.size(), c_substringLength);
  };
  constexpr auto all = [](Fixed & string) {
    string.erase();
  };

  CHECK(editedDifference(middle, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle));
  CHECK(editedDifference(tail, c_sampleHead) == fullMatch(c_sampleHead));
  CHECK(editedDifference(beyond, c_sampleHead) == fullMatch(c_sampleHead));
  CHECK(editedDifference(atEnd, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(all, "") == 1);

  static_assert(editedDifference(middle, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle),
                "the characters after the erased ones must close the gap");
  static_assert(editedDifference(tail, c_sampleHead) == fullMatch(c_sampleHead),
                "the default count must erase to the end");
  static_assert(editedDifference(beyond, c_sampleHead) == fullMatch(c_sampleHead),
                "a count past the end must stop at the end");
  static_assert(editedDifference(atEnd, c_sample) == fullMatch(c_sample), "an offset of size() must erase nothing");
  static_assert(editedDifference(all, "") == 1, "the defaults must erase every character");
}

// Erasure at an iterator or over an iterator range returns an iterator to the character that followed.
TEST_CASE("string/erasure_at_iterator") {
  constexpr auto single = [](Fixed & string) {
    return string.erase(string.begin() + 1);
  };
  constexpr auto last = [](Fixed & string) {
    return string.erase(string.cend() - 1);
  };
  constexpr auto range = [](Fixed & string) {
    return string.erase(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength);
  };
  constexpr auto empty = [](Fixed & string) {
    return string.erase(string.cbegin() + 1, string.cbegin() + 1);
  };

  CHECK(editedDifference(single, "payer") == fullMatch("payer"));
  CHECK(returnedOffset(single) == 1);
  CHECK(editedDifference(last, "playe") == fullMatch("playe"));
  CHECK(returnedOffset(last) == c_sampleLength - 1);
  CHECK(editedDifference(range, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle));
  CHECK(returnedOffset(range) == c_substringOffset);
  CHECK(editedDifference(empty, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(single, "payer") == fullMatch("payer"), "the character at the iterator must go");
  static_assert(returnedOffset(single) == 1, "the result must point at the character that followed");
  static_assert(returnedOffset(last) == c_sampleLength - 1, "erasing the last character must return end()");
  static_assert(editedDifference(range, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle),
                "a mixed iterator pair must erase the range between them");
  static_assert(returnedOffset(range) == c_substringOffset, "the result must point where the range began");
  static_assert(editedDifference(empty, c_sample) == fullMatch(c_sample), "an empty range must erase nothing");
}

#if !defined(_DEBUG)
// Without the debug checks an erasure past the end, at end(), or over a reversed or overlong range keeps the contents.
TEST_CASE("string/erasure_rejected") {
  constexpr auto pastEnd = [](Fixed & string) {
    string.erase(c_sampleLength + 1);
  };
  constexpr auto atEnd = [](Fixed & string) {
    string.erase(string.end());
  };
  constexpr auto reversed = [](Fixed & string) {
    string.erase(string.begin() + c_substringOffset, string.cbegin());
  };
  constexpr auto overlong = [](Fixed & string) {
    string.erase(string.begin(), string.cend() + 1);
  };

  CHECK(editedDifference(pastEnd, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(atEnd, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(reversed, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(overlong, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(pastEnd, c_sample) == fullMatch(c_sample),
                "an offset past the end must keep the old contents");
  static_assert(editedDifference(atEnd, c_sample) == fullMatch(c_sample), "end() must keep the old contents");
  static_assert(editedDifference(reversed, c_sample) == fullMatch(c_sample),
                "a reversed range must keep the old contents");
  static_assert(editedDifference(overlong, c_sample) == fullMatch(c_sample),
                "a range that ends past end() must keep the old contents");
}
#endif // !_DEBUG

// push_back() puts one character after the last one, and the terminator after it.
TEST_CASE("string/push_back") {
  constexpr auto pushed = [](Fixed & string) {
    string.push_back(c_fillCharacter);
  };

  CHECK(editedDifference(pushed, "playerx") == fullMatch("playerx"));

  static_assert(editedDifference(pushed, "playerx") == fullMatch("playerx"),
                "the character must follow the old ones and precede the terminator");
}

// pop_back() drops the last character and moves the terminator onto its place.
TEST_CASE("string/pop_back") {
  constexpr auto popped = [](Fixed & string) {
    string.pop_back();
  };
  constexpr auto emptied = [](Fixed & string) {
    string.assign(c_sample, 1);
    string.pop_back();
  };

  CHECK(editedDifference(popped, "playe") == fullMatch("playe"));
  CHECK(editedDifference(emptied, "") == 1);

  static_assert(editedDifference(popped, "playe") == fullMatch("playe"), "the last character must go");
  static_assert(editedDifference(emptied, "") == 1, "popping the only character must leave an empty string");
}

#if !defined(_DEBUG)
// Without the debug checks pop_back() on an empty string leaves it empty.
TEST_CASE("string/pop_back_rejected") {
  constexpr auto poppedEmpty = [](Fixed & string) {
    string.clear();
    string.pop_back();
  };

  CHECK(editedDifference(poppedEmpty, "") == 1);

  static_assert(editedDifference(poppedEmpty, "") == 1, "an empty string must stay empty");
}
#endif // !_DEBUG

// Copies of one character go after the last character; a count of zero adds nothing.
TEST_CASE("string/appending_fill") {
  constexpr auto filled = [](Fixed & string) {
    string.append(c_fillCount, c_fillCharacter);
  };
  constexpr auto none = [](Fixed & string) {
    string.append(0, c_fillCharacter);
  };

  CHECK(editedDifference(filled, c_filledBack) == fullMatch(c_filledBack));
  CHECK(editedDifference(none, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(filled, c_filledBack) == fullMatch(c_filledBack),
                "copies must follow the old characters");
  static_assert(editedDifference(none, c_sample) == fullMatch(c_sample), "a count of zero must change nothing");
}

// A C string goes after the last character up to its terminator; a counted copy takes exactly its count.
TEST_CASE("string/appending_from_pointer") {
  constexpr auto terminated = [](Fixed & string) {
    string.append(c_listed);
  };
  constexpr auto counted = [](Fixed & string) {
    string.append(c_embeddedNull, c_embeddedNullLength);
  };
  constexpr auto empty = [](Fixed & string) {
    string.append(nullptr, 0);
  };

  CHECK(editedDifference(terminated, c_listedAppended) == fullMatch(c_listedAppended));
  CHECK(editedDifference(counted, c_embeddedNullAppended, c_embeddedNullAppendedLength)
        == c_embeddedNullAppendedLength + 1);
  CHECK(editedDifference(empty, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(terminated, c_listedAppended) == fullMatch(c_listedAppended),
                "a C string must go in whole after the old characters");
  static_assert(editedDifference(counted, c_embeddedNullAppended, c_embeddedNullAppendedLength)
                  == c_embeddedNullAppendedLength + 1,
                "a counted copy must take every byte, a null byte included");
  static_assert(editedDifference(empty, c_sample) == fullMatch(c_sample),
                "a null pointer with a count of zero must change nothing");
}

// A view or a string over another storage goes after the last character, whole or as a substring.
TEST_CASE("string/appending_from_string_like") {
  constexpr auto view = [](Fixed & string) {
    string.append(StringView{c_listed});
  };
  constexpr auto wide = [](Fixed & string) {
    string.append(Wide(c_listed));
  };
  constexpr auto substring = [](Fixed & string) {
    string.append(StringView{c_sample}, 0, c_substringOffset);
  };
  constexpr auto toEnd = [](Fixed & string) {
    string.append(StringView{c_sample}, c_substringOffset);
  };

  CHECK(editedDifference(view, c_listedAppended) == fullMatch(c_listedAppended));
  CHECK(editedDifference(wide, c_listedAppended) == fullMatch(c_listedAppended));
  CHECK(editedDifference(substring, "playerpl") == fullMatch("playerpl"));
  CHECK(editedDifference(toEnd, "playerayer") == fullMatch("playerayer"));

  static_assert(editedDifference(view, c_listedAppended) == fullMatch(c_listedAppended), "a view must go in whole");
  static_assert(editedDifference(wide, c_listedAppended) == fullMatch(c_listedAppended),
                "a string over another storage must go in whole");
  static_assert(editedDifference(substring, "playerpl") == fullMatch("playerpl"),
                "a substring must take at most count bytes from the offset");
  static_assert(editedDifference(toEnd, "playerayer") == fullMatch("playerayer"),
                "the default count must take the source up to its end");
}

// An iterator range goes after the last character, whether it is contiguous, forward only, or single-pass.
TEST_CASE("string/appending_from_iterators") {
  constexpr auto contiguous = [](Fixed & string) {
    string.append(c_listed, c_listed + c_listedLength);
  };
  constexpr auto forward = [](Fixed & string) {
    string.append(std::reverse_iterator(c_listed + c_listedLength), std::reverse_iterator(c_listed));
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.append(SinglePassIterator(c_listed), SinglePassIterator(c_listed + c_listedLength));
  };
  constexpr auto empty = [](Fixed & string) {
    string.append(c_listed, c_listed);
  };

  CHECK(editedDifference(contiguous, c_listedAppended) == fullMatch(c_listedAppended));
  CHECK(editedDifference(forward, c_listedReversedAppended) == fullMatch(c_listedReversedAppended));
  CHECK(editedDifference(singlePass, c_listedAppended) == fullMatch(c_listedAppended));
  CHECK(editedDifference(empty, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(contiguous, c_listedAppended) == fullMatch(c_listedAppended),
                "a pointer range must follow the old characters");
  static_assert(editedDifference(forward, c_listedReversedAppended) == fullMatch(c_listedReversedAppended),
                "a forward range must go in in its own order");
  static_assert(editedDifference(singlePass, c_listedAppended) == fullMatch(c_listedAppended),
                "a single-pass range must follow the old characters");
  static_assert(editedDifference(empty, c_sample) == fullMatch(c_sample), "an empty range must change nothing");
}

// A braced list goes after the last character, as the same characters in a pointer range would.
TEST_CASE("string/appending_from_initializer_list") {
  constexpr auto listed = [](Fixed & string) {
    string.append({'a', 'b', 'c'});
  };

  CHECK(editedDifference(listed, c_listedAppended) == fullMatch(c_listedAppended));

  static_assert(editedDifference(listed, c_listedAppended) == fullMatch(c_listedAppended),
                "a braced list must follow the old characters");
}

// append_range() takes a range whose end may have a type of its own, single-pass ranges included.
TEST_CASE("string/appending_from_range") {
  constexpr auto view = [](Fixed & string) {
    string.append_range(StringView{c_listed});
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.append_range(std::ranges::subrange(SinglePassIterator(c_listed),
                                              SinglePassIterator(c_listed + c_listedLength)));
  };

  CHECK(editedDifference(view, c_listedAppended) == fullMatch(c_listedAppended));
  CHECK(editedDifference(singlePass, c_listedAppended) == fullMatch(c_listedAppended));

  static_assert(editedDifference(view, c_listedAppended) == fullMatch(c_listedAppended),
                "a contiguous range must follow the old characters");
  static_assert(editedDifference(singlePass, c_listedAppended) == fullMatch(c_listedAppended),
                "a single-pass range must follow the old characters");
}

// The string appended to itself comes out doubled, even when the buffer moves to grow.
TEST_CASE("string/appending_from_itself") {
  constexpr auto whole = [](Relocating & string) {
    string.append(string);
  };
  constexpr auto iterated = [](Relocating & string) {
    string.append(string.begin(), string.end());
  };
  constexpr auto ranged = [](Relocating & string) {
    string.append_range(StringView{string.c_str()});
  };

  CHECK(editedDifference<Relocating>(whole, c_sampleDoubled) == fullMatch(c_sampleDoubled));
  CHECK(editedDifference<Relocating>(iterated, c_sampleDoubled) == fullMatch(c_sampleDoubled));
  CHECK(editedDifference<Relocating>(ranged, c_sampleDoubled) == fullMatch(c_sampleDoubled));

  static_assert(editedDifference<Relocating>(whole, c_sampleDoubled) == fullMatch(c_sampleDoubled),
                "the string must go after itself whole");
  static_assert(editedDifference<Relocating>(iterated, c_sampleDoubled) == fullMatch(c_sampleDoubled),
                "an iterator range over the string must go after itself");
  static_assert(editedDifference<Relocating>(ranged, c_sampleDoubled) == fullMatch(c_sampleDoubled),
                "a view of the string must go after itself");
}

#if !defined(_DEBUG)
// Without the debug checks an append that does not fit keeps the old contents, a single-pass range included.
TEST_CASE("string/appending_rejected") {
  constexpr auto pushedFull = [](Fixed & string) {
    string.assign(c_long, c_capacity);
    string.push_back(c_fillCharacter);
  };
  constexpr auto filled = [](Fixed & string) {
    string.append(c_capacity, c_fillCharacter);
  };
  constexpr auto terminated = [](Fixed & string) {
    string.append(c_long);
  };
  constexpr auto substring = [](Fixed & string) {
    string.append(StringView{c_listed}, c_listedLength + 1);
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.append(SinglePassIterator(c_long), SinglePassIterator(c_long + c_longLength));
  };
  constexpr auto compound = [](Fixed & string) {
    string += c_long;
  };

  CHECK(editedDifference(pushedFull, c_long, c_capacity) == c_capacity + 1);
  CHECK(editedDifference(filled, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(terminated, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(substring, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(singlePass, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(compound, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(pushedFull, c_long, c_capacity) == c_capacity + 1,
                "push_back() on a full string must keep the old contents");
  static_assert(editedDifference(filled, c_sample) == fullMatch(c_sample),
                "a rejected fill must keep the old contents");
  static_assert(editedDifference(terminated, c_sample) == fullMatch(c_sample),
                "a rejected C string must keep the old contents");
  static_assert(editedDifference(substring, c_sample) == fullMatch(c_sample),
                "a substring offset past the end must keep the old contents");
  static_assert(editedDifference(singlePass, c_sample) == fullMatch(c_sample),
                "a rejected single-pass range must roll back what it appended");
  static_assert(editedDifference(compound, c_sample) == fullMatch(c_sample),
                "a rejected operator+= must keep the old contents");
}
#endif // !_DEBUG

// operator+= appends a character, a C string, a braced list, or a string-like source.
TEST_CASE("string/compound_assignment") {
  constexpr auto character = [](Fixed & string) {
    string += c_fillCharacter;
  };
  constexpr auto terminated = [](Fixed & string) {
    string += c_listed;
  };
  constexpr auto listed = [](Fixed & string) {
    string += {'a', 'b', 'c'};
  };
  constexpr auto view = [](Fixed & string) {
    string += StringView{c_listed};
  };
  constexpr auto wide = [](Fixed & string) {
    string += Wide(c_listed);
  };

  CHECK(editedDifference(character, "playerx") == fullMatch("playerx"));
  CHECK(editedDifference(terminated, c_listedAppended) == fullMatch(c_listedAppended));
  CHECK(editedDifference(listed, c_listedAppended) == fullMatch(c_listedAppended));
  CHECK(editedDifference(view, c_listedAppended) == fullMatch(c_listedAppended));
  CHECK(editedDifference(wide, c_listedAppended) == fullMatch(c_listedAppended));

  static_assert(editedDifference(character, "playerx") == fullMatch("playerx"), "a character must follow the old ones");
  static_assert(editedDifference(terminated, c_listedAppended) == fullMatch(c_listedAppended),
                "a C string must follow the old characters");
  static_assert(editedDifference(listed, c_listedAppended) == fullMatch(c_listedAppended),
                "a braced list must follow the old characters");
  static_assert(editedDifference(view, c_listedAppended) == fullMatch(c_listedAppended),
                "a view must follow the old characters");
  static_assert(editedDifference(wide, c_listedAppended) == fullMatch(c_listedAppended),
                "a string over another storage must follow the old characters");
}

// replace() removes up to count characters from an offset, stopping at the end, and puts the source in their place.
TEST_CASE("string/replacement") {
  constexpr auto equal = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, c_listed);
  };
  constexpr auto growing = [](Fixed & string) {
    string.replace(c_substringOffset, 1, c_listed);
  };
  constexpr auto shrinking = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, c_listed, 1);
  };
  constexpr auto beyond = [](Fixed & string) {
    string.replace(c_substringOffset, c_longLength, c_listed);
  };
  constexpr auto atEnd = [](Fixed & string) {
    string.replace(string.size(), c_substringLength, c_listed);
  };

  CHECK(editedDifference(equal, c_listedReplacing) == fullMatch(c_listedReplacing));
  CHECK(editedDifference(growing, "plabcyer") == fullMatch("plabcyer"));
  CHECK(editedDifference(shrinking, "plar") == fullMatch("plar"));
  CHECK(editedDifference(beyond, "plabc") == fullMatch("plabc"));
  CHECK(editedDifference(atEnd, c_listedAppended) == fullMatch(c_listedAppended));

  static_assert(editedDifference(equal, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a source as long as the run must take its place");
  static_assert(editedDifference(growing, "plabcyer") == fullMatch("plabcyer"),
                "a longer source must push the tail right");
  static_assert(editedDifference(shrinking, "plar") == fullMatch("plar"), "a shorter source must pull the tail left");
  static_assert(editedDifference(beyond, "plabc") == fullMatch("plabc"), "a count past the end must stop at the end");
  static_assert(editedDifference(atEnd, c_listedAppended) == fullMatch(c_listedAppended),
                "an offset of size() must remove nothing and append the source");
}

// A counted source takes exactly its count, a null byte included; a null pointer with a count of zero only removes.
TEST_CASE("string/replacement_from_pointer") {
  constexpr auto counted = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, c_embeddedNull, c_embeddedNullLength);
  };
  constexpr auto empty = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, nullptr, 0);
  };

  CHECK(editedDifference(counted, c_embeddedNullReplacing, c_embeddedNullReplacingLength)
        == c_embeddedNullReplacingLength + 1);
  CHECK(editedDifference(empty, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle));

  static_assert(editedDifference(counted, c_embeddedNullReplacing, c_embeddedNullReplacingLength)
                  == c_embeddedNullReplacingLength + 1,
                "a counted source must take every byte, a null byte included");
  static_assert(editedDifference(empty, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle),
                "a null pointer with a count of zero must only remove the run");
}

// Copies of one character take the place of the run; a count of zero only removes it.
TEST_CASE("string/replacement_fill") {
  constexpr auto filled = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, c_fillCount, c_fillCharacter);
  };
  constexpr auto none = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, 0, c_fillCharacter);
  };
  constexpr auto atEnd = [](Fixed & string) {
    string.replace(string.size(), 0, c_fillCount, c_fillCharacter);
  };

  CHECK(editedDifference(filled, c_filledReplacing) == fullMatch(c_filledReplacing));
  CHECK(editedDifference(none, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle));
  CHECK(editedDifference(atEnd, c_filledBack) == fullMatch(c_filledBack));

  static_assert(editedDifference(filled, c_filledReplacing) == fullMatch(c_filledReplacing),
                "copies must take the place of the run");
  static_assert(editedDifference(none, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle),
                "a count of zero must only remove the run");
  static_assert(editedDifference(atEnd, c_filledBack) == fullMatch(c_filledBack),
                "copies at size() must follow the old characters");
}

// A view or a string over another storage takes the place of the run, whole or as a substring.
TEST_CASE("string/replacement_from_string_like") {
  constexpr auto view = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, StringView{c_listed});
  };
  constexpr auto wide = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, Wide(c_listed));
  };
  constexpr auto substring = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, StringView{c_listed}, 1, 1);
  };
  constexpr auto toEnd = [](Fixed & string) {
    string.replace(c_substringOffset, c_substringLength, StringView{c_listed}, 1);
  };

  CHECK(editedDifference(view, c_listedReplacing) == fullMatch(c_listedReplacing));
  CHECK(editedDifference(wide, c_listedReplacing) == fullMatch(c_listedReplacing));
  CHECK(editedDifference(substring, "plbr") == fullMatch("plbr"));
  CHECK(editedDifference(toEnd, "plbcr") == fullMatch("plbcr"));

  static_assert(editedDifference(view, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a view must take the place of the run");
  static_assert(editedDifference(wide, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a string over another storage must take the place of the run");
  static_assert(editedDifference(substring, "plbr") == fullMatch("plbr"),
                "a substring must take at most count bytes from the offset");
  static_assert(editedDifference(toEnd, "plbcr") == fullMatch("plbcr"),
                "the default count must take the source up to its end");
}

// Each source takes the place of the characters between an iterator and a const_iterator.
TEST_CASE("string/replacement_at_iterators") {
  constexpr auto counted = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   c_listed, 2);
  };
  constexpr auto terminated = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   c_listed);
  };
  constexpr auto view = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   StringView{c_listed});
  };
  constexpr auto filled = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   c_fillCount, c_fillCharacter);
  };
  constexpr auto listed = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   {'a', 'b', 'c'});
  };
  constexpr auto empty = [](Fixed & string) {
    string.replace(string.begin() + 1, string.cbegin() + 1, c_listed);
  };

  CHECK(editedDifference(counted, "plabr") == fullMatch("plabr"));
  CHECK(editedDifference(terminated, c_listedReplacing) == fullMatch(c_listedReplacing));
  CHECK(editedDifference(view, c_listedReplacing) == fullMatch(c_listedReplacing));
  CHECK(editedDifference(filled, c_filledReplacing) == fullMatch(c_filledReplacing));
  CHECK(editedDifference(listed, c_listedReplacing) == fullMatch(c_listedReplacing));
  CHECK(editedDifference(empty, "pabclayer") == fullMatch("pabclayer"));

  static_assert(editedDifference(counted, "plabr") == fullMatch("plabr"),
                "a counted source must take the place of the range");
  static_assert(editedDifference(terminated, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a C string must take the place of the range");
  static_assert(editedDifference(view, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a view must take the place of the range");
  static_assert(editedDifference(filled, c_filledReplacing) == fullMatch(c_filledReplacing),
                "copies must take the place of the range");
  static_assert(editedDifference(listed, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a braced list must take the place of the range");
  static_assert(editedDifference(empty, "pabclayer") == fullMatch("pabclayer"),
                "an empty range must insert the source at the iterator");
}

// An iterator range takes the place of the run, whether it is contiguous, forward only, or single-pass.
TEST_CASE("string/replacement_from_iterators") {
  constexpr auto contiguous = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   c_listed, c_listed + c_listedLength);
  };
  constexpr auto forward = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   std::reverse_iterator(c_listed + c_listedLength), std::reverse_iterator(c_listed));
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   SinglePassIterator(c_listed), SinglePassIterator(c_listed + c_listedLength));
  };
  constexpr auto empty = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   c_listed, c_listed);
  };

  CHECK(editedDifference(contiguous, c_listedReplacing) == fullMatch(c_listedReplacing));
  CHECK(editedDifference(forward, c_listedReversedReplacing) == fullMatch(c_listedReversedReplacing));
  CHECK(editedDifference(singlePass, c_listedReplacing) == fullMatch(c_listedReplacing));
  CHECK(editedDifference(empty, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle));

  static_assert(editedDifference(contiguous, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a pointer range must take the place of the run");
  static_assert(editedDifference(forward, c_listedReversedReplacing) == fullMatch(c_listedReversedReplacing),
                "a forward range must go in in its own order");
  static_assert(editedDifference(singlePass, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a single-pass range must be rotated into place");
  static_assert(editedDifference(empty, c_sampleWithoutMiddle) == fullMatch(c_sampleWithoutMiddle),
                "an empty range must only remove the run");
}

// A single-pass source shorter or longer than the run fits, and needs room only for the final length.
TEST_CASE("string/replacement_from_single_pass") {
  constexpr auto shorter = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_substringLength,
                   SinglePassIterator(c_listed), SinglePassIterator(c_listed + 1));
  };
  constexpr auto longer = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + 1,
                   SinglePassIterator(c_listed), SinglePassIterator(c_listed + c_listedLength));
  };
  constexpr auto nearlyFull = [](Fixed & string) {
    string.assign(c_long, c_capacity);
    string.replace_with_range(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + c_fillCount,
                              std::ranges::subrange(SinglePassIterator(c_listed),
                                                    SinglePassIterator(c_listed + c_listedLength)));
  };

  CHECK(editedDifference(shorter, "plar") == fullMatch("plar"));
  CHECK(editedDifference(longer, "plabcyer") == fullMatch("plabcyer"));
  CHECK(editedDifference(nearlyFull, "ababchijklmno") == fullMatch("ababchijklmno"));

  static_assert(editedDifference(shorter, "plar") == fullMatch("plar"),
                "a source that ends inside the run must close up the rest of it");
  static_assert(editedDifference(longer, "plabcyer") == fullMatch("plabcyer"),
                "what follows the run must make room for the rest of the source");
  static_assert(editedDifference(nearlyFull, "ababchijklmno") == fullMatch("ababchijklmno"),
                "a full string must take a shorter single-pass replacement");
}

// replace_with_range() takes a range whose end may have a type of its own, single-pass ranges included.
TEST_CASE("string/replacement_from_range") {
  constexpr auto view = [](Fixed & string) {
    string.replace_with_range(string.begin() + c_substringOffset,
                              string.cbegin() + c_substringOffset + c_substringLength, StringView{c_listed});
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.replace_with_range(string.begin() + c_substringOffset,
                              string.cbegin() + c_substringOffset + c_substringLength,
                              std::ranges::subrange(SinglePassIterator(c_listed),
                                                    SinglePassIterator(c_listed + c_listedLength)));
  };

  CHECK(editedDifference(view, c_listedReplacing) == fullMatch(c_listedReplacing));
  CHECK(editedDifference(singlePass, c_listedReplacing) == fullMatch(c_listedReplacing));

  static_assert(editedDifference(view, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a contiguous range must take the place of the run");
  static_assert(editedDifference(singlePass, c_listedReplacing) == fullMatch(c_listedReplacing),
                "a single-pass range must be rotated into place");
}

// Characters read from the string itself take the place of a run unchanged, even when the buffer moves to grow.
TEST_CASE("string/replacement_from_itself") {
  constexpr auto whole = [](Relocating & string) {
    string.replace(c_substringOffset, 1, string);
  };
  constexpr auto iterated = [](Relocating & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + 1, string.begin(),
                   string.end());
  };
  constexpr auto ranged = [](Relocating & string) {
    string.replace_with_range(string.begin() + c_substringOffset, string.cbegin() + c_substringOffset + 1,
                              StringView{string.c_str()});
  };

  // Names the quadruple that breaks, where the static_assert below only counts them.
  const auto checkSelfReplacement = [&](size_t position, size_t removed, size_t offset, size_t count, size_t difference,
                                        size_t match) {
    INFO("position ", static_cast<long long>(position));
    INFO("removed ", static_cast<long long>(removed));
    INFO("offset ", static_cast<long long>(offset));
    INFO("count ", static_cast<long long>(count));
    CHECK(difference == match);
  };

  forEachSelfReplacement<Fixed>(checkSelfReplacement);
  forEachSelfReplacement<Relocating>(checkSelfReplacement);
  CHECK(editedDifference<Relocating>(whole, c_sampleSpliced) == fullMatch(c_sampleSpliced));
  CHECK(editedDifference<Relocating>(iterated, c_sampleSpliced) == fullMatch(c_sampleSpliced));
  CHECK(editedDifference<Relocating>(ranged, c_sampleSpliced) == fullMatch(c_sampleSpliced));

  static_assert(selfReplacementFailures<Fixed>() == 0, "every slice of the string must survive replacing a run of it");
  static_assert(selfReplacementFailures<Relocating>() == 0, "every slice must survive a buffer that moves to grow");
  static_assert(editedDifference<Relocating>(whole, c_sampleSpliced) == fullMatch(c_sampleSpliced),
                "the string must take the place of a run of itself whole");
  static_assert(editedDifference<Relocating>(iterated, c_sampleSpliced) == fullMatch(c_sampleSpliced),
                "an iterator range over the string must take the place of a run of itself");
  static_assert(editedDifference<Relocating>(ranged, c_sampleSpliced) == fullMatch(c_sampleSpliced),
                "a view of the string must take the place of a run of itself");
}

#if !defined(_DEBUG)
// Without the debug checks a replacement past the end, over a bad range, or one that does not fit keeps the contents.
TEST_CASE("string/replacement_rejected") {
  constexpr auto pastEnd = [](Fixed & string) {
    string.replace(c_sampleLength + 1, 0, c_listed);
  };
  constexpr auto counted = [](Fixed & string) {
    string.replace(0, 1, c_long, c_longLength);
  };
  constexpr auto filled = [](Fixed & string) {
    string.replace(0, 1, c_capacity + 1, c_fillCharacter);
  };
  constexpr auto substring = [](Fixed & string) {
    string.replace(0, 1, StringView{c_listed}, c_listedLength + 1);
  };
  constexpr auto reversed = [](Fixed & string) {
    string.replace(string.begin() + c_substringOffset, string.cbegin(), c_listed);
  };
  constexpr auto overlong = [](Fixed & string) {
    string.replace(string.begin(), string.cend() + 1, c_listed);
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.replace(string.begin(), string.cbegin() + 1, SinglePassIterator(c_long),
                   SinglePassIterator(c_long + c_longLength));
  };
  constexpr auto reversedRange = [](Fixed & string) {
    string.replace_with_range(string.begin() + c_substringOffset, string.cbegin(), StringView{c_listed});
  };

  CHECK(editedDifference(pastEnd, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(counted, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(filled, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(substring, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(reversed, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(overlong, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(singlePass, "alayer") == fullMatch("alayer"));
  CHECK(editedDifference(reversedRange, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(pastEnd, c_sample) == fullMatch(c_sample),
                "an offset past the end must keep the old contents");
  static_assert(editedDifference(counted, c_sample) == fullMatch(c_sample),
                "a rejected counted source must keep the old contents");
  static_assert(editedDifference(filled, c_sample) == fullMatch(c_sample),
                "a rejected fill must keep the old contents");
  static_assert(editedDifference(substring, c_sample) == fullMatch(c_sample),
                "a substring offset past the end must keep the old contents");
  static_assert(editedDifference(reversed, c_sample) == fullMatch(c_sample),
                "a reversed range must keep the old contents");
  static_assert(editedDifference(overlong, c_sample) == fullMatch(c_sample),
                "a range that ends past end() must keep the old contents");
  static_assert(editedDifference(singlePass, "alayer") == fullMatch("alayer"),
                "a rejected single-pass range must keep the old length, its first character over the replaced one");
  static_assert(editedDifference(reversedRange, c_sample) == fullMatch(c_sample),
                "replace_with_range() over a reversed range must keep the old contents");
}
#endif // !_DEBUG

// copy() writes up to count characters from an offset, stops at the end, adds no terminator, and returns the count.
TEST_CASE("string/copy") {
  constexpr auto middle = [](char * buffer) {
    return c_sampleString.copy(buffer, c_substringLength, c_substringOffset);
  };
  constexpr auto beyond = [](char * buffer) {
    return c_sampleString.copy(buffer, c_longLength, c_substringOffset);
  };
  constexpr auto fromFront = [](char * buffer) {
    return c_sampleString.copy(buffer, c_substringOffset);
  };
  constexpr auto atEnd = [](char * buffer) {
    return c_sampleString.copy(buffer, c_substringLength, c_sampleLength);
  };
  constexpr auto nowhere = [](char *) {
    return c_sampleString.copy(nullptr, 0);
  };

  const CopiedCharacters copied = copiedBy(middle);
  REQUIRE(copied.count == c_substringLength);
  CHECK(std::char_traits<char>::compare(copied.buffer.data(), c_sample + c_substringOffset, c_substringLength) == 0);
  CHECK(copied.buffer[c_substringLength] == c_fillCharacter);
  CHECK(copiedBy(beyond).count == c_tailLength);
  CHECK(copiedBy(fromFront).buffer[0] == c_sample[0]);
  CHECK(copiedBy(atEnd).count == 0);
  CHECK(copiedBy(atEnd).buffer[0] == c_fillCharacter);
  CHECK(copiedBy(nowhere).count == 0);

  static_assert(copiedBy(middle).count == c_substringLength, "copy() must return the count it wrote");
  static_assert(std::char_traits<char>::compare(copiedBy(middle).buffer.data(), c_sample + c_substringOffset,
                                                c_substringLength)
                  == 0,
                "the characters must come from the offset");
  static_assert(copiedBy(middle).buffer[c_substringLength] == c_fillCharacter, "copy() must write no terminator");
  static_assert(copiedBy(beyond).count == c_tailLength, "a count past the end must stop at the end");
  static_assert(copiedBy(fromFront).count == c_substringOffset && copiedBy(fromFront).buffer[0] == c_sample[0],
                "the default offset must copy from the first character");
  static_assert(copiedBy(atEnd).count == 0, "an offset of size() must copy nothing");
  static_assert(copiedBy(nowhere).count == 0, "a null destination with a count of zero must copy nothing");
}

#if !defined(_DEBUG)
// Without the debug checks a copy from past the end writes nothing and returns zero.
TEST_CASE("string/copy_rejected") {
  constexpr auto pastEnd = [](char * buffer) {
    return c_sampleString.copy(buffer, 1, c_sampleLength + 1);
  };

  CHECK(copiedBy(pastEnd).count == 0);
  CHECK(copiedBy(pastEnd).buffer[0] == c_fillCharacter);

  static_assert(copiedBy(pastEnd).count == 0 && copiedBy(pastEnd).buffer[0] == c_fillCharacter,
                "an offset past the end must copy nothing");
}
#endif // !_DEBUG

// resize() drops characters past the new size or appends copies of a character, a null one by default.
TEST_CASE("string/resize") {
  constexpr auto filled = [](Fixed & string) {
    string.resize(c_sampleLength + c_fillCount, c_fillCharacter);
  };
  constexpr auto nulls = [](Fixed & string) {
    string.resize(c_sampleWithNullsLength);
  };
  constexpr auto shrunk = [](Fixed & string) {
    string.resize(c_substringOffset, c_fillCharacter);
  };
  constexpr auto unchanged = [](Fixed & string) {
    string.resize(c_sampleLength, c_fillCharacter);
  };

  CHECK(editedDifference(filled, c_filledBack) == fullMatch(c_filledBack));
  CHECK(editedDifference(nulls, c_sampleWithNulls, c_sampleWithNullsLength) == c_sampleWithNullsLength + 1);
  CHECK(editedDifference(shrunk, c_sampleHead) == fullMatch(c_sampleHead));
  CHECK(editedDifference(unchanged, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(filled, c_filledBack) == fullMatch(c_filledBack),
                "growing must append copies of the character");
  static_assert(editedDifference(nulls, c_sampleWithNulls, c_sampleWithNullsLength) == c_sampleWithNullsLength + 1,
                "growing without a character must append null bytes");
  static_assert(editedDifference(shrunk, c_sampleHead) == fullMatch(c_sampleHead),
                "shrinking must drop the characters past the new size");
  static_assert(editedDifference(unchanged, c_sample) == fullMatch(c_sample), "the same size must change nothing");
}

#if !defined(_DEBUG)
// Without the debug checks a resize past the capacity keeps the old contents.
TEST_CASE("string/resize_rejected") {
  constexpr auto oversized = [](Fixed & string) {
    string.resize(c_capacity + 1, c_fillCharacter);
  };

  CHECK(editedDifference(oversized, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(oversized, c_sample) == fullMatch(c_sample),
                "a size past the capacity must keep the old contents");
}
#endif // !_DEBUG

// The operation sees the old characters and the requested count, and the size it returns becomes the new size.
TEST_CASE("string/resize_and_overwrite") {
  constexpr auto filled = [](Fixed & string) {
    string.resize_and_overwrite(c_sampleLength + c_fillCount, [](char * data, size_t count) {
      std::char_traits<char>::assign(data + c_sampleLength, count - c_sampleLength, c_fillCharacter);
      return count;
    });
  };
  constexpr auto shortened = [](Fixed & string) {
    string.resize_and_overwrite(c_capacity, [](char *, size_t) {
      return c_substringOffset;
    });
  };
  constexpr auto truncated = [](Fixed & string) {
    string.resize_and_overwrite(c_substringOffset, [](char *, size_t count) {
      return count;
    });
  };
  constexpr auto signedSize = [](Fixed & string) {
    string.resize_and_overwrite(c_capacity, [](char *, size_t) {
      return static_cast<int>(c_sampleLength);
    });
  };

  CHECK(editedDifference(filled, c_filledBack) == fullMatch(c_filledBack));
  CHECK(editedDifference(shortened, c_sampleHead) == fullMatch(c_sampleHead));
  CHECK(editedDifference(truncated, c_sampleHead) == fullMatch(c_sampleHead));
  CHECK(editedDifference(signedSize, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(filled, c_filledBack) == fullMatch(c_filledBack),
                "the operation must find the old characters and write up to the requested count");
  static_assert(editedDifference(shortened, c_sampleHead) == fullMatch(c_sampleHead),
                "a size below the requested count must become the new size");
  static_assert(editedDifference(truncated, c_sampleHead) == fullMatch(c_sampleHead),
                "a count below size() must keep the characters before it");
  static_assert(editedDifference(signedSize, c_sample) == fullMatch(c_sample),
                "a signed size must be taken as the new size");
}

#if !defined(_DEBUG)
// Without the debug checks a count past the capacity or a returned size outside [0, count] keeps the old size.
TEST_CASE("string/resize_and_overwrite_rejected") {
  constexpr auto oversized = [](Fixed & string) {
    string.resize_and_overwrite(c_capacity + 1, [](char * data, size_t count) {
      std::char_traits<char>::assign(data, count, c_fillCharacter);
      return count;
    });
  };
  constexpr auto overcounted = [](Fixed & string) {
    string.resize_and_overwrite(c_capacity, [](char *, size_t count) {
      return count + 1;
    });
  };
  constexpr auto negative = [](Fixed & string) {
    string.resize_and_overwrite(c_capacity, [](char *, size_t) {
      return -1;
    });
  };
  constexpr auto overwrittenTerminator = [](Fixed & string) {
    string.resize_and_overwrite(c_capacity, [](char * data, size_t count) {
      std::char_traits<char>::assign(data + c_sampleLength, count - c_sampleLength, c_fillCharacter);
      return count + 1;
    });
  };

  CHECK(editedDifference(oversized, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(overcounted, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(negative, c_sample) == fullMatch(c_sample));
  CHECK(editedDifference(overwrittenTerminator, c_sample) == fullMatch(c_sample));

  static_assert(editedDifference(oversized, c_sample) == fullMatch(c_sample),
                "a count past the capacity must not run the operation");
  static_assert(editedDifference(overcounted, c_sample) == fullMatch(c_sample),
                "a size past the requested count must keep the old size");
  static_assert(editedDifference(negative, c_sample) == fullMatch(c_sample), "a negative size must keep the old size");
  static_assert(editedDifference(overwrittenTerminator, c_sample) == fullMatch(c_sample),
                "a rejected size must write the terminator after the old size again");
}
#endif // !_DEBUG

// swap() exchanges the contents of two strings, whichever way the storage copies, and a self-swap changes nothing.
TEST_CASE("string/swap") {
  constexpr auto self = [](Fixed & string) {
    string.swap(string);
  };

  const auto fixed = swappedDifferences<Fixed>();
  const auto large = swappedDifferences<Large>();
  CHECK(fixed[0] == fullMatch(c_listed));
  CHECK(fixed[1] == fullMatch(c_sample));
  CHECK(large[0] == fullMatch(c_listed));
  CHECK(large[1] == fullMatch(c_sample));
  CHECK(editedDifference(self, c_sample) == fullMatch(c_sample));

  static_assert(swappedDifferences<Fixed>() == std::array{fullMatch(c_listed), fullMatch(c_sample)},
                "a storage copied whole must exchange contents");
  static_assert(swappedDifferences<Large>() == std::array{fullMatch(c_listed), fullMatch(c_sample)},
                "a storage copied by length must exchange contents");
  static_assert(editedDifference(self, c_sample) == fullMatch(c_sample), "a self-swap must change nothing");
}

// Which modifiers never throw, what each returns, and that a literal 0 picks the overload taking an index.
TEST_CASE("string/modifier_signatures") {
  static_assert(noexcept(std::declval<Fixed &>().clear()) && noexcept(std::declval<Fixed &>().erase())
                  && noexcept(std::declval<Fixed &>().insert(0, c_listed))
                  && noexcept(std::declval<Fixed &>().insert(std::declval<Fixed &>().begin(), c_fillCharacter)),
                "the modifiers must not throw, where std::basic_string throws on a bad offset or length");

  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().insert(0, 1, c_fillCharacter)), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>().erase(0)), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>().erase(0, 0)), Fixed &>,
                "a literal 0 must select the overloads taking an index, which return the string");
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().insert(std::declval<Fixed::const_iterator>(),
                                                                       c_fillCharacter)),
                               Fixed::iterator>
                  && std::is_same_v<decltype(std::declval<Fixed &>().erase(std::declval<Fixed::iterator>())),
                                    Fixed::iterator>
                  && std::is_same_v<decltype(std::declval<Fixed &>().insert_range(std::declval<Fixed::iterator>(),
                                                                                  StringView{})),
                                    Fixed::iterator>,
                "the overloads taking an iterator must return a mutable iterator, as std::basic_string does");

  static_assert(noexcept(std::declval<Fixed &>().push_back(c_fillCharacter))
                  && noexcept(std::declval<Fixed &>().pop_back()) && noexcept(std::declval<Fixed &>().append(c_listed))
                  && noexcept(std::declval<Fixed &>() += c_listed)
                  && noexcept(std::declval<Fixed &>().replace(0, 0, c_listed))
                  && noexcept(std::declval<const Fixed &>().copy(nullptr, 0))
                  && noexcept(std::declval<Fixed &>().resize(0))
                  && noexcept(std::declval<Fixed &>().swap(std::declval<Fixed &>())),
                "the modifiers added with append and replace must not throw either");

  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().append(0, c_fillCharacter)), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>() += c_fillCharacter), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>().replace(0, 0, 0, c_fillCharacter)), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>().replace(std::declval<Fixed::iterator>(),
                                                                             std::declval<Fixed::const_iterator>(),
                                                                             c_listed)),
                                    Fixed &>
                  && std::is_same_v<decltype(std::declval<const Fixed &>().copy(nullptr, 0)), size_t>,
                "append(), operator+= and replace() must return the string, and copy() the count it wrote");
}

} // namespace toy
