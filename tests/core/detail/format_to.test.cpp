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

// Constexpr probes cannot recover the sink from the context, so they use a null-context callback and check the returned
// byte count.
constexpr void sinkWrite(void *, span<const char>) noexcept {}

// Records walk output for runtime string comparisons.
struct RecordSink {
  char   data[64];
  size_t length = 0;
};

constexpr void recordWrite(void * context, span<const char> data) noexcept {
  auto & sink = *static_cast<RecordSink *>(context);
  for (const char byte : data)
    sink.data[sink.length++] = byte;
}

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

// Returns the byte count reported by detail::formatTo() with a null-context sink.
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

// Returns runtime walk output from a fresh recording sink as a fixed string.
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

// Literal text passes through unchanged, and an empty pattern writes nothing.
TEST_CASE("format_to/walk/literals") {
  CHECK(recorded("hi") == "hi");
  CHECK(recorded("") == "");
  CHECK(written("hi") == 2);
  CHECK(written("") == 0);

  static_assert(written("hi") == 2, "literals pass through");
  static_assert(written("") == 0, "an empty pattern writes nothing");
}

// A doubled brace collapses to one brace in the output.
TEST_CASE("format_to/walk/escapes") {
  CHECK(recorded("a{{b}}c") == "a{b}c");
  CHECK(written("a{{b}}c") == 5);

  static_assert(written("a{{b}}c") == 5, "doubled braces collapse to single braces");
}

// Auto placeholders consume arguments in order.
TEST_CASE("format_to/walk/auto_placeholders") {
  CHECK(recorded("{}-{}", detail::packArg('a'), detail::packArg(true)) == "a-true");
  CHECK(written("{}-{}", detail::packArg('a'), detail::packArg(true)) == 6);

  static_assert(written("{}-{}", detail::packArg('a'), detail::packArg(true)) == 6,
                "auto placeholders consume in order");
}

// Positional placeholders index the pack, may repeat an index, and leave unused arguments alone.
TEST_CASE("format_to/walk/positional_placeholders") {
  CHECK(recorded("{1}-{0}", detail::packArg('a'), detail::packArg(true)) == "true-a");
  CHECK(written("{1}-{0}", detail::packArg('a'), detail::packArg(true)) == 6);
  CHECK(recorded("{0}{0}", detail::packArg('x')) == "xx");
  CHECK(written("{0}{0}", detail::packArg('x')) == 2);
  CHECK(recorded("{0}", detail::packArg('a'), detail::packArg(true)) == "a");
  CHECK(written("{0}", detail::packArg('a'), detail::packArg(true)) == 1);

  static_assert(written("{1}-{0}", detail::packArg('a'), detail::packArg(true)) == 6,
                "positional placeholders index the pack");
  static_assert(written("{0}{0}", detail::packArg('x')) == 2, "a positional placeholder may repeat");
  static_assert(written("{0}", detail::packArg('a'), detail::packArg(true)) == 1,
                "an argument without a placeholder is ignored");
}

// Each argument tag selects the formatter for its fixed representation.
TEST_CASE("format_to/dispatch/char_and_bool") {
  CHECK(recorded("{}", detail::packArg(true)) == "true");
  CHECK(recorded("{}", detail::packArg(false)) == "false");
  CHECK(recorded("{}", detail::packArg('x')) == "x");
  CHECK(written("{}", detail::packArg(true)) == 4);
  CHECK(written("{}", detail::packArg(false)) == 5);
  CHECK(written("{}", detail::packArg('x')) == 1);

  static_assert(written("{}", detail::packArg(true)) == 4, "true renders as a word");
  static_assert(written("{}", detail::packArg(false)) == 5, "false renders as a word");
  static_assert(written("{}", detail::packArg('x')) == 1, "a character renders itself");
}

// Assertions report invalid patterns; stray braces stay literal and invalid placeholders write nothing.
TEST_CASE("format_to/walk/invalid_patterns") {
  assertionReports = 0;
  toy::assertion::initialize();
  toy::assertion::setCallbacks(countReports, nullptr);

  // A stray closing brace writes as a literal.
  CHECK(recorded("a}", {}) == "a}");
  // A trailing opening brace writes as a literal and counts toward the returned byte count.
  RecordSink            trailingBrace;
  detail::FormatContext trailingBraceOutput{&trailingBrace, &recordWrite};
  const size_t          trailingBraceBytes = detail::formatTo(trailingBraceOutput, "a{", {});
  CHECK(FixedString<64>(trailingBrace.data, trailingBrace.length) == "a{");
  CHECK(trailingBraceBytes == 2);
  // An out-of-range index writes nothing.
  CHECK(recorded("{9}", detail::packArg('a')) == "");
#ifdef TOYGINE_ASSERTIONS_ENABLED
  CHECK(assertionReports == 3);
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

// The returned byte count matches the number of bytes delivered to the sink.
TEST_CASE("format_to/walk/returned_byte_count") {
  RecordSink              recording;
  detail::FormatContext   recordingOutput{&recording, &recordWrite};
  const detail::FormatArg pack[] = {detail::packArg('a'), detail::packArg(true)};
  const size_t writtenBytes      = detail::formatTo(recordingOutput, "{}-{}", span<const detail::FormatArg>{pack});

  CHECK(writtenBytes == recording.length);
  CHECK(recording.length == 6);
}

} // namespace toy
