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

#include "render.hpp"

namespace toy::render {

// Bitwise AND: intersection of clear flags.
TEST_CASE("render/clear_flags/operator_and") {
  // All & Target yields Target.
  SUBCASE("all_and_target") {
    REQUIRE((ClearFlags::All & ClearFlags::Target) == ClearFlags::Target);

    static_assert((ClearFlags::All & ClearFlags::Target) == ClearFlags::Target, "All & Target must equal Target");
  }

  // Target & Depth yields None (no common bits).
  SUBCASE("target_and_depth") {
    REQUIRE((ClearFlags::Target & ClearFlags::Depth) == ClearFlags::None);

    static_assert((ClearFlags::Target & ClearFlags::Depth) == ClearFlags::None, "Target & Depth must equal None");
  }

  // All & All yields All.
  SUBCASE("all_and_all") {
    REQUIRE((ClearFlags::All & ClearFlags::All) == ClearFlags::All);

    static_assert((ClearFlags::All & ClearFlags::All) == ClearFlags::All, "All & All must equal All");
  }
}

// Bitwise OR: combines clear flags.
TEST_CASE("render/clear_flags/operator_or") {
  // Target | Depth yields combined flags.
  SUBCASE("target_or_depth") {
    REQUIRE(static_cast<unsigned int>(ClearFlags::Target | ClearFlags::Depth) == 0x03);

    static_assert(static_cast<unsigned int>(ClearFlags::Target | ClearFlags::Depth) == 0x03u,
                  "Target | Depth must equal 0x03");
  }

  // Target | Depth | Stencil yields All.
  SUBCASE("target_or_depth_or_stencil") {
    REQUIRE((ClearFlags::Target | ClearFlags::Depth | ClearFlags::Stencil) == ClearFlags::All);

    static_assert((ClearFlags::Target | ClearFlags::Depth | ClearFlags::Stencil) == ClearFlags::All,
                  "Target | Depth | Stencil must equal All");
  }

  // None | Target yields Target (None is identity for OR).
  SUBCASE("none_or_target") {
    REQUIRE((ClearFlags::None | ClearFlags::Target) == ClearFlags::Target);

    static_assert((ClearFlags::None | ClearFlags::Target) == ClearFlags::Target, "None | Target must equal Target");
  }

  // All | Target yields All (idempotent).
  SUBCASE("all_or_target") {
    REQUIRE((ClearFlags::All | ClearFlags::Target) == ClearFlags::All);

    static_assert((ClearFlags::All | ClearFlags::Target) == ClearFlags::All, "All | Target must equal All");
  }
}

// Bitwise XOR: symmetric difference of clear flags.
TEST_CASE("render/clear_flags/operator_xor") {
  // Target ^ Depth yields combined (no overlap).
  SUBCASE("target_xor_depth") {
    REQUIRE((ClearFlags::Target ^ ClearFlags::Depth) == (ClearFlags::Target | ClearFlags::Depth));

    static_assert((ClearFlags::Target ^ ClearFlags::Depth) == (ClearFlags::Target | ClearFlags::Depth),
                  "Target ^ Depth must equal Target | Depth");
  }

  // Target ^ Target yields None.
  SUBCASE("target_xor_target") {
    REQUIRE((ClearFlags::Target ^ ClearFlags::Target) == ClearFlags::None);

    static_assert((ClearFlags::Target ^ ClearFlags::Target) == ClearFlags::None, "Target ^ Target must equal None");
  }
}

} // namespace toy::render
