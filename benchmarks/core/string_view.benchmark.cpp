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
  \brief  Benchmarks for search and comparison of \ref toy::StringView; desktop adds std::string_view.
*/

#include <algorithm>
#include <string_view>

#include "core.hpp"
#include "toy_benchmark.hpp"

namespace toy {

namespace {

// Text every measurement reads. Long enough that a scan costs something, short enough to stay in the first cache level.
constexpr const char *
  haystack = "the quick brown fox jumps over the lazy dog while the quick brown cat sleeps under the warm summer sun";

// Word sitting near the end of the text, so a search pays close to its worst case before it answers.
constexpr const char * needleText = "summer";

// Character the text does not hold, so a search for it scans every byte before it answers.
constexpr char missing = '!';

// Characters the text does not hold, so a set search scans every byte and tests the whole set against each one.
constexpr const char * absentSet = "!?#%&";

// The same, four times as many characters, so a second measurement shows how the cost follows the size of the set.
constexpr const char * absentWideSet = "!?#%&@$^*()[]{}<>|~+";

// Prefix the text carries, long enough that an answer takes more than the first character.
constexpr const char * prefixText = "the quick brown fox";

// Text in three lead widths: ASCII, two-byte Cyrillic, three-byte Japanese, so a character count meets each of them.
constexpr const char * utf8Text
  = "the quick brown fox, съешь же ещё этих мягких французских булок, すばやい茶色の狐 jumps over the lazy dog";

// Long text length: forty times the short one, inside the first cache level, so a case measures search not memory.
constexpr size_t largeSize = 4096;

// Long text every large case reads: the prefix at the front, the needle at the back, an alphabet cycle between them.
// Neither the prefix nor the needle occurs inside the cycle, so every search crosses the whole text.
// The cycle repeats each letter every 26 bytes, which is how often a substring search meets a candidate first byte.
[[nodiscard]] const char * largeHaystack() noexcept {
  static const array<char, largeSize + 1> text = [] {
    array<char, largeSize + 1> buffer{};

    const size_t prefixLength = std::char_traits<char>::length(prefixText);
    const size_t needleLength = std::char_traits<char>::length(needleText);

    std::char_traits<char>::copy(buffer.data(), prefixText, prefixLength);
    for (size_t index = prefixLength; index < largeSize - needleLength; ++index)
      buffer[index] = static_cast<char>('a' + (index % 26));
    std::char_traits<char>::copy(buffer.data() + largeSize - needleLength, needleText, needleLength);

    return buffer;
  }();

  return text.data();
}

// Hands a pointer through an opaque barrier, so a measured call reads the text instead of a folded constant.
[[nodiscard]] const char * opaque(const char * string) noexcept {
  benchmark::doNotOptimizeAway(string);

  return string;
}

} // namespace

BENCHMARK_CASE("core/string_view/find") {
  const StringView view(opaque(haystack));
  const StringView needle(opaque(needleText));

  bench.unit("search");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.find(needle));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(haystack));
  const std::string_view needleReference(opaque(needleText));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.find(needleReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/find_large") {
  const StringView view(opaque(largeHaystack()));
  const StringView needle(opaque(needleText));

  bench.unit("search");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.find(needle));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(largeHaystack()));
  const std::string_view needleReference(opaque(needleText));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.find(needleReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/find_char_large") {
  const StringView view(opaque(largeHaystack()));

  bench.unit("search");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.find(missing));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(largeHaystack()));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.find(missing));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/find_first_of_large") {
  const StringView view(opaque(largeHaystack()));
  const StringView set(opaque(absentSet));

  bench.unit("search");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.find_first_of(set));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(largeHaystack()));
  const std::string_view setReference(opaque(absentSet));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.find_first_of(setReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/rfind_late_large") {
  const StringView view(opaque(largeHaystack()));
  const StringView needle(opaque(needleText));

  bench.unit("search");

  // The needle sits at the very end, the shape a backward scan answers at once and a forward scan pays in full.
  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.rfind(needle));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(largeHaystack()));
  const std::string_view needleReference(opaque(needleText));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.rfind(needleReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/rfind_large") {
  const StringView view(opaque(largeHaystack()));
  const StringView prefix(opaque(prefixText));

  bench.unit("search");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.rfind(prefix));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(largeHaystack()));
  const std::string_view prefixReference(opaque(prefixText));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.rfind(prefixReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/compare") {
  array<char, std::char_traits<char>::length(haystack) + 1> haystackCopy;
  std::memcpy(haystackCopy.data(), haystack, std::char_traits<char>::length(haystack) + 1);

  // The right-hand operands read a separate buffer: over one buffer the comparison can answer from pointer identity.
  StringView       view(opaque(haystack));
  const StringView same(opaque(haystackCopy.data()));

  bench.unit("comparison");

  // Both operands hold the same text, so the comparison runs to the end instead of stopping at the first difference.
  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.compare(same));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(haystack));
  const std::string_view sameReference(opaque(haystackCopy.data()));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.compare(sameReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/find_char") {
  const StringView view(opaque(haystack));

  bench.unit("search");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.find(missing));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(haystack));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.find(missing));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/find_first_of") {
  const StringView view(opaque(haystack));
  const StringView set(opaque(absentSet));

  bench.unit("search");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.find_first_of(set));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(haystack));
  const std::string_view setReference(opaque(absentSet));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.find_first_of(setReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/find_first_of_wide_set") {
  const StringView view(opaque(haystack));
  const StringView set(opaque(absentWideSet));

  bench.unit("search");

  // Against the case above: the set table is built once per call, so a wider set costs more setup, the same per byte.
  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.find_first_of(set));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(haystack));
  const std::string_view setReference(opaque(absentWideSet));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.find_first_of(setReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/rfind") {
  const StringView view(opaque(haystack));
  const StringView prefix(opaque(prefixText));

  bench.unit("search");

  // The only match sits at the front, so a backward search walks the whole text before it answers.
  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.rfind(prefix));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(haystack));
  const std::string_view prefixReference(opaque(prefixText));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.rfind(prefixReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/rfind_char") {
  const StringView view(opaque(haystack));

  bench.unit("search");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.rfind(missing));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(haystack));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.rfind(missing));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/starts_with") {
  const StringView view(opaque(haystack));
  const StringView prefix(opaque(prefixText));

  bench.unit("test");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.starts_with(prefix));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  const std::string_view reference(opaque(haystack));
  const std::string_view prefixReference(opaque(prefixText));

  bench.run("std::string_view", [&] {
    benchmark::doNotOptimizeAway(reference.starts_with(prefixReference));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_view/utf8_size") {
  const StringView view(opaque(utf8Text));

  bench.unit("count");

  bench.run("StringView", [&] {
    benchmark::doNotOptimizeAway(view.utf8_size());
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  // The standard algorithm alone: every byte outside the 10xxxxxx continuation form starts a character.
  bench.run("std::count_if", [&] {
    benchmark::doNotOptimizeAway(std::count_if(view.begin(), view.end(), [](char byte) {
      return (static_cast<unsigned char>(byte) & 0xC0) != 0x80;
    }));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

} // namespace toy
