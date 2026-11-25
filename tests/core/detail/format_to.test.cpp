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
  \file   format_to.test.cpp
  \brief  Unit tests for toy::detail::formatTo() and the per-type formatters it dispatches to.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// A constant expression cannot cast the context back to the sink, so the constexpr probes run with a null-context
// callback and verify the walk through the byte count detail::formatTo returns.
constexpr void sinkWrite(void *, span<const char>) noexcept {}

// Records what the walk writes, so a runtime case can read the output back as a string.
struct RecordSink {
  char   data[64];
  size_t length = 0;
};

constexpr void recordWrite(void * context, span<const char> data) noexcept {
  auto & sink = *static_cast<RecordSink *>(context);
  for (const char byte : data)
    sink.data[sink.length++] = byte;
}

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

// Walks the pattern through a null-context sink and returns the byte count detail::formatTo reports.
constexpr size_t written(StringView pattern, span<const detail::FormatArg> args = {}) {
  detail::FormatContext output{nullptr, &sinkWrite};
  return detail::formatTo(output, pattern, args);
}

template <typename... Args>
  requires((std::same_as<Args, detail::FormatArg> && ...))
constexpr size_t written(StringView pattern, const Args &... args) {
  if constexpr (sizeof...(Args) == 0)
    return written(pattern, span<const detail::FormatArg>{});
  else {
    const detail::FormatArg pack[] = {args...};
    return written(pattern, span<const detail::FormatArg>{pack});
  }
}

// Walks the pattern over a fresh recording sink and returns the output as a fixed string; runtime only.
FixedString<64> recorded(StringView pattern, span<const detail::FormatArg> args = {}) {
  RecordSink            sink;
  detail::FormatContext output{&sink, &recordWrite};
  detail::formatTo(output, pattern, args);
  return FixedString<64>(sink.data, sink.length);
}

template <typename... Args>
  requires((std::same_as<Args, detail::FormatArg> && ...))
FixedString<64> recorded(StringView pattern, const Args &... args) {
  if constexpr (sizeof...(Args) == 0)
    return recorded(pattern, span<const detail::FormatArg>{});
  else {
    const detail::FormatArg pack[] = {args...};
    return recorded(pattern, span<const detail::FormatArg>{pack});
  }
}

} // namespace

// Literals pass through and a doubled brace collapses to one brace in the output.
TEST_CASE("format_to/walk/literals_and_escapes") {
  static_assert(written("hi") == 2, "literals pass through");
  static_assert(written("") == 0, "an empty pattern writes nothing");
  static_assert(written("a{{b}}c") == 5, "doubled braces collapse to single braces");
  CHECK(recorded("a{{b}}c") == "a{b}c");
}

// Auto placeholders consume in order, positional ones index the pack, a repeat and an extra argument ride along.
TEST_CASE("format_to/walk/auto_and_positional") {
  static_assert(written("{}-{}", detail::packArg('a'), detail::packArg(true)) == 6,
                "auto placeholders consume in order");
  static_assert(written("{1}-{0}", detail::packArg('a'), detail::packArg(true)) == 6,
                "positional placeholders index the pack");
  static_assert(written("{0}{0}", detail::packArg('x')) == 2, "a positional placeholder may repeat");
  static_assert(written("{0}", detail::packArg('a'), detail::packArg(true)) == 1,
                "an argument without a placeholder rides along unused");
  CHECK(recorded("{}-{}", detail::packArg('a'), detail::packArg(true)) == "a-true");
  CHECK(recorded("{1}-{0}", detail::packArg('a'), detail::packArg(true)) == "true-a");
  CHECK(recorded("{0}{0}", detail::packArg('x')) == "xx");
}

// Every tag dispatches to its formatter and renders the dictionary the engine fixes.
TEST_CASE("format_to/dispatch/char_and_bool") {
  static_assert(written("{}", detail::packArg(true)) == 4, "true renders as a word");
  static_assert(written("{}", detail::packArg(false)) == 5, "false renders as a word");
  static_assert(written("{}", detail::packArg('x')) == 1, "a character renders itself");
  CHECK(recorded("{}", detail::packArg(true)) == "true");
  CHECK(recorded("{}", detail::packArg(false)) == "false");
  CHECK(recorded("{}", detail::packArg('x')) == "x");
}

// Debug reports a pattern invalid for the pack and continues; release handles the same garbage deterministically.
TEST_CASE("format_to/walk/release_semantics") {
  assertionReports = 0;
  toy::assertion::initialize();
  toy::assertion::setCallbacks(countReports, nullptr);

  // A stray closing brace writes as a literal.
  CHECK(recorded("a}", {}) == "a}");
  // An out-of-range index writes nothing.
  CHECK(recorded("{9}", detail::packArg('a')) == "");
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

// Runtime walks produce the same counts and strings the constant expressions above assert.
TEST_CASE("format_to/walk/constexpr_parity") {
  CHECK(written("hi") == 2);
  CHECK(written("a{{b}}c") == 5);
  CHECK(written("{}-{}", detail::packArg('a'), detail::packArg(true)) == 6);
  CHECK(written("{1}-{0}", detail::packArg('a'), detail::packArg(true)) == 6);
  CHECK(written("{0}{0}", detail::packArg('x')) == 2);
  CHECK(written("{0}", detail::packArg('a'), detail::packArg(true)) == 1);
  CHECK(recorded("{}", detail::packArg(true)) == "true");
  CHECK(recorded("{}", detail::packArg(false)) == "false");
  CHECK(recorded("{}", detail::packArg('x')) == "x");

  // A real recording sink receives exactly the byte count the return reports.
  RecordSink              recording;
  detail::FormatContext   recordingOutput{&recording, &recordWrite};
  const detail::FormatArg pack[] = {detail::packArg('a'), detail::packArg(true)};
  CHECK(detail::formatTo(recordingOutput, "{}-{}", span<const detail::FormatArg>{pack}) == recording.length);
  CHECK(recording.length == 6);
}

// The grammar helpers moved to the shared detail header keep every validator verdict unchanged.
TEST_CASE("format_to/pattern_detail/validator_unchanged") {
  static_assert(detail::validateFormatPattern("a{{b}}c", 0) == detail::FormatPatternValidationError::none,
                "escapes stay valid");
  static_assert(detail::validateFormatPattern("{}", 1) == detail::FormatPatternValidationError::none,
                "an auto placeholder fits one argument");
  static_assert(detail::validateFormatPattern("{9}", 1) == detail::FormatPatternValidationError::indexOutOfRange,
                "an out-of-range index reports");
  static_assert(detail::validateFormatPattern("a}", 0) == detail::FormatPatternValidationError::unmatchedBrace,
                "a stray closing brace reports");
}

} // namespace toy
