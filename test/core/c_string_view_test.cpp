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

TEST_CASE("CStringView constructors", "[core][c_string_view]") {
  SECTION("Default constructor") {
    constexpr CStringView emptyStr;

    REQUIRE(emptyStr.size() == 0);
    REQUIRE(std::strcmp(emptyStr.c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(emptyStr.size() == 0);
    STATIC_REQUIRE(cstrcmp(emptyStr.c_str(), "") == 0);
  }

  SECTION("Copy constructor") {
    constexpr CStringView original("CopyTest");
    constexpr CStringView copy1(original);
    constexpr CStringView copy2(original);
    constexpr CStringView copy3(original);

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
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("World");
    constexpr CStringView str3("Test");
    constexpr CStringView str4("This is a longer string for testing");

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
    constexpr CStringView empty1("");
    constexpr CStringView empty2("");

    REQUIRE(empty1.size() == 0);
    REQUIRE(empty2.size() == 0);

    STATIC_REQUIRE(empty1.size() == 0);
    STATIC_REQUIRE(empty2.size() == 0);

    // Single character
    constexpr CStringView single("X");

    REQUIRE(single.size() == 1);
    REQUIRE(std::strcmp(single.c_str(), "X") == 0);

    STATIC_REQUIRE(single.size() == 1);
    STATIC_REQUIRE(cstrcmp(single.c_str(), "X") == 0);
  }

  SECTION("Special characters") {
    constexpr CStringView newline("Line1\nLine2");
    constexpr CStringView tab("Col1\tCol2");
    constexpr CStringView mixed("Mix\t\nEnd");

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
    constexpr CStringView unicode("Привет мир");
    constexpr CStringView emoji("Hello 🌍");

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

TEST_CASE("CStringView operators=", "[core][c_string_view]") {
  SECTION("C string assignment") {
    CStringView str1;
    CStringView str2;
    CStringView str3;

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
    constexpr CStringView constStr1 = "This is a longer string";
    STATIC_REQUIRE(constStr1.size() == 23);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "This is a longer string") == 0);
  }

  SECTION("CStringView assignment") {
    CStringView str1("Hello");
    CStringView str2;
    CStringView str3("World");

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
    constexpr CStringView constStr1("Hello");
    constexpr CStringView constStr2 = constStr1;
    STATIC_REQUIRE(constStr2.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Hello") == 0);
  }

  SECTION("Edge cases") {
    CStringView str1;
    CStringView str2;

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
    CStringView str1;
    CStringView str2;

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
    CStringView str1;
    CStringView str2;

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

TEST_CASE("CStringView assign", "[core][c_string_view]") {
  SECTION("C string assignment") {
    CStringView str1;
    CStringView str2;
    CStringView str3;

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
    constexpr auto constStr1 = CStringView().assign("Hello");
    constexpr auto constStr2 = CStringView("World").assign("VeryLongString");
    constexpr auto constStr3 = CStringView("A").assign("");
    STATIC_REQUIRE(constStr1.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello") == 0);
    STATIC_REQUIRE(constStr2.size() == 14);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "VeryLongString") == 0);
    STATIC_REQUIRE(constStr3.size() == 0);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "") == 0);
  }

  SECTION("CStringView assignment") {
    CStringView str1("Hello");
    CStringView str2("World");
    CStringView str3;

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
    CStringView emptyStr("");
    str1.assign(emptyStr);
    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Compile-time checks
    constexpr CStringView constStr1("Hello");
    constexpr auto constStr2 = CStringView("World").assign(constStr1);
    constexpr auto constStr3 = CStringView().assign(constStr2);
    STATIC_REQUIRE(constStr2.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Hello") == 0);
    STATIC_REQUIRE(constStr3.size() == 5);
    STATIC_REQUIRE(cstrcmp(constStr3.c_str(), "Hello") == 0);
  }

  SECTION("Edge cases") {
    CStringView str1("ABC");
    CStringView str2("ABC");
    CStringView str3("ABCD");

    // Assign from own c_str() (no-op path)
    str2.assign(str2.c_str());
    REQUIRE(str2.size() == 3);
    REQUIRE(std::strcmp(str2.c_str(), "ABC") == 0);

    // Assign empty string
    str3.assign("");
    REQUIRE(str3.size() == 0);
    REQUIRE(std::strcmp(str3.c_str(), "") == 0);

    // Compile-time checks
    constexpr auto constStr1 = CStringView("ABC").assign("XYZ");
    constexpr auto constStr2 = CStringView("ABCD").assign("");
    STATIC_REQUIRE(constStr1.size() == 3);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "XYZ") == 0);
    STATIC_REQUIRE(constStr2.size() == 0);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "") == 0);
  }

  SECTION("Special characters") {
    CStringView str1;
    CStringView str2;

    // Newline and tab
    str1.assign("Hello\n\tWorld");
    REQUIRE(str1.size() == 12);
    REQUIRE(std::strcmp(str1.c_str(), "Hello\n\tWorld") == 0);

    // Special characters
    str2.assign("!@#$%^&*()");
    REQUIRE(str2.size() == 10);
    REQUIRE(std::strcmp(str2.c_str(), "!@#$%^&*()") == 0);

    // Compile-time checks
    constexpr auto constStr1 = CStringView().assign("Hello\n\tWorld");
    constexpr auto constStr2 = CStringView().assign("!@#$%^&*()");
    STATIC_REQUIRE(constStr1.size() == 12);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello\n\tWorld") == 0);
    STATIC_REQUIRE(constStr2.size() == 10);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "!@#$%^&*()") == 0);
  }

  SECTION("Unicode content") {
    CStringView str1;
    CStringView str2;

    // Unicode characters
    str1.assign("Hello 世界");
    REQUIRE(str1.size() == 12);
    REQUIRE(std::strcmp(str1.c_str(), "Hello 世界") == 0);

    // Mixed ASCII and Unicode
    str2.assign("Test 🌍");
    REQUIRE(str2.size() == 9);
    REQUIRE(std::strcmp(str2.c_str(), "Test 🌍") == 0);

    // Compile-time checks
    constexpr auto constStr1 = CStringView().assign("Hello 世界");
    constexpr auto constStr2 = CStringView().assign("Test 🌍");
    STATIC_REQUIRE(constStr1.size() == 12);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "Hello 世界") == 0);
    STATIC_REQUIRE(constStr2.size() == 9);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Test 🌍") == 0);
  }

  SECTION("Chaining assign") {
    CStringView str1;

    // Chaining assign operations
    str1.assign("a").assign("b");
    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "b") == 0);

    // Multiple chaining
    str1.assign("Hello").assign("World").assign("Test");
    REQUIRE(str1.size() == 4);
    REQUIRE(std::strcmp(str1.c_str(), "Test") == 0);

    // Compile-time checks
    constexpr auto constStr1 = CStringView("a").assign("b");
    constexpr auto constStr2 = CStringView("Hello").assign("Test");
    STATIC_REQUIRE(constStr1.size() == 1);
    STATIC_REQUIRE(cstrcmp(constStr1.c_str(), "b") == 0);
    STATIC_REQUIRE(constStr2.size() == 4);
    STATIC_REQUIRE(cstrcmp(constStr2.c_str(), "Test") == 0);
  }
}

TEST_CASE("CStringView at", "[core][c_string_view]") {
  SECTION("at() access") {
    constexpr CStringView str("World");

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
    constexpr CStringView str;

    REQUIRE(str.at(0) == '\0');

    // Compile-time checks
    STATIC_REQUIRE(str.at(0) == '\0');
  }
}

TEST_CASE("CStringView operator[]", "[core][c_string_view]") {
  SECTION("[] access") {
    constexpr CStringView str("Hello");
    constexpr CStringView longStr("VeryLongString");

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
    constexpr CStringView str;

    REQUIRE(str[0] == '\0');

    // Compile-time checks
    STATIC_REQUIRE(str[0] == '\0');
  }
}

