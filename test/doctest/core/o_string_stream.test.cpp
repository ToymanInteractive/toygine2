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

#include <limits>
#include <numbers>

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

// Constructors from default, FixedString, std::string, CStringView.
TEST_CASE("core/o_string_stream/constructors") {
  // Default constructor yields empty string.
  SUBCASE("default_constructor") {
    constexpr OStringStream<FixedString<32>> emptyStream;

    REQUIRE(emptyStream.str() == "");

    static_assert(emptyStream.str() == "", "default-constructed stream must have empty str");
  }

  // Constructor from FixedString.
  SUBCASE("constructor_from_fixed_string") {
    constexpr FixedString<16> source("Hello");
    constexpr OStringStream<FixedString<32>> stream(source);

    REQUIRE(stream.str() == "Hello");

    static_assert(stream.str() == "Hello", "str must match source FixedString");
  }

  // Constructor from std::string.
  SUBCASE("constructor_from_std_string") {
    std::string source = "World";
    OStringStream<FixedString<64>> stream(source);

    REQUIRE(stream.str() == "World");
  }

  // Constructor from CStringView.
  SUBCASE("constructor_from_c_string_view") {
    constexpr CStringView view("Test");
    constexpr OStringStream<FixedString<32>> stream(view);

    REQUIRE(stream.str() == "Test");

    static_assert(stream.str() == "Test", "str must match source CStringView");
  }

  // Constructor with empty string.
  SUBCASE("constructor_with_empty_string") {
    constexpr FixedString<16> empty;
    constexpr OStringStream<FixedString<32>> stream(empty);

    REQUIRE(stream.str() == "");

    static_assert(stream.str() == "", "str must be empty for empty source");
  }
}

// Copy and move assignment.
TEST_CASE("core/o_string_stream/assignment_operators") {
  // Copy assignment copies content.
  SUBCASE("copy_assignment") {
    constexpr OStringStream<FixedString<32>> source(FixedString<16>("Source"));
    OStringStream<FixedString<32>> target;

    target = source;

    REQUIRE(target.str() == "Source");
    REQUIRE(source.str() == "Source");
  }

  // Move assignment moves content.
  SUBCASE("move_assignment") {
    OStringStream<FixedString<32>> source(FixedString<16>("Move"));
    OStringStream<FixedString<32>> target;

    target = std::move(source);

    REQUIRE(target.str() == "Move");
  }
}

