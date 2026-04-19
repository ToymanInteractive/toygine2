//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \brief  Unit tests for \ref toy::FormatContext.
*/

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

struct CallTracker {
  size_t callCount  = 0;
  size_t totalBytes = 0;
};

void fixedStringCallback(void * context, const char * data, size_t count) noexcept {
  static_cast<FixedString<128> *>(context)->append(data, count);
}

void trackingCallback(void * context, const char * data, size_t count) noexcept {
  assert_message(data != nullptr, "data must not be null");

  auto * tracker = static_cast<CallTracker *>(context);
  ++tracker->callCount;
  tracker->totalBytes += count;
}

// Object layout.
TEST_CASE("core/format_context/object_structure") {
  static_assert(sizeof(FormatContext) == 2 * sizeof(void *), "FormatContext must be two-pointer sized");

  static_assert(!std::is_trivial_v<FormatContext>, "FormatContext must not be trivial");
  static_assert(std::is_trivially_copyable_v<FormatContext>, "FormatContext must be trivially copyable");
  static_assert(std::is_standard_layout_v<FormatContext>, "FormatContext must have standard layout");
}

// Constructor is constexpr with valid callback.
TEST_CASE("core/format_context/construction") {
  constexpr FormatContext ctx{nullptr, trackingCallback};

  REQUIRE(sizeof(ctx) > 0);

  static_assert(sizeof(ctx) > 0, "constexpr FormatContext must be constructible");
}

// write() delegates to the stored callback.
TEST_CASE("core/format_context/write") {
  FixedString<128> buf;
  FormatContext    ctx{static_cast<void *>(&buf), fixedStringCallback};

  // Single write appends the data verbatim.
  SUBCASE("single_write") {
    ctx.write("hello", 5);

    REQUIRE(buf.size() == std::char_traits<char>::length("hello"));
    REQUIRE(buf == "hello");
  }

  // Consecutive writes accumulate into the output.
  SUBCASE("multiple_writes") {
    ctx.write("foo", 3);
    ctx.write("bar", 3);

    REQUIRE(buf.size() == std::char_traits<char>::length("foobar"));
    REQUIRE(buf == "foobar");
  }

  // Partial write uses only the first count bytes from data.
  SUBCASE("partial_data") {
    ctx.write("abcdef", 3);

    REQUIRE(buf.size() == std::char_traits<char>::length("abc"));
    REQUIRE(buf == "abc");
  }

  // write() with count == 0 does not invoke the callback.
  SUBCASE("zero_count_is_noop") {
    CallTracker   tracker;
    FormatContext trackCtx{static_cast<void *>(&tracker), trackingCallback};

    trackCtx.write("data", 0);

    REQUIRE(tracker.callCount == 0);
    REQUIRE(tracker.totalBytes == 0);
  }
}

// put() appends a single character through the callback.
TEST_CASE("core/format_context/put") {
  FixedString<128> buf;
  FormatContext    ctx{&buf, fixedStringCallback};

  // Single put appends one character.
  SUBCASE("single_put") {
    ctx.put('A');

    REQUIRE(buf.size() == 1);
    REQUIRE(buf[0] == 'A');
  }

  // Consecutive puts accumulate characters.
  SUBCASE("multiple_puts") {
    ctx.put('X');
    ctx.put('Y');
    ctx.put('Z');

    REQUIRE(buf.size() == std::char_traits<char>::length("XYZ"));
    REQUIRE(buf == "XYZ");
  }
}

// Interleaved write() and put() calls accumulate in order.
TEST_CASE("core/format_context/mixed_operations") {
  FixedString<128> buf;
  FormatContext    ctx{&buf, fixedStringCallback};

  ctx.write("ab", 2);
  ctx.put('c');
  ctx.write("de", 2);
  ctx.put('f');

  REQUIRE(buf.size() == std::char_traits<char>::length("abcdef"));
  REQUIRE(buf == "abcdef");
}

// The same FormatContext interface works with unrelated backing storage.
TEST_CASE("core/format_context/type_erasure") {
  struct RawBuffer {
    char   data[64] = {};
    size_t length   = 0;
  };

  RawBuffer     raw;
  FormatContext ctx{static_cast<void *>(&raw), [](void * context, const char * data, size_t count) noexcept {
                      auto * b = static_cast<RawBuffer *>(context);
                      std::memcpy(b->data + b->length, data, count);
                      b->length          += count;
                      b->data[b->length]  = '\0';
                    }};

  ctx.write("hello", 5);
  ctx.put(' ');
  ctx.write("world", 5);

  REQUIRE(raw.length == std::char_traits<char>::length("hello world"));
  REQUIRE(std::strcmp(raw.data, "hello world") == 0);
}

} // namespace toy
