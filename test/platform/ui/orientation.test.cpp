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

// Bitwise OR: combines orientation flags.
TEST_CASE("platform/ui/orientation/operator_or") {
  // Landscape | LandscapeFlipped yields LandscapeAny.
  SUBCASE("landscape_or_landscape_flipped") {
    REQUIRE((Orientation::Landscape | Orientation::LandscapeFlipped) == Orientation::LandscapeAny);

    static_assert((Orientation::Landscape | Orientation::LandscapeFlipped) == Orientation::LandscapeAny,
                  "Landscape | LandscapeFlipped must equal LandscapeAny");
  }

  // Portrait | PortraitFlipped yields PortraitAny.
  SUBCASE("portrait_or_portrait_flipped") {
    REQUIRE((Orientation::Portrait | Orientation::PortraitFlipped) == Orientation::PortraitAny);

    static_assert((Orientation::Portrait | Orientation::PortraitFlipped) == Orientation::PortraitAny,
                  "Portrait | PortraitFlipped must equal PortraitAny");
  }

  // LandscapeAny | PortraitAny yields Any.
  SUBCASE("landscape_any_or_portrait_any") {
    REQUIRE((Orientation::LandscapeAny | Orientation::PortraitAny) == Orientation::Any);

    static_assert((Orientation::LandscapeAny | Orientation::PortraitAny) == Orientation::Any,
                  "LandscapeAny | PortraitAny must equal Any");
  }
}

// Bitwise AND: intersection of orientation flags.
TEST_CASE("platform/ui/orientation/operator_and") {
  // Landscape & LandscapeAny yields Landscape.
  SUBCASE("landscape_and_landscape_any") {
    REQUIRE((Orientation::Landscape & Orientation::LandscapeAny) == Orientation::Landscape);

    static_assert((Orientation::Landscape & Orientation::LandscapeAny) == Orientation::Landscape,
                  "Landscape & LandscapeAny must equal Landscape");
  }

  // Portrait & PortraitAny yields Portrait.
  SUBCASE("portrait_and_portrait_any") {
    REQUIRE((Orientation::Portrait & Orientation::PortraitAny) == Orientation::Portrait);

    static_assert((Orientation::Portrait & Orientation::PortraitAny) == Orientation::Portrait,
                  "Portrait & PortraitAny must equal Portrait");
  }

  // Landscape & Portrait yields no common bits.
  SUBCASE("landscape_and_portrait") {
    REQUIRE(static_cast<unsigned int>(Orientation::Landscape & Orientation::Portrait) == 0);

    static_assert(static_cast<unsigned int>(Orientation::Landscape & Orientation::Portrait) == 0,
                  "Landscape & Portrait must have no common bits");
  }
}

// Bitwise XOR: symmetric difference of orientation flags.
TEST_CASE("platform/ui/orientation/operator_xor") {
  // Landscape ^ LandscapeFlipped yields bits that differ.
  SUBCASE("landscape_xor_landscape_flipped") {
    REQUIRE((Orientation::Landscape ^ Orientation::LandscapeFlipped) == Orientation::LandscapeAny);

    static_assert((Orientation::Landscape ^ Orientation::LandscapeFlipped) == Orientation::LandscapeAny,
                  "Landscape ^ LandscapeFlipped must equal LandscapeAny");
  }

  // Same value XOR same value yields zero.
  SUBCASE("same_xor_same") {
    REQUIRE(static_cast<unsigned int>(Orientation::Landscape ^ Orientation::Landscape) == 0);

    static_assert(static_cast<unsigned int>(Orientation::Landscape ^ Orientation::Landscape) == 0,
                  "Landscape ^ Landscape must be zero");
  }
}

} // namespace toy::platform::ui
