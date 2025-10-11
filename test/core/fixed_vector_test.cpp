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

  SECTION("Initializer list constructor") {
    const FixedVector<int, 5> vec{1, 2, 3, 4};

    REQUIRE(vec.size() == 4);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
    REQUIRE(vec[3] == 4);
  }

  SECTION("Empty initializer list constructor") {
    const FixedVector<int, 5> vec{};

    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 5);
  }

  SECTION("Single element initializer list constructor") {
    const FixedVector<int, 5> vec{42};

    REQUIRE(vec.size() == 1);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 42);
  }

  SECTION("Different types constructor") {
    const FixedVector<FixedString<8>, 3> strVec(2, FixedString<8>("test"));

    REQUIRE(strVec.size() == 2);
    REQUIRE(strVec.capacity() == 3);
    REQUIRE(strVec[0] == "test");
    REQUIRE(strVec[1] == "test");
  }

  SECTION("Double type constructor") {
    const FixedVector<double, 4> doubleVec(3, 3.14);

    REQUIRE(doubleVec.size() == 3);
    REQUIRE(doubleVec.capacity() == 4);
    REQUIRE(doubleVec[0] == 3.14);
    REQUIRE(doubleVec[1] == 3.14);
    REQUIRE(doubleVec[2] == 3.14);
  }

  SECTION("Bool type constructor") {
    const FixedVector<bool, 3> boolVec(2, true);

    REQUIRE(boolVec.size() == 2);
    REQUIRE(boolVec.capacity() == 3);
    REQUIRE(boolVec[0] == true);
    REQUIRE(boolVec[1] == true);
  }

  SECTION("Edge cases") {
    // Maximum capacity
    const FixedVector<int, 1> maxVec(1, 999);
    REQUIRE(maxVec.size() == 1);
    REQUIRE(maxVec.capacity() == 1);
    REQUIRE(maxVec[0] == 999);

    // Zero count
    const FixedVector<int, 5> zeroVec(0);
    REQUIRE(zeroVec.size() == 0);
    REQUIRE(zeroVec.capacity() == 5);
  }
}

TEST_CASE("FixedVector assignment operators", "[core][fixed_vector]") {
  SECTION("Copy assignment operator (same capacity)") {
    const FixedVector<int, 5> vec1{1, 2, 3};
    FixedVector<int, 5> vec2{4, 5};

    vec2 = vec1;

    REQUIRE(vec2.size() == vec1.size());
    REQUIRE(vec2[0] == vec1[0]);
    REQUIRE(vec2[1] == vec1[1]);
    REQUIRE(vec2[2] == vec1[2]);
  }

  SECTION("Copy assignment operator (different capacity)") {
    const FixedVector<int, 3> vec1{1, 2, 3};
    FixedVector<int, 5> vec2{4, 5};

    vec2 = vec1;

    REQUIRE(vec2.size() == vec1.size());
    REQUIRE(vec2[0] == vec1[0]);
    REQUIRE(vec2[1] == vec1[1]);
    REQUIRE(vec2[2] == vec1[2]);
  }

  SECTION("Move assignment operator (same capacity)") {
    FixedVector<int, 5> vec1{1, 2, 3};
    FixedVector<int, 5> vec2{4, 5};

    vec2 = std::move(vec1);

    REQUIRE(vec2.size() == 3);
    REQUIRE(vec2[0] == 1);
    REQUIRE(vec2[1] == 2);
    REQUIRE(vec2[2] == 3);

    // Source vector should be cleared
    REQUIRE(vec1.size() == 0);
  }

  SECTION("Move assignment operator (different capacity)") {
    FixedVector<int, 3> vec1{1, 2, 3};
    FixedVector<int, 5> vec2{4, 5};

    vec2 = std::move(vec1);

    REQUIRE(vec2.size() == 3);
    REQUIRE(vec2[0] == 1);
    REQUIRE(vec2[1] == 2);
    REQUIRE(vec2[2] == 3);

    // Source vector should be cleared
    REQUIRE(vec1.size() == 0);
  }

  SECTION("Initializer list assignment operator") {
    FixedVector<int, 5> vec{1, 2, 3};

    vec = {4, 5, 6, 7};

    REQUIRE(vec.size() == 4);
    REQUIRE(vec[0] == 4);
    REQUIRE(vec[1] == 5);
    REQUIRE(vec[2] == 6);
    REQUIRE(vec[3] == 7);
  }

  SECTION("Initializer list assignment operator (empty)") {
    FixedVector<int, 5> vec{1, 2, 3};

    vec = {};

    REQUIRE(vec.size() == 0);
  }

  SECTION("Initializer list assignment operator (single element)") {
    FixedVector<int, 5> vec{1, 2, 3};

    vec = {42};

    REQUIRE(vec.size() == 1);
    REQUIRE(vec[0] == 42);
  }

  SECTION("Self-assignment (copy)") {
    FixedVector<int, 5> vec{1, 2, 3};

    vec = vec;

    REQUIRE(vec.size() == 3);
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
  }

  SECTION("Self-assignment (move)") {
    FixedVector<int, 5> vec{1, 2, 3};

    vec = std::move(vec);

    REQUIRE(vec.size() == 3);
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
  }

  SECTION("Assignment with different types") {
    FixedVector<std::string, 3> vec1{"hello", "world"};
    FixedVector<std::string, 5> vec2{"test"};

    vec2 = vec1;

    REQUIRE(vec2.size() == 2);
    REQUIRE(vec2[0] == "hello");
    REQUIRE(vec2[1] == "world");
  }

  SECTION("Assignment with complex types") {
    FixedVector<std::vector<int>, 2> vec1;
    vec1.push_back(std::vector<int>{1, 2, 3});
    vec1.push_back(std::vector<int>{4, 5});

    FixedVector<std::vector<int>, 3> vec2;
    vec2.push_back(std::vector<int>{6, 7, 8});

    vec2 = vec1;

    REQUIRE(vec2.size() == 2);
    REQUIRE(vec2[0].size() == 3);
    REQUIRE(vec2[0][0] == 1);
    REQUIRE(vec2[0][1] == 2);
    REQUIRE(vec2[0][2] == 3);
    REQUIRE(vec2[1].size() == 2);
    REQUIRE(vec2[1][0] == 4);
    REQUIRE(vec2[1][1] == 5);
  }
}