TEST_CASE("CStringView front and back", "[core][c_string_view]") {
  SECTION("Front method") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.front() == 'H');
    REQUIRE(testString[0] == 'H');
    STATIC_REQUIRE(testString.front() == 'H');
    STATIC_REQUIRE(testString[0] == 'H');
  }

  SECTION("Back method") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.back() == 'd');
    REQUIRE(testString[testString.size() - 1] == 'd');
    STATIC_REQUIRE(testString.back() == 'd');
    STATIC_REQUIRE(testString[testString.size() - 1] == 'd');
  }

  SECTION("Single character string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.front() == 'A');
    REQUIRE(testString.back() == 'A');
    REQUIRE(testString.front() == testString.back());

    STATIC_REQUIRE(testString.front() == 'A');
    STATIC_REQUIRE(testString.back() == 'A');
    STATIC_REQUIRE(testString.front() == testString.back());
  }

  SECTION("Empty string") {
    constexpr CStringView testString;

    REQUIRE(testString.front() == '\0');
    STATIC_REQUIRE(testString.front() == '\0');
  }

  SECTION("Two character string") {
    constexpr CStringView testString("AB");

    REQUIRE(testString.front() == 'A');
    REQUIRE(testString.back() == 'B');
    REQUIRE(testString.front() != testString.back());

    STATIC_REQUIRE(testString.front() == 'A');
    STATIC_REQUIRE(testString.back() == 'B');
    STATIC_REQUIRE(testString.front() != testString.back());
  }

  SECTION("Const references") {
    constexpr CStringView testString("Hello World");

    STATIC_REQUIRE(testString.front() == 'H');
    STATIC_REQUIRE(testString.back() == 'd');
  }

  SECTION("Numeric content") {
    constexpr CStringView testString("12345");

    REQUIRE(testString.front() == '1');
    REQUIRE(testString.back() == '5');

    STATIC_REQUIRE(testString.front() == '1');
    STATIC_REQUIRE(testString.back() == '5');
  }

  SECTION("Mixed content") {
    constexpr CStringView testString("123Hello456");

    REQUIRE(testString.front() == '1');
    REQUIRE(testString.back() == '6');

    STATIC_REQUIRE(testString.front() == '1');
    STATIC_REQUIRE(testString.back() == '6');
  }

  SECTION("Long strings") {
    constexpr CStringView testString("This is a very long string for performance testing");

    REQUIRE(testString.front() == 'T');
    REQUIRE(testString.back() == 'g');

    STATIC_REQUIRE(testString.front() == 'T');
    STATIC_REQUIRE(testString.back() == 'g');
  }

  SECTION("Case sensitivity") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.front() == 'H'); // Uppercase
    REQUIRE(testString.back() == 'd'); // Lowercase

    STATIC_REQUIRE(testString.front() == 'H');
    STATIC_REQUIRE(testString.back() == 'd');
  }

  SECTION("Whitespace handling") {
    constexpr CStringView testString(" Hello ");

    REQUIRE(testString.front() == ' ');
    REQUIRE(testString.back() == ' ');

    STATIC_REQUIRE(testString.front() == ' ');
    STATIC_REQUIRE(testString.back() == ' ');
  }

  SECTION("Constexpr operations") {
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("World");
    constexpr CStringView str3("Test");

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

TEST_CASE("CStringView data", "[core][c_string_view]") {
  SECTION("Basic data access") {
    constexpr CStringView testString("Hello World");
    constexpr CStringView emptyString("");
    constexpr CStringView singleChar("A");

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
    constexpr CStringView testString("Stability Test");
    constexpr CStringView copy1(testString);
    constexpr CStringView copy2(testString);

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
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

    // Test data() with empty strings
    REQUIRE(std::strcmp(emptyString.data(), "") == 0);
    REQUIRE(std::strcmp(defaultString.data(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(cstrcmp(emptyString.data(), "") == 0);
    STATIC_REQUIRE(cstrcmp(defaultString.data(), "") == 0);
  }
}

TEST_CASE("CStringView c_str method", "[core][c_string_view]") {
  SECTION("Basic c_str access") {
    constexpr CStringView testString("Hello World");
    constexpr CStringView emptyString("");
    constexpr CStringView singleChar("A");

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
    constexpr CStringView testString("Stability Test");
    constexpr CStringView copy1(testString);
    constexpr CStringView copy2(testString);

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
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

    // Test c_str() with empty strings
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);

    // Compile-time checks
    STATIC_REQUIRE(emptyString.c_str() == emptyString.data());
    STATIC_REQUIRE(defaultString.c_str() == defaultString.data());
  }
}

TEST_CASE("CStringView empty method", "[core][c_string_view]") {
  SECTION("Basic empty check") {
    constexpr CStringView nonEmptyString("Hello World");
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

    REQUIRE_FALSE(nonEmptyString.empty());
    REQUIRE(emptyString.empty());
    REQUIRE(defaultString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(nonEmptyString.empty());
    STATIC_REQUIRE(emptyString.empty());
    STATIC_REQUIRE(defaultString.empty());
  }

  SECTION("Single character strings") {
    constexpr CStringView singleChar("A");
    constexpr CStringView emptyString("");

    REQUIRE_FALSE(singleChar.empty());
    REQUIRE(emptyString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(singleChar.empty());
    STATIC_REQUIRE(emptyString.empty());
  }

  SECTION("Different capacities") {
    constexpr CStringView smallString("Hi");
    constexpr CStringView mediumString("Hello World");
    constexpr CStringView largeString("This is a longer string");
    constexpr CStringView emptySmall("");
    constexpr CStringView emptyMedium("");
    constexpr CStringView emptyLarge("");

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
    constexpr CStringView newlineString("Hello\nWorld");
    constexpr CStringView tabString("Hello\tWorld");
    constexpr CStringView specialString("!@#$%^&*()");
    constexpr CStringView emptyString("");

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
    constexpr CStringView unicodeString("Привет мир");
    constexpr CStringView emojiString("Hello 🌍 World");
    constexpr CStringView mixedString("Hello 世界");
    constexpr CStringView emptyString("");

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
    constexpr CStringView numericStringView("12345");
    constexpr CStringView floatString("3.14159");
    constexpr CStringView hexString("0xABCD");
    constexpr CStringView emptyString("");

    REQUIRE_FALSE(numericStringView.empty());
    REQUIRE_FALSE(floatString.empty());
    REQUIRE_FALSE(hexString.empty());
    REQUIRE(emptyString.empty());

    // Compile-time checks
    STATIC_REQUIRE_FALSE(numericStringView.empty());
    STATIC_REQUIRE_FALSE(floatString.empty());
    STATIC_REQUIRE_FALSE(hexString.empty());
    STATIC_REQUIRE(emptyString.empty());
  }

  SECTION("Mixed content") {
    constexpr CStringView mixedString("Hello123World!@#");
    constexpr CStringView complexString("Test\n123\t!@#");
    constexpr CStringView longString("This is a very long string with mixed content 123!@#");
    constexpr CStringView emptyString("");

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
    constexpr CStringView maxString("123456789012345"); // 15 characters
    constexpr CStringView maxSmall("1234567"); // 7 characters
    constexpr CStringView maxTiny("123"); // 3 characters
    constexpr CStringView emptyString("");

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
    constexpr CStringView singleChar("A");
    constexpr CStringView twoChars("AB");
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

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

TEST_CASE("CStringView size method", "[core][c_string_view]") {
  SECTION("Basic size check") {
    constexpr CStringView testString("Hello World");
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

    REQUIRE(testString.size() == 11);
    REQUIRE(emptyString.size() == 0);
    REQUIRE(defaultString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.size() == 11);
    STATIC_REQUIRE(emptyString.size() == 0);
    STATIC_REQUIRE(defaultString.size() == 0);
  }

  SECTION("Single character strings") {
    constexpr CStringView singleChar("A");
    constexpr CStringView emptyString("");

    REQUIRE(singleChar.size() == 1);
    REQUIRE(emptyString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(singleChar.size() == 1);
    STATIC_REQUIRE(emptyString.size() == 0);
  }

  SECTION("Different capacities") {
    constexpr CStringView smallString("Hi");
    constexpr CStringView mediumString("Hello World");
    constexpr CStringView largeString("This is a longer string");
    constexpr CStringView emptySmall("");
    constexpr CStringView emptyMedium("");
    constexpr CStringView emptyLarge("");

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
    constexpr CStringView newlineString("Hello\nWorld");
    constexpr CStringView tabString("Hello\tWorld");
    constexpr CStringView specialString("!@#$%^&*()");
    constexpr CStringView emptyString("");

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
    constexpr CStringView unicodeString("Привет мир");
    constexpr CStringView emojiString("Hello 🌍 World");
    constexpr CStringView mixedString("Hello 世界");
    constexpr CStringView emptyString("");

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
    constexpr CStringView numericStringView("12345");
    constexpr CStringView floatString("3.14159");
    constexpr CStringView hexString("0xABCD");
    constexpr CStringView emptyString("");

    REQUIRE(numericStringView.size() == 5);
    REQUIRE(floatString.size() == 7);
    REQUIRE(hexString.size() == 6);
    REQUIRE(emptyString.size() == 0);

    // Compile-time checks
    STATIC_REQUIRE(numericStringView.size() == 5);
    STATIC_REQUIRE(floatString.size() == 7);
    STATIC_REQUIRE(hexString.size() == 6);
    STATIC_REQUIRE(emptyString.size() == 0);
  }

  SECTION("Mixed content") {
    constexpr CStringView mixedString("Hello123World!@#");
    constexpr CStringView complexString("Test\n123\t!@#");
    constexpr CStringView longString("This is a very long string with mixed content 123!@#");
    constexpr CStringView emptyString("");

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
    constexpr CStringView maxString("123456789012345"); // 15 characters
    constexpr CStringView maxSmall("1234567"); // 7 characters
    constexpr CStringView maxTiny("123"); // 3 characters
    constexpr CStringView emptyString("");

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
    constexpr CStringView singleChar("A");
    constexpr CStringView twoChars("AB");
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

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

TEST_CASE("CStringView utf8_size", "[core][c_string_view]") {
  SECTION("ASCII strings") {
    constexpr CStringView asciiString("Hello World");
    constexpr CStringView emptyString("");
    constexpr CStringView singleChar("A");

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

    constexpr CStringView cyrillicStringView(cyrillicText.data());

    REQUIRE(cyrillicStringView.size() == 19);
    REQUIRE(cyrillicStringView.utf8_size() == 10);
  }

  SECTION("Mixed ASCII and UTF-8") {
    // "Hello 世界" in UTF-8
    static constexpr std::array<char, 13> mixedText{{char(0x48), char(0x65), char(0x6C), char(0x6C), char(0x6F),
                                                     char(0x20), char(0xE4), char(0xB8), char(0x96), char(0xE7),
                                                     char(0x95), char(0x8C), char(0x00)}};

    constexpr CStringView mixedString(mixedText.data());

    REQUIRE(mixedString.size() == 12);
    REQUIRE(mixedString.utf8_size() == 8); // 6 ASCII + 2 Chinese characters
  }

  SECTION("Emoji characters") {
    // "Hello 🌍" in UTF-8
    static constexpr std::array<char, 11> emojiText{{char(0x48), char(0x65), char(0x6C), char(0x6C), char(0x6F),
                                                     char(0x20), char(0xF0), char(0x9F), char(0x8C), char(0x8D),
                                                     char(0x00)}};

    constexpr CStringView emojiString(emojiText.data());

    REQUIRE(emojiString.size() == 10);
    REQUIRE(emojiString.utf8_size() == 7); // 6 ASCII + 1 emoji
  }

  SECTION("Special characters") {
    constexpr CStringView specialString("!@#$%^&*()");
    constexpr CStringView numericStringView("1234567890");
    constexpr CStringView punctuationString(".,;:!?");

    REQUIRE(specialString.utf8_size() == 10);
    REQUIRE(numericStringView.utf8_size() == 10);
    REQUIRE(punctuationString.utf8_size() == 6);

    // Special characters are ASCII, so utf8_size equals size
    REQUIRE(specialString.utf8_size() == specialString.size());
    REQUIRE(numericStringView.utf8_size() == numericStringView.size());
    REQUIRE(punctuationString.utf8_size() == punctuationString.size());
  }

  SECTION("Edge cases") {
    constexpr CStringView singleByte("A");
    constexpr CStringView twoByte("А"); // Cyrillic A
    constexpr CStringView threeByte("中"); // Chinese character
    constexpr CStringView fourByte("🌍"); // Emoji

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

    constexpr CStringView longString(longUtf8Text.data());

    REQUIRE(longString.size() == 66); // 66 bytes
    REQUIRE(longString.utf8_size() == 43); // 43 characters
  }
}

TEST_CASE("CStringView length", "[core][c_string_view]") {
  SECTION("Basic length check") {
    constexpr CStringView testString("Hello World");
    constexpr CStringView emptyString("");
    constexpr CStringView singleChar("A");

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
    constexpr CStringView smallString("Hi");
    constexpr CStringView mediumString("Hello World");
    constexpr CStringView largeString("This is a longer string");
    constexpr CStringView emptySmall("");
    constexpr CStringView emptyMedium("");
    constexpr CStringView emptyLarge("");

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
    constexpr CStringView newlineString("Hello\nWorld");
    constexpr CStringView tabString("Hello\tWorld");
    constexpr CStringView specialString("!@#$%^&*()");
    constexpr CStringView emptyString("");

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
    constexpr CStringView unicodeString("Привет мир");
    constexpr CStringView emojiString("Hello 🌍 World");
    constexpr CStringView mixedString("Hello 世界");
    constexpr CStringView emptyString("");

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
    constexpr CStringView numericStringView("12345");
    constexpr CStringView floatString("3.14159");
    constexpr CStringView hexString("0xABCD");
    constexpr CStringView emptyString("");

    REQUIRE(numericStringView.length() == 5);
    REQUIRE(floatString.length() == 7);
    REQUIRE(hexString.length() == 6);
    REQUIRE(emptyString.length() == 0);

    // length() should equal size() for all strings
    REQUIRE(numericStringView.length() == numericStringView.size());
    REQUIRE(floatString.length() == floatString.size());
    REQUIRE(hexString.length() == hexString.size());
    REQUIRE(emptyString.length() == emptyString.size());

    STATIC_REQUIRE(numericStringView.length() == 5);
    STATIC_REQUIRE(floatString.length() == 7);
    STATIC_REQUIRE(hexString.length() == 6);
  }

  SECTION("Mixed content") {
    constexpr CStringView mixedString("Hello123World!@#");
    constexpr CStringView complexString("Test\n123\t!@#");
    constexpr CStringView longString("This is a very long string with mixed content 123!@#");
    constexpr CStringView emptyString("");

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
    constexpr CStringView singleChar("A");
    constexpr CStringView twoChars("AB");
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

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

TEST_CASE("CStringView max_size", "[core][c_string_view]") {
  SECTION("Basic max_size check") {
    constexpr CStringView smallString("Hi");
    constexpr CStringView mediumString("Hello World");
    constexpr CStringView largeString("This is a longer string");
    constexpr CStringView extraLargeString("This is an even longer string for testing");

    REQUIRE(smallString.max_size() == 2);
    REQUIRE(mediumString.max_size() == 11);
    REQUIRE(largeString.max_size() == 23);
    REQUIRE(extraLargeString.max_size() == 41);

    STATIC_REQUIRE(smallString.max_size() == 2);
    STATIC_REQUIRE(mediumString.max_size() == 11);
    STATIC_REQUIRE(largeString.max_size() == 23);
    STATIC_REQUIRE(extraLargeString.max_size() == 41);
  }

  SECTION("Empty string") {
    constexpr CStringView empty("");

    REQUIRE(empty.max_size() == 0);

    STATIC_REQUIRE(empty.max_size() == 0);
  }

  SECTION("Default constructed string") {
    constexpr CStringView defaultString;

    REQUIRE(defaultString.max_size() == 0);

    STATIC_REQUIRE(defaultString.max_size() == 0);
  }

  SECTION("Single character string") {
    constexpr CStringView single("A");

    REQUIRE(single.max_size() == 1);

    STATIC_REQUIRE(single.max_size() == 1);
  }
}

TEST_CASE("CStringView capacity", "[core][c_string_view]") {
  SECTION("Basic capacity check") {
    constexpr CStringView smallString("Hi");
    constexpr CStringView mediumString("Hello World");
    constexpr CStringView largeString("This is a longer string");
    constexpr CStringView extraLargeString("This is an even longer string for testing");

    REQUIRE(smallString.capacity() == 2);
    REQUIRE(mediumString.capacity() == 11);
    REQUIRE(largeString.capacity() == 23);
    REQUIRE(extraLargeString.capacity() == 41);

    STATIC_REQUIRE(smallString.capacity() == 2);
    STATIC_REQUIRE(mediumString.capacity() == 11);
    STATIC_REQUIRE(largeString.capacity() == 23);
    STATIC_REQUIRE(extraLargeString.capacity() == 41);
  }

  SECTION("Empty string") {
    constexpr CStringView empty("");

    REQUIRE(empty.capacity() == 0);

    STATIC_REQUIRE(empty.capacity() == 0);
  }

  SECTION("Default constructed string") {
    constexpr CStringView defaultString;

    REQUIRE(defaultString.capacity() == 0);

    STATIC_REQUIRE(defaultString.capacity() == 0);
  }

  SECTION("Single character string") {
    constexpr CStringView single("A");

    REQUIRE(single.capacity() == 1);

    STATIC_REQUIRE(single.capacity() == 1);
  }
}

TEST_CASE("CStringView clear", "[core][c_string_view]") {
  SECTION("Basic clear functionality") {
    CStringView testString("Hello World");

    REQUIRE_FALSE(testString.empty());
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.clear();

    REQUIRE(testString.empty());
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SECTION("Clear empty string") {
    CStringView emptyString("");

    REQUIRE(emptyString.empty());
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);

    emptyString.clear();

    REQUIRE(emptyString.empty());
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
  }

  SECTION("Clear default constructed string") {
    CStringView defaultString;

    REQUIRE(defaultString.empty());
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);

    defaultString.clear();

    REQUIRE(defaultString.empty());
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);
  }

  SECTION("Clear single character string") {
    CStringView singleChar("A");

    REQUIRE_FALSE(singleChar.empty());
    REQUIRE(std::strcmp(singleChar.c_str(), "A") == 0);

    singleChar.clear();

    REQUIRE(singleChar.empty());
    REQUIRE(std::strcmp(singleChar.c_str(), "") == 0);
  }

  SECTION("Clear longer string") {
    CStringView maxString("1234567890");

    REQUIRE_FALSE(maxString.empty());
    REQUIRE(std::strcmp(maxString.c_str(), "1234567890") == 0);

    maxString.clear();

    REQUIRE(maxString.empty());
    REQUIRE(std::strcmp(maxString.c_str(), "") == 0);
  }

  SECTION("Clear different capacities") {
    CStringView smallString("Hi");
    CStringView mediumString("Hello World");
    CStringView largeString("This is a longer string");
    CStringView extraLargeString("This is an even longer string for testing");

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
    REQUIRE(std::strcmp(smallString.c_str(), "") == 0);
    REQUIRE(std::strcmp(mediumString.c_str(), "") == 0);
    REQUIRE(std::strcmp(largeString.c_str(), "") == 0);
    REQUIRE(std::strcmp(extraLargeString.c_str(), "") == 0);
  }

  SECTION("Clear special characters") {
    CStringView newlineString("Hello\nWorld");
    CStringView tabString("Hello\tWorld");
    CStringView specialString("!@#$%^&*()");

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
    CStringView unicodeString("Привет мир");
    CStringView emojiString("Hello 🌍 World");
    CStringView mixedString("Hello 世界");

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

  SECTION("Clear and reassignment") {
    CStringView testString("Original");

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
    CStringView testString("Test");

    // First clear
    testString.clear();
    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);

    // Assign new content
    testString = "New";
    REQUIRE_FALSE(testString.empty());
    REQUIRE(testString.size() == 3);

    // Second clear
    testString.clear();
    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);

    // Third clear (should be idempotent)
    testString.clear();
    REQUIRE(testString.empty());
    REQUIRE(testString.size() == 0);
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }
}

TEST_CASE("CStringView swap", "[core][c_string_view]") {
  SECTION("Swap two different strings") {
    CStringView string1("Hello");
    CStringView string2("World");

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
    CStringView string1("Short");
    CStringView string2("This is a much longer string");

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
    CStringView string1("Hello World");
    CStringView string2("");

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
    CStringView string1("");
    CStringView string2("");

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
    CStringView string1("Hello World");

    REQUIRE(string1.size() == 11);
    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);

    string1.swap(string1);

    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);
    REQUIRE(string1.size() == 11);
  }

  SECTION("Swap with single character strings") {
    CStringView string1("A");
    CStringView string2("B");

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

  SECTION("Chained swap operations") {
    CStringView string1("First");
    CStringView string2("Second");
    CStringView string3("Third");

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
    CStringView string1("AAA");
    CStringView string2("BBB");

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
    CStringView string1("Hello\n\tWorld!");
    CStringView string2("Test!@#$%^&*()");

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
    CStringView string1("Hello 世界");
    CStringView string2("Привет мир");

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
    CStringView string1("12345");
    CStringView string2("67890");

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
    CStringView string1("Hello123World!@#");
    CStringView string2("Test\n456\t!@#$");

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

TEST_CASE("CStringView find", "[core][c_string_view]") {
  SECTION("Find CStringView substring") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find(CStringView("World")) == 6);
    REQUIRE(testString.find(CStringView("Hello")) == 0);
    REQUIRE(testString.find(CStringView("lo Wo")) == 3);
    REQUIRE(testString.find(CStringView("xyz")) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(CStringView("World")) == 6);
    STATIC_REQUIRE(testString.find(CStringView("Hello")) == 0);
    STATIC_REQUIRE(testString.find(CStringView("lo Wo")) == 3);
    STATIC_REQUIRE(testString.find(CStringView("xyz")) == CStringView::npos);
  }

  SECTION("Find StringLike substring") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find(std::string("World")) == 6);
    REQUIRE(testString.find(std::string("Hello")) == 0);
    REQUIRE(testString.find(std::string("lo Wo")) == 3);
    REQUIRE(testString.find(std::string("xyz")) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(CStringView("World")) == 6);
    STATIC_REQUIRE(testString.find(CStringView("Hello")) == 0);
    STATIC_REQUIRE(testString.find(CStringView("lo Wo")) == 3);
    STATIC_REQUIRE(testString.find(CStringView("xyz")) == CStringView::npos);
  }

  SECTION("Find C string substring") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("lo Wo") == 3);
    REQUIRE(testString.find("xyz") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("World") == 6);
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find("lo Wo") == 3);
    STATIC_REQUIRE(testString.find("xyz") == CStringView::npos);
  }

  SECTION("Find character") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find('H') == 0);
    REQUIRE(testString.find('l') == 2);
    REQUIRE(testString.find('o') == 4);
    REQUIRE(testString.find('W') == 6);
    REQUIRE(testString.find('d') == 10);
    REQUIRE(testString.find('x') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find('H') == 0);
    STATIC_REQUIRE(testString.find('l') == 2);
    STATIC_REQUIRE(testString.find('o') == 4);
    STATIC_REQUIRE(testString.find('W') == 6);
    STATIC_REQUIRE(testString.find('d') == 10);
    STATIC_REQUIRE(testString.find('x') == CStringView::npos);
  }

  SECTION("Find with position parameter") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == 12);
    REQUIRE(testString.find("Hello", 13) == CStringView::npos);
    REQUIRE(testString.find('l', 0) == 2);
    REQUIRE(testString.find('l', 3) == 3);
    REQUIRE(testString.find('l', 4) == 9);
    REQUIRE(testString.find('l', 10) == 14);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("Hello", 0) == 0);
    STATIC_REQUIRE(testString.find("Hello", 1) == 12);
    STATIC_REQUIRE(testString.find("Hello", 13) == CStringView::npos);
    STATIC_REQUIRE(testString.find('l', 0) == 2);
    STATIC_REQUIRE(testString.find('l', 3) == 3);
    STATIC_REQUIRE(testString.find('l', 4) == 9);
    STATIC_REQUIRE(testString.find('l', 10) == 14);
  }

  SECTION("Find empty substring") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find(CStringView("")) == 0);
    REQUIRE(testString.find(std::string("")) == 0);
    REQUIRE(testString.find("") == 0);
    REQUIRE(testString.find("", 5) == 5);
    REQUIRE(testString.find("", 11) == 11);
    REQUIRE(testString.find("", 12) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(CStringView("")) == 0);
    STATIC_REQUIRE(testString.find(FixedString<8>("")) == 0);
    STATIC_REQUIRE(testString.find("") == 0);
    STATIC_REQUIRE(testString.find("", 5) == 5);
    STATIC_REQUIRE(testString.find("", 11) == 11);
    STATIC_REQUIRE(testString.find("", 12) == CStringView::npos);
  }

  SECTION("Find in empty string") {
    constexpr CStringView testString("");

    REQUIRE(testString.find(CStringView("Hello")) == CStringView::npos);
    REQUIRE(testString.find(std::string("Hello")) == CStringView::npos);
    REQUIRE(testString.find("Hello") == CStringView::npos);
    REQUIRE(testString.find('H') == CStringView::npos);
    REQUIRE(testString.find("") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(CStringView("Hello")) == CStringView::npos);
    STATIC_REQUIRE(testString.find(FixedString<8>("Hello")) == CStringView::npos);
    STATIC_REQUIRE(testString.find("Hello") == CStringView::npos);
    STATIC_REQUIRE(testString.find('H') == CStringView::npos);
    STATIC_REQUIRE(testString.find("") == 0);
  }

  SECTION("Find with position beyond string size") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find("World", 10) == CStringView::npos);
    REQUIRE(testString.find('H', 10) == CStringView::npos);
    REQUIRE(testString.find("", 10) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("World", 10) == CStringView::npos);
    STATIC_REQUIRE(testString.find('H', 10) == CStringView::npos);
    STATIC_REQUIRE(testString.find("", 10) == CStringView::npos);
  }

  SECTION("Find substring at end") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("d") == 10);
    REQUIRE(testString.find("ld") == 9);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("World") == 6);
    STATIC_REQUIRE(testString.find("d") == 10);
    STATIC_REQUIRE(testString.find("ld") == 9);
  }

  SECTION("Find substring at beginning") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("H") == 0);
    REQUIRE(testString.find("He") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find("H") == 0);
    STATIC_REQUIRE(testString.find("He") == 0);
  }

  SECTION("Find overlapping substrings") {
    constexpr CStringView testString("ababab");

    REQUIRE(testString.find("ab") == 0);
    REQUIRE(testString.find("ab", 1) == 2);
    REQUIRE(testString.find("ab", 3) == 4);
    REQUIRE(testString.find("ab", 5) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("ab") == 0);
    STATIC_REQUIRE(testString.find("ab", 1) == 2);
    STATIC_REQUIRE(testString.find("ab", 3) == 4);
    STATIC_REQUIRE(testString.find("ab", 5) == CStringView::npos);
  }

  SECTION("Find with repeated characters") {
    constexpr CStringView testString("aaaaa");

    REQUIRE(testString.find("aa") == 0);
    REQUIRE(testString.find("aa", 1) == 1);
    REQUIRE(testString.find("aa", 2) == 2);
    REQUIRE(testString.find("aa", 3) == 3);
    REQUIRE(testString.find("aa", 4) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("aa") == 0);
    STATIC_REQUIRE(testString.find("aa", 1) == 1);
    STATIC_REQUIRE(testString.find("aa", 2) == 2);
    STATIC_REQUIRE(testString.find("aa", 3) == 3);
    STATIC_REQUIRE(testString.find("aa", 4) == CStringView::npos);
  }

  SECTION("Find case sensitivity") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("hello") == CStringView::npos);
    REQUIRE(testString.find("WORLD") == CStringView::npos);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("World") == 6);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("hello") == CStringView::npos);
    STATIC_REQUIRE(testString.find("WORLD") == CStringView::npos);
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find("World") == 6);
  }

  SECTION("Find with different CStringView capacities") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find(CStringView("World")) == 6);

    // Compile-time checks
    STATIC_REQUIRE(testString.find(CStringView("World")) == 6);
  }

  SECTION("Find with exact match") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find("Hello", 0) == 0);
    STATIC_REQUIRE(testString.find("Hello", 1) == CStringView::npos);
  }

  SECTION("Find with single character string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.find("A") == 0);
    REQUIRE(testString.find('A') == 0);
    REQUIRE(testString.find("B") == CStringView::npos);
    REQUIRE(testString.find('B') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("A") == 0);
    STATIC_REQUIRE(testString.find('A') == 0);
    STATIC_REQUIRE(testString.find("B") == CStringView::npos);
    STATIC_REQUIRE(testString.find('B') == CStringView::npos);
  }

  SECTION("Find with special characters") {
    constexpr CStringView testString("Hello\n\tWorld!");

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
    constexpr CStringView testString("Hello 世界");

    REQUIRE(testString.find("世界") == 6);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find(" ") == 5);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("世界") == 6);
    STATIC_REQUIRE(testString.find("Hello") == 0);
    STATIC_REQUIRE(testString.find(" ") == 5);
  }

  SECTION("Find with numeric content") {
    constexpr CStringView testString("12345Hello67890");

    REQUIRE(testString.find("12345") == 0);
    REQUIRE(testString.find("Hello") == 5);
    REQUIRE(testString.find("67890") == 10);
    REQUIRE(testString.find("456") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("12345") == 0);
    STATIC_REQUIRE(testString.find("Hello") == 5);
    STATIC_REQUIRE(testString.find("67890") == 10);
    STATIC_REQUIRE(testString.find("456") == CStringView::npos);
  }

  SECTION("Find with mixed content") {
    constexpr CStringView testString("Hello123World!@#");

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
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == CStringView::npos);
    REQUIRE(testString.find("World", 6) == 6);
    REQUIRE(testString.find("World", 7) == CStringView::npos);
    REQUIRE(testString.find("", 0) == 0);
    REQUIRE(testString.find("", 11) == 11);
    REQUIRE(testString.find("", 12) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find("Hello", 0) == 0);
    STATIC_REQUIRE(testString.find("Hello", 1) == CStringView::npos);
    STATIC_REQUIRE(testString.find("World", 6) == 6);
    STATIC_REQUIRE(testString.find("World", 7) == CStringView::npos);
    STATIC_REQUIRE(testString.find("", 0) == 0);
    STATIC_REQUIRE(testString.find("", 11) == 11);
    STATIC_REQUIRE(testString.find("", 12) == CStringView::npos);
  }
}

