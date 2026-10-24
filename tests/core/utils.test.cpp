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
constexpr const char * ascii    = "player";
constexpr const char * cyrillic = "привет";
constexpr const char * cjk      = "日本語";
constexpr const char * emoji    = "🎮🎲";

// The same four scripts in one string, the parts separated by three spaces.
constexpr const char * mixed = "player привет 日本語 🎮🎲";

// The shortest and the longest sequence each narrowed second-byte range admits: U+0800, U+D7FF, U+10000, U+10FFFF.
constexpr char lowestThreeByte[]  = {'\xE0', '\xA0', '\x80', '\0'};
constexpr char beforeSurrogates[] = {'\xED', '\x9F', '\xBF', '\0'};
constexpr char lowestFourByte[]   = {'\xF0', '\x90', '\x80', '\x80', '\0'};
constexpr char highestCodePoint[] = {'\xF4', '\x8F', '\xBF', '\xBF', '\0'};

// A continuation byte, which the lookup table maps to a length of zero because it starts no sequence.
constexpr const char * continuationByte = "\x80";

// A three-byte lead byte the terminator cuts short after a single continuation byte.
constexpr char truncatedSymbol[] = {'a', '\xE6', '\x97', '\0'};

// A two-byte lead byte followed by an ASCII character rather than a byte of the 10xxxxxx form.
constexpr char lonelyLead[] = {'\xC2', 'A', '\0'};

// The ill-formed sequences the lead byte alone admits: an overlong '/', the surrogate U+D800, and the code point one
// past U+10FFFF.
constexpr char overlongSymbol[]  = {'\xE0', '\x80', '\xAF', '\0'};
constexpr char surrogateSymbol[] = {'\xED', '\xA0', '\x80', '\0'};
constexpr char aboveRange[]      = {'\xF4', '\x90', '\x80', '\x80', '\0'};

// A terminator between two ASCII characters, which a counted range reads as one more character.
constexpr char innerTerminator[] = {'a', '\0', 'b'};

// The UTF-16 counterpart of innerTerminator.
constexpr char16_t utf16InnerTerminator[] = {u'a', u'\0', u'b'};

// A UTF-16 literal and the UTF-8 literal of the same text.
struct Utf16Sample {
  const char16_t * utf16;
  const char *     utf8;
};

// UTF-16 literals of one encoding width each, then all four widths in one string.
constexpr std::array<Utf16Sample, 5> utf16Samples{
  {
   {u"player", ascii},
   {u"привет", cyrillic},
   {u"日本語", cjk},
   {u"🎮🎲", emoji},
   {u"player привет 日本語 🎮🎲", mixed},
   }
};

// Surrogates outside a pair after an 'a': high before ASCII, high at the end, lone low, and a reversed pair.
constexpr std::array<std::array<char16_t, 4>, 4> loneSurrogates{
  {
   {u'a', 0xD83C, u'b', u'\0'},
   {u'a', 0xD83C, u'\0', u'\0'},
   {u'a', 0xDFAE, u'b', u'\0'},
   {u'a', 0xDFAE, 0xD83C, u'\0'},
   }
};

// The units at both ends of the surrogate range, alone or in a pair: U+D7FF, U+E000, U+10000, U+10FFFF.
constexpr char16_t utf16BeforeSurrogates[] = {0xD7FF, u'\0'};
constexpr char16_t utf16AfterSurrogates[]  = {0xE000, u'\0'};
constexpr char16_t utf16LowestPair[]       = {0xD800, 0xDC00, u'\0'};
constexpr char16_t utf16HighestCodePoint[] = {0xDBFF, 0xDFFF, u'\0'};

// The sequences toy::validateUtf8() rejects, the three a narrowed second-byte range catches among them.
constexpr std::array<const char *, 5> rejectedSequences{continuationByte, lonelyLead, overlongSymbol, surrogateSymbol,
                                                        aboveRange};

// A byte the conversion never writes, so a surviving one shows where the call stopped.
constexpr char sentinel = '#';

// The UTF-16 counterpart of sentinel, for buffers of char16_t.
constexpr char16_t utf16Sentinel = u'#';

} // namespace

