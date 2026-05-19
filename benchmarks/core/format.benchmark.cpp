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
  \file   format.benchmark.cpp
  \brief  Implementation of picobench benchmarks for toy::vformat() and toy::vformatTo().
*/

#include "core.hpp"
#include "picobench/picobench.hpp"

namespace toy {

static void vformatToNoPlaceholders(picobench::state & state) noexcept {
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    FixedString<64> output;
    vformatTo(output, CStringView("Hello World"));
  }
}

static void vformatToSingleInt(picobench::state & state) noexcept {
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    FixedString<64> output;
    vformatTo(output, CStringView("value: {}"), 42);
  }
}

static void vformatToSingleCString(picobench::state & state) noexcept {
  const char * msg = "hello";

  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    FixedString<64> output;
    vformatTo(output, CStringView("say: {}"), msg);
  }
}

static void vformatToSingleFixedString(picobench::state & state) noexcept {
  FixedString<16> name("engine");

  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    FixedString<64> output;
    vformatTo(output, CStringView("toy {}"), name);
  }
}

static void vformatToSingleCStringView(picobench::state & state) noexcept {
  CStringView sv("engine");

  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    FixedString<64> output;
    vformatTo(output, CStringView("toy {}"), sv);
  }
}

static void vformatTo3ArgsMixed(picobench::state & state) noexcept {
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    FixedString<64> output;
    vformatTo(output, CStringView("{} {} {}"), 42, "world", true);
  }
}

static void vformatTo5ArgsInt(picobench::state & state) noexcept {
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    FixedString<128> output;
    vformatTo(output, CStringView("{} {} {} {} {}"), 1, 2, 3, 4, 5);
  }
}

static void vformatToPositionalReorder(picobench::state & state) noexcept {
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    FixedString<64> output;
    vformatTo(output, CStringView("{1} before {0}"), 10, 20);
  }
}

static void vformatToEscapedBraces(picobench::state & state) noexcept {
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    FixedString<64> output;
    vformatTo(output, CStringView("{{{}}}: {{done}}"), 42);
  }
}

PICOBENCH_SUITE("toy::vformatTo");
PICOBENCH(vformatToNoPlaceholders);
PICOBENCH(vformatToSingleInt);
PICOBENCH(vformatToSingleCString);
PICOBENCH(vformatToSingleFixedString);
PICOBENCH(vformatToSingleCStringView);
PICOBENCH(vformatTo3ArgsMixed);
PICOBENCH(vformatTo5ArgsInt);
PICOBENCH(vformatToPositionalReorder);
PICOBENCH(vformatToEscapedBraces);

static void vformatSingleInt(picobench::state & state) noexcept {
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    [[maybe_unused]] auto result = vformat<64>(CStringView("value: {}"), 42);
  }
}

static void vformat3ArgsMixed(picobench::state & state) noexcept {
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    [[maybe_unused]] auto result = vformat<64>(CStringView("{} {} {}"), 42, "world", true);
  }
}

static void vformat5ArgsInt(picobench::state & state) noexcept {
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    [[maybe_unused]] auto result = vformat<128>(CStringView("{} {} {} {} {}"), 1, 2, 3, 4, 5);
  }
}

PICOBENCH_SUITE("toy::vformat");
PICOBENCH(vformatSingleInt);
PICOBENCH(vformat3ArgsMixed);
PICOBENCH(vformat5ArgsInt);

} // namespace toy
