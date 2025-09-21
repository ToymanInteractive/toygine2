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

TEST_CASE("FixedString constructors", "[core][fixed_string]") {
  SECTION("Default constructor") {
    constexpr FixedString<32> emptyStr;

    REQUIRE(emptyStr.size() == 0);
    REQUIRE(std::strcmp(emptyStr.c_str(), "") == 0);
    REQUIRE(emptyStr.capacity() == 31);
    REQUIRE(emptyStr.max_size() == 31);

    // Compile-time checks
    STATIC_REQUIRE(emptyStr.size() == 0);
    STATIC_REQUIRE(cstrcmp(emptyStr.c_str(), "") == 0);
    STATIC_REQUIRE(emptyStr.capacity() == 31);
    STATIC_REQUIRE(emptyStr.max_size() == 31);
  }

  SECTION("C string constructor") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<32> str2("World");
    constexpr FixedString<8> str3("Test");
    constexpr FixedString<64> str4("This is a longer string for testing");

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "World") == 0);
    REQUIRE(str3.size() == 4);
    REQUIRE(std::strcmp(str3.c_str(), "Test") == 0);
    REQUIRE(str4.size() == 35);
    REQUIRE(std::strcmp(str4.c_str(), "This is a longer string for testing") == 0);

    // Compile-time checks
    STATIC_REQUIRE(str1.size() == 5);
    STATIC_REQUIRE(cstrcmp(str1.c_str(), "Hello") == 0);
    STATIC_REQUIRE(str2.size() == 5);
    STATIC_REQUIRE(cstrcmp(str2.c_str(), "World") == 0);
    STATIC_REQUIRE(str3.size() == 4);
    STATIC_REQUIRE(cstrcmp(str3.c_str(), "Test") == 0);
    STATIC_REQUIRE(str4.size() == 35);
    STATIC_REQUIRE(cstrcmp(str4.c_str(), "This is a longer string for testing") == 0);
  }

  SECTION("Copy constructor") {
    constexpr FixedString<16> original("CopyTest");
    constexpr FixedString<16> copy1(original);
    constexpr FixedString<32> copy2(original);
    constexpr FixedString<12> copy3(original);

    REQUIRE(copy1.size() == 8);
    REQUIRE(std::strcmp(copy1.c_str(), "CopyTest") == 0);
    REQUIRE(copy2.size() == 8);
    REQUIRE(std::strcmp(copy2.c_str(), "CopyTest") == 0);
    REQUIRE(copy3.size() == 8);
    REQUIRE(std::strcmp(copy3.c_str(), "CopyTest") == 0);

    // Compile-time checks
    STATIC_REQUIRE(copy1.size() == 8);
    STATIC_REQUIRE(cstrcmp(copy1.c_str(), "CopyTest") == 0);
    STATIC_REQUIRE(copy2.size() == 8);
    STATIC_REQUIRE(cstrcmp(copy2.c_str(), "CopyTest") == 0);
    STATIC_REQUIRE(copy3.size() == 8);
    STATIC_REQUIRE(cstrcmp(copy3.c_str(), "CopyTest") == 0);

    // Verify independence
    REQUIRE(copy1 == original);
    REQUIRE(copy2 == original);
    REQUIRE(copy3 == original);

    // Compile-time checks
    STATIC_REQUIRE(copy1 == original);
    STATIC_REQUIRE(copy2 == original);
    STATIC_REQUIRE(copy3 == original);
  }

  SECTION("Character constructor") {
    constexpr FixedString<16> single('A');
    constexpr FixedString<32> multiple('B', 5);
    constexpr FixedString<8> many('C', 7);
    constexpr FixedString<64> empty('D', 0);

    REQUIRE(single.size() == 1);
    REQUIRE(std::strcmp(single.c_str(), "A") == 0);
    REQUIRE(multiple.size() == 5);
    REQUIRE(std::strcmp(multiple.c_str(), "BBBBB") == 0);
    REQUIRE(many.size() == 7);
    REQUIRE(std::strcmp(many.c_str(), "CCCCCCC") == 0);
    REQUIRE(empty.size() == 0);
    REQUIRE(std::strcmp(empty.c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(single.size() == 1);
    STATIC_REQUIRE(cstrcmp(single.c_str(), "A") == 0);
    STATIC_REQUIRE(multiple.size() == 5);
    STATIC_REQUIRE(cstrcmp(multiple.c_str(), "BBBBB") == 0);
    STATIC_REQUIRE(many.size() == 7);
    STATIC_REQUIRE(cstrcmp(many.c_str(), "CCCCCCC") == 0);
    STATIC_REQUIRE(empty.size() == 0);
    STATIC_REQUIRE(cstrcmp(empty.c_str(), "") == 0);
  }

  SECTION("StringLike constructor") {
    const FixedString<16> fromStd(std::string("StringLike"));
    constexpr FixedString<16> fromFix(CStringView("StringLike"));

    REQUIRE(fromStd.size() == 10);
    REQUIRE(std::strcmp(fromStd.c_str(), "StringLike") == 0);

    // Compile-time checks
    STATIC_REQUIRE(fromFix.size() == 10);
    STATIC_REQUIRE(cstrcmp(fromFix.c_str(), "StringLike") == 0);
  }

  SECTION("Edge cases") {
    // Empty string
    constexpr FixedString<16> empty1("");
    constexpr FixedString<32> empty2("");

    REQUIRE(empty1.size() == 0);
    REQUIRE(empty2.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(empty1.size() == 0);
    STATIC_REQUIRE(empty2.size() == 0);

    // Single character
    constexpr FixedString<8> single("X");

    REQUIRE(single.size() == 1);
    REQUIRE(std::strcmp(single.c_str(), "X") == 0);

    // Compile-time checks
    STATIC_REQUIRE(single.size() == 1);
    STATIC_REQUIRE(cstrcmp(single.c_str(), "X") == 0);

    // Maximum length
    constexpr FixedString<5> maxLen("Test");

    REQUIRE(maxLen.size() == 4);
    REQUIRE(std::strcmp(maxLen.c_str(), "Test") == 0);

    // Compile-time checks
    STATIC_REQUIRE(maxLen.size() == 4);
    STATIC_REQUIRE(cstrcmp(maxLen.c_str(), "Test") == 0);
  }

  SECTION("Special characters") {
    constexpr FixedString<32> newline("Line1\nLine2");
    constexpr FixedString<32> tab("Col1\tCol2");
    constexpr FixedString<32> mixed("Mix\t\nEnd");

    REQUIRE(newline.size() == 11);
    REQUIRE(std::strcmp(newline.c_str(), "Line1\nLine2") == 0);
    REQUIRE(tab.size() == 9);
    REQUIRE(std::strcmp(tab.c_str(), "Col1\tCol2") == 0);
    REQUIRE(mixed.size() == 8);
    REQUIRE(std::strcmp(mixed.c_str(), "Mix\t\nEnd") == 0);

    // Compile-time checks
    STATIC_REQUIRE(newline.size() == 11);
    STATIC_REQUIRE(cstrcmp(newline.c_str(), "Line1\nLine2") == 0);
    STATIC_REQUIRE(tab.size() == 9);
    STATIC_REQUIRE(cstrcmp(tab.c_str(), "Col1\tCol2") == 0);
    STATIC_REQUIRE(mixed.size() == 8);
    STATIC_REQUIRE(cstrcmp(mixed.c_str(), "Mix\t\nEnd") == 0);
  }

  SECTION("Unicode content") {
    constexpr FixedString<64> unicode("Привет мир");
    constexpr FixedString<32> emoji("Hello 🌍");

    REQUIRE(unicode.size() == 19); // UTF-8 bytes
    REQUIRE(std::strcmp(unicode.c_str(), "Привет мир") == 0);
    REQUIRE(emoji.size() == 10); // UTF-8 bytes
    REQUIRE(std::strcmp(emoji.c_str(), "Hello 🌍") == 0);

    // Compile-time checks
    STATIC_REQUIRE(unicode.size() == 19);
    STATIC_REQUIRE(cstrcmp(unicode.c_str(), "Привет мир") == 0);
    STATIC_REQUIRE(emoji.size() == 10);
    STATIC_REQUIRE(cstrcmp(emoji.c_str(), "Hello 🌍") == 0);
  }
}

TEST_CASE("FixedString operators=", "[core][fixed_string]") {
  SECTION("C string assignment") {
    FixedString<12> str1;
    FixedString<24> str2;
    FixedString<8> str3;

    str1 = "Hello";
    str2 = "World";
    str3 = "Test";

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "World") == 0);
    REQUIRE(str3.size() == 4);
    REQUIRE(std::strcmp(str3.c_str(), "Test") == 0);

    // Empty string assignment
    str1 = "";
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Long string assignment
    str2 = "This is a longer string";
    REQUIRE(str2.size() == 23);
    REQUIRE(std::strcmp(str2.c_str(), "This is a longer string") == 0);

    // Compile-time checks
    constexpr FixedString<24> constStr1 = "This is a longer string";
    STATIC_REQUIRE(constStr1.size() == 23);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "This is a longer string") == 0);
  }

  SECTION("FixedString assignment (same capacity)") {
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

    // Compile-time checks
    constexpr FixedString<24> constStr1("Hello");
    constexpr FixedString<24> constStr2 = constStr1;
    STATIC_REQUIRE(constStr2.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Hello") == 0);
  }

  SECTION("FixedString assignment (different capacities)") {
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

    // Compile-time checks
    constexpr FixedString<8> constStr1("Test");
    constexpr FixedString<32> constStr2 = constStr1;
    constexpr FixedString<16> constStr3 = constStr2;
    STATIC_REQUIRE(constStr2.size() == 4);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Test") == 0);
    STATIC_REQUIRE(constStr3.size() == 4);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "Test") == 0);
  }

  SECTION("StringLike assignment") {
    FixedString<12> str1;
    FixedString<24> str2;

    str1 = std::string("Hello World");
    REQUIRE(str1.size() == 11);
    REQUIRE(std::strcmp(str1.c_str(), "Hello World") == 0);

    str2 = FixedString<16>("Test String");
    REQUIRE(str2.size() == 11);
    REQUIRE(std::strcmp(str2.c_str(), "Test String") == 0);
  }

  SECTION("Character assignment") {
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

  SECTION("Edge cases") {
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

  SECTION("Special characters") {
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

  SECTION("Unicode content") {
    FixedString<32> str1;
    FixedString<48> str2;

    str1 = "Привет";
    REQUIRE(str1.size() == 12); // UTF-8 encoding
    REQUIRE(std::strcmp(str1.c_str(), "Привет") == 0);

    str2 = "Hello 🌍";
    REQUIRE(str2.size() == 10); // UTF-8 encoding
    REQUIRE(std::strcmp(str2.c_str(), "Hello 🌍") == 0);

    // Assignment between Unicode strings
    str1 = str2;
    REQUIRE(str1.size() == 10);
    REQUIRE(std::strcmp(str1.c_str(), "Hello 🌍") == 0);
  }
}

TEST_CASE("FixedString assign", "[core][fixed_string]") {
  SECTION("C string assignment") {
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

    // Compile-time checks
    constexpr auto constStr1 = FixedString<16>().assign("Hello");
    constexpr auto constStr2 = FixedString<32>("World").assign("VeryLongString");
    constexpr auto constStr3 = FixedString<16>("A").assign("");
    STATIC_REQUIRE(constStr1.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello") == 0);
    STATIC_REQUIRE(constStr2.size() == 14);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "VeryLongString") == 0);
    STATIC_REQUIRE(constStr3.size() == 0);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "") == 0);
  }

  SECTION("FixedString assignment (same capacity)") {
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

    // Compile-time checks
    constexpr FixedString<16> constStr1("Hello");
    constexpr auto constStr2 = FixedString<16>("World").assign(constStr1);
    constexpr auto constStr3 = FixedString<16>().assign(constStr2);
    STATIC_REQUIRE(constStr2.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Hello") == 0);
    STATIC_REQUIRE(constStr3.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "Hello") == 0);
  }

  SECTION("FixedString assignment (different capacities)") {
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

    // Compile-time checks
    constexpr FixedString<8> constStr1("Hi");
    constexpr FixedString<16> constStr2("Hello");
    constexpr auto constStr3 = FixedString<8>("Hi").assign(constStr2);
    constexpr auto constStr4 = FixedString<16>("Hello").assign(constStr1);
    STATIC_REQUIRE(constStr3.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "Hello") == 0);
    STATIC_REQUIRE(constStr4.size() == 2);
    STATIC_REQUIRE(cstrcmp(constStr4.c_str(), "Hi") == 0);
  }

  SECTION("StringLike assignment") {
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

  SECTION("Character assignment") {
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

    // Compile-time checks
    constexpr auto constStr1 = FixedString<8>().assign('A');
    constexpr auto constStr2 = FixedString<8>().assign('B', 3);
    STATIC_REQUIRE(constStr1.size() == 1);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "A") == 0);
    STATIC_REQUIRE(constStr2.size() == 3);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "BBB") == 0);
  }

  SECTION("Edge cases") {
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

    // Compile-time checks
    constexpr auto constStr1 = FixedString<4>("ABC").assign("XYZ");
    constexpr auto constStr2 = FixedString<8>("ABCD").assign("");
    STATIC_REQUIRE(constStr1.size() == 3);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "XYZ") == 0);
    STATIC_REQUIRE(constStr2.size() == 0);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "") == 0);
  }

  SECTION("Special characters") {
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

    // Compile-time checks
    constexpr auto constStr1 = FixedString<32>().assign("Hello\n\tWorld");
    constexpr auto constStr2 = FixedString<16>().assign("!@#$%^&*()");
    STATIC_REQUIRE(constStr1.size() == 12);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello\n\tWorld") == 0);
    STATIC_REQUIRE(constStr2.size() == 10);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "!@#$%^&*()") == 0);
  }

  SECTION("Unicode content") {
    FixedString<32> str1;
    FixedString<16> str2;

    // Unicode characters
    str1.assign("Hello 世界");
    REQUIRE(str1.size() == 12);
    REQUIRE(std::strcmp(str1.c_str(), "Hello 世界") == 0);

    // Mixed ASCII and Unicode
    str2.assign("Test 🌍");
    REQUIRE(str2.size() == 9);
    REQUIRE(std::strcmp(str2.c_str(), "Test 🌍") == 0);

    // Compile-time checks
    constexpr auto constStr1 = FixedString<32>().assign("Hello 世界");
    constexpr auto constStr2 = FixedString<16>().assign("Test 🌍");
    STATIC_REQUIRE(constStr1.size() == 12);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello 世界") == 0);
    STATIC_REQUIRE(constStr2.size() == 9);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Test 🌍") == 0);
  }

  SECTION("Chaining assign") {
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

    // Compile-time checks
    constexpr auto constStr1 = FixedString<16>("a").assign("b");
    constexpr auto constStr2 = FixedString<16>("Hello").assign("Test");
    STATIC_REQUIRE(constStr1.size() == 1);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "b") == 0);
    STATIC_REQUIRE(constStr2.size() == 4);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Test") == 0);
  }
}

