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

#include <numbers>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include "core.hpp"

namespace toy {

TEST_CASE("OStringStream constructors", "[core][o_string_stream]") {
  SECTION("Default constructor") {
    constexpr OStringStream<FixedString<32>> emptyStream;

    REQUIRE(emptyStream.str() == "");

    // Compile-time checks
    static_assert(emptyStream.str() == "");
  }

  SECTION("Constructor from FixedString") {
    constexpr FixedString<16> source("Hello");
    constexpr OStringStream<FixedString<32>> stream(source);

    REQUIRE(stream.str() == "Hello");

    // Compile-time checks
    static_assert(stream.str() == "Hello");
  }

  SECTION("Constructor from std::string") {
    std::string source = "World";
    OStringStream<FixedString<64>> stream(source);

    REQUIRE(stream.str() == "World");
  }

  SECTION("Constructor from CStringView") {
    constexpr CStringView view("Test");
    constexpr OStringStream<FixedString<32>> stream(view);

    REQUIRE(stream.str() == "Test");

    // Compile-time checks
    static_assert(stream.str() == "Test");
  }

  SECTION("Constructor with empty string") {
    constexpr FixedString<16> empty;
    constexpr OStringStream<FixedString<32>> stream(empty);

    REQUIRE(stream.str() == "");

    // Compile-time checks
    static_assert(stream.str() == "");
  }
}

TEST_CASE("OStringStream assignment operators", "[core][o_string_stream]") {
  SECTION("Copy assignment") {
    constexpr OStringStream<FixedString<32>> source(FixedString<16>("Source"));
    OStringStream<FixedString<32>> target;

    target = source;

    REQUIRE(target.str() == "Source");
    REQUIRE(source.str() == "Source");
  }

  SECTION("Move assignment") {
    OStringStream<FixedString<32>> source(FixedString<16>("Move"));
    OStringStream<FixedString<32>> target;

    target = std::move(source);

    REQUIRE(target.str() == "Move");
  }
}

TEST_CASE("OStringStream swap", "[core][o_string_stream]") {
  SECTION("Swap two streams with content") {
    OStringStream<FixedString<32>> stream1(FixedString<16>("First"));
    OStringStream<FixedString<32>> stream2(FixedString<16>("Second"));

    stream1.precision(3);
    stream2.precision(9);

    stream1.swap(stream2);

    REQUIRE(stream1.str() == "Second");
    REQUIRE(stream1.precision() == 9);
    REQUIRE(stream2.str() == "First");
    REQUIRE(stream2.precision() == 3);
  }

  SECTION("Swap with empty stream") {
    OStringStream<FixedString<32>> stream1(FixedString<16>("Content"));
    OStringStream<FixedString<32>> stream2;

    stream1.precision(5);

    stream1.swap(stream2);

    REQUIRE(stream1.str() == "");
    REQUIRE(stream1.precision() == 6);
    REQUIRE(stream2.str() == "Content");
    REQUIRE(stream2.precision() == 5);
  }

  SECTION("Self-swap") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));

    stream.precision(8);
    stream.swap(stream);

    REQUIRE(stream.str() == "Test");
    REQUIRE(stream.precision() == 8);
  }

  SECTION("Swap empty streams") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;

    stream1.precision(2);
    stream2.precision(10);

    stream1.swap(stream2);

    REQUIRE(stream1.str() == "");
    REQUIRE(stream1.precision() == 10);
    REQUIRE(stream2.str() == "");
    REQUIRE(stream2.precision() == 2);
  }
}

TEST_CASE("OStringStream str setter", "[core][o_string_stream]") {
  SECTION("Set from FixedString") {
    OStringStream<FixedString<64>> stream(FixedString<16>("Old"));
    constexpr FixedString<16> source("NewContent");

    stream.str(source);

    REQUIRE(stream.str() == "NewContent");
  }

  SECTION("Set from CStringView") {
    OStringStream<FixedString<64>> stream(FixedString<16>("Old"));
    constexpr CStringView view("NewContent");

    stream.str(view);

    REQUIRE(stream.str() == "NewContent");
  }

  SECTION("Set from std::string") {
    OStringStream<FixedString<64>> stream(FixedString<16>("Old"));
    std::string source = "NewContent";

    stream.str(source);

    REQUIRE(stream.str() == "NewContent");
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
    REQUIRE(stream.str() == "First");

    stream.str(second);
    REQUIRE(stream.str() == "Second");

    stream.str(third);
    REQUIRE(stream.str() == "Third");
  }
}

