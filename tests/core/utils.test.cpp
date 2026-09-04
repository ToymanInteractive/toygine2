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

} // namespace

// How many characters a UTF-8 string holds, against the bytes it occupies.
TEST_CASE("utf8_len/counts_characters_not_bytes") {
  CHECK(utf8Len(c_ascii) == char_traits<char>::length(c_ascii));
  CHECK(utf8Len(c_cyrillic) == char_traits<char>::length(c_cyrillic) / 2);
  CHECK(utf8Len(c_cjk) == char_traits<char>::length(c_cjk) / 3);
  CHECK(utf8Len(c_emoji) == char_traits<char>::length(c_emoji) / 4);

  // A mixed string counts every character once, whatever width each one is stored in.
  CHECK(utf8Len(c_mixed) == utf8Len(c_ascii) + utf8Len(c_cyrillic) + utf8Len(c_cjk) + utf8Len(c_emoji) + 3);

  CHECK(utf8Len("") == 0);
}

} // namespace toy
