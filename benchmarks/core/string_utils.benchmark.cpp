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
  \file   string_utils.benchmark.cpp
  \brief  Text conversion, validation, and integer formatting through the core text utilities.
*/

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
#include <charconv>
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE

#include "core.hpp"
#include "toy_benchmark.hpp"

namespace {

// Text in all four encoded widths, so a conversion meets each of them, a surrogate pair included.
// About 100 units, under 600 bytes with the largest buffer, so a desktop run stays inside the L1 data cache.
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

// Hands an integer through an opaque barrier, so a measured call converts the value instead of a folded constant.
template <typename Value>
[[nodiscard]] Value opaqueValue(Value value) noexcept {
  toy::benchmark::doNotOptimizeAway(value);

  return value;
}

// The decimal ladder 1, 12, 123, up to nineteen digits, the count an int64_t still holds whole.
constexpr toy::array<int64_t, 19> c_digitLadder = [] {
  toy::array<int64_t, 19> values{};
  int64_t                 digits = 0;
  for (size_t index = 0; index < values.size(); ++index) {
    digits        = digits * 10 + 1;
    values[index] = digits;
  }

  return values;
}();

// Converts one rung of the ladder with toy::string_utils::itoa(), the standard printing as the reference row.
void runDigitLadderCase(toy::benchmark::Bench & bench, size_t digitCount) {
  const int64_t value = opaqueValue(c_digitLadder[digitCount - 1]);

  // Sign, nineteen digits, and the terminator, so no rung truncates.
  toy::array<char, 21> buffer{};

  bench.unit("conversion");

  const toy::span out{buffer};

  bench.run("toy::string_utils::itoa", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::itoa(out, value));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  // The standard equivalent writes the same digits but no terminator: the floor of the engine row.
  bench.run("std::to_chars", [&] {
    toy::benchmark::doNotOptimizeAway(std::to_chars(buffer.data(), buffer.data() + buffer.size(), value));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

} // namespace

BENCHMARK_CASE("core/string_utils/utf16_to_utf8") {
  const char16_t * const text = opaque(c_utf16Text);

  // Room for every unit at its widest encoding plus the terminator, so the whole text converts every time.
  toy::array<char, c_utf16Length * toy::string_utils::utf16InUtf8MaxSize + 1> buffer{};

  bench.unit("conversion");

  const toy::span out{buffer};
  const toy::span in{text, c_utf16Length};

  bench.run("toy::string_utils::utf16ToUtf8", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::utf16ToUtf8(out, in));
  });
}

BENCHMARK_CASE("core/string_utils/utf8_to_utf16") {
  const char * const text = opaque(c_utf8Text);

  // One unit per byte at most, plus the terminator, so the whole text converts every time.
  toy::array<char16_t, c_utf8Length + 1> buffer{};

  bench.unit("conversion");

  const toy::span out{buffer};
  const toy::span in{text, c_utf8Length};

  bench.run("toy::string_utils::utf8ToUtf16", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::utf8ToUtf16(out, in));
  });
}

BENCHMARK_CASE("core/string_utils/utf8_len") {
  const char * const text = opaque(c_utf8Text);

  bench.unit("count");

  bench.run("toy::string_utils::utf8Len", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::utf8Len(text));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  // Counts bytes up to the terminator, not characters: the floor of any walk over the same text.
  bench.run("std::char_traits<char>::length", [&] {
    toy::benchmark::doNotOptimizeAway(std::char_traits<char>::length(text));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_utils/utf16_len") {
  const char16_t * const text = opaque(c_utf16Text);

  bench.unit("count");

  bench.run("toy::string_utils::utf16Len", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::utf16Len(text));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  // Counts units up to the terminator, not characters: the floor of any walk over the same text.
  bench.run("std::char_traits<char16_t>::length", [&] {
    toy::benchmark::doNotOptimizeAway(std::char_traits<char16_t>::length(text));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_utils/validate_utf8") {
  // The text is well-formed, so each row walks it to the end through every sequence width.
  const char * const text = opaque(c_utf8Text);

  bench.unit("validation");

  bench.run("toy::string_utils::validateUtf8(const char *)", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::validateUtf8(text));
  });

  const toy::span range{text, c_utf8Length};

  bench.run("toy::string_utils::validateUtf8(toy::span<const char>)", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::validateUtf8(range));
  });
}

BENCHMARK_CASE("core/string_utils/validate_utf16") {
  // The text is well-formed, so each row walks it to the end, its surrogate pairs included.
  const char16_t * const text = opaque(c_utf16Text);

  bench.unit("validation");

  bench.run("toy::string_utils::validateUtf16(const char16_t *)", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::validateUtf16(text));
  });

  const toy::span range{text, c_utf16Length};

  bench.run("toy::string_utils::validateUtf16(toy::span<const char16_t>)", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::validateUtf16(range));
  });
}

