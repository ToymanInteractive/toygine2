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
  \file   o_string_stream.benchmark.cpp
  \brief  Implementation of picobench benchmarks for \ref toy::OStringStream.
*/

#include "core.hpp"
#include "picobench/picobench.hpp"

namespace toy {

static void oStringStreamDefaultConstruct(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<64>> s;
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamLiteralAndInt(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<64>> s;
    s << "n=" << 12345;
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamChainedMixed(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<64>> s;
    s << "v=" << -42 << ' ' << 3.1415926F << CStringView(" end");
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamDurationNanoseconds(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<32>> s;
    s << chrono::nanoseconds{16'000'000};
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamDurationMilliseconds(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<32>> s;
    s << chrono::milliseconds{1'042};
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamDurationSeconds(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<32>> s;
    s << chrono::seconds{65};
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamDurationNegativeMilliseconds(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<32>> s;
    s << chrono::milliseconds{-500};
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamDurationFormatHhMmSs(picobench::state & state) noexcept {
  const chrono::DurationFormat fmt{"h:m:s:z", chrono::milliseconds{3'723'000}};

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<32>> s;
    s << fmt;
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamDurationFormatHhMmSsZzz(picobench::state & state) noexcept {
  const chrono::DurationFormat fmt{"hh:mm:ss.zzz", chrono::milliseconds{3'723'042}};

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<32>> s;
    s << fmt;
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamTimePointMilliseconds(picobench::state & state) noexcept {
  const chrono::TimePoint<chrono::SteadyClock, chrono::milliseconds> tp{chrono::milliseconds{3'723'042}};

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<32>> s;
    s << tp;
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamTimePointFormatHhMmSsZzz(picobench::state & state) noexcept {
  const chrono::TimePoint<chrono::SteadyClock, chrono::milliseconds> tp{chrono::milliseconds{3'723'042}};
  const chrono::TimePointFormat                                      fmt{"hh:mm:ss.zzz", tp};

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<32>> s;
    s << fmt;
    result += s.str().size();
  }
  state.set_result(result);
}

static void oStringStreamCalendarTime(picobench::state & state) noexcept {
  const chrono::CalendarTime ct = chrono::SystemClock::now();

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    OStringStream<FixedString<32>> s;
    s << ct;
    result += s.str().size();
  }
  state.set_result(result);
}

PICOBENCH_SUITE("toy::OStringStream");
PICOBENCH(oStringStreamDefaultConstruct);
PICOBENCH(oStringStreamLiteralAndInt);
PICOBENCH(oStringStreamChainedMixed);
PICOBENCH(oStringStreamDurationNanoseconds);
PICOBENCH(oStringStreamDurationMilliseconds);
PICOBENCH(oStringStreamDurationSeconds);
PICOBENCH(oStringStreamDurationNegativeMilliseconds);
PICOBENCH(oStringStreamDurationFormatHhMmSs);
PICOBENCH(oStringStreamDurationFormatHhMmSsZzz);
PICOBENCH(oStringStreamTimePointMilliseconds);
PICOBENCH(oStringStreamTimePointFormatHhMmSsZzz);
PICOBENCH(oStringStreamCalendarTime);

} // namespace toy
