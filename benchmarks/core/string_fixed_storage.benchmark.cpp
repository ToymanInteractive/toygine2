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
  \brief  Implementations for string fixed storage nanobench benchmarks in the core module.
*/

#include "../benchmark_factory.hpp"
#include "core.hpp"

namespace toy {

// StringFixedStorage benchmarks
void stringFixedStorageCoreBenchmarks(ankerl::nanobench::Bench & bench) noexcept {
  bench.run("StringFixedStorage<32> default construct", [] {
    toy::StringFixedStorage<32> s;
    doNotOptimize(s.data());
  });

  bench.run("StringFixedStorage<256> capacity", [] {
    auto c = toy::StringFixedStorage<256>::capacity();
    doNotOptimize(c);
  });

  bench.run("StringFixedStorage<8> size", [] {
    toy::StringFixedStorage<8> s;
    auto n = s.size();
    doNotOptimize(n);
  });

  bench.run("StringFixedStorage<32> setSize", [] {
    toy::StringFixedStorage<32> s;
    s.data()[0] = 'a';
    s.data()[1] = 'b';
    s.setSize(2);
    doNotOptimize(s.data());
  });
}

} // namespace toy