TEST_CASE("FixedString at", "[core][fixed_string]") {
  SECTION("Basic at() functionality") {
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

  SECTION("Const at() access") {
    constexpr FixedString<8> str("World");

    REQUIRE(str.at(0) == 'W');
    REQUIRE(str.at(1) == 'o');
    REQUIRE(str.at(2) == 'r');
    REQUIRE(str.at(3) == 'l');
    REQUIRE(str.at(4) == 'd');

    // Compile-time checks
    STATIC_REQUIRE(str.at(0) == 'W');
    STATIC_REQUIRE(str.at(1) == 'o');
    STATIC_REQUIRE(str.at(2) == 'r');
    STATIC_REQUIRE(str.at(3) == 'l');
    STATIC_REQUIRE(str.at(4) == 'd');
  }

  SECTION("empty string") {
    constexpr FixedString<8> str;

    REQUIRE(str.at(0) == '\0');

    // Compile-time checks
    STATIC_REQUIRE(str.at(0) == '\0');
  }

  SECTION("Reference modification") {
    FixedString<8> str("Test");

    REQUIRE(str.at(0) == 'T');

    // Get reference and modify
    auto & ref = str.at(0);
    ref = 'B';

    REQUIRE(str.at(0) == 'B');

    REQUIRE(str.size() == 4);
    REQUIRE(std::strcmp(str.c_str(), "Best") == 0);
  }

  SECTION("Sequential modifications") {
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

  SECTION("Single character string") {
    FixedString<8> str("A");

    REQUIRE(str.at(0) == 'A');

    str.at(0) = 'B';

    REQUIRE(str.at(0) == 'B');

    REQUIRE(str.size() == 1);
    REQUIRE(std::strcmp(str.c_str(), "B") == 0);
  }

  SECTION("Special characters") {
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

  SECTION("Unicode content") {
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

  SECTION("Maximum length string") {
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

TEST_CASE("FixedString operator[]", "[core][fixed_string]") {
  SECTION("Non-const operator[] - basic access") {
    FixedString<16> str("Hello");
    constexpr FixedString<16> constStr("World");

    // Basic access
    REQUIRE(str[0] == 'H');
    REQUIRE(str[1] == 'e');
    REQUIRE(str[2] == 'l');
    REQUIRE(str[3] == 'l');
    REQUIRE(str[4] == 'o');

    // Compile-time checks
    STATIC_REQUIRE(constStr[0] == 'W');
    STATIC_REQUIRE(constStr[1] == 'o');
    STATIC_REQUIRE(constStr[2] == 'r');
    STATIC_REQUIRE(constStr[3] == 'l');
    STATIC_REQUIRE(constStr[4] == 'd');
  }

  SECTION("Non-const operator[] - modification") {
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

  SECTION("Non-const operator[] - reference modification") {
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

  SECTION("Const operator[] - read-only access") {
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

    // Compile-time checks
    STATIC_REQUIRE(str[0] == 'H');
    STATIC_REQUIRE(str[1] == 'e');
    STATIC_REQUIRE(str[2] == 'l');
    STATIC_REQUIRE(str[3] == 'l');
    STATIC_REQUIRE(str[4] == 'o');

    STATIC_REQUIRE(longStr[0] == 'V');
    STATIC_REQUIRE(longStr[1] == 'e');
    STATIC_REQUIRE(longStr[2] == 'r');
    STATIC_REQUIRE(longStr[3] == 'y');
    STATIC_REQUIRE(longStr[4] == 'L');
  }

  SECTION("Single character strings") {
    FixedString<8> str("A");
    constexpr FixedString<8> constStr("B");

    // Non-const access
    REQUIRE(str[0] == 'A');
    str[0] = 'X';
    REQUIRE(str[0] == 'X');
    REQUIRE(std::strcmp(str.c_str(), "X") == 0);

    // Const access
    REQUIRE(constStr[0] == 'B');

    // Compile-time checks
    STATIC_REQUIRE(constStr[0] == 'B');
  }

  SECTION("Special characters") {
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

    // Compile-time checks
    STATIC_REQUIRE(constStr[4] == '!');
    STATIC_REQUIRE(constStr[5] == '@');
    STATIC_REQUIRE(constStr[6] == '#');
    STATIC_REQUIRE(constStr[7] == '$');
    STATIC_REQUIRE(constStr[8] == '%');
  }

  SECTION("Unicode content") {
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

    // Compile-time checks
    STATIC_REQUIRE(constStr[4] == ' ');
    STATIC_REQUIRE(constStr[5] == '\xF0');
    STATIC_REQUIRE(constStr[6] == '\x9F');
    STATIC_REQUIRE(constStr[7] == '\x8C');
  }

  SECTION("Edge cases") {
    FixedString<4> str("ABC"); // Maximum length for capacity 4
    constexpr FixedString<4> constStr("XYZ");

    // Access to last character
    REQUIRE(str[2] == 'C');
    REQUIRE(constStr[2] == 'Z');

    // Modification of last character
    str[2] = 'D';
    REQUIRE(str[2] == 'D');
    REQUIRE(std::strcmp(str.c_str(), "ABD") == 0);

    // Compile-time checks
    STATIC_REQUIRE(constStr[0] == 'X');
    STATIC_REQUIRE(constStr[1] == 'Y');
    STATIC_REQUIRE(constStr[2] == 'Z');
  }

  SECTION("Reference semantics") {
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

  SECTION("empty string") {
    constexpr FixedString<16> str;

    REQUIRE(str[0] == '\0');

    // Compile-time checks
    STATIC_REQUIRE(str[0] == '\0');
  }
}

TEST_CASE("FixedString front and back", "[core][fixed_string]") {
  SECTION("Front method") {
    FixedString<16> testString("Hello World");
    constexpr FixedString<16> constString("Hello World");

    REQUIRE(testString.front() == 'H');
    REQUIRE(testString[0] == 'H');

    // Test modification
    testString.front() = 'h';
    REQUIRE(testString.front() == 'h');
    REQUIRE(testString[0] == 'h');

    // Compile-time checks
    STATIC_REQUIRE(constString.front() == 'H');
    STATIC_REQUIRE(constString[0] == 'H');
  }

  SECTION("Back method") {
    FixedString<16> testString("Hello World");
    constexpr FixedString<16> constString("Hello World");

    REQUIRE(testString.back() == 'd');
    REQUIRE(testString[testString.size() - 1] == 'd');

    // Test modification
    testString.back() = 'D';
    REQUIRE(testString.back() == 'D');
    REQUIRE(testString[testString.size() - 1] == 'D');

    // Compile-time checks
    STATIC_REQUIRE(constString.back() == 'd');
    STATIC_REQUIRE(constString[constString.size() - 1] == 'd');
  }

  SECTION("Single character string") {
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

    // Compile-time checks
    constexpr FixedString<2> constTestString("A");
    STATIC_REQUIRE(constTestString.front() == 'A');
    STATIC_REQUIRE(constTestString.back() == 'A');
    STATIC_REQUIRE(constTestString.front() == constTestString.back());
  }

  SECTION("Empty string") {
    FixedString<16> testString("");

    REQUIRE(testString.front() == '\0');

    // Compile-time checks
    constexpr FixedString<16> constTestString("");
    STATIC_REQUIRE(constTestString.front() == '\0');
  }

  SECTION("Two character string") {
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

    // Compile-time checks
    constexpr FixedString<8> constTestString("AB");
    STATIC_REQUIRE(constTestString.front() == 'A');
    STATIC_REQUIRE(constTestString.back() == 'B');
    STATIC_REQUIRE(constTestString.front() != constTestString.back());
  }

  SECTION("Modification through references") {
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

  SECTION("Const references") {
    constexpr FixedString<32> testString("Hello World");

    // Compile-time checks
    STATIC_REQUIRE(testString.front() == 'H');
    STATIC_REQUIRE(testString.back() == 'd');
  }

  SECTION("Special characters") {
    FixedString<16> testString("Hello\n\tWorld!");

    REQUIRE(testString.front() == 'H');
    REQUIRE(testString.back() == '!');

    // Test modification with special characters
    testString.front() = '\n';
    REQUIRE(testString.front() == '\n');

    testString.back() = '\t';
    REQUIRE(testString.back() == '\t');

    // Compile-time checks
    constexpr FixedString<16> constTestString("Hello\n\tWorld!");
    STATIC_REQUIRE(constTestString.front() == 'H');
    STATIC_REQUIRE(constTestString.back() == '!');
  }

  SECTION("Numeric content") {
    FixedString<16> testString("12345");

    REQUIRE(testString.front() == '1');
    REQUIRE(testString.back() == '5');

    // Test modification
    testString.front() = '9';
    REQUIRE(testString.front() == '9');

    testString.back() = '0';
    REQUIRE(testString.back() == '0');

    // Compile-time checks
    constexpr FixedString<16> constTestString("12345");
    STATIC_REQUIRE(constTestString.front() == '1');
    STATIC_REQUIRE(constTestString.back() == '5');
  }

  SECTION("Mixed content") {
    FixedString<32> testString("123Hello456");

    REQUIRE(testString.front() == '1');
    REQUIRE(testString.back() == '6');

    // Test modification
    testString.front() = '9';
    REQUIRE(testString.front() == '9');

    testString.back() = '0';
    REQUIRE(testString.back() == '0');

    // Compile-time checks
    constexpr FixedString<32> constTestString("123Hello456");
    STATIC_REQUIRE(constTestString.front() == '1');
    STATIC_REQUIRE(constTestString.back() == '6');
  }

  SECTION("Long strings") {
    FixedString<64> testString("This is a very long string for performance testing");

    REQUIRE(testString.front() == 'T');
    REQUIRE(testString.back() == 'g');

    // Test modification
    testString.front() = 'X';
    REQUIRE(testString.front() == 'X');

    testString.back() = 'Y';
    REQUIRE(testString.back() == 'Y');

    // Compile-time checks
    constexpr FixedString<64> constTestString("This is a very long string for performance testing");
    STATIC_REQUIRE(constTestString.front() == 'T');
    STATIC_REQUIRE(constTestString.back() == 'g');
  }

  SECTION("Case sensitivity") {
    FixedString<16> testString("Hello World");

    REQUIRE(testString.front() == 'H'); // Uppercase
    REQUIRE(testString.back() == 'd'); // Lowercase

    // Test case modification
    testString.front() = 'h'; // Change to lowercase
    REQUIRE(testString.front() == 'h');

    testString.back() = 'D'; // Change to uppercase
    REQUIRE(testString.back() == 'D');

    // Compile-time checks
    constexpr FixedString<16> constTestString("Hello World");
    STATIC_REQUIRE(constTestString.front() == 'H');
    STATIC_REQUIRE(constTestString.back() == 'd');
  }

  SECTION("Whitespace handling") {
    FixedString<16> testString(" Hello ");

    REQUIRE(testString.front() == ' ');
    REQUIRE(testString.back() == ' ');

    // Test modification
    testString.front() = 'X';
    REQUIRE(testString.front() == 'X');

    testString.back() = 'Y';
    REQUIRE(testString.back() == 'Y');

    // Compile-time checks
    constexpr FixedString<16> constTestString(" Hello ");
    STATIC_REQUIRE(constTestString.front() == ' ');
    STATIC_REQUIRE(constTestString.back() == ' ');
  }

  SECTION("Reference stability") {
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

  SECTION("Constexpr operations") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<16> str2("World");
    constexpr FixedString<16> str3("Test");

    // Compile-time front operations
    STATIC_REQUIRE(str1.front() == 'H');
    STATIC_REQUIRE(str2.front() == 'W');
    STATIC_REQUIRE(str3.front() == 'T');

    // Compile-time back operations
    STATIC_REQUIRE(str1.back() == 'o');
    STATIC_REQUIRE(str2.back() == 'd');
    STATIC_REQUIRE(str3.back() == 't');
  }
}

TEST_CASE("FixedString data", "[core][fixed_string]") {
  SECTION("Basic data access") {
    constexpr FixedString<16> testString("Hello World");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<4> singleChar("A");

    // Test that data() points to null-terminated string
    REQUIRE(std::strcmp(testString.data(), "Hello World") == 0);
    REQUIRE(std::strcmp(emptyString.data(), "") == 0);
    REQUIRE(std::strcmp(singleChar.data(), "A") == 0);

    // Compile-time checks
    STATIC_REQUIRE(cstrcmp(testString.data(), "Hello World") == 0);
    STATIC_REQUIRE(cstrcmp(emptyString.data(), "") == 0);
    STATIC_REQUIRE(cstrcmp(singleChar.data(), "A") == 0);
  }

  SECTION("Data pointer stability") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.data() == testString.data());
    STATIC_REQUIRE(copy1.data() == copy1.data());
    STATIC_REQUIRE(copy2.data() == copy2.data());

    STATIC_REQUIRE(cstrcmp(testString.data(), "Stability Test") == 0);
    STATIC_REQUIRE(cstrcmp(copy1.data(), "Stability Test") == 0);
    STATIC_REQUIRE(cstrcmp(copy2.data(), "Stability Test") == 0);
  }

  SECTION("Empty string") {
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<16> defaultString;

    // Test data() with empty strings
    REQUIRE(std::strcmp(emptyString.data(), "") == 0);
    REQUIRE(std::strcmp(defaultString.data(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(cstrcmp(emptyString.data(), "") == 0);
    STATIC_REQUIRE(cstrcmp(defaultString.data(), "") == 0);
  }
}

TEST_CASE("FixedString c_str method", "[core][fixed_string]") {
  SECTION("Basic c_str access") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.c_str() == testString.data());
    STATIC_REQUIRE(emptyString.c_str() == emptyString.data());
    STATIC_REQUIRE(singleChar.c_str() == singleChar.data());

    STATIC_REQUIRE(cstrcmp(testString.c_str(), "Hello World") == 0);
    STATIC_REQUIRE(cstrcmp(emptyString.c_str(), "") == 0);
    STATIC_REQUIRE(cstrcmp(singleChar.c_str(), "A") == 0);
  }

  SECTION("C string pointer stability") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.c_str() == testString.c_str());
    STATIC_REQUIRE(copy1.c_str() == copy1.c_str());
    STATIC_REQUIRE(copy2.c_str() == copy2.c_str());

    STATIC_REQUIRE(cstrcmp(testString.c_str(), "Stability Test") == 0);
    STATIC_REQUIRE(cstrcmp(copy1.c_str(), "Stability Test") == 0);
    STATIC_REQUIRE(cstrcmp(copy2.c_str(), "Stability Test") == 0);
  }

  SECTION("Empty string") {
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<16> defaultString;

    // Test c_str() with empty strings
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(emptyString.c_str() == emptyString.data());
    STATIC_REQUIRE(defaultString.c_str() == defaultString.data());
  }
}

TEST_CASE("FixedString empty method", "[core][fixed_string]") {
  SECTION("Basic empty check") {
    constexpr FixedString<16> nonEmptyString("Hello World");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<4> defaultString;

    REQUIRE_FALSE(nonEmptyString.empty());
    REQUIRE(emptyString.empty());
    REQUIRE(defaultString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(nonEmptyString.empty());
    STATIC_REQUIRE(emptyString.empty());
    STATIC_REQUIRE(defaultString.empty());
  }

  SECTION("Single character strings") {
    constexpr FixedString<8> singleChar("A");
    constexpr FixedString<4> emptyString("");

    REQUIRE_FALSE(singleChar.empty());
    REQUIRE(emptyString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(singleChar.empty());
    STATIC_REQUIRE(emptyString.empty());
  }

  SECTION("Different capacities") {
    constexpr FixedString<8> smallString("Hi");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<8> emptySmall("");
    constexpr FixedString<16> emptyMedium("");
    constexpr FixedString<32> emptyLarge("");

    REQUIRE_FALSE(smallString.empty());
    REQUIRE_FALSE(mediumString.empty());
    REQUIRE_FALSE(largeString.empty());
    REQUIRE(emptySmall.empty());
    REQUIRE(emptyMedium.empty());
    REQUIRE(emptyLarge.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(smallString.empty());
    STATIC_REQUIRE_FALSE(mediumString.empty());
    STATIC_REQUIRE_FALSE(largeString.empty());
    STATIC_REQUIRE(emptySmall.empty());
    STATIC_REQUIRE(emptyMedium.empty());
    STATIC_REQUIRE(emptyLarge.empty());
  }

  SECTION("Special characters") {
    constexpr FixedString<32> newlineString("Hello\nWorld");
    constexpr FixedString<32> tabString("Hello\tWorld");
    constexpr FixedString<32> specialString("!@#$%^&*()");
    constexpr FixedString<32> emptyString("");

    REQUIRE_FALSE(newlineString.empty());
    REQUIRE_FALSE(tabString.empty());
    REQUIRE_FALSE(specialString.empty());
    REQUIRE(emptyString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(newlineString.empty());
    STATIC_REQUIRE_FALSE(tabString.empty());
    STATIC_REQUIRE_FALSE(specialString.empty());
    STATIC_REQUIRE(emptyString.empty());
  }

  SECTION("Unicode content") {
    constexpr FixedString<64> unicodeString("Привет мир");
    constexpr FixedString<64> emojiString("Hello 🌍 World");
    constexpr FixedString<64> mixedString("Hello 世界");
    constexpr FixedString<64> emptyString("");

    REQUIRE_FALSE(unicodeString.empty());
    REQUIRE_FALSE(emojiString.empty());
    REQUIRE_FALSE(mixedString.empty());
    REQUIRE(emptyString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(unicodeString.empty());
    STATIC_REQUIRE_FALSE(emojiString.empty());
    STATIC_REQUIRE_FALSE(mixedString.empty());
    STATIC_REQUIRE(emptyString.empty());
  }

  SECTION("Numeric content") {
    constexpr FixedString<16> numericString("12345");
    constexpr FixedString<16> floatString("3.14159");
    constexpr FixedString<16> hexString("0xABCD");
    constexpr FixedString<16> emptyString("");

    REQUIRE_FALSE(numericString.empty());
    REQUIRE_FALSE(floatString.empty());
    REQUIRE_FALSE(hexString.empty());
    REQUIRE(emptyString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(numericString.empty());
    STATIC_REQUIRE_FALSE(floatString.empty());
    STATIC_REQUIRE_FALSE(hexString.empty());
    STATIC_REQUIRE(emptyString.empty());
  }

  SECTION("Mixed content") {
    constexpr FixedString<32> mixedString("Hello123World!@#");
    constexpr FixedString<32> complexString("Test\n123\t!@#");
    constexpr FixedString<64> longString("This is a very long string with mixed content 123!@#");
    constexpr FixedString<32> emptyString("");

    REQUIRE_FALSE(mixedString.empty());
    REQUIRE_FALSE(complexString.empty());
    REQUIRE_FALSE(longString.empty());
    REQUIRE(emptyString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(mixedString.empty());
    STATIC_REQUIRE_FALSE(complexString.empty());
    STATIC_REQUIRE_FALSE(longString.empty());
    STATIC_REQUIRE(emptyString.empty());
  }

  SECTION("Maximum length strings") {
    constexpr FixedString<16> maxString("123456789012345"); // 15 characters
    constexpr FixedString<8> maxSmall("1234567"); // 7 characters
    constexpr FixedString<4> maxTiny("123"); // 3 characters
    constexpr FixedString<16> emptyString("");

    REQUIRE_FALSE(maxString.empty());
    REQUIRE_FALSE(maxSmall.empty());
    REQUIRE_FALSE(maxTiny.empty());
    REQUIRE(emptyString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(maxString.empty());
    STATIC_REQUIRE_FALSE(maxSmall.empty());
    STATIC_REQUIRE_FALSE(maxTiny.empty());
    STATIC_REQUIRE(emptyString.empty());
  }

  SECTION("Edge cases") {
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

    // Compile-time checks
    STATIC_REQUIRE_FALSE(singleChar.empty());
    STATIC_REQUIRE_FALSE(twoChars.empty());
    STATIC_REQUIRE(emptyString.empty());
    STATIC_REQUIRE(defaultString.empty());
    STATIC_REQUIRE(singleChar.size() == 1);
    STATIC_REQUIRE(twoChars.size() == 2);
    STATIC_REQUIRE(emptyString.size() == 0);
    STATIC_REQUIRE(defaultString.size() == 0);
  }
}

TEST_CASE("FixedString size method", "[core][fixed_string]") {
  SECTION("Basic size check") {
    constexpr FixedString<16> testString("Hello World");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<4> defaultString;

    REQUIRE(testString.size() == 11);
    REQUIRE(emptyString.size() == 0);
    REQUIRE(defaultString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.size() == 11);
    STATIC_REQUIRE(emptyString.size() == 0);
    STATIC_REQUIRE(defaultString.size() == 0);
  }

  SECTION("Single character strings") {
    constexpr FixedString<8> singleChar("A");
    constexpr FixedString<4> emptyString("");

    REQUIRE(singleChar.size() == 1);
    REQUIRE(emptyString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(singleChar.size() == 1);
    STATIC_REQUIRE(emptyString.size() == 0);
  }

  SECTION("Different capacities") {
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

    // Compile-time checks
    STATIC_REQUIRE(smallString.size() == 2);
    STATIC_REQUIRE(mediumString.size() == 11);
    STATIC_REQUIRE(largeString.size() == 23);
    STATIC_REQUIRE(emptySmall.size() == 0);
    STATIC_REQUIRE(emptyMedium.size() == 0);
    STATIC_REQUIRE(emptyLarge.size() == 0);
  }

  SECTION("Special characters") {
    constexpr FixedString<32> newlineString("Hello\nWorld");
    constexpr FixedString<32> tabString("Hello\tWorld");
    constexpr FixedString<32> specialString("!@#$%^&*()");
    constexpr FixedString<32> emptyString("");

    REQUIRE(newlineString.size() == 11);
    REQUIRE(tabString.size() == 11);
    REQUIRE(specialString.size() == 10);
    REQUIRE(emptyString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(newlineString.size() == 11);
    STATIC_REQUIRE(tabString.size() == 11);
    STATIC_REQUIRE(specialString.size() == 10);
    STATIC_REQUIRE(emptyString.size() == 0);
  }

  SECTION("Unicode content") {
    constexpr FixedString<64> unicodeString("Привет мир");
    constexpr FixedString<64> emojiString("Hello 🌍 World");
    constexpr FixedString<64> mixedString("Hello 世界");
    constexpr FixedString<64> emptyString("");

    REQUIRE(unicodeString.size() == 19);
    REQUIRE(emojiString.size() == 16);
    REQUIRE(mixedString.size() == 12);
    REQUIRE(emptyString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(unicodeString.size() == 19);
    STATIC_REQUIRE(emojiString.size() == 16);
    STATIC_REQUIRE(mixedString.size() == 12);
    STATIC_REQUIRE(emptyString.size() == 0);
  }

  SECTION("Numeric content") {
    constexpr FixedString<16> numericString("12345");
    constexpr FixedString<16> floatString("3.14159");
    constexpr FixedString<16> hexString("0xABCD");
    constexpr FixedString<16> emptyString("");

    REQUIRE(numericString.size() == 5);
    REQUIRE(floatString.size() == 7);
    REQUIRE(hexString.size() == 6);
    REQUIRE(emptyString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(numericString.size() == 5);
    STATIC_REQUIRE(floatString.size() == 7);
    STATIC_REQUIRE(hexString.size() == 6);
    STATIC_REQUIRE(emptyString.size() == 0);
  }

  SECTION("Mixed content") {
    constexpr FixedString<32> mixedString("Hello123World!@#");
    constexpr FixedString<32> complexString("Test\n123\t!@#");
    constexpr FixedString<64> longString("This is a very long string with mixed content 123!@#");
    constexpr FixedString<32> emptyString("");

    REQUIRE(mixedString.size() == 16);
    REQUIRE(complexString.size() == 12);
    REQUIRE(longString.size() == 52);
    REQUIRE(emptyString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(mixedString.size() == 16);
    STATIC_REQUIRE(complexString.size() == 12);
    STATIC_REQUIRE(longString.size() == 52);
    STATIC_REQUIRE(emptyString.size() == 0);
  }

  SECTION("Maximum length strings") {
    constexpr FixedString<16> maxString("123456789012345"); // 15 characters
    constexpr FixedString<8> maxSmall("1234567"); // 7 characters
    constexpr FixedString<4> maxTiny("123"); // 3 characters
    constexpr FixedString<16> emptyString("");

    REQUIRE(maxString.size() == 15);
    REQUIRE(maxSmall.size() == 7);
    REQUIRE(maxTiny.size() == 3);
    REQUIRE(emptyString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(maxString.size() == 15);
    STATIC_REQUIRE(maxSmall.size() == 7);
    STATIC_REQUIRE(maxTiny.size() == 3);
    STATIC_REQUIRE(emptyString.size() == 0);
  }

  SECTION("Edge cases") {
    constexpr FixedString<8> singleChar("A");
    constexpr FixedString<8> twoChars("AB");
    constexpr FixedString<8> emptyString("");
    constexpr FixedString<8> defaultString;

    REQUIRE(singleChar.size() == 1);
    REQUIRE(twoChars.size() == 2);
    REQUIRE(emptyString.size() == 0);
    REQUIRE(defaultString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(singleChar.size() == 1);
    STATIC_REQUIRE(twoChars.size() == 2);
    STATIC_REQUIRE(emptyString.size() == 0);
    STATIC_REQUIRE(defaultString.size() == 0);
  }
}

TEST_CASE("FixedString utf8_size", "[core][fixed_string]") {
  SECTION("ASCII strings") {
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

  SECTION("UTF-8 Cyrillic text") {
    // "Привет мир" in UTF-8
    static constexpr std::array<char, 20> cyrillicText{{char(0xD0), char(0x9F), char(0xD1), char(0x80), char(0xD0),
                                                        char(0xB8), char(0xD0), char(0xB2), char(0xD0), char(0xB5),
                                                        char(0xD1), char(0x82), char(0x20), char(0xD0), char(0xBC),
                                                        char(0xD0), char(0xB8), char(0xD1), char(0x80), char(0x00)}};

    constexpr FixedString<32> cyrillicString(cyrillicText.data());

    REQUIRE(cyrillicString.size() == 19);
    REQUIRE(cyrillicString.utf8_size() == 10);
  }

  SECTION("Mixed ASCII and UTF-8") {
    // "Hello 世界" in UTF-8
    static constexpr std::array<char, 13> mixedText{{char(0x48), char(0x65), char(0x6C), char(0x6C), char(0x6F),
                                                     char(0x20), char(0xE4), char(0xB8), char(0x96), char(0xE7),
                                                     char(0x95), char(0x8C), char(0x00)}};

    constexpr FixedString<16> mixedString(mixedText.data());

    REQUIRE(mixedString.size() == 12);
    REQUIRE(mixedString.utf8_size() == 8); // 6 ASCII + 2 Chinese characters
  }

  SECTION("Emoji characters") {
    // "Hello 🌍" in UTF-8
    static constexpr std::array<char, 11> emojiText{{char(0x48), char(0x65), char(0x6C), char(0x6C), char(0x6F),
                                                     char(0x20), char(0xF0), char(0x9F), char(0x8C), char(0x8D),
                                                     char(0x00)}};

    constexpr FixedString<16> emojiString(emojiText.data());

    REQUIRE(emojiString.size() == 10);
    REQUIRE(emojiString.utf8_size() == 7); // 6 ASCII + 1 emoji
  }

  SECTION("Special characters") {
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

  SECTION("Edge cases") {
    constexpr FixedString<8> singleByte("A");
    constexpr FixedString<16> twoByte("А"); // Cyrillic A
    constexpr FixedString<16> threeByte("中"); // Chinese character
    constexpr FixedString<16> fourByte("🌍"); // Emoji

    REQUIRE(singleByte.utf8_size() == 1);
    REQUIRE(twoByte.utf8_size() == 1);
    REQUIRE(threeByte.utf8_size() == 1);
    REQUIRE(fourByte.utf8_size() == 1);
  }

  SECTION("Long UTF-8 text") {
    // "ToyGine2 - Бесплатный 2D/3D игровой движок." in UTF-8
    static constexpr std::array<char, 67> longUtf8Text{
      {char(0x54), char(0x6F), char(0x79), char(0x47), char(0x69), char(0x6E), char(0x65), char(0x32), char(0x20),
       char(0x2D), char(0x20), char(0xD0), char(0x91), char(0xD0), char(0xB5), char(0xD1), char(0x81), char(0xD0),
       char(0xBF), char(0xD0), char(0xBB), char(0xD0), char(0xB0), char(0xD1), char(0x82), char(0xD0), char(0xBD),
       char(0xD1), char(0x8B), char(0xD0), char(0xB9), char(0x20), char(0x32), char(0x44), char(0x2F), char(0x33),
       char(0x44), char(0x20), char(0xD0), char(0xB8), char(0xD0), char(0xB3), char(0xD1), char(0x80), char(0xD0),
       char(0xBE), char(0xD0), char(0xB2), char(0xD0), char(0xBE), char(0xD0), char(0xB9), char(0x20), char(0xD0),
       char(0xB4), char(0xD0), char(0xB2), char(0xD0), char(0xB8), char(0xD0), char(0xB6), char(0xD0), char(0xBE),
       char(0xD0), char(0xBA), char(0x2E), char(0x00)}};

    constexpr FixedString<80> longString(longUtf8Text.data());

    REQUIRE(longString.size() == 66); // 66 bytes
    REQUIRE(longString.utf8_size() == 43); // 43 characters
  }
}

TEST_CASE("FixedString length", "[core][fixed_string]") {
  SECTION("Basic length check") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.length() == 11);
    STATIC_REQUIRE(emptyString.length() == 0);
    STATIC_REQUIRE(singleChar.length() == 1);
  }

  SECTION("Different capacities") {
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

    // Compile-time checks
    STATIC_REQUIRE(smallString.length() == 2);
    STATIC_REQUIRE(mediumString.length() == 11);
    STATIC_REQUIRE(largeString.length() == 23);
  }

  SECTION("Special characters") {
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

    // Compile-time checks
    STATIC_REQUIRE(newlineString.length() == 11);
    STATIC_REQUIRE(tabString.length() == 11);
    STATIC_REQUIRE(specialString.length() == 10);
  }

  SECTION("Unicode content") {
    constexpr FixedString<64> unicodeString("Привет мир");
    constexpr FixedString<64> emojiString("Hello 🌍 World");
    constexpr FixedString<64> mixedString("Hello 世界");
    constexpr FixedString<64> emptyString("");

    REQUIRE(unicodeString.length() == 19);
    REQUIRE(emojiString.length() == 16);
    REQUIRE(mixedString.length() == 12);
    REQUIRE(emptyString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(unicodeString.length() == unicodeString.size());
    REQUIRE(emojiString.length() == emojiString.size());
    REQUIRE(mixedString.length() == mixedString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    // Compile-time checks
    STATIC_REQUIRE(unicodeString.length() == 19);
    STATIC_REQUIRE(emojiString.length() == 16);
    STATIC_REQUIRE(mixedString.length() == 12);
  }

  SECTION("Numeric content") {
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

    // Compile-time checks
    STATIC_REQUIRE(numericString.length() == 5);
    STATIC_REQUIRE(floatString.length() == 7);
    STATIC_REQUIRE(hexString.length() == 6);
  }

  SECTION("Mixed content") {
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

    // Compile-time checks
    STATIC_REQUIRE(mixedString.length() == 16);
    STATIC_REQUIRE(complexString.length() == 12);
    STATIC_REQUIRE(longString.length() == 52);
  }

  SECTION("Edge cases") {
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

    // Compile-time checks
    STATIC_REQUIRE(singleChar.length() == 1);
    STATIC_REQUIRE(twoChars.length() == 2);
    STATIC_REQUIRE(emptyString.length() == 0);
    STATIC_REQUIRE(defaultString.length() == 0);
  }
}

TEST_CASE("FixedString max_size", "[core][fixed_string]") {
  SECTION("Basic max_size check") {
    constexpr FixedString<8> smallString("Hi");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<64> extraLargeString("This is an even longer string for testing");

    REQUIRE(smallString.max_size() == 7);
    REQUIRE(mediumString.max_size() == 15);
    REQUIRE(largeString.max_size() == 31);
    REQUIRE(extraLargeString.max_size() == 63);

    // Compile-time checks
    STATIC_REQUIRE(smallString.max_size() == 7);
    STATIC_REQUIRE(mediumString.max_size() == 15);
    STATIC_REQUIRE(largeString.max_size() == 31);
    STATIC_REQUIRE(extraLargeString.max_size() == 63);
  }

  SECTION("Empty strings") {
    constexpr FixedString<8> emptySmall("");
    constexpr FixedString<16> emptyMedium("");
    constexpr FixedString<32> emptyLarge("");
    constexpr FixedString<64> emptyExtraLarge("");

    REQUIRE(emptySmall.max_size() == 7);
    REQUIRE(emptyMedium.max_size() == 15);
    REQUIRE(emptyLarge.max_size() == 31);
    REQUIRE(emptyExtraLarge.max_size() == 63);

    // Compile-time checks
    STATIC_REQUIRE(emptySmall.max_size() == 7);
    STATIC_REQUIRE(emptyMedium.max_size() == 15);
    STATIC_REQUIRE(emptyLarge.max_size() == 31);
    STATIC_REQUIRE(emptyExtraLarge.max_size() == 63);
  }

  SECTION("Default constructed strings") {
    constexpr FixedString<8> defaultSmall;
    constexpr FixedString<16> defaultMedium;
    constexpr FixedString<32> defaultLarge;
    constexpr FixedString<64> defaultExtraLarge;

    REQUIRE(defaultSmall.max_size() == 7);
    REQUIRE(defaultMedium.max_size() == 15);
    REQUIRE(defaultLarge.max_size() == 31);
    REQUIRE(defaultExtraLarge.max_size() == 63);

    // Compile-time checks
    STATIC_REQUIRE(defaultSmall.max_size() == 7);
    STATIC_REQUIRE(defaultMedium.max_size() == 15);
    STATIC_REQUIRE(defaultLarge.max_size() == 31);
    STATIC_REQUIRE(defaultExtraLarge.max_size() == 63);
  }

  SECTION("Single character strings") {
    constexpr FixedString<8> singleSmall("A");
    constexpr FixedString<16> singleMedium("B");
    constexpr FixedString<32> singleLarge("C");
    constexpr FixedString<64> singleExtraLarge("D");

    REQUIRE(singleSmall.max_size() == 7);
    REQUIRE(singleMedium.max_size() == 15);
    REQUIRE(singleLarge.max_size() == 31);
    REQUIRE(singleExtraLarge.max_size() == 63);

    // Compile-time checks
    STATIC_REQUIRE(singleSmall.max_size() == 7);
    STATIC_REQUIRE(singleMedium.max_size() == 15);
    STATIC_REQUIRE(singleLarge.max_size() == 31);
    STATIC_REQUIRE(singleExtraLarge.max_size() == 63);
  }

  SECTION("Maximum length strings") {
    constexpr FixedString<8> maxSmall("1234567"); // 7 characters (max for capacity 8)
    constexpr FixedString<16> maxMedium("123456789012345"); // 15 characters (max for capacity 16)
    constexpr FixedString<32> maxLarge("1234567890123456789012345678901"); // 31 characters (max for capacity 32)
    constexpr FixedString<64> maxExtraLarge(
      "123456789012345678901234567890123456789012345678901234567890123"); // 63 characters (max for capacity 64)

    REQUIRE(maxSmall.max_size() == 7);
    REQUIRE(maxMedium.max_size() == 15);
    REQUIRE(maxLarge.max_size() == 31);
    REQUIRE(maxExtraLarge.max_size() == 63);

    // Compile-time checks
    STATIC_REQUIRE(maxSmall.max_size() == 7);
    STATIC_REQUIRE(maxMedium.max_size() == 15);
    STATIC_REQUIRE(maxLarge.max_size() == 31);
    STATIC_REQUIRE(maxExtraLarge.max_size() == 63);
  }

  SECTION("Different template parameters") {
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

    // Compile-time checks
    STATIC_REQUIRE(tinyString.max_size() == 3);
    STATIC_REQUIRE(smallString.max_size() == 7);
    STATIC_REQUIRE(mediumString.max_size() == 15);
    STATIC_REQUIRE(largeString.max_size() == 31);
    STATIC_REQUIRE(extraLargeString.max_size() == 63);
    STATIC_REQUIRE(hugeString.max_size() == 127);
  }

  SECTION("Edge cases") {
    constexpr FixedString<1> minimalString; // Should have max_size() == 0
    constexpr FixedString<2> twoCharString("A");
    constexpr FixedString<3> threeCharString("AB");

    REQUIRE(minimalString.max_size() == 0);
    REQUIRE(twoCharString.max_size() == 1);
    REQUIRE(threeCharString.max_size() == 2);

    // Compile-time checks
    STATIC_REQUIRE(minimalString.max_size() == 0);
    STATIC_REQUIRE(twoCharString.max_size() == 1);
    STATIC_REQUIRE(threeCharString.max_size() == 2);
  }

  SECTION("Consistency with capacity") {
    constexpr FixedString<8> testString1("Hello");
    constexpr FixedString<16> testString2("World");
    constexpr FixedString<32> testString3("Test");

    // max_size() should equal capacity() for all FixedString instances
    REQUIRE(testString1.max_size() == testString1.capacity());
    REQUIRE(testString2.max_size() == testString2.capacity());
    REQUIRE(testString3.max_size() == testString3.capacity());

    // Compile-time checks
    STATIC_REQUIRE(testString1.max_size() == testString1.capacity());
    STATIC_REQUIRE(testString2.max_size() == testString2.capacity());
    STATIC_REQUIRE(testString3.max_size() == testString3.capacity());
  }
}

TEST_CASE("FixedString capacity", "[core][fixed_string]") {
  SECTION("Basic capacity check") {
    constexpr FixedString<8> smallString("Hi");
    constexpr FixedString<16> mediumString("Hello World");
    constexpr FixedString<32> largeString("This is a longer string");
    constexpr FixedString<64> extraLargeString("This is an even longer string for testing");

    REQUIRE(smallString.capacity() == 7);
    REQUIRE(mediumString.capacity() == 15);
    REQUIRE(largeString.capacity() == 31);
    REQUIRE(extraLargeString.capacity() == 63);

    // Compile-time checks
    STATIC_REQUIRE(smallString.capacity() == 7);
    STATIC_REQUIRE(mediumString.capacity() == 15);
    STATIC_REQUIRE(largeString.capacity() == 31);
    STATIC_REQUIRE(extraLargeString.capacity() == 63);
  }

  SECTION("Empty strings") {
    constexpr FixedString<8> emptySmall("");
    constexpr FixedString<16> emptyMedium("");
    constexpr FixedString<32> emptyLarge("");
    constexpr FixedString<64> emptyExtraLarge("");

    REQUIRE(emptySmall.capacity() == 7);
    REQUIRE(emptyMedium.capacity() == 15);
    REQUIRE(emptyLarge.capacity() == 31);
    REQUIRE(emptyExtraLarge.capacity() == 63);

    // Compile-time checks
    STATIC_REQUIRE(emptySmall.capacity() == 7);
    STATIC_REQUIRE(emptyMedium.capacity() == 15);
    STATIC_REQUIRE(emptyLarge.capacity() == 31);
    STATIC_REQUIRE(emptyExtraLarge.capacity() == 63);
  }

  SECTION("Default constructed strings") {
    constexpr FixedString<8> defaultSmall;
    constexpr FixedString<16> defaultMedium;
    constexpr FixedString<32> defaultLarge;
    constexpr FixedString<64> defaultExtraLarge;

    REQUIRE(defaultSmall.capacity() == 7);
    REQUIRE(defaultMedium.capacity() == 15);
    REQUIRE(defaultLarge.capacity() == 31);
    REQUIRE(defaultExtraLarge.capacity() == 63);

    // Compile-time checks
    STATIC_REQUIRE(defaultSmall.capacity() == 7);
    STATIC_REQUIRE(defaultMedium.capacity() == 15);
    STATIC_REQUIRE(defaultLarge.capacity() == 31);
    STATIC_REQUIRE(defaultExtraLarge.capacity() == 63);
  }

  SECTION("Single character strings") {
    constexpr FixedString<8> singleSmall("A");
    constexpr FixedString<16> singleMedium("B");
    constexpr FixedString<32> singleLarge("C");
    constexpr FixedString<64> singleExtraLarge("D");

    REQUIRE(singleSmall.capacity() == 7);
    REQUIRE(singleMedium.capacity() == 15);
    REQUIRE(singleLarge.capacity() == 31);
    REQUIRE(singleExtraLarge.capacity() == 63);

    // Compile-time checks
    STATIC_REQUIRE(singleSmall.capacity() == 7);
    STATIC_REQUIRE(singleMedium.capacity() == 15);
    STATIC_REQUIRE(singleLarge.capacity() == 31);
    STATIC_REQUIRE(singleExtraLarge.capacity() == 63);
  }

  SECTION("Maximum length strings") {
    constexpr FixedString<8> maxSmall("1234567"); // 7 characters (max for capacity 8)
    constexpr FixedString<16> maxMedium("123456789012345"); // 15 characters (max for capacity 16)
    constexpr FixedString<32> maxLarge("1234567890123456789012345678901"); // 31 characters (max for capacity 32)
    constexpr FixedString<64> maxExtraLarge(
      "123456789012345678901234567890123456789012345678901234567890123"); // 63 characters (max for capacity 64)

    REQUIRE(maxSmall.capacity() == 7);
    REQUIRE(maxMedium.capacity() == 15);
    REQUIRE(maxLarge.capacity() == 31);
    REQUIRE(maxExtraLarge.capacity() == 63);

    // Compile-time checks
    STATIC_REQUIRE(maxSmall.capacity() == 7);
    STATIC_REQUIRE(maxMedium.capacity() == 15);
    STATIC_REQUIRE(maxLarge.capacity() == 31);
    STATIC_REQUIRE(maxExtraLarge.capacity() == 63);
  }

  SECTION("Different template parameters") {
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

    // Compile-time checks
    STATIC_REQUIRE(tinyString.capacity() == 3);
    STATIC_REQUIRE(smallString.capacity() == 7);
    STATIC_REQUIRE(mediumString.capacity() == 15);
    STATIC_REQUIRE(largeString.capacity() == 31);
    STATIC_REQUIRE(extraLargeString.capacity() == 63);
    STATIC_REQUIRE(hugeString.capacity() == 127);
  }

  SECTION("Edge cases") {
    constexpr FixedString<1> minimalString; // Should have capacity() == 0
    constexpr FixedString<2> twoCharString("A");
    constexpr FixedString<3> threeCharString("AB");

    REQUIRE(minimalString.capacity() == 0);
    REQUIRE(twoCharString.capacity() == 1);
    REQUIRE(threeCharString.capacity() == 2);

    // Compile-time checks
    STATIC_REQUIRE(minimalString.capacity() == 0);
    STATIC_REQUIRE(twoCharString.capacity() == 1);
    STATIC_REQUIRE(threeCharString.capacity() == 2);
  }

  SECTION("Consistency with max_size") {
    constexpr FixedString<8> testString1("Hello");
    constexpr FixedString<16> testString2("World");
    constexpr FixedString<32> testString3("Test");

    // capacity() should equal max_size() for all FixedString instances
    REQUIRE(testString1.capacity() == testString1.max_size());
    REQUIRE(testString2.capacity() == testString2.max_size());
    REQUIRE(testString3.capacity() == testString3.max_size());

    // Compile-time checks
    STATIC_REQUIRE(testString1.capacity() == testString1.max_size());
    STATIC_REQUIRE(testString2.capacity() == testString2.max_size());
    STATIC_REQUIRE(testString3.capacity() == testString3.max_size());
  }

  SECTION("Capacity formula validation") {
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

    // Compile-time checks
    STATIC_REQUIRE(test4.capacity() == 3);
    STATIC_REQUIRE(test8.capacity() == 7);
    STATIC_REQUIRE(test16.capacity() == 15);
    STATIC_REQUIRE(test32.capacity() == 31);
    STATIC_REQUIRE(test64.capacity() == 63);
  }
}

TEST_CASE("FixedString clear", "[core][fixed_string]") {
  SECTION("Basic clear functionality") {
    FixedString<32> testString("Hello World");

    REQUIRE_FALSE(testString.empty());
    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.clear();

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SECTION("Clear empty string") {
    FixedString<16> emptyString("");

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString.clear();

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
  }

  SECTION("Clear default constructed string") {
    FixedString<8> defaultString;

    REQUIRE(defaultString.empty());
    REQUIRE(defaultString.size() == 0);

    defaultString.clear();

    REQUIRE(defaultString.empty());
    REQUIRE(defaultString.size() == 0);
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);
  }

  SECTION("Clear single character string") {
    FixedString<16> singleChar("A");

    REQUIRE_FALSE(singleChar.empty());
    REQUIRE(singleChar.size() == 1);
    REQUIRE(std::strcmp(singleChar.c_str(), "A") == 0);

    singleChar.clear();

    REQUIRE(singleChar.empty());
    REQUIRE(singleChar.size() == 0);
    REQUIRE(std::strcmp(singleChar.c_str(), "") == 0);
  }

  SECTION("Clear maximum length string") {
    FixedString<8> maxString("1234567"); // 7 characters (max for capacity 8)

    REQUIRE_FALSE(maxString.empty());
    REQUIRE(maxString.size() == 7);
    REQUIRE(std::strcmp(maxString.c_str(), "1234567") == 0);

    maxString.clear();

    REQUIRE(maxString.empty());
    REQUIRE(maxString.size() == 0);
    REQUIRE(std::strcmp(maxString.c_str(), "") == 0);
  }

  SECTION("Clear different capacities") {
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

  SECTION("Clear special characters") {
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

  SECTION("Clear Unicode content") {
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

  SECTION("Clear and capacity preservation") {
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

  SECTION("Clear and reassignment") {
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

  SECTION("Multiple clear operations") {
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

TEST_CASE("FixedString insert", "[core][fixed_string]") {
  SECTION("Insert FixedString at beginning") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(0, FixedString<16>("Hi "));

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hi Hello World") == 0);
  }

  SECTION("Insert FixedString in middle") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(6, FixedString<16>("Beautiful "));

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
  }

  SECTION("Insert FixedString at end") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(11, FixedString<8>("!"));

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);
  }

  SECTION("Insert C-string at beginning") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(0, "Hi ");

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hi Hello World") == 0);
  }

  SECTION("Insert C-string in middle") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(6, "Beautiful ");

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
  }

  SECTION("Insert C-string at end") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(11, "!");

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);
  }

  SECTION("Insert single character") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(5, ' ');

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello  World") == 0);
  }

  SECTION("Insert multiple characters") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.insert(0, '*', 3);

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "***Hello World") == 0);
  }

  SECTION("Insert zero characters") {
    FixedString<32> testString("Hello World");

    const auto originalSize = testString.size();
    const auto originalContent = std::string(testString.c_str());

    testString.insert(0, 'X', 0);

    REQUIRE(testString.size() == originalSize);
    REQUIRE(std::strcmp(testString.c_str(), originalContent.c_str()) == 0);
  }

  SECTION("Insert into empty string") {
    FixedString<32> emptyString;

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString.insert(0, "Hello");

    REQUIRE_FALSE(emptyString.empty());
    REQUIRE(emptyString.size() == 5);
    REQUIRE(std::strcmp(emptyString.c_str(), "Hello") == 0);
  }

  SECTION("Insert at position 0") {
    FixedString<32> testString("World");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "World") == 0);

    testString.insert(0, "Hello ");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Insert at end position") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.insert(5, " World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Insert special characters") {
    FixedString<32> testString("Hello World");

    testString.insert(5, '\n');
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n World") == 0);
    REQUIRE(testString.size() == 12);

    testString.insert(0, '\t');
    REQUIRE(std::strcmp(testString.c_str(), "\tHello\n World") == 0);
    REQUIRE(testString.size() == 13);
  }

  SECTION("Insert Unicode content") {
    FixedString<64> testString("Hello");

    testString.insert(5, " 世界");
    REQUIRE(std::strcmp(testString.c_str(), "Hello 世界") == 0);
    REQUIRE(testString.size() == 12);

    testString.insert(0, "Привет ");
    REQUIRE(std::strcmp(testString.c_str(), "Привет Hello 世界") == 0);
    REQUIRE(testString.size() == 25);
  }

  SECTION("Insert with different capacities") {
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

  SECTION("Multiple insert operations") {
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

  SECTION("Insert character at various positions") {
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

TEST_CASE("FixedString erase", "[core][fixed_string]") {
  SECTION("Erase from beginning") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.erase(0, 5);

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), " World") == 0);
  }

  SECTION("Erase from middle") {
    FixedString<32> testString("Hello Beautiful World");

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);

    testString.erase(6, 10);

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Erase from end") {
    FixedString<32> testString("Hello World!");

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);

    testString.erase(11, 1);

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Erase single character") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.erase(5, 1);

    REQUIRE(testString.size() == 10);
    REQUIRE(std::strcmp(testString.c_str(), "HelloWorld") == 0);
  }

  SECTION("Erase zero characters") {
    FixedString<32> testString("Hello World");

    const auto originalSize = testString.size();
    const auto originalContent = std::string(testString.c_str());

    testString.erase(5, 0);

    REQUIRE(testString.size() == originalSize);
    REQUIRE(std::strcmp(testString.c_str(), originalContent.c_str()) == 0);
  }

  SECTION("Erase from position to end") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.erase(6);

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ") == 0);
  }

  SECTION("Erase everything") {
    FixedString<32> testString("Hello World");

    REQUIRE_FALSE(testString.empty());
    REQUIRE(testString.size() == 11);

    testString.erase(0);

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SECTION("Erase special characters") {
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

  SECTION("Erase Unicode content") {
    FixedString<64> testString("Hello 世界 World");

    REQUIRE(testString.size() == 18);
    REQUIRE(std::strcmp(testString.c_str(), "Hello 世界 World") == 0);

    testString.erase(6, 3); // Erase Chinese characters

    REQUIRE(testString.size() == 15);
    REQUIRE(std::strcmp(testString.c_str(), "Hello 界 World") == 0);
  }

  SECTION("Erase with different capacities") {
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

  SECTION("Multiple erase operations") {
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

  SECTION("Erase at various positions") {
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

  SECTION("Erase entire words") {
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

TEST_CASE("FixedString push_back", "[core][fixed_string]") {
  SECTION("Push back single character") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.push_back('!');

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello!") == 0);
  }

  SECTION("Push back multiple characters") {
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

  SECTION("Push back to empty string") {
    FixedString<16> emptyString;

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString.push_back('A');

    REQUIRE_FALSE(emptyString.empty());
    REQUIRE(emptyString.size() == 1);
    REQUIRE(std::strcmp(emptyString.c_str(), "A") == 0);
  }

  SECTION("Push back special characters") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.push_back('\n');
    testString.push_back('\t');
    testString.push_back('\0');

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n\t") == 0);
  }

  SECTION("Push back with different capacities") {
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

  SECTION("Push back numeric characters") {
    FixedString<16> testString("123");

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "123") == 0);

    testString.push_back('4');
    testString.push_back('5');
    testString.push_back('6');

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "123456") == 0);
  }

  SECTION("Push back mixed content") {
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

  SECTION("Push back edge cases") {
    FixedString<16> testString("A");

    REQUIRE(testString.size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString.push_back('B');
    testString.push_back('C');
    testString.push_back('D');

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "ABCD") == 0);
  }

  SECTION("Push back and size consistency") {
    FixedString<16> testString;

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    for (int i = 0; i < 5; ++i) {
      testString.push_back('A' + i);
      REQUIRE(testString.size() == i + 1);
    }

    REQUIRE(std::strcmp(testString.c_str(), "ABCDE") == 0);
    REQUIRE(testString.size() == 5);
  }

  SECTION("Push back with different character types") {
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

TEST_CASE("FixedString pop_back", "[core][fixed_string]") {
  SECTION("Pop back single character") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.pop_back();

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Hell") == 0);
  }

  SECTION("Pop back multiple characters") {
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

  SECTION("Pop back from single character string") {
    FixedString<16> testString("A");

    REQUIRE(testString.size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString.pop_back();

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SECTION("Pop back special characters") {
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

  SECTION("Pop back with different capacities") {
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

  SECTION("Pop back numeric characters") {
    FixedString<16> testString("123456");

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "123456") == 0);

    testString.pop_back();
    testString.pop_back();
    testString.pop_back();

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "123") == 0);
  }

  SECTION("Pop back mixed content") {
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

  SECTION("Pop back edge cases") {
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

  SECTION("Pop back and size consistency") {
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

  SECTION("Pop back with different character types") {
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

  SECTION("Pop back and reassignment") {
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

  SECTION("Pop back with capacity preservation") {
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

TEST_CASE("FixedString utf8_pop_back", "[core][fixed_string]") {
  SECTION("Pop back single ASCII character") {
    FixedString<16> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(testString.utf8_size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.utf8_pop_back();

    REQUIRE(testString.size() == 4);
    REQUIRE(testString.utf8_size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Hell") == 0);
  }

  SECTION("Pop back multiple ASCII characters") {
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

  SECTION("Pop back UTF-8 Cyrillic characters") {
    FixedString<32> testString("Hello привет");

    REQUIRE(testString.size() == 18);
    REQUIRE(testString.utf8_size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello привет") == 0);

    testString.utf8_pop_back(); // Remove 'т'

    REQUIRE(testString.size() == 16);
    REQUIRE(testString.utf8_size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello приве") == 0);
  }

  SECTION("Pop back multiple UTF-8 characters") {
    FixedString<32> testString("Hello привет");

    REQUIRE(testString.size() == 18);
    REQUIRE(testString.utf8_size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello привет") == 0);

    testString.utf8_pop_back(); // Remove 'т'
    testString.utf8_pop_back(); // Remove 'е'
    testString.utf8_pop_back(); // Remove 'в'

    REQUIRE(testString.size() == 12);
    REQUIRE(testString.utf8_size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Hello при") == 0);
  }

  SECTION("Pop back mixed ASCII and UTF-8") {
    FixedString<32> testString("Hello привет");

    REQUIRE(testString.size() == 18);
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

  SECTION("Pop back from single ASCII character") {
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

  SECTION("Pop back from single UTF-8 character") {
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

  SECTION("Pop back UTF-8 emoji characters") {
    FixedString<32> testString("Hello 🌍 World");

    REQUIRE(testString.size() == 16);
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

  SECTION("Pop back with different capacities") {
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
    REQUIRE(mediumString.size() == 9);
    REQUIRE(largeString.size() == 23);

    REQUIRE(smallString.utf8_size() == 1);
    REQUIRE(mediumString.utf8_size() == 7);
    REQUIRE(largeString.utf8_size() == 15);
  }

  SECTION("Pop back numeric and special characters") {
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

  SECTION("Pop back edge cases") {
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

  SECTION("Pop back and size consistency") {
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

  SECTION("Pop back with capacity preservation") {
    FixedString<20> testString("Hello привет");

    const auto originalCapacity = testString.capacity();
    const auto originalMaxSize = testString.max_size();

    REQUIRE(testString.size() == 18);
    REQUIRE(testString.utf8_size() == 12);

    testString.utf8_pop_back();
    testString.utf8_pop_back();
    testString.utf8_pop_back();

    REQUIRE(testString.size() == 12);
    REQUIRE(testString.utf8_size() == 9);
    REQUIRE(testString.capacity() == originalCapacity);
    REQUIRE(testString.max_size() == originalMaxSize);
  }

  SECTION("Pop back and reassignment") {
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

TEST_CASE("FixedString append", "[core][fixed_string]") {
  SECTION("Append FixedString") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(FixedString<16>(" World"));

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Append C-string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(" World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Append std::string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(std::string(" World"));

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Append single character") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append('!');

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello!") == 0);
  }

  SECTION("Append multiple characters") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(' ', 3);

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hello   ") == 0);
  }

  SECTION("Append to empty string") {
    FixedString<32> emptyString;

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString.append("Hello");

    REQUIRE_FALSE(emptyString.empty());
    REQUIRE(emptyString.size() == 5);
    REQUIRE(std::strcmp(emptyString.c_str(), "Hello") == 0);
  }

  SECTION("Append zero characters") {
    FixedString<32> testString("Hello");

    const auto originalSize = testString.size();
    const auto originalContent = std::string(testString.c_str());

    testString.append('X', 0);

    REQUIRE(testString.size() == originalSize);
    REQUIRE(std::strcmp(testString.c_str(), originalContent.c_str()) == 0);
  }

  SECTION("Append special characters") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append('\n');
    testString.append('\t');
    testString.append('!');

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n\t!") == 0);
  }

  SECTION("Append Unicode content") {
    FixedString<64> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString.append(" 世界");

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello 世界") == 0);
  }

  SECTION("Append with different capacities") {
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

  SECTION("Append numeric content") {
    FixedString<16> testString("123");

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "123") == 0);

    testString.append("456");

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "123456") == 0);
  }

  SECTION("Append mixed content") {
    FixedString<32> testString("Test");

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Test") == 0);

    testString.append(" 123!");

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Test 123!") == 0);
  }

  SECTION("Append edge cases") {
    FixedString<16> testString("A");

    REQUIRE(testString.size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString.append("BC");

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "ABC") == 0);
  }

  SECTION("Append and size consistency") {
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

  SECTION("Append chaining") {
    FixedString<32> testString("");

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    testString.append(FixedString<16>("Nothing"))
      .append(FixedString<8>(" else"))
      .append(std::string(" really"))
      .append(" matters")
      .append('.', 3);

    REQUIRE(testString.size() == 30);
    REQUIRE(std::strcmp(testString.c_str(), "Nothing else really matters...") == 0);
  }

  SECTION("Append and reassignment") {
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

  SECTION("Append with capacity preservation") {
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

TEST_CASE("FixedString operators+=", "[core][fixed_string]") {
  SECTION("Operator += with FixedString") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += FixedString<16>(" World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Operator += with C-string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += " World";

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Operator += with std::string") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += std::string(" World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Operator += with single character") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += '!';

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello!") == 0);
  }

  SECTION("Operator += to empty string") {
    FixedString<32> emptyString;

    REQUIRE(emptyString.empty());
    REQUIRE(emptyString.size() == 0);

    emptyString += "Hello";

    REQUIRE_FALSE(emptyString.empty());
    REQUIRE(emptyString.size() == 5);
    REQUIRE(std::strcmp(emptyString.c_str(), "Hello") == 0);
  }

  SECTION("Operator += with special characters") {
    FixedString<32> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += '\n';
    testString += '\t';
    testString += '!';

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hello\n\t!") == 0);
  }

  SECTION("Operator += with Unicode content") {
    FixedString<64> testString("Hello");

    REQUIRE(testString.size() == 5);
    REQUIRE(std::strcmp(testString.c_str(), "Hello") == 0);

    testString += " 世界";

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello 世界") == 0);
  }

  SECTION("Operator += with different capacities") {
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

  SECTION("Operator += numeric content") {
    FixedString<16> testString("123");

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "123") == 0);

    testString += "456";

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "123456") == 0);
  }

  SECTION("Operator += mixed content") {
    FixedString<32> testString("Test");

    REQUIRE(testString.size() == 4);
    REQUIRE(std::strcmp(testString.c_str(), "Test") == 0);

    testString += " 123!";

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Test 123!") == 0);
  }

  SECTION("Operator += edge cases") {
    FixedString<16> testString("A");

    REQUIRE(testString.size() == 1);
    REQUIRE(std::strcmp(testString.c_str(), "A") == 0);

    testString += "BC";

    REQUIRE(testString.size() == 3);
    REQUIRE(std::strcmp(testString.c_str(), "ABC") == 0);
  }

  SECTION("Operator += and size consistency") {
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

  SECTION("Operator += chaining") {
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

  SECTION("Operator += and reassignment") {
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

  SECTION("Operator += with capacity preservation") {
    FixedString<16> testString("Hello");

    const auto originalCapacity = testString.capacity();
    const auto originalMaxSize = testString.max_size();

    REQUIRE(testString.size() == 5);

    testString += " World";

    REQUIRE(testString.size() == 11);
    REQUIRE(testString.capacity() == originalCapacity);
    REQUIRE(testString.max_size() == originalMaxSize);
  }

  SECTION("Operator += with different types") {
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

  SECTION("Operator += with empty operands") {
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

TEST_CASE("FixedString replace", "[core][fixed_string]") {
  SECTION("Replace with FixedString") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, FixedString<16>("Universe"));

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SECTION("Replace with std::string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, std::string("Universe"));

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SECTION("Replace with C-string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, "Universe");

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SECTION("Replace with repeated characters") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, '*', 3);

    REQUIRE(testString.size() == 9);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ***") == 0);
  }

  SECTION("Replace at beginning") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 5, "Hi");

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hi World") == 0);
  }

  SECTION("Replace at end") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, "Universe!");

    REQUIRE(testString.size() == 15);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe!") == 0);
  }

  SECTION("Replace with empty string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(5, 1, "");

    REQUIRE(testString.size() == 10);
    REQUIRE(std::strcmp(testString.c_str(), "HelloWorld") == 0);
  }

  SECTION("Replace with longer string") {
    FixedString<32> testString("Hi");

    REQUIRE(testString.size() == 2);
    REQUIRE(std::strcmp(testString.c_str(), "Hi") == 0);

    testString.replace(0, 2, "Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
  }

  SECTION("Replace with shorter string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 5, "Hi");

    REQUIRE(testString.size() == 8);
    REQUIRE(std::strcmp(testString.c_str(), "Hi World") == 0);
  }

  SECTION("Replace single character") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 1, "h");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "hello World") == 0);
  }

  SECTION("Replace multiple characters with single character") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 5, "H");

    REQUIRE(testString.size() == 7);
    REQUIRE(std::strcmp(testString.c_str(), "H World") == 0);
  }

  SECTION("Replace with repeated characters at different positions") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 1, '*', 2);
    testString.replace(8, 2, '#', 3);

    REQUIRE(testString.size() == 13);
    REQUIRE(std::strcmp(testString.c_str(), "**ello W###ld") == 0);
  }

  SECTION("Replace entire string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 11, "Goodbye Universe");

    REQUIRE(testString.size() == 16);
    REQUIRE(std::strcmp(testString.c_str(), "Goodbye Universe") == 0);
  }

  SECTION("Replace with zero count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 0, "Beautiful ");

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
  }

  SECTION("Replace with single character count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, '!', 1);

    REQUIRE(testString.size() == 7);
    REQUIRE(std::strcmp(testString.c_str(), "Hello !") == 0);
  }

  SECTION("Replace with multiple character count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, '=', 4);

    REQUIRE(testString.size() == 10);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ====") == 0);
  }

  SECTION("Replace with zero character count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, 'X', 0);

    REQUIRE(testString.size() == 6);
    REQUIRE(std::strcmp(testString.c_str(), "Hello ") == 0);
  }

  SECTION("Replace middle portion") {
    FixedString<32> testString("Hello Beautiful World");

    REQUIRE(testString.size() == 21);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);

    testString.replace(6, 9, "Amazing");

    REQUIRE(testString.size() == 19);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Amazing World") == 0);
  }

  SECTION("Replace with same length string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 5, "Greet");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Greet World") == 0);
  }

  SECTION("Replace with FixedString of different capacity") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, FixedString<12>("Universe"));

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SECTION("Replace with std::string") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, std::string("Universe"));

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SECTION("Replace with array") {
    FixedString<32> testString("Hello World");
    constexpr std::array<char, 9> arr = {'U', 'n', 'i', 'v', 'e', 'r', 's', 'e', '\0'};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(6, 5, arr.data());

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
  }

  SECTION("Replace at position 0 with zero count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(0, 0, "Hi ");

    REQUIRE(testString.size() == 14);
    REQUIRE(std::strcmp(testString.c_str(), "Hi Hello World") == 0);
  }

  SECTION("Replace at end with zero count") {
    FixedString<32> testString("Hello World");

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.replace(11, 0, "!");

    REQUIRE(testString.size() == 12);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);
  }
}

TEST_CASE("FixedString copy", "[core][fixed_string]") {
  SECTION("Copy entire string") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 11) == 11);
    REQUIRE(strncmp(buffer, "Hello World", 11) == 0);
  }

  SECTION("Copy partial string from beginning") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5) == 5);
    REQUIRE(strncmp(buffer, "Hello", 5) == 0);
  }

  SECTION("Copy partial string from middle") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with npos count") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, FixedString<16>::npos) == 11);
    REQUIRE(strncmp(buffer, "Hello World", 11) == 0);
  }

  SECTION("Copy with count exceeding remaining characters") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 20, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy from position 0") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5, 0) == 5);
    REQUIRE(strncmp(buffer, "Hello", 5) == 0);
  }

  SECTION("Copy single character") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 1, 6) == 1);
    REQUIRE(buffer[0] == 'W');
  }

  SECTION("Copy from end position") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 1, 10) == 1);
    REQUIRE(buffer[0] == 'd');
  }

  SECTION("Copy with zero count") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 0) == 0);
  }

  SECTION("Copy from empty string") {
    constexpr FixedString<16> testString("");
    char buffer[16] = {0};

    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);

    REQUIRE(testString.copy(buffer, 5) == 0);
  }

  SECTION("Copy to small buffer") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[3] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 2) == 2);
    REQUIRE(strncmp(buffer, "He", 2) == 0);
  }

  SECTION("Copy with position at end") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5, 11) == 0);
  }

  SECTION("Copy with npos from middle") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, FixedString<16>::npos, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with exact remaining characters") {
    constexpr FixedString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 11);
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    REQUIRE(testString.copy(buffer, 5, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with count larger than string size") {
    constexpr FixedString<16> testString("Hi");
    char buffer[16] = {0};

    REQUIRE(testString.size() == 2);
    REQUIRE(std::strcmp(testString.c_str(), "Hi") == 0);

    REQUIRE(testString.copy(buffer, 10) == 2);
    REQUIRE(strncmp(buffer, "Hi", 2) == 0);
  }
}

TEST_CASE("FixedString swap", "[core][fixed_string]") {
  SECTION("Swap two different strings") {
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

  SECTION("Swap strings of different lengths") {
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

  SECTION("Swap with empty string") {
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

  SECTION("Swap two empty strings") {
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

  SECTION("Self-swap (no-op)") {
    FixedString<16> string1("Hello World");

    REQUIRE(string1.size() == 11);
    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);

    string1.swap(string1);

    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);
    REQUIRE(string1.size() == 11);
  }

  SECTION("Swap with single character strings") {
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

  SECTION("Swap with maximum length strings") {
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

  SECTION("Chained swap operations") {
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

  SECTION("Swap with repeated characters") {
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

  SECTION("Swap with special characters") {
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

  SECTION("Swap with Unicode content") {
    FixedString<64> string1("Hello 世界");
    FixedString<64> string2("Привет мир");

    REQUIRE(string1.size() == 12);
    REQUIRE(std::strcmp(string1.c_str(), "Hello 世界") == 0);
    REQUIRE(string2.size() == 19);
    REQUIRE(std::strcmp(string2.c_str(), "Привет мир") == 0);

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "Привет мир") == 0);
    REQUIRE(string1.size() == 19);
    REQUIRE(std::strcmp(string2.c_str(), "Hello 世界") == 0);
    REQUIRE(string2.size() == 12);
  }

  SECTION("Swap with numeric content") {
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

  SECTION("Swap with mixed content") {
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

TEST_CASE("FixedString find", "[core][fixed_string]") {
  SECTION("Find FixedString substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find(FixedString<16>("World")) == 6);
    REQUIRE(testString.find(FixedString<16>("Hello")) == 0);
    REQUIRE(testString.find(FixedString<16>("lo Wo")) == 3);
    REQUIRE(testString.find(FixedString<16>("xyz")) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(FixedString<16>("World")) == 6);
    STATIC_REQUIRE(testString.find(FixedString<16>("Hello")) == 0);
    STATIC_REQUIRE(testString.find(FixedString<16>("lo Wo")) == 3);
    STATIC_REQUIRE(testString.find(FixedString<16>("xyz")) == FixedString<32>::npos);
  }

  SECTION("Find StringLike substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find(std::string("World")) == 6);
    REQUIRE(testString.find(std::string("Hello")) == 0);
    REQUIRE(testString.find(std::string("lo Wo")) == 3);
    REQUIRE(testString.find(std::string("xyz")) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(CStringView("World")) == 6);
    STATIC_REQUIRE(testString.find(CStringView("Hello")) == 0);
    STATIC_REQUIRE(testString.find(CStringView("lo Wo")) == 3);
    STATIC_REQUIRE(testString.find(CStringView("xyz")) == FixedString<32>::npos);
  }

  SECTION("Find C string substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("lo Wo") == 3);
    REQUIRE(testString.find("xyz") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("World") == 6);
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find("lo Wo") == 3);
    STATIC_REQUIRE(testString.find("xyz") == FixedString<32>::npos);
  }

  SECTION("Find character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find('H') == 0);
    REQUIRE(testString.find('l') == 2);
    REQUIRE(testString.find('o') == 4);
    REQUIRE(testString.find('W') == 6);
    REQUIRE(testString.find('d') == 10);
    REQUIRE(testString.find('x') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find('H') == 0);
    STATIC_REQUIRE(testString.find('l') == 2);
    STATIC_REQUIRE(testString.find('o') == 4);
    STATIC_REQUIRE(testString.find('W') == 6);
    STATIC_REQUIRE(testString.find('d') == 10);
    STATIC_REQUIRE(testString.find('x') == FixedString<32>::npos);
  }

  SECTION("Find with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == 12);
    REQUIRE(testString.find("Hello", 13) == FixedString<32>::npos);
    REQUIRE(testString.find('l', 0) == 2);
    REQUIRE(testString.find('l', 3) == 3);
    REQUIRE(testString.find('l', 4) == 9);
    REQUIRE(testString.find('l', 10) == 14);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("Hello", 0) == 0);
    STATIC_REQUIRE(testString.find("Hello", 1) == 12);
    STATIC_REQUIRE(testString.find("Hello", 13) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find('l', 0) == 2);
    STATIC_REQUIRE(testString.find('l', 3) == 3);
    STATIC_REQUIRE(testString.find('l', 4) == 9);
    STATIC_REQUIRE(testString.find('l', 10) == 14);
  }

  SECTION("Find empty substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find(FixedString<16>("")) == 0);
    REQUIRE(testString.find(std::string("")) == 0);
    REQUIRE(testString.find("") == 0);
    REQUIRE(testString.find("", 5) == 5);
    REQUIRE(testString.find("", 11) == 11);
    REQUIRE(testString.find("", 12) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(FixedString<16>("")) == 0);
    STATIC_REQUIRE(testString.find(CStringView("")) == 0);
    STATIC_REQUIRE(testString.find("") == 0);
    STATIC_REQUIRE(testString.find("", 5) == 5);
    STATIC_REQUIRE(testString.find("", 11) == 11);
    STATIC_REQUIRE(testString.find("", 12) == FixedString<32>::npos);
  }

  SECTION("Find in empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.find(FixedString<16>("Hello")) == FixedString<32>::npos);
    REQUIRE(testString.find(std::string("Hello")) == FixedString<32>::npos);
    REQUIRE(testString.find("Hello") == FixedString<32>::npos);
    REQUIRE(testString.find('H') == FixedString<32>::npos);
    REQUIRE(testString.find("") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(FixedString<16>("Hello")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find(CStringView("Hello")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find("Hello") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find('H') == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find("") == 0);
  }

  SECTION("Find with position beyond string size") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find("World", 10) == FixedString<32>::npos);
    REQUIRE(testString.find('H', 10) == FixedString<32>::npos);
    REQUIRE(testString.find("", 10) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("World", 10) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find('H', 10) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find("", 10) == FixedString<32>::npos);
  }

  SECTION("Find substring at end") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("d") == 10);
    REQUIRE(testString.find("ld") == 9);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("World") == 6);
    STATIC_REQUIRE(testString.find("d") == 10);
    STATIC_REQUIRE(testString.find("ld") == 9);
  }

  SECTION("Find substring at beginning") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("H") == 0);
    REQUIRE(testString.find("He") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find("H") == 0);
    STATIC_REQUIRE(testString.find("He") == 0);
  }

  SECTION("Find overlapping substrings") {
    constexpr FixedString<32> testString("ababab");

    REQUIRE(testString.find("ab") == 0);
    REQUIRE(testString.find("ab", 1) == 2);
    REQUIRE(testString.find("ab", 3) == 4);
    REQUIRE(testString.find("ab", 5) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("ab") == 0);
    STATIC_REQUIRE(testString.find("ab", 1) == 2);
    STATIC_REQUIRE(testString.find("ab", 3) == 4);
    STATIC_REQUIRE(testString.find("ab", 5) == FixedString<32>::npos);
  }

  SECTION("Find with repeated characters") {
    constexpr FixedString<32> testString("aaaaa");

    REQUIRE(testString.find("aa") == 0);
    REQUIRE(testString.find("aa", 1) == 1);
    REQUIRE(testString.find("aa", 2) == 2);
    REQUIRE(testString.find("aa", 3) == 3);
    REQUIRE(testString.find("aa", 4) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("aa") == 0);
    STATIC_REQUIRE(testString.find("aa", 1) == 1);
    STATIC_REQUIRE(testString.find("aa", 2) == 2);
    STATIC_REQUIRE(testString.find("aa", 3) == 3);
    STATIC_REQUIRE(testString.find("aa", 4) == FixedString<32>::npos);
  }

  SECTION("Find case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("hello") == FixedString<32>::npos);
    REQUIRE(testString.find("WORLD") == FixedString<32>::npos);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("World") == 6);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("hello") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find("WORLD") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find("World") == 6);
  }

  SECTION("Find with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find(FixedString<8>("World")) == 6);
    REQUIRE(testString.find(FixedString<16>("World")) == 6);
    REQUIRE(testString.find(FixedString<64>("World")) == 6);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(FixedString<8>("World")) == 6);
    STATIC_REQUIRE(testString.find(FixedString<16>("World")) == 6);
    STATIC_REQUIRE(testString.find(FixedString<64>("World")) == 6);
  }

  SECTION("Find with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find("Hello", 0) == 0);
    STATIC_REQUIRE(testString.find("Hello", 1) == FixedString<32>::npos);
  }

  SECTION("Find with single character string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.find("A") == 0);
    REQUIRE(testString.find('A') == 0);
    REQUIRE(testString.find("B") == FixedString<32>::npos);
    REQUIRE(testString.find('B') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("A") == 0);
    STATIC_REQUIRE(testString.find('A') == 0);
    STATIC_REQUIRE(testString.find("B") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find('B') == FixedString<32>::npos);
  }

  SECTION("Find with special characters") {
    constexpr FixedString<32> testString("Hello\n\tWorld!");

    REQUIRE(testString.find("\n") == 5);
    REQUIRE(testString.find("\t") == 6);
    REQUIRE(testString.find("!") == 12);
    REQUIRE(testString.find("\n\t") == 5);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("\n") == 5);
    STATIC_REQUIRE(testString.find("\t") == 6);
    STATIC_REQUIRE(testString.find("!") == 12);
    STATIC_REQUIRE(testString.find("\n\t") == 5);
  }

  SECTION("Find with Unicode content") {
    constexpr FixedString<64> testString("Hello 世界");

    REQUIRE(testString.find("世界") == 6);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find(" ") == 5);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("世界") == 6);
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find(" ") == 5);
  }

  SECTION("Find with numeric content") {
    constexpr FixedString<32> testString("12345Hello67890");

    REQUIRE(testString.find("12345") == 0);
    REQUIRE(testString.find("Hello") == 5);
    REQUIRE(testString.find("67890") == 10);
    REQUIRE(testString.find("456") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("12345") == 0);
    STATIC_REQUIRE(testString.find("Hello") == 5);
    STATIC_REQUIRE(testString.find("67890") == 10);
    STATIC_REQUIRE(testString.find("456") == FixedString<32>::npos);
  }

  SECTION("Find with mixed content") {
    constexpr FixedString<32> testString("Hello123World!@#");

    REQUIRE(testString.find("123") == 5);
    REQUIRE(testString.find("!@#") == 13);
    REQUIRE(testString.find("Hello123") == 0);
    REQUIRE(testString.find("World!@#") == 8);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("123") == 5);
    STATIC_REQUIRE(testString.find("!@#") == 13);
    STATIC_REQUIRE(testString.find("Hello123") == 0);
    STATIC_REQUIRE(testString.find("World!@#") == 8);
  }

  SECTION("Find with position edge cases") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == FixedString<32>::npos);
    REQUIRE(testString.find("World", 6) == 6);
    REQUIRE(testString.find("World", 7) == FixedString<32>::npos);
    REQUIRE(testString.find("", 0) == 0);
    REQUIRE(testString.find("", 11) == 11);
    REQUIRE(testString.find("", 12) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("Hello", 0) == 0);
    STATIC_REQUIRE(testString.find("Hello", 1) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find("World", 6) == 6);
    STATIC_REQUIRE(testString.find("World", 7) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find("", 0) == 0);
    STATIC_REQUIRE(testString.find("", 11) == 11);
    STATIC_REQUIRE(testString.find("", 12) == FixedString<32>::npos);
  }
}

