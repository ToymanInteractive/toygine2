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
  \brief  Unit tests for \ref toy::String and the free functions that take it.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// Buffer size every case builds on, the terminator included.
constexpr size_t allocatedSize = 16;

using Fixed = FixedString<allocatedSize>;

// A string over a larger buffer, the source for the cross-storage cases.
using Wide = FixedString<allocatedSize * 2>;

// Most characters a Fixed string holds.
constexpr size_t capacity = allocatedSize - 1;

// The literal most cases copy; its length is measured from the literal.
constexpr const char * sampleText   = "player";
constexpr size_t       sampleLength = std::char_traits<char>::length(sampleText);

// Byte count a counted construction takes, short enough to leave sample characters out.
constexpr size_t prefixLength = 4;

// Offset and byte count the substring cases take, so the substring stops short of both ends of the sample.
constexpr size_t substringOffset = 2;
constexpr size_t substringLength = 3;

// Bytes from the substring offset to the end of the sample.
constexpr size_t tailLength = sampleLength - substringOffset;

// Range with a null byte inside; its length comes from the array, since a terminator scan stops at the first null.
constexpr char   embeddedNull[]     = "ab\0cd";
constexpr size_t embeddedNullLength = sizeof(embeddedNull) - 1;

// Literal longer than the buffer, so a counted construction can fill the buffer exactly.
constexpr const char * longText   = "abcdefghijklmnopqrstuvwxyz";
constexpr size_t       longLength = std::char_traits<char>::length(longText);

// Character and count the fill cases write, and the same fill spelled out.
constexpr char         fillCharacter = 'x';
constexpr const char * filledText    = "xxxxx";
constexpr size_t       fillCount     = std::char_traits<char>::length(filledText);

// The characters the initializer-list cases write between braces, spelled as a literal.
constexpr const char * listedText   = "abc";
constexpr size_t       listedLength = std::char_traits<char>::length(listedText);

// Offset where the string first differs from count bytes of expected and a terminator; count + 1 on a full match.
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
  const Fixed string(SinglePassIterator(sampleText), SinglePassIterator(sampleText + sampleLength));

  return firstDifference(string, sampleText, sampleLength);
}

// Where a copy of the sample string first differs from the sample.
template <typename StringType = Fixed>
[[nodiscard]] constexpr size_t copiedDifference() noexcept {
  const StringType source(sampleText);
  const StringType copy(source);

  return firstDifference(copy, sampleText, sampleLength);
}

// Where a string moved from the sample string first differs from the sample.
template <typename StringType = Fixed>
[[nodiscard]] constexpr size_t movedDifference() noexcept {
  StringType       source(sampleText);
  const StringType moved(std::move(source));

  return firstDifference(moved, sampleText, sampleLength);
}

// Where a substring of an lvalue sample string, taken with an offset alone, first differs from the sample's tail.
[[nodiscard]] constexpr size_t lvalueTailDifference() noexcept {
  const Fixed source(sampleText);
  const Fixed tail(source, substringOffset);

  return firstDifference(tail, sampleText + substringOffset, tailLength);
}

// Buffer large enough that assignment copies only the characters in use, the path Fixed does not take.
constexpr size_t largeAllocatedSize = 256;

using Large = FixedString<largeAllocatedSize>;

static_assert(sizeof(Fixed) <= platform::inlineCopyMaxBytes,
              "Fixed must copy its whole object on every target, so the assignment cases reach that path");
static_assert(sizeof(Large) > platform::inlineCopyMaxBytes,
              "Large must copy by length on every target, so the assignment cases reach that path");

// String holding a value longer than the sample, so an assignment of the sample has to move the terminator.
template <typename StringType>
[[nodiscard]] constexpr StringType longerThanSample() noexcept {
  return StringType(longText, capacity);
}

// Where a string holding a longer value, then copy-assigned the sample string, first differs from the sample.
template <typename StringType>
[[nodiscard]] constexpr size_t copyAssignedDifference() noexcept {
  const StringType source(sampleText);
  StringType       target = longerThanSample<StringType>();
  target                  = source;

  return firstDifference(target, sampleText, sampleLength);
}

// Where a string holding a longer value, then move-assigned the sample string, first differs from the sample.
template <typename StringType>
[[nodiscard]] constexpr size_t moveAssignedDifference() noexcept {
  StringType source(sampleText);
  StringType target = longerThanSample<StringType>();
  target            = std::move(source);

  return firstDifference(target, sampleText, sampleLength);
}

// Where the sample string, assigned to itself through an alias, first differs from the sample.
template <typename StringType>
[[nodiscard]] constexpr size_t selfAssignedDifference() noexcept {
  StringType         string(sampleText);
  const StringType & alias = string;
  string                   = alias;

  return firstDifference(string, sampleText, sampleLength);
}

// Where the sample string, assigned a substring of itself, first differs from that substring.
[[nodiscard]] constexpr size_t selfSubstringDifference() noexcept {
  Fixed string(sampleText);
  string.assign(string, substringOffset, substringLength);

  return firstDifference(string, sampleText + substringOffset, substringLength);
}

// Where the sample string, assigned counted bytes from inside its own buffer, first differs from those bytes.
[[nodiscard]] constexpr size_t selfPointerDifference() noexcept {
  Fixed string(sampleText);
  string.assign(string.c_str() + substringOffset, substringLength);

  return firstDifference(string, sampleText + substringOffset, substringLength);
}

// Where the sample string, assigned a view of its own tail as a range, first differs from that tail.
[[nodiscard]] constexpr size_t selfRangeDifference() noexcept {
  Fixed string(sampleText);
  string.assign_range(StringView{string.c_str() + substringOffset});

  return firstDifference(string, sampleText + substringOffset, tailLength);
}

// Where a string holding a longer value, then assigned a single-pass range over the sample, first differs from it.
[[nodiscard]] constexpr size_t singlePassAssignedDifference() noexcept {
  Fixed string = longerThanSample<Fixed>();
  string.assign(SinglePassIterator(sampleText), SinglePassIterator(sampleText + sampleLength));

  return firstDifference(string, sampleText, sampleLength);
}

// Where a string holding a longer value, then assigned a single-pass range with assign_range(), first differs from it.
[[nodiscard]] constexpr size_t singlePassRangeDifference() noexcept {
  const std::ranges::subrange range(SinglePassIterator(sampleText), SinglePassIterator(sampleText + sampleLength));

  Fixed string = longerThanSample<Fixed>();
  string.assign_range(range);

  return firstDifference(string, sampleText, sampleLength);
}

// The sample as a constant, so the element-access cases reach the const overloads in a constant expression.
constexpr Fixed sampleString(sampleText);

// The sample after front(), at(), operator[] and back() each write one character, offsets 0, 1, 2 and the last.
constexpr const char * overwrittenText = "abcyed";

// The sample after a write through data() replaces its first character with the fill character.
constexpr const char * overwrittenFirst = "xlayer";

// The sample string after front(), at(), operator[] and back() each write the matching character of overwrittenText.
[[nodiscard]] constexpr Fixed overwrittenThroughElements() noexcept {
  Fixed string(sampleText);
  string.front() = overwrittenText[0];
  string.at(1)   = overwrittenText[1];
  string[2]      = overwrittenText[2];
  string.back()  = overwrittenText[sampleLength - 1];

  return string;
}

// The sample string after a write through data() replaces its first character.
[[nodiscard]] constexpr Fixed overwrittenThroughData() noexcept {
  Fixed string(sampleText);
  string.data()[0] = fillCharacter;

  return string;
}

// A string holding no characters, kept at namespace scope so constant evaluation can compare its iterators.
constexpr Fixed emptyString{};

// The sample characters back to front, the order a reverse walk must yield.
constexpr const char * sampleReversed = "reyalp";

// The sample after writes through begin() and rbegin() replace its first and last characters.
constexpr const char * overwrittenEnds = "alayed";

// The sample string after begin() and rbegin() each write the matching character of overwrittenEnds.
[[nodiscard]] constexpr Fixed overwrittenThroughIterators() noexcept {
  Fixed string(sampleText);
  *string.begin()  = overwrittenEnds[0];
  *string.rbegin() = overwrittenEnds[sampleLength - 1];

  return string;
}

// The sample string after a reserve() of newCapacity, so a constant expression reads what the request left behind.
[[nodiscard]] constexpr Fixed reservedSample(size_t newCapacity) noexcept {
  Fixed string(sampleText);
  string.reserve(newCapacity);

  return string;
}