// The type's most negative value: a sign, the widest digit count, and the division by ten behind every digit.
BENCHMARK_CASE("core/string_utils/itoa") {
  const int32_t value = opaqueValue(std::numeric_limits<int32_t>::min());

  // Sign, ten digits, and the terminator, so no row ever truncates.
  toy::array<char, 12> buffer{};

  bench.unit("conversion");

  const toy::span out{buffer};

  bench.run("toy::string_utils::itoa", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::itoa(out, value));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::to_chars", [&] {
    toy::benchmark::doNotOptimizeAway(std::to_chars(buffer.data(), buffer.data() + buffer.size(), value));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

// The same conversion one width up, where digit extraction runs through 64-bit division.
BENCHMARK_CASE("core/string_utils/itoa_int64") {
  const int64_t value = opaqueValue(std::numeric_limits<int64_t>::min());

  // Sign, nineteen digits, and the terminator, so no row ever truncates.
  toy::array<char, 21> buffer{};

  bench.unit("conversion");

  const toy::span out{buffer};

  bench.run("toy::string_utils::itoa", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::itoa(out, value));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::to_chars", [&] {
    toy::benchmark::doNotOptimizeAway(std::to_chars(buffer.data(), buffer.data() + buffer.size(), value));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

// The ladder group: the same conversion over 1, 12, 123, up to nineteen digits, the int64_t ceiling of
// toy::string_utils::itoa(), which no standard signed type crosses.
BENCHMARK_CASE("core/string_utils/itoa_1_digit") {
  runDigitLadderCase(bench, 1);
}

BENCHMARK_CASE("core/string_utils/itoa_2_digits") {
  runDigitLadderCase(bench, 2);
}

BENCHMARK_CASE("core/string_utils/itoa_3_digits") {
  runDigitLadderCase(bench, 3);
}

BENCHMARK_CASE("core/string_utils/itoa_4_digits") {
  runDigitLadderCase(bench, 4);
}

BENCHMARK_CASE("core/string_utils/itoa_5_digits") {
  runDigitLadderCase(bench, 5);
}

BENCHMARK_CASE("core/string_utils/itoa_6_digits") {
  runDigitLadderCase(bench, 6);
}

BENCHMARK_CASE("core/string_utils/itoa_7_digits") {
  runDigitLadderCase(bench, 7);
}

BENCHMARK_CASE("core/string_utils/itoa_8_digits") {
  runDigitLadderCase(bench, 8);
}

BENCHMARK_CASE("core/string_utils/itoa_9_digits") {
  runDigitLadderCase(bench, 9);
}

BENCHMARK_CASE("core/string_utils/itoa_10_digits") {
  runDigitLadderCase(bench, 10);
}

BENCHMARK_CASE("core/string_utils/itoa_11_digits") {
  runDigitLadderCase(bench, 11);
}

BENCHMARK_CASE("core/string_utils/itoa_12_digits") {
  runDigitLadderCase(bench, 12);
}

BENCHMARK_CASE("core/string_utils/itoa_13_digits") {
  runDigitLadderCase(bench, 13);
}

BENCHMARK_CASE("core/string_utils/itoa_14_digits") {
  runDigitLadderCase(bench, 14);
}

BENCHMARK_CASE("core/string_utils/itoa_15_digits") {
  runDigitLadderCase(bench, 15);
}

BENCHMARK_CASE("core/string_utils/itoa_16_digits") {
  runDigitLadderCase(bench, 16);
}

BENCHMARK_CASE("core/string_utils/itoa_17_digits") {
  runDigitLadderCase(bench, 17);
}

BENCHMARK_CASE("core/string_utils/itoa_18_digits") {
  runDigitLadderCase(bench, 18);
}

BENCHMARK_CASE("core/string_utils/itoa_19_digits") {
  runDigitLadderCase(bench, 19);
}

// The widest decimal form of each unsigned type: ten digits of u32, twenty of u64, no sign to carry.
BENCHMARK_CASE("core/string_utils/utoa") {
  const uint32_t value = opaqueValue(std::numeric_limits<uint32_t>::max());

  // Ten digits and the terminator, so no row ever truncates.
  toy::array<char, 11> buffer{};

  bench.unit("conversion");

  bench.run("toy::string_utils::utoa", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::utoa(toy::span{buffer}, value));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::to_chars", [&] {
    toy::benchmark::doNotOptimizeAway(std::to_chars(buffer.data(), buffer.data() + buffer.size(), value));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_utils/utoa_int64") {
  const uint64_t value = opaqueValue(std::numeric_limits<uint64_t>::max());

  // Twenty digits and the terminator, the longest decimal form any standard type has.
  toy::array<char, 21> buffer{};

  bench.unit("conversion");

  bench.run("toy::string_utils::utoa", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::utoa(toy::span{buffer}, value));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::to_chars", [&] {
    toy::benchmark::doNotOptimizeAway(std::to_chars(buffer.data(), buffer.data() + buffer.size(), value));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

// Fixed-width hexadecimal: eight and sixteen uppercase digits, zero-padded. std::to_chars prints the same digits
// unpadded and lowercase: the floor of the engine row.
BENCHMARK_CASE("core/string_utils/utoa_hex") {
  const uint32_t value = opaqueValue(static_cast<uint32_t>(0xDEADBEEF));

  // Eight digits and the terminator, so the rows never truncate.
  toy::array<char, 9> buffer{};

  bench.unit("conversion");

  bench.run("toy::string_utils::utoaHex", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::utoaHex(toy::span{buffer}, value));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::to_chars", [&] {
    toy::benchmark::doNotOptimizeAway(std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, 16));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}

BENCHMARK_CASE("core/string_utils/utoa_hex_int64") {
  const uint64_t value = opaqueValue(UINT64_C(0xDEADBEEFCAFEBABE));

  // Sixteen digits and the terminator, so the rows never truncate.
  toy::array<char, 17> buffer{};

  bench.unit("conversion");

  bench.run("toy::string_utils::utoaHex", [&] {
    toy::benchmark::doNotOptimizeAway(toy::string_utils::utoaHex(toy::span{buffer}, value));
  });

#if defined(TOYGINE_BENCHMARKS_STD_REFERENCE)
  bench.run("std::to_chars", [&] {
    toy::benchmark::doNotOptimizeAway(std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, 16));
  });
#endif // TOYGINE_BENCHMARKS_STD_REFERENCE
}
