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
  \file   utils.test.cpp
  \brief  Unit tests for the core UTF-8 utilities.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// Literals of one encoding width each, so an expected count derives from the byte length instead of a hand count.
constexpr const char * c_ascii    = "player";
constexpr const char * c_cyrillic = "привет";
constexpr const char * c_cjk      = "日本語";
constexpr const char * c_emoji    = "🎮🎲";

// The same four scripts in one string, the parts separated by three spaces.
constexpr const char * c_mixed = "player привет 日本語 🎮🎲";

// The shortest and the longest sequence each narrowed second-byte range admits: U+0800, U+D7FF, U+10000, U+10FFFF.
constexpr char c_lowestThreeByte[]  = {'\xE0', '\xA0', '\x80', '\0'};
constexpr char c_beforeSurrogates[] = {'\xED', '\x9F', '\xBF', '\0'};
constexpr char c_lowestFourByte[]   = {'\xF0', '\x90', '\x80', '\x80', '\0'};
constexpr char c_highestCodePoint[] = {'\xF4', '\x8F', '\xBF', '\xBF', '\0'};

// A continuation byte, which the lookup table maps to a length of zero because it starts no sequence.
constexpr const char * c_continuationByte = "\x80";

// A three-byte lead byte the terminator cuts short after a single continuation byte.
constexpr char c_truncatedSymbol[] = {'a', '\xE6', '\x97', '\0'};

// A two-byte lead byte followed by an ASCII character rather than a byte of the 10xxxxxx form.
constexpr char c_lonelyLead[] = {'\xC2', 'A', '\0'};

// The ill-formed sequences the lead byte alone admits: an overlong '/', the surrogate U+D800, and the code point one
// past U+10FFFF.
constexpr char c_overlongSymbol[]  = {'\xE0', '\x80', '\xAF', '\0'};
constexpr char c_surrogateSymbol[] = {'\xED', '\xA0', '\x80', '\0'};
constexpr char c_aboveRange[]      = {'\xF4', '\x90', '\x80', '\x80', '\0'};

// A terminator between two ASCII characters, which a counted range reads as one more character.
constexpr char c_innerTerminator[] = {'a', '\0', 'b'};

// The UTF-16 counterpart of c_innerTerminator.
constexpr char16_t c_utf16InnerTerminator[] = {u'a', u'\0', u'b'};

// A UTF-16 literal and the UTF-8 literal of the same text.
struct Utf16Sample {
  const char16_t * utf16;
  const char *     utf8;
};

// UTF-16 literals of one encoding width each, then all four widths in one string.
constexpr std::array<Utf16Sample, 5> c_utf16Samples{
  {
   {u"player", c_ascii},
   {u"привет", c_cyrillic},
   {u"日本語", c_cjk},
   {u"🎮🎲", c_emoji},
   {u"player привет 日本語 🎮🎲", c_mixed},
   }
};

// Surrogates outside a pair after an 'a': high before ASCII, high at the end, lone low, and a reversed pair.
constexpr std::array<std::array<char16_t, 4>, 4> c_loneSurrogates{
  {
   {u'a', 0xD83C, u'b', u'\0'},
   {u'a', 0xD83C, u'\0', u'\0'},
   {u'a', 0xDFAE, u'b', u'\0'},
   {u'a', 0xDFAE, 0xD83C, u'\0'},
   }
};

// The units at both ends of the surrogate range, alone or in a pair: U+D7FF, U+E000, U+10000, U+10FFFF.
constexpr char16_t c_utf16BeforeSurrogates[] = {0xD7FF, u'\0'};
constexpr char16_t c_utf16AfterSurrogates[]  = {0xE000, u'\0'};
constexpr char16_t c_utf16LowestPair[]       = {0xD800, 0xDC00, u'\0'};
constexpr char16_t c_utf16HighestCodePoint[] = {0xDBFF, 0xDFFF, u'\0'};

// The sequences toy::validateUtf8() rejects, the three a narrowed second-byte range catches among them.
constexpr std::array<const char *, 5> c_rejectedSequences{c_continuationByte, c_lonelyLead, c_overlongSymbol,
                                                          c_surrogateSymbol, c_aboveRange};