TEST_CASE("CStringView rfind", "[core][c_string_view]") {
  SECTION("Rfind CStringView substring") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind(CStringView("Hello")) == 12);
    REQUIRE(testString.rfind(CStringView("World")) == 6);
    REQUIRE(testString.rfind(CStringView("lo")) == 15);
    REQUIRE(testString.rfind(CStringView("xyz")) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind(CStringView("Hello")) == 12);
    STATIC_REQUIRE(testString.rfind(CStringView("World")) == 6);
    STATIC_REQUIRE(testString.rfind(CStringView("lo")) == 15);
    STATIC_REQUIRE(testString.rfind(CStringView("xyz")) == CStringView::npos);
  }

  SECTION("Rfind StringLike substring") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind(std::string("Hello")) == 12);
    REQUIRE(testString.rfind(std::string("World")) == 6);
    REQUIRE(testString.rfind(std::string("lo")) == 15);
    REQUIRE(testString.rfind(std::string("xyz")) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind(CStringView("Hello")) == 12);
    STATIC_REQUIRE(testString.rfind(CStringView("World")) == 6);
    STATIC_REQUIRE(testString.rfind(CStringView("lo")) == 15);
    STATIC_REQUIRE(testString.rfind(CStringView("xyz")) == CStringView::npos);
  }

  SECTION("Rfind C string substring") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("lo") == 15);
    REQUIRE(testString.rfind("xyz") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello") == 12);
    STATIC_REQUIRE(testString.rfind("World") == 6);
    STATIC_REQUIRE(testString.rfind("lo") == 15);
    STATIC_REQUIRE(testString.rfind("xyz") == CStringView::npos);
  }

  SECTION("Rfind character") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind('H') == 12);
    REQUIRE(testString.rfind('l') == 15);
    REQUIRE(testString.rfind('o') == 16);
    REQUIRE(testString.rfind('W') == 6);
    REQUIRE(testString.rfind('d') == 10);
    REQUIRE(testString.rfind('x') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind('H') == 12);
    STATIC_REQUIRE(testString.rfind('l') == 15);
    STATIC_REQUIRE(testString.rfind('o') == 16);
    STATIC_REQUIRE(testString.rfind('W') == 6);
    STATIC_REQUIRE(testString.rfind('d') == 10);
    STATIC_REQUIRE(testString.rfind('x') == CStringView::npos);
  }

  SECTION("Rfind with position parameter") {
    constexpr CStringView testString("Hello World Hello");

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
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.rfind(CStringView("")) == 11);
    REQUIRE(testString.rfind(std::string("")) == 11);
    REQUIRE(testString.rfind("") == 11);
    REQUIRE(testString.rfind("", 5) == 5);
    REQUIRE(testString.rfind("", 0) == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind(CStringView("")) == 11);
    STATIC_REQUIRE(testString.rfind(FixedString<4>("")) == 11);
    STATIC_REQUIRE(testString.rfind("") == 11);
    STATIC_REQUIRE(testString.rfind("", 5) == 5);
    STATIC_REQUIRE(testString.rfind("", 0) == 0);
  }

  SECTION("Rfind in empty string") {
    constexpr CStringView testString("");

    REQUIRE(testString.rfind(CStringView("Hello")) == CStringView::npos);
    REQUIRE(testString.rfind(std::string("Hello")) == CStringView::npos);
    REQUIRE(testString.rfind("Hello") == CStringView::npos);
    REQUIRE(testString.rfind('H') == CStringView::npos);
    REQUIRE(testString.rfind("") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind(CStringView("Hello")) == CStringView::npos);
    STATIC_REQUIRE(testString.rfind(FixedString<8>("Hello")) == CStringView::npos);
    STATIC_REQUIRE(testString.rfind("Hello") == CStringView::npos);
    STATIC_REQUIRE(testString.rfind('H') == CStringView::npos);
    STATIC_REQUIRE(testString.rfind("") == 0);
  }

  SECTION("Rfind substring at end") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("d") == 10);
    REQUIRE(testString.rfind("ld") == 9);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("World") == 6);
    STATIC_REQUIRE(testString.rfind("d") == 10);
    STATIC_REQUIRE(testString.rfind("ld") == 9);
  }

  SECTION("Rfind substring at beginning") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("H") == 12);
    REQUIRE(testString.rfind("He") == 12);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello") == 12);
    STATIC_REQUIRE(testString.rfind("H") == 12);
    STATIC_REQUIRE(testString.rfind("He") == 12);
  }

  SECTION("Rfind overlapping substrings") {
    constexpr CStringView testString("ababab");

    REQUIRE(testString.rfind("ab") == 4);
    REQUIRE(testString.rfind("ab", 3) == 2);
    REQUIRE(testString.rfind("ab", 1) == 0);
    REQUIRE(testString.rfind("ab", 0) == 0);
    REQUIRE(testString.rfind("ab", 5) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("ab") == 4);
    STATIC_REQUIRE(testString.rfind("ab", 3) == 2);
    STATIC_REQUIRE(testString.rfind("ab", 1) == 0);
    STATIC_REQUIRE(testString.rfind("ab", 0) == 0);
    STATIC_REQUIRE(testString.rfind("ab", 5) == CStringView::npos);
  }

  SECTION("Rfind with repeated characters") {
    constexpr CStringView testString("aaaaa");

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
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind("hello") == CStringView::npos);
    REQUIRE(testString.rfind("WORLD") == CStringView::npos);
    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("hello") == CStringView::npos);
    STATIC_REQUIRE(testString.rfind("WORLD") == CStringView::npos);
    STATIC_REQUIRE(testString.rfind("Hello") == 12);
    STATIC_REQUIRE(testString.rfind("World") == 6);
  }

  SECTION("Rfind with exact match") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.rfind("Hello") == 0);
    REQUIRE(testString.rfind("Hello", 0) == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello") == 0);
    STATIC_REQUIRE(testString.rfind("Hello", 0) == 0);
  }

  SECTION("Rfind with single character string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.rfind("A") == 0);
    REQUIRE(testString.rfind('A') == 0);
    REQUIRE(testString.rfind("B") == CStringView::npos);
    REQUIRE(testString.rfind('B') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("A") == 0);
    STATIC_REQUIRE(testString.rfind('A') == 0);
    STATIC_REQUIRE(testString.rfind("B") == CStringView::npos);
    STATIC_REQUIRE(testString.rfind('B') == CStringView::npos);
  }

  SECTION("Rfind with position 0") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.rfind("Hello", 0) == 0);
    REQUIRE(testString.rfind("World", 0) == CStringView::npos);
    REQUIRE(testString.rfind('H', 0) == 0);
    REQUIRE(testString.rfind('W', 0) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello", 0) == 0);
    STATIC_REQUIRE(testString.rfind("World", 0) == CStringView::npos);
    STATIC_REQUIRE(testString.rfind('H', 0) == 0);
    STATIC_REQUIRE(testString.rfind('W', 0) == CStringView::npos);
  }

  SECTION("Rfind with substring longer than string") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.rfind("Hello World") == CStringView::npos);
    REQUIRE(testString.rfind("Hello World", 10) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.rfind("Hello World") == CStringView::npos);
    STATIC_REQUIRE(testString.rfind("Hello World", 10) == CStringView::npos);
  }

  SECTION("Rfind with multiple occurrences") {
    constexpr CStringView testString("abababab");

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
    constexpr CStringView testString("Hello World Hello");

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

TEST_CASE("CStringView find_first_of", "[core][c_string_view]") {
  SECTION("Find first of CStringView characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of(CStringView("aeiou")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_of(CStringView("H")) == 0);
    REQUIRE(testString.find_first_of(CStringView("d")) == 10);
    REQUIRE(testString.find_first_of(CStringView("xyz")) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(CStringView("aeiou")) == 1);
    STATIC_REQUIRE(testString.find_first_of(CStringView("H")) == 0);
    STATIC_REQUIRE(testString.find_first_of(CStringView("d")) == 10);
    STATIC_REQUIRE(testString.find_first_of(CStringView("xyz")) == CStringView::npos);
  }

  SECTION("Find first of StringLike characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of(std::string("aeiou")) == 1);
    REQUIRE(testString.find_first_of(std::string("H")) == 0);
    REQUIRE(testString.find_first_of(std::string("d")) == 10);
    REQUIRE(testString.find_first_of(std::string("xyz")) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(CStringView("aeiou")) == 1);
    STATIC_REQUIRE(testString.find_first_of(CStringView("H")) == 0);
    STATIC_REQUIRE(testString.find_first_of(CStringView("d")) == 10);
    STATIC_REQUIRE(testString.find_first_of(CStringView("xyz")) == CStringView::npos);
  }

  SECTION("Find first of C string characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou") == 1);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("d") == 10);
    REQUIRE(testString.find_first_of("xyz") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou") == 1);
    STATIC_REQUIRE(testString.find_first_of("H") == 0);
    STATIC_REQUIRE(testString.find_first_of("d") == 10);
    STATIC_REQUIRE(testString.find_first_of("xyz") == CStringView::npos);
  }

  SECTION("Find first of single character") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of('H') == 0);
    REQUIRE(testString.find_first_of('e') == 1);
    REQUIRE(testString.find_first_of('l') == 2);
    REQUIRE(testString.find_first_of('o') == 4);
    REQUIRE(testString.find_first_of('W') == 6);
    REQUIRE(testString.find_first_of('d') == 10);
    REQUIRE(testString.find_first_of('x') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of('H') == 0);
    STATIC_REQUIRE(testString.find_first_of('e') == 1);
    STATIC_REQUIRE(testString.find_first_of('l') == 2);
    STATIC_REQUIRE(testString.find_first_of('o') == 4);
    STATIC_REQUIRE(testString.find_first_of('W') == 6);
    STATIC_REQUIRE(testString.find_first_of('d') == 10);
    STATIC_REQUIRE(testString.find_first_of('x') == CStringView::npos);
  }

  SECTION("Find first of with position parameter") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("aeiou", 2) == 4);
    REQUIRE(testString.find_first_of("aeiou", 5) == 7);
    REQUIRE(testString.find_first_of("aeiou", 8) == 13);
    REQUIRE(testString.find_first_of("aeiou", 14) == 16);
    REQUIRE(testString.find_first_of("aeiou", 17) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 2) == 4);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 5) == 7);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 8) == 13);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 14) == 16);
    STATIC_REQUIRE(testString.find_first_of("aeiou", 17) == CStringView::npos);
  }

  SECTION("Find first of empty character set") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of(CStringView("")) == CStringView::npos);
    REQUIRE(testString.find_first_of(std::string("")) == CStringView::npos);
    REQUIRE(testString.find_first_of("") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(CStringView("")) == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of(FixedString<4>("")) == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of("") == CStringView::npos);
  }

  SECTION("Find first of in empty string") {
    constexpr CStringView testString("");

    REQUIRE(testString.find_first_of(CStringView("aeiou")) == CStringView::npos);
    REQUIRE(testString.find_first_of(std::string("aeiou")) == CStringView::npos);
    REQUIRE(testString.find_first_of("aeiou") == CStringView::npos);
    REQUIRE(testString.find_first_of('a') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(CStringView("aeiou")) == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of(FixedString<8>("aeiou")) == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of("aeiou") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of('a') == CStringView::npos);
  }

  SECTION("Find first of with position beyond string size") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_first_of("aeiou", 10) == CStringView::npos);
    REQUIRE(testString.find_first_of('a', 10) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou", 10) == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of('a', 10) == CStringView::npos);
  }

  SECTION("Find first of with repeated characters") {
    constexpr CStringView testString("aaaaa");

    REQUIRE(testString.find_first_of("a") == 0);
    REQUIRE(testString.find_first_of("ab") == 0);
    REQUIRE(testString.find_first_of("b") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("a") == 0);
    STATIC_REQUIRE(testString.find_first_of("ab") == 0);
    STATIC_REQUIRE(testString.find_first_of("b") == CStringView::npos);
  }

  SECTION("Find first of with multiple character sets") {
    constexpr CStringView testString("Hello World");

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
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of("h") == CStringView::npos);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("w") == CStringView::npos);
    REQUIRE(testString.find_first_of("W") == 6);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("h") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of("H") == 0);
    STATIC_REQUIRE(testString.find_first_of("w") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of("W") == 6);
  }

  SECTION("Find first of with special characters") {
    constexpr CStringView testString("Hello, World!");

    REQUIRE(testString.find_first_of("!,") == 5); // ',' at position 5
    REQUIRE(testString.find_first_of("!") == 12); // '!' at position 12
    REQUIRE(testString.find_first_of(".,!") == 5); // ',' at position 5

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("!,") == 5);
    STATIC_REQUIRE(testString.find_first_of("!") == 12);
    STATIC_REQUIRE(testString.find_first_of(".,!") == 5);
  }

  SECTION("Find first of with numbers") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_first_of("0123456789") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("123") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("456") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("0123456789") == 5);
    STATIC_REQUIRE(testString.find_first_of("123") == 5);
    STATIC_REQUIRE(testString.find_first_of("456") == CStringView::npos);
  }

  SECTION("Find first of with whitespace") {
    constexpr CStringView testString("Hello World\t\n");

    REQUIRE(testString.find_first_of(" \t\n") == 5); // ' ' at position 5
    REQUIRE(testString.find_first_of("\t") == 11);
    REQUIRE(testString.find_first_of("\n") == 12);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of(" \t\n") == 5);
    STATIC_REQUIRE(testString.find_first_of("\t") == 11);
    STATIC_REQUIRE(testString.find_first_of("\n") == 12);
  }

  SECTION("Find first of with single character string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.find_first_of("A") == 0);
    REQUIRE(testString.find_first_of('A') == 0);
    REQUIRE(testString.find_first_of("B") == CStringView::npos);
    REQUIRE(testString.find_first_of('B') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("A") == 0);
    STATIC_REQUIRE(testString.find_first_of('A') == 0);
    STATIC_REQUIRE(testString.find_first_of("B") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of('B') == CStringView::npos);
  }

  SECTION("Find first of with position 0") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("H", 0) == 0);
    REQUIRE(testString.find_first_of("xyz", 0) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    STATIC_REQUIRE(testString.find_first_of("H", 0) == 0);
    STATIC_REQUIRE(testString.find_first_of("xyz", 0) == CStringView::npos);
  }

  SECTION("Find first of with all characters found") {
    constexpr CStringView testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_of("aeiou") == 0);
    REQUIRE(testString.find_first_of("xyz") == 23);
    REQUIRE(testString.find_first_of("z") == 25);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("aeiou") == 0);
    STATIC_REQUIRE(testString.find_first_of("xyz") == 23);
    STATIC_REQUIRE(testString.find_first_of("z") == 25);
  }

  SECTION("Find first of with no characters found") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of("0123456789") == CStringView::npos);
    REQUIRE(testString.find_first_of("!@#$%^&*()") == CStringView::npos);
    REQUIRE(testString.find_first_of("[]{}|\\:;\"'<>?/") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_of("0123456789") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of("!@#$%^&*()") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_of("[]{}|\\:;\"'<>?/") == CStringView::npos);
  }
}

