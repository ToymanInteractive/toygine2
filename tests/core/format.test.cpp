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

// Reports counted by the cases that own the assertion seam; the callback itself ignores the failure.
int assertionReports = 0;

toy::assertion::Action countReports(const char *) noexcept {
  ++assertionReports;
  return toy::assertion::Action::Ignore;
}

#ifdef TOYGINE_TESTS_USE_DOCTEST

// The doctest main registers a failure reporter the seam cases replace; this restores the same behavior after them.
toy::assertion::Action reportFailure(const char * message) noexcept {
  ADD_FAIL_CHECK_AT(__FILE__, __LINE__, message);
  return toy::assertion::Action::Ignore;
}

#endif // TOYGINE_TESTS_USE_DOCTEST

} // namespace

// The two supported argument kinds render through the fixed string at compile time.
TEST_CASE("format/constexpr_cases") {
  static_assert(format<16>("x={}", 'a') == "x=a", "constexpr format fills the fixed string");
  static_assert(format<16>("{}-{}", true, false) == "true-false", "auto placeholders bind in order");
  static_assert(format<8>("hi") == "hi", "a pattern without placeholders binds none");
  static_assert(format<16>("{1} then {0}", 'x', true) == "true then x", "positional placeholders index the pack");
  static_assert(format<8>("{}", true) == "true", "true renders as a word");
  static_assert(format<8>("{}", false) == "false", "false renders as a word");
  static_assert(format<8>("{}", 'x') == "x", "a character renders itself");
  static_assert(format<8>("a{{b}}c") == "a{b}c", "doubled braces collapse to single braces");
}

// A runtime pattern walks the same engine and produces the strings the constant expressions above assert.
TEST_CASE("format/runtime_vformat") {
  CHECK(vformat<16>(StringView{"x={}"}, 'a') == "x=a");
  CHECK(vformat<16>(StringView{"{}-{}"}, true, false) == "true-false");
  CHECK(vformat<8>(StringView{"hi"}) == "hi");
  CHECK(vformat<8>(StringView{"{}"}, true) == "true");
  CHECK(vformat<8>(StringView{"{}"}, 'x') == "x");
}

// Output past the capacity clamps byte by byte; a UTF-8 sequence may cut mid-character, which is the documented
// byte-cut contract.
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

// BufferSize 1 gives capacity 0: every write clamps to nothing and the result stays empty.
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