// A byte the conversion never writes, so a surviving one shows where the call stopped.
constexpr char c_sentinel = '#';

// The UTF-16 counterpart of c_sentinel, for buffers of char16_t.
constexpr char16_t c_utf16Sentinel = u'#';

} // namespace

// How many characters a UTF-8 string holds, against the bytes it occupies.
TEST_CASE("utf8_len/counts_characters_not_bytes") {
  CHECK(utf8Len(c_ascii) == std::char_traits<char>::length(c_ascii));
  CHECK(utf8Len(c_cyrillic) == std::char_traits<char>::length(c_cyrillic) / 2);
  CHECK(utf8Len(c_cjk) == std::char_traits<char>::length(c_cjk) / 3);
  CHECK(utf8Len(c_emoji) == std::char_traits<char>::length(c_emoji) / 4);

  // A mixed string counts every character once, whatever width each one is stored in.
  CHECK(utf8Len(c_mixed) == utf8Len(c_ascii) + utf8Len(c_cyrillic) + utf8Len(c_cjk) + utf8Len(c_emoji) + 3);

  CHECK(utf8Len("") == 0);
}

// Which sequences the narrowed second-byte ranges still accept, at both ends of every narrowed range.
TEST_CASE("utf8_len/counts_sequences_at_the_range_edges") {
  CHECK(utf8Len(c_lowestThreeByte) == 1);
  CHECK(utf8Len(c_beforeSurrogates) == 1);
  CHECK(utf8Len(c_lowestFourByte) == 1);
  CHECK(utf8Len(c_highestCodePoint) == 1);
}

// A surrogate pair counts as one character, so every sample holds as many characters as its UTF-8 form.
TEST_CASE("utf16_len/counts_characters_not_units") {
  for (size_t index = 0; index < c_utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = c_utf16Samples[index];

    CHECK(utf16Len(utf16) == utf8Len(utf8));
  }

  CHECK(utf16Len(u"🎮🎲") == std::char_traits<char16_t>::length(u"🎮🎲") / 2);
  CHECK(utf16Len(u"") == 0);
}

// A character at either edge of the surrogate range counts once, whether it takes one unit or a pair.
TEST_CASE("utf16_len/counts_units_at_the_surrogate_edges") {
  CHECK(utf16Len(c_utf16BeforeSurrogates) == 1);
  CHECK(utf16Len(c_utf16AfterSurrogates) == 1);
  CHECK(utf16Len(c_utf16LowestPair) == 1);
  CHECK(utf16Len(c_utf16HighestCodePoint) == 1);
}

// Which strings the validator accepts: every encoding width and both ends of every narrowed range.
TEST_CASE("validate_utf8/accepts_well_formed_sequences") {
  CHECK(validateUtf8(c_ascii));
  CHECK(validateUtf8(c_cyrillic));
  CHECK(validateUtf8(c_cjk));
  CHECK(validateUtf8(c_emoji));
  CHECK(validateUtf8(c_mixed));

  CHECK(validateUtf8(c_lowestThreeByte));
  CHECK(validateUtf8(c_beforeSurrogates));
  CHECK(validateUtf8(c_lowestFourByte));
  CHECK(validateUtf8(c_highestCodePoint));

  // A string holding no character holds no ill-formed one either.
  CHECK(validateUtf8(""));

  // The counted form accepts the same strings, measured by their length.
  CHECK(validateUtf8(c_mixed, std::char_traits<char>::length(c_mixed)));
  CHECK(validateUtf8(c_highestCodePoint, std::size(c_highestCodePoint) - 1));
  CHECK(validateUtf8(nullptr, 0));
}