TEST_CASE("CStringView find_first_not_of", "[core][c_string_view]") {
  SECTION("Find first not of CStringView characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of(CStringView("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(CStringView("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(CStringView("Helo Wrd")) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("H")) == 1);
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("Hel")) == 4);
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("Helo Wrd")) == CStringView::npos);
  }

  SECTION("Find first not of StringLike characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of(std::string("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(std::string("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(std::string("Helo Wrd")) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("H")) == 1);
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("Hel")) == 4);
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("Helo Wrd")) == CStringView::npos);
  }

  SECTION("Find first not of C string characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of("H") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Helo Wrd") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("H") == 1);
    STATIC_REQUIRE(testString.find_first_not_of("Hel") == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Helo Wrd") == CStringView::npos);
  }

  SECTION("Find first not of single character") {
    constexpr CStringView testString("aaaaab");

    REQUIRE(testString.find_first_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_first_not_of('b') == 0); // 'a' at position 0
    REQUIRE(testString.find_first_not_of('x') == 0); // 'a' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of('a') == 5);
    STATIC_REQUIRE(testString.find_first_not_of('b') == 0);
    STATIC_REQUIRE(testString.find_first_not_of('x') == 0);
  }

  SECTION("Find first not of with position parameter") {
    constexpr CStringView testString("Hello World Hello");

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
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of(CStringView("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of(std::string("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("", 5) == 5); // ' ' at position 5

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("")) == 0);
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<4>("")) == 0);
    STATIC_REQUIRE(testString.find_first_not_of("") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("", 5) == 5);
  }

  SECTION("Find first not of in empty string") {
    constexpr CStringView testString("");

    REQUIRE(testString.find_first_not_of(CStringView("aeiou")) == CStringView::npos);
    REQUIRE(testString.find_first_not_of(std::string("aeiou")) == CStringView::npos);
    REQUIRE(testString.find_first_not_of("aeiou") == CStringView::npos);
    REQUIRE(testString.find_first_not_of('a') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(CStringView("aeiou")) == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of(FixedString<8>("aeiou")) == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("aeiou") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of('a') == CStringView::npos);
  }

  SECTION("Find first not of with position beyond string size") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_first_not_of("aeiou", 10) == CStringView::npos);
    REQUIRE(testString.find_first_not_of('a', 10) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("aeiou", 10) == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of('a', 10) == CStringView::npos);
  }

  SECTION("Find first not of with repeated characters") {
    constexpr CStringView testString("aaaaa");

    REQUIRE(testString.find_first_not_of("a") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("ab") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("b") == 0); // 'a' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("a") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("ab") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("b") == 0);
  }

  SECTION("Find first not of with multiple character sets") {
    constexpr CStringView testString("Hello World");

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
    constexpr CStringView testString("Hello World");

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
    constexpr CStringView testString("Hello, World!");

    REQUIRE(testString.find_first_not_of("Helo, Wrd!") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Helo, Wrd") == 12); // '!' at position 12

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("Helo, Wrd!") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Helo, Wrd") == 12);
  }

  SECTION("Find first not of with numbers") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo123Wrd") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("0123456789") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wrd") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wr") == 12);
  }

  SECTION("Find first not of with whitespace") {
    constexpr CStringView testString("Hello World\t\n");

    REQUIRE(testString.find_first_not_of(" \t\n") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo Wrd\t\n") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Helo Wrd") == 11); // '\t' at position 11

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of(" \t\n") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("Helo Wrd\t\n") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Helo Wrd") == 11);
  }

  SECTION("Find first not of with single character string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.find_first_not_of("A") == CStringView::npos);
    REQUIRE(testString.find_first_not_of('A') == CStringView::npos);
    REQUIRE(testString.find_first_not_of("B") == 0); // 'A' at position 0
    REQUIRE(testString.find_first_not_of('B') == 0); // 'A' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("A") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of('A') == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("B") == 0);
    STATIC_REQUIRE(testString.find_first_not_of('B') == 0);
  }

  SECTION("Find first not of with position 0") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of("H", 0) == 1);
    REQUIRE(testString.find_first_not_of("Hel", 0) == 4);
    REQUIRE(testString.find_first_not_of("Helo Wrd", 0) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("H", 0) == 1);
    STATIC_REQUIRE(testString.find_first_not_of("Hel", 0) == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Helo Wrd", 0) == CStringView::npos);
  }

  SECTION("Find first not of with all characters excluded") {
    constexpr CStringView testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24); // 'y' at position 24

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25);
    STATIC_REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24);
  }

  SECTION("Find first not of with no characters excluded") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of("xyz") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("!@#$%^&*()") == 0); // 'H' at position 0

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("xyz") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("0123456789") == 0);
    STATIC_REQUIRE(testString.find_first_not_of("!@#$%^&*()") == 0);
  }

  SECTION("Find first not of with mixed content") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_first_not_of("Helo123Wrd") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12
    REQUIRE(testString.find_first_not_of("Helo123Wd") == 10); // 'r' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wrd") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wr") == 12);
    STATIC_REQUIRE(testString.find_first_not_of("Helo123Wd") == 10);
  }

  SECTION("Find first not of with position in middle") {
    constexpr CStringView testString("Hello World Hello");

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
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_first_not_of("Hello") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4

    // Compile-time checks
    STATIC_REQUIRE(testString.find_first_not_of("Hello") == CStringView::npos);
    STATIC_REQUIRE(testString.find_first_not_of("Hell") == 4);
    STATIC_REQUIRE(testString.find_first_not_of("Hel") == 4);
  }
}

