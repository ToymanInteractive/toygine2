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
#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

// Default, C string, copy, character, StringLike constructors.
TEST_CASE("core/fixed_string/constructors") {
  SUBCASE("default_constructor") {
    constexpr FixedString<32> emptyStr;

    static_assert(emptyStr.size() == 0);
    static_assert(cstrcmp(emptyStr.c_str(), "") == 0);
    static_assert(emptyStr.capacity() == 31);
    static_assert(emptyStr.max_size() == 31);
  }

  SUBCASE("c_string_constructor") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<32> str2("World");
    constexpr FixedString<8> str3("Test");
    constexpr FixedString<64> str4("This is a longer string for testing");

    static_assert(str1.size() == 5);
    static_assert(cstrcmp(str1.c_str(), "Hello") == 0);
    static_assert(str2.size() == 5);
    static_assert(cstrcmp(str2.c_str(), "World") == 0);
    static_assert(str3.size() == 4);
    static_assert(cstrcmp(str3.c_str(), "Test") == 0);
    static_assert(str4.size() == 35);
    static_assert(cstrcmp(str4.c_str(), "This is a longer string for testing") == 0);
  }

  SUBCASE("copy_constructor") {
    constexpr FixedString<16> original("CopyTest");
    constexpr FixedString<16> copy1(original);
    constexpr FixedString<32> copy2(original);
    constexpr FixedString<12> copy3(original);

    static_assert(copy1.size() == 8);
    static_assert(cstrcmp(copy1.c_str(), "CopyTest") == 0);
    static_assert(copy2.size() == 8);
    static_assert(cstrcmp(copy2.c_str(), "CopyTest") == 0);
    static_assert(copy3.size() == 8);
    static_assert(cstrcmp(copy3.c_str(), "CopyTest") == 0);
    static_assert(copy1 == original);
    static_assert(copy2 == original);
    static_assert(copy3 == original);
  }

  SUBCASE("character_constructor") {
    constexpr FixedString<16> single(1, 'A');
    constexpr FixedString<32> multiple(5, 'B');
    constexpr FixedString<8> many(7, 'C');
    constexpr FixedString<64> empty(0, 'D');

    static_assert(single.size() == 1);
    static_assert(cstrcmp(single.c_str(), "A") == 0);
    static_assert(multiple.size() == 5);
    static_assert(cstrcmp(multiple.c_str(), "BBBBB") == 0);
    static_assert(many.size() == 7);
    static_assert(cstrcmp(many.c_str(), "CCCCCCC") == 0);
    static_assert(empty.size() == 0);
    static_assert(cstrcmp(empty.c_str(), "") == 0);
  }

  SUBCASE("string_like_constructor") {
    const FixedString<16> fromStd(std::string("StringLike"));
    constexpr FixedString<16> fromFix(CStringView("StringLike"));

    REQUIRE(fromStd.size() == 10);
    REQUIRE(std::strcmp(fromStd.c_str(), "StringLike") == 0);

    static_assert(fromFix.size() == 10);
    static_assert(cstrcmp(fromFix.c_str(), "StringLike") == 0);
  }

  SUBCASE("edge_cases") {
    // Empty string
    constexpr FixedString<16> empty1("");
    constexpr FixedString<32> empty2("");

    static_assert(empty1.size() == 0);
    static_assert(empty2.size() == 0);

    // Single character
    constexpr FixedString<8> single("X");

    static_assert(single.size() == 1);
    static_assert(cstrcmp(single.c_str(), "X") == 0);

    // Maximum length
    constexpr FixedString<5> maxLen("Test");

    static_assert(maxLen.size() == 4);
    static_assert(cstrcmp(maxLen.c_str(), "Test") == 0);
  }

  SUBCASE("special_characters") {
    constexpr FixedString<32> newline("Line1\nLine2");
    constexpr FixedString<32> tab("Col1\tCol2");
    constexpr FixedString<32> mixed("Mix\t\nEnd");

    static_assert(newline.size() == 11);
    static_assert(cstrcmp(newline.c_str(), "Line1\nLine2") == 0);
    static_assert(tab.size() == 9);
    static_assert(cstrcmp(tab.c_str(), "Col1\tCol2") == 0);
    static_assert(mixed.size() == 8);
    static_assert(cstrcmp(mixed.c_str(), "Mix\t\nEnd") == 0);
  }

  SUBCASE("unicode_content") {
    constexpr FixedString<64> unicode("Привет мир");
    constexpr FixedString<32> emoji("Hello 🌍");

    static_assert(unicode.size() == std::char_traits<char>::length("Привет мир"));
    static_assert(cstrcmp(unicode.c_str(), "Привет мир") == 0);
    static_assert(emoji.size() == std::char_traits<char>::length("Hello 🌍"));
    static_assert(cstrcmp(emoji.c_str(), "Hello 🌍") == 0);
  }
}

// Assignment from C string, FixedString, StringLike, character.
TEST_CASE("core/fixed_string/operators_assign") {
  SUBCASE("c_string_assignment") {
    FixedString<12> str1;
    FixedString<24> str2;
    FixedString<8> str3;

    str1 = "Hello";
    str2 = "World";
    str3 = "Test";

    REQUIRE(str1.size() == 5);
    REQUIRE(str1 == "Hello");
    REQUIRE(str2.size() == 5);
    REQUIRE(str2 == "World");
    REQUIRE(str3.size() == 4);
    REQUIRE(str3 == "Test");

    // Empty string assignment
    str1 = "";
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Long string assignment
    str2 = "This is a longer string";
    REQUIRE(str2.size() == 23);
    REQUIRE(std::strcmp(str2.c_str(), "This is a longer string") == 0);

    // Self assignment
    str2 = str2.c_str();
    REQUIRE(str2.size() == 23);
    REQUIRE(std::strcmp(str2.c_str(), "This is a longer string") == 0);

    constexpr auto constStr1 = FixedString<24>("This is a longer string");
    static_assert(constStr1.size() == 23);
    static_assert(cstrcmp(constStr1.c_str(), "This is a longer string") == 0);
  }

  SUBCASE("fixed_string_assignment_same_capacity") {
    FixedString<12> str1("Hello");
    FixedString<12> str2;
    FixedString<12> str3("World");

    str2 = str1;
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);

    str3 = str2;
    REQUIRE(str3.size() == 5);
    REQUIRE(std::strcmp(str3.c_str(), "Hello") == 0);

    // Self-assignment
    str1 = str1;
    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);

    constexpr FixedString<24> constStr1("Hello");
    constexpr FixedString<24> constStr2 = constStr1;
    static_assert(constStr2.size() == 5);
    static_assert(cstrcmp(constStr2.c_str(), "Hello") == 0);
  }

  SUBCASE("fixed_string_assignment_different_capacities") {
    FixedString<8> str1("Test");
    FixedString<16> str2;
    FixedString<32> str3;

    str2 = str1;
    REQUIRE(str2.size() == 4);
    REQUIRE(std::strcmp(str2.c_str(), "Test") == 0);

    str3 = str2;
    REQUIRE(str3.size() == 4);
    REQUIRE(std::strcmp(str3.c_str(), "Test") == 0);

    // Reverse assignment
    str1 = str3;
    REQUIRE(str1.size() == 4);
    REQUIRE(std::strcmp(str1.c_str(), "Test") == 0);

    constexpr FixedString<8> constStr1("Test");
    constexpr auto constStr2 = FixedString<32>(constStr1);
    constexpr auto constStr3 = FixedString<16>(constStr2);
    static_assert(constStr2.size() == 4);
    static_assert(cstrcmp(constStr2.c_str(), "Test") == 0);
    static_assert(constStr3.size() == 4);
    static_assert(cstrcmp(constStr3.c_str(), "Test") == 0);
  }

  SUBCASE("string_like_assignment") {
    FixedString<12> str1;
    FixedString<24> str2;

    str1 = std::string("Hello World");
    REQUIRE(str1.size() == 11);
    REQUIRE(std::strcmp(str1.c_str(), "Hello World") == 0);

    str2 = CStringView("Test String");
    REQUIRE(str2.size() == 11);
    REQUIRE(std::strcmp(str2.c_str(), "Test String") == 0);
  }

  SUBCASE("character_assignment") {
    FixedString<8> str1;
    FixedString<16> str2;
    FixedString<32> str3;

    str1 = 'A';
    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "A") == 0);

    str2 = 'B';
    REQUIRE(str2.size() == 1);
    REQUIRE(std::strcmp(str2.c_str(), "B") == 0);

    str3 = 'Z';
    REQUIRE(str3.size() == 1);
    REQUIRE(std::strcmp(str3.c_str(), "Z") == 0);

    // Special characters
    str1 = '\n';
    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "\n") == 0);

    str2 = '\t';
    REQUIRE(str2.size() == 1);
    REQUIRE(std::strcmp(str2.c_str(), "\t") == 0);
  }

  SUBCASE("edge_cases") {
    FixedString<4> str1;
    FixedString<8> str2;

    // Maximum length assignment
    str1 = "ABC"; // 3 chars + null terminator = 4 total
    REQUIRE(str1.size() == 3);
    REQUIRE(std::strcmp(str1.c_str(), "ABC") == 0);

    // Non-empty to empty
    str2 = "XYZ";
    str1 = str2;
    REQUIRE(str1.size() == 3);
    REQUIRE(std::strcmp(str1.c_str(), "XYZ") == 0);

    // Empty to non-empty
    str2 = "";
    str1 = str2;
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
  }

  SUBCASE("special_characters") {
    FixedString<16> str1;
    FixedString<24> str2;

    str1 = "Line1\nLine2";
    REQUIRE(str1.size() == 11);
    REQUIRE(std::strcmp(str1.c_str(), "Line1\nLine2") == 0);

    str2 = "Col1\tCol2";
    REQUIRE(str2.size() == 9);
    REQUIRE(std::strcmp(str2.c_str(), "Col1\tCol2") == 0);

    // Mixed special characters
    str1 = "Mix\t\nEnd";
    REQUIRE(str1.size() == 8);
    REQUIRE(std::strcmp(str1.c_str(), "Mix\t\nEnd") == 0);
  }

  SUBCASE("unicode_content") {
    FixedString<32> str1;
    FixedString<48> str2;

    str1 = "Привет";
    REQUIRE(str1.size() == std::char_traits<char>::length("Привет"));
    REQUIRE(std::strcmp(str1.c_str(), "Привет") == 0);

    str2 = "Hello 🌍";
    REQUIRE(str2.size() == std::char_traits<char>::length("Hello 🌍"));
    REQUIRE(std::strcmp(str2.c_str(), "Hello 🌍") == 0);

    str1 = str2;
    REQUIRE(str1.size() == std::char_traits<char>::length("Hello 🌍"));
    REQUIRE(std::strcmp(str1.c_str(), "Hello 🌍") == 0);
  }
}

// assign() from C string, FixedString, StringLike, character.
TEST_CASE("core/fixed_string/assign") {
  SUBCASE("c_string_assignment") {
    FixedString<16> str1;
    FixedString<32> str2;
    FixedString<8> str3;

    // Basic assignment
    str1.assign("Hello");
    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);

    str2.assign("World");
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "World") == 0);

    // Empty string assignment
    str1.assign("");
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Long string assignment
    str2.assign("VeryLongString");
    REQUIRE(str2.size() == 14);
    REQUIRE(std::strcmp(str2.c_str(), "VeryLongString") == 0);

    // Single character
    str3.assign("A");
    REQUIRE(str3.size() == 1);
    REQUIRE(std::strcmp(str3.c_str(), "A") == 0);

    constexpr auto constStr1 = FixedString<16>().assign("Hello");
    constexpr auto constStr2 = FixedString<32>("World").assign("VeryLongString");
    constexpr auto constStr3 = FixedString<16>("A").assign("");
    static_assert(constStr1.size() == 5);
    static_assert(cstrcmp(constStr1.c_str(), "Hello") == 0);
    static_assert(constStr2.size() == 14);
    static_assert(cstrcmp(constStr2.c_str(), "VeryLongString") == 0);
    static_assert(constStr3.size() == 0);
    static_assert(cstrcmp(constStr3.c_str(), "") == 0);
  }

  SUBCASE("fixed_string_assignment_same_capacity") {
    FixedString<16> str1("Hello");
    FixedString<16> str2("World");
    FixedString<16> str3;

    // Basic assignment
    str3.assign(str1);
    REQUIRE(str3.size() == 5);
    REQUIRE(std::strcmp(str3.c_str(), "Hello") == 0);

    // Assignment from another string
    str2.assign(str1);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);

    // Self-assignment
    str1.assign(str1);
    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);

    // Empty string assignment
    FixedString<16> emptyStr("");
    str1.assign(emptyStr);
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    constexpr FixedString<16> constStr1("Hello");
    constexpr auto constStr2 = FixedString<16>("World").assign(constStr1);
    constexpr auto constStr3 = FixedString<16>().assign(constStr2);
    static_assert(constStr2.size() == 5);
    static_assert(cstrcmp(constStr2.c_str(), "Hello") == 0);
    static_assert(constStr3.size() == 5);
    static_assert(cstrcmp(constStr3.c_str(), "Hello") == 0);
  }

  SUBCASE("fixed_string_assignment_different_capacities") {
    FixedString<8> str1("Hi");
    FixedString<16> str2("Hello");

    // Assign from smaller to larger
    str2.assign(str1);
    REQUIRE(str2.size() == 2);
    REQUIRE(std::strcmp(str2.c_str(), "Hi") == 0);

    // Assign from larger to smaller
    str1.assign(str2);
    REQUIRE(str1.size() == 2);
    REQUIRE(std::strcmp(str1.c_str(), "Hi") == 0);

    constexpr FixedString<8> constStr1("Hi");
    constexpr FixedString<16> constStr2("Hello");
    constexpr auto constStr3 = FixedString<8>("Hi").assign(constStr2);
    constexpr auto constStr4 = FixedString<16>("Hello").assign(constStr1);
    static_assert(constStr3.size() == 5);
    static_assert(cstrcmp(constStr3.c_str(), "Hello") == 0);
    static_assert(constStr4.size() == 2);
    static_assert(cstrcmp(constStr4.c_str(), "Hi") == 0);
  }

  SUBCASE("string_like_assignment") {
    FixedString<16> str1;

    // Assign from std::string
    str1.assign(std::string("Hello"));
    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);

    // Assign from another std::string
    str1.assign(std::string("World"));
    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "World") == 0);

    // Assign from empty std::string
    str1.assign(std::string(""));
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
  }

  SUBCASE("character_assignment") {
    FixedString<16> str1;
    FixedString<8> str2;

    // Single character assignment
    str1.assign('A');
    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "A") == 0);

    // Multiple character assignment
    str2.assign('B', 3);
    REQUIRE(str2.size() == 3);
    REQUIRE(std::strcmp(str2.c_str(), "BBB") == 0);

    // Zero count assignment
    str1.assign('C', 0);
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Large count assignment
    str2.assign('D', 5);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "DDDDD") == 0);

    constexpr auto constStr1 = FixedString<8>().assign('A');
    constexpr auto constStr2 = FixedString<8>().assign('B', 3);
    static_assert(constStr1.size() == 1);
    static_assert(cstrcmp(constStr1.c_str(), "A") == 0);
    static_assert(constStr2.size() == 3);
    static_assert(cstrcmp(constStr2.c_str(), "BBB") == 0);
  }

  SUBCASE("edge_cases") {
    FixedString<4> str1("ABC");
    FixedString<8> str2("ABC");
    FixedString<16> str3("ABCD");

    // Assign to maximum capacity
    str1.assign("XYZ");
    REQUIRE(str1.size() == 3);
    REQUIRE(std::strcmp(str1.c_str(), "XYZ") == 0);

    // Assign from own c_str() (no-op path)
    str2.assign(str2.c_str());
    REQUIRE(str2.size() == 3);
    REQUIRE(std::strcmp(str2.c_str(), "ABC") == 0);

    // Assign empty string
    str3.assign("");
    REQUIRE(str3.size() == 0);
    REQUIRE(std::strcmp(str3.c_str(), "") == 0);

    constexpr auto constStr1 = FixedString<4>("ABC").assign("XYZ");
    constexpr auto constStr2 = FixedString<8>("ABCD").assign("");
    static_assert(constStr1.size() == 3);
    static_assert(cstrcmp(constStr1.c_str(), "XYZ") == 0);
    static_assert(constStr2.size() == 0);
    static_assert(cstrcmp(constStr2.c_str(), "") == 0);
  }

  SUBCASE("special_characters") {
    FixedString<32> str1;
    FixedString<16> str2;

    // Newline and tab
    str1.assign("Hello\n\tWorld");
    REQUIRE(str1.size() == 12);
    REQUIRE(std::strcmp(str1.c_str(), "Hello\n\tWorld") == 0);

    // Special characters
    str2.assign("!@#$%^&*()");
    REQUIRE(str2.size() == 10);
    REQUIRE(std::strcmp(str2.c_str(), "!@#$%^&*()") == 0);

    constexpr auto constStr1 = FixedString<32>().assign("Hello\n\tWorld");
    constexpr auto constStr2 = FixedString<16>().assign("!@#$%^&*()");
    static_assert(constStr1.size() == 12);
    static_assert(cstrcmp(constStr1.c_str(), "Hello\n\tWorld") == 0);
    static_assert(constStr2.size() == 10);
    static_assert(cstrcmp(constStr2.c_str(), "!@#$%^&*()") == 0);
  }

  SUBCASE("unicode_content") {
    FixedString<32> str1;
    FixedString<16> str2;

    // Unicode characters
    str1.assign("Hello 世界");
    REQUIRE(str1.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(std::strcmp(str1.c_str(), "Hello 世界") == 0);

    str2.assign("Test 🌍");
    REQUIRE(str2.size() == std::char_traits<char>::length("Test 🌍"));
    REQUIRE(std::strcmp(str2.c_str(), "Test 🌍") == 0);

    constexpr auto constStr1 = FixedString<32>().assign("Hello 世界");
    constexpr auto constStr2 = FixedString<16>().assign("Test 🌍");
    static_assert(constStr1.size() == std::char_traits<char>::length("Hello 世界"));
    static_assert(cstrcmp(constStr1.c_str(), "Hello 世界") == 0);
    static_assert(constStr2.size() == std::char_traits<char>::length("Test 🌍"));
    static_assert(cstrcmp(constStr2.c_str(), "Test 🌍") == 0);
  }

  SUBCASE("chaining_assign") {
    FixedString<16> str1;

    // Chaining assign operations
    str1.assign("a").assign("b");
    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "b") == 0);

    // Multiple chaining
    str1.assign("Hello").assign("World").assign("Test");
    REQUIRE(str1.size() == 4);
    REQUIRE(std::strcmp(str1.c_str(), "Test") == 0);

    // Chaining with different types
    str1.assign("Initial").assign(std::string("Chained"));
    REQUIRE(str1.size() == 7);
    REQUIRE(std::strcmp(str1.c_str(), "Chained") == 0);

    constexpr auto constStr1 = FixedString<16>("a").assign("b");
    constexpr auto constStr2 = FixedString<16>("Hello").assign("Test");
    static_assert(constStr1.size() == 1);
    static_assert(cstrcmp(constStr1.c_str(), "b") == 0);
    static_assert(constStr2.size() == 4);
    static_assert(cstrcmp(constStr2.c_str(), "Test") == 0);
  }
}

// at() bounds-checked access.
TEST_CASE("core/fixed_string/at") {
  SUBCASE("basic_at_functionality") {
    FixedString<8> str("Hello");

    REQUIRE(str.at(0) == 'H');
    REQUIRE(str.at(1) == 'e');
    REQUIRE(str.at(2) == 'l');
    REQUIRE(str.at(3) == 'l');
    REQUIRE(str.at(4) == 'o');

    // Modify characters using at()
    str.at(0) = 'h';
    str.at(4) = '!';

    REQUIRE(str.at(0) == 'h');
    REQUIRE(str.at(4) == '!');

    REQUIRE(str.size() == 5);
    REQUIRE(std::strcmp(str.c_str(), "hell!") == 0);
  }

  SUBCASE("const_at_access") {
    constexpr FixedString<8> str("World");

    REQUIRE(str.at(0) == 'W');
    REQUIRE(str.at(1) == 'o');
    REQUIRE(str.at(2) == 'r');
    REQUIRE(str.at(3) == 'l');
    REQUIRE(str.at(4) == 'd');

    static_assert(str.at(0) == 'W');
    static_assert(str.at(1) == 'o');
    static_assert(str.at(2) == 'r');
    static_assert(str.at(3) == 'l');
    static_assert(str.at(4) == 'd');
  }

  SUBCASE("empty_string") {
    constexpr FixedString<8> str;

    REQUIRE(str.at(0) == '\0');

    static_assert(str.at(0) == '\0');
  }

  SUBCASE("reference_modification") {
    FixedString<8> str("Test");

    REQUIRE(str.at(0) == 'T');

    // Get reference and modify
    auto & ref = str.at(0);
    ref = 'B';

    REQUIRE(str.at(0) == 'B');

    REQUIRE(str.size() == 4);
    REQUIRE(std::strcmp(str.c_str(), "Best") == 0);
  }

  SUBCASE("sequential_modifications") {
    FixedString<8> str("abcd");

    // Modify all characters
    str.at(0) = 'e';
    str.at(1) = 'f';
    str.at(2) = 'g';
    str.at(3) = 'h';

    REQUIRE(str.at(0) == 'e');
    REQUIRE(str.at(1) == 'f');
    REQUIRE(str.at(2) == 'g');
    REQUIRE(str.at(3) == 'h');

    REQUIRE(str.size() == 4);
    REQUIRE(std::strcmp(str.c_str(), "efgh") == 0);
  }

  SUBCASE("single_character_string") {
    FixedString<8> str("A");

    REQUIRE(str.at(0) == 'A');

    str.at(0) = 'B';

    REQUIRE(str.at(0) == 'B');

    REQUIRE(str.size() == 1);
    REQUIRE(std::strcmp(str.c_str(), "B") == 0);
  }

  SUBCASE("special_characters") {
    FixedString<8> str("A\nB\tC");

    REQUIRE(str.at(0) == 'A');
    REQUIRE(str.at(1) == '\n');
    REQUIRE(str.at(2) == 'B');
    REQUIRE(str.at(3) == '\t');
    REQUIRE(str.at(4) == 'C');

    str.at(1) = ' ';
    str.at(3) = ' ';

    REQUIRE(str.at(1) == ' ');
    REQUIRE(str.at(3) == ' ');

    REQUIRE(str.size() == 5);
    REQUIRE(std::strcmp(str.c_str(), "A B C") == 0);
  }

  SUBCASE("unicode_content") {
    FixedString<16> str("Привет");

    // Access individual bytes (not characters)
    REQUIRE(str.at(0) == static_cast<char>(0xD0)); // First byte of 'П'
    REQUIRE(str.at(1) == static_cast<char>(0x9F)); // Second byte of 'П'

    // Modify bytes
    str.at(0) = 'A';
    str.at(1) = 'B';

    REQUIRE(str.at(0) == 'A');
    REQUIRE(str.at(1) == 'B');

    REQUIRE(str.size() == 12);
    REQUIRE(std::strcmp(str.c_str(), "ABривет") == 0);
  }

  SUBCASE("maximum_length_string") {
    FixedString<8> str("1234567"); // 7 characters, capacity 7

    REQUIRE(str.at(0) == '1');
    REQUIRE(str.at(6) == '7');

    str.at(0) = 'A';
    str.at(6) = 'Z';

    REQUIRE(str.at(0) == 'A');
    REQUIRE(str.at(6) == 'Z');

    REQUIRE(str.size() == 7);
    REQUIRE(std::strcmp(str.c_str(), "A23456Z") == 0);
  }
}

// operator[] access.
TEST_CASE("core/fixed_string/operator_bracket") {
  SUBCASE("non_const_operator_bracket_basic_access") {
    FixedString<16> str("Hello");
    constexpr FixedString<16> constStr("World");

    // Basic access
    REQUIRE(str[0] == 'H');
    REQUIRE(str[1] == 'e');
    REQUIRE(str[2] == 'l');
    REQUIRE(str[3] == 'l');
    REQUIRE(str[4] == 'o');

    static_assert(constStr[0] == 'W');
    static_assert(constStr[1] == 'o');
    static_assert(constStr[2] == 'r');
    static_assert(constStr[3] == 'l');
    static_assert(constStr[4] == 'd');
  }

  SUBCASE("non_const_operator_bracket_modification") {
    FixedString<16> str("Hello");

    // Direct modification
    str[0] = 'h';
    str[1] = 'E';
    str[2] = 'L';
    str[3] = 'L';
    str[4] = 'O';

    REQUIRE(std::strcmp(str.c_str(), "hELLO") == 0);

    REQUIRE(str[0] == 'h');
    REQUIRE(str[1] == 'E');
    REQUIRE(str[2] == 'L');
    REQUIRE(str[3] == 'L');
    REQUIRE(str[4] == 'O');
  }

  SUBCASE("non_const_operator_bracket_reference_modification") {
    FixedString<16> str("Hello");

    // Modification through reference
    auto & ref1 = str[0];
    auto & ref2 = str[1];
    auto & ref3 = str[2];

    ref1 = 'X';
    ref2 = 'Y';
    ref3 = 'Z';

    REQUIRE(std::strcmp(str.c_str(), "XYZlo") == 0);

    REQUIRE(str[0] == 'X');
    REQUIRE(str[1] == 'Y');
    REQUIRE(str[2] == 'Z');
    REQUIRE(str[3] == 'l');
    REQUIRE(str[4] == 'o');
  }

  SUBCASE("const_operator_bracket_read_only_access") {
    constexpr FixedString<16> str("Hello");
    constexpr FixedString<32> longStr("VeryLongString");

    // Read-only access
    REQUIRE(str[0] == 'H');
    REQUIRE(str[1] == 'e');
    REQUIRE(str[2] == 'l');
    REQUIRE(str[3] == 'l');
    REQUIRE(str[4] == 'o');

    REQUIRE(longStr[0] == 'V');
    REQUIRE(longStr[1] == 'e');
    REQUIRE(longStr[2] == 'r');
    REQUIRE(longStr[3] == 'y');
    REQUIRE(longStr[4] == 'L');

    static_assert(str[0] == 'H');
    static_assert(str[1] == 'e');
    static_assert(str[2] == 'l');
    static_assert(str[3] == 'l');
    static_assert(str[4] == 'o');

    static_assert(longStr[0] == 'V');
    static_assert(longStr[1] == 'e');
    static_assert(longStr[2] == 'r');
    static_assert(longStr[3] == 'y');
    static_assert(longStr[4] == 'L');
  }

  SUBCASE("single_character_strings") {
    FixedString<8> str("A");
    constexpr FixedString<8> constStr("B");

    // Non-const access
    REQUIRE(str[0] == 'A');
    str[0] = 'X';
    REQUIRE(str[0] == 'X');
    REQUIRE(std::strcmp(str.c_str(), "X") == 0);

    // Const access
    REQUIRE(constStr[0] == 'B');

    static_assert(constStr[0] == 'B');
  }

  SUBCASE("special_characters") {
    FixedString<32> str("Hello\n\tWorld");
    constexpr FixedString<32> constStr("Test!@#$%");

    // Special characters
    REQUIRE(str[5] == '\n');
    REQUIRE(str[6] == '\t');
    REQUIRE(str[7] == 'W');

    REQUIRE(constStr[4] == '!');
    REQUIRE(constStr[5] == '@');
    REQUIRE(constStr[6] == '#');
    REQUIRE(constStr[7] == '$');
    REQUIRE(constStr[8] == '%');

    static_assert(constStr[4] == '!');
    static_assert(constStr[5] == '@');
    static_assert(constStr[6] == '#');
    static_assert(constStr[7] == '$');
    static_assert(constStr[8] == '%');
  }

  SUBCASE("unicode_content") {
    FixedString<32> str("Hello 世界");
    constexpr FixedString<32> constStr("Test 🌍");

    // Unicode characters (byte-level access)
    REQUIRE(str[5] == ' '); // Space
    REQUIRE(str[6] == '\xE4'); // First byte of 世
    REQUIRE(str[7] == '\xB8'); // Second byte of 世
    REQUIRE(str[8] == '\x96'); // Third byte of 世

    REQUIRE(constStr[4] == ' '); // Space
    REQUIRE(constStr[5] == '\xF0'); // First byte of 🌍
    REQUIRE(constStr[6] == '\x9F'); // Second byte of 🌍
    REQUIRE(constStr[7] == '\x8C'); // Third byte of 🌍

    static_assert(constStr[4] == ' ');
    static_assert(constStr[5] == '\xF0');
    static_assert(constStr[6] == '\x9F');
    static_assert(constStr[7] == '\x8C');
  }

  SUBCASE("edge_cases") {
    FixedString<4> str("ABC"); // Maximum length for capacity 4
    constexpr FixedString<4> constStr("XYZ");

    // Access to last character
    REQUIRE(str[2] == 'C');
    REQUIRE(constStr[2] == 'Z');

    // Modification of last character
    str[2] = 'D';
    REQUIRE(str[2] == 'D');
    REQUIRE(std::strcmp(str.c_str(), "ABD") == 0);

    static_assert(constStr[0] == 'X');
    static_assert(constStr[1] == 'Y');
    static_assert(constStr[2] == 'Z');
  }

  SUBCASE("reference_semantics") {
    FixedString<16> str("Hello");

    // Get references to multiple elements
    auto & ref0 = str[0];
    auto & ref1 = str[1];
    auto & ref2 = str[2];

    // Modify through references
    ref0 = 'A';
    ref1 = 'B';
    ref2 = 'C';

    // Verify changes
    REQUIRE(str[0] == 'A');
    REQUIRE(str[1] == 'B');
    REQUIRE(str[2] == 'C');

    REQUIRE(std::strcmp(str.c_str(), "ABClo") == 0);

    // References should still be valid
    REQUIRE(ref0 == 'A');
    REQUIRE(ref1 == 'B');
    REQUIRE(ref2 == 'C');
  }

  SUBCASE("empty_string") {
    constexpr FixedString<16> str;

    REQUIRE(str[0] == '\0');

    static_assert(str[0] == '\0');
  }
}

// front() and back() access.
TEST_CASE("core/fixed_string/front_and_back") {
  SUBCASE("front_method") {
    FixedString<16> testString("Hello World");
    constexpr FixedString<16> constString("Hello World");

    REQUIRE(testString.front() == 'H');
    REQUIRE(constString.front() == 'H');
    REQUIRE(testString[0] == 'H');
    REQUIRE(constString[0] == 'H');

    // Test modification
    testString.front() = 'h';
    REQUIRE(testString.front() == 'h');
    REQUIRE(testString[0] == 'h');

    static_assert(constString.front() == 'H');
    static_assert(constString[0] == 'H');
  }

  SUBCASE("back_method") {
    FixedString<16> testString("Hello World");
    constexpr FixedString<16> constString("Hello World");

    REQUIRE(testString.back() == 'd');
    REQUIRE(constString.back() == 'd');
    REQUIRE(testString[testString.size() - 1] == 'd');
    REQUIRE(constString[constString.size() - 1] == 'd');

    // Test modification
    testString.back() = 'D';
    REQUIRE(testString.back() == 'D');
    REQUIRE(testString[testString.size() - 1] == 'D');

    static_assert(constString.back() == 'd');
    static_assert(constString[constString.size() - 1] == 'd');
  }

  SUBCASE("single_character_string") {
    FixedString<2> testString("A");

    REQUIRE(testString.front() == 'A');
    REQUIRE(testString.back() == 'A');
    REQUIRE(testString.front() == testString.back());

    // Test modification
    testString.front() = 'B';
    REQUIRE(testString.front() == 'B');
    REQUIRE(testString.back() == 'B');

    testString.back() = 'C';
    REQUIRE(testString.front() == 'C');
    REQUIRE(testString.back() == 'C');

    constexpr FixedString<2> constTestString("A");
    static_assert(constTestString.front() == 'A');
    static_assert(constTestString.back() == 'A');
    static_assert(constTestString.front() == constTestString.back());
  }

  SUBCASE("empty_string") {
    FixedString<16> testString("");

    REQUIRE(testString.front() == '\0');

    constexpr FixedString<16> constTestString("");
    static_assert(constTestString.front() == '\0');
  }

  SUBCASE("two_character_string") {
    FixedString<8> testString("AB");

    REQUIRE(testString.front() == 'A');
    REQUIRE(testString.back() == 'B');
    REQUIRE(testString.front() != testString.back());

    // Test modification
    testString.front() = 'X';
    REQUIRE(testString.front() == 'X');
    REQUIRE(testString.back() == 'B');

    testString.back() = 'Y';
    REQUIRE(testString.front() == 'X');
    REQUIRE(testString.back() == 'Y');

    constexpr FixedString<8> constTestString("AB");
    static_assert(constTestString.front() == 'A');
    static_assert(constTestString.back() == 'B');
    static_assert(constTestString.front() != constTestString.back());
  }

  SUBCASE("modification_through_references") {
    FixedString<32> testString("Hello World");

    // Modify through front reference
    char & frontRef = testString.front();
    frontRef = 'X';
    REQUIRE(testString.front() == 'X');
    REQUIRE(testString[0] == 'X');

    // Modify through back reference
    char & backRef = testString.back();
    backRef = 'Y';
    REQUIRE(testString.back() == 'Y');
    REQUIRE(testString[testString.size() - 1] == 'Y');
  }

  SUBCASE("const_references") {
    constexpr FixedString<32> testString("Hello World");

    static_assert(testString.front() == 'H');
    static_assert(testString.back() == 'd');
  }

  SUBCASE("special_characters") {
    FixedString<16> testString("Hello\n\tWorld!");

    REQUIRE(testString.front() == 'H');
    REQUIRE(testString.back() == '!');

    // Test modification with special characters
    testString.front() = '\n';
    REQUIRE(testString.front() == '\n');

    testString.back() = '\t';
    REQUIRE(testString.back() == '\t');

    constexpr FixedString<16> constTestString("Hello\n\tWorld!");
    static_assert(constTestString.front() == 'H');
    static_assert(constTestString.back() == '!');
  }

  SUBCASE("numeric_content") {
    FixedString<16> testString("12345");

    REQUIRE(testString.front() == '1');
    REQUIRE(testString.back() == '5');

    // Test modification
    testString.front() = '9';
    REQUIRE(testString.front() == '9');

    testString.back() = '0';
    REQUIRE(testString.back() == '0');

    constexpr FixedString<16> constTestString("12345");
    static_assert(constTestString.front() == '1');
    static_assert(constTestString.back() == '5');
  }

  SUBCASE("mixed_content") {
    FixedString<32> testString("123Hello456");

    REQUIRE(testString.front() == '1');
    REQUIRE(testString.back() == '6');

    // Test modification
    testString.front() = '9';
    REQUIRE(testString.front() == '9');

    testString.back() = '0';
    REQUIRE(testString.back() == '0');

    constexpr FixedString<32> constTestString("123Hello456");
    static_assert(constTestString.front() == '1');
    static_assert(constTestString.back() == '6');
  }

  SUBCASE("long_strings") {
    FixedString<64> testString("This is a very long string for performance testing");

    REQUIRE(testString.front() == 'T');
    REQUIRE(testString.back() == 'g');

    // Test modification
    testString.front() = 'X';
    REQUIRE(testString.front() == 'X');

    testString.back() = 'Y';
    REQUIRE(testString.back() == 'Y');

    constexpr FixedString<64> constTestString("This is a very long string for performance testing");
    static_assert(constTestString.front() == 'T');
    static_assert(constTestString.back() == 'g');
  }

  SUBCASE("case_sensitivity") {
    FixedString<16> testString("Hello World");

    REQUIRE(testString.front() == 'H'); // Uppercase
    REQUIRE(testString.back() == 'd'); // Lowercase

    // Test case modification
    testString.front() = 'h'; // Change to lowercase
    REQUIRE(testString.front() == 'h');

    testString.back() = 'D'; // Change to uppercase
    REQUIRE(testString.back() == 'D');

    constexpr FixedString<16> constTestString("Hello World");
    static_assert(constTestString.front() == 'H');
    static_assert(constTestString.back() == 'd');
  }

  SUBCASE("whitespace_handling") {
    FixedString<16> testString(" Hello ");

    REQUIRE(testString.front() == ' ');
    REQUIRE(testString.back() == ' ');

    // Test modification
    testString.front() = 'X';
    REQUIRE(testString.front() == 'X');

    testString.back() = 'Y';
    REQUIRE(testString.back() == 'Y');

    constexpr FixedString<16> constTestString(" Hello ");
    static_assert(constTestString.front() == ' ');
    static_assert(constTestString.back() == ' ');
  }

  SUBCASE("reference_stability") {
    FixedString<32> testString("Hello World");

    // Get references
    char & frontRef = testString.front();
    char & backRef = testString.back();

    // Modify through references
    frontRef = 'X';
    backRef = 'Y';

    // Verify references still work
    REQUIRE(frontRef == 'X');
    REQUIRE(backRef == 'Y');
    REQUIRE(testString.front() == 'X');
    REQUIRE(testString.back() == 'Y');
  }

  SUBCASE("constexpr_operations") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<16> str2("World");
    constexpr FixedString<16> str3("Test");

    // Compile-time front operations
    static_assert(str1.front() == 'H');
    static_assert(str2.front() == 'W');
    static_assert(str3.front() == 'T');

    // Compile-time back operations
    static_assert(str1.back() == 'o');
    static_assert(str2.back() == 'd');
    static_assert(str3.back() == 't');
  }
}

