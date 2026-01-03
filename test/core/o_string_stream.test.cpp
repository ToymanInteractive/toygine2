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

#include <string>

#include <catch2/catch_test_macros.hpp>

#include "core.hpp"

namespace toy {

TEST_CASE("OStringStream constructors", "[core][o_string_stream]") {
  SECTION("Default constructor") {
    constexpr OStringStream<FixedString<32>> emptyStream;

    REQUIRE(emptyStream.str().size() == 0);
    REQUIRE(std::strcmp(emptyStream.str().c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(emptyStream.str().size() == 0);
    STATIC_REQUIRE(cstrcmp(emptyStream.str().c_str(), "") == 0);
  }

  SECTION("Constructor from FixedString") {
    constexpr FixedString<16> source("Hello");
    constexpr OStringStream<FixedString<32>> stream(source);

    REQUIRE(stream.str().size() == 5);
    REQUIRE(std::strcmp(stream.str().c_str(), "Hello") == 0);

    // Compile-time checks
    STATIC_REQUIRE(stream.str().size() == 5);
    STATIC_REQUIRE(cstrcmp(stream.str().c_str(), "Hello") == 0);
  }

  SECTION("Constructor from std::string") {
    std::string source = "World";
    OStringStream<FixedString<64>> stream(source);

    REQUIRE(stream.str().size() == 5);
    REQUIRE(std::strcmp(stream.str().c_str(), "World") == 0);
  }

  SECTION("Constructor from CStringView") {
    constexpr CStringView view("Test");
    constexpr OStringStream<FixedString<32>> stream(view);

    REQUIRE(stream.str().size() == 4);
    REQUIRE(std::strcmp(stream.str().c_str(), "Test") == 0);

    // Compile-time checks
    STATIC_REQUIRE(stream.str().size() == 4);
    STATIC_REQUIRE(cstrcmp(stream.str().c_str(), "Test") == 0);
  }

  SECTION("Constructor with empty string") {
    constexpr FixedString<16> empty;
    constexpr OStringStream<FixedString<32>> stream(empty);

    REQUIRE(stream.str().size() == 0);
    REQUIRE(std::strcmp(stream.str().c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(stream.str().size() == 0);
    STATIC_REQUIRE(cstrcmp(stream.str().c_str(), "") == 0);
  }
}

TEST_CASE("OStringStream assignment operators", "[core][o_string_stream]") {
  SECTION("Copy assignment") {
    constexpr OStringStream<FixedString<32>> source(FixedString<16>("Source"));
    OStringStream<FixedString<32>> target;

    target = source;

    REQUIRE(target.str().size() == 6);
    REQUIRE(std::strcmp(target.str().c_str(), "Source") == 0);
    REQUIRE(source.str().size() == 6);
    REQUIRE(std::strcmp(source.str().c_str(), "Source") == 0);
  }

  SECTION("Move assignment") {
    OStringStream<FixedString<32>> source(FixedString<16>("Move"));
    OStringStream<FixedString<32>> target;

    target = std::move(source);

    REQUIRE(target.str().size() == 4);
    REQUIRE(std::strcmp(target.str().c_str(), "Move") == 0);
  }
}

TEST_CASE("OStringStream swap", "[core][o_string_stream]") {
  SECTION("Swap two streams with content") {
    OStringStream<FixedString<32>> stream1(FixedString<16>("First"));
    OStringStream<FixedString<32>> stream2(FixedString<16>("Second"));

    stream1.swap(stream2);

    REQUIRE(stream1.str().size() == 6);
    REQUIRE(std::strcmp(stream1.str().c_str(), "Second") == 0);
    REQUIRE(stream2.str().size() == 5);
    REQUIRE(std::strcmp(stream2.str().c_str(), "First") == 0);
  }

  SECTION("Swap with empty stream") {
    OStringStream<FixedString<32>> stream1(FixedString<16>("Content"));
    OStringStream<FixedString<32>> stream2;

    stream1.swap(stream2);

    REQUIRE(stream1.str().size() == 0);
    REQUIRE(std::strcmp(stream1.str().c_str(), "") == 0);
    REQUIRE(stream2.str().size() == 7);
    REQUIRE(std::strcmp(stream2.str().c_str(), "Content") == 0);
  }

  SECTION("Self-swap") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));

    stream.swap(stream);

    REQUIRE(stream.str().size() == 4);
    REQUIRE(std::strcmp(stream.str().c_str(), "Test") == 0);
  }

  SECTION("Swap empty streams") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;

    stream1.swap(stream2);

    REQUIRE(stream1.str().size() == 0);
    REQUIRE(std::strcmp(stream1.str().c_str(), "") == 0);
    REQUIRE(stream2.str().size() == 0);
    REQUIRE(std::strcmp(stream2.str().c_str(), "") == 0);
  }
}

} // namespace toy