TEST_CASE("CStringView find_last_of", "[core][c_string_view]") {
  SECTION("Find last of CStringView characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of(CStringView("aeiou")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of(CStringView("l")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of(CStringView("H")) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of(CStringView("d")) == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of(CStringView("aeiou")) == 7);
    STATIC_REQUIRE(testString.find_last_of(CStringView("l")) == 9);
    STATIC_REQUIRE(testString.find_last_of(CStringView("H")) == 0);
    STATIC_REQUIRE(testString.find_last_of(CStringView("d")) == 10);
  }

  SECTION("Find last of StringLike characters") {
    constexpr CStringView testString("Hello World");

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
    constexpr CStringView testString("Hello World");

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
    constexpr CStringView testString("Hello World");

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
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    REQUIRE(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1
    REQUIRE(testString.find_last_of("Hel", 0) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("Hel", 17) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Hel", 8) == 3);
    STATIC_REQUIRE(testString.find_last_of("Hel", 4) == 3);
    STATIC_REQUIRE(testString.find_last_of("Hel", 2) == 2);
    STATIC_REQUIRE(testString.find_last_of("Hel", 1) == 1);
    STATIC_REQUIRE(testString.find_last_of("Hel", 0) == 0);
    STATIC_REQUIRE(testString.find_last_of("Hel", 17) == CStringView::npos);
  }

  SECTION("Find last of empty character set") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of(CStringView("")) == CStringView::npos);
    REQUIRE(testString.find_last_of(std::string("")) == CStringView::npos);
    REQUIRE(testString.find_last_of("") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of(CStringView("")) == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_of(FixedString<4>("")) == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_of("") == CStringView::npos);
  }

  SECTION("Find last of with no characters found") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of("0123456789") == CStringView::npos);
    REQUIRE(testString.find_last_of("!@#$%^&*()") == CStringView::npos);
    REQUIRE(testString.find_last_of("[]{}|\\:;\"'<>?/") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("0123456789") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_of("!@#$%^&*()") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_of("[]{}|\\:;\"'<>?/") == CStringView::npos);
  }

  SECTION("Find last of with multiple character sets") {
    constexpr CStringView testString("Hello World");

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
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of("h") == CStringView::npos);
    REQUIRE(testString.find_last_of("H") == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("w") == CStringView::npos);
    REQUIRE(testString.find_last_of("W") == 6); // 'W' at position 6

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("h") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_of("H") == 0);
    STATIC_REQUIRE(testString.find_last_of("w") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_of("W") == 6);
  }

  SECTION("Find last of with special characters") {
    constexpr CStringView testString("Hello, World!");

    REQUIRE(testString.find_last_of("Helo, Wrd!") == 12); // '!' at position 12
    REQUIRE(testString.find_last_of("Helo, Wrd") == 11); // 'd' at position 11
    REQUIRE(testString.find_last_of("Helo, Wr") == 10); // 'l' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Helo, Wrd!") == 12);
    STATIC_REQUIRE(testString.find_last_of("Helo, Wrd") == 11);
    STATIC_REQUIRE(testString.find_last_of("Helo, Wr") == 10);
  }

  SECTION("Find last of with numbers") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_last_of("0123456789") == 7); // '3' at position 7
    REQUIRE(testString.find_last_of("Helo123Wrd") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_of("Helo123Wr") == 11); // 'l' at position 11

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("0123456789") == 7);
    STATIC_REQUIRE(testString.find_last_of("Helo123Wrd") == 12);
    STATIC_REQUIRE(testString.find_last_of("Helo123Wr") == 11);
  }

  SECTION("Find last of with whitespace") {
    constexpr CStringView testString("Hello World\t\n");

    REQUIRE(testString.find_last_of(" \t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd\t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of(" \t\n") == 12);
    STATIC_REQUIRE(testString.find_last_of("Helo Wrd\t\n") == 12);
    STATIC_REQUIRE(testString.find_last_of("Helo Wrd") == 10);
  }

  SECTION("Find last of with repeated characters") {
    constexpr CStringView testString("aaaaab");

    REQUIRE(testString.find_last_of('a') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_of('b') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_of('x') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of('a') == 4);
    STATIC_REQUIRE(testString.find_last_of('b') == 5);
    STATIC_REQUIRE(testString.find_last_of('x') == CStringView::npos);
  }

  SECTION("Find last of with single character string") {
    constexpr CStringView testString("AAAAA");

    REQUIRE(testString.find_last_of("A") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of('A') == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of("B") == CStringView::npos);
    REQUIRE(testString.find_last_of('B') == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("A") == 4);
    STATIC_REQUIRE(testString.find_last_of('A') == 4);
    STATIC_REQUIRE(testString.find_last_of("B") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_of('B') == CStringView::npos);
  }

  SECTION("Find last of with alphabet") {
    constexpr CStringView testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24); // 'y' at position 24
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23); // 'x' at position 23

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25);
    STATIC_REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24);
    STATIC_REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23);
  }

  SECTION("Find last of with all characters found") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_of("Helo Wr") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo W") == 9); // 'l' at position 9

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Helo Wrd") == 10);
    STATIC_REQUIRE(testString.find_last_of("Helo Wr") == 9);
    STATIC_REQUIRE(testString.find_last_of("Helo W") == 9);
  }

  SECTION("Find last of with position in middle") {
    constexpr CStringView testString("Hello World Hello");

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
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_last_of("Hello") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_of("Hell") == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel") == 3); // 'l' at position 3

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_of("Hello") == 4);
    STATIC_REQUIRE(testString.find_last_of("Hell") == 3);
    STATIC_REQUIRE(testString.find_last_of("Hel") == 3);
  }
}

