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

using namespace toygine;

TEST_CASE("FixString constructors", "[core][fixstring]") {
  SECTION("Default constructor") {
    constexpr FixString<32> emptyStr;

    REQUIRE(emptyStr.size() == 0);
    REQUIRE(std::strcmp(emptyStr.c_str(), "") == 0);
    REQUIRE(emptyStr.capacity() == 31);
    REQUIRE(emptyStr.max_size() == 31);

    STATIC_REQUIRE(emptyStr.size() == 0);
    STATIC_REQUIRE(cstrcmp(emptyStr.c_str(), "") == 0);
    STATIC_REQUIRE(emptyStr.capacity() == 31);
    STATIC_REQUIRE(emptyStr.max_size() == 31);
  }

  SECTION("C string constructor") {
    constexpr FixString<16> str1("Hello");
    constexpr FixString<32> str2("World");
    constexpr FixString<8> str3("Test");
    constexpr FixString<64> str4("This is a longer string for testing");

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "World") == 0);
    REQUIRE(str3.size() == 4);
    REQUIRE(std::strcmp(str3.c_str(), "Test") == 0);
    REQUIRE(str4.size() == 35);
    REQUIRE(std::strcmp(str4.c_str(), "This is a longer string for testing") == 0);

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
    constexpr FixString<16> original("CopyTest");
    constexpr FixString<16> copy1(original);
    constexpr FixString<32> copy2(original);
    constexpr FixString<12> copy3(original);

    REQUIRE(copy1.size() == 8);
    REQUIRE(std::strcmp(copy1.c_str(), "CopyTest") == 0);
    REQUIRE(copy2.size() == 8);
    REQUIRE(std::strcmp(copy2.c_str(), "CopyTest") == 0);
    REQUIRE(copy3.size() == 8);
    REQUIRE(std::strcmp(copy3.c_str(), "CopyTest") == 0);

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

    STATIC_REQUIRE(copy1 == original);
    STATIC_REQUIRE(copy2 == original);
    STATIC_REQUIRE(copy3 == original);
  }

  SECTION("Character constructor") {
    constexpr FixString<16> single('A');
    constexpr FixString<32> multiple('B', 5);
    constexpr FixString<8> many('C', 7);
    constexpr FixString<64> empty('D', 0);

    REQUIRE(single.size() == 1);
    REQUIRE(std::strcmp(single.c_str(), "A") == 0);
    REQUIRE(multiple.size() == 5);
    REQUIRE(std::strcmp(multiple.c_str(), "BBBBB") == 0);
    REQUIRE(many.size() == 7);
    REQUIRE(std::strcmp(many.c_str(), "CCCCCCC") == 0);
    REQUIRE(empty.size() == 0);
    REQUIRE(std::strcmp(empty.c_str(), "") == 0);

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
    const std::string stdStr("StringLike");
    const FixString<16> fromStd(stdStr);
    const FixString<32> fromFix(fromStd);

    REQUIRE(fromStd.size() == 10);
    REQUIRE(std::strcmp(fromStd.c_str(), "StringLike") == 0);
    REQUIRE(fromFix.size() == 10);
    REQUIRE(std::strcmp(fromFix.c_str(), "StringLike") == 0);

    // std::string can't be a constant expression
  }

  SECTION("Edge cases") {
    // Empty string
    constexpr FixString<16> empty1("");
    constexpr FixString<32> empty2("");

    REQUIRE(empty1.size() == 0);
    REQUIRE(empty2.size() == 0);

    STATIC_REQUIRE(empty1.size() == 0);
    STATIC_REQUIRE(empty2.size() == 0);

    // Single character
    constexpr FixString<8> single("X");

    REQUIRE(single.size() == 1);
    REQUIRE(std::strcmp(single.c_str(), "X") == 0);

    STATIC_REQUIRE(single.size() == 1);
    STATIC_REQUIRE(cstrcmp(single.c_str(), "X") == 0);

    // Maximum length
    constexpr FixString<5> maxLen("Test");

    REQUIRE(maxLen.size() == 4);
    REQUIRE(std::strcmp(maxLen.c_str(), "Test") == 0);

    STATIC_REQUIRE(maxLen.size() == 4);
    STATIC_REQUIRE(cstrcmp(maxLen.c_str(), "Test") == 0);
  }

  SECTION("Special characters") {
    constexpr FixString<32> newline("Line1\nLine2");
    constexpr FixString<32> tab("Col1\tCol2");
    constexpr FixString<32> mixed("Mix\t\nEnd");

    REQUIRE(newline.size() == 11);
    REQUIRE(std::strcmp(newline.c_str(), "Line1\nLine2") == 0);
    REQUIRE(tab.size() == 9);
    REQUIRE(std::strcmp(tab.c_str(), "Col1\tCol2") == 0);
    REQUIRE(mixed.size() == 8);
    REQUIRE(std::strcmp(mixed.c_str(), "Mix\t\nEnd") == 0);

    STATIC_REQUIRE(newline.size() == 11);
    STATIC_REQUIRE(cstrcmp(newline.c_str(), "Line1\nLine2") == 0);
    STATIC_REQUIRE(tab.size() == 9);
    STATIC_REQUIRE(cstrcmp(tab.c_str(), "Col1\tCol2") == 0);
    STATIC_REQUIRE(mixed.size() == 8);
    STATIC_REQUIRE(cstrcmp(mixed.c_str(), "Mix\t\nEnd") == 0);
  }

  SECTION("Unicode content") {
    constexpr FixString<64> unicode("Привет мир");
    constexpr FixString<32> emoji("Hello 🌍");

    REQUIRE(unicode.size() == 19); // UTF-8 bytes
    REQUIRE(std::strcmp(unicode.c_str(), "Привет мир") == 0);
    REQUIRE(emoji.size() == 10); // UTF-8 bytes
    REQUIRE(std::strcmp(emoji.c_str(), "Hello 🌍") == 0);

    STATIC_REQUIRE(unicode.size() == 19);
    STATIC_REQUIRE(cstrcmp(unicode.c_str(), "Привет мир") == 0);
    STATIC_REQUIRE(emoji.size() == 10);
    STATIC_REQUIRE(cstrcmp(emoji.c_str(), "Hello 🌍") == 0);
  }
}

TEST_CASE("FixString operators=", "[core][fixstring]") {
  SECTION("C string assignment") {
    FixString<12> str1;
    FixString<24> str2;
    FixString<8> str3;

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
  }

  SECTION("FixString assignment (same capacity)") {
    FixString<12> str1("Hello");
    FixString<12> str2;
    FixString<12> str3("World");

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
  }

  SECTION("FixString assignment (different capacities)") {
    FixString<8> str1("Test");
    FixString<16> str2;
    FixString<32> str3;

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
  }

  SECTION("StringLike assignment") {
    FixString<12> str1;
    FixString<24> str2;

    const std::string stdStr("Hello World");
    const FixString<16> fixStr("Test String");

    str1 = stdStr;
    REQUIRE(str1.size() == 11);
    REQUIRE(std::strcmp(str1.c_str(), "Hello World") == 0);

    str2 = fixStr;
    REQUIRE(str2.size() == 11);
    REQUIRE(std::strcmp(str2.c_str(), "Test String") == 0);
  }

  SECTION("Character assignment") {
    FixString<8> str1;
    FixString<16> str2;
    FixString<32> str3;

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
    FixString<6> str1;
    FixString<8> str2;

    // Maximum length assignment
    str1 = "ABC"; // 3 chars + null terminator = 4 total
    REQUIRE(str1.size() == 3);
    REQUIRE(std::strcmp(str1.c_str(), "ABC") == 0);

    // Empty to non-empty
    str2 = "Test";
    str1 = str2;
    REQUIRE(str1.size() == 4);
    REQUIRE(std::strcmp(str1.c_str(), "Test") == 0);

    // Non-empty to empty
    str2 = "";
    str1 = str2;
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
  }

  SECTION("Special characters") {
    FixString<16> str1;
    FixString<24> str2;

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
    FixString<32> str1;
    FixString<48> str2;

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

  SECTION("Constexpr operations") {
    constexpr FixString<12> str1 = "Test";
    constexpr FixString<24> str2 = str1;
    constexpr FixString<16> str3 = str2;
    constexpr FixString<32> str4 = "";

    REQUIRE(str1.size() == 4);
    REQUIRE(std::strcmp(str1.c_str(), "Test") == 0);
    REQUIRE(str2.size() == 4);
    REQUIRE(std::strcmp(str2.c_str(), "Test") == 0);
    REQUIRE(str3.size() == 4);
    REQUIRE(std::strcmp(str3.c_str(), "Test") == 0);
    REQUIRE(str4.size() == 0);
    REQUIRE(std::strcmp(str4.c_str(), "") == 0);

    STATIC_REQUIRE(str1.size() == 4);
    STATIC_REQUIRE(cstrcmp(str1.c_str(), "Test") == 0);
    STATIC_REQUIRE(str2.size() == 4);
    STATIC_REQUIRE(cstrcmp(str2.c_str(), "Test") == 0);
    STATIC_REQUIRE(str3.size() == 4);
    STATIC_REQUIRE(cstrcmp(str3.c_str(), "Test") == 0);
    STATIC_REQUIRE(str4.size() == 0);
    STATIC_REQUIRE(cstrcmp(str4.c_str(), "") == 0);
  }
}

TEST_CASE("FixString assign", "[core][fixstring]") {
  SECTION("C string assignment") {
    FixString<16> str1;
    FixString<32> str2;
    FixString<8> str3;

    // Basic assignment
    str1.assign("Hello");
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);

    str2.assign("World");
    REQUIRE(std::strcmp(str2.c_str(), "World") == 0);

    // Empty string assignment
    str1.assign("");
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Long string assignment
    str2.assign("VeryLongString");
    REQUIRE(std::strcmp(str2.c_str(), "VeryLongString") == 0);

    // Single character
    str3.assign("A");
    REQUIRE(std::strcmp(str3.c_str(), "A") == 0);

    // Compile-time checks
    constexpr FixString<16> constStr1 = FixString<16>().assign("Hello");
    constexpr FixString<32> constStr2 = FixString<32>("World").assign("VeryLongString");
    constexpr FixString<16> constStr3 = FixString<16>("A").assign("");
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello") == 0);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "VeryLongString") == 0);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "") == 0);
  }

  SECTION("FixString assignment (same capacity)") {
    FixString<16> str1("Hello");
    FixString<16> str2("World");
    FixString<16> str3;

    // Basic assignment
    str3.assign(str1);
    REQUIRE(std::strcmp(str3.c_str(), "Hello") == 0);

    // Assignment from another string
    str2.assign(str1);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);

    // Self-assignment
    str1.assign(str1);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);

    // Empty string assignment
    FixString<16> emptyStr("");
    str1.assign(emptyStr);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Compile-time checks
    constexpr FixString<16> constStr1("Hello");
    constexpr FixString<16> constStr2();
    constexpr FixString<16> constStr3 = FixString<16>("World").assign(constStr1);
    constexpr FixString<16> constStr4 = FixString<16>().assign(constStr3);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "Hello") == 0);
    STATIC_REQUIRE(cstrcmp(constStr4.c_str(), "Hello") == 0);
  }

  SECTION("FixString assignment (different capacities)") {
    FixString<8> str1("Hi");
    FixString<16> str2("Hello");

    // Assign from smaller to larger
    str2.assign(str1);
    REQUIRE(std::strcmp(str2.c_str(), "Hi") == 0);

    // Assign from larger to smaller
    str1.assign(str2);
    REQUIRE(std::strcmp(str1.c_str(), "Hi") == 0);

    // Compile-time checks
    constexpr FixString<8> constStr1("Hi");
    constexpr FixString<16> constStr2("Hello");
    constexpr FixString<16> constStr3 = FixString<8>("Hi").assign(constStr2);
    constexpr FixString<8> constStr4 = FixString<16>("Hello").assign(constStr1);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "Hello") == 0);
    STATIC_REQUIRE(cstrcmp(constStr4.c_str(), "Hi") == 0);
  }

  SECTION("StringLike assignment") {
    FixString<16> str1;
    std::string str2("Hello");
    std::string str3("World");

    // Assign from std::string
    str1.assign(str2);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);

    // Assign from another std::string
    str1.assign(str3);
    REQUIRE(std::strcmp(str1.c_str(), "World") == 0);

    // Assign from empty std::string
    std::string emptyStr("");
    str1.assign(emptyStr);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
  }

  SECTION("Character assignment") {
    FixString<16> str1;
    FixString<8> str2;

    // Single character assignment
    str1.assign('A');
    REQUIRE(std::strcmp(str1.c_str(), "A") == 0);

    // Multiple character assignment
    str2.assign('B', 3);
    REQUIRE(std::strcmp(str2.c_str(), "BBB") == 0);

    // Zero count assignment
    str1.assign('C', 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Large count assignment
    str2.assign('D', 5);
    REQUIRE(std::strcmp(str2.c_str(), "DDDDD") == 0);

    // Compile-time checks
    constexpr FixString<8> constStr1 = FixString<8>().assign('A');
    constexpr FixString<8> constStr2 = FixString<8>().assign('B', 3);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "A") == 0);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "BBB") == 0);
  }

  SECTION("Edge cases") {
    FixString<4> str1("ABC");
    FixString<8> str2("ABC");
    FixString<16> str3("ABCD");

    // Assign to maximum capacity
    str1.assign("XYZ");
    REQUIRE(std::strcmp(str1.c_str(), "XYZ") == 0);

    // Assign from own c_str() (no-op path)
    str2.assign(str2.c_str());
    REQUIRE(std::strcmp(str2.c_str(), "ABC") == 0);

    // Assign empty string
    str3.assign("");
    REQUIRE(std::strcmp(str3.c_str(), "") == 0);

    // Compile-time checks
    constexpr FixString<4> constStr1 = FixString<4>("ABC").assign("XYZ");
    constexpr FixString<8> constStr2 = FixString<8>("ABCD").assign("");
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "XYZ") == 0);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "") == 0);
  }

  SECTION("Special characters") {
    FixString<32> str1;
    FixString<16> str2;

    // Newline and tab
    str1.assign("Hello\n\tWorld");
    REQUIRE(std::strcmp(str1.c_str(), "Hello\n\tWorld") == 0);

    // Special characters
    str2.assign("!@#$%^&*()");
    REQUIRE(std::strcmp(str2.c_str(), "!@#$%^&*()") == 0);

    // Compile-time checks
    constexpr FixString<32> constStr1 = FixString<32>().assign("Hello\n\tWorld");
    constexpr FixString<16> constStr2 = FixString<16>().assign("!@#$%^&*()");
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello\n\tWorld") == 0);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "!@#$%^&*()") == 0);
  }

  SECTION("Unicode content") {
    FixString<32> str1;
    FixString<16> str2;

    // Unicode characters
    str1.assign("Hello 世界");
    REQUIRE(std::strcmp(str1.c_str(), "Hello 世界") == 0);

    // Mixed ASCII and Unicode
    str2.assign("Test 🌍");
    REQUIRE(std::strcmp(str2.c_str(), "Test 🌍") == 0);

    // Compile-time checks
    constexpr FixString<32> constStr1 = FixString<32>().assign("Hello 世界");
    constexpr FixString<16> constStr2 = FixString<16>().assign("Test 🌍");
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello 世界") == 0);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Test 🌍") == 0);
  }

  SECTION("Chaining assign") {
    FixString<16> str1;

    // Chaining assign operations
    str1.assign("a").assign("b");
    REQUIRE(std::strcmp(str1.c_str(), "b") == 0);

    // Multiple chaining
    str1.assign("Hello").assign("World").assign("Test");
    REQUIRE(std::strcmp(str1.c_str(), "Test") == 0);

    // Chaining with different types
    std::string str2("Chained");
    str1.assign("Initial").assign(str2);
    REQUIRE(std::strcmp(str1.c_str(), "Chained") == 0);

    // Compile-time checks
    constexpr FixString<16> constStr1 = FixString<16>("a").assign("b");
    constexpr FixString<16> constStr2 = FixString<16>("Hello").assign("Test");
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "b") == 0);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Test") == 0);
  }

  SECTION("Performance test") {
    FixString<64> str1;
    FixString<32> str2;
    std::string str3("PerformanceTest");

    // Multiple assignments
    for (int i = 0; i < 100; ++i) {
      str1.assign("Test").assign(str3).assign("Another");
      str2.assign(str1);
    }

    REQUIRE(std::strcmp(str1.c_str(), "Another") == 0);
    REQUIRE(std::strcmp(str2.c_str(), "Another") == 0);
  }
}

