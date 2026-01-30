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

#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "core.hpp"

namespace toy {

TEST_CASE("FixedVector constructors", "[core][fixed_vector]") {
  SECTION("Default constructor") {
    constexpr FixedVector<int, 10> emptyVec;

    static_assert(emptyVec.size() == 0);
    static_assert(emptyVec.capacity() == 10);
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
    const FixedVector<double, 4> doubleVec(3, 3.15);

    REQUIRE(doubleVec.size() == 3);
    REQUIRE(doubleVec.capacity() == 4);
    REQUIRE(doubleVec[0] == 3.15);
    REQUIRE(doubleVec[1] == 3.15);
    REQUIRE(doubleVec[2] == 3.15);
  }

  SECTION("Bool type constructor") {
    const FixedVector<bool, 3> boolVec(2, true);

    REQUIRE(boolVec.size() == 2);
    REQUIRE(boolVec.capacity() == 3);
    REQUIRE(boolVec[0]);
    REQUIRE(boolVec[1]);
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

  SECTION("Iterator constructor") {
    constexpr array<int, 4> sourceArray{1, 2, 3, 4};
    const FixedVector<int, 5> vec(sourceArray.begin(), sourceArray.end());

    REQUIRE(vec.size() == 4);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
    REQUIRE(vec[3] == 4);
  }

  SECTION("Iterator constructor with array") {
    constexpr array<int, 3> sourceArray{10, 20, 30};
    const FixedVector<int, 5> vec(std::begin(sourceArray), std::end(sourceArray));

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
    REQUIRE(vec[2] == 30);
  }

  SECTION("Iterator constructor with empty range") {
    constexpr array<int, 0> emptyArray{};
    const FixedVector<int, 5> vec(emptyArray.begin(), emptyArray.end());

    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 5);
  }

  SECTION("Iterator constructor with different types") {
    constexpr array<FixedString<10>, 2> sourceArray{FixedString<10>("hello"), FixedString<10>("world")};
    const FixedVector<FixedString<10>, 5> vec(sourceArray.begin(), sourceArray.end());

    REQUIRE(vec.size() == 2);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == "hello");
    REQUIRE(vec[1] == "world");
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

TEST_CASE("FixedVector assign methods", "[core][fixed_vector]") {
  SECTION("Assign count and value") {
    FixedVector<int, 5> vec{1, 2, 3};

    vec.assign(2, 42);

    REQUIRE(vec.size() == 2);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 42);
    REQUIRE(vec[1] == 42);
  }

  SECTION("Assign count and value with existing elements") {
    FixedVector<int, 5> vec{1, 2, 3, 4, 5};

    vec.assign(3, 99);

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 99);
    REQUIRE(vec[1] == 99);
    REQUIRE(vec[2] == 99);
  }