TEST_CASE("FixedString rfind", "[core][fixed_string]") {
  SECTION("Rfind FixedString substring") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind(FixedString<16>("Hello")) == 12);
    REQUIRE(testString.rfind(FixedString<16>("World")) == 6);
    REQUIRE(testString.rfind(FixedString<16>("lo")) == 15);
    REQUIRE(testString.rfind(FixedString<16>("xyz")) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind(FixedString<16>("Hello")) == 12);
    STATIC_REQUIRE(testString.rfind(FixedString<16>("World")) == 6);
    STATIC_REQUIRE(testString.rfind(FixedString<16>("lo")) == 15);
    STATIC_REQUIRE(testString.rfind(FixedString<16>("xyz")) == FixedString<32>::npos);
  }

  SECTION("Rfind StringLike substring") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind(std::string("Hello")) == 12);
    REQUIRE(testString.rfind(std::string("World")) == 6);
    REQUIRE(testString.rfind(std::string("lo")) == 15);
    REQUIRE(testString.rfind(std::string("xyz")) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind(CStringView("Hello")) == 12);
    STATIC_REQUIRE(testString.rfind(CStringView("World")) == 6);
    STATIC_REQUIRE(testString.rfind(CStringView("lo")) == 15);
    STATIC_REQUIRE(testString.rfind(CStringView("xyz")) == FixedString<32>::npos);
  }

  SECTION("Rfind C string substring") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("lo") == 15);
    REQUIRE(testString.rfind("xyz") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello") == 12);
    STATIC_REQUIRE(testString.rfind("World") == 6);
    STATIC_REQUIRE(testString.rfind("lo") == 15);
    STATIC_REQUIRE(testString.rfind("xyz") == FixedString<32>::npos);
  }

  SECTION("Rfind character") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind('H') == 12);
    REQUIRE(testString.rfind('l') == 15);
    REQUIRE(testString.rfind('o') == 16);
    REQUIRE(testString.rfind('W') == 6);
    REQUIRE(testString.rfind('d') == 10);
    REQUIRE(testString.rfind('x') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind('H') == 12);
    STATIC_REQUIRE(testString.rfind('l') == 15);
    STATIC_REQUIRE(testString.rfind('o') == 16);
    STATIC_REQUIRE(testString.rfind('W') == 6);
    STATIC_REQUIRE(testString.rfind('d') == 10);
    STATIC_REQUIRE(testString.rfind('x') == FixedString<32>::npos);
  }

  SECTION("Rfind with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello", 12) == 12);
    REQUIRE(testString.rfind("Hello", 11) == 0);
    REQUIRE(testString.rfind("Hello", 0) == 0);
    REQUIRE(testString.rfind('l', 16) == 15);
    REQUIRE(testString.rfind('l', 13) == 9);
    REQUIRE(testString.rfind('l', 8) == 3);
    REQUIRE(testString.rfind('l', 2) == 2);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello", 12) == 12);
    STATIC_REQUIRE(testString.rfind("Hello", 11) == 0);
    STATIC_REQUIRE(testString.rfind("Hello", 0) == 0);
    STATIC_REQUIRE(testString.rfind('l', 16) == 15);
    STATIC_REQUIRE(testString.rfind('l', 13) == 9);
    STATIC_REQUIRE(testString.rfind('l', 8) == 3);
    STATIC_REQUIRE(testString.rfind('l', 2) == 2);
  }

  SECTION("Rfind empty substring") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.rfind(FixedString<16>("")) == 11);
    REQUIRE(testString.rfind(std::string("")) == 11);
    REQUIRE(testString.rfind("") == 11);
    REQUIRE(testString.rfind("", 5) == 5);
    REQUIRE(testString.rfind("", 0) == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind(FixedString<16>("")) == 11);
    STATIC_REQUIRE(testString.rfind(CStringView("")) == 11);
    STATIC_REQUIRE(testString.rfind("") == 11);
    STATIC_REQUIRE(testString.rfind("", 5) == 5);
    STATIC_REQUIRE(testString.rfind("", 0) == 0);
  }

  SECTION("Rfind in empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.rfind(FixedString<16>("Hello")) == FixedString<32>::npos);
    REQUIRE(testString.rfind(std::string("Hello")) == FixedString<32>::npos);
    REQUIRE(testString.rfind("Hello") == FixedString<32>::npos);
    REQUIRE(testString.rfind('H') == FixedString<32>::npos);
    REQUIRE(testString.rfind("") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind(FixedString<16>("Hello")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.rfind(CStringView("Hello")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.rfind("Hello") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.rfind('H') == FixedString<32>::npos);
    STATIC_REQUIRE(testString.rfind("") == 0);
  }

  SECTION("Rfind substring at end") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("d") == 10);
    REQUIRE(testString.rfind("ld") == 9);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("World") == 6);
    STATIC_REQUIRE(testString.rfind("d") == 10);
    STATIC_REQUIRE(testString.rfind("ld") == 9);
  }

  SECTION("Rfind substring at beginning") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("H") == 12);
    REQUIRE(testString.rfind("He") == 12);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello") == 12);
    STATIC_REQUIRE(testString.rfind("H") == 12);
    STATIC_REQUIRE(testString.rfind("He") == 12);
  }

  SECTION("Rfind overlapping substrings") {
    constexpr FixedString<32> testString("ababab");

    REQUIRE(testString.rfind("ab") == 4);
    REQUIRE(testString.rfind("ab", 3) == 2);
    REQUIRE(testString.rfind("ab", 1) == 0);
    REQUIRE(testString.rfind("ab", 0) == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("ab") == 4);
    STATIC_REQUIRE(testString.rfind("ab", 3) == 2);
    STATIC_REQUIRE(testString.rfind("ab", 1) == 0);
    STATIC_REQUIRE(testString.rfind("ab", 0) == 0);
  }

  SECTION("Rfind with repeated characters") {
    constexpr FixedString<32> testString("aaaaa");

    REQUIRE(testString.rfind("aa") == 3);
    REQUIRE(testString.rfind("aa", 2) == 2);
    REQUIRE(testString.rfind("aa", 1) == 1);
    REQUIRE(testString.rfind("aa", 0) == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("aa") == 3);
    STATIC_REQUIRE(testString.rfind("aa", 2) == 2);
    STATIC_REQUIRE(testString.rfind("aa", 1) == 1);
    STATIC_REQUIRE(testString.rfind("aa", 0) == 0);
  }

  SECTION("Rfind case sensitivity") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("hello") == FixedString<32>::npos);
    REQUIRE(testString.rfind("WORLD") == FixedString<32>::npos);
    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("hello") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.rfind("WORLD") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.rfind("Hello") == 12);
    STATIC_REQUIRE(testString.rfind("World") == 6);
  }

  SECTION("Rfind with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind(FixedString<8>("Hello")) == 12);
    REQUIRE(testString.rfind(FixedString<16>("Hello")) == 12);
    REQUIRE(testString.rfind(FixedString<64>("Hello")) == 12);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind(FixedString<8>("Hello")) == 12);
    STATIC_REQUIRE(testString.rfind(FixedString<16>("Hello")) == 12);
    STATIC_REQUIRE(testString.rfind(FixedString<64>("Hello")) == 12);
  }

  SECTION("Rfind with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.rfind("Hello") == 0);
    REQUIRE(testString.rfind("Hello", 0) == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello") == 0);
    STATIC_REQUIRE(testString.rfind("Hello", 0) == 0);
  }

  SECTION("Rfind with single character string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.rfind("A") == 0);
    REQUIRE(testString.rfind('A') == 0);
    REQUIRE(testString.rfind("B") == FixedString<32>::npos);
    REQUIRE(testString.rfind('B') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("A") == 0);
    STATIC_REQUIRE(testString.rfind('A') == 0);
    STATIC_REQUIRE(testString.rfind("B") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.rfind('B') == FixedString<32>::npos);
  }

  SECTION("Rfind with position 0") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.rfind("Hello", 0) == 0);
    REQUIRE(testString.rfind("World", 0) == FixedString<32>::npos);
    REQUIRE(testString.rfind('H', 0) == 0);
    REQUIRE(testString.rfind('W', 0) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello", 0) == 0);
    STATIC_REQUIRE(testString.rfind("World", 0) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.rfind('H', 0) == 0);
    STATIC_REQUIRE(testString.rfind('W', 0) == FixedString<32>::npos);
  }

  SECTION("Rfind with substring longer than string") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.rfind("Hello World") == FixedString<32>::npos);
    REQUIRE(testString.rfind("Hello World", 10) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello World") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.rfind("Hello World", 10) == FixedString<32>::npos);
  }

  SECTION("Rfind with multiple occurrences") {
    constexpr FixedString<32> testString("abababab");

    REQUIRE(testString.rfind("ab") == 6);
    REQUIRE(testString.rfind("ab", 5) == 4);
    REQUIRE(testString.rfind("ab", 3) == 2);
    REQUIRE(testString.rfind("ab", 1) == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("ab") == 6);
    STATIC_REQUIRE(testString.rfind("ab", 5) == 4);
    STATIC_REQUIRE(testString.rfind("ab", 3) == 2);
    STATIC_REQUIRE(testString.rfind("ab", 1) == 0);
  }

  SECTION("Rfind with position in middle") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello", 8) == 0);
    REQUIRE(testString.rfind("Hello", 12) == 12);
    REQUIRE(testString.rfind('l', 8) == 3);
    REQUIRE(testString.rfind('l', 15) == 15);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello", 8) == 0);
    STATIC_REQUIRE(testString.rfind("Hello", 12) == 12);
    STATIC_REQUIRE(testString.rfind('l', 8) == 3);
    STATIC_REQUIRE(testString.rfind('l', 15) == 15);
  }
}