// The sample string after a shrink_to_fit() call.
[[nodiscard]] constexpr Fixed shrunkSample() noexcept {
  Fixed string(sampleText);
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

static_assert(StringStorage<RelocatingStorage<capacity>>, "the relocating stub must back a string");

// String whose every growth moves the buffer, so an edit must find a source inside it again after the move.
using Relocating = String<RelocatingStorage<capacity>>;

// Fixed buffer that refuses to be copied, so a string over it moves but never copies.
class MoveOnlyStorage : public FixedStringStorage<allocatedSize> {
public:
  constexpr MoveOnlyStorage() noexcept                                     = default;
  constexpr MoveOnlyStorage(const MoveOnlyStorage & other)                 = delete;
  constexpr MoveOnlyStorage(MoveOnlyStorage && other) noexcept             = default;
  constexpr MoveOnlyStorage & operator=(const MoveOnlyStorage & other)     = delete;
  constexpr MoveOnlyStorage & operator=(MoveOnlyStorage && other) noexcept = default;
  constexpr ~MoveOnlyStorage() noexcept                                    = default;
};

static_assert(StringStorage<MoveOnlyStorage> && !std::copy_constructible<String<MoveOnlyStorage>>,
              "the move-only stub must back a string that cannot be copied");

// String over a storage that cannot be copied.
using MoveOnly = String<MoveOnlyStorage>;

// What firstDifference() returns when a string holds exactly the characters of expected.
[[nodiscard]] constexpr size_t fullMatch(const char * expected) noexcept {
  return std::char_traits<char>::length(expected) + 1;
}

// Where the sample string, after edit runs on it, first differs from the count bytes of expected.
template <typename StringType = Fixed, typename Edit>
[[nodiscard]] constexpr size_t editedDifference(Edit edit, const char * expected, size_t count) noexcept {
  StringType string(sampleText);
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
  Fixed string(sampleText);

  return static_cast<size_t>(edit(string) - string.begin());
}

// The sample with removed characters at position replaced by count characters from offset of the sample itself.
[[nodiscard]] constexpr std::array<char, allocatedSize> selfReplacedSample(size_t position, size_t removed,
                                                                           size_t offset, size_t count) noexcept {
  std::array<char, allocatedSize> expected{};

  auto output = std::ranges::copy_n(sampleText, static_cast<ptrdiff_t>(position), expected.begin()).out;
  output      = std::ranges::copy_n(sampleText + offset, static_cast<ptrdiff_t>(count), output).out;
  std::ranges::copy(sampleText + position + removed, sampleText + sampleLength, output);

  return expected;
}

// Inserts each slice of the sample into itself; report() gets the triple, the difference, and the full-match value.
template <typename StringType, typename Report>
constexpr void forEachSelfInsertion(Report report) noexcept {
  for (const size_t position : std::views::iota(size_t{0}, sampleLength + 1)) {
    for (const size_t offset : std::views::iota(size_t{0}, sampleLength)) {
      for (const size_t count : std::views::iota(size_t{1}, sampleLength - offset + 1)) {
        StringType string(sampleText);
        string.insert(position, string.c_str() + offset, count);

        const size_t length = sampleLength + count;
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
  for (const size_t position : std::views::iota(size_t{0}, sampleLength)) {
    for (const size_t removed : std::views::iota(size_t{1}, sampleLength - position + 1)) {
      for (const size_t offset : std::views::iota(size_t{0}, sampleLength)) {
        for (const size_t count : std::views::iota(size_t{1}, sampleLength - offset + 1)) {
          StringType string(sampleText);
          string.replace(position, removed, string.c_str() + offset, count);

          const size_t length = sampleLength - removed + count;
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
  StringType sample(sampleText);
  StringType listed(listedText);
  sample.swap(listed);

  return {firstDifference(sample, listedText, listedLength), firstDifference(listed, sampleText, sampleLength)};
}

// What copy() leaves in a buffer of fill characters, and the count it returns.
struct CopiedCharacters {
  size_t                          count;
  std::array<char, allocatedSize> buffer;
};

// Runs copy over a buffer of fill characters and keeps what it wrote.
template <typename Copy>
[[nodiscard]] constexpr CopiedCharacters copiedBy(Copy copy) noexcept {
  CopiedCharacters copied{.count = 0, .buffer = {}};
  copied.buffer.fill(fillCharacter);
  copied.count = copy(copied.buffer.data());

  return copied;
}

// The sample with its characters from substringOffset on removed.
constexpr const char * sampleHead = "pl";

// The sample with substringLength characters removed from substringOffset.
constexpr const char * sampleWithoutMiddle = "plr";

// The sample with the fill characters inserted at substringOffset.
constexpr const char * filledInside = "plxxxxxayer";

// The sample with the fill characters inserted before its first character.
constexpr const char * filledFront = "xxxxxplayer";

// The sample with the fill characters inserted after its last character.
constexpr const char * filledBack = "playerxxxxx";

// The sample with the listed characters inserted at substringOffset.
constexpr const char * listedInside = "plabcayer";

// The sample with the listed characters, back to front, inserted at substringOffset.
constexpr const char * listedReversedInside = "plcbaayer";

// The sample with a copy of itself inserted at substringOffset.
constexpr const char * sampleDoubledInside = "plplayerayer";

// The sample with embeddedNull inserted at substringOffset, the null byte included.
constexpr char   embeddedNullInside[]     = "plab\0cdayer";
constexpr size_t embeddedNullInsideLength = sizeof(embeddedNullInside) - 1;

// The sample with the listed characters after its last character.
constexpr const char * listedAppended = "playerabc";

// The sample with the listed characters, back to front, after its last character.
constexpr const char * listedReversedAppended = "playercba";

// The sample followed by a copy of itself.
constexpr const char * sampleDoubled = "playerplayer";

// The sample with embeddedNull after its last character, the null byte included.
constexpr char   embeddedNullAppended[]     = "playerab\0cd";
constexpr size_t embeddedNullAppendedLength = sizeof(embeddedNullAppended) - 1;

// The sample with its substringLength characters from substringOffset replaced by the listed characters.
constexpr const char * listedReplacing = "plabcr";

// The same run replaced by the listed characters back to front.
constexpr const char * listedReversedReplacing = "plcbar";

// The same run replaced by the fill characters.
constexpr const char * filledReplacing = "plxxxxxr";

// The same run replaced by embeddedNull, the null byte included.
constexpr char   embeddedNullReplacing[]     = "plab\0cdr";
constexpr size_t embeddedNullReplacingLength = sizeof(embeddedNullReplacing) - 1;

// The sample with its character at substringOffset replaced by a copy of the whole sample.
constexpr const char * sampleSpliced = "plplayeryer";

// The sample followed by two null bytes, what a resize() with the default character leaves.
constexpr char   sampleWithNulls[]     = "player\0\0";
constexpr size_t sampleWithNullsLength = sizeof(sampleWithNulls) - 1;

// A literal whose groups repeat, so a search has more than one candidate to pick between, and the string holding it.
constexpr const char * repeated       = "abracadabra";
constexpr size_t       repeatedLength = std::char_traits<char>::length(repeated);
constexpr Fixed        repeatedString(repeated);

// embeddedNull as a string, so a search has to read past its null byte to reach the last two characters.
constexpr Fixed embeddedNullString(embeddedNull, embeddedNullLength);

// The run of embeddedNull that starts at its second byte and holds the null byte in the middle.
constexpr size_t embeddedNullRunOffset = 1;
constexpr size_t embeddedNullRunLength = 3;

// Whether the string holds exactly the characters of the C string expected, followed by a terminator.
template <typename StringType>
[[nodiscard]] constexpr bool holds(const StringType & string, const char * expected) noexcept {
  return firstDifference(string, expected, std::char_traits<char>::length(expected)) == fullMatch(expected);
}

// Whether a String deduces its storage from an argument of the type, so a rejected one reads as false.
template <typename Source>
concept DeducesString = requires(Source source) { String(source); };

// Whether erase_if() accepts the type as its predicate, so a rejected one reads as false instead of a build error.
template <typename Predicate>
concept ErasableIf = requires(Fixed & string, Predicate predicate) { erase_if(string, predicate); };

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
  const Fixed filled(fillCount, fillCharacter);
  CHECK(firstDifference(filled, filledText, fillCount) == fillCount + 1);

  const Fixed empty(0, fillCharacter);
  CHECK(empty.size() == 0);
  CHECK(*empty.c_str() == '\0');

  static_assert(firstDifference(Fixed(fillCount, fillCharacter), filledText, fillCount) == fillCount + 1,
                "a fill construction must write count copies of the character and terminate them");
  static_assert(Fixed(0, fillCharacter).size() == 0, "a fill count of zero must build an empty string");
}

// An iterator range is copied whole, whether the iterators allow several passes or one.
TEST_CASE("string/construction_from_iterators") {
  const Fixed fromPointers(sampleText, sampleText + sampleLength);
  CHECK(firstDifference(fromPointers, sampleText, sampleLength) == sampleLength + 1);

  const Fixed fromEmpty(sampleText, sampleText);
  CHECK(fromEmpty.size() == 0);

  CHECK(singlePassDifference() == sampleLength + 1);

  static_assert(firstDifference(Fixed(sampleText, sampleText + sampleLength), sampleText, sampleLength)
                  == sampleLength + 1,
                "a forward range must be copied whole");
  static_assert(singlePassDifference() == sampleLength + 1, "a single-pass range must be copied whole");
}

#if defined(__cpp_lib_ranges_to_container)
// A range tagged with std::from_range is copied whole.
TEST_CASE("string/construction_from_range_tag") {
  const Fixed string(std::from_range, StringView{sampleText});
  CHECK(firstDifference(string, sampleText, sampleLength) == sampleLength + 1);

  static_assert(firstDifference(Fixed(std::from_range, StringView{sampleText}), sampleText, sampleLength)
                  == sampleLength + 1,
                "a tagged range must be copied whole");
}
#endif // __cpp_lib_ranges_to_container

// A counted construction copies exactly count bytes: a null among them is copied, the source's terminator is not.
TEST_CASE("string/construction_from_counted_pointer") {
  const Fixed prefix(sampleText, prefixLength);
  CHECK(firstDifference(prefix, sampleText, prefixLength) == prefixLength + 1);

  const Fixed embedded(embeddedNull, embeddedNullLength);
  CHECK(firstDifference(embedded, embeddedNull, embeddedNullLength) == embeddedNullLength + 1);

  static_assert(firstDifference(Fixed(sampleText, prefixLength), sampleText, prefixLength) == prefixLength + 1,
                "a counted construction must copy the prefix and terminate it");
  static_assert(firstDifference(Fixed(embeddedNull, embeddedNullLength), embeddedNull, embeddedNullLength)
                  == embeddedNullLength + 1,
                "a null byte inside the range must not end the copy");
}

// An empty counted range builds an empty string, a null pointer with a count of zero included.
TEST_CASE("string/construction_from_counted_pointer_empty") {
  const Fixed fromNull(nullptr, 0);
  CHECK(fromNull.size() == 0);
  CHECK(*fromNull.c_str() == '\0');

  const Fixed fromSample(sampleText, 0);
  CHECK(fromSample.size() == 0);

  static_assert(Fixed(nullptr, 0).size() == 0, "a null pointer with a count of zero must build an empty string");
  static_assert(*Fixed(sampleText, 0).c_str() == '\0', "a count of zero must copy no byte");
}

// A counted range as long as the capacity fills the buffer and still ends in a terminator.
TEST_CASE("string/construction_from_counted_pointer_full_capacity") {
  static_assert(longLength > capacity, "the long literal must not fit, so the count alone bounds the copy");

  const Fixed string(longText, capacity);
  CHECK(firstDifference(string, longText, capacity) == capacity + 1);

  static_assert(firstDifference(Fixed(longText, capacity), longText, capacity) == capacity + 1,
                "a range as long as the capacity must fit with its terminator");
}

// A null-terminated source is measured and copied into the string's own buffer.
TEST_CASE("string/construction_from_pointer") {
  const Fixed string(sampleText);

  CHECK(firstDifference(string, sampleText, sampleLength) == sampleLength + 1);
  CHECK(string.c_str() != sampleText);

  static_assert(firstDifference(Fixed(sampleText), sampleText, sampleLength) == sampleLength + 1,
                "the pointer constructor must copy every byte and terminate the copy");
}

// A string-like source, a view or a string over another storage, is copied whole.
TEST_CASE("string/construction_from_string_like") {
  const Fixed fromView(StringView{sampleText});
  CHECK(firstDifference(fromView, sampleText, sampleLength) == sampleLength + 1);

  const Wide  wide(sampleText);
  const Fixed fromWide(wide);
  CHECK(firstDifference(fromWide, sampleText, sampleLength) == sampleLength + 1);

  static_assert(firstDifference(Fixed(StringView{sampleText}), sampleText, sampleLength) == sampleLength + 1,
                "a view must be copied whole");
  static_assert(firstDifference(Fixed(Wide(sampleText)), sampleText, sampleLength) == sampleLength + 1,
                "a string over another storage must be copied whole");
}

// A substring starts at the offset, takes at most count bytes, and stops at the end of the source.
TEST_CASE("string/construction_substring") {
  const StringView view{sampleText};

  const Fixed bounded(view, substringOffset, substringLength);
  CHECK(firstDifference(bounded, sampleText + substringOffset, substringLength) == substringLength + 1);

  const Fixed tail(view, substringOffset);
  CHECK(firstDifference(tail, sampleText + substringOffset, tailLength) == tailLength + 1);

  const Fixed atEnd(view, sampleLength);
  CHECK(atEnd.size() == 0);

  static_assert(firstDifference(Fixed(StringView{sampleText}, substringOffset, substringLength),
                                sampleText + substringOffset, substringLength)
                  == substringLength + 1,
                "a substring must take count bytes from the offset");
  static_assert(Fixed(StringView{sampleText}, substringOffset, Fixed::npos).size() == tailLength,
                "a count past the end must stop at the end of the source");
  static_assert(Fixed(StringView{sampleText}, sampleLength).size() == 0,
                "an offset at the end of the source must build an empty string");
}

// The std::basic_string(const basic_string &, pos[, count]) substring of an lvalue string, same storage or not.
TEST_CASE("string/construction_substring_of_string") {
  const Fixed source(sampleText);

  const Fixed tail(source, substringOffset);
  CHECK(firstDifference(tail, sampleText + substringOffset, tailLength) == tailLength + 1);

  const Fixed bounded(source, substringOffset, substringLength);
  CHECK(firstDifference(bounded, sampleText + substringOffset, substringLength) == substringLength + 1);

  const Wide  wide(sampleText);
  const Fixed fromWide(wide, substringOffset);
  CHECK(firstDifference(fromWide, sampleText + substringOffset, tailLength) == tailLength + 1);

  static_assert(lvalueTailDifference() == tailLength + 1, "an offset alone must take the rest of the string");
}

// A copy holds the same characters as its source in a buffer of its own.
TEST_CASE("string/construction_copy") {
  const Fixed source(sampleText);
  const Fixed copy(source);

  CHECK(copy.c_str() != source.c_str());
  CHECK(firstDifference(copy, sampleText, sampleLength) == sampleLength + 1);
  CHECK(copiedDifference<Large>() == sampleLength + 1);

  static_assert(copiedDifference() == sampleLength + 1, "a copy must hold every byte of its source");
  static_assert(copiedDifference<Large>() == sampleLength + 1,
                "a copy that takes only the characters in use must still hold every one of them");
}

// A moved-to string holds the characters its source held.
TEST_CASE("string/construction_move") {
  Fixed       source(sampleText);
  const Fixed moved(std::move(source));

  CHECK(firstDifference(moved, sampleText, sampleLength) == sampleLength + 1);
  CHECK(movedDifference<Large>() == sampleLength + 1);

  static_assert(movedDifference() == sampleLength + 1, "a moved-to string must hold every byte of its source");
  static_assert(movedDifference<Large>() == sampleLength + 1,
                "a move that takes only the characters in use must still hold every one of them");
}

// A substring of an rvalue string moves the kept bytes to the front of the storage it takes over.
TEST_CASE("string/construction_substring_move") {
  Fixed       source(sampleText);
  const Fixed bounded(std::move(source), substringOffset, substringLength);
  CHECK(firstDifference(bounded, sampleText + substringOffset, substringLength) == substringLength + 1);

  const Fixed whole(Fixed(sampleText), 0);
  CHECK(firstDifference(whole, sampleText, sampleLength) == sampleLength + 1);

  static_assert(firstDifference(Fixed(Fixed(sampleText), substringOffset, substringLength),
                                sampleText + substringOffset, substringLength)
                  == substringLength + 1,
                "a moved-from substring must keep count bytes from the offset");
  static_assert(firstDifference(Fixed(Fixed(sampleText), substringOffset), sampleText + substringOffset, tailLength)
                  == tailLength + 1,
                "an offset alone must keep the rest of the moved string");
  static_assert(Fixed(Fixed(sampleText), sampleLength).size() == 0,
                "an offset at the end of the moved string must build an empty string");
}

// Brace initialization copies the listed characters, so a count and a character read as two characters.
TEST_CASE("string/construction_from_initializer_list") {
  const Fixed string{'a', 'b', 'c'};
  CHECK(firstDifference(string, listedText, listedLength) == listedLength + 1);

  const Fixed braced{static_cast<char>(3), 'a'};
  CHECK(braced.size() == 2);

  static_assert(firstDifference(Fixed{'a', 'b', 'c'}, listedText, listedLength) == listedLength + 1,
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
  filled.assign(fillCount, fillCharacter);
  CHECK(firstDifference(filled, filledText, fillCount) == fillCount + 1);

  Fixed emptied = longerThanSample<Fixed>();
  emptied.assign(0, fillCharacter);
  CHECK(emptied.size() == 0);
  CHECK(*emptied.c_str() == '\0');

  Fixed single = longerThanSample<Fixed>();
  single       = fillCharacter;
  CHECK(firstDifference(single, filledText, 1) == 2);

  static_assert(firstDifference(longerThanSample<Fixed>().assign(fillCount, fillCharacter), filledText, fillCount)
                  == fillCount + 1,
                "a fill assignment must write count copies of the character and terminate them");
  static_assert(longerThanSample<Fixed>().assign(0, fillCharacter).size() == 0,
                "a fill count of zero must empty the string");
  static_assert(firstDifference(longerThanSample<Fixed>() = fillCharacter, filledText, 1) == 2,
                "a character must assign as a fill of one");
}

// A pointer assignment copies a C string whole, or exactly count bytes of a counted one, embedded nulls included.
TEST_CASE("string/assignment_from_pointer") {
  Fixed measured = longerThanSample<Fixed>();
  measured       = sampleText;
  CHECK(firstDifference(measured, sampleText, sampleLength) == sampleLength + 1);
  CHECK(measured.c_str() != sampleText);

  Fixed prefix = longerThanSample<Fixed>();
  prefix.assign(sampleText, prefixLength);
  CHECK(firstDifference(prefix, sampleText, prefixLength) == prefixLength + 1);

  Fixed embedded = longerThanSample<Fixed>();
  embedded.assign(embeddedNull, embeddedNullLength);
  CHECK(firstDifference(embedded, embeddedNull, embeddedNullLength) == embeddedNullLength + 1);

  Fixed fromNull = longerThanSample<Fixed>();
  fromNull.assign(nullptr, 0);
  CHECK(fromNull.size() == 0);

  static_assert(firstDifference(longerThanSample<Fixed>() = sampleText, sampleText, sampleLength) == sampleLength + 1,
                "a null-terminated source must be copied whole");
  static_assert(firstDifference(longerThanSample<Fixed>().assign(sampleText), sampleText, sampleLength)
                  == sampleLength + 1,
                "assign() must copy a null-terminated source as operator= does");
  static_assert(firstDifference(longerThanSample<Fixed>().assign(sampleText, prefixLength), sampleText, prefixLength)
                  == prefixLength + 1,
                "a counted assignment must copy the prefix and terminate it");
  static_assert(firstDifference(longerThanSample<Fixed>().assign(embeddedNull, embeddedNullLength), embeddedNull,
                                embeddedNullLength)
                  == embeddedNullLength + 1,
                "a null byte inside the range must not end the copy");
  static_assert(longerThanSample<Fixed>().assign(nullptr, 0).size() == 0,
                "a null pointer with a count of zero must empty the string");
}

// A string-like source, a view or a string over another storage, is copied whole by operator= and assign() alike.
TEST_CASE("string/assignment_from_string_like") {
  Fixed fromView = longerThanSample<Fixed>();
  fromView       = StringView{sampleText};
  CHECK(firstDifference(fromView, sampleText, sampleLength) == sampleLength + 1);

  const Wide wide(sampleText);
  Fixed      fromWide = longerThanSample<Fixed>();
  fromWide.assign(wide);
  CHECK(firstDifference(fromWide, sampleText, sampleLength) == sampleLength + 1);

  static_assert(firstDifference(longerThanSample<Fixed>() = StringView{sampleText}, sampleText, sampleLength)
                  == sampleLength + 1,
                "a view must be copied whole");
  static_assert(firstDifference(longerThanSample<Fixed>().assign(Wide(sampleText)), sampleText, sampleLength)
                  == sampleLength + 1,
                "a string over another storage must be copied whole");
}

// A substring assignment starts at the offset, takes at most count bytes, and stops at the end of the source.
TEST_CASE("string/assignment_substring") {
  const StringView view{sampleText};

  Fixed bounded = longerThanSample<Fixed>();
  bounded.assign(view, substringOffset, substringLength);
  CHECK(firstDifference(bounded, sampleText + substringOffset, substringLength) == substringLength + 1);

  Fixed tail = longerThanSample<Fixed>();
  tail.assign(Wide(sampleText), substringOffset);
  CHECK(firstDifference(tail, sampleText + substringOffset, tailLength) == tailLength + 1);

  Fixed atEnd = longerThanSample<Fixed>();
  atEnd.assign(view, sampleLength);
  CHECK(atEnd.size() == 0);

  static_assert(firstDifference(longerThanSample<Fixed>().assign(StringView{sampleText}, substringOffset,
                                                                 substringLength),
                                sampleText + substringOffset, substringLength)
                  == substringLength + 1,
                "a substring must take count bytes from the offset");
  static_assert(longerThanSample<Fixed>().assign(StringView{sampleText}, substringOffset).size() == tailLength,
                "an offset alone must take the rest of the source");
  static_assert(longerThanSample<Fixed>().assign(StringView{sampleText}, sampleLength).size() == 0,
                "an offset at the end of the source must empty the string");
}

// An iterator range replaces the contents whole, whether the iterators allow several passes or one.
TEST_CASE("string/assignment_from_iterators") {
  Fixed fromPointers = longerThanSample<Fixed>();
  fromPointers.assign(sampleText, sampleText + sampleLength);
  CHECK(firstDifference(fromPointers, sampleText, sampleLength) == sampleLength + 1);

  Fixed fromEmpty = longerThanSample<Fixed>();
  fromEmpty.assign(sampleText, sampleText);
  CHECK(fromEmpty.size() == 0);

  CHECK(singlePassAssignedDifference() == sampleLength + 1);

  static_assert(firstDifference(longerThanSample<Fixed>().assign(sampleText, sampleText + sampleLength), sampleText,
                                sampleLength)
                  == sampleLength + 1,
                "a forward range must replace the contents whole");
  static_assert(singlePassAssignedDifference() == sampleLength + 1,
                "a single-pass range must replace the contents whole");
}

// assign_range() replaces the contents with a range, contiguous or single-pass.
TEST_CASE("string/assignment_from_range") {
  Fixed fromView = longerThanSample<Fixed>();
  fromView.assign_range(StringView{sampleText});
  CHECK(firstDifference(fromView, sampleText, sampleLength) == sampleLength + 1);

  CHECK(singlePassRangeDifference() == sampleLength + 1);

  static_assert(firstDifference(longerThanSample<Fixed>().assign_range(StringView{sampleText}), sampleText,
                                sampleLength)
                  == sampleLength + 1,
                "a contiguous range must replace the contents whole");
  static_assert(singlePassRangeDifference() == sampleLength + 1, "a single-pass range must replace the contents whole");
}

// A braced list replaces the contents character by character, through operator= and assign() alike.
TEST_CASE("string/assignment_from_initializer_list") {
  Fixed string = longerThanSample<Fixed>();
  string       = {'a', 'b', 'c'};
  CHECK(firstDifference(string, listedText, listedLength) == listedLength + 1);

  static_assert(firstDifference(longerThanSample<Fixed>() = {'a', 'b', 'c'}, listedText, listedLength)
                  == listedLength + 1,
                "a list must be copied character by character");
  static_assert(firstDifference(longerThanSample<Fixed>().assign({'a', 'b', 'c'}), listedText, listedLength)
                  == listedLength + 1,
                "assign() must copy a list as operator= does");
}

// A copy assignment reproduces the source, whether the storage copies its whole object or only the characters in use.
TEST_CASE("string/assignment_copy") {
  CHECK(copyAssignedDifference<Fixed>() == sampleLength + 1);
  CHECK(copyAssignedDifference<Large>() == sampleLength + 1);

  const Large source(sampleText);
  Large       target = longerThanSample<Large>();
  target.assign(source);
  CHECK(firstDifference(target, sampleText, sampleLength) == sampleLength + 1);
  CHECK(target.c_str() != source.c_str());

  static_assert(copyAssignedDifference<Fixed>() == sampleLength + 1,
                "a whole-object copy assignment must reproduce the source");
  static_assert(copyAssignedDifference<Large>() == sampleLength + 1,
                "a length-based copy assignment must reproduce the source and move the terminator");
}

// A move assignment leaves the target holding what the source held, over either copy path.
TEST_CASE("string/assignment_move") {
  CHECK(moveAssignedDifference<Fixed>() == sampleLength + 1);
  CHECK(moveAssignedDifference<Large>() == sampleLength + 1);

  Large source(sampleText);
  Large target = longerThanSample<Large>();
  target.assign(std::move(source));
  CHECK(firstDifference(target, sampleText, sampleLength) == sampleLength + 1);

  static_assert(moveAssignedDifference<Fixed>() == sampleLength + 1,
                "a whole-object move assignment must reproduce the source");
  static_assert(moveAssignedDifference<Large>() == sampleLength + 1,
                "a length-based move assignment must reproduce the source");
}

// A source inside the string itself, the whole string or a part of it, survives the assignment that reads it.
TEST_CASE("string/assignment_from_itself") {
  CHECK(selfAssignedDifference<Fixed>() == sampleLength + 1);
  CHECK(selfAssignedDifference<Large>() == sampleLength + 1);
  CHECK(selfSubstringDifference() == substringLength + 1);
  CHECK(selfPointerDifference() == substringLength + 1);
  CHECK(selfRangeDifference() == tailLength + 1);

  static_assert(selfAssignedDifference<Fixed>() == sampleLength + 1,
                "a whole-object self-assignment must keep the contents");
  static_assert(selfAssignedDifference<Large>() == sampleLength + 1,
                "a length-based self-assignment must keep the contents");
  static_assert(selfSubstringDifference() == substringLength + 1, "a substring of the string itself must survive");
  static_assert(selfPointerDifference() == substringLength + 1, "bytes read from the string's own buffer must survive");
  static_assert(selfRangeDifference() == tailLength + 1, "a view of the string's own tail must survive");
}

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
  const Fixed string(sampleText);

  for (size_t index = 0; index < sampleLength; ++index) {
    INFO("index ", index);
    CHECK(string[index] == sampleText[index]);
    CHECK(string.at(index) == sampleText[index]);
  }

  CHECK(string.front() == sampleText[0]);
  CHECK(string.back() == sampleText[sampleLength - 1]);

  // Each access hands back a reference into the string, not a copy of the character.
  CHECK(&string.front() == string.c_str());
  CHECK(&string.back() == string.c_str() + sampleLength - 1);

  static_assert(sampleString[0] == sampleText[0], "an indexed read must yield the literal's byte at that offset");
  static_assert(sampleString.at(sampleLength - 1) == sampleText[sampleLength - 1],
                "at() must read the same byte as operator[]");
  static_assert(sampleString.front() == sampleText[0], "the front character must be the literal's first byte");
  static_assert(sampleString.back() == sampleText[sampleLength - 1],
                "the back character must be the literal's last byte");
  static_assert(&sampleString.back() == sampleString.c_str() + sampleLength - 1,
                "back() must reference the character before the terminator");
}

// A write through a reference from front(), at(), operator[] or back() changes that character in place.
TEST_CASE("string/element_access_writes") {
  const Fixed string = overwrittenThroughElements();

  CHECK(firstDifference(string, overwrittenText, sampleLength) == sampleLength + 1);

  static_assert(firstDifference(overwrittenThroughElements(), overwrittenText, sampleLength) == sampleLength + 1,
                "each writable access must change the character it references and leave the length alone");
}

// operator[] reaches one offset past the characters, where it yields the terminator, on a writable string as well.
TEST_CASE("string/element_access_terminator") {
  Fixed         string(sampleText);
  const Fixed & constString = string;

  CHECK(constString[sampleLength] == '\0');
  CHECK(&string[sampleLength] == string.c_str() + sampleLength);

  const Fixed empty;
  CHECK(empty[0] == '\0');

  static_assert(sampleString[sampleLength] == '\0', "the offset size() must read the terminator");
  static_assert(Fixed()[0] == '\0', "the offset 0 of an empty string must read the terminator");
}

// data() and c_str() hand out the same buffer, and a write through data() changes a character but not the length.
TEST_CASE("string/pointer_access") {
  Fixed         string(sampleText);
  const Fixed & constString = string;

  CHECK(string.data() == string.c_str());
  CHECK(constString.data() == string.c_str());

  const Fixed written = overwrittenThroughData();
  CHECK(firstDifference(written, overwrittenFirst, sampleLength) == sampleLength + 1);

  static_assert(sampleString.data() == sampleString.c_str(), "data() must point where c_str() points");
  static_assert(firstDifference(overwrittenThroughData(), overwrittenFirst, sampleLength) == sampleLength + 1,
                "a write through data() must change the character and leave the length alone");
}

// A string passes where a view is taken, and the view reads the string's own characters.
TEST_CASE("string/conversion_to_string_view") {
  const Fixed      string(sampleText);
  const StringView view = string;

  CHECK(view.data() == string.c_str());
  CHECK(view.size() == string.size());

  static_assert(StringView(sampleString).data() == sampleString.c_str(),
                "the view must read the string's buffer, not a copy");
  static_assert(StringView(sampleString).size() == sampleLength, "the view must cover every character");
}

// The view takes the length the string reports, so a null character inside the string stays inside the view.
TEST_CASE("string/conversion_to_string_view_embedded_null") {
  const Fixed      string(embeddedNull, embeddedNullLength);
  const StringView view = string;

  CHECK(view.size() == string.size());

  static_assert(StringView(Fixed(embeddedNull, embeddedNullLength)).size() == embeddedNullLength,
                "the view must cover the characters past a null one");
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
  const Fixed string(sampleText);

  CHECK(string.begin() == string.c_str());
  CHECK(string.end() == string.c_str() + sampleLength);
  CHECK(string.cbegin() == string.begin());
  CHECK(string.cend() == string.end());
  CHECK(firstDifference(Fixed(string.begin(), string.end()), sampleText, sampleLength) == sampleLength + 1);

  static_assert(sampleString.begin() == sampleString.c_str(), "begin() must point at the first character");
  static_assert(sampleString.begin() + sampleLength == sampleString.end(),
                "end() must sit one past the last character");
  static_assert(sampleString.cbegin() == sampleString.begin() && sampleString.cend() == sampleString.end(),
                "the c overloads must return what begin() and end() return");
}

// A back-to-front walk yields the characters in reverse and ends before the first one.
TEST_CASE("string/reverse_iterators") {
  const Fixed string(sampleText);

  CHECK(&*string.rbegin() == string.c_str() + sampleLength - 1);
  CHECK(string.rend().base() == string.begin());
  CHECK(string.crbegin() == string.rbegin());
  CHECK(string.crend() == string.rend());
  CHECK(firstDifference(Fixed(string.rbegin(), string.rend()), sampleReversed, sampleLength) == sampleLength + 1);

  static_assert(firstDifference(Fixed(sampleString.rbegin(), sampleString.rend()), sampleReversed, sampleLength)
                  == sampleLength + 1,
                "a reverse walk must yield the characters back to front");
  static_assert(sampleString.crbegin() == sampleString.rbegin() && sampleString.crend() == sampleString.rend(),
                "the c overloads must return what rbegin() and rend() return");
}

// An empty string yields an empty walk in both directions.
TEST_CASE("string/iterators_empty") {
  const Fixed empty;

  CHECK(empty.begin() == empty.end());
  CHECK(empty.rbegin() == empty.rend());

  static_assert(emptyString.begin() == emptyString.end(), "an empty string must have no characters to walk");
  static_assert(emptyString.rbegin() == emptyString.rend(), "an empty string must have no characters to walk back");
}

// A write through begin() or rbegin() changes that character in place and leaves the length alone.
TEST_CASE("string/iterator_writes") {
  const Fixed string = overwrittenThroughIterators();

  CHECK(firstDifference(string, overwrittenEnds, sampleLength) == sampleLength + 1);

  static_assert(firstDifference(overwrittenThroughIterators(), overwrittenEnds, sampleLength) == sampleLength + 1,
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
  CHECK_FALSE(Fixed(sampleText).empty());
  CHECK_FALSE(Fixed(1, '\0').empty());

  static_assert(emptyString.empty(), "a default-constructed string must be empty");
  static_assert(!sampleString.empty(), "a string with characters must not be empty");
  static_assert(!Fixed(1, '\0').empty(), "a null character must count as a character");
}

// length() reports the same count as size(), embedded null characters included.
TEST_CASE("string/length") {
  const Fixed sample(sampleText);
  const Fixed embedded(embeddedNull, embeddedNullLength);

  CHECK(sample.length() == sampleLength);
  CHECK(embedded.length() == embeddedNullLength);
  CHECK(Fixed().length() == 0);

  static_assert(sampleString.length() == sampleLength, "length() must count the characters");
  static_assert(Fixed(embeddedNull, embeddedNullLength).length() == embeddedNullLength,
                "length() must count an embedded null like any other character");
  static_assert(emptyString.length() == 0, "an empty string must have a length of zero");
}

// Over a fixed buffer the longest length is the capacity, whatever the string holds now.
TEST_CASE("string/max_size") {
  CHECK(Fixed().max_size() == capacity);
  CHECK(Fixed(sampleText).max_size() == capacity);

  static_assert(sampleString.max_size() == capacity, "max_size() must equal the capacity of a fixed buffer");
  static_assert(FixedString<1>().max_size() == 0, "the smallest buffer must hold no characters at all");
}

// utf8_size() counts the characters every byte of the string encodes, a null byte inside it included.
TEST_CASE("string/utf8_size") {
  constexpr const char * cyrillic       = "привет";
  constexpr size_t       cyrillicLength = std::char_traits<char>::length(cyrillic);

  CHECK(sampleString.utf8_size() == sampleLength);
  CHECK(Fixed(cyrillic).utf8_size() == cyrillicLength / 2);
  CHECK(emptyString.utf8_size() == 0);
  CHECK(embeddedNullString.utf8_size() == embeddedNullLength);

  static_assert(sampleString.utf8_size() == sampleLength, "an ASCII string counts one character per byte");
  static_assert(Fixed(cyrillic).utf8_size() == cyrillicLength / 2, "a two-byte character counts once");
  static_assert(emptyString.utf8_size() == 0, "an empty string counts nothing");
  static_assert(embeddedNullString.utf8_size() == embeddedNullLength,
                "a null byte inside the string must count as a character");
}

// capacity() reports the characters the buffer takes, the terminator excluded, independent of the length.
TEST_CASE("string/capacity") {
  CHECK(Fixed().capacity() == capacity);
  CHECK(Fixed(sampleText).capacity() == capacity);
  CHECK(Wide().capacity() == allocatedSize * 2 - 1);

  static_assert(emptyString.capacity() == capacity, "capacity must be the buffer size less the terminator");
  static_assert(sampleString.capacity() == capacity, "capacity must not depend on the length");
}

// An accepted reserve() leaves the characters, the capacity and the buffer as they were.
TEST_CASE("string/reserve") {
  Fixed        string(sampleText);
  const char * buffer = string.c_str();

  string.reserve(0);
  CHECK(firstDifference(string, sampleText, sampleLength) == sampleLength + 1);

  string.reserve(capacity);
  CHECK(firstDifference(string, sampleText, sampleLength) == sampleLength + 1);
  CHECK(string.capacity() == capacity);
  CHECK(string.c_str() == buffer);

  static_assert(firstDifference(reservedSample(0), sampleText, sampleLength) == sampleLength + 1,
                "a request below the length must keep the characters");
  static_assert(firstDifference(reservedSample(capacity), sampleText, sampleLength) == sampleLength + 1,
                "a request at capacity must keep the characters");
  static_assert(reservedSample(capacity).capacity() == capacity, "a request at capacity must keep the capacity");
}

// A shrink request over a fixed buffer keeps the characters, the capacity and the buffer itself.
TEST_CASE("string/shrink_to_fit") {
  Fixed        string(sampleText);
  const char * buffer = string.c_str();

  string.shrink_to_fit();

  CHECK(firstDifference(string, sampleText, sampleLength) == sampleLength + 1);
  CHECK(string.capacity() == capacity);
  CHECK(string.c_str() == buffer);

  static_assert(firstDifference(shrunkSample(), sampleText, sampleLength) == sampleLength + 1,
                "a shrink request must keep the characters");
  static_assert(shrunkSample().capacity() == capacity, "a fixed buffer must keep its capacity");
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

  Fixed        string(sampleText);
  const char * buffer = string.c_str();
  clear(string);

  CHECK(firstDifference(string, "", 0) == 1);
  CHECK(string.capacity() == capacity);
  CHECK(string.c_str() == buffer);

  static_assert(editedDifference(clear, "") == 1, "clear() must leave no characters and a terminator");
}

// Copies of one character go in before the given offset, at the front, inside, or after the last character.
TEST_CASE("string/insertion_fill") {
  constexpr auto front = [](Fixed & string) {
    string.insert(0, fillCount, fillCharacter);
  };
  constexpr auto inside = [](Fixed & string) {
    string.insert(substringOffset, fillCount, fillCharacter);
  };
  constexpr auto back = [](Fixed & string) {
    string.insert(string.size(), fillCount, fillCharacter);
  };
  constexpr auto none = [](Fixed & string) {
    string.insert(substringOffset, 0, fillCharacter);
  };

  CHECK(editedDifference(front, filledFront) == fullMatch(filledFront));
  CHECK(editedDifference(inside, filledInside) == fullMatch(filledInside));
  CHECK(editedDifference(back, filledBack) == fullMatch(filledBack));
  CHECK(editedDifference(none, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(front, filledFront) == fullMatch(filledFront),
                "copies inserted at offset 0 must precede the old characters");
  static_assert(editedDifference(inside, filledInside) == fullMatch(filledInside),
                "copies inserted inside must split the old characters at the offset");
  static_assert(editedDifference(back, filledBack) == fullMatch(filledBack),
                "copies inserted at size() must follow the old characters");
  static_assert(editedDifference(none, sampleText) == fullMatch(sampleText), "a count of zero must change nothing");
}

// A C string goes in up to its terminator; a counted copy takes exactly its count, a null byte included.
TEST_CASE("string/insertion_from_pointer") {
  constexpr auto terminated = [](Fixed & string) {
    string.insert(substringOffset, listedText);
  };
  constexpr auto counted = [](Fixed & string) {
    string.insert(substringOffset, embeddedNull, embeddedNullLength);
  };
  constexpr auto empty = [](Fixed & string) {
    string.insert(substringOffset, nullptr, 0);
  };

  CHECK(editedDifference(terminated, listedInside) == fullMatch(listedInside));
  CHECK(editedDifference(empty, sampleText) == fullMatch(sampleText));

  CHECK(editedDifference(counted, embeddedNullInside, embeddedNullInsideLength) == embeddedNullInsideLength + 1);

  static_assert(editedDifference(terminated, listedInside) == fullMatch(listedInside),
                "a C string must go in whole at the offset");
  static_assert(editedDifference(empty, sampleText) == fullMatch(sampleText),
                "a null pointer with a count of zero must change nothing");
  static_assert(editedDifference(counted, embeddedNullInside, embeddedNullInsideLength) == embeddedNullInsideLength + 1,
                "a counted copy must take every byte, a null byte included");
}

// A view or a string over another storage goes in whole, or as the substring that starts at an offset into it.
TEST_CASE("string/insertion_from_string_like") {
  constexpr auto view = [](Fixed & string) {
    string.insert(substringOffset, StringView{listedText});
  };
  constexpr auto wide = [](Fixed & string) {
    string.insert(substringOffset, Wide(listedText));
  };
  constexpr auto substring = [](Fixed & string) {
    string.insert(substringOffset, StringView{sampleText}, 0, substringOffset);
  };
  constexpr auto toEnd = [](Fixed & string) {
    string.insert(string.size(), StringView{sampleText}, substringOffset);
  };

  CHECK(editedDifference(view, listedInside) == fullMatch(listedInside));
  CHECK(editedDifference(wide, listedInside) == fullMatch(listedInside));
  CHECK(editedDifference(substring, "plplayer") == fullMatch("plplayer"));
  CHECK(editedDifference(toEnd, "playerayer") == fullMatch("playerayer"));

  static_assert(editedDifference(view, listedInside) == fullMatch(listedInside), "a view must go in whole");
  static_assert(editedDifference(wide, listedInside) == fullMatch(listedInside),
                "a string over another storage must go in whole");
  static_assert(editedDifference(substring, "plplayer") == fullMatch("plplayer"),
                "a substring must take at most count bytes from the offset");
  static_assert(editedDifference(toEnd, "playerayer") == fullMatch("playerayer"),
                "the default count must take the source up to its end");
}

// Insertion at an iterator puts the characters where it pointed and returns an iterator to the first of them.
TEST_CASE("string/insertion_at_iterator") {
  constexpr auto single = [](Fixed & string) {
    return string.insert(string.begin() + 1, fillCharacter);
  };
  constexpr auto filled = [](Fixed & string) {
    return string.insert(string.cbegin() + substringOffset, fillCount, fillCharacter);
  };
  constexpr auto none = [](Fixed & string) {
    return string.insert(string.end(), 0, fillCharacter);
  };

  CHECK(editedDifference(single, "pxlayer") == fullMatch("pxlayer"));
  CHECK(returnedOffset(single) == 1);
  CHECK(editedDifference(filled, filledInside) == fullMatch(filledInside));
  CHECK(returnedOffset(filled) == substringOffset);
  CHECK(editedDifference(none, sampleText) == fullMatch(sampleText));
  CHECK(returnedOffset(none) == sampleLength);

  static_assert(editedDifference(single, "pxlayer") == fullMatch("pxlayer"),
                "one character must go in before the iterator");
  static_assert(returnedOffset(single) == 1, "the result must point at the inserted character");
  static_assert(editedDifference(filled, filledInside) == fullMatch(filledInside),
                "a const_iterator must take copies before it");
  static_assert(returnedOffset(filled) == substringOffset, "the result must point at the first copy");
  static_assert(returnedOffset(none) == sampleLength, "a count of zero must return the position it was given");
}

// An iterator range goes in whole, whether it is contiguous, forward only, or single-pass.
TEST_CASE("string/insertion_from_iterators") {
  constexpr auto contiguous = [](Fixed & string) {
    return string.insert(string.begin() + substringOffset, listedText, listedText + listedLength);
  };
  constexpr auto forward = [](Fixed & string) {
    return string.insert(string.begin() + substringOffset, std::reverse_iterator(listedText + listedLength),
                         std::reverse_iterator(listedText));
  };
  constexpr auto singlePass = [](Fixed & string) {
    return string.insert(string.begin() + substringOffset, SinglePassIterator(listedText),
                         SinglePassIterator(listedText + listedLength));
  };
  constexpr auto empty = [](Fixed & string) {
    return string.insert(string.begin() + substringOffset, listedText, listedText);
  };

  CHECK(editedDifference(contiguous, listedInside) == fullMatch(listedInside));
  CHECK(editedDifference(forward, listedReversedInside) == fullMatch(listedReversedInside));
  CHECK(editedDifference(singlePass, listedInside) == fullMatch(listedInside));
  CHECK(returnedOffset(singlePass) == substringOffset);
  CHECK(editedDifference(empty, sampleText) == fullMatch(sampleText));
  CHECK(returnedOffset(empty) == substringOffset);

  static_assert(editedDifference(contiguous, listedInside) == fullMatch(listedInside),
                "a pointer range must go in before the iterator");
  static_assert(editedDifference(forward, listedReversedInside) == fullMatch(listedReversedInside),
                "a forward range must go in in its own order");
  static_assert(editedDifference(singlePass, listedInside) == fullMatch(listedInside),
                "a single-pass range must be rotated into place");
  static_assert(returnedOffset(singlePass) == substringOffset, "the result must point at the first inserted one");
  static_assert(editedDifference(empty, sampleText) == fullMatch(sampleText), "an empty range must change nothing");
}

// A braced list goes in before the iterator, as the same characters in a pointer range would.
TEST_CASE("string/insertion_from_initializer_list") {
  constexpr auto listed = [](Fixed & string) {
    return string.insert(string.begin() + substringOffset, {'a', 'b', 'c'});
  };

  CHECK(editedDifference(listed, listedInside) == fullMatch(listedInside));
  CHECK(returnedOffset(listed) == substringOffset);

  static_assert(editedDifference(listed, listedInside) == fullMatch(listedInside),
                "a braced list must go in before the iterator");
  static_assert(returnedOffset(listed) == substringOffset, "the result must point at the first listed character");
}

// insert_range() takes a range whose end may have a type of its own, single-pass ranges included.
TEST_CASE("string/insertion_from_range") {
  constexpr auto view = [](Fixed & string) {
    return string.insert_range(string.begin() + substringOffset, StringView{listedText});
  };
  constexpr auto singlePass = [](Fixed & string) {
    return string.insert_range(string.begin() + substringOffset,
                               std::ranges::subrange(SinglePassIterator(listedText),
                                                     SinglePassIterator(listedText + listedLength)));
  };

  CHECK(editedDifference(view, listedInside) == fullMatch(listedInside));
  CHECK(returnedOffset(view) == substringOffset);
  CHECK(editedDifference(singlePass, listedInside) == fullMatch(listedInside));

  static_assert(editedDifference(view, listedInside) == fullMatch(listedInside),
                "a contiguous range must go in before the iterator");
  static_assert(returnedOffset(view) == substringOffset, "the result must point at the first inserted character");
  static_assert(editedDifference(singlePass, listedInside) == fullMatch(listedInside),
                "a single-pass range must be rotated into place");
}

// Characters read from the string itself go in unchanged, wherever they lie and even when the buffer moves to grow.
TEST_CASE("string/insertion_from_itself") {
  constexpr auto whole = [](Relocating & string) {
    string.insert(substringOffset, string);
  };
  constexpr auto iterated = [](Relocating & string) {
    string.insert(string.begin() + substringOffset, string.begin(), string.end());
  };
  constexpr auto ranged = [](Relocating & string) {
    string.insert_range(string.begin() + substringOffset, StringView{string.c_str()});
  };

  Relocating   relocating(sampleText);
  const char * buffer = relocating.c_str();
  whole(relocating);
  REQUIRE(relocating.c_str() != buffer);
  CHECK(firstDifference(relocating, sampleDoubledInside, sampleLength * 2) == sampleLength * 2 + 1);

  // Names the triple that breaks, where the static_assert below only counts them.
  const auto checkSelfInsertion = [&](size_t position, size_t offset, size_t count, size_t difference, size_t match) {
    INFO("position ", position);
    INFO("offset ", offset);
    INFO("count ", count);
    CHECK(difference == match);
  };

  forEachSelfInsertion<Fixed>(checkSelfInsertion);
  forEachSelfInsertion<Relocating>(checkSelfInsertion);
  CHECK(editedDifference<Relocating>(iterated, sampleDoubledInside) == fullMatch(sampleDoubledInside));
  CHECK(editedDifference<Relocating>(ranged, sampleDoubledInside) == fullMatch(sampleDoubledInside));

  static_assert(selfInsertionFailures<Fixed>() == 0, "every slice of the string must survive insertion into itself");
  static_assert(selfInsertionFailures<Relocating>() == 0, "every slice must survive a buffer that moves to grow");
  static_assert(editedDifference<Relocating>(whole, sampleDoubledInside) == fullMatch(sampleDoubledInside),
                "the string must go into itself whole");
  static_assert(editedDifference<Relocating>(iterated, sampleDoubledInside) == fullMatch(sampleDoubledInside),
                "an iterator range over the string must go into itself");
  static_assert(editedDifference<Relocating>(ranged, sampleDoubledInside) == fullMatch(sampleDoubledInside),
                "a view of the string must go into itself");
}

// erase() takes up to count characters from an offset and stops at the end; the defaults take everything.
TEST_CASE("string/erasure") {
  constexpr auto middle = [](Fixed & string) {
    string.erase(substringOffset, substringLength);
  };
  constexpr auto tail = [](Fixed & string) {
    string.erase(substringOffset);
  };
  constexpr auto beyond = [](Fixed & string) {
    string.erase(substringOffset, longLength);
  };
  constexpr auto atEnd = [](Fixed & string) {
    string.erase(string.size(), substringLength);
  };
  constexpr auto all = [](Fixed & string) {
    string.erase();
  };

  CHECK(editedDifference(middle, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle));
  CHECK(editedDifference(tail, sampleHead) == fullMatch(sampleHead));
  CHECK(editedDifference(beyond, sampleHead) == fullMatch(sampleHead));
  CHECK(editedDifference(atEnd, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(all, "") == 1);

  static_assert(editedDifference(middle, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle),
                "the characters after the erased ones must close the gap");
  static_assert(editedDifference(tail, sampleHead) == fullMatch(sampleHead), "the default count must erase to the end");
  static_assert(editedDifference(beyond, sampleHead) == fullMatch(sampleHead),
                "a count past the end must stop at the end");
  static_assert(editedDifference(atEnd, sampleText) == fullMatch(sampleText), "an offset of size() must erase nothing");
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
    return string.erase(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength);
  };
  constexpr auto empty = [](Fixed & string) {
    return string.erase(string.cbegin() + 1, string.cbegin() + 1);
  };

  CHECK(editedDifference(single, "payer") == fullMatch("payer"));
  CHECK(returnedOffset(single) == 1);
  CHECK(editedDifference(last, "playe") == fullMatch("playe"));
  CHECK(returnedOffset(last) == sampleLength - 1);
  CHECK(editedDifference(range, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle));
  CHECK(returnedOffset(range) == substringOffset);
  CHECK(editedDifference(empty, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(single, "payer") == fullMatch("payer"), "the character at the iterator must go");
  static_assert(returnedOffset(single) == 1, "the result must point at the character that followed");
  static_assert(returnedOffset(last) == sampleLength - 1, "erasing the last character must return end()");
  static_assert(editedDifference(range, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle),
                "a mixed iterator pair must erase the range between them");
  static_assert(returnedOffset(range) == substringOffset, "the result must point where the range began");
  static_assert(editedDifference(empty, sampleText) == fullMatch(sampleText), "an empty range must erase nothing");
}

// push_back() puts one character after the last one, and the terminator after it.
TEST_CASE("string/push_back") {
  constexpr auto pushed = [](Fixed & string) {
    string.push_back(fillCharacter);
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
    string.assign(sampleText, 1);
    string.pop_back();
  };

  CHECK(editedDifference(popped, "playe") == fullMatch("playe"));
  CHECK(editedDifference(emptied, "") == 1);

  static_assert(editedDifference(popped, "playe") == fullMatch("playe"), "the last character must go");
  static_assert(editedDifference(emptied, "") == 1, "popping the only character must leave an empty string");
}

// Copies of one character go after the last character; a count of zero adds nothing.
TEST_CASE("string/appending_fill") {
  constexpr auto filled = [](Fixed & string) {
    string.append(fillCount, fillCharacter);
  };
  constexpr auto none = [](Fixed & string) {
    string.append(0, fillCharacter);
  };

  CHECK(editedDifference(filled, filledBack) == fullMatch(filledBack));
  CHECK(editedDifference(none, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(filled, filledBack) == fullMatch(filledBack), "copies must follow the old characters");
  static_assert(editedDifference(none, sampleText) == fullMatch(sampleText), "a count of zero must change nothing");
}

// A C string goes after the last character up to its terminator; a counted copy takes exactly its count.
TEST_CASE("string/appending_from_pointer") {
  constexpr auto terminated = [](Fixed & string) {
    string.append(listedText);
  };
  constexpr auto counted = [](Fixed & string) {
    string.append(embeddedNull, embeddedNullLength);
  };
  constexpr auto empty = [](Fixed & string) {
    string.append(nullptr, 0);
  };

  CHECK(editedDifference(terminated, listedAppended) == fullMatch(listedAppended));
  CHECK(editedDifference(counted, embeddedNullAppended, embeddedNullAppendedLength) == embeddedNullAppendedLength + 1);
  CHECK(editedDifference(empty, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(terminated, listedAppended) == fullMatch(listedAppended),
                "a C string must go in whole after the old characters");
  static_assert(editedDifference(counted, embeddedNullAppended, embeddedNullAppendedLength)
                  == embeddedNullAppendedLength + 1,
                "a counted copy must take every byte, a null byte included");
  static_assert(editedDifference(empty, sampleText) == fullMatch(sampleText),
                "a null pointer with a count of zero must change nothing");
}

// A view or a string over another storage goes after the last character, whole or as a substring.
TEST_CASE("string/appending_from_string_like") {
  constexpr auto view = [](Fixed & string) {
    string.append(StringView{listedText});
  };
  constexpr auto wide = [](Fixed & string) {
    string.append(Wide(listedText));
  };
  constexpr auto substring = [](Fixed & string) {
    string.append(StringView{sampleText}, 0, substringOffset);
  };
  constexpr auto toEnd = [](Fixed & string) {
    string.append(StringView{sampleText}, substringOffset);
  };

  CHECK(editedDifference(view, listedAppended) == fullMatch(listedAppended));
  CHECK(editedDifference(wide, listedAppended) == fullMatch(listedAppended));
  CHECK(editedDifference(substring, "playerpl") == fullMatch("playerpl"));
  CHECK(editedDifference(toEnd, "playerayer") == fullMatch("playerayer"));

  static_assert(editedDifference(view, listedAppended) == fullMatch(listedAppended), "a view must go in whole");
  static_assert(editedDifference(wide, listedAppended) == fullMatch(listedAppended),
                "a string over another storage must go in whole");
  static_assert(editedDifference(substring, "playerpl") == fullMatch("playerpl"),
                "a substring must take at most count bytes from the offset");
  static_assert(editedDifference(toEnd, "playerayer") == fullMatch("playerayer"),
                "the default count must take the source up to its end");
}

// An iterator range goes after the last character, whether it is contiguous, forward only, or single-pass.
TEST_CASE("string/appending_from_iterators") {
  constexpr auto contiguous = [](Fixed & string) {
    string.append(listedText, listedText + listedLength);
  };
  constexpr auto forward = [](Fixed & string) {
    string.append(std::reverse_iterator(listedText + listedLength), std::reverse_iterator(listedText));
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.append(SinglePassIterator(listedText), SinglePassIterator(listedText + listedLength));
  };
  constexpr auto empty = [](Fixed & string) {
    string.append(listedText, listedText);
  };

  CHECK(editedDifference(contiguous, listedAppended) == fullMatch(listedAppended));
  CHECK(editedDifference(forward, listedReversedAppended) == fullMatch(listedReversedAppended));
  CHECK(editedDifference(singlePass, listedAppended) == fullMatch(listedAppended));
  CHECK(editedDifference(empty, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(contiguous, listedAppended) == fullMatch(listedAppended),
                "a pointer range must follow the old characters");
  static_assert(editedDifference(forward, listedReversedAppended) == fullMatch(listedReversedAppended),
                "a forward range must go in in its own order");
  static_assert(editedDifference(singlePass, listedAppended) == fullMatch(listedAppended),
                "a single-pass range must follow the old characters");
  static_assert(editedDifference(empty, sampleText) == fullMatch(sampleText), "an empty range must change nothing");
}

// A braced list goes after the last character, as the same characters in a pointer range would.
TEST_CASE("string/appending_from_initializer_list") {
  constexpr auto listed = [](Fixed & string) {
    string.append({'a', 'b', 'c'});
  };

  CHECK(editedDifference(listed, listedAppended) == fullMatch(listedAppended));

  static_assert(editedDifference(listed, listedAppended) == fullMatch(listedAppended),
                "a braced list must follow the old characters");
}

// append_range() takes a range whose end may have a type of its own, single-pass ranges included.
TEST_CASE("string/appending_from_range") {
  constexpr auto view = [](Fixed & string) {
    string.append_range(StringView{listedText});
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.append_range(std::ranges::subrange(SinglePassIterator(listedText),
                                              SinglePassIterator(listedText + listedLength)));
  };

  CHECK(editedDifference(view, listedAppended) == fullMatch(listedAppended));
  CHECK(editedDifference(singlePass, listedAppended) == fullMatch(listedAppended));

  static_assert(editedDifference(view, listedAppended) == fullMatch(listedAppended),
                "a contiguous range must follow the old characters");
  static_assert(editedDifference(singlePass, listedAppended) == fullMatch(listedAppended),
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

  CHECK(editedDifference<Relocating>(whole, sampleDoubled) == fullMatch(sampleDoubled));
  CHECK(editedDifference<Relocating>(iterated, sampleDoubled) == fullMatch(sampleDoubled));
  CHECK(editedDifference<Relocating>(ranged, sampleDoubled) == fullMatch(sampleDoubled));

  static_assert(editedDifference<Relocating>(whole, sampleDoubled) == fullMatch(sampleDoubled),
                "the string must go after itself whole");
  static_assert(editedDifference<Relocating>(iterated, sampleDoubled) == fullMatch(sampleDoubled),
                "an iterator range over the string must go after itself");
  static_assert(editedDifference<Relocating>(ranged, sampleDoubled) == fullMatch(sampleDoubled),
                "a view of the string must go after itself");
}

// operator+= appends a character, a C string, a braced list, or a string-like source.
TEST_CASE("string/compound_assignment") {
  constexpr auto character = [](Fixed & string) {
    string += fillCharacter;
  };
  constexpr auto terminated = [](Fixed & string) {
    string += listedText;
  };
  constexpr auto listed = [](Fixed & string) {
    string += {'a', 'b', 'c'};
  };
  constexpr auto view = [](Fixed & string) {
    string += StringView{listedText};
  };
  constexpr auto wide = [](Fixed & string) {
    string += Wide(listedText);
  };

  CHECK(editedDifference(character, "playerx") == fullMatch("playerx"));
  CHECK(editedDifference(terminated, listedAppended) == fullMatch(listedAppended));
  CHECK(editedDifference(listed, listedAppended) == fullMatch(listedAppended));
  CHECK(editedDifference(view, listedAppended) == fullMatch(listedAppended));
  CHECK(editedDifference(wide, listedAppended) == fullMatch(listedAppended));

  static_assert(editedDifference(character, "playerx") == fullMatch("playerx"), "a character must follow the old ones");
  static_assert(editedDifference(terminated, listedAppended) == fullMatch(listedAppended),
                "a C string must follow the old characters");
  static_assert(editedDifference(listed, listedAppended) == fullMatch(listedAppended),
                "a braced list must follow the old characters");
  static_assert(editedDifference(view, listedAppended) == fullMatch(listedAppended),
                "a view must follow the old characters");
  static_assert(editedDifference(wide, listedAppended) == fullMatch(listedAppended),
                "a string over another storage must follow the old characters");
}

// replace() removes up to count characters from an offset, stopping at the end, and puts the source in their place.
TEST_CASE("string/replacement") {
  constexpr auto equal = [](Fixed & string) {
    string.replace(substringOffset, substringLength, listedText);
  };
  constexpr auto growing = [](Fixed & string) {
    string.replace(substringOffset, 1, listedText);
  };
  constexpr auto shrinking = [](Fixed & string) {
    string.replace(substringOffset, substringLength, listedText, 1);
  };
  constexpr auto beyond = [](Fixed & string) {
    string.replace(substringOffset, longLength, listedText);
  };
  constexpr auto atEnd = [](Fixed & string) {
    string.replace(string.size(), substringLength, listedText);
  };

  CHECK(editedDifference(equal, listedReplacing) == fullMatch(listedReplacing));
  CHECK(editedDifference(growing, "plabcyer") == fullMatch("plabcyer"));
  CHECK(editedDifference(shrinking, "plar") == fullMatch("plar"));
  CHECK(editedDifference(beyond, "plabc") == fullMatch("plabc"));
  CHECK(editedDifference(atEnd, listedAppended) == fullMatch(listedAppended));

  static_assert(editedDifference(equal, listedReplacing) == fullMatch(listedReplacing),
                "a source as long as the run must take its place");
  static_assert(editedDifference(growing, "plabcyer") == fullMatch("plabcyer"),
                "a longer source must push the tail right");
  static_assert(editedDifference(shrinking, "plar") == fullMatch("plar"), "a shorter source must pull the tail left");
  static_assert(editedDifference(beyond, "plabc") == fullMatch("plabc"), "a count past the end must stop at the end");
  static_assert(editedDifference(atEnd, listedAppended) == fullMatch(listedAppended),
                "an offset of size() must remove nothing and append the source");
}

// A counted source takes exactly its count, a null byte included; a null pointer with a count of zero only removes.
TEST_CASE("string/replacement_from_pointer") {
  constexpr auto counted = [](Fixed & string) {
    string.replace(substringOffset, substringLength, embeddedNull, embeddedNullLength);
  };
  constexpr auto empty = [](Fixed & string) {
    string.replace(substringOffset, substringLength, nullptr, 0);
  };

  CHECK(editedDifference(counted, embeddedNullReplacing, embeddedNullReplacingLength)
        == embeddedNullReplacingLength + 1);
  CHECK(editedDifference(empty, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle));

  static_assert(editedDifference(counted, embeddedNullReplacing, embeddedNullReplacingLength)
                  == embeddedNullReplacingLength + 1,
                "a counted source must take every byte, a null byte included");
  static_assert(editedDifference(empty, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle),
                "a null pointer with a count of zero must only remove the run");
}

// Copies of one character take the place of the run; a count of zero only removes it.
TEST_CASE("string/replacement_fill") {
  constexpr auto filled = [](Fixed & string) {
    string.replace(substringOffset, substringLength, fillCount, fillCharacter);
  };
  constexpr auto none = [](Fixed & string) {
    string.replace(substringOffset, substringLength, 0, fillCharacter);
  };
  constexpr auto atEnd = [](Fixed & string) {
    string.replace(string.size(), 0, fillCount, fillCharacter);
  };

  CHECK(editedDifference(filled, filledReplacing) == fullMatch(filledReplacing));
  CHECK(editedDifference(none, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle));
  CHECK(editedDifference(atEnd, filledBack) == fullMatch(filledBack));

  static_assert(editedDifference(filled, filledReplacing) == fullMatch(filledReplacing),
                "copies must take the place of the run");
  static_assert(editedDifference(none, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle),
                "a count of zero must only remove the run");
  static_assert(editedDifference(atEnd, filledBack) == fullMatch(filledBack),
                "copies at size() must follow the old characters");
}

// A view or a string over another storage takes the place of the run, whole or as a substring.
TEST_CASE("string/replacement_from_string_like") {
  constexpr auto view = [](Fixed & string) {
    string.replace(substringOffset, substringLength, StringView{listedText});
  };
  constexpr auto wide = [](Fixed & string) {
    string.replace(substringOffset, substringLength, Wide(listedText));
  };
  constexpr auto substring = [](Fixed & string) {
    string.replace(substringOffset, substringLength, StringView{listedText}, 1, 1);
  };
  constexpr auto toEnd = [](Fixed & string) {
    string.replace(substringOffset, substringLength, StringView{listedText}, 1);
  };

  CHECK(editedDifference(view, listedReplacing) == fullMatch(listedReplacing));
  CHECK(editedDifference(wide, listedReplacing) == fullMatch(listedReplacing));
  CHECK(editedDifference(substring, "plbr") == fullMatch("plbr"));
  CHECK(editedDifference(toEnd, "plbcr") == fullMatch("plbcr"));

  static_assert(editedDifference(view, listedReplacing) == fullMatch(listedReplacing),
                "a view must take the place of the run");
  static_assert(editedDifference(wide, listedReplacing) == fullMatch(listedReplacing),
                "a string over another storage must take the place of the run");
  static_assert(editedDifference(substring, "plbr") == fullMatch("plbr"),
                "a substring must take at most count bytes from the offset");
  static_assert(editedDifference(toEnd, "plbcr") == fullMatch("plbcr"),
                "the default count must take the source up to its end");
}

// Each source takes the place of the characters between an iterator and a const_iterator.
TEST_CASE("string/replacement_at_iterators") {
  constexpr auto counted = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength, listedText,
                   2);
  };
  constexpr auto terminated = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength, listedText);
  };
  constexpr auto view = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength,
                   StringView{listedText});
  };
  constexpr auto filled = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength, fillCount,
                   fillCharacter);
  };
  constexpr auto listed = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength,
                   {'a', 'b', 'c'});
  };
  constexpr auto empty = [](Fixed & string) {
    string.replace(string.begin() + 1, string.cbegin() + 1, listedText);
  };

  CHECK(editedDifference(counted, "plabr") == fullMatch("plabr"));
  CHECK(editedDifference(terminated, listedReplacing) == fullMatch(listedReplacing));
  CHECK(editedDifference(view, listedReplacing) == fullMatch(listedReplacing));
  CHECK(editedDifference(filled, filledReplacing) == fullMatch(filledReplacing));
  CHECK(editedDifference(listed, listedReplacing) == fullMatch(listedReplacing));
  CHECK(editedDifference(empty, "pabclayer") == fullMatch("pabclayer"));

  static_assert(editedDifference(counted, "plabr") == fullMatch("plabr"),
                "a counted source must take the place of the range");
  static_assert(editedDifference(terminated, listedReplacing) == fullMatch(listedReplacing),
                "a C string must take the place of the range");
  static_assert(editedDifference(view, listedReplacing) == fullMatch(listedReplacing),
                "a view must take the place of the range");
  static_assert(editedDifference(filled, filledReplacing) == fullMatch(filledReplacing),
                "copies must take the place of the range");
  static_assert(editedDifference(listed, listedReplacing) == fullMatch(listedReplacing),
                "a braced list must take the place of the range");
  static_assert(editedDifference(empty, "pabclayer") == fullMatch("pabclayer"),
                "an empty range must insert the source at the iterator");
}

// An iterator range takes the place of the run, whether it is contiguous, forward only, or single-pass.
TEST_CASE("string/replacement_from_iterators") {
  constexpr auto contiguous = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength, listedText,
                   listedText + listedLength);
  };
  constexpr auto forward = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength,
                   std::reverse_iterator(listedText + listedLength), std::reverse_iterator(listedText));
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength,
                   SinglePassIterator(listedText), SinglePassIterator(listedText + listedLength));
  };
  constexpr auto empty = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength, listedText,
                   listedText);
  };

  CHECK(editedDifference(contiguous, listedReplacing) == fullMatch(listedReplacing));
  CHECK(editedDifference(forward, listedReversedReplacing) == fullMatch(listedReversedReplacing));
  CHECK(editedDifference(singlePass, listedReplacing) == fullMatch(listedReplacing));
  CHECK(editedDifference(empty, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle));

  static_assert(editedDifference(contiguous, listedReplacing) == fullMatch(listedReplacing),
                "a pointer range must take the place of the run");
  static_assert(editedDifference(forward, listedReversedReplacing) == fullMatch(listedReversedReplacing),
                "a forward range must go in in its own order");
  static_assert(editedDifference(singlePass, listedReplacing) == fullMatch(listedReplacing),
                "a single-pass range must be rotated into place");
  static_assert(editedDifference(empty, sampleWithoutMiddle) == fullMatch(sampleWithoutMiddle),
                "an empty range must only remove the run");
}