// Which strings it rejects, among them the sequences a lead byte alone admits.
TEST_CASE("validate_utf8/rejects_ill_formed_sequences") {
  // A null pointer names no string to check, which the validator answers rather than asserts.
  CHECK_FALSE(validateUtf8(nullptr));

  CHECK_FALSE(validateUtf8(c_continuationByte));
  CHECK_FALSE(validateUtf8(c_truncatedSymbol));

  // A trailing byte outside the 10xxxxxx form, whatever width the lead byte announced.
  CHECK_FALSE(validateUtf8(c_lonelyLead));

  // The three a narrowed second-byte range rejects: overlong, surrogate, and past U+10FFFF.
  CHECK_FALSE(validateUtf8(c_overlongSymbol));
  CHECK_FALSE(validateUtf8(c_surrogateSymbol));
  CHECK_FALSE(validateUtf8(c_aboveRange));

  // The counted form rejects the same sequences, and a null pointer that names bytes.
  CHECK_FALSE(validateUtf8(c_continuationByte, 1));
  CHECK_FALSE(validateUtf8(c_lonelyLead, std::size(c_lonelyLead) - 1));
  CHECK_FALSE(validateUtf8(c_overlongSymbol, std::size(c_overlongSymbol) - 1));
  CHECK_FALSE(validateUtf8(c_surrogateSymbol, std::size(c_surrogateSymbol) - 1));
  CHECK_FALSE(validateUtf8(c_aboveRange, std::size(c_aboveRange) - 1));
  CHECK_FALSE(validateUtf8(nullptr, 1));
}

// A counted range ends at its length: bytes past it go unread, and a terminator inside it is an ordinary byte.
TEST_CASE("validate_utf8/reads_only_the_counted_range") {
  // The ill-formed tail sits past the length, so the range holds the leading 'a' alone.
  CHECK(validateUtf8(c_truncatedSymbol, 1));

  // A length ending inside a sequence cuts it short, even where the bytes after it would complete it.
  CHECK_FALSE(validateUtf8(c_cyrillic, 1));
  CHECK(validateUtf8(c_cyrillic, 2));

  // A terminator inside the range is U+0000, a well-formed one-byte character.
  CHECK(validateUtf8(c_innerTerminator, std::size(c_innerTerminator)));
}

// Which strings the validator accepts: both widths and both ends of the surrogate range.
TEST_CASE("validate_utf16/accepts_well_formed_strings") {
  for (size_t index = 0; index < c_utf16Samples.size(); ++index) {
    INFO("index ", index);
    const char16_t * const utf16 = c_utf16Samples[index].utf16;

    CHECK(validateUtf16(utf16));
    CHECK(validateUtf16(utf16, std::char_traits<char16_t>::length(utf16)));
  }

  CHECK(validateUtf16(c_utf16BeforeSurrogates));
  CHECK(validateUtf16(c_utf16AfterSurrogates));
  CHECK(validateUtf16(c_utf16LowestPair));
  CHECK(validateUtf16(c_utf16HighestCodePoint));

  // A string holding no character holds no ill-formed one either.
  CHECK(validateUtf16(u""));
  CHECK(validateUtf16(nullptr, 0));
}

// Which strings it rejects: a surrogate outside a pair, wherever it sits, and a null pointer.
TEST_CASE("validate_utf16/rejects_ill_formed_strings") {
  // A null pointer names no string to check, which the validator answers rather than asserts.
  CHECK_FALSE(validateUtf16(nullptr));

  for (size_t index = 0; index < c_loneSurrogates.size(); ++index) {
    INFO("index ", index);
    const char16_t * const units = c_loneSurrogates[index].data();

    CHECK_FALSE(validateUtf16(units));
    CHECK_FALSE(validateUtf16(units, std::char_traits<char16_t>::length(units)));
  }

  // The counted form rejects a null pointer that names units.
  CHECK_FALSE(validateUtf16(nullptr, 1));
}

// A counted range ends at its length: units past it go unread, and a terminator inside it is an ordinary unit.
TEST_CASE("validate_utf16/reads_only_the_counted_range") {
  // The high surrogate sits past the length, so the range holds the leading 'a' alone.
  CHECK(validateUtf16(c_loneSurrogates[1].data(), 1));

  // A length ending between the halves of a pair cuts it short, even where the unit after it would complete it.
  CHECK_FALSE(validateUtf16(u"a🎮", 2));
  CHECK(validateUtf16(u"a🎮", 3));

  // A terminator inside the range is U+0000, a well-formed one-unit character.
  CHECK(validateUtf16(c_utf16InnerTerminator, std::size(c_utf16InnerTerminator)));
}

