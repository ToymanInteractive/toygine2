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
  \file   log_metadata.test.cpp
  \brief  Unit tests for \ref toy::log::Metadata.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy::log {

// What a copy and a scope exit cost the caller, the initializer form the call site may write, and what a
// declaration without one leaves behind.
TEST_CASE("log/metadata/value_semantics") {
  static_assert(std::is_trivially_copyable_v<Metadata>, "the record must copy as bytes");
  static_assert(std::is_trivially_destructible_v<Metadata>, "leaving scope must run no destructor");
  static_assert(std::is_standard_layout_v<Metadata>, "the layout must stay readable from a C interface");

  static_assert(std::is_aggregate_v<Metadata>, "designated initializers require the type to stay an aggregate");
  static_assert(std::is_trivially_default_constructible_v<Metadata>,
                "the fields carry no in-class initializer, so a declaration without an initializer leaves them unset");
}

} // namespace toy::log