// A single-pass source shorter or longer than the run fits, and needs room only for the final length.
TEST_CASE("string/replacement_from_single_pass") {
  constexpr auto shorter = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength,
                   SinglePassIterator(listedText), SinglePassIterator(listedText + 1));
  };
  constexpr auto longer = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + 1,
                   SinglePassIterator(listedText), SinglePassIterator(listedText + listedLength));
  };
  constexpr auto nearlyFull = [](Fixed & string) {
    string.assign(longText, capacity);
    string.replace_with_range(string.begin() + substringOffset, string.cbegin() + substringOffset + fillCount,
                              std::ranges::subrange(SinglePassIterator(listedText),
                                                    SinglePassIterator(listedText + listedLength)));
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
    string.replace_with_range(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength,
                              StringView{listedText});
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.replace_with_range(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength,
                              std::ranges::subrange(SinglePassIterator(listedText),
                                                    SinglePassIterator(listedText + listedLength)));
  };

  CHECK(editedDifference(view, listedReplacing) == fullMatch(listedReplacing));
  CHECK(editedDifference(singlePass, listedReplacing) == fullMatch(listedReplacing));

  static_assert(editedDifference(view, listedReplacing) == fullMatch(listedReplacing),
                "a contiguous range must take the place of the run");
  static_assert(editedDifference(singlePass, listedReplacing) == fullMatch(listedReplacing),
                "a single-pass range must be rotated into place");
}