// Every width converts to the bytes of the matching UTF-8 literal, and the call hands back the buffer it wrote.
TEST_CASE("utf16_to_utf8/encodes_every_width") {
  for (size_t index = 0; index < c_utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = c_utf16Samples[index];
    const auto length          = std::char_traits<char>::length(utf8);

    std::array<char, 64> buffer{};
    CHECK(utf16ToUtf8(buffer.data(), buffer.size(), utf16, std::char_traits<char16_t>::length(utf16)) == buffer.data());
    CHECK(std::char_traits<char>::length(buffer.data()) == length);
    CHECK(std::char_traits<char>::compare(buffer.data(), utf8, length) == 0);
  }
}

// The unit count bounds the input: a terminator inside it is encoded, and a pair it cuts short ends the call.
TEST_CASE("utf16_to_utf8/reads_only_the_counted_units") {
  std::array<char, 8> inner{c_sentinel, c_sentinel, c_sentinel, c_sentinel, c_sentinel};
  utf16ToUtf8(inner.data(), inner.size(), c_utf16InnerTerminator, std::size(c_utf16InnerTerminator));
  CHECK(inner[0] == 'a');
  CHECK(inner[1] == '\0');
  CHECK(inner[2] == 'b');
  CHECK(inner[3] == '\0');

  // Three units of a two-byte script encode three characters and leave the rest unread.
  const auto prefixBytes = std::char_traits<char>::length("при");

  std::array<char, 16> prefix{};
  utf16ToUtf8(prefix.data(), prefix.size(), u"привет", 3);
  CHECK(std::char_traits<char>::length(prefix.data()) == prefixBytes);
  CHECK(std::char_traits<char>::compare(prefix.data(), "при", prefixBytes) == 0);

  // A count ending between the halves of a pair keeps what precedes the pair and stops there.
  std::array<char, 16> cut{c_sentinel, c_sentinel, c_sentinel, c_sentinel};
  utf16ToUtf8(cut.data(), cut.size(), u"a🎮", 2);
  CHECK(cut[0] == 'a');
  CHECK(cut[1] == '\0');
}

// A character that does not fit whole stays out, and the terminator always fits.
TEST_CASE("utf16_to_utf8/stops_before_a_character_that_does_not_fit") {
  const auto length = std::char_traits<char>::length(c_cyrillic);
  const auto units  = std::char_traits<char16_t>::length(u"привет");

  // A buffer of exactly the encoded length plus the terminator holds the whole string.
  std::array<char, 32> exact{};
  utf16ToUtf8(exact.data(), length + 1, u"привет", units);
  CHECK(std::char_traits<char>::length(exact.data()) == length);

  // One byte less leaves out the whole last two-byte character.
  std::array<char, 32> shorter{};
  utf16ToUtf8(shorter.data(), length, u"привет", units);
  CHECK(std::char_traits<char>::length(shorter.data()) == length - 2);
  CHECK(std::char_traits<char>::compare(shorter.data(), c_cyrillic, length - 2) == 0);

  // A single byte holds the terminator alone, and nothing past the size is written.
  std::array<char, 4> single{c_sentinel, c_sentinel, c_sentinel, c_sentinel};
  utf16ToUtf8(single.data(), 1, u"привет", units);
  CHECK(single[0] == '\0');
  CHECK(single[1] == c_sentinel);
}

// A surrogate pair goes in as one four-byte sequence or not at all.
TEST_CASE("utf16_to_utf8/writes_a_surrogate_pair_whole_or_not_at_all") {
  const auto pairBytes = std::char_traits<char>::length("🎮");
  const auto pairUnits = std::char_traits<char16_t>::length(u"🎮");

  std::array<char, 8> pairShort{c_sentinel, c_sentinel, c_sentinel, c_sentinel};
  utf16ToUtf8(pairShort.data(), pairBytes, u"🎮", pairUnits);
  CHECK(pairShort[0] == '\0');

  std::array<char, 8> pairWhole{};
  utf16ToUtf8(pairWhole.data(), pairBytes + 1, u"🎮", pairUnits);
  CHECK(std::char_traits<char>::compare(pairWhole.data(), "🎮", pairBytes + 1) == 0);
}

