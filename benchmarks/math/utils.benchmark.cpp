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
  \file   utils.benchmark.cpp
  \brief  Implementation of picobench benchmarks for angle conversion utilities in the math module (float vs fixed).
*/

#include "math.hpp"
#include "picobench/picobench.hpp"

namespace toy::math {

using fixed_type = fixed<int32_t, int64_t, 24>;

static void deg2radFloat(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += static_cast<size_t>(deg2rad(15.0F));

  state.set_result(result);
}

static void deg2radFixed(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += static_cast<size_t>(deg2rad(fixed_type{15.0F}).rawValue());

  state.set_result(result);
}

static void rad2degFloat(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += static_cast<size_t>(rad2deg(1.25F));

  state.set_result(result);
}

static void rad2degFixed(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += static_cast<size_t>(rad2deg(fixed_type{1.25F}).rawValue());

  state.set_result(result);
}

PICOBENCH_SUITE("toy::math::utils");
PICOBENCH(deg2radFloat);
PICOBENCH(deg2radFixed);
PICOBENCH(rad2degFloat);
PICOBENCH(rad2degFixed);

} // namespace toy::math