// data() pointer access.
TEST_CASE("core/fixed_string/data") {
  SUBCASE("basic_data_access") {
    constexpr FixedString<16> testString("Hello World");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<4> singleChar("A");
    FixedString<16> string("Hello World");

    // Test that data() points to null-terminated string
    REQUIRE(std::strcmp(testString.data(), "Hello World") == 0);
    REQUIRE(std::strcmp(emptyString.data(), "") == 0);
    REQUIRE(std::strcmp(singleChar.data(), "A") == 0);

    // Modifications
    REQUIRE(std::strcmp(string.data(), "Hello World") == 0);
    string.data()[0] = 'h';
    REQUIRE(std::strcmp(string.data(), "hello World") == 0);

    static_assert(cstrcmp(testString.data(), "Hello World") == 0);
    static_assert(cstrcmp(emptyString.data(), "") == 0);
    static_assert(cstrcmp(singleChar.data(), "A") == 0);
  }

  SUBCASE("data_pointer_stability") {
    constexpr FixedString<32> testString("Stability Test");
    constexpr FixedString<32> copy1(testString);
    constexpr FixedString<32> copy2(testString);

    // Test that data() returns consistent pointers
    REQUIRE(testString.data() == testString.data());
    REQUIRE(copy1.data() == copy1.data());
    REQUIRE(copy2.data() == copy2.data());

    // Test that data() points to the same content
    REQUIRE(std::strcmp(testString.data(), "Stability Test") == 0);
    REQUIRE(std::strcmp(copy1.data(), "Stability Test") == 0);
    REQUIRE(std::strcmp(copy2.data(), "Stability Test") == 0);

    static_assert(testString.data() == testString.data());
    static_assert(copy1.data() == copy1.data());
    static_assert(copy2.data() == copy2.data());

    static_assert(cstrcmp(testString.data(), "Stability Test") == 0);
    static_assert(cstrcmp(copy1.data(), "Stability Test") == 0);
    static_assert(cstrcmp(copy2.data(), "Stability Test") == 0);
  }

  SUBCASE("empty_string") {
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<16> defaultString;

    // Test data() with empty strings
    REQUIRE(std::strcmp(emptyString.data(), "") == 0);
    REQUIRE(std::strcmp(defaultString.data(), "") == 0);

    static_assert(cstrcmp(emptyString.data(), "") == 0);
    static_assert(cstrcmp(defaultString.data(), "") == 0);
  }
}

// c_str() null-terminated pointer.
TEST_CASE("core/fixed_string/c_str") {
  SUBCASE("basic_c_str_access") {
    constexpr FixedString<16> testString("Hello World");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<4> singleChar("A");

    // Test that c_str() returns the same as data()
    REQUIRE(testString.c_str() == testString.data());
    REQUIRE(emptyString.c_str() == emptyString.data());
    REQUIRE(singleChar.c_str() == singleChar.data());

    // Test that c_str() points to null-terminated string
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
    REQUIRE(std::strcmp(singleChar.c_str(), "A") == 0);

    static_assert(testString.c_str() == testString.data());
    static_assert(emptyString.c_str() == emptyString.data());
    static_assert(singleChar.c_str() == singleChar.data());

    static_assert(cstrcmp(testString.c_str(), "Hello World") == 0);
    static_assert(cstrcmp(emptyString.c_str(), "") == 0);
    static_assert(cstrcmp(singleChar.c_str(), "A") == 0);
  }

  SUBCASE("c_string_stability") {
    constexpr FixedString<32> testString("Stability Test");
    constexpr FixedString<32> copy1(testString);
    constexpr FixedString<32> copy2(testString);

    // Test that c_str() returns consistent pointers
    REQUIRE(testString.c_str() == testString.c_str());
    REQUIRE(copy1.c_str() == copy1.c_str());
    REQUIRE(copy2.c_str() == copy2.c_str());

    // Test that c_str() points to the same content
    REQUIRE(std::strcmp(testString.c_str(), "Stability Test") == 0);
    REQUIRE(std::strcmp(copy1.c_str(), "Stability Test") == 0);
    REQUIRE(std::strcmp(copy2.c_str(), "Stability Test") == 0);

    static_assert(testString.c_str() == testString.c_str());
    static_assert(copy1.c_str() == copy1.c_str());
    static_assert(copy2.c_str() == copy2.c_str());

    static_assert(cstrcmp(testString.c_str(), "Stability Test") == 0);
    static_assert(cstrcmp(copy1.c_str(), "Stability Test") == 0);
    static_assert(cstrcmp(copy2.c_str(), "Stability Test") == 0);
  }

  SUBCASE("empty_string") {
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<16> defaultString;

    // Test c_str() with empty strings
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);

    static_assert(emptyString.c_str() == emptyString.data());
    static_assert(defaultString.c_str() == defaultString.data());
  }
}

// empty() query.
TEST_CASE("core/fixed_string/empty") {
  SUBCASE("basic_empty_check") {
    constexpr FixedString<16> nonEmptyString("Hello World");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<4> defaultString;

    static_assert(!nonEmptyString.empty());
    static_assert(emptyString.empty());
    static_assert(defaultString.empty());
  }

  SUBCASE("single_character_strings") {
    constexpr FixedString<8> singleChar("A");
    constexpr FixedString<4> emptyString("");

    static_assert(!singleChar.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("different_capacities") {
    constexpr FixedString<8> smallString("Hi");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<8> emptySmall("");
    constexpr FixedString<16> emptyMedium("");
    constexpr FixedString<32> emptyLarge("");

    static_assert(!smallString.empty());
    static_assert(!mediumString.empty());
    static_assert(!largeString.empty());
    static_assert(emptySmall.empty());
    static_assert(emptyMedium.empty());
    static_assert(emptyLarge.empty());
  }

  SUBCASE("special_characters") {
    constexpr FixedString<32> newlineString("Hello\nWorld");
    constexpr FixedString<32> tabString("Hello\tWorld");
    constexpr FixedString<32> specialString("!@#$%^&*()");
    constexpr FixedString<32> emptyString("");

    static_assert(!newlineString.empty());
    static_assert(!tabString.empty());
    static_assert(!specialString.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("unicode_content") {
    constexpr FixedString<64> unicodeString("Привет мир");
    constexpr FixedString<64> emojiString("Hello 🌍 World");
    constexpr FixedString<64> mixedString("Hello 世界");
    constexpr FixedString<64> emptyString("");

    static_assert(!unicodeString.empty());
    static_assert(!emojiString.empty());
    static_assert(!mixedString.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("numeric_content") {
    constexpr FixedString<16> numericString("12345");
    constexpr FixedString<16> floatString("3.14159");
    constexpr FixedString<16> hexString("0xABCD");
    constexpr FixedString<16> emptyString("");

    static_assert(!numericString.empty());
    static_assert(!floatString.empty());
    static_assert(!hexString.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("mixed_content") {
    constexpr FixedString<32> mixedString("Hello123World!@#");
    constexpr FixedString<32> complexString("Test\n123\t!@#");
    constexpr FixedString<64> longString("This is a very long string with mixed content 123!@#");
    constexpr FixedString<32> emptyString("");

    static_assert(!mixedString.empty());
    static_assert(!complexString.empty());
    static_assert(!longString.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("maximum_length_strings") {
    constexpr FixedString<16> maxString("123456789012345"); // 15 characters
    constexpr FixedString<8> maxSmall("1234567"); // 7 characters
    constexpr FixedString<4> maxTiny("123"); // 3 characters
    constexpr FixedString<16> emptyString("");

    static_assert(!maxString.empty());
    static_assert(!maxSmall.empty());
    static_assert(!maxTiny.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("edge_cases") {
    constexpr FixedString<8> singleChar("A");
    constexpr FixedString<8> twoChars("AB");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<8> defaultString;

    REQUIRE_FALSE(singleChar.empty());
    REQUIRE_FALSE(twoChars.empty());
    REQUIRE(emptyString.empty());
    REQUIRE(defaultString.empty());
    REQUIRE(singleChar.size() == 1);
    REQUIRE(twoChars.size() == 2);
    REQUIRE(emptyString.size() == 0);
    REQUIRE(defaultString.size() == 0);

    static_assert(!singleChar.empty());
    static_assert(!twoChars.empty());
    static_assert(emptyString.empty());
    static_assert(defaultString.empty());
    static_assert(singleChar.size() == 1);
    static_assert(twoChars.size() == 2);
    static_assert(emptyString.size() == 0);
    static_assert(defaultString.size() == 0);
  }
}

// size() in bytes.
TEST_CASE("core/fixed_string/size") {
  SUBCASE("basic_size_check") {
    constexpr FixedString<16> testString("Hello World");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<4> defaultString;

    static_assert(testString.size() == 11);
    static_assert(emptyString.size() == 0);
    static_assert(defaultString.size() == 0);
  }

  SUBCASE("single_character_strings") {
    constexpr FixedString<8> singleChar("A");
    constexpr FixedString<4> emptyString("");

    REQUIRE(singleChar.size() == 1);
    REQUIRE(emptyString.size() == 0);

    static_assert(singleChar.size() == 1);
    static_assert(emptyString.size() == 0);
  }

  SUBCASE("different_capacities") {
    constexpr FixedString<8> smallString("Hi");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<8> emptySmall("");
    constexpr FixedString<16> emptyMedium("");
    constexpr FixedString<32> emptyLarge("");

    REQUIRE(smallString.size() == 2);
    REQUIRE(mediumString.size() == 11);
    REQUIRE(largeString.size() == 23);
    REQUIRE(emptySmall.size() == 0);
    REQUIRE(emptyMedium.size() == 0);
    REQUIRE(emptyLarge.size() == 0);

    static_assert(smallString.size() == 2);
    static_assert(mediumString.size() == 11);
    static_assert(largeString.size() == 23);
    static_assert(emptySmall.size() == 0);
    static_assert(emptyMedium.size() == 0);
    static_assert(emptyLarge.size() == 0);
  }

  SUBCASE("special_characters") {
    constexpr FixedString<32> newlineString("Hello\nWorld");
    constexpr FixedString<32> tabString("Hello\tWorld");
    constexpr FixedString<32> specialString("!@#$%^&*()");
    constexpr FixedString<32> emptyString("");

    REQUIRE(newlineString.size() == 11);
    REQUIRE(tabString.size() == 11);
    REQUIRE(specialString.size() == 10);
    REQUIRE(emptyString.size() == 0);

    static_assert(newlineString.size() == 11);
    static_assert(tabString.size() == 11);
    static_assert(specialString.size() == 10);
    static_assert(emptyString.size() == 0);
  }

  SUBCASE("unicode_content") {
    constexpr FixedString<64> unicodeString("Привет мир");
    constexpr FixedString<64> emojiString("Hello 🌍 World");
    constexpr FixedString<64> mixedString("Hello 世界");
    constexpr FixedString<64> emptyString("");

    REQUIRE(unicodeString.size() == std::char_traits<char>::length("Привет мир"));
    REQUIRE(emojiString.size() == std::char_traits<char>::length("Hello 🌍 World"));
    REQUIRE(mixedString.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(emptyString.size() == 0);

    static_assert(unicodeString.size() == std::char_traits<char>::length("Привет мир"));
    static_assert(emojiString.size() == std::char_traits<char>::length("Hello 🌍 World"));
    static_assert(mixedString.size() == std::char_traits<char>::length("Hello 世界"));
    static_assert(emptyString.size() == 0);
  }

  SUBCASE("numeric_content") {
    constexpr FixedString<16> numericString("12345");
    constexpr FixedString<16> floatString("3.14159");
    constexpr FixedString<16> hexString("0xABCD");
    constexpr FixedString<16> emptyString("");

    REQUIRE(numericString.size() == 5);
    REQUIRE(floatString.size() == 7);
    REQUIRE(hexString.size() == 6);
    REQUIRE(emptyString.size() == 0);

    static_assert(numericString.size() == 5);
    static_assert(floatString.size() == 7);
    static_assert(hexString.size() == 6);
    static_assert(emptyString.size() == 0);
  }

  SUBCASE("mixed_content") {
    constexpr FixedString<32> mixedString("Hello123World!@#");
    constexpr FixedString<32> complexString("Test\n123\t!@#");
    constexpr FixedString<64> longString("This is a very long string with mixed content 123!@#");
    constexpr FixedString<32> emptyString("");

    REQUIRE(mixedString.size() == 16);
    REQUIRE(complexString.size() == 12);
    REQUIRE(longString.size() == 52);
    REQUIRE(emptyString.size() == 0);

    static_assert(mixedString.size() == 16);
    static_assert(complexString.size() == 12);
    static_assert(longString.size() == 52);
    static_assert(emptyString.size() == 0);
  }

  SUBCASE("maximum_length_strings") {
    constexpr FixedString<16> maxString("123456789012345"); // 15 characters
    constexpr FixedString<8> maxSmall("1234567"); // 7 characters
    constexpr FixedString<4> maxTiny("123"); // 3 characters
    constexpr FixedString<16> emptyString("");

    REQUIRE(maxString.size() == 15);
    REQUIRE(maxSmall.size() == 7);
    REQUIRE(maxTiny.size() == 3);
    REQUIRE(emptyString.size() == 0);

    static_assert(maxString.size() == 15);
    static_assert(maxSmall.size() == 7);
    static_assert(maxTiny.size() == 3);
    static_assert(emptyString.size() == 0);
  }

  SUBCASE("edge_cases") {
    constexpr FixedString<8> singleChar("A");
    constexpr FixedString<8> twoChars("AB");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<8> defaultString;

    REQUIRE(singleChar.size() == 1);
    REQUIRE(twoChars.size() == 2);
    REQUIRE(emptyString.size() == 0);
    REQUIRE(defaultString.size() == 0);

    static_assert(singleChar.size() == 1);
    static_assert(twoChars.size() == 2);
    static_assert(emptyString.size() == 0);
    static_assert(defaultString.size() == 0);
  }
}

// utf8_size() code point count.
TEST_CASE("core/fixed_string/utf8_size") {
  SUBCASE("ascii_strings") {
    constexpr FixedString<32> asciiString("Hello World");
    constexpr FixedString<16> emptyString("");
    constexpr FixedString<8> singleChar("A");

    REQUIRE(asciiString.utf8_size() == 11);
    REQUIRE(emptyString.utf8_size() == 0);
    REQUIRE(singleChar.utf8_size() == 1);

    // For ASCII strings, utf8_size should equal size
    REQUIRE(asciiString.utf8_size() == asciiString.size());
    REQUIRE(emptyString.utf8_size() == emptyString.size());
    REQUIRE(singleChar.utf8_size() == singleChar.size());
  }

  SUBCASE("utf8_cyrillic_text") {
    // "Привет мир" in UTF-8
    constexpr const char * cyrillicText = "Привет мир";

    constexpr FixedString<32> cyrillicString(cyrillicText);

    REQUIRE(cyrillicString.size() == std::char_traits<char>::length("Привет мир"));
    REQUIRE(cyrillicString.utf8_size() == 10);
  }

  SUBCASE("mixed_ascii_and_utf8") {
    // "Hello 世界" in UTF-8
    constexpr const char * mixedText = "Hello 世界";

    constexpr FixedString<16> mixedString(mixedText);

    REQUIRE(mixedString.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(mixedString.utf8_size() == 8); // 6 ASCII + 2 Chinese characters
  }

  SUBCASE("emoji_characters") {
    // "Hello 🌍" in UTF-8
    constexpr const char * emojiText = "Hello 🌍";

    constexpr FixedString<16> emojiString(emojiText);

    REQUIRE(emojiString.size() == std::char_traits<char>::length("Hello 🌍"));
    REQUIRE(emojiString.utf8_size() == 7); // 6 ASCII + 1 emoji
  }

  SUBCASE("special_characters") {
    constexpr FixedString<32> specialString("!@#$%^&*()");
    constexpr FixedString<16> numericString("1234567890");
    constexpr FixedString<8> punctuationString(".,;:!?");

    REQUIRE(specialString.utf8_size() == 10);
    REQUIRE(numericString.utf8_size() == 10);
    REQUIRE(punctuationString.utf8_size() == 6);

    // Special characters are ASCII, so utf8_size equals size
    REQUIRE(specialString.utf8_size() == specialString.size());
    REQUIRE(numericString.utf8_size() == numericString.size());
    REQUIRE(punctuationString.utf8_size() == punctuationString.size());
  }

  SUBCASE("edge_cases") {
    constexpr FixedString<8> singleByte("A");
    constexpr FixedString<16> twoByte("А"); // Cyrillic A
    constexpr FixedString<16> threeByte("中"); // Chinese character
    constexpr FixedString<16> fourByte("🌍"); // Emoji

    REQUIRE(singleByte.utf8_size() == 1);
    REQUIRE(twoByte.utf8_size() == 1);
    REQUIRE(threeByte.utf8_size() == 1);
    REQUIRE(fourByte.utf8_size() == 1);
  }

  SUBCASE("long_utf8_text") {
    // "ToyGine2 - Бесплатный 2D/3D игровой движок." in UTF-8
    constexpr const char * longUtf8Text = "ToyGine2 - Бесплатный 2D/3D игровой движок.";

    constexpr FixedString<80> longString(longUtf8Text);

    REQUIRE(longString.size() == std::char_traits<char>::length("ToyGine2 - Бесплатный 2D/3D игровой движок."));
    REQUIRE(longString.utf8_size() == 43); // 43 characters
  }
}

// length() alias for size().
TEST_CASE("core/fixed_string/length") {
  SUBCASE("basic_length_check") {
    constexpr FixedString<32> testString("Hello World");
    constexpr FixedString<16> emptyString("");
    constexpr FixedString<8> singleChar("A");

    REQUIRE(testString.length() == 11);
    REQUIRE(emptyString.length() == 0);
    REQUIRE(singleChar.length() == 1);

    // length() should equal size() for all strings
    REQUIRE(testString.length() == testString.size());
    REQUIRE(emptyString.length() == emptyString.size());
    REQUIRE(singleChar.length() == singleChar.size());

    static_assert(testString.length() == 11);
    static_assert(emptyString.length() == 0);
    static_assert(singleChar.length() == 1);
  }

  SUBCASE("different_capacities") {
    constexpr FixedString<8> smallString("Hi");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<8> emptySmall("");
    constexpr FixedString<16> emptyMedium("");
    constexpr FixedString<32> emptyLarge("");

    REQUIRE(smallString.length() == 2);
    REQUIRE(mediumString.length() == 11);
    REQUIRE(largeString.length() == 23);
    REQUIRE(emptySmall.length() == 0);
    REQUIRE(emptyMedium.length() == 0);
    REQUIRE(emptyLarge.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(smallString.length() == smallString.size());
    REQUIRE(mediumString.length() == mediumString.size());
    REQUIRE(largeString.length() == largeString.size());
    REQUIRE(emptySmall.length() == emptySmall.size());
    REQUIRE(emptyMedium.length() == emptyMedium.size());
    REQUIRE(emptyLarge.length() == emptyLarge.size());

    static_assert(smallString.length() == 2);
    static_assert(mediumString.length() == 11);
    static_assert(largeString.length() == 23);
  }

  SUBCASE("special_characters") {
    constexpr FixedString<32> newlineString("Hello\nWorld");
    constexpr FixedString<32> tabString("Hello\tWorld");
    constexpr FixedString<32> specialString("!@#$%^&*()");
    constexpr FixedString<32> emptyString("");

    REQUIRE(newlineString.length() == 11);
    REQUIRE(tabString.length() == 11);
    REQUIRE(specialString.length() == 10);
    REQUIRE(emptyString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(newlineString.length() == newlineString.size());
    REQUIRE(tabString.length() == tabString.size());
    REQUIRE(specialString.length() == specialString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    static_assert(newlineString.length() == 11);
    static_assert(tabString.length() == 11);
    static_assert(specialString.length() == 10);
  }

  SUBCASE("unicode_content") {
    constexpr FixedString<64> unicodeString("Привет мир");
    constexpr FixedString<64> emojiString("Hello 🌍 World");
    constexpr FixedString<64> mixedString("Hello 世界");
    constexpr FixedString<64> emptyString("");

    REQUIRE(unicodeString.length() == std::char_traits<char>::length("Привет мир"));
    REQUIRE(emojiString.length() == std::char_traits<char>::length("Hello 🌍 World"));
    REQUIRE(mixedString.length() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(emptyString.length() == 0);

    REQUIRE(unicodeString.length() == unicodeString.size());
    REQUIRE(emojiString.length() == emojiString.size());
    REQUIRE(mixedString.length() == mixedString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    static_assert(unicodeString.length() == std::char_traits<char>::length("Привет мир"));
    static_assert(emojiString.length() == std::char_traits<char>::length("Hello 🌍 World"));
    static_assert(mixedString.length() == std::char_traits<char>::length("Hello 世界"));
  }

  SUBCASE("numeric_content") {
    constexpr FixedString<32> numericString("12345");
    constexpr FixedString<32> floatString("3.14159");
    constexpr FixedString<32> hexString("0xABCD");
    constexpr FixedString<32> emptyString("");

    REQUIRE(numericString.length() == 5);
    REQUIRE(floatString.length() == 7);
    REQUIRE(hexString.length() == 6);
    REQUIRE(emptyString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(numericString.length() == numericString.size());
    REQUIRE(floatString.length() == floatString.size());
    REQUIRE(hexString.length() == hexString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    static_assert(numericString.length() == 5);
    static_assert(floatString.length() == 7);
    static_assert(hexString.length() == 6);
  }

  SUBCASE("mixed_content") {
    constexpr FixedString<64> mixedString("Hello123World!@#");
    constexpr FixedString<64> complexString("Test\n123\t!@#");
    constexpr FixedString<64> longString("This is a very long string with mixed content 123!@#");
    constexpr FixedString<64> emptyString("");

    REQUIRE(mixedString.length() == 16);
    REQUIRE(complexString.length() == 12);
    REQUIRE(longString.length() == 52);
    REQUIRE(emptyString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(mixedString.length() == mixedString.size());
    REQUIRE(complexString.length() == complexString.size());
    REQUIRE(longString.length() == longString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    static_assert(mixedString.length() == 16);
    static_assert(complexString.length() == 12);
    static_assert(longString.length() == 52);
  }

  SUBCASE("edge_cases") {
    constexpr FixedString<8> singleChar("A");
    constexpr FixedString<8> twoChars("AB");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<8> defaultString;

    REQUIRE(singleChar.length() == 1);
    REQUIRE(twoChars.length() == 2);
    REQUIRE(emptyString.length() == 0);
    REQUIRE(defaultString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(singleChar.length() == singleChar.size());
    REQUIRE(twoChars.length() == twoChars.size());
    REQUIRE(emptyString.length() == emptyString.size());
    REQUIRE(defaultString.length() == defaultString.size());

    static_assert(singleChar.length() == 1);
    static_assert(twoChars.length() == 2);
    static_assert(emptyString.length() == 0);
    static_assert(defaultString.length() == 0);
  }
}

// max_size() maximum capacity.
TEST_CASE("core/fixed_string/max_size") {
  SUBCASE("basic_max_size_check") {
    constexpr FixedString<8> smallString("Hi");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<64> extraLargeString("This is an even longer string for testing");

    REQUIRE(smallString.max_size() == 7);
    REQUIRE(mediumString.max_size() == 15);
    REQUIRE(largeString.max_size() == 31);
    REQUIRE(extraLargeString.max_size() == 63);

    static_assert(smallString.max_size() == 7);
    static_assert(mediumString.max_size() == 15);
    static_assert(largeString.max_size() == 31);
    static_assert(extraLargeString.max_size() == 63);
  }

  SUBCASE("empty_strings") {
    constexpr FixedString<8> emptySmall("");
    constexpr FixedString<16> emptyMedium("");
    constexpr FixedString<32> emptyLarge("");
    constexpr FixedString<64> emptyExtraLarge("");

    REQUIRE(emptySmall.max_size() == 7);
    REQUIRE(emptyMedium.max_size() == 15);
    REQUIRE(emptyLarge.max_size() == 31);
    REQUIRE(emptyExtraLarge.max_size() == 63);

    static_assert(emptySmall.max_size() == 7);
    static_assert(emptyMedium.max_size() == 15);
    static_assert(emptyLarge.max_size() == 31);
    static_assert(emptyExtraLarge.max_size() == 63);
  }

  SUBCASE("default_constructed_strings") {
    constexpr FixedString<8> defaultSmall;
    constexpr FixedString<16> defaultMedium;
    constexpr FixedString<32> defaultLarge;
    constexpr FixedString<64> defaultExtraLarge;

    REQUIRE(defaultSmall.max_size() == 7);
    REQUIRE(defaultMedium.max_size() == 15);
    REQUIRE(defaultLarge.max_size() == 31);
    REQUIRE(defaultExtraLarge.max_size() == 63);

    static_assert(defaultSmall.max_size() == 7);
    static_assert(defaultMedium.max_size() == 15);
    static_assert(defaultLarge.max_size() == 31);
    static_assert(defaultExtraLarge.max_size() == 63);
  }

  SUBCASE("single_character_strings") {
    constexpr FixedString<8> singleSmall("A");
    constexpr FixedString<16> singleMedium("B");
    constexpr FixedString<32> singleLarge("C");
    constexpr FixedString<64> singleExtraLarge("D");

    REQUIRE(singleSmall.max_size() == 7);
    REQUIRE(singleMedium.max_size() == 15);
    REQUIRE(singleLarge.max_size() == 31);
    REQUIRE(singleExtraLarge.max_size() == 63);

    static_assert(singleSmall.max_size() == 7);
    static_assert(singleMedium.max_size() == 15);
    static_assert(singleLarge.max_size() == 31);
    static_assert(singleExtraLarge.max_size() == 63);
  }

  SUBCASE("maximum_length_strings") {
    constexpr FixedString<8> maxSmall("1234567"); // 7 characters (max for capacity 8)
    constexpr FixedString<16> maxMedium("123456789012345"); // 15 characters (max for capacity 16)
    constexpr FixedString<32> maxLarge("1234567890123456789012345678901"); // 31 characters (max for capacity 32)
    constexpr FixedString<64> maxExtraLarge(
      "123456789012345678901234567890123456789012345678901234567890123"); // 63 characters (max for capacity 64)

    REQUIRE(maxSmall.max_size() == 7);
    REQUIRE(maxMedium.max_size() == 15);
    REQUIRE(maxLarge.max_size() == 31);
    REQUIRE(maxExtraLarge.max_size() == 63);

    static_assert(maxSmall.max_size() == 7);
    static_assert(maxMedium.max_size() == 15);
    static_assert(maxLarge.max_size() == 31);
    static_assert(maxExtraLarge.max_size() == 63);
  }

  SUBCASE("different_template_parameters") {
    constexpr FixedString<4> tinyString("Hi");
    constexpr FixedString<8> smallString("Hello");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<64> extraLargeString("This is an even longer string for testing");
    constexpr FixedString<128> hugeString(
      "This is a very long string that tests the maximum capacity of a large FixedString buffer");

    REQUIRE(tinyString.max_size() == 3);
    REQUIRE(smallString.max_size() == 7);
    REQUIRE(mediumString.max_size() == 15);
    REQUIRE(largeString.max_size() == 31);
    REQUIRE(extraLargeString.max_size() == 63);
    REQUIRE(hugeString.max_size() == 127);

    static_assert(tinyString.max_size() == 3);
    static_assert(smallString.max_size() == 7);
    static_assert(mediumString.max_size() == 15);
    static_assert(largeString.max_size() == 31);
    static_assert(extraLargeString.max_size() == 63);
    static_assert(hugeString.max_size() == 127);
  }

  SUBCASE("edge_cases") {
    constexpr FixedString<1> minimalString; // Should have max_size() == 0
    constexpr FixedString<2> twoCharString("A");
    constexpr FixedString<3> threeCharString("AB");

    REQUIRE(minimalString.max_size() == 0);
    REQUIRE(twoCharString.max_size() == 1);
    REQUIRE(threeCharString.max_size() == 2);

    static_assert(minimalString.max_size() == 0);
    static_assert(twoCharString.max_size() == 1);
    static_assert(threeCharString.max_size() == 2);
  }

  SUBCASE("consistency_with_capacity") {
    constexpr FixedString<8> testString1("Hello");
    constexpr FixedString<16> testString2("World");
    constexpr FixedString<32> testString3("Test");

    // max_size() should equal capacity() for all FixedString instances
    REQUIRE(testString1.max_size() == testString1.capacity());
    REQUIRE(testString2.max_size() == testString2.capacity());
    REQUIRE(testString3.max_size() == testString3.capacity());

    static_assert(testString1.max_size() == testString1.capacity());
    static_assert(testString2.max_size() == testString2.capacity());
    static_assert(testString3.max_size() == testString3.capacity());
  }
}

// capacity() storage limit.
TEST_CASE("core/fixed_string/capacity") {
  SUBCASE("basic_capacity_check") {
    constexpr FixedString<8> smallString("Hi");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<64> extraLargeString("This is an even longer string for testing");

    REQUIRE(smallString.capacity() == 7);
    REQUIRE(mediumString.capacity() == 15);
    REQUIRE(largeString.capacity() == 31);
    REQUIRE(extraLargeString.capacity() == 63);

    static_assert(smallString.capacity() == 7);
    static_assert(mediumString.capacity() == 15);
    static_assert(largeString.capacity() == 31);
    static_assert(extraLargeString.capacity() == 63);
  }

  SUBCASE("empty_strings") {
    constexpr FixedString<8> emptySmall("");
    constexpr FixedString<16> emptyMedium("");
    constexpr FixedString<32> emptyLarge("");
    constexpr FixedString<64> emptyExtraLarge("");

    REQUIRE(emptySmall.capacity() == 7);
    REQUIRE(emptyMedium.capacity() == 15);
    REQUIRE(emptyLarge.capacity() == 31);
    REQUIRE(emptyExtraLarge.capacity() == 63);

    static_assert(emptySmall.capacity() == 7);
    static_assert(emptyMedium.capacity() == 15);
    static_assert(emptyLarge.capacity() == 31);
    static_assert(emptyExtraLarge.capacity() == 63);
  }

  SUBCASE("default_constructed_strings") {
    constexpr FixedString<8> defaultSmall;
    constexpr FixedString<16> defaultMedium;
    constexpr FixedString<32> defaultLarge;
    constexpr FixedString<64> defaultExtraLarge;

    REQUIRE(defaultSmall.capacity() == 7);
    REQUIRE(defaultMedium.capacity() == 15);
    REQUIRE(defaultLarge.capacity() == 31);
    REQUIRE(defaultExtraLarge.capacity() == 63);

    static_assert(defaultSmall.capacity() == 7);
    static_assert(defaultMedium.capacity() == 15);
    static_assert(defaultLarge.capacity() == 31);
    static_assert(defaultExtraLarge.capacity() == 63);
  }

  SUBCASE("single_character_strings") {
    constexpr FixedString<8> singleSmall("A");
    constexpr FixedString<16> singleMedium("B");
    constexpr FixedString<32> singleLarge("C");
    constexpr FixedString<64> singleExtraLarge("D");

    REQUIRE(singleSmall.capacity() == 7);
    REQUIRE(singleMedium.capacity() == 15);
    REQUIRE(singleLarge.capacity() == 31);
    REQUIRE(singleExtraLarge.capacity() == 63);

    static_assert(singleSmall.capacity() == 7);
    static_assert(singleMedium.capacity() == 15);
    static_assert(singleLarge.capacity() == 31);
    static_assert(singleExtraLarge.capacity() == 63);
  }

  SUBCASE("maximum_length_strings") {
    constexpr FixedString<8> maxSmall("1234567"); // 7 characters (max for capacity 8)
    constexpr FixedString<16> maxMedium("123456789012345"); // 15 characters (max for capacity 16)
    constexpr FixedString<32> maxLarge("1234567890123456789012345678901"); // 31 characters (max for capacity 32)
    constexpr FixedString<64> maxExtraLarge(
      "123456789012345678901234567890123456789012345678901234567890123"); // 63 characters (max for capacity 64)

    REQUIRE(maxSmall.capacity() == 7);
    REQUIRE(maxMedium.capacity() == 15);
    REQUIRE(maxLarge.capacity() == 31);
    REQUIRE(maxExtraLarge.capacity() == 63);

    static_assert(maxSmall.capacity() == 7);
    static_assert(maxMedium.capacity() == 15);
    static_assert(maxLarge.capacity() == 31);
    static_assert(maxExtraLarge.capacity() == 63);
  }

  SUBCASE("different_template_parameters") {
    constexpr FixedString<4> tinyString("Hi");
    constexpr FixedString<8> smallString("Hello");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<64> extraLargeString("This is an even longer string for testing");
    constexpr FixedString<128> hugeString(
      "This is a very long string that tests the maximum capacity of a large FixedString buffer");

    REQUIRE(tinyString.capacity() == 3);
    REQUIRE(smallString.capacity() == 7);
    REQUIRE(mediumString.capacity() == 15);
    REQUIRE(largeString.capacity() == 31);
    REQUIRE(extraLargeString.capacity() == 63);
    REQUIRE(hugeString.capacity() == 127);

    static_assert(tinyString.capacity() == 3);
    static_assert(smallString.capacity() == 7);
    static_assert(mediumString.capacity() == 15);
    static_assert(largeString.capacity() == 31);
    static_assert(extraLargeString.capacity() == 63);
    static_assert(hugeString.capacity() == 127);
  }

  SUBCASE("edge_cases") {
    constexpr FixedString<1> minimalString; // Should have capacity() == 0
    constexpr FixedString<2> twoCharString("A");
    constexpr FixedString<3> threeCharString("AB");

    REQUIRE(minimalString.capacity() == 0);
    REQUIRE(twoCharString.capacity() == 1);
    REQUIRE(threeCharString.capacity() == 2);

    static_assert(minimalString.capacity() == 0);
    static_assert(twoCharString.capacity() == 1);
    static_assert(threeCharString.capacity() == 2);
  }

  SUBCASE("consistency_with_max_size") {
    constexpr FixedString<8> testString1("Hello");
    constexpr FixedString<16> testString2("World");
    constexpr FixedString<32> testString3("Test");

    // capacity() should equal max_size() for all FixedString instances
    REQUIRE(testString1.capacity() == testString1.max_size());
    REQUIRE(testString2.capacity() == testString2.max_size());
    REQUIRE(testString3.capacity() == testString3.max_size());

    static_assert(testString1.capacity() == testString1.max_size());
    static_assert(testString2.capacity() == testString2.max_size());
    static_assert(testString3.capacity() == testString3.max_size());
  }

  SUBCASE("capacity_formula_validation") {
    // Test the formula: capacity = N - 1 (where N is the template parameter)
    constexpr FixedString<4> test4("Hi");
    constexpr FixedString<8> test8("Hello");
    constexpr FixedString<16> test16("Hello World");
    constexpr FixedString<32> test32("This is a longer string");
    constexpr FixedString<64> test64("This is an even longer string for testing");

    REQUIRE(test4.capacity() == 3); // 4 - 1 = 3
    REQUIRE(test8.capacity() == 7); // 8 - 1 = 7
    REQUIRE(test16.capacity() == 15); // 16 - 1 = 15
    REQUIRE(test32.capacity() == 31); // 32 - 1 = 31
    REQUIRE(test64.capacity() == 63); // 64 - 1 = 63

    static_assert(test4.capacity() == 3);
    static_assert(test8.capacity() == 7);
    static_assert(test16.capacity() == 15);
    static_assert(test32.capacity() == 31);
    static_assert(test64.capacity() == 63);
  }
}

// clear(); size becomes 0.
TEST_CASE("core/fixed_string/clear") {
  SUBCASE("basic_clear_functionality") {
    FixedString<32> testString("Hello World");

    REQUIRE_FALSE(testString.empty());
    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.clear();

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("clear_empty_string") {
    FixedString<16> emptyString("");

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString.clear();

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
  }

  SUBCASE("clear_default_constructed_string") {
    FixedString<8> defaultString;

    REQUIRE(defaultString.empty());
    REQUIRE(defaultString.size() == 0);

    defaultString.clear();

    REQUIRE(defaultString.empty());
    REQUIRE(defaultString.size() == 0);
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);
  }

  SUBCASE("clear_single_character_string") {
    FixedString<16> singleChar("A");

    REQUIRE_FALSE(singleChar.empty());
    REQUIRE(singleChar.size() == 1);
    REQUIRE(std::strcmp(singleChar.c_str(), "A") == 0);

    singleChar.clear();

    REQUIRE(singleChar.empty());
    REQUIRE(singleChar.size() == 0);
    REQUIRE(std::strcmp(singleChar.c_str(), "") == 0);
  }

  SUBCASE("clear_maximum_length_string") {
    FixedString<8> maxString("1234567"); // 7 characters (max for capacity 8)

    REQUIRE_FALSE(maxString.empty());
    REQUIRE(maxString.size() == 7);
    REQUIRE(std::strcmp(maxString.c_str(), "1234567") == 0);

    maxString.clear();

    REQUIRE(maxString.empty());
    REQUIRE(maxString.size() == 0);
    REQUIRE(std::strcmp(maxString.c_str(), "") == 0);
  }

  SUBCASE("clear_different_capacities") {
    FixedString<8> smallString("Hi");
    FixedString<16> mediumString("Hello World");
    FixedString<32> largeString("This is a longer string");
    FixedString<64> extraLargeString("This is an even longer string for testing");

    // Before clear
    REQUIRE_FALSE(smallString.empty());
    REQUIRE_FALSE(mediumString.empty());
    REQUIRE_FALSE(largeString.empty());
    REQUIRE_FALSE(extraLargeString.empty());

    // Clear all
    smallString.clear();
    mediumString.clear();
    largeString.clear();
    extraLargeString.clear();

    // After clear
    REQUIRE(smallString.empty());
    REQUIRE(mediumString.empty());
    REQUIRE(largeString.empty());
    REQUIRE(extraLargeString.empty());

    REQUIRE(smallString.size() == 0);
    REQUIRE(mediumString.size() == 0);
    REQUIRE(largeString.size() == 0);
    REQUIRE(extraLargeString.size() == 0);
  }

  SUBCASE("clear_special_characters") {
    FixedString<32> newlineString("Hello\nWorld");
    FixedString<32> tabString("Hello\tWorld");
    FixedString<32> specialString("!@#$%^&*()");

    REQUIRE_FALSE(newlineString.empty());
    REQUIRE_FALSE(tabString.empty());
    REQUIRE_FALSE(specialString.empty());

    newlineString.clear();
    tabString.clear();
    specialString.clear();

    REQUIRE(newlineString.empty());
    REQUIRE(tabString.empty());
    REQUIRE(specialString.empty());

    REQUIRE(std::strcmp(newlineString.c_str(), "") == 0);
    REQUIRE(std::strcmp(tabString.c_str(), "") == 0);
    REQUIRE(std::strcmp(specialString.c_str(), "") == 0);
  }

  SUBCASE("clear_unicode_content") {
    FixedString<64> unicodeString("Привет мир");
    FixedString<64> emojiString("Hello 🌍 World");
    FixedString<64> mixedString("Hello 世界");

    REQUIRE_FALSE(unicodeString.empty());
    REQUIRE_FALSE(emojiString.empty());
    REQUIRE_FALSE(mixedString.empty());

    unicodeString.clear();
    emojiString.clear();
    mixedString.clear();

    REQUIRE(unicodeString.empty());
    REQUIRE(emojiString.empty());
    REQUIRE(mixedString.empty());

    REQUIRE(std::strcmp(unicodeString.c_str(), "") == 0);
    REQUIRE(std::strcmp(emojiString.c_str(), "") == 0);
    REQUIRE(std::strcmp(mixedString.c_str(), "") == 0);
  }

  SUBCASE("clear_and_capacity_preservation") {
    FixedString<16> testString("Hello World");

    const auto originalCapacity = testString.capacity();
    const auto originalMaxSize = testString.max_size();

    REQUIRE_FALSE(testString.empty());
    REQUIRE(testString.size() == 11);

    testString.clear();

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(testString.capacity() == originalCapacity);
    REQUIRE(testString.max_size() == originalMaxSize);
  }

  SUBCASE("clear_and_reassignment") {
    FixedString<32> testString("Original");

    REQUIRE_FALSE(testString.empty());
    REQUIRE(std::strcmp(testString.c_str(), "Original") == 0);

    testString.clear();

    REQUIRE(testString.empty());
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);

    // Reassign after clear
    testString = "New content";

    REQUIRE_FALSE(testString.empty());
    REQUIRE(std::strcmp(testString.c_str(), "New content") == 0);
    REQUIRE(testString.size() == 11);
  }

  SUBCASE("multiple_clear_operations") {
    FixedString<16> testString("Test");

    // First clear
    testString.clear();
    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    // Assign new content
    testString = "New";
    REQUIRE_FALSE(testString.empty());
    REQUIRE(testString.size() == 3);

    // Second clear
    testString.clear();
    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    // Third clear (should be idempotent)
    testString.clear();
    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
  }
}

// insert() at position.
TEST_CASE("core/fixed_string/insert") {
  SUBCASE("insert_fixed_string_at_beginning") {
    FixedString<32> testString("World");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "World") == 0);

    testString.insert(0, FixedString<32>("Beautiful "));

    REQUIRE(testString.size() == 15);
    REQUIRE(std::strcmp(testString.c_str(), "Beautiful World") == 0);

    testString.insert(0, FixedString<8>("Hello "));

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
  }

  SUBCASE("insert_fixed_string_in_middle") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(6, FixedString<16>("Beautiful "));

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
  }

  SUBCASE("insert_fixed_string_at_end") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(11, FixedString<8>("!"));

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);
  }

  SUBCASE("insert_c_string_at_beginning") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(0, "Hi ");

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hi Hello World") == 0);
  }

  SUBCASE("insert_c_string_in_middle") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(6, "Beautiful ");

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
  }

  SUBCASE("insert_c_string_at_end") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(11, "!");

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);
  }

  SUBCASE("insert_single_character") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(5, ' ');

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello  World") == 0);
  }

  SUBCASE("insert_multiple_characters") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(0, '*', 3);

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "***Hello World") == 0);
  }

  SUBCASE("insert_zero_characters") {
    FixedString<32> testString("Hello World");

    const auto originalSize = testString.size();
    const auto originalContent = std::string(testString.c_str());

    testString.insert(0, FixedString<32>());

    REQUIRE(testString.size() == originalSize);
    REQUIRE(std::strcmp(testString.c_str(), originalContent.c_str()) == 0);

    testString.insert(0, std::string());

    REQUIRE(testString.size() == originalSize);
    REQUIRE(std::strcmp(testString.c_str(), originalContent.c_str()) == 0);

    testString.insert(0, 'X', 0);

    REQUIRE(testString.size() == originalSize);
    REQUIRE(std::strcmp(testString.c_str(), originalContent.c_str()) == 0);
  }

  SUBCASE("insert_into_empty_string") {
    FixedString<32> emptyString;

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString.insert(0, "Hello");

    REQUIRE_FALSE(emptyString.empty());
    REQUIRE(emptyString.size() == 5);
    REQUIRE(std::strcmp(emptyString.c_str(), "Hello") == 0);
  }

  SUBCASE("insert_at_position_0") {
    FixedString<32> testString("World");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "World") == 0);

    testString.insert(0, "Hello ");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("insert_at_end_position") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.insert(5, " World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("insert_special_characters") {
    FixedString<32> testString("Hello World");

    testString.insert(5, '\n');
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n World") == 0);
    REQUIRE(testString.size() == 12);

    testString.insert(0, '\t');
    REQUIRE(std::strcmp(testString.c_str(), "\tHello\n World") == 0);
    REQUIRE(testString.size() == 13);
  }

  SUBCASE("insert_unicode_content") {
    FixedString<64> testString("Hello");

    testString.insert(5, " 世界");
    REQUIRE(std::strcmp(testString.c_str(), "Hello 世界") == 0);
    REQUIRE(testString.size() == std::char_traits<char>::length("Hello 世界"));

    testString.insert(0, "Привет ");
    REQUIRE(std::strcmp(testString.c_str(), "Привет Hello 世界") == 0);
    REQUIRE(testString.size() == std::char_traits<char>::length("Привет Hello 世界"));
  }

  SUBCASE("insert_with_different_capacities") {
    FixedString<8> smallString("Hi");
    FixedString<16> mediumString("Hello");
    FixedString<32> largeString("Hello World");

    smallString.insert(2, "!");
    mediumString.insert(5, " World");
    largeString.insert(11, "!");

    REQUIRE(std::strcmp(smallString.c_str(), "Hi!") == 0);
    REQUIRE(std::strcmp(mediumString.c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(largeString.c_str(), "Hello World!") == 0);

    REQUIRE(smallString.size() == 3);
    REQUIRE(mediumString.size() == 11);
    REQUIRE(largeString.size() == 12);
  }

  SUBCASE("multiple_insert_operations") {
    FixedString<32> testString("Hello");

    // First insert
    testString.insert(5, " World");
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
    REQUIRE(testString.size() == 11);

    // Second insert
    testString.insert(0, "Hi ");
    REQUIRE(std::strcmp(testString.c_str(), "Hi Hello World") == 0);
    REQUIRE(testString.size() == 14);

    // Third insert
    testString.insert(14, "!");
    REQUIRE(std::strcmp(testString.c_str(), "Hi Hello World!") == 0);
    REQUIRE(testString.size() == 15);
  }

  SUBCASE("insert_character_at_various_positions") {
    FixedString<32> testString("ABCD");

    testString.insert(0, 'X');
    REQUIRE(std::strcmp(testString.c_str(), "XABCD") == 0);
    REQUIRE(testString.size() == 5);

    testString.insert(3, 'Y');
    REQUIRE(std::strcmp(testString.c_str(), "XABYCD") == 0);
    REQUIRE(testString.size() == 6);

    testString.insert(6, 'Z');
    REQUIRE(std::strcmp(testString.c_str(), "XABYCDZ") == 0);
    REQUIRE(testString.size() == 7);
  }
}

// erase() at position.
TEST_CASE("core/fixed_string/erase") {
  SUBCASE("erase_from_beginning") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.erase(0, 5);

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), " World") == 0);
  }

  SUBCASE("erase_from_middle") {
    FixedString<32> testString("Hello Beautiful World");

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);

    testString.erase(6, 10);

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("erase_from_end") {
    FixedString<32> testString("Hello World!");

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);

    testString.erase(11, 1);

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("erase_single_character") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.erase(5, 1);

    REQUIRE(testString.size() == 10);
    REQUIRE(std::strcmp(testString.c_str(), "HelloWorld") == 0);
  }

  SUBCASE("erase_zero_characters") {
    FixedString<32> testString("Hello World");

    const auto originalSize = testString.size();
    const auto originalContent = std::string(testString.c_str());

    testString.erase(5, 0);

    REQUIRE(testString.size() == originalSize);
    REQUIRE(std::strcmp(testString.c_str(), originalContent.c_str()) == 0);
  }

  SUBCASE("erase_from_position_to_end") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.erase(6);

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ") == 0);
  }

  SUBCASE("erase_everything") {
    FixedString<32> testString("Hello World");

    REQUIRE_FALSE(testString.empty());
    REQUIRE(testString.size() == 11);

    testString.erase(0);

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("erase_special_characters") {
    FixedString<32> testString("Hello\nWorld\t!");

    REQUIRE(testString.size() == 13);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\nWorld\t!") == 0);

    testString.erase(5, 1); // Erase newline

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "HelloWorld\t!") == 0);

    testString.erase(10, 1); // Erase tab

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "HelloWorld!") == 0);
  }

  SUBCASE("erase_unicode_content") {
    FixedString<64> testString("Hello 世界 World");

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello 世界 World"));
    REQUIRE(std::strcmp(testString.c_str(), "Hello 世界 World") == 0);

    testString.erase(6, 3); // Erase Chinese characters

    REQUIRE(testString.size() == 15);
    REQUIRE(std::strcmp(testString.c_str(), "Hello 界 World") == 0);
  }

  SUBCASE("erase_with_different_capacities") {
    FixedString<8> smallString("Hi!");
    FixedString<16> mediumString("Hello World");
    FixedString<32> largeString("This is a longer string");

    smallString.erase(2, 1);
    mediumString.erase(5, 1);
    largeString.erase(4, 3);

    REQUIRE(std::strcmp(smallString.c_str(), "Hi") == 0);
    REQUIRE(std::strcmp(mediumString.c_str(), "HelloWorld") == 0);
    REQUIRE(std::strcmp(largeString.c_str(), "This a longer string") == 0);

    REQUIRE(smallString.size() == 2);
    REQUIRE(mediumString.size() == 10);
    REQUIRE(largeString.size() == 20);
  }

  SUBCASE("multiple_erase_operations") {
    FixedString<32> testString("Hello Beautiful World!");

    // First erase
    testString.erase(6, 10);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);
    REQUIRE(testString.size() == 12);

    // Second erase
    testString.erase(5, 1);
    REQUIRE(std::strcmp(testString.c_str(), "HelloWorld!") == 0);
    REQUIRE(testString.size() == 11);

    // Third erase
    testString.erase(10, 1);
    REQUIRE(std::strcmp(testString.c_str(), "HelloWorld") == 0);
    REQUIRE(testString.size() == 10);
  }

  SUBCASE("erase_at_various_positions") {
    FixedString<32> testString("ABCDEFGH");

    testString.erase(0, 1); // Erase 'A'
    REQUIRE(std::strcmp(testString.c_str(), "BCDEFGH") == 0);
    REQUIRE(testString.size() == 7);

    testString.erase(3, 1); // Erase 'E'
    REQUIRE(std::strcmp(testString.c_str(), "BCDFGH") == 0);
    REQUIRE(testString.size() == 6);

    testString.erase(5, 1); // Erase 'H'
    REQUIRE(std::strcmp(testString.c_str(), "BCDFG") == 0);
    REQUIRE(testString.size() == 5);
  }

  SUBCASE("erase_entire_words") {
    FixedString<32> testString("The quick brown fox");

    testString.erase(0, 4); // Erase "The "
    REQUIRE(std::strcmp(testString.c_str(), "quick brown fox") == 0);
    REQUIRE(testString.size() == 15);

    testString.erase(6, 6); // Erase "brown "
    REQUIRE(std::strcmp(testString.c_str(), "quick fox") == 0);
    REQUIRE(testString.size() == 9);

    testString.erase(6); // Erase "fox"
    REQUIRE(std::strcmp(testString.c_str(), "quick ") == 0);
    REQUIRE(testString.size() == 6);
  }
}

