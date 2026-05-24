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
  \file   log_backend.test.cpp
  \brief  Unit tests for \ref toy::log::Backend.
*/

#include <vector>

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy::log {

namespace {

constexpr Metadata c_meta{"value: {}", "log_backend.test.cpp", 1, Level::Info};
constexpr Metadata c_metaNoArgs{"hello", "log_backend.test.cpp", 2, Level::Info};
constexpr Metadata c_metaThreeArgs{"{} + {} = {}", "log_backend.test.cpp", 3, Level::Info};

class RecordingSink final : public ISink {
public:
  RecordingSink() noexcept           = default;
  ~RecordingSink() noexcept override = default;

  TOYGINE_NO_COPY_MOVE(RecordingSink);

  void write(const Record & record) noexcept override {
    metas.push_back(record.meta);
    timestamps.push_back(record.timestamp);
    messages.emplace_back(record.message.data(), record.message.size());
  }

  std::vector<const Metadata *> metas;
  std::vector<uint32_t>         timestamps;
  std::vector<std::string>      messages;
};

uint32_t fixedTimestamp42() noexcept {
  return 42;
}

uint32_t monotonicCounter() noexcept {
  static uint32_t counter{0};

  return ++counter;
}

} // namespace

// Object layout: non-copyable, non-movable, noexcept ctor/dtor; instance() and defaultTimestamp() are static.
TEST_CASE("log/backend/object_structure") {
  static_assert(!std::is_trivial_v<Backend>, "Log Backend must not be trivial");
  static_assert(!std::is_trivially_copyable_v<Backend>, "Log Backend must not be trivially copyable");
  static_assert(!std::is_trivially_destructible_v<Backend>, "Log Backend must not be trivially destructible");
  static_assert(std::is_standard_layout_v<Backend>, "Log Backend must have standard layout");
  static_assert(!std::is_copy_constructible_v<Backend>, "Log Backend must not be copyable");
  static_assert(!std::is_copy_assignable_v<Backend>, "Log Backend must not be copy-assignable");
  static_assert(!std::is_move_constructible_v<Backend>, "Log Backend must not be movable");
  static_assert(!std::is_move_assignable_v<Backend>, "Log Backend must not be move-assignable");
  static_assert(std::is_nothrow_default_constructible_v<Backend>, "Log Backend default ctor must be noexcept");
  static_assert(std::is_nothrow_destructible_v<Backend>, "Log Backend dtor must be noexcept");
  static_assert(noexcept(Backend::instance()), "Backend::instance() must be noexcept");
  static_assert(noexcept(Backend::defaultTimestamp()), "Backend::defaultTimestamp() must be noexcept");
}

// Constructed instance becomes the active backend reachable via instance().
TEST_CASE("log/backend/instance_returns_constructed_backend") {
  Backend backend;

  REQUIRE(&Backend::instance() == &backend);
}

// Fresh backend has no sink installed.
TEST_CASE("log/backend/constructor_starts_without_sink") {
  Backend backend;

  REQUIRE(backend.sink() == nullptr);
}

// Fresh backend installs defaultTimestamp as the policy.
TEST_CASE("log/backend/constructor_installs_default_timestamp_policy") {
  Backend backend;

  REQUIRE(backend.timestampPolicy() == &Backend::defaultTimestamp);
}

// defaultTimestamp always returns zero regardless of caller context.
TEST_CASE("log/backend/default_timestamp_returns_zero") {
  REQUIRE(Backend::defaultTimestamp() == 0);
}

// setSink swaps the installed sink and reflects the change via sink().
TEST_CASE("log/backend/set_sink_replaces_destination") {
  Backend       backend;
  RecordingSink sinkA;
  RecordingSink sinkB;

  backend.setSink(&sinkA);
  REQUIRE(backend.sink() == &sinkA);

  backend.setSink(&sinkB);
  REQUIRE(backend.sink() == &sinkB);

  backend.setSink(nullptr);
  REQUIRE(backend.sink() == nullptr);
}

// push() without a sink installed must be a silent no-op.
TEST_CASE("log/backend/push_without_sink_is_noop") {
  Backend backend;
  backend.setTimestampPolicy(&monotonicCounter);

  backend.push(&c_meta, 7);

  REQUIRE(backend.sink() == nullptr);
}

// push() formats the record and forwards it synchronously to the installed sink.
TEST_CASE("log/backend/push_dispatches_record_to_sink") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);

  backend.push(&c_metaNoArgs);

  REQUIRE(sink.metas.size() == 1);
  REQUIRE(sink.metas[0] == &c_metaNoArgs);
  REQUIRE(sink.messages[0] == "hello");
}

// push() forwards every format argument through vformatTo into the record message.
TEST_CASE("log/backend/push_forwards_format_arguments") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);

  backend.push(&c_meta, 99);
  backend.push(&c_metaThreeArgs, 1, 2, 3);

  REQUIRE(sink.messages.size() == 2);
  REQUIRE(sink.messages[0] == "value: 99");
  REQUIRE(sink.messages[1] == "1 + 2 = 3");
}

// Each push() captures the timestamp from the installed policy at call time.
TEST_CASE("log/backend/push_captures_timestamp_from_policy") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);
  backend.setTimestampPolicy(&fixedTimestamp42);

  backend.push(&c_metaNoArgs);
  backend.push(&c_metaNoArgs);

  REQUIRE(sink.timestamps.size() == 2);
  REQUIRE(sink.timestamps[0] == 42);
  REQUIRE(sink.timestamps[1] == 42);
}

// setTimestampPolicy(nullptr) restores the default zero-returning policy.
TEST_CASE("log/backend/set_timestamp_policy_null_restores_default") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);

  backend.setTimestampPolicy(&fixedTimestamp42);
  REQUIRE(backend.timestampPolicy() == &fixedTimestamp42);

  backend.setTimestampPolicy(nullptr);
  REQUIRE(backend.timestampPolicy() == &Backend::defaultTimestamp);

  backend.push(&c_metaNoArgs);
  REQUIRE(sink.timestamps.size() == 1);
  REQUIRE(sink.timestamps[0] == 0);
}

} // namespace toy::log
