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

namespace {

// The callback counts assertion reports and ignores each failure.
int assertionReports = 0;

toy::assertion::Action countReports(const char *) noexcept {
  ++assertionReports;
  return toy::assertion::Action::Ignore;
}

#ifdef TOYGINE_TESTS_USE_DOCTEST

// Restores the doctest failure reporter after the assertion seam cases replace it.
toy::assertion::Action reportFailure(const char * message) noexcept {
  ADD_FAIL_CHECK_AT(__FILE__, __LINE__, message);
  return toy::assertion::Action::Ignore;
}

#endif // TOYGINE_TESTS_USE_DOCTEST

} // namespace

// A pattern without placeholders returns its literal text.
TEST_CASE("format/literals") {
  CHECK(format<8>("hi") == "hi");
  CHECK(vformat<8>(StringView{"hi"}) == "hi");

  static_assert(format<8>("hi") == "hi", "a pattern without placeholders returns its literals");
}

// Auto placeholders bind arguments in order and render characters and booleans.
TEST_CASE("format/auto_placeholders") {
  CHECK(format<16>("x={}", 'a') == "x=a");
  CHECK(vformat<16>(StringView{"x={}"}, 'a') == "x=a");
  CHECK(format<16>("{}-{}", true, false) == "true-false");
  CHECK(vformat<16>(StringView{"{}-{}"}, true, false) == "true-false");

  static_assert(format<16>("x={}", 'a') == "x=a", "constexpr format renders the character argument");
  static_assert(format<16>("{}-{}", true, false) == "true-false", "auto placeholders bind in order");
}

// Positional placeholders select arguments by index.
TEST_CASE("format/positional_placeholders") {
  CHECK(format<16>("{1} then {0}", 'x', true) == "true then x");
  CHECK(vformat<16>(StringView{"{1} then {0}"}, 'x', true) == "true then x");

  static_assert(format<16>("{1} then {0}", 'x', true) == "true then x", "positional placeholders index the pack");
}

// Doubled braces collapse to single braces in the result.
TEST_CASE("format/escaped_braces") {
  CHECK(format<8>("a{{b}}c") == "a{b}c");
  CHECK(vformat<8>(StringView{"a{{b}}c"}) == "a{b}c");

  static_assert(format<8>("a{{b}}c") == "a{b}c", "doubled braces collapse to single braces");
}

// A runtime pattern may end with an opening brace, which stays literal.
TEST_CASE("format/invalid_pattern") {
  assertionReports = 0;
  toy::assertion::initialize();
  toy::assertion::setCallbacks(countReports, nullptr);

  CHECK(vformat<8>(StringView{"a{"}) == "a{");
#ifdef TOYGINE_ASSERTIONS_ENABLED
  CHECK(assertionReports == 1);
#else
  CHECK(assertionReports == 0);
#endif

  toy::assertion::setCallbacks(nullptr, nullptr);
  toy::assertion::deInitialize();
#ifdef TOYGINE_TESTS_USE_DOCTEST
  toy::assertion::initialize();
  toy::assertion::setCallbacks(reportFailure, nullptr);
#endif
}

// Output truncates byte by byte at capacity, so a cut may split a UTF-8 sequence.
TEST_CASE("format/truncation") {
  assertionReports = 0;
  toy::assertion::initialize();
  toy::assertion::setCallbacks(countReports, nullptr);

  CHECK(format<4>("hello world") == "hel");
  CHECK(format<8>("привет") == "при\x{D0}");
#ifdef TOYGINE_ASSERTIONS_ENABLED
  CHECK(assertionReports == 2);
#else
  CHECK(assertionReports == 0);
#endif

  toy::assertion::setCallbacks(nullptr, nullptr);
  toy::assertion::deInitialize();
#ifdef TOYGINE_TESTS_USE_DOCTEST
  toy::assertion::initialize();
  toy::assertion::setCallbacks(reportFailure, nullptr);
#endif
}

// BufferSize 1 leaves no output capacity, so writes truncate and the result stays empty.
TEST_CASE("format/degenerate_capacity") {
  assertionReports = 0;
  toy::assertion::initialize();
  toy::assertion::setCallbacks(countReports, nullptr);

  CHECK(format<1>("hi") == "");
#ifdef TOYGINE_ASSERTIONS_ENABLED
  CHECK(assertionReports == 1);
#else
  CHECK(assertionReports == 0);
#endif

  toy::assertion::setCallbacks(nullptr, nullptr);
  toy::assertion::deInitialize();
#ifdef TOYGINE_TESTS_USE_DOCTEST
  toy::assertion::initialize();
  toy::assertion::setCallbacks(reportFailure, nullptr);
#endif
}

} // namespace toy
