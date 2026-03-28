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
  \file   endpoint.test.cpp
  \brief  Unit tests for \ref toy::network::Endpoint.
*/

#include <doctest/doctest.h>

#include "network.hpp"

namespace toy::network {

namespace endpoint_test {

// Concrete Endpoint implementation for testing port() and setPort(); virtuals are stubs.
class TestEndpoint : public Endpoint {
public:
  bool setHostname(const char *) noexcept override;
  bool isLocalHost() const noexcept override;
  void setLocalHost() noexcept override;
  const char * hostnameAsText() const noexcept override;
};

bool TestEndpoint::setHostname(const char *) noexcept {
  return true;
}

bool TestEndpoint::isLocalHost() const noexcept {
  return false;
}

void TestEndpoint::setLocalHost() noexcept {}

const char * TestEndpoint::hostnameAsText() const noexcept {
  return "";
}

} // namespace endpoint_test

// Check object structure.
TEST_CASE("network/endpoint/object_structure") {
  static_assert(!std::is_trivial_v<Endpoint>, "Endpoint must not be trivial (has non-trivial default init)");
  static_assert(!std::is_trivially_copyable_v<Endpoint>, "Endpoint must not be trivially copyable");
  static_assert(!std::is_standard_layout_v<Endpoint>, "Endpoint must not have standard layout");
}

// Default port is zero.
TEST_CASE("network/endpoint/port_default") {
  endpoint_test::TestEndpoint ep;

  REQUIRE(ep.port() == 0);
}

// setPort stores the value; port() returns it.
TEST_CASE("network/endpoint/set_port_and_port") {
  endpoint_test::TestEndpoint ep;

  ep.setPort(8080);

  REQUIRE(ep.port() == 8080);
}

// setPort can overwrite the previous value.
TEST_CASE("network/endpoint/set_port_overwrite") {
  endpoint_test::TestEndpoint ep;

  ep.setPort(80);
  ep.setPort(443);

  REQUIRE(ep.port() == 443);
}

// port() and setPort() work when called through base pointer.
TEST_CASE("network/endpoint/port_via_base") {
  endpoint_test::TestEndpoint ep;
  Endpoint * base = &ep;

  base->setPort(9000);

  REQUIRE(base->port() == 9000);
}

} // namespace toy::network
