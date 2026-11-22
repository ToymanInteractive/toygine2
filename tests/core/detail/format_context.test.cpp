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
  \file   format_context.test.cpp
  \brief  Unit tests for \ref toy::detail::FormatContext.
*/

#include <cstring>

#include "core.hpp"
#include "toy_test.hpp"

namespace toy::detail {

namespace {

// Records what every invocation saw; the sink's own address is the context, so the checks verify the callback's side
// of each call.
struct RecordingSink {
  const void * contextSeen = nullptr;
  const char * dataSeen    = nullptr;
  char         data[4]     = {};
  // Exact size of the case's input; the recorder reads no further, so an inflated count fails the count check without
  // an over-read.
  size_t       copyLimit   = 0;
  size_t       count       = 0;
  int          calls       = 0;
};

// Copies a bounded sample of what the forwarded view named during the call, and records the arguments as given.
void recordWrite(void * context, span<const char> data) noexcept {
  auto & sink      = *static_cast<RecordingSink *>(context);
  sink.contextSeen = context;
  sink.dataSeen    = data.data();
  sink.count       = data.size();
  for (size_t i = 0; i < data.size() && i < sink.copyLimit && i < 4; ++i)
    sink.data[i] = data[i];
  ++sink.calls;
}

// Records into a recorder whose address travels in the payload bytes; a null context leaves the callback nothing to
// write through.
struct NullContextRecord {
  const void * contextSeen = nullptr;
  int          calls       = 0;
};

void recordNullContext(void * context, span<const char> data) noexcept {
  NullContextRecord * recorder = nullptr;
  // A mismatched payload reads nothing; calls stays at zero, and the case fails instead of over-reading.
  if (data.size() != sizeof(recorder))
    return;

  std::memcpy(&recorder, data.data(), sizeof(recorder));
  recorder->contextSeen = context;
  ++recorder->calls;
}

// The usage example from the FormatContext class block, lifted into a case so the build catches an example the API
// left behind; keep both in sync.
struct Sink {
  char   buffer[16];
  size_t length = 0;
};

// Writes the view only when it fits whole.
void append(void * context, span<const char> data) noexcept {
  auto & sink = *static_cast<Sink *>(context);
  if (sink.length + data.size() > 16)
    return;
  for (const char character : data)
    sink.buffer[sink.length++] = character;
}

// Returns false when the sink did not take the whole greeting.
bool writeGreeting(Sink & sink) {
  const size_t               start = sink.length;
  toy::detail::FormatContext output{&sink, &append};
  output.write(span<const char>{"hi", 2});
  output.put('!');
  return sink.length - start == 3;
}

// A callback constant evaluation accepts, for the compile-time probe below.
constexpr void noopWrite(void *, span<const char>) noexcept {}

// Builds the object in a constant expression; [[maybe_unused]] because the build itself is the assertion.
constexpr bool constructsInConstantExpression() noexcept {
  [[maybe_unused]] FormatContext context{nullptr, noopWrite};
  return true;
}

} // namespace

// The type stays two plain pointers: free to copy, nothing to destroy, and buildable in a constant expression.
TEST_CASE("detail/format_context/traits") {
  static_assert(std::is_trivially_copyable_v<FormatContext>, "the object copies as plain data");
  static_assert(std::is_trivially_destructible_v<FormatContext>, "two pointers own nothing to destroy");
  static_assert(std::is_nothrow_copy_constructible_v<FormatContext>, "copying two pointers cannot fail");
  static_assert(std::is_nothrow_move_constructible_v<FormatContext>, "moving the type is copying it");
  static_assert(noexcept(FormatContext{nullptr, noopWrite}), "construction cannot fail");
  static_assert(constructsInConstantExpression(), "the constructor runs in a constant expression");
  static_assert(std::is_same_v<FormatContext::WriteFunction, void (*)(void *, span<const char>) noexcept>,
                "the callback signature carries noexcept");
}

// write() hands the callback exactly what it got: the constructor's context and the caller's view.
TEST_CASE("detail/format_context/write_forwards_to_the_callback") {
  RecordingSink sink;
  FormatContext context{&sink, &recordWrite};

  // An embedded zero separates the view from a C string: a length taken from the string would drop the tail.
  const char input[] = {'h', '\0', 'i'};
  sink.copyLimit     = 3;
  context.write(span{input});
  CHECK(sink.calls == 1);
  CHECK(sink.contextSeen == &sink);
  CHECK(sink.dataSeen == input);
  CHECK(sink.count == 3);
  CHECK(sink.data[0] == 'h');
  CHECK(sink.data[1] == '\0');
  CHECK(sink.data[2] == 'i');

  const char single[] = "!";
  sink.copyLimit      = 1;
  context.write(span<const char>{single, 1});
  CHECK(sink.calls == 2);
  CHECK(sink.dataSeen == single);
  CHECK(sink.count == 1);
  CHECK(sink.data[0] == '!');
}

// An empty view returns before the callback, so even a null pointer is a legal call.
TEST_CASE("detail/format_context/write_with_zero_count_skips_the_callback") {
  RecordingSink sink;
  FormatContext context{&sink, &recordWrite};

  context.write(span<const char>{});
  context.write(span<const char>{"x", 0});
  CHECK(sink.calls == 0);
}

// put() forwards one character: the callback sees a one-character view and reads the character through it.
TEST_CASE("detail/format_context/put_writes_one_character") {
  RecordingSink sink;
  FormatContext context{&sink, &recordWrite};

  sink.copyLimit = 1;
  context.put('x');
  CHECK(sink.calls == 1);
  CHECK(sink.count == 1);
  CHECK(sink.data[0] == 'x');

  context.put('y');
  CHECK(sink.calls == 2);
  CHECK(sink.count == 1);
  CHECK(sink.data[0] == 'y');
}

// A copy carries the same pair, so it writes into the same sink through the same callback.
TEST_CASE("detail/format_context/copy_writes_through_the_same_pair") {
  RecordingSink sink;
  FormatContext original{&sink, &recordWrite};

  FormatContext copied{original};
  const char    input[] = "z";
  sink.copyLimit        = 1;
  copied.write(span<const char>{input, 1});
  CHECK(sink.calls == 1);
  CHECK(sink.contextSeen == &sink);
  CHECK(sink.dataSeen == input);
  CHECK(sink.count == 1);
  CHECK(sink.data[0] == 'z');
}

// A null context is legal: construction accepts it and the callback receives it as passed.
TEST_CASE("detail/format_context/null_context_reaches_the_callback") {
  NullContextRecord record;
  FormatContext     context{nullptr, &recordNullContext};

  NullContextRecord * recorder = &record;
  context.write(span<const char>{static_cast<const char *>(static_cast<const void *>(&recorder)), sizeof(recorder)});
  CHECK(record.calls == 1);
  CHECK(record.contextSeen == nullptr);
}

// Both outcomes of the documented example: the greeting lands in an empty sink, and a one-byte-short sink takes only
// part of it.
TEST_CASE("detail/format_context/usage_example_greeting") {
  Sink empty;
  CHECK(writeGreeting(empty));
  REQUIRE(empty.length == 3);
  CHECK(empty.buffer[0] == 'h');
  CHECK(empty.buffer[1] == 'i');
  CHECK(empty.buffer[2] == '!');

  Sink nearlyFull;
  nearlyFull.length = 14;
  CHECK(!writeGreeting(nearlyFull));
  REQUIRE(nearlyFull.length == 16);
  CHECK(nearlyFull.buffer[14] == 'h');
  CHECK(nearlyFull.buffer[15] == 'i');
}

} // namespace toy::detail