// Characters read from the string itself take the place of a run unchanged, even when the buffer moves to grow.
TEST_CASE("string/replacement_from_itself") {
  constexpr auto whole = [](Relocating & string) {
    string.replace(substringOffset, 1, string);
  };
  constexpr auto iterated = [](Relocating & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + 1, string.begin(),
                   string.end());
  };
  constexpr auto ranged = [](Relocating & string) {
    string.replace_with_range(string.begin() + substringOffset, string.cbegin() + substringOffset + 1,
                              StringView{string.c_str()});
  };

  // Names the quadruple that breaks, where the static_assert below only counts them.
  const auto checkSelfReplacement = [&](size_t position, size_t removed, size_t offset, size_t count, size_t difference,
                                        size_t match) {
    INFO("position ", position);
    INFO("removed ", removed);
    INFO("offset ", offset);
    INFO("count ", count);
    CHECK(difference == match);
  };

  forEachSelfReplacement<Fixed>(checkSelfReplacement);
  forEachSelfReplacement<Relocating>(checkSelfReplacement);
  CHECK(editedDifference<Relocating>(whole, sampleSpliced) == fullMatch(sampleSpliced));
  CHECK(editedDifference<Relocating>(iterated, sampleSpliced) == fullMatch(sampleSpliced));
  CHECK(editedDifference<Relocating>(ranged, sampleSpliced) == fullMatch(sampleSpliced));

  static_assert(selfReplacementFailures<Fixed>() == 0, "every slice of the string must survive replacing a run of it");
  static_assert(selfReplacementFailures<Relocating>() == 0, "every slice must survive a buffer that moves to grow");
  static_assert(editedDifference<Relocating>(whole, sampleSpliced) == fullMatch(sampleSpliced),
                "the string must take the place of a run of itself whole");
  static_assert(editedDifference<Relocating>(iterated, sampleSpliced) == fullMatch(sampleSpliced),
                "an iterator range over the string must take the place of a run of itself");
  static_assert(editedDifference<Relocating>(ranged, sampleSpliced) == fullMatch(sampleSpliced),
                "a view of the string must take the place of a run of itself");
}

