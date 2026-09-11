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
  \file   string_view.benchmark.cpp
  \brief  Search and comparison of \ref toy::StringView measured against std::string_view.
*/

#include <array>
#include <string>
#include <string_view>

#include "core.hpp"
#include "toy_benchmark.hpp"

namespace {

// Text every measurement reads. Long enough that a scan costs something, short enough to stay in the first cache level.
constexpr const char *
  c_haystack = "the quick brown fox jumps over the lazy dog while the quick brown cat sleeps under the warm summer sun";

// Word sitting near the end of the text, so a search pays close to its worst case before it answers.
constexpr const char * c_needle = "summer";

// Character the text does not hold, so a search for it scans every byte before it answers.
constexpr char c_missing = '!';

// Characters the text does not hold, so a set search scans every byte and tests the whole set against each one.
constexpr const char * c_absentSet = "!?#%&";

// The same, four times as many characters, so a second measurement shows how the cost follows the size of the set.
constexpr const char * c_absentWideSet = "!?#%&@$^*()[]{}<>|~+";

// Prefix the text carries, long enough that an answer takes more than the first character.
constexpr const char * c_prefix = "the quick brown fox";

// Long text length: forty times the short one, inside the first cache level, so a case measures search not memory.
constexpr size_t c_largeSize = 4096;

// Long text every large case reads: the prefix at the front, the needle at the back, an alphabet cycle between them.
// Neither the prefix nor the needle occurs inside the cycle, so every search crosses the whole text.
// The cycle repeats each letter every 26 bytes, which is how often a substring search meets a candidate first byte.
[[nodiscard]] const char * largeHaystack() noexcept {
  static const std::array<char, c_largeSize + 1> text = [] {
    std::array<char, c_largeSize + 1> buffer{};

    const size_t prefixLength = std::char_traits<char>::length(c_prefix);
    const size_t needleLength = std::char_traits<char>::length(c_needle);

    std::char_traits<char>::copy(buffer.data(), c_prefix, prefixLength);
    for (size_t index = prefixLength; index < c_largeSize - needleLength; ++index)
      buffer[index] = static_cast<char>('a' + (index % 26));
    std::char_traits<char>::copy(buffer.data() + c_largeSize - needleLength, c_needle, needleLength);

    return buffer;
  }();

  return text.data();
}

// Hands a pointer through an opaque barrier, so a measured call reads the text instead of a folded constant.
[[nodiscard]] const char * opaque(const char * string) noexcept {
  ankerl::nanobench::doNotOptimizeAway(string);

  return string;
}

} // namespace

BENCHMARK_CASE("core/string_view/find") {
  const toy::StringView  view(opaque(c_haystack));
  const std::string_view reference(opaque(c_haystack));
  const toy::StringView  needle(opaque(c_needle));
  const std::string_view needleReference(opaque(c_needle));

  bench.unit("search");

  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.find(needle));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.find(needleReference));
  });
}

BENCHMARK_CASE("core/string_view/find_large") {
  const toy::StringView  view(opaque(largeHaystack()));
  const std::string_view reference(opaque(largeHaystack()));
  const toy::StringView  needle(opaque(c_needle));
  const std::string_view needleReference(opaque(c_needle));

  bench.unit("search");

  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.find(needle));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.find(needleReference));
  });
}

BENCHMARK_CASE("core/string_view/find_char_large") {
  const toy::StringView  view(opaque(largeHaystack()));
  const std::string_view reference(opaque(largeHaystack()));

  bench.unit("search");

  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.find(c_missing));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.find(c_missing));
  });
}

BENCHMARK_CASE("core/string_view/find_first_of_large") {
  const toy::StringView  view(opaque(largeHaystack()));
  const std::string_view reference(opaque(largeHaystack()));
  const toy::StringView  set(opaque(c_absentSet));
  const std::string_view setReference(opaque(c_absentSet));

  bench.unit("search");

  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.find_first_of(set));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.find_first_of(setReference));
  });
}