// push_back() single character.
TEST_CASE("core/fixed_string/push_back") {
  SUBCASE("push_back_single_character") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.push_back('!');

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello!") == 0);
  }

  SUBCASE("push_back_multiple_characters") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.push_back(' ');
    testString.push_back('W');
    testString.push_back('o');
    testString.push_back('r');
    testString.push_back('l');
    testString.push_back('d');

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("push_back_to_empty_string") {
    FixedString<16> emptyString;

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString.push_back('A');

    REQUIRE_FALSE(emptyString.empty());
    REQUIRE(emptyString.size() == 1);
    REQUIRE(std::strcmp(emptyString.c_str(), "A") == 0);
  }

  SUBCASE("push_back_special_characters") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.push_back('\n');
    testString.push_back('\t');

    REQUIRE(testString.size() == 7);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n\t") == 0);
  }

  SUBCASE("push_back_with_different_capacities") {
    FixedString<8> smallString("Hi");
    FixedString<16> mediumString("Hello");
    FixedString<32> largeString("Hello World");

    smallString.push_back('!');
    mediumString.push_back(' ');
    mediumString.push_back('W');
    largeString.push_back('!');

    REQUIRE(std::strcmp(smallString.c_str(), "Hi!") == 0);
    REQUIRE(std::strcmp(mediumString.c_str(), "Hello W") == 0);
    REQUIRE(std::strcmp(largeString.c_str(), "Hello World!") == 0);

    REQUIRE(smallString.size() == 3);
    REQUIRE(mediumString.size() == 7);
    REQUIRE(largeString.size() == 12);
  }

  SUBCASE("push_back_numeric_characters") {
    FixedString<16> testString("123");

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "123") == 0);

    testString.push_back('4');
    testString.push_back('5');
    testString.push_back('6');

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "123456") == 0);
  }

  SUBCASE("push_back_mixed_content") {
    FixedString<32> testString("Test");

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Test") == 0);

    testString.push_back(' ');
    testString.push_back('1');
    testString.push_back('2');
    testString.push_back('3');
    testString.push_back('!');

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Test 123!") == 0);
  }

  SUBCASE("push_back_edge_cases") {
    FixedString<16> testString("A");

    REQUIRE(testString.size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString.push_back('B');
    testString.push_back('C');
    testString.push_back('D');

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "ABCD") == 0);
  }

  SUBCASE("push_back_and_size_consistency") {
    FixedString<16> testString;

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    for (int8_t i = 0; i < 5; ++i) {
      testString.push_back('A' + i);
      REQUIRE(testString.size() == i + 1);
    }

    REQUIRE(std::strcmp(testString.c_str(), "ABCDE") == 0);
    REQUIRE(testString.size() == 5);
  }

  SUBCASE("push_back_with_different_character_types") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.push_back(' ');
    testString.push_back('W');
    testString.push_back('o');
    testString.push_back('r');
    testString.push_back('l');
    testString.push_back('d');
    testString.push_back('!');

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);
  }
}

// pop_back() remove last character.
TEST_CASE("core/fixed_string/pop_back") {
  SUBCASE("pop_back_single_character") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.pop_back();

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Hell") == 0);
  }

  SUBCASE("pop_back_multiple_characters") {
    FixedString<16> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.pop_back();
    testString.pop_back();
    testString.pop_back();
    testString.pop_back();
    testString.pop_back();
    testString.pop_back();

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);
  }

  SUBCASE("pop_back_from_single_character_string") {
    FixedString<16> testString("A");

    REQUIRE(testString.size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString.pop_back();

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("pop_back_special_characters") {
    FixedString<16> testString("Hello\n\t!");

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n\t!") == 0);

    testString.pop_back(); // Remove '!'
    REQUIRE(testString.size() == 7);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n\t") == 0);

    testString.pop_back(); // Remove '\t'
    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n") == 0);

    testString.pop_back(); // Remove '\n'
    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);
  }

  SUBCASE("pop_back_with_different_capacities") {
    FixedString<8> smallString("Hi!");
    FixedString<16> mediumString("Hello World");
    FixedString<32> largeString("This is a longer string");

    smallString.pop_back();
    mediumString.pop_back();
    largeString.pop_back();

    REQUIRE(std::strcmp(smallString.c_str(), "Hi") == 0);
    REQUIRE(std::strcmp(mediumString.c_str(), "Hello Worl") == 0);
    REQUIRE(std::strcmp(largeString.c_str(), "This is a longer strin") == 0);

    REQUIRE(smallString.size() == 2);
    REQUIRE(mediumString.size() == 10);
    REQUIRE(largeString.size() == 22);
  }

  SUBCASE("pop_back_numeric_characters") {
    FixedString<16> testString("123456");

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "123456") == 0);

    testString.pop_back();
    testString.pop_back();
    testString.pop_back();

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "123") == 0);
  }

  SUBCASE("pop_back_mixed_content") {
    FixedString<32> testString("Test 123!");

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Test 123!") == 0);

    testString.pop_back(); // Remove '!'
    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Test 123") == 0);

    testString.pop_back(); // Remove '3'
    REQUIRE(testString.size() == 7);
    REQUIRE(std::strcmp(testString.c_str(), "Test 12") == 0);

    testString.pop_back(); // Remove '2'
    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Test 1") == 0);
  }

  SUBCASE("pop_back_edge_cases") {
    FixedString<16> testString("ABCD");

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "ABCD") == 0);

    testString.pop_back(); // Remove 'D'
    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "ABC") == 0);

    testString.pop_back(); // Remove 'C'
    REQUIRE(testString.size() == 2);
    REQUIRE(std::strcmp(testString.c_str(), "AB") == 0);

    testString.pop_back(); // Remove 'B'
    REQUIRE(testString.size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString.pop_back(); // Remove 'A'
    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("pop_back_and_size_consistency") {
    FixedString<16> testString("ABCDE");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "ABCDE") == 0);

    for (int i = 4; i >= 0; --i) {
      testString.pop_back();
      REQUIRE(testString.size() == i);
    }

    REQUIRE(testString.empty());
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("pop_back_with_different_character_types") {
    FixedString<16> testString("Hello World!");

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);

    testString.pop_back(); // Remove '!'
    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.pop_back(); // Remove 'd'
    REQUIRE(testString.size() == 10);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Worl") == 0);

    testString.pop_back(); // Remove 'l'
    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Wor") == 0);
  }

  SUBCASE("pop_back_and_reassignment") {
    FixedString<16> testString("Original");

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Original") == 0);

    testString.pop_back();
    testString.pop_back();
    testString.pop_back();

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Origi") == 0);

    // Reassign after pop_back
    testString = "New content";

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "New content") == 0);
  }

  SUBCASE("pop_back_with_capacity_preservation") {
    FixedString<16> testString("Hello World");

    const auto originalCapacity = testString.capacity();
    const auto originalMaxSize = testString.max_size();

    REQUIRE(testString.size() == 11);

    testString.pop_back();
    testString.pop_back();
    testString.pop_back();

    REQUIRE(testString.size() == 8);
    REQUIRE(testString.capacity() == originalCapacity);
    REQUIRE(testString.max_size() == originalMaxSize);
  }
}

// utf8_pop_back() remove last code point.
TEST_CASE("core/fixed_string/utf8_pop_back") {
  SUBCASE("pop_back_single_ascii_character") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(testString.utf8_size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.utf8_pop_back();

    REQUIRE(testString.size() == 4);
    REQUIRE(testString.utf8_size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Hell") == 0);
  }

  SUBCASE("pop_back_multiple_ascii_characters") {
    FixedString<16> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(testString.utf8_size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.utf8_pop_back();
    testString.utf8_pop_back();
    testString.utf8_pop_back();

    REQUIRE(testString.size() == 8);
    REQUIRE(testString.utf8_size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Wo") == 0);
  }

  SUBCASE("pop_back_utf8_cyrillic_characters") {
    FixedString<32> testString("Hello привет");

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello привет"));
    REQUIRE(testString.utf8_size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello привет") == 0);

    testString.utf8_pop_back(); // Remove 'т'

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello приве"));
    REQUIRE(testString.utf8_size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello приве") == 0);
  }

  SUBCASE("pop_back_multiple_utf8_characters") {
    FixedString<32> testString("Hello привет");

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello привет"));
    REQUIRE(testString.utf8_size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello привет") == 0);

    testString.utf8_pop_back(); // Remove 'т'
    testString.utf8_pop_back(); // Remove 'е'
    testString.utf8_pop_back(); // Remove 'в'

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello при"));
    REQUIRE(testString.utf8_size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Hello при") == 0);
  }

  SUBCASE("pop_back_mixed_ascii_and_utf8") {
    FixedString<32> testString("Hello привет");

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello привет"));
    REQUIRE(testString.utf8_size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello привет") == 0);

    testString.utf8_pop_back(); // Remove 'т'
    testString.utf8_pop_back(); // Remove 'е'
    testString.utf8_pop_back(); // Remove 'в'
    testString.utf8_pop_back(); // Remove 'и'
    testString.utf8_pop_back(); // Remove 'р'
    testString.utf8_pop_back(); // Remove 'п'

    REQUIRE(testString.size() == 6);
    REQUIRE(testString.utf8_size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ") == 0);
  }

  SUBCASE("pop_back_from_single_ascii_character") {
    FixedString<8> testString("A");

    REQUIRE(testString.size() == 1);
    REQUIRE(testString.utf8_size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString.utf8_pop_back();

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(testString.utf8_size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("pop_back_from_single_utf8_character") {
    FixedString<8> testString("п");

    REQUIRE(testString.size() == 2);
    REQUIRE(testString.utf8_size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "п") == 0);

    testString.utf8_pop_back();

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(testString.utf8_size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("pop_back_utf8_emoji_characters") {
    FixedString<32> testString("Hello 🌍 World");

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello 🌍 World"));
    REQUIRE(testString.utf8_size() == 13);
    REQUIRE(std::strcmp(testString.c_str(), "Hello 🌍 World") == 0);

    testString.utf8_pop_back(); // Remove 'd'
    testString.utf8_pop_back(); // Remove 'l'
    testString.utf8_pop_back(); // Remove 'r'
    testString.utf8_pop_back(); // Remove 'o'
    testString.utf8_pop_back(); // Remove 'W'
    testString.utf8_pop_back(); // Remove ' '
    testString.utf8_pop_back(); // Remove '🌍'
    testString.utf8_pop_back(); // Remove ' '

    REQUIRE(testString.size() == 5);
    REQUIRE(testString.utf8_size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);
  }

  SUBCASE("pop_back_with_different_capacities") {
    FixedString<8> smallString("Hi");
    FixedString<16> mediumString("Hello 世界");
    FixedString<32> largeString("Hello привет мир");

    smallString.utf8_pop_back();
    mediumString.utf8_pop_back();
    largeString.utf8_pop_back();

    REQUIRE(std::strcmp(smallString.c_str(), "H") == 0);
    REQUIRE(std::strcmp(mediumString.c_str(), "Hello 世") == 0);
    REQUIRE(std::strcmp(largeString.c_str(), "Hello привет ми") == 0);

    REQUIRE(smallString.size() == 1);
    REQUIRE(mediumString.size() == std::char_traits<char>::length("Hello 世"));
    REQUIRE(largeString.size() == std::char_traits<char>::length("Hello привет ми"));

    REQUIRE(smallString.utf8_size() == 1);
    REQUIRE(mediumString.utf8_size() == 7);
    REQUIRE(largeString.utf8_size() == 15);
  }

  SUBCASE("pop_back_numeric_and_special_characters") {
    FixedString<16> testString("123!@#");

    REQUIRE(testString.size() == 6);
    REQUIRE(testString.utf8_size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "123!@#") == 0);

    testString.utf8_pop_back(); // Remove '#'
    testString.utf8_pop_back(); // Remove '@'
    testString.utf8_pop_back(); // Remove '!'

    REQUIRE(testString.size() == 3);
    REQUIRE(testString.utf8_size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "123") == 0);
  }

  SUBCASE("pop_back_edge_cases") {
    FixedString<16> testString("ABC");

    REQUIRE(testString.size() == 3);
    REQUIRE(testString.utf8_size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "ABC") == 0);

    testString.utf8_pop_back(); // Remove 'C'
    REQUIRE(testString.size() == 2);
    REQUIRE(testString.utf8_size() == 2);
    REQUIRE(std::strcmp(testString.c_str(), "AB") == 0);

    testString.utf8_pop_back(); // Remove 'B'
    REQUIRE(testString.size() == 1);
    REQUIRE(testString.utf8_size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString.utf8_pop_back(); // Remove 'A'
    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(testString.utf8_size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("pop_back_and_size_consistency") {
    FixedString<16> testString("ABCDE");

    REQUIRE(testString.size() == 5);
    REQUIRE(testString.utf8_size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "ABCDE") == 0);

    for (int i = 4; i >= 0; --i) {
      testString.utf8_pop_back();
      REQUIRE(testString.size() == i);
      REQUIRE(testString.utf8_size() == i);
    }

    REQUIRE(testString.empty());
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("pop_back_with_capacity_preservation") {
    FixedString<20> testString("Hello привет");

    const auto originalCapacity = testString.capacity();
    const auto originalMaxSize = testString.max_size();

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello привет"));
    REQUIRE(testString.utf8_size() == 12);

    testString.utf8_pop_back();
    testString.utf8_pop_back();
    testString.utf8_pop_back();

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello при"));
    REQUIRE(testString.utf8_size() == 9);
    REQUIRE(testString.capacity() == originalCapacity);
    REQUIRE(testString.max_size() == originalMaxSize);
  }

  SUBCASE("pop_back_and_reassignment") {
    FixedString<16> testString("Original");

    REQUIRE(testString.size() == 8);
    REQUIRE(testString.utf8_size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Original") == 0);

    testString.utf8_pop_back();
    testString.utf8_pop_back();
    testString.utf8_pop_back();

    REQUIRE(testString.size() == 5);
    REQUIRE(testString.utf8_size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Origi") == 0);

    // Reassign after utf8_pop_back
    testString = "New content";

    REQUIRE(testString.size() == 11);
    REQUIRE(testString.utf8_size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "New content") == 0);
  }
}

// append() string or characters.
TEST_CASE("core/fixed_string/append") {
  SUBCASE("append_fixed_string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(FixedString<32>(" Beautiful"));

    REQUIRE(testString.size() == 15);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful") == 0);

    testString.append(FixedString<16>(" World"));

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
  }

  SUBCASE("append_c_string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(" World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("append_c_string_with_count") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(" World!", 6);

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("append_partial_c_string") {
    FixedString<32> testString("Test");

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Test") == 0);

    testString.append("12345", 3);

    REQUIRE(testString.size() == 7);
    REQUIRE(std::strcmp(testString.c_str(), "Test123") == 0);
  }

  SUBCASE("append_zero_count_from_c_string") {
    FixedString<32> testString("Hello");

    const auto originalSize = testString.size();
    const auto originalContent = std::string(testString.c_str());

    testString.append("World", 0);

    REQUIRE(testString.size() == originalSize);
    REQUIRE(std::strcmp(testString.c_str(), originalContent.c_str()) == 0);
  }

  SUBCASE("append_std_string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(std::string(" World"));

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("append_single_character") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(1, '!');

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello!") == 0);
  }

  SUBCASE("append_multiple_characters") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(3, ' ');

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hello   ") == 0);
  }

  SUBCASE("append_to_empty_string") {
    FixedString<32> emptyString;

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString.append("Hello");

    REQUIRE_FALSE(emptyString.empty());
    REQUIRE(emptyString.size() == 5);
    REQUIRE(std::strcmp(emptyString.c_str(), "Hello") == 0);
  }

  SUBCASE("append_zero_characters") {
    FixedString<32> testString("Hello");

    const auto originalSize = testString.size();
    const auto originalContent = std::string(testString.c_str());

    testString.append(0, 'X');

    REQUIRE(testString.size() == originalSize);
    REQUIRE(std::strcmp(testString.c_str(), originalContent.c_str()) == 0);
  }

  SUBCASE("append_special_characters") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(1, '\n');
    testString.append(1, '\t');
    testString.append(1, '!');

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n\t!") == 0);
  }

  SUBCASE("append_unicode_content") {
    FixedString<64> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(" 世界");

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(std::strcmp(testString.c_str(), "Hello 世界") == 0);
  }

  SUBCASE("append_with_different_capacities") {
    FixedString<8> smallString("Hi");
    FixedString<16> mediumString("Hello");
    FixedString<32> largeString("Hello World");

    smallString.append("!");
    mediumString.append(" World");
    largeString.append("!");

    REQUIRE(std::strcmp(smallString.c_str(), "Hi!") == 0);
    REQUIRE(std::strcmp(mediumString.c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(largeString.c_str(), "Hello World!") == 0);

    REQUIRE(smallString.size() == 3);
    REQUIRE(mediumString.size() == 11);
    REQUIRE(largeString.size() == 12);
  }

  SUBCASE("append_numeric_content") {
    FixedString<16> testString("123");

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "123") == 0);

    testString.append("456");

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "123456") == 0);
  }

  SUBCASE("append_mixed_content") {
    FixedString<32> testString("Test");

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Test") == 0);

    testString.append(" 123!");

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Test 123!") == 0);
  }

  SUBCASE("append_edge_cases") {
    FixedString<16> testString("A");

    REQUIRE(testString.size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString.append("BC");

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "ABC") == 0);
  }

  SUBCASE("append_and_size_consistency") {
    FixedString<16> testString;

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    testString.append("A");
    REQUIRE(testString.size() == 1);

    testString.append("B");
    REQUIRE(testString.size() == 2);

    testString.append("C");
    REQUIRE(testString.size() == 3);

    REQUIRE(std::strcmp(testString.c_str(), "ABC") == 0);
  }

  SUBCASE("append_chaining") {
    FixedString<32> testString("");

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    testString.append(FixedString<16>("Nothing"))
      .append(FixedString<8>(" else"))
      .append(std::string(" really"))
      .append(" matters")
      .append(3, '.');

    REQUIRE(testString.size() == 30);
    REQUIRE(std::strcmp(testString.c_str(), "Nothing else really matters...") == 0);
  }

  SUBCASE("append_and_reassignment") {
    FixedString<18> testString("Original");

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Original") == 0);

    testString.append(" content");

    REQUIRE(testString.size() == 16);
    REQUIRE(std::strcmp(testString.c_str(), "Original content") == 0);

    // Reassign after append
    testString = "New";

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "New") == 0);
  }

  SUBCASE("append_with_capacity_preservation") {
    FixedString<16> testString("Hello");

    const auto originalCapacity = testString.capacity();
    const auto originalMaxSize = testString.max_size();

    REQUIRE(testString.size() == 5);

    testString.append(" World");

    REQUIRE(testString.size() == 11);
    REQUIRE(testString.capacity() == originalCapacity);
    REQUIRE(testString.max_size() == originalMaxSize);
  }
}

// operator+= with FixedString, C string, character.
TEST_CASE("core/fixed_string/operators_plus_assign") {
  SUBCASE("operator_plus_assign_with_fixed_string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += FixedString<32>(" Beautiful");

    REQUIRE(testString.size() == 15);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful") == 0);

    testString += FixedString<16>(" World");

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
  }

  SUBCASE("operator_plus_assign_with_c_string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += " World";

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("operator_plus_assign_with_std_string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += std::string(" World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("operator_plus_assign_with_single_character") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += '!';

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello!") == 0);
  }

  SUBCASE("operator_plus_assign_to_empty_string") {
    FixedString<32> emptyString;

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString += "Hello";

    REQUIRE_FALSE(emptyString.empty());
    REQUIRE(emptyString.size() == 5);
    REQUIRE(std::strcmp(emptyString.c_str(), "Hello") == 0);
  }

  SUBCASE("operator_plus_assign_with_special_characters") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += '\n';
    testString += '\t';
    testString += '!';

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n\t!") == 0);
  }

  SUBCASE("operator_plus_assign_with_unicode_content") {
    FixedString<64> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += " 世界";

    REQUIRE(testString.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(std::strcmp(testString.c_str(), "Hello 世界") == 0);
  }

  SUBCASE("operator_plus_assign_with_different_capacities") {
    FixedString<8> smallString("Hi");
    FixedString<16> mediumString("Hello");
    FixedString<32> largeString("Hello World");

    smallString += "!";
    mediumString += " World";
    largeString += "!";

    REQUIRE(std::strcmp(smallString.c_str(), "Hi!") == 0);
    REQUIRE(std::strcmp(mediumString.c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(largeString.c_str(), "Hello World!") == 0);

    REQUIRE(smallString.size() == 3);
    REQUIRE(mediumString.size() == 11);
    REQUIRE(largeString.size() == 12);
  }

  SUBCASE("operator_plus_assign_numeric_content") {
    FixedString<16> testString("123");

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "123") == 0);

    testString += "456";

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "123456") == 0);
  }

  SUBCASE("operator_plus_assign_mixed_content") {
    FixedString<32> testString("Test");

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Test") == 0);

    testString += " 123!";

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Test 123!") == 0);
  }

  SUBCASE("operator_plus_assign_edge_cases") {
    FixedString<16> testString("A");

    REQUIRE(testString.size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString += "BC";

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "ABC") == 0);
  }

  SUBCASE("operator_plus_assign_and_size_consistency") {
    FixedString<16> testString;

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    testString += "A";
    REQUIRE(testString.size() == 1);

    testString += "B";
    REQUIRE(testString.size() == 2);

    testString += "C";
    REQUIRE(testString.size() == 3);

    REQUIRE(std::strcmp(testString.c_str(), "ABC") == 0);
  }

  SUBCASE("operator_plus_assign_chaining") {
    FixedString<32> testString("");

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    testString += FixedString<16>("Nothing");
    testString += FixedString<8>(" else");
    testString += std::string(" really");
    testString += " matters";
    testString += '.';

    REQUIRE(testString.size() == 28);
    REQUIRE(std::strcmp(testString.c_str(), "Nothing else really matters.") == 0);
  }

  SUBCASE("operator_plus_assign_and_reassignment") {
    FixedString<18> testString("Original");

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Original") == 0);

    testString += " content";

    REQUIRE(testString.size() == 16);
    REQUIRE(std::strcmp(testString.c_str(), "Original content") == 0);

    // Reassign after +=
    testString = "New";

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "New") == 0);
  }

  SUBCASE("operator_plus_assign_with_capacity_preservation") {
    FixedString<16> testString("Hello");

    const auto originalCapacity = testString.capacity();
    const auto originalMaxSize = testString.max_size();

    REQUIRE(testString.size() == 5);

    testString += " World";

    REQUIRE(testString.size() == 11);
    REQUIRE(testString.capacity() == originalCapacity);
    REQUIRE(testString.max_size() == originalMaxSize);
  }

  SUBCASE("operator_plus_assign_with_different_types") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += FixedString<8>(" ");
    testString += "World";
    testString += std::string("!");
    testString += ' ';
    testString += '2';

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World! 2") == 0);
  }

  SUBCASE("operator_plus_assign_with_empty_operands") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += FixedString<8>("");
    testString += "";
    testString += std::string("");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);
  }
}