TEST_CASE("OStringStream view", "[core][o_string_stream]") {
  SECTION("View with content") {
    constexpr OStringStream<FixedString<32>> stream(FixedString<16>("Hello"));
    auto view = stream.view();

    REQUIRE(view == "Hello");
  }

  SECTION("View with empty stream") {
    constexpr OStringStream<FixedString<32>> stream;
    auto view = stream.view();

    REQUIRE(view.empty());
  }

  SECTION("View reflects current content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Initial"));
    auto view1 = stream.view();

    REQUIRE(view1 == "Initial");

    stream.str(FixedString<16>("Updated"));
    auto view2 = stream.view();

    REQUIRE(view2 == "Updated");
  }

  SECTION("Multiple views of same stream") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Content"));
    auto view1 = stream.view();
    auto view2 = stream.view();

    REQUIRE(view1 == view2);
  }
}

TEST_CASE("OStringStream put", "[core][o_string_stream]") {
  SECTION("Put single character to empty stream") {
    OStringStream<FixedString<32>> stream;

    stream.put('A');

    REQUIRE(stream.str() == "A");
  }

  SECTION("Put single character to stream with content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Hello"));

    stream.put('!');

    REQUIRE(stream.str() == "Hello!");
  }

  SECTION("Put multiple characters with chaining") {
    OStringStream<FixedString<32>> stream;

    stream.put('H').put('e').put('l').put('l').put('o');

    REQUIRE(stream.str() == "Hello");
  }

  SECTION("Put to stream and verify content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));

    stream.put('1').put('2').put('3');

    REQUIRE(stream.str() == "Test123");
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
    REQUIRE(stream1.str() == "A");
    REQUIRE(stream2.str() == "B");
  }
}

TEST_CASE("OStringStream write", "[core][o_string_stream]") {
  SECTION("Write to empty stream") {
    OStringStream<FixedString<32>> stream;
    const char * buffer = "Hello";

    stream.write(buffer, 5);

    REQUIRE(stream.str() == "Hello");
  }

  SECTION("Write to stream with content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));
    const char * buffer = "123";

    stream.write(buffer, 3);

    REQUIRE(stream.str() == "Test123");
  }

  SECTION("Write zero count") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Initial"));
    const char * buffer = "Data";

    stream.write(buffer, 0);

    REQUIRE(stream.str() == "Initial");
  }

  SECTION("Write partial string") {
    OStringStream<FixedString<32>> stream;
    const char * buffer = "Hello World";

    stream.write(buffer, 5);

    REQUIRE(stream.str() == "Hello");
  }

  SECTION("Write multiple times with chaining") {
    OStringStream<FixedString<32>> stream;
    const char * buffer1 = "Hello";
    const char * buffer2 = " World";

    stream.write(buffer1, 5).write(buffer2, 6);

    REQUIRE(stream.str() == "Hello World");
  }

  SECTION("Write binary data") {
    OStringStream<FixedString<32>> stream;
    constexpr const char buffer[] = {'H', 'e', 'l', 'l', 'o'};

    stream.write(buffer, 5);

    REQUIRE(stream.str() == "Hello");
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
    REQUIRE(stream1.str() == "First");
    REQUIRE(stream2.str() == "Second");
  }
}