  SECTION("Assign count and value with empty vector") {
    FixedVector<int, 5> vec;

    vec.assign(4, 10);

    REQUIRE(vec.size() == 4);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 10);
    REQUIRE(vec[2] == 10);
    REQUIRE(vec[3] == 10);
  }

  SECTION("Assign count and value with zero count") {
    FixedVector<int, 5> vec{1, 2, 3};

    vec.assign(0, 42);

    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 5);
  }

  SECTION("Assign from iterator range") {
    constexpr array<int, 4> source{10, 20, 30, 40};
    FixedVector<int, 5> vec{1, 2, 3};

    vec.assign(source.begin(), source.end());

    REQUIRE(vec.size() == 4);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
    REQUIRE(vec[2] == 30);
    REQUIRE(vec[3] == 40);
  }

  SECTION("Assign from iterator range with array") {
    constexpr array<int, 3> sourceArray{100, 200, 300};
    FixedVector<int, 5> vec{1, 2, 3, 4};

    vec.assign(std::begin(sourceArray), std::end(sourceArray));

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 100);
    REQUIRE(vec[1] == 200);
    REQUIRE(vec[2] == 300);
  }

  SECTION("Assign from empty iterator range") {
    constexpr array<int, 0> emptyArray{};
    FixedVector<int, 5> vec{1, 2, 3};

    vec.assign(emptyArray.begin(), emptyArray.end());

    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 5);
  }

  SECTION("Assign from iterator range with different types") {
    constexpr array<FixedString<10>, 2> source{FixedString<10>("foo"), FixedString<10>("bar")};
    FixedVector<FixedString<10>, 5> vec{FixedString<10>("old")};

    vec.assign(source.begin(), source.end());

    REQUIRE(vec.size() == 2);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == "foo");
    REQUIRE(vec[1] == "bar");
  }

  SECTION("Assign from initializer list") {
    FixedVector<int, 5> vec{1, 2, 3};

    vec.assign({4, 5, 6, 7});

    REQUIRE(vec.size() == 4);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 4);
    REQUIRE(vec[1] == 5);
    REQUIRE(vec[2] == 6);
    REQUIRE(vec[3] == 7);
  }

  SECTION("Assign from empty initializer list") {
    FixedVector<int, 5> vec{1, 2, 3, 4, 5};

    vec.assign({});

    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 5);
  }

  SECTION("Assign from single element initializer list") {
    FixedVector<int, 5> vec{1, 2, 3};

    vec.assign({999});

    REQUIRE(vec.size() == 1);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 999);
  }

  SECTION("Assign from initializer list with different sizes") {
    FixedVector<int, 5> vec{1, 2, 3, 4, 5};

    vec.assign({10, 20});

    REQUIRE(vec.size() == 2);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);

    vec.assign({100, 200, 300, 400, 500});

    REQUIRE(vec.size() == 5);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 100);
    REQUIRE(vec[4] == 500);
  }

  SECTION("Assign with complex types") {
    const std::vector<std::vector<int>> source{{1, 2}, {3, 4, 5}};
    FixedVector<std::vector<int>, 3> vec;

    vec.assign(source.begin(), source.end());

    REQUIRE(vec.size() == 2);
    REQUIRE(vec.capacity() == 3);
    REQUIRE(vec[0].size() == 2);
    REQUIRE(vec[0][0] == 1);
    REQUIRE(vec[0][1] == 2);
    REQUIRE(vec[1].size() == 3);
    REQUIRE(vec[1][0] == 3);
    REQUIRE(vec[1][1] == 4);
    REQUIRE(vec[1][2] == 5);
  }

  SECTION("Assign chained operations") {
    FixedVector<int, 5> vec;

    vec.assign(3, 1);
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[2] == 1);

    vec.assign({2, 3, 4, 5});
    REQUIRE(vec.size() == 4);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[3] == 5);

    constexpr array<int, 2> a{10, 20};
    vec.assign(a.begin(), a.end());
    REQUIRE(vec.size() == 2);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
  }

  SECTION("Assign edge cases") {
    // Maximum capacity assignment
    FixedVector<int, 1> maxVec;
    maxVec.assign(1, 999);
    REQUIRE(maxVec.size() == 1);
    REQUIRE(maxVec.capacity() == 1);
    REQUIRE(maxVec[0] == 999);

    // Assign same value multiple times
    FixedVector<int, 5> sameVec;
    sameVec.assign(5, 42);
    REQUIRE(sameVec.size() == 5);
    for (size_t i = 0; i < sameVec.size(); ++i) {
      REQUIRE(sameVec[i] == 42);
    }
  }
}

TEST_CASE("FixedVector at methods", "[core][fixed_vector]") {
  SECTION("Non-const at method") {
    FixedVector<int, 5> vec{10, 20, 30};

    REQUIRE(vec.at(0) == 10);
    REQUIRE(vec.at(1) == 20);
    REQUIRE(vec.at(2) == 30);

    // Modify through at()
    vec.at(1) = 99;
    REQUIRE(vec.at(1) == 99);
  }

  SECTION("Const at method") {
    const FixedVector<int, 5> vec{10, 20, 30};

    REQUIRE(vec.at(0) == 10);
    REQUIRE(vec.at(1) == 20);
    REQUIRE(vec.at(2) == 30);
  }
}

TEST_CASE("FixedVector operator[] methods", "[core][fixed_vector]") {
  SECTION("Non-const operator[]") {
    FixedVector<int, 5> vec{10, 20, 30};

    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
    REQUIRE(vec[2] == 30);

    // Modify through operator[]
    vec[1] = 99;
    REQUIRE(vec[1] == 99);
  }

  SECTION("Const operator[]") {
    const FixedVector<int, 5> vec{10, 20, 30};

    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
    REQUIRE(vec[2] == 30);
  }

  SECTION("String elements") {
    FixedVector<std::string, 3> stringVec{"foo", "bar", "baz"};

    REQUIRE(stringVec[0] == "foo");
    REQUIRE(stringVec[1] == "bar");
    REQUIRE(stringVec[2] == "baz");

    // Modify through operator[]
    stringVec[1] = "qux";
    REQUIRE(stringVec[1] == "qux");
  }
}

