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

TEST_CASE("CString constructors", "[core][c_string]") {
  SECTION("Default constructor") {
    constexpr const CString emptyStr;

    REQUIRE(emptyStr.size() == 0);
    REQUIRE(std::strcmp(emptyStr.c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(emptyStr.size() == 0);
    STATIC_REQUIRE(cstrcmp(emptyStr.c_str(), "") == 0);
  }

  SECTION("Copy constructor") {
    constexpr const CString original("CopyTest");
    constexpr const CString copy1(original);
    constexpr const CString copy2(original);
    constexpr const CString copy3(original);

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
  }

  SECTION("C string constructor") {
    constexpr const CString str1("Hello");
    constexpr const CString str2("World");
    constexpr const CString str3("Test");
    constexpr const CString str4("This is a longer string for testing");

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

  SECTION("Edge cases") {
    // Empty string
    constexpr const CString empty1("");
    constexpr const CString empty2("");

    REQUIRE(empty1.size() == 0);
    REQUIRE(empty2.size() == 0);

    STATIC_REQUIRE(empty1.size() == 0);
    STATIC_REQUIRE(empty2.size() == 0);

    // Single character
    constexpr const CString single("X");

    REQUIRE(single.size() == 1);
    REQUIRE(std::strcmp(single.c_str(), "X") == 0);

    STATIC_REQUIRE(single.size() == 1);
    STATIC_REQUIRE(cstrcmp(single.c_str(), "X") == 0);
  }

  SECTION("Special characters") {
    constexpr const CString newline("Line1\nLine2");
    constexpr const CString tab("Col1\tCol2");
    constexpr const CString mixed("Mix\t\nEnd");

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
    constexpr const CString unicode("Привет мир");
    constexpr const CString emoji("Hello 🌍");

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

TEST_CASE("CString operators=", "[core][c_string]") {
  SECTION("C string assignment") {
    CString str1;
    CString str2;
    CString str3;

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
    constexpr const CString constStr1 = "This is a longer string";
    STATIC_REQUIRE(constStr1.size() == 23);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "This is a longer string") == 0);
  }

  SECTION("CString assignment") {
    CString str1("Hello");
    CString str2;
    CString str3("World");

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
    constexpr const CString constStr1("Hello");
    constexpr const CString constStr2 = constStr1;
    STATIC_REQUIRE(constStr2.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Hello") == 0);
  }

  SECTION("Edge cases") {
    CString str1;
    CString str2;

    // Non-empty to empty
    str2 = "Test";
    str1 = str2;
    REQUIRE(str1.size() == 4);
    REQUIRE(std::strcmp(str1.c_str(), "Test") == 0);

    // Empty to non-empty
    str2 = "";
    str1 = str2;
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
  }

  SECTION("Special characters") {
    CString str1;
    CString str2;

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
    CString str1;
    CString str2;

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

TEST_CASE("CString assign", "[core][c_string]") {
  SECTION("C string assignment") {
    CString str1;
    CString str2;
    CString str3;

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
    constexpr auto constStr1 = CString().assign("Hello");
    constexpr auto constStr2 = CString("World").assign("VeryLongString");
    constexpr auto constStr3 = CString("A").assign("");
    STATIC_REQUIRE(constStr1.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello") == 0);
    STATIC_REQUIRE(constStr2.size() == 14);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "VeryLongString") == 0);
    STATIC_REQUIRE(constStr3.size() == 0);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "") == 0);
  }

  SECTION("CString assignment") {
    CString str1("Hello");
    CString str2("World");
    CString str3;

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
    CString emptyStr("");
    str1.assign(emptyStr);
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Compile-time checks
    constexpr const CString constStr1("Hello");
    constexpr auto constStr2 = CString("World").assign(constStr1);
    constexpr auto constStr3 = CString().assign(constStr2);
    STATIC_REQUIRE(constStr2.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Hello") == 0);
    STATIC_REQUIRE(constStr3.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "Hello") == 0);
  }

  SECTION("Edge cases") {
    CString str1("ABC");
    CString str2("ABC");
    CString str3("ABCD");

    // Assign from own c_str() (no-op path)
    str2.assign(str2.c_str());
    REQUIRE(str2.size() == 3);
    REQUIRE(std::strcmp(str2.c_str(), "ABC") == 0);

    // Assign empty string
    str3.assign("");
    REQUIRE(str3.size() == 0);
    REQUIRE(std::strcmp(str3.c_str(), "") == 0);

    // Compile-time checks
    constexpr auto constStr1 = CString("ABC").assign("XYZ");
    constexpr auto constStr2 = CString("ABCD").assign("");
    STATIC_REQUIRE(constStr1.size() == 3);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "XYZ") == 0);
    STATIC_REQUIRE(constStr2.size() == 0);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "") == 0);
  }

  SECTION("Special characters") {
    CString str1;
    CString str2;

    // Newline and tab
    str1.assign("Hello\n\tWorld");
    REQUIRE(str1.size() == 12);
    REQUIRE(std::strcmp(str1.c_str(), "Hello\n\tWorld") == 0);

    // Special characters
    str2.assign("!@#$%^&*()");
    REQUIRE(str2.size() == 10);
    REQUIRE(std::strcmp(str2.c_str(), "!@#$%^&*()") == 0);

    // Compile-time checks
    constexpr auto constStr1 = CString().assign("Hello\n\tWorld");
    constexpr auto constStr2 = CString().assign("!@#$%^&*()");
    STATIC_REQUIRE(constStr1.size() == 12);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello\n\tWorld") == 0);
    STATIC_REQUIRE(constStr2.size() == 10);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "!@#$%^&*()") == 0);
  }

  SECTION("Unicode content") {
    CString str1;
    CString str2;

    // Unicode characters
    str1.assign("Hello 世界");
    REQUIRE(str1.size() == 12);
    REQUIRE(std::strcmp(str1.c_str(), "Hello 世界") == 0);

    // Mixed ASCII and Unicode
    str2.assign("Test 🌍");
    REQUIRE(str2.size() == 9);
    REQUIRE(std::strcmp(str2.c_str(), "Test 🌍") == 0);

    // Compile-time checks
    constexpr auto constStr1 = CString().assign("Hello 世界");
    constexpr auto constStr2 = CString().assign("Test 🌍");
    STATIC_REQUIRE(constStr1.size() == 12);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello 世界") == 0);
    STATIC_REQUIRE(constStr2.size() == 9);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Test 🌍") == 0);
  }

  SECTION("Chaining assign") {
    CString str1;

    // Chaining assign operations
    str1.assign("a").assign("b");
    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "b") == 0);

    // Multiple chaining
    str1.assign("Hello").assign("World").assign("Test");
    REQUIRE(str1.size() == 4);
    REQUIRE(std::strcmp(str1.c_str(), "Test") == 0);

    // Compile-time checks
    constexpr auto constStr1 = CString("a").assign("b");
    constexpr auto constStr2 = CString("Hello").assign("Test");
    STATIC_REQUIRE(constStr1.size() == 1);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "b") == 0);
    STATIC_REQUIRE(constStr2.size() == 4);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Test") == 0);
  }
}