// replace() at position.
TEST_CASE("core/fixed_string/replace") {
  SUBCASE("replace_with_fixed_string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, FixedString<32>("Universe"));

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SUBCASE("replace_with_std_string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, std::string("Universe"));

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SUBCASE("replace_with_c_string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, "Universe");

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SUBCASE("replace_with_repeated_characters") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, '*', 3);

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ***") == 0);

    testString.replace(2, 0, '*', 0);

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ***") == 0);

    testString.replace(0, 5, '*', 5);

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "***** ***") == 0);
  }

  SUBCASE("replace_at_beginning") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 5, "Hi");

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hi World") == 0);
  }

  SUBCASE("replace_at_end") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, "Universe!");

    REQUIRE(testString.size() == 15);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe!") == 0);
  }

  SUBCASE("replace_with_empty_string") {
    FixedString<32> testString1("Hello World");
    FixedString<32> testString2("Hello World");

    REQUIRE(testString1.size() == 11);
    REQUIRE(testString2.size() == 11);
    REQUIRE(std::strcmp(testString1.c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(testString2.c_str(), "Hello World") == 0);

    testString1.replace(5, 1, "");
    testString2.replace(5, 0, "");

    REQUIRE(testString1.size() == 10);
    REQUIRE(testString2.size() == 11);
    REQUIRE(std::strcmp(testString1.c_str(), "HelloWorld") == 0);
    REQUIRE(std::strcmp(testString2.c_str(), "Hello World") == 0);
  }

  SUBCASE("replace_with_longer_string") {
    FixedString<32> testString("Hi");

    REQUIRE(testString.size() == 2);
    REQUIRE(std::strcmp(testString.c_str(), "Hi") == 0);

    testString.replace(0, 2, "Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SUBCASE("replace_with_shorter_string") {
    FixedString<32> testString1("Hello World");
    FixedString<32> testString2("Hello World");

    REQUIRE(testString1.size() == 11);
    REQUIRE(testString2.size() == 11);
    REQUIRE(std::strcmp(testString1.c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(testString2.c_str(), "Hello World") == 0);

    testString1.replace(0, 5, "Hi");
    testString2.replace(0, 5, "HELLO");

    REQUIRE(testString1.size() == 8);
    REQUIRE(testString2.size() == 11);
    REQUIRE(std::strcmp(testString1.c_str(), "Hi World") == 0);
    REQUIRE(std::strcmp(testString2.c_str(), "HELLO World") == 0);
  }

  SUBCASE("replace_single_character") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 1, "h");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "hello World") == 0);
  }

  SUBCASE("replace_multiple_characters_with_single_character") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 5, "H");

    REQUIRE(testString.size() == 7);
    REQUIRE(std::strcmp(testString.c_str(), "H World") == 0);
  }

  SUBCASE("replace_with_repeated_characters_at_different_positions") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 1, '*', 2);
    testString.replace(8, 2, '#', 3);

    REQUIRE(testString.size() == 13);
    REQUIRE(std::strcmp(testString.c_str(), "**ello W###ld") == 0);
  }

  SUBCASE("replace_entire_string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 11, "Goodbye Universe");

    REQUIRE(testString.size() == 16);
    REQUIRE(std::strcmp(testString.c_str(), "Goodbye Universe") == 0);
  }

  SUBCASE("replace_with_zero_count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 0, "Beautiful ");

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
  }

  SUBCASE("replace_with_single_character_count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, '!', 1);

    REQUIRE(testString.size() == 7);
    REQUIRE(std::strcmp(testString.c_str(), "Hello !") == 0);
  }

  SUBCASE("replace_with_multiple_character_count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, '=', 4);

    REQUIRE(testString.size() == 10);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ====") == 0);
  }

  SUBCASE("replace_with_zero_character_count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 0, FixedString<32>());

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, 'X', 0);

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ") == 0);
  }

  SUBCASE("replace_middle_portion") {
    FixedString<32> testString("Hello Beautiful World");

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);

    testString.replace(6, 9, "Amazing");

    REQUIRE(testString.size() == 19);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Amazing World") == 0);
  }

  SUBCASE("replace_with_same_length_string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 5, "Greet");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Greet World") == 0);
  }

  SUBCASE("replace_with_fixed_string_of_different_capacity") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, FixedString<12>("Universe"));

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SUBCASE("replace_with_array") {
    FixedString<32> testString("Hello World");
    constexpr array<char, 9> arr = {'U', 'n', 'i', 'v', 'e', 'r', 's', 'e', '\0'};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, arr.data());

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SUBCASE("replace_at_position_0_with_zero_count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 0, "Hi ");

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hi Hello World") == 0);
  }

  SUBCASE("replace_at_end_with_zero_count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(11, 0, "!");

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);
  }
}

// copy() to buffer.
TEST_CASE("core/fixed_string/copy") {
  SUBCASE("copy_entire_string") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 11) == 11);
    REQUIRE(strncmp(buffer, "Hello World", 11) == 0);
  }

  SUBCASE("copy_partial_string_from_beginning") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5) == 5);
    REQUIRE(strncmp(buffer, "Hello", 5) == 0);
  }

  SUBCASE("copy_partial_string_from_middle") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SUBCASE("copy_with_npos_count") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, FixedString<16>::npos) == 11);
    REQUIRE(strncmp(buffer, "Hello World", 11) == 0);
  }

  SUBCASE("copy_with_count_exceeding_remaining_characters") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 20, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SUBCASE("copy_from_position_0") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5, 0) == 5);
    REQUIRE(strncmp(buffer, "Hello", 5) == 0);
  }

  SUBCASE("copy_single_character") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 1, 6) == 1);
    REQUIRE(buffer[0] == 'W');
  }

  SUBCASE("copy_from_end_position") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 1, 10) == 1);
    REQUIRE(buffer[0] == 'd');
  }

  SUBCASE("copy_with_zero_count") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 0) == 0);
  }

  SUBCASE("copy_from_empty_string") {
    constexpr FixedString<16> testString("");
    char buffer[16] = {0};

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    REQUIRE(testString.copy(buffer, 5) == 0);
  }

  SUBCASE("copy_to_small_buffer") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[3] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 2) == 2);
    REQUIRE(strncmp(buffer, "He", 2) == 0);
  }

  SUBCASE("copy_with_position_at_end") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5, 11) == 0);
  }

  SUBCASE("copy_with_npos_from_middle") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, FixedString<16>::npos, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SUBCASE("copy_with_exact_remaining_characters") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SUBCASE("copy_with_count_larger_than_string_size") {
    constexpr FixedString<16> testString("Hi");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 2);
    REQUIRE(std::strcmp(testString.c_str(), "Hi") == 0);

    REQUIRE(testString.copy(buffer, 10) == 2);
    REQUIRE(strncmp(buffer, "Hi", 2) == 0);
  }
}

// swap() member.
TEST_CASE("core/fixed_string/swap") {
  SUBCASE("swap_two_different_strings") {
    FixedString<16> string1("Hello");
    FixedString<16> string2("World");

    REQUIRE(string1.size() == 5);
    REQUIRE(std::strcmp(string1.c_str(), "Hello") == 0);
    REQUIRE(string2.size() == 5);
    REQUIRE(std::strcmp(string2.c_str(), "World") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "World") == 0);
    REQUIRE(string1.size() == 5);
    REQUIRE(std::strcmp(string2.c_str(), "Hello") == 0);
    REQUIRE(string2.size() == 5);
  }

  SUBCASE("swap_strings_of_different_lengths") {
    FixedString<32> string1("Short");
    FixedString<32> string2("This is a much longer string");

    REQUIRE(string1.size() == 5);
    REQUIRE(std::strcmp(string1.c_str(), "Short") == 0);
    REQUIRE(string2.size() == 28);
    REQUIRE(std::strcmp(string2.c_str(), "This is a much longer string") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "This is a much longer string") == 0);
    REQUIRE(string1.size() == 28);
    REQUIRE(std::strcmp(string2.c_str(), "Short") == 0);
    REQUIRE(string2.size() == 5);
  }

  SUBCASE("swap_with_empty_string") {
    FixedString<16> string1("Hello World");
    FixedString<16> string2("");

    REQUIRE(string1.size() == 11);
    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);
    REQUIRE(string2.empty());
    REQUIRE(string2.size() == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "") == 0);
    REQUIRE(string1.empty());
    REQUIRE(string1.size() == 0);
    REQUIRE(std::strcmp(string2.c_str(), "Hello World") == 0);
    REQUIRE(string2.size() == 11);
  }

  SUBCASE("swap_two_empty_strings") {
    FixedString<16> string1("");
    FixedString<16> string2("");

    REQUIRE(string1.empty());
    REQUIRE(string1.size() == 0);
    REQUIRE(string2.empty());
    REQUIRE(string2.size() == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "") == 0);
    REQUIRE(string1.empty());
    REQUIRE(string1.size() == 0);
    REQUIRE(std::strcmp(string2.c_str(), "") == 0);
    REQUIRE(string2.empty());
    REQUIRE(string2.size() == 0);
  }

  SUBCASE("self_swap_no_op") {
    FixedString<16> string1("Hello World");

    REQUIRE(string1.size() == 11);
    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);

    string1.swap(string1);

    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);
    REQUIRE(string1.size() == 11);
  }

  SUBCASE("swap_with_single_character_strings") {
    FixedString<8> string1("A");
    FixedString<8> string2("B");

    REQUIRE(string1.size() == 1);
    REQUIRE(std::strcmp(string1.c_str(), "A") == 0);
    REQUIRE(string2.size() == 1);
    REQUIRE(std::strcmp(string2.c_str(), "B") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "B") == 0);
    REQUIRE(string1.size() == 1);
    REQUIRE(std::strcmp(string2.c_str(), "A") == 0);
    REQUIRE(string2.size() == 1);
  }

  SUBCASE("swap_with_maximum_length_strings") {
    FixedString<16> string1("123456789012345");
    FixedString<16> string2("ABCDEFGHIJKLMNO");

    REQUIRE(string1.size() == 15);
    REQUIRE(std::strcmp(string1.c_str(), "123456789012345") == 0);
    REQUIRE(string2.size() == 15);
    REQUIRE(std::strcmp(string2.c_str(), "ABCDEFGHIJKLMNO") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "ABCDEFGHIJKLMNO") == 0);
    REQUIRE(string1.size() == 15);
    REQUIRE(std::strcmp(string2.c_str(), "123456789012345") == 0);
    REQUIRE(string2.size() == 15);
  }

  SUBCASE("chained_swap_operations") {
    FixedString<16> string1("First");
    FixedString<16> string2("Second");
    FixedString<16> string3("Third");

    REQUIRE(string1.size() == 5);
    REQUIRE(std::strcmp(string1.c_str(), "First") == 0);
    REQUIRE(string2.size() == 6);
    REQUIRE(std::strcmp(string2.c_str(), "Second") == 0);
    REQUIRE(string3.size() == 5);
    REQUIRE(std::strcmp(string3.c_str(), "Third") == 0);

    string3.swap(string2);
    string2.swap(string1);
    string1.swap(string3);

    REQUIRE(std::strcmp(string1.c_str(), "Second") == 0);
    REQUIRE(string1.size() == 6);
    REQUIRE(std::strcmp(string2.c_str(), "First") == 0);
    REQUIRE(string2.size() == 5);
    REQUIRE(std::strcmp(string3.c_str(), "Third") == 0);
    REQUIRE(string3.size() == 5);
  }

  SUBCASE("swap_with_repeated_characters") {
    FixedString<20> string1("AAA");
    FixedString<20> string2("BBB");

    REQUIRE(string1.size() == 3);
    REQUIRE(std::strcmp(string1.c_str(), "AAA") == 0);
    REQUIRE(string2.size() == 3);
    REQUIRE(std::strcmp(string2.c_str(), "BBB") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "BBB") == 0);
    REQUIRE(string1.size() == 3);
    REQUIRE(std::strcmp(string2.c_str(), "AAA") == 0);
    REQUIRE(string2.size() == 3);
  }

  SUBCASE("swap_with_special_characters") {
    FixedString<32> string1("Hello\n\tWorld!");
    FixedString<32> string2("Test!@#$%^&*()");

    REQUIRE(string1.size() == 13);
    REQUIRE(std::strcmp(string1.c_str(), "Hello\n\tWorld!") == 0);
    REQUIRE(string2.size() == 14);
    REQUIRE(std::strcmp(string2.c_str(), "Test!@#$%^&*()") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "Test!@#$%^&*()") == 0);
    REQUIRE(string1.size() == 14);
    REQUIRE(std::strcmp(string2.c_str(), "Hello\n\tWorld!") == 0);
    REQUIRE(string2.size() == 13);
  }

  SUBCASE("swap_with_unicode_content") {
    FixedString<64> string1("Hello 世界");
    FixedString<64> string2("Привет мир");

    REQUIRE(string1.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(std::strcmp(string1.c_str(), "Hello 世界") == 0);
    REQUIRE(string2.size() == std::char_traits<char>::length("Привет мир"));
    REQUIRE(std::strcmp(string2.c_str(), "Привет мир") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "Привет мир") == 0);
    REQUIRE(string1.size() == std::char_traits<char>::length("Привет мир"));
    REQUIRE(std::strcmp(string2.c_str(), "Hello 世界") == 0);
    REQUIRE(string2.size() == std::char_traits<char>::length("Hello 世界"));
  }

  SUBCASE("swap_with_numeric_content") {
    FixedString<16> string1("12345");
    FixedString<16> string2("67890");

    REQUIRE(string1.size() == 5);
    REQUIRE(std::strcmp(string1.c_str(), "12345") == 0);
    REQUIRE(string2.size() == 5);
    REQUIRE(std::strcmp(string2.c_str(), "67890") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "67890") == 0);
    REQUIRE(string1.size() == 5);
    REQUIRE(std::strcmp(string2.c_str(), "12345") == 0);
    REQUIRE(string2.size() == 5);
  }

  SUBCASE("swap_with_mixed_content") {
    FixedString<32> string1("Hello123World!@#");
    FixedString<32> string2("Test\n456\t!@#$");

    REQUIRE(string1.size() == 16);
    REQUIRE(std::strcmp(string1.c_str(), "Hello123World!@#") == 0);
    REQUIRE(string2.size() == 13);
    REQUIRE(std::strcmp(string2.c_str(), "Test\n456\t!@#$") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "Test\n456\t!@#$") == 0);
    REQUIRE(string1.size() == 13);
    REQUIRE(std::strcmp(string2.c_str(), "Hello123World!@#") == 0);
    REQUIRE(string2.size() == 16);
  }
}

// find() substring and character.
TEST_CASE("core/fixed_string/find") {
  SUBCASE("find_fixed_string_substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find(FixedString<32>("World")) == 6);
    REQUIRE(testString.find(FixedString<32>("Hello")) == 0);
    REQUIRE(testString.find(FixedString<32>("lo Wo")) == 3);
    REQUIRE(testString.find(FixedString<32>(" ")) == 5);
    REQUIRE(testString.find(FixedString<32>("xyz")) == FixedString<32>::npos);

    static_assert(testString.find(FixedString<32>("World")) == 6);
    static_assert(testString.find(FixedString<32>("Hello")) == 0);
    static_assert(testString.find(FixedString<32>("lo Wo")) == 3);
    static_assert(testString.find(FixedString<32>(" ")) == 5);
    static_assert(testString.find(FixedString<32>("xyz")) == FixedString<32>::npos);
  }

  SUBCASE("find_string_like_substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find(std::string("World")) == 6);
    REQUIRE(testString.find(std::string("Hello")) == 0);
    REQUIRE(testString.find(std::string("lo Wo")) == 3);
    REQUIRE(testString.find(std::string("xyz")) == FixedString<32>::npos);

    static_assert(testString.find(CStringView("World")) == 6);
    static_assert(testString.find(CStringView("Hello")) == 0);
    static_assert(testString.find(CStringView("lo Wo")) == 3);
    static_assert(testString.find(CStringView("xyz")) == FixedString<32>::npos);
  }

  SUBCASE("find_c_string_substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("lo Wo") == 3);
    REQUIRE(testString.find("xyz") == FixedString<32>::npos);

    static_assert(testString.find("World") == 6);
    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find("lo Wo") == 3);
    static_assert(testString.find("xyz") == FixedString<32>::npos);
  }

  SUBCASE("find_character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find('H') == 0);
    REQUIRE(testString.find('l') == 2);
    REQUIRE(testString.find('o') == 4);
    REQUIRE(testString.find('W') == 6);
    REQUIRE(testString.find('d') == 10);
    REQUIRE(testString.find('x') == FixedString<32>::npos);

    static_assert(testString.find('H') == 0);
    static_assert(testString.find('l') == 2);
    static_assert(testString.find('o') == 4);
    static_assert(testString.find('W') == 6);
    static_assert(testString.find('d') == 10);
    static_assert(testString.find('x') == FixedString<32>::npos);
  }

  SUBCASE("find_with_position_parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == 12);
    REQUIRE(testString.find("Hello", 13) == FixedString<32>::npos);
    REQUIRE(testString.find('l', 0) == 2);
    REQUIRE(testString.find('l', 3) == 3);
    REQUIRE(testString.find('l', 4) == 9);
    REQUIRE(testString.find('l', 10) == 14);

    static_assert(testString.find("Hello", 0) == 0);
    static_assert(testString.find("Hello", 1) == 12);
    static_assert(testString.find("Hello", 13) == FixedString<32>::npos);
    static_assert(testString.find('l', 0) == 2);
    static_assert(testString.find('l', 3) == 3);
    static_assert(testString.find('l', 4) == 9);
    static_assert(testString.find('l', 10) == 14);
  }

  SUBCASE("find_empty_substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find(FixedString<16>("")) == 0);
    REQUIRE(testString.find(FixedString<16>()) == 0);
    REQUIRE(testString.find(std::string("")) == 0);
    REQUIRE(testString.find("") == 0);
    REQUIRE(testString.find("", 5) == 5);
    REQUIRE(testString.find("", 11) == 11);
    REQUIRE(testString.find("", 12) == FixedString<32>::npos);

    static_assert(testString.find(FixedString<16>("")) == 0);
    static_assert(testString.find(FixedString<16>()) == 0);
    static_assert(testString.find(CStringView("")) == 0);
    static_assert(testString.find("") == 0);
    static_assert(testString.find("", 5) == 5);
    static_assert(testString.find("", 11) == 11);
    static_assert(testString.find("", 12) == FixedString<32>::npos);
  }

  SUBCASE("find_in_empty_string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.find(FixedString<16>("Hello")) == FixedString<32>::npos);
    REQUIRE(testString.find(std::string("Hello")) == FixedString<32>::npos);
    REQUIRE(testString.find("Hello") == FixedString<32>::npos);
    REQUIRE(testString.find('H') == FixedString<32>::npos);
    REQUIRE(testString.find("") == 0);

    static_assert(testString.find(FixedString<16>("Hello")) == FixedString<32>::npos);
    static_assert(testString.find(CStringView("Hello")) == FixedString<32>::npos);
    static_assert(testString.find("Hello") == FixedString<32>::npos);
    static_assert(testString.find('H') == FixedString<32>::npos);
    static_assert(testString.find("") == 0);
  }

  SUBCASE("find_with_position_beyond_string_size") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find("World", 10) == FixedString<32>::npos);
    REQUIRE(testString.find('H', 10) == FixedString<32>::npos);
    REQUIRE(testString.find("", 10) == FixedString<32>::npos);

    static_assert(testString.find("World", 10) == FixedString<32>::npos);
    static_assert(testString.find('H', 10) == FixedString<32>::npos);
    static_assert(testString.find("", 10) == FixedString<32>::npos);
  }

  SUBCASE("find_substring_at_end") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("d") == 10);
    REQUIRE(testString.find("ld") == 9);

    static_assert(testString.find("World") == 6);
    static_assert(testString.find("d") == 10);
    static_assert(testString.find("ld") == 9);
  }

  SUBCASE("find_substring_at_beginning") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("H") == 0);
    REQUIRE(testString.find("He") == 0);

    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find("H") == 0);
    static_assert(testString.find("He") == 0);
  }

  SUBCASE("find_overlapping_substrings") {
    constexpr FixedString<32> testString("ababab");

    REQUIRE(testString.find("ab") == 0);
    REQUIRE(testString.find("ab", 1) == 2);
    REQUIRE(testString.find("ab", 3) == 4);
    REQUIRE(testString.find("ab", 5) == FixedString<32>::npos);

    static_assert(testString.find("ab") == 0);
    static_assert(testString.find("ab", 1) == 2);
    static_assert(testString.find("ab", 3) == 4);
    static_assert(testString.find("ab", 5) == FixedString<32>::npos);
  }

  SUBCASE("find_with_repeated_characters") {
    constexpr FixedString<32> testString("aaaaa");

    REQUIRE(testString.find("aa") == 0);
    REQUIRE(testString.find("aa", 1) == 1);
    REQUIRE(testString.find("aa", 2) == 2);
    REQUIRE(testString.find("aa", 3) == 3);
    REQUIRE(testString.find("aa", 4) == FixedString<32>::npos);

    static_assert(testString.find("aa") == 0);
    static_assert(testString.find("aa", 1) == 1);
    static_assert(testString.find("aa", 2) == 2);
    static_assert(testString.find("aa", 3) == 3);
    static_assert(testString.find("aa", 4) == FixedString<32>::npos);
  }

  SUBCASE("find_case_sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("hello") == FixedString<32>::npos);
    REQUIRE(testString.find("WORLD") == FixedString<32>::npos);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("World") == 6);

    static_assert(testString.find("hello") == FixedString<32>::npos);
    static_assert(testString.find("WORLD") == FixedString<32>::npos);
    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find("World") == 6);
  }

  SUBCASE("find_with_different_fixed_string_capacities") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find(FixedString<8>("World")) == 6);
    REQUIRE(testString.find(FixedString<16>("World")) == 6);
    REQUIRE(testString.find(FixedString<64>("World")) == 6);

    static_assert(testString.find(FixedString<8>("World")) == 6);
    static_assert(testString.find(FixedString<16>("World")) == 6);
    static_assert(testString.find(FixedString<64>("World")) == 6);
  }

  SUBCASE("find_with_exact_match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == FixedString<32>::npos);

    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find("Hello", 0) == 0);
    static_assert(testString.find("Hello", 1) == FixedString<32>::npos);
  }

  SUBCASE("find_with_single_character_string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.find("A") == 0);
    REQUIRE(testString.find('A') == 0);
    REQUIRE(testString.find("B") == FixedString<32>::npos);
    REQUIRE(testString.find('B') == FixedString<32>::npos);

    static_assert(testString.find("A") == 0);
    static_assert(testString.find('A') == 0);
    static_assert(testString.find("B") == FixedString<32>::npos);
    static_assert(testString.find('B') == FixedString<32>::npos);
  }

  SUBCASE("find_with_special_characters") {
    constexpr FixedString<32> testString("Hello\n\tWorld!");

    REQUIRE(testString.find("\n") == 5);
    REQUIRE(testString.find("\t") == 6);
    REQUIRE(testString.find("!") == 12);
    REQUIRE(testString.find("\n\t") == 5);

    static_assert(testString.find("\n") == 5);
    static_assert(testString.find("\t") == 6);
    static_assert(testString.find("!") == 12);
    static_assert(testString.find("\n\t") == 5);
  }

  SUBCASE("find_with_unicode_content") {
    constexpr FixedString<64> testString("Hello 世界");

    REQUIRE(testString.find("世界") == 6);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find(" ") == 5);

    static_assert(testString.find("世界") == 6);
    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find(" ") == 5);
  }

  SUBCASE("find_with_numeric_content") {
    constexpr FixedString<32> testString("12345Hello67890");

    REQUIRE(testString.find("12345") == 0);
    REQUIRE(testString.find("Hello") == 5);
    REQUIRE(testString.find("67890") == 10);
    REQUIRE(testString.find("456") == FixedString<32>::npos);

    static_assert(testString.find("12345") == 0);
    static_assert(testString.find("Hello") == 5);
    static_assert(testString.find("67890") == 10);
    static_assert(testString.find("456") == FixedString<32>::npos);
  }

  SUBCASE("find_with_mixed_content") {
    constexpr FixedString<32> testString("Hello123World!@#");

    REQUIRE(testString.find("123") == 5);
    REQUIRE(testString.find("!@#") == 13);
    REQUIRE(testString.find("Hello123") == 0);
    REQUIRE(testString.find("World!@#") == 8);

    static_assert(testString.find("123") == 5);
    static_assert(testString.find("!@#") == 13);
    static_assert(testString.find("Hello123") == 0);
    static_assert(testString.find("World!@#") == 8);
  }

  SUBCASE("find_with_position_edge_cases") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == FixedString<32>::npos);
    REQUIRE(testString.find("World", 6) == 6);
    REQUIRE(testString.find("World", 7) == FixedString<32>::npos);
    REQUIRE(testString.find("", 0) == 0);
    REQUIRE(testString.find("", 11) == 11);
    REQUIRE(testString.find("", 12) == FixedString<32>::npos);

    static_assert(testString.find("Hello", 0) == 0);
    static_assert(testString.find("Hello", 1) == FixedString<32>::npos);
    static_assert(testString.find("World", 6) == 6);
    static_assert(testString.find("World", 7) == FixedString<32>::npos);
    static_assert(testString.find("", 0) == 0);
    static_assert(testString.find("", 11) == 11);
    static_assert(testString.find("", 12) == FixedString<32>::npos);
  }
}

// rfind() from end.
TEST_CASE("core/fixed_string/rfind") {
  SUBCASE("Rfind FixedString substring") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind(FixedString<32>("Hello")) == 12);
    REQUIRE(testString.rfind(FixedString<32>("World")) == 6);
    REQUIRE(testString.rfind(FixedString<32>("lo")) == 15);
    REQUIRE(testString.rfind(FixedString<32>("xyz")) == FixedString<32>::npos);

    static_assert(testString.rfind(FixedString<32>("Hello")) == 12);
    static_assert(testString.rfind(FixedString<32>("World")) == 6);
    static_assert(testString.rfind(FixedString<32>("lo")) == 15);
    static_assert(testString.rfind(FixedString<32>("xyz")) == FixedString<32>::npos);
  }

  SUBCASE("Rfind StringLike substring") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind(std::string("Hello")) == 12);
    REQUIRE(testString.rfind(std::string("World")) == 6);
    REQUIRE(testString.rfind(std::string("lo")) == 15);
    REQUIRE(testString.rfind(std::string("xyz")) == FixedString<32>::npos);

    static_assert(testString.rfind(CStringView("Hello")) == 12);
    static_assert(testString.rfind(CStringView("World")) == 6);
    static_assert(testString.rfind(CStringView("lo")) == 15);
    static_assert(testString.rfind(CStringView("xyz")) == FixedString<32>::npos);
  }

  SUBCASE("Rfind C string substring") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("lo") == 15);
    REQUIRE(testString.rfind("xyz") == FixedString<32>::npos);

    static_assert(testString.rfind("Hello") == 12);
    static_assert(testString.rfind("World") == 6);
    static_assert(testString.rfind("lo") == 15);
    static_assert(testString.rfind("xyz") == FixedString<32>::npos);
  }

  SUBCASE("Rfind character") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind('H') == 12);
    REQUIRE(testString.rfind('l') == 15);
    REQUIRE(testString.rfind('o') == 16);
    REQUIRE(testString.rfind('W') == 6);
    REQUIRE(testString.rfind('d') == 10);
    REQUIRE(testString.rfind('x') == FixedString<32>::npos);

    static_assert(testString.rfind('H') == 12);
    static_assert(testString.rfind('l') == 15);
    static_assert(testString.rfind('o') == 16);
    static_assert(testString.rfind('W') == 6);
    static_assert(testString.rfind('d') == 10);
    static_assert(testString.rfind('x') == FixedString<32>::npos);
  }

  SUBCASE("Rfind with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello", 12) == 12);
    REQUIRE(testString.rfind("Hello", 11) == 0);
    REQUIRE(testString.rfind("Hello", 0) == 0);
    REQUIRE(testString.rfind('l', 16) == 15);
    REQUIRE(testString.rfind('l', 13) == 9);
    REQUIRE(testString.rfind('l', 8) == 3);
    REQUIRE(testString.rfind('l', 2) == 2);

    static_assert(testString.rfind("Hello", 12) == 12);
    static_assert(testString.rfind("Hello", 11) == 0);
    static_assert(testString.rfind("Hello", 0) == 0);
    static_assert(testString.rfind('l', 16) == 15);
    static_assert(testString.rfind('l', 13) == 9);
    static_assert(testString.rfind('l', 8) == 3);
    static_assert(testString.rfind('l', 2) == 2);
  }

  SUBCASE("Rfind empty substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.rfind(FixedString<16>("")) == 11);
    REQUIRE(testString.rfind(std::string("")) == 11);
    REQUIRE(testString.rfind("") == 11);
    REQUIRE(testString.rfind("", 5) == 5);
    REQUIRE(testString.rfind("", 0) == 0);

    static_assert(testString.rfind(FixedString<16>("")) == 11);
    static_assert(testString.rfind(CStringView("")) == 11);
    static_assert(testString.rfind("") == 11);
    static_assert(testString.rfind("", 5) == 5);
    static_assert(testString.rfind("", 0) == 0);
  }

  SUBCASE("Rfind in empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.rfind(FixedString<16>("Hello")) == FixedString<32>::npos);
    REQUIRE(testString.rfind(std::string("Hello")) == FixedString<32>::npos);
    REQUIRE(testString.rfind("Hello") == FixedString<32>::npos);
    REQUIRE(testString.rfind('H') == FixedString<32>::npos);
    REQUIRE(testString.rfind("") == 0);

    static_assert(testString.rfind(FixedString<16>("Hello")) == FixedString<32>::npos);
    static_assert(testString.rfind(CStringView("Hello")) == FixedString<32>::npos);
    static_assert(testString.rfind("Hello") == FixedString<32>::npos);
    static_assert(testString.rfind('H') == FixedString<32>::npos);
    static_assert(testString.rfind("") == 0);
  }

  SUBCASE("Rfind substring at end") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("d") == 10);
    REQUIRE(testString.rfind("ld") == 9);

    static_assert(testString.rfind("World") == 6);
    static_assert(testString.rfind("d") == 10);
    static_assert(testString.rfind("ld") == 9);
  }

  SUBCASE("Rfind substring at beginning") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("H") == 12);
    REQUIRE(testString.rfind("He") == 12);

    static_assert(testString.rfind("Hello") == 12);
    static_assert(testString.rfind("H") == 12);
    static_assert(testString.rfind("He") == 12);
  }

  SUBCASE("Rfind overlapping substrings") {
    constexpr FixedString<32> testString("ababab");

    REQUIRE(testString.rfind("ab") == 4);
    REQUIRE(testString.rfind("ab", 3) == 2);
    REQUIRE(testString.rfind("ab", 1) == 0);
    REQUIRE(testString.rfind("ab", 0) == 0);
    REQUIRE(testString.rfind("ab", 5) == FixedString<32>::npos);

    static_assert(testString.rfind("ab") == 4);
    static_assert(testString.rfind("ab", 3) == 2);
    static_assert(testString.rfind("ab", 1) == 0);
    static_assert(testString.rfind("ab", 0) == 0);
    static_assert(testString.rfind("ab", 5) == FixedString<32>::npos);
  }

  SUBCASE("Rfind with repeated characters") {
    constexpr FixedString<32> testString("aaaaa");

    REQUIRE(testString.rfind("aa") == 3);
    REQUIRE(testString.rfind("aa", 2) == 2);
    REQUIRE(testString.rfind("aa", 1) == 1);
    REQUIRE(testString.rfind("aa", 0) == 0);

    static_assert(testString.rfind("aa") == 3);
    static_assert(testString.rfind("aa", 2) == 2);
    static_assert(testString.rfind("aa", 1) == 1);
    static_assert(testString.rfind("aa", 0) == 0);
  }

  SUBCASE("Rfind case sensitivity") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("hello") == FixedString<32>::npos);
    REQUIRE(testString.rfind("WORLD") == FixedString<32>::npos);
    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);

    static_assert(testString.rfind("hello") == FixedString<32>::npos);
    static_assert(testString.rfind("WORLD") == FixedString<32>::npos);
    static_assert(testString.rfind("Hello") == 12);
    static_assert(testString.rfind("World") == 6);
  }

  SUBCASE("Rfind with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind(FixedString<8>("Hello")) == 12);
    REQUIRE(testString.rfind(FixedString<16>("Hello")) == 12);
    REQUIRE(testString.rfind(FixedString<64>("Hello")) == 12);

    static_assert(testString.rfind(FixedString<8>("Hello")) == 12);
    static_assert(testString.rfind(FixedString<16>("Hello")) == 12);
    static_assert(testString.rfind(FixedString<64>("Hello")) == 12);
  }

  SUBCASE("Rfind with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.rfind("Hello") == 0);
    REQUIRE(testString.rfind("Hello", 0) == 0);

    static_assert(testString.rfind("Hello") == 0);
    static_assert(testString.rfind("Hello", 0) == 0);
  }

  SUBCASE("Rfind with single character string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.rfind("A") == 0);
    REQUIRE(testString.rfind('A') == 0);
    REQUIRE(testString.rfind("B") == FixedString<32>::npos);
    REQUIRE(testString.rfind('B') == FixedString<32>::npos);

    static_assert(testString.rfind("A") == 0);
    static_assert(testString.rfind('A') == 0);
    static_assert(testString.rfind("B") == FixedString<32>::npos);
    static_assert(testString.rfind('B') == FixedString<32>::npos);
  }

  SUBCASE("Rfind with position 0") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.rfind("Hello", 0) == 0);
    REQUIRE(testString.rfind("World", 0) == FixedString<32>::npos);
    REQUIRE(testString.rfind('H', 0) == 0);
    REQUIRE(testString.rfind('W', 0) == FixedString<32>::npos);

    static_assert(testString.rfind("Hello", 0) == 0);
    static_assert(testString.rfind("World", 0) == FixedString<32>::npos);
    static_assert(testString.rfind('H', 0) == 0);
    static_assert(testString.rfind('W', 0) == FixedString<32>::npos);
  }

  SUBCASE("Rfind with substring longer than string") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.rfind("Hello World") == FixedString<32>::npos);
    REQUIRE(testString.rfind("Hello World", 10) == FixedString<32>::npos);

    static_assert(testString.rfind("Hello World") == FixedString<32>::npos);
    static_assert(testString.rfind("Hello World", 10) == FixedString<32>::npos);
  }

  SUBCASE("Rfind with multiple occurrences") {
    constexpr FixedString<32> testString("abababab");

    REQUIRE(testString.rfind("ab") == 6);
    REQUIRE(testString.rfind("ab", 5) == 4);
    REQUIRE(testString.rfind("ab", 3) == 2);
    REQUIRE(testString.rfind("ab", 1) == 0);

    static_assert(testString.rfind("ab") == 6);
    static_assert(testString.rfind("ab", 5) == 4);
    static_assert(testString.rfind("ab", 3) == 2);
    static_assert(testString.rfind("ab", 1) == 0);
  }

  SUBCASE("Rfind with position in middle") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello", 8) == 0);
    REQUIRE(testString.rfind("Hello", 12) == 12);
    REQUIRE(testString.rfind('l', 8) == 3);
    REQUIRE(testString.rfind('l', 15) == 15);

    static_assert(testString.rfind("Hello", 8) == 0);
    static_assert(testString.rfind("Hello", 12) == 12);
    static_assert(testString.rfind('l', 8) == 3);
    static_assert(testString.rfind('l', 15) == 15);
  }
}

