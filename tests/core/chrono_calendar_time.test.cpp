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
  \file   chrono_calendar_time.test.cpp
  \brief  Unit tests for \ref toy::chrono::CalendarTime.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy::chrono {

constexpr int64_t msPerDay = 24LL * 60 * 60 * 1000;

TEST_CASE("chrono/calendar_time/value_semantics") {
  static_assert(std::is_trivially_copyable_v<CalendarTime>, "the calendar time must copy as bytes");
  static_assert(std::is_trivially_destructible_v<CalendarTime>, "leaving scope must run no destructor");
  static_assert(!std::is_trivially_default_constructible_v<CalendarTime>,
                "the in-class initializers must zero every field on a bare declaration");
  static_assert(sizeof(CalendarTime) == 12, "nine fixed-width fields must pack into 12 bytes");
}

TEST_CASE("chrono/calendar_time/default_values") {
  constexpr CalendarTime bare;

  CHECK(bare.year == 0);
  CHECK(bare.month == 0);
  CHECK(bare.day == 0);
  CHECK(bare.dayOfWeek == 0);
  CHECK(bare.hour == 0);
  CHECK(bare.minute == 0);
  CHECK(bare.second == 0);
  CHECK(bare.millisecond == 0);
  CHECK(bare.tzOffsetMinutes == 0);

  static_assert(CalendarTime{}.tzOffsetMinutes == 0, "value initialization leaves the offset at zero");
}

TEST_CASE("chrono/calendar_time/from_uptime_base") {
  const auto base = CalendarTime::fromUptime(Duration<int64_t, std::milli>{0});

  CHECK(base.year == 0);
  CHECK(base.month == 0);
  CHECK(base.day == 0);
  CHECK(base.dayOfWeek == 0);
  CHECK(base.hour == 0);
  CHECK(base.minute == 0);
  CHECK(base.second == 0);
  CHECK(base.millisecond == 0);
  CHECK(base.tzOffsetMinutes == 0);

  static_assert(CalendarTime::fromUptime(Duration<int64_t, std::milli>{0}).second == 0,
                "the synthesis base is all zeros");
}

TEST_CASE("chrono/calendar_time/from_uptime_one_second") {
  const auto oneSecond = CalendarTime::fromUptime(Duration<int64_t, std::milli>{1'000});

  CHECK(oneSecond.year == 0);
  CHECK(oneSecond.month == 0);
  CHECK(oneSecond.day == 0);
  CHECK(oneSecond.hour == 0);
  CHECK(oneSecond.minute == 0);
  CHECK(oneSecond.second == 1);
  CHECK(oneSecond.millisecond == 0);

  static_assert(CalendarTime::fromUptime(Duration<int64_t, std::milli>{1'000}).second == 1,
                "one second of uptime must read 0000-00-00 00:00:01");
}

TEST_CASE("chrono/calendar_time/from_uptime_rolls") {
  static_assert(
    [] {
      const auto midMonth = CalendarTime::fromUptime(Duration<int64_t, std::milli>{15 * msPerDay});

      return midMonth.year == 0 && midMonth.month == 0 && midMonth.day == 15;
    }(),
    "day 15 must stay inside month zero"
  );
  static_assert(
    [] {
      const auto secondMonth = CalendarTime::fromUptime(Duration<int64_t, std::milli>{30 * msPerDay});

      return secondMonth.year == 0 && secondMonth.month == 1 && secondMonth.day == 0;
    }(),
    "day 30 must open month one at day zero"
  );
  static_assert(
    [] {
      const auto monthAndHalf = CalendarTime::fromUptime(Duration<int64_t, std::milli>{45 * msPerDay});

      return monthAndHalf.month == 1 && monthAndHalf.day == 15;
    }(),
    "day 45 must read month one, day fifteen"
  );
  static_assert(
    [] {
      const auto secondYear = CalendarTime::fromUptime(Duration<int64_t, std::milli>{360 * msPerDay});

      return secondYear.year == 1 && secondYear.month == 0 && secondYear.day == 0;
    }(),
    "day 360 must open year one"
  );
}

TEST_CASE("chrono/calendar_time/from_uptime_decomposition") {
  static_assert(
    [] {
      const auto t = CalendarTime::fromUptime(Duration<int64_t, std::milli>{3'661'234});

      return t.hour == 1 && t.minute == 1 && t.second == 1 && t.millisecond == 234;
    }(),
    "3'661'234 ms must decompose into 1h 1m 1s 234ms"
  );
}

TEST_CASE("chrono/calendar_time/day_boundaries") {
  static_assert(
    [] {
      const auto lastMs = CalendarTime::fromUptime(Duration<int64_t, std::milli>{msPerDay - 1});

      return lastMs.hour == 23 && lastMs.minute == 59 && lastMs.second == 59 && lastMs.millisecond == 999;
    }(),
    "the last millisecond of a day must read 23:59:59.999"
  );
  static_assert(
    [] {
      const auto nextDay = CalendarTime::fromUptime(Duration<int64_t, std::milli>{msPerDay});

      return nextDay.day == 1 && nextDay.hour == 0 && nextDay.millisecond == 0;
    }(),
    "one full day must open day one at 00:00:00.000"
  );
}

TEST_CASE("chrono/calendar_time/equality") {
  constexpr auto a = CalendarTime::fromUptime(Duration<int64_t, std::milli>{0});
  constexpr auto b = CalendarTime::fromUptime(Duration<int64_t, std::milli>{0});
  constexpr auto c = CalendarTime::fromUptime(Duration<int64_t, std::milli>{1});

  static_assert(a == b, "equal uptimes must compare equal");
  static_assert(!(a == c), "a millisecond difference must break equality");
  static_assert(a != c, "!= must come from the defaulted ==");
}

} // namespace toy::chrono
