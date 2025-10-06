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
    constexpr FixedVector<int, 5> vec(3);

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 0);
    REQUIRE(vec[1] == 0);
    REQUIRE(vec[2] == 0);

    // Compile-time checks
    STATIC_REQUIRE(vec.size() == 3);
    STATIC_REQUIRE(vec.capacity() == 5);
  }

  SECTION("Count-value constructor") {
    constexpr FixedVector<int, 5> vec(3, 42);

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 42);
    REQUIRE(vec[1] == 42);
    REQUIRE(vec[2] == 42);

    // Compile-time checks
    STATIC_REQUIRE(vec.size() == 3);
    STATIC_REQUIRE(vec.capacity() == 5);
  }

  SECTION("Copy constructor same capacity") {
    FixedVector<int, 5> source;
    source.push_back(1);
    source.push_back(2);
    source.push_back(3);

    FixedVector<int, 5> copy(source);

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

    FixedVector<int, 5> copy(source);

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

    FixedVector<int, 5> moved(std::move(source));

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

    FixedVector<int, 5> moved(std::move(source));

    REQUIRE(moved.size() == 2);
    REQUIRE(moved.capacity() == 5);
    REQUIRE(moved[0] == 1000);
    REQUIRE(moved[1] == 2000);

    // Source should be empty after move
    REQUIRE(source.size() == 0);
  }

  SECTION("Initializer list constructor") {
    FixedVector<int, 5> vec{1, 2, 3, 4};

    REQUIRE(vec.size() == 4);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
    REQUIRE(vec[3] == 4);
  }

  SECTION("Empty initializer list constructor") {
    FixedVector<int, 5> vec{};

    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 5);
  }

  SECTION("Single element initializer list constructor") {
    FixedVector<int, 5> vec{42};

    REQUIRE(vec.size() == 1);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 42);
  }

  SECTION("Different types constructor") {
    constexpr FixedVector<std::string, 3> strVec(2, "test");

    REQUIRE(strVec.size() == 2);
    REQUIRE(strVec.capacity() == 3);
    REQUIRE(strVec[0] == "test");
    REQUIRE(strVec[1] == "test");
  }

  SECTION("Double type constructor") {
    constexpr FixedVector<double, 4> doubleVec(3, 3.14);

    REQUIRE(doubleVec.size() == 3);
    REQUIRE(doubleVec.capacity() == 4);
    REQUIRE(doubleVec[0] == 3.14);
    REQUIRE(doubleVec[1] == 3.14);
    REQUIRE(doubleVec[2] == 3.14);
  }

  SECTION("Bool type constructor") {
    constexpr FixedVector<bool, 3> boolVec(2, true);

    REQUIRE(boolVec.size() == 2);
    REQUIRE(boolVec.capacity() == 3);
    REQUIRE(boolVec[0] == true);
    REQUIRE(boolVec[1] == true);
  }

  SECTION("Constexpr operations") {
    constexpr FixedVector<int, 5> vec1(3, 10);
    constexpr FixedVector<int, 5> vec2(vec1);
    constexpr FixedVector<int, 5> vec3(std::move(vec1));

    STATIC_REQUIRE(vec1.size() == 0); // vec1 is empty after move
    STATIC_REQUIRE(vec2.size() == 3);
    STATIC_REQUIRE(vec3.size() == 3);
    STATIC_REQUIRE(vec2[0] == 10);
    STATIC_REQUIRE(vec3[0] == 10);
  }

  SECTION("Edge cases") {
    // Maximum capacity
    constexpr FixedVector<int, 1> maxVec(1, 999);
    REQUIRE(maxVec.size() == 1);
    REQUIRE(maxVec.capacity() == 1);
    REQUIRE(maxVec[0] == 999);

    // Zero count
    constexpr FixedVector<int, 5> zeroVec(0);
    REQUIRE(zeroVec.size() == 0);
    REQUIRE(zeroVec.capacity() == 5);
  }
}
