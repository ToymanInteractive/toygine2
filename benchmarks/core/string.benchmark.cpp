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
  \file   string.benchmark.cpp
  \brief  Construction, assignment, modifiers, search, and comparison of \ref toy::String measured against std::string.
*/

#include <forward_list>
#include <string>
#include <string_view>

#include "core.hpp"
#include "toy_benchmark.hpp"

namespace {

// Characters every case copies: past the inline buffer of std::string, so its rows allocate, and small enough for L1.
constexpr size_t c_length = 1024;

// Offset the substring cases start at: half the source, so the kept part is as long as the dropped one.
constexpr size_t c_substringOffset = c_length / 2;

// Characters in the short copy cases: an identifier held in a buffer sized for c_length.
constexpr size_t c_shortLength = 8;

// String sized to hold every case exactly, large enough on every target to copy only the characters in use.
using LongString = toy::FixedString<c_length + 1>;

// Hands a value through an opaque barrier, so a measured construction reads it instead of a folded constant.
template <typename T>
[[nodiscard]] T opaque(T value) noexcept {
  toy::benchmark::doNotOptimizeAway(value);

  return value;
}

// Source characters: an alphabet cycle of c_length bytes followed by a terminator.
[[nodiscard]] const char * sourceText() noexcept {
  static const std::array<char, c_length + 1> text = [] {
    std::array<char, c_length + 1> buffer{};
    for (size_t index = 0; index < c_length; ++index)
      buffer[index] = static_cast<char>('a' + (index % 26));

    return buffer;
  }();

  return text.data();
}

// Iterator over a character array that is single-pass by its category, so neither row can measure the range first.
class SinglePassIterator {
public:
  using iterator_category = std::input_iterator_tag;
  using value_type        = char;
  using difference_type   = std::ptrdiff_t;
  using pointer           = const char *;
  using reference         = char;

  SinglePassIterator() noexcept = default;

  explicit SinglePassIterator(const char * position) noexcept
    : _position(position) {}

  [[nodiscard]] char operator*() const noexcept {
    return *_position;
  }

  SinglePassIterator & operator++() noexcept {
    ++_position;
    return *this;
  }

  SinglePassIterator operator++(int) noexcept {
    const SinglePassIterator previous = *this;
    ++_position;
    return previous;
  }

  [[nodiscard]] bool operator==(const SinglePassIterator & other) const noexcept = default;

private:
  const char * _position{nullptr};
};

// Marker at the front of the search text and needle at its back; the alphabet cycle between holds neither.
constexpr const char * c_marker = "0123";
constexpr const char * c_needle = "summer";

// Characters the search text does not hold, so a set search tests the whole set against every byte.
constexpr const char * c_absentSet = "!?#%&";

// Every character the search text holds, so a search for one outside the set crosses the whole text.
constexpr const char * c_textSet = "0123456789abcdefghijklmnopqrstuvwxyz";

// Text the search and comparison cases read: c_length bytes, the marker first, the needle last, the cycle between.
// A forward search for the needle and a backward one for the marker each cross the whole text before they answer.
[[nodiscard]] const char * searchText() noexcept {
  static const std::array<char, c_length + 1> text = [] {
    std::array<char, c_length + 1> buffer{};

    const size_t markerLength = std::char_traits<char>::length(c_marker);
    const size_t needleLength = std::char_traits<char>::length(c_needle);

    std::char_traits<char>::copy(buffer.data(), c_marker, markerLength);
    for (size_t index = markerLength; index < c_length - needleLength; ++index)
      buffer[index] = static_cast<char>('a' + (index % 26));
    std::char_traits<char>::copy(buffer.data() + c_length - needleLength, c_needle, needleLength);

    return buffer;
  }();

  return text.data();
}

// End of a null-terminated string, found by reading rather than by subtraction, so a range ending in it has no size.
struct NullTerminator {
  [[nodiscard]] friend bool operator==(const char * position, NullTerminator) noexcept {
    return *position == '\0';
  }
};

} // namespace

