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
  \file   section.benchmark.cpp
  \brief  Implementation of picobench benchmarks for \ref toy::geometry::Section.
*/

#include "geometry.hpp"
#include "picobench/picobench.hpp"

namespace toy::geometry {

using fixed_type = math::fixed<int32_t, int64_t, 24>;

static void sectionIntMidpoint(picobench::state & state) noexcept {
  Section s(10, 20);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    s.start  = 10 + (i & 1);
    result  += static_cast<size_t>(s.midpoint());
  }
  state.set_result(result);
}

static void sectionFloatMidpoint(picobench::state & state) noexcept {
  Section s(10.0F, 20.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    s.start  = 10.0F + static_cast<float>(i & 1);
    result  += static_cast<size_t>(s.midpoint());
  }
  state.set_result(result);
}

static void sectionFixedMidpoint(picobench::state & state) noexcept {
  Section s(fixed_type(10), fixed_type(20));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    s.start  = fixed_type(10 + (i & 1));
    result  += static_cast<size_t>(s.midpoint().rawValue());
  }
  state.set_result(result);
}

static void sectionIntLength(picobench::state & state) noexcept {
  Section s(10, 20);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    s.start  = 10 + (i & 1);
    result  += static_cast<size_t>(s.length());
  }
  state.set_result(result);
}

static void sectionFloatLength(picobench::state & state) noexcept {
  Section s(10.0F, 20.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    s.start  = 10.0F + static_cast<float>(i & 1);
    result  += static_cast<size_t>(s.length());
  }
  state.set_result(result);
}

static void sectionFixedLength(picobench::state & state) noexcept {
  Section s(fixed_type(10), fixed_type(20));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    s.start  = fixed_type(10 + (i & 1));
    result  += static_cast<size_t>(s.length().rawValue());
  }
  state.set_result(result);
}

static void sectionIntReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section s(10 + (i & 1), 20);
    s.reset();
    result += static_cast<size_t>(s.start) + static_cast<size_t>(s.end);
  }
  state.set_result(result);
}

static void sectionFloatReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section s(10.0F + static_cast<float>(i & 1), 20.0F);
    s.reset();
    result += static_cast<size_t>(s.start) + static_cast<size_t>(s.end);
  }
  state.set_result(result);
}

static void sectionFixedReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section s(fixed_type(10 + (i & 1)), fixed_type(20));
    s.reset();
    result += static_cast<size_t>(s.start.rawValue()) + static_cast<size_t>(s.end.rawValue());
  }
  state.set_result(result);
}

static void sectionIntExpandValue(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section s(10, 20);
    s.expand(5 + (i & 1));
    result += static_cast<size_t>(s.start) + static_cast<size_t>(s.end);
  }
  state.set_result(result);
}

static void sectionFloatExpandValue(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section s(10.0F, 20.0F);
    s.expand(5.0F + static_cast<float>(i & 1));
    result += static_cast<size_t>(s.start) + static_cast<size_t>(s.end);
  }
  state.set_result(result);
}

static void sectionFixedExpandValue(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section s(fixed_type(10), fixed_type(20));
    s.expand(fixed_type(5 + (i & 1)));
    result += static_cast<size_t>(s.start.rawValue()) + static_cast<size_t>(s.end.rawValue());
  }
  state.set_result(result);
}

static void sectionIntExpandSection(picobench::state & state) noexcept {
  constexpr Section other(25, 30);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section s(10 + (i & 1), 20);
    s.expand(other);
    result += static_cast<size_t>(s.start) + static_cast<size_t>(s.end);
  }
  state.set_result(result);
}

static void sectionFloatExpandSection(picobench::state & state) noexcept {
  constexpr Section other(25.0F, 30.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section s(10.0F + static_cast<float>(i & 1), 20.0F);
    s.expand(other);
    result += static_cast<size_t>(s.start) + static_cast<size_t>(s.end);
  }
  state.set_result(result);
}

static void sectionFixedExpandSection(picobench::state & state) noexcept {
  constexpr Section other(fixed_type(25), fixed_type(30));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section s(fixed_type(10 + (i & 1)), fixed_type(20));
    s.expand(other);
    result += static_cast<size_t>(s.start.rawValue()) + static_cast<size_t>(s.end.rawValue());
  }
  state.set_result(result);
}