TEST_CASE("FixedVector front methods", "[core][fixed_vector]") {
  SECTION("Non-const front method") {
    FixedVector<int, 5> vec{10, 20, 30};

    REQUIRE(vec.front() == 10);

    // Modify through front()
    vec.front() = 99;
    REQUIRE(vec.front() == 99);
    REQUIRE(vec[0] == 99);
  }

  SECTION("Const front method") {
    const FixedVector<int, 5> vec{10, 20, 30};

    REQUIRE(vec.front() == 10);
  }

  SECTION("Single element") {
    FixedVector<int, 5> vec{42};

    REQUIRE(vec.front() == 42);
    REQUIRE(vec.front() == vec.back());

    vec.front() = 100;
    REQUIRE(vec.front() == 100);
  }

  SECTION("String elements") {
    FixedVector<std::string, 3> stringVec{"foo", "bar", "baz"};

    REQUIRE(stringVec.front() == "foo");

    stringVec.front() = "qux";
    REQUIRE(stringVec.front() == "qux");
  }
}

TEST_CASE("FixedVector back methods", "[core][fixed_vector]") {
  SECTION("Non-const back method") {
    FixedVector<int, 5> vec{10, 20, 30};

    REQUIRE(vec.back() == 30);

    // Modify through back()
    vec.back() = 99;
    REQUIRE(vec.back() == 99);
    REQUIRE(vec[2] == 99);
  }

  SECTION("Const back method") {
    const FixedVector<int, 5> vec{10, 20, 30};

    REQUIRE(vec.back() == 30);
  }

  SECTION("Single element") {
    FixedVector<int, 5> vec{42};

    REQUIRE(vec.back() == 42);
    REQUIRE(vec.back() == vec.front());

    vec.back() = 100;
    REQUIRE(vec.back() == 100);
  }

  SECTION("String elements") {
    FixedVector<std::string, 3> stringVec{"foo", "bar", "baz"};

    REQUIRE(stringVec.back() == "baz");

    stringVec.back() = "qux";
    REQUIRE(stringVec.back() == "qux");
  }

  SECTION("Multiple elements - front and back different") {
    FixedVector<int, 5> vec{1, 2, 3, 4, 5};

    REQUIRE(vec.front() == 1);
    REQUIRE(vec.back() == 5);

    vec.front() = 10;
    vec.back() = 50;

    REQUIRE(vec.front() == 10);
    REQUIRE(vec.back() == 50);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[4] == 50);
  }
}

TEST_CASE("FixedVector data methods", "[core][fixed_vector]") {
  SECTION("Non-const data method") {
    FixedVector<int, 5> vec{10, 20, 30};

    int * dataPtr = vec.data();

    REQUIRE(dataPtr != nullptr);
    REQUIRE(dataPtr == &vec[0]);
    REQUIRE(*dataPtr == 10);

    // Modify through data pointer
    *dataPtr = 99;
    REQUIRE(vec[0] == 99);

    // Access elements through pointer arithmetic
    REQUIRE(dataPtr[1] == 20);
    REQUIRE(dataPtr[2] == 30);
  }

  SECTION("Const data method") {
    const FixedVector<int, 5> vec{10, 20, 30};

    const int * dataPtr = vec.data();

    REQUIRE(dataPtr != nullptr);
    REQUIRE(dataPtr == &vec[0]);
    REQUIRE(*dataPtr == 10);

    // Read elements through pointer arithmetic
    REQUIRE(dataPtr[1] == 20);
    REQUIRE(dataPtr[2] == 30);
  }

  SECTION("Empty vector") {
    FixedVector<int, 5> emptyVec;

    int * dataPtr = emptyVec.data();

    REQUIRE(dataPtr != nullptr);
    REQUIRE(emptyVec.size() == 0);
  }

  SECTION("Single element") {
    FixedVector<int, 5> vec{42};

    int * dataPtr = vec.data();

    REQUIRE(*dataPtr == 42);
    REQUIRE(dataPtr == &vec.front());
    REQUIRE(dataPtr == &vec.back());
  }

  SECTION("String elements") {
    FixedVector<std::string, 3> stringVec{"foo", "bar", "baz"};

    std::string * dataPtr = stringVec.data();

    REQUIRE(dataPtr != nullptr);
    REQUIRE(dataPtr[0] == "foo");
    REQUIRE(dataPtr[1] == "bar");
    REQUIRE(dataPtr[2] == "baz");

    // Modify through data pointer
    dataPtr[1] = "qux";
    REQUIRE(stringVec[1] == "qux");
  }

  SECTION("Data pointer remains valid after modifications") {
    FixedVector<int, 5> vec{1, 2, 3};

    int * dataPtr = vec.data();

    vec.push_back(4);
    REQUIRE(*dataPtr == 1);
    REQUIRE(dataPtr[0] == 1);
    REQUIRE(dataPtr[1] == 2);
    REQUIRE(dataPtr[2] == 3);
    REQUIRE(dataPtr[3] == 4);

    // Same pointer after modification
    REQUIRE(dataPtr == vec.data());
  }
}

