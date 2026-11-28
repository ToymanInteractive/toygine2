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
  \file   format.test.cpp
  \brief  Unit tests for toy::format() and toy::vformat().
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

// A pattern without placeholders returns its literal text.
TEST_CASE("format/literals") {
  CHECK(format<8>("hi") == "hi");
  CHECK(vformat<8>(StringView{"hi"}) == "hi");

  static_assert(format<8>("hi") == "hi", "a pattern without placeholders returns its literals");
  static_assert(vformat<8>(StringView{"hi"}) == "hi", "constexpr vformat returns literal text");
}

// Auto placeholders bind arguments in order and render characters and booleans.
TEST_CASE("format/auto_placeholders") {
  CHECK(format<16>("x={}", 'a') == "x=a");
  CHECK(vformat<16>(StringView{"x={}"}, 'a') == "x=a");
  CHECK(format<16>("{}-{}", true, false) == "true-false");
  CHECK(vformat<16>(StringView{"{}-{}"}, true, false) == "true-false");

  static_assert(format<16>("x={}", 'a') == "x=a", "constexpr format renders the character argument");
  static_assert(format<16>("{}-{}", true, false) == "true-false", "auto placeholders bind in order");
  static_assert(vformat<16>(StringView{"x={}"}, 'a') == "x=a", "constexpr vformat renders the character argument");
  static_assert(vformat<16>(StringView{"{}-{}"}, true, false) == "true-false",
                "constexpr vformat binds auto placeholders in order");
}

// Positional placeholders select arguments by index.
TEST_CASE("format/positional_placeholders") {
  CHECK(format<16>("{1} then {0}", 'x', true) == "true then x");
  CHECK(vformat<16>(StringView{"{1} then {0}"}, 'x', true) == "true then x");

  static_assert(format<16>("{1} then {0}", 'x', true) == "true then x", "positional placeholders index the pack");
  static_assert(vformat<16>(StringView{"{1} then {0}"}, 'x', true) == "true then x",
                "constexpr vformat indexes positional placeholders");
}

// Doubled braces collapse to single braces in the result.
TEST_CASE("format/escaped_braces") {
  CHECK(format<8>("a{{b}}c") == "a{b}c");
  CHECK(vformat<8>(StringView{"a{{b}}c"}) == "a{b}c");

  static_assert(format<8>("a{{b}}c") == "a{b}c", "doubled braces collapse to single braces");
  static_assert(vformat<8>(StringView{"a{{b}}c"}) == "a{b}c", "constexpr vformat collapses doubled braces");
}

} // namespace toy