// find_first_of() character set.
TEST_CASE("core/fixed_string/find_first_of") {
  SUBCASE("Find first of FixedString characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(FixedString<32>("aeiou")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_of(FixedString<32>("H")) == 0);
    REQUIRE(testString.find_first_of(FixedString<32>("d")) == 10);
    REQUIRE(testString.find_first_of(FixedString<32>("xyz")) == FixedString<32>::npos);

    static_assert(testString.find_first_of(FixedString<32>("aeiou")) == 1);
    static_assert(testString.find_first_of(FixedString<32>("H")) == 0);
    static_assert(testString.find_first_of(FixedString<32>("d")) == 10);
    static_assert(testString.find_first_of(FixedString<32>("xyz")) == FixedString<32>::npos);
  }

  SUBCASE("Find first of StringLike characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(std::string("aeiou")) == 1);
    REQUIRE(testString.find_first_of(std::string("H")) == 0);
    REQUIRE(testString.find_first_of(std::string("d")) == 10);
    REQUIRE(testString.find_first_of(std::string("xyz")) == FixedString<32>::npos);

    static_assert(testString.find_first_of(CStringView("aeiou")) == 1);
    static_assert(testString.find_first_of(CStringView("H")) == 0);
    static_assert(testString.find_first_of(CStringView("d")) == 10);
    static_assert(testString.find_first_of(CStringView("xyz")) == FixedString<32>::npos);
  }

  SUBCASE("Find first of C string characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou") == 1);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("d") == 10);
    REQUIRE(testString.find_first_of("xyz") == FixedString<32>::npos);

    static_assert(testString.find_first_of("aeiou") == 1);
    static_assert(testString.find_first_of("H") == 0);
    static_assert(testString.find_first_of("d") == 10);
    static_assert(testString.find_first_of("xyz") == FixedString<32>::npos);
  }

  SUBCASE("Find first of single character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of('H') == 0);
    REQUIRE(testString.find_first_of('e') == 1);
    REQUIRE(testString.find_first_of('l') == 2);
    REQUIRE(testString.find_first_of('o') == 4);
    REQUIRE(testString.find_first_of('W') == 6);
    REQUIRE(testString.find_first_of('d') == 10);
    REQUIRE(testString.find_first_of('x') == FixedString<32>::npos);

    static_assert(testString.find_first_of('H') == 0);
    static_assert(testString.find_first_of('e') == 1);
    static_assert(testString.find_first_of('l') == 2);
    static_assert(testString.find_first_of('o') == 4);
    static_assert(testString.find_first_of('W') == 6);
    static_assert(testString.find_first_of('d') == 10);
    static_assert(testString.find_first_of('x') == FixedString<32>::npos);
  }

  SUBCASE("Find first of with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("aeiou", 2) == 4);
    REQUIRE(testString.find_first_of("aeiou", 5) == 7);
    REQUIRE(testString.find_first_of("aeiou", 8) == 13);
    REQUIRE(testString.find_first_of("aeiou", 14) == 16);
    REQUIRE(testString.find_first_of("aeiou", 17) == FixedString<32>::npos);

    static_assert(testString.find_first_of("aeiou", 0) == 1);
    static_assert(testString.find_first_of("aeiou", 2) == 4);
    static_assert(testString.find_first_of("aeiou", 5) == 7);
    static_assert(testString.find_first_of("aeiou", 8) == 13);
    static_assert(testString.find_first_of("aeiou", 14) == 16);
    static_assert(testString.find_first_of("aeiou", 17) == FixedString<32>::npos);
  }

  SUBCASE("Find first of empty character set") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(FixedString<16>("")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of(std::string("")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("") == FixedString<32>::npos);

    static_assert(testString.find_first_of(FixedString<16>("")) == FixedString<32>::npos);
    static_assert(testString.find_first_of(CStringView("")) == FixedString<32>::npos);
    static_assert(testString.find_first_of("") == FixedString<32>::npos);
  }

  SUBCASE("Find first of in empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.find_first_of(FixedString<16>("aeiou")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of(std::string("aeiou")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("aeiou") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of('a') == FixedString<32>::npos);

    static_assert(testString.find_first_of(FixedString<16>("aeiou")) == FixedString<32>::npos);
    static_assert(testString.find_first_of(CStringView("aeiou")) == FixedString<32>::npos);
    static_assert(testString.find_first_of("aeiou") == FixedString<32>::npos);
    static_assert(testString.find_first_of('a') == FixedString<32>::npos);
  }

  SUBCASE("Find first of with position beyond string size") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_first_of("aeiou", 10) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of('a', 10) == FixedString<32>::npos);

    static_assert(testString.find_first_of("aeiou", 10) == FixedString<32>::npos);
    static_assert(testString.find_first_of('a', 10) == FixedString<32>::npos);
  }

  SUBCASE("Find first of with repeated characters") {
    constexpr FixedString<32> testString("aaaaa");

    REQUIRE(testString.find_first_of("a") == 0);
    REQUIRE(testString.find_first_of("ab") == 0);
    REQUIRE(testString.find_first_of("b") == FixedString<32>::npos);

    static_assert(testString.find_first_of("a") == 0);
    static_assert(testString.find_first_of("ab") == 0);
    static_assert(testString.find_first_of("b") == FixedString<32>::npos);
  }

  SUBCASE("Find first of with multiple character sets") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("Hl") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_of("lo") == 2); // 'l' at position 2
    REQUIRE(testString.find_first_of("Wr") == 6); // 'W' at position 6
    REQUIRE(testString.find_first_of("dl") == 2); // 'l' at position 2

    static_assert(testString.find_first_of("Hl") == 0);
    static_assert(testString.find_first_of("lo") == 2);
    static_assert(testString.find_first_of("Wr") == 6);
    static_assert(testString.find_first_of("dl") == 2);
  }

  SUBCASE("Find first of case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("h") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("w") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("W") == 6);

    static_assert(testString.find_first_of("h") == FixedString<32>::npos);
    static_assert(testString.find_first_of("H") == 0);
    static_assert(testString.find_first_of("w") == FixedString<32>::npos);
    static_assert(testString.find_first_of("W") == 6);
  }

  SUBCASE("Find first of with special characters") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.find_first_of("!,") == 5); // ',' at position 5
    REQUIRE(testString.find_first_of("!") == 12); // '!' at position 12
    REQUIRE(testString.find_first_of(".,!") == 5); // ',' at position 5

    static_assert(testString.find_first_of("!,") == 5);
    static_assert(testString.find_first_of("!") == 12);
    static_assert(testString.find_first_of(".,!") == 5);
  }

  SUBCASE("Find first of with numbers") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_first_of("0123456789") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("123") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("456") == FixedString<32>::npos);

    static_assert(testString.find_first_of("0123456789") == 5);
    static_assert(testString.find_first_of("123") == 5);
    static_assert(testString.find_first_of("456") == FixedString<32>::npos);
  }

  SUBCASE("Find first of with whitespace") {
    constexpr FixedString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_first_of(" \t\n") == 5); // ' ' at position 5
    REQUIRE(testString.find_first_of("\t") == 11);
    REQUIRE(testString.find_first_of("\n") == 12);

    static_assert(testString.find_first_of(" \t\n") == 5);
    static_assert(testString.find_first_of("\t") == 11);
    static_assert(testString.find_first_of("\n") == 12);
  }

  SUBCASE("Find first of with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(FixedString<8>("aeiou")) == 1);
    REQUIRE(testString.find_first_of(FixedString<16>("aeiou")) == 1);
    REQUIRE(testString.find_first_of(FixedString<64>("aeiou")) == 1);

    static_assert(testString.find_first_of(FixedString<8>("aeiou")) == 1);
    static_assert(testString.find_first_of(FixedString<16>("aeiou")) == 1);
    static_assert(testString.find_first_of(FixedString<64>("aeiou")) == 1);
  }

  SUBCASE("Find first of with single character string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.find_first_of("A") == 0);
    REQUIRE(testString.find_first_of('A') == 0);
    REQUIRE(testString.find_first_of("B") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of('B') == FixedString<32>::npos);

    static_assert(testString.find_first_of("A") == 0);
    static_assert(testString.find_first_of('A') == 0);
    static_assert(testString.find_first_of("B") == FixedString<32>::npos);
    static_assert(testString.find_first_of('B') == FixedString<32>::npos);
  }

  SUBCASE("Find first of with position 0") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("H", 0) == 0);
    REQUIRE(testString.find_first_of("xyz", 0) == FixedString<32>::npos);

    static_assert(testString.find_first_of("aeiou", 0) == 1);
    static_assert(testString.find_first_of("H", 0) == 0);
    static_assert(testString.find_first_of("xyz", 0) == FixedString<32>::npos);
  }

  SUBCASE("Find first of with all characters found") {
    constexpr FixedString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_of("aeiou") == 0);
    REQUIRE(testString.find_first_of("xyz") == 23);
    REQUIRE(testString.find_first_of("z") == 25);

    static_assert(testString.find_first_of("aeiou") == 0);
    static_assert(testString.find_first_of("xyz") == 23);
    static_assert(testString.find_first_of("z") == 25);
  }

  SUBCASE("Find first of with no characters found") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("0123456789") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("!@#$%^&*()") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("[]{}|\\:;\"'<>?/") == FixedString<32>::npos);

    static_assert(testString.find_first_of("0123456789") == FixedString<32>::npos);
    static_assert(testString.find_first_of("!@#$%^&*()") == FixedString<32>::npos);
    static_assert(testString.find_first_of("[]{}|\\:;\"'<>?/") == FixedString<32>::npos);
  }
}

// find_first_not_of() character set.
TEST_CASE("core/fixed_string/find_first_not_of") {
  SUBCASE("Find first not of FixedString characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(FixedString<32>("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(FixedString<32>("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(FixedString<32>("Helo Wrd")) == FixedString<32>::npos);

    static_assert(testString.find_first_not_of(FixedString<32>("H")) == 1);
    static_assert(testString.find_first_not_of(FixedString<32>("Hel")) == 4);
    static_assert(testString.find_first_not_of(FixedString<32>("Helo Wrd")) == FixedString<32>::npos);
  }

  SUBCASE("Find first not of StringLike characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(std::string("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(std::string("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(std::string("Helo Wrd")) == FixedString<32>::npos);

    static_assert(testString.find_first_not_of(CStringView("H")) == 1);
    static_assert(testString.find_first_not_of(CStringView("Hel")) == 4);
    static_assert(testString.find_first_not_of(CStringView("Helo Wrd")) == FixedString<32>::npos);
  }

  SUBCASE("Find first not of C string characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("H") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Helo Wrd") == FixedString<32>::npos);

    static_assert(testString.find_first_not_of("H") == 1);
    static_assert(testString.find_first_not_of("Hel") == 4);
    static_assert(testString.find_first_not_of("Helo Wrd") == FixedString<32>::npos);
  }

  SUBCASE("Find first not of single character") {
    constexpr FixedString<32> testString("aaaaab");

    REQUIRE(testString.find_first_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_first_not_of('b') == 0); // 'a' at position 0
    REQUIRE(testString.find_first_not_of('x') == 0); // 'a' at position 0

    static_assert(testString.find_first_not_of('a') == 5);
    static_assert(testString.find_first_not_of('b') == 0);
    static_assert(testString.find_first_not_of('x') == 0);
  }

  SUBCASE("Find first not of with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_first_not_of("Hel", 0) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 5) == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Hel", 6) == 6); // 'W' at position 6
    REQUIRE(testString.find_first_not_of("Hel", 7) == 7); // 'o' at position 7

    static_assert(testString.find_first_not_of("Hel", 0) == 4);
    static_assert(testString.find_first_not_of("Hel", 4) == 4);
    static_assert(testString.find_first_not_of("Hel", 5) == 5);
    static_assert(testString.find_first_not_of("Hel", 6) == 6);
    static_assert(testString.find_first_not_of("Hel", 7) == 7);
  }

  SUBCASE("Find first not of empty character set") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(FixedString<16>("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of(std::string("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("", 5) == 5); // ' ' at position 5

    static_assert(testString.find_first_not_of(FixedString<16>("")) == 0);
    static_assert(testString.find_first_not_of(CStringView("")) == 0);
    static_assert(testString.find_first_not_of("") == 0);
    static_assert(testString.find_first_not_of("", 5) == 5);
  }

  SUBCASE("Find first not of in empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.find_first_not_of(FixedString<16>("aeiou")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of(std::string("aeiou")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("aeiou") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of('a') == FixedString<32>::npos);

    static_assert(testString.find_first_not_of(FixedString<16>("aeiou")) == FixedString<32>::npos);
    static_assert(testString.find_first_not_of(CStringView("aeiou")) == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("aeiou") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of('a') == FixedString<32>::npos);
  }

  SUBCASE("Find first not of with position beyond string size") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_first_not_of("aeiou", 10) == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of('a', 10) == FixedString<32>::npos);

    static_assert(testString.find_first_not_of("aeiou", 10) == FixedString<32>::npos);
    static_assert(testString.find_first_not_of('a', 10) == FixedString<32>::npos);
  }

  SUBCASE("Find first not of with repeated characters") {
    constexpr FixedString<32> testString("aaaaa");

    REQUIRE(testString.find_first_not_of("a") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("ab") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("b") == 0); // 'a' at position 0

    static_assert(testString.find_first_not_of("a") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("ab") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("b") == 0);
  }

  SUBCASE("Find first not of with multiple character sets") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("Hl") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Helo") == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Helo ") == 6); // 'W' at position 6

    static_assert(testString.find_first_not_of("Hl") == 1);
    static_assert(testString.find_first_not_of("Hel") == 4);
    static_assert(testString.find_first_not_of("Helo") == 5);
    static_assert(testString.find_first_not_of("Helo ") == 6);
  }

  SUBCASE("Find first not of case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("h") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("H") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("w") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("W") == 0); // 'H' at position 0

    static_assert(testString.find_first_not_of("h") == 0);
    static_assert(testString.find_first_not_of("H") == 1);
    static_assert(testString.find_first_not_of("w") == 0);
    static_assert(testString.find_first_not_of("W") == 0);
  }

  SUBCASE("Find first not of with special characters") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.find_first_not_of("Helo, Wrd!") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo, Wrd") == 12); // '!' at position 12

    static_assert(testString.find_first_not_of("Helo, Wrd!") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("Helo, Wrd") == 12);
  }

  SUBCASE("Find first not of with numbers") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo123Wrd") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12

    static_assert(testString.find_first_not_of("0123456789") == 0);
    static_assert(testString.find_first_not_of("Helo123Wrd") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("Helo123Wr") == 12);
  }

  SUBCASE("Find first not of with whitespace") {
    constexpr FixedString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_first_not_of(" \t\n") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo Wrd\t\n") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo Wrd") == 11); // '\t' at position 11

    static_assert(testString.find_first_not_of(" \t\n") == 0);
    static_assert(testString.find_first_not_of("Helo Wrd\t\n") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("Helo Wrd") == 11);
  }

  SUBCASE("Find first not of with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(FixedString<8>("H")) == 1);
    REQUIRE(testString.find_first_not_of(FixedString<16>("H")) == 1);
    REQUIRE(testString.find_first_not_of(FixedString<64>("H")) == 1);

    static_assert(testString.find_first_not_of(FixedString<8>("H")) == 1);
    static_assert(testString.find_first_not_of(FixedString<16>("H")) == 1);
    static_assert(testString.find_first_not_of(FixedString<64>("H")) == 1);
  }

  SUBCASE("Find first not of with single character string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.find_first_not_of("A") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of('A') == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("B") == 0); // 'A' at position 0
    REQUIRE(testString.find_first_not_of('B') == 0); // 'A' at position 0

    static_assert(testString.find_first_not_of("A") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of('A') == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("B") == 0);
    static_assert(testString.find_first_not_of('B') == 0);
  }

  SUBCASE("Find first not of with position 0") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("H", 0) == 1);
    REQUIRE(testString.find_first_not_of("Hel", 0) == 4);
    REQUIRE(testString.find_first_not_of("Helo Wrd", 0) == FixedString<32>::npos);

    static_assert(testString.find_first_not_of("H", 0) == 1);
    static_assert(testString.find_first_not_of("Hel", 0) == 4);
    static_assert(testString.find_first_not_of("Helo Wrd", 0) == FixedString<32>::npos);
  }

  SUBCASE("Find first not of with all characters excluded") {
    constexpr FixedString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24); // 'y' at position 24

    static_assert(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25);
    static_assert(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24);
  }

  SUBCASE("Find first not of with no characters excluded") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("xyz") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("!@#$%^&*()") == 0); // 'H' at position 0

    static_assert(testString.find_first_not_of("xyz") == 0);
    static_assert(testString.find_first_not_of("0123456789") == 0);
    static_assert(testString.find_first_not_of("!@#$%^&*()") == 0);
  }

  SUBCASE("Find first not of with mixed content") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_first_not_of("Helo123Wrd") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12
    REQUIRE(testString.find_first_not_of("Helo123Wd") == 10); // 'r' at position 10

    static_assert(testString.find_first_not_of("Helo123Wrd") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("Helo123Wr") == 12);
    static_assert(testString.find_first_not_of("Helo123Wd") == 10);
  }

  SUBCASE("Find first not of with position in middle") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_first_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 5) == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Hel", 6) == 6); // 'W' at position 6
    REQUIRE(testString.find_first_not_of("Hel", 7) == 7); // 'o' at position 7

    static_assert(testString.find_first_not_of("Hel", 4) == 4);
    static_assert(testString.find_first_not_of("Hel", 5) == 5);
    static_assert(testString.find_first_not_of("Hel", 6) == 6);
    static_assert(testString.find_first_not_of("Hel", 7) == 7);
  }

  SUBCASE("Find first not of with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_first_not_of("Hello") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4

    static_assert(testString.find_first_not_of("Hello") == FixedString<32>::npos);
    static_assert(testString.find_first_not_of("Hell") == 4);
    static_assert(testString.find_first_not_of("Hel") == 4);
  }
}

// find_last_of() character set.
TEST_CASE("core/fixed_string/find_last_of") {
  SUBCASE("Find last of FixedString characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of(FixedString<32>("aeiou")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of(FixedString<32>("l")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of(FixedString<32>("H")) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of(FixedString<32>("d")) == 10); // 'd' at position 10

    static_assert(testString.find_last_of(FixedString<32>("aeiou")) == 7);
    static_assert(testString.find_last_of(FixedString<32>("l")) == 9);
    static_assert(testString.find_last_of(FixedString<32>("H")) == 0);
    static_assert(testString.find_last_of(FixedString<32>("d")) == 10);
  }

  SUBCASE("Find last of StringLike characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of(std::string("aeiou")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of(std::string("l")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of(std::string("H")) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of(std::string("d")) == 10); // 'd' at position 10

    static_assert(testString.find_last_of(CStringView("aeiou")) == 7);
    static_assert(testString.find_last_of(CStringView("l")) == 9);
    static_assert(testString.find_last_of(CStringView("H")) == 0);
    static_assert(testString.find_last_of(CStringView("d")) == 10);
  }

  SUBCASE("Find last of C string characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("aeiou") == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of("l") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("H") == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("d") == 10); // 'd' at position 10

    static_assert(testString.find_last_of("aeiou") == 7);
    static_assert(testString.find_last_of("l") == 9);
    static_assert(testString.find_last_of("H") == 0);
    static_assert(testString.find_last_of("d") == 10);
  }

  SUBCASE("Find last of single character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of('l') == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of('o') == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of('H') == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of('d') == 10); // 'd' at position 10

    static_assert(testString.find_last_of('l') == 9);
    static_assert(testString.find_last_of('o') == 7);
    static_assert(testString.find_last_of('H') == 0);
    static_assert(testString.find_last_of('d') == 10);
  }

  SUBCASE("Find last of with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    REQUIRE(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1
    REQUIRE(testString.find_last_of("Hel", 0) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("Hel", 17) == FixedString<32>::npos);

    static_assert(testString.find_last_of("Hel", 8) == 3);
    static_assert(testString.find_last_of("Hel", 4) == 3);
    static_assert(testString.find_last_of("Hel", 2) == 2);
    static_assert(testString.find_last_of("Hel", 1) == 1);
    static_assert(testString.find_last_of("Hel", 0) == 0);
    static_assert(testString.find_last_of("Hel", 17) == FixedString<32>::npos);
  }

  SUBCASE("Find last of empty character set") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of(FixedString<16>("")) == FixedString<32>::npos);
    REQUIRE(testString.find_last_of(std::string("")) == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("") == FixedString<32>::npos);

    static_assert(testString.find_last_of(FixedString<16>("")) == FixedString<32>::npos);
    static_assert(testString.find_last_of(CStringView("")) == FixedString<32>::npos);
    static_assert(testString.find_last_of("") == FixedString<32>::npos);
  }

  SUBCASE("Find last of with no characters found") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("0123456789") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("!@#$%^&*()") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("[]{}|\\:;\"'<>?/") == FixedString<32>::npos);

    static_assert(testString.find_last_of("0123456789") == FixedString<32>::npos);
    static_assert(testString.find_last_of("!@#$%^&*()") == FixedString<32>::npos);
    static_assert(testString.find_last_of("[]{}|\\:;\"'<>?/") == FixedString<32>::npos);
  }

  SUBCASE("Find last of with multiple character sets") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("Hl") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Hel") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo ") == 9); // 'l' at position 9

    static_assert(testString.find_last_of("Hl") == 9);
    static_assert(testString.find_last_of("Hel") == 9);
    static_assert(testString.find_last_of("Helo") == 9);
    static_assert(testString.find_last_of("Helo ") == 9);
  }

  SUBCASE("Find last of case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("h") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("H") == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("w") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("W") == 6); // 'W' at position 6

    static_assert(testString.find_last_of("h") == FixedString<32>::npos);
    static_assert(testString.find_last_of("H") == 0);
    static_assert(testString.find_last_of("w") == FixedString<32>::npos);
    static_assert(testString.find_last_of("W") == 6);
  }

  SUBCASE("Find last of with special characters") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.find_last_of("Helo, Wrd!") == 12); // '!' at position 12
    REQUIRE(testString.find_last_of("Helo, Wrd") == 11); // 'd' at position 11
    REQUIRE(testString.find_last_of("Helo, Wr") == 10); // 'l' at position 10

    static_assert(testString.find_last_of("Helo, Wrd!") == 12);
    static_assert(testString.find_last_of("Helo, Wrd") == 11);
    static_assert(testString.find_last_of("Helo, Wr") == 10);
  }

  SUBCASE("Find last of with numbers") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_last_of("0123456789") == 7); // '3' at position 7
    REQUIRE(testString.find_last_of("Helo123Wrd") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_of("Helo123Wr") == 11); // 'l' at position 11

    static_assert(testString.find_last_of("0123456789") == 7);
    static_assert(testString.find_last_of("Helo123Wrd") == 12);
    static_assert(testString.find_last_of("Helo123Wr") == 11);
  }

  SUBCASE("Find last of with whitespace") {
    constexpr FixedString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_last_of(" \t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd\t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10

    static_assert(testString.find_last_of(" \t\n") == 12);
    static_assert(testString.find_last_of("Helo Wrd\t\n") == 12);
    static_assert(testString.find_last_of("Helo Wrd") == 10);
  }

  SUBCASE("Find last of with repeated characters") {
    constexpr FixedString<32> testString("aaaaab");

    REQUIRE(testString.find_last_of('a') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_of('b') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_of('x') == FixedString<32>::npos);

    static_assert(testString.find_last_of('a') == 4);
    static_assert(testString.find_last_of('b') == 5);
    static_assert(testString.find_last_of('x') == FixedString<32>::npos);
  }

  SUBCASE("Find last of with single character string") {
    constexpr FixedString<32> testString("AAAAA");

    REQUIRE(testString.find_last_of("A") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of('A') == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of("B") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of('B') == FixedString<32>::npos);

    static_assert(testString.find_last_of("A") == 4);
    static_assert(testString.find_last_of('A') == 4);
    static_assert(testString.find_last_of("B") == FixedString<32>::npos);
    static_assert(testString.find_last_of('B') == FixedString<32>::npos);
  }

  SUBCASE("Find last of with alphabet") {
    constexpr FixedString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24); // 'y' at position 24
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23); // 'x' at position 23

    static_assert(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25);
    static_assert(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24);
    static_assert(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23);
  }

  SUBCASE("Find last of with all characters found") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_of("Helo Wr") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo W") == 9); // 'l' at position 9

    static_assert(testString.find_last_of("Helo Wrd") == 10);
    static_assert(testString.find_last_of("Helo Wr") == 9);
    static_assert(testString.find_last_of("Helo W") == 9);
  }

  SUBCASE("Find last of with position in middle") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    REQUIRE(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1

    static_assert(testString.find_last_of("Hel", 8) == 3);
    static_assert(testString.find_last_of("Hel", 4) == 3);
    static_assert(testString.find_last_of("Hel", 2) == 2);
    static_assert(testString.find_last_of("Hel", 1) == 1);
  }

  SUBCASE("Find last of with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_last_of("Hello") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_of("Hell") == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel") == 3); // 'l' at position 3

    static_assert(testString.find_last_of("Hello") == 4);
    static_assert(testString.find_last_of("Hell") == 3);
    static_assert(testString.find_last_of("Hel") == 3);
  }
}

// find_last_not_of() character set.
TEST_CASE("core/fixed_string/find_last_not_of") {
  SUBCASE("Find last not of FixedString characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of(FixedString<32>("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(FixedString<32>("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(FixedString<32>("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(FixedString<32>("World")) == 5); // ' ' at position 5

    static_assert(testString.find_last_not_of(FixedString<32>("d")) == 9);
    static_assert(testString.find_last_not_of(FixedString<32>("ld")) == 8);
    static_assert(testString.find_last_not_of(FixedString<32>("rld")) == 7);
    static_assert(testString.find_last_not_of(FixedString<32>("World")) == 5);
  }

  SUBCASE("Find last not of StringLike characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of(std::string("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(std::string("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(std::string("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(std::string("World")) == 5); // ' ' at position 5

    static_assert(testString.find_last_not_of(CStringView("d")) == 9);
    static_assert(testString.find_last_not_of(CStringView("ld")) == 8);
    static_assert(testString.find_last_not_of(CStringView("rld")) == 7);
    static_assert(testString.find_last_not_of(CStringView("World")) == 5);
  }

  SUBCASE("Find last not of C string characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("d") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of("ld") == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("rld") == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of("World") == 5); // ' ' at position 5

    static_assert(testString.find_last_not_of("d") == 9);
    static_assert(testString.find_last_not_of("ld") == 8);
    static_assert(testString.find_last_not_of("rld") == 7);
    static_assert(testString.find_last_not_of("World") == 5);
  }

  SUBCASE("Find last not of single character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of('d') == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of('l') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('o') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('H') == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of('d') == 9);
    static_assert(testString.find_last_not_of('l') == 10);
    static_assert(testString.find_last_not_of('o') == 10);
    static_assert(testString.find_last_not_of('H') == 10);
  }

  SUBCASE("Find last not of with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 0) == FixedString<32>::npos);

    static_assert(testString.find_last_not_of("Hel", 8) == 8);
    static_assert(testString.find_last_not_of("Hel", 4) == 4);
    static_assert(testString.find_last_not_of("Hel", 2) == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("Hel", 1) == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("Hel", 0) == FixedString<32>::npos);
  }

  SUBCASE("Find last not of empty character set") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of(FixedString<16>("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of(std::string("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("", 5) == 5); // ' ' at position 5

    static_assert(testString.find_last_not_of(FixedString<16>("")) == 10);
    static_assert(testString.find_last_not_of(CStringView("")) == 10);
    static_assert(testString.find_last_not_of("") == 10);
    static_assert(testString.find_last_not_of("", 5) == 5);
  }

  SUBCASE("Find last not of with all characters excluded") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("Helo Wrd") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo Wr") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo W") == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of("Helo Wrd") == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("Helo Wr") == 10);
    static_assert(testString.find_last_not_of("Helo W") == 10);
  }

  SUBCASE("Find last not of with multiple character sets") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("Hl") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Hel") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo ") == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of("Hl") == 10);
    static_assert(testString.find_last_not_of("Hel") == 10);
    static_assert(testString.find_last_not_of("Helo") == 10);
    static_assert(testString.find_last_not_of("Helo ") == 10);
  }

  SUBCASE("Find last not of case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("h") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("H") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("w") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("W") == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of("h") == 10);
    static_assert(testString.find_last_not_of("H") == 10);
    static_assert(testString.find_last_not_of("w") == 10);
    static_assert(testString.find_last_not_of("W") == 10);
  }

  SUBCASE("Find last not of with special characters") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.find_last_not_of("Helo, Wrd!") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo, Wrd") == 12); // '!' at position 12
    REQUIRE(testString.find_last_not_of("Helo, Wr") == 12); // '!' at position 12

    static_assert(testString.find_last_not_of("Helo, Wrd!") == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("Helo, Wrd") == 12);
    static_assert(testString.find_last_not_of("Helo, Wr") == 12);
  }

  SUBCASE("Find last not of with numbers") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_last_not_of("0123456789") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_not_of("Helo123Wrd") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo123Wr") == 12); // 'd' at position 12

    static_assert(testString.find_last_not_of("0123456789") == 12);
    static_assert(testString.find_last_not_of("Helo123Wrd") == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("Helo123Wr") == 12);
  }

  SUBCASE("Find last not of with whitespace") {
    constexpr FixedString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_last_not_of(" \t\n") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo Wrd\t\n") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo Wrd") == 12); // '\n' at position 12

    static_assert(testString.find_last_not_of(" \t\n") == 10);
    static_assert(testString.find_last_not_of("Helo Wrd\t\n") == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("Helo Wrd") == 12);
  }

  SUBCASE("Find last not of with repeated characters") {
    constexpr FixedString<32> testString("aaaaab");

    REQUIRE(testString.find_last_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_not_of('b') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_not_of('x') == 5); // 'b' at position 5

    static_assert(testString.find_last_not_of('a') == 5);
    static_assert(testString.find_last_not_of('b') == 4);
    static_assert(testString.find_last_not_of('x') == 5);
  }

  SUBCASE("Find last not of with single character string") {
    constexpr FixedString<32> testString("AAAAA");

    REQUIRE(testString.find_last_not_of("A") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of('A') == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("B") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_not_of('B') == 4); // 'A' at position 4

    static_assert(testString.find_last_not_of("A") == FixedString<32>::npos);
    static_assert(testString.find_last_not_of('A') == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("B") == 4);
    static_assert(testString.find_last_not_of('B') == 4);
  }

  SUBCASE("Find last not of with alphabet") {
    constexpr FixedString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxyz") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwx") == 25); // 'z' at position 25

    static_assert(testString.find_last_not_of("abcdefghijklmnopqrstuvwxyz") == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("abcdefghijklmnopqrstuvwxy") == 25);
    static_assert(testString.find_last_not_of("abcdefghijklmnopqrstuvwx") == 25);
  }

  SUBCASE("Find last not of with no characters excluded") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("xyz") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("0123456789") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("!@#$%^&*()") == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of("xyz") == 10);
    static_assert(testString.find_last_not_of("0123456789") == 10);
    static_assert(testString.find_last_not_of("!@#$%^&*()") == 10);
  }

  SUBCASE("Find last not of with position in middle") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 17) == FixedString<32>::npos);

    static_assert(testString.find_last_not_of("Hel", 8) == 8);
    static_assert(testString.find_last_not_of("Hel", 4) == 4);
    static_assert(testString.find_last_not_of("Hel", 2) == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("Hel", 1) == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("Hel", 17) == FixedString<32>::npos);
  }

  SUBCASE("Find last not of with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_last_not_of("Hello") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel") == 4); // 'o' at position 4

    static_assert(testString.find_last_not_of("Hello") == FixedString<32>::npos);
    static_assert(testString.find_last_not_of("Hell") == 4);
    static_assert(testString.find_last_not_of("Hel") == 4);
  }

  SUBCASE("Find last not of in empty string") {
    constexpr FixedString<4> testString("");

    REQUIRE(testString.find_last_not_of("Hello") == FixedString<4>::npos);

    static_assert(testString.find_last_not_of("Hello") == FixedString<4>::npos);
  }
}