TEST_CASE("CStringView find_last_not_of", "[core][c_string_view]") {
  SECTION("Find last not of CStringView characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of(CStringView("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(CStringView("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(CStringView("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(CStringView("World")) == 5); // ' ' at position 5

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of(CStringView("d")) == 9);
    STATIC_REQUIRE(testString.find_last_not_of(CStringView("ld")) == 8);
    STATIC_REQUIRE(testString.find_last_not_of(CStringView("rld")) == 7);
    STATIC_REQUIRE(testString.find_last_not_of(CStringView("World")) == 5);
  }

  SECTION("Find last not of StringLike characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of(std::string("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(std::string("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(std::string("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(std::string("World")) == 5); // ' ' at position 5

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of(CStringView("d")) == 9);
    STATIC_REQUIRE(testString.find_last_not_of(CStringView("ld")) == 8);
    STATIC_REQUIRE(testString.find_last_not_of(CStringView("rld")) == 7);
    STATIC_REQUIRE(testString.find_last_not_of(CStringView("World")) == 5);
  }

  SECTION("Find last not of C string characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("d") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of("ld") == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("rld") == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of("World") == 5); // ' ' at position 5

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("d") == 9);
    STATIC_REQUIRE(testString.find_last_not_of("ld") == 8);
    STATIC_REQUIRE(testString.find_last_not_of("rld") == 7);
    STATIC_REQUIRE(testString.find_last_not_of("World") == 5);
  }

  SECTION("Find last not of single character") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of('d') == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of('l') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('o') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('H') == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of('d') == 9);
    STATIC_REQUIRE(testString.find_last_not_of('l') == 10);
    STATIC_REQUIRE(testString.find_last_not_of('o') == 10);
    STATIC_REQUIRE(testString.find_last_not_of('H') == 10);
  }

  SECTION("Find last not of with position parameter") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hel", 0) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 8) == 8);
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 4) == 4);
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 2) == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 1) == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 0) == CStringView::npos);
  }

  SECTION("Find last not of empty character set") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of(CStringView("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of(std::string("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("", 5) == 5); // ' ' at position 5

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of(CStringView("")) == 10);
    STATIC_REQUIRE(testString.find_last_not_of(FixedString<10>("")) == 10);
    STATIC_REQUIRE(testString.find_last_not_of("") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("", 5) == 5);
  }

  SECTION("Find last not of with all characters excluded") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("Helo Wrd") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Helo Wr") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo W") == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("Helo Wrd") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("Helo Wr") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("Helo W") == 10);
  }

  SECTION("Find last not of with multiple character sets") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("Hl") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Hel") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo ") == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("Hl") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("Hel") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("Helo") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("Helo ") == 10);
  }

  SECTION("Find last not of case sensitivity") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("h") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("H") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("w") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("W") == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("h") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("H") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("w") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("W") == 10);
  }

  SECTION("Find last not of with special characters") {
    constexpr CStringView testString("Hello, World!");

    REQUIRE(testString.find_last_not_of("Helo, Wrd!") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Helo, Wrd") == 12); // '!' at position 12
    REQUIRE(testString.find_last_not_of("Helo, Wr") == 12); // '!' at position 12

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("Helo, Wrd!") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("Helo, Wrd") == 12);
    STATIC_REQUIRE(testString.find_last_not_of("Helo, Wr") == 12);
  }

  SECTION("Find last not of with numbers") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_last_not_of("0123456789") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_not_of("Helo123Wrd") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Helo123Wr") == 12); // 'd' at position 12

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("0123456789") == 12);
    STATIC_REQUIRE(testString.find_last_not_of("Helo123Wrd") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("Helo123Wr") == 12);
  }

  SECTION("Find last not of with whitespace") {
    constexpr CStringView testString("Hello World\t\n");

    REQUIRE(testString.find_last_not_of(" \t\n") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo Wrd\t\n") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Helo Wrd") == 12); // '\n' at position 12

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of(" \t\n") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("Helo Wrd\t\n") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("Helo Wrd") == 12);
  }

  SECTION("Find last not of with repeated characters") {
    constexpr CStringView testString("aaaaab");

    REQUIRE(testString.find_last_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_not_of('b') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_not_of('x') == 5); // 'b' at position 5

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of('a') == 5);
    STATIC_REQUIRE(testString.find_last_not_of('b') == 4);
    STATIC_REQUIRE(testString.find_last_not_of('x') == 5);
  }

  SECTION("Find last not of with single character string") {
    constexpr CStringView testString("AAAAA");

    REQUIRE(testString.find_last_not_of("A") == CStringView::npos);
    REQUIRE(testString.find_last_not_of('A') == CStringView::npos);
    REQUIRE(testString.find_last_not_of("B") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_not_of('B') == 4); // 'A' at position 4

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("A") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of('A') == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("B") == 4);
    STATIC_REQUIRE(testString.find_last_not_of('B') == 4);
  }

  SECTION("Find last not of with alphabet") {
    constexpr CStringView testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxyz") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwx") == 25); // 'z' at position 25

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxyz") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxy") == 25);
    STATIC_REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwx") == 25);
  }

  SECTION("Find last not of with no characters excluded") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("xyz") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("0123456789") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("!@#$%^&*()") == 10); // 'd' at position 10

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("xyz") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("0123456789") == 10);
    STATIC_REQUIRE(testString.find_last_not_of("!@#$%^&*()") == 10);
  }

  SECTION("Find last not of with position in middle") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hel", 17) == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 8) == 8);
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 4) == 4);
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 2) == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 1) == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("Hel", 1) == CStringView::npos);
    (testString.find_last_not_of("Hel", 17) == CStringView::npos);
  }

  SECTION("Find last not of with exact match") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_last_not_of("Hello") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel") == 4); // 'o' at position 4

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("Hello") == CStringView::npos);
    STATIC_REQUIRE(testString.find_last_not_of("Hell") == 4);
    STATIC_REQUIRE(testString.find_last_not_of("Hel") == 4);
  }

  SECTION("Find last not of in empty string") {
    constexpr CStringView testString("");

    REQUIRE(testString.find_last_not_of("Hello") == CStringView::npos);

    // Compile-time checks
    STATIC_REQUIRE(testString.find_last_not_of("Hello") == CStringView::npos);
  }
}

TEST_CASE("CStringView compare", "[core][c_string_view]") {
  SECTION("Compare CStringView with CStringView") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hello");
    constexpr CStringView testString3("World");
    constexpr CStringView testString4("Hell");

    REQUIRE(testString1.compare(testString2) == 0);
    REQUIRE(testString1.compare(testString3) < 0);
    REQUIRE(testString1.compare(testString4) > 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) == 0);
    STATIC_REQUIRE(testString1.compare(testString3) < 0);
    STATIC_REQUIRE(testString1.compare(testString4) > 0);
  }

  SECTION("Compare CStringView with StringLike") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.compare(std::string("Hello")) == 0);
    REQUIRE(testString.compare(std::string("World")) < 0);
    REQUIRE(testString.compare(std::string("Hell")) > 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.compare(CStringView("Hello")) == 0);
    STATIC_REQUIRE(testString.compare(CStringView("World")) < 0);
    STATIC_REQUIRE(testString.compare(CStringView("Hell")) > 0);
  }

  SECTION("Compare CStringView with C string") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.compare("Hello") == 0);
    REQUIRE(testString.compare("World") < 0);
    REQUIRE(testString.compare("Hell") > 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.compare("Hello") == 0);
    STATIC_REQUIRE(testString.compare("World") < 0);
    STATIC_REQUIRE(testString.compare("Hell") > 0);
  }

  SECTION("Compare identical strings") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.compare(CStringView("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare("Hello World") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.compare(CStringView("Hello World")) == 0);
    STATIC_REQUIRE(testString.compare(FixedString<16>("Hello World")) == 0);
    STATIC_REQUIRE(testString.compare("Hello World") == 0);
  }

  SECTION("Compare with empty strings") {
    constexpr CStringView testString1("");
    constexpr CStringView testString2("Hello");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("") == 0);
    REQUIRE(testString1.compare(std::string("")) == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) < 0);
    STATIC_REQUIRE(testString2.compare(testString1) > 0);
    STATIC_REQUIRE(testString1.compare("") == 0);
    STATIC_REQUIRE(testString1.compare(CStringView("")) == 0);
  }

  SECTION("Compare strings with different lengths") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hello World");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) < 0);
    STATIC_REQUIRE(testString2.compare(testString1) > 0);
  }

  SECTION("Compare strings with same prefix") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hell");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) > 0);
    STATIC_REQUIRE(testString2.compare(testString1) < 0);
  }

  SECTION("Compare strings with different first character") {
    constexpr CStringView testString1("Apple");
    constexpr CStringView testString2("Banana");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) < 0);
    STATIC_REQUIRE(testString2.compare(testString1) > 0);
  }

  SECTION("Compare strings with different middle character") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hallo");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) > 0);
    STATIC_REQUIRE(testString2.compare(testString1) < 0);
  }

  SECTION("Compare strings with different last character") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hellp");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) < 0);
    STATIC_REQUIRE(testString2.compare(testString1) > 0);
  }

  SECTION("Compare case sensitivity") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.compare("hello") < 0);
    REQUIRE(testString.compare("HELLO") > 0);
    REQUIRE(testString.compare("Hello") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.compare("hello") < 0);
    STATIC_REQUIRE(testString.compare("HELLO") > 0);
    STATIC_REQUIRE(testString.compare("Hello") == 0);
  }

  SECTION("Compare with single character strings") {
    constexpr CStringView testString1("A");
    constexpr CStringView testString2("B");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("A") == 0);
    REQUIRE(testString1.compare("B") < 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) < 0);
    STATIC_REQUIRE(testString2.compare(testString1) > 0);
    STATIC_REQUIRE(testString1.compare("A") == 0);
    STATIC_REQUIRE(testString1.compare("B") < 0);
  }

  SECTION("Compare with repeated characters") {
    constexpr CStringView testString1("aaa");
    constexpr CStringView testString2("aa");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("aaa") == 0);
    REQUIRE(testString1.compare("aa") > 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) > 0);
    STATIC_REQUIRE(testString2.compare(testString1) < 0);
    STATIC_REQUIRE(testString1.compare("aaa") == 0);
    STATIC_REQUIRE(testString1.compare("aa") > 0);
  }

  SECTION("Compare with special characters") {
    constexpr CStringView testString1("Hello!");
    constexpr CStringView testString2("Hello");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("Hello!") == 0);
    REQUIRE(testString1.compare("Hello") > 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) > 0);
    STATIC_REQUIRE(testString2.compare(testString1) < 0);
    STATIC_REQUIRE(testString1.compare("Hello!") == 0);
    STATIC_REQUIRE(testString1.compare("Hello") > 0);
  }

  SECTION("Compare with numbers") {
    constexpr CStringView testString1("123");
    constexpr CStringView testString2("456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123") == 0);
    REQUIRE(testString1.compare("456") < 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) < 0);
    STATIC_REQUIRE(testString2.compare(testString1) > 0);
    STATIC_REQUIRE(testString1.compare("123") == 0);
    STATIC_REQUIRE(testString1.compare("456") < 0);
  }

  SECTION("Compare with mixed content") {
    constexpr CStringView testString1("Hello123");
    constexpr CStringView testString2("Hello456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("Hello123") == 0);
    REQUIRE(testString1.compare("Hello456") < 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) < 0);
    STATIC_REQUIRE(testString2.compare(testString1) > 0);
    STATIC_REQUIRE(testString1.compare("Hello123") == 0);
    STATIC_REQUIRE(testString1.compare("Hello456") < 0);
  }

  SECTION("Compare with maximum length strings") {
    constexpr CStringView testString1("123456789012345");
    constexpr CStringView testString2("123456789012346");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123456789012345") == 0);
    REQUIRE(testString1.compare("123456789012346") < 0);

    // Compile-time checks
    STATIC_REQUIRE(testString1.compare(testString2) < 0);
    STATIC_REQUIRE(testString2.compare(testString1) > 0);
    STATIC_REQUIRE(testString1.compare("123456789012345") == 0);
    STATIC_REQUIRE(testString1.compare("123456789012346") < 0);
  }

  SECTION("Compare with StringLike") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello")) > 0);
    REQUIRE(testString.compare(std::string("World")) < 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.compare(FixedString<16>("Hello World")) == 0);
    STATIC_REQUIRE(testString.compare(FixedString<16>("Hello")) > 0);
    STATIC_REQUIRE(testString.compare(FixedString<16>("World")) < 0);
  }

  SECTION("Compare with array") {
    constexpr CStringView testString("Hello");
    constexpr std::array<char, 6> arr = {'H', 'e', 'l', 'l', 'o', '\0'};

    REQUIRE(testString.compare(arr.data()) == 0);
    REQUIRE(testString.compare("Hello") == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.compare(arr.data()) == 0);
    STATIC_REQUIRE(testString.compare("Hello") == 0);
  }

  SECTION("Compare edge cases") {
    constexpr CStringView testString("Hello");

    // Compare with null-terminated string
    REQUIRE(testString.compare("Hello\0World") == 0);

    // Compare with string containing null character
    constexpr CStringView testStringWithNull("Hello\0World");
    REQUIRE(testString.compare(testStringWithNull) == 0);

    // Compile-time checks
    STATIC_REQUIRE(testString.compare("Hello\0World") == 0);
    STATIC_REQUIRE(testString.compare(testStringWithNull) == 0);
  }
}

