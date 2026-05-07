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
  \file   chrono_stopwatch.test.cpp
  \brief  Unit tests for \ref toy::chrono::Stopwatch.
*/

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy::chrono {

// Stopwatch has fixed size and contiguous layout.
TEST_CASE("chrono/stop_watch/object_structure") {
  static_assert(sizeof(Stopwatch) == 8, "Stopwatch must be 8 bytes");

  static_assert(!std::is_trivial_v<Stopwatch>, "Stopwatch must not be trivial");
  static_assert(std::is_trivially_copyable_v<Stopwatch>, "Stopwatch must be trivially copyable");
  static_assert(std::is_trivially_destructible_v<Stopwatch>, "Stopwatch must be trivially destructible");
  static_assert(std::is_standard_layout_v<Stopwatch>, "Stopwatch must have standard layout");
}

} // namespace toy::chrono