// to refactor

TEST_CASE("FixString at", "[core][fixstring]") {
  FixString<8> testString1("abcd");
  constexpr FixString<8> testString2("dcba");

  testString1.at(0) = 'e';
  testString1.at(1) = 'f';
  testString1.at(2) = 'g';
  testString1.at(3) = 'h';

  REQUIRE(std::strcmp(testString1.c_str(), "efgh") == 0);

  auto & ref = testString1.at(2);
  ref = 'Z';
  REQUIRE(testString1.at(2) == 'Z');

  REQUIRE(testString1.at(1) == 'f');
  REQUIRE(testString2.at(1) == 'c');

  REQUIRE(testString1.size() == 4);
  REQUIRE(std::strcmp(testString2.c_str(), "dcba") == 0);

  // Compile-time checks with STATIC_REQUIRE
  STATIC_REQUIRE(testString2.at(0) == 'd');
  STATIC_REQUIRE(testString2.at(1) == 'c');
  STATIC_REQUIRE(testString2.at(2) == 'b');
  STATIC_REQUIRE(testString2.at(3) == 'a');
  STATIC_REQUIRE(cstrcmp(testString2.c_str(), "dcba") == 0);
  STATIC_REQUIRE(testString2.size() == 4);
}

TEST_CASE("FixString operators[]", "[core][fixstring]") {
  FixString<8> testString1("abcd");
  constexpr FixString<8> testString2("dcba");

  testString1[0] = 'e';
  testString1[1] = 'f';
  testString1[2] = 'g';
  testString1[3] = 'h';

  REQUIRE(std::strcmp(testString1.c_str(), "efgh") == 0);

  auto & ref = testString1[2];
  ref = 'Z';
  REQUIRE(testString1[2] == 'Z');

  REQUIRE(testString1[1] == 'f');
  REQUIRE(testString2[1] == 'c');

  REQUIRE(testString1.size() == 4);
  REQUIRE(std::strcmp(testString2.c_str(), "dcba") == 0);

  // Compile-time checks with STATIC_REQUIRE
  // STATIC_REQUIRE(testString2[0] == 'd');
  // STATIC_REQUIRE(testString2[1] == 'c');
  // STATIC_REQUIRE(testString2[2] == 'b');
  // STATIC_REQUIRE(testString2[3] == 'a');
  // STATIC_REQUIRE(std::strcmp(testString2.c_str(), "dcba") == 0);
  // STATIC_REQUIRE(testString2.size() == 4);
}

TEST_CASE("FixString front and back", "[core][fixstring]") {
  SECTION("Front method") {
    FixString<16> testString("Hello World");

    REQUIRE(testString.front() == 'H');
    REQUIRE(testString[0] == 'H');

    // Test modification
    testString.front() = 'h';
    REQUIRE(testString.front() == 'h');
    REQUIRE(testString[0] == 'h');
  }

  SECTION("Front const method") {
    constexpr FixString<12> testString("Hello World");

    REQUIRE(testString.front() == 'H');
    REQUIRE(testString[0] == 'H');
  }

  SECTION("Back method") {
    FixString<16> testString("Hello World");

    REQUIRE(testString.back() == 'd');
    REQUIRE(testString[testString.size() - 1] == 'd');

    // Test modification
    testString.back() = 'D';
    REQUIRE(testString.back() == 'D');
    REQUIRE(testString[testString.size() - 1] == 'D');
  }

  SECTION("Back const method") {
    constexpr FixString<12> testString("Hello World");

    REQUIRE(testString.back() == 'd');
    REQUIRE(testString[testString.size() - 1] == 'd');
  }

  SECTION("Single character string") {
    FixString<2> testString("A");

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
  }

  SECTION("Modification through references") {
    FixString<32> testString("Hello World");

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
    constexpr FixString<32> testString("Hello World");

    // Get const references
    const auto & frontRef = testString.front();
    const auto & backRef = testString.back();

    REQUIRE(frontRef == 'H');
    REQUIRE(backRef == 'd');
    REQUIRE(frontRef == testString[0]);
    REQUIRE(backRef == testString[testString.size() - 1]);
  }
}

TEST_CASE("FixString data", "[core][fixstring]") {
  constexpr FixString<8> testString1("abcd");
  constexpr FixString<8> testString2("dcba");

  REQUIRE(std::strcmp(testString1.data(), "abcd") == 0);
  REQUIRE(std::strcmp(testString2.data(), "dcba") == 0);

  // Compile-time checks with STATIC_REQUIRE
  // STATIC_REQUIRE(std::strcmp(testString1.data(), "abcd") == 0);
  // STATIC_REQUIRE(std::strcmp(testString2.data(), "dcba") == 0);
  // STATIC_REQUIRE(testString1.data()[0] == 'a');
  // STATIC_REQUIRE(testString1.data()[1] == 'b');
  // STATIC_REQUIRE(testString2.data()[0] == 'd');
  // STATIC_REQUIRE(testString2.data()[1] == 'c');
}

TEST_CASE("FixString c_str", "[core][fixstring]") {
  constexpr FixString<8> testString1("abcd");
  constexpr FixString<8> testString2("dcba");

  REQUIRE(std::strcmp(testString1.c_str(), "abcd") == 0);
  REQUIRE(std::strcmp(testString2.c_str(), "dcba") == 0);

  // Compile-time checks with STATIC_REQUIRE
  // STATIC_REQUIRE(std::strcmp(testString1.c_str(), "abcd") == 0);
  // STATIC_REQUIRE(std::strcmp(testString2.c_str(), "dcba") == 0);
  // STATIC_REQUIRE(testString1.c_str()[0] == 'a');
  // STATIC_REQUIRE(testString1.c_str()[1] == 'b');
  // STATIC_REQUIRE(testString2.c_str()[0] == 'd');
  // STATIC_REQUIRE(testString2.c_str()[1] == 'c');
}

TEST_CASE("FixString empty", "[core][fixstring]") {
  constexpr FixString<16> testString1("ToyGine2");
  constexpr FixString<4> testString2("");

  REQUIRE_FALSE(testString1.empty());
  REQUIRE(testString2.empty());

  // Compile-time checks with STATIC_REQUIRE
  // STATIC_REQUIRE_FALSE(testString1.empty());
  // STATIC_REQUIRE(testString2.empty());
  // STATIC_REQUIRE(testString1.size() > 0);
  // STATIC_REQUIRE(testString2.size() == 0);
}

TEST_CASE("FixString size", "[core][fixstring]") {
  constexpr FixString<64> testString1("ToyGine2 - Free 2D/3D game engine.");
  constexpr FixString<64> testString2;

  REQUIRE(testString1.size() == 34);
  REQUIRE(testString2.size() == 0);

  // Compile-time checks with STATIC_REQUIRE
  // STATIC_REQUIRE(testString1.size() == 34);
  // STATIC_REQUIRE(testString2.size() == 0);
  // STATIC_REQUIRE(testString1.length() == 34);
  // STATIC_REQUIRE(testString2.length() == 0);
}

TEST_CASE("FixString utf8_size", "[core][fixstring]") {
  static constexpr char ansiText[] = "ToyGine2 - Free 2D/3D game engine.";
  // UTF8 encoding                   "ToyGine2 - Бесплатный 2D/3D игровой движок.";

  static constexpr std::array<char, 67> utf8Text{
    {char(0x54), char(0x6F), char(0x79), char(0x47), char(0x69), char(0x6E), char(0x65), char(0x32), char(0x20),
     char(0x2D), char(0x20), char(0xD0), char(0x91), char(0xD0), char(0xB5), char(0xD1), char(0x81), char(0xD0),
     char(0xBF), char(0xD0), char(0xBB), char(0xD0), char(0xB0), char(0xD1), char(0x82), char(0xD0), char(0xBD),
     char(0xD1), char(0x8B), char(0xD0), char(0xB9), char(0x20), char(0x32), char(0x44), char(0x2F), char(0x33),
     char(0x44), char(0x20), char(0xD0), char(0xB8), char(0xD0), char(0xB3), char(0xD1), char(0x80), char(0xD0),
     char(0xBE), char(0xD0), char(0xB2), char(0xD0), char(0xBE), char(0xD0), char(0xB9), char(0x20), char(0xD0),
     char(0xB4), char(0xD0), char(0xB2), char(0xD0), char(0xB8), char(0xD0), char(0xB6), char(0xD0), char(0xBE),
     char(0xD0), char(0xBA), char(0x2E), char(0x00)}};

  constexpr FixString<64> testString1(ansiText);
  constexpr FixString<80> testString2(utf8Text.data());
  constexpr FixString<96> testString3;

  REQUIRE(testString1.size() == testString1.utf8_size());
  REQUIRE(testString3.size() == testString3.utf8_size());
  REQUIRE(testString2.size() == 66);
  REQUIRE(testString2.utf8_size() == 43);
}

TEST_CASE("FixString length", "[core][fixstring]") {
  constexpr FixString<64> testString1("ToyGine2 - Free 2D/3D game engine.");
  constexpr FixString<64> testString2;

  REQUIRE(testString1.length() == 34);
  REQUIRE(testString2.length() == 0);
}

TEST_CASE("FixString max_size", "[core][fixstring]") {
  constexpr FixString<64> testString1("ToyGine2 - Free 2D/3D game engine.");
  constexpr FixString<16> testString2("");

  REQUIRE(testString1.max_size() == 63);
  REQUIRE(testString2.max_size() == 15);
}

TEST_CASE("FixString capacity", "[core][fixstring]") {
  constexpr FixString<64> testString1("ToyGine2");
  constexpr FixString<16> testString2("");

  REQUIRE(testString1.capacity() == 63);
  REQUIRE(testString2.capacity() == 15);
}

TEST_CASE("FixString clear", "[core][fixstring]") {
  FixString<64> testString1("ToyGine2 - Free 2D/3D game engine.");
  FixString<16> testString2;

  REQUIRE_FALSE(testString1.empty());
  REQUIRE(testString2.empty());

  testString1.clear();
  testString2.clear();

  REQUIRE(testString1.empty());
  REQUIRE(testString2.empty());
}

TEST_CASE("FixString insert", "[core][fixstring]") {
  FixString<32> testString1("Hello World");
  FixString<32> testString2("Hello World");
  FixString<32> testString3("Hello World");

  // Insert at beginning
  testString1.insert(0, FixString<32>("Hi "));
  testString2.insert(0, FixString<16>("Hi "));
  testString3.insert(0, "Hi ");
  REQUIRE(std::strcmp(testString1.c_str(), "Hi Hello World") == 0);
  REQUIRE(std::strcmp(testString2.c_str(), "Hi Hello World") == 0);
  REQUIRE(std::strcmp(testString3.c_str(), "Hi Hello World") == 0);
  REQUIRE(testString1.size() == 14);
  REQUIRE(testString2.size() == 14);
  REQUIRE(testString3.size() == 14);

  // Insert in middle
  testString1.insert(9, FixString<32>("Beautiful "));
  testString2.insert(9, FixString<64>("Beautiful "));
  testString3.insert(9, "Beautiful ");
  REQUIRE(std::strcmp(testString1.c_str(), "Hi Hello Beautiful World") == 0);
  REQUIRE(std::strcmp(testString2.c_str(), "Hi Hello Beautiful World") == 0);
  REQUIRE(std::strcmp(testString3.c_str(), "Hi Hello Beautiful World") == 0);
  REQUIRE(testString1.size() == 24);
  REQUIRE(testString2.size() == 24);
  REQUIRE(testString3.size() == 24);

  // Insert at end
  testString1.insert(24, FixString<32>("!"));
  testString2.insert(24, FixString<8>("!"));
  testString3.insert(24, "!");
  REQUIRE(std::strcmp(testString1.c_str(), "Hi Hello Beautiful World!") == 0);
  REQUIRE(std::strcmp(testString2.c_str(), "Hi Hello Beautiful World!") == 0);
  REQUIRE(std::strcmp(testString3.c_str(), "Hi Hello Beautiful World!") == 0);
  REQUIRE(testString1.size() == 25);
  REQUIRE(testString2.size() == 25);
  REQUIRE(testString3.size() == 25);

  FixString<32> testString4("Hello World");

  // Insert single character
  testString4.insert(5, ' ');
  REQUIRE(std::strcmp(testString4.c_str(), "Hello  World") == 0);
  REQUIRE(testString4.size() == 12);

  // Insert multiple characters
  testString4.insert(0, '*', 3);
  REQUIRE(std::strcmp(testString4.c_str(), "***Hello  World") == 0);
  REQUIRE(testString4.size() == 15);

  // Insert at end
  testString4.insert(15, '!', 2);
  REQUIRE(std::strcmp(testString4.c_str(), "***Hello  World!!") == 0);
  REQUIRE(testString4.size() == 17);

  // Insert zero characters
  testString4.insert(0, 'X', 0);
  REQUIRE(std::strcmp(testString4.c_str(), "***Hello  World!!") == 0);
  REQUIRE(testString4.size() == 17);
}

TEST_CASE("FixString erase", "[core][fixstring]") {
  FixString<32> testString1("Hello World");

  // Erase first 5 characters
  testString1.erase(0, 5);
  REQUIRE(std::strcmp(testString1.c_str(), " World") == 0);
  REQUIRE(testString1.size() == 6);

  // Erase remaining characters
  testString1.erase(0);
  REQUIRE(std::strcmp(testString1.c_str(), "") == 0);
  REQUIRE(testString1.size() == 0);

  FixString<32> testString2("Hello Beautiful World");

  // Erase middle word
  testString2.erase(6, 10);
  REQUIRE(std::strcmp(testString2.c_str(), "Hello World") == 0);
  REQUIRE(testString2.size() == 11);

  // Erase part of remaining text
  testString2.erase(5, 1);
  REQUIRE(std::strcmp(testString2.c_str(), "HelloWorld") == 0);
  REQUIRE(testString2.size() == 10);

  FixString<32> testString3("Hello World!");

  // Erase last character
  testString3.erase(11, 1);
  REQUIRE(std::strcmp(testString3.c_str(), "Hello World") == 0);
  REQUIRE(testString3.size() == 11);

  // No-op: erase with count == 0
  testString3.erase(5, 0);
  REQUIRE(std::strcmp(testString3.c_str(), "Hello World") == 0);
  REQUIRE(testString3.size() == 11);

  // Erase last word
  testString3.erase(6);
  REQUIRE(std::strcmp(testString3.c_str(), "Hello ") == 0);
  REQUIRE(testString3.size() == 6);

  // Erase everything from position 0
  testString3.erase(0);
  REQUIRE(testString3.empty());
}

TEST_CASE("FixString push_back", "[core][fixstring]") {
  FixString<16> testString1("Hello");

  testString1.push_back(' ');
  testString1.push_back('W');
  testString1.push_back('o');
  testString1.push_back('r');
  testString1.push_back('l');
  testString1.push_back('d');

  REQUIRE(std::strcmp(testString1.c_str(), "Hello World") == 0);
  REQUIRE(testString1.size() == 11);
}

TEST_CASE("FixString pop_back", "[core][fixstring]") {
  FixString<16> testString("Hi");

  testString.pop_back();
  REQUIRE(std::strcmp(testString.c_str(), "H") == 0);
  REQUIRE(testString.size() == 1);

  testString.pop_back();
  REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  REQUIRE(testString.size() == 0);
}

