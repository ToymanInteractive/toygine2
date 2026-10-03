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
  \file   log_record.test.cpp
  \brief  Unit tests for \ref toy::log::Record.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy::log {

// What a copy and a scope exit cost the caller, the initializer form the call site may write, and what a
// declaration without one leaves behind.
TEST_CASE("log/record/value_semantics") {
  static_assert(std::is_trivially_copyable_v<Record> == std::is_trivially_copyable_v<FixedString<c_messageCapacity>>,
                "the record must copy as bytes exactly when its message does");
  static_assert(std::is_trivially_destructible_v<Record>, "leaving scope must run no destructor");

  static_assert(std::is_aggregate_v<Record>, "designated initializers require the type to stay an aggregate");
  static_assert(!std::is_trivially_default_constructible_v<Record>,
                "every field carries an in-class initializer, so a declaration without one leaves none unset");
}

} // namespace toy::log