TEST_CASE("FixedVector iterator methods", "[core][fixed_vector]") {
  SECTION("Begin methods") {
    FixedVector<int, 5> vec{10, 20, 30};

    // Non-const begin
    auto it = vec.begin();
    REQUIRE(it == vec.data());
    REQUIRE(*it == 10);
    REQUIRE(it != vec.end());

    // Const begin
    const FixedVector<int, 5> constVec{10, 20, 30};
    auto constIt = constVec.begin();
    REQUIRE(constIt == constVec.data());
    REQUIRE(*constIt == 10);
    REQUIRE(constIt != constVec.end());

    // cbegin
    auto cbeginIt = constVec.cbegin();
    REQUIRE(cbeginIt == constVec.begin());
    REQUIRE(*cbeginIt == 10);
  }

  SECTION("End methods") {
    FixedVector<int, 5> vec{10, 20, 30};

    // Non-const end
    auto it = vec.end();
    REQUIRE(it == vec.data() + vec.size());
    REQUIRE(it != vec.begin());

    // Const end
    const FixedVector<int, 5> constVec{10, 20, 30};
    auto constIt = constVec.end();
    REQUIRE(constIt == constVec.data() + constVec.size());
    REQUIRE(constIt != constVec.begin());

    // cend
    auto cendIt = constVec.cend();
    REQUIRE(cendIt == constVec.end());
  }

  SECTION("Empty vector iterators") {
    FixedVector<int, 5> emptyVec;
    REQUIRE(emptyVec.begin() == emptyVec.end());

    constexpr FixedVector<int, 5> constEmptyVec;
    REQUIRE(constEmptyVec.begin() == constEmptyVec.end());
    REQUIRE(constEmptyVec.cbegin() == constEmptyVec.cend());
  }

  SECTION("Forward iteration") {
    FixedVector<int, 5> vec{10, 20, 30};

    auto it = vec.begin();
    REQUIRE(*it == 10);
    ++it;
    REQUIRE(*it == 20);
    ++it;
    REQUIRE(*it == 30);
    ++it;
    REQUIRE(it == vec.end());
  }

  SECTION("Iterator dereference and modification") {
    FixedVector<int, 5> vec{1, 2, 3};

    *vec.begin() = 99;
    REQUIRE(vec[0] == 99);

    auto it = vec.begin();
    ++it;
    *it = 88;
    REQUIRE(vec[1] == 88);
  }

  SECTION("Range-based for loop") {
    FixedVector<int, 5> vec{1, 2, 3};
    std::vector<int> result;

    for (auto & element : vec) {
      result.push_back(element);
      element = element * 2;
    }

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 1);
    REQUIRE(result[1] == 2);
    REQUIRE(result[2] == 3);
    REQUIRE(vec[0] == 2);
    REQUIRE(vec[1] == 4);
    REQUIRE(vec[2] == 6);
  }

  SECTION("Range-based for loop with const vector") {
    const FixedVector<int, 5> vec{1, 2, 3};
    std::vector<int> result;

    for (const auto & element : vec) {
      result.push_back(element);
    }

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 1);
    REQUIRE(result[1] == 2);
    REQUIRE(result[2] == 3);
  }

  SECTION("Iterator arithmetic") {
    FixedVector<int, 5> vec{10, 20, 30, 40};

    auto it = vec.begin();
    REQUIRE(*(it + 0) == 10);
    REQUIRE(*(it + 1) == 20);
    REQUIRE(*(it + 2) == 30);
    REQUIRE(*(it + 3) == 40);

    REQUIRE(it + 4 == vec.end());
    REQUIRE(vec.end() - vec.begin() == 4);
  }

  SECTION("Reverse begin methods") {
    FixedVector<int, 5> vec{10, 20, 30};

    // Non-const rbegin
    auto rit = vec.rbegin();
    REQUIRE(*rit == 30);
    REQUIRE(rit != vec.rend());

    // Const rbegin
    const FixedVector<int, 5> constVec{10, 20, 30};
    auto constRit = constVec.rbegin();
    REQUIRE(*constRit == 30);
    REQUIRE(constRit != constVec.rend());

    // crbegin
    auto rcbeginIt = constVec.crbegin();
    REQUIRE(rcbeginIt == constVec.rbegin());
    REQUIRE(*rcbeginIt == 30);
  }

  SECTION("Reverse end methods") {
    FixedVector<int, 5> vec{10, 20, 30};

    // Non-const rend
    auto rit = vec.rend();
    REQUIRE(rit != vec.rbegin());

    // Const rend
    const FixedVector<int, 5> constVec{10, 20, 30};
    auto constRend = constVec.rend();
    REQUIRE(constRend != constVec.rbegin());

    // crend
    auto rcendIt = constVec.crend();
    REQUIRE(rcendIt == constVec.rend());
  }

  SECTION("Reverse iteration") {
    FixedVector<int, 5> vec{10, 20, 30};

    auto rit = vec.rbegin();
    REQUIRE(*rit == 30);
    ++rit;
    REQUIRE(*rit == 20);
    ++rit;
    REQUIRE(*rit == 10);
    ++rit;
    REQUIRE(rit == vec.rend());
  }

  SECTION("Reverse iteration empty vector") {
    FixedVector<int, 5> emptyVec;
    REQUIRE(emptyVec.rbegin() == emptyVec.rend());

    const FixedVector<int, 5> constEmptyVec;
    REQUIRE(constEmptyVec.rbegin() == constEmptyVec.rend());
    REQUIRE(constEmptyVec.crbegin() == constEmptyVec.crend());
  }

  SECTION("Reverse iterator modification") {
    FixedVector<int, 5> vec{10, 20, 30};

    *vec.rbegin() = 99;
    REQUIRE(vec[2] == 99);

    auto rit = vec.rbegin();
    ++rit;
    *rit = 88;
    REQUIRE(vec[1] == 88);
  }

  SECTION("String elements with iterators") {
    FixedVector<std::string, 4> stringVec{"a", "b", "c"};

    auto it = stringVec.begin();
    REQUIRE(*it == "a");
    ++it;
    REQUIRE(*it == "b");
    ++it;
    REQUIRE(*it == "c");

    // Reverse iteration
    auto rit = stringVec.rbegin();
    REQUIRE(*rit == "c");
    ++rit;
    REQUIRE(*rit == "b");
    ++rit;
    REQUIRE(*rit == "a");
  }

  SECTION("Iterator comparison with std algorithms") {
    FixedVector<int, 5> vec{3, 1, 4, 1, 5};

    // std::find
    auto found = std::find(vec.begin(), vec.end(), 4);
    REQUIRE(found != vec.end());
    REQUIRE(*found == 4);

    // std::min_element
    auto minIt = std::min_element(vec.begin(), vec.end());
    REQUIRE(minIt != vec.end());
    REQUIRE(*minIt == 1);

    // std::max_element
    auto maxIt = std::max_element(vec.begin(), vec.end());
    REQUIRE(maxIt != vec.end());
    REQUIRE(*maxIt == 5);
  }

  SECTION("Const iterator immutability") {
    const FixedVector<int, 5> constVec{1, 2, 3};

    // Should compile - reading is allowed
    auto it = constVec.begin();
    int value = *it;

    REQUIRE(value == 1);
  }
}