TEST_CASE("FixString utf8_pop_back", "[core][fixstring]") {
  SECTION("Pop back single ASCII character") {
    FixString<16> testString("Hello");

    testString.utf8_pop_back();

    REQUIRE(std::strcmp(testString.c_str(), "Hell") == 0);
    REQUIRE(testString.size() == 4);
    REQUIRE(testString.utf8_size() == 4);
  }

  SECTION("Pop back multiple ASCII characters") {
    FixString<16> testString("Hello");

    testString.utf8_pop_back();
    testString.utf8_pop_back();
    testString.utf8_pop_back();

    REQUIRE(std::strcmp(testString.c_str(), "He") == 0);
    REQUIRE(testString.size() == 2);
    REQUIRE(testString.utf8_size() == 2);
  }

  SECTION("Pop back UTF-8 character (2 bytes)") {
    // "Hello привет" - "привет" contains 6 Cyrillic characters, each 2 bytes in UTF-8
    FixString<32> testString("Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82");

    testString.utf8_pop_back(); // Remove 'т' (2 bytes)

    REQUIRE(std::strcmp(testString.c_str(), "Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5") == 0);
    REQUIRE(testString.size() == 16); // "Hello приве" (5 + 1 + 5*2 = 16 bytes)
    REQUIRE(testString.utf8_size() == 11);
  }

  SECTION("Pop back multiple UTF-8 characters") {
    // "Hello привет" - "привет" contains 6 Cyrillic characters
    FixString<32> testString("Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82");

    testString.utf8_pop_back(); // Remove 'т'
    testString.utf8_pop_back(); // Remove 'е'
    testString.utf8_pop_back(); // Remove 'в'

    REQUIRE(std::strcmp(testString.c_str(), "Hello \xD0\xBF\xD1\x80\xD0\xB8") == 0);
    REQUIRE(testString.size() == 12); // "Hello при" (5 + 1 + 3*2 = 12 bytes)
    REQUIRE(testString.utf8_size() == 9);
  }

  SECTION("Pop back mixed ASCII and UTF-8") {
    // "Hello привет" - mix of ASCII and Cyrillic
    FixString<32> testString("Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82");

    testString.utf8_pop_back(); // Remove 'т'
    testString.utf8_pop_back(); // Remove 'е'
    testString.utf8_pop_back(); // Remove 'в'
    testString.utf8_pop_back(); // Remove 'и'
    testString.utf8_pop_back(); // Remove 'р'
    testString.utf8_pop_back(); // Remove 'п'

    REQUIRE(std::strcmp(testString.c_str(), "Hello ") == 0);
    REQUIRE(testString.size() == 6);
    REQUIRE(testString.utf8_size() == 6);
  }

  SECTION("Pop back from single character string") {
    FixString<8> testString("A");

    testString.utf8_pop_back();

    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
    REQUIRE(testString.size() == 0);
    REQUIRE(testString.utf8_size() == 0);
  }

  SECTION("Pop back from single UTF-8 character string") {
    // Single Cyrillic character 'п'
    FixString<8> testString("\xD0\xBF");

    testString.utf8_pop_back();

    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
    REQUIRE(testString.size() == 0);
    REQUIRE(testString.utf8_size() == 0);
  }
}

TEST_CASE("FixString append", "[core][fixstring]") {
  FixString<32> testString("");

  testString.append(FixString<16>("Nothing"))
    .append(FixString<8>(" else"))
    .append(std::string(" really"))
    .append(" matters")
    .append('.', 3);

  REQUIRE(std::strcmp(testString.c_str(), "Nothing else really matters...") == 0);
  REQUIRE(testString.size() == 30);
}

TEST_CASE("FixString operators+=", "[core][fixstring]") {
  FixString<32> testString("");

  testString += FixString<16>("Nothing");
  testString += FixString<8>(" else");
  testString += std::string(" really");
  testString += " matters";
  testString += '.';

  REQUIRE(std::strcmp(testString.c_str(), "Nothing else really matters.") == 0);
  REQUIRE(testString.size() == 28);
}

TEST_CASE("FixString replace", "[core][fixstring]") {
  SECTION("Replace with FixString") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, FixString<16>("Universe"));

    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
    REQUIRE(testString.size() == 14);
  }

  SECTION("Replace with string-like object") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, std::string("Universe"));

    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
    REQUIRE(testString.size() == 14);
  }

  SECTION("Replace with C string") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, "Universe");

    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
    REQUIRE(testString.size() == 14);
  }

  SECTION("Replace with repeated characters") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, '*', 3);

    REQUIRE(std::strcmp(testString.c_str(), "Hello ***") == 0);
    REQUIRE(testString.size() == 9);
  }

  SECTION("Replace at beginning") {
    FixString<32> testString("Hello World");

    testString.replace(0, 5, "Hi");

    REQUIRE(std::strcmp(testString.c_str(), "Hi World") == 0);
    REQUIRE(testString.size() == 8);
  }

  SECTION("Replace at end") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, "Universe!");

    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe!") == 0);
    REQUIRE(testString.size() == 15);
  }

  SECTION("Replace with empty string") {
    FixString<32> testString("Hello World");

    testString.replace(5, 1, "");

    REQUIRE(std::strcmp(testString.c_str(), "HelloWorld") == 0);
    REQUIRE(testString.size() == 10);
  }

  SECTION("Replace with longer string") {
    FixString<32> testString("Hi");

    testString.replace(0, 2, "Hello World");

    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);
    REQUIRE(testString.size() == 11);
  }

  SECTION("Replace with shorter string") {
    FixString<32> testString("Hello World");

    testString.replace(0, 5, "Hi");

    REQUIRE(std::strcmp(testString.c_str(), "Hi World") == 0);
    REQUIRE(testString.size() == 8);
  }

  SECTION("Replace single character") {
    FixString<32> testString("Hello World");

    testString.replace(0, 1, "h");

    REQUIRE(std::strcmp(testString.c_str(), "hello World") == 0);
    REQUIRE(testString.size() == 11);
  }

  SECTION("Replace multiple characters with single character") {
    FixString<32> testString("Hello World");

    testString.replace(0, 5, "H");

    REQUIRE(std::strcmp(testString.c_str(), "H World") == 0);
    REQUIRE(testString.size() == 7);
  }

  SECTION("Replace with repeated characters at different positions") {
    FixString<32> testString("Hello World");

    testString.replace(0, 1, '*', 2);
    testString.replace(8, 2, '#', 3);

    REQUIRE(std::strcmp(testString.c_str(), "**ello W###ld") == 0);
    REQUIRE(testString.size() == 13);
  }

  SECTION("Replace entire string") {
    FixString<32> testString("Hello World");

    testString.replace(0, 11, "Goodbye Universe");

    REQUIRE(std::strcmp(testString.c_str(), "Goodbye Universe") == 0);
    REQUIRE(testString.size() == 16);
  }

  SECTION("Replace with zero count") {
    FixString<32> testString("Hello World");

    testString.replace(6, 0, "Beautiful ");

    REQUIRE(std::strcmp(testString.c_str(), "Hello Beautiful World") == 0);
    REQUIRE(testString.size() == 21);
  }

  SECTION("Replace with single character count") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, '!', 1);

    REQUIRE(std::strcmp(testString.c_str(), "Hello !") == 0);
    REQUIRE(testString.size() == 7);
  }

  SECTION("Replace with multiple character count") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, '=', 4);

    REQUIRE(std::strcmp(testString.c_str(), "Hello ====") == 0);
    REQUIRE(testString.size() == 10);
  }

  SECTION("Replace with zero character count") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, 'X', 0);

    REQUIRE(std::strcmp(testString.c_str(), "Hello ") == 0);
    REQUIRE(testString.size() == 6);
  }

  SECTION("Replace middle portion") {
    FixString<32> testString("Hello Beautiful World");

    testString.replace(6, 9, "Amazing");

    REQUIRE(std::strcmp(testString.c_str(), "Hello Amazing World") == 0);
    REQUIRE(testString.size() == 19);
  }

  SECTION("Replace with same length string") {
    FixString<32> testString("Hello World");

    testString.replace(0, 5, "Greet");

    REQUIRE(std::strcmp(testString.c_str(), "Greet World") == 0);
    REQUIRE(testString.size() == 11);
  }

  SECTION("Replace with FixString of different capacity") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, FixString<12>("Universe"));

    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
    REQUIRE(testString.size() == 14);
  }

  SECTION("Replace with std::string") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, std::string("Universe"));

    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
    REQUIRE(testString.size() == 14);
  }

  SECTION("Replace with array") {
    FixString<32> testString("Hello World");
    constexpr std::array<char, 9> arr = {'U', 'n', 'i', 'v', 'e', 'r', 's', 'e', '\0'};

    testString.replace(6, 5, arr.data());

    REQUIRE(std::strcmp(testString.c_str(), "Hello Universe") == 0);
    REQUIRE(testString.size() == 14);
  }

  SECTION("Replace at position 0 with zero count") {
    FixString<32> testString("Hello World");

    testString.replace(0, 0, "Hi ");

    REQUIRE(std::strcmp(testString.c_str(), "Hi Hello World") == 0);
    REQUIRE(testString.size() == 14);
  }

  SECTION("Replace at end with zero count") {
    FixString<32> testString("Hello World");

    testString.replace(11, 0, "!");

    REQUIRE(std::strcmp(testString.c_str(), "Hello World!") == 0);
    REQUIRE(testString.size() == 12);
  }
}

TEST_CASE("FixString copy", "[core][fixstring]") {
  SECTION("Copy entire string") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 11) == 11);
    REQUIRE(strncmp(buffer, "Hello World", 11) == 0);
  }

  SECTION("Copy partial string from beginning") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 5) == 5);
    REQUIRE(strncmp(buffer, "Hello", 5) == 0);
  }

  SECTION("Copy partial string from middle") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 5, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with npos count") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, FixString<16>::npos) == 11);
    REQUIRE(strncmp(buffer, "Hello World", 11) == 0);
  }

  SECTION("Copy with count exceeding remaining characters") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 20, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy from position 0") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 5, 0) == 5);
    REQUIRE(strncmp(buffer, "Hello", 5) == 0);
  }

  SECTION("Copy single character") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 1, 6) == 1);
    REQUIRE(buffer[0] == 'W');
  }

  SECTION("Copy from end position") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 1, 10) == 1);
    REQUIRE(buffer[0] == 'd');
  }

  SECTION("Copy with zero count") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 0) == 0);
  }

  SECTION("Copy from empty string") {
    constexpr FixString<16> testString("");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 5) == 0);
  }

  SECTION("Copy to small buffer") {
    constexpr FixString<16> testString("Hello World");
    char buffer[3] = {0};

    REQUIRE(testString.copy(buffer, 2) == 2);
    REQUIRE(strncmp(buffer, "He", 2) == 0);
  }

  SECTION("Copy with position at end") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 5, 11) == 0);
  }

  SECTION("Copy with npos from middle") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, FixString<16>::npos, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with exact remaining characters") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 5, 6) == 5);
    REQUIRE(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with count larger than string size") {
    constexpr FixString<16> testString("Hi");
    char buffer[16] = {0};

    REQUIRE(testString.copy(buffer, 10) == 2);
    REQUIRE(strncmp(buffer, "Hi", 2) == 0);
  }
}

TEST_CASE("FixString swap", "[core][fixstring]") {
  SECTION("Swap two different strings") {
    FixString<16> string1("Hello");
    FixString<16> string2("World");

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "World") == 0);
    REQUIRE(string1.size() == 5);
    REQUIRE(std::strcmp(string2.c_str(), "Hello") == 0);
    REQUIRE(string2.size() == 5);
  }

  SECTION("Swap strings of different lengths") {
    FixString<32> string1("Short");
    FixString<32> string2("This is a much longer string");

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "This is a much longer string") == 0);
    REQUIRE(string1.size() == 28);
    REQUIRE(std::strcmp(string2.c_str(), "Short") == 0);
    REQUIRE(string2.size() == 5);
  }

  SECTION("Swap with empty string") {
    FixString<16> string1("Hello World");
    FixString<16> string2("");

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "") == 0);
    REQUIRE(string1.size() == 0);
    REQUIRE(std::strcmp(string2.c_str(), "Hello World") == 0);
    REQUIRE(string2.size() == 11);
  }

  SECTION("Swap two empty strings") {
    FixString<16> string1("");
    FixString<16> string2("");

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "") == 0);
    REQUIRE(string1.size() == 0);
    REQUIRE(std::strcmp(string2.c_str(), "") == 0);
    REQUIRE(string2.size() == 0);
  }

  SECTION("Self-swap (no-op)") {
    FixString<16> string1("Hello World");

    string1.swap(string1);

    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);
    REQUIRE(string1.size() == 11);
  }

  SECTION("Swap with single character strings") {
    FixString<8> string1("A");
    FixString<8> string2("B");

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "B") == 0);
    REQUIRE(string1.size() == 1);
    REQUIRE(std::strcmp(string2.c_str(), "A") == 0);
    REQUIRE(string2.size() == 1);
  }

  SECTION("Swap with maximum length strings") {
    FixString<16> string1("123456789012345");
    FixString<16> string2("ABCDEFGHIJKLMNO");

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "ABCDEFGHIJKLMNO") == 0);
    REQUIRE(string1.size() == 15);
    REQUIRE(std::strcmp(string2.c_str(), "123456789012345") == 0);
    REQUIRE(string2.size() == 15);
  }

  SECTION("Chained swap operations") {
    FixString<16> string1("First");
    FixString<16> string2("Second");
    FixString<16> string3("Third");

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
    FixString<20> string1("AAA");
    FixString<20> string2("BBB");

    string1.swap(string2);

    REQUIRE(std::strcmp(string1.c_str(), "BBB") == 0);
    REQUIRE(string1.size() == 3);
    REQUIRE(std::strcmp(string2.c_str(), "AAA") == 0);
    REQUIRE(string2.size() == 3);
  }
}

TEST_CASE("FixString find", "[core][fixstring]") {
  SECTION("Find FixString substring") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find(FixString<16>("World")) == 6);
    REQUIRE(testString.find(FixString<16>("Hello")) == 0);
    REQUIRE(testString.find(FixString<16>("lo Wo")) == 3);
    REQUIRE(testString.find(FixString<16>("xyz")) == FixString<32>::npos);
  }

  SECTION("Find StringLike substring") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find(std::string("World")) == 6);
    REQUIRE(testString.find(std::string("Hello")) == 0);
    REQUIRE(testString.find(std::string("lo Wo")) == 3);
    REQUIRE(testString.find(std::string("xyz")) == FixString<32>::npos);
  }

  SECTION("Find C string substring") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("lo Wo") == 3);
    REQUIRE(testString.find("xyz") == FixString<32>::npos);
  }

  SECTION("Find character") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find('H') == 0);
    REQUIRE(testString.find('l') == 2);
    REQUIRE(testString.find('o') == 4);
    REQUIRE(testString.find('W') == 6);
    REQUIRE(testString.find('d') == 10);
    REQUIRE(testString.find('x') == FixString<32>::npos);
  }

  SECTION("Find with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == 12);
    REQUIRE(testString.find("Hello", 13) == FixString<32>::npos);
    REQUIRE(testString.find('l', 0) == 2);
    REQUIRE(testString.find('l', 3) == 3);
    REQUIRE(testString.find('l', 4) == 9);
    REQUIRE(testString.find('l', 10) == 14);
  }

  SECTION("Find empty substring") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find(FixString<16>("")) == 0);
    REQUIRE(testString.find(std::string("")) == 0);
    REQUIRE(testString.find("") == 0);
    REQUIRE(testString.find("", 5) == 5);
    REQUIRE(testString.find("", 11) == FixString<32>::npos);
  }

  SECTION("Find in empty string") {
    constexpr FixString<32> testString("");

    REQUIRE(testString.find(FixString<16>("Hello")) == FixString<32>::npos);
    REQUIRE(testString.find(std::string("Hello")) == FixString<32>::npos);
    REQUIRE(testString.find("Hello") == FixString<32>::npos);
    REQUIRE(testString.find('H') == FixString<32>::npos);
    REQUIRE(testString.find("") == FixString<32>::npos);
  }

  SECTION("Find with position beyond string size") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.find("World", 10) == FixString<32>::npos);
    REQUIRE(testString.find('H', 10) == FixString<32>::npos);
    REQUIRE(testString.find("", 10) == FixString<32>::npos);
  }

  SECTION("Find substring at end") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("d") == 10);
    REQUIRE(testString.find("ld") == 9);
  }

  SECTION("Find substring at beginning") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("H") == 0);
    REQUIRE(testString.find("He") == 0);
  }

  SECTION("Find overlapping substrings") {
    constexpr FixString<32> testString("ababab");

    REQUIRE(testString.find("ab") == 0);
    REQUIRE(testString.find("ab", 1) == 2);
    REQUIRE(testString.find("ab", 3) == 4);
    REQUIRE(testString.find("ab", 5) == FixString<32>::npos);
  }

  SECTION("Find with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    REQUIRE(testString.find("aa") == 0);
    REQUIRE(testString.find("aa", 1) == 1);
    REQUIRE(testString.find("aa", 2) == 2);
    REQUIRE(testString.find("aa", 3) == 3);
    REQUIRE(testString.find("aa", 4) == FixString<32>::npos);
  }

  SECTION("Find case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find("hello") == FixString<32>::npos);
    REQUIRE(testString.find("WORLD") == FixString<32>::npos);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("World") == 6);
  }

  SECTION("Find with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find(FixString<8>("World")) == 6);
    REQUIRE(testString.find(FixString<16>("World")) == 6);
    REQUIRE(testString.find(FixString<64>("World")) == 6);
  }

  SECTION("Find with exact match") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == FixString<32>::npos);
  }

  SECTION("Find with single character string") {
    constexpr FixString<32> testString("A");

    REQUIRE(testString.find("A") == 0);
    REQUIRE(testString.find('A') == 0);
    REQUIRE(testString.find("B") == FixString<32>::npos);
    REQUIRE(testString.find('B') == FixString<32>::npos);
  }
}