TEST_CASE("OStringStream tellp", "[core][o_string_stream]") {
  SECTION("Tellp on empty stream") {
    constexpr OStringStream<FixedString<32>> stream;

    REQUIRE(stream.tellp() == 0);

    // Compile-time check
    static_assert(stream.tellp() == 0);
  }

  SECTION("Tellp after construction") {
    constexpr OStringStream<FixedString<32>> stream(FixedString<16>("Hello"));

    REQUIRE(stream.tellp() == 5);

    // Compile-time check
    static_assert(stream.tellp() == 5);
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

TEST_CASE("OStringStream precision", "[core][o_string_stream]") {
  SECTION("Default precision") {
    constexpr OStringStream<FixedString<32>> stream;

    REQUIRE(stream.precision() == 6);

    // Compile-time check
    static_assert(stream.precision() == 6);
  }

  SECTION("Set precision") {
    OStringStream<FixedString<32>> stream;

    REQUIRE(stream.precision() == 6);

    const auto oldPrecision = stream.precision(10);

    REQUIRE(oldPrecision == 6);
    REQUIRE(stream.precision() == 10);
  }

  SECTION("Set precision multiple times") {
    OStringStream<FixedString<32>> stream;

    REQUIRE(stream.precision() == 6);

    auto prev = stream.precision(2);
    REQUIRE(prev == 6);
    REQUIRE(stream.precision() == 2);

    prev = stream.precision(15);
    REQUIRE(prev == 2);
    REQUIRE(stream.precision() == 15);

    prev = stream.precision(0);
    REQUIRE(prev == 15);
    REQUIRE(stream.precision() == 0);
  }

  SECTION("Precision is independent of stream content") {
    OStringStream<FixedString<32>> stream;

    REQUIRE(stream.precision() == 6);

    stream.put('A').put('B');
    REQUIRE(stream.precision() == 6);

    stream.precision(3);
    REQUIRE(stream.precision() == 3);
    REQUIRE(stream.str() == "AB");
  }
}

TEST_CASE("OStringStream operator<<", "[core][o_string_stream]") {
  SECTION("Insert true boolean value") {
    OStringStream<FixedString<32>> stream;

    stream << true;

    REQUIRE(stream.str() == "true");
  }

  SECTION("Insert false boolean value") {
    OStringStream<FixedString<32>> stream;

    stream << false;

    REQUIRE(stream.str() == "false");
  }

  SECTION("Insert boolean to stream with content") {
    OStringStream<FixedString<32>> stream(CStringView("Value: "));

    stream << true;

    REQUIRE(stream.str() == "Value: true");
  }

  SECTION("Operator chaining with boolean") {
    OStringStream<FixedString<32>> stream;

    stream << true << false << true;

    REQUIRE(stream.str() == "truefalsetrue");
  }

  SECTION("Insert boolean with separator") {
    OStringStream<FixedString<32>> stream;

    stream << true;
    stream.put(' ');
    stream << false;

    REQUIRE(stream.str() == "true false");
  }

  SECTION("Insert boolean returns reference for chaining") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;

    auto & ref1 = stream1 << true;
    auto & ref2 = stream2 << false;

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "true");
    REQUIRE(stream2.str() == "false");
  }

  SECTION("Insert int8 integer") {
    OStringStream<FixedString<8>> stream1;
    OStringStream<FixedString<8>> stream2;
    OStringStream<FixedString<8>> stream3;

    stream1 << static_cast<int8_t>(123);
    stream2 << static_cast<int8_t>(-123);
    stream3 << static_cast<uint8_t>(123);

    REQUIRE(stream1.str() == "123");
    REQUIRE(stream2.str() == "-123");
    REQUIRE(stream3.str() == "123");
  }

  SECTION("Insert int16 integer") {
    OStringStream<FixedString<8>> stream1;
    OStringStream<FixedString<8>> stream2;
    OStringStream<FixedString<8>> stream3;

    stream1 << static_cast<int16_t>(12345);
    stream2 << static_cast<int16_t>(-12345);
    stream3 << static_cast<uint16_t>(12345);

    REQUIRE(stream1.str() == "12345");
    REQUIRE(stream2.str() == "-12345");
    REQUIRE(stream3.str() == "12345");
  }

  SECTION("Insert int32 integer") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;
    OStringStream<FixedString<16>> stream3;

    stream1 << static_cast<int32_t>(123456789);
    stream2 << static_cast<int32_t>(-123456789);
    stream3 << static_cast<uint32_t>(123456789);

    REQUIRE(stream1.str() == "123456789");
    REQUIRE(stream2.str() == "-123456789");
    REQUIRE(stream3.str() == "123456789");
  }

  SECTION("Insert int64 integer") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;
    OStringStream<FixedString<32>> stream3;

    stream1 << static_cast<int64_t>(123456789);
    stream2 << static_cast<int64_t>(-123456789);
    stream3 << static_cast<uint64_t>(123456789);

    REQUIRE(stream1.str() == "123456789");
    REQUIRE(stream2.str() == "-123456789");
    REQUIRE(stream3.str() == "123456789");
  }

  SECTION("Insert float") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    stream1 << 123.456f;
    stream2 << -123.456f;

    REQUIRE(stream1.str() == "123.456");
    REQUIRE(stream2.str() == "-123.456");
  }

  SECTION("Insert double") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    stream1 << 123.456;
    stream2 << -123.456;

    REQUIRE(stream1.str() == "123.456");
    REQUIRE(stream2.str() == "-123.456");
  }

  SECTION("Insert int integer") {
    OStringStream<FixedString<8>> stream1;
    OStringStream<FixedString<8>> stream2;
    OStringStream<FixedString<8>> stream3;

    stream1 << 12345;
    stream2 << -12345;
    stream3 << 12345U;

    REQUIRE(stream1.str() == "12345");
    REQUIRE(stream2.str() == "-12345");
    REQUIRE(stream3.str() == "12345");
  }

  SECTION("Insert zero values") {
    OStringStream<FixedString<32>> stream;

    stream << static_cast<int8_t>(0) << static_cast<uint8_t>(0) << static_cast<int16_t>(0) << static_cast<uint16_t>(0)
           << static_cast<int32_t>(0) << static_cast<uint32_t>(0) << static_cast<int64_t>(0) << static_cast<uint64_t>(0)
           << 0.0f << 0.0 << 0 << 0U;

    REQUIRE(stream.str() == "000000000000");
  }

  SECTION("Insert numbers to stream with content") {
    OStringStream<FixedString<64>> stream1(CStringView("Value: "));
    OStringStream<FixedString<64>> stream2(CStringView("Value: "));
    OStringStream<FixedString<64>> stream3(CStringView("Value: "));
    OStringStream<FixedString<64>> stream4(CStringView("Value: "));
    OStringStream<FixedString<64>> stream5(CStringView("Value: "));
    OStringStream<FixedString<64>> stream6(CStringView("Value: "));
    OStringStream<FixedString<64>> stream7(CStringView("Value: "));
    OStringStream<FixedString<64>> stream8(CStringView("Value: "));
    OStringStream<FixedString<64>> stream9(CStringView("Value: "));
    OStringStream<FixedString<64>> stream10(CStringView("Value: "));
    OStringStream<FixedString<64>> stream11(CStringView("Value: "));
    OStringStream<FixedString<64>> stream12(CStringView("Value: "));

    stream1 << static_cast<int8_t>(-12);
    stream2 << static_cast<uint8_t>(23);
    stream3 << static_cast<int16_t>(-34);
    stream4 << static_cast<uint16_t>(45);
    stream5 << static_cast<int32_t>(-56);
    stream6 << static_cast<uint32_t>(67);
    stream7 << static_cast<int64_t>(-78);
    stream8 << static_cast<uint64_t>(89);
    stream9 << -9.0f;
    stream10 << 0.1;
    stream11 << -12;
    stream12 << 23;

    REQUIRE(stream1.str() == "Value: -12");
    REQUIRE(stream2.str() == "Value: 23");
    REQUIRE(stream3.str() == "Value: -34");
    REQUIRE(stream4.str() == "Value: 45");
    REQUIRE(stream5.str() == "Value: -56");
    REQUIRE(stream6.str() == "Value: 67");
    REQUIRE(stream7.str() == "Value: -78");
    REQUIRE(stream8.str() == "Value: 89");
    REQUIRE(stream9.str() == "Value: -9");
    REQUIRE(stream10.str() == "Value: 0.1");
    REQUIRE(stream11.str() == "Value: -12");
    REQUIRE(stream12.str() == "Value: 23");
  }

  SECTION("Insert int8 returns reference for chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << static_cast<int8_t>(-100);
    auto & ref2 = stream2 << static_cast<uint8_t>(200);

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "200");
  }

  SECTION("Insert int16 returns reference for chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << static_cast<int16_t>(-100);
    auto & ref2 = stream2 << static_cast<uint16_t>(200);

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "200");
  }

  SECTION("Insert int32 returns reference for chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << static_cast<int32_t>(-100);
    auto & ref2 = stream2 << static_cast<uint32_t>(200);

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "200");
  }

  SECTION("Insert int64 returns reference for chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << static_cast<int64_t>(-100);
    auto & ref2 = stream2 << static_cast<uint64_t>(200);

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "200");
  }

  SECTION("Insert float returns reference for chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << 100.5f;
    auto & ref2 = stream2 << 200.75f;

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "100.5");
    REQUIRE(stream2.str() == "200.75");
  }

  SECTION("Insert double returns reference for chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << 100.5;
    auto & ref2 = stream2 << 200.75;

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "100.5");
    REQUIRE(stream2.str() == "200.75");
  }

  SECTION("Insert int returns reference for chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << -100;
    auto & ref2 = stream2 << 100U;

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "100");
  }

  SECTION("Insert int8 min/max with separator") {
    OStringStream<FixedString<64>> stream;

    stream << std::numeric_limits<int8_t>::min();
    stream.put(' ');
    stream << std::numeric_limits<int8_t>::max();
    stream.put(' ');
    stream << std::numeric_limits<uint8_t>::min();
    stream.put(' ');
    stream << std::numeric_limits<uint8_t>::max();

    REQUIRE(stream.str() == "-128 127 0 255");
  }

  SECTION("Insert int16 min/max with separator") {
    OStringStream<FixedString<64>> stream;

    stream << std::numeric_limits<int16_t>::min();
    stream.put(' ');
    stream << std::numeric_limits<int16_t>::max();
    stream.put(' ');
    stream << std::numeric_limits<uint16_t>::min();
    stream.put(' ');
    stream << std::numeric_limits<uint16_t>::max();

    REQUIRE(stream.str() == "-32768 32767 0 65535");
  }

  SECTION("Insert int32 min/max with separator") {
    OStringStream<FixedString<64>> stream;

    stream << std::numeric_limits<int32_t>::min();
    stream.put(' ');
    stream << std::numeric_limits<int32_t>::max();
    stream.put(' ');
    stream << std::numeric_limits<uint32_t>::min();
    stream.put(' ');
    stream << std::numeric_limits<uint32_t>::max();

    REQUIRE(stream.str() == "-2147483648 2147483647 0 4294967295");
  }

  SECTION("Insert int64 min/max with separator") {
    OStringStream<FixedString<64>> stream;

    stream << std::numeric_limits<int64_t>::min();
    stream.put(' ');
    stream << std::numeric_limits<int64_t>::max();
    stream.put(' ');
    stream << std::numeric_limits<uint64_t>::min();
    stream.put(' ');
    stream << std::numeric_limits<uint64_t>::max();

    REQUIRE(stream.str() == "-9223372036854775808 9223372036854775807 0 18446744073709551615");
  }

  SECTION("Insert int min/max with separator") {
    OStringStream<FixedString<64>> stream;

    stream << std::numeric_limits<int>::min();
    stream.put(' ');
    stream << std::numeric_limits<int>::max();
    stream.put(' ');
    stream << std::numeric_limits<unsigned int>::min();
    stream.put(' ');
    stream << std::numeric_limits<unsigned int>::max();

    if constexpr (sizeof(int) == 4) {
      REQUIRE(stream.str() == "-2147483648 2147483647 0 4294967295");
    } else if constexpr (sizeof(int) == 8) {
      REQUIRE(stream.str() == "-9223372036854775808 9223372036854775807 0 18446744073709551615");
    } else {
      static_assert(sizeof(int) == 4 || sizeof(int) == 8, "Unsupported value size");
    }
  }

  SECTION("Insert float with custom precision") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;
    OStringStream<FixedString<64>> stream3;
    OStringStream<FixedString<64>> stream4;

    stream2.precision(3);
    stream3.precision(1);
    stream4.precision(15);

    stream1 << std::numbers::pi_v<float>;
    stream2 << std::numbers::pi_v<float>;
    stream3 << std::numbers::pi_v<float>;
    stream4 << std::numbers::pi_v<float>;

    REQUIRE(stream1.str() == "3.14159");
    REQUIRE(stream2.str() == "3.14");
    REQUIRE(stream3.str() == "3");
    REQUIRE(stream4.str() == "3.14159244298935");
  }

  SECTION("Insert double with custom precision") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;
    OStringStream<FixedString<64>> stream3;
    OStringStream<FixedString<64>> stream4;

    stream2.precision(3);
    stream3.precision(1);
    stream4.precision(15);

    stream1 << std::numbers::pi;
    stream2 << std::numbers::pi;
    stream3 << std::numbers::pi;
    stream4 << std::numbers::pi;

    REQUIRE(stream1.str() == "3.14159");
    REQUIRE(stream2.str() == "3.14");
    REQUIRE(stream3.str() == "3");
    REQUIRE(stream4.str() == "3.14159244298935");
  }

  SECTION("Insert void pointer") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;
    OStringStream<FixedString<32>> stream3;

    constexpr int value = 42;
    constexpr const int * ptr = nullptr;

    stream1 << &value;
    stream2 << ptr;
    stream3 << nullptr;

    REQUIRE(stream1.str().starts_with("0x"));

    if constexpr (sizeof(void *) == 4) {
      REQUIRE(stream1.str().length() == 10);
    } else if constexpr (sizeof(void *) == 8) {
      REQUIRE(stream1.str().length() == 18);
    } else {
      static_assert(sizeof(void *) == 4 || sizeof(void *) == 8, "Unsupported value size");
    }

    REQUIRE(stream2.str() == "nullptr");
    REQUIRE(stream3.str() == "nullptr");
  }

  SECTION("Insert pointer to stream with content") {
    OStringStream<FixedString<32>> stream1(CStringView("Ptr: "));
    OStringStream<FixedString<32>> stream2(CStringView("Ptr: "));

    constexpr int value = 100;

    stream1 << &value;
    stream2 << nullptr;

    REQUIRE(stream1.str().starts_with("Ptr: 0x"));
    REQUIRE(stream1.str().size() > 6);
    REQUIRE(stream2.str() == "Ptr: nullptr");
  }

  SECTION("Insert char_type character") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;

    stream1 << 'A';
    stream2 << 'Z';

    REQUIRE(stream1.str() == "A");
    REQUIRE(stream2.str() == "Z");
  }

  SECTION("Insert char_type to stream with content") {
    OStringStream<FixedString<32>> stream(CStringView("Hello"));

    stream << '!';

    REQUIRE(stream.str() == "Hello!");
  }

  SECTION("Insert char_type with chaining") {
    OStringStream<FixedString<32>> stream;

    stream << 'H' << 'e' << 'l' << 'l' << 'o';

    REQUIRE(stream.str() == "Hello");
  }

  SECTION("Insert char_type special characters") {
    OStringStream<FixedString<32>> stream;

    stream << ' ' << '\n' << '\t' << '!';

    REQUIRE(stream.str().size() == 4);
    REQUIRE(stream.str()[0] == ' ');
    REQUIRE(stream.str()[1] == '\n');
    REQUIRE(stream.str()[2] == '\t');
    REQUIRE(stream.str()[3] == '!');
  }

  SECTION("Insert char_type returns reference for chaining") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;

    auto & ref1 = stream1 << 'A';
    auto & ref2 = stream2 << 'B';

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "A");
    REQUIRE(stream2.str() == "B");
  }

  SECTION("Insert StringLike FixedString") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;

    const FixedString<16> str1("Hello");
    const FixedString<16> str2("World");

    stream1 << str1;
    stream2 << str2;

    REQUIRE(stream1.str() == "Hello");
    REQUIRE(stream2.str() == "World");
  }

  SECTION("Insert StringLike CStringView") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;

    const CStringView view1("Test");
    const CStringView view2("String");

    stream1 << view1;
    stream2 << view2;

    REQUIRE(stream1.str() == "Test");
    REQUIRE(stream2.str() == "String");
  }

  SECTION("Insert StringLike to stream with content") {
    OStringStream<FixedString<64>> stream(CStringView("Prefix: "));

    const FixedString<16> suffix("Suffix");

    stream << suffix;

    REQUIRE(stream.str() == "Prefix: Suffix");
  }

  SECTION("Insert StringLike with chaining") {
    OStringStream<FixedString<64>> stream;

    const FixedString<16> hello("Hello");
    const CStringView space(" ");
    const FixedString<16> world("World");

    stream << hello << space << world;

    REQUIRE(stream.str() == "Hello World");
  }

  SECTION("Insert StringLike returns reference for chaining") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;

    const FixedString<16> str1("First");
    const FixedString<16> str2("Second");

    auto & ref1 = stream1 << str1;
    auto & ref2 = stream2 << str2;

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "First");
    REQUIRE(stream2.str() == "Second");
  }

  SECTION("Insert C string literal") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;

    stream1 << "Hello";
    stream2 << "World";

    REQUIRE(stream1.str() == "Hello");
    REQUIRE(stream2.str() == "World");
  }

  SECTION("Insert C string to stream with content") {
    OStringStream<FixedString<64>> stream(CStringView("Prefix: "));

    stream << "Suffix";

    REQUIRE(stream.str() == "Prefix: Suffix");
  }

  SECTION("Insert C string with chaining") {
    OStringStream<FixedString<64>> stream;

    stream << "Hello" << " " << "World";

    REQUIRE(stream.str() == "Hello World");
  }

  SECTION("Insert C string returns reference for chaining") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;

    auto & ref1 = stream1 << "First";
    auto & ref2 = stream2 << "Second";

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "First");
    REQUIRE(stream2.str() == "Second");
  }

  SECTION("Insert C string empty string") {
    OStringStream<FixedString<64>> stream;

    stream << "";

    REQUIRE(stream.str() == "");
  }

  SECTION("Insert C string with special characters") {
    OStringStream<FixedString<64>> stream;

    stream << "A\nB\tC";

    REQUIRE(stream.str() == "A\nB\tC");
  }
}

} // namespace toy