// A surrogate outside a pair encodes no code point, so the call keeps what precedes it and stops there.
TEST_CASE("utf16_to_utf8/stops_before_a_lone_surrogate") {
  for (size_t index = 0; index < c_loneSurrogates.size(); ++index) {
    INFO("index ", index);

    const char16_t * const units = c_loneSurrogates[index].data();

    std::array<char, 16> buffer{};
    utf16ToUtf8(buffer.data(), buffer.size(), units, std::char_traits<char16_t>::length(units));
    CHECK(std::char_traits<char>::length(buffer.data()) == 1);
    CHECK(buffer[0] == 'a');
  }
}

// A null or empty source converts as an empty string; a buffer that cannot hold the terminator gets no write at all.
TEST_CASE("utf16_to_utf8/handles_null_and_empty_arguments") {
  std::array<char, 4> untouched{c_sentinel, c_sentinel, c_sentinel, c_sentinel};
  CHECK(utf16ToUtf8(nullptr, untouched.size(), u"player", 1) == nullptr);
  CHECK(utf16ToUtf8(untouched.data(), 0, u"player", 1) == nullptr);
  CHECK(untouched[0] == c_sentinel);

  std::array<char, 4> fromNull{c_sentinel, c_sentinel, c_sentinel, c_sentinel};
  CHECK(utf16ToUtf8(fromNull.data(), fromNull.size(), nullptr, 4) == fromNull.data());
  CHECK(fromNull[0] == '\0');

  std::array<char, 4> fromEmpty{c_sentinel, c_sentinel, c_sentinel, c_sentinel};
  CHECK(utf16ToUtf8(fromEmpty.data(), fromEmpty.size(), u"player", 0) == fromEmpty.data());
  CHECK(fromEmpty[0] == '\0');
}

// Every width decodes to the matching UTF-16 literal, a pair for four bytes, and the call hands back its buffer.
TEST_CASE("utf8_to_utf16/decodes_every_width") {
  for (size_t index = 0; index < c_utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = c_utf16Samples[index];
    const auto length          = std::char_traits<char16_t>::length(utf16);

    std::array<char16_t, 64> buffer{};
    CHECK(utf8ToUtf16(buffer.data(), buffer.size(), utf8, std::char_traits<char>::length(utf8)) == buffer.data());
    CHECK(std::char_traits<char16_t>::length(buffer.data()) == length);
    CHECK(std::char_traits<char16_t>::compare(buffer.data(), utf16, length) == 0);
  }
}

// The byte count bounds the input: a terminator inside it is decoded, and a sequence it cuts short ends the call.
TEST_CASE("utf8_to_utf16/reads_only_the_counted_bytes") {
  std::array<char16_t, 8> inner{};
  utf8ToUtf16(inner.data(), inner.size(), c_innerTerminator, std::size(c_innerTerminator));
  CHECK(inner[0] == u'a');
  CHECK(inner[1] == u'\0');
  CHECK(inner[2] == u'b');
  CHECK(inner[3] == u'\0');

  // Three bytes of a two-byte script hold one whole character and the lead byte of the next.
  std::array<char16_t, 8> cut{};
  utf8ToUtf16(cut.data(), cut.size(), c_cyrillic, 3);
  CHECK(std::char_traits<char16_t>::length(cut.data()) == 1);
  CHECK(cut[0] == u'п');
}

// A sequence the validator rejects ends the call, and the characters before it stay.
TEST_CASE("utf8_to_utf16/stops_at_a_sequence_it_cannot_decode") {
  for (size_t index = 0; index < c_rejectedSequences.size(); ++index) {
    INFO("index ", index);
    const char * const sequence = c_rejectedSequences[index];

    std::array<char16_t, 4> buffer{c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel};
    utf8ToUtf16(buffer.data(), buffer.size(), sequence, std::char_traits<char>::length(sequence));
    CHECK(buffer[0] == u'\0');
  }

  std::array<char16_t, 4> prefix{};
  utf8ToUtf16(prefix.data(), prefix.size(), c_truncatedSymbol, std::size(c_truncatedSymbol) - 1);
  CHECK(prefix[0] == u'a');
  CHECK(prefix[1] == u'\0');
}