TEST_CASE("FixString rfind", "[core][fixstring]") {
  SECTION("Rfind FixString substring") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind(FixString<16>("Hello")) == 12);
    REQUIRE(testString.rfind(FixString<16>("World")) == 6);
    REQUIRE(testString.rfind(FixString<16>("lo")) == 15);
    REQUIRE(testString.rfind(FixString<16>("xyz")) == FixString<32>::npos);
  }

  SECTION("Rfind StringLike substring") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind(std::string("Hello")) == 12);
    REQUIRE(testString.rfind(std::string("World")) == 6);
    REQUIRE(testString.rfind(std::string("lo")) == 15);
    REQUIRE(testString.rfind(std::string("xyz")) == FixString<32>::npos);
  }

  SECTION("Rfind C string substring") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("lo") == 15);
    REQUIRE(testString.rfind("xyz") == FixString<32>::npos);
  }

  SECTION("Rfind character") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind('H') == 12);
    REQUIRE(testString.rfind('l') == 15);
    REQUIRE(testString.rfind('o') == 16);
    REQUIRE(testString.rfind('W') == 6);
    REQUIRE(testString.rfind('d') == 10);
    REQUIRE(testString.rfind('x') == FixString<32>::npos);
  }

  SECTION("Rfind with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello", 12) == 12);
    REQUIRE(testString.rfind("Hello", 11) == 0);
    REQUIRE(testString.rfind("Hello", 0) == 0);
    REQUIRE(testString.rfind('l', 16) == 15);
    REQUIRE(testString.rfind('l', 13) == 9);
    REQUIRE(testString.rfind('l', 8) == 3);
    REQUIRE(testString.rfind('l', 2) == 2);
  }

  SECTION("Rfind empty substring") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.rfind(FixString<16>("")) == 11);
    REQUIRE(testString.rfind(std::string("")) == 11);
    REQUIRE(testString.rfind("") == 11);
    REQUIRE(testString.rfind("", 5) == 5);
    REQUIRE(testString.rfind("", 0) == 0);
  }

  SECTION("Rfind in empty string") {
    constexpr FixString<32> testString("");

    REQUIRE(testString.rfind(FixString<16>("Hello")) == FixString<32>::npos);
    REQUIRE(testString.rfind(std::string("Hello")) == FixString<32>::npos);
    REQUIRE(testString.rfind("Hello") == FixString<32>::npos);
    REQUIRE(testString.rfind('H') == FixString<32>::npos);
    REQUIRE(testString.rfind("") == 0);
  }

  SECTION("Rfind substring at end") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("d") == 10);
    REQUIRE(testString.rfind("ld") == 9);
  }

  SECTION("Rfind substring at beginning") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("H") == 12);
    REQUIRE(testString.rfind("He") == 12);
  }

  SECTION("Rfind overlapping substrings") {
    constexpr FixString<32> testString("ababab");

    REQUIRE(testString.rfind("ab") == 4);
    REQUIRE(testString.rfind("ab", 3) == 2);
    REQUIRE(testString.rfind("ab", 1) == 0);
    REQUIRE(testString.rfind("ab", 0) == 0);
  }

  SECTION("Rfind with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    REQUIRE(testString.rfind("aa") == 3);
    REQUIRE(testString.rfind("aa", 2) == 2);
    REQUIRE(testString.rfind("aa", 1) == 1);
    REQUIRE(testString.rfind("aa", 0) == 0);
  }

  SECTION("Rfind case sensitivity") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("hello") == FixString<32>::npos);
    REQUIRE(testString.rfind("WORLD") == FixString<32>::npos);
    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);
  }

  SECTION("Rfind with different FixString capacities") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind(FixString<8>("Hello")) == 12);
    REQUIRE(testString.rfind(FixString<16>("Hello")) == 12);
    REQUIRE(testString.rfind(FixString<64>("Hello")) == 12);
  }

  SECTION("Rfind with exact match") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.rfind("Hello") == 0);
    REQUIRE(testString.rfind("Hello", 0) == 0);
  }

  SECTION("Rfind with single character string") {
    constexpr FixString<32> testString("A");

    REQUIRE(testString.rfind("A") == 0);
    REQUIRE(testString.rfind('A') == 0);
    REQUIRE(testString.rfind("B") == FixString<32>::npos);
    REQUIRE(testString.rfind('B') == FixString<32>::npos);
  }

  SECTION("Rfind with position 0") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.rfind("Hello", 0) == 0);
    REQUIRE(testString.rfind("World", 0) == FixString<32>::npos);
    REQUIRE(testString.rfind('H', 0) == 0);
    REQUIRE(testString.rfind('W', 0) == FixString<32>::npos);
  }

  SECTION("Rfind with substring longer than string") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.rfind("Hello World") == FixString<32>::npos);
    REQUIRE(testString.rfind("Hello World", 10) == FixString<32>::npos);
  }

  SECTION("Rfind with multiple occurrences") {
    constexpr FixString<32> testString("abababab");

    REQUIRE(testString.rfind("ab") == 6);
    REQUIRE(testString.rfind("ab", 5) == 4);
    REQUIRE(testString.rfind("ab", 3) == 2);
    REQUIRE(testString.rfind("ab", 1) == 0);
  }

  SECTION("Rfind with position in middle") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello", 8) == 0);
    REQUIRE(testString.rfind("Hello", 12) == 12);
    REQUIRE(testString.rfind('l', 8) == 3);
    REQUIRE(testString.rfind('l', 15) == 15);
  }
}

TEST_CASE("FixString find_first_of", "[core][fixstring]") {
  SECTION("Find first of FixString characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(FixString<16>("aeiou")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_of(FixString<16>("H")) == 0);
    REQUIRE(testString.find_first_of(FixString<16>("d")) == 10);
    REQUIRE(testString.find_first_of(FixString<16>("xyz")) == FixString<32>::npos);
  }

  SECTION("Find first of StringLike characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(std::string("aeiou")) == 1);
    REQUIRE(testString.find_first_of(std::string("H")) == 0);
    REQUIRE(testString.find_first_of(std::string("d")) == 10);
    REQUIRE(testString.find_first_of(std::string("xyz")) == FixString<32>::npos);
  }

  SECTION("Find first of C string characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou") == 1);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("d") == 10);
    REQUIRE(testString.find_first_of("xyz") == FixString<32>::npos);
  }

  SECTION("Find first of single character") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of('H') == 0);
    REQUIRE(testString.find_first_of('e') == 1);
    REQUIRE(testString.find_first_of('l') == 2);
    REQUIRE(testString.find_first_of('o') == 4);
    REQUIRE(testString.find_first_of('W') == 6);
    REQUIRE(testString.find_first_of('d') == 10);
    REQUIRE(testString.find_first_of('x') == FixString<32>::npos);
  }

  SECTION("Find first of with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("aeiou", 2) == 4);
    REQUIRE(testString.find_first_of("aeiou", 5) == 7);
    REQUIRE(testString.find_first_of("aeiou", 8) == 13);
    REQUIRE(testString.find_first_of("aeiou", 14) == 16);
    REQUIRE(testString.find_first_of("aeiou", 17) == FixString<32>::npos);
  }

  SECTION("Find first of empty character set") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(FixString<16>("")) == FixString<32>::npos);
    REQUIRE(testString.find_first_of(std::string("")) == FixString<32>::npos);
    REQUIRE(testString.find_first_of("") == FixString<32>::npos);
  }

  SECTION("Find first of in empty string") {
    constexpr FixString<32> testString("");

    REQUIRE(testString.find_first_of(FixString<16>("aeiou")) == FixString<32>::npos);
    REQUIRE(testString.find_first_of(std::string("aeiou")) == FixString<32>::npos);
    REQUIRE(testString.find_first_of("aeiou") == FixString<32>::npos);
    REQUIRE(testString.find_first_of('a') == FixString<32>::npos);
  }

  SECTION("Find first of with position beyond string size") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.find_first_of("aeiou", 10) == FixString<32>::npos);
    REQUIRE(testString.find_first_of('a', 10) == FixString<32>::npos);
  }

  SECTION("Find first of with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    REQUIRE(testString.find_first_of("a") == 0);
    REQUIRE(testString.find_first_of("ab") == 0);
    REQUIRE(testString.find_first_of("b") == FixString<32>::npos);
  }

  SECTION("Find first of with multiple character sets") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("Hl") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_of("lo") == 2); // 'l' at position 2
    REQUIRE(testString.find_first_of("Wr") == 6); // 'W' at position 6
    REQUIRE(testString.find_first_of("dl") == 2); // 'l' at position 2
  }

  SECTION("Find first of case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("h") == FixString<32>::npos);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("w") == FixString<32>::npos);
    REQUIRE(testString.find_first_of("W") == 6);
  }

  SECTION("Find first of with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    REQUIRE(testString.find_first_of("!,") == 5); // ',' at position 5
    REQUIRE(testString.find_first_of("!") == 12); // '!' at position 12
    REQUIRE(testString.find_first_of(".,!") == 5); // ',' at position 5
  }

  SECTION("Find first of with numbers") {
    constexpr FixString<32> testString("Hello123World");

    REQUIRE(testString.find_first_of("0123456789") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("123") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("456") == FixString<32>::npos);
  }

  SECTION("Find first of with whitespace") {
    constexpr FixString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_first_of(" \t\n") == 5); // ' ' at position 5
    REQUIRE(testString.find_first_of("\t") == 11);
    REQUIRE(testString.find_first_of("\n") == 12);
  }

  SECTION("Find first of with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of(FixString<8>("aeiou")) == 1);
    REQUIRE(testString.find_first_of(FixString<16>("aeiou")) == 1);
    REQUIRE(testString.find_first_of(FixString<64>("aeiou")) == 1);
  }

  SECTION("Find first of with single character string") {
    constexpr FixString<32> testString("A");

    REQUIRE(testString.find_first_of("A") == 0);
    REQUIRE(testString.find_first_of('A') == 0);
    REQUIRE(testString.find_first_of("B") == FixString<32>::npos);
    REQUIRE(testString.find_first_of('B') == FixString<32>::npos);
  }

  SECTION("Find first of with position 0") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("H", 0) == 0);
    REQUIRE(testString.find_first_of("xyz", 0) == FixString<32>::npos);
  }

  SECTION("Find first of with all characters found") {
    constexpr FixString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_of("aeiou") == 0);
    REQUIRE(testString.find_first_of("xyz") == 23);
    REQUIRE(testString.find_first_of("z") == 25);
  }

  SECTION("Find first of with no characters found") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_of("0123456789") == FixString<32>::npos);
    REQUIRE(testString.find_first_of("!@#$%^&*()") == FixString<32>::npos);
    REQUIRE(testString.find_first_of("[]{}|\\:;\"'<>?/") == FixString<32>::npos);
  }
}

TEST_CASE("FixString find_first_not_of", "[core][fixstring]") {
  SECTION("Find first not of FixString characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(FixString<16>("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(FixString<16>("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(FixString<16>("Helo Wrd")) == FixString<32>::npos);
  }

  SECTION("Find first not of StringLike characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(std::string("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(std::string("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(std::string("Helo Wrd")) == FixString<32>::npos);
  }

  SECTION("Find first not of C string characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("H") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Helo Wrd") == FixString<32>::npos);
  }

  SECTION("Find first not of single character") {
    constexpr FixString<32> testString("aaaaab");

    REQUIRE(testString.find_first_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_first_not_of('b') == 0); // 'a' at position 0
    REQUIRE(testString.find_first_not_of('x') == 0); // 'a' at position 0
  }

  SECTION("Find first not of with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.find_first_not_of("Hel", 0) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 5) == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Hel", 6) == 6); // 'W' at position 6
    REQUIRE(testString.find_first_not_of("Hel", 7) == 7); // 'o' at position 7
  }

  SECTION("Find first not of empty character set") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(FixString<16>("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of(std::string("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("", 5) == 5); // ' ' at position 5
  }

  SECTION("Find first not of in empty string") {
    constexpr FixString<32> testString("");

    REQUIRE(testString.find_first_not_of(FixString<16>("aeiou")) == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of(std::string("aeiou")) == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("aeiou") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of('a') == FixString<32>::npos);
  }

  SECTION("Find first not of with position beyond string size") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.find_first_not_of("aeiou", 10) == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of('a', 10) == FixString<32>::npos);
  }

  SECTION("Find first not of with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    REQUIRE(testString.find_first_not_of("a") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("ab") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("b") == 0); // 'a' at position 0
  }

  SECTION("Find first not of with multiple character sets") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("Hl") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Helo") == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Helo ") == 6); // 'W' at position 6
  }

  SECTION("Find first not of case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("h") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("H") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("w") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("W") == 0); // 'H' at position 0
  }

  SECTION("Find first not of with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    REQUIRE(testString.find_first_not_of("Helo, Wrd!") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo, Wrd") == 12); // '!' at position 12
  }

  SECTION("Find first not of with numbers") {
    constexpr FixString<32> testString("Hello123World");

    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo123Wrd") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12
  }

  SECTION("Find first not of with whitespace") {
    constexpr FixString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_first_not_of(" \t\n") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo Wrd\t\n") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo Wrd") == 11); // '\t' at position 11
  }

  SECTION("Find first not of with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of(FixString<8>("H")) == 1);
    REQUIRE(testString.find_first_not_of(FixString<16>("H")) == 1);
    REQUIRE(testString.find_first_not_of(FixString<64>("H")) == 1);
  }

  SECTION("Find first not of with single character string") {
    constexpr FixString<32> testString("A");

    REQUIRE(testString.find_first_not_of("A") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of('A') == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("B") == 0); // 'A' at position 0
    REQUIRE(testString.find_first_not_of('B') == 0); // 'A' at position 0
  }

  SECTION("Find first not of with position 0") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("H", 0) == 1);
    REQUIRE(testString.find_first_not_of("Hel", 0) == 4);
    REQUIRE(testString.find_first_not_of("Helo Wrd", 0) == FixString<32>::npos);
  }

  SECTION("Find first not of with all characters excluded") {
    constexpr FixString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24); // 'y' at position 24
  }

  SECTION("Find first not of with no characters excluded") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_first_not_of("xyz") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("!@#$%^&*()") == 0); // 'H' at position 0
  }

  SECTION("Find first not of with mixed content") {
    constexpr FixString<32> testString("Hello123World");

    REQUIRE(testString.find_first_not_of("Helo123Wrd") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12
    REQUIRE(testString.find_first_not_of("Helo123Wd") == 10); // 'r' at position 10
  }

  SECTION("Find first not of with position in middle") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.find_first_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 5) == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Hel", 6) == 6); // 'W' at position 6
    REQUIRE(testString.find_first_not_of("Hel", 7) == 7); // 'o' at position 7
  }

  SECTION("Find first not of with exact match") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.find_first_not_of("Hello") == FixString<32>::npos);
    REQUIRE(testString.find_first_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
  }
}