static void sectionIntIsReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section<int> s;
    s.expand(i & 1);
    result += s.isReset() ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionFloatIsReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section<float> s;
    s.expand(static_cast<float>(i & 1));
    result += s.isReset() ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionFixedIsReset(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    Section<fixed_type> s;
    s.expand(fixed_type(i & 1));
    result += s.isReset() ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionIntIsValid(picobench::state & state) noexcept {
  Section s(10, 20);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    s.start  = 10 + (i & 1);
    result  += s.isValid() ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionFloatIsValid(picobench::state & state) noexcept {
  Section s(10.0F, 20.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    s.start  = 10.0F + static_cast<float>(i & 1);
    result  += s.isValid() ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionFixedIsValid(picobench::state & state) noexcept {
  Section s(fixed_type(10), fixed_type(20));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    s.start  = fixed_type(10 + (i & 1));
    result  += s.isValid() ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionIntIsContains(picobench::state & state) noexcept {
  constexpr Section s(10, 20);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += s.isContains(15 + (i & 1)) ? 1 : 0;

  state.set_result(result);
}

static void sectionFloatIsContains(picobench::state & state) noexcept {
  constexpr Section s(10.0F, 20.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += s.isContains(15.0F + static_cast<float>(i & 1)) ? 1 : 0;

  state.set_result(result);
}

static void sectionFixedIsContains(picobench::state & state) noexcept {
  constexpr Section s(fixed_type(10), fixed_type(20));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += s.isContains(fixed_type(15 + (i & 1))) ? 1 : 0;

  state.set_result(result);
}

static void sectionIntEqual(picobench::state & state) noexcept {
  constexpr Section a(10, 20);
  Section           b(10, 20);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.start  = 10 + (i & 1);
    result  += (a == b) ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionFloatEqual(picobench::state & state) noexcept {
  constexpr Section a(10.0F, 20.0F);
  Section           b(10.0F, 20.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.start  = 10.0F + static_cast<float>(i & 1);
    result  += (a == b) ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionFixedEqual(picobench::state & state) noexcept {
  constexpr Section a(fixed_type(10), fixed_type(20));
  Section           b(fixed_type(10), fixed_type(20));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.start  = fixed_type(10 + (i & 1));
    result  += (a == b) ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionIntNotEqual(picobench::state & state) noexcept {
  constexpr Section a(10, 20);
  Section           b(15, 25);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.start  = 15 + (i & 1);
    result  += (a != b) ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionFloatNotEqual(picobench::state & state) noexcept {
  constexpr Section a(10.0F, 20.0F);
  Section           b(15.0F, 25.0F);

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.start  = 15.0F + static_cast<float>(i & 1);
    result  += (a != b) ? 1 : 0;
  }
  state.set_result(result);
}

static void sectionFixedNotEqual(picobench::state & state) noexcept {
  constexpr Section a(fixed_type(10), fixed_type(20));
  Section           b(fixed_type(15), fixed_type(25));

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    b.start  = fixed_type(15 + (i & 1));
    result  += (a != b) ? 1 : 0;
  }
  state.set_result(result);
}

PICOBENCH_SUITE("toy::geometry::Section");
PICOBENCH(sectionIntMidpoint);
PICOBENCH(sectionFloatMidpoint);
PICOBENCH(sectionFixedMidpoint);
PICOBENCH(sectionIntLength);
PICOBENCH(sectionFloatLength);
PICOBENCH(sectionFixedLength);
PICOBENCH(sectionIntReset);
PICOBENCH(sectionFloatReset);
PICOBENCH(sectionFixedReset);
PICOBENCH(sectionIntExpandValue);
PICOBENCH(sectionFloatExpandValue);
PICOBENCH(sectionFixedExpandValue);
PICOBENCH(sectionIntExpandSection);
PICOBENCH(sectionFloatExpandSection);
PICOBENCH(sectionFixedExpandSection);
PICOBENCH(sectionIntIsReset);
PICOBENCH(sectionFloatIsReset);
PICOBENCH(sectionFixedIsReset);
PICOBENCH(sectionIntIsValid);
PICOBENCH(sectionFloatIsValid);
PICOBENCH(sectionFixedIsValid);
PICOBENCH(sectionIntIsContains);
PICOBENCH(sectionFloatIsContains);
PICOBENCH(sectionFixedIsContains);
PICOBENCH(sectionIntEqual);
PICOBENCH(sectionFloatEqual);
PICOBENCH(sectionFixedEqual);
PICOBENCH(sectionIntNotEqual);
PICOBENCH(sectionFloatNotEqual);
PICOBENCH(sectionFixedNotEqual);

} // namespace toy::geometry
