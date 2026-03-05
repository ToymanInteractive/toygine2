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

#include <doctest/doctest.h>

#include "platform/ui.hpp"

namespace toy::platform::ui {

// Bitwise OR: combines window style flags.
TEST_CASE("platform/ui/window_style/operator_or") {
  // Close | MinMax yields combined title-bar flags.
  SUBCASE("close_or_min_max") {
    constexpr auto combined = WindowStyle::Close | WindowStyle::MinMax;

    REQUIRE(std::to_underlying(combined) == 3);

    static_assert(std::to_underlying(combined) == 3, "Close | MinMax must equal combined Close and MinMax bits");
  }

  // Simple is zero; OR with Simple preserves the other operand.
  SUBCASE("simple_or_preserves_other") {
    constexpr auto combined = WindowStyle::Simple | WindowStyle::Close;

    REQUIRE(combined == WindowStyle::Close);

    static_assert(combined == WindowStyle::Close, "Simple | Close must equal Close because Simple is zero");
  }
}

// Bitwise AND: intersection of window style flags.
TEST_CASE("platform/ui/window_style/operator_and") {
  // (Close | MinMax) & Close yields Close.
  SUBCASE("combined_and_close") {
    constexpr auto combined = WindowStyle::Close | WindowStyle::MinMax;

    REQUIRE((combined & WindowStyle::Close) == WindowStyle::Close);

    static_assert((combined & WindowStyle::Close) == WindowStyle::Close, "Combined & Close must equal Close");
  }

  // Close & MinMax have no common bits; result is Simple (zero).
  SUBCASE("close_and_min_max") {
    constexpr auto combined = WindowStyle::Close & WindowStyle::MinMax;

    REQUIRE(std::to_underlying(combined) == 0);

    static_assert(std::to_underlying(combined) == 0, "Close & MinMax must have no common bits");
  }
}

// Bitwise XOR: symmetric difference of window style flags.
TEST_CASE("platform/ui/window_style/operator_xor") {
  // Close ^ MinMax yields bits set in exactly one operand.
  SUBCASE("close_xor_min_max") {
    constexpr auto combined = WindowStyle::Close | WindowStyle::MinMax;

    REQUIRE((WindowStyle::Close ^ WindowStyle::MinMax) == combined);

    static_assert((WindowStyle::Close ^ WindowStyle::MinMax) == combined, "Close ^ MinMax must equal Close | MinMax");
  }

  // Same value XOR same value yields zero.
  SUBCASE("same_xor_same") {
    constexpr auto combined = WindowStyle::FullScreen ^ WindowStyle::FullScreen;

    REQUIRE(std::to_underlying(combined) == 0);

    static_assert(std::to_underlying(combined) == 0, "FullScreen ^ FullScreen must be zero");
  }
}

} // namespace toy::platform::ui
