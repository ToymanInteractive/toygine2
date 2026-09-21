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
  \brief  Construction of \ref toy::String from iterators, ranges, views, and substrings measured against std::string.
*/

#include <forward_list>
#include <string>
#include <string_view>

#include "core.hpp"
#include "toy_benchmark.hpp"

namespace {

// Characters every case copies: past every inline buffer of std::string, so its rows allocate, and inside the first
// cache level, so a case measures the construction rather than memory.
constexpr size_t c_length = 1024;

// Offset the substring cases start at: half the source, so the kept part is as long as the dropped one.
constexpr size_t c_substringOffset = c_length / 2;

// String sized to hold every case exactly, so the storage zeroes no more buffer than the source needs.
using LongString = toy::FixedString<c_length + 1>;

// Hands a value through an opaque barrier, so a measured construction reads it instead of a folded constant.
template <typename T>
[[nodiscard]] T opaque(T value) noexcept {
  ankerl::nanobench::doNotOptimizeAway(value);

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
    ankerl::nanobench::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(first, last);
    ankerl::nanobench::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_from_forward_list") {
  // Built once outside the loop: the list allocates a node per character, the rows only walk it.
  const std::forward_list<char> list(sourceText(), sourceText() + c_length);

  bench.unit("construction");

  // A forward range without subtraction is walked twice by both rows: once to measure, once to copy.
  bench.run("toy::FixedString", [&] {
    const LongString string(list.begin(), list.end());
    ankerl::nanobench::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(list.begin(), list.end());
    ankerl::nanobench::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_from_single_pass") {
  const SinglePassIterator first(opaque(sourceText()));
  const SinglePassIterator last(opaque(sourceText()) + c_length);

  bench.unit("construction");

  // The toy row checks the storage per character; the std::string row appends and regrows its heap buffer.
  bench.run("toy::FixedString", [&] {
    const LongString string(first, last);
    ankerl::nanobench::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(first, last);
    ankerl::nanobench::doNotOptimizeAway(string);
  });
}

#if defined(__cpp_lib_ranges_to_container) && defined(__cpp_lib_containers_ranges)
BENCHMARK_CASE("core/string/construction_from_range") {
  const std::string_view range(opaque(sourceText()), opaque(c_length));

  bench.unit("construction");

  // A contiguous range with a size is measured by one subtraction and copied as one block.
  bench.run("toy::FixedString", [&] {
    const LongString string(std::from_range, range);
    ankerl::nanobench::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(std::from_range, range);
    ankerl::nanobench::doNotOptimizeAway(string);
  });
}

BENCHMARK_CASE("core/string/construction_from_range_null_terminated") {
  const std::ranges::subrange<const char *, NullTerminator> range(opaque(sourceText()), NullTerminator{});

  bench.unit("construction");

  // The sentinel differs from the iterator, so the length comes from a scan for the terminator before the copy.
  bench.run("toy::FixedString", [&] {
    const LongString string(std::from_range, range);
    ankerl::nanobench::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(std::from_range, range);
    ankerl::nanobench::doNotOptimizeAway(string);
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
    ankerl::nanobench::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(reference);
    ankerl::nanobench::doNotOptimizeAway(string);
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
    ankerl::nanobench::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    const std::string string(reference, offset);
    ankerl::nanobench::doNotOptimizeAway(string);
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
    ankerl::nanobench::doNotOptimizeAway(string);
  });
  bench.run("std::string", [&] {
    std::string       source(referencePrototype);
    const std::string string(std::move(source), offset);
    ankerl::nanobench::doNotOptimizeAway(string);
  });
}
