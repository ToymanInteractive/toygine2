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
  \file   log_macros_filter.test.cpp
  \brief  Verifies the \c LOG_MAX_LEVEL compile-time gate by capping verbosity at Warn before including core.hpp.
*/

// Cap verbosity at Warn so LOG_TRACE / LOG_DEBUG / LOG_INFO are eliminated at compile time.
#define LOG_MAX_LEVEL 20

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

// With LOG_MAX_LEVEL set to Warn the more verbose macros must not dispatch to the sink.
TEST_CASE("log/macros/filter_eliminates_verbose_levels") {
  Backend       backend;
  RecordingSink sink;
  backend.setSink(&sink);

  LOG_TRACE("trace");
  LOG_DEBUG("debug");
  LOG_INFO("info");
  LOG_WARN("warn");
  LOG_ERROR("error");

  REQUIRE(sink.metas.size() == 2);
  REQUIRE(sink.metas[0]->level == Level::Warn);
  REQUIRE(sink.metas[1]->level == Level::Error);
  REQUIRE(sink.messages[0] == "warn");
  REQUIRE(sink.messages[1] == "error");
}

} // namespace toy::log