// swap() exchanges content and precision.
TEST_CASE("core/o_string_stream/swap") {
  // Swap two streams with content and precision.
  SUBCASE("swap_two_streams_with_content") {
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

  // Swap with empty stream.
  SUBCASE("swap_with_empty_stream") {
    OStringStream<FixedString<32>> stream1(FixedString<16>("Content"));
    OStringStream<FixedString<32>> stream2;

    stream1.precision(5);

    stream1.swap(stream2);

    REQUIRE(stream1.str() == "");
    REQUIRE(stream1.precision() == 6);
    REQUIRE(stream2.str() == "Content");
    REQUIRE(stream2.precision() == 5);
  }

  // Self-swap is no-op.
  SUBCASE("self_swap") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));

    stream.precision(8);
    stream.swap(stream);

    REQUIRE(stream.str() == "Test");
    REQUIRE(stream.precision() == 8);
  }

  // Swap empty streams; precision exchanged.
  SUBCASE("swap_empty_streams") {
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

// str() setter from FixedString, CStringView, std::string.
TEST_CASE("core/o_string_stream/str_setter") {
  // str() setter from FixedString.
  SUBCASE("set_from_fixed_string") {
    OStringStream<FixedString<64>> stream(FixedString<16>("Old"));
    constexpr FixedString<16> source("NewContent");

    stream.str(source);

    REQUIRE(stream.str() == "NewContent");
  }

  // str() setter from CStringView.
  SUBCASE("set_from_c_string_view") {
    OStringStream<FixedString<64>> stream(FixedString<16>("Old"));
    constexpr CStringView view("NewContent");

    stream.str(view);

    REQUIRE(stream.str() == "NewContent");
  }

  // str() setter from std::string.
  SUBCASE("set_from_std_string") {
    OStringStream<FixedString<64>> stream(FixedString<16>("Old"));
    std::string source = "NewContent";

    stream.str(source);

    REQUIRE(stream.str() == "NewContent");
  }

  // str() setter to empty string.
  SUBCASE("set_empty_string") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Content"));
    constexpr FixedString<16> empty;

    stream.str(empty);

    REQUIRE(stream.str().empty());
  }

  // str() setter multiple times.
  SUBCASE("set_multiple_times") {
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

// view() returns CStringView of current content.
TEST_CASE("core/o_string_stream/view") {
  // view() with content.
  SUBCASE("view_with_content") {
    constexpr OStringStream<FixedString<32>> stream(FixedString<16>("Hello"));
    auto view = stream.view();

    REQUIRE(view == "Hello");
  }

  // view() with empty stream.
  SUBCASE("view_with_empty_stream") {
    constexpr OStringStream<FixedString<32>> stream;
    auto view = stream.view();

    REQUIRE(view.empty());
  }

  // view() reflects current content after str().
  SUBCASE("view_reflects_current_content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Initial"));
    auto view1 = stream.view();

    REQUIRE(view1 == "Initial");

    stream.str(FixedString<16>("Updated"));
    auto view2 = stream.view();

    REQUIRE(view2 == "Updated");
  }

  // Multiple views of same stream are equal.
  SUBCASE("multiple_views_of_same_stream") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Content"));
    auto view1 = stream.view();
    auto view2 = stream.view();

    REQUIRE(view1 == view2);
  }
}