TEST_CASE("CString at", "[core][c_string]") {
  SECTION("at() access") {
    constexpr const CString str("World");

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
    constexpr const CString str;

    REQUIRE(str.at(0) == '\0');

    // Compile-time checks
    STATIC_REQUIRE(str.at(0) == '\0');
  }
}

TEST_CASE("CString operator[]", "[core][c_string]") {
  SECTION("[] access") {
    constexpr const CString str("Hello");
    constexpr const CString longStr("VeryLongString");

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
    REQUIRE(longStr[13] == 'g');

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
    STATIC_REQUIRE(longStr[13] == 'g');
  }

  SECTION("empty string") {
    constexpr const CString str;

    REQUIRE(str[0] == '\0');

    // Compile-time checks
    STATIC_REQUIRE(str[0] == '\0');
  }
}

TEST_CASE("CString front and back", "[core][c_string]") {
  SECTION("Front method") {
    constexpr const CString testString("Hello World");

    REQUIRE(testString.front() == 'H');
    REQUIRE(testString[0] == 'H');
    STATIC_REQUIRE(testString.front() == 'H');
    STATIC_REQUIRE(testString[0] == 'H');
  }

  SECTION("Back method") {
    constexpr const CString testString("Hello World");

    REQUIRE(testString.back() == 'd');
    REQUIRE(testString[testString.size() - 1] == 'd');
    STATIC_REQUIRE(testString.back() == 'd');
    STATIC_REQUIRE(testString[testString.size() - 1] == 'd');
  }

  SECTION("Single character string") {
    constexpr const CString testString("A");

    REQUIRE(testString.front() == 'A');
    REQUIRE(testString.back() == 'A');
    REQUIRE(testString.front() == testString.back());

    STATIC_REQUIRE(testString.front() == 'A');
    STATIC_REQUIRE(testString.back() == 'A');
    STATIC_REQUIRE(testString.front() == testString.back());
  }

  SECTION("Empty string") {
    constexpr const CString testString;

    REQUIRE(testString.front() == '\0');
    STATIC_REQUIRE(testString.front() == '\0');
  }

  SECTION("Two character string") {
    constexpr const CString testString("AB");

    REQUIRE(testString.front() == 'A');
    REQUIRE(testString.back() == 'B');
    REQUIRE(testString.front() != testString.back());

    STATIC_REQUIRE(testString.front() == 'A');
    STATIC_REQUIRE(testString.back() == 'B');
    STATIC_REQUIRE(testString.front() != testString.back());
  }

  SECTION("Const references") {
    constexpr const CString testString("Hello World");

    STATIC_REQUIRE(testString.front() == 'H');
    STATIC_REQUIRE(testString.back() == 'd');
  }

  SECTION("Numeric content") {
    constexpr const CString testString("12345");

    REQUIRE(testString.front() == '1');
    REQUIRE(testString.back() == '5');

    STATIC_REQUIRE(testString.front() == '1');
    STATIC_REQUIRE(testString.back() == '5');
  }

  SECTION("Mixed content") {
    constexpr const CString testString("123Hello456");

    REQUIRE(testString.front() == '1');
    REQUIRE(testString.back() == '6');

    STATIC_REQUIRE(testString.front() == '1');
    STATIC_REQUIRE(testString.back() == '6');
  }

  SECTION("Long strings") {
    constexpr const CString testString("This is a very long string for performance testing");

    REQUIRE(testString.front() == 'T');
    REQUIRE(testString.back() == 'g');

    STATIC_REQUIRE(testString.front() == 'T');
    STATIC_REQUIRE(testString.back() == 'g');
  }

  SECTION("Case sensitivity") {
    constexpr const CString testString("Hello World");

    REQUIRE(testString.front() == 'H'); // Uppercase
    REQUIRE(testString.back() == 'd'); // Lowercase

    STATIC_REQUIRE(testString.front() == 'H');
    STATIC_REQUIRE(testString.back() == 'd');
  }

  SECTION("Whitespace handling") {
    constexpr const CString testString(" Hello ");

    REQUIRE(testString.front() == ' ');
    REQUIRE(testString.back() == ' ');

    STATIC_REQUIRE(testString.front() == ' ');
    STATIC_REQUIRE(testString.back() == ' ');
  }

  SECTION("Constexpr operations") {
    constexpr const CString str1("Hello");
    constexpr const CString str2("World");
    constexpr const CString str3("Test");

    // Compile-time front operations
    constexpr const char & front1 = str1.front();
    constexpr const char & front2 = str2.front();
    constexpr const char & front3 = str3.front();

    STATIC_REQUIRE(front1 == 'H');
    STATIC_REQUIRE(front2 == 'W');
    STATIC_REQUIRE(front3 == 'T');

    // Compile-time back operations
    constexpr const char & back1 = str1.back();
    constexpr const char & back2 = str2.back();
    constexpr const char & back3 = str3.back();

    STATIC_REQUIRE(back1 == 'o');
    STATIC_REQUIRE(back2 == 'd');
    STATIC_REQUIRE(back3 == 't');
  }
}