TEST_CASE("FixedString find_first_of", "[core][fixed_string]") {
  SECTION("Find first of FixedString characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(FixedString<16>("aeiou")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_of(FixedString<16>("H")) == 0);
    REQUIRE(testString.find_first_of(FixedString<16>("d")) == 10);
    REQUIRE(testString.find_first_of(FixedString<16>("xyz")) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(FixedString<16>("aeiou")) == 1);
    STATIC_REQUIRE(testString.find_first_of(FixedString<16>("H")) == 0);
    STATIC_REQUIRE(testString.find_first_of(FixedString<16>("d")) == 10);
    STATIC_REQUIRE(testString.find_first_of(FixedString<16>("xyz")) == FixedString<32>::npos);
  }

  SECTION("Find first of StringLike characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(std::string("aeiou")) == 1);
    REQUIRE(testString.find_first_of(std::string("H")) == 0);
    REQUIRE(testString.find_first_of(std::string("d")) == 10);
    REQUIRE(testString.find_first_of(std::string("xyz")) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(CStringView("aeiou")) == 1);
    STATIC_REQUIRE(testString.find_first_of(CStringView("H")) == 0);
    STATIC_REQUIRE(testString.find_first_of(CStringView("d")) == 10);
    STATIC_REQUIRE(testString.find_first_of(CStringView("xyz")) == FixedString<32>::npos);
  }

  SECTION("Find first of C string characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou") == 1);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("d") == 10);
    REQUIRE(testString.find_first_of("xyz") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou") == 1);
    STATIC_REQUIRE(testString.find_first_of("H") == 0);
    STATIC_REQUIRE(testString.find_first_of("d") == 10);
    STATIC_REQUIRE(testString.find_first_of("xyz") == FixedString<32>::npos);
  }

  SECTION("Find first of single character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of('H') == 0);
    REQUIRE(testString.find_first_of('e') == 1);
    REQUIRE(testString.find_first_of('l') == 2);
    REQUIRE(testString.find_first_of('o') == 4);
    REQUIRE(testString.find_first_of('W') == 6);
    REQUIRE(testString.find_first_of('d') == 10);
    REQUIRE(testString.find_first_of('x') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of('H') == 0);
    STATIC_REQUIRE(testString.find_first_of('e') == 1);
    STATIC_REQUIRE(testString.find_first_of('l') == 2);
    STATIC_REQUIRE(testString.find_first_of('o') == 4);
    STATIC_REQUIRE(testString.find_first_of('W') == 6);
    STATIC_REQUIRE(testString.find_first_of('d') == 10);
    STATIC_REQUIRE(testString.find_first_of('x') == FixedString<32>::npos);
  }

  SECTION("Find first of with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("aeiou", 2) == 4);
    REQUIRE(testString.find_first_of("aeiou", 5) == 7);
    REQUIRE(testString.find_first_of("aeiou", 8) == 13);
    REQUIRE(testString.find_first_of("aeiou", 14) == 16);
    REQUIRE(testString.find_first_of("aeiou", 17) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 2) == 4);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 5) == 7);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 8) == 13);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 14) == 16);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 17) == FixedString<32>::npos);
  }

  SECTION("Find first of empty character set") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(FixedString<16>("")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of(std::string("")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(FixedString<16>("")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of(CStringView("")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of("") == FixedString<32>::npos);
  }

  SECTION("Find first of in empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.find_first_of(FixedString<16>("aeiou")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of(std::string("aeiou")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("aeiou") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of('a') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(FixedString<16>("aeiou")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of(CStringView("aeiou")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of("aeiou") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of('a') == FixedString<32>::npos);
  }

  SECTION("Find first of with position beyond string size") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_first_of("aeiou", 10) == FixedString<32>::npos);
    REQUIRE(testString.find_first_of('a', 10) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou", 10) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of('a', 10) == FixedString<32>::npos);
  }

  SECTION("Find first of with repeated characters") {
    constexpr FixedString<32> testString("aaaaa");

    REQUIRE(testString.find_first_of("a") == 0);
    REQUIRE(testString.find_first_of("ab") == 0);
    REQUIRE(testString.find_first_of("b") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("a") == 0);
    STATIC_REQUIRE(testString.find_first_of("ab") == 0);
    STATIC_REQUIRE(testString.find_first_of("b") == FixedString<32>::npos);
  }

  SECTION("Find first of with multiple character sets") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("Hl") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_of("lo") == 2); // 'l' at position 2
    REQUIRE(testString.find_first_of("Wr") == 6); // 'W' at position 6
    REQUIRE(testString.find_first_of("dl") == 2); // 'l' at position 2

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("Hl") == 0);
    STATIC_REQUIRE(testString.find_first_of("lo") == 2);
    STATIC_REQUIRE(testString.find_first_of("Wr") == 6);
    STATIC_REQUIRE(testString.find_first_of("dl") == 2);
  }

  SECTION("Find first of case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("h") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("w") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("W") == 6);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("h") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of("H") == 0);
    STATIC_REQUIRE(testString.find_first_of("w") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of("W") == 6);
  }

  SECTION("Find first of with special characters") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.find_first_of("!,") == 5); // ',' at position 5
    REQUIRE(testString.find_first_of("!") == 12); // '!' at position 12
    REQUIRE(testString.find_first_of(".,!") == 5); // ',' at position 5

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("!,") == 5);
    STATIC_REQUIRE(testString.find_first_of("!") == 12);
    STATIC_REQUIRE(testString.find_first_of(".,!") == 5);
  }

  SECTION("Find first of with numbers") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_first_of("0123456789") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("123") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("456") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("0123456789") == 5);
    STATIC_REQUIRE(testString.find_first_of("123") == 5);
    STATIC_REQUIRE(testString.find_first_of("456") == FixedString<32>::npos);
  }

  SECTION("Find first of with whitespace") {
    constexpr FixedString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_first_of(" \t\n") == 5); // ' ' at position 5
    REQUIRE(testString.find_first_of("\t") == 11);
    REQUIRE(testString.find_first_of("\n") == 12);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(" \t\n") == 5);
    STATIC_REQUIRE(testString.find_first_of("\t") == 11);
    STATIC_REQUIRE(testString.find_first_of("\n") == 12);
  }

  SECTION("Find first of with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(FixedString<8>("aeiou")) == 1);
    REQUIRE(testString.find_first_of(FixedString<16>("aeiou")) == 1);
    REQUIRE(testString.find_first_of(FixedString<64>("aeiou")) == 1);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(FixedString<8>("aeiou")) == 1);
    STATIC_REQUIRE(testString.find_first_of(FixedString<16>("aeiou")) == 1);
    STATIC_REQUIRE(testString.find_first_of(FixedString<64>("aeiou")) == 1);
  }

  SECTION("Find first of with single character string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.find_first_of("A") == 0);
    REQUIRE(testString.find_first_of('A') == 0);
    REQUIRE(testString.find_first_of("B") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of('B') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("A") == 0);
    STATIC_REQUIRE(testString.find_first_of('A') == 0);
    STATIC_REQUIRE(testString.find_first_of("B") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of('B') == FixedString<32>::npos);
  }

  SECTION("Find first of with position 0") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("H", 0) == 0);
    REQUIRE(testString.find_first_of("xyz", 0) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    STATIC_REQUIRE(testString.find_first_of("H", 0) == 0);
    STATIC_REQUIRE(testString.find_first_of("xyz", 0) == FixedString<32>::npos);
  }

  SECTION("Find first of with all characters found") {
    constexpr FixedString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_of("aeiou") == 0);
    REQUIRE(testString.find_first_of("xyz") == 23);
    REQUIRE(testString.find_first_of("z") == 25);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou") == 0);
    STATIC_REQUIRE(testString.find_first_of("xyz") == 23);
    STATIC_REQUIRE(testString.find_first_of("z") == 25);
  }

  SECTION("Find first of with no characters found") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("0123456789") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("!@#$%^&*()") == FixedString<32>::npos);
    REQUIRE(testString.find_first_of("[]{}|\\:;\"'<>?/") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("0123456789") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of("!@#$%^&*()") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_of("[]{}|\\:;\"'<>?/") == FixedString<32>::npos);
  }
}