// With assertions compiled out, the cases below reach the documented fallbacks.
#if !defined(TOYGINE_ASSERTIONS_ENABLED)
// A construction the storage rejects leaves the string empty, whatever the source.
TEST_CASE("string/construction_past_capacity") {
  CHECK(holds(Fixed(capacity + 1, fillCharacter), ""));
  CHECK(holds(Fixed(longText, longLength), ""));
  CHECK(holds(Fixed(longText, longText + longLength), ""));
  CHECK(holds(Fixed(SinglePassIterator(longText), SinglePassIterator(longText + longLength)), ""));
  CHECK(holds(Fixed(StringView{longText}), ""));
  CHECK(holds(Fixed(Wide(longText), substringOffset), ""));

  static_assert(holds(Fixed(capacity + 1, fillCharacter), ""), "a rejected fill must leave the string empty");
  static_assert(holds(Fixed(longText, longLength), ""), "a rejected counted pointer must leave the string empty");
  static_assert(holds(Fixed(longText, longText + longLength), ""), "a rejected pointer range must leave it empty");
  static_assert(holds(Fixed(SinglePassIterator(longText), SinglePassIterator(longText + longLength)), ""),
                "a rejected single-pass range must leave the string empty");
  static_assert(holds(Fixed(StringView{longText}), ""), "a rejected view must leave the string empty");
  static_assert(holds(Fixed(Wide(longText), substringOffset), ""), "a rejected substring must leave it empty");
}