TEST_CASE("CString data", "[core][c_string]") {
  SECTION("Basic data access") {
    constexpr const CString testString("Hello World");
    constexpr const CString emptyString("");
    constexpr const CString singleChar("A");

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
    constexpr const CString testString("Stability Test");
    constexpr const CString copy1(testString);
    constexpr const CString copy2(testString);

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
    constexpr const CString emptyString("");
    constexpr const CString defaultString;

    // Test data() with empty strings
    REQUIRE(std::strcmp(emptyString.data(), "") == 0);
    REQUIRE(std::strcmp(defaultString.data(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(cstrcmp(emptyString.data(), "") == 0);
    STATIC_REQUIRE(cstrcmp(defaultString.data(), "") == 0);
  }
}

TEST_CASE("CString c_str method", "[core][c_string]") {
  SECTION("Basic c_str access") {
    constexpr const CString testString("Hello World");
    constexpr const CString emptyString("");
    constexpr const CString singleChar("A");

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
    constexpr const CString testString("Stability Test");
    constexpr const CString copy1(testString);
    constexpr const CString copy2(testString);

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
    constexpr const CString emptyString("");
    constexpr const CString defaultString;

    // Test c_str() with empty strings
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(emptyString.c_str() == emptyString.data());
    STATIC_REQUIRE(defaultString.c_str() == defaultString.data());
  }
}

TEST_CASE("CString empty method", "[core][c_string]") {
  SECTION("Basic empty check") {
    constexpr const CString nonEmptyString("Hello World");
    constexpr const CString emptyString("");
    constexpr const CString defaultString;

    REQUIRE_FALSE(nonEmptyString.empty());
    REQUIRE(emptyString.empty());
    REQUIRE(defaultString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(nonEmptyString.empty());
    STATIC_REQUIRE(emptyString.empty());
    STATIC_REQUIRE(defaultString.empty());
  }

  SECTION("Single character strings") {
    constexpr const CString singleChar("A");
    constexpr const CString emptyString("");

    REQUIRE_FALSE(singleChar.empty());
    REQUIRE(emptyString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(singleChar.empty());
    STATIC_REQUIRE(emptyString.empty());
  }

  SECTION("Different capacities") {
    constexpr const CString smallString("Hi");
    constexpr const CString mediumString("Hello World");
    constexpr const CString largeString("This is a longer string");
    constexpr const CString emptySmall("");
    constexpr const CString emptyMedium("");
    constexpr const CString emptyLarge("");

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
    constexpr const CString newlineString("Hello\nWorld");
    constexpr const CString tabString("Hello\tWorld");
    constexpr const CString specialString("!@#$%^&*()");
    constexpr const CString emptyString("");

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
    constexpr const CString unicodeString("Привет мир");
    constexpr const CString emojiString("Hello 🌍 World");
    constexpr const CString mixedString("Hello 世界");
    constexpr const CString emptyString("");

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
    constexpr const CString numericString("12345");
    constexpr const CString floatString("3.14159");
    constexpr const CString hexString("0xABCD");
    constexpr const CString emptyString("");

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
    constexpr const CString mixedString("Hello123World!@#");
    constexpr const CString complexString("Test\n123\t!@#");
    constexpr const CString longString("This is a very long string with mixed content 123!@#");
    constexpr const CString emptyString("");

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
    constexpr const CString maxString("123456789012345"); // 15 characters
    constexpr const CString maxSmall("1234567"); // 7 characters
    constexpr const CString maxTiny("123"); // 3 characters
    constexpr const CString emptyString("");

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
    constexpr const CString singleChar("A");
    constexpr const CString twoChars("AB");
    constexpr const CString emptyString("");
    constexpr const CString defaultString;

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

TEST_CASE("CString size method", "[core][c_string]") {
  SECTION("Basic size check") {
    constexpr const CString testString("Hello World");
    constexpr const CString emptyString("");
    constexpr const CString defaultString;

    REQUIRE(testString.size() == 11);
    REQUIRE(emptyString.size() == 0);
    REQUIRE(defaultString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.size() == 11);
    STATIC_REQUIRE(emptyString.size() == 0);
    STATIC_REQUIRE(defaultString.size() == 0);
  }

  SECTION("Single character strings") {
    constexpr const CString singleChar("A");
    constexpr const CString emptyString("");

    REQUIRE(singleChar.size() == 1);
    REQUIRE(emptyString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(singleChar.size() == 1);
    STATIC_REQUIRE(emptyString.size() == 0);
  }

  SECTION("Different capacities") {
    constexpr const CString smallString("Hi");
    constexpr const CString mediumString("Hello World");
    constexpr const CString largeString("This is a longer string");
    constexpr const CString emptySmall("");
    constexpr const CString emptyMedium("");
    constexpr const CString emptyLarge("");

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
    constexpr const CString newlineString("Hello\nWorld");
    constexpr const CString tabString("Hello\tWorld");
    constexpr const CString specialString("!@#$%^&*()");
    constexpr const CString emptyString("");

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
    constexpr const CString unicodeString("Привет мир");
    constexpr const CString emojiString("Hello 🌍 World");
    constexpr const CString mixedString("Hello 世界");
    constexpr const CString emptyString("");

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
    constexpr const CString numericString("12345");
    constexpr const CString floatString("3.14159");
    constexpr const CString hexString("0xABCD");
    constexpr const CString emptyString("");

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
    constexpr const CString mixedString("Hello123World!@#");
    constexpr const CString complexString("Test\n123\t!@#");
    constexpr const CString longString("This is a very long string with mixed content 123!@#");
    constexpr const CString emptyString("");

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
    constexpr const CString maxString("123456789012345"); // 15 characters
    constexpr const CString maxSmall("1234567"); // 7 characters
    constexpr const CString maxTiny("123"); // 3 characters
    constexpr const CString emptyString("");

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
    constexpr const CString singleChar("A");
    constexpr const CString twoChars("AB");
    constexpr const CString emptyString("");
    constexpr const CString defaultString;

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

TEST_CASE("CString utf8_size", "[core][c_string]") {
  SECTION("ASCII strings") {
    constexpr const CString asciiString("Hello World");
    constexpr const CString emptyString("");
    constexpr const CString singleChar("A");

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

    constexpr const CString cyrillicString(cyrillicText.data());

    REQUIRE(cyrillicString.size() == 19);
    REQUIRE(cyrillicString.utf8_size() == 10);
  }

  SECTION("Mixed ASCII and UTF-8") {
    // "Hello 世界" in UTF-8
    static constexpr std::array<char, 13> mixedText{{char(0x48), char(0x65), char(0x6C), char(0x6C), char(0x6F),
                                                     char(0x20), char(0xE4), char(0xB8), char(0x96), char(0xE7),
                                                     char(0x95), char(0x8C), char(0x00)}};

    constexpr const CString mixedString(mixedText.data());

    REQUIRE(mixedString.size() == 12);
    REQUIRE(mixedString.utf8_size() == 8); // 6 ASCII + 2 Chinese characters
  }

  SECTION("Emoji characters") {
    // "Hello 🌍" in UTF-8
    static constexpr std::array<char, 11> emojiText{{char(0x48), char(0x65), char(0x6C), char(0x6C), char(0x6F),
                                                     char(0x20), char(0xF0), char(0x9F), char(0x8C), char(0x8D),
                                                     char(0x00)}};

    constexpr const CString emojiString(emojiText.data());

    REQUIRE(emojiString.size() == 10);
    REQUIRE(emojiString.utf8_size() == 7); // 6 ASCII + 1 emoji
  }

  SECTION("Special characters") {
    constexpr const CString specialString("!@#$%^&*()");
    constexpr const CString numericString("1234567890");
    constexpr const CString punctuationString(".,;:!?");

    REQUIRE(specialString.utf8_size() == 10);
    REQUIRE(numericString.utf8_size() == 10);
    REQUIRE(punctuationString.utf8_size() == 6);

    // Special characters are ASCII, so utf8_size equals size
    REQUIRE(specialString.utf8_size() == specialString.size());
    REQUIRE(numericString.utf8_size() == numericString.size());
    REQUIRE(punctuationString.utf8_size() == punctuationString.size());
  }

  SECTION("Edge cases") {
    constexpr const CString singleByte("A");
    constexpr const CString twoByte("А"); // Cyrillic A
    constexpr const CString threeByte("中"); // Chinese character
    constexpr const CString fourByte("🌍"); // Emoji

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

    constexpr const CString longString(longUtf8Text.data());

    REQUIRE(longString.size() == 66); // 66 bytes
    REQUIRE(longString.utf8_size() == 43); // 43 characters
  }
}

TEST_CASE("CString length", "[core][c_string]") {
  SECTION("Basic length check") {
    constexpr const CString testString("Hello World");
    constexpr const CString emptyString("");
    constexpr const CString singleChar("A");

    REQUIRE(testString.length() == 11);
    REQUIRE(emptyString.length() == 0);
    REQUIRE(singleChar.length() == 1);

    // length() should equal size() for all strings
    REQUIRE(testString.length() == testString.size());
    REQUIRE(emptyString.length() == emptyString.size());
    REQUIRE(singleChar.length() == singleChar.size());

    STATIC_REQUIRE(testString.length() == 11);
    STATIC_REQUIRE(emptyString.length() == 0);
    STATIC_REQUIRE(singleChar.length() == 1);
  }

  SECTION("Different capacities") {
    constexpr const CString smallString("Hi");
    constexpr const CString mediumString("Hello World");
    constexpr const CString largeString("This is a longer string");
    constexpr const CString emptySmall("");
    constexpr const CString emptyMedium("");
    constexpr const CString emptyLarge("");

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

    STATIC_REQUIRE(smallString.length() == 2);
    STATIC_REQUIRE(mediumString.length() == 11);
    STATIC_REQUIRE(largeString.length() == 23);
  }

  SECTION("Special characters") {
    constexpr const CString newlineString("Hello\nWorld");
    constexpr const CString tabString("Hello\tWorld");
    constexpr const CString specialString("!@#$%^&*()");
    constexpr const CString emptyString("");

    REQUIRE(newlineString.length() == 11);
    REQUIRE(tabString.length() == 11);
    REQUIRE(specialString.length() == 10);
    REQUIRE(emptyString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(newlineString.length() == newlineString.size());
    REQUIRE(tabString.length() == tabString.size());
    REQUIRE(specialString.length() == specialString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    STATIC_REQUIRE(newlineString.length() == 11);
    STATIC_REQUIRE(tabString.length() == 11);
    STATIC_REQUIRE(specialString.length() == 10);
  }

  SECTION("Unicode content") {
    constexpr const CString unicodeString("Привет мир");
    constexpr const CString emojiString("Hello 🌍 World");
    constexpr const CString mixedString("Hello 世界");
    constexpr const CString emptyString("");

    REQUIRE(unicodeString.length() == 19);
    REQUIRE(emojiString.length() == 16);
    REQUIRE(mixedString.length() == 12);
    REQUIRE(emptyString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(unicodeString.length() == unicodeString.size());
    REQUIRE(emojiString.length() == emojiString.size());
    REQUIRE(mixedString.length() == mixedString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    STATIC_REQUIRE(unicodeString.length() == 19);
    STATIC_REQUIRE(emojiString.length() == 16);
    STATIC_REQUIRE(mixedString.length() == 12);
  }

  SECTION("Numeric content") {
    constexpr const CString numericString("12345");
    constexpr const CString floatString("3.14159");
    constexpr const CString hexString("0xABCD");
    constexpr const CString emptyString("");

    REQUIRE(numericString.length() == 5);
    REQUIRE(floatString.length() == 7);
    REQUIRE(hexString.length() == 6);
    REQUIRE(emptyString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(numericString.length() == numericString.size());
    REQUIRE(floatString.length() == floatString.size());
    REQUIRE(hexString.length() == hexString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    STATIC_REQUIRE(numericString.length() == 5);
    STATIC_REQUIRE(floatString.length() == 7);
    STATIC_REQUIRE(hexString.length() == 6);
  }

  SECTION("Mixed content") {
    constexpr const CString mixedString("Hello123World!@#");
    constexpr const CString complexString("Test\n123\t!@#");
    constexpr const CString longString("This is a very long string with mixed content 123!@#");
    constexpr const CString emptyString("");

    REQUIRE(mixedString.length() == 16);
    REQUIRE(complexString.length() == 12);
    REQUIRE(longString.length() == 52);
    REQUIRE(emptyString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(mixedString.length() == mixedString.size());
    REQUIRE(complexString.length() == complexString.size());
    REQUIRE(longString.length() == longString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    STATIC_REQUIRE(mixedString.length() == 16);
    STATIC_REQUIRE(complexString.length() == 12);
    STATIC_REQUIRE(longString.length() == 52);
  }

  SECTION("Edge cases") {
    constexpr const CString singleChar("A");
    constexpr const CString twoChars("AB");
    constexpr const CString emptyString("");
    constexpr const CString defaultString;

    REQUIRE(singleChar.length() == 1);
    REQUIRE(twoChars.length() == 2);
    REQUIRE(emptyString.length() == 0);
    REQUIRE(defaultString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(singleChar.length() == singleChar.size());
    REQUIRE(twoChars.length() == twoChars.size());
    REQUIRE(emptyString.length() == emptyString.size());
    REQUIRE(defaultString.length() == defaultString.size());

    STATIC_REQUIRE(singleChar.length() == 1);
    STATIC_REQUIRE(twoChars.length() == 2);
    STATIC_REQUIRE(emptyString.length() == 0);
    STATIC_REQUIRE(defaultString.length() == 0);
  }
}
