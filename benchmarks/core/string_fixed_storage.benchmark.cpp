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
  \file   string_fixed_storage.benchmark.cpp
  \brief  Implementation of picobench benchmarks for \ref toy::StringFixedStorage.
*/

#include "core.hpp"
#include "picobench/picobench.hpp"

namespace toy {

static void capacity(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    result += StringFixedStorage<16>::capacity();
  }
  state.set_result(result);
}

static void size(picobench::state & state) noexcept {
  StringFixedStorage<16> storage;

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    result += storage.size();
  }
  state.set_result(result);
}

static void setSize(picobench::state & state) noexcept {
  StringFixedStorage<16> storage;

  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    storage.setSize(2);
}

PICOBENCH_SUITE("toy::StringFixedStorage");
PICOBENCH(capacity);
PICOBENCH(size);
PICOBENCH(setSize);

} // namespace toy