TEST_CASE("FixString find_last_of", "[core][fixstring]") {
  SECTION("Find last of FixString characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_of(FixString<16>("aeiou")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of(FixString<16>("l")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of(FixString<16>("H")) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of(FixString<16>("d")) == 10); // 'd' at position 10
  }

  SECTION("Find last of StringLike characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_of(std::string("aeiou")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of(std::string("l")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of(std::string("H")) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of(std::string("d")) == 10); // 'd' at position 10
  }

  SECTION("Find last of C string characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("aeiou") == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of("l") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("H") == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("d") == 10); // 'd' at position 10
  }

  SECTION("Find last of single character") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_of('l') == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of('o') == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of('H') == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of('d') == 10); // 'd' at position 10
  }

  SECTION("Find last of with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    REQUIRE(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1
    REQUIRE(testString.find_last_of("Hel", 0) == 0); // 'H' at position 0
  }

  SECTION("Find last of empty character set") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_of(FixString<16>("")) == FixString<32>::npos);
    REQUIRE(testString.find_last_of(std::string("")) == FixString<32>::npos);
    REQUIRE(testString.find_last_of("") == FixString<32>::npos);
  }

  SECTION("Find last of with no characters found") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("0123456789") == FixString<32>::npos);
    REQUIRE(testString.find_last_of("!@#$%^&*()") == FixString<32>::npos);
    REQUIRE(testString.find_last_of("[]{}|\\:;\"'<>?/") == FixString<32>::npos);
  }

  SECTION("Find last of with multiple character sets") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("Hl") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Hel") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo ") == 9); // 'l' at position 9
  }

  SECTION("Find last of case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("h") == FixString<32>::npos);
    REQUIRE(testString.find_last_of("H") == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("w") == FixString<32>::npos);
    REQUIRE(testString.find_last_of("W") == 6); // 'W' at position 6
  }

  SECTION("Find last of with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    REQUIRE(testString.find_last_of("Helo, Wrd!") == 12); // '!' at position 12
    REQUIRE(testString.find_last_of("Helo, Wrd") == 11); // 'd' at position 11
    REQUIRE(testString.find_last_of("Helo, Wr") == 10); // 'l' at position 10
  }

  SECTION("Find last of with numbers") {
    constexpr FixString<32> testString("Hello123World");

    REQUIRE(testString.find_last_of("0123456789") == 7); // '3' at position 7
    REQUIRE(testString.find_last_of("Helo123Wrd") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_of("Helo123Wr") == 11); // 'l' at position 11
  }

  SECTION("Find last of with whitespace") {
    constexpr FixString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_last_of(" \t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd\t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10
  }

  SECTION("Find last of with repeated characters") {
    constexpr FixString<32> testString("aaaaab");

    REQUIRE(testString.find_last_of('a') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_of('b') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_of('x') == FixString<32>::npos);
  }

  SECTION("Find last of with single character string") {
    constexpr FixString<32> testString("AAAAA");

    REQUIRE(testString.find_last_of("A") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of('A') == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of("B") == FixString<32>::npos);
    REQUIRE(testString.find_last_of('B') == FixString<32>::npos);
  }

  SECTION("Find last of with alphabet") {
    constexpr FixString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24); // 'y' at position 24
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23); // 'x' at position 23
  }

  SECTION("Find last of with all characters found") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_of("Helo Wr") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo W") == 9); // 'l' at position 9
  }

  SECTION("Find last of with position in middle") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    REQUIRE(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1
  }

  SECTION("Find last of with exact match") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.find_last_of("Hello") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_of("Hell") == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel") == 3); // 'l' at position 3
  }
}

TEST_CASE("FixString find_last_not_of", "[core][fixstring]") {
  SECTION("Find last not of FixString characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of(FixString<16>("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(FixString<16>("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(FixString<16>("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(FixString<16>("World")) == 5); // ' ' at position 5
  }

  SECTION("Find last not of StringLike characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of(std::string("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(std::string("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(std::string("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(std::string("World")) == 5); // ' ' at position 5
  }

  SECTION("Find last not of C string characters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("d") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of("ld") == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("rld") == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of("World") == 5); // ' ' at position 5
  }

  SECTION("Find last not of single character") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of('d') == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of('l') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('o') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('H') == 10); // 'd' at position 10
  }

  SECTION("Find last not of with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 0) == FixString<32>::npos);
  }

  SECTION("Find last not of empty character set") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of(FixString<16>("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of(std::string("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("", 5) == 5); // ' ' at position 5
  }

  SECTION("Find last not of with all characters excluded") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("Helo Wrd") == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo Wr") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo W") == 10); // 'd' at position 10
  }

  SECTION("Find last not of with multiple character sets") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("Hl") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Hel") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo ") == 10); // 'd' at position 10
  }

  SECTION("Find last not of case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("h") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("H") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("w") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("W") == 10); // 'd' at position 10
  }

  SECTION("Find last not of with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    REQUIRE(testString.find_last_not_of("Helo, Wrd!") == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo, Wrd") == 12); // '!' at position 12
    REQUIRE(testString.find_last_not_of("Helo, Wr") == 12); // '!' at position 12
  }

  SECTION("Find last not of with numbers") {
    constexpr FixString<32> testString("Hello123World");

    REQUIRE(testString.find_last_not_of("0123456789") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_not_of("Helo123Wrd") == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo123Wr") == 12); // 'd' at position 12
  }

  SECTION("Find last not of with whitespace") {
    constexpr FixString<32> testString("Hello World\t\n");

    REQUIRE(testString.find_last_not_of(" \t\n") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo Wrd\t\n") == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("Helo Wrd") == 12); // '\n' at position 12
  }

  SECTION("Find last not of with repeated characters") {
    constexpr FixString<32> testString("aaaaab");

    REQUIRE(testString.find_last_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_not_of('b') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_not_of('x') == 5); // 'b' at position 5
  }

  SECTION("Find last not of with single character string") {
    constexpr FixString<32> testString("AAAAA");

    REQUIRE(testString.find_last_not_of("A") == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of('A') == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("B") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_not_of('B') == 4); // 'A' at position 4
  }

  SECTION("Find last not of with alphabet") {
    constexpr FixString<32> testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxyz") == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwx") == 25); // 'z' at position 25
  }

  SECTION("Find last not of with no characters excluded") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.find_last_not_of("xyz") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("0123456789") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("!@#$%^&*()") == 10); // 'd' at position 10
  }

  SECTION("Find last not of with position in middle") {
    constexpr FixString<32> testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == FixString<32>::npos);
  }

  SECTION("Find last not of with exact match") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.find_last_not_of("Hello") == FixString<32>::npos);
    REQUIRE(testString.find_last_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel") == 4); // 'o' at position 4
  }
}

TEST_CASE("FixString compare", "[core][fixstring]") {
  SECTION("Compare FixString with FixString") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hello");
    constexpr FixString<32> testString3("World");
    constexpr FixString<32> testString4("Hell");

    REQUIRE(testString1.compare(testString2) == 0);
    REQUIRE(testString1.compare(testString3) < 0);
    REQUIRE(testString1.compare(testString4) > 0);
  }

  SECTION("Compare FixString with StringLike") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.compare(std::string("Hello")) == 0);
    REQUIRE(testString.compare(std::string("World")) < 0);
    REQUIRE(testString.compare(std::string("Hell")) > 0);
  }

  SECTION("Compare FixString with C string") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.compare("Hello") == 0);
    REQUIRE(testString.compare("World") < 0);
    REQUIRE(testString.compare("Hell") > 0);
  }

  SECTION("Compare identical strings") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.compare(FixString<32>("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare("Hello World") == 0);
  }

  SECTION("Compare with empty strings") {
    constexpr FixString<32> testString1("");
    constexpr FixString<32> testString2("Hello");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("") == 0);
    REQUIRE(testString1.compare(std::string("")) == 0);
  }

  SECTION("Compare strings with different lengths") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hello World");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
  }

  SECTION("Compare strings with same prefix") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hell");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
  }

  SECTION("Compare strings with different first character") {
    constexpr FixString<32> testString1("Apple");
    constexpr FixString<32> testString2("Banana");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
  }

  SECTION("Compare strings with different middle character") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hallo");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
  }

  SECTION("Compare strings with different last character") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hellp");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
  }

  SECTION("Compare case sensitivity") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.compare("hello") < 0);
    REQUIRE(testString.compare("HELLO") > 0);
    REQUIRE(testString.compare("Hello") == 0);
  }

  SECTION("Compare with different FixString capacities") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.compare(FixString<8>("Hello")) == 0);
    REQUIRE(testString.compare(FixString<16>("Hello")) == 0);
    REQUIRE(testString.compare(FixString<64>("Hello")) == 0);
    REQUIRE(testString.compare(FixString<8>("World")) < 0);
    REQUIRE(testString.compare(FixString<16>("Hell")) > 0);
  }

  SECTION("Compare with single character strings") {
    constexpr FixString<32> testString1("A");
    constexpr FixString<32> testString2("B");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("A") == 0);
    REQUIRE(testString1.compare("B") < 0);
  }

  SECTION("Compare with repeated characters") {
    constexpr FixString<32> testString1("aaa");
    constexpr FixString<32> testString2("aa");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("aaa") == 0);
    REQUIRE(testString1.compare("aa") > 0);
  }

  SECTION("Compare with special characters") {
    constexpr FixString<32> testString1("Hello!");
    constexpr FixString<32> testString2("Hello");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("Hello!") == 0);
    REQUIRE(testString1.compare("Hello") > 0);
  }

  SECTION("Compare with numbers") {
    constexpr FixString<32> testString1("123");
    constexpr FixString<32> testString2("456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123") == 0);
    REQUIRE(testString1.compare("456") < 0);
  }

  SECTION("Compare with mixed content") {
    constexpr FixString<32> testString1("Hello123");
    constexpr FixString<32> testString2("Hello456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("Hello123") == 0);
    REQUIRE(testString1.compare("Hello456") < 0);
  }

  SECTION("Compare with maximum length strings") {
    constexpr FixString<16> testString1("123456789012345");
    constexpr FixString<16> testString2("123456789012346");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123456789012345") == 0);
    REQUIRE(testString1.compare("123456789012346") < 0);
  }

  SECTION("Compare with std::string") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello")) > 0);
    REQUIRE(testString.compare(std::string("World")) < 0);
  }

  SECTION("Compare with array") {
    constexpr FixString<32> testString("Hello");
    constexpr std::array<char, 6> arr = {'H', 'e', 'l', 'l', 'o', '\0'};

    REQUIRE(testString.compare(arr.data()) == 0);
    REQUIRE(testString.compare("Hello") == 0);
  }

  SECTION("Compare edge cases") {
    constexpr FixString<32> testString("Hello");

    // Compare with null-terminated string
    REQUIRE(testString.compare("Hello\0World") == 0);

    // Compare with string containing null character
    constexpr FixString<32> testStringWithNull("Hello\0World");
    REQUIRE(testString.compare(testStringWithNull) == 0);
  }
}

TEST_CASE("FixString starts_with", "[core][fixstring]") {
  SECTION("Starts with FixString") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.starts_with(FixString<16>("Hello")) == true);
    REQUIRE(testString.starts_with(FixString<16>("Hello World")) == true);
    REQUIRE(testString.starts_with(FixString<16>("H")) == true);
    REQUIRE(testString.starts_with(FixString<16>("World")) == false);
    REQUIRE(testString.starts_with(FixString<16>("xyz")) == false);
    REQUIRE(testString.starts_with(FixString<16>("")) == true);
  }

  SECTION("Starts with StringLike") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")) == true);
    REQUIRE(testString.starts_with(std::string("Hello World")) == true);
    REQUIRE(testString.starts_with(std::string("H")) == true);
    REQUIRE(testString.starts_with(std::string("World")) == false);
    REQUIRE(testString.starts_with(std::string("xyz")) == false);
    REQUIRE(testString.starts_with(std::string("")) == true);
  }

  SECTION("Starts with C string") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hello World") == true);
    REQUIRE(testString.starts_with("H") == true);
    REQUIRE(testString.starts_with("World") == false);
    REQUIRE(testString.starts_with("xyz") == false);
    REQUIRE(testString.starts_with("") == true);
  }

  SECTION("Starts with character") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.starts_with('H') == true);
    REQUIRE(testString.starts_with('h') == false);
    REQUIRE(testString.starts_with('W') == false);
    REQUIRE(testString.starts_with('x') == false);
  }

  SECTION("Starts with empty string") {
    constexpr FixString<32> testString("");

    REQUIRE(testString.starts_with(FixString<16>("Hello")) == false);
    REQUIRE(testString.starts_with(std::string("Hello")) == false);
    REQUIRE(testString.starts_with("Hello") == false);
    REQUIRE(testString.starts_with('H') == false);
    REQUIRE(testString.starts_with("") == true);
  }

  SECTION("Starts with single character string") {
    constexpr FixString<32> testString("A");

    REQUIRE(testString.starts_with("A") == true);
    REQUIRE(testString.starts_with('A') == true);
    REQUIRE(testString.starts_with("B") == false);
    REQUIRE(testString.starts_with('B') == false);
    REQUIRE(testString.starts_with("") == true);
  }

  SECTION("Starts with longer prefix") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.starts_with("Hello World") == false);
    REQUIRE(testString.starts_with("Hello Universe") == false);
    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hell") == true);
  }

  SECTION("Starts with case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.starts_with("hello") == false);
    REQUIRE(testString.starts_with("HELLO") == false);
    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with('h') == false);
    REQUIRE(testString.starts_with('H') == true);
  }

  SECTION("Starts with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.starts_with(FixString<8>("Hello")) == true);
    REQUIRE(testString.starts_with(FixString<16>("Hello")) == true);
    REQUIRE(testString.starts_with(FixString<64>("Hello")) == true);
    REQUIRE(testString.starts_with(FixString<8>("World")) == false);
  }

  SECTION("Starts with repeated characters") {
    constexpr FixString<32> testString("aaaab");

    REQUIRE(testString.starts_with("aaa") == true);
    REQUIRE(testString.starts_with("aaaa") == true);
    REQUIRE(testString.starts_with("aaaab") == true);
    REQUIRE(testString.starts_with("aaab") == false);
    REQUIRE(testString.starts_with('a') == true);
    REQUIRE(testString.starts_with('b') == false);
  }

  SECTION("Starts with special characters") {
    constexpr FixString<32> testString("!@#$%");

    REQUIRE(testString.starts_with("!@#") == true);
    REQUIRE(testString.starts_with("!@#$%") == true);
    REQUIRE(testString.starts_with("!@#$%^") == false);
    REQUIRE(testString.starts_with('!') == true);
    REQUIRE(testString.starts_with('@') == false);
  }

  SECTION("Starts with numbers") {
    constexpr FixString<32> testString("12345");

    REQUIRE(testString.starts_with("123") == true);
    REQUIRE(testString.starts_with("12345") == true);
    REQUIRE(testString.starts_with("123456") == false);
    REQUIRE(testString.starts_with('1') == true);
    REQUIRE(testString.starts_with('2') == false);
  }

  SECTION("Starts with mixed content") {
    constexpr FixString<32> testString("Hello123");

    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hello1") == true);
    REQUIRE(testString.starts_with("Hello123") == true);
    REQUIRE(testString.starts_with("Hello1234") == false);
    REQUIRE(testString.starts_with('H') == true);
    REQUIRE(testString.starts_with('1') == false);
  }

  SECTION("Starts with maximum length strings") {
    constexpr FixString<16> testString("123456789012345");

    REQUIRE(testString.starts_with("123456789012345") == true);
    REQUIRE(testString.starts_with("12345678901234") == true);
    REQUIRE(testString.starts_with("1234567890123456") == false);
    REQUIRE(testString.starts_with('1') == true);
    REQUIRE(testString.starts_with('5') == false);
  }

  SECTION("Starts with std::string") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")) == true);
    REQUIRE(testString.starts_with(std::string("Hello World")) == true);
    REQUIRE(testString.starts_with(std::string("World")) == false);
  }

  SECTION("Starts with array") {
    constexpr FixString<32> testString("Hello");
    constexpr std::array<char, 4> arr = {'H', 'e', 'l', '\0'};

    REQUIRE(testString.starts_with(arr.data()) == true);
    REQUIRE(testString.starts_with("Hel") == true);
  }

  SECTION("Starts with edge cases") {
    constexpr FixString<32> testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.starts_with("Hello\0World") == true);

    // Test with string containing null character
    constexpr FixString<32> testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.starts_with("Hello") == true);
  }

  SECTION("Starts with whitespace") {
    constexpr FixString<32> testString(" Hello World");

    REQUIRE(testString.starts_with(" ") == true);
    REQUIRE(testString.starts_with(" Hello") == true);
    REQUIRE(testString.starts_with("Hello") == false);
    REQUIRE(testString.starts_with(' ') == true);
    REQUIRE(testString.starts_with('H') == false);
  }

  SECTION("Starts with exact match") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hell") == true);
    REQUIRE(testString.starts_with("H") == true);
    REQUIRE(testString.starts_with("") == true);
  }
}