TEST_CASE("FixedString find_first_not_of", "[core][fixed_string]") {
  SECTION("Find first not of FixedString characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(FixedString<16>("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(FixedString<16>("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(FixedString<16>("Helo Wrd")) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<16>("H")) == 1);
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<16>("Hel")) == 4);
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<16>("Helo Wrd")) == FixedString<32>::npos);
  }

  SECTION("Find first not of StringLike characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(std::string("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(std::string("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(std::string("Helo Wrd")) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("H")) == 1);
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("Hel")) == 4);
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("Helo Wrd")) == FixedString<32>::npos);
  }

  SECTION("Find first not of C string characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("H") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Helo Wrd") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("H") == 1);
    STATIC_REQUIRE(testString.find_first_not_of("Hel") == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Helo Wrd") == FixedString<32>::npos);
  }

  SECTION("Find first not of single character") {
    constexpr FixedString<32> testString("aaaaab");

    REQUIRE(testString.find_first_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_first_not_of('b') == 0); // 'a' at position 0
    REQUIRE(testString.find_first_not_of('x') == 0); // 'a' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of('a') == 5);
    STATIC_REQUIRE(testString.find_first_not_of('b') == 0);
    STATIC_REQUIRE(testString.find_first_not_of('x') == 0);
  }

  SECTION("Find first not of with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_first_not_of("Hel", 0) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 5) == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Hel", 6) == 6); // 'W' at position 6
    REQUIRE(testString.find_first_not_of("Hel", 7) == 7); // 'o' at position 7

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 0) == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 4) == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 5) == 5);
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 6) == 6);
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 7) == 7);
  }

  SECTION("Find first not of empty character set") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(FixedString<16>("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of(std::string("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("", 5) == 5); // ' ' at position 5

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<16>("")) == 0);
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("")) == 0);
    STATIC_REQUIRE(testString.find_first_not_of("") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("", 5) == 5);
  }

  SECTION("Find first not of in empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.find_first_not_of(FixedString<16>("aeiou")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of(std::string("aeiou")) == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("aeiou") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of('a') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<16>("aeiou")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("aeiou")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("aeiou") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of('a') == FixedString<32>::npos);
  }

  SECTION("Find first not of with position beyond string size") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_first_not_of("aeiou", 10) == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of('a', 10) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("aeiou", 10) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of('a', 10) == FixedString<32>::npos);
  }

  SECTION("Find first not of with repeated characters") {
    constexpr FixedString<32> testString("aaaaa");

    REQUIRE(testString.find_first_not_of("a") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("ab") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("b") == 0); // 'a' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("a") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("ab") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("b") == 0);
  }

  SECTION("Find first not of with multiple character sets") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("Hl") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Helo") == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Helo ") == 6); // 'W' at position 6

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("Hl") == 1);
    STATIC_REQUIRE(testString.find_first_not_of("Hel") == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Helo") == 5);
    STATIC_REQUIRE(testString.find_first_not_of("Helo ") == 6);
  }

  SECTION("Find first not of case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("h") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("H") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("w") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("W") == 0); // 'H' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("h") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("H") == 1);
    STATIC_REQUIRE(testString.find_first_not_of("w") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("W") == 0);
  }

  SECTION("Find first not of with special characters") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.find_first_not_of("Helo, Wrd!") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo, Wrd") == 12); // '!' at position 12

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("Helo, Wrd!") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Helo, Wrd") == 12);
  }

  SECTION("Find first not of with numbers") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo123Wrd") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("0123456789") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wrd") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wr") == 12);
  }

  SECTION("Find first not of with whitespace") {
    constexpr FixedString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_first_not_of(" \t\n") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo Wrd\t\n") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo Wrd") == 11); // '\t' at position 11

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(" \t\n") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("Helo Wrd\t\n") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Helo Wrd") == 11);
  }

  SECTION("Find first not of with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(FixedString<8>("H")) == 1);
    REQUIRE(testString.find_first_not_of(FixedString<16>("H")) == 1);
    REQUIRE(testString.find_first_not_of(FixedString<64>("H")) == 1);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<8>("H")) == 1);
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<16>("H")) == 1);
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<64>("H")) == 1);
  }

  SECTION("Find first not of with single character string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.find_first_not_of("A") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of('A') == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("B") == 0); // 'A' at position 0
    REQUIRE(testString.find_first_not_of('B') == 0); // 'A' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("A") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of('A') == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("B") == 0);
    STATIC_REQUIRE(testString.find_first_not_of('B') == 0);
  }

  SECTION("Find first not of with position 0") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("H", 0) == 1);
    REQUIRE(testString.find_first_not_of("Hel", 0) == 4);
    REQUIRE(testString.find_first_not_of("Helo Wrd", 0) == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("H", 0) == 1);
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 0) == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Helo Wrd", 0) == FixedString<32>::npos);
  }

  SECTION("Find first not of with all characters excluded") {
    constexpr FixedString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24); // 'y' at position 24

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25);
    STATIC_REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24);
  }

  SECTION("Find first not of with no characters excluded") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("xyz") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("!@#$%^&*()") == 0); // 'H' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("xyz") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("0123456789") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("!@#$%^&*()") == 0);
  }

  SECTION("Find first not of with mixed content") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_first_not_of("Helo123Wrd") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12
    REQUIRE(testString.find_first_not_of("Helo123Wd") == 10); // 'r' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wrd") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wr") == 12);
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wd") == 10);
  }

  SECTION("Find first not of with position in middle") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_first_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 5) == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Hel", 6) == 6); // 'W' at position 6
    REQUIRE(testString.find_first_not_of("Hel", 7) == 7); // 'o' at position 7

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 4) == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 5) == 5);
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 6) == 6);
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 7) == 7);
  }

  SECTION("Find first not of with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_first_not_of("Hello") == FixedString<32>::npos);
    REQUIRE(testString.find_first_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("Hello") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Hell") == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Hel") == 4);
  }
}

TEST_CASE("FixedString find_last_of", "[core][fixed_string]") {
  SECTION("Find last of FixedString characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of(FixedString<16>("aeiou")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of(FixedString<16>("l")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of(FixedString<16>("H")) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of(FixedString<16>("d")) == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of(FixedString<16>("aeiou")) == 7);
    STATIC_REQUIRE(testString.find_last_of(FixedString<16>("l")) == 9);
    STATIC_REQUIRE(testString.find_last_of(FixedString<16>("H")) == 0);
    STATIC_REQUIRE(testString.find_last_of(FixedString<16>("d")) == 10);
  }

  SECTION("Find last of StringLike characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of(std::string("aeiou")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of(std::string("l")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of(std::string("H")) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of(std::string("d")) == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of(CStringView("aeiou")) == 7);
    STATIC_REQUIRE(testString.find_last_of(CStringView("l")) == 9);
    STATIC_REQUIRE(testString.find_last_of(CStringView("H")) == 0);
    STATIC_REQUIRE(testString.find_last_of(CStringView("d")) == 10);
  }

  SECTION("Find last of C string characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("aeiou") == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of("l") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("H") == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("d") == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("aeiou") == 7);
    STATIC_REQUIRE(testString.find_last_of("l") == 9);
    STATIC_REQUIRE(testString.find_last_of("H") == 0);
    STATIC_REQUIRE(testString.find_last_of("d") == 10);
  }

  SECTION("Find last of single character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of('l') == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of('o') == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of('H') == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of('d') == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of('l') == 9);
    STATIC_REQUIRE(testString.find_last_of('o') == 7);
    STATIC_REQUIRE(testString.find_last_of('H') == 0);
    STATIC_REQUIRE(testString.find_last_of('d') == 10);
  }

  SECTION("Find last of with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    REQUIRE(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1
    REQUIRE(testString.find_last_of("Hel", 0) == 0); // 'H' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Hel", 8) == 3);
    STATIC_REQUIRE(testString.find_last_of("Hel", 4) == 3);
    STATIC_REQUIRE(testString.find_last_of("Hel", 2) == 2);
    STATIC_REQUIRE(testString.find_last_of("Hel", 1) == 1);
    STATIC_REQUIRE(testString.find_last_of("Hel", 0) == 0);
  }

  SECTION("Find last of empty character set") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of(FixedString<16>("")) == FixedString<32>::npos);
    REQUIRE(testString.find_last_of(std::string("")) == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of(FixedString<16>("")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_last_of(CStringView("")) == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_last_of("") == FixedString<32>::npos);
  }

  SECTION("Find last of with no characters found") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("0123456789") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("!@#$%^&*()") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("[]{}|\\:;\"'<>?/") == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("0123456789") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_last_of("!@#$%^&*()") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_last_of("[]{}|\\:;\"'<>?/") == FixedString<32>::npos);
  }

  SECTION("Find last of with multiple character sets") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("Hl") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Hel") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo ") == 9); // 'l' at position 9

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Hl") == 9);
    STATIC_REQUIRE(testString.find_last_of("Hel") == 9);
    STATIC_REQUIRE(testString.find_last_of("Helo") == 9);
    STATIC_REQUIRE(testString.find_last_of("Helo ") == 9);
  }

  SECTION("Find last of case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("h") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("H") == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("w") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of("W") == 6); // 'W' at position 6

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("h") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_last_of("H") == 0);
    STATIC_REQUIRE(testString.find_last_of("w") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_last_of("W") == 6);
  }

  SECTION("Find last of with special characters") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.find_last_of("Helo, Wrd!") == 12); // '!' at position 12
    REQUIRE(testString.find_last_of("Helo, Wrd") == 11); // 'd' at position 11
    REQUIRE(testString.find_last_of("Helo, Wr") == 10); // 'l' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Helo, Wrd!") == 12);
    STATIC_REQUIRE(testString.find_last_of("Helo, Wrd") == 11);
    STATIC_REQUIRE(testString.find_last_of("Helo, Wr") == 10);
  }

  SECTION("Find last of with numbers") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_last_of("0123456789") == 7); // '3' at position 7
    REQUIRE(testString.find_last_of("Helo123Wrd") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_of("Helo123Wr") == 11); // 'l' at position 11

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("0123456789") == 7);
    STATIC_REQUIRE(testString.find_last_of("Helo123Wrd") == 12);
    STATIC_REQUIRE(testString.find_last_of("Helo123Wr") == 11);
  }

  SECTION("Find last of with whitespace") {
    constexpr FixedString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_last_of(" \t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd\t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of(" \t\n") == 12);
    STATIC_REQUIRE(testString.find_last_of("Helo Wrd\t\n") == 12);
    STATIC_REQUIRE(testString.find_last_of("Helo Wrd") == 10);
  }

  SECTION("Find last of with repeated characters") {
    constexpr FixedString<32> testString("aaaaab");

    REQUIRE(testString.find_last_of('a') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_of('b') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_of('x') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of('a') == 4);
    STATIC_REQUIRE(testString.find_last_of('b') == 5);
    STATIC_REQUIRE(testString.find_last_of('x') == FixedString<32>::npos);
  }

  SECTION("Find last of with single character string") {
    constexpr FixedString<32> testString("AAAAA");

    REQUIRE(testString.find_last_of("A") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of('A') == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of("B") == FixedString<32>::npos);
    REQUIRE(testString.find_last_of('B') == FixedString<32>::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("A") == 4);
    STATIC_REQUIRE(testString.find_last_of('A') == 4);
    STATIC_REQUIRE(testString.find_last_of("B") == FixedString<32>::npos);
    STATIC_REQUIRE(testString.find_last_of('B') == FixedString<32>::npos);
  }

  SECTION("Find last of with alphabet") {
    constexpr FixedString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24); // 'y' at position 24
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23); // 'x' at position 23

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25);
    STATIC_REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24);
    STATIC_REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23);
  }

  SECTION("Find last of with all characters found") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_of("Helo Wr") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo W") == 9); // 'l' at position 9

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Helo Wrd") == 10);
    STATIC_REQUIRE(testString.find_last_of("Helo Wr") == 9);
    STATIC_REQUIRE(testString.find_last_of("Helo W") == 9);
  }

  SECTION("Find last of with position in middle") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    REQUIRE(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Hel", 8) == 3);
    STATIC_REQUIRE(testString.find_last_of("Hel", 4) == 3);
    STATIC_REQUIRE(testString.find_last_of("Hel", 2) == 2);
    STATIC_REQUIRE(testString.find_last_of("Hel", 1) == 1);
  }

  SECTION("Find last of with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_last_of("Hello") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_of("Hell") == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel") == 3); // 'l' at position 3

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Hello") == 4);
    STATIC_REQUIRE(testString.find_last_of("Hell") == 3);
    STATIC_REQUIRE(testString.find_last_of("Hel") == 3);
  }
}

// to refactor 6684 - 6143 = 541

TEST_CASE("FixedString find_last_not_of", "[core][fixed_string]") {
  SECTION("Find last not of FixedString characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of(FixedString<16>("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(FixedString<16>("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(FixedString<16>("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(FixedString<16>("World")) == 5); // ' ' at position 5
  }

  SECTION("Find last not of StringLike characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of(std::string("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(std::string("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(std::string("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(std::string("World")) == 5); // ' ' at position 5
  }

  SECTION("Find last not of C string characters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("d") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of("ld") == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("rld") == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of("World") == 5); // ' ' at position 5
  }

  SECTION("Find last not of single character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of('d') == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of('l') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('o') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('H') == 10); // 'd' at position 10
  }

  SECTION("Find last not of with position parameter") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 0) == FixedString<32>::npos);
  }

  SECTION("Find last not of empty character set") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of(FixedString<16>("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of(std::string("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("", 5) == 5); // ' ' at position 5
  }

  SECTION("Find last not of with all characters excluded") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("Helo Wrd") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo Wr") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo W") == 10); // 'd' at position 10
  }

  SECTION("Find last not of with multiple character sets") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("Hl") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Hel") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo ") == 10); // 'd' at position 10
  }

  SECTION("Find last not of case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("h") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("H") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("w") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("W") == 10); // 'd' at position 10
  }

  SECTION("Find last not of with special characters") {
    constexpr FixedString<32> testString("Hello, World!");

    REQUIRE(testString.find_last_not_of("Helo, Wrd!") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo, Wrd") == 12); // '!' at position 12
    REQUIRE(testString.find_last_not_of("Helo, Wr") == 12); // '!' at position 12
  }

  SECTION("Find last not of with numbers") {
    constexpr FixedString<32> testString("Hello123World");

    REQUIRE(testString.find_last_not_of("0123456789") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_not_of("Helo123Wrd") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo123Wr") == 12); // 'd' at position 12
  }

  SECTION("Find last not of with whitespace") {
    constexpr FixedString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_last_not_of(" \t\n") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo Wrd\t\n") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo Wrd") == 12); // '\n' at position 12
  }

  SECTION("Find last not of with repeated characters") {
    constexpr FixedString<32> testString("aaaaab");

    REQUIRE(testString.find_last_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_not_of('b') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_not_of('x') == 5); // 'b' at position 5
  }

  SECTION("Find last not of with single character string") {
    constexpr FixedString<32> testString("AAAAA");

    REQUIRE(testString.find_last_not_of("A") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of('A') == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("B") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_not_of('B') == 4); // 'A' at position 4
  }

  SECTION("Find last not of with alphabet") {
    constexpr FixedString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxyz") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwx") == 25); // 'z' at position 25
  }

  SECTION("Find last not of with no characters excluded") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("xyz") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("0123456789") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("!@#$%^&*()") == 10); // 'd' at position 10
  }

  SECTION("Find last not of with position in middle") {
    constexpr FixedString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == FixedString<32>::npos);
  }

  SECTION("Find last not of with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.find_last_not_of("Hello") == FixedString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel") == 4); // 'o' at position 4
  }
}

TEST_CASE("FixedString compare", "[core][fixed_string]") {
  SECTION("Compare FixedString with FixedString") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hello");
    constexpr FixedString<32> testString3("World");
    constexpr FixedString<32> testString4("Hell");

    REQUIRE(testString1.compare(testString2) == 0);
    REQUIRE(testString1.compare(testString3) < 0);
    REQUIRE(testString1.compare(testString4) > 0);
  }

  SECTION("Compare FixedString with StringLike") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.compare(std::string("Hello")) == 0);
    REQUIRE(testString.compare(std::string("World")) < 0);
    REQUIRE(testString.compare(std::string("Hell")) > 0);
  }

  SECTION("Compare FixedString with C string") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.compare("Hello") == 0);
    REQUIRE(testString.compare("World") < 0);
    REQUIRE(testString.compare("Hell") > 0);
  }

  SECTION("Compare identical strings") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.compare(FixedString<32>("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare("Hello World") == 0);
  }

  SECTION("Compare with empty strings") {
    constexpr FixedString<32> testString1("");
    constexpr FixedString<32> testString2("Hello");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("") == 0);
    REQUIRE(testString1.compare(std::string("")) == 0);
  }

  SECTION("Compare strings with different lengths") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hello World");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
  }

  SECTION("Compare strings with same prefix") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hell");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
  }

  SECTION("Compare strings with different first character") {
    constexpr FixedString<32> testString1("Apple");
    constexpr FixedString<32> testString2("Banana");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
  }

  SECTION("Compare strings with different middle character") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hallo");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
  }

  SECTION("Compare strings with different last character") {
    constexpr FixedString<32> testString1("Hello");
    constexpr FixedString<32> testString2("Hellp");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
  }

  SECTION("Compare case sensitivity") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.compare("hello") < 0);
    REQUIRE(testString.compare("HELLO") > 0);
    REQUIRE(testString.compare("Hello") == 0);
  }

  SECTION("Compare with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.compare(FixedString<8>("Hello")) == 0);
    REQUIRE(testString.compare(FixedString<16>("Hello")) == 0);
    REQUIRE(testString.compare(FixedString<64>("Hello")) == 0);
    REQUIRE(testString.compare(FixedString<8>("World")) < 0);
    REQUIRE(testString.compare(FixedString<16>("Hell")) > 0);
  }

  SECTION("Compare with single character strings") {
    constexpr FixedString<32> testString1("A");
    constexpr FixedString<32> testString2("B");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("A") == 0);
    REQUIRE(testString1.compare("B") < 0);
  }

  SECTION("Compare with repeated characters") {
    constexpr FixedString<32> testString1("aaa");
    constexpr FixedString<32> testString2("aa");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("aaa") == 0);
    REQUIRE(testString1.compare("aa") > 0);
  }

  SECTION("Compare with special characters") {
    constexpr FixedString<32> testString1("Hello!");
    constexpr FixedString<32> testString2("Hello");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("Hello!") == 0);
    REQUIRE(testString1.compare("Hello") > 0);
  }

  SECTION("Compare with numbers") {
    constexpr FixedString<32> testString1("123");
    constexpr FixedString<32> testString2("456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123") == 0);
    REQUIRE(testString1.compare("456") < 0);
  }

  SECTION("Compare with mixed content") {
    constexpr FixedString<32> testString1("Hello123");
    constexpr FixedString<32> testString2("Hello456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("Hello123") == 0);
    REQUIRE(testString1.compare("Hello456") < 0);
  }

  SECTION("Compare with maximum length strings") {
    constexpr FixedString<16> testString1("123456789012345");
    constexpr FixedString<16> testString2("123456789012346");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123456789012345") == 0);
    REQUIRE(testString1.compare("123456789012346") < 0);
  }

  SECTION("Compare with std::string") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello")) > 0);
    REQUIRE(testString.compare(std::string("World")) < 0);
  }

  SECTION("Compare with array") {
    constexpr FixedString<32> testString("Hello");
    constexpr std::array<char, 6> arr = {'H', 'e', 'l', 'l', 'o', '\0'};

    REQUIRE(testString.compare(arr.data()) == 0);
    REQUIRE(testString.compare("Hello") == 0);
  }

  SECTION("Compare edge cases") {
    constexpr FixedString<32> testString("Hello");

    // Compare with null-terminated string
    REQUIRE(testString.compare("Hello\0World") == 0);

    // Compare with string containing null character
    constexpr FixedString<32> testStringWithNull("Hello\0World");
    REQUIRE(testString.compare(testStringWithNull) == 0);
  }
}

