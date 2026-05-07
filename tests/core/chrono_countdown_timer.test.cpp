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
  \file   chrono_countdown_timer.test.cpp
  \brief  Unit tests for \ref toy::chrono::CountdownTimer.
*/

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy::chrono {

// CountdownTimer has fixed size and trivially copyable layout.
TEST_CASE("chrono/countdown_timer/object_structure") {
  static_assert(sizeof(CountdownTimer) == 8, "CountdownTimer must be 8 bytes");

  static_assert(!std::is_trivial_v<CountdownTimer>, "CountdownTimer must not be trivial");
  static_assert(std::is_trivially_copyable_v<CountdownTimer>, "CountdownTimer must be trivially copyable");
  static_assert(std::is_trivially_destructible_v<CountdownTimer>, "CountdownTimer must be trivially destructible");
  static_assert(std::is_standard_layout_v<CountdownTimer>, "CountdownTimer must have standard layout");
}

// expired() returns false immediately after construction with a large timeout.
TEST_CASE("chrono/countdown_timer/not_expired_after_construction") {
  ClockSource clock;

  const CountdownTimer timer{std::chrono::hours{1}};

  REQUIRE(!timer.expired());
}

// expired() returns true when constructed with a zero timeout.
TEST_CASE("chrono/countdown_timer/expired_with_zero_timeout") {
  ClockSource clock;

  const CountdownTimer timer{CountdownTimer::duration{0}};

  REQUIRE(timer.expired());
}

// remaining() returns duration{0} when the timer has expired.
TEST_CASE("chrono/countdown_timer/remaining_zero_when_expired") {
  ClockSource clock;

  const CountdownTimer timer{CountdownTimer::duration{0}};

  REQUIRE(timer.remaining() == CountdownTimer::duration{0});
}

// remaining() returns a positive value for a timer that has not expired.
TEST_CASE("chrono/countdown_timer/remaining_positive_when_not_expired") {
  ClockSource clock;

  constexpr auto       timeout = std::chrono::hours{1};
  const CountdownTimer timer{timeout};

  REQUIRE(timer.remaining() > CountdownTimer::duration{0});
  REQUIRE(timer.remaining() <= duration_cast<CountdownTimer::duration>(timeout));
}

// restart() moves the deadline forward; an expired timer becomes active again.
TEST_CASE("chrono/countdown_timer/restart_resets_deadline") {
  ClockSource clock;

  CountdownTimer timer{CountdownTimer::duration{0}};
  REQUIRE(timer.expired());

  timer.restart(std::chrono::hours{1});
  REQUIRE(!timer.expired());
}

} // namespace toy::chrono