// An assignment the storage rejects keeps the old contents, except from a single pass.
TEST_CASE("string/assignment_past_capacity") {
  constexpr auto fill = [](Fixed & string) {
    string.assign(capacity + 1, fillCharacter);
  };
  constexpr auto counted = [](Fixed & string) {
    string.assign(longText, longLength);
  };
  constexpr auto contiguous = [](Fixed & string) {
    string.assign(longText, longText + longLength);
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.assign(SinglePassIterator(longText), SinglePassIterator(longText + longLength));
  };

  CHECK(editedDifference(fill, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(counted, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(contiguous, sampleText) == fullMatch(sampleText));

  // A single pass overwrites the string before it learns the length, so the rejection leaves nothing.
  CHECK(editedDifference(singlePass, "") == fullMatch(""));

  static_assert(editedDifference(fill, sampleText) == fullMatch(sampleText), "a rejected fill must keep the string");
  static_assert(editedDifference(counted, sampleText) == fullMatch(sampleText),
                "a rejected counted pointer must keep the string");
  static_assert(editedDifference(contiguous, sampleText) == fullMatch(sampleText),
                "a rejected pointer range must keep the string");
  static_assert(editedDifference(singlePass, "") == fullMatch(""),
                "a rejected single-pass range must leave the string empty");
}

// An append the storage rejects leaves the string unchanged, a single pass included.
TEST_CASE("string/appending_past_capacity") {
  constexpr auto fill = [](Fixed & string) {
    string.append(capacity, fillCharacter);
  };
  constexpr auto counted = [](Fixed & string) {
    string.append(longText, longLength);
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.append(SinglePassIterator(longText), SinglePassIterator(longText + longLength));
  };
  constexpr auto characterToFull = [] {
    Fixed string(longText, capacity);
    string.push_back(fillCharacter);
    string += fillCharacter;

    return firstDifference(string, longText, capacity);
  };

  CHECK(editedDifference(fill, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(counted, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(singlePass, sampleText) == fullMatch(sampleText));
  CHECK(characterToFull() == capacity + 1);

  static_assert(editedDifference(fill, sampleText) == fullMatch(sampleText), "a rejected fill must keep the string");
  static_assert(editedDifference(counted, sampleText) == fullMatch(sampleText),
                "a rejected counted pointer must keep the string");
  static_assert(editedDifference(singlePass, sampleText) == fullMatch(sampleText),
                "a rejected single-pass range must keep the string");
  static_assert(characterToFull() == capacity + 1, "a character past a full string must keep it as it was");
}

// An insertion the storage rejects leaves the string unchanged, a single pass included.
TEST_CASE("string/insertion_past_capacity") {
  constexpr auto fill = [](Fixed & string) {
    string.insert(substringOffset, capacity, fillCharacter);
  };
  constexpr auto counted = [](Fixed & string) {
    string.insert(substringOffset, longText, longLength);
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.insert(string.begin() + substringOffset, SinglePassIterator(longText),
                  SinglePassIterator(longText + longLength));
  };

  CHECK(editedDifference(fill, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(counted, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(singlePass, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(fill, sampleText) == fullMatch(sampleText), "a rejected fill must keep the string");
  static_assert(editedDifference(counted, sampleText) == fullMatch(sampleText),
                "a rejected counted pointer must keep the string");
  static_assert(editedDifference(singlePass, sampleText) == fullMatch(sampleText),
                "a rejected single-pass range must keep the string");
}

// A replacement the storage rejects leaves the string unchanged, except from a single pass.
TEST_CASE("string/replacement_past_capacity") {
  constexpr auto fill = [](Fixed & string) {
    string.replace(substringOffset, substringLength, capacity, fillCharacter);
  };
  constexpr auto counted = [](Fixed & string) {
    string.replace(substringOffset, substringLength, longText, longLength);
  };
  constexpr auto contiguous = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength, longText,
                   longText + longLength);
  };
  constexpr auto singlePass = [](Fixed & string) {
    string.replace(string.begin() + substringOffset, string.cbegin() + substringOffset + substringLength,
                   SinglePassIterator(longText), SinglePassIterator(longText + longLength));
  };

  CHECK(editedDifference(fill, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(counted, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(contiguous, sampleText) == fullMatch(sampleText));

  // A single pass overwrites the run before it learns the length, so the old length comes back over those characters.
  CHECK(editedDifference(singlePass, "plabcr") == fullMatch("plabcr"));

  static_assert(editedDifference(fill, sampleText) == fullMatch(sampleText), "a rejected fill must keep the string");
  static_assert(editedDifference(counted, sampleText) == fullMatch(sampleText),
                "a rejected counted pointer must keep the string");
  static_assert(editedDifference(contiguous, sampleText) == fullMatch(sampleText),
                "a rejected pointer range must keep the string");
  static_assert(editedDifference(singlePass, "plabcr") == fullMatch("plabcr"),
                "a rejected single-pass range must keep the old length over the overwritten run");
}

// A substring construction from an offset past the end leaves the string empty.
TEST_CASE("string/construction_past_end") {
  constexpr auto fromMoved = [] {
    Fixed source(sampleText);

    return Fixed(std::move(source), sampleLength + 1);
  };

  CHECK(holds(Fixed(sampleString, sampleLength + 1), ""));
  CHECK(holds(fromMoved(), ""));

  static_assert(holds(Fixed(sampleString, sampleLength + 1), ""), "a copied substring from past the end must be empty");
  static_assert(holds(fromMoved(), ""), "a substring taken over from a moved string must be empty");
}

// An edit at an offset past the end of the string or of its source leaves it unchanged.
TEST_CASE("string/edit_past_end") {
  constexpr auto assigned = [](Fixed & string) {
    string.assign(StringView{listedText}, listedLength + 1);
  };
  constexpr auto insertedFill = [](Fixed & string) {
    string.insert(sampleLength + 1, 1, fillCharacter);
  };
  constexpr auto insertedCounted = [](Fixed & string) {
    string.insert(sampleLength + 1, listedText, listedLength);
  };
  constexpr auto insertedFromPastSource = [](Fixed & string) {
    string.insert(substringOffset, StringView{listedText}, listedLength + 1);
  };
  constexpr auto appended = [](Fixed & string) {
    string.append(StringView{listedText}, listedLength + 1);
  };
  constexpr auto erased = [](Fixed & string) {
    string.erase(sampleLength + 1);
  };
  constexpr auto replacedCounted = [](Fixed & string) {
    string.replace(sampleLength + 1, 1, listedText, listedLength);
  };
  constexpr auto replacedFill = [](Fixed & string) {
    string.replace(sampleLength + 1, 1, 1, fillCharacter);
  };
  constexpr auto replacedFromPastSource = [](Fixed & string) {
    string.replace(substringOffset, 1, StringView{listedText}, listedLength + 1);
  };

  CHECK(editedDifference(assigned, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(insertedFill, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(insertedCounted, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(insertedFromPastSource, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(appended, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(erased, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(replacedCounted, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(replacedFill, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(replacedFromPastSource, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(assigned, sampleText) == fullMatch(sampleText),
                "an assignment from past the source must keep the string");
  static_assert(editedDifference(insertedFill, sampleText) == fullMatch(sampleText),
                "a fill inserted past the end must keep the string");
  static_assert(editedDifference(insertedCounted, sampleText) == fullMatch(sampleText),
                "a pointer inserted past the end must keep the string");
  static_assert(editedDifference(insertedFromPastSource, sampleText) == fullMatch(sampleText),
                "an insertion from past the source must keep the string");
  static_assert(editedDifference(appended, sampleText) == fullMatch(sampleText),
                "an append from past the source must keep the string");
  static_assert(editedDifference(erased, sampleText) == fullMatch(sampleText),
                "an erasure past the end must keep the string");
  static_assert(editedDifference(replacedCounted, sampleText) == fullMatch(sampleText),
                "a pointer replacement past the end must keep the string");
  static_assert(editedDifference(replacedFill, sampleText) == fullMatch(sampleText),
                "a fill replacement past the end must keep the string");
  static_assert(editedDifference(replacedFromPastSource, sampleText) == fullMatch(sampleText),
                "a replacement from past the source must keep the string");
}

// An iterator edit at end() or over a reversed run leaves the string unchanged.
TEST_CASE("string/edit_at_iterators_out_of_order") {
  constexpr auto erasedAtEnd = [](Fixed & string) {
    string.erase(string.end());
  };
  constexpr auto erasedReversed = [](Fixed & string) {
    string.erase(string.begin() + substringOffset + substringLength, string.cbegin() + substringOffset);
  };
  constexpr auto replacedCounted = [](Fixed & string) {
    string.replace(string.begin() + substringOffset + substringLength, string.cbegin() + substringOffset, listedText,
                   listedLength);
  };
  constexpr auto replacedFill = [](Fixed & string) {
    string.replace(string.begin() + substringOffset + substringLength, string.cbegin() + substringOffset, 1,
                   fillCharacter);
  };
  constexpr auto replacedSinglePass = [](Fixed & string) {
    string.replace(string.begin() + substringOffset + substringLength, string.cbegin() + substringOffset,
                   SinglePassIterator(listedText), SinglePassIterator(listedText + listedLength));
  };
  constexpr auto endReturned = [] {
    Fixed string(sampleText);

    return string.erase(string.end()) == string.end();
  };

  CHECK(editedDifference(erasedAtEnd, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(erasedReversed, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(replacedCounted, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(replacedFill, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(replacedSinglePass, sampleText) == fullMatch(sampleText));
  CHECK(endReturned());

  static_assert(editedDifference(erasedAtEnd, sampleText) == fullMatch(sampleText),
                "erasing at end() must keep the string");
  static_assert(editedDifference(erasedReversed, sampleText) == fullMatch(sampleText),
                "erasing a reversed run must keep the string");
  static_assert(editedDifference(replacedCounted, sampleText) == fullMatch(sampleText),
                "replacing a reversed run with a pointer must keep the string");
  static_assert(editedDifference(replacedFill, sampleText) == fullMatch(sampleText),
                "replacing a reversed run with a fill must keep the string");
  static_assert(editedDifference(replacedSinglePass, sampleText) == fullMatch(sampleText),
                "replacing a reversed run with a single-pass range must keep the string");
  static_assert(endReturned(), "erasing at end() must return end()");
}

// A read from an offset past the end sees an empty range.
TEST_CASE("string/read_past_end") {
  constexpr auto copied = [](char * buffer) {
    return sampleString.copy(buffer, substringLength, sampleLength + 1);
  };

  CHECK(copiedBy(copied).count == 0);
  CHECK(copiedBy(copied).buffer[0] == fillCharacter);
  CHECK(sampleString.compare(sampleLength + 1, 1, StringView{}) == 0);
  CHECK(sampleString.compare(sampleLength + 1, 1, StringView{listedText}) < 0);
  CHECK(sampleString.compare(0, Fixed::npos, StringView{listedText}, listedLength + 1) > 0);
  CHECK(sampleString.compare(sampleLength + 1, 1, listedText) < 0);
  CHECK(sampleString.compare(sampleLength + 1, 1, listedText, listedLength) < 0);
  CHECK(holds(sampleString.substr(sampleLength + 1), ""));
  CHECK(holds(Fixed(sampleText).substr(sampleLength + 1), ""));
  CHECK(sampleString.subview(sampleLength + 1).empty());
  CHECK(sampleString.subview(sampleLength + 1).data() == sampleString.data() + sampleLength);

  static_assert(copiedBy(copied).count == 0, "a copy from past the end must write nothing");
  static_assert(copiedBy(copied).buffer[0] == fillCharacter,
                "a copy from past the end must leave the buffer untouched");
  static_assert(sampleString.compare(sampleLength + 1, 1, StringView{}) == 0,
                "a part past the end must compare equal to an empty string");
  static_assert(sampleString.compare(sampleLength + 1, 1, StringView{listedText}) < 0,
                "a part past the end must order before a non-empty string");
  static_assert(sampleString.compare(0, Fixed::npos, StringView{listedText}, listedLength + 1) > 0,
                "an argument part past its end must compare as empty");
  static_assert(sampleString.compare(sampleLength + 1, 1, listedText) < 0,
                "a part past the end must order before a non-empty pointer");
  static_assert(sampleString.compare(sampleLength + 1, 1, listedText, listedLength) < 0,
                "a part past the end must order before a non-empty counted pointer");
  static_assert(holds(sampleString.substr(sampleLength + 1), ""), "a copied substring past the end must be empty");
  static_assert(holds(Fixed(sampleText).substr(sampleLength + 1), ""), "a moved substring past the end must be empty");
  static_assert(sampleString.subview(sampleLength + 1).data() == sampleString.data() + sampleLength,
                "a view past the end must view the empty tail");
}

// A size change the storage rejects leaves the string unchanged and calls no operation.
TEST_CASE("string/resizing_past_capacity") {
  constexpr auto resized = [](Fixed & string) {
    string.resize(capacity + 1);
  };
  constexpr auto resizedWithFill = [](Fixed & string) {
    string.resize(capacity + 1, fillCharacter);
  };
  constexpr auto reserved = [](Fixed & string) {
    string.reserve(capacity + 1);
  };
  constexpr auto overwritten = [](Fixed & string) {
    string.resize_and_overwrite(capacity + 1, [](char * data, size_t) {
      data[0] = fillCharacter;
      return size_t{0};
    });
  };

  CHECK(editedDifference(resized, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(resizedWithFill, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(reserved, sampleText) == fullMatch(sampleText));
  CHECK(editedDifference(overwritten, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(resized, sampleText) == fullMatch(sampleText),
                "a rejected resize must keep the string");
  static_assert(editedDifference(resizedWithFill, sampleText) == fullMatch(sampleText),
                "a rejected resize with a fill must keep the string");
  static_assert(editedDifference(reserved, sampleText) == fullMatch(sampleText),
                "a rejected reserve must keep the string");
  static_assert(editedDifference(overwritten, sampleText) == fullMatch(sampleText),
                "a rejected count must keep the string and leave the operation uncalled");
}

// An operation returning a size outside [0, count] keeps the old size over what it wrote.
TEST_CASE("string/resize_and_overwrite_out_of_range") {
  constexpr auto tooLarge = [](Fixed & string) {
    string.resize_and_overwrite(capacity, [](char * data, size_t count) {
      std::char_traits<char>::assign(data, count, fillCharacter);
      return count + 1;
    });
  };
  constexpr auto negative = [](Fixed & string) {
    string.resize_and_overwrite(capacity, [](char * data, size_t) {
      data[0] = fillCharacter;
      return -1;
    });
  };

  // The operation wrote over the old terminator, so the call writes it again after the old size.
  CHECK(editedDifference(tooLarge, "xxxxxx") == fullMatch("xxxxxx"));
  CHECK(editedDifference(negative, "xlayer") == fullMatch("xlayer"));

  static_assert(editedDifference(tooLarge, "xxxxxx") == fullMatch("xxxxxx"),
                "a size past the count must keep the old size, terminated, over the written characters");
  static_assert(editedDifference(negative, "xlayer") == fullMatch("xlayer"),
                "a negative size must keep the old size over the written characters");
}
#endif // !TOYGINE_ASSERTIONS_ENABLED

// copy() writes up to count characters from an offset, stops at the end, adds no terminator, and returns the count.
TEST_CASE("string/copy") {
  constexpr auto middle = [](char * buffer) {
    return sampleString.copy(buffer, substringLength, substringOffset);
  };
  constexpr auto beyond = [](char * buffer) {
    return sampleString.copy(buffer, longLength, substringOffset);
  };
  constexpr auto fromFront = [](char * buffer) {
    return sampleString.copy(buffer, substringOffset);
  };
  constexpr auto atEnd = [](char * buffer) {
    return sampleString.copy(buffer, substringLength, sampleLength);
  };
  constexpr auto nowhere = [](char *) {
    return sampleString.copy(nullptr, 0);
  };

  const CopiedCharacters copied = copiedBy(middle);
  REQUIRE(copied.count == substringLength);
  CHECK(std::char_traits<char>::compare(copied.buffer.data(), sampleText + substringOffset, substringLength) == 0);
  CHECK(copied.buffer[substringLength] == fillCharacter);
  CHECK(copiedBy(beyond).count == tailLength);
  CHECK(copiedBy(fromFront).buffer[0] == sampleText[0]);
  CHECK(copiedBy(atEnd).count == 0);
  CHECK(copiedBy(atEnd).buffer[0] == fillCharacter);
  CHECK(copiedBy(nowhere).count == 0);

  static_assert(copiedBy(middle).count == substringLength, "copy() must return the count it wrote");
  static_assert(std::char_traits<char>::compare(copiedBy(middle).buffer.data(), sampleText + substringOffset,
                                                substringLength)
                  == 0,
                "the characters must come from the offset");
  static_assert(copiedBy(middle).buffer[substringLength] == fillCharacter, "copy() must write no terminator");
  static_assert(copiedBy(beyond).count == tailLength, "a count past the end must stop at the end");
  static_assert(copiedBy(fromFront).count == substringOffset && copiedBy(fromFront).buffer[0] == sampleText[0],
                "the default offset must copy from the first character");
  static_assert(copiedBy(atEnd).count == 0, "an offset of size() must copy nothing");
  static_assert(copiedBy(nowhere).count == 0, "a null destination with a count of zero must copy nothing");
}

// resize() drops characters past the new size or appends copies of a character, a null one by default.
TEST_CASE("string/resize") {
  constexpr auto filled = [](Fixed & string) {
    string.resize(sampleLength + fillCount, fillCharacter);
  };
  constexpr auto nulls = [](Fixed & string) {
    string.resize(sampleWithNullsLength);
  };
  constexpr auto shrunk = [](Fixed & string) {
    string.resize(substringOffset, fillCharacter);
  };
  constexpr auto unchanged = [](Fixed & string) {
    string.resize(sampleLength, fillCharacter);
  };

  CHECK(editedDifference(filled, filledBack) == fullMatch(filledBack));
  CHECK(editedDifference(nulls, sampleWithNulls, sampleWithNullsLength) == sampleWithNullsLength + 1);
  CHECK(editedDifference(shrunk, sampleHead) == fullMatch(sampleHead));
  CHECK(editedDifference(unchanged, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(filled, filledBack) == fullMatch(filledBack),
                "growing must append copies of the character");
  static_assert(editedDifference(nulls, sampleWithNulls, sampleWithNullsLength) == sampleWithNullsLength + 1,
                "growing without a character must append null bytes");
  static_assert(editedDifference(shrunk, sampleHead) == fullMatch(sampleHead),
                "shrinking must drop the characters past the new size");
  static_assert(editedDifference(unchanged, sampleText) == fullMatch(sampleText), "the same size must change nothing");
}

// The operation sees the old characters and the requested count, and the size it returns becomes the new size.
TEST_CASE("string/resize_and_overwrite") {
  constexpr auto filled = [](Fixed & string) {
    string.resize_and_overwrite(sampleLength + fillCount, [](char * data, size_t count) {
      std::char_traits<char>::assign(data + sampleLength, count - sampleLength, fillCharacter);
      return count;
    });
  };
  constexpr auto shortened = [](Fixed & string) {
    string.resize_and_overwrite(capacity, [](char *, size_t) {
      return substringOffset;
    });
  };
  constexpr auto truncated = [](Fixed & string) {
    string.resize_and_overwrite(substringOffset, [](char *, size_t count) {
      return count;
    });
  };
  constexpr auto signedSize = [](Fixed & string) {
    string.resize_and_overwrite(capacity, [](char *, size_t) {
      return static_cast<int>(sampleLength);
    });
  };

  CHECK(editedDifference(filled, filledBack) == fullMatch(filledBack));
  CHECK(editedDifference(shortened, sampleHead) == fullMatch(sampleHead));
  CHECK(editedDifference(truncated, sampleHead) == fullMatch(sampleHead));
  CHECK(editedDifference(signedSize, sampleText) == fullMatch(sampleText));

  static_assert(editedDifference(filled, filledBack) == fullMatch(filledBack),
                "the operation must find the old characters and write up to the requested count");
  static_assert(editedDifference(shortened, sampleHead) == fullMatch(sampleHead),
                "a size below the requested count must become the new size");
  static_assert(editedDifference(truncated, sampleHead) == fullMatch(sampleHead),
                "a count below size() must keep the characters before it");
  static_assert(editedDifference(signedSize, sampleText) == fullMatch(sampleText),
                "a signed size must be taken as the new size");
}

// swap() exchanges the contents of two strings, whichever way the storage copies, and a self-swap changes nothing.
TEST_CASE("string/swap") {
  constexpr auto self = [](Fixed & string) {
    string.swap(string);
  };

  const auto fixed = swappedDifferences<Fixed>();
  const auto large = swappedDifferences<Large>();
  CHECK(fixed[0] == fullMatch(listedText));
  CHECK(fixed[1] == fullMatch(sampleText));
  CHECK(large[0] == fullMatch(listedText));
  CHECK(large[1] == fullMatch(sampleText));
  CHECK(editedDifference(self, sampleText) == fullMatch(sampleText));

  static_assert(swappedDifferences<Fixed>() == std::array{fullMatch(listedText), fullMatch(sampleText)},
                "a storage copied whole must exchange contents");
  static_assert(swappedDifferences<Large>() == std::array{fullMatch(listedText), fullMatch(sampleText)},
                "a storage copied by length must exchange contents");
  static_assert(editedDifference(self, sampleText) == fullMatch(sampleText), "a self-swap must change nothing");
}

// Which modifiers never throw, what each returns, and that a literal 0 picks the overload taking an index.
TEST_CASE("string/modifier_signatures") {
  static_assert(noexcept(std::declval<Fixed &>().clear()) && noexcept(std::declval<Fixed &>().erase())
                  && noexcept(std::declval<Fixed &>().insert(0, listedText))
                  && noexcept(std::declval<Fixed &>().insert(std::declval<Fixed &>().begin(), fillCharacter)),
                "the modifiers must not throw, where std::basic_string throws on a bad offset or length");

  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().insert(0, 1, fillCharacter)), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>().erase(0)), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>().erase(0, 0)), Fixed &>,
                "a literal 0 must select the overloads taking an index, which return the string");
  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().insert(std::declval<Fixed::const_iterator>(),
                                                                       fillCharacter)),
                               Fixed::iterator>
                  && std::is_same_v<decltype(std::declval<Fixed &>().erase(std::declval<Fixed::iterator>())),
                                    Fixed::iterator>
                  && std::is_same_v<decltype(std::declval<Fixed &>().insert_range(std::declval<Fixed::iterator>(),
                                                                                  StringView{})),
                                    Fixed::iterator>,
                "the overloads taking an iterator must return a mutable iterator, as std::basic_string does");

  static_assert(noexcept(std::declval<Fixed &>().push_back(fillCharacter))
                  && noexcept(std::declval<Fixed &>().pop_back())
                  && noexcept(std::declval<Fixed &>().append(listedText))
                  && noexcept(std::declval<Fixed &>() += listedText)
                  && noexcept(std::declval<Fixed &>().replace(0, 0, listedText))
                  && noexcept(std::declval<const Fixed &>().copy(nullptr, 0))
                  && noexcept(std::declval<Fixed &>().resize(0))
                  && noexcept(std::declval<Fixed &>().swap(std::declval<Fixed &>())),
                "the modifiers added with append and replace must not throw either");

  static_assert(std::is_same_v<decltype(std::declval<Fixed &>().append(0, fillCharacter)), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>() += fillCharacter), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>().replace(0, 0, 0, fillCharacter)), Fixed &>
                  && std::is_same_v<decltype(std::declval<Fixed &>().replace(std::declval<Fixed::iterator>(),
                                                                             std::declval<Fixed::const_iterator>(),
                                                                             listedText)),
                                    Fixed &>
                  && std::is_same_v<decltype(std::declval<const Fixed &>().copy(nullptr, 0)), size_t>,
                "append(), operator+= and replace() must return the string, and copy() the count it wrote");
}

// Where each overload of find() first matches: a view, another storage, a counted or measured pointer, or a character.
TEST_CASE("string/find") {
  CHECK(repeatedString.find(StringView{"abra"}) == 0);
  CHECK(repeatedString.find(Wide("abra"), 1) == 7);
  CHECK(repeatedString.find("abrasive", 1, 4) == 7);
  CHECK(repeatedString.find("cad") == 4);
  CHECK(repeatedString.find("cad", 5) == Fixed::npos);
  CHECK(repeatedString.find('a') == 0);
  CHECK(repeatedString.find('a', 1) == 3);

  // A null pointer with a count of zero matches at the offset, as an empty pattern does.
  CHECK(repeatedString.find(nullptr, 4, 0) == 4);

  // An empty string holds no character, though an empty pattern still matches at its start.
  CHECK(emptyString.find('a') == Fixed::npos);
  CHECK(emptyString.find("") == 0);

  static_assert(repeatedString.find(Wide("abra"), 1) == 7, "a string over another storage must pass its offset on");
  static_assert(repeatedString.find("abrasive", 1, 4) == 7, "the count must bound what is read");
  static_assert(repeatedString.find("cad") == 4, "the pointer overload must measure its argument");
  static_assert(repeatedString.find('a', 1) == 3, "the character overload must pass its offset on");
  static_assert(emptyString.find("") == 0, "an empty pattern must match at the start of an empty string");
}

// Where each overload of rfind() last matches, the default offset covering the whole string.
TEST_CASE("string/rfind") {
  CHECK(repeatedString.rfind(StringView{"abra"}) == 7);
  CHECK(repeatedString.rfind(Wide("abra"), 6) == 0);
  CHECK(repeatedString.rfind("abrasive", Fixed::npos, 4) == 7);
  CHECK(repeatedString.rfind("cad") == 4);
  CHECK(repeatedString.rfind("cad", 3) == Fixed::npos);
  CHECK(repeatedString.rfind('a') == repeatedLength - 1);
  CHECK(repeatedString.rfind('a', 9) == 7);

  // An empty pattern matches at the end of the string.
  CHECK(repeatedString.rfind("") == repeatedLength);

  static_assert(repeatedString.rfind(Wide("abra"), 6) == 0, "a string over another storage must pass its offset on");
  static_assert(repeatedString.rfind("abrasive", Fixed::npos, 4) == 7, "the count must bound what is read");
  static_assert(repeatedString.rfind("cad") == 4, "the pointer overload must measure its argument");
  static_assert(repeatedString.rfind('a', 9) == 7, "the character overload must pass its offset on");
  static_assert(repeatedString.rfind("") == repeatedLength, "an empty pattern must match at the end");
}

// Where each overload of find_first_of() first meets a member of the set; a set of one reads as a character search.
TEST_CASE("string/find_first_of") {
  CHECK(repeatedString.find_first_of(StringView{"rc"}) == 2);
  CHECK(repeatedString.find_first_of(Wide("rc"), 3) == 4);
  CHECK(repeatedString.find_first_of("rcxyz", 3, 2) == 4);
  CHECK(repeatedString.find_first_of("rc") == 2);
  CHECK(repeatedString.find_first_of("rc", 10) == Fixed::npos);
  CHECK(repeatedString.find_first_of('d') == 6);
  CHECK(repeatedString.find_first_of('a', 1) == 3);

  static_assert(repeatedString.find_first_of(Wide("rc"), 3) == 4,
                "a string over another storage must pass its offset on");
  static_assert(repeatedString.find_first_of("rcxyz", 3, 2) == 4, "the count must bound what is read");
  static_assert(repeatedString.find_first_of("rc") == 2, "the pointer overload must measure its argument");
  static_assert(repeatedString.find_first_of('d') == 6, "a set of one must be that character");
}

// Where each overload of find_last_of() last meets a member of the set.
TEST_CASE("string/find_last_of") {
  CHECK(repeatedString.find_last_of(StringView{"rc"}) == 9);
  CHECK(repeatedString.find_last_of(Wide("rc"), 8) == 4);
  CHECK(repeatedString.find_last_of("rcxyz", 8, 2) == 4);
  CHECK(repeatedString.find_last_of("rc") == 9);
  CHECK(repeatedString.find_last_of("rc", 1) == Fixed::npos);
  CHECK(repeatedString.find_last_of('a') == repeatedLength - 1);
  CHECK(repeatedString.find_last_of('a', 9) == 7);

  static_assert(repeatedString.find_last_of(Wide("rc"), 8) == 4,
                "a string over another storage must pass its offset on");
  static_assert(repeatedString.find_last_of("rcxyz", 8, 2) == 4, "the count must bound what is read");
  static_assert(repeatedString.find_last_of("rc") == 9, "the pointer overload must measure its argument");
  static_assert(repeatedString.find_last_of('a') == repeatedLength - 1, "a set of one must be that character");
}

// Where each overload of find_first_not_of() first meets a character outside the set.
TEST_CASE("string/find_first_not_of") {
  CHECK(repeatedString.find_first_not_of(StringView{"ab"}) == 2);
  CHECK(repeatedString.find_first_not_of(Wide("ab"), 3) == 4);
  CHECK(repeatedString.find_first_not_of("abxyz", 3, 2) == 4);
  CHECK(repeatedString.find_first_not_of("ab") == 2);
  CHECK(repeatedString.find_first_not_of("ab", 10) == Fixed::npos);
  CHECK(repeatedString.find_first_not_of('a') == 1);
  CHECK(repeatedString.find_first_not_of('a', 3) == 4);

  static_assert(repeatedString.find_first_not_of(Wide("ab"), 3) == 4,
                "a string over another storage must pass its offset on");
  static_assert(repeatedString.find_first_not_of("abxyz", 3, 2) == 4, "the count must bound what is read");
  static_assert(repeatedString.find_first_not_of("ab") == 2, "the pointer overload must measure its argument");
  static_assert(repeatedString.find_first_not_of('a', 3) == 4, "the character overload must pass its offset on");
}

// Where each overload of find_last_not_of() last meets a character outside the set.
TEST_CASE("string/find_last_not_of") {
  CHECK(repeatedString.find_last_not_of(StringView{"ab"}) == 9);
  CHECK(repeatedString.find_last_not_of(Wide("ab"), 8) == 6);
  CHECK(repeatedString.find_last_not_of("abxyz", 8, 2) == 6);
  CHECK(repeatedString.find_last_not_of("ab") == 9);
  CHECK(repeatedString.find_last_not_of("ab", 1) == Fixed::npos);
  CHECK(repeatedString.find_last_not_of('a') == 9);
  CHECK(repeatedString.find_last_not_of('a', 8) == 8);

  static_assert(repeatedString.find_last_not_of(Wide("ab"), 8) == 6,
                "a string over another storage must pass its offset on");
  static_assert(repeatedString.find_last_not_of("abxyz", 8, 2) == 6, "the count must bound what is read");
  static_assert(repeatedString.find_last_not_of("ab") == 9, "the pointer overload must measure its argument");
  static_assert(repeatedString.find_last_not_of('a', 8) == 8, "the character overload must pass its offset on");
}

// A search covers size() characters, so a null byte inside the string neither ends it nor hides what follows.
TEST_CASE("string/search_reads_past_embedded_null") {
  CHECK(embeddedNullString.find('c') == 3);
  CHECK(embeddedNullString.find('\0') == 2);
  CHECK(embeddedNullString.rfind('a') == 0);
  CHECK(embeddedNullString.find_first_of("dc") == 3);
  CHECK(embeddedNullString.find_first_not_of("ab") == 2);
  CHECK(embeddedNullString.find_last_not_of('d') == 3);

  // A pattern holding the null byte matches through the counted pointer and through a string of the same characters.
  CHECK(embeddedNullString.find(embeddedNull + embeddedNullRunOffset, 0, embeddedNullRunLength)
        == embeddedNullRunOffset);
  CHECK(embeddedNullString.find(Fixed(embeddedNull + embeddedNullRunOffset, embeddedNullRunLength))
        == embeddedNullRunOffset);

  static_assert(embeddedNullString.find('c') == 3, "a search must continue past the null byte");
  static_assert(embeddedNullString.rfind('\0') == 2, "a backward search must start at the last character");
  static_assert(embeddedNullString.find_last_not_of('d') == 3, "the last characters must stay reachable");
  static_assert(embeddedNullString.find(Fixed(embeddedNull + embeddedNullRunOffset, embeddedNullRunLength))
                  == embeddedNullRunOffset,
                "a string pattern must keep its null byte");
}

// The searches never throw and report an offset of the string's own size type.
TEST_CASE("string/search_signatures") {
  static_assert(noexcept(repeatedString.find(StringView{})) && noexcept(repeatedString.find(listedText, 0, 0))
                  && noexcept(repeatedString.rfind(listedText)) && noexcept(repeatedString.find_first_of('a'))
                  && noexcept(repeatedString.find_last_of(listedText))
                  && noexcept(repeatedString.find_first_not_of(Wide{}))
                  && noexcept(repeatedString.find_last_not_of(listedText, 0, 0)),
                "the searches must not throw");

  static_assert(std::is_same_v<decltype(repeatedString.find('a')), Fixed::size_type>
                  && std::is_same_v<decltype(repeatedString.rfind(StringView{})), Fixed::size_type>
                  && std::is_same_v<decltype(repeatedString.find_first_of(listedText)), Fixed::size_type>
                  && std::is_same_v<decltype(repeatedString.find_last_not_of(listedText, 0, 0)), Fixed::size_type>,
                "every search must return an offset of size_type");

  // The value every search reports for no match is the one the view and the shared search functions use.
  static_assert(Fixed::npos == StringView::npos && Fixed::npos == string_utils::npos,
                "npos must match StringView::npos and string_utils::npos");
}

// What compare() reports against a view, another storage, or a pointer, whole or by a substring of either side.
TEST_CASE("string/compare") {
  CHECK(sampleString.compare(StringView{sampleText}) == 0);
  CHECK(sampleString.compare(Wide("playground")) < 0);
  CHECK(sampleString.compare("play") > 0);
  CHECK(sampleString.compare("plays") < 0);

  CHECK(sampleString.compare(2, 3, StringView{"aye"}) == 0);
  CHECK(sampleString.compare(2, Fixed::npos, Wide("ayer")) == 0);
  CHECK(sampleString.compare(0, 4, StringView{"xxplayxx"}, 2, 4) == 0);
  CHECK(sampleString.compare(2, 4, Wide(sampleText), 2) == 0);
  CHECK(sampleString.compare(2, 3, "aye") == 0);
  CHECK(sampleString.compare(2, 3, "ayes", 3) == 0);
  CHECK(sampleString.compare(0, 0, nullptr, 0) == 0);

  // A range starting at the end is empty, and so equal to an empty string.
  CHECK(sampleString.compare(sampleLength, 1, "") == 0);
  CHECK(emptyString.compare("") == 0);
  CHECK(emptyString.compare(sampleText) < 0);

  static_assert(sampleString.compare(StringView{sampleText}) == 0, "a string must compare equal to its own characters");
  static_assert(sampleString.compare(Wide("playground")) < 0, "the first differing character must decide");
  static_assert(sampleString.compare("play") > 0, "a string must order after its prefix");
  static_assert(sampleString.compare(2, Fixed::npos, Wide("ayer")) == 0, "a count past the end must stop there");
  static_assert(sampleString.compare(0, 4, StringView{"xxplayxx"}, 2, 4) == 0,
                "both substrings must start at their offsets");
  static_assert(sampleString.compare(2, 4, Wide(sampleText), 2) == 0,
                "the default count must take the other string to its end");
  static_assert(sampleString.compare(2, 3, "ayes", 3) == 0, "the count must bound the counted pointer");
  static_assert(sampleString.compare(0, 0, nullptr, 0) == 0, "a null pointer with a count of zero must be empty");
  static_assert(sampleString.compare(sampleLength, 1, "") == 0, "a range at the end must be empty");
}

// Whether the string opens with a view, another storage, a measured pointer, or a character.
TEST_CASE("string/starts_with") {
  CHECK(sampleString.starts_with(StringView{"pla"}));
  CHECK_FALSE(sampleString.starts_with(Wide("playground")));
  CHECK(sampleString.starts_with(sampleText));
  CHECK_FALSE(sampleString.starts_with("players"));
  CHECK(sampleString.starts_with(""));
  CHECK(sampleString.starts_with('p'));
  CHECK_FALSE(sampleString.starts_with('l'));

  // An empty string starts with the empty string and with no character.
  CHECK(emptyString.starts_with(""));
  CHECK_FALSE(emptyString.starts_with('p'));

  static_assert(sampleString.starts_with(StringView{"pla"}), "a leading part must match");
  static_assert(!sampleString.starts_with(Wide("playground")), "a longer string must not match");
  static_assert(sampleString.starts_with(sampleText), "the whole string must match");
  static_assert(sampleString.starts_with('p') && !sampleString.starts_with('l'),
                "the character overload must read the first character only");
  static_assert(!emptyString.starts_with('p'), "an empty string must start with no character");
}

// Whether the string closes with a view, another storage, a measured pointer, or a character.
TEST_CASE("string/ends_with") {
  CHECK(sampleString.ends_with(StringView{"yer"}));
  CHECK(sampleString.ends_with(Wide("layer")));
  CHECK_FALSE(sampleString.ends_with("aplayer"));
  CHECK_FALSE(sampleString.ends_with("aye"));
  CHECK(sampleString.ends_with(""));
  CHECK(sampleString.ends_with('r'));
  CHECK_FALSE(sampleString.ends_with('e'));

  CHECK(emptyString.ends_with(""));
  CHECK_FALSE(emptyString.ends_with('r'));

  static_assert(sampleString.ends_with(StringView{"yer"}), "a trailing part must match");
  static_assert(sampleString.ends_with(Wide("layer")), "a string over another storage must match");
  static_assert(!sampleString.ends_with("aplayer"), "a longer string must not match");
  static_assert(!sampleString.ends_with("aye"), "a part away from the end must not match");
  static_assert(sampleString.ends_with('r') && !sampleString.ends_with('e'),
                "the character overload must read the last character only");
  static_assert(!emptyString.ends_with('r'), "an empty string must end with no character");
}

// Whether a view, another storage, a measured pointer, or a character appears anywhere in the string.
TEST_CASE("string/contains") {
  CHECK(sampleString.contains(StringView{"aye"}));
  CHECK(sampleString.contains(Wide("ye")));
  CHECK(sampleString.contains("lay"));
  CHECK_FALSE(sampleString.contains("yay"));
  CHECK(sampleString.contains(""));
  CHECK(sampleString.contains('y'));
  CHECK_FALSE(sampleString.contains('z'));

  static_assert(sampleString.contains(StringView{"aye"}), "a view the string holds must be contained");
  static_assert(sampleString.contains(Wide("ye")), "a string over another storage must be contained");
  static_assert(!sampleString.contains("yay"), "characters the string lacks must not be contained");
  static_assert(sampleString.contains('y') && !sampleString.contains('z'),
                "the character overload must match the same characters");
}

// A comparison covers size() characters, so a null byte inside the string neither ends it nor hides what follows.
TEST_CASE("string/comparison_reads_past_embedded_null") {
  CHECK(embeddedNullString.compare(Fixed(embeddedNull, embeddedNullLength)) == 0);
  CHECK(embeddedNullString.compare("ab") > 0);
  CHECK(embeddedNullString.compare(3, 2, "cd") == 0);
  CHECK(embeddedNullString.ends_with("cd"));
  CHECK(embeddedNullString.ends_with('d'));
  CHECK(embeddedNullString.contains('c'));

  static_assert(embeddedNullString.compare(Fixed(embeddedNull, embeddedNullLength)) == 0,
                "the null byte must compare as a character");
  static_assert(embeddedNullString.compare("ab") > 0, "the characters past the null byte must count");
  static_assert(embeddedNullString.ends_with("cd"), "the end must lie past the null byte");
}

// substr() copies up to count characters from an offset into a new string, from a string kept or one given up.
TEST_CASE("string/substr") {
  CHECK(firstDifference(sampleString.substr(substringOffset, substringLength), sampleText + substringOffset,
                        substringLength)
        == substringLength + 1);
  CHECK(firstDifference(sampleString.substr(substringOffset), sampleText + substringOffset, tailLength)
        == tailLength + 1);
  CHECK(firstDifference(sampleString.substr(), sampleText, sampleLength) == sampleLength + 1);
  CHECK(sampleString.substr(sampleLength).empty());

  // A string given up hands its characters over instead of being copied from.
  CHECK(firstDifference(Fixed(sampleText).substr(substringOffset, substringLength), sampleText + substringOffset,
                        substringLength)
        == substringLength + 1);

  // The null byte travels with the characters around it.
  CHECK(firstDifference(embeddedNullString.substr(embeddedNullRunOffset, embeddedNullRunLength),
                        embeddedNull + embeddedNullRunOffset, embeddedNullRunLength)
        == embeddedNullRunLength + 1);

  static_assert(firstDifference(sampleString.substr(substringOffset, substringLength), sampleText + substringOffset,
                                substringLength)
                  == substringLength + 1,
                "substr() must take count characters from the offset");
  static_assert(firstDifference(sampleString.substr(substringOffset), sampleText + substringOffset, tailLength)
                  == tailLength + 1,
                "the default count must take the string to its end");
  static_assert(sampleString.substr(sampleLength).empty(), "an offset of size() must yield an empty string");
  static_assert(firstDifference(Fixed(sampleText).substr(substringOffset, substringLength),
                                sampleText + substringOffset, substringLength)
                  == substringLength + 1,
                "a string given up must yield the same substring");
  static_assert(firstDifference(embeddedNullString.substr(embeddedNullRunOffset, embeddedNullRunLength),
                                embeddedNull + embeddedNullRunOffset, embeddedNullRunLength)
                  == embeddedNullRunLength + 1,
                "the null byte must stay in the substring");
}

// subview() views the string from an offset to its end, over the string's own characters.
TEST_CASE("string/subview") {
  CHECK(sampleString.subview(substringOffset) == StringView{sampleText + substringOffset});
  CHECK(sampleString.subview(substringOffset).data() == sampleString.data() + substringOffset);
  CHECK(sampleString.subview() == StringView{sampleText});
  CHECK(sampleString.subview(sampleLength).empty());

  static_assert(sampleString.subview(substringOffset) == StringView{sampleText + substringOffset},
                "the view must hold the characters from the offset to the end");
  static_assert(sampleString.subview(substringOffset).data() == sampleString.data() + substringOffset,
                "the view must point into the string instead of a copy");
  static_assert(sampleString.subview().size() == sampleLength, "the default offset must view the whole string");
  static_assert(sampleString.subview(sampleLength).empty(), "an offset of size() must yield an empty view");
}

// The view measures up to the first null character, so a tail holding one is viewed only up to it.
TEST_CASE("string/subview_embedded_null") {
  constexpr size_t headLength = std::char_traits<char>::length(embeddedNull);

  CHECK(embeddedNullString.subview().size() == headLength);
  CHECK(embeddedNullString.subview(headLength + 1) == StringView{embeddedNull + headLength + 1});

  static_assert(embeddedNullString.subview().size() == headLength, "the view must stop at the first null character");
  static_assert(embeddedNullString.subview(headLength + 1) == StringView{embeddedNull + headLength + 1},
                "a view past the null character must hold the characters after it");
}

// The operations never throw; compare() reports an int, the predicates a bool, substr() a string of the same storage.
TEST_CASE("string/operation_signatures") {
  static_assert(noexcept(sampleString.compare(StringView{})) && noexcept(sampleString.compare(0, 0, listedText, 0))
                  && noexcept(sampleString.starts_with('p')) && noexcept(sampleString.ends_with(listedText))
                  && noexcept(sampleString.contains(Wide{})) && noexcept(sampleString.substr())
                  && noexcept(Fixed().substr()) && noexcept(sampleString.subview()),
                "the operations must not throw");

  static_assert(std::is_same_v<decltype(sampleString.compare(listedText)), int>
                  && std::is_same_v<decltype(sampleString.starts_with(listedText)), bool>
                  && std::is_same_v<decltype(sampleString.contains('a')), bool>
                  && std::is_same_v<decltype(sampleString.substr()), Fixed>
                  && std::is_same_v<decltype(Fixed().substr()), Fixed>
                  && std::is_same_v<decltype(sampleString.subview()), StringView>,
                "compare() must return an int, the predicates a bool, substr() a string, and subview() a view");
}

// operator+ joins a string with another string, a view, a pointer, or a character, on either side.
TEST_CASE("string/concatenation") {
  CHECK(holds(sampleString + Fixed(listedText), "playerabc"));
  CHECK(holds(sampleString + Wide(listedText), "playerabc"));
  CHECK(holds(sampleString + StringView{listedText}, "playerabc"));
  CHECK(holds(sampleString + listedText, "playerabc"));
  CHECK(holds(sampleString + fillCharacter, "playerx"));
  CHECK(holds(listedText + sampleString, "abcplayer"));
  CHECK(holds(fillCharacter + sampleString, "xplayer"));
  CHECK(holds(StringView{listedText} + sampleString, "abcplayer"));

  // A string given up takes the right operand in place, so a chain builds one string from left to right.
  CHECK(holds(Fixed(listedText) + sampleString, "abcplayer"));
  CHECK(holds(Fixed(listedText) + fillCharacter + sampleText, "abcxplayer"));

  // The null byte travels with the characters around it.
  CHECK(firstDifference(embeddedNullString + listedText, "ab\0cdabc", embeddedNullLength + listedLength)
        == embeddedNullLength + listedLength + 1);

  static_assert(holds(sampleString + Wide(listedText), "playerabc"), "a string over another storage must go after");
  static_assert(holds(sampleString + listedText, "playerabc"), "a pointer must go after");
  static_assert(holds(sampleString + fillCharacter, "playerx"), "a character must go after");
  static_assert(holds(listedText + sampleString, "abcplayer"), "a pointer must go before");
  static_assert(holds(fillCharacter + sampleString, "xplayer"), "a character must go before");
  static_assert(holds(StringView{listedText} + sampleString, "abcplayer"), "a view must go before");
  static_assert(holds(Fixed(listedText) + fillCharacter + sampleText, "abcxplayer"), "a chain must join left to right");
  static_assert(firstDifference(embeddedNullString + listedText, "ab\0cdabc", embeddedNullLength + listedLength)
                  == embeddedNullLength + listedLength + 1,
                "the null byte must stay in the joined string");

  // The string operand picks the storage of the result, the left one when both are strings.
  static_assert(std::is_same_v<decltype(sampleString + Wide{}), Fixed>
                  && std::is_same_v<decltype(Wide{} + sampleString), Wide>
                  && std::is_same_v<decltype(listedText + sampleString), Fixed>
                  && std::is_same_v<decltype(StringView{} + sampleString), Fixed>,
                "the result must take the storage of its string operand");
}

// The left operand may read the right string itself, since the result starts as a copy of the right one.
TEST_CASE("string/concatenation_from_right_operand") {
  CHECK(holds(sampleString.c_str() + sampleString, "playerplayer"));
  CHECK(holds(sampleString.subview(substringOffset) + sampleString, "ayerplayer"));

  static_assert(holds(sampleString.c_str() + sampleString, "playerplayer"),
                "a pointer into the right string must go before it");
  static_assert(holds(sampleString.subview(substringOffset) + sampleString, "ayerplayer"),
                "a view into the right string must go before it");
}

// A string over a storage that cannot be copied still takes a pointer, a character, or a view on its left.
TEST_CASE("string/concatenation_over_move_only_storage") {
  constexpr auto joinedPointer = [] {
    const MoveOnly right(sampleText);

    return holds(listedText + right, "abcplayer");
  };
  constexpr auto joinedCharacter = [] {
    const MoveOnly right(sampleText);

    return holds(fillCharacter + right, "xplayer");
  };
  constexpr auto joinedView = [] {
    const MoveOnly right(sampleText);

    return holds(StringView{listedText} + right, "abcplayer");
  };

  CHECK(joinedPointer());
  CHECK(joinedCharacter());
  CHECK(joinedView());

  static_assert(joinedPointer(), "a pointer must go before a string that cannot be copied");
  static_assert(joinedCharacter(), "a character must go before a string that cannot be copied");
  static_assert(joinedView(), "a view must go before a string that cannot be copied");
}

// A join that fills the storage to its capacity fits with the pointer, the character, or the view on the left.
TEST_CASE("string/concatenation_to_capacity") {
  constexpr Fixed afterListed(longText, capacity - listedLength);
  constexpr Fixed afterCharacter(longText, capacity - 1);

  CHECK(holds(listedText + afterListed, "abcabcdefghijkl"));
  CHECK(holds(StringView{listedText} + afterListed, "abcabcdefghijkl"));
  CHECK(holds(fillCharacter + afterCharacter, "xabcdefghijklmn"));

  static_assert(holds(listedText + afterListed, "abcabcdefghijkl"), "a pointer must fill the storage exactly");
  static_assert(holds(StringView{listedText} + afterListed, "abcabcdefghijkl"), "a view must fill the storage exactly");
  static_assert(holds(fillCharacter + afterCharacter, "xabcdefghijklmn"), "a character must fill the storage exactly");
}

// With assertions compiled out, the cases below reach the documented fallbacks.
#if !defined(TOYGINE_ASSERTIONS_ENABLED)
// A join the storage rejects leaves the string operand alone, on either side.
TEST_CASE("string/concatenation_past_capacity") {
  constexpr Fixed full(longText, capacity);
  constexpr Fixed nearlyFull(longText, capacity - 1);
  constexpr Fixed listed(listedText);

  // Each side fits on its own, and only the joined length overflows.
  CHECK((full + listedText) == full);
  CHECK((nearlyFull.c_str() + listed) == listed);
  CHECK((nearlyFull.subview() + listed) == listed);
  CHECK((fillCharacter + full) == full);

  // The left side alone overflows the storage.
  CHECK((longText + listed) == listed);
  CHECK((StringView{longText} + listed) == listed);

  static_assert((full + listedText) == full, "a rejected join must keep the left string");
  static_assert((nearlyFull.c_str() + listed) == listed, "a rejected join must keep the right string");
  static_assert((nearlyFull.subview() + listed) == listed, "a rejected join must keep the right string");
  static_assert((fillCharacter + full) == full, "a rejected join must keep the right string");
  static_assert((longText + listed) == listed, "a pointer that overflows alone must leave the right string");
  static_assert((StringView{longText} + listed) == listed, "a view that overflows alone must leave the right string");
}
#endif // !TOYGINE_ASSERTIONS_ENABLED

// Whether a string holds the same characters as another string, a view, or a pointer, on either side.
TEST_CASE("string/equality") {
  CHECK(sampleString == Fixed(sampleText));
  CHECK(sampleString == Wide(sampleText));
  CHECK(sampleString == StringView{sampleText});
  CHECK(sampleString == sampleText);
  CHECK(sampleText == sampleString);
  CHECK(StringView{sampleText} == sampleString);
  CHECK(Wide(sampleText) == sampleString);

  CHECK(sampleString != "play");
  CHECK(sampleString != Wide("players"));
  CHECK(emptyString == "");
  CHECK(emptyString != sampleString);

  // The lengths take part, so a null byte inside the string keeps it apart from its leading characters.
  CHECK(embeddedNullString == Fixed(embeddedNull, embeddedNullLength));
  CHECK(embeddedNullString != "ab");
  CHECK(embeddedNullString != Fixed("ab"));

  static_assert(sampleString == Wide(sampleText), "strings over different storages must compare equal");
  static_assert(sampleString == sampleText && sampleText == sampleString, "a pointer must compare on either side");
  static_assert(StringView{sampleText} == sampleString, "a view must compare on the left");
  static_assert(sampleString != "play", "a prefix must not compare equal");
  static_assert(embeddedNullString == Fixed(embeddedNull, embeddedNullLength),
                "the null byte must compare as a character");
  static_assert(embeddedNullString != "ab", "the characters past the null byte must count");
}

// Where a string sits in a lexicographic order against another string, a view, or a pointer, on either side.
TEST_CASE("string/ordering") {
  CHECK(sampleString < "plays");
  CHECK(sampleString > "play");
  CHECK(sampleString <= sampleText);
  CHECK(sampleString >= Wide("pla"));
  CHECK("play" < sampleString);
  CHECK(StringView{"q"} > sampleString);
  CHECK((sampleString <=> sampleText) == std::strong_ordering::equal);
  CHECK((sampleString <=> Wide("playground")) == std::strong_ordering::less);

  // Bytes order as unsigned char, so a byte above the ASCII range follows an ASCII one.
  CHECK(Fixed("\xff") > Fixed("a"));

  static_assert(sampleString < "plays" && sampleString > "play", "the first difference and length must decide");
  static_assert("play" < sampleString, "a pointer must order on the left");
  static_assert(StringView{"q"} > sampleString, "a view must order on the left");
  static_assert((sampleString <=> Wide("playground")) == std::strong_ordering::less,
                "strings over different storages must order");
  static_assert(Fixed("\xff") > Fixed("a"), "a high byte must order after an ASCII one");
  static_assert(std::is_same_v<decltype(sampleString <=> sampleText), std::strong_ordering>,
                "the order must be strong, as for std::basic_string");
}

// The free swap() exchanges two strings and is the one found beside std::swap, the form generic code writes.
TEST_CASE("string/non_member_swap") {
  constexpr auto swapped = [] {
    using std::swap;

    Fixed sample(sampleText);
    Fixed listed(listedText);
    swap(sample, listed);

    return holds(sample, listedText) && holds(listed, sampleText);
  };

  CHECK(swapped());

  static_assert(swapped(), "the free swap() must exchange contents");
}

// erase() drops every copy of a character and reports how many it dropped.
TEST_CASE("string/erase_value") {
  constexpr auto erasedCount = [](char value) {
    Fixed string(repeated);

    return erase(string, value);
  };
  constexpr auto erasedLeaves = [](char value, const char * expected) {
    Fixed                         string(repeated);
    [[maybe_unused]] const size_t count = erase(string, value);

    return holds(string, expected);
  };
  constexpr auto erasedNull = [] {
    Fixed string(embeddedNull, embeddedNullLength);

    return erase(string, '\0') == 1 && holds(string, "abcd");
  };

  CHECK(erasedCount('a') == 5);
  CHECK(erasedLeaves('a', "brcdbr"));
  CHECK(erasedCount('z') == 0);
  CHECK(erasedLeaves('z', repeated));
  CHECK(erasedNull());

  static_assert(erasedCount('a') == 5 && erasedLeaves('a', "brcdbr"), "every copy of the character must go");
  static_assert(erasedCount('z') == 0 && erasedLeaves('z', repeated), "an absent character must change nothing");
  static_assert(erasedNull(), "a null byte must be erased like any other character");
}

// erase_if() drops every character the predicate accepts, asking it once per character, and reports the count.
TEST_CASE("string/erase_if") {
  constexpr auto isVowel = [](char ch) {
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
  };
  constexpr auto erasedVowels = [isVowel] {
    Fixed string(repeated);

    return erase_if(string, isVowel) == 5 && holds(string, "brcdbr");
  };
  constexpr auto callsPerCharacter = [] {
    Fixed                         string(repeated);
    size_t                        calls = 0;
    [[maybe_unused]] const size_t count = erase_if(string, [&calls](char) {
      ++calls;

      return false;
    });

    return calls == repeatedLength && count == 0 && holds(string, repeated);
  };

  CHECK(erasedVowels());
  CHECK(callsPerCharacter());

  static_assert(erasedVowels(), "every accepted character must go");
  static_assert(callsPerCharacter(), "the predicate must run once per character and keep a rejected one");
}

// The free functions never throw, report what std::basic_string reports, and accept only a predicate.
TEST_CASE("string/non_member_signatures") {
  static_assert(noexcept(sampleString + listedText) && noexcept(listedText + sampleString)
                  && noexcept(sampleString == listedText) && noexcept(sampleString <=> listedText)
                  && noexcept(swap(std::declval<Fixed &>(), std::declval<Fixed &>()))
                  && noexcept(erase(std::declval<Fixed &>(), fillCharacter))
                  && noexcept(erase_if(std::declval<Fixed &>(),
                                       [](char) {
                                         return true;
                                       })),
                "the free functions must not throw");

  static_assert(std::is_same_v<decltype(erase(std::declval<Fixed &>(), fillCharacter)), Fixed::size_type>
                  && std::is_same_v<decltype(erase_if(std::declval<Fixed &>(),
                                                      [](char) {
                                                        return true;
                                                      })),
                                    Fixed::size_type>,
                "erase() and erase_if() must return the count they dropped");

  static_assert(ErasableIf<bool (*)(char)> && !ErasableIf<int>, "erase_if() must accept a predicate and nothing else");
}

// A string built from a character array deduces fixed storage sized to the array, the terminator included.
TEST_CASE("string/deduction_from_array") {
  constexpr String deduced("player");
  constexpr String copied   = "player";
  constexpr String embedded = "ab\0cd";

  CHECK(holds(deduced, sampleText));
  CHECK(holds(copied, sampleText));
  CHECK(deduced.capacity() == sampleLength);

  // The measuring constructor stops at a null byte inside the array; the storage still fits the whole array.
  CHECK(holds(embedded, "ab"));
  CHECK(embedded.capacity() == embeddedNullLength);

  static_assert(std::is_same_v<decltype(String("player")), FixedString<sizeof("player")>>,
                "an array must deduce fixed storage sized to it");
  static_assert(holds(deduced, sampleText) && holds(copied, sampleText),
                "direct and copy initialization must deduce the same string");
  static_assert(holds(embedded, "ab") && embedded.capacity() == embeddedNullLength,
                "a null byte must end the string but not shrink the storage");

  // Without an array the length is not known at compile time, so nothing picks the storage.
  static_assert(DeducesString<const char (&)[4]> && !DeducesString<const char *> && !DeducesString<StringView>,
                "only a character array must deduce a string");
}

} // namespace toy
