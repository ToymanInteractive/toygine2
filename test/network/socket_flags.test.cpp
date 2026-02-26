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

#include "network.hpp"

namespace toy::network {

// Bitwise OR: combines socket flags.
TEST_CASE("network/socket_flags/operator_or") {
  SUBCASE("none_or_non_blocking") {
    constexpr auto result = SocketFlags::None | SocketFlags::NonBlocking;

    REQUIRE(result == SocketFlags::NonBlocking);

    static_assert(result == SocketFlags::NonBlocking, "None | NonBlocking must equal NonBlocking");
  }

  SUBCASE("non_blocking_or_reuse_address") {
    REQUIRE(static_cast<uint8_t>(SocketFlags::NonBlocking | SocketFlags::ReuseAddress) == 0x03);

    static_assert(static_cast<uint8_t>(SocketFlags::NonBlocking | SocketFlags::ReuseAddress) == 0x03,
                  "NonBlocking | ReuseAddress must equal 0x03");
  }

  SUBCASE("combine_three_flags") {
    constexpr auto combined = SocketFlags::NonBlocking | SocketFlags::ReuseAddress | SocketFlags::Listen;

    REQUIRE(static_cast<uint8_t>(combined) == 0x0b);

    static_assert(static_cast<uint8_t>(combined) == 0x0b, "NonBlocking | ReuseAddress | Listen must equal 0x0b");
  }
}

// Bitwise AND: intersection of socket flags.
TEST_CASE("network/socket_flags/operator_and") {
  SUBCASE("non_blocking_and_combined") {
    constexpr auto combined = SocketFlags::NonBlocking | SocketFlags::ReuseAddress;

    REQUIRE((SocketFlags::NonBlocking & combined) == SocketFlags::NonBlocking);

    static_assert((SocketFlags::NonBlocking & combined) == SocketFlags::NonBlocking,
                  "NonBlocking & (NonBlocking | ReuseAddress) must equal NonBlocking");
  }

  SUBCASE("none_and_bind") {
    constexpr auto combined = SocketFlags::None & SocketFlags::Bind;

    REQUIRE(combined == SocketFlags::None);

    static_assert(combined == SocketFlags::None, "None & Bind must equal None");
  }

  SUBCASE("same_and_same") {
    constexpr auto combined = SocketFlags::Listen & SocketFlags::Listen;

    REQUIRE(combined == SocketFlags::Listen);

    static_assert(combined == SocketFlags::Listen, "Listen & Listen must equal Listen");
  }
}

// Bitwise XOR: symmetric difference of socket flags.
TEST_CASE("network/socket_flags/operator_xor") {
  SUBCASE("non_blocking_xor_non_blocking") {
    constexpr auto combined = SocketFlags::NonBlocking ^ SocketFlags::NonBlocking;

    REQUIRE(combined == SocketFlags::None);

    static_assert(combined == SocketFlags::None, "NonBlocking ^ NonBlocking must equal None");
  }

  SUBCASE("non_blocking_xor_reuse_address") {
    constexpr auto combined = SocketFlags::NonBlocking ^ SocketFlags::ReuseAddress;

    REQUIRE(static_cast<uint8_t>(combined) == 0x03);

    static_assert(static_cast<uint8_t>(combined) == 0x03, "NonBlocking ^ ReuseAddress must equal 0x03");
  }

  SUBCASE("combined_xor_one_flag") {
    constexpr auto combined = SocketFlags::NonBlocking | SocketFlags::ReuseAddress;

    REQUIRE((combined ^ SocketFlags::NonBlocking) == SocketFlags::ReuseAddress);

    static_assert((combined ^ SocketFlags::NonBlocking) == SocketFlags::ReuseAddress,
                  "(NonBlocking | ReuseAddress) ^ NonBlocking must equal ReuseAddress");
  }
}

} // namespace toy::network