TEST_CASE("FixedVector size and capacity methods", "[core][fixed_vector]") {
  SECTION("Empty method") {
    FixedVector<int, 5> emptyVec{};
    REQUIRE(emptyVec.empty());

    FixedVector<int, 5> nonEmptyVec{1, 2, 3};
    REQUIRE(nonEmptyVec.empty() == false);

    const FixedVector<int, 5> constEmptyVec{};
    REQUIRE(constEmptyVec.empty());

    const FixedVector<int, 5> constNonEmptyVec{1, 2};
    REQUIRE(constNonEmptyVec.empty() == false);

    constexpr FixedVector<int, 5> constexprEmptyVec{};
    static_assert(constexprEmptyVec.empty());
  }

  SECTION("Size method") {
    FixedVector<int, 5> emptyVec{};
    REQUIRE(emptyVec.size() == 0);

    FixedVector<int, 5> vec{1, 2, 3};
    REQUIRE(vec.size() == 3);

    vec.push_back(4);
    REQUIRE(vec.size() == 4);

    vec.clear();
    REQUIRE(vec.size() == 0);

    const FixedVector<int, 5> constVec{10, 20};
    REQUIRE(constVec.size() == 2);

    constexpr FixedVector<int, 5> constexprEmptyVec{};
    static_assert(constexprEmptyVec.size() == 0);
  }

  SECTION("Max size method") {
    FixedVector<int, 5> vec{};
    REQUIRE(vec.max_size() == 5);

    FixedVector<int, 10> largeVec{};
    REQUIRE(largeVec.max_size() == 10);

    FixedVector<FixedString<8>, 3> stringVec{};
    REQUIRE(stringVec.max_size() == 3);

    const FixedVector<int, 5> constVec{};
    REQUIRE(constVec.max_size() == 5);

    // max_size equals capacity
    REQUIRE(vec.max_size() == vec.capacity());
    REQUIRE(largeVec.max_size() == largeVec.capacity());

    constexpr FixedVector<int, 5> constexprVec{};
    static_assert(constexprVec.max_size() == 5);
  }

  SECTION("Capacity method") {
    FixedVector<int, 5> vec{};
    REQUIRE(vec.capacity() == 5);

    FixedVector<int, 10> largeVec{};
    REQUIRE(largeVec.capacity() == 10);

    FixedVector<std::string, 3> stringVec{};
    REQUIRE(stringVec.capacity() == 3);

    const FixedVector<int, 5> constVec{};
    REQUIRE(constVec.capacity() == 5);

    // Capacity remains constant
    vec.push_back(1);
    REQUIRE(vec.capacity() == 5);

    vec.push_back(2);
    REQUIRE(vec.capacity() == 5);

    vec.clear();
    REQUIRE(vec.capacity() == 5);

    constexpr FixedVector<int, 5> constexprVec{};
    static_assert(constexprVec.capacity() == 5);
  }

  SECTION("Size and capacity relationship") {
    FixedVector<int, 5> vec{};

    // Initially empty
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec.size() <= vec.capacity());
    REQUIRE(vec.empty());

    // After adding elements
    vec.push_back(1);
    REQUIRE(vec.size() == 1);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec.size() <= vec.capacity());
    REQUIRE(vec.empty() == false);

    vec.push_back(2);
    vec.push_back(3);
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec.size() <= vec.capacity());

    // After clearing
    vec.clear();
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 5);
    REQUIRE(vec.empty());
  }

  SECTION("Max size equals capacity") {
    FixedVector<int, 5> vec{};
    REQUIRE(vec.max_size() == vec.capacity());

    FixedVector<int, 10> largeVec{};
    REQUIRE(largeVec.max_size() == largeVec.capacity());

    FixedVector<std::string, 20> stringVec{"a", "b"};
    REQUIRE(stringVec.max_size() == stringVec.capacity());
  }

  SECTION("Different template parameters") {
    // Different sizes
    static_assert(FixedVector<int, 1>{}.capacity() == 1);
    static_assert(FixedVector<int, 1>{}.max_size() == 1);
    static_assert(FixedVector<int, 1>{}.size() == 0);
    static_assert(FixedVector<int, 1>{}.empty());

    static_assert(FixedVector<int, 100>{}.capacity() == 100);
    static_assert(FixedVector<int, 100>{}.max_size() == 100);

    // Different types
    FixedVector<double, 5> doubleVec{};
    REQUIRE(doubleVec.capacity() == 5);
    REQUIRE(doubleVec.max_size() == 5);

    FixedVector<FixedString<10>, 3> stringVec{};
    REQUIRE(stringVec.capacity() == 3);
    REQUIRE(stringVec.max_size() == 3);
  }

  SECTION("Constexpr evaluation") {
    constexpr FixedVector<int, 5> emptyVec{};

    static_assert(emptyVec.size() == 0);
    static_assert(emptyVec.empty());
    static_assert(emptyVec.capacity() == 5);
    static_assert(emptyVec.max_size() == 5);
  }
}