// How many characters a UTF-8 string holds, against the bytes it occupies.
TEST_CASE("utf8_len/counts_characters_not_bytes") {
  CHECK(utf8Len(ascii) == std::char_traits<char>::length(ascii));
  CHECK(utf8Len(cyrillic) == std::char_traits<char>::length(cyrillic) / 2);
  CHECK(utf8Len(cjk) == std::char_traits<char>::length(cjk) / 3);
  CHECK(utf8Len(emoji) == std::char_traits<char>::length(emoji) / 4);

  // A mixed string counts every character once, whatever width each one is stored in.
  CHECK(utf8Len(mixed) == utf8Len(ascii) + utf8Len(cyrillic) + utf8Len(cjk) + utf8Len(emoji) + 3);

  CHECK(utf8Len("") == 0);
}

// Which sequences the narrowed second-byte ranges still accept, at both ends of every narrowed range.
TEST_CASE("utf8_len/counts_sequences_at_the_range_edges") {
  CHECK(utf8Len(lowestThreeByte) == 1);
  CHECK(utf8Len(beforeSurrogates) == 1);
  CHECK(utf8Len(lowestFourByte) == 1);
  CHECK(utf8Len(highestCodePoint) == 1);
}

// A surrogate pair counts as one character, so every sample holds as many characters as its UTF-8 form.
TEST_CASE("utf16_len/counts_characters_not_units") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];

    CHECK(utf16Len(utf16) == utf8Len(utf8));
  }

  CHECK(utf16Len(u"🎮🎲") == std::char_traits<char16_t>::length(u"🎮🎲") / 2);
  CHECK(utf16Len(u"") == 0);
}

// A character at either edge of the surrogate range counts once, whether it takes one unit or a pair.
TEST_CASE("utf16_len/counts_units_at_the_surrogate_edges") {
  CHECK(utf16Len(utf16BeforeSurrogates) == 1);
  CHECK(utf16Len(utf16AfterSurrogates) == 1);
  CHECK(utf16Len(utf16LowestPair) == 1);
  CHECK(utf16Len(utf16HighestCodePoint) == 1);
}

// Which strings the validator accepts: every encoding width and both ends of every narrowed range.
TEST_CASE("validate_utf8/accepts_well_formed_sequences") {
  CHECK(validateUtf8(ascii));
  CHECK(validateUtf8(cyrillic));
  CHECK(validateUtf8(cjk));
  CHECK(validateUtf8(emoji));
  CHECK(validateUtf8(mixed));

  CHECK(validateUtf8(lowestThreeByte));
  CHECK(validateUtf8(beforeSurrogates));
  CHECK(validateUtf8(lowestFourByte));
  CHECK(validateUtf8(highestCodePoint));

  // A string holding no character holds no ill-formed one either.
  CHECK(validateUtf8(""));

  // The counted form accepts the same strings, measured by their length.
  CHECK(validateUtf8(mixed, std::char_traits<char>::length(mixed)));
  CHECK(validateUtf8(highestCodePoint, std::size(highestCodePoint) - 1));
  CHECK(validateUtf8(nullptr, 0));
}

// Which strings it rejects, among them the sequences a lead byte alone admits.
TEST_CASE("validate_utf8/rejects_ill_formed_sequences") {
  // A null pointer names no string to check, which the validator answers rather than asserts.
  CHECK_FALSE(validateUtf8(nullptr));

  CHECK_FALSE(validateUtf8(continuationByte));
  CHECK_FALSE(validateUtf8(truncatedSymbol));

  // A trailing byte outside the 10xxxxxx form, whatever width the lead byte announced.
  CHECK_FALSE(validateUtf8(lonelyLead));

  // The three a narrowed second-byte range rejects: overlong, surrogate, and past U+10FFFF.
  CHECK_FALSE(validateUtf8(overlongSymbol));
  CHECK_FALSE(validateUtf8(surrogateSymbol));
  CHECK_FALSE(validateUtf8(aboveRange));

  // The counted form rejects the same sequences, and a null pointer that names bytes.
  CHECK_FALSE(validateUtf8(continuationByte, 1));
  CHECK_FALSE(validateUtf8(lonelyLead, std::size(lonelyLead) - 1));
  CHECK_FALSE(validateUtf8(overlongSymbol, std::size(overlongSymbol) - 1));
  CHECK_FALSE(validateUtf8(surrogateSymbol, std::size(surrogateSymbol) - 1));
  CHECK_FALSE(validateUtf8(aboveRange, std::size(aboveRange) - 1));
  CHECK_FALSE(validateUtf8(nullptr, 1));
}

