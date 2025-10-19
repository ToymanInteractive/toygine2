//
// Copyright (c) 2025 by Toyman Interactive
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

#include <catch2/catch_test_macros.hpp>

#include "app.hpp"

using namespace toy::app;

TEST_CASE("Version constructors", "[app][version]") {
  SECTION("Default constructor") {
    const Version version;
    REQUIRE(version.major == 0);
    REQUIRE(version.minor == 0);
    REQUIRE(version.maintenance == 0);
    REQUIRE(version.revision == 0);

    // Compile-time checks
    constexpr Version constexprVersion;
    STATIC_REQUIRE(constexprVersion.major == 0);
    STATIC_REQUIRE(constexprVersion.minor == 0);
    STATIC_REQUIRE(constexprVersion.maintenance == 0);
    STATIC_REQUIRE(constexprVersion.revision == 0);
  }
}
