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
  \file   chrono_timestamp.test.cpp
  \brief  Unit tests for \ref toy::chrono::Timestamp.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy::chrono {

TEST_CASE("chrono/timestamp/value_semantics") {
  static_assert(std::is_trivially_copyable_v<Timestamp>, "the timestamp must copy as bytes");
  static_assert(std::is_trivially_destructible_v<Timestamp>, "leaving scope must run no destructor");
  static_assert(sizeof(Timestamp) == 8, "the millisecond count is the only state");
  static_assert(Timestamp{}.milliseconds() == 0, "a default stamp counts from the startup zero");
}

TEST_CASE("chrono/timestamp/ordering") {
  constexpr Timestamp zero{};
  constexpr Timestamp same{};

  CHECK(zero == same);
  CHECK(!(zero < same));
  CHECK(!(same < zero));

  CHECK(!(Timestamp::now() < zero));
}

} // namespace toy::chrono