// A counted range ends at its length: bytes past it go unread, and a terminator inside it is an ordinary byte.
TEST_CASE("validate_utf8/reads_only_the_counted_range") {
  // The ill-formed tail sits past the length, so the range holds the leading 'a' alone.
  CHECK(validateUtf8(truncatedSymbol, 1));

  // A length ending inside a sequence cuts it short, even where the bytes after it would complete it.
  CHECK_FALSE(validateUtf8(cyrillic, 1));
  CHECK(validateUtf8(cyrillic, 2));

  // A terminator inside the range is U+0000, a well-formed one-byte character.
  CHECK(validateUtf8(innerTerminator, std::size(innerTerminator)));
}

// Which strings the validator accepts: both widths and both ends of the surrogate range.
TEST_CASE("validate_utf16/accepts_well_formed_strings") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const char16_t * const utf16 = utf16Samples[index].utf16;

    CHECK(validateUtf16(utf16));
    CHECK(validateUtf16(utf16, std::char_traits<char16_t>::length(utf16)));
  }

  CHECK(validateUtf16(utf16BeforeSurrogates));
  CHECK(validateUtf16(utf16AfterSurrogates));
  CHECK(validateUtf16(utf16LowestPair));
  CHECK(validateUtf16(utf16HighestCodePoint));

  // A string holding no character holds no ill-formed one either.
  CHECK(validateUtf16(u""));
  CHECK(validateUtf16(nullptr, 0));
}

// Which strings it rejects: a surrogate outside a pair, wherever it sits, and a null pointer.
TEST_CASE("validate_utf16/rejects_ill_formed_strings") {
  // A null pointer names no string to check, which the validator answers rather than asserts.
  CHECK_FALSE(validateUtf16(nullptr));

  for (size_t index = 0; index < loneSurrogates.size(); ++index) {
    INFO("index ", index);
    const char16_t * const units = loneSurrogates[index].data();

    CHECK_FALSE(validateUtf16(units));
    CHECK_FALSE(validateUtf16(units, std::char_traits<char16_t>::length(units)));
  }

  // The counted form rejects a null pointer that names units.
  CHECK_FALSE(validateUtf16(nullptr, 1));
}

// A counted range ends at its length: units past it go unread, and a terminator inside it is an ordinary unit.
TEST_CASE("validate_utf16/reads_only_the_counted_range") {
  // The high surrogate sits past the length, so the range holds the leading 'a' alone.
  CHECK(validateUtf16(loneSurrogates[1].data(), 1));

  // A length ending between the halves of a pair cuts it short, even where the unit after it would complete it.
  CHECK_FALSE(validateUtf16(u"a🎮", 2));
  CHECK(validateUtf16(u"a🎮", 3));

  // A terminator inside the range is U+0000, a well-formed one-unit character.
  CHECK(validateUtf16(utf16InnerTerminator, std::size(utf16InnerTerminator)));
}

// Every width converts to the bytes of the matching UTF-8 literal, and the call hands back the buffer it wrote.
TEST_CASE("utf16_to_utf8/encodes_every_width") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];
    const auto length          = std::char_traits<char>::length(utf8);

    std::array<char, 64> buffer{};
    CHECK(utf16ToUtf8(buffer.data(), buffer.size(), utf16, std::char_traits<char16_t>::length(utf16)) == buffer.data());
    CHECK(std::char_traits<char>::length(buffer.data()) == length);
    CHECK(std::char_traits<char>::compare(buffer.data(), utf8, length) == 0);
  }
}

// The unit count bounds the input: a terminator inside it is encoded, and a pair it cuts short ends the call.
TEST_CASE("utf16_to_utf8/reads_only_the_counted_units") {
  std::array<char, 8> inner{sentinel, sentinel, sentinel, sentinel, sentinel};
  utf16ToUtf8(inner.data(), inner.size(), utf16InnerTerminator, std::size(utf16InnerTerminator));
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
  std::array<char, 16> cut{sentinel, sentinel, sentinel, sentinel};
  utf16ToUtf8(cut.data(), cut.size(), u"a🎮", 2);
  CHECK(cut[0] == 'a');
  CHECK(cut[1] == '\0');
}