// A character that does not fit stays out, and the terminator always fits.
TEST_CASE("utf8_to_utf16/stops_when_the_buffer_is_full") {
  const auto bytes  = std::char_traits<char>::length(c_cyrillic);
  const auto length = utf8Len(c_cyrillic);

  // A buffer of exactly the character count plus the terminator holds the whole string.
  std::array<char16_t, 16> exact{};
  utf8ToUtf16(exact.data(), length + 1, c_cyrillic, bytes);
  CHECK(std::char_traits<char16_t>::compare(exact.data(), u"привет", length) == 0);
  CHECK(exact[length] == u'\0');

  // One element less leaves out the last character.
  std::array<char16_t, 16> shorter{};
  utf8ToUtf16(shorter.data(), length, c_cyrillic, bytes);
  CHECK(std::char_traits<char16_t>::length(shorter.data()) == length - 1);
  CHECK(std::char_traits<char16_t>::compare(shorter.data(), u"привет", length - 1) == 0);

  // ASCII takes a shorter path through the decoder and stops at a full buffer the same way.
  const auto asciiLength = std::char_traits<char>::length(c_ascii);

  std::array<char16_t, 16> asciiShorter{};
  utf8ToUtf16(asciiShorter.data(), asciiLength, c_ascii, asciiLength);
  CHECK(std::char_traits<char16_t>::length(asciiShorter.data()) == asciiLength - 1);
  CHECK(std::char_traits<char16_t>::compare(asciiShorter.data(), u"player", asciiLength - 1) == 0);

  // A single element holds the terminator alone, and nothing past the size is written.
  std::array<char16_t, 4> single{c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel};
  utf8ToUtf16(single.data(), 1, c_cyrillic, bytes);
  CHECK(single[0] == u'\0');
  CHECK(single[1] == c_utf16Sentinel);
}

// A four-byte character goes in as a whole surrogate pair or not at all.
TEST_CASE("utf8_to_utf16/writes_a_surrogate_pair_whole_or_not_at_all") {
  const auto pairBytes = std::char_traits<char>::length("🎮");
  const auto pairUnits = std::char_traits<char16_t>::length(u"🎮");

  std::array<char16_t, 4> pairShort{c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel};
  utf8ToUtf16(pairShort.data(), pairUnits, "🎮", pairBytes);
  CHECK(pairShort[0] == u'\0');

  std::array<char16_t, 4> pairWhole{};
  utf8ToUtf16(pairWhole.data(), pairUnits + 1, "🎮", pairBytes);
  CHECK(std::char_traits<char16_t>::compare(pairWhole.data(), u"🎮", pairUnits + 1) == 0);
}

// A null or empty source decodes as an empty string; a buffer that cannot hold the terminator gets no write at all.
TEST_CASE("utf8_to_utf16/handles_null_and_empty_arguments") {
  std::array<char16_t, 4> untouched{c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel};
  CHECK(utf8ToUtf16(nullptr, untouched.size(), c_ascii, 1) == nullptr);
  CHECK(utf8ToUtf16(untouched.data(), 0, c_ascii, 1) == nullptr);
  CHECK(untouched[0] == c_utf16Sentinel);

  std::array<char16_t, 4> fromNull{c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel};
  CHECK(utf8ToUtf16(fromNull.data(), fromNull.size(), nullptr, 4) == fromNull.data());
  CHECK(fromNull[0] == u'\0');

  std::array<char16_t, 4> fromEmpty{c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel, c_utf16Sentinel};
  CHECK(utf8ToUtf16(fromEmpty.data(), fromEmpty.size(), c_ascii, 0) == fromEmpty.data());
  CHECK(fromEmpty[0] == u'\0');
}

} // namespace toy
