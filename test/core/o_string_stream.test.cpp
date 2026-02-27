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
/*
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
    array<OStringStream<FixedString<32>>, 2> streams;

    streams[0].precision(2);
    streams[1].precision(10);

    streams[0].swap(streams[1]);

    REQUIRE(streams[0].str() == "");
    REQUIRE(streams[0].precision() == 10);
    REQUIRE(streams[1].str() == "");
    REQUIRE(streams[1].precision() == 2);
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
    array<OStringStream<FixedString<32>>, 2> streams;

    auto & ref0 = streams[0].put('A');
    auto & ref1 = streams[1].put('B');

    REQUIRE(&ref0 == &streams[0]);
    REQUIRE(&ref1 == &streams[1]);
    REQUIRE(streams[0].str() == "A");
    REQUIRE(streams[1].str() == "B");
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
    array<OStringStream<FixedString<32>>, 2> streams;

    auto & ref0 = streams[0].write("First", 5);
    auto & ref1 = streams[1].write("Second", 6);

    REQUIRE(&ref0 == &streams[0]);
    REQUIRE(&ref1 == &streams[1]);
    REQUIRE(streams[0].str() == "First");
    REQUIRE(streams[1].str() == "Second");
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
  // operator<< bool.
  SUBCASE("insert_boolean_value") {
    array<OStringStream<FixedString<8>>, 2> streams;

    streams[0] << true;
    streams[1] << false;

    REQUIRE(streams[0].str() == "true");
    REQUIRE(streams[1].str() == "false");
  }

  // operator<< boolean to stream with content.
  SUBCASE("insert_boolean_to_stream_with_content") {
    OStringStream<FixedString<16>> stream(CStringView("Value: "));

    stream << true;

    REQUIRE(stream.str() == "Value: true");
  }

  // operator<< chaining with boolean.
  SUBCASE("operator_chaining_with_boolean") {
    OStringStream<FixedString<16>> stream;

    stream << true << false << true;

    REQUIRE(stream.str() == "truefalsetrue");
  }

  // operator<< boolean returns reference for chaining.
  SUBCASE("insert_boolean_returns_reference_for_chaining") {
    array<OStringStream<FixedString<8>>, 2> streams;

    auto & ref0 = streams[0] << true;
    auto & ref1 = streams[1] << false;

    REQUIRE(&ref0 == &streams[0]);
    REQUIRE(&ref1 == &streams[1]);

    REQUIRE(streams[0].str() == "true");
    REQUIRE(streams[1].str() == "false");
  }

  // operator<< boolean with separator.
  SUBCASE("insert_boolean_with_separator") {
    OStringStream<FixedString<16>> stream;

    stream << true;
    stream.put(' ');
    stream << false;

    REQUIRE(stream.str() == "true false");
  }

  // operator<< integer numbers.
  SUBCASE("insert_integers") {
    array<OStringStream<FixedString<24>>, 36> streams;

    streams[0] << long(123);
    streams[1] << long(-123);
    streams[2] << static_cast<unsigned long>(123);
    streams[3] << static_cast<unsigned long>(0);
    streams[4] << numeric_limits<long long>::min();
    streams[5] << numeric_limits<long long>::max();
    streams[6] << numeric_limits<unsigned long long>::min();
    streams[7] << numeric_limits<unsigned long long>::max();
    streams[8] << numeric_limits<short>::min();
    streams[9] << numeric_limits<short>::max();
    streams[10] << numeric_limits<unsigned short>::min();
    streams[11] << numeric_limits<unsigned short>::max();
    streams[12] << numeric_limits<int>::min();
    streams[13] << numeric_limits<int>::max();
    streams[14] << numeric_limits<unsigned int>::min();
    streams[15] << numeric_limits<unsigned int>::max();
    streams[16] << numeric_limits<signed char>::min();
    streams[17] << numeric_limits<signed char>::max();
    streams[18] << numeric_limits<unsigned char>::min();
    streams[19] << numeric_limits<unsigned char>::max();
    streams[20] << numeric_limits<int8_t>::min();
    streams[21] << numeric_limits<int8_t>::max();
    streams[22] << numeric_limits<uint8_t>::min();
    streams[23] << numeric_limits<uint8_t>::max();
    streams[24] << numeric_limits<int16_t>::min();
    streams[25] << numeric_limits<int16_t>::max();
    streams[26] << numeric_limits<uint16_t>::min();
    streams[27] << numeric_limits<uint16_t>::max();
    streams[28] << numeric_limits<int32_t>::min();
    streams[29] << numeric_limits<int32_t>::max();
    streams[30] << numeric_limits<uint32_t>::min();
    streams[31] << numeric_limits<uint32_t>::max();
    streams[32] << numeric_limits<int64_t>::min();
    streams[33] << numeric_limits<int64_t>::max();
    streams[34] << numeric_limits<uint64_t>::min();
    streams[35] << numeric_limits<uint64_t>::max();

    REQUIRE(streams[0].str() == "123");
    REQUIRE(streams[1].str() == "-123");
    REQUIRE(streams[2].str() == "123");
    REQUIRE(streams[3].str() == "0");
    REQUIRE(streams[4].str() == "-9223372036854775808");
    REQUIRE(streams[5].str() == "9223372036854775807");
    REQUIRE(streams[6].str() == "0");
    REQUIRE(streams[7].str() == "18446744073709551615");
    REQUIRE(streams[8].str() == "-32768");
    REQUIRE(streams[9].str() == "32767");
    REQUIRE(streams[10].str() == "0");
    REQUIRE(streams[11].str() == "65535");
    REQUIRE(streams[12].str() == "-2147483648");
    REQUIRE(streams[13].str() == "2147483647");
    REQUIRE(streams[14].str() == "0");
    REQUIRE(streams[15].str() == "4294967295");
    REQUIRE(streams[16].str() == "-128");
    REQUIRE(streams[17].str() == "127");
    REQUIRE(streams[18].str() == "0");
    REQUIRE(streams[19].str() == "255");
    REQUIRE(streams[20].str() == "-128");
    REQUIRE(streams[21].str() == "127");
    REQUIRE(streams[22].str() == "0");
    REQUIRE(streams[23].str() == "255");
    REQUIRE(streams[24].str() == "-32768");
    REQUIRE(streams[25].str() == "32767");
    REQUIRE(streams[26].str() == "0");
    REQUIRE(streams[27].str() == "65535");
    REQUIRE(streams[28].str() == "-2147483648");
    REQUIRE(streams[29].str() == "2147483647");
    REQUIRE(streams[30].str() == "0");
    REQUIRE(streams[31].str() == "4294967295");
    REQUIRE(streams[32].str() == "-9223372036854775808");
    REQUIRE(streams[33].str() == "9223372036854775807");
    REQUIRE(streams[34].str() == "0");
    REQUIRE(streams[35].str() == "18446744073709551615");
  }

  // operator<< natural numbers.
  SUBCASE("insert_natural_numbers") {
    array<OStringStream<FixedString<16>>, 6> streams;

    streams[0] << 123.456f;
    streams[1] << -123.456f;
    streams[2] << 0.0f;
    streams[3] << 123.456;
    streams[4] << -123.456;
    streams[5] << 0.0;

    constexpr std::array<const char *, 6> expects = {"123.456", "-123.456", "0", "123.456", "-123.456", "0"};
    for (size_t index = 0; index < expects.size(); ++index)
      REQUIRE(streams[index].str() == expects[index]);
  }

  // operator<< zero values for all integer and float types.
  SUBCASE("insert_zero_values") {
    OStringStream<FixedString<24>> stream;

    stream << long(0) << static_cast<unsigned long>(0) << static_cast<long long>(0)
           << static_cast<unsigned long long>(0) << short(0) << static_cast<unsigned short>(0) << int(0)
           << static_cast<unsigned int>(0) << static_cast<signed char>(0) << static_cast<unsigned char>(0) << int8_t(0)
           << uint8_t(0) << int16_t(0) << uint16_t(0) << int32_t(0) << uint32_t(0) << int64_t(0) << uint64_t(0) << 0.0f
           << 0.0;

    REQUIRE(stream.str() == "00000000000000000000");
  }

  // operator<< numbers to stream with content.
  SUBCASE("insert_numbers_to_stream_with_content") {
    array<OStringStream<FixedString<16>>, 12> streams;
    constexpr CStringView prefix("Value: ");
    for (auto & s : streams)
      s = OStringStream<FixedString<16>>(prefix);

    streams[0] << long(123);
    streams[1] << static_cast<unsigned long>(123);
    streams[2] << static_cast<long long>(123);
    streams[3] << static_cast<unsigned long long>(123);
    streams[4] << 123.0;
    streams[5] << short(123);
    streams[6] << int(123);
    streams[7] << static_cast<unsigned short>(123);
    streams[8] << static_cast<unsigned int>(123);
    streams[9] << 123.0f;
    streams[10] << static_cast<signed char>(123);
    streams[11] << static_cast<unsigned char>(123);

    for (size_t index = 0; index < streams.size(); ++index) {
      REQUIRE(streams[index].str() == "Value: 123");
    }
  }

  // operator<< numbers returns reference for chaining.
  SUBCASE("insert_numbers_returns_reference_for_chaining") {
    array<OStringStream<FixedString<4>>, 12> streams;

    auto & ref0 = streams[0] << long(123);
    auto & ref1 = streams[1] << static_cast<unsigned long>(234);
    auto & ref2 = streams[2] << static_cast<long long>(345);
    auto & ref3 = streams[3] << static_cast<unsigned long long>(456);
    auto & ref4 = streams[4] << 567.0;
    auto & ref5 = streams[5] << short(678);
    auto & ref6 = streams[6] << int(789);
    auto & ref7 = streams[7] << static_cast<unsigned short>(890);
    auto & ref8 = streams[8] << static_cast<unsigned int>(901);
    auto & ref9 = streams[9] << 012.0f;
    auto & ref10 = streams[10] << static_cast<signed char>(123);
    auto & ref11 = streams[11] << static_cast<unsigned char>(234);

    REQUIRE(&ref0 == &streams[0]);
    REQUIRE(&ref1 == &streams[1]);
    REQUIRE(&ref2 == &streams[2]);
    REQUIRE(&ref3 == &streams[3]);
    REQUIRE(&ref4 == &streams[4]);
    REQUIRE(&ref5 == &streams[5]);
    REQUIRE(&ref6 == &streams[6]);
    REQUIRE(&ref7 == &streams[7]);
    REQUIRE(&ref8 == &streams[8]);
    REQUIRE(&ref9 == &streams[9]);
    REQUIRE(&ref10 == &streams[10]);
    REQUIRE(&ref11 == &streams[11]);

    REQUIRE(streams[0].str() == "123");
    REQUIRE(streams[1].str() == "234");
    REQUIRE(streams[2].str() == "345");
    REQUIRE(streams[3].str() == "456");
    REQUIRE(streams[4].str() == "567");
    REQUIRE(streams[5].str() == "678");
    REQUIRE(streams[6].str() == "789");
    REQUIRE(streams[7].str() == "890");
    REQUIRE(streams[8].str() == "901");
    REQUIRE(streams[9].str() == "12");
    REQUIRE(streams[10].str() == "123");
    REQUIRE(streams[11].str() == "234");
  }

  // operator<< int8/uint8 min/max with separator.
  SUBCASE("insert_int8_uint8_min_max_with_separator") {
    OStringStream<FixedString<48>> stream;

    stream << long(123);
    stream.put(' ');
    stream << static_cast<unsigned long>(234);
    stream.put(' ');
    stream << static_cast<long long>(345);
    stream.put(' ');
    stream << static_cast<unsigned long long>(456);
    stream.put(' ');
    stream << 567.0;
    stream.put(' ');
    stream << short(678);
    stream.put(' ');
    stream << int(789);
    stream.put(' ');
    stream << static_cast<unsigned short>(890);
    stream.put(' ');
    stream << static_cast<unsigned int>(901);
    stream.put(' ');
    stream << 012.0f;
    stream.put(' ');
    stream << static_cast<signed char>(123);
    stream.put(' ');
    stream << static_cast<unsigned char>(234);

    REQUIRE(stream.str() == "123 234 345 456 567 678 789 890 901 12 123 234");
  }

  // operator<< natural numbers with custom precision.
  SUBCASE("insert_natural_numbers_with_custom_precision") {
    array<OStringStream<FixedString<24>>, 8> streams;

    streams[1].precision(3);
    streams[2].precision(1);
    streams[3].precision(15);
    streams[5].precision(3);
    streams[6].precision(1);
    streams[7].precision(15);

    streams[0] << std::numbers::pi_v<float>;
    streams[1] << std::numbers::pi_v<float>;
    streams[2] << std::numbers::pi_v<float>;
    streams[3] << std::numbers::pi_v<float>;
    streams[4] << std::numbers::pi;
    streams[5] << std::numbers::pi;
    streams[6] << std::numbers::pi;
    streams[7] << std::numbers::pi;

    const std::array<const char *, 8> expects
      = {"3.14159", "3.14", "3", "3.14159244298935", "3.14159", "3.14", "3", "3.14159244298935"};
    for (size_t index = 0; index < expects.size(); ++index)
      REQUIRE(streams[index].str() == expects[index]);
  }

  // operator<< void pointer and nullptr.
  SUBCASE("insert_void_pointer_and_nullptr") {
    array<OStringStream<FixedString<32>>, 3> streams;

    constexpr int value = 42;
    constexpr const int * ptr = nullptr;

    streams[0] << &value;
    streams[1] << ptr;
    streams[2] << nullptr;

    REQUIRE(streams[0].str().starts_with("0x"));

    if constexpr (sizeof(void *) == 4) {
      REQUIRE(streams[0].str().length() == 10u);
    } else if constexpr (sizeof(void *) == 8) {
      REQUIRE(streams[0].str().length() == 18u);
    } else {
      static_assert(sizeof(void *) == 4 || sizeof(void *) == 8, "Unsupported value size");
    }

    REQUIRE(streams[1].str() == "nullptr");
    REQUIRE(streams[2].str() == "nullptr");
  }

  // operator<< pointer to stream with content.
  SUBCASE("insert_pointer_to_stream_with_content") {
    auto stream0 = OStringStream<FixedString<32>>(CStringView("Ptr: "));
    auto stream1 = OStringStream<FixedString<32>>(CStringView("Ptr: "));

    constexpr int value = 100;

    stream0 << &value;
    stream1 << nullptr;

    REQUIRE(stream0.str().starts_with("Ptr: 0x"));
    REQUIRE(stream0.str().size() > 6);
    REQUIRE(stream1.str() == "Ptr: nullptr");
  }

  // operator<< char.
  SUBCASE("insert_char_type_character") {
    array<OStringStream<FixedString<32>>, 2> streams;

    streams[0] << 'A';
    streams[1] << 'Z';

    REQUIRE(streams[0].str() == "A");
    REQUIRE(streams[1].str() == "Z");
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
    array<OStringStream<FixedString<32>>, 2> streams;

    auto & ref0 = streams[0] << 'A';
    auto & ref1 = streams[1] << 'B';

    REQUIRE(&ref0 == &streams[0]);
    REQUIRE(&ref1 == &streams[1]);
    REQUIRE(streams[0].str() == "A");
    REQUIRE(streams[1].str() == "B");
  }

  // operator<< FixedString.
  SUBCASE("insert_string_like_fixed_string") {
    array<OStringStream<FixedString<64>>, 2> streams;

    const FixedString<16> str0("Hello");
    const FixedString<16> str1("World");

    streams[0] << str0;
    streams[1] << str1;

    REQUIRE(streams[0].str() == "Hello");
    REQUIRE(streams[1].str() == "World");
  }

  // operator<< CStringView.
  SUBCASE("insert_string_like_c_string_view") {
    array<OStringStream<FixedString<64>>, 2> streams;

    const CStringView view0("Test");
    const CStringView view1("String");

    streams[0] << view0;
    streams[1] << view1;

    REQUIRE(streams[0].str() == "Test");
    REQUIRE(streams[1].str() == "String");
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
    array<OStringStream<FixedString<64>>, 2> streams;

    const FixedString<16> str0("First");
    const FixedString<16> str1("Second");

    auto & ref0 = streams[0] << str0;
    auto & ref1 = streams[1] << str1;

    REQUIRE(&ref0 == &streams[0]);
    REQUIRE(&ref1 == &streams[1]);
    REQUIRE(streams[0].str() == "First");
    REQUIRE(streams[1].str() == "Second");
  }

  // operator<< C string literal.
  SUBCASE("insert_c_string_literal") {
    array<OStringStream<FixedString<64>>, 2> streams;

    streams[0] << "Hello";
    streams[1] << "World";

    REQUIRE(streams[0].str() == "Hello");
    REQUIRE(streams[1].str() == "World");
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
    array<OStringStream<FixedString<64>>, 2> streams;

    auto & ref0 = streams[0] << "First";
    auto & ref1 = streams[1] << "Second";

    REQUIRE(&ref0 == &streams[0]);
    REQUIRE(&ref1 == &streams[1]);
    REQUIRE(streams[0].str() == "First");
    REQUIRE(streams[1].str() == "Second");
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
*/
