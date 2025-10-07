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

#include "core.hpp"

using namespace toy;

TEST_CASE("FixedVector constructors", "[core][fixed_vector]") {
  SECTION("Default constructor") {
    constexpr FixedVector<int, 10> emptyVec;

    REQUIRE(emptyVec.size() == 0);
    REQUIRE(emptyVec.capacity() == 10);

    // Compile-time checks
    STATIC_REQUIRE(emptyVec.size() == 0);
    STATIC_REQUIRE(emptyVec.capacity() == 10);
  }

  SECTION("Count constructor") {
    const FixedVector<int, 5> vec(3);

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 0);
    REQUIRE(vec[1] == 0);
    REQUIRE(vec[2] == 0);
  }

  SECTION("Count-value constructor") {
    const FixedVector<int, 5> vec(3, 42);

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 42);
    REQUIRE(vec[1] == 42);
    REQUIRE(vec[2] == 42);
  }

  SECTION("Copy constructor same capacity") {
    FixedVector<int, 5> source;
    source.push_back(1);
    source.push_back(2);
    source.push_back(3);
    const FixedVector<int, 5> & sourceRef = source;

    const FixedVector<int, 5> copy(sourceRef);
    REQUIRE(copy.size() == 3);
    REQUIRE(copy.capacity() == 5);
    REQUIRE(copy[0] == 1);
    REQUIRE(copy[1] == 2);
    REQUIRE(copy[2] == 3);
  }

  SECTION("Copy constructor different capacity") {
    FixedVector<int, 3> source;
    source.push_back(10);
    source.push_back(20);
    const FixedVector<int, 3> & sourceRef = source;

    const FixedVector<int, 5> copy(sourceRef);
    REQUIRE(copy.size() == 2);
    REQUIRE(copy.capacity() == 5);
    REQUIRE(copy[0] == 10);
    REQUIRE(copy[1] == 20);
  }

  SECTION("Move constructor same capacity") {
    FixedVector<int, 5> source;
    source.push_back(100);
    source.push_back(200);
    source.push_back(300);

    const FixedVector<int, 5> moved(std::move(source));

    REQUIRE(moved.size() == 3);
    REQUIRE(moved.capacity() == 5);
    REQUIRE(moved[0] == 100);
    REQUIRE(moved[1] == 200);
    REQUIRE(moved[2] == 300);

    // Source should be empty after move
    REQUIRE(source.size() == 0);
  }

  SECTION("Move constructor different capacity") {
    FixedVector<int, 3> source;
    source.push_back(1000);
    source.push_back(2000);

    const FixedVector<int, 5> moved(std::move(source));

    REQUIRE(moved.size() == 2);
    REQUIRE(moved.capacity() == 5);
    REQUIRE(moved[0] == 1000);
    REQUIRE(moved[1] == 2000);

    // Source should be empty after move
    REQUIRE(source.size() == 0);
  }
}