BENCHMARK_CASE("core/string_view/rfind_late_large") {
  const toy::StringView  view(opaque(largeHaystack()));
  const std::string_view reference(opaque(largeHaystack()));
  const toy::StringView  needle(opaque(c_needle));
  const std::string_view needleReference(opaque(c_needle));

  bench.unit("search");

  // The needle sits at the very end, the shape a backward scan answers at once and a forward scan pays in full.
  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.rfind(needle));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.rfind(needleReference));
  });
}

BENCHMARK_CASE("core/string_view/rfind_large") {
  const toy::StringView  view(opaque(largeHaystack()));
  const std::string_view reference(opaque(largeHaystack()));
  const toy::StringView  prefix(opaque(c_prefix));
  const std::string_view prefixReference(opaque(c_prefix));

  bench.unit("search");

  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.rfind(prefix));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.rfind(prefixReference));
  });
}

BENCHMARK_CASE("core/string_view/compare") {
  std::array<char, std::char_traits<char>::length(c_haystack) + 1> haystackCopy;
  std::memcpy(haystackCopy.data(), c_haystack, std::char_traits<char>::length(c_haystack) + 1);

  // The right-hand operands read a separate buffer: over one buffer the comparison can answer from pointer identity.
  toy::StringView        view(opaque(c_haystack));
  const toy::StringView  same(opaque(haystackCopy.data()));
  const std::string_view reference(opaque(c_haystack));
  const std::string_view sameReference(opaque(haystackCopy.data()));

  bench.unit("comparison");

  // Both operands hold the same text, so the comparison runs to the end instead of stopping at the first difference.
  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.compare(same));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.compare(sameReference));
  });
}

BENCHMARK_CASE("core/string_view/find_char") {
  const toy::StringView  view(opaque(c_haystack));
  const std::string_view reference(opaque(c_haystack));

  bench.unit("search");

  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.find(c_missing));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.find(c_missing));
  });
}

BENCHMARK_CASE("core/string_view/find_first_of") {
  const toy::StringView  view(opaque(c_haystack));
  const std::string_view reference(opaque(c_haystack));
  const toy::StringView  set(opaque(c_absentSet));
  const std::string_view setReference(opaque(c_absentSet));

  bench.unit("search");

  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.find_first_of(set));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.find_first_of(setReference));
  });
}

BENCHMARK_CASE("core/string_view/find_first_of_wide_set") {
  const toy::StringView  view(opaque(c_haystack));
  const std::string_view reference(opaque(c_haystack));
  const toy::StringView  set(opaque(c_absentWideSet));
  const std::string_view setReference(opaque(c_absentWideSet));

  bench.unit("search");

  // Against the table above: a cost that follows the size of the set is what a lookup table would flatten.
  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.find_first_of(set));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.find_first_of(setReference));
  });
}

BENCHMARK_CASE("core/string_view/rfind") {
  const toy::StringView  view(opaque(c_haystack));
  const std::string_view reference(opaque(c_haystack));
  const toy::StringView  prefix(opaque(c_prefix));
  const std::string_view prefixReference(opaque(c_prefix));

  bench.unit("search");

  // The only match sits at the front, so a backward search walks the whole text before it answers.
  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.rfind(prefix));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.rfind(prefixReference));
  });
}

BENCHMARK_CASE("core/string_view/rfind_char") {
  const toy::StringView  view(opaque(c_haystack));
  const std::string_view reference(opaque(c_haystack));

  bench.unit("search");

  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.rfind(c_missing));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.rfind(c_missing));
  });
}

BENCHMARK_CASE("core/string_view/starts_with") {
  const toy::StringView  view(opaque(c_haystack));
  const std::string_view reference(opaque(c_haystack));
  const toy::StringView  prefix(opaque(c_prefix));
  const std::string_view prefixReference(opaque(c_prefix));

  bench.unit("test");

  bench.run("toy::StringView", [&] {
    ankerl::nanobench::doNotOptimizeAway(view.starts_with(prefix));
  });
  bench.run("std::string_view", [&] {
    ankerl::nanobench::doNotOptimizeAway(reference.starts_with(prefixReference));
  });
}
