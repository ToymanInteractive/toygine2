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

TEST_CASE("OStringStream str setter", "[core][o_string_stream]") {
  SECTION("Set from FixedString") {
    OStringStream<FixedString<64>> stream(FixedString<16>("Old"));
    constexpr FixedString<16> source("NewContent");

    stream.str(source);

    REQUIRE(stream.str().size() == 10);
    REQUIRE(std::strcmp(stream.str().c_str(), "NewContent") == 0);
  }

  SECTION("Set from CStringView") {
    OStringStream<FixedString<64>> stream(FixedString<16>("Old"));
    constexpr CStringView view("NewContent");

    stream.str(view);

    REQUIRE(stream.str().size() == 10);
    REQUIRE(std::strcmp(stream.str().c_str(), "NewContent") == 0);
  }

  SECTION("Set from std::string") {
    OStringStream<FixedString<64>> stream(FixedString<16>("Old"));
    std::string source = "NewContent";

    stream.str(source);

    REQUIRE(stream.str().size() == 10);
    REQUIRE(std::strcmp(stream.str().c_str(), "NewContent") == 0);
  }

  SECTION("Set empty string") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Content"));
    constexpr FixedString<16> empty;

    stream.str(empty);

    REQUIRE(stream.str().empty());
  }

  SECTION("Set multiple times") {
    OStringStream<FixedString<32>> stream;
    constexpr FixedString<16> first("First");
    constexpr FixedString<16> second("Second");
    constexpr FixedString<16> third("Third");

    stream.str(first);
    REQUIRE(std::strcmp(stream.str().c_str(), "First") == 0);

    stream.str(second);
    REQUIRE(std::strcmp(stream.str().c_str(), "Second") == 0);

    stream.str(third);
    REQUIRE(std::strcmp(stream.str().c_str(), "Third") == 0);
  }
}

TEST_CASE("OStringStream view", "[core][o_string_stream]") {
  SECTION("View with content") {
    constexpr OStringStream<FixedString<32>> stream(FixedString<16>("Hello"));
    auto view = stream.view();

    REQUIRE(view.size() == 5);
    REQUIRE(std::strcmp(view.c_str(), "Hello") == 0);
  }

  SECTION("View with empty stream") {
    constexpr OStringStream<FixedString<32>> stream;
    auto view = stream.view();

    REQUIRE(view.empty());
  }

  SECTION("View reflects current content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Initial"));
    auto view1 = stream.view();

    REQUIRE(view1.size() == 7);
    REQUIRE(std::strcmp(view1.c_str(), "Initial") == 0);

    stream.str(FixedString<16>("Updated"));
    auto view2 = stream.view();

    REQUIRE(view2.size() == 7);
    REQUIRE(std::strcmp(view2.c_str(), "Updated") == 0);
  }

  SECTION("Multiple views of same stream") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Content"));
    auto view1 = stream.view();
    auto view2 = stream.view();

    REQUIRE(view1.size() == view2.size());
    REQUIRE(std::strcmp(view1.c_str(), view2.c_str()) == 0);
    REQUIRE(view1 == view2);
  }
}

TEST_CASE("OStringStream put", "[core][o_string_stream]") {
  SECTION("Put single character to empty stream") {
    OStringStream<FixedString<32>> stream;

    stream.put('A');

    REQUIRE(stream.str().size() == 1);
    REQUIRE(std::strcmp(stream.str().c_str(), "A") == 0);
  }

  SECTION("Put single character to stream with content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Hello"));

    stream.put('!');

    REQUIRE(stream.str().size() == 6);
    REQUIRE(std::strcmp(stream.str().c_str(), "Hello!") == 0);
  }

  SECTION("Put multiple characters with chaining") {
    OStringStream<FixedString<32>> stream;

    stream.put('H').put('e').put('l').put('l').put('o');

    REQUIRE(stream.str().size() == 5);
    REQUIRE(std::strcmp(stream.str().c_str(), "Hello") == 0);
  }

  SECTION("Put to stream and verify content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));

    stream.put('1').put('2').put('3');

    REQUIRE(stream.str().size() == 7);
    REQUIRE(std::strcmp(stream.str().c_str(), "Test123") == 0);
  }

  SECTION("Put special characters") {
    OStringStream<FixedString<32>> stream;

    stream.put(' ').put('\n').put('\t').put('!');

    REQUIRE(stream.str().size() == 4);
    REQUIRE(stream.str()[0] == ' ');
    REQUIRE(stream.str()[1] == '\n');
    REQUIRE(stream.str()[2] == '\t');
    REQUIRE(stream.str()[3] == '!');
  }

  SECTION("Put returns reference for chaining") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;

    auto & ref1 = stream1.put('A');
    auto & ref2 = stream2.put('B');

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(std::strcmp(stream1.str().c_str(), "A") == 0);
    REQUIRE(std::strcmp(stream2.str().c_str(), "B") == 0);
  }
}