TEST_CASE("CStringView starts_with", "[core][c_string_view]") {
  SECTION("Starts with CStringView") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with(CStringView("Hello")) == true);
    REQUIRE(testString.starts_with(CStringView("Hello World")) == true);
    REQUIRE(testString.starts_with(CStringView("H")) == true);
    REQUIRE(testString.starts_with(CStringView("World")) == false);
    REQUIRE(testString.starts_with(CStringView("xyz")) == false);
    REQUIRE(testString.starts_with(CStringView("")) == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with(CStringView("Hello")) == true);
    STATIC_REQUIRE(testString.starts_with(CStringView("Hello World")) == true);
    STATIC_REQUIRE(testString.starts_with(CStringView("H")) == true);
    STATIC_REQUIRE(testString.starts_with(CStringView("World")) == false);
    STATIC_REQUIRE(testString.starts_with(CStringView("xyz")) == false);
    STATIC_REQUIRE(testString.starts_with(CStringView("")) == true);
  }

  SECTION("Starts with StringLike") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")) == true);
    REQUIRE(testString.starts_with(std::string("Hello World")) == true);
    REQUIRE(testString.starts_with(std::string("H")) == true);
    REQUIRE(testString.starts_with(std::string("World")) == false);
    REQUIRE(testString.starts_with(std::string("xyz")) == false);
    REQUIRE(testString.starts_with(std::string("")) == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("Hello")) == true);
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("Hello World")) == true);
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("H")) == true);
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("World")) == false);
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("xyz")) == false);
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("")) == true);
  }

  SECTION("Starts with C string") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hello World") == true);
    REQUIRE(testString.starts_with("H") == true);
    REQUIRE(testString.starts_with("World") == false);
    REQUIRE(testString.starts_with("xyz") == false);
    REQUIRE(testString.starts_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("Hello") == true);
    STATIC_REQUIRE(testString.starts_with("Hello World") == true);
    STATIC_REQUIRE(testString.starts_with("H") == true);
    STATIC_REQUIRE(testString.starts_with("World") == false);
    STATIC_REQUIRE(testString.starts_with("xyz") == false);
    STATIC_REQUIRE(testString.starts_with("") == true);
  }

  SECTION("Starts with character") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with('H') == true);
    REQUIRE(testString.starts_with('h') == false);
    REQUIRE(testString.starts_with('W') == false);
    REQUIRE(testString.starts_with('x') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with('H') == true);
    STATIC_REQUIRE(testString.starts_with('h') == false);
    STATIC_REQUIRE(testString.starts_with('W') == false);
    STATIC_REQUIRE(testString.starts_with('x') == false);
  }

  SECTION("Starts with empty string") {
    constexpr CStringView testString("");

    REQUIRE(testString.starts_with(CStringView("Hello")) == false);
    REQUIRE(testString.starts_with(std::string("Hello")) == false);
    REQUIRE(testString.starts_with("Hello") == false);
    REQUIRE(testString.starts_with('H') == false);
    REQUIRE(testString.starts_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with(CStringView("Hello")) == false);
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("Hello")) == false);
    STATIC_REQUIRE(testString.starts_with("Hello") == false);
    STATIC_REQUIRE(testString.starts_with('H') == false);
    STATIC_REQUIRE(testString.starts_with("") == true);
  }

  SECTION("Starts with single character string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.starts_with("A") == true);
    REQUIRE(testString.starts_with('A') == true);
    REQUIRE(testString.starts_with("B") == false);
    REQUIRE(testString.starts_with('B') == false);
    REQUIRE(testString.starts_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("A") == true);
    STATIC_REQUIRE(testString.starts_with('A') == true);
    STATIC_REQUIRE(testString.starts_with("B") == false);
    STATIC_REQUIRE(testString.starts_with('B') == false);
    STATIC_REQUIRE(testString.starts_with("") == true);
  }

  SECTION("Starts with longer prefix") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.starts_with("Hello World") == false);
    REQUIRE(testString.starts_with("Hello Universe") == false);
    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hell") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("Hello World") == false);
    STATIC_REQUIRE(testString.starts_with("Hello Universe") == false);
    STATIC_REQUIRE(testString.starts_with("Hello") == true);
    STATIC_REQUIRE(testString.starts_with("Hell") == true);
  }

  SECTION("Starts with case sensitivity") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with("hello") == false);
    REQUIRE(testString.starts_with("HELLO") == false);
    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with('h') == false);
    REQUIRE(testString.starts_with('H') == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("hello") == false);
    STATIC_REQUIRE(testString.starts_with("HELLO") == false);
    STATIC_REQUIRE(testString.starts_with("Hello") == true);
    STATIC_REQUIRE(testString.starts_with('h') == false);
    STATIC_REQUIRE(testString.starts_with('H') == true);
  }

  SECTION("Starts with repeated characters") {
    constexpr CStringView testString("aaaab");

    REQUIRE(testString.starts_with("aaa") == true);
    REQUIRE(testString.starts_with("aaaa") == true);
    REQUIRE(testString.starts_with("aaaab") == true);
    REQUIRE(testString.starts_with("aaab") == false);
    REQUIRE(testString.starts_with('a') == true);
    REQUIRE(testString.starts_with('b') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("aaa") == true);
    STATIC_REQUIRE(testString.starts_with("aaaa") == true);
    STATIC_REQUIRE(testString.starts_with("aaaab") == true);
    STATIC_REQUIRE(testString.starts_with("aaab") == false);
    STATIC_REQUIRE(testString.starts_with('a') == true);
    STATIC_REQUIRE(testString.starts_with('b') == false);
  }

  SECTION("Starts with special characters") {
    constexpr CStringView testString("!@#$%");

    REQUIRE(testString.starts_with("!@#") == true);
    REQUIRE(testString.starts_with("!@#$%") == true);
    REQUIRE(testString.starts_with("!@#$%^") == false);
    REQUIRE(testString.starts_with('!') == true);
    REQUIRE(testString.starts_with('@') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("!@#") == true);
    STATIC_REQUIRE(testString.starts_with("!@#$%") == true);
    STATIC_REQUIRE(testString.starts_with("!@#$%^") == false);
    STATIC_REQUIRE(testString.starts_with('!') == true);
    STATIC_REQUIRE(testString.starts_with('@') == false);
  }

  SECTION("Starts with numbers") {
    constexpr CStringView testString("12345");

    REQUIRE(testString.starts_with("123") == true);
    REQUIRE(testString.starts_with("12345") == true);
    REQUIRE(testString.starts_with("123456") == false);
    REQUIRE(testString.starts_with('1') == true);
    REQUIRE(testString.starts_with('2') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("123") == true);
    STATIC_REQUIRE(testString.starts_with("12345") == true);
    STATIC_REQUIRE(testString.starts_with("123456") == false);
    STATIC_REQUIRE(testString.starts_with('1') == true);
    STATIC_REQUIRE(testString.starts_with('2') == false);
  }

  SECTION("Starts with mixed content") {
    constexpr CStringView testString("Hello123");

    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hello1") == true);
    REQUIRE(testString.starts_with("Hello123") == true);
    REQUIRE(testString.starts_with("Hello1234") == false);
    REQUIRE(testString.starts_with('H') == true);
    REQUIRE(testString.starts_with('1') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("Hello") == true);
    STATIC_REQUIRE(testString.starts_with("Hello1") == true);
    STATIC_REQUIRE(testString.starts_with("Hello123") == true);
    STATIC_REQUIRE(testString.starts_with("Hello1234") == false);
    STATIC_REQUIRE(testString.starts_with('H') == true);
    STATIC_REQUIRE(testString.starts_with('1') == false);
  }

  SECTION("Starts with maximum length strings") {
    constexpr CStringView testString("123456789012345");

    REQUIRE(testString.starts_with("123456789012345") == true);
    REQUIRE(testString.starts_with("12345678901234") == true);
    REQUIRE(testString.starts_with("1234567890123456") == false);
    REQUIRE(testString.starts_with('1') == true);
    REQUIRE(testString.starts_with('5') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("123456789012345") == true);
    STATIC_REQUIRE(testString.starts_with("12345678901234") == true);
    STATIC_REQUIRE(testString.starts_with("1234567890123456") == false);
    STATIC_REQUIRE(testString.starts_with('1') == true);
    STATIC_REQUIRE(testString.starts_with('5') == false);
  }

  SECTION("Starts with std::string") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")) == true);
    REQUIRE(testString.starts_with(std::string("Hello World")) == true);
    REQUIRE(testString.starts_with(std::string("World")) == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("Hello")) == true);
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("Hello World")) == true);
    STATIC_REQUIRE(testString.starts_with(FixedString<16>("World")) == false);
  }

  SECTION("Starts with array") {
    constexpr CStringView testString("Hello");
    constexpr std::array<char, 4> arr = {'H', 'e', 'l', '\0'};

    REQUIRE(testString.starts_with(arr.data()) == true);
    REQUIRE(testString.starts_with("Hel") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with(arr.data()) == true);
    STATIC_REQUIRE(testString.starts_with("Hel") == true);
  }

  SECTION("Starts with edge cases") {
    constexpr CStringView testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.starts_with("Hello\0World") == true);

    // Test with string containing null character
    constexpr CStringView testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.starts_with("Hello") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("Hello\0World") == true);
    STATIC_REQUIRE(testStringWithNull.starts_with("Hello") == true);
  }

  SECTION("Starts with whitespace") {
    constexpr CStringView testString(" Hello World");

    REQUIRE(testString.starts_with(" ") == true);
    REQUIRE(testString.starts_with(" Hello") == true);
    REQUIRE(testString.starts_with("Hello") == false);
    REQUIRE(testString.starts_with(' ') == true);
    REQUIRE(testString.starts_with('H') == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with(" ") == true);
    STATIC_REQUIRE(testString.starts_with(" Hello") == true);
    STATIC_REQUIRE(testString.starts_with("Hello") == false);
    STATIC_REQUIRE(testString.starts_with(' ') == true);
    STATIC_REQUIRE(testString.starts_with('H') == false);
  }

  SECTION("Starts with exact match") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.starts_with("Hello") == true);
    REQUIRE(testString.starts_with("Hell") == true);
    REQUIRE(testString.starts_with("H") == true);
    REQUIRE(testString.starts_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.starts_with("Hello") == true);
    STATIC_REQUIRE(testString.starts_with("Hell") == true);
    STATIC_REQUIRE(testString.starts_with("H") == true);
    STATIC_REQUIRE(testString.starts_with("") == true);
  }
}

