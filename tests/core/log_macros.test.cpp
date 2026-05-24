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
  \file   log_macros.test.cpp
  \brief  Unit tests for \c LOG_TRACE, \c LOG_DEBUG, \c LOG_INFO, \c LOG_WARN, \c LOG_ERROR.
*/

#include <vector>

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy::log {

namespace {

class RecordingSink final : public ISink {
public:
  RecordingSink() noexcept           = default;
  ~RecordingSink() noexcept override = default;

  TOYGINE_NO_COPY_MOVE(RecordingSink);

  void write(const Record & record) noexcept override {
    metas.push_back(record.meta);
    messages.emplace_back(record.message.data(), record.message.size());
  }

  std::vector<const Metadata *> metas;
  std::vector<std::string>      messages;
};

} // namespace

// Each LOG_* macro forwards a record stamped with the matching severity to the active sink.
TEST_CASE("log/macros/expands_each_level") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);

  LOG_TRACE("t");
  LOG_DEBUG("d");
  LOG_INFO("i");
  LOG_WARN("w");
  LOG_ERROR("e");

  REQUIRE(sink.metas.size() == 5);
  REQUIRE(sink.metas[0]->level == Level::Trace);
  REQUIRE(sink.metas[1]->level == Level::Debug);
  REQUIRE(sink.metas[2]->level == Level::Info);
  REQUIRE(sink.metas[3]->level == Level::Warn);
  REQUIRE(sink.metas[4]->level == Level::Error);
  REQUIRE(sink.messages.size() == 5);
  REQUIRE(sink.messages[0] == "t");
  REQUIRE(sink.messages[4] == "e");
}

// Each LOG_* call stores __FILE__ and __LINE__ into the per-call-site Metadata.
TEST_CASE("log/macros/captures_file_and_line") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);

  const uint32_t expectedLine = static_cast<uint32_t>(__LINE__) + 1;
  LOG_INFO("anchor");

  REQUIRE(sink.metas.size() == 1);
  REQUIRE(sink.metas[0]->line == expectedLine);

  const std::string_view file{sink.metas[0]->file};
  REQUIRE(file.find("log_macros.test.cpp") != std::string_view::npos);
}

// Zero-argument call writes the literal format pattern verbatim to the record message.
TEST_CASE("log/macros/zero_args") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);

  LOG_INFO("plain message");

  REQUIRE(sink.messages.size() == 1);
  REQUIRE(sink.messages[0] == "plain message");
}

// Variadic arguments are forwarded through toy::vformatTo and substituted into the pattern.
TEST_CASE("log/macros/variadic_args") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);

  LOG_INFO("a={} b={}", 1, 2);
  LOG_WARN("{} + {} = {}", 2, 3, 5);

  REQUIRE(sink.messages.size() == 2);
  REQUIRE(sink.messages[0] == "a=1 b=2");
  REQUIRE(sink.messages[1] == "2 + 3 = 5");
}

// Without a sink installed every LOG_* call returns silently.
TEST_CASE("log/macros/no_sink_is_noop") {
  Backend backend;

  LOG_TRACE("dropped");
  LOG_DEBUG("dropped");
  LOG_INFO("dropped");
  LOG_WARN("dropped");
  LOG_ERROR("dropped");

  REQUIRE(backend.sink() == nullptr);
}

// Multiple LOG_* calls in the same function body produce independent Metadata instances.
TEST_CASE("log/macros/per_call_site_metadata_is_unique") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);

  LOG_INFO("first");
  LOG_INFO("second");

  REQUIRE(sink.metas.size() == 2);
  REQUIRE(sink.metas[0] != sink.metas[1]);
  REQUIRE(sink.metas[0]->line != sink.metas[1]->line);
}

} // namespace toy::log
