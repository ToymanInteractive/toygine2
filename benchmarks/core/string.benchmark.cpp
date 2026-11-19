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
  \brief  Benchmarks for construction, assignment, search and comparison of \ref toy::String; desktop adds std::string.
*/

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
#include <forward_list>
#include <string>
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE

#include <string_view>

#include "core.hpp"
#include "toy_benchmark.hpp"

namespace toy {

namespace {

// Characters every case copies: past the inline buffer of std::string, so its rows allocate, and small enough for L1.
constexpr size_t length = 1024;

// Offset the substring cases start at: half the source, so the kept part is as long as the dropped one.
constexpr size_t substringOffset = length / 2;

// Characters in the short copy cases: an identifier held in a buffer sized for length.
constexpr size_t shortLength = 8;

// String sized to hold every case exactly, large enough on every target to copy only the characters in use.
using LongString = FixedString<length + 1>;

// Hands a value through an opaque barrier, so a measured construction reads it instead of a folded constant.
template <typename T>
[[nodiscard]] T opaque(T value) noexcept {
  benchmark::doNotOptimizeAway(value);

  return value;
}

// Source characters: an alphabet cycle of length bytes followed by a terminator.
[[nodiscard]] const char * sourceText() noexcept {
  static const array<char, length + 1> text = [] {
    array<char, length + 1> buffer{};
    for (size_t index = 0; index < length; ++index)
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
constexpr const char * markerText = "0123";
constexpr const char * needleText = "summer";

// Characters the search text does not hold, so a set search tests the whole set against every byte.
constexpr const char * absentSet = "!?#%&";

// Every character the search text holds, so a search for one outside the set crosses the whole text.
constexpr const char * textSet = "0123456789abcdefghijklmnopqrstuvwxyz";

// Text the search and comparison cases read: length bytes, the marker first, the needle last, the cycle between.
// A forward search for the needle and a backward one for the marker each cross the whole text before they answer.
[[nodiscard]] const char * searchText() noexcept {
  static const array<char, length + 1> text = [] {
    array<char, length + 1> buffer{};

    const size_t markerLength = std::char_traits<char>::length(markerText);
    const size_t needleLength = std::char_traits<char>::length(needleText);

    std::char_traits<char>::copy(buffer.data(), markerText, markerLength);
    for (size_t index = markerLength; index < length - needleLength; ++index)
      buffer[index] = static_cast<char>('a' + (index % 26));
    std::char_traits<char>::copy(buffer.data() + length - needleLength, needleText, needleLength);

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
  const char * const last  = first + opaque(length);

  bench.unit("construction");

  // A pointer pair is measured by one subtraction and copied as one block.
  bench.run("FixedString", [&] {
    const LongString string(first, last);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::string", [&] {
    const std::string string(first, last);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
BENCHMARK_CASE("core/string/construction_from_forward_list") {
  // Built once outside the loop: the list allocates a node per character, the rows only walk it.
  const std::forward_list<char> list(sourceText(), sourceText() + length);

  bench.unit("construction");

  // A forward range without subtraction is walked twice by both rows: once to measure, once to copy.
  bench.run("FixedString", [&] {
    const LongString string(list.begin(), list.end());
    benchmark::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(list.begin(), list.end());
    benchmark::doNotOptimizeAway(string);
  });
}
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE

BENCHMARK_CASE("core/string/construction_from_single_pass") {
  const SinglePassIterator first(opaque(sourceText()));
  const SinglePassIterator last(opaque(sourceText()) + length);

  bench.unit("construction");

  // The toy row checks the storage only when its buffer fills; the std::string row appends and regrows its heap buffer.
  bench.run("FixedString", [&] {
    const LongString string(first, last);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::string", [&] {
    const std::string string(first, last);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

#if defined(__cpp_lib_ranges_to_container) && defined(__cpp_lib_containers_ranges)
BENCHMARK_CASE("core/string/construction_from_range") {
  const std::string_view range(opaque(sourceText()), opaque(length));

  bench.unit("construction");

  // A contiguous range with a size is measured by one subtraction and copied as one block.
  bench.run("FixedString", [&] {
    const LongString string(std::from_range, range);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::string", [&] {
    const std::string string(std::from_range, range);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/construction_from_range_null_terminated") {
  const std::ranges::subrange<const char *, NullTerminator> range(opaque(sourceText()), NullTerminator{});

  bench.unit("construction");

  // The sentinel differs from the iterator, so the length comes from a scan for the terminator before the copy.
  bench.run("FixedString", [&] {
    const LongString string(std::from_range, range);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::string", [&] {
    const std::string string(std::from_range, range);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}
#endif // __cpp_lib_ranges_to_container && __cpp_lib_containers_ranges

BENCHMARK_CASE("core/string/construction_from_string_like") {
  const StringView view(opaque(sourceText()));

  bench.unit("construction");

  // Both sources know their length, so each row copies one block of known size.
  bench.run("FixedString", [&] {
    const LongString string(view);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(sourceText()), length);

  bench.run("std::string", [&] {
    const std::string string(reference);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/construction_substring") {
  const LongString source(opaque(sourceText()));
  const size_t     offset = opaque(substringOffset);

  bench.unit("construction");

  // The std::basic_string(const basic_string &, pos) shape: the second half of a string copied into a new one.
  bench.run("FixedString", [&] {
    const LongString string(source, offset);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string reference(opaque(sourceText()));

  bench.run("std::string", [&] {
    const std::string string(reference, offset);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/construction_substring_move") {
  const LongString prototype(opaque(sourceText()));
  const size_t     offset = opaque(substringOffset);

  bench.unit("construction");

  // An rvalue source is consumed, so each row copies a fresh one from its prototype, and that copy is measured too.
  bench.run("FixedString", [&] {
    LongString       source(prototype);
    const LongString string(std::move(source), offset);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string referencePrototype(opaque(sourceText()));

  bench.run("std::string", [&] {
    std::string       source(referencePrototype);
    const std::string string(std::move(source), offset);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/construction_fill") {
  const size_t count = opaque(length);

  bench.unit("construction");

  // Both rows write count copies of one character; the std::string row allocates its buffer first.
  bench.run("FixedString", [&] {
    const LongString string(count, 'x');
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::string", [&] {
    const std::string string(count, 'x');
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/construction_from_pointer") {
  const char * const text = opaque(sourceText());

  bench.unit("construction");

  // A null-terminated source is measured by a scan for the terminator, then copied as one block.
  bench.run("FixedString", [&] {
    const LongString string(text);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::string", [&] {
    const std::string string(text);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/construction_copy") {
  const LongString prototype(opaque(sourceText()));

  bench.unit("construction");

  // The toy row copies the characters in use into its own buffer; the std::string row allocates and copies them.
  bench.run("FixedString", [&] {
    const LongString string(prototype);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string referencePrototype(opaque(sourceText()));

  bench.run("std::string", [&] {
    const std::string string(referencePrototype);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/construction_copy_short") {
  const LongString prototype(opaque(sourceText()), opaque(shortLength));

  bench.unit("construction");

  // A short string in a large buffer: the toy row copies the characters in use, std::string fits them inline.
  bench.run("FixedString", [&] {
    const LongString string(prototype);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string referencePrototype(opaque(sourceText()), opaque(shortLength));

  bench.run("std::string", [&] {
    const std::string string(referencePrototype);
    benchmark::doNotOptimizeAway(string);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/assignment_fill") {
  const size_t count = opaque(length);
  LongString   string;

  bench.unit("assignment");

  // Both rows overwrite a buffer that already fits count characters, so neither allocates.
  bench.run("FixedString", [&] {
    string.assign(count, 'x');
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    reference.assign(count, 'x');
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/assignment_from_pointer") {
  const char * const text = opaque(sourceText());
  LongString         string;

  bench.unit("assignment");

  // Both rows scan for the terminator, then copy into a buffer that already fits the source.
  bench.run("FixedString", [&] {
    string = text;
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    reference = text;
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/assignment_from_string_like") {
  const StringView view(opaque(sourceText()));
  LongString       string;

  bench.unit("assignment");

  // Both sources know their length, so each row copies one block of known size; the toy row also checks for overlap.
  bench.run("FixedString", [&] {
    string = view;
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view referenceView(opaque(sourceText()), length);
  std::string            reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    reference = referenceView;
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/assignment_substring") {
  const LongString source(opaque(sourceText()));
  const size_t     offset = opaque(substringOffset);
  LongString       string;

  bench.unit("assignment");

  // The assign(const basic_string &, pos) shape: the second half of a string copied over another one.
  bench.run("FixedString", [&] {
    string.assign(source, offset);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string referenceSource(opaque(sourceText()));
  std::string       reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    reference.assign(referenceSource, offset);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/assignment_from_iterators") {
  const char * const first = opaque(sourceText());
  const char * const last  = first + opaque(length);
  LongString         string;

  bench.unit("assignment");

  // A pointer pair is measured by one subtraction and copied as one block.
  bench.run("FixedString", [&] {
    string.assign(first, last);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    reference.assign(first, last);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/assignment_from_single_pass") {
  const SinglePassIterator first(opaque(sourceText()));
  const SinglePassIterator last(opaque(sourceText()) + length);
  LongString               string;

  bench.unit("assignment");

  // The toy row checks the storage only when its buffer fills; the std::string row appends into reserved capacity.
  bench.run("FixedString", [&] {
    string.assign(first, last);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    reference.assign(first, last);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

#if defined(__cpp_lib_containers_ranges)
BENCHMARK_CASE("core/string/assignment_from_range") {
  const std::string_view range(opaque(sourceText()), opaque(length));
  LongString             string;

  bench.unit("assignment");

  // A contiguous range with a size is measured by one subtraction and copied as one block.
  bench.run("FixedString", [&] {
    string.assign_range(range);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    reference.assign_range(range);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}
#endif // __cpp_lib_containers_ranges

BENCHMARK_CASE("core/string/copy_assignment") {
  LongString source(opaque(sourceText()));
  LongString string;

  bench.unit("assignment");

  // The source passes through the barrier on every run, so neither row can keep it in registers.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(source);
    string = source;
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string referenceSource(opaque(sourceText()));
  std::string reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(referenceSource);
    reference = referenceSource;
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/copy_assignment_short") {
  LongString source(opaque(sourceText()), opaque(shortLength));
  LongString string;

  bench.unit("assignment");

  // A short string in a large buffer: the toy row copies the characters in use, std::string fits them inline.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(source);
    string = source;
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string referenceSource(opaque(sourceText()), opaque(shortLength));
  std::string reference;

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(referenceSource);
    reference = referenceSource;
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/move_assignment") {
  const LongString prototype(opaque(sourceText()));
  LongString       string;

  bench.unit("assignment");

  // A moved-from source is spent, so each row copies a fresh one from its prototype, and that copy is measured too.
  bench.run("FixedString", [&] {
    LongString source(prototype);
    string = std::move(source);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string referencePrototype(opaque(sourceText()));
  std::string       reference;

  bench.run("std::string", [&] {
    std::string source(referencePrototype);
    reference = std::move(source);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/append") {
  const char * const text  = opaque(sourceText());
  const size_t       count = opaque(shortLength);
  LongString         string;

  bench.unit("append");

  // A short run goes after the last character; a full string is cleared, once every length / shortLength runs.
  bench.run("FixedString", [&] {
    if (string.size() + count > length)
      string.clear();
    string.append(text, count);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    if (reference.size() + count > length)
      reference.clear();
    reference.append(text, count);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/push_back") {
  const char character = opaque('x');
  LongString string;

  bench.unit("push_back");

  // One character after the last one; a full string is cleared, once every length runs.
  bench.run("FixedString", [&] {
    if (string.size() == length)
      string.clear();
    string.push_back(character);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference;
  reference.reserve(length);

  bench.run("std::string", [&] {
    if (reference.size() == length)
      reference.clear();
    reference.push_back(character);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/insert") {
  const char * const text  = opaque(sourceText());
  const size_t       count = opaque(shortLength);
  LongString         string(text, substringOffset);

  bench.unit("insert");

  // A short run goes into the middle and moves the tail behind it; a full string drops back to half its length.
  bench.run("FixedString", [&] {
    if (string.size() + count > length)
      string.resize(substringOffset);
    string.insert(string.size() / 2, text, count);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference(text, substringOffset);
  reference.reserve(length);

  bench.run("std::string", [&] {
    if (reference.size() + count > length)
      reference.resize(substringOffset);
    reference.insert(reference.size() / 2, text, count);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/erase") {
  const char * const text  = opaque(sourceText());
  const size_t       count = opaque(shortLength);
  LongString         string(text, length);

  bench.unit("erase");

  // A short run leaves the middle and the tail closes up; a string down to half is refilled, an amortized short copy.
  bench.run("FixedString", [&] {
    if (string.size() < substringOffset + count)
      string.append(text, length - string.size());
    string.erase(string.size() / 2, count);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference(text, length);

  bench.run("std::string", [&] {
    if (reference.size() < substringOffset + count)
      reference.append(text, length - reference.size());
    reference.erase(reference.size() / 2, count);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/replace") {
  const char * const text   = opaque(sourceText());
  const size_t       count  = opaque(shortLength);
  const size_t       offset = opaque(substringOffset);
  LongString         string(text, length);

  bench.unit("replace");

  // A short run in the middle swapped for one of the same length, so the tail stays in place and the length holds.
  bench.run("FixedString", [&] {
    string.replace(offset, count, text, count);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference(text, length);

  bench.run("std::string", [&] {
    reference.replace(offset, count, text, count);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/swap") {
  LongString string(opaque(sourceText()));
  LongString other(opaque(sourceText()), opaque(shortLength));

  bench.unit("swap");

  // Fixed storage has no buffer to hand over, so the toy row copies characters where std::string exchanges pointers.
  bench.run("FixedString", [&] {
    string.swap(other);
    benchmark::doNotOptimizeAway(string);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference(opaque(sourceText()));
  std::string otherReference(opaque(sourceText()), opaque(shortLength));

  bench.run("std::string", [&] {
    reference.swap(otherReference);
    benchmark::doNotOptimizeAway(reference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/find") {
  const LongString string(opaque(searchText()));
  const char *     needle = opaque(needleText);

  bench.unit("search");

  // The needle sits at the end, and its first letter recurs every 26 bytes, each a candidate the search rejects.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(string.find(needle));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string reference(opaque(searchText()));

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(reference.find(needle));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/rfind") {
  const LongString string(opaque(searchText()));
  const char *     marker = opaque(markerText);

  bench.unit("search");

  // The marker sits at the front, so a backward search crosses the whole text before it answers.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(string.rfind(marker));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string reference(opaque(searchText()));

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(reference.rfind(marker));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/find_first_of") {
  const LongString string(opaque(searchText()));
  const char *     set = opaque(absentSet);

  bench.unit("search");

  // No byte of the text is in the set, so the search reads every byte, each with one lookup in the table of the set.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(string.find_first_of(set));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string reference(opaque(searchText()));

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(reference.find_first_of(set));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/find_first_not_of") {
  const LongString string(opaque(searchText()));
  const char *     set = opaque(textSet);

  bench.unit("search");

  // Every byte of the text is in the set, so the search crosses the text and finds every byte inside the set.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(string.find_first_not_of(set));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string reference(opaque(searchText()));

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(reference.find_first_not_of(set));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/compare") {
  // The operands live in separate strings, so the comparison cannot answer from pointer identity.
  const LongString string(opaque(searchText()));
  const LongString same(opaque(searchText()));

  bench.unit("comparison");

  // Both operands hold the same text, so the comparison runs to the end.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(string.compare(same));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string reference(opaque(searchText()));
  const std::string sameReference(opaque(searchText()));

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(reference.compare(sameReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/equality") {
  const LongString string(opaque(searchText()));
  const LongString same(opaque(searchText()));

  bench.unit("comparison");

  // Equal lengths pass the first check, so operator== compares every byte.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(string == same);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string reference(opaque(searchText()));
  const std::string sameReference(opaque(searchText()));

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(reference == sameReference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/substr") {
  const LongString string(opaque(sourceText()));
  const size_t     offset = opaque(substringOffset);

  bench.unit("substr");

  // The second half becomes a new string: the toy row copies it into fixed storage, std::string allocates for it.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(string.substr(offset));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string reference(opaque(sourceText()));

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(reference.substr(offset));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/concatenation") {
  const LongString left(opaque(sourceText()), opaque(substringOffset));
  const LongString right(opaque(sourceText()), opaque(substringOffset));

  bench.unit("concatenation");

  // Two halves join into a new full string: the left one is copied, the right one appended after it.
  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(left + right);
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string leftReference(opaque(sourceText()), opaque(substringOffset));
  const std::string rightReference(opaque(sourceText()), opaque(substringOffset));

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(leftReference + rightReference);
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string/erase_if") {
  LongString string(opaque(sourceText()));

  bench.unit("erase_if");

  // The text holds no digit, so the predicate rejects every byte and the string keeps its length between runs.
  const auto isDigit = [](char character) noexcept {
    return character >= '0' && character <= '9';
  };

  bench.run("FixedString", [&] {
    benchmark::doNotOptimizeAway(erase_if(string, isDigit));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  std::string reference(opaque(sourceText()));

  bench.run("std::string", [&] {
    benchmark::doNotOptimizeAway(std::erase_if(reference, isDigit));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

} // namespace toy