TEST_CASE("CStringView ends_with", "[core][c_string_view]") {
  SECTION("CStringView ends_with") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.ends_with(CStringView("World")) == true);
    REQUIRE(testString.ends_with(CStringView("Hello World")) == true);
    REQUIRE(testString.ends_with(CStringView("d")) == true);
    REQUIRE(testString.ends_with(CStringView("Hello")) == false);
    REQUIRE(testString.ends_with(CStringView("xyz")) == false);
    REQUIRE(testString.ends_with(CStringView("")) == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(CStringView("World")) == true);
    STATIC_REQUIRE(testString.ends_with(CStringView("Hello World")) == true);
    STATIC_REQUIRE(testString.ends_with(CStringView("d")) == true);
    STATIC_REQUIRE(testString.ends_with(CStringView("Hello")) == false);
    STATIC_REQUIRE(testString.ends_with(CStringView("xyz")) == false);
    STATIC_REQUIRE(testString.ends_with(CStringView("")) == true);
  }

  SECTION("StringLike ends_with") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.ends_with(std::string("World")) == true);
    REQUIRE(testString.ends_with(std::string("Hello World")) == true);
    REQUIRE(testString.ends_with(std::string("d")) == true);
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("xyz")) == false);
    REQUIRE(testString.ends_with(std::string("")) == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("World")) == true);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("Hello World")) == true);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("d")) == true);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("Hello")) == false);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("xyz")) == false);
    STATIC_REQUIRE(testString.ends_with(FixedString<16>("")) == true);
  }

  SECTION("C string ends_with") {
    constexpr CStringView testString("Hello World");

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
    constexpr CStringView testString("Hello World");

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
    constexpr CStringView testString("");

    REQUIRE(testString.ends_with(CStringView("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with('H') == false);
    REQUIRE(testString.ends_with("") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(CStringView("Hello")) == false);
    STATIC_REQUIRE(testString.ends_with(FixedString<8>("Hello")) == false);
    STATIC_REQUIRE(testString.ends_with("Hello") == false);
    STATIC_REQUIRE(testString.ends_with('H') == false);
    STATIC_REQUIRE(testString.ends_with("") == true);
  }

  SECTION("Single character string ends_with") {
    constexpr CStringView testString("A");

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
    constexpr CStringView testString("Hello");

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
    constexpr CStringView testString("Hello World");

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

  SECTION("Repeated characters ends_with") {
    constexpr CStringView testString("baaaa");

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
    constexpr CStringView testString("%$#@!");

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
    constexpr CStringView testString("54321");

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
    constexpr CStringView testString("123Hello");

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
    constexpr CStringView testString("123456789012345");

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
    constexpr CStringView testString("Hello");
    constexpr std::array<char, 4> arr = {'l', 'l', 'o', '\0'};

    REQUIRE(testString.ends_with(arr.data()) == true);
    REQUIRE(testString.ends_with("llo") == true);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with(arr.data()) == true);
    STATIC_REQUIRE(testString.ends_with("llo") == true);
  }

  SECTION("Edge cases ends_with") {
    constexpr CStringView testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.ends_with("Hello\0World") == true);

    // Test with string containing null character
    constexpr CStringView testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.ends_with("World") == false);

    // Compile-time checks
    STATIC_REQUIRE(testString.ends_with("Hello\0World") == true);
    STATIC_REQUIRE(testStringWithNull.ends_with("World") == false);
  }

  SECTION("Whitespace ends_with") {
    constexpr CStringView testString("Hello World ");

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
    constexpr CStringView testString("Hello");

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
    constexpr CStringView testString("ababab");

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
    constexpr CStringView testString("abababab");

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
    constexpr CStringView testString("Hello 世界");

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
    constexpr CStringView testString("This is a very long string for performance testing");

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
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("World");
    constexpr CStringView str3("Test");

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

TEST_CASE("CStringView contains", "[core][c_string_view]") {
  SECTION("C string contains") {
    constexpr CStringView str("Hello World");

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

  SECTION("CStringView contains") {
    constexpr CStringView str("Hello World");

    // Basic contains with CStringView
    REQUIRE(str.contains(CStringView("World")) == true);
    REQUIRE(str.contains(CStringView("Hello")) == true);
    REQUIRE(str.contains(CStringView("lo Wo")) == true);
    REQUIRE(str.contains(CStringView("Hello World")) == true);
    REQUIRE(str.contains(CStringView("xyz")) == false);
    REQUIRE(str.contains(CStringView("")) == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains(CStringView("World")) == true);
    STATIC_REQUIRE(str.contains(CStringView("Hello")) == true);
    STATIC_REQUIRE(str.contains(CStringView("lo Wo")) == true);
    STATIC_REQUIRE(str.contains(CStringView("Hello World")) == true);
    STATIC_REQUIRE(str.contains(CStringView("xyz")) == false);
    STATIC_REQUIRE(str.contains(CStringView("")) == true);
  }

  SECTION("StringLike contains") {
    constexpr CStringView str("Hello World");

    REQUIRE(str.contains(std::string("World")) == true);
    REQUIRE(str.contains(std::string("Hello")) == true);
    REQUIRE(str.contains(std::string("lo Wo")) == true);
    REQUIRE(str.contains(std::string("Hello World")) == true);
    REQUIRE(str.contains(std::string("xyz")) == false);
    REQUIRE(str.contains(std::string("")) == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains(FixedString<16>("World")) == true);
    STATIC_REQUIRE(str.contains(FixedString<16>("Hello")) == true);
    STATIC_REQUIRE(str.contains(FixedString<16>("lo Wo")) == true);
    STATIC_REQUIRE(str.contains(FixedString<16>("Hello World")) == true);
    STATIC_REQUIRE(str.contains(FixedString<16>("xyz")) == false);
    STATIC_REQUIRE(str.contains(FixedString<16>("")) == true);
  }

  SECTION("Character contains") {
    constexpr CStringView str("Hello World");

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
    constexpr CStringView str("");

    // Empty string contains
    REQUIRE(str.contains(CStringView("Hello")) == false);
    REQUIRE(str.contains(std::string("Hello")) == false);
    REQUIRE(str.contains("Hello") == false);
    REQUIRE(str.contains('H') == false);
    REQUIRE(str.contains("") == true);

    // Compile-time checks
    STATIC_REQUIRE(str.contains(CStringView("Hello")) == false);
    STATIC_REQUIRE(str.contains(FixedString<8>("Hello")) == false);
    STATIC_REQUIRE(str.contains("Hello") == false);
    STATIC_REQUIRE(str.contains('H') == false);
    STATIC_REQUIRE(str.contains("") == true);
  }

  SECTION("Single character string contains") {
    constexpr CStringView str("A");

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
    constexpr CStringView str("Hello World");

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
    constexpr CStringView str("Hello");

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
    constexpr CStringView str("Hello\n\tWorld!");

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
    constexpr CStringView str("Hello 世界");

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
    constexpr CStringView str("ababab");

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
    constexpr CStringView str("12345");

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
    constexpr CStringView str("123Hello456");

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
    constexpr CStringView str("Hello World");

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

TEST_CASE("CStringView operator==", "[core][c_string_view]") {
  SECTION("CStringView == CStringView") {
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("Hello");
    constexpr CStringView str3("World");
    constexpr CStringView empty1;
    constexpr CStringView empty2;

    REQUIRE(str1 == str2);
    REQUIRE(str2 == str1);
    REQUIRE_FALSE((str1 == str3));
    REQUIRE_FALSE((str3 == str1));
    REQUIRE(empty1 == empty2);
    REQUIRE(empty2 == empty1);
    REQUIRE_FALSE((str1 == empty1));
    REQUIRE_FALSE((empty1 == str1));

    // Compile-time checks
    STATIC_REQUIRE(str1 == str2);
    STATIC_REQUIRE(str2 == str1);
    STATIC_REQUIRE_FALSE((str1 == str3));
    STATIC_REQUIRE_FALSE((str3 == str1));
    STATIC_REQUIRE(empty1 == empty2);
    STATIC_REQUIRE(empty2 == empty1);
    STATIC_REQUIRE_FALSE((str1 == empty1));
    STATIC_REQUIRE_FALSE((empty1 == str1));
  }

  SECTION("CStringView == StringLike") {
    constexpr CStringView str1("Hello");
    constexpr CStringView str2;
    const std::string stdStr1;
    const std::string stdStr2("Hello");
    const std::string stdStr3("World");

    REQUIRE_FALSE(str1 == stdStr1);
    REQUIRE_FALSE(stdStr1 == str1);
    REQUIRE(str1 == stdStr2);
    REQUIRE(stdStr2 == str1);
    REQUIRE_FALSE(str1 == stdStr3);
    REQUIRE_FALSE(stdStr3 == str1);
    REQUIRE(str2 == stdStr1);
    REQUIRE_FALSE(str2 == stdStr2);
    REQUIRE_FALSE(str2 == stdStr3);
  }

  SECTION("CStringView == C string") {
    constexpr CStringView str1("Hello");
    constexpr CStringView empty;

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
    constexpr CStringView str1("A");
    constexpr CStringView str2("B");
    constexpr CStringView empty1;
    constexpr CStringView empty2;

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
    constexpr CStringView small("Hi");
    constexpr CStringView large("Hi");

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
    constexpr CStringView str1("Hello\nWorld");
    constexpr CStringView str2("Hello\tWorld");
    constexpr CStringView str3("Hello World");

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
    constexpr CStringView str1("Привет");
    constexpr CStringView str2("Мир");
    constexpr CStringView str3("Привет");

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
    constexpr CStringView str1("This is a longer string for performance testing");
    constexpr CStringView str2("This is a longer string for performance testing");
    constexpr CStringView str3("This is a different string for performance testing");

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
    constexpr CStringView str1("Test");
    constexpr CStringView str2("Test");
    constexpr CStringView str3("Different");

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

TEST_CASE("CStringView operator<=>", "[core][c_string_view]") {
  SECTION("CStringView <=> CStringView") {
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("Hello");
    constexpr CStringView str3("World");
    constexpr CStringView str4("Hi");
    constexpr CStringView str5("Hell");

    // Equal strings
    REQUIRE((str1 <=> str1) == std::strong_ordering::equal);
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
    STATIC_REQUIRE((str1 <=> str1) == std::strong_ordering::equal);
    STATIC_REQUIRE((str1 <=> str2) == std::strong_ordering::equal);
    STATIC_REQUIRE((str2 <=> str1) == std::strong_ordering::equal);

    STATIC_REQUIRE((str1 <=> str3) == std::strong_ordering::less);
    STATIC_REQUIRE((str3 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str4) == std::strong_ordering::less);
    STATIC_REQUIRE((str4 <=> str1) == std::strong_ordering::greater);
    STATIC_REQUIRE((str1 <=> str5) == std::strong_ordering::greater);
    STATIC_REQUIRE((str5 <=> str1) == std::strong_ordering::less);
  }

  SECTION("CStringView <=> StringLike") {
    constexpr CStringView str("Hello");
    const std::string stdStr1("Hello");
    const std::string stdStr2("World");

    REQUIRE((str <=> stdStr1) == std::strong_ordering::equal);
    REQUIRE((stdStr1 <=> str) == std::strong_ordering::equal);
    REQUIRE((str <=> stdStr2) == std::strong_ordering::less);
    REQUIRE((stdStr2 <=> str) == std::strong_ordering::greater);
  }

  SECTION("CStringView <=> C string") {
    constexpr CStringView str1("Hello");
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
    constexpr CStringView empty1("");
    constexpr CStringView empty2("");
    constexpr CStringView nonEmpty("Test");
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
    constexpr CStringView str1("A");
    constexpr CStringView str2("B");
    constexpr CStringView str3("A");
    constexpr CStringView str4("Z");

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
    constexpr CStringView lower("hello");
    constexpr CStringView upper("HELLO");
    constexpr CStringView mixed("Hello");

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
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("HelloWorld");
    constexpr CStringView str3("Hell");
    constexpr CStringView str4("Hello");

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
    constexpr CStringView str1("Hello\nWorld");
    constexpr CStringView str2("Hello\tWorld");
    constexpr CStringView str3("Hello World");
    constexpr CStringView str4("Hello\nWorld");

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
    constexpr CStringView str1("Привет");
    constexpr CStringView str2("Мир");
    constexpr CStringView str3("Привет");
    constexpr CStringView str4("Hello 🌍");

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
    constexpr CStringView str1("123");
    constexpr CStringView str2("456");
    constexpr CStringView str3("123");
    constexpr CStringView str4("12");
    constexpr CStringView str5("1234");

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

  SECTION("Constexpr operations") {
    constexpr CStringView str1("Test");
    constexpr CStringView str2("Test");
    constexpr CStringView str3("Different");
    constexpr CStringView str4("Test");
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
    constexpr CStringView str1("This is a very long string for performance testing");
    constexpr CStringView str2("This is a very long string for performance testing");
    constexpr CStringView str3("This is a very long string for performance testing!");
    constexpr CStringView str4("This is a different string for performance testing");

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

TEST_CASE("CStringView std::swap", "[core][c_string_view]") {
  SECTION("Basic swap functionality") {
    CStringView str1("Hello");
    CStringView str2("World");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "World") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);
  }

  SECTION("Swap with empty strings") {
    CStringView str1("Hello");
    CStringView str2("");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);
  }

  SECTION("Swap two empty strings") {
    CStringView str1("");
    CStringView str2("");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
    REQUIRE(str2.size() == 0);
    REQUIRE(std::strcmp(str2.c_str(), "") == 0);
  }

  SECTION("Self-swap") {
    CStringView str1("Hello");

    std::swap(str1, str1);

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);
  }

  SECTION("Swap with different sizes") {
    CStringView str1("Hi");
    CStringView str2("VeryLongString");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 14);
    REQUIRE(std::strcmp(str1.c_str(), "VeryLongString") == 0);
    REQUIRE(str2.size() == 2);
    REQUIRE(std::strcmp(str2.c_str(), "Hi") == 0);
  }

  SECTION("Swap with maximum length strings") {
    CStringView str1("123456789012345"); // 15 chars
    CStringView str2("ABCDEFGHIJKLMNO"); // 15 chars

    std::swap(str1, str2);

    REQUIRE(str1.size() == 15);
    REQUIRE(std::strcmp(str1.c_str(), "ABCDEFGHIJKLMNO") == 0);
    REQUIRE(str2.size() == 15);
    REQUIRE(std::strcmp(str2.c_str(), "123456789012345") == 0);
  }

  SECTION("Swap with special characters") {
    CStringView str1("Hello,\n\t!");
    CStringView str2("World,\r\n?");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 9);
    REQUIRE(std::strcmp(str1.c_str(), "World,\r\n?") == 0);
    REQUIRE(str2.size() == 9);
    REQUIRE(std::strcmp(str2.c_str(), "Hello,\n\t!") == 0);
  }

  SECTION("Swap with Unicode content") {
    CStringView str1("Hello 世界");
    CStringView str2("World 宇宙");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 12);
    REQUIRE(std::strcmp(str1.c_str(), "World 宇宙") == 0);
    REQUIRE(str2.size() == 12);
    REQUIRE(std::strcmp(str2.c_str(), "Hello 世界") == 0);
  }

  SECTION("Multiple swaps") {
    CStringView str1("First");
    CStringView str2("Second");
    CStringView str3("Third");

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
    CStringView str1("This is a very long string that tests swap performance");
    CStringView str2("Another very long string for performance testing");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 48);
    REQUIRE(std::strcmp(str1.c_str(), "Another very long string for performance testing") == 0);
    REQUIRE(str2.size() == 54);
    REQUIRE(std::strcmp(str2.c_str(), "This is a very long string that tests swap performance") == 0);
  }

  SECTION("Swap with single character strings") {
    CStringView str1("A");
    CStringView str2("B");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "B") == 0);
    REQUIRE(str2.size() == 1);
    REQUIRE(std::strcmp(str2.c_str(), "A") == 0);
  }
}