// A character that does not fit whole stays out, and the terminator always fits.
TEST_CASE("utf16_to_utf8/stops_before_a_character_that_does_not_fit") {
  const auto length = std::char_traits<char>::length(cyrillic);
  const auto units  = std::char_traits<char16_t>::length(u"привет");

  // A buffer of exactly the encoded length plus the terminator holds the whole string.
  std::array<char, 32> exact{};
  utf16ToUtf8(exact.data(), length + 1, u"привет", units);
  CHECK(std::char_traits<char>::length(exact.data()) == length);

  // One byte less leaves out the whole last two-byte character.
  std::array<char, 32> shorter{};
  utf16ToUtf8(shorter.data(), length, u"привет", units);
  CHECK(std::char_traits<char>::length(shorter.data()) == length - 2);
  CHECK(std::char_traits<char>::compare(shorter.data(), cyrillic, length - 2) == 0);

  // ASCII and a three-byte script each take their own branch of the encoder and stop at a full buffer the same way.
  const auto asciiLength = std::char_traits<char>::length(ascii);

  std::array<char, 16> asciiShorter{};
  utf16ToUtf8(asciiShorter.data(), asciiLength, u"player", std::char_traits<char16_t>::length(u"player"));
  CHECK(std::char_traits<char>::length(asciiShorter.data()) == asciiLength - 1);
  CHECK(std::char_traits<char>::compare(asciiShorter.data(), ascii, asciiLength - 1) == 0);

  const auto cjkLength    = std::char_traits<char>::length(cjk);
  const auto cjkCharBytes = cjkLength / utf8Len(cjk);

  std::array<char, 16> cjkShorter{};
  utf16ToUtf8(cjkShorter.data(), cjkLength, u"日本語", std::char_traits<char16_t>::length(u"日本語"));
  CHECK(std::char_traits<char>::length(cjkShorter.data()) == cjkLength - cjkCharBytes);
  CHECK(std::char_traits<char>::compare(cjkShorter.data(), cjk, cjkLength - cjkCharBytes) == 0);

  // A single byte holds the terminator alone, and nothing past the size is written.
  std::array<char, 4> single{sentinel, sentinel, sentinel, sentinel};
  utf16ToUtf8(single.data(), 1, u"привет", units);
  CHECK(single[0] == '\0');
  CHECK(single[1] == sentinel);
}

// A surrogate pair goes in as one four-byte sequence or not at all.
TEST_CASE("utf16_to_utf8/writes_a_surrogate_pair_whole_or_not_at_all") {
  const auto pairBytes = std::char_traits<char>::length("🎮");
  const auto pairUnits = std::char_traits<char16_t>::length(u"🎮");

  std::array<char, 8> pairShort{sentinel, sentinel, sentinel, sentinel};
  utf16ToUtf8(pairShort.data(), pairBytes, u"🎮", pairUnits);
  CHECK(pairShort[0] == '\0');

  std::array<char, 8> pairWhole{};
  utf16ToUtf8(pairWhole.data(), pairBytes + 1, u"🎮", pairUnits);
  CHECK(std::char_traits<char>::compare(pairWhole.data(), "🎮", pairBytes + 1) == 0);
}

// A surrogate outside a pair encodes no code point, so the call keeps what precedes it and stops there.
TEST_CASE("utf16_to_utf8/stops_before_a_lone_surrogate") {
  for (size_t index = 0; index < loneSurrogates.size(); ++index) {
    INFO("index ", index);

    const char16_t * const units = loneSurrogates[index].data();

    std::array<char, 16> buffer{};
    utf16ToUtf8(buffer.data(), buffer.size(), units, std::char_traits<char16_t>::length(units));
    CHECK(std::char_traits<char>::length(buffer.data()) == 1);
    CHECK(buffer[0] == 'a');
  }
}

// A null or empty source converts as an empty string; a buffer that cannot hold the terminator gets no write at all.
TEST_CASE("utf16_to_utf8/handles_null_and_empty_arguments") {
  std::array<char, 4> untouched{sentinel, sentinel, sentinel, sentinel};
  CHECK(utf16ToUtf8(nullptr, untouched.size(), u"player", 1) == nullptr);
  CHECK(utf16ToUtf8(untouched.data(), 0, u"player", 1) == nullptr);
  CHECK(untouched[0] == sentinel);

  std::array<char, 4> fromNull{sentinel, sentinel, sentinel, sentinel};
  CHECK(utf16ToUtf8(fromNull.data(), fromNull.size(), nullptr, 4) == fromNull.data());
  CHECK(fromNull[0] == '\0');

  std::array<char, 4> fromEmpty{sentinel, sentinel, sentinel, sentinel};
  CHECK(utf16ToUtf8(fromEmpty.data(), fromEmpty.size(), u"player", 0) == fromEmpty.data());
  CHECK(fromEmpty[0] == '\0');
}

// Every width measures the byte length of the matching UTF-8 literal.
TEST_CASE("utf16_in_utf8_size/matches_every_width") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];

    CHECK(utf16InUtf8Size(utf16, std::char_traits<char16_t>::length(utf16)) == std::char_traits<char>::length(utf8));
  }
}