TEST_CASE("FixedVector clear method", "[core][fixed_vector]") {
  SECTION("Clear empty vector") {
    FixedVector<int, 5> emptyVec{};

    REQUIRE(emptyVec.empty());
    REQUIRE(emptyVec.size() == 0);

    emptyVec.clear();

    REQUIRE(emptyVec.empty());
    REQUIRE(emptyVec.size() == 0);
    REQUIRE(emptyVec.capacity() == 5);
  }

  SECTION("Clear vector with elements") {
    FixedVector<int, 5> vec{1, 2, 3};

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.empty() == false);

    vec.clear();

    REQUIRE(vec.size() == 0);
    REQUIRE(vec.empty());
    REQUIRE(vec.capacity() == 5);
  }

  SECTION("Capacity remains unchanged after clear") {
    FixedVector<int, 5> vec{1, 2, 3, 4, 5};

    const auto capacityBefore = vec.capacity();
    REQUIRE(capacityBefore == 5);

    vec.clear();

    REQUIRE(vec.capacity() == capacityBefore);
    REQUIRE(vec.capacity() == 5);
  }

  SECTION("Can reuse vector after clear") {
    FixedVector<int, 5> vec{10, 20, 30};

    vec.clear();

    REQUIRE(vec.size() == 0);
    REQUIRE(vec.empty());

    vec.push_back(100);
    REQUIRE(vec.size() == 1);
    REQUIRE(vec[0] == 100);
    REQUIRE(vec.empty() == false);

    vec.push_back(200);
    REQUIRE(vec.size() == 2);
    REQUIRE(vec[1] == 200);
  }

  SECTION("Clear multiple times") {
    FixedVector<int, 5> vec{1, 2};

    vec.clear();
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.empty());

    vec.push_back(10);
    REQUIRE(vec.size() == 1);

    vec.clear();
    REQUIRE(vec.size() == 0);

    vec.clear(); // Clear again when already empty
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.empty());
  }

  SECTION("Clear with string elements") {
    FixedVector<FixedString<10>, 5> stringVec{FixedString<10>("a"), FixedString<10>("b"), FixedString<10>("c")};

    REQUIRE(stringVec.size() == 3);

    stringVec.clear();

    REQUIRE(stringVec.size() == 0);
    REQUIRE(stringVec.empty());
    REQUIRE(stringVec.capacity() == 5);

    // Verify destructors were called - add new elements
    stringVec.push_back(FixedString<10>("new"));
    REQUIRE(stringVec.size() == 1);
    REQUIRE(stringVec[0] == "new");
  }

  SECTION("Clear with complex types") {
    FixedVector<std::vector<int>, 3> complexVec;

    complexVec.push_back(std::vector<int>{1, 2, 3});
    complexVec.push_back(std::vector<int>{4, 5});

    REQUIRE(complexVec.size() == 2);

    complexVec.clear();

    REQUIRE(complexVec.size() == 0);
    REQUIRE(complexVec.empty());

    // Reuse after clear
    complexVec.push_back(std::vector<int>{10, 20});
    REQUIRE(complexVec.size() == 1);
    REQUIRE(complexVec[0].size() == 2);
  }

  SECTION("Clear preserves capacity for different sizes") {
    FixedVector<int, 10> largeVec{1, 2, 3, 4, 5};

    REQUIRE(largeVec.capacity() == 10);

    largeVec.clear();

    REQUIRE(largeVec.capacity() == 10);
    REQUIRE(largeVec.size() == 0);

    FixedVector<int, 1> smallVec{42};

    REQUIRE(smallVec.capacity() == 1);

    smallVec.clear();

    REQUIRE(smallVec.capacity() == 1);
    REQUIRE(smallVec.size() == 0);
  }
}

} // namespace toy