// compare() three-way.
TEST_CASE("core/fixed_string/compare") {
  SUBCASE("Compare FixedString with FixedString") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hello");
    constexpr FixedString<32> testString3("World");
    constexpr FixedString<32> testString4("Hell");

    REQUIRE(testString1.compare(testString2) == 0);
    REQUIRE(testString1.compare(testString3) < 0);
    REQUIRE(testString1.compare(testString4) > 0);

    static_assert(testString1.compare(testString2) == 0);
    static_assert(testString1.compare(testString3) < 0);
    static_assert(testString1.compare(testString4) > 0);
  }

  SUBCASE("Compare FixedString with StringLike") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.compare(std::string("Hello")) == 0);
    REQUIRE(testString.compare(std::string("World")) < 0);
    REQUIRE(testString.compare(std::string("Hell")) > 0);

    static_assert(testString.compare(CStringView("Hello")) == 0);
    static_assert(testString.compare(CStringView("World")) < 0);
    static_assert(testString.compare(CStringView("Hell")) > 0);
  }

  SUBCASE("Compare FixedString with C string") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.compare("Hello") == 0);
    REQUIRE(testString.compare("World") < 0);
    REQUIRE(testString.compare("Hell") > 0);

    static_assert(testString.compare("Hello") == 0);
    static_assert(testString.compare("World") < 0);
    static_assert(testString.compare("Hell") > 0);
  }

  SUBCASE("Compare identical strings") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.compare(FixedString<32>("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare("Hello World") == 0);

    static_assert(testString.compare(FixedString<32>("Hello World")) == 0);
    static_assert(testString.compare(CStringView("Hello World")) == 0);
    static_assert(testString.compare("Hello World") == 0);
  }

  SUBCASE("Compare with empty strings") {
    constexpr FixedString<32> testString1("");
    constexpr FixedString<32> testString2("Hello");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("") == 0);
    REQUIRE(testString1.compare(std::string("")) == 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("") == 0);
    static_assert(testString1.compare(CStringView("")) == 0);
  }

  SUBCASE("Compare strings with different lengths") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hello World");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
  }

  SUBCASE("Compare strings with same prefix") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hell");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);

    static_assert(testString1.compare(testString2) > 0);
    static_assert(testString2.compare(testString1) < 0);
  }

  SUBCASE("Compare strings with different first character") {
    constexpr FixedString<32> testString1("Apple");
    constexpr FixedString<32> testString2("Banana");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
  }

  SUBCASE("Compare strings with different middle character") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hallo");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);

    static_assert(testString1.compare(testString2) > 0);
    static_assert(testString2.compare(testString1) < 0);
  }

  SUBCASE("Compare strings with different last character") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hellp");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
  }

  SUBCASE("Compare case sensitivity") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.compare("hello") < 0);
    REQUIRE(testString.compare("HELLO") > 0);
    REQUIRE(testString.compare("Hello") == 0);

    static_assert(testString.compare("hello") < 0);
    static_assert(testString.compare("HELLO") > 0);
    static_assert(testString.compare("Hello") == 0);
  }

  SUBCASE("Compare with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.compare(FixedString<8>("Hello")) == 0);
    REQUIRE(testString.compare(FixedString<16>("Hello")) == 0);
    REQUIRE(testString.compare(FixedString<64>("Hello")) == 0);
    REQUIRE(testString.compare(FixedString<8>("World")) < 0);
    REQUIRE(testString.compare(FixedString<16>("Hell")) > 0);

    static_assert(testString.compare(FixedString<8>("Hello")) == 0);
    static_assert(testString.compare(FixedString<16>("Hello")) == 0);
    static_assert(testString.compare(FixedString<64>("Hello")) == 0);
    static_assert(testString.compare(FixedString<8>("World")) < 0);
    static_assert(testString.compare(FixedString<16>("Hell")) > 0);
  }

  SUBCASE("Compare with single character strings") {
    constexpr FixedString<32> testString1("A");
    constexpr FixedString<32> testString2("B");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("A") == 0);
    REQUIRE(testString1.compare("B") < 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("A") == 0);
    static_assert(testString1.compare("B") < 0);
  }

  SUBCASE("Compare with repeated characters") {
    constexpr FixedString<32> testString1("aaa");
    constexpr FixedString<32> testString2("aa");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("aaa") == 0);
    REQUIRE(testString1.compare("aa") > 0);

    static_assert(testString1.compare(testString2) > 0);
    static_assert(testString2.compare(testString1) < 0);
    static_assert(testString1.compare("aaa") == 0);
    static_assert(testString1.compare("aa") > 0);
  }

  SUBCASE("Compare with special characters") {
    constexpr FixedString<32> testString1("Hello!");
    constexpr FixedString<32> testString2("Hello");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("Hello!") == 0);
    REQUIRE(testString1.compare("Hello") > 0);

    static_assert(testString1.compare(testString2) > 0);
    static_assert(testString2.compare(testString1) < 0);
    static_assert(testString1.compare("Hello!") == 0);
    static_assert(testString1.compare("Hello") > 0);
  }

  SUBCASE("Compare with numbers") {
    constexpr FixedString<32> testString1("123");
    constexpr FixedString<32> testString2("456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123") == 0);
    REQUIRE(testString1.compare("456") < 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("123") == 0);
    static_assert(testString1.compare("456") < 0);
  }

  SUBCASE("Compare with mixed content") {
    constexpr FixedString<32> testString1("Hello123");
    constexpr FixedString<32> testString2("Hello456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("Hello123") == 0);
    REQUIRE(testString1.compare("Hello456") < 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("Hello123") == 0);
    static_assert(testString1.compare("Hello456") < 0);
  }

  SUBCASE("Compare with maximum length strings") {
    constexpr FixedString<16> testString1("123456789012345");
    constexpr FixedString<16> testString2("123456789012346");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123456789012345") == 0);
    REQUIRE(testString1.compare("123456789012346") < 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("123456789012345") == 0);
    static_assert(testString1.compare("123456789012346") < 0);
  }

  SUBCASE("Compare with std::string") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello")) > 0);
    REQUIRE(testString.compare(std::string("World")) < 0);

    static_assert(testString.compare(CStringView("Hello World")) == 0);
    static_assert(testString.compare(CStringView("Hello")) > 0);
    static_assert(testString.compare(CStringView("World")) < 0);
  }

  SUBCASE("Compare with array") {
    constexpr FixedString<32> testString("Hello");
    constexpr array<char, 6> arr = {'H', 'e', 'l', 'l', 'o', '\0'};

    REQUIRE(testString.compare(arr.data()) == 0);
    REQUIRE(testString.compare("Hello") == 0);

    static_assert(testString.compare(arr.data()) == 0);
    static_assert(testString.compare("Hello") == 0);
  }

  SUBCASE("Compare edge cases") {
    constexpr FixedString<32> testString("Hello");

    // Compare with null-terminated string
    REQUIRE(testString.compare("Hello\0World") == 0);

    // Compare with string containing null character
    constexpr FixedString<32> testStringWithNull("Hello\0World");
    REQUIRE(testString.compare(testStringWithNull) == 0);

    static_assert(testString.compare("Hello\0World") == 0);
    static_assert(testString.compare(testStringWithNull) == 0);
  }
}

// starts_with() prefix check.
TEST_CASE("core/fixed_string/starts_with") {
  SUBCASE("Starts with FixedString") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with(FixedString<32>("Hello")));
    REQUIRE(testString.starts_with(FixedString<32>("Hello World")));
    REQUIRE(testString.starts_with(FixedString<32>("H")));
    REQUIRE(testString.starts_with(FixedString<32>("World")) == false);
    REQUIRE(testString.starts_with(FixedString<32>("xyz")) == false);
    REQUIRE(testString.starts_with(FixedString<32>("")));

    static_assert(testString.starts_with(FixedString<32>("Hello")));
    static_assert(testString.starts_with(FixedString<32>("Hello World")));
    static_assert(testString.starts_with(FixedString<32>("H")));
    static_assert(testString.starts_with(FixedString<32>("World")) == false);
    static_assert(testString.starts_with(FixedString<32>("xyz")) == false);
    static_assert(testString.starts_with(FixedString<32>("")));
  }

  SUBCASE("Starts with StringLike") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")));
    REQUIRE(testString.starts_with(std::string("Hello World")));
    REQUIRE(testString.starts_with(std::string("H")));
    REQUIRE(testString.starts_with(std::string("World")) == false);
    REQUIRE(testString.starts_with(std::string("xyz")) == false);
    REQUIRE(testString.starts_with(std::string("")));

    static_assert(testString.starts_with(CStringView("Hello")));
    static_assert(testString.starts_with(CStringView("Hello World")));
    static_assert(testString.starts_with(CStringView("H")));
    static_assert(testString.starts_with(CStringView("World")) == false);
    static_assert(testString.starts_with(CStringView("xyz")) == false);
    static_assert(testString.starts_with(CStringView("")));
  }

  SUBCASE("Starts with C string") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with("Hello"));
    REQUIRE(testString.starts_with("Hello World"));
    REQUIRE(testString.starts_with("H"));
    REQUIRE(testString.starts_with("World") == false);
    REQUIRE(testString.starts_with("xyz") == false);
    REQUIRE(testString.starts_with(""));

    static_assert(testString.starts_with("Hello"));
    static_assert(testString.starts_with("Hello World"));
    static_assert(testString.starts_with("H"));
    static_assert(testString.starts_with("World") == false);
    static_assert(testString.starts_with("xyz") == false);
    static_assert(testString.starts_with(""));
  }

  SUBCASE("Starts with character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with('H'));
    REQUIRE(testString.starts_with('h') == false);
    REQUIRE(testString.starts_with('W') == false);
    REQUIRE(testString.starts_with('x') == false);

    static_assert(testString.starts_with('H'));
    static_assert(testString.starts_with('h') == false);
    static_assert(testString.starts_with('W') == false);
    static_assert(testString.starts_with('x') == false);
  }

  SUBCASE("Starts with empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.starts_with(FixedString<16>("Hello")) == false);
    REQUIRE(testString.starts_with(std::string("Hello")) == false);
    REQUIRE(testString.starts_with("Hello") == false);
    REQUIRE(testString.starts_with('H') == false);
    REQUIRE(testString.starts_with(""));

    static_assert(testString.starts_with(FixedString<16>("Hello")) == false);
    static_assert(testString.starts_with(CStringView("Hello")) == false);
    static_assert(testString.starts_with("Hello") == false);
    static_assert(testString.starts_with('H') == false);
    static_assert(testString.starts_with(""));
  }

  SUBCASE("Starts with single character string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.starts_with("A"));
    REQUIRE(testString.starts_with('A'));
    REQUIRE(testString.starts_with("B") == false);
    REQUIRE(testString.starts_with('B') == false);
    REQUIRE(testString.starts_with(""));

    static_assert(testString.starts_with("A"));
    static_assert(testString.starts_with('A'));
    static_assert(testString.starts_with("B") == false);
    static_assert(testString.starts_with('B') == false);
    static_assert(testString.starts_with(""));
  }

  SUBCASE("Starts with longer prefix") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.starts_with("Hello World") == false);
    REQUIRE(testString.starts_with("Hello Universe") == false);
    REQUIRE(testString.starts_with("Hello"));
    REQUIRE(testString.starts_with("Hell"));

    static_assert(testString.starts_with("Hello World") == false);
    static_assert(testString.starts_with("Hello Universe") == false);
    static_assert(testString.starts_with("Hello"));
    static_assert(testString.starts_with("Hell"));
  }

  SUBCASE("Starts with case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with("hello") == false);
    REQUIRE(testString.starts_with("HELLO") == false);
    REQUIRE(testString.starts_with("Hello"));
    REQUIRE(testString.starts_with('h') == false);
    REQUIRE(testString.starts_with('H'));

    static_assert(testString.starts_with("hello") == false);
    static_assert(testString.starts_with("HELLO") == false);
    static_assert(testString.starts_with("Hello"));
    static_assert(testString.starts_with('h') == false);
    static_assert(testString.starts_with('H'));
  }

  SUBCASE("Starts with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with(FixedString<8>("Hello")));
    REQUIRE(testString.starts_with(FixedString<16>("Hello")));
    REQUIRE(testString.starts_with(FixedString<64>("Hello")));
    REQUIRE(testString.starts_with(FixedString<8>("World")) == false);

    static_assert(testString.starts_with(FixedString<8>("Hello")));
    static_assert(testString.starts_with(FixedString<16>("Hello")));
    static_assert(testString.starts_with(FixedString<64>("Hello")));
    static_assert(testString.starts_with(FixedString<8>("World")) == false);
  }

  SUBCASE("Starts with repeated characters") {
    constexpr FixedString<32> testString("aaaab");

    REQUIRE(testString.starts_with("aaa"));
    REQUIRE(testString.starts_with("aaaa"));
    REQUIRE(testString.starts_with("aaaab"));
    REQUIRE(testString.starts_with("aaab") == false);
    REQUIRE(testString.starts_with('a'));
    REQUIRE(testString.starts_with('b') == false);

    static_assert(testString.starts_with("aaa"));
    static_assert(testString.starts_with("aaaa"));
    static_assert(testString.starts_with("aaaab"));
    static_assert(testString.starts_with("aaab") == false);
    static_assert(testString.starts_with('a'));
    static_assert(testString.starts_with('b') == false);
  }

  SUBCASE("Starts with special characters") {
    constexpr FixedString<32> testString("!@#$%");

    REQUIRE(testString.starts_with("!@#"));
    REQUIRE(testString.starts_with("!@#$%"));
    REQUIRE(testString.starts_with("!@#$%^") == false);
    REQUIRE(testString.starts_with('!'));
    REQUIRE(testString.starts_with('@') == false);

    static_assert(testString.starts_with("!@#"));
    static_assert(testString.starts_with("!@#$%"));
    static_assert(testString.starts_with("!@#$%^") == false);
    static_assert(testString.starts_with('!'));
    static_assert(testString.starts_with('@') == false);
  }

  SUBCASE("Starts with numbers") {
    constexpr FixedString<32> testString("12345");

    REQUIRE(testString.starts_with("123"));
    REQUIRE(testString.starts_with("12345"));
    REQUIRE(testString.starts_with("123456") == false);
    REQUIRE(testString.starts_with('1'));
    REQUIRE(testString.starts_with('2') == false);

    static_assert(testString.starts_with("123"));
    static_assert(testString.starts_with("12345"));
    static_assert(testString.starts_with("123456") == false);
    static_assert(testString.starts_with('1'));
    static_assert(testString.starts_with('2') == false);
  }

  SUBCASE("Starts with mixed content") {
    constexpr FixedString<32> testString("Hello123");

    REQUIRE(testString.starts_with("Hello"));
    REQUIRE(testString.starts_with("Hello1"));
    REQUIRE(testString.starts_with("Hello123"));
    REQUIRE(testString.starts_with("Hello1234") == false);
    REQUIRE(testString.starts_with('H'));
    REQUIRE(testString.starts_with('1') == false);

    static_assert(testString.starts_with("Hello"));
    static_assert(testString.starts_with("Hello1"));
    static_assert(testString.starts_with("Hello123"));
    static_assert(testString.starts_with("Hello1234") == false);
    static_assert(testString.starts_with('H'));
    static_assert(testString.starts_with('1') == false);
  }

  SUBCASE("Starts with maximum length strings") {
    constexpr FixedString<16> testString("123456789012345");

    REQUIRE(testString.starts_with("123456789012345"));
    REQUIRE(testString.starts_with("12345678901234"));
    REQUIRE(testString.starts_with("1234567890123456") == false);
    REQUIRE(testString.starts_with('1'));
    REQUIRE(testString.starts_with('5') == false);

    static_assert(testString.starts_with("123456789012345"));
    static_assert(testString.starts_with("12345678901234"));
    static_assert(testString.starts_with("1234567890123456") == false);
    static_assert(testString.starts_with('1'));
    static_assert(testString.starts_with('5') == false);
  }

  SUBCASE("Starts with std::string") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")));
    REQUIRE(testString.starts_with(std::string("Hello World")));
    REQUIRE(testString.starts_with(std::string("World")) == false);

    static_assert(testString.starts_with(CStringView("Hello")));
    static_assert(testString.starts_with(CStringView("Hello World")));
    static_assert(testString.starts_with(CStringView("World")) == false);
  }

  SUBCASE("Starts with array") {
    constexpr FixedString<32> testString("Hello");
    constexpr array<char, 4> arr = {'H', 'e', 'l', '\0'};

    REQUIRE(testString.starts_with(arr.data()));
    REQUIRE(testString.starts_with("Hel"));

    static_assert(testString.starts_with(arr.data()));
    static_assert(testString.starts_with("Hel"));
  }

  SUBCASE("Starts with edge cases") {
    constexpr FixedString<32> testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.starts_with("Hello\0World"));

    // Test with string containing null character
    constexpr FixedString<32> testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.starts_with("Hello"));

    static_assert(testString.starts_with("Hello\0World"));
    static_assert(testStringWithNull.starts_with("Hello"));
  }

  SUBCASE("Starts with whitespace") {
    constexpr FixedString<32> testString(" Hello World");

    REQUIRE(testString.starts_with(" "));
    REQUIRE(testString.starts_with(" Hello"));
    REQUIRE(testString.starts_with("Hello") == false);
    REQUIRE(testString.starts_with(' '));
    REQUIRE(testString.starts_with('H') == false);

    static_assert(testString.starts_with(" "));
    static_assert(testString.starts_with(" Hello"));
    static_assert(testString.starts_with("Hello") == false);
    static_assert(testString.starts_with(' '));
    static_assert(testString.starts_with('H') == false);
  }

  SUBCASE("Starts with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.starts_with("Hello"));
    REQUIRE(testString.starts_with("Hell"));
    REQUIRE(testString.starts_with("H"));
    REQUIRE(testString.starts_with(""));

    static_assert(testString.starts_with("Hello"));
    static_assert(testString.starts_with("Hell"));
    static_assert(testString.starts_with("H"));
    static_assert(testString.starts_with(""));
  }
}

// ends_with() suffix check.
TEST_CASE("core/fixed_string/ends_with") {
  SUBCASE("FixedString ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with(FixedString<32>("World")));
    REQUIRE(testString.ends_with(FixedString<32>("Hello World")));
    REQUIRE(testString.ends_with(FixedString<32>("d")));
    REQUIRE(testString.ends_with(FixedString<32>("Hello")) == false);
    REQUIRE(testString.ends_with(FixedString<32>("xyz")) == false);
    REQUIRE(testString.ends_with(FixedString<32>("")));

    static_assert(testString.ends_with(FixedString<32>("World")));
    static_assert(testString.ends_with(FixedString<32>("Hello World")));
    static_assert(testString.ends_with(FixedString<32>("d")));
    static_assert(testString.ends_with(FixedString<32>("Hello")) == false);
    static_assert(testString.ends_with(FixedString<32>("xyz")) == false);
    static_assert(testString.ends_with(FixedString<32>("")));
  }

  SUBCASE("StringLike ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with(std::string("World")));
    REQUIRE(testString.ends_with(std::string("Hello World")));
    REQUIRE(testString.ends_with(std::string("d")));
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("xyz")) == false);
    REQUIRE(testString.ends_with(std::string("")));

    static_assert(testString.ends_with(CStringView("World")));
    static_assert(testString.ends_with(CStringView("Hello World")));
    static_assert(testString.ends_with(CStringView("d")));
    static_assert(testString.ends_with(CStringView("Hello")) == false);
    static_assert(testString.ends_with(CStringView("xyz")) == false);
    static_assert(testString.ends_with(CStringView("")));
  }

  SUBCASE("C string ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with("World"));
    REQUIRE(testString.ends_with("Hello World"));
    REQUIRE(testString.ends_with("d"));
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with("xyz") == false);
    REQUIRE(testString.ends_with(""));

    static_assert(testString.ends_with("World"));
    static_assert(testString.ends_with("Hello World"));
    static_assert(testString.ends_with("d"));
    static_assert(testString.ends_with("Hello") == false);
    static_assert(testString.ends_with("xyz") == false);
    static_assert(testString.ends_with(""));
  }

  SUBCASE("Character ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with('d'));
    REQUIRE(testString.ends_with('D') == false);
    REQUIRE(testString.ends_with('H') == false);
    REQUIRE(testString.ends_with('x') == false);

    static_assert(testString.ends_with('d'));
    static_assert(testString.ends_with('D') == false);
    static_assert(testString.ends_with('H') == false);
    static_assert(testString.ends_with('x') == false);
  }

  SUBCASE("Empty string ends_with") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.ends_with(FixedString<16>("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with('H') == false);
    REQUIRE(testString.ends_with(""));

    static_assert(testString.ends_with(FixedString<16>("Hello")) == false);
    static_assert(testString.ends_with(CStringView("Hello")) == false);
    static_assert(testString.ends_with("Hello") == false);
    static_assert(testString.ends_with('H') == false);
    static_assert(testString.ends_with(""));
  }

  SUBCASE("Single character string ends_with") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.ends_with("A"));
    REQUIRE(testString.ends_with('A'));
    REQUIRE(testString.ends_with("B") == false);
    REQUIRE(testString.ends_with('B') == false);
    REQUIRE(testString.ends_with(""));

    static_assert(testString.ends_with("A"));
    static_assert(testString.ends_with('A'));
    static_assert(testString.ends_with("B") == false);
    static_assert(testString.ends_with('B') == false);
    static_assert(testString.ends_with(""));
  }

  SUBCASE("Longer suffix ends_with") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.ends_with("Hello World") == false);
    REQUIRE(testString.ends_with("Hello Universe") == false);
    REQUIRE(testString.ends_with("Hello"));
    REQUIRE(testString.ends_with("llo"));

    static_assert(testString.ends_with("Hello World") == false);
    static_assert(testString.ends_with("Hello Universe") == false);
    static_assert(testString.ends_with("Hello"));
    static_assert(testString.ends_with("llo"));
  }

  SUBCASE("Case sensitivity ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with("world") == false);
    REQUIRE(testString.ends_with("WORLD") == false);
    REQUIRE(testString.ends_with("World"));
    REQUIRE(testString.ends_with('d'));
    REQUIRE(testString.ends_with('D') == false);

    static_assert(testString.ends_with("world") == false);
    static_assert(testString.ends_with("WORLD") == false);
    static_assert(testString.ends_with("World"));
    static_assert(testString.ends_with('d'));
    static_assert(testString.ends_with('D') == false);
  }

  SUBCASE("Different FixedString capacities ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with(FixedString<8>("World")));
    REQUIRE(testString.ends_with(FixedString<16>("World")));
    REQUIRE(testString.ends_with(FixedString<64>("World")));
    REQUIRE(testString.ends_with(FixedString<8>("Hello")) == false);

    static_assert(testString.ends_with(FixedString<8>("World")));
    static_assert(testString.ends_with(FixedString<16>("World")));
    static_assert(testString.ends_with(FixedString<64>("World")));
    static_assert(testString.ends_with(FixedString<8>("Hello")) == false);
  }

  SUBCASE("Repeated characters ends_with") {
    constexpr FixedString<32> testString("baaaa");

    REQUIRE(testString.ends_with("aaa"));
    REQUIRE(testString.ends_with("aaaa"));
    REQUIRE(testString.ends_with("baaaa"));
    REQUIRE(testString.ends_with("aaaaa") == false);
    REQUIRE(testString.ends_with('a'));
    REQUIRE(testString.ends_with('b') == false);

    static_assert(testString.ends_with("aaa"));
    static_assert(testString.ends_with("aaaa"));
    static_assert(testString.ends_with("baaaa"));
    static_assert(testString.ends_with("aaaaa") == false);
    static_assert(testString.ends_with('a'));
    static_assert(testString.ends_with('b') == false);
  }

  SUBCASE("Special characters ends_with") {
    constexpr FixedString<32> testString("%$#@!");

    REQUIRE(testString.ends_with("@!"));
    REQUIRE(testString.ends_with("%$#@!"));
    REQUIRE(testString.ends_with("^%$#@!") == false);
    REQUIRE(testString.ends_with('!'));
    REQUIRE(testString.ends_with('@') == false);

    static_assert(testString.ends_with("@!"));
    static_assert(testString.ends_with("%$#@!"));
    static_assert(testString.ends_with("^%$#@!") == false);
    static_assert(testString.ends_with('!'));
    static_assert(testString.ends_with('@') == false);
  }

  SUBCASE("Numeric content ends_with") {
    constexpr FixedString<32> testString("54321");

    REQUIRE(testString.ends_with("321"));
    REQUIRE(testString.ends_with("54321"));
    REQUIRE(testString.ends_with("654321") == false);
    REQUIRE(testString.ends_with('1'));
    REQUIRE(testString.ends_with('2') == false);

    static_assert(testString.ends_with("321"));
    static_assert(testString.ends_with("54321"));
    static_assert(testString.ends_with("654321") == false);
    static_assert(testString.ends_with('1'));
    static_assert(testString.ends_with('2') == false);
  }

  SUBCASE("Mixed content ends_with") {
    constexpr FixedString<32> testString("123Hello");

    REQUIRE(testString.ends_with("Hello"));
    REQUIRE(testString.ends_with("3Hello"));
    REQUIRE(testString.ends_with("123Hello"));
    REQUIRE(testString.ends_with("0123Hello") == false);
    REQUIRE(testString.ends_with('o'));
    REQUIRE(testString.ends_with('1') == false);

    static_assert(testString.ends_with("Hello"));
    static_assert(testString.ends_with("3Hello"));
    static_assert(testString.ends_with("123Hello"));
    static_assert(testString.ends_with("0123Hello") == false);
    static_assert(testString.ends_with('o'));
    static_assert(testString.ends_with('1') == false);
  }

  SUBCASE("Maximum length strings ends_with") {
    constexpr FixedString<16> testString("123456789012345");

    REQUIRE(testString.ends_with("123456789012345"));
    REQUIRE(testString.ends_with("23456789012345"));
    REQUIRE(testString.ends_with("0123456789012345") == false);
    REQUIRE(testString.ends_with('5'));
    REQUIRE(testString.ends_with('1') == false);

    static_assert(testString.ends_with("123456789012345"));
    static_assert(testString.ends_with("23456789012345"));
    static_assert(testString.ends_with("0123456789012345") == false);
    static_assert(testString.ends_with('5'));
    static_assert(testString.ends_with('1') == false);
  }

  SUBCASE("Array ends_with") {
    constexpr FixedString<32> testString("Hello");
    constexpr array<char, 4> arr = {'l', 'l', 'o', '\0'};

    REQUIRE(testString.ends_with(arr.data()));
    REQUIRE(testString.ends_with("llo"));

    static_assert(testString.ends_with(arr.data()));
    static_assert(testString.ends_with("llo"));
  }

  SUBCASE("Edge cases ends_with") {
    constexpr FixedString<32> testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.ends_with("Hello\0World"));

    // Test with string containing null character
    constexpr FixedString<32> testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.ends_with("World") == false);

    static_assert(testString.ends_with("Hello\0World"));
    static_assert(testStringWithNull.ends_with("World") == false);
  }

  SUBCASE("Whitespace ends_with") {
    constexpr FixedString<32> testString("Hello World ");

    REQUIRE(testString.ends_with(" "));
    REQUIRE(testString.ends_with("World "));
    REQUIRE(testString.ends_with("World") == false);
    REQUIRE(testString.ends_with(' '));
    REQUIRE(testString.ends_with('d') == false);

    static_assert(testString.ends_with(" "));
    static_assert(testString.ends_with("World "));
    static_assert(testString.ends_with("World") == false);
    static_assert(testString.ends_with(' '));
    static_assert(testString.ends_with('d') == false);
  }

  SUBCASE("Exact match ends_with") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.ends_with("Hello"));
    REQUIRE(testString.ends_with("llo"));
    REQUIRE(testString.ends_with("o"));
    REQUIRE(testString.ends_with(""));

    static_assert(testString.ends_with("Hello"));
    static_assert(testString.ends_with("llo"));
    static_assert(testString.ends_with("o"));
    static_assert(testString.ends_with(""));
  }

  SUBCASE("Overlapping patterns ends_with") {
    constexpr FixedString<32> testString("ababab");

    REQUIRE(testString.ends_with("ab"));
    REQUIRE(testString.ends_with("bab"));
    REQUIRE(testString.ends_with("abab"));
    REQUIRE(testString.ends_with("babab"));
    REQUIRE(testString.ends_with("ababab"));
    REQUIRE(testString.ends_with("bababab") == false);

    static_assert(testString.ends_with("ab"));
    static_assert(testString.ends_with("bab"));
    static_assert(testString.ends_with("abab"));
    static_assert(testString.ends_with("babab"));
    static_assert(testString.ends_with("ababab"));
    static_assert(testString.ends_with("bababab") == false);
  }

  SUBCASE("Multiple occurrences ends_with") {
    constexpr FixedString<32> testString("abababab");

    REQUIRE(testString.ends_with("ab"));
    REQUIRE(testString.ends_with("bab"));
    REQUIRE(testString.ends_with("abab"));
    REQUIRE(testString.ends_with("ababab"));
    REQUIRE(testString.ends_with("abababab"));

    static_assert(testString.ends_with("ab"));
    static_assert(testString.ends_with("bab"));
    static_assert(testString.ends_with("abab"));
    static_assert(testString.ends_with("ababab"));
    static_assert(testString.ends_with("abababab"));
  }

  SUBCASE("Unicode content ends_with") {
    constexpr FixedString<32> testString("Hello 世界");

    REQUIRE(testString.ends_with("世界"));
    REQUIRE(testString.ends_with("Hello 世界"));
    REQUIRE(testString.ends_with("界"));
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with("xyz") == false);
    REQUIRE(testString.ends_with(""));

    static_assert(testString.ends_with("世界"));
    static_assert(testString.ends_with("Hello 世界"));
    static_assert(testString.ends_with("界"));
    static_assert(testString.ends_with("Hello") == false);
    static_assert(testString.ends_with("xyz") == false);
    static_assert(testString.ends_with(""));
  }

  SUBCASE("Long strings ends_with") {
    constexpr FixedString<64> testString("This is a very long string for performance testing");

    REQUIRE(testString.ends_with("testing"));
    REQUIRE(testString.ends_with("performance testing"));
    REQUIRE(testString.ends_with("This is a very long string for performance testing"));
    REQUIRE(testString.ends_with("g"));
    REQUIRE(testString.ends_with("T") == false);
    REQUIRE(testString.ends_with(""));

    static_assert(testString.ends_with("testing"));
    static_assert(testString.ends_with("performance testing"));
    static_assert(testString.ends_with("This is a very long string for performance testing"));
    static_assert(testString.ends_with("g"));
    static_assert(testString.ends_with("T") == false);
    static_assert(testString.ends_with(""));
  }

  SUBCASE("Constexpr operations ends_with") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<16> str2("World");
    constexpr FixedString<16> str3("Test");

    // Compile-time ends_with operations
    constexpr bool ends1 = str1.ends_with("llo");
    constexpr bool ends2 = str2.ends_with("rld");
    constexpr bool ends3 = str3.ends_with("est");

    static_assert(ends1);
    static_assert(ends2);
    static_assert(ends3);

    // Compile-time character ends_with operations
    constexpr bool charEnds1 = str1.ends_with('o');
    constexpr bool charEnds2 = str2.ends_with('d');
    constexpr bool charEnds3 = str3.ends_with('t');

    static_assert(charEnds1);
    static_assert(charEnds2);
    static_assert(charEnds3);
  }
}

// contains() substring check.
TEST_CASE("core/fixed_string/contains") {
  SUBCASE("C string contains") {
    constexpr FixedString<32> str("Hello World");

    // Basic contains
    REQUIRE(str.contains("World"));
    REQUIRE(str.contains("Hello"));
    REQUIRE(str.contains("lo Wo"));
    REQUIRE(str.contains("Hello World"));
    REQUIRE(str.contains("xyz") == false);
    REQUIRE(str.contains(""));

    static_assert(str.contains("World"));
    static_assert(str.contains("Hello"));
    static_assert(str.contains("lo Wo"));
    static_assert(str.contains("Hello World"));
    static_assert(str.contains("xyz") == false);
    static_assert(str.contains(""));
  }

  SUBCASE("FixedString contains") {
    constexpr FixedString<16> str("Hello World");

    // Basic contains with FixedString
    REQUIRE(str.contains(FixedString<16>("World")));
    REQUIRE(str.contains(FixedString<16>("Hello")));
    REQUIRE(str.contains(FixedString<16>("lo Wo")));
    REQUIRE(str.contains(FixedString<16>("Hello World")));
    REQUIRE(str.contains(FixedString<16>("xyz")) == false);
    REQUIRE(str.contains(FixedString<16>("")));

    // Different capacities
    REQUIRE(str.contains(FixedString<8>("World")));
    REQUIRE(str.contains(FixedString<64>("World")));

    static_assert(str.contains(FixedString<16>("World")));
    static_assert(str.contains(FixedString<16>("Hello")));
    static_assert(str.contains(FixedString<16>("lo Wo")));
    static_assert(str.contains(FixedString<16>("Hello World")));
    static_assert(str.contains(FixedString<16>("xyz")) == false);
    static_assert(str.contains(FixedString<16>("")));

    static_assert(str.contains(FixedString<8>("World")));
    static_assert(str.contains(FixedString<64>("World")));
  }

  SUBCASE("StringLike contains") {
    constexpr FixedString<32> str("Hello World");

    REQUIRE(str.contains(std::string("World")));
    REQUIRE(str.contains(std::string("Hello")));
    REQUIRE(str.contains(std::string("lo Wo")));
    REQUIRE(str.contains(std::string("Hello World")));
    REQUIRE(str.contains(std::string("xyz")) == false);
    REQUIRE(str.contains(std::string("")));

    static_assert(str.contains(CStringView("World")));
    static_assert(str.contains(CStringView("Hello")));
    static_assert(str.contains(CStringView("lo Wo")));
    static_assert(str.contains(CStringView("Hello World")));
    static_assert(str.contains(CStringView("xyz")) == false);
    static_assert(str.contains(CStringView("")));
  }

  SUBCASE("Character contains") {
    constexpr FixedString<32> str("Hello World");

    // Character contains
    REQUIRE(str.contains('H'));
    REQUIRE(str.contains('o'));
    REQUIRE(str.contains('l'));
    REQUIRE(str.contains(' '));
    REQUIRE(str.contains('x') == false);
    REQUIRE(str.contains('Z') == false);

    static_assert(str.contains('H'));
    static_assert(str.contains('o'));
    static_assert(str.contains('l'));
    static_assert(str.contains(' '));
    static_assert(str.contains('x') == false);
    static_assert(str.contains('Z') == false);
  }

  SUBCASE("Empty string contains") {
    constexpr FixedString<32> str("");

    // Empty string contains
    REQUIRE(str.contains(FixedString<16>("Hello")) == false);
    REQUIRE(str.contains(std::string("Hello")) == false);
    REQUIRE(str.contains("Hello") == false);
    REQUIRE(str.contains('H') == false);
    REQUIRE(str.contains(""));

    static_assert(str.contains(FixedString<16>("Hello")) == false);
    static_assert(str.contains(CStringView("Hello")) == false);
    static_assert(str.contains("Hello") == false);
    static_assert(str.contains('H') == false);
    static_assert(str.contains(""));
  }

  SUBCASE("Single character string contains") {
    constexpr FixedString<8> str("A");

    // Single character contains
    REQUIRE(str.contains("A"));
    REQUIRE(str.contains('A'));
    REQUIRE(str.contains("B") == false);
    REQUIRE(str.contains('B') == false);
    REQUIRE(str.contains(""));

    static_assert(str.contains("A"));
    static_assert(str.contains('A'));
    static_assert(str.contains("B") == false);
    static_assert(str.contains('B') == false);
    static_assert(str.contains(""));
  }

  SUBCASE("case_sensitivity") {
    constexpr FixedString<32> str("Hello World");

    // Case sensitive contains
    REQUIRE(str.contains("world") == false);
    REQUIRE(str.contains("WORLD") == false);
    REQUIRE(str.contains("World"));
    REQUIRE(str.contains('h') == false);
    REQUIRE(str.contains('H'));

    static_assert(str.contains("world") == false);
    static_assert(str.contains("WORLD") == false);
    static_assert(str.contains("World"));
    static_assert(str.contains('h') == false);
    static_assert(str.contains('H'));
  }

  SUBCASE("edge_cases") {
    constexpr FixedString<16> str("Hello");

    // Longer substring than string
    REQUIRE(str.contains("Hello World") == false);
    REQUIRE(str.contains("Hello Universe") == false);

    // Exact match
    REQUIRE(str.contains("Hello"));

    // Substrings
    REQUIRE(str.contains("llo"));
    REQUIRE(str.contains("ell"));

    static_assert(str.contains("Hello World") == false);
    static_assert(str.contains("Hello Universe") == false);
    static_assert(str.contains("Hello"));
    static_assert(str.contains("llo"));
    static_assert(str.contains("ell"));
  }

  SUBCASE("special_characters") {
    constexpr FixedString<32> str("Hello\n\tWorld!");

    // Special characters
    REQUIRE(str.contains("\n"));
    REQUIRE(str.contains("\t"));
    REQUIRE(str.contains("!"));
    REQUIRE(str.contains("Hello\n"));
    REQUIRE(str.contains("\tWorld"));
    REQUIRE(str.contains("World!"));

    static_assert(str.contains("\n"));
    static_assert(str.contains("\t"));
    static_assert(str.contains("!"));
    static_assert(str.contains("Hello\n"));
    static_assert(str.contains("\tWorld"));
    static_assert(str.contains("World!"));
  }

  SUBCASE("unicode_content") {
    constexpr FixedString<32> str("Hello 世界");

    // Unicode contains
    REQUIRE(str.contains("世界"));
    REQUIRE(str.contains("Hello 世"));
    REQUIRE(str.contains("界"));
    REQUIRE(str.contains("世"));
    REQUIRE(str.contains("宇宙") == false);

    static_assert(str.contains("世界"));
    static_assert(str.contains("Hello 世"));
    static_assert(str.contains("界"));
    static_assert(str.contains("世"));
    static_assert(str.contains("宇宙") == false);
  }

  SUBCASE("Repeated patterns") {
    constexpr FixedString<32> str("ababab");

    // Overlapping patterns
    REQUIRE(str.contains("ab"));
    REQUIRE(str.contains("bab"));
    REQUIRE(str.contains("abab"));
    REQUIRE(str.contains("ababab"));
    REQUIRE(str.contains("babab"));
    REQUIRE(str.contains("abababa") == false);

    static_assert(str.contains("ab"));
    static_assert(str.contains("bab"));
    static_assert(str.contains("abab"));
    static_assert(str.contains("ababab"));
    static_assert(str.contains("babab"));
    static_assert(str.contains("abababa") == false);
  }

  SUBCASE("numeric_content") {
    constexpr FixedString<32> str("12345");

    // Numeric contains
    REQUIRE(str.contains("123"));
    REQUIRE(str.contains("345"));
    REQUIRE(str.contains("234"));
    REQUIRE(str.contains("12345"));
    REQUIRE(str.contains("678") == false);
    REQUIRE(str.contains('1'));
    REQUIRE(str.contains('5'));
    REQUIRE(str.contains('9') == false);

    static_assert(str.contains("123"));
    static_assert(str.contains("345"));
    static_assert(str.contains("234"));
    static_assert(str.contains("12345"));
    static_assert(str.contains("678") == false);
    static_assert(str.contains('1'));
    static_assert(str.contains('5'));
    static_assert(str.contains('9') == false);
  }

  SUBCASE("mixed_content") {
    constexpr FixedString<32> str("123Hello456");

    // Mixed alphanumeric contains
    REQUIRE(str.contains("123"));
    REQUIRE(str.contains("Hello"));
    REQUIRE(str.contains("456"));
    REQUIRE(str.contains("3Hello4"));
    REQUIRE(str.contains("123Hello456"));
    REQUIRE(str.contains("789") == false);

    static_assert(str.contains("123"));
    static_assert(str.contains("Hello"));
    static_assert(str.contains("456"));
    static_assert(str.contains("3Hello4"));
    static_assert(str.contains("123Hello456"));
    static_assert(str.contains("789") == false);
  }

  SUBCASE("Position-specific contains") {
    constexpr FixedString<32> str("Hello World");

    // Beginning
    REQUIRE(str.contains("H"));
    REQUIRE(str.contains("He"));
    REQUIRE(str.contains("Hello"));

    // Middle
    REQUIRE(str.contains("l"));
    REQUIRE(str.contains("ll"));
    REQUIRE(str.contains("lo W"));

    // End
    REQUIRE(str.contains("d"));
    REQUIRE(str.contains("ld"));
    REQUIRE(str.contains("World"));

    static_assert(str.contains("H"));
    static_assert(str.contains("He"));
    static_assert(str.contains("Hello"));
    static_assert(str.contains("l"));
    static_assert(str.contains("ll"));
    static_assert(str.contains("lo W"));
    static_assert(str.contains("d"));
    static_assert(str.contains("ld"));
    static_assert(str.contains("World"));
  }
}

