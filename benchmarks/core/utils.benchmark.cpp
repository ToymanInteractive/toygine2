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
  \file   utils.benchmark.cpp
  \brief  UTF-8 and UTF-16 conversion and validation through the core text utilities.
*/

#include "core.hpp"
#include "toy_benchmark.hpp"

namespace {

// Text in all four encoded widths, so a conversion meets each of them, a surrogate pair included.
constexpr const char16_t * c_utf16Text
  = u"the quick brown fox, съешь же ещё этих мягких французских булок, すばやい茶色の狐 🎮🎲 jumps over the lazy dog";

// The same text in UTF-8.
constexpr const char * c_utf8Text
  = "the quick brown fox, съешь же ещё этих мягких французских булок, すばやい茶色の狐 🎮🎲 jumps over the lazy dog";

constexpr size_t c_utf16Length = std::char_traits<char16_t>::length(c_utf16Text);
constexpr size_t c_utf8Length  = std::char_traits<char>::length(c_utf8Text);

// Hands a pointer through an opaque barrier, so a measured call reads the text instead of a folded constant.
template <typename Char>
[[nodiscard]] const Char * opaque(const Char * string) noexcept {
  toy::benchmark::doNotOptimizeAway(string);

  return string;
}

} // namespace

BENCHMARK_CASE("core/utils/utf16_to_utf8") {
  const char16_t * const text = opaque(c_utf16Text);

  // Room for every unit at its widest encoding plus the terminator, so the whole text converts every time.
  std::array<char, c_utf16Length * toy::c_utf16InUtf8MaxSize + 1> buffer{};

  bench.unit("conversion");

  bench.run("toy::utf16ToUtf8", [&] {
    toy::benchmark::doNotOptimizeAway(toy::utf16ToUtf8(buffer.data(), buffer.size(), text, c_utf16Length));
  });
}

BENCHMARK_CASE("core/utils/utf8_to_utf16") {
  const char * const text = opaque(c_utf8Text);

  // One unit per byte at most, plus the terminator, so the whole text converts every time.
  std::array<char16_t, c_utf8Length + 1> buffer{};

  bench.unit("conversion");

  bench.run("toy::utf8ToUtf16", [&] {
    toy::benchmark::doNotOptimizeAway(toy::utf8ToUtf16(buffer.data(), buffer.size(), text, c_utf8Length));
  });
}

BENCHMARK_CASE("core/utils/utf8_len") {
  const char * const text = opaque(c_utf8Text);

  bench.unit("count");

  bench.run("toy::utf8Len", [&] {
    toy::benchmark::doNotOptimizeAway(toy::utf8Len(text));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  // Counts bytes up to the terminator, not characters: the floor of any walk over the same text.
  bench.run("std::char_traits<char>::length", [&] {
    toy::benchmark::doNotOptimizeAway(std::char_traits<char>::length(text));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/utils/utf16_len") {
  const char16_t * const text = opaque(c_utf16Text);

  bench.unit("count");

  bench.run("toy::utf16Len", [&] {
    toy::benchmark::doNotOptimizeAway(toy::utf16Len(text));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  // Counts units up to the terminator, not characters: the floor of any walk over the same text.
  bench.run("std::char_traits<char16_t>::length", [&] {
    toy::benchmark::doNotOptimizeAway(std::char_traits<char16_t>::length(text));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/utils/validate_utf8") {
  // The text is well-formed, so each row walks it to the end through every sequence width.
  const char * const text = opaque(c_utf8Text);

  bench.unit("validation");

  bench.run("toy::validateUtf8(const char *)", [&] {
    toy::benchmark::doNotOptimizeAway(toy::validateUtf8(text));
  });

  bench.run("toy::validateUtf8(const char *, size_t)", [&] {
    toy::benchmark::doNotOptimizeAway(toy::validateUtf8(text, c_utf8Length));
  });
}

BENCHMARK_CASE("core/utils/validate_utf16") {
  // The text is well-formed, so each row walks it to the end, its surrogate pairs included.
  const char16_t * const text = opaque(c_utf16Text);

  bench.unit("validation");

  bench.run("toy::validateUtf16(const char16_t *)", [&] {
    toy::benchmark::doNotOptimizeAway(toy::validateUtf16(text));
  });

  bench.run("toy::validateUtf16(const char16_t *, size_t)", [&] {
    toy::benchmark::doNotOptimizeAway(toy::validateUtf16(text, c_utf16Length));
  });
}