TEST_CASE("FixedString starts_with", "[core][fixed_string]") {
  SECTION("Starts with FixedString") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with(FixedString<16>("Hello")) == true);
    REQUIRE(testString.starts_with(FixedString<16>("Hello World")) == true);
    REQUIRE(testString.starts_with(FixedString<16>("H")) == true);
    REQUIRE(testString.starts_with(FixedString<16>("World")) == false);
    REQUIRE(testString.starts_with(FixedString<16>("xyz")) == false);
    REQUIRE(testString.starts_with(FixedString<16>("")) == true);
  }

  SECTION("Starts with StringLike") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")) == true);
    REQUIRE(testString.starts_with(std::string("Hello World")) == true);
    REQUIRE(testString.starts_with(std::string("H")) == true);
    REQUIRE(testString.starts_with(std::string("World")) == false);
    REQUIRE(testString.starts_with(std::string("xyz")) == false);
    REQUIRE(testString.starts_with(std::string("")) == true);
  }

  SECTION("Starts with C string") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hello World") == true);
    REQUIRE(testString.starts_with("H") == true);
    REQUIRE(testString.starts_with("World") == false);
    REQUIRE(testString.starts_with("xyz") == false);
    REQUIRE(testString.starts_with("") == true);
  }

  SECTION("Starts with character") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with('H') == true);
    REQUIRE(testString.starts_with('h') == false);
    REQUIRE(testString.starts_with('W') == false);
    REQUIRE(testString.starts_with('x') == false);
  }

  SECTION("Starts with empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.starts_with(FixedString<16>("Hello")) == false);
    REQUIRE(testString.starts_with(std::string("Hello")) == false);
    REQUIRE(testString.starts_with("Hello") == false);
    REQUIRE(testString.starts_with('H') == false);
    REQUIRE(testString.starts_with("") == true);
  }

  SECTION("Starts with single character string") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.starts_with("A") == true);
    REQUIRE(testString.starts_with('A') == true);
    REQUIRE(testString.starts_with("B") == false);
    REQUIRE(testString.starts_with('B') == false);
    REQUIRE(testString.starts_with("") == true);
  }

  SECTION("Starts with longer prefix") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.starts_with("Hello World") == false);
    REQUIRE(testString.starts_with("Hello Universe") == false);
    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hell") == true);
  }

  SECTION("Starts with case sensitivity") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with("hello") == false);
    REQUIRE(testString.starts_with("HELLO") == false);
    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with('h') == false);
    REQUIRE(testString.starts_with('H') == true);
  }

  SECTION("Starts with different FixedString capacities") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with(FixedString<8>("Hello")) == true);
    REQUIRE(testString.starts_with(FixedString<16>("Hello")) == true);
    REQUIRE(testString.starts_with(FixedString<64>("Hello")) == true);
    REQUIRE(testString.starts_with(FixedString<8>("World")) == false);
  }

  SECTION("Starts with repeated characters") {
    constexpr FixedString<32> testString("aaaab");

    REQUIRE(testString.starts_with("aaa") == true);
    REQUIRE(testString.starts_with("aaaa") == true);
    REQUIRE(testString.starts_with("aaaab") == true);
    REQUIRE(testString.starts_with("aaab") == false);
    REQUIRE(testString.starts_with('a') == true);
    REQUIRE(testString.starts_with('b') == false);
  }

  SECTION("Starts with special characters") {
    constexpr FixedString<32> testString("!@#$%");

    REQUIRE(testString.starts_with("!@#") == true);
    REQUIRE(testString.starts_with("!@#$%") == true);
    REQUIRE(testString.starts_with("!@#$%^") == false);
    REQUIRE(testString.starts_with('!') == true);
    REQUIRE(testString.starts_with('@') == false);
  }

  SECTION("Starts with numbers") {
    constexpr FixedString<32> testString("12345");

    REQUIRE(testString.starts_with("123") == true);
    REQUIRE(testString.starts_with("12345") == true);
    REQUIRE(testString.starts_with("123456") == false);
    REQUIRE(testString.starts_with('1') == true);
    REQUIRE(testString.starts_with('2') == false);
  }

  SECTION("Starts with mixed content") {
    constexpr FixedString<32> testString("Hello123");

    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hello1") == true);
    REQUIRE(testString.starts_with("Hello123") == true);
    REQUIRE(testString.starts_with("Hello1234") == false);
    REQUIRE(testString.starts_with('H') == true);
    REQUIRE(testString.starts_with('1') == false);
  }

  SECTION("Starts with maximum length strings") {
    constexpr FixedString<16> testString("123456789012345");

    REQUIRE(testString.starts_with("123456789012345") == true);
    REQUIRE(testString.starts_with("12345678901234") == true);
    REQUIRE(testString.starts_with("1234567890123456") == false);
    REQUIRE(testString.starts_with('1') == true);
    REQUIRE(testString.starts_with('5') == false);
  }

  SECTION("Starts with std::string") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")) == true);
    REQUIRE(testString.starts_with(std::string("Hello World")) == true);
    REQUIRE(testString.starts_with(std::string("World")) == false);
  }

  SECTION("Starts with array") {
    constexpr FixedString<32> testString("Hello");
    constexpr std::array<char, 4> arr = {'H', 'e', 'l', '\0'};

    REQUIRE(testString.starts_with(arr.data()) == true);
    REQUIRE(testString.starts_with("Hel") == true);
  }

  SECTION("Starts with edge cases") {
    constexpr FixedString<32> testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.starts_with("Hello\0World") == true);

    // Test with string containing null character
    constexpr FixedString<32> testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.starts_with("Hello") == true);
  }

  SECTION("Starts with whitespace") {
    constexpr FixedString<32> testString(" Hello World");

    REQUIRE(testString.starts_with(" ") == true);
    REQUIRE(testString.starts_with(" Hello") == true);
    REQUIRE(testString.starts_with("Hello") == false);
    REQUIRE(testString.starts_with(' ') == true);
    REQUIRE(testString.starts_with('H') == false);
  }

  SECTION("Starts with exact match") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hell") == true);
    REQUIRE(testString.starts_with("H") == true);
    REQUIRE(testString.starts_with("") == true);
  }
}

// after refactor

TEST_CASE("FixedString ends_with", "[core][fixed_string]") {
  SECTION("FixedString ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with(FixedString<16>("World")) == true);
    REQUIRE(testString.ends_with(FixedString<16>("Hello World")) == true);
    REQUIRE(testString.ends_with(FixedString<16>("d")) == true);
    REQUIRE(testString.ends_with(FixedString<16>("Hello")) == false);
    REQUIRE(testString.ends_with(FixedString<16>("xyz")) == false);
    REQUIRE(testString.ends_with(FixedString<16>("")) == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("World")) == true);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("Hello World")) == true);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("d")) == true);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("Hello")) == false);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("xyz")) == false);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("")) == true);
  }

  SECTION("StringLike ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with(std::string("World")) == true);
    REQUIRE(testString.ends_with(std::string("Hello World")) == true);
    REQUIRE(testString.ends_with(std::string("d")) == true);
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("xyz")) == false);
    REQUIRE(testString.ends_with(std::string("")) == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(CStringView("World")) == true);
    STATIC_REQUIRE(testString.ends_with(CStringView("Hello World")) == true);
    STATIC_REQUIRE(testString.ends_with(CStringView("d")) == true);
    STATIC_REQUIRE(testString.ends_with(CStringView("Hello")) == false);
    STATIC_REQUIRE(testString.ends_with(CStringView("xyz")) == false);
    STATIC_REQUIRE(testString.ends_with(CStringView("")) == true);
  }

  SECTION("C string ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with("World") == true);
    REQUIRE(testString.ends_with("Hello World") == true);
    REQUIRE(testString.ends_with("d") == true);
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with("xyz") == false);
    REQUIRE(testString.ends_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("World") == true);
    STATIC_REQUIRE(testString.ends_with("Hello World") == true);
    STATIC_REQUIRE(testString.ends_with("d") == true);
    STATIC_REQUIRE(testString.ends_with("Hello") == false);
    STATIC_REQUIRE(testString.ends_with("xyz") == false);
    STATIC_REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Character ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with('d') == true);
    REQUIRE(testString.ends_with('D') == false);
    REQUIRE(testString.ends_with('H') == false);
    REQUIRE(testString.ends_with('x') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with('d') == true);
    STATIC_REQUIRE(testString.ends_with('D') == false);
    STATIC_REQUIRE(testString.ends_with('H') == false);
    STATIC_REQUIRE(testString.ends_with('x') == false);
  }

  SECTION("Empty string ends_with") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.ends_with(FixedString<16>("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with('H') == false);
    REQUIRE(testString.ends_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("Hello")) == false);
    STATIC_REQUIRE(testString.ends_with(CStringView("Hello")) == false);
    STATIC_REQUIRE(testString.ends_with("Hello") == false);
    STATIC_REQUIRE(testString.ends_with('H') == false);
    STATIC_REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Single character string ends_with") {
    constexpr FixedString<32> testString("A");

    REQUIRE(testString.ends_with("A") == true);
    REQUIRE(testString.ends_with('A') == true);
    REQUIRE(testString.ends_with("B") == false);
    REQUIRE(testString.ends_with('B') == false);
    REQUIRE(testString.ends_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("A") == true);
    STATIC_REQUIRE(testString.ends_with('A') == true);
    STATIC_REQUIRE(testString.ends_with("B") == false);
    STATIC_REQUIRE(testString.ends_with('B') == false);
    STATIC_REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Longer suffix ends_with") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.ends_with("Hello World") == false);
    REQUIRE(testString.ends_with("Hello Universe") == false);
    REQUIRE(testString.ends_with("Hello") == true);
    REQUIRE(testString.ends_with("llo") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("Hello World") == false);
    STATIC_REQUIRE(testString.ends_with("Hello Universe") == false);
    STATIC_REQUIRE(testString.ends_with("Hello") == true);
    STATIC_REQUIRE(testString.ends_with("llo") == true);
  }

  SECTION("Case sensitivity ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with("world") == false);
    REQUIRE(testString.ends_with("WORLD") == false);
    REQUIRE(testString.ends_with("World") == true);
    REQUIRE(testString.ends_with('d') == true);
    REQUIRE(testString.ends_with('D') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("world") == false);
    STATIC_REQUIRE(testString.ends_with("WORLD") == false);
    STATIC_REQUIRE(testString.ends_with("World") == true);
    STATIC_REQUIRE(testString.ends_with('d') == true);
    STATIC_REQUIRE(testString.ends_with('D') == false);
  }

  SECTION("Different FixedString capacities ends_with") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.ends_with(FixedString<8>("World")) == true);
    REQUIRE(testString.ends_with(FixedString<16>("World")) == true);
    REQUIRE(testString.ends_with(FixedString<64>("World")) == true);
    REQUIRE(testString.ends_with(FixedString<8>("Hello")) == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(FixedString<8>("World")) == true);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("World")) == true);
    STATIC_REQUIRE(testString.ends_with(FixedString<64>("World")) == true);
    STATIC_REQUIRE(testString.ends_with(FixedString<8>("Hello")) == false);
  }

  SECTION("Repeated characters ends_with") {
    constexpr FixedString<32> testString("baaaa");

    REQUIRE(testString.ends_with("aaa") == true);
    REQUIRE(testString.ends_with("aaaa") == true);
    REQUIRE(testString.ends_with("baaaa") == true);
    REQUIRE(testString.ends_with("aaaaa") == false);
    REQUIRE(testString.ends_with('a') == true);
    REQUIRE(testString.ends_with('b') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("aaa") == true);
    STATIC_REQUIRE(testString.ends_with("aaaa") == true);
    STATIC_REQUIRE(testString.ends_with("baaaa") == true);
    STATIC_REQUIRE(testString.ends_with("aaaaa") == false);
    STATIC_REQUIRE(testString.ends_with('a') == true);
    STATIC_REQUIRE(testString.ends_with('b') == false);
  }

  SECTION("Special characters ends_with") {
    constexpr FixedString<32> testString("%$#@!");

    REQUIRE(testString.ends_with("@!") == true);
    REQUIRE(testString.ends_with("%$#@!") == true);
    REQUIRE(testString.ends_with("^%$#@!") == false);
    REQUIRE(testString.ends_with('!') == true);
    REQUIRE(testString.ends_with('@') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("@!") == true);
    STATIC_REQUIRE(testString.ends_with("%$#@!") == true);
    STATIC_REQUIRE(testString.ends_with("^%$#@!") == false);
    STATIC_REQUIRE(testString.ends_with('!') == true);
    STATIC_REQUIRE(testString.ends_with('@') == false);
  }

  SECTION("Numeric content ends_with") {
    constexpr FixedString<32> testString("54321");

    REQUIRE(testString.ends_with("321") == true);
    REQUIRE(testString.ends_with("54321") == true);
    REQUIRE(testString.ends_with("654321") == false);
    REQUIRE(testString.ends_with('1') == true);
    REQUIRE(testString.ends_with('2') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("321") == true);
    STATIC_REQUIRE(testString.ends_with("54321") == true);
    STATIC_REQUIRE(testString.ends_with("654321") == false);
    STATIC_REQUIRE(testString.ends_with('1') == true);
    STATIC_REQUIRE(testString.ends_with('2') == false);
  }

  SECTION("Mixed content ends_with") {
    constexpr FixedString<32> testString("123Hello");

    REQUIRE(testString.ends_with("Hello") == true);
    REQUIRE(testString.ends_with("3Hello") == true);
    REQUIRE(testString.ends_with("123Hello") == true);
    REQUIRE(testString.ends_with("0123Hello") == false);
    REQUIRE(testString.ends_with('o') == true);
    REQUIRE(testString.ends_with('1') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("Hello") == true);
    STATIC_REQUIRE(testString.ends_with("3Hello") == true);
    STATIC_REQUIRE(testString.ends_with("123Hello") == true);
    STATIC_REQUIRE(testString.ends_with("0123Hello") == false);
    STATIC_REQUIRE(testString.ends_with('o') == true);
    STATIC_REQUIRE(testString.ends_with('1') == false);
  }

  SECTION("Maximum length strings ends_with") {
    constexpr FixedString<16> testString("123456789012345");

    REQUIRE(testString.ends_with("123456789012345") == true);
    REQUIRE(testString.ends_with("23456789012345") == true);
    REQUIRE(testString.ends_with("0123456789012345") == false);
    REQUIRE(testString.ends_with('5') == true);
    REQUIRE(testString.ends_with('1') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("123456789012345") == true);
    STATIC_REQUIRE(testString.ends_with("23456789012345") == true);
    STATIC_REQUIRE(testString.ends_with("0123456789012345") == false);
    STATIC_REQUIRE(testString.ends_with('5') == true);
    STATIC_REQUIRE(testString.ends_with('1') == false);
  }

  SECTION("Array ends_with") {
    constexpr FixedString<32> testString("Hello");
    constexpr std::array<char, 4> arr = {'l', 'l', 'o', '\0'};

    REQUIRE(testString.ends_with(arr.data()) == true);
    REQUIRE(testString.ends_with("llo") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(arr.data()) == true);
    STATIC_REQUIRE(testString.ends_with("llo") == true);
  }

  SECTION("Edge cases ends_with") {
    constexpr FixedString<32> testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.ends_with("Hello\0World") == true);

    // Test with string containing null character
    constexpr FixedString<32> testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.ends_with("World") == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("Hello\0World") == true);
    STATIC_REQUIRE(testStringWithNull.ends_with("World") == false);
  }

  SECTION("Whitespace ends_with") {
    constexpr FixedString<32> testString("Hello World ");

    REQUIRE(testString.ends_with(" ") == true);
    REQUIRE(testString.ends_with("World ") == true);
    REQUIRE(testString.ends_with("World") == false);
    REQUIRE(testString.ends_with(' ') == true);
    REQUIRE(testString.ends_with('d') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(" ") == true);
    STATIC_REQUIRE(testString.ends_with("World ") == true);
    STATIC_REQUIRE(testString.ends_with("World") == false);
    STATIC_REQUIRE(testString.ends_with(' ') == true);
    STATIC_REQUIRE(testString.ends_with('d') == false);
  }

  SECTION("Exact match ends_with") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.ends_with("Hello") == true);
    REQUIRE(testString.ends_with("llo") == true);
    REQUIRE(testString.ends_with("o") == true);
    REQUIRE(testString.ends_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("Hello") == true);
    STATIC_REQUIRE(testString.ends_with("llo") == true);
    STATIC_REQUIRE(testString.ends_with("o") == true);
    STATIC_REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Overlapping patterns ends_with") {
    constexpr FixedString<32> testString("ababab");

    REQUIRE(testString.ends_with("ab") == true);
    REQUIRE(testString.ends_with("bab") == true);
    REQUIRE(testString.ends_with("abab") == true);
    REQUIRE(testString.ends_with("babab") == true);
    REQUIRE(testString.ends_with("ababab") == true);
    REQUIRE(testString.ends_with("bababab") == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("ab") == true);
    STATIC_REQUIRE(testString.ends_with("bab") == true);
    STATIC_REQUIRE(testString.ends_with("abab") == true);
    STATIC_REQUIRE(testString.ends_with("babab") == true);
    STATIC_REQUIRE(testString.ends_with("ababab") == true);
    STATIC_REQUIRE(testString.ends_with("bababab") == false);
  }

  SECTION("Multiple occurrences ends_with") {
    constexpr FixedString<32> testString("abababab");

    REQUIRE(testString.ends_with("ab") == true);
    REQUIRE(testString.ends_with("bab") == true);
    REQUIRE(testString.ends_with("abab") == true);
    REQUIRE(testString.ends_with("ababab") == true);
    REQUIRE(testString.ends_with("abababab") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("ab") == true);
    STATIC_REQUIRE(testString.ends_with("bab") == true);
    STATIC_REQUIRE(testString.ends_with("abab") == true);
    STATIC_REQUIRE(testString.ends_with("ababab") == true);
    STATIC_REQUIRE(testString.ends_with("abababab") == true);
  }

  SECTION("Unicode content ends_with") {
    constexpr FixedString<32> testString("Hello 世界");

    REQUIRE(testString.ends_with("世界") == true);
    REQUIRE(testString.ends_with("Hello 世界") == true);
    REQUIRE(testString.ends_with("界") == true);
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with("xyz") == false);
    REQUIRE(testString.ends_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("世界") == true);
    STATIC_REQUIRE(testString.ends_with("Hello 世界") == true);
    STATIC_REQUIRE(testString.ends_with("界") == true);
    STATIC_REQUIRE(testString.ends_with("Hello") == false);
    STATIC_REQUIRE(testString.ends_with("xyz") == false);
    STATIC_REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Long strings ends_with") {
    constexpr FixedString<64> testString("This is a very long string for performance testing");

    REQUIRE(testString.ends_with("testing") == true);
    REQUIRE(testString.ends_with("performance testing") == true);
    REQUIRE(testString.ends_with("This is a very long string for performance testing") == true);
    REQUIRE(testString.ends_with("g") == true);
    REQUIRE(testString.ends_with("T") == false);
    REQUIRE(testString.ends_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("testing") == true);
    STATIC_REQUIRE(testString.ends_with("performance testing") == true);
    STATIC_REQUIRE(testString.ends_with("This is a very long string for performance testing") == true);
    STATIC_REQUIRE(testString.ends_with("g") == true);
    STATIC_REQUIRE(testString.ends_with("T") == false);
    STATIC_REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Constexpr operations ends_with") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<16> str2("World");
    constexpr FixedString<16> str3("Test");

    // Compile-time ends_with operations
    constexpr bool ends1 = str1.ends_with("llo");
    constexpr bool ends2 = str2.ends_with("rld");
    constexpr bool ends3 = str3.ends_with("est");

    STATIC_REQUIRE(ends1 == true);
    STATIC_REQUIRE(ends2 == true);
    STATIC_REQUIRE(ends3 == true);

    // Compile-time character ends_with operations
    constexpr bool charEnds1 = str1.ends_with('o');
    constexpr bool charEnds2 = str2.ends_with('d');
    constexpr bool charEnds3 = str3.ends_with('t');

    STATIC_REQUIRE(charEnds1 == true);
    STATIC_REQUIRE(charEnds2 == true);
    STATIC_REQUIRE(charEnds3 == true);
  }
}

TEST_CASE("FixedString contains", "[core][fixed_string]") {
  SECTION("C string contains") {
    constexpr FixedString<32> str("Hello World");

    // Basic contains
    REQUIRE(str.contains("World") == true);
    REQUIRE(str.contains("Hello") == true);
    REQUIRE(str.contains("lo Wo") == true);
    REQUIRE(str.contains("Hello World") == true);
    REQUIRE(str.contains("xyz") == false);
    REQUIRE(str.contains("") == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("World") == true);
    STATIC_REQUIRE(str.contains("Hello") == true);
    STATIC_REQUIRE(str.contains("lo Wo") == true);
    STATIC_REQUIRE(str.contains("Hello World") == true);
    STATIC_REQUIRE(str.contains("xyz") == false);
    STATIC_REQUIRE(str.contains("") == true);
  }

  SECTION("FixedString contains") {
    constexpr FixedString<16> str("Hello World");

    // Basic contains with FixedString
    REQUIRE(str.contains(FixedString<16>("World")) == true);
    REQUIRE(str.contains(FixedString<16>("Hello")) == true);
    REQUIRE(str.contains(FixedString<16>("lo Wo")) == true);
    REQUIRE(str.contains(FixedString<16>("Hello World")) == true);
    REQUIRE(str.contains(FixedString<16>("xyz")) == false);
    REQUIRE(str.contains(FixedString<16>("")) == true);

    // Different capacities
    REQUIRE(str.contains(FixedString<8>("World")) == true);
    REQUIRE(str.contains(FixedString<64>("World")) == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains(FixedString<16>("World")) == true);
    STATIC_REQUIRE(str.contains(FixedString<16>("Hello")) == true);
    STATIC_REQUIRE(str.contains(FixedString<16>("lo Wo")) == true);
    STATIC_REQUIRE(str.contains(FixedString<16>("Hello World")) == true);
    STATIC_REQUIRE(str.contains(FixedString<16>("xyz")) == false);
    STATIC_REQUIRE(str.contains(FixedString<16>("")) == true);

    STATIC_REQUIRE(str.contains(FixedString<8>("World")) == true);
    STATIC_REQUIRE(str.contains(FixedString<64>("World")) == true);
  }

  SECTION("StringLike contains") {
    constexpr FixedString<32> str("Hello World");

    REQUIRE(str.contains(std::string("World")) == true);
    REQUIRE(str.contains(std::string("Hello")) == true);
    REQUIRE(str.contains(std::string("lo Wo")) == true);
    REQUIRE(str.contains(std::string("Hello World")) == true);
    REQUIRE(str.contains(std::string("xyz")) == false);
    REQUIRE(str.contains(std::string("")) == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains(CStringView("World")) == true);
    STATIC_REQUIRE(str.contains(CStringView("Hello")) == true);
    STATIC_REQUIRE(str.contains(CStringView("lo Wo")) == true);
    STATIC_REQUIRE(str.contains(CStringView("Hello World")) == true);
    STATIC_REQUIRE(str.contains(CStringView("xyz")) == false);
    STATIC_REQUIRE(str.contains(CStringView("")) == true);
  }

  SECTION("Character contains") {
    constexpr FixedString<32> str("Hello World");

    // Character contains
    REQUIRE(str.contains('H') == true);
    REQUIRE(str.contains('o') == true);
    REQUIRE(str.contains('l') == true);
    REQUIRE(str.contains(' ') == true);
    REQUIRE(str.contains('x') == false);
    REQUIRE(str.contains('Z') == false);

    // Compile-time checks
    STATIC_REQUIRE(str.contains('H') == true);
    STATIC_REQUIRE(str.contains('o') == true);
    STATIC_REQUIRE(str.contains('l') == true);
    STATIC_REQUIRE(str.contains(' ') == true);
    STATIC_REQUIRE(str.contains('x') == false);
    STATIC_REQUIRE(str.contains('Z') == false);
  }

  SECTION("Empty string contains") {
    constexpr FixedString<32> str("");

    // Empty string contains
    REQUIRE(str.contains(FixedString<16>("Hello")) == false);
    REQUIRE(str.contains(std::string("Hello")) == false);
    REQUIRE(str.contains("Hello") == false);
    REQUIRE(str.contains('H') == false);
    REQUIRE(str.contains("") == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains(FixedString<16>("Hello")) == false);
    STATIC_REQUIRE(str.contains(CStringView("Hello")) == false);
    STATIC_REQUIRE(str.contains("Hello") == false);
    STATIC_REQUIRE(str.contains('H') == false);
    STATIC_REQUIRE(str.contains("") == true);
  }

  SECTION("Single character string contains") {
    constexpr FixedString<8> str("A");

    // Single character contains
    REQUIRE(str.contains("A") == true);
    REQUIRE(str.contains('A') == true);
    REQUIRE(str.contains("B") == false);
    REQUIRE(str.contains('B') == false);
    REQUIRE(str.contains("") == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("A") == true);
    STATIC_REQUIRE(str.contains('A') == true);
    STATIC_REQUIRE(str.contains("B") == false);
    STATIC_REQUIRE(str.contains('B') == false);
    STATIC_REQUIRE(str.contains("") == true);
  }

  SECTION("Case sensitivity") {
    constexpr FixedString<32> str("Hello World");

    // Case sensitive contains
    REQUIRE(str.contains("world") == false);
    REQUIRE(str.contains("WORLD") == false);
    REQUIRE(str.contains("World") == true);
    REQUIRE(str.contains('h') == false);
    REQUIRE(str.contains('H') == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("world") == false);
    STATIC_REQUIRE(str.contains("WORLD") == false);
    STATIC_REQUIRE(str.contains("World") == true);
    STATIC_REQUIRE(str.contains('h') == false);
    STATIC_REQUIRE(str.contains('H') == true);
  }

  SECTION("Edge cases") {
    constexpr FixedString<16> str("Hello");

    // Longer substring than string
    REQUIRE(str.contains("Hello World") == false);
    REQUIRE(str.contains("Hello Universe") == false);

    // Exact match
    REQUIRE(str.contains("Hello") == true);

    // Substrings
    REQUIRE(str.contains("llo") == true);
    REQUIRE(str.contains("ell") == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("Hello World") == false);
    STATIC_REQUIRE(str.contains("Hello Universe") == false);
    STATIC_REQUIRE(str.contains("Hello") == true);
    STATIC_REQUIRE(str.contains("llo") == true);
    STATIC_REQUIRE(str.contains("ell") == true);
  }

  SECTION("Special characters") {
    constexpr FixedString<32> str("Hello\n\tWorld!");

    // Special characters
    REQUIRE(str.contains("\n") == true);
    REQUIRE(str.contains("\t") == true);
    REQUIRE(str.contains("!") == true);
    REQUIRE(str.contains("Hello\n") == true);
    REQUIRE(str.contains("\tWorld") == true);
    REQUIRE(str.contains("World!") == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("\n") == true);
    STATIC_REQUIRE(str.contains("\t") == true);
    STATIC_REQUIRE(str.contains("!") == true);
    STATIC_REQUIRE(str.contains("Hello\n") == true);
    STATIC_REQUIRE(str.contains("\tWorld") == true);
    STATIC_REQUIRE(str.contains("World!") == true);
  }

  SECTION("Unicode content") {
    constexpr FixedString<32> str("Hello 世界");

    // Unicode contains
    REQUIRE(str.contains("世界") == true);
    REQUIRE(str.contains("Hello 世") == true);
    REQUIRE(str.contains("界") == true);
    REQUIRE(str.contains("世") == true);
    REQUIRE(str.contains("宇宙") == false);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("世界") == true);
    STATIC_REQUIRE(str.contains("Hello 世") == true);
    STATIC_REQUIRE(str.contains("界") == true);
    STATIC_REQUIRE(str.contains("世") == true);
    STATIC_REQUIRE(str.contains("宇宙") == false);
  }

  SECTION("Repeated patterns") {
    constexpr FixedString<32> str("ababab");

    // Overlapping patterns
    REQUIRE(str.contains("ab") == true);
    REQUIRE(str.contains("bab") == true);
    REQUIRE(str.contains("abab") == true);
    REQUIRE(str.contains("ababab") == true);
    REQUIRE(str.contains("babab") == true);
    REQUIRE(str.contains("abababa") == false);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("ab") == true);
    STATIC_REQUIRE(str.contains("bab") == true);
    STATIC_REQUIRE(str.contains("abab") == true);
    STATIC_REQUIRE(str.contains("ababab") == true);
    STATIC_REQUIRE(str.contains("babab") == true);
    STATIC_REQUIRE(str.contains("abababa") == false);
  }

  SECTION("Numeric content") {
    constexpr FixedString<32> str("12345");

    // Numeric contains
    REQUIRE(str.contains("123") == true);
    REQUIRE(str.contains("345") == true);
    REQUIRE(str.contains("234") == true);
    REQUIRE(str.contains("12345") == true);
    REQUIRE(str.contains("678") == false);
    REQUIRE(str.contains('1') == true);
    REQUIRE(str.contains('5') == true);
    REQUIRE(str.contains('9') == false);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("123") == true);
    STATIC_REQUIRE(str.contains("345") == true);
    STATIC_REQUIRE(str.contains("234") == true);
    STATIC_REQUIRE(str.contains("12345") == true);
    STATIC_REQUIRE(str.contains("678") == false);
    STATIC_REQUIRE(str.contains('1') == true);
    STATIC_REQUIRE(str.contains('5') == true);
    STATIC_REQUIRE(str.contains('9') == false);
  }

  SECTION("Mixed content") {
    constexpr FixedString<32> str("123Hello456");

    // Mixed alphanumeric contains
    REQUIRE(str.contains("123") == true);
    REQUIRE(str.contains("Hello") == true);
    REQUIRE(str.contains("456") == true);
    REQUIRE(str.contains("3Hello4") == true);
    REQUIRE(str.contains("123Hello456") == true);
    REQUIRE(str.contains("789") == false);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("123") == true);
    STATIC_REQUIRE(str.contains("Hello") == true);
    STATIC_REQUIRE(str.contains("456") == true);
    STATIC_REQUIRE(str.contains("3Hello4") == true);
    STATIC_REQUIRE(str.contains("123Hello456") == true);
    STATIC_REQUIRE(str.contains("789") == false);
  }

  SECTION("Position-specific contains") {
    constexpr FixedString<32> str("Hello World");

    // Beginning
    REQUIRE(str.contains("H") == true);
    REQUIRE(str.contains("He") == true);
    REQUIRE(str.contains("Hello") == true);

    // Middle
    REQUIRE(str.contains("l") == true);
    REQUIRE(str.contains("ll") == true);
    REQUIRE(str.contains("lo W") == true);

    // End
    REQUIRE(str.contains("d") == true);
    REQUIRE(str.contains("ld") == true);
    REQUIRE(str.contains("World") == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains("H") == true);
    STATIC_REQUIRE(str.contains("He") == true);
    STATIC_REQUIRE(str.contains("Hello") == true);
    STATIC_REQUIRE(str.contains("l") == true);
    STATIC_REQUIRE(str.contains("ll") == true);
    STATIC_REQUIRE(str.contains("lo W") == true);
    STATIC_REQUIRE(str.contains("d") == true);
    STATIC_REQUIRE(str.contains("ld") == true);
    STATIC_REQUIRE(str.contains("World") == true);
  }
}