// substr() view or copy.
TEST_CASE("core/fixed_string/substr") {
  SUBCASE("Substr basic functionality") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(0).size() == 11);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "Hello World") == 0);
    REQUIRE(testString.substr(0, 5).size() == 5);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(testString.substr(6).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);
    REQUIRE(testString.substr(6, 5).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6, 5).c_str(), "World") == 0);
    REQUIRE(testString.substr(6, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    REQUIRE(testString.substr(0, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);

    static_assert(testString.substr(0).size() == 11);
    static_assert(cstrcmp(testString.substr(0).c_str(), "Hello World") == 0);
    static_assert(testString.substr(0, 5).size() == 5);
    static_assert(cstrcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    static_assert(testString.substr(6).size() == 5);
    static_assert(cstrcmp(testString.substr(6).c_str(), "World") == 0);
    static_assert(testString.substr(6, 5).size() == 5);
    static_assert(cstrcmp(testString.substr(6, 5).c_str(), "World") == 0);
    static_assert(testString.substr(6, 3).size() == 3);
    static_assert(cstrcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    static_assert(testString.substr(0, 0).size() == 0);
    static_assert(cstrcmp(testString.substr(0, 0).c_str(), "") == 0);
  }

  SUBCASE("Substr with default parameters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr().size() == 11);
    REQUIRE(std::strcmp(testString.substr().c_str(), "Hello World") == 0);
    REQUIRE(testString.substr(0).size() == 11);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "Hello World") == 0);
    REQUIRE(testString.substr(6).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);

    static_assert(testString.substr().size() == 11);
    static_assert(cstrcmp(testString.substr().c_str(), "Hello World") == 0);
    static_assert(testString.substr(0).size() == 11);
    static_assert(cstrcmp(testString.substr(0).c_str(), "Hello World") == 0);
    static_assert(testString.substr(6).size() == 5);
    static_assert(cstrcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SUBCASE("Substr with count parameter") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(0, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "H") == 0);
    REQUIRE(testString.substr(0, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(0, 2).c_str(), "He") == 0);
    REQUIRE(testString.substr(0, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(0, 3).c_str(), "Hel") == 0);
    REQUIRE(testString.substr(0, 4).size() == 4);
    REQUIRE(std::strcmp(testString.substr(0, 4).c_str(), "Hell") == 0);
    REQUIRE(testString.substr(0, 5).size() == 5);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(testString.substr(6, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(6, 1).c_str(), "W") == 0);
    REQUIRE(testString.substr(6, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(6, 2).c_str(), "Wo") == 0);
    REQUIRE(testString.substr(6, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    REQUIRE(testString.substr(6, 4).size() == 4);
    REQUIRE(std::strcmp(testString.substr(6, 4).c_str(), "Worl") == 0);
    REQUIRE(testString.substr(6, 5).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6, 5).c_str(), "World") == 0);

    static_assert(testString.substr(0, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(0, 1).c_str(), "H") == 0);
    static_assert(testString.substr(0, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(0, 2).c_str(), "He") == 0);
    static_assert(testString.substr(0, 3).size() == 3);
    static_assert(cstrcmp(testString.substr(0, 3).c_str(), "Hel") == 0);
    static_assert(testString.substr(0, 4).size() == 4);
    static_assert(cstrcmp(testString.substr(0, 4).c_str(), "Hell") == 0);
    static_assert(testString.substr(0, 5).size() == 5);
    static_assert(cstrcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    static_assert(testString.substr(6, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(6, 1).c_str(), "W") == 0);
    static_assert(testString.substr(6, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(6, 2).c_str(), "Wo") == 0);
    static_assert(testString.substr(6, 3).size() == 3);
    static_assert(cstrcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    static_assert(testString.substr(6, 4).size() == 4);
    static_assert(cstrcmp(testString.substr(6, 4).c_str(), "Worl") == 0);
    static_assert(testString.substr(6, 5).size() == 5);
    static_assert(cstrcmp(testString.substr(6, 5).c_str(), "World") == 0);
  }

  SUBCASE("Substr with npos count") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(0, FixedString<32>::npos).size() == 11);
    REQUIRE(std::strcmp(testString.substr(0, FixedString<32>::npos).c_str(), "Hello World") == 0);
    REQUIRE(testString.substr(6, FixedString<32>::npos).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6, FixedString<32>::npos).c_str(), "World") == 0);
    REQUIRE(testString.substr(10, FixedString<32>::npos).size() == 1);
    REQUIRE(std::strcmp(testString.substr(10, FixedString<32>::npos).c_str(), "d") == 0);

    static_assert(testString.substr(0, FixedString<32>::npos).size() == 11);
    static_assert(cstrcmp(testString.substr(0, FixedString<32>::npos).c_str(), "Hello World") == 0);
    static_assert(testString.substr(6, FixedString<32>::npos).size() == 5);
    static_assert(cstrcmp(testString.substr(6, FixedString<32>::npos).c_str(), "World") == 0);
    static_assert(testString.substr(10, FixedString<32>::npos).size() == 1);
    static_assert(cstrcmp(testString.substr(10, FixedString<32>::npos).c_str(), "d") == 0);
  }

  SUBCASE("Substr with position at end") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(11).size() == 0);
    REQUIRE(std::strcmp(testString.substr(11).c_str(), "") == 0);
    REQUIRE(testString.substr(11, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(11, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(11, 5).size() == 0);
    REQUIRE(std::strcmp(testString.substr(11, 5).c_str(), "") == 0);

    static_assert(testString.substr(11).size() == 0);
    static_assert(cstrcmp(testString.substr(11).c_str(), "") == 0);
    static_assert(testString.substr(11, 0).size() == 0);
    static_assert(cstrcmp(testString.substr(11, 0).c_str(), "") == 0);
    static_assert(testString.substr(11, 5).size() == 0);
    static_assert(cstrcmp(testString.substr(11, 5).c_str(), "") == 0);
  }

  SUBCASE("Substr from empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.substr(0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "") == 0);
    REQUIRE(testString.substr(0, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(0, 5).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "") == 0);

    static_assert(testString.substr(0).size() == 0);
    static_assert(cstrcmp(testString.substr(0).c_str(), "") == 0);
    static_assert(testString.substr(0, 0).size() == 0);
    static_assert(cstrcmp(testString.substr(0, 0).c_str(), "") == 0);
    static_assert(testString.substr(0, 5).size() == 0);
    static_assert(cstrcmp(testString.substr(0, 5).c_str(), "") == 0);
  }

  SUBCASE("Substr single character") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.substr(0).size() == 1);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "A") == 0);
    REQUIRE(testString.substr(0, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "A") == 0);
    REQUIRE(testString.substr(0, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(1).size() == 0);
    REQUIRE(std::strcmp(testString.substr(1).c_str(), "") == 0);
    REQUIRE(testString.substr(1, 1).size() == 0);
    REQUIRE(std::strcmp(testString.substr(1, 1).c_str(), "") == 0);

    static_assert(testString.substr(0).size() == 1);
    static_assert(cstrcmp(testString.substr(0).c_str(), "A") == 0);
    static_assert(testString.substr(0, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(0, 1).c_str(), "A") == 0);
    static_assert(testString.substr(0, 0).size() == 0);
    static_assert(cstrcmp(testString.substr(0, 0).c_str(), "") == 0);
    static_assert(testString.substr(1).size() == 0);
    static_assert(cstrcmp(testString.substr(1).c_str(), "") == 0);
    static_assert(testString.substr(1, 1).size() == 0);
    static_assert(cstrcmp(testString.substr(1, 1).c_str(), "") == 0);
  }

  SUBCASE("Substr with repeated characters") {
    constexpr FixedString<32> testString("aaaaa");

    REQUIRE(testString.substr(0).size() == 5);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "aaaaa") == 0);
    REQUIRE(testString.substr(0, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "a") == 0);
    REQUIRE(testString.substr(0, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(0, 2).c_str(), "aa") == 0);
    REQUIRE(testString.substr(0, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(0, 3).c_str(), "aaa") == 0);
    REQUIRE(testString.substr(0, 4).size() == 4);
    REQUIRE(std::strcmp(testString.substr(0, 4).c_str(), "aaaa") == 0);
    REQUIRE(testString.substr(0, 5).size() == 5);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "aaaaa") == 0);
    REQUIRE(testString.substr(1).size() == 4);
    REQUIRE(std::strcmp(testString.substr(1).c_str(), "aaaa") == 0);
    REQUIRE(testString.substr(2).size() == 3);
    REQUIRE(std::strcmp(testString.substr(2).c_str(), "aaa") == 0);
    REQUIRE(testString.substr(3).size() == 2);
    REQUIRE(std::strcmp(testString.substr(3).c_str(), "aa") == 0);
    REQUIRE(testString.substr(4).size() == 1);
    REQUIRE(std::strcmp(testString.substr(4).c_str(), "a") == 0);
    REQUIRE(testString.substr(5).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "") == 0);

    static_assert(testString.substr(0).size() == 5);
    static_assert(cstrcmp(testString.substr(0).c_str(), "aaaaa") == 0);
    static_assert(testString.substr(0, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(0, 1).c_str(), "a") == 0);
    static_assert(testString.substr(0, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(0, 2).c_str(), "aa") == 0);
    static_assert(testString.substr(0, 3).size() == 3);
    static_assert(cstrcmp(testString.substr(0, 3).c_str(), "aaa") == 0);
    static_assert(testString.substr(0, 4).size() == 4);
    static_assert(cstrcmp(testString.substr(0, 4).c_str(), "aaaa") == 0);
    static_assert(testString.substr(0, 5).size() == 5);
    static_assert(cstrcmp(testString.substr(0, 5).c_str(), "aaaaa") == 0);
    static_assert(testString.substr(1).size() == 4);
    static_assert(cstrcmp(testString.substr(1).c_str(), "aaaa") == 0);
    static_assert(testString.substr(2).size() == 3);
    static_assert(cstrcmp(testString.substr(2).c_str(), "aaa") == 0);
    static_assert(testString.substr(3).size() == 2);
    static_assert(cstrcmp(testString.substr(3).c_str(), "aa") == 0);
    static_assert(testString.substr(4).size() == 1);
    static_assert(cstrcmp(testString.substr(4).c_str(), "a") == 0);
    static_assert(testString.substr(5).size() == 0);
    static_assert(cstrcmp(testString.substr(5).c_str(), "") == 0);
  }

  SUBCASE("Substr with special characters") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.substr(5).size() == 8);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), ", World!") == 0);
    REQUIRE(testString.substr(5, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), ",") == 0);
    REQUIRE(testString.substr(5, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), ", ") == 0);
    REQUIRE(testString.substr(12).size() == 1);
    REQUIRE(std::strcmp(testString.substr(12).c_str(), "!") == 0);
    REQUIRE(testString.substr(12, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(12, 1).c_str(), "!") == 0);

    static_assert(testString.substr(5).size() == 8);
    static_assert(cstrcmp(testString.substr(5).c_str(), ", World!") == 0);
    static_assert(testString.substr(5, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(5, 1).c_str(), ",") == 0);
    static_assert(testString.substr(5, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(5, 2).c_str(), ", ") == 0);
    static_assert(testString.substr(12).size() == 1);
    static_assert(cstrcmp(testString.substr(12).c_str(), "!") == 0);
    static_assert(testString.substr(12, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(12, 1).c_str(), "!") == 0);
  }

  SUBCASE("Substr with numbers") {
    constexpr FixedString<32> testString("12345");

    REQUIRE(testString.substr(0).size() == 5);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "12345") == 0);
    REQUIRE(testString.substr(0, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "1") == 0);
    REQUIRE(testString.substr(0, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(0, 2).c_str(), "12") == 0);
    REQUIRE(testString.substr(0, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(0, 3).c_str(), "123") == 0);
    REQUIRE(testString.substr(0, 4).size() == 4);
    REQUIRE(std::strcmp(testString.substr(0, 4).c_str(), "1234") == 0);
    REQUIRE(testString.substr(0, 5).size() == 5);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "12345") == 0);
    REQUIRE(testString.substr(1).size() == 4);
    REQUIRE(std::strcmp(testString.substr(1).c_str(), "2345") == 0);
    REQUIRE(testString.substr(2).size() == 3);
    REQUIRE(std::strcmp(testString.substr(2).c_str(), "345") == 0);
    REQUIRE(testString.substr(3).size() == 2);
    REQUIRE(std::strcmp(testString.substr(3).c_str(), "45") == 0);
    REQUIRE(testString.substr(4).size() == 1);
    REQUIRE(std::strcmp(testString.substr(4).c_str(), "5") == 0);
    REQUIRE(testString.substr(5).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "") == 0);

    static_assert(testString.substr(0).size() == 5);
    static_assert(cstrcmp(testString.substr(0).c_str(), "12345") == 0);
    static_assert(testString.substr(0, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(0, 1).c_str(), "1") == 0);
    static_assert(testString.substr(0, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(0, 2).c_str(), "12") == 0);
    static_assert(testString.substr(0, 3).size() == 3);
    static_assert(cstrcmp(testString.substr(0, 3).c_str(), "123") == 0);
    static_assert(testString.substr(0, 4).size() == 4);
    static_assert(cstrcmp(testString.substr(0, 4).c_str(), "1234") == 0);
    static_assert(testString.substr(0, 5).size() == 5);
    static_assert(cstrcmp(testString.substr(0, 5).c_str(), "12345") == 0);
    static_assert(testString.substr(1).size() == 4);
    static_assert(cstrcmp(testString.substr(1).c_str(), "2345") == 0);
    static_assert(testString.substr(2).size() == 3);
    static_assert(cstrcmp(testString.substr(2).c_str(), "345") == 0);
    static_assert(testString.substr(3).size() == 2);
    static_assert(cstrcmp(testString.substr(3).c_str(), "45") == 0);
    static_assert(testString.substr(4).size() == 1);
    static_assert(cstrcmp(testString.substr(4).c_str(), "5") == 0);
    static_assert(testString.substr(5).size() == 0);
    static_assert(cstrcmp(testString.substr(5).c_str(), "") == 0);
  }

  SUBCASE("Substr with whitespace") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(5).size() == 6);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), " World") == 0);
    REQUIRE(testString.substr(5, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), " ") == 0);
    REQUIRE(testString.substr(5, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), " W") == 0);
    REQUIRE(testString.substr(5, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(5, 3).c_str(), " Wo") == 0);

    static_assert(testString.substr(5).size() == 6);
    static_assert(cstrcmp(testString.substr(5).c_str(), " World") == 0);
    static_assert(testString.substr(5, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(5, 1).c_str(), " ") == 0);
    static_assert(testString.substr(5, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(5, 2).c_str(), " W") == 0);
    static_assert(testString.substr(5, 3).size() == 3);
    static_assert(cstrcmp(testString.substr(5, 3).c_str(), " Wo") == 0);
  }

  SUBCASE("Substr with newlines") {
    constexpr FixedString<32> testString("Hello\nWorld");

    REQUIRE(testString.substr(5).size() == 6);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "\nWorld") == 0);
    REQUIRE(testString.substr(5, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "\n") == 0);
    REQUIRE(testString.substr(5, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), "\nW") == 0);
    REQUIRE(testString.substr(6).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);

    static_assert(testString.substr(5).size() == 6);
    static_assert(cstrcmp(testString.substr(5).c_str(), "\nWorld") == 0);
    static_assert(testString.substr(5, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(5, 1).c_str(), "\n") == 0);
    static_assert(testString.substr(5, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(5, 2).c_str(), "\nW") == 0);
    static_assert(testString.substr(6).size() == 5);
    static_assert(cstrcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SUBCASE("Substr with tabs") {
    constexpr FixedString<32> testString("Hello\tWorld");

    REQUIRE(testString.substr(5).size() == 6);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "\tWorld") == 0);
    REQUIRE(testString.substr(5, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "\t") == 0);
    REQUIRE(testString.substr(5, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), "\tW") == 0);
    REQUIRE(testString.substr(6).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);

    static_assert(testString.substr(5).size() == 6);
    static_assert(cstrcmp(testString.substr(5).c_str(), "\tWorld") == 0);
    static_assert(testString.substr(5, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(5, 1).c_str(), "\t") == 0);
    static_assert(testString.substr(5, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(5, 2).c_str(), "\tW") == 0);
    static_assert(testString.substr(6).size() == 5);
    static_assert(cstrcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SUBCASE("Substr maximum length") {
    constexpr FixedString<16> testString("123456789012345"); // 15 characters

    REQUIRE(testString.substr(0).size() == 15);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "123456789012345") == 0);
    REQUIRE(testString.substr(0, 15).size() == 15);
    REQUIRE(std::strcmp(testString.substr(0, 15).c_str(), "123456789012345") == 0);
    REQUIRE(testString.substr(0, 16).size() == 15);
    REQUIRE(std::strcmp(testString.substr(0, 16).c_str(), "123456789012345") == 0);
    REQUIRE(testString.substr(14).size() == 1);
    REQUIRE(std::strcmp(testString.substr(14).c_str(), "5") == 0);
    REQUIRE(testString.substr(14, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(14, 1).c_str(), "5") == 0);
    REQUIRE(testString.substr(15).size() == 0);
    REQUIRE(std::strcmp(testString.substr(15).c_str(), "") == 0);

    static_assert(testString.substr(0).size() == 15);
    static_assert(cstrcmp(testString.substr(0).c_str(), "123456789012345") == 0);
    static_assert(testString.substr(0, 15).size() == 15);
    static_assert(cstrcmp(testString.substr(0, 15).c_str(), "123456789012345") == 0);
    static_assert(testString.substr(0, 16).size() == 15);
    static_assert(cstrcmp(testString.substr(0, 16).c_str(), "123456789012345") == 0);
    static_assert(testString.substr(14).size() == 1);
    static_assert(cstrcmp(testString.substr(14).c_str(), "5") == 0);
    static_assert(testString.substr(14, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(14, 1).c_str(), "5") == 0);
    static_assert(testString.substr(15).size() == 0);
    static_assert(cstrcmp(testString.substr(15).c_str(), "") == 0);
  }

  SUBCASE("Substr with mixed content") {
    constexpr FixedString<32> testString("123Hello");

    REQUIRE(testString.substr(0).size() == 8);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "123Hello") == 0);
    REQUIRE(testString.substr(0, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(0, 3).c_str(), "123") == 0);
    REQUIRE(testString.substr(3).size() == 5);
    REQUIRE(std::strcmp(testString.substr(3).c_str(), "Hello") == 0);
    REQUIRE(testString.substr(3, 5).size() == 5);
    REQUIRE(std::strcmp(testString.substr(3, 5).c_str(), "Hello") == 0);
    REQUIRE(testString.substr(2, 4).size() == 4);
    REQUIRE(std::strcmp(testString.substr(2, 4).c_str(), "3Hel") == 0);

    static_assert(testString.substr(0).size() == 8);
    static_assert(cstrcmp(testString.substr(0).c_str(), "123Hello") == 0);
    static_assert(testString.substr(0, 3).size() == 3);
    static_assert(cstrcmp(testString.substr(0, 3).c_str(), "123") == 0);
    static_assert(testString.substr(3).size() == 5);
    static_assert(cstrcmp(testString.substr(3).c_str(), "Hello") == 0);
    static_assert(testString.substr(3, 5).size() == 5);
    static_assert(cstrcmp(testString.substr(3, 5).c_str(), "Hello") == 0);
    static_assert(testString.substr(2, 4).size() == 4);
    static_assert(cstrcmp(testString.substr(2, 4).c_str(), "3Hel") == 0);
  }

  SUBCASE("Substr with overlapping ranges") {
    constexpr FixedString<32> testString("abcdef");

    REQUIRE(testString.substr(0, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "a") == 0);
    REQUIRE(testString.substr(1, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(1, 1).c_str(), "b") == 0);
    REQUIRE(testString.substr(2, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(2, 1).c_str(), "c") == 0);
    REQUIRE(testString.substr(3, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(3, 1).c_str(), "d") == 0);
    REQUIRE(testString.substr(4, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(4, 1).c_str(), "e") == 0);
    REQUIRE(testString.substr(5, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "f") == 0);
    REQUIRE(testString.substr(0, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(0, 2).c_str(), "ab") == 0);
    REQUIRE(testString.substr(1, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(1, 2).c_str(), "bc") == 0);
    REQUIRE(testString.substr(2, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(2, 2).c_str(), "cd") == 0);
    REQUIRE(testString.substr(3, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(3, 2).c_str(), "de") == 0);
    REQUIRE(testString.substr(4, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(4, 2).c_str(), "ef") == 0);

    static_assert(testString.substr(0, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(0, 1).c_str(), "a") == 0);
    static_assert(testString.substr(1, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(1, 1).c_str(), "b") == 0);
    static_assert(testString.substr(2, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(2, 1).c_str(), "c") == 0);
    static_assert(testString.substr(3, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(3, 1).c_str(), "d") == 0);
    static_assert(testString.substr(4, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(4, 1).c_str(), "e") == 0);
    static_assert(testString.substr(5, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(5, 1).c_str(), "f") == 0);
    static_assert(testString.substr(0, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(0, 2).c_str(), "ab") == 0);
    static_assert(testString.substr(1, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(1, 2).c_str(), "bc") == 0);
    static_assert(testString.substr(2, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(2, 2).c_str(), "cd") == 0);
    static_assert(testString.substr(3, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(3, 2).c_str(), "de") == 0);
    static_assert(testString.substr(4, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(4, 2).c_str(), "ef") == 0);
  }

  SUBCASE("Substr with exact string length") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.substr(0, 5).size() == 5);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(testString.substr(0, 6).size() == 5);
    REQUIRE(std::strcmp(testString.substr(0, 6).c_str(), "Hello") == 0);
    REQUIRE(testString.substr(1, 4).size() == 4);
    REQUIRE(std::strcmp(testString.substr(1, 4).c_str(), "ello") == 0);
    REQUIRE(testString.substr(2, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(2, 3).c_str(), "llo") == 0);
    REQUIRE(testString.substr(3, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(3, 2).c_str(), "lo") == 0);
    REQUIRE(testString.substr(4, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(4, 1).c_str(), "o") == 0);

    static_assert(testString.substr(0, 5).size() == 5);
    static_assert(cstrcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    static_assert(testString.substr(0, 6).size() == 5);
    static_assert(cstrcmp(testString.substr(0, 6).c_str(), "Hello") == 0);
    static_assert(testString.substr(1, 4).size() == 4);
    static_assert(cstrcmp(testString.substr(1, 4).c_str(), "ello") == 0);
    static_assert(testString.substr(2, 3).size() == 3);
    static_assert(cstrcmp(testString.substr(2, 3).c_str(), "llo") == 0);
    static_assert(testString.substr(3, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(3, 2).c_str(), "lo") == 0);
    static_assert(testString.substr(4, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(4, 1).c_str(), "o") == 0);
  }

  SUBCASE("Substr with zero count") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(0, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(5, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(10, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(10, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(11, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(11, 0).c_str(), "") == 0);

    static_assert(testString.substr(0, 0).size() == 0);
    static_assert(cstrcmp(testString.substr(0, 0).c_str(), "") == 0);
    static_assert(testString.substr(5, 0).size() == 0);
    static_assert(cstrcmp(testString.substr(5, 0).c_str(), "") == 0);
    static_assert(testString.substr(10, 0).size() == 0);
    static_assert(cstrcmp(testString.substr(10, 0).c_str(), "") == 0);
    static_assert(testString.substr(11, 0).size() == 0);
    static_assert(cstrcmp(testString.substr(11, 0).c_str(), "") == 0);
  }

  SUBCASE("Substr with position at string size") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.substr(5).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "") == 0);
    REQUIRE(testString.substr(5, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(5, 1).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "") == 0);

    static_assert(testString.substr(5).size() == 0);
    static_assert(cstrcmp(testString.substr(5).c_str(), "") == 0);
    static_assert(testString.substr(5, 0).size() == 0);
    static_assert(cstrcmp(testString.substr(5, 0).c_str(), "") == 0);
    static_assert(testString.substr(5, 1).size() == 0);
    static_assert(cstrcmp(testString.substr(5, 1).c_str(), "") == 0);
  }

  SUBCASE("Substr with different FixedString capacities") {
    constexpr FixedString<8> smallString("Hello");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("Hello World Universe");

    REQUIRE(smallString.substr(0, 3).size() == 3);
    REQUIRE(std::strcmp(smallString.substr(0, 3).c_str(), "Hel") == 0);
    REQUIRE(mediumString.substr(0, 5).size() == 5);
    REQUIRE(std::strcmp(mediumString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(largeString.substr(0, 5).size() == 5);
    REQUIRE(std::strcmp(largeString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(smallString.substr(2).size() == 3);
    REQUIRE(std::strcmp(smallString.substr(2).c_str(), "llo") == 0);
    REQUIRE(mediumString.substr(6).size() == 5);
    REQUIRE(std::strcmp(mediumString.substr(6).c_str(), "World") == 0);
    REQUIRE(largeString.substr(12).size() == 8);
    REQUIRE(std::strcmp(largeString.substr(12).c_str(), "Universe") == 0);

    static_assert(smallString.substr(0, 3).size() == 3);
    static_assert(cstrcmp(smallString.substr(0, 3).c_str(), "Hel") == 0);
    static_assert(mediumString.substr(0, 5).size() == 5);
    static_assert(cstrcmp(mediumString.substr(0, 5).c_str(), "Hello") == 0);
    static_assert(largeString.substr(0, 5).size() == 5);
    static_assert(cstrcmp(largeString.substr(0, 5).c_str(), "Hello") == 0);
    static_assert(smallString.substr(2).size() == 3);
    static_assert(cstrcmp(smallString.substr(2).c_str(), "llo") == 0);
    static_assert(mediumString.substr(6).size() == 5);
    static_assert(cstrcmp(mediumString.substr(6).c_str(), "World") == 0);
    static_assert(largeString.substr(12).size() == 8);
    static_assert(cstrcmp(largeString.substr(12).c_str(), "Universe") == 0);
  }

  SUBCASE("Substr with punctuation") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.substr(5, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), ",") == 0);
    REQUIRE(testString.substr(6, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(6, 1).c_str(), " ") == 0);
    REQUIRE(testString.substr(12, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(12, 1).c_str(), "!") == 0);
    REQUIRE(testString.substr(5, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(5, 3).c_str(), ", W") == 0);
    REQUIRE(testString.substr(11, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(11, 2).c_str(), "d!") == 0);

    static_assert(testString.substr(5, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(5, 1).c_str(), ",") == 0);
    static_assert(testString.substr(6, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(6, 1).c_str(), " ") == 0);
    static_assert(testString.substr(12, 1).size() == 1);
    static_assert(cstrcmp(testString.substr(12, 1).c_str(), "!") == 0);
    static_assert(testString.substr(5, 3).size() == 3);
    static_assert(cstrcmp(testString.substr(5, 3).c_str(), ", W") == 0);
    static_assert(testString.substr(11, 2).size() == 2);
    static_assert(cstrcmp(testString.substr(11, 2).c_str(), "d!") == 0);
  }

  SUBCASE("Substr with unicode-like content") {
    constexpr FixedString<32> testString("Hello 世界");

    REQUIRE(testString.substr(0, 6).size() == 6);
    REQUIRE(std::strcmp(testString.substr(0, 6).c_str(), "Hello ") == 0);
    REQUIRE(testString.substr(6).size() == 6);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "世界") == 0);

    static_assert(testString.substr(0, 6).size() == 6);
    static_assert(cstrcmp(testString.substr(0, 6).c_str(), "Hello ") == 0);
    static_assert(testString.substr(6).size() == 6);
    static_assert(cstrcmp(testString.substr(6).c_str(), "世界") == 0);
  }
}