TEST_CASE("OStringStream write", "[core][o_string_stream]") {
  SECTION("Write to empty stream") {
    OStringStream<FixedString<32>> stream;
    const char * buffer = "Hello";

    stream.write(buffer, 5);

    REQUIRE(stream.str().size() == 5);
    REQUIRE(std::strcmp(stream.str().c_str(), "Hello") == 0);
  }

  SECTION("Write to stream with content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));
    const char * buffer = "123";

    stream.write(buffer, 3);

    REQUIRE(stream.str().size() == 7);
    REQUIRE(std::strcmp(stream.str().c_str(), "Test123") == 0);
  }

  SECTION("Write zero count") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Initial"));
    const char * buffer = "Data";

    stream.write(buffer, 0);

    REQUIRE(stream.str().size() == 7);
    REQUIRE(std::strcmp(stream.str().c_str(), "Initial") == 0);
  }

  SECTION("Write partial string") {
    OStringStream<FixedString<32>> stream;
    const char * buffer = "Hello World";

    stream.write(buffer, 5);

    REQUIRE(stream.str().size() == 5);
    REQUIRE(std::strcmp(stream.str().c_str(), "Hello") == 0);
  }

  SECTION("Write multiple times with chaining") {
    OStringStream<FixedString<32>> stream;
    const char * buffer1 = "Hello";
    const char * buffer2 = " World";

    stream.write(buffer1, 5).write(buffer2, 6);

    REQUIRE(stream.str().size() == 11);
    REQUIRE(std::strcmp(stream.str().c_str(), "Hello World") == 0);
  }

  SECTION("Write binary data") {
    OStringStream<FixedString<32>> stream;
    constexpr const char buffer[] = {'H', 'e', 'l', 'l', 'o'};

    stream.write(buffer, 5);

    REQUIRE(stream.str().size() == 5);
    REQUIRE(std::strcmp(stream.str().c_str(), "Hello") == 0);
  }

  SECTION("Write returns reference for chaining") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;
    const char * buffer1 = "First";
    const char * buffer2 = "Second";

    auto & ref1 = stream1.write(buffer1, 5);
    auto & ref2 = stream2.write(buffer2, 6);

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(std::strcmp(stream1.str().c_str(), "First") == 0);
    REQUIRE(std::strcmp(stream2.str().c_str(), "Second") == 0);
  }
}

TEST_CASE("OStringStream tellp", "[core][o_string_stream]") {
  SECTION("Tellp on empty stream") {
    constexpr OStringStream<FixedString<32>> stream;

    REQUIRE(stream.tellp() == 0);

    // Compile-time check
    STATIC_REQUIRE(stream.tellp() == 0);
  }

  SECTION("Tellp after construction") {
    constexpr OStringStream<FixedString<32>> stream(FixedString<16>("Hello"));

    REQUIRE(stream.tellp() == 5);

    // Compile-time check
    STATIC_REQUIRE(stream.tellp() == 5);
  }

  SECTION("Tellp after put") {
    OStringStream<FixedString<32>> stream;

    REQUIRE(stream.tellp() == 0);

    stream.put('A');
    REQUIRE(stream.tellp() == 1);

    stream.put('B');
    REQUIRE(stream.tellp() == 2);
  }

  SECTION("Tellp after write") {
    OStringStream<FixedString<32>> stream;
    const char * buffer = "Hello";

    REQUIRE(stream.tellp() == 0);

    stream.write(buffer, 5);
    REQUIRE(stream.tellp() == 5);

    stream.write(buffer, 5);
    REQUIRE(stream.tellp() == 10);
  }

  SECTION("Tellp after multiple operations") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));

    REQUIRE(stream.tellp() == 4);

    stream.put('1');
    REQUIRE(stream.tellp() == 5);

    stream.write("23", 2);
    REQUIRE(stream.tellp() == 7);
  }

  SECTION("Tellp equals string size") {
    OStringStream<FixedString<32>> stream;

    stream.put('A').put('B').put('C');

    REQUIRE(stream.tellp() == stream.str().size());
    REQUIRE(stream.tellp() == 3);
  }
}

} // namespace toy