// Each width boundary counts on its side: U+007F one byte, U+0080 and U+07FF two, U+0800 three, a pair four.
TEST_CASE("utf16_in_utf8_size/counts_the_width_edges") {
  constexpr char16_t widthEdges[] = {0x7F, 0x80, 0x7FF, 0x800};

  CHECK(utf16InUtf8Size(&widthEdges[0], 1) == 1);
  CHECK(utf16InUtf8Size(&widthEdges[1], 1) == 2);
  CHECK(utf16InUtf8Size(&widthEdges[2], 1) == 2);
  CHECK(utf16InUtf8Size(&widthEdges[3], 1) == 3);
  CHECK(utf16InUtf8Size(utf16BeforeSurrogates, 1) == 3);
  CHECK(utf16InUtf8Size(utf16AfterSurrogates, 1) == 3);
  CHECK(utf16InUtf8Size(utf16LowestPair, 2) == 4);
  CHECK(utf16InUtf8Size(utf16HighestCodePoint, 2) == 4);
}

// The unit count bounds the input: a terminator inside it counts one byte, and a pair it cuts short ends the count.
TEST_CASE("utf16_in_utf8_size/reads_only_the_counted_units") {
  CHECK(utf16InUtf8Size(utf16InnerTerminator, std::size(utf16InnerTerminator)) == 3);
  CHECK(utf16InUtf8Size(u"привет", 3) == std::char_traits<char>::length("при"));
  CHECK(utf16InUtf8Size(u"a🎮", 2) == 1);
}

// A surrogate outside a pair ends the count where toy::utf16ToUtf8() stops encoding.
TEST_CASE("utf16_in_utf8_size/stops_before_a_lone_surrogate") {
  for (size_t index = 0; index < loneSurrogates.size(); ++index) {
    INFO("index ", index);

    const char16_t * const units = loneSurrogates[index].data();
    const auto             count = std::char_traits<char16_t>::length(units);

    std::array<char, 16> buffer{};
    utf16ToUtf8(buffer.data(), buffer.size(), units, count);
    CHECK(utf16InUtf8Size(units, count) == std::char_traits<char>::length(buffer.data()));
    CHECK(utf16InUtf8Size(units, count) == 1);
  }
}

// Over the units a decode wrote, the count gives the bytes the decode consumed before an ill-formed sequence.
TEST_CASE("utf16_in_utf8_size/measures_the_bytes_a_decode_consumed") {
  for (size_t index = 0; index < rejectedSequences.size(); ++index) {
    INFO("index ", index);
    const char * const rejected = rejectedSequences[index];

    std::array<char, 64> input{};
    const auto           prefixLength = std::char_traits<char>::length(mixed);
    std::char_traits<char>::copy(input.data(), mixed, prefixLength);
    std::char_traits<char>::copy(input.data() + prefixLength, rejected, std::char_traits<char>::length(rejected));

    std::array<char16_t, 64> decoded{};
    utf8ToUtf16(decoded.data(), decoded.size(), input.data(), std::char_traits<char>::length(input.data()));
    CHECK(utf16InUtf8Size(decoded.data(), std::char_traits<char16_t>::length(decoded.data())) == prefixLength);
  }
}

// A null or empty source measures nothing, whatever the count.
TEST_CASE("utf16_in_utf8_size/handles_null_and_empty_arguments") {
  CHECK(utf16InUtf8Size(nullptr, 0) == 0);
  CHECK(utf16InUtf8Size(nullptr, 4) == 0);
  CHECK(utf16InUtf8Size(u"player", 0) == 0);
}