// operator+ concatenation.
TEST_CASE("core/fixed_string/operators_plus") {
  SUBCASE("Basic concatenation tests") {
    constexpr auto testString1 = FixedString<14>("12") + "test text 1";
    constexpr auto testString2 = FixedString<14>("23") + FixedString<14>("test text 2");
    constexpr auto testString3 = FixedString<20>("34") + FixedString<14>("test text 3");
    constexpr auto testString4 = FixedString<20>("45") + FixedString<26>("test text 4");
    constexpr auto testString5 = FixedString<8>("a") + "b";
    constexpr auto testString6 = testString5 + testString5;

    REQUIRE(testString1.size() == 13);
    REQUIRE(std::strcmp(testString1.c_str(), "12test text 1") == 0);
    static_assert(testString1.size() == 13);
    static_assert(cstrcmp(testString1.c_str(), "12test text 1") == 0);

    REQUIRE(testString2.size() == 13);
    REQUIRE(std::strcmp(testString2.c_str(), "23test text 2") == 0);
    static_assert(testString2.size() == 13);
    static_assert(cstrcmp(testString2.c_str(), "23test text 2") == 0);

    REQUIRE(testString3.size() == 13);
    REQUIRE(std::strcmp(testString3.c_str(), "34test text 3") == 0);
    static_assert(testString3.size() == 13);
    static_assert(cstrcmp(testString3.c_str(), "34test text 3") == 0);

    REQUIRE(testString4.size() == 13);
    REQUIRE(std::strcmp(testString4.c_str(), "45test text 4") == 0);
    static_assert(testString4.size() == 13);
    static_assert(cstrcmp(testString4.c_str(), "45test text 4") == 0);

    REQUIRE(testString5.size() == 2);
    REQUIRE(std::strcmp(testString5.c_str(), "ab") == 0);
    static_assert(testString5.size() == 2);
    static_assert(cstrcmp(testString5.c_str(), "ab") == 0);

    REQUIRE(testString6.size() == 4);
    REQUIRE(std::strcmp(testString6.c_str(), "abab") == 0);
    static_assert(testString6.size() == 4);
    static_assert(cstrcmp(testString6.c_str(), "abab") == 0);
  }

  SUBCASE("FixedString + FixedString (same size)") {
    auto result = FixedString<20>("Hello") + FixedString<20>("World");
    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    constexpr auto constexprResult = FixedString<20>("Hello") + FixedString<20>("World");
    static_assert(constexprResult.size() == 10);
    static_assert(cstrcmp(constexprResult.c_str(), "HelloWorld") == 0);
  }

  SUBCASE("FixedString + FixedString (different sizes)") {
    auto result = FixedString<20>("Hello") + FixedString<10>("World");
    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    constexpr auto constexprResult = FixedString<20>("Hello") + FixedString<10>("World");
    static_assert(constexprResult.size() == 10);
    static_assert(cstrcmp(constexprResult.c_str(), "HelloWorld") == 0);
  }

  SUBCASE("FixedString + C-string") {
    auto result = FixedString<20>("Hello") + "World";
    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    constexpr auto constexprResult = FixedString<20>("Hello") + "World";
    static_assert(constexprResult.size() == 10);
    static_assert(cstrcmp(constexprResult.c_str(), "HelloWorld") == 0);
  }

  SUBCASE("C-string + FixedString") {
    auto result = "Hello" + FixedString<20>("World");
    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    constexpr auto constexprResult = "Hello" + FixedString<20>("World");
    static_assert(constexprResult.size() == 10);
    static_assert(cstrcmp(constexprResult.c_str(), "HelloWorld") == 0);
  }

  SUBCASE("FixedString + std::string (StringLike)") {
    auto result = FixedString<20>("Hello") + std::string("World");
    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    constexpr auto constexprResult = FixedString<20>("Hello") + std::string("World");
    static_assert(constexprResult.size() == 10);
    static_assert(cstrcmp(constexprResult.c_str(), "HelloWorld") == 0);
  }

  SUBCASE("std::string + FixedString (StringLike)") {
    auto result = std::string("Hello") + FixedString<20>("World");
    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    constexpr auto constexprResult = std::string("Hello") + FixedString<20>("World");
    static_assert(constexprResult.size() == 10);
    static_assert(cstrcmp(constexprResult.c_str(), "HelloWorld") == 0);
  }

  SUBCASE("Empty string concatenation") {
    auto result = FixedString<20>("") + FixedString<20>("");
    REQUIRE(result.size() == 0);
    REQUIRE(std::strcmp(result.c_str(), "") == 0);

    constexpr auto constexprResult = FixedString<20>("") + FixedString<20>("");
    static_assert(constexprResult.size() == 0);
    static_assert(cstrcmp(constexprResult.c_str(), "") == 0);
  }

  SUBCASE("One empty string concatenation") {
    constexpr FixedString<20> str1("Hello");
    constexpr FixedString<20> str2("");
    constexpr auto result1 = str1 + str2;
    constexpr auto result2 = str2 + str1;

    REQUIRE(result1.size() == 5);
    REQUIRE(std::strcmp(result1.c_str(), "Hello") == 0);
    REQUIRE(result2.size() == 5);
    REQUIRE(std::strcmp(result2.c_str(), "Hello") == 0);

    static_assert(result1.size() == 5);
    static_assert(cstrcmp(result1.c_str(), "Hello") == 0);
    static_assert(result2.size() == 5);
    static_assert(cstrcmp(result2.c_str(), "Hello") == 0);
  }

  SUBCASE("Chained concatenation") {
    constexpr auto result = FixedString<20>("A") + FixedString<20>("B") + FixedString<20>("C");

    REQUIRE(result.size() == 3);
    REQUIRE(std::strcmp(result.c_str(), "ABC") == 0);

    static_assert(result.size() == 3);
    static_assert(cstrcmp(result.c_str(), "ABC") == 0);
  }

  SUBCASE("Constexpr concatenation") {
    constexpr FixedString<20> str1("Hello");
    constexpr FixedString<20> str2("World");
    constexpr auto result = str1 + str2;

    REQUIRE(result.size() == 10);
    REQUIRE(result == "HelloWorld");

    static_assert(result.size() == 10);
    static_assert(cstrcmp(result.c_str(), "HelloWorld") == 0);
  }

  SUBCASE("Edge case: maximum capacity") {
    constexpr auto result = FixedString<5>("AB") + FixedString<5>("CD");

    REQUIRE(result.size() == 4);
    REQUIRE(std::strcmp(result.c_str(), "ABCD") == 0);

    static_assert(result.size() == 4);
    static_assert(cstrcmp(result.c_str(), "ABCD") == 0);
  }

  SUBCASE("Edge case: single character") {
    constexpr auto result = FixedString<20>("A") + FixedString<20>("B");

    REQUIRE(result.size() == 2);
    REQUIRE(std::strcmp(result.c_str(), "AB") == 0);

    static_assert(result.size() == 2);
    static_assert(cstrcmp(result.c_str(), "AB") == 0);
  }

  SUBCASE("FixedString + char") {
    auto result = FixedString<20>("Hello") + '!';
    REQUIRE(result.size() == 6);
    REQUIRE(std::strcmp(result.c_str(), "Hello!") == 0);

    constexpr auto constexprResult = FixedString<20>("Hello") + '!';
    static_assert(constexprResult.size() == 6);
    static_assert(cstrcmp(constexprResult.c_str(), "Hello!") == 0);
  }

  SUBCASE("char + FixedString") {
    auto result = '!' + FixedString<20>("Hello");
    REQUIRE(result.size() == 6);
    REQUIRE(std::strcmp(result.c_str(), "!Hello") == 0);

    constexpr auto constexprResult = '!' + FixedString<20>("Hello");
    static_assert(constexprResult.size() == 6);
    static_assert(cstrcmp(constexprResult.c_str(), "!Hello") == 0);
  }

  SUBCASE("FixedString + char (empty string)") {
    constexpr auto result = FixedString<20>("") + 'A';

    REQUIRE(result.size() == 1);
    REQUIRE(std::strcmp(result.c_str(), "A") == 0);

    static_assert(result.size() == 1);
    static_assert(cstrcmp(result.c_str(), "A") == 0);
  }

  SUBCASE("char + FixedString (empty string)") {
    constexpr auto result = 'A' + FixedString<20>("");

    REQUIRE(result.size() == 1);
    REQUIRE(std::strcmp(result.c_str(), "A") == 0);

    static_assert(result.size() == 1);
    static_assert(cstrcmp(result.c_str(), "A") == 0);
  }

  SUBCASE("FixedString + char (special characters)") {
    constexpr auto result1 = FixedString<20>("Test") + '\n';
    constexpr auto result2 = FixedString<20>("Test") + '\t';
    constexpr auto result3 = FixedString<20>("Test") + ' ';

    REQUIRE(result1.size() == 5);
    REQUIRE(std::strcmp(result1.c_str(), "Test\n") == 0);

    static_assert(result1.size() == 5);
    static_assert(cstrcmp(result1.c_str(), "Test\n") == 0);

    REQUIRE(result2.size() == 5);
    REQUIRE(std::strcmp(result2.c_str(), "Test\t") == 0);

    static_assert(result2.size() == 5);
    static_assert(cstrcmp(result2.c_str(), "Test\t") == 0);

    REQUIRE(result3.size() == 5);
    REQUIRE(std::strcmp(result3.c_str(), "Test ") == 0);

    static_assert(result3.size() == 5);
    static_assert(cstrcmp(result3.c_str(), "Test ") == 0);
  }

  SUBCASE("char + FixedString (special characters)") {
    constexpr auto result1 = '\n' + FixedString<20>("Test");
    constexpr auto result2 = '\t' + FixedString<20>("Test");
    constexpr auto result3 = ' ' + FixedString<20>("Test");

    REQUIRE(result1.size() == 5);
    REQUIRE(std::strcmp(result1.c_str(), "\nTest") == 0);

    static_assert(result1.size() == 5);
    static_assert(cstrcmp(result1.c_str(), "\nTest") == 0);

    REQUIRE(result2.size() == 5);
    REQUIRE(std::strcmp(result2.c_str(), "\tTest") == 0);

    static_assert(result2.size() == 5);
    static_assert(cstrcmp(result2.c_str(), "\tTest") == 0);

    REQUIRE(result3.size() == 5);
    REQUIRE(std::strcmp(result3.c_str(), " Test") == 0);

    static_assert(result3.size() == 5);
    static_assert(cstrcmp(result3.c_str(), " Test") == 0);
  }

  SUBCASE("FixedString + char (numeric characters)") {
    constexpr auto result = FixedString<20>("Number") + '1';

    REQUIRE(result.size() == 7);
    REQUIRE(std::strcmp(result.c_str(), "Number1") == 0);

    static_assert(result.size() == 7);
    static_assert(cstrcmp(result.c_str(), "Number1") == 0);
  }

  SUBCASE("char + FixedString (numeric characters)") {
    constexpr auto result = '1' + FixedString<20>("Number");

    REQUIRE(result.size() == 7);
    REQUIRE(std::strcmp(result.c_str(), "1Number") == 0);

    static_assert(result.size() == 7);
    static_assert(cstrcmp(result.c_str(), "1Number") == 0);
  }

  SUBCASE("FixedString + char (punctuation)") {
    constexpr auto result = FixedString<20>("Hello") + ',';

    REQUIRE(result.size() == 6);
    REQUIRE(std::strcmp(result.c_str(), "Hello,") == 0);

    static_assert(result.size() == 6);
    static_assert(cstrcmp(result.c_str(), "Hello,") == 0);
  }

  SUBCASE("char + FixedString (punctuation)") {
    constexpr auto result = ',' + FixedString<20>("Hello");

    REQUIRE(result.size() == 6);
    REQUIRE(std::strcmp(result.c_str(), ",Hello") == 0);

    static_assert(result.size() == 6);
    static_assert(cstrcmp(result.c_str(), ",Hello") == 0);
  }

  SUBCASE("FixedString + char (chained operations)") {
    constexpr auto result = FixedString<20>("A") + 'B' + 'C';

    REQUIRE(result.size() == 3);
    REQUIRE(std::strcmp(result.c_str(), "ABC") == 0);

    static_assert(result.size() == 3);
    static_assert(cstrcmp(result.c_str(), "ABC") == 0);
  }

  SUBCASE("FixedString + char (maximum capacity)") {
    constexpr auto result = FixedString<6>("ABCD") + 'E';

    REQUIRE(result.size() == 5);
    REQUIRE(std::strcmp(result.c_str(), "ABCDE") == 0);

    static_assert(result.size() == 5);
    static_assert(cstrcmp(result.c_str(), "ABCDE") == 0);
  }

  SUBCASE("char + FixedString (maximum capacity)") {
    constexpr auto result = 'A' + FixedString<6>("BCDE");

    REQUIRE(result.size() == 5);
    REQUIRE(std::strcmp(result.c_str(), "ABCDE") == 0);

    static_assert(result.size() == 5);
    static_assert(cstrcmp(result.c_str(), "ABCDE") == 0);
  }

  SUBCASE("FixedString + char (constexpr operations)") {
    constexpr FixedString<20> str("Hello");
    constexpr char ch = '!';
    constexpr auto result = str + ch;

    REQUIRE(result.size() == 6);
    REQUIRE(result == "Hello!");

    static_assert(result.size() == 6);
    static_assert(cstrcmp(result.c_str(), "Hello!") == 0);
  }

  SUBCASE("char + FixedString (constexpr operations)") {
    constexpr char ch = '!';
    constexpr FixedString<20> str("Hello");
    constexpr auto result = ch + str;

    REQUIRE(result.size() == 6);
    REQUIRE(result == "!Hello");

    static_assert(result.size() == 6);
    static_assert(cstrcmp(result.c_str(), "!Hello") == 0);
  }
}

// operator== and operator!=.
TEST_CASE("core/fixed_string/operator_equal") {
  SUBCASE("FixedString == FixedString") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<32> str2("Hello");
    constexpr FixedString<16> str3("World");
    constexpr FixedString<8> str4("Hello");
    constexpr FixedString<16> empty1;
    constexpr FixedString<32> empty2;

    REQUIRE(str1 == str1);
    REQUIRE(empty1 == empty1);
    REQUIRE(str1 == str2);
    REQUIRE(str2 == str1);
    REQUIRE(str1 == str4);
    REQUIRE(str4 == str1);
    REQUIRE_FALSE(str1 == str3);
    REQUIRE_FALSE(str3 == str1);
    REQUIRE(empty1 == empty2);
    REQUIRE(empty2 == empty1);
    REQUIRE_FALSE(str1 == empty1);
    REQUIRE_FALSE(empty1 == str1);

    static_assert(str1 == str1);
    static_assert(empty1 == empty1);
    static_assert(str1 == str2);
    static_assert(str2 == str1);
    static_assert(str1 == str4);
    static_assert(str4 == str1);
    static_assert(str1 != str3);
    static_assert(str3 != str1);
    static_assert(empty1 == empty2);
    static_assert(empty2 == empty1);
    static_assert(str1 != empty1);
    static_assert(empty1 != str1);
  }

  SUBCASE("FixedString == StringLike") {
    constexpr FixedString<8> str("Hello");
    constexpr FixedString<8> empty;
    const std::string stdHello("Hello");
    const std::string stdWorld("World");
    const std::string stdEmpty;

    REQUIRE(str == stdHello);
    REQUIRE(stdHello == str);
    REQUIRE_FALSE(str == stdWorld);
    REQUIRE_FALSE(stdWorld == str);

    REQUIRE(empty == stdEmpty);
    REQUIRE(stdEmpty == empty);

    REQUIRE_FALSE(stdEmpty == str);
    REQUIRE_FALSE(str == stdEmpty);
    REQUIRE_FALSE(empty == stdHello);
    REQUIRE_FALSE(empty == stdWorld);
  }

  SUBCASE("FixedString == C string") {
    constexpr FixedString<8> str("Hello");
    constexpr FixedString<8> empty;

    REQUIRE(str == "Hello");
    REQUIRE("Hello" == str);
    REQUIRE_FALSE(str == "World");
    REQUIRE_FALSE("World" == str);

    REQUIRE(str == str.c_str());

    REQUIRE(empty == "");
    REQUIRE("" == empty);
    REQUIRE_FALSE(str == "");
    REQUIRE_FALSE("" == str);

    static_assert(str == "Hello");
    static_assert("Hello" == str);
    static_assert(str != "World");
    static_assert("World" != str);
    static_assert(str == str.c_str());
    static_assert(empty == "");
    static_assert("" == empty);
    static_assert(str != "");
    static_assert("" != str);
  }

  SUBCASE("edge_cases") {
    constexpr FixedString<16> str("A");
    constexpr FixedString<16> empty1;
    constexpr FixedString<32> empty2;

    // Different sizes with same content
    constexpr FixedString<8> small("Hi");
    constexpr FixedString<16> large("Hi");

    static_assert(str == "A");
    static_assert("A" == str);
    static_assert(str != "B");
    static_assert("B" != str);

    static_assert(empty1 == empty2);
    static_assert(empty2 == empty1);
    static_assert(empty1 == "");
    static_assert("" == empty1);

    static_assert(small == large);
    static_assert(large == small);
  }

  SUBCASE("special_characters") {
    constexpr FixedString<16> str1("Hello\nWorld");
    constexpr FixedString<16> str2("Hello\tWorld");
    constexpr FixedString<16> str3("Hello World");

    static_assert(str1 == "Hello\nWorld");
    static_assert("Hello\nWorld" == str1);
    static_assert(str2 == "Hello\tWorld");
    static_assert("Hello\tWorld" == str2);
    static_assert(str1 != str2);
    static_assert(str2 != str1);
    static_assert(str1 != str3);
    static_assert(str3 != str1);
  }

  SUBCASE("unicode_content") {
    constexpr FixedString<32> str1("Привет");
    constexpr FixedString<32> str2("Мир");
    constexpr FixedString<32> str3("Привет");

    static_assert(str1 == "Привет");
    static_assert("Привет" == str1);
    static_assert(str1 == str3);
    static_assert(str3 == str1);
    static_assert(str1 != str2);
    static_assert(str2 != str1);
  }

  SUBCASE("Performance test") {
    constexpr FixedString<64> str1("This is a longer string for performance testing");
    constexpr FixedString<64> str2("This is a longer string for performance testing");
    constexpr FixedString<64> str3("This is a different string for performance testing");

    static_assert(str1 == str2);
    static_assert(str2 == str1);
    static_assert(str1 != str3);
    static_assert(str3 != str1);
  }

  SUBCASE("constexpr_operations") {
    constexpr FixedString<16> str1("Test");
    constexpr FixedString<16> str2("Test");
    constexpr FixedString<16> str3("Different");

    constexpr bool eq1 = str1 == str2;
    constexpr bool eq2 = str1 == str3;
    constexpr bool eq3 = str1 == "Test";
    constexpr bool eq4 = "Test" == str1;
    constexpr bool eq5 = str1 == "Different";
    constexpr bool eq6 = "Different" == str1;

    static_assert(eq1);
    static_assert(!eq2);
    static_assert(eq3);
    static_assert(eq4);
    static_assert(!eq5);
    static_assert(!eq6);
  }
}

// operator<=> three-way comparison.
TEST_CASE("core/fixed_string/operator_three_way") {
  SUBCASE("FixedString <=> FixedString") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<32> str2("Hello");
    constexpr FixedString<16> str3("World");
    constexpr FixedString<8> str4("Hello");
    constexpr FixedString<16> str5("Hi");
    constexpr FixedString<16> str6("Hell");

    // Equal strings
    REQUIRE((str1 <=> str1) == strong_ordering::equal);
    REQUIRE((str1 <=> str2) == strong_ordering::equal);
    REQUIRE((str2 <=> str1) == strong_ordering::equal);
    REQUIRE((str1 <=> str4) == strong_ordering::equal);
    REQUIRE((str4 <=> str1) == strong_ordering::equal);

    // Different strings
    REQUIRE((str1 <=> str3) == strong_ordering::less);
    REQUIRE((str3 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> str5) == strong_ordering::less);
    REQUIRE((str5 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> str6) == strong_ordering::greater);
    REQUIRE((str6 <=> str1) == strong_ordering::less);

    static_assert((str1 <=> str1) == strong_ordering::equal);
    static_assert((str1 <=> str2) == strong_ordering::equal);
    static_assert((str2 <=> str1) == strong_ordering::equal);
    static_assert((str1 <=> str4) == strong_ordering::equal);
    static_assert((str4 <=> str1) == strong_ordering::equal);

    static_assert((str1 <=> str3) == strong_ordering::less);
    static_assert((str3 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> str5) == strong_ordering::less);
    static_assert((str5 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> str6) == strong_ordering::greater);
    static_assert((str6 <=> str1) == strong_ordering::less);
  }

  SUBCASE("FixedString <=> StringLike") {
    constexpr FixedString<8> str("Hello");
    constexpr FixedString<16> strFull("Hello World");
    constexpr FixedString<8> empty;
    const std::string stdStr1("Hello");
    const std::string stdStr2("World");
    const std::string stdStr3("Big");
    const std::string stdFull("Hello World");
    const std::string stdEmpty;

    REQUIRE((str <=> stdStr1) == strong_ordering::equal);
    REQUIRE((stdStr1 <=> str) == strong_ordering::equal);
    REQUIRE((str <=> stdStr2) == strong_ordering::less);
    REQUIRE((stdStr2 <=> str) == strong_ordering::greater);
    REQUIRE((str <=> stdStr3) == strong_ordering::greater);
    REQUIRE((stdStr3 <=> str) == strong_ordering::less);
    REQUIRE((str <=> stdEmpty) == strong_ordering::greater);
    REQUIRE((stdEmpty <=> str) == strong_ordering::less);

    REQUIRE((empty <=> stdStr1) == strong_ordering::less);
    REQUIRE((stdStr1 <=> empty) == strong_ordering::greater);
    REQUIRE((empty <=> stdStr2) == strong_ordering::less);
    REQUIRE((stdStr2 <=> empty) == strong_ordering::greater);
    REQUIRE((empty <=> stdStr3) == strong_ordering::less);
    REQUIRE((stdStr3 <=> empty) == strong_ordering::greater);
    REQUIRE((empty <=> stdEmpty) == strong_ordering::equal);
    REQUIRE((stdEmpty <=> empty) == strong_ordering::equal);

    REQUIRE((strFull <=> stdStr1) == strong_ordering::greater);
    REQUIRE((stdStr1 <=> strFull) == strong_ordering::less);
    REQUIRE((str <=> stdFull) == strong_ordering::less);
    REQUIRE((stdFull <=> str) == strong_ordering::greater);
  }

  SUBCASE("FixedString <=> C string") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<16> str2("World");
    constexpr FixedString<16> fullStr("Hello World");

    constexpr const char * cStr1 = "Hello";
    constexpr const char * cStr2 = "World";
    constexpr const char * cStr3 = "Hi";
    constexpr const char * cStr4 = "Hell";
    constexpr const char * strFull = "Hello World";

    REQUIRE((fullStr <=> cStr1) == strong_ordering::greater);
    REQUIRE((cStr1 <=> fullStr) == strong_ordering::less);
    REQUIRE((strFull <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> strFull) == strong_ordering::less);

    // Equal strings
    REQUIRE((str1 <=> cStr1) == strong_ordering::equal);
    REQUIRE((cStr1 <=> str1) == strong_ordering::equal);
    REQUIRE((str2 <=> cStr2) == strong_ordering::equal);
    REQUIRE((cStr2 <=> str2) == strong_ordering::equal);

    // Different strings
    REQUIRE((str1 <=> cStr2) == strong_ordering::less);
    REQUIRE((cStr2 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> cStr3) == strong_ordering::less);
    REQUIRE((cStr3 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> cStr4) == strong_ordering::greater);
    REQUIRE((cStr4 <=> str1) == strong_ordering::less);

    REQUIRE((str2 <=> cStr1) == strong_ordering::greater);
    REQUIRE((cStr1 <=> str2) == strong_ordering::less);
    REQUIRE((str2 <=> cStr3) == strong_ordering::greater);
    REQUIRE((cStr3 <=> str2) == strong_ordering::less);
    REQUIRE((str2 <=> cStr4) == strong_ordering::greater);
    REQUIRE((cStr4 <=> str2) == strong_ordering::less);

    REQUIRE((fullStr <=> cStr2) == strong_ordering::less);
    REQUIRE((cStr2 <=> fullStr) == strong_ordering::greater);

    static_assert((fullStr <=> cStr1) == strong_ordering::greater);
    static_assert((cStr1 <=> fullStr) == strong_ordering::less);
    static_assert((strFull <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> strFull) == strong_ordering::less);

    static_assert((str1 <=> cStr1) == strong_ordering::equal);
    static_assert((cStr1 <=> str1) == strong_ordering::equal);
    static_assert((str2 <=> cStr2) == strong_ordering::equal);
    static_assert((cStr2 <=> str2) == strong_ordering::equal);

    static_assert((str1 <=> cStr2) == strong_ordering::less);
    static_assert((cStr2 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> cStr3) == strong_ordering::less);
    static_assert((cStr3 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> cStr4) == strong_ordering::greater);
    static_assert((cStr4 <=> str1) == strong_ordering::less);

    static_assert((str2 <=> cStr1) == strong_ordering::greater);
    static_assert((cStr1 <=> str2) == strong_ordering::less);
    static_assert((str2 <=> cStr3) == strong_ordering::greater);
    static_assert((cStr3 <=> str2) == strong_ordering::less);
    static_assert((str2 <=> cStr4) == strong_ordering::greater);
    static_assert((cStr4 <=> str2) == strong_ordering::less);

    static_assert((fullStr <=> cStr2) == strong_ordering::less);
    static_assert((cStr2 <=> fullStr) == strong_ordering::greater);
  }

  SUBCASE("Empty string comparisons") {
    constexpr FixedString<16> empty1("");
    constexpr FixedString<32> empty2("");
    constexpr FixedString<16> nonEmpty("Test");
    constexpr const char * emptyCStr = "";
    constexpr const char * nonEmptyCStr = "Test";

    // Empty vs empty
    REQUIRE((empty1 <=> empty2) == strong_ordering::equal);
    REQUIRE((empty2 <=> empty1) == strong_ordering::equal);
    REQUIRE((empty1 <=> emptyCStr) == strong_ordering::equal);
    REQUIRE((emptyCStr <=> empty1) == strong_ordering::equal);

    // Empty vs non-empty
    REQUIRE((empty1 <=> nonEmpty) == strong_ordering::less);
    REQUIRE((nonEmpty <=> empty1) == strong_ordering::greater);
    REQUIRE((empty1 <=> nonEmptyCStr) == strong_ordering::less);
    REQUIRE((nonEmptyCStr <=> empty1) == strong_ordering::greater);
    REQUIRE((emptyCStr <=> nonEmpty) == strong_ordering::less);
    REQUIRE((nonEmpty <=> emptyCStr) == strong_ordering::greater);

    static_assert((empty1 <=> empty2) == strong_ordering::equal);
    static_assert((empty2 <=> empty1) == strong_ordering::equal);
    static_assert((empty1 <=> emptyCStr) == strong_ordering::equal);
    static_assert((emptyCStr <=> empty1) == strong_ordering::equal);

    static_assert((empty1 <=> nonEmpty) == strong_ordering::less);
    static_assert((nonEmpty <=> empty1) == strong_ordering::greater);
    static_assert((empty1 <=> nonEmptyCStr) == strong_ordering::less);
    static_assert((nonEmptyCStr <=> empty1) == strong_ordering::greater);
    static_assert((emptyCStr <=> nonEmpty) == strong_ordering::less);
    static_assert((nonEmpty <=> emptyCStr) == strong_ordering::greater);
  }

  SUBCASE("single_character_strings") {
    constexpr FixedString<8> str1("A");
    constexpr FixedString<8> str2("B");
    constexpr FixedString<8> str3("A");
    constexpr FixedString<8> str4("Z");

    // Equal single characters
    REQUIRE((str1 <=> str3) == strong_ordering::equal);
    REQUIRE((str3 <=> str1) == strong_ordering::equal);

    // Different single characters
    REQUIRE((str1 <=> str2) == strong_ordering::less);
    REQUIRE((str2 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> str4) == strong_ordering::less);
    REQUIRE((str4 <=> str1) == strong_ordering::greater);
    REQUIRE((str2 <=> str4) == strong_ordering::less);
    REQUIRE((str4 <=> str2) == strong_ordering::greater);

    static_assert((str1 <=> str3) == strong_ordering::equal);
    static_assert((str3 <=> str1) == strong_ordering::equal);

    static_assert((str1 <=> str2) == strong_ordering::less);
    static_assert((str2 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> str4) == strong_ordering::less);
    static_assert((str4 <=> str1) == strong_ordering::greater);
    static_assert((str2 <=> str4) == strong_ordering::less);
    static_assert((str4 <=> str2) == strong_ordering::greater);
  }

  SUBCASE("case_sensitivity") {
    constexpr FixedString<16> lower("hello");
    constexpr FixedString<16> upper("HELLO");
    constexpr FixedString<16> mixed("Hello");

    // Case-sensitive comparisons
    REQUIRE((lower <=> upper) == strong_ordering::greater); // 'h' > 'H' in ASCII
    REQUIRE((upper <=> lower) == strong_ordering::less);
    REQUIRE((lower <=> mixed) == strong_ordering::greater); // 'h' > 'H' in ASCII
    REQUIRE((mixed <=> lower) == strong_ordering::less);
    REQUIRE((upper <=> mixed) == strong_ordering::less); // 'H' < 'H' (same), but 'E' < 'e'
    REQUIRE((mixed <=> upper) == strong_ordering::greater);

    static_assert((lower <=> upper) == strong_ordering::greater);
    static_assert((upper <=> lower) == strong_ordering::less);
    static_assert((lower <=> mixed) == strong_ordering::greater);
    static_assert((mixed <=> lower) == strong_ordering::less);
    static_assert((upper <=> mixed) == strong_ordering::less);
    static_assert((mixed <=> upper) == strong_ordering::greater);
  }

  SUBCASE("Prefix comparisons") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<16> str2("HelloWorld");
    constexpr FixedString<16> str3("Hell");
    constexpr FixedString<16> str4("Hello");

    // One string is prefix of another
    REQUIRE((str1 <=> str2) == strong_ordering::less); // "Hello" < "HelloWorld"
    REQUIRE((str2 <=> str1) == strong_ordering::greater);
    REQUIRE((str3 <=> str1) == strong_ordering::less); // "Hell" < "Hello"
    REQUIRE((str1 <=> str3) == strong_ordering::greater);

    // Equal strings
    REQUIRE((str1 <=> str4) == strong_ordering::equal);
    REQUIRE((str4 <=> str1) == strong_ordering::equal);

    static_assert((str1 <=> str2) == strong_ordering::less);
    static_assert((str2 <=> str1) == strong_ordering::greater);
    static_assert((str3 <=> str1) == strong_ordering::less);
    static_assert((str1 <=> str3) == strong_ordering::greater);

    static_assert((str1 <=> str4) == strong_ordering::equal);
    static_assert((str4 <=> str1) == strong_ordering::equal);
  }

  SUBCASE("special_characters") {
    constexpr FixedString<32> str1("Hello\nWorld");
    constexpr FixedString<32> str2("Hello\tWorld");
    constexpr FixedString<32> str3("Hello World");
    constexpr FixedString<32> str4("Hello\nWorld");

    // Equal strings with special characters
    REQUIRE((str1 <=> str4) == strong_ordering::equal);
    REQUIRE((str4 <=> str1) == strong_ordering::equal);

    // Different special characters
    REQUIRE((str1 <=> str2) == strong_ordering::greater); // '\n' > '\t' in ASCII
    REQUIRE((str2 <=> str1) == strong_ordering::less);
    REQUIRE((str1 <=> str3) == strong_ordering::less); // '\n' < ' ' in ASCII
    REQUIRE((str3 <=> str1) == strong_ordering::greater);
    REQUIRE((str2 <=> str3) == strong_ordering::less); // '\t' < ' ' in ASCII
    REQUIRE((str3 <=> str2) == strong_ordering::greater);

    static_assert((str1 <=> str4) == strong_ordering::equal);
    static_assert((str4 <=> str1) == strong_ordering::equal);

    static_assert((str1 <=> str2) == strong_ordering::greater);
    static_assert((str2 <=> str1) == strong_ordering::less);
    static_assert((str1 <=> str3) == strong_ordering::less);
    static_assert((str3 <=> str1) == strong_ordering::greater);
    static_assert((str2 <=> str3) == strong_ordering::less);
    static_assert((str3 <=> str2) == strong_ordering::greater);
  }

  SUBCASE("unicode_content") {
    constexpr FixedString<32> str1("Привет");
    constexpr FixedString<32> str2("Мир");
    constexpr FixedString<32> str3("Привет");
    constexpr FixedString<32> str4("Hello 🌍");

    // Equal Unicode strings
    REQUIRE((str1 <=> str3) == strong_ordering::equal);
    REQUIRE((str3 <=> str1) == strong_ordering::equal);

    // Different Unicode strings
    REQUIRE((str1 <=> str2) != strong_ordering::equal);
    REQUIRE((str2 <=> str1) != strong_ordering::equal);
    REQUIRE((str1 <=> str4) != strong_ordering::equal);
    REQUIRE((str4 <=> str1) != strong_ordering::equal);

    static_assert((str1 <=> str3) == strong_ordering::equal);
    static_assert((str3 <=> str1) == strong_ordering::equal);

    static_assert((str1 <=> str2) != strong_ordering::equal);
    static_assert((str2 <=> str1) != strong_ordering::equal);
    static_assert((str1 <=> str4) != strong_ordering::equal);
    static_assert((str4 <=> str1) != strong_ordering::equal);
  }

  SUBCASE("Numeric strings") {
    constexpr FixedString<16> str1("123");
    constexpr FixedString<16> str2("456");
    constexpr FixedString<16> str3("123");
    constexpr FixedString<16> str4("12");
    constexpr FixedString<16> str5("1234");

    // Equal numeric strings
    REQUIRE((str1 <=> str3) == strong_ordering::equal);
    REQUIRE((str3 <=> str1) == strong_ordering::equal);

    // Different numeric strings
    REQUIRE((str1 <=> str2) == strong_ordering::less); // "123" < "456"
    REQUIRE((str2 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> str4) == strong_ordering::greater); // "123" > "12"
    REQUIRE((str4 <=> str1) == strong_ordering::less);
    REQUIRE((str1 <=> str5) == strong_ordering::less); // "123" < "1234"
    REQUIRE((str5 <=> str1) == strong_ordering::greater);

    static_assert((str1 <=> str3) == strong_ordering::equal);
    static_assert((str3 <=> str1) == strong_ordering::equal);

    static_assert((str1 <=> str2) == strong_ordering::less);
    static_assert((str2 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> str4) == strong_ordering::greater);
    static_assert((str4 <=> str1) == strong_ordering::less);
    static_assert((str1 <=> str5) == strong_ordering::less);
    static_assert((str5 <=> str1) == strong_ordering::greater);
  }

  SUBCASE("edge_cases") {
    constexpr FixedString<4> str1("ABC"); // Maximum length for capacity 4
    constexpr FixedString<8> str2("ABC");
    constexpr FixedString<16> str3("ABCD");

    // Same content, different capacities
    REQUIRE((str1 <=> str2) == strong_ordering::equal);
    REQUIRE((str2 <=> str1) == strong_ordering::equal);

    // Different lengths
    REQUIRE((str1 <=> str3) == strong_ordering::less); // "ABC" < "ABCD"
    REQUIRE((str3 <=> str1) == strong_ordering::greater);

    static_assert((str1 <=> str2) == strong_ordering::equal);
    static_assert((str2 <=> str1) == strong_ordering::equal);

    static_assert((str1 <=> str3) == strong_ordering::less);
    static_assert((str3 <=> str1) == strong_ordering::greater);
  }

  SUBCASE("constexpr_operations") {
    constexpr FixedString<16> str1("Test");
    constexpr FixedString<16> str2("Test");
    constexpr FixedString<16> str3("Different");
    constexpr FixedString<16> str4("Test");
    constexpr const char * str5 = "Test";
    constexpr const char * str6 = "Different";

    constexpr auto eq1 = str1 <=> str2;
    constexpr auto eq2 = str1 <=> str3;
    constexpr auto eq3 = str1 <=> str4;
    constexpr auto eq4 = str1 <=> str5;
    constexpr auto eq5 = str1 <=> str6;
    constexpr auto eq6 = str5 <=> str1;

    REQUIRE(eq1 == strong_ordering::equal);
    REQUIRE(eq2 != strong_ordering::equal);
    REQUIRE(eq3 == strong_ordering::equal);
    REQUIRE(eq4 == strong_ordering::equal);
    REQUIRE(eq5 != strong_ordering::equal);
    REQUIRE(eq6 == strong_ordering::equal);

    static_assert(eq1 == strong_ordering::equal);
    static_assert(eq2 != strong_ordering::equal);
    static_assert(eq3 == strong_ordering::equal);
    static_assert(eq4 == strong_ordering::equal);
    static_assert(eq5 != strong_ordering::equal);
    static_assert(eq6 == strong_ordering::equal);
  }

  SUBCASE("Performance test") {
    constexpr FixedString<64> str1("This is a very long string for performance testing");
    constexpr FixedString<64> str2("This is a very long string for performance testing");
    constexpr FixedString<64> str3("This is a very long string for performance testing!");
    constexpr FixedString<64> str4("This is a different string for performance testing");

    // Equal long strings
    REQUIRE((str1 <=> str2) == strong_ordering::equal);
    REQUIRE((str2 <=> str1) == strong_ordering::equal);

    // Different long strings
    REQUIRE((str1 <=> str3) == strong_ordering::less); // Missing '!' at end
    REQUIRE((str3 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> str4) != strong_ordering::equal); // Different content
    REQUIRE((str4 <=> str1) != strong_ordering::equal);

    static_assert((str1 <=> str2) == strong_ordering::equal);
    static_assert((str2 <=> str1) == strong_ordering::equal);
    static_assert((str1 <=> str3) == strong_ordering::less);
    static_assert((str3 <=> str1) == strong_ordering::greater);
  }
}

// std::swap() overload.
TEST_CASE("core/fixed_string/std_swap") {
  SUBCASE("Basic swap functionality") {
    FixedString<32> str1("Hello");
    FixedString<32> str2("World");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "World") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);
  }

  SUBCASE("Swap with empty strings") {
    FixedString<32> str1("Hello");
    FixedString<32> str2("");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);
  }

  SUBCASE("swap_two_empty_strings") {
    FixedString<32> str1("");
    FixedString<32> str2("");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
    REQUIRE(str2.size() == 0);
    REQUIRE(std::strcmp(str2.c_str(), "") == 0);
  }

  SUBCASE("Self-swap") {
    FixedString<32> str1("Hello");

    std::swap(str1, str1);

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);
  }

  SUBCASE("Swap with different sizes") {
    FixedString<32> str1("Hi");
    FixedString<32> str2("VeryLongString");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 14);
    REQUIRE(std::strcmp(str1.c_str(), "VeryLongString") == 0);
    REQUIRE(str2.size() == 2);
    REQUIRE(std::strcmp(str2.c_str(), "Hi") == 0);
  }

  SUBCASE("swap_with_maximum_length_strings") {
    FixedString<16> str1("123456789012345"); // 15 chars
    FixedString<16> str2("ABCDEFGHIJKLMNO"); // 15 chars

    std::swap(str1, str2);

    REQUIRE(str1.size() == 15);
    REQUIRE(std::strcmp(str1.c_str(), "ABCDEFGHIJKLMNO") == 0);
    REQUIRE(str2.size() == 15);
    REQUIRE(std::strcmp(str2.c_str(), "123456789012345") == 0);
  }

  SUBCASE("swap_with_special_characters") {
    FixedString<32> str1("Hello,\n\t!");
    FixedString<32> str2("World,\r\n?");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 9);
    REQUIRE(std::strcmp(str1.c_str(), "World,\r\n?") == 0);
    REQUIRE(str2.size() == 9);
    REQUIRE(std::strcmp(str2.c_str(), "Hello,\n\t!") == 0);
  }

  SUBCASE("swap_with_unicode_content") {
    FixedString<32> str1("Hello 世界");
    FixedString<32> str2("World 宇宙");

    std::swap(str1, str2);

    REQUIRE(str1.size() == std::char_traits<char>::length("World 宇宙"));
    REQUIRE(std::strcmp(str1.c_str(), "World 宇宙") == 0);
    REQUIRE(str2.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(std::strcmp(str2.c_str(), "Hello 世界") == 0);
  }

  SUBCASE("Multiple swaps") {
    FixedString<32> str1("First");
    FixedString<32> str2("Second");
    FixedString<32> str3("Third");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 6);
    REQUIRE(std::strcmp(str1.c_str(), "Second") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "First") == 0);

    std::swap(str2, str3);

    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Third") == 0);
    REQUIRE(str3.size() == 5);
    REQUIRE(std::strcmp(str3.c_str(), "First") == 0);

    std::swap(str1, str3);

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "First") == 0);
    REQUIRE(str3.size() == 6);
    REQUIRE(std::strcmp(str3.c_str(), "Second") == 0);
  }

  SUBCASE("Performance test with large strings") {
    FixedString<64> str1("This is a very long string that tests swap performance");
    FixedString<64> str2("Another very long string for performance testing");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 48);
    REQUIRE(std::strcmp(str1.c_str(), "Another very long string for performance testing") == 0);
    REQUIRE(str2.size() == 54);
    REQUIRE(std::strcmp(str2.c_str(), "This is a very long string that tests swap performance") == 0);
  }

  SUBCASE("swap_with_single_character_strings") {
    FixedString<8> str1("A");
    FixedString<8> str2("B");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "B") == 0);
    REQUIRE(str2.size() == 1);
    REQUIRE(std::strcmp(str2.c_str(), "A") == 0);
  }
}

} // namespace toy
*/