TEST_CASE("FixedString substr", "[core][fixed_string]") {
  SECTION("Substr basic functionality") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0).size() == 11);
    STATIC_REQUIRE(cstrcmp(testString.substr(0).c_str(), "Hello World") == 0);
    STATIC_REQUIRE(testString.substr(0, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    STATIC_REQUIRE(testString.substr(6).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(6).c_str(), "World") == 0);
    STATIC_REQUIRE(testString.substr(6, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(6, 5).c_str(), "World") == 0);
    STATIC_REQUIRE(testString.substr(6, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    STATIC_REQUIRE(testString.substr(0, 0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 0).c_str(), "") == 0);
  }

  SECTION("Substr with default parameters") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr().size() == 11);
    REQUIRE(std::strcmp(testString.substr().c_str(), "Hello World") == 0);
    REQUIRE(testString.substr(0).size() == 11);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "Hello World") == 0);
    REQUIRE(testString.substr(6).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr().size() == 11);
    STATIC_REQUIRE(cstrcmp(testString.substr().c_str(), "Hello World") == 0);
    STATIC_REQUIRE(testString.substr(0).size() == 11);
    STATIC_REQUIRE(cstrcmp(testString.substr(0).c_str(), "Hello World") == 0);
    STATIC_REQUIRE(testString.substr(6).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SECTION("Substr with count parameter") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 1).c_str(), "H") == 0);
    STATIC_REQUIRE(testString.substr(0, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 2).c_str(), "He") == 0);
    STATIC_REQUIRE(testString.substr(0, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 3).c_str(), "Hel") == 0);
    STATIC_REQUIRE(testString.substr(0, 4).size() == 4);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 4).c_str(), "Hell") == 0);
    STATIC_REQUIRE(testString.substr(0, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    STATIC_REQUIRE(testString.substr(6, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(6, 1).c_str(), "W") == 0);
    STATIC_REQUIRE(testString.substr(6, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(6, 2).c_str(), "Wo") == 0);
    STATIC_REQUIRE(testString.substr(6, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    STATIC_REQUIRE(testString.substr(6, 4).size() == 4);
    STATIC_REQUIRE(cstrcmp(testString.substr(6, 4).c_str(), "Worl") == 0);
    STATIC_REQUIRE(testString.substr(6, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(6, 5).c_str(), "World") == 0);
  }

  SECTION("Substr with npos count") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(0, FixedString<32>::npos).size() == 11);
    REQUIRE(std::strcmp(testString.substr(0, FixedString<32>::npos).c_str(), "Hello World") == 0);
    REQUIRE(testString.substr(6, FixedString<32>::npos).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6, FixedString<32>::npos).c_str(), "World") == 0);
    REQUIRE(testString.substr(10, FixedString<32>::npos).size() == 1);
    REQUIRE(std::strcmp(testString.substr(10, FixedString<32>::npos).c_str(), "d") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0, FixedString<32>::npos).size() == 11);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, FixedString<32>::npos).c_str(), "Hello World") == 0);
    STATIC_REQUIRE(testString.substr(6, FixedString<32>::npos).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(6, FixedString<32>::npos).c_str(), "World") == 0);
    STATIC_REQUIRE(testString.substr(10, FixedString<32>::npos).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(10, FixedString<32>::npos).c_str(), "d") == 0);
  }

  SECTION("Substr with position at end") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(11).size() == 0);
    REQUIRE(std::strcmp(testString.substr(11).c_str(), "") == 0);
    REQUIRE(testString.substr(11, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(11, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(11, 5).size() == 0);
    REQUIRE(std::strcmp(testString.substr(11, 5).c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(11).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(11).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(11, 0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(11, 0).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(11, 5).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(11, 5).c_str(), "") == 0);
  }

  SECTION("Substr from empty string") {
    constexpr FixedString<32> testString("");

    REQUIRE(testString.substr(0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "") == 0);
    REQUIRE(testString.substr(0, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(0, 5).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(0).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(0, 0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 0).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(0, 5).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 5).c_str(), "") == 0);
  }

  SECTION("Substr single character") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(0).c_str(), "A") == 0);
    STATIC_REQUIRE(testString.substr(0, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 1).c_str(), "A") == 0);
    STATIC_REQUIRE(testString.substr(0, 0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 0).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(1).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(1).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(1, 1).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(1, 1).c_str(), "") == 0);
  }

  SECTION("Substr with repeated characters") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(0).c_str(), "aaaaa") == 0);
    STATIC_REQUIRE(testString.substr(0, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 1).c_str(), "a") == 0);
    STATIC_REQUIRE(testString.substr(0, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 2).c_str(), "aa") == 0);
    STATIC_REQUIRE(testString.substr(0, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 3).c_str(), "aaa") == 0);
    STATIC_REQUIRE(testString.substr(0, 4).size() == 4);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 4).c_str(), "aaaa") == 0);
    STATIC_REQUIRE(testString.substr(0, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 5).c_str(), "aaaaa") == 0);
    STATIC_REQUIRE(testString.substr(1).size() == 4);
    STATIC_REQUIRE(cstrcmp(testString.substr(1).c_str(), "aaaa") == 0);
    STATIC_REQUIRE(testString.substr(2).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(2).c_str(), "aaa") == 0);
    STATIC_REQUIRE(testString.substr(3).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(3).c_str(), "aa") == 0);
    STATIC_REQUIRE(testString.substr(4).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(4).c_str(), "a") == 0);
    STATIC_REQUIRE(testString.substr(5).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(5).c_str(), "") == 0);
  }

  SECTION("Substr with special characters") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(5).size() == 8);
    STATIC_REQUIRE(cstrcmp(testString.substr(5).c_str(), ", World!") == 0);
    STATIC_REQUIRE(testString.substr(5, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 1).c_str(), ",") == 0);
    STATIC_REQUIRE(testString.substr(5, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 2).c_str(), ", ") == 0);
    STATIC_REQUIRE(testString.substr(12).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(12).c_str(), "!") == 0);
    STATIC_REQUIRE(testString.substr(12, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(12, 1).c_str(), "!") == 0);
  }

  SECTION("Substr with numbers") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(0).c_str(), "12345") == 0);
    STATIC_REQUIRE(testString.substr(0, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 1).c_str(), "1") == 0);
    STATIC_REQUIRE(testString.substr(0, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 2).c_str(), "12") == 0);
    STATIC_REQUIRE(testString.substr(0, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 3).c_str(), "123") == 0);
    STATIC_REQUIRE(testString.substr(0, 4).size() == 4);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 4).c_str(), "1234") == 0);
    STATIC_REQUIRE(testString.substr(0, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 5).c_str(), "12345") == 0);
    STATIC_REQUIRE(testString.substr(1).size() == 4);
    STATIC_REQUIRE(cstrcmp(testString.substr(1).c_str(), "2345") == 0);
    STATIC_REQUIRE(testString.substr(2).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(2).c_str(), "345") == 0);
    STATIC_REQUIRE(testString.substr(3).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(3).c_str(), "45") == 0);
    STATIC_REQUIRE(testString.substr(4).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(4).c_str(), "5") == 0);
    STATIC_REQUIRE(testString.substr(5).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(5).c_str(), "") == 0);
  }

  SECTION("Substr with whitespace") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(5).size() == 6);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), " World") == 0);
    REQUIRE(testString.substr(5, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), " ") == 0);
    REQUIRE(testString.substr(5, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), " W") == 0);
    REQUIRE(testString.substr(5, 3).size() == 3);
    REQUIRE(std::strcmp(testString.substr(5, 3).c_str(), " Wo") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(5).size() == 6);
    STATIC_REQUIRE(cstrcmp(testString.substr(5).c_str(), " World") == 0);
    STATIC_REQUIRE(testString.substr(5, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 1).c_str(), " ") == 0);
    STATIC_REQUIRE(testString.substr(5, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 2).c_str(), " W") == 0);
    STATIC_REQUIRE(testString.substr(5, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 3).c_str(), " Wo") == 0);
  }

  SECTION("Substr with newlines") {
    constexpr FixedString<32> testString("Hello\nWorld");

    REQUIRE(testString.substr(5).size() == 6);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "\nWorld") == 0);
    REQUIRE(testString.substr(5, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "\n") == 0);
    REQUIRE(testString.substr(5, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), "\nW") == 0);
    REQUIRE(testString.substr(6).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(5).size() == 6);
    STATIC_REQUIRE(cstrcmp(testString.substr(5).c_str(), "\nWorld") == 0);
    STATIC_REQUIRE(testString.substr(5, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 1).c_str(), "\n") == 0);
    STATIC_REQUIRE(testString.substr(5, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 2).c_str(), "\nW") == 0);
    STATIC_REQUIRE(testString.substr(6).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SECTION("Substr with tabs") {
    constexpr FixedString<32> testString("Hello\tWorld");

    REQUIRE(testString.substr(5).size() == 6);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "\tWorld") == 0);
    REQUIRE(testString.substr(5, 1).size() == 1);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "\t") == 0);
    REQUIRE(testString.substr(5, 2).size() == 2);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), "\tW") == 0);
    REQUIRE(testString.substr(6).size() == 5);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(5).size() == 6);
    STATIC_REQUIRE(cstrcmp(testString.substr(5).c_str(), "\tWorld") == 0);
    STATIC_REQUIRE(testString.substr(5, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 1).c_str(), "\t") == 0);
    STATIC_REQUIRE(testString.substr(5, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 2).c_str(), "\tW") == 0);
    STATIC_REQUIRE(testString.substr(6).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SECTION("Substr maximum length") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0).size() == 15);
    STATIC_REQUIRE(cstrcmp(testString.substr(0).c_str(), "123456789012345") == 0);
    STATIC_REQUIRE(testString.substr(0, 15).size() == 15);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 15).c_str(), "123456789012345") == 0);
    STATIC_REQUIRE(testString.substr(0, 16).size() == 15);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 16).c_str(), "123456789012345") == 0);
    STATIC_REQUIRE(testString.substr(14).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(14).c_str(), "5") == 0);
    STATIC_REQUIRE(testString.substr(14, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(14, 1).c_str(), "5") == 0);
    STATIC_REQUIRE(testString.substr(15).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(15).c_str(), "") == 0);
  }

  SECTION("Substr with mixed content") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0).size() == 8);
    STATIC_REQUIRE(cstrcmp(testString.substr(0).c_str(), "123Hello") == 0);
    STATIC_REQUIRE(testString.substr(0, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 3).c_str(), "123") == 0);
    STATIC_REQUIRE(testString.substr(3).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(3).c_str(), "Hello") == 0);
    STATIC_REQUIRE(testString.substr(3, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(3, 5).c_str(), "Hello") == 0);
    STATIC_REQUIRE(testString.substr(2, 4).size() == 4);
    STATIC_REQUIRE(cstrcmp(testString.substr(2, 4).c_str(), "3Hel") == 0);
  }

  SECTION("Substr with overlapping ranges") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 1).c_str(), "a") == 0);
    STATIC_REQUIRE(testString.substr(1, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(1, 1).c_str(), "b") == 0);
    STATIC_REQUIRE(testString.substr(2, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(2, 1).c_str(), "c") == 0);
    STATIC_REQUIRE(testString.substr(3, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(3, 1).c_str(), "d") == 0);
    STATIC_REQUIRE(testString.substr(4, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(4, 1).c_str(), "e") == 0);
    STATIC_REQUIRE(testString.substr(5, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 1).c_str(), "f") == 0);
    STATIC_REQUIRE(testString.substr(0, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 2).c_str(), "ab") == 0);
    STATIC_REQUIRE(testString.substr(1, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(1, 2).c_str(), "bc") == 0);
    STATIC_REQUIRE(testString.substr(2, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(2, 2).c_str(), "cd") == 0);
    STATIC_REQUIRE(testString.substr(3, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(3, 2).c_str(), "de") == 0);
    STATIC_REQUIRE(testString.substr(4, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(4, 2).c_str(), "ef") == 0);
  }

  SECTION("Substr with exact string length") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    STATIC_REQUIRE(testString.substr(0, 6).size() == 5);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 6).c_str(), "Hello") == 0);
    STATIC_REQUIRE(testString.substr(1, 4).size() == 4);
    STATIC_REQUIRE(cstrcmp(testString.substr(1, 4).c_str(), "ello") == 0);
    STATIC_REQUIRE(testString.substr(2, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(2, 3).c_str(), "llo") == 0);
    STATIC_REQUIRE(testString.substr(3, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(3, 2).c_str(), "lo") == 0);
    STATIC_REQUIRE(testString.substr(4, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(4, 1).c_str(), "o") == 0);
  }

  SECTION("Substr with zero count") {
    constexpr FixedString<32> testString("Hello World");

    REQUIRE(testString.substr(0, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(5, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(10, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(10, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(11, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(11, 0).c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0, 0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 0).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(5, 0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 0).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(10, 0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(10, 0).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(11, 0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(11, 0).c_str(), "") == 0);
  }

  SECTION("Substr with position at string size") {
    constexpr FixedString<32> testString("Hello");

    REQUIRE(testString.substr(5).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "") == 0);
    REQUIRE(testString.substr(5, 0).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5, 0).c_str(), "") == 0);
    REQUIRE(testString.substr(5, 1).size() == 0);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(5).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(5).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(5, 0).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 0).c_str(), "") == 0);
    STATIC_REQUIRE(testString.substr(5, 1).size() == 0);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 1).c_str(), "") == 0);
  }

  SECTION("Substr with different FixedString capacities") {
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

    // Compile-time checks
    STATIC_REQUIRE(smallString.substr(0, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(smallString.substr(0, 3).c_str(), "Hel") == 0);
    STATIC_REQUIRE(mediumString.substr(0, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(mediumString.substr(0, 5).c_str(), "Hello") == 0);
    STATIC_REQUIRE(largeString.substr(0, 5).size() == 5);
    STATIC_REQUIRE(cstrcmp(largeString.substr(0, 5).c_str(), "Hello") == 0);
    STATIC_REQUIRE(smallString.substr(2).size() == 3);
    STATIC_REQUIRE(cstrcmp(smallString.substr(2).c_str(), "llo") == 0);
    STATIC_REQUIRE(mediumString.substr(6).size() == 5);
    STATIC_REQUIRE(cstrcmp(mediumString.substr(6).c_str(), "World") == 0);
    STATIC_REQUIRE(largeString.substr(12).size() == 8);
    STATIC_REQUIRE(cstrcmp(largeString.substr(12).c_str(), "Universe") == 0);
  }

  SECTION("Substr with punctuation") {
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

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(5, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 1).c_str(), ",") == 0);
    STATIC_REQUIRE(testString.substr(6, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(6, 1).c_str(), " ") == 0);
    STATIC_REQUIRE(testString.substr(12, 1).size() == 1);
    STATIC_REQUIRE(cstrcmp(testString.substr(12, 1).c_str(), "!") == 0);
    STATIC_REQUIRE(testString.substr(5, 3).size() == 3);
    STATIC_REQUIRE(cstrcmp(testString.substr(5, 3).c_str(), ", W") == 0);
    STATIC_REQUIRE(testString.substr(11, 2).size() == 2);
    STATIC_REQUIRE(cstrcmp(testString.substr(11, 2).c_str(), "d!") == 0);
  }

  SECTION("Substr with unicode-like content") {
    constexpr FixedString<32> testString("Hello 世界");

    REQUIRE(testString.substr(0, 6).size() == 6);
    REQUIRE(std::strcmp(testString.substr(0, 6).c_str(), "Hello ") == 0);
    REQUIRE(testString.substr(6).size() == 6);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "世界") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.substr(0, 6).size() == 6);
    STATIC_REQUIRE(cstrcmp(testString.substr(0, 6).c_str(), "Hello ") == 0);
    STATIC_REQUIRE(testString.substr(6).size() == 6);
    STATIC_REQUIRE(cstrcmp(testString.substr(6).c_str(), "世界") == 0);
  }
}

TEST_CASE("FixedString operators+", "[core][fixed_string]") {
  SECTION("Basic concatenation tests") {
    constexpr auto testString1 = FixedString<14>("12") + "test text 1";
    constexpr auto testString2 = FixedString<14>("23") + FixedString<14>("test text 2");
    constexpr auto testString3 = FixedString<20>("34") + FixedString<14>("test text 3");
    constexpr auto testString4 = FixedString<20>("45") + FixedString<26>("test text 4");
    constexpr auto testString5 = FixedString<8>("a") + "b";
    constexpr auto testString6 = testString5 + testString5;

    REQUIRE(testString1.size() == 13);
    REQUIRE(std::strcmp(testString1.c_str(), "12test text 1") == 0);
    STATIC_REQUIRE(testString1.size() == 13);
    STATIC_REQUIRE(cstrcmp(testString1.c_str(), "12test text 1") == 0);

    REQUIRE(testString2.size() == 13);
    REQUIRE(std::strcmp(testString2.c_str(), "23test text 2") == 0);
    STATIC_REQUIRE(testString2.size() == 13);
    STATIC_REQUIRE(cstrcmp(testString2.c_str(), "23test text 2") == 0);

    REQUIRE(testString3.size() == 13);
    REQUIRE(std::strcmp(testString3.c_str(), "34test text 3") == 0);
    STATIC_REQUIRE(testString3.size() == 13);
    STATIC_REQUIRE(cstrcmp(testString3.c_str(), "34test text 3") == 0);

    REQUIRE(testString4.size() == 13);
    REQUIRE(std::strcmp(testString4.c_str(), "45test text 4") == 0);
    STATIC_REQUIRE(testString4.size() == 13);
    STATIC_REQUIRE(cstrcmp(testString4.c_str(), "45test text 4") == 0);

    REQUIRE(testString5.size() == 2);
    REQUIRE(std::strcmp(testString5.c_str(), "ab") == 0);
    STATIC_REQUIRE(testString5.size() == 2);
    STATIC_REQUIRE(cstrcmp(testString5.c_str(), "ab") == 0);

    REQUIRE(testString6.size() == 4);
    REQUIRE(std::strcmp(testString6.c_str(), "abab") == 0);
    STATIC_REQUIRE(testString6.size() == 4);
    STATIC_REQUIRE(cstrcmp(testString6.c_str(), "abab") == 0);
  }

  SECTION("FixedString + FixedString (same size)") {
    constexpr auto result = FixedString<20>("Hello") + FixedString<20>("World");

    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 10);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "HelloWorld") == 0);
  }

  SECTION("FixedString + FixedString (different sizes)") {
    constexpr auto result = FixedString<20>("Hello") + FixedString<10>("World");

    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 10);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "HelloWorld") == 0);
  }

  SECTION("FixedString + C-string") {
    constexpr auto result = FixedString<20>("Hello") + "World";

    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 10);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "HelloWorld") == 0);
  }

  SECTION("C-string + FixedString") {
    constexpr auto result = "Hello" + FixedString<20>("World");

    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 10);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "HelloWorld") == 0);
  }

  SECTION("FixedString + std::string (StringLike)") {
    constexpr auto result = FixedString<20>("Hello") + std::string("World");

    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 10);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "HelloWorld") == 0);
  }

  SECTION("std::string + FixedString (StringLike)") {
    constexpr auto result = std::string("Hello") + FixedString<20>("World");

    REQUIRE(result.size() == 10);
    REQUIRE(std::strcmp(result.c_str(), "HelloWorld") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 10);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "HelloWorld") == 0);
  }

  SECTION("Empty string concatenation") {
    constexpr auto result = FixedString<20>("") + FixedString<20>("");

    REQUIRE(result.size() == 0);
    REQUIRE(std::strcmp(result.c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 0);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "") == 0);
  }

  SECTION("One empty string concatenation") {
    constexpr FixedString<20> str1("Hello");
    constexpr FixedString<20> str2("");
    constexpr auto result1 = str1 + str2;
    constexpr auto result2 = str2 + str1;

    REQUIRE(result1.size() == 5);
    REQUIRE(std::strcmp(result1.c_str(), "Hello") == 0);
    REQUIRE(result2.size() == 5);
    REQUIRE(std::strcmp(result2.c_str(), "Hello") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result1.size() == 5);
    STATIC_REQUIRE(cstrcmp(result1.c_str(), "Hello") == 0);
    STATIC_REQUIRE(result2.size() == 5);
    STATIC_REQUIRE(cstrcmp(result2.c_str(), "Hello") == 0);
  }

  SECTION("Chained concatenation") {
    constexpr auto result = FixedString<20>("A") + FixedString<20>("B") + FixedString<20>("C");

    REQUIRE(result.size() == 3);
    REQUIRE(std::strcmp(result.c_str(), "ABC") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 3);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "ABC") == 0);
  }

  SECTION("Constexpr concatenation") {
    constexpr FixedString<20> str1("Hello");
    constexpr FixedString<20> str2("World");
    constexpr auto result = str1 + str2;

    REQUIRE(result.size() == 10);
    REQUIRE(result == "HelloWorld");

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 10);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "HelloWorld") == 0);
  }

  SECTION("Edge case: maximum capacity") {
    constexpr auto result = FixedString<5>("AB") + FixedString<5>("CD");

    REQUIRE(result.size() == 4);
    REQUIRE(std::strcmp(result.c_str(), "ABCD") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 4);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "ABCD") == 0);
  }

  SECTION("Edge case: single character") {
    constexpr auto result = FixedString<20>("A") + FixedString<20>("B");

    REQUIRE(result.size() == 2);
    REQUIRE(std::strcmp(result.c_str(), "AB") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 2);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "AB") == 0);
  }

  SECTION("FixedString + char") {
    constexpr auto result = FixedString<20>("Hello") + '!';

    REQUIRE(result.size() == 6);
    REQUIRE(std::strcmp(result.c_str(), "Hello!") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 6);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "Hello!") == 0);
  }

  SECTION("char + FixedString") {
    constexpr auto result = '!' + FixedString<20>("Hello");

    REQUIRE(result.size() == 6);
    REQUIRE(std::strcmp(result.c_str(), "!Hello") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 6);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "!Hello") == 0);
  }

  SECTION("FixedString + char (empty string)") {
    constexpr auto result = FixedString<20>("") + 'A';

    REQUIRE(result.size() == 1);
    REQUIRE(std::strcmp(result.c_str(), "A") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 1);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "A") == 0);
  }

  SECTION("char + FixedString (empty string)") {
    constexpr auto result = 'A' + FixedString<20>("");

    REQUIRE(result.size() == 1);
    REQUIRE(std::strcmp(result.c_str(), "A") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 1);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "A") == 0);
  }

  SECTION("FixedString + char (special characters)") {
    constexpr auto result1 = FixedString<20>("Test") + '\n';
    constexpr auto result2 = FixedString<20>("Test") + '\t';
    constexpr auto result3 = FixedString<20>("Test") + ' ';

    REQUIRE(result1.size() == 5);
    REQUIRE(std::strcmp(result1.c_str(), "Test\n") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result1.size() == 5);
    STATIC_REQUIRE(cstrcmp(result1.c_str(), "Test\n") == 0);

    REQUIRE(result2.size() == 5);
    REQUIRE(std::strcmp(result2.c_str(), "Test\t") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result2.size() == 5);
    STATIC_REQUIRE(cstrcmp(result2.c_str(), "Test\t") == 0);

    REQUIRE(result3.size() == 5);
    REQUIRE(std::strcmp(result3.c_str(), "Test ") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result3.size() == 5);
    STATIC_REQUIRE(cstrcmp(result3.c_str(), "Test ") == 0);
  }

  SECTION("char + FixedString (special characters)") {
    constexpr auto result1 = '\n' + FixedString<20>("Test");
    constexpr auto result2 = '\t' + FixedString<20>("Test");
    constexpr auto result3 = ' ' + FixedString<20>("Test");

    REQUIRE(result1.size() == 5);
    REQUIRE(std::strcmp(result1.c_str(), "\nTest") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result1.size() == 5);
    STATIC_REQUIRE(cstrcmp(result1.c_str(), "\nTest") == 0);

    REQUIRE(result2.size() == 5);
    REQUIRE(std::strcmp(result2.c_str(), "\tTest") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result2.size() == 5);
    STATIC_REQUIRE(cstrcmp(result2.c_str(), "\tTest") == 0);

    REQUIRE(result3.size() == 5);
    REQUIRE(std::strcmp(result3.c_str(), " Test") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result3.size() == 5);
    STATIC_REQUIRE(cstrcmp(result3.c_str(), " Test") == 0);
  }

  SECTION("FixedString + char (numeric characters)") {
    constexpr auto result = FixedString<20>("Number") + '1';

    REQUIRE(result.size() == 7);
    REQUIRE(std::strcmp(result.c_str(), "Number1") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 7);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "Number1") == 0);
  }

  SECTION("char + FixedString (numeric characters)") {
    constexpr auto result = '1' + FixedString<20>("Number");

    REQUIRE(result.size() == 7);
    REQUIRE(std::strcmp(result.c_str(), "1Number") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 7);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "1Number") == 0);
  }

  SECTION("FixedString + char (punctuation)") {
    constexpr auto result = FixedString<20>("Hello") + ',';

    REQUIRE(result.size() == 6);
    REQUIRE(std::strcmp(result.c_str(), "Hello,") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 6);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "Hello,") == 0);
  }

  SECTION("char + FixedString (punctuation)") {
    constexpr auto result = ',' + FixedString<20>("Hello");

    REQUIRE(result.size() == 6);
    REQUIRE(std::strcmp(result.c_str(), ",Hello") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 6);
    STATIC_REQUIRE(cstrcmp(result.c_str(), ",Hello") == 0);
  }

  SECTION("FixedString + char (chained operations)") {
    constexpr auto result = FixedString<20>("A") + 'B' + 'C';

    REQUIRE(result.size() == 3);
    REQUIRE(std::strcmp(result.c_str(), "ABC") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 3);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "ABC") == 0);
  }

  SECTION("FixedString + char (maximum capacity)") {
    constexpr auto result = FixedString<6>("ABCD") + 'E';

    REQUIRE(result.size() == 5);
    REQUIRE(std::strcmp(result.c_str(), "ABCDE") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 5);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "ABCDE") == 0);
  }

  SECTION("char + FixedString (maximum capacity)") {
    constexpr auto result = 'A' + FixedString<6>("BCDE");

    REQUIRE(result.size() == 5);
    REQUIRE(std::strcmp(result.c_str(), "ABCDE") == 0);

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 5);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "ABCDE") == 0);
  }

  SECTION("FixedString + char (constexpr operations)") {
    constexpr FixedString<20> str("Hello");
    constexpr char ch = '!';
    constexpr auto result = str + ch;

    REQUIRE(result.size() == 6);
    REQUIRE(result == "Hello!");

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 6);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "Hello!") == 0);
  }

  SECTION("char + FixedString (constexpr operations)") {
    constexpr char ch = '!';
    constexpr FixedString<20> str("Hello");
    constexpr auto result = ch + str;

    REQUIRE(result.size() == 6);
    REQUIRE(result == "!Hello");

    // Compile-time checks
    STATIC_REQUIRE(result.size() == 6);
    STATIC_REQUIRE(cstrcmp(result.c_str(), "!Hello") == 0);
  }
}