TEST_CASE("FixString ends_with", "[core][fixstring]") {
  SECTION("Ends with FixString") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.ends_with(FixString<16>("World")) == true);
    REQUIRE(testString.ends_with(FixString<16>("Hello World")) == true);
    REQUIRE(testString.ends_with(FixString<16>("d")) == true);
    REQUIRE(testString.ends_with(FixString<16>("Hello")) == false);
    REQUIRE(testString.ends_with(FixString<16>("xyz")) == false);
    REQUIRE(testString.ends_with(FixString<16>("")) == true);
  }

  SECTION("Ends with StringLike") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.ends_with(std::string("World")) == true);
    REQUIRE(testString.ends_with(std::string("Hello World")) == true);
    REQUIRE(testString.ends_with(std::string("d")) == true);
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("xyz")) == false);
    REQUIRE(testString.ends_with(std::string("")) == true);
  }

  SECTION("Ends with C string") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.ends_with("World") == true);
    REQUIRE(testString.ends_with("Hello World") == true);
    REQUIRE(testString.ends_with("d") == true);
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with("xyz") == false);
    REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Ends with character") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.ends_with('d') == true);
    REQUIRE(testString.ends_with('D') == false);
    REQUIRE(testString.ends_with('H') == false);
    REQUIRE(testString.ends_with('x') == false);
  }

  SECTION("Ends with empty string") {
    constexpr FixString<32> testString("");

    REQUIRE(testString.ends_with(FixString<16>("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with('H') == false);
    REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Ends with single character string") {
    constexpr FixString<32> testString("A");

    REQUIRE(testString.ends_with("A") == true);
    REQUIRE(testString.ends_with('A') == true);
    REQUIRE(testString.ends_with("B") == false);
    REQUIRE(testString.ends_with('B') == false);
    REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Ends with longer suffix") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.ends_with("Hello World") == false);
    REQUIRE(testString.ends_with("Hello Universe") == false);
    REQUIRE(testString.ends_with("Hello") == true);
    REQUIRE(testString.ends_with("llo") == true);
  }

  SECTION("Ends with case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.ends_with("world") == false);
    REQUIRE(testString.ends_with("WORLD") == false);
    REQUIRE(testString.ends_with("World") == true);
    REQUIRE(testString.ends_with('d') == true);
    REQUIRE(testString.ends_with('D') == false);
  }

  SECTION("Ends with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.ends_with(FixString<8>("World")) == true);
    REQUIRE(testString.ends_with(FixString<16>("World")) == true);
    REQUIRE(testString.ends_with(FixString<64>("World")) == true);
    REQUIRE(testString.ends_with(FixString<8>("Hello")) == false);
  }

  SECTION("Ends with repeated characters") {
    constexpr FixString<32> testString("baaaa");

    REQUIRE(testString.ends_with("aaa") == true);
    REQUIRE(testString.ends_with("aaaa") == true);
    REQUIRE(testString.ends_with("baaaa") == true);
    REQUIRE(testString.ends_with("aaaaa") == false);
    REQUIRE(testString.ends_with('a') == true);
    REQUIRE(testString.ends_with('b') == false);
  }

  SECTION("Ends with special characters") {
    constexpr FixString<32> testString("%$#@!");

    REQUIRE(testString.ends_with("@!") == true);
    REQUIRE(testString.ends_with("%$#@!") == true);
    REQUIRE(testString.ends_with("^%$#@!") == false);
    REQUIRE(testString.ends_with('!') == true);
    REQUIRE(testString.ends_with('@') == false);
  }

  SECTION("Ends with numbers") {
    constexpr FixString<32> testString("54321");

    REQUIRE(testString.ends_with("321") == true);
    REQUIRE(testString.ends_with("54321") == true);
    REQUIRE(testString.ends_with("654321") == false);
    REQUIRE(testString.ends_with('1') == true);
    REQUIRE(testString.ends_with('2') == false);
  }

  SECTION("Ends with mixed content") {
    constexpr FixString<32> testString("123Hello");

    REQUIRE(testString.ends_with("Hello") == true);
    REQUIRE(testString.ends_with("3Hello") == true);
    REQUIRE(testString.ends_with("123Hello") == true);
    REQUIRE(testString.ends_with("0123Hello") == false);
    REQUIRE(testString.ends_with('o') == true);
    REQUIRE(testString.ends_with('1') == false);
  }

  SECTION("Ends with maximum length strings") {
    constexpr FixString<16> testString("123456789012345");

    REQUIRE(testString.ends_with("123456789012345") == true);
    REQUIRE(testString.ends_with("23456789012345") == true);
    REQUIRE(testString.ends_with("0123456789012345") == false);
    REQUIRE(testString.ends_with('5') == true);
    REQUIRE(testString.ends_with('1') == false);
  }

  SECTION("Ends with std::string") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.ends_with(std::string("World")) == true);
    REQUIRE(testString.ends_with(std::string("Hello World")) == true);
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
  }

  SECTION("Ends with array") {
    constexpr FixString<32> testString("Hello");
    constexpr std::array<char, 4> arr = {'l', 'l', 'o', '\0'};

    REQUIRE(testString.ends_with(arr.data()) == true);
    REQUIRE(testString.ends_with("llo") == true);
  }

  SECTION("Ends with edge cases") {
    constexpr FixString<32> testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.ends_with("Hello\0World") == true);

    // Test with string containing null character
    constexpr FixString<32> testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.ends_with("World") == false);
  }

  SECTION("Ends with whitespace") {
    constexpr FixString<32> testString("Hello World ");

    REQUIRE(testString.ends_with(" ") == true);
    REQUIRE(testString.ends_with("World ") == true);
    REQUIRE(testString.ends_with("World") == false);
    REQUIRE(testString.ends_with(' ') == true);
    REQUIRE(testString.ends_with('d') == false);
  }

  SECTION("Ends with exact match") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.ends_with("Hello") == true);
    REQUIRE(testString.ends_with("llo") == true);
    REQUIRE(testString.ends_with("o") == true);
    REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Ends with overlapping patterns") {
    constexpr FixString<32> testString("ababab");

    REQUIRE(testString.ends_with("ab") == true);
    REQUIRE(testString.ends_with("bab") == true);
    REQUIRE(testString.ends_with("abab") == true);
    REQUIRE(testString.ends_with("babab") == true);
    REQUIRE(testString.ends_with("ababab") == true);
    REQUIRE(testString.ends_with("bababab") == false);
  }

  SECTION("Ends with multiple occurrences") {
    constexpr FixString<32> testString("abababab");

    REQUIRE(testString.ends_with("ab") == true);
    REQUIRE(testString.ends_with("bab") == true);
    REQUIRE(testString.ends_with("abab") == true);
    REQUIRE(testString.ends_with("ababab") == true);
    REQUIRE(testString.ends_with("abababab") == true);
  }
}

TEST_CASE("FixString contains", "[core][fixstring]") {
  SECTION("Contains FixString") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains(FixString<16>("World")) == true);
    REQUIRE(testString.contains(FixString<16>("Hello")) == true);
    REQUIRE(testString.contains(FixString<16>("lo Wo")) == true);
    REQUIRE(testString.contains(FixString<16>("Hello World")) == true);
    REQUIRE(testString.contains(FixString<16>("xyz")) == false);
    REQUIRE(testString.contains(FixString<16>("")) == true);
  }

  SECTION("Contains StringLike") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains(std::string("World")) == true);
    REQUIRE(testString.contains(std::string("Hello")) == true);
    REQUIRE(testString.contains(std::string("lo Wo")) == true);
    REQUIRE(testString.contains(std::string("Hello World")) == true);
    REQUIRE(testString.contains(std::string("xyz")) == false);
    REQUIRE(testString.contains(std::string("")) == true);
  }

  SECTION("Contains C string") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains("World") == true);
    REQUIRE(testString.contains("Hello") == true);
    REQUIRE(testString.contains("lo Wo") == true);
    REQUIRE(testString.contains("Hello World") == true);
    REQUIRE(testString.contains("xyz") == false);
    REQUIRE(testString.contains("") == true);
  }

  SECTION("Contains character") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains('H') == true);
    REQUIRE(testString.contains('o') == true);
    REQUIRE(testString.contains('l') == true);
    REQUIRE(testString.contains(' ') == true);
    REQUIRE(testString.contains('x') == false);
    REQUIRE(testString.contains('Z') == false);
  }

  SECTION("Contains in empty string") {
    constexpr FixString<32> testString("");

    REQUIRE(testString.contains(FixString<16>("Hello")) == false);
    REQUIRE(testString.contains(std::string("Hello")) == false);
    REQUIRE(testString.contains("Hello") == false);
    REQUIRE(testString.contains('H') == false);
    REQUIRE(testString.contains("") == true);
  }

  SECTION("Contains in single character string") {
    constexpr FixString<32> testString("A");

    REQUIRE(testString.contains("A") == true);
    REQUIRE(testString.contains('A') == true);
    REQUIRE(testString.contains("B") == false);
    REQUIRE(testString.contains('B') == false);
    REQUIRE(testString.contains("") == true);
  }

  SECTION("Contains longer substring") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.contains("Hello World") == false);
    REQUIRE(testString.contains("Hello Universe") == false);
    REQUIRE(testString.contains("Hello") == true);
    REQUIRE(testString.contains("llo") == true);
    REQUIRE(testString.contains("ell") == true);
  }

  SECTION("Contains case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains("world") == false);
    REQUIRE(testString.contains("WORLD") == false);
    REQUIRE(testString.contains("World") == true);
    REQUIRE(testString.contains('h') == false);
    REQUIRE(testString.contains('H') == true);
  }

  SECTION("Contains different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains(FixString<8>("World")) == true);
    REQUIRE(testString.contains(FixString<16>("World")) == true);
    REQUIRE(testString.contains(FixString<64>("World")) == true);
    REQUIRE(testString.contains(FixString<8>("Hello")) == true);
  }

  SECTION("Contains repeated characters") {
    constexpr FixString<32> testString("baaaa");

    REQUIRE(testString.contains("aaa") == true);
    REQUIRE(testString.contains("aaaa") == true);
    REQUIRE(testString.contains("baaaa") == true);
    REQUIRE(testString.contains("aaaaa") == false);
    REQUIRE(testString.contains('a') == true);
    REQUIRE(testString.contains('b') == true);
  }

  SECTION("Contains special characters") {
    constexpr FixString<32> testString("%$#@!");

    REQUIRE(testString.contains("@!") == true);
    REQUIRE(testString.contains("%$#@!") == true);
    REQUIRE(testString.contains("^%$#@!") == false);
    REQUIRE(testString.contains('!') == true);
    REQUIRE(testString.contains('@') == true);
    REQUIRE(testString.contains('$') == true);
  }

  SECTION("Contains numbers") {
    constexpr FixString<32> testString("54321");

    REQUIRE(testString.contains("321") == true);
    REQUIRE(testString.contains("54321") == true);
    REQUIRE(testString.contains("654321") == false);
    REQUIRE(testString.contains('1') == true);
    REQUIRE(testString.contains('2') == true);
    REQUIRE(testString.contains('5') == true);
  }

  SECTION("Contains mixed content") {
    constexpr FixString<32> testString("123Hello");

    REQUIRE(testString.contains("Hello") == true);
    REQUIRE(testString.contains("3Hello") == true);
    REQUIRE(testString.contains("123Hello") == true);
    REQUIRE(testString.contains("0123Hello") == false);
    REQUIRE(testString.contains('o') == true);
    REQUIRE(testString.contains('1') == true);
    REQUIRE(testString.contains('3') == true);
  }

  SECTION("Contains maximum length strings") {
    constexpr FixString<16> testString("123456789012345"); // 15 characters

    REQUIRE(testString.contains("123456789012345") == true);
    REQUIRE(testString.contains("23456789012345") == true);
    REQUIRE(testString.contains("0123456789012345") == false);
    REQUIRE(testString.contains('5') == true);
    REQUIRE(testString.contains('1') == true);
  }

  SECTION("Contains std::string") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains(std::string("World")) == true);
    REQUIRE(testString.contains(std::string("Hello World")) == true);
    REQUIRE(testString.contains(std::string("Hello")) == true);
  }

  SECTION("Contains array") {
    constexpr FixString<32> testString("Hello");
    constexpr std::array<char, 4> arr = {'l', 'l', 'o', '\0'};

    REQUIRE(testString.contains(arr.data()) == true);
    REQUIRE(testString.contains("llo") == true);
  }

  SECTION("Contains whitespace") {
    constexpr FixString<32> testString("Hello World ");

    REQUIRE(testString.contains(" ") == true);
    REQUIRE(testString.contains("World ") == true);
    REQUIRE(testString.contains("World") == true);
    REQUIRE(testString.contains(' ') == true);
    REQUIRE(testString.contains('d') == true);
  }

  SECTION("Contains exact match") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(testString.contains("Hello") == true);
    REQUIRE(testString.contains("llo") == true);
    REQUIRE(testString.contains("o") == true);
    REQUIRE(testString.contains("") == true);
  }

  SECTION("Contains overlapping patterns") {
    constexpr FixString<32> testString("ababab");

    REQUIRE(testString.contains("ab") == true);
    REQUIRE(testString.contains("bab") == true);
    REQUIRE(testString.contains("abab") == true);
    REQUIRE(testString.contains("ababab") == true);
    REQUIRE(testString.contains("babab") == true);
  }

  SECTION("Contains multiple occurrences") {
    constexpr FixString<32> testString("abababab");

    REQUIRE(testString.contains("ab") == true);
    REQUIRE(testString.contains("bab") == true);
    REQUIRE(testString.contains("abab") == true);
    REQUIRE(testString.contains("ababab") == true);
    REQUIRE(testString.contains("abababab") == true);
  }

  SECTION("Contains at beginning") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains("H") == true);
    REQUIRE(testString.contains("He") == true);
    REQUIRE(testString.contains("Hel") == true);
    REQUIRE(testString.contains("Hello") == true);
  }

  SECTION("Contains at end") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains("d") == true);
    REQUIRE(testString.contains("ld") == true);
    REQUIRE(testString.contains("rld") == true);
    REQUIRE(testString.contains("World") == true);
  }

  SECTION("Contains in middle") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(testString.contains("l") == true);
    REQUIRE(testString.contains("ll") == true);
    REQUIRE(testString.contains("lo") == true);
    REQUIRE(testString.contains("lo W") == true);
  }

  SECTION("Contains single character multiple times") {
    constexpr FixString<32> testString("aaaaa");

    REQUIRE(testString.contains("a") == true);
    REQUIRE(testString.contains("aa") == true);
    REQUIRE(testString.contains("aaa") == true);
    REQUIRE(testString.contains("aaaa") == true);
    REQUIRE(testString.contains("aaaaa") == true);
    REQUIRE(testString.contains("aaaaaa") == false);
  }

  SECTION("Contains with punctuation") {
    constexpr FixString<32> testString("Hello, World!");

    REQUIRE(testString.contains(",") == true);
    REQUIRE(testString.contains("!") == true);
    REQUIRE(testString.contains(", ") == true);
    REQUIRE(testString.contains("World!") == true);
    REQUIRE(testString.contains("Hello,") == true);
  }

  SECTION("Contains with newlines") {
    constexpr FixString<32> testString("Hello\nWorld");

    REQUIRE(testString.contains("\n") == true);
    REQUIRE(testString.contains("Hello\n") == true);
    REQUIRE(testString.contains("\nWorld") == true);
    REQUIRE(testString.contains("Hello\nWorld") == true);
  }

  SECTION("Contains with tabs") {
    constexpr FixString<32> testString("Hello\tWorld");

    REQUIRE(testString.contains("\t") == true);
    REQUIRE(testString.contains("Hello\t") == true);
    REQUIRE(testString.contains("\tWorld") == true);
    REQUIRE(testString.contains("Hello\tWorld") == true);
  }
}