// Every width decodes to the matching UTF-16 literal, a pair for four bytes, and the call hands back its buffer.
TEST_CASE("utf8_to_utf16/decodes_every_width") {
  for (size_t index = 0; index < utf16Samples.size(); ++index) {
    INFO("index ", index);
    const auto & [utf16, utf8] = utf16Samples[index];
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
  utf8ToUtf16(inner.data(), inner.size(), innerTerminator, std::size(innerTerminator));
  CHECK(inner[0] == u'a');
  CHECK(inner[1] == u'\0');
  CHECK(inner[2] == u'b');
  CHECK(inner[3] == u'\0');

  // Three bytes of a two-byte script hold one whole character and the lead byte of the next.
  std::array<char16_t, 8> cut{};
  utf8ToUtf16(cut.data(), cut.size(), cyrillic, 3);
  CHECK(std::char_traits<char16_t>::length(cut.data()) == 1);
  CHECK(cut[0] == u'п');
}

// A sequence the validator rejects ends the call, and the characters before it stay.
TEST_CASE("utf8_to_utf16/stops_at_a_sequence_it_cannot_decode") {
  for (size_t index = 0; index < rejectedSequences.size(); ++index) {
    INFO("index ", index);
    const char * const sequence = rejectedSequences[index];

    std::array<char16_t, 4> buffer{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
    utf8ToUtf16(buffer.data(), buffer.size(), sequence, std::char_traits<char>::length(sequence));
    CHECK(buffer[0] == u'\0');
  }

  std::array<char16_t, 4> prefix{};
  utf8ToUtf16(prefix.data(), prefix.size(), truncatedSymbol, std::size(truncatedSymbol) - 1);
  CHECK(prefix[0] == u'a');
  CHECK(prefix[1] == u'\0');
}

// A character that does not fit stays out, and the terminator always fits.
TEST_CASE("utf8_to_utf16/stops_when_the_buffer_is_full") {
  const auto bytes  = std::char_traits<char>::length(cyrillic);
  const auto length = utf8Len(cyrillic);

  // A buffer of exactly the character count plus the terminator holds the whole string.
  std::array<char16_t, 16> exact{};
  utf8ToUtf16(exact.data(), length + 1, cyrillic, bytes);
  CHECK(std::char_traits<char16_t>::compare(exact.data(), u"привет", length) == 0);
  CHECK(exact[length] == u'\0');

  // One element less leaves out the last character.
  std::array<char16_t, 16> shorter{};
  utf8ToUtf16(shorter.data(), length, cyrillic, bytes);
  CHECK(std::char_traits<char16_t>::length(shorter.data()) == length - 1);
  CHECK(std::char_traits<char16_t>::compare(shorter.data(), u"привет", length - 1) == 0);

  // ASCII takes a shorter path through the decoder and stops at a full buffer the same way.
  const auto asciiLength = std::char_traits<char>::length(ascii);

  std::array<char16_t, 16> asciiShorter{};
  utf8ToUtf16(asciiShorter.data(), asciiLength, ascii, asciiLength);
  CHECK(std::char_traits<char16_t>::length(asciiShorter.data()) == asciiLength - 1);
  CHECK(std::char_traits<char16_t>::compare(asciiShorter.data(), u"player", asciiLength - 1) == 0);

  // A single element holds the terminator alone, and nothing past the size is written.
  std::array<char16_t, 4> single{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  utf8ToUtf16(single.data(), 1, cyrillic, bytes);
  CHECK(single[0] == u'\0');
  CHECK(single[1] == utf16Sentinel);
}

// A four-byte character goes in as a whole surrogate pair or not at all.
TEST_CASE("utf8_to_utf16/writes_a_surrogate_pair_whole_or_not_at_all") {
  const auto pairBytes = std::char_traits<char>::length("🎮");
  const auto pairUnits = std::char_traits<char16_t>::length(u"🎮");

  std::array<char16_t, 4> pairShort{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  utf8ToUtf16(pairShort.data(), pairUnits, "🎮", pairBytes);
  CHECK(pairShort[0] == u'\0');

  std::array<char16_t, 4> pairWhole{};
  utf8ToUtf16(pairWhole.data(), pairUnits + 1, "🎮", pairBytes);
  CHECK(std::char_traits<char16_t>::compare(pairWhole.data(), u"🎮", pairUnits + 1) == 0);
}

// A null or empty source decodes as an empty string; a buffer that cannot hold the terminator gets no write at all.
TEST_CASE("utf8_to_utf16/handles_null_and_empty_arguments") {
  std::array<char16_t, 4> untouched{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  CHECK(utf8ToUtf16(nullptr, untouched.size(), ascii, 1) == nullptr);
  CHECK(utf8ToUtf16(untouched.data(), 0, ascii, 1) == nullptr);
  CHECK(untouched[0] == utf16Sentinel);

  std::array<char16_t, 4> fromNull{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  CHECK(utf8ToUtf16(fromNull.data(), fromNull.size(), nullptr, 4) == fromNull.data());
  CHECK(fromNull[0] == u'\0');

  std::array<char16_t, 4> fromEmpty{utf16Sentinel, utf16Sentinel, utf16Sentinel, utf16Sentinel};
  CHECK(utf8ToUtf16(fromEmpty.data(), fromEmpty.size(), ascii, 0) == fromEmpty.data());
  CHECK(fromEmpty[0] == u'\0');
}

} // namespace toy