TEST_CASE("FixedString operator==", "[core][fixed_string]") {
  SECTION("FixedString == FixedString") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<32> str2("Hello");
    constexpr FixedString<16> str3("World");
    constexpr FixedString<8> str4("Hello");
    constexpr FixedString<16> empty1;
    constexpr FixedString<32> empty2;

    REQUIRE(str1 == str2);
    REQUIRE(str2 == str1);
    REQUIRE(str1 == str4);
    REQUIRE(str4 == str1);
    REQUIRE_FALSE((str1 == str3));
    REQUIRE_FALSE((str3 == str1));
    REQUIRE(empty1 == empty2);
    REQUIRE(empty2 == empty1);
    REQUIRE_FALSE((str1 == empty1));
    REQUIRE_FALSE((empty1 == str1));

    // Compile-time checks
    STATIC_REQUIRE(str1 == str2);
    STATIC_REQUIRE(str2 == str1);
    STATIC_REQUIRE(str1 == str4);
    STATIC_REQUIRE(str4 == str1);
    STATIC_REQUIRE_FALSE((str1 == str3));
    STATIC_REQUIRE_FALSE((str3 == str1));
    STATIC_REQUIRE(empty1 == empty2);
    STATIC_REQUIRE(empty2 == empty1);
    STATIC_REQUIRE_FALSE((str1 == empty1));
    STATIC_REQUIRE_FALSE((empty1 == str1));
  }

  SECTION("FixedString == StringLike") {
    constexpr FixedString<16> str("Hello");
    const std::string stdStr1;
    const std::string stdStr2("Hello");
    const std::string stdStr3("World");
    constexpr CStringView strView1;
    constexpr CStringView strView2("Hello");
    constexpr CStringView strView3("World");

    REQUIRE_FALSE((str == stdStr1));
    REQUIRE_FALSE((stdStr1 == str));
    REQUIRE(str == stdStr2);
    REQUIRE(stdStr2 == str);
    REQUIRE_FALSE((str == stdStr3));
    REQUIRE_FALSE((stdStr3 == str));

    STATIC_REQUIRE_FALSE((str == strView1));
    STATIC_REQUIRE_FALSE((strView1 == str));
    STATIC_REQUIRE(str == strView2);
    STATIC_REQUIRE(strView2 == str);
    STATIC_REQUIRE_FALSE((str == strView3));
    STATIC_REQUIRE_FALSE((strView3 == str));
  }

  SECTION("FixedString == C string") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<16> empty;

    REQUIRE(str1 == "Hello");
    REQUIRE("Hello" == str1);
    REQUIRE_FALSE((str1 == "World"));
    REQUIRE_FALSE(("World" == str1));
    REQUIRE(empty == "");
    REQUIRE("" == empty);
    REQUIRE_FALSE((str1 == ""));
    REQUIRE_FALSE(("" == str1));

    // Compile-time checks
    STATIC_REQUIRE(str1 == "Hello");
    STATIC_REQUIRE("Hello" == str1);
    STATIC_REQUIRE_FALSE((str1 == "World"));
    STATIC_REQUIRE_FALSE(("World" == str1));
    STATIC_REQUIRE(empty == "");
    STATIC_REQUIRE("" == empty);
    STATIC_REQUIRE_FALSE((str1 == ""));
    STATIC_REQUIRE_FALSE(("" == str1));
  }

  SECTION("Edge cases") {
    constexpr FixedString<16> str1("A");
    constexpr FixedString<16> str2("B");
    constexpr FixedString<16> empty1;
    constexpr FixedString<32> empty2;

    // Single character comparison
    REQUIRE(str1 == "A");
    REQUIRE("A" == str1);
    REQUIRE_FALSE((str1 == "B"));
    REQUIRE_FALSE(("B" == str1));

    // Empty string comparisons
    REQUIRE(empty1 == empty2);
    REQUIRE(empty2 == empty1);
    REQUIRE(empty1 == "");
    REQUIRE("" == empty1);

    // Different sizes with same content
    constexpr FixedString<8> small("Hi");
    constexpr FixedString<16> large("Hi");

    REQUIRE(small == large);
    REQUIRE(large == small);

    // Compile-time checks
    STATIC_REQUIRE(str1 == "A");
    STATIC_REQUIRE("A" == str1);
    STATIC_REQUIRE_FALSE((str1 == "B"));
    STATIC_REQUIRE_FALSE(("B" == str1));

    STATIC_REQUIRE(empty1 == empty2);
    STATIC_REQUIRE(empty2 == empty1);
    STATIC_REQUIRE(empty1 == "");
    STATIC_REQUIRE("" == empty1);

    STATIC_REQUIRE(small == large);
    STATIC_REQUIRE(large == small);
  }

  SECTION("Special characters") {
    constexpr FixedString<16> str1("Hello\nWorld");
    constexpr FixedString<16> str2("Hello\tWorld");
    constexpr FixedString<16> str3("Hello World");

    REQUIRE(str1 == "Hello\nWorld");
    REQUIRE("Hello\nWorld" == str1);
    REQUIRE(str2 == "Hello\tWorld");
    REQUIRE("Hello\tWorld" == str2);
    REQUIRE_FALSE((str1 == str2));
    REQUIRE_FALSE((str2 == str1));
    REQUIRE_FALSE((str1 == str3));
    REQUIRE_FALSE((str3 == str1));

    // Compile-time checks
    STATIC_REQUIRE(str1 == "Hello\nWorld");
    STATIC_REQUIRE("Hello\nWorld" == str1);
    STATIC_REQUIRE(str2 == "Hello\tWorld");
    STATIC_REQUIRE("Hello\tWorld" == str2);
    STATIC_REQUIRE_FALSE((str1 == str2));
    STATIC_REQUIRE_FALSE((str2 == str1));
    STATIC_REQUIRE_FALSE((str1 == str3));
    STATIC_REQUIRE_FALSE((str3 == str1));
  }

  SECTION("Unicode content") {
    constexpr FixedString<32> str1("Привет");
    constexpr FixedString<32> str2("Мир");
    constexpr FixedString<32> str3("Привет");

    REQUIRE(str1 == "Привет");
    REQUIRE("Привет" == str1);
    REQUIRE(str1 == str3);
    REQUIRE(str3 == str1);
    REQUIRE_FALSE((str1 == str2));
    REQUIRE_FALSE((str2 == str1));

    // Compile-time checks
    STATIC_REQUIRE(str1 == "Привет");
    STATIC_REQUIRE("Привет" == str1);
    STATIC_REQUIRE(str1 == str3);
    STATIC_REQUIRE(str3 == str1);
    STATIC_REQUIRE_FALSE((str1 == str2));
    STATIC_REQUIRE_FALSE((str2 == str1));
  }

  SECTION("Performance test") {
    constexpr FixedString<64> str1("This is a longer string for performance testing");
    constexpr FixedString<64> str2("This is a longer string for performance testing");
    constexpr FixedString<64> str3("This is a different string for performance testing");

    REQUIRE(str1 == str2);
    REQUIRE(str2 == str1);
    REQUIRE_FALSE((str1 == str3));
    REQUIRE_FALSE((str3 == str1));

    // Compile-time checks
    STATIC_REQUIRE(str1 == str2);
    STATIC_REQUIRE(str2 == str1);
    STATIC_REQUIRE_FALSE((str1 == str3));
    STATIC_REQUIRE_FALSE((str3 == str1));
  }

  SECTION("Constexpr operations") {
    constexpr FixedString<16> str1("Test");
    constexpr FixedString<16> str2("Test");
    constexpr FixedString<16> str3("Different");

    constexpr bool eq1 = str1 == str2;
    constexpr bool eq2 = str1 == str3;
    constexpr bool eq3 = str1 == "Test";
    constexpr bool eq4 = "Test" == str1;
    constexpr bool eq5 = str1 == "Different";
    constexpr bool eq6 = "Different" == str1;

    REQUIRE(eq1);
    REQUIRE_FALSE(eq2);
    REQUIRE(eq3);
    REQUIRE(eq4);
    REQUIRE_FALSE(eq5);
    REQUIRE_FALSE(eq6);

    // Compile-time checks
    STATIC_REQUIRE(eq1);
    STATIC_REQUIRE_FALSE(eq2);
    STATIC_REQUIRE(eq3);
    STATIC_REQUIRE(eq4);
    STATIC_REQUIRE_FALSE(eq5);
    STATIC_REQUIRE_FALSE(eq6);
  }
}

TEST_CASE("FixedString operator<=>", "[core][fixed_string]") {
  SECTION("FixedString <=> FixedString") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<32> str2("Hello");
    constexpr FixedString<16> str3("World");
    constexpr FixedString<8> str4("Hello");
    constexpr FixedString<16> str5("Hi");
    constexpr FixedString<16> str6("Hell");

    // Equal strings
    REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) == std::strong_ordering::equal);
    REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    // Different strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str5) == std::strong_ordering::less);
    REQUIRE((str5 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str6) == std::strong_ordering::greater);
    REQUIRE((str6 <=> str1) == std::strong_ordering::less);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::equal);
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str5) == std::strong_ordering::less);
    STATIC_REQUIRE((str5 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str6) == std::strong_ordering::greater);
    STATIC_REQUIRE((str6 <=> str1) == std::strong_ordering::less);
  }

  SECTION("FixedString <=> StringLike") {
    constexpr FixedString<16> str("Hello");
    const std::string stdStr1("Hello");
    const std::string stdStr2("World");
    constexpr CStringView strView1("Hello");
    constexpr CStringView strView2("World");

    REQUIRE((str <=> stdStr1) == std::strong_ordering::equal);
    REQUIRE((stdStr1 <=> str) == std::strong_ordering::equal);
    REQUIRE((str <=> stdStr2) == std::strong_ordering::less);
    REQUIRE((stdStr2 <=> str) == std::strong_ordering::greater);

    STATIC_REQUIRE((str <=> strView1) == std::strong_ordering::equal);
    STATIC_REQUIRE((strView1 <=> str) == std::strong_ordering::equal);
    STATIC_REQUIRE((str <=> strView2) == std::strong_ordering::less);
    STATIC_REQUIRE((strView2 <=> str) == std::strong_ordering::greater);
  }

  SECTION("FixedString <=> C string") {
    constexpr FixedString<16> str1("Hello");
    constexpr const char * str2 = "Hello";
    constexpr const char * str3 = "World";
    constexpr const char * str4 = "Hi";
    constexpr const char * str5 = "Hell";

    // Equal strings
    REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    // Different strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str4) == std::strong_ordering::less);
    REQUIRE((str4 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str5) == std::strong_ordering::greater);
    REQUIRE((str5 <=> str1) == std::strong_ordering::less);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::less);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str5) == std::strong_ordering::greater);
    STATIC_REQUIRE((str5 <=> str1) == std::strong_ordering::less);
  }

  SECTION("Empty string comparisons") {
    constexpr FixedString<16> empty1("");
    constexpr FixedString<32> empty2("");
    constexpr FixedString<16> nonEmpty("Test");
    constexpr const char * emptyCStr = "";
    constexpr const char * nonEmptyCStr = "Test";

    // Empty vs empty
    REQUIRE((empty1 <=> empty2) == std::strong_ordering::equal);
    REQUIRE((empty2 <=> empty1) == std::strong_ordering::equal);
    REQUIRE((empty1 <=> emptyCStr) == std::strong_ordering::equal);
    REQUIRE((emptyCStr <=> empty1) == std::strong_ordering::equal);

    // Empty vs non-empty
    REQUIRE((empty1 <=> nonEmpty) == std::strong_ordering::less);
    REQUIRE((nonEmpty <=> empty1) == std::strong_ordering::greater);
    REQUIRE((empty1 <=> nonEmptyCStr) == std::strong_ordering::less);
    REQUIRE((nonEmptyCStr <=> empty1) == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE((empty1 <=> empty2) == std::strong_ordering::equal);
    STATIC_REQUIRE((empty2 <=> empty1) == std::strong_ordering::equal);
    STATIC_REQUIRE((empty1 <=> emptyCStr) == std::strong_ordering::equal);
    STATIC_REQUIRE((emptyCStr <=> empty1) == std::strong_ordering::equal);

    STATIC_REQUIRE((empty1 <=> nonEmpty) == std::strong_ordering::less);
    STATIC_REQUIRE((nonEmpty <=> empty1) == std::strong_ordering::greater);
    STATIC_REQUIRE((empty1 <=> nonEmptyCStr) == std::strong_ordering::less);
    STATIC_REQUIRE((nonEmptyCStr <=> empty1) == std::strong_ordering::greater);
  }

  SECTION("Single character strings") {
    constexpr FixedString<8> str1("A");
    constexpr FixedString<8> str2("B");
    constexpr FixedString<8> str3("A");
    constexpr FixedString<8> str4("Z");

    // Equal single characters
    REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    // Different single characters
    REQUIRE((str1 <=> str2) == std::strong_ordering::less);
    REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str4) == std::strong_ordering::less);
    REQUIRE((str4 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str2 <=> str4) == std::strong_ordering::less);
    REQUIRE((str4 <=> str2) == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::less);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::less);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str2 <=> str4) == std::strong_ordering::less);
    STATIC_REQUIRE((str4 <=> str2) == std::strong_ordering::greater);
  }

  SECTION("Case sensitivity") {
    constexpr FixedString<16> lower("hello");
    constexpr FixedString<16> upper("HELLO");
    constexpr FixedString<16> mixed("Hello");

    // Case-sensitive comparisons
    REQUIRE((lower <=> upper) == std::strong_ordering::greater); // 'h' > 'H' in ASCII
    REQUIRE((upper <=> lower) == std::strong_ordering::less);
    REQUIRE((lower <=> mixed) == std::strong_ordering::greater); // 'h' > 'H' in ASCII
    REQUIRE((mixed <=> lower) == std::strong_ordering::less);
    REQUIRE((upper <=> mixed) == std::strong_ordering::less); // 'H' < 'H' (same), but 'E' < 'e'
    REQUIRE((mixed <=> upper) == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE((lower <=> upper) == std::strong_ordering::greater);
    STATIC_REQUIRE((upper <=> lower) == std::strong_ordering::less);
    STATIC_REQUIRE((lower <=> mixed) == std::strong_ordering::greater);
    STATIC_REQUIRE((mixed <=> lower) == std::strong_ordering::less);
    STATIC_REQUIRE((upper <=> mixed) == std::strong_ordering::less);
    STATIC_REQUIRE((mixed <=> upper) == std::strong_ordering::greater);
  }

  SECTION("Prefix comparisons") {
    constexpr FixedString<16> str1("Hello");
    constexpr FixedString<16> str2("HelloWorld");
    constexpr FixedString<16> str3("Hell");
    constexpr FixedString<16> str4("Hello");

    // One string is prefix of another
    REQUIRE((str1 <=> str2) == std::strong_ordering::less); // "Hello" < "HelloWorld"
    REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str3 <=> str1) == std::strong_ordering::less); // "Hell" < "Hello"
    REQUIRE((str1 <=> str3) == std::strong_ordering::greater);

    // Equal strings
    REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::less);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::less);
    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::greater);

    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::equal);
  }

  SECTION("Special characters") {
    constexpr FixedString<32> str1("Hello\nWorld");
    constexpr FixedString<32> str2("Hello\tWorld");
    constexpr FixedString<32> str3("Hello World");
    constexpr FixedString<32> str4("Hello\nWorld");

    // Equal strings with special characters
    REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    // Different special characters
    REQUIRE((str1 <=> str2) == std::strong_ordering::greater); // '\n' > '\t' in ASCII
    REQUIRE((str2 <=> str1) == std::strong_ordering::less);
    REQUIRE((str1 <=> str3) == std::strong_ordering::less); // '\n' < ' ' in ASCII
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str2 <=> str3) == std::strong_ordering::less); // '\t' < ' ' in ASCII
    REQUIRE((str3 <=> str2) == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::greater);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::less);
    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str2 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str2) == std::strong_ordering::greater);
  }

  SECTION("Unicode content") {
    constexpr FixedString<32> str1("Привет");
    constexpr FixedString<32> str2("Мир");
    constexpr FixedString<32> str3("Привет");
    constexpr FixedString<32> str4("Hello 🌍");

    // Equal Unicode strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    // Different Unicode strings
    REQUIRE((str1 <=> str2) != std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) != std::strong_ordering::equal);
    REQUIRE((str1 <=> str4) != std::strong_ordering::equal);
    REQUIRE((str4 <=> str1) != std::strong_ordering::equal);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str2) != std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) != std::strong_ordering::equal);
    STATIC_REQUIRE((str1 <=> str4) != std::strong_ordering::equal);
    STATIC_REQUIRE((str4 <=> str1) != std::strong_ordering::equal);
  }

  SECTION("Numeric strings") {
    constexpr FixedString<16> str1("123");
    constexpr FixedString<16> str2("456");
    constexpr FixedString<16> str3("123");
    constexpr FixedString<16> str4("12");
    constexpr FixedString<16> str5("1234");

    // Equal numeric strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    // Different numeric strings
    REQUIRE((str1 <=> str2) == std::strong_ordering::less); // "123" < "456"
    REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str4) == std::strong_ordering::greater); // "123" > "12"
    REQUIRE((str4 <=> str1) == std::strong_ordering::less);
    REQUIRE((str1 <=> str5) == std::strong_ordering::less); // "123" < "1234"
    REQUIRE((str5 <=> str1) == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::less);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::greater);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::less);
    STATIC_REQUIRE((str1 <=> str5) == std::strong_ordering::less);
    STATIC_REQUIRE((str5 <=> str1) == std::strong_ordering::greater);
  }

  SECTION("Edge cases") {
    constexpr FixedString<4> str1("ABC"); // Maximum length for capacity 4
    constexpr FixedString<8> str2("ABC");
    constexpr FixedString<16> str3("ABCD");

    // Same content, different capacities
    REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    // Different lengths
    REQUIRE((str1 <=> str3) == std::strong_ordering::less); // "ABC" < "ABCD"
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
  }

  SECTION("Constexpr operations") {
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

    REQUIRE(eq1 == std::strong_ordering::equal);
    REQUIRE(eq2 != std::strong_ordering::equal);
    REQUIRE(eq3 == std::strong_ordering::equal);
    REQUIRE(eq4 == std::strong_ordering::equal);
    REQUIRE(eq5 != std::strong_ordering::equal);
    REQUIRE(eq6 == std::strong_ordering::equal);

    // Compile-time checks
    STATIC_REQUIRE(eq1 == std::strong_ordering::equal);
    STATIC_REQUIRE(eq2 != std::strong_ordering::equal);
    STATIC_REQUIRE(eq3 == std::strong_ordering::equal);
    STATIC_REQUIRE(eq4 == std::strong_ordering::equal);
    STATIC_REQUIRE(eq5 != std::strong_ordering::equal);
    STATIC_REQUIRE(eq6 == std::strong_ordering::equal);
  }

  SECTION("Performance test") {
    constexpr FixedString<64> str1("This is a very long string for performance testing");
    constexpr FixedString<64> str2("This is a very long string for performance testing");
    constexpr FixedString<64> str3("This is a very long string for performance testing!");
    constexpr FixedString<64> str4("This is a different string for performance testing");

    // Equal long strings
    REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    // Different long strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::less); // Missing '!' at end
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str4) != std::strong_ordering::equal); // Different content
    REQUIRE((str4 <=> str1) != std::strong_ordering::equal);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::equal);
    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
  }
}

TEST_CASE("FixedString std::swap", "[core][fixed_string]") {
  SECTION("Basic swap functionality") {
    FixedString<32> str1("Hello");
    FixedString<32> str2("World");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "World") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);
  }

  SECTION("Swap with empty strings") {
    FixedString<32> str1("Hello");
    FixedString<32> str2("");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);
  }

  SECTION("Swap two empty strings") {
    FixedString<32> str1("");
    FixedString<32> str2("");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
    REQUIRE(str2.size() == 0);
    REQUIRE(std::strcmp(str2.c_str(), "") == 0);
  }

  SECTION("Self-swap") {
    FixedString<32> str1("Hello");

    std::swap(str1, str1);

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);
  }

  SECTION("Swap with different sizes") {
    FixedString<32> str1("Hi");
    FixedString<32> str2("VeryLongString");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 14);
    REQUIRE(std::strcmp(str1.c_str(), "VeryLongString") == 0);
    REQUIRE(str2.size() == 2);
    REQUIRE(std::strcmp(str2.c_str(), "Hi") == 0);
  }

  SECTION("Swap with maximum length strings") {
    FixedString<16> str1("123456789012345"); // 15 chars
    FixedString<16> str2("ABCDEFGHIJKLMNO"); // 15 chars

    std::swap(str1, str2);

    REQUIRE(str1.size() == 15);
    REQUIRE(std::strcmp(str1.c_str(), "ABCDEFGHIJKLMNO") == 0);
    REQUIRE(str2.size() == 15);
    REQUIRE(std::strcmp(str2.c_str(), "123456789012345") == 0);
  }

  SECTION("Swap with special characters") {
    FixedString<32> str1("Hello,\n\t!");
    FixedString<32> str2("World,\r\n?");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 9);
    REQUIRE(std::strcmp(str1.c_str(), "World,\r\n?") == 0);
    REQUIRE(str2.size() == 9);
    REQUIRE(std::strcmp(str2.c_str(), "Hello,\n\t!") == 0);
  }

  SECTION("Swap with Unicode content") {
    FixedString<32> str1("Hello 世界");
    FixedString<32> str2("World 宇宙");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 12);
    REQUIRE(std::strcmp(str1.c_str(), "World 宇宙") == 0);
    REQUIRE(str2.size() == 12);
    REQUIRE(std::strcmp(str2.c_str(), "Hello 世界") == 0);
  }

  SECTION("Multiple swaps") {
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

  SECTION("Performance test with large strings") {
    FixedString<64> str1("This is a very long string that tests swap performance");
    FixedString<64> str2("Another very long string for performance testing");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 48);
    REQUIRE(std::strcmp(str1.c_str(), "Another very long string for performance testing") == 0);
    REQUIRE(str2.size() == 54);
    REQUIRE(std::strcmp(str2.c_str(), "This is a very long string that tests swap performance") == 0);
  }

  SECTION("Swap with single character strings") {
    FixedString<8> str1("A");
    FixedString<8> str2("B");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "B") == 0);
    REQUIRE(str2.size() == 1);
    REQUIRE(std::strcmp(str2.c_str(), "A") == 0);
  }
}