TEST_CASE("FixString substr", "[core][fixstring]") {
  SECTION("Substr basic functionality") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(std::strcmp(testString.substr(0).c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);
    REQUIRE(std::strcmp(testString.substr(6, 5).c_str(), "World") == 0);
    REQUIRE(std::strcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);
  }

  SECTION("Substr with default parameters") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(std::strcmp(testString.substr().c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(testString.substr(0).c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SECTION("Substr with count parameter") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "H") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 2).c_str(), "He") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 3).c_str(), "Hel") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 4).c_str(), "Hell") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(std::strcmp(testString.substr(6, 1).c_str(), "W") == 0);
    REQUIRE(std::strcmp(testString.substr(6, 2).c_str(), "Wo") == 0);
    REQUIRE(std::strcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    REQUIRE(std::strcmp(testString.substr(6, 4).c_str(), "Worl") == 0);
    REQUIRE(std::strcmp(testString.substr(6, 5).c_str(), "World") == 0);
  }

  SECTION("Substr with npos count") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(std::strcmp(testString.substr(0, FixString<32>::npos).c_str(), "Hello World") == 0);
    REQUIRE(std::strcmp(testString.substr(6, FixString<32>::npos).c_str(), "World") == 0);
    REQUIRE(std::strcmp(testString.substr(10, FixString<32>::npos).c_str(), "d") == 0);
  }

  SECTION("Substr with position at end") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(std::strcmp(testString.substr(11).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(11, 0).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(11, 5).c_str(), "") == 0);
  }

  SECTION("Substr from empty string") {
    constexpr FixString<32> testString("");

    REQUIRE(std::strcmp(testString.substr(0).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "") == 0);
  }

  SECTION("Substr single character") {
    constexpr FixString<32> testString("A");

    REQUIRE(std::strcmp(testString.substr(0).c_str(), "A") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "A") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(1).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(1, 1).c_str(), "") == 0);
  }

  SECTION("Substr with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    REQUIRE(std::strcmp(testString.substr(0).c_str(), "aaaaa") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "a") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 2).c_str(), "aa") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 3).c_str(), "aaa") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 4).c_str(), "aaaa") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "aaaaa") == 0);
    REQUIRE(std::strcmp(testString.substr(1).c_str(), "aaaa") == 0);
    REQUIRE(std::strcmp(testString.substr(2).c_str(), "aaa") == 0);
    REQUIRE(std::strcmp(testString.substr(3).c_str(), "aa") == 0);
    REQUIRE(std::strcmp(testString.substr(4).c_str(), "a") == 0);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "") == 0);
  }

  SECTION("Substr with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    REQUIRE(std::strcmp(testString.substr(5).c_str(), ", World!") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), ",") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), ", ") == 0);
    REQUIRE(std::strcmp(testString.substr(12).c_str(), "!") == 0);
    REQUIRE(std::strcmp(testString.substr(12, 1).c_str(), "!") == 0);
  }

  SECTION("Substr with numbers") {
    constexpr FixString<32> testString("12345");

    REQUIRE(std::strcmp(testString.substr(0).c_str(), "12345") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "1") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 2).c_str(), "12") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 3).c_str(), "123") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 4).c_str(), "1234") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "12345") == 0);
    REQUIRE(std::strcmp(testString.substr(1).c_str(), "2345") == 0);
    REQUIRE(std::strcmp(testString.substr(2).c_str(), "345") == 0);
    REQUIRE(std::strcmp(testString.substr(3).c_str(), "45") == 0);
    REQUIRE(std::strcmp(testString.substr(4).c_str(), "5") == 0);
    REQUIRE(std::strcmp(testString.substr(5).c_str(), "") == 0);
  }

  SECTION("Substr with whitespace") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(std::strcmp(testString.substr(5).c_str(), " World") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), " ") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), " W") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 3).c_str(), " Wo") == 0);
  }

  SECTION("Substr with newlines") {
    constexpr FixString<32> testString("Hello\nWorld");

    REQUIRE(std::strcmp(testString.substr(5).c_str(), "\nWorld") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "\n") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), "\nW") == 0);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SECTION("Substr with tabs") {
    constexpr FixString<32> testString("Hello\tWorld");

    REQUIRE(std::strcmp(testString.substr(5).c_str(), "\tWorld") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "\t") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 2).c_str(), "\tW") == 0);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SECTION("Substr maximum length") {
    constexpr FixString<16> testString("123456789012345"); // 15 characters

    REQUIRE(std::strcmp(testString.substr(0).c_str(), "123456789012345") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 15).c_str(), "123456789012345") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 16).c_str(), "123456789012345") == 0);
    REQUIRE(std::strcmp(testString.substr(14).c_str(), "5") == 0);
    REQUIRE(std::strcmp(testString.substr(14, 1).c_str(), "5") == 0);
    REQUIRE(std::strcmp(testString.substr(15).c_str(), "") == 0);
  }

  SECTION("Substr with mixed content") {
    constexpr FixString<32> testString("123Hello");

    REQUIRE(std::strcmp(testString.substr(0).c_str(), "123Hello") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 3).c_str(), "123") == 0);
    REQUIRE(std::strcmp(testString.substr(3).c_str(), "Hello") == 0);
    REQUIRE(std::strcmp(testString.substr(3, 5).c_str(), "Hello") == 0);
    REQUIRE(std::strcmp(testString.substr(2, 4).c_str(), "3Hel") == 0);
  }

  SECTION("Substr with overlapping ranges") {
    constexpr FixString<32> testString("abcdef");

    REQUIRE(std::strcmp(testString.substr(0, 1).c_str(), "a") == 0);
    REQUIRE(std::strcmp(testString.substr(1, 1).c_str(), "b") == 0);
    REQUIRE(std::strcmp(testString.substr(2, 1).c_str(), "c") == 0);
    REQUIRE(std::strcmp(testString.substr(3, 1).c_str(), "d") == 0);
    REQUIRE(std::strcmp(testString.substr(4, 1).c_str(), "e") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "f") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 2).c_str(), "ab") == 0);
    REQUIRE(std::strcmp(testString.substr(1, 2).c_str(), "bc") == 0);
    REQUIRE(std::strcmp(testString.substr(2, 2).c_str(), "cd") == 0);
    REQUIRE(std::strcmp(testString.substr(3, 2).c_str(), "de") == 0);
    REQUIRE(std::strcmp(testString.substr(4, 2).c_str(), "ef") == 0);
  }

  SECTION("Substr with exact string length") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(std::strcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(std::strcmp(testString.substr(0, 6).c_str(), "Hello") == 0);
    REQUIRE(std::strcmp(testString.substr(1, 4).c_str(), "ello") == 0);
    REQUIRE(std::strcmp(testString.substr(2, 3).c_str(), "llo") == 0);
    REQUIRE(std::strcmp(testString.substr(3, 2).c_str(), "lo") == 0);
    REQUIRE(std::strcmp(testString.substr(4, 1).c_str(), "o") == 0);
  }

  SECTION("Substr with zero count") {
    constexpr FixString<32> testString("Hello World");

    REQUIRE(std::strcmp(testString.substr(0, 0).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 0).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(10, 0).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(11, 0).c_str(), "") == 0);
  }

  SECTION("Substr with position at string size") {
    constexpr FixString<32> testString("Hello");

    REQUIRE(std::strcmp(testString.substr(5).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 0).c_str(), "") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), "") == 0);
  }

  SECTION("Substr with different FixString capacities") {
    constexpr FixString<8> smallString("Hello");
    constexpr FixString<16> mediumString("Hello World");
    constexpr FixString<32> largeString("Hello World Universe");

    REQUIRE(std::strcmp(smallString.substr(0, 3).c_str(), "Hel") == 0);
    REQUIRE(std::strcmp(mediumString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(std::strcmp(largeString.substr(0, 5).c_str(), "Hello") == 0);
    REQUIRE(std::strcmp(smallString.substr(2).c_str(), "llo") == 0);
    REQUIRE(std::strcmp(mediumString.substr(6).c_str(), "World") == 0);
    REQUIRE(std::strcmp(largeString.substr(12).c_str(), "Universe") == 0);
  }

  SECTION("Substr with punctuation") {
    constexpr FixString<32> testString("Hello, World!");

    REQUIRE(std::strcmp(testString.substr(5, 1).c_str(), ",") == 0);
    REQUIRE(std::strcmp(testString.substr(6, 1).c_str(), " ") == 0);
    REQUIRE(std::strcmp(testString.substr(12, 1).c_str(), "!") == 0);
    REQUIRE(std::strcmp(testString.substr(5, 3).c_str(), ", W") == 0);
    REQUIRE(std::strcmp(testString.substr(11, 2).c_str(), "d!") == 0);
  }

  SECTION("Substr with unicode-like content") {
    constexpr FixString<32> testString("Hello 世界");

    REQUIRE(std::strcmp(testString.substr(0, 6).c_str(), "Hello ") == 0);
    REQUIRE(std::strcmp(testString.substr(6).c_str(), "世界") == 0);
  }
}

TEST_CASE("FixString operators+", "[core][fixstring]") {
  const auto testString1 = FixString<14>("12") + "test text 1";
  const auto testString2 = FixString<14>("12") + FixString<14>("test text 2");
  const auto testString3 = FixString<20>("12") + FixString<14>("test text 3");
  const auto testString4 = FixString<20>("12") + FixString<26>("test text 4");
  const auto testString5 = FixString<4>("12") + 't';
  const auto testString6 = FixString<8>("a") + "b" + 'c';
  const auto testString7 = testString6 + testString6;

  REQUIRE(std::strcmp(testString1.c_str(), "12test text 1") == 0);
  REQUIRE(testString1.size() == 13);

  REQUIRE(std::strcmp(testString2.c_str(), "12test text 2") == 0);
  REQUIRE(testString2.size() == 13);

  REQUIRE(std::strcmp(testString3.c_str(), "12test text 3") == 0);
  REQUIRE(testString3.size() == 13);

  REQUIRE(std::strcmp(testString4.c_str(), "12test text 4") == 0);
  REQUIRE(testString4.size() == 13);

  REQUIRE(std::strcmp(testString5.c_str(), "12t") == 0);
  REQUIRE(testString5.size() == 3);

  REQUIRE(std::strcmp(testString6.c_str(), "abc") == 0);
  REQUIRE(testString6.size() == 3);

  REQUIRE(std::strcmp(testString7.c_str(), "abcabc") == 0);
  REQUIRE(testString7.size() == 6);
}

// after refactor

TEST_CASE("FixString operator==", "[core][fixstring]") {
  SECTION("FixString == FixString") {
    constexpr FixString<16> str1("Hello");
    constexpr FixString<32> str2("Hello");
    constexpr FixString<16> str3("World");
    constexpr FixString<8> str4("Hello");
    constexpr FixString<16> empty1;
    constexpr FixString<32> empty2;

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

  SECTION("FixString == StringLike") {
    constexpr FixString<16> str1("Hello");
    const std::string str2("Hello");
    const std::string str3("World");
    const std::string empty;

    REQUIRE(str1 == str2);
    REQUIRE(str2 == str1);
    REQUIRE_FALSE((str1 == str3));
    REQUIRE_FALSE((str3 == str1));
    REQUIRE_FALSE((str1 == empty));
    REQUIRE_FALSE((empty == str1));

    // std::string can't be a constant expression
  }

  SECTION("FixString == C string") {
    constexpr FixString<16> str1("Hello");
    constexpr FixString<16> empty;

    REQUIRE(str1 == "Hello");
    REQUIRE("Hello" == str1);
    REQUIRE_FALSE((str1 == "World"));
    REQUIRE_FALSE(("World" == str1));
    REQUIRE(empty == "");
    REQUIRE("" == empty);
    REQUIRE_FALSE((str1 == ""));
    REQUIRE_FALSE(("" == str1));

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
    constexpr FixString<16> str1("A");
    constexpr FixString<16> str2("B");
    constexpr FixString<16> empty1;
    constexpr FixString<32> empty2;

    // Single character comparison
    REQUIRE(str1 == "A");
    REQUIRE("A" == str1);
    REQUIRE_FALSE((str1 == "B"));
    REQUIRE_FALSE(("B" == str1));

    STATIC_REQUIRE(str1 == "A");
    STATIC_REQUIRE("A" == str1);
    STATIC_REQUIRE_FALSE((str1 == "B"));
    STATIC_REQUIRE_FALSE(("B" == str1));

    // Empty string comparisons
    REQUIRE(empty1 == empty2);
    REQUIRE(empty2 == empty1);
    REQUIRE(empty1 == "");
    REQUIRE("" == empty1);

    STATIC_REQUIRE(empty1 == empty2);
    STATIC_REQUIRE(empty2 == empty1);
    STATIC_REQUIRE(empty1 == "");
    STATIC_REQUIRE("" == empty1);

    // Different sizes with same content
    constexpr FixString<8> small("Hi");
    constexpr FixString<16> large("Hi");

    REQUIRE(small == large);
    REQUIRE(large == small);

    STATIC_REQUIRE(small == large);
    STATIC_REQUIRE(large == small);
  }

  SECTION("Special characters") {
    constexpr FixString<16> str1("Hello\nWorld");
    constexpr FixString<16> str2("Hello\tWorld");
    constexpr FixString<16> str3("Hello World");

    REQUIRE(str1 == "Hello\nWorld");
    REQUIRE("Hello\nWorld" == str1);
    REQUIRE(str2 == "Hello\tWorld");
    REQUIRE("Hello\tWorld" == str2);
    REQUIRE_FALSE((str1 == str2));
    REQUIRE_FALSE((str2 == str1));
    REQUIRE_FALSE((str1 == str3));
    REQUIRE_FALSE((str3 == str1));

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
    constexpr FixString<32> str1("Привет");
    constexpr FixString<32> str2("Мир");
    constexpr FixString<32> str3("Привет");

    REQUIRE(str1 == "Привет");
    REQUIRE("Привет" == str1);
    REQUIRE(str1 == str3);
    REQUIRE(str3 == str1);
    REQUIRE_FALSE((str1 == str2));
    REQUIRE_FALSE((str2 == str1));

    STATIC_REQUIRE(str1 == "Привет");
    STATIC_REQUIRE("Привет" == str1);
    STATIC_REQUIRE(str1 == str3);
    STATIC_REQUIRE(str3 == str1);
    STATIC_REQUIRE_FALSE((str1 == str2));
    STATIC_REQUIRE_FALSE((str2 == str1));
  }

  SECTION("Performance test") {
    constexpr FixString<64> str1("This is a longer string for performance testing");
    constexpr FixString<64> str2("This is a longer string for performance testing");
    constexpr FixString<64> str3("This is a different string for performance testing");

    REQUIRE(str1 == str2);
    REQUIRE(str2 == str1);
    REQUIRE_FALSE((str1 == str3));
    REQUIRE_FALSE((str3 == str1));

    STATIC_REQUIRE(str1 == str2);
    STATIC_REQUIRE(str2 == str1);
    STATIC_REQUIRE_FALSE((str1 == str3));
    STATIC_REQUIRE_FALSE((str3 == str1));
  }

  SECTION("Constexpr operations") {
    constexpr FixString<16> str1("Test");
    constexpr FixString<16> str2("Test");
    constexpr FixString<16> str3("Different");

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

    STATIC_REQUIRE(eq1);
    STATIC_REQUIRE_FALSE(eq2);
    STATIC_REQUIRE(eq3);
    STATIC_REQUIRE(eq4);
    STATIC_REQUIRE_FALSE(eq5);
    STATIC_REQUIRE_FALSE(eq6);
  }
}

TEST_CASE("FixString operator<=>", "[core][fixstring]") {
  SECTION("FixString <=> FixString") {
    constexpr FixString<16> str1("Hello");
    constexpr FixString<32> str2("Hello");
    constexpr FixString<16> str3("World");
    constexpr FixString<8> str4("Hello");
    constexpr FixString<16> str5("Hi");
    constexpr FixString<16> str6("Hell");

    // Equal strings
    REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) == std::strong_ordering::equal);
    REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::equal);
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    // Different strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str5) == std::strong_ordering::less);
    REQUIRE((str5 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str6) == std::strong_ordering::greater);
    REQUIRE((str6 <=> str1) == std::strong_ordering::less);

    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str5) == std::strong_ordering::less);
    STATIC_REQUIRE((str5 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str6) == std::strong_ordering::greater);
    STATIC_REQUIRE((str6 <=> str1) == std::strong_ordering::less);
  }

  SECTION("FixString <=> StringLike") {
    constexpr FixString<16> str1("Hello");
    const std::string str2("Hello");
    const std::string str3("World");

    // Equal strings
    REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    // Different strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
  }

  SECTION("FixString <=> C string") {
    constexpr FixString<16> str1("Hello");
    constexpr const char * str2 = "Hello";
    constexpr const char * str3 = "World";
    constexpr const char * str4 = "Hi";
    constexpr const char * str5 = "Hell";

    // Equal strings
    REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    // Different strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str4) == std::strong_ordering::less);
    REQUIRE((str4 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str5) == std::strong_ordering::greater);
    REQUIRE((str5 <=> str1) == std::strong_ordering::less);

    // Compile-time checks
    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::less);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str5) == std::strong_ordering::greater);
    STATIC_REQUIRE((str5 <=> str1) == std::strong_ordering::less);
  }

  SECTION("Empty string comparisons") {
    constexpr FixString<16> empty1("");
    constexpr FixString<32> empty2("");
    constexpr FixString<16> nonEmpty("Test");
    constexpr const char * emptyCStr = "";
    constexpr const char * nonEmptyCStr = "Test";

    // Empty vs empty
    REQUIRE((empty1 <=> empty2) == std::strong_ordering::equal);
    REQUIRE((empty2 <=> empty1) == std::strong_ordering::equal);
    REQUIRE((empty1 <=> emptyCStr) == std::strong_ordering::equal);
    REQUIRE((emptyCStr <=> empty1) == std::strong_ordering::equal);

    STATIC_REQUIRE((empty1 <=> empty2) == std::strong_ordering::equal);
    STATIC_REQUIRE((empty2 <=> empty1) == std::strong_ordering::equal);
    STATIC_REQUIRE((empty1 <=> emptyCStr) == std::strong_ordering::equal);
    STATIC_REQUIRE((emptyCStr <=> empty1) == std::strong_ordering::equal);

    // Empty vs non-empty
    REQUIRE((empty1 <=> nonEmpty) == std::strong_ordering::less);
    REQUIRE((nonEmpty <=> empty1) == std::strong_ordering::greater);
    REQUIRE((empty1 <=> nonEmptyCStr) == std::strong_ordering::less);
    REQUIRE((nonEmptyCStr <=> empty1) == std::strong_ordering::greater);

    STATIC_REQUIRE((empty1 <=> nonEmpty) == std::strong_ordering::less);
    STATIC_REQUIRE((nonEmpty <=> empty1) == std::strong_ordering::greater);
    STATIC_REQUIRE((empty1 <=> nonEmptyCStr) == std::strong_ordering::less);
    STATIC_REQUIRE((nonEmptyCStr <=> empty1) == std::strong_ordering::greater);
  }

  SECTION("Single character strings") {
    constexpr FixString<8> str1("A");
    constexpr FixString<8> str2("B");
    constexpr FixString<8> str3("A");
    constexpr FixString<8> str4("Z");

    // Equal single characters
    REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    // Different single characters
    REQUIRE((str1 <=> str2) == std::strong_ordering::less);
    REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str4) == std::strong_ordering::less);
    REQUIRE((str4 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str2 <=> str4) == std::strong_ordering::less);
    REQUIRE((str4 <=> str2) == std::strong_ordering::greater);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::less);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::less);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str2 <=> str4) == std::strong_ordering::less);
    STATIC_REQUIRE((str4 <=> str2) == std::strong_ordering::greater);
  }

  SECTION("Case sensitivity") {
    constexpr FixString<16> lower("hello");
    constexpr FixString<16> upper("HELLO");
    constexpr FixString<16> mixed("Hello");

    // Case-sensitive comparisons
    REQUIRE((lower <=> upper) == std::strong_ordering::greater); // 'h' > 'H' in ASCII
    REQUIRE((upper <=> lower) == std::strong_ordering::less);
    REQUIRE((lower <=> mixed) == std::strong_ordering::greater); // 'h' > 'H' in ASCII
    REQUIRE((mixed <=> lower) == std::strong_ordering::less);
    REQUIRE((upper <=> mixed) == std::strong_ordering::less); // 'H' < 'H' (same), but 'E' < 'e'
    REQUIRE((mixed <=> upper) == std::strong_ordering::greater);

    STATIC_REQUIRE((lower <=> upper) == std::strong_ordering::greater);
    STATIC_REQUIRE((upper <=> lower) == std::strong_ordering::less);
    STATIC_REQUIRE((lower <=> mixed) == std::strong_ordering::greater);
    STATIC_REQUIRE((mixed <=> lower) == std::strong_ordering::less);
    STATIC_REQUIRE((upper <=> mixed) == std::strong_ordering::less);
    STATIC_REQUIRE((mixed <=> upper) == std::strong_ordering::greater);
  }

  SECTION("Prefix comparisons") {
    constexpr FixString<16> str1("Hello");
    constexpr FixString<16> str2("HelloWorld");
    constexpr FixString<16> str3("Hell");
    constexpr FixString<16> str4("Hello");

    // One string is prefix of another
    REQUIRE((str1 <=> str2) == std::strong_ordering::less); // "Hello" < "HelloWorld"
    REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str3 <=> str1) == std::strong_ordering::less); // "Hell" < "Hello"
    REQUIRE((str1 <=> str3) == std::strong_ordering::greater);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::less);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::less);
    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::greater);

    // Equal strings
    REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::equal);
  }

  SECTION("Special characters") {
    constexpr FixString<32> str1("Hello\nWorld");
    constexpr FixString<32> str2("Hello\tWorld");
    constexpr FixString<32> str3("Hello World");
    constexpr FixString<32> str4("Hello\nWorld");

    // Equal strings with special characters
    REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::equal);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::equal);

    // Different special characters
    REQUIRE((str1 <=> str2) == std::strong_ordering::greater); // '\n' > '\t' in ASCII
    REQUIRE((str2 <=> str1) == std::strong_ordering::less);
    REQUIRE((str1 <=> str3) == std::strong_ordering::less); // '\n' < ' ' in ASCII
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str2 <=> str3) == std::strong_ordering::less); // '\t' < ' ' in ASCII
    REQUIRE((str3 <=> str2) == std::strong_ordering::greater);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::greater);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::less);
    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str2 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str2) == std::strong_ordering::greater);
  }

  SECTION("Unicode content") {
    constexpr FixString<32> str1("Привет");
    constexpr FixString<32> str2("Мир");
    constexpr FixString<32> str3("Привет");
    constexpr FixString<32> str4("Hello 🌍");

    // Equal Unicode strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    // Different Unicode strings
    REQUIRE((str1 <=> str2) != std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) != std::strong_ordering::equal);
    REQUIRE((str1 <=> str4) != std::strong_ordering::equal);
    REQUIRE((str4 <=> str1) != std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str2) != std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) != std::strong_ordering::equal);
    STATIC_REQUIRE((str1 <=> str4) != std::strong_ordering::equal);
    STATIC_REQUIRE((str4 <=> str1) != std::strong_ordering::equal);
  }

  SECTION("Numeric strings") {
    constexpr FixString<16> str1("123");
    constexpr FixString<16> str2("456");
    constexpr FixString<16> str3("123");
    constexpr FixString<16> str4("12");
    constexpr FixString<16> str5("1234");

    // Equal numeric strings
    REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::equal);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::equal);

    // Different numeric strings
    REQUIRE((str1 <=> str2) == std::strong_ordering::less); // "123" < "456"
    REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    REQUIRE((str1 <=> str4) == std::strong_ordering::greater); // "123" > "12"
    REQUIRE((str4 <=> str1) == std::strong_ordering::less);
    REQUIRE((str1 <=> str5) == std::strong_ordering::less); // "123" < "1234"
    REQUIRE((str5 <=> str1) == std::strong_ordering::greater);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::less);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::greater);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::less);
    STATIC_REQUIRE((str1 <=> str5) == std::strong_ordering::less);
    STATIC_REQUIRE((str5 <=> str1) == std::strong_ordering::greater);
  }

  SECTION("Edge cases") {
    constexpr FixString<4> str1("ABC"); // Maximum length for capacity 4
    constexpr FixString<8> str2("ABC");
    constexpr FixString<16> str3("ABCD");

    // Same content, different capacities
    REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    // Different lengths
    REQUIRE((str1 <=> str3) == std::strong_ordering::less); // "ABC" < "ABCD"
    REQUIRE((str3 <=> str1) == std::strong_ordering::greater);

    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
  }

  SECTION("Constexpr operations") {
    constexpr FixString<16> str1("Test");
    constexpr FixString<16> str2("Test");
    constexpr FixString<16> str3("Different");
    constexpr FixString<16> str4("Test");
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
    constexpr FixString<64> str1("This is a very long string for performance testing");
    constexpr FixString<64> str2("This is a very long string for performance testing");
    constexpr FixString<64> str3("This is a very long string for performance testing!");
    constexpr FixString<64> str4("This is a different string for performance testing");

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

TEST_CASE("FixString std::swap", "[core][fixstring]") {
  SECTION("Basic swap functionality") {
    FixString<32> str1("Hello");
    FixString<32> str2("World");

    CHECK(strcmp(str1.c_str(), "Hello") == 0);
    CHECK(strcmp(str2.c_str(), "World") == 0);

    std::swap(str1, str2);

    CHECK(strcmp(str1.c_str(), "World") == 0);
    CHECK(strcmp(str2.c_str(), "Hello") == 0);
  }

  SECTION("Swap with empty strings") {
    FixString<32> str1("Hello");
    FixString<32> str2("");

    CHECK(strcmp(str1.c_str(), "Hello") == 0);
    CHECK(strcmp(str2.c_str(), "") == 0);

    std::swap(str1, str2);

    CHECK(strcmp(str1.c_str(), "") == 0);
    CHECK(strcmp(str2.c_str(), "Hello") == 0);
  }

  SECTION("Swap two empty strings") {
    FixString<32> str1("");
    FixString<32> str2("");

    CHECK(strcmp(str1.c_str(), "") == 0);
    CHECK(strcmp(str2.c_str(), "") == 0);

    std::swap(str1, str2);

    CHECK(strcmp(str1.c_str(), "") == 0);
    CHECK(strcmp(str2.c_str(), "") == 0);
  }

  SECTION("Self-swap") {
    FixString<32> str1("Hello");

    CHECK(strcmp(str1.c_str(), "Hello") == 0);

    std::swap(str1, str1);

    CHECK(strcmp(str1.c_str(), "Hello") == 0);
  }

  SECTION("Swap with different sizes") {
    FixString<32> str1("Hi");
    FixString<32> str2("VeryLongString");

    CHECK(strcmp(str1.c_str(), "Hi") == 0);
    CHECK(strcmp(str2.c_str(), "VeryLongString") == 0);

    std::swap(str1, str2);

    CHECK(strcmp(str1.c_str(), "VeryLongString") == 0);
    CHECK(strcmp(str2.c_str(), "Hi") == 0);
  }

  SECTION("Swap with maximum length strings") {
    FixString<16> str1("123456789012345"); // 15 chars
    FixString<16> str2("ABCDEFGHIJKLMNO"); // 15 chars

    CHECK(strcmp(str1.c_str(), "123456789012345") == 0);
    CHECK(strcmp(str2.c_str(), "ABCDEFGHIJKLMNO") == 0);

    std::swap(str1, str2);

    CHECK(strcmp(str1.c_str(), "ABCDEFGHIJKLMNO") == 0);
    CHECK(strcmp(str2.c_str(), "123456789012345") == 0);
  }

  SECTION("Swap with special characters") {
    FixString<32> str1("Hello,\n\t!");
    FixString<32> str2("World,\r\n?");

    CHECK(strcmp(str1.c_str(), "Hello,\n\t!") == 0);
    CHECK(strcmp(str2.c_str(), "World,\r\n?") == 0);

    std::swap(str1, str2);

    CHECK(strcmp(str1.c_str(), "World,\r\n?") == 0);
    CHECK(strcmp(str2.c_str(), "Hello,\n\t!") == 0);
  }

  SECTION("Swap with Unicode content") {
    FixString<32> str1("Hello 世界");
    FixString<32> str2("World 宇宙");

    CHECK(strcmp(str1.c_str(), "Hello 世界") == 0);
    CHECK(strcmp(str2.c_str(), "World 宇宙") == 0);

    std::swap(str1, str2);

    CHECK(strcmp(str1.c_str(), "World 宇宙") == 0);
    CHECK(strcmp(str2.c_str(), "Hello 世界") == 0);
  }

  SECTION("Multiple swaps") {
    FixString<32> str1("First");
    FixString<32> str2("Second");
    FixString<32> str3("Third");

    CHECK(strcmp(str1.c_str(), "First") == 0);
    CHECK(strcmp(str2.c_str(), "Second") == 0);
    CHECK(strcmp(str3.c_str(), "Third") == 0);

    std::swap(str1, str2);
    CHECK(strcmp(str1.c_str(), "Second") == 0);
    CHECK(strcmp(str2.c_str(), "First") == 0);

    std::swap(str2, str3);
    CHECK(strcmp(str2.c_str(), "Third") == 0);
    CHECK(strcmp(str3.c_str(), "First") == 0);

    std::swap(str1, str3);
    CHECK(strcmp(str1.c_str(), "First") == 0);
    CHECK(strcmp(str3.c_str(), "Second") == 0);
  }

  SECTION("Performance test with large strings") {
    FixString<64> str1("This is a very long string that tests swap performance");
    FixString<64> str2("Another very long string for performance testing");

    std::swap(str1, str2);

    CHECK(strcmp(str1.c_str(), "Another very long string for performance testing") == 0);
    CHECK(strcmp(str2.c_str(), "This is a very long string that tests swap performance") == 0);
  }

  SECTION("Swap with single character strings") {
    FixString<8> str1("A");
    FixString<8> str2("B");

    CHECK(strcmp(str1.c_str(), "A") == 0);
    CHECK(strcmp(str2.c_str(), "B") == 0);

    std::swap(str1, str2);

    CHECK(strcmp(str1.c_str(), "B") == 0);
    CHECK(strcmp(str2.c_str(), "A") == 0);
  }
}