BENCHMARK_CASE("core/string/construction_from_iterators") {
  const char * const first = opaque(sourceText());
  const char * const last  = first + opaque(c_length);

  bench.unit("construction");

  // A pointer pair is measured by one subtraction and copied as one block.
  bench.run("toy::FixedString", [&] {
    const LongString string(first, last);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(first, last);
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_from_forward_list") {
  // Built once outside the loop: the list allocates a node per character, the rows only walk it.
  const std::forward_list<char> list(sourceText(), sourceText() + c_length);

  bench.unit("construction");

  // A forward range without subtraction is walked twice by both rows: once to measure, once to copy.
  bench.run("toy::FixedString", [&] {
    const LongString string(list.begin(), list.end());
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(list.begin(), list.end());
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_from_single_pass") {
  const SinglePassIterator first(opaque(sourceText()));
  const SinglePassIterator last(opaque(sourceText()) + c_length);

  bench.unit("construction");

  // The toy row checks the storage per character; the std::string row appends and regrows its heap buffer.
  bench.run("toy::FixedString", [&] {
    const LongString string(first, last);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(first, last);
    toy::benchmark::doNotOptimizeAway(string);
  });
}

#if defined(__cpp_lib_ranges_to_container) && defined(__cpp_lib_containers_ranges)
BENCHMARK_CASE("core/string/construction_from_range") {
  const std::string_view range(opaque(sourceText()), opaque(c_length));

  bench.unit("construction");

  // A contiguous range with a size is measured by one subtraction and copied as one block.
  bench.run("toy::FixedString", [&] {
    const LongString string(std::from_range, range);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(std::from_range, range);
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_from_range_null_terminated") {
  const std::ranges::subrange<const char *, NullTerminator> range(opaque(sourceText()), NullTerminator{});

  bench.unit("construction");

  // The sentinel differs from the iterator, so the length comes from a scan for the terminator before the copy.
  bench.run("toy::FixedString", [&] {
    const LongString string(std::from_range, range);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(std::from_range, range);
    toy::benchmark::doNotOptimizeAway(string);
  });
}
#endif // __cpp_lib_ranges_to_container && __cpp_lib_containers_ranges

BENCHMARK_CASE("core/string/construction_from_string_like") {
  const toy::StringView  view(opaque(sourceText()));
  const std::string_view reference(opaque(sourceText()), c_length);

  bench.unit("construction");

  // Both sources know their length, so each row copies one block of known size.
  bench.run("toy::FixedString", [&] {
    const LongString string(view);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(reference);
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_substring") {
  const LongString  source(opaque(sourceText()));
  const std::string reference(opaque(sourceText()));
  const size_t      offset = opaque(c_substringOffset);

  bench.unit("construction");

  // The std::basic_string(const basic_string &, pos) shape: the second half of a string copied into a new one.
  bench.run("toy::FixedString", [&] {
    const LongString string(source, offset);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(reference, offset);
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_substring_move") {
  const LongString  prototype(opaque(sourceText()));
  const std::string referencePrototype(opaque(sourceText()));
  const size_t      offset = opaque(c_substringOffset);

  bench.unit("construction");

  // An rvalue source is consumed, so each row first copies a fresh one from its prototype; that copy is part of the
  // measurement in both rows.
  bench.run("toy::FixedString", [&] {
    LongString       source(prototype);
    const LongString string(std::move(source), offset);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    std::string       source(referencePrototype);
    const std::string string(std::move(source), offset);
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_fill") {
  const size_t count = opaque(c_length);

  bench.unit("construction");

  // Both rows write count copies of one character; the std::string row allocates its buffer first.
  bench.run("toy::FixedString", [&] {
    const LongString string(count, 'x');
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(count, 'x');
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_from_pointer") {
  const char * const text = opaque(sourceText());

  bench.unit("construction");

  // A null-terminated source is measured by a scan for the terminator, then copied as one block.
  bench.run("toy::FixedString", [&] {
    const LongString string(text);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(text);
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_copy") {
  const LongString  prototype(opaque(sourceText()));
  const std::string referencePrototype(opaque(sourceText()));

  bench.unit("construction");

  // The toy row copies the characters in use into its own buffer; the std::string row allocates and copies them.
  bench.run("toy::FixedString", [&] {
    const LongString string(prototype);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(referencePrototype);
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_copy_short") {
  const LongString  prototype(opaque(sourceText()), opaque(c_shortLength));
  const std::string referencePrototype(opaque(sourceText()), opaque(c_shortLength));

  bench.unit("construction");

  // A short string in a large buffer: the toy row copies the characters in use, std::string fits them inline.
  bench.run("toy::FixedString", [&] {
    const LongString string(prototype);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(referencePrototype);
    toy::benchmark::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/assignment_fill") {
  const size_t count = opaque(c_length);
  LongString   string;
  std::string  reference;
  reference.reserve(c_length);

  bench.unit("assignment");

  // Both rows overwrite a buffer that already fits count characters, so neither allocates.
  bench.run("toy::FixedString", [&] {
    string.assign(count, 'x');
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    reference.assign(count, 'x');
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/assignment_from_pointer") {
  const char * const text = opaque(sourceText());
  LongString         string;
  std::string        reference;
  reference.reserve(c_length);

  bench.unit("assignment");

  // Both rows scan for the terminator, then copy into a buffer that already fits the source.
  bench.run("toy::FixedString", [&] {
    string = text;
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    reference = text;
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/assignment_from_string_like") {
  const toy::StringView  view(opaque(sourceText()));
  const std::string_view referenceView(opaque(sourceText()), c_length);
  LongString             string;
  std::string            reference;
  reference.reserve(c_length);

  bench.unit("assignment");

  // Both sources know their length, so each row copies one block of known size; the toy row also checks for overlap.
  bench.run("toy::FixedString", [&] {
    string = view;
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    reference = referenceView;
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/assignment_substring") {
  const LongString  source(opaque(sourceText()));
  const std::string referenceSource(opaque(sourceText()));
  const size_t      offset = opaque(c_substringOffset);
  LongString        string;
  std::string       reference;
  reference.reserve(c_length);

  bench.unit("assignment");

  // The assign(const basic_string &, pos) shape: the second half of a string copied over another one.
  bench.run("toy::FixedString", [&] {
    string.assign(source, offset);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    reference.assign(referenceSource, offset);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/assignment_from_iterators") {
  const char * const first = opaque(sourceText());
  const char * const last  = first + opaque(c_length);
  LongString         string;
  std::string        reference;
  reference.reserve(c_length);

  bench.unit("assignment");

  // A pointer pair is measured by one subtraction and copied as one block.
  bench.run("toy::FixedString", [&] {
    string.assign(first, last);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    reference.assign(first, last);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/assignment_from_single_pass") {
  const SinglePassIterator first(opaque(sourceText()));
  const SinglePassIterator last(opaque(sourceText()) + c_length);
  LongString               string;
  std::string              reference;
  reference.reserve(c_length);

  bench.unit("assignment");

  // The toy row checks the storage only when its buffer fills; the std::string row appends into reserved capacity.
  bench.run("toy::FixedString", [&] {
    string.assign(first, last);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    reference.assign(first, last);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

#if defined(__cpp_lib_containers_ranges)
BENCHMARK_CASE("core/string/assignment_from_range") {
  const std::string_view range(opaque(sourceText()), opaque(c_length));
  LongString             string;
  std::string            reference;
  reference.reserve(c_length);

  bench.unit("assignment");

  // A contiguous range with a size is measured by one subtraction and copied as one block.
  bench.run("toy::FixedString", [&] {
    string.assign_range(range);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    reference.assign_range(range);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}
#endif // __cpp_lib_containers_ranges

BENCHMARK_CASE("core/string/copy_assignment") {
  LongString  source(opaque(sourceText()));
  std::string referenceSource(opaque(sourceText()));
  LongString  string;
  std::string reference;
  reference.reserve(c_length);

  bench.unit("assignment");

  // The source passes through the barrier on every run, so neither row can keep it in registers.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(source);
    string = source;
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(referenceSource);
    reference = referenceSource;
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/copy_assignment_short") {
  LongString  source(opaque(sourceText()), opaque(c_shortLength));
  std::string referenceSource(opaque(sourceText()), opaque(c_shortLength));
  LongString  string;
  std::string reference;

  bench.unit("assignment");

  // A short string in a large buffer: the toy row copies the characters in use, std::string fits them inline.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(source);
    string = source;
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(referenceSource);
    reference = referenceSource;
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/move_assignment") {
  const LongString  prototype(opaque(sourceText()));
  const std::string referencePrototype(opaque(sourceText()));
  LongString        string;
  std::string       reference;

  bench.unit("assignment");

  // A moved-from source is spent, so each row copies a fresh one from its prototype, and that copy is measured too.
  bench.run("toy::FixedString", [&] {
    LongString source(prototype);
    string = std::move(source);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    std::string source(referencePrototype);
    reference = std::move(source);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/append") {
  const char * const text  = opaque(sourceText());
  const size_t       count = opaque(c_shortLength);
  LongString         string;
  std::string        reference;
  reference.reserve(c_length);

  bench.unit("append");

  // A short run goes after the last character; a full string is cleared, once every c_length / c_shortLength runs.
  bench.run("toy::FixedString", [&] {
    if (string.size() + count > c_length)
      string.clear();
    string.append(text, count);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    if (reference.size() + count > c_length)
      reference.clear();
    reference.append(text, count);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/push_back") {
  const char  character = opaque('x');
  LongString  string;
  std::string reference;
  reference.reserve(c_length);

  bench.unit("push_back");

  // One character after the last one; a full string is cleared, once every c_length runs.
  bench.run("toy::FixedString", [&] {
    if (string.size() == c_length)
      string.clear();
    string.push_back(character);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    if (reference.size() == c_length)
      reference.clear();
    reference.push_back(character);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/insert") {
  const char * const text  = opaque(sourceText());
  const size_t       count = opaque(c_shortLength);
  LongString         string(text, c_substringOffset);
  std::string        reference(text, c_substringOffset);
  reference.reserve(c_length);

  bench.unit("insert");

  // A short run goes into the middle and moves the tail behind it; a full string drops back to half its length.
  bench.run("toy::FixedString", [&] {
    if (string.size() + count > c_length)
      string.resize(c_substringOffset);
    string.insert(string.size() / 2, text, count);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    if (reference.size() + count > c_length)
      reference.resize(c_substringOffset);
    reference.insert(reference.size() / 2, text, count);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/erase") {
  const char * const text  = opaque(sourceText());
  const size_t       count = opaque(c_shortLength);
  LongString         string(text, c_length);
  std::string        reference(text, c_length);

  bench.unit("erase");

  // A short run leaves the middle and the tail closes up; a string down to half is refilled, an amortized short copy.
  bench.run("toy::FixedString", [&] {
    if (string.size() < c_substringOffset + count)
      string.append(text, c_length - string.size());
    string.erase(string.size() / 2, count);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    if (reference.size() < c_substringOffset + count)
      reference.append(text, c_length - reference.size());
    reference.erase(reference.size() / 2, count);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/replace") {
  const char * const text   = opaque(sourceText());
  const size_t       count  = opaque(c_shortLength);
  const size_t       offset = opaque(c_substringOffset);
  LongString         string(text, c_length);
  std::string        reference(text, c_length);

  bench.unit("replace");

  // A short run in the middle swapped for one of the same length, so the tail stays in place and the length holds.
  bench.run("toy::FixedString", [&] {
    string.replace(offset, count, text, count);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    reference.replace(offset, count, text, count);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/swap") {
  LongString  string(opaque(sourceText()));
  LongString  other(opaque(sourceText()), opaque(c_shortLength));
  std::string reference(opaque(sourceText()));
  std::string otherReference(opaque(sourceText()), opaque(c_shortLength));

  bench.unit("swap");

  // Fixed storage has no buffer to hand over, so the toy row copies characters where std::string exchanges pointers.
  bench.run("toy::FixedString", [&] {
    string.swap(other);
    toy::benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    reference.swap(otherReference);
    toy::benchmark::doNotOptimizeAway(reference);
  });
}

BENCHMARK_CASE("core/string/find") {
  const LongString  string(opaque(searchText()));
  const std::string reference(opaque(searchText()));
  const char *      needle = opaque(c_needle);

  bench.unit("search");

  // The needle sits at the end, and its first letter recurs every 26 bytes, each a candidate the search rejects.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(string.find(needle));
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(reference.find(needle));
  });
}

BENCHMARK_CASE("core/string/rfind") {
  const LongString  string(opaque(searchText()));
  const std::string reference(opaque(searchText()));
  const char *      marker = opaque(c_marker);

  bench.unit("search");

  // The marker sits at the front, so a backward search crosses the whole text before it answers.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(string.rfind(marker));
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(reference.rfind(marker));
  });
}

BENCHMARK_CASE("core/string/find_first_of") {
  const LongString  string(opaque(searchText()));
  const std::string reference(opaque(searchText()));
  const char *      set = opaque(c_absentSet);

  bench.unit("search");

  // No byte of the text is in the set, so every byte is tested against the whole set.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(string.find_first_of(set));
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(reference.find_first_of(set));
  });
}

BENCHMARK_CASE("core/string/find_first_not_of") {
  const LongString  string(opaque(searchText()));
  const std::string reference(opaque(searchText()));
  const char *      set = opaque(c_textSet);

  bench.unit("search");

  // Every byte of the text is in the set, so the search crosses the text and finds every byte inside the set.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(string.find_first_not_of(set));
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(reference.find_first_not_of(set));
  });
}

BENCHMARK_CASE("core/string/compare") {
  // The operands live in separate strings, so the comparison cannot answer from pointer identity.
  const LongString  string(opaque(searchText()));
  const LongString  same(opaque(searchText()));
  const std::string reference(opaque(searchText()));
  const std::string sameReference(opaque(searchText()));

  bench.unit("comparison");

  // Both operands hold the same text, so the comparison runs to the end.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(string.compare(same));
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(reference.compare(sameReference));
  });
}

BENCHMARK_CASE("core/string/equality") {
  const LongString  string(opaque(searchText()));
  const LongString  same(opaque(searchText()));
  const std::string reference(opaque(searchText()));
  const std::string sameReference(opaque(searchText()));

  bench.unit("comparison");

  // Equal lengths pass the first check, so operator== compares every byte.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(string == same);
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(reference == sameReference);
  });
}

BENCHMARK_CASE("core/string/substr") {
  const LongString  string(opaque(sourceText()));
  const std::string reference(opaque(sourceText()));
  const size_t      offset = opaque(c_substringOffset);

  bench.unit("substr");

  // The second half becomes a new string: the toy row copies it into fixed storage, std::string allocates for it.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(string.substr(offset));
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(reference.substr(offset));
  });
}

BENCHMARK_CASE("core/string/concatenation") {
  const LongString  left(opaque(sourceText()), opaque(c_substringOffset));
  const LongString  right(opaque(sourceText()), opaque(c_substringOffset));
  const std::string leftReference(opaque(sourceText()), opaque(c_substringOffset));
  const std::string rightReference(opaque(sourceText()), opaque(c_substringOffset));

  bench.unit("concatenation");

  // Two halves join into a new full string: the left one is copied, the right one appended after it.
  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(left + right);
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(leftReference + rightReference);
  });
}

BENCHMARK_CASE("core/string/erase_if") {
  LongString  string(opaque(sourceText()));
  std::string reference(opaque(sourceText()));

  bench.unit("erase_if");

  // The text holds no digit, so the predicate rejects every byte and the string keeps its length between runs.
  const auto isDigit = [](char character) noexcept {
    return character >= '0' && character <= '9';
  };

  bench.run("toy::FixedString", [&] {
    toy::benchmark::doNotOptimizeAway(erase_if(string, isDigit));
  });
  bench.run("std::string", [&] {
    toy::benchmark::doNotOptimizeAway(std::erase_if(reference, isDigit));
  });
}