// put() appends a single character; returns reference for chaining.
TEST_CASE("core/o_string_stream/put") {
  // put() single character to empty stream.
  SUBCASE("put_single_character_to_empty_stream") {
    OStringStream<FixedString<32>> stream;

    stream.put('A');

    REQUIRE(stream.str() == "A");
  }

  // put() single character to stream with content.
  SUBCASE("put_single_character_to_stream_with_content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Hello"));

    stream.put('!');

    REQUIRE(stream.str() == "Hello!");
  }

  // put() multiple characters with chaining.
  SUBCASE("put_multiple_characters_with_chaining") {
    OStringStream<FixedString<32>> stream;

    stream.put('H').put('e').put('l').put('l').put('o');

    REQUIRE(stream.str() == "Hello");
  }

  // put() to stream and verify content.
  SUBCASE("put_to_stream_and_verify_content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));

    stream.put('1').put('2').put('3');

    REQUIRE(stream.str() == "Test123");
  }

  // put() special characters (space, newline, tab).
  SUBCASE("put_special_characters") {
    OStringStream<FixedString<32>> stream;

    stream.put(' ').put('\n').put('\t').put('!');

    REQUIRE(stream.str().size() == 4);
    REQUIRE(stream.str()[0] == ' ');
    REQUIRE(stream.str()[1] == '\n');
    REQUIRE(stream.str()[2] == '\t');
    REQUIRE(stream.str()[3] == '!');
  }

  // put() returns reference for chaining.
  SUBCASE("put_returns_reference_for_chaining") {
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

// write() appends count bytes; returns reference for chaining.
TEST_CASE("core/o_string_stream/write") {
  // write() to empty stream.
  SUBCASE("write_to_empty_stream") {
    OStringStream<FixedString<32>> stream;
    const char * buffer = "Hello";

    stream.write(buffer, 5);

    REQUIRE(stream.str() == "Hello");
  }

  // write() to stream with content.
  SUBCASE("write_to_stream_with_content") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));
    const char * buffer = "123";

    stream.write(buffer, 3);

    REQUIRE(stream.str() == "Test123");
  }

  // write() with zero count leaves content unchanged.
  SUBCASE("write_zero_count") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Initial"));
    const char * buffer = "Data";

    stream.write(buffer, 0);

    REQUIRE(stream.str() == "Initial");
  }

  // write() partial string.
  SUBCASE("write_partial_string") {
    OStringStream<FixedString<32>> stream;
    const char * buffer = "Hello World";

    stream.write(buffer, 5);

    REQUIRE(stream.str() == "Hello");
  }

  // write() multiple times with chaining.
  SUBCASE("write_multiple_times_with_chaining") {
    OStringStream<FixedString<32>> stream;
    const char * buffer1 = "Hello";
    const char * buffer2 = " World";

    stream.write(buffer1, 5).write(buffer2, 6);

    REQUIRE(stream.str() == "Hello World");
  }

  // write() binary data.
  SUBCASE("write_binary_data") {
    OStringStream<FixedString<32>> stream;
    constexpr const char buffer[] = {'H', 'e', 'l', 'l', 'o'};

    stream.write(buffer, 5);

    REQUIRE(stream.str() == "Hello");
  }

  // write() returns reference for chaining.
  SUBCASE("write_returns_reference_for_chaining") {
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

// tellp() returns current write position.
TEST_CASE("core/o_string_stream/tellp") {
  // tellp() on empty stream is zero.
  SUBCASE("tellp_on_empty_stream") {
    constexpr OStringStream<FixedString<32>> stream;

    REQUIRE(stream.tellp() == 0);

    static_assert(stream.tellp() == 0, "tellp must be 0 for empty stream");
  }

  // tellp() after construction equals length.
  SUBCASE("tellp_after_construction") {
    constexpr OStringStream<FixedString<32>> stream(FixedString<16>("Hello"));

    REQUIRE(stream.tellp() == 5);

    static_assert(stream.tellp() == 5, "tellp must match string length after construction");
  }

  // tellp() after put().
  SUBCASE("tellp_after_put") {
    OStringStream<FixedString<32>> stream;

    REQUIRE(stream.tellp() == 0);

    stream.put('A');
    REQUIRE(stream.tellp() == 1);

    stream.put('B');
    REQUIRE(stream.tellp() == 2);
  }

  // tellp() after write().
  SUBCASE("tellp_after_write") {
    OStringStream<FixedString<32>> stream;
    const char * buffer = "Hello";

    REQUIRE(stream.tellp() == 0);

    stream.write(buffer, 5);
    REQUIRE(stream.tellp() == 5);

    stream.write(buffer, 5);
    REQUIRE(stream.tellp() == 10);
  }

  // tellp() after multiple operations.
  SUBCASE("tellp_after_multiple_operations") {
    OStringStream<FixedString<32>> stream(FixedString<16>("Test"));

    REQUIRE(stream.tellp() == 4);

    stream.put('1');
    REQUIRE(stream.tellp() == 5);

    stream.write("23", 2);
    REQUIRE(stream.tellp() == 7);
  }

  // tellp() equals string size.
  SUBCASE("tellp_equals_string_size") {
    OStringStream<FixedString<32>> stream;

    stream.put('A').put('B').put('C');

    REQUIRE(stream.tellp() == stream.str().size());
    REQUIRE(stream.tellp() == 3);
  }
}

// precision() get/set; default 6.
TEST_CASE("core/o_string_stream/precision") {
  // Default precision is 6.
  SUBCASE("default_precision") {
    constexpr OStringStream<FixedString<32>> stream;

    REQUIRE(stream.precision() == 6);

    static_assert(stream.precision() == 6, "default precision must be 6");
  }

  // set_precision() and precision() getter.
  SUBCASE("set_precision") {
    OStringStream<FixedString<32>> stream;

    REQUIRE(stream.precision() == 6);

    const auto oldPrecision = stream.precision(10);

    REQUIRE(oldPrecision == 6);
    REQUIRE(stream.precision() == 10);
  }

  // set_precision() multiple times.
  SUBCASE("set_precision_multiple_times") {
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

  // Precision independent of stream content.
  SUBCASE("precision_is_independent_of_stream_content") {
    OStringStream<FixedString<32>> stream;

    REQUIRE(stream.precision() == 6);

    stream.put('A').put('B');
    REQUIRE(stream.precision() == 6);

    stream.precision(3);
    REQUIRE(stream.precision() == 3);
    REQUIRE(stream.str() == "AB");
  }
}

// operator<< for bool, integers, float, double, pointer, char, StringLike, C string.
TEST_CASE("core/o_string_stream/operator_insert") {
  // operator<< true.
  SUBCASE("insert_true_boolean_value") {
    OStringStream<FixedString<32>> stream;

    stream << true;

    REQUIRE(stream.str() == "true");
  }

  // operator<< false.
  SUBCASE("insert_false_boolean_value") {
    OStringStream<FixedString<32>> stream;

    stream << false;

    REQUIRE(stream.str() == "false");
  }

  // operator<< boolean to stream with content.
  SUBCASE("insert_boolean_to_stream_with_content") {
    OStringStream<FixedString<32>> stream(CStringView("Value: "));

    stream << true;

    REQUIRE(stream.str() == "Value: true");
  }

  // operator<< chaining with boolean.
  SUBCASE("operator_chaining_with_boolean") {
    OStringStream<FixedString<32>> stream;

    stream << true << false << true;

    REQUIRE(stream.str() == "truefalsetrue");
  }

  // operator<< boolean with separator.
  SUBCASE("insert_boolean_with_separator") {
    OStringStream<FixedString<32>> stream;

    stream << true;
    stream.put(' ');
    stream << false;

    REQUIRE(stream.str() == "true false");
  }

  // operator<< boolean returns reference for chaining.
  SUBCASE("insert_boolean_returns_reference_for_chaining") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;

    auto & ref1 = stream1 << true;
    auto & ref2 = stream2 << false;

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "true");
    REQUIRE(stream2.str() == "false");
  }

  // operator<< int8_t and uint8_t.
  SUBCASE("insert_int8_integer") {
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

  // operator<< int16_t and uint16_t.
  SUBCASE("insert_int16_integer") {
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

  // operator<< int32_t and uint32_t.
  SUBCASE("insert_int32_integer") {
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

  // operator<< int64_t and uint64_t.
  SUBCASE("insert_int64_integer") {
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

  // operator<< float.
  SUBCASE("insert_float") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    stream1 << 123.456f;
    stream2 << -123.456f;

    REQUIRE(stream1.str() == "123.456");
    REQUIRE(stream2.str() == "-123.456");
  }

  // operator<< double.
  SUBCASE("insert_double") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    stream1 << 123.456;
    stream2 << -123.456;

    REQUIRE(stream1.str() == "123.456");
    REQUIRE(stream2.str() == "-123.456");
  }

  // operator<< int and unsigned.
  SUBCASE("insert_int_integer") {
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

  // operator<< zero values for all integer types.
  SUBCASE("insert_zero_values") {
    OStringStream<FixedString<32>> stream;

    stream << static_cast<int8_t>(0) << static_cast<uint8_t>(0) << static_cast<int16_t>(0) << static_cast<uint16_t>(0)
           << static_cast<int32_t>(0) << static_cast<uint32_t>(0) << static_cast<int64_t>(0) << static_cast<uint64_t>(0)
           << 0.0f << 0.0 << 0 << 0U;

    REQUIRE(stream.str() == "000000000000");
  }

  // operator<< numbers to stream with content.
  SUBCASE("insert_numbers_to_stream_with_content") {
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

  // operator<< int8 returns reference for chaining.
  SUBCASE("insert_int8_returns_reference_for_chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << static_cast<int8_t>(-100);
    auto & ref2 = stream2 << static_cast<uint8_t>(200);

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "200");
  }

  // operator<< int16 returns reference for chaining.
  SUBCASE("insert_int16_returns_reference_for_chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << static_cast<int16_t>(-100);
    auto & ref2 = stream2 << static_cast<uint16_t>(200);

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "200");
  }

  // operator<< int32 returns reference for chaining.
  SUBCASE("insert_int32_returns_reference_for_chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << static_cast<int32_t>(-100);
    auto & ref2 = stream2 << static_cast<uint32_t>(200);

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "200");
  }

  // operator<< int64 returns reference for chaining.
  SUBCASE("insert_int64_returns_reference_for_chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << static_cast<int64_t>(-100);
    auto & ref2 = stream2 << static_cast<uint64_t>(200);

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "200");
  }

  // operator<< float returns reference for chaining.
  SUBCASE("insert_float_returns_reference_for_chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << 100.5f;
    auto & ref2 = stream2 << 200.75f;

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "100.5");
    REQUIRE(stream2.str() == "200.75");
  }

  // operator<< double returns reference for chaining.
  SUBCASE("insert_double_returns_reference_for_chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << 100.5;
    auto & ref2 = stream2 << 200.75;

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "100.5");
    REQUIRE(stream2.str() == "200.75");
  }

  // operator<< int returns reference for chaining.
  SUBCASE("insert_int_returns_reference_for_chaining") {
    OStringStream<FixedString<16>> stream1;
    OStringStream<FixedString<16>> stream2;

    auto & ref1 = stream1 << -100;
    auto & ref2 = stream2 << 100U;

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "-100");
    REQUIRE(stream2.str() == "100");
  }

  // operator<< int8 min/max with separator.
  SUBCASE("insert_int8_min_max_with_separator") {
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

  // operator<< int16 min/max with separator.
  SUBCASE("insert_int16_min_max_with_separator") {
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

  // operator<< int32 min/max with separator.
  SUBCASE("insert_int32_min_max_with_separator") {
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

  // operator<< int64 min/max with separator.
  SUBCASE("insert_int64_min_max_with_separator") {
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

  // operator<< int min/max with separator.
  SUBCASE("insert_int_min_max_with_separator") {
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

  // operator<< float with custom precision.
  SUBCASE("insert_float_with_custom_precision") {
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

  // operator<< double with custom precision.
  SUBCASE("insert_double_with_custom_precision") {
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

  // operator<< void pointer and nullptr.
  SUBCASE("insert_void_pointer") {
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

  // operator<< pointer to stream with content.
  SUBCASE("insert_pointer_to_stream_with_content") {
    OStringStream<FixedString<32>> stream1(CStringView("Ptr: "));
    OStringStream<FixedString<32>> stream2(CStringView("Ptr: "));

    constexpr int value = 100;

    stream1 << &value;
    stream2 << nullptr;

    REQUIRE(stream1.str().starts_with("Ptr: 0x"));
    REQUIRE(stream1.str().size() > 6);
    REQUIRE(stream2.str() == "Ptr: nullptr");
  }

  // operator<< char.
  SUBCASE("insert_char_type_character") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;

    stream1 << 'A';
    stream2 << 'Z';

    REQUIRE(stream1.str() == "A");
    REQUIRE(stream2.str() == "Z");
  }

  // operator<< char to stream with content.
  SUBCASE("insert_char_type_to_stream_with_content") {
    OStringStream<FixedString<32>> stream(CStringView("Hello"));

    stream << '!';

    REQUIRE(stream.str() == "Hello!");
  }

  // operator<< char with chaining.
  SUBCASE("insert_char_type_with_chaining") {
    OStringStream<FixedString<32>> stream;

    stream << 'H' << 'e' << 'l' << 'l' << 'o';

    REQUIRE(stream.str() == "Hello");
  }

  // operator<< special characters.
  SUBCASE("insert_char_type_special_characters") {
    OStringStream<FixedString<32>> stream;

    stream << ' ' << '\n' << '\t' << '!';

    REQUIRE(stream.str().size() == 4);
    REQUIRE(stream.str()[0] == ' ');
    REQUIRE(stream.str()[1] == '\n');
    REQUIRE(stream.str()[2] == '\t');
    REQUIRE(stream.str()[3] == '!');
  }

  // operator<< char returns reference for chaining.
  SUBCASE("insert_char_type_returns_reference_for_chaining") {
    OStringStream<FixedString<32>> stream1;
    OStringStream<FixedString<32>> stream2;

    auto & ref1 = stream1 << 'A';
    auto & ref2 = stream2 << 'B';

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "A");
    REQUIRE(stream2.str() == "B");
  }

  // operator<< FixedString.
  SUBCASE("insert_string_like_fixed_string") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;

    const FixedString<16> str1("Hello");
    const FixedString<16> str2("World");

    stream1 << str1;
    stream2 << str2;

    REQUIRE(stream1.str() == "Hello");
    REQUIRE(stream2.str() == "World");
  }

  // operator<< CStringView.
  SUBCASE("insert_string_like_c_string_view") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;

    const CStringView view1("Test");
    const CStringView view2("String");

    stream1 << view1;
    stream2 << view2;

    REQUIRE(stream1.str() == "Test");
    REQUIRE(stream2.str() == "String");
  }

  // operator<< StringLike to stream with content.
  SUBCASE("insert_string_like_to_stream_with_content") {
    OStringStream<FixedString<64>> stream(CStringView("Prefix: "));

    const FixedString<16> suffix("Suffix");

    stream << suffix;

    REQUIRE(stream.str() == "Prefix: Suffix");
  }

  // operator<< StringLike with chaining.
  SUBCASE("insert_string_like_with_chaining") {
    OStringStream<FixedString<64>> stream;

    const FixedString<16> hello("Hello");
    const CStringView space(" ");
    const FixedString<16> world("World");

    stream << hello << space << world;

    REQUIRE(stream.str() == "Hello World");
  }

  // operator<< StringLike returns reference for chaining.
  SUBCASE("insert_string_like_returns_reference_for_chaining") {
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

  // operator<< C string literal.
  SUBCASE("insert_c_string_literal") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;

    stream1 << "Hello";
    stream2 << "World";

    REQUIRE(stream1.str() == "Hello");
    REQUIRE(stream2.str() == "World");
  }

  // operator<< C string to stream with content.
  SUBCASE("insert_c_string_to_stream_with_content") {
    OStringStream<FixedString<64>> stream(CStringView("Prefix: "));

    stream << "Suffix";

    REQUIRE(stream.str() == "Prefix: Suffix");
  }

  // operator<< C string with chaining.
  SUBCASE("insert_c_string_with_chaining") {
    OStringStream<FixedString<64>> stream;

    stream << "Hello" << " " << "World";

    REQUIRE(stream.str() == "Hello World");
  }

  // operator<< C string returns reference for chaining.
  SUBCASE("insert_c_string_returns_reference_for_chaining") {
    OStringStream<FixedString<64>> stream1;
    OStringStream<FixedString<64>> stream2;

    auto & ref1 = stream1 << "First";
    auto & ref2 = stream2 << "Second";

    REQUIRE(&ref1 == &stream1);
    REQUIRE(&ref2 == &stream2);
    REQUIRE(stream1.str() == "First");
    REQUIRE(stream2.str() == "Second");
  }

  // operator<< empty C string.
  SUBCASE("insert_c_string_empty_string") {
    OStringStream<FixedString<64>> stream;

    stream << "";

    REQUIRE(stream.str() == "");
  }

  // operator<< C string with special characters.
  SUBCASE("insert_c_string_with_special_characters") {
    OStringStream<FixedString<64>> stream;

    stream << "A\nB\tC";

    REQUIRE(stream.str() == "A\nB\tC");
  }
}

} // namespace toy
