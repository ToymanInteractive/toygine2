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

// Default, copy, C string constructors; edge cases.
TEST_CASE("core/c_string_view/constructors") {
  // Object size equals pointer size.
  SUBCASE("object_size") {
    static_assert(sizeof(CStringView) == sizeof(char *), "CStringView must be pointer-sized");
  }

  // Default constructor yields empty view.
  SUBCASE("default_constructor") {
    constexpr CStringView emptyStr;

    REQUIRE(emptyStr.size() == 0);
    REQUIRE(std::strcmp(emptyStr.c_str(), "") == 0);

    static_assert(emptyStr.size() == 0);
    static_assert(cstrcmp(emptyStr.c_str(), "") == 0);
  }

  // Copy constructor shares underlying pointer.
  SUBCASE("copy_constructor") {
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

    static_assert(copy1.size() == 8);
    static_assert(cstrcmp(copy1.c_str(), "CopyTest") == 0);
    static_assert(copy2.size() == 8);
    static_assert(cstrcmp(copy2.c_str(), "CopyTest") == 0);
    static_assert(copy3.size() == 8);
    static_assert(cstrcmp(copy3.c_str(), "CopyTest") == 0);
  }

  // C string constructor; length from null terminator.
  SUBCASE("c_string_constructor") {
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

    static_assert(str1.size() == 5);
    static_assert(cstrcmp(str1.c_str(), "Hello") == 0);
    static_assert(str2.size() == 5);
    static_assert(cstrcmp(str2.c_str(), "World") == 0);
    static_assert(str3.size() == 4);
    static_assert(cstrcmp(str3.c_str(), "Test") == 0);
    static_assert(str4.size() == 35);
    static_assert(cstrcmp(str4.c_str(), "This is a longer string for testing") == 0);
  }

  // Empty string, single character.
  SUBCASE("edge_cases") {
    // Empty string
    constexpr CStringView empty1("");
    constexpr CStringView empty2("");

    REQUIRE(empty1.empty());
    REQUIRE(empty2.empty());

    static_assert(empty1.empty());
    static_assert(empty2.empty());

    // Single character
    constexpr CStringView single("X");

    REQUIRE(single.size() == 1);
    REQUIRE(std::strcmp(single.c_str(), "X") == 0);

    static_assert(single.size() == 1);
    static_assert(cstrcmp(single.c_str(), "X") == 0);
  }

  // Newline, tab, mixed.
  SUBCASE("special_characters") {
    constexpr CStringView newline("Line1\nLine2");
    constexpr CStringView tab("Col1\tCol2");
    constexpr CStringView mixed("Mix\t\nEnd");

    REQUIRE(newline.size() == 11);
    REQUIRE(std::strcmp(newline.c_str(), "Line1\nLine2") == 0);
    REQUIRE(tab.size() == 9);
    REQUIRE(std::strcmp(tab.c_str(), "Col1\tCol2") == 0);
    REQUIRE(mixed.size() == 8);
    REQUIRE(std::strcmp(mixed.c_str(), "Mix\t\nEnd") == 0);

    static_assert(newline.size() == 11);
    static_assert(cstrcmp(newline.c_str(), "Line1\nLine2") == 0);
    static_assert(tab.size() == 9);
    static_assert(cstrcmp(tab.c_str(), "Col1\tCol2") == 0);
    static_assert(mixed.size() == 8);
    static_assert(cstrcmp(mixed.c_str(), "Mix\t\nEnd") == 0);
  }

  // Unicode and emoji.
  SUBCASE("unicode_content") {
    constexpr CStringView unicode("Привет мир");
    constexpr CStringView emoji("Hello 🌍");

    REQUIRE(unicode.size() == std::char_traits<char>::length("Привет мир"));
    REQUIRE(std::strcmp(unicode.c_str(), "Привет мир") == 0);
    REQUIRE(emoji.size() == std::char_traits<char>::length("Hello 🌍"));
    REQUIRE(std::strcmp(emoji.c_str(), "Hello 🌍") == 0);

    static_assert(unicode.size() == std::char_traits<char>::length("Привет мир"));
    static_assert(cstrcmp(unicode.c_str(), "Привет мир") == 0);
    static_assert(emoji.size() == std::char_traits<char>::length("Hello 🌍"));
    static_assert(cstrcmp(emoji.c_str(), "Hello 🌍") == 0);
  }
}

// Assignment from C string and CStringView.
TEST_CASE("core/c_string_view/operators_assign") {
  SUBCASE("c_string_assignment") {
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
    REQUIRE(str1.empty());
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Long string assignment
    str2 = "This is a longer string";
    REQUIRE(str2.size() == 23);
    REQUIRE(std::strcmp(str2.c_str(), "This is a longer string") == 0);

    constexpr auto constStr1 = CStringView("This is a longer string");
    static_assert(constStr1.size() == 23);
    static_assert(cstrcmp(constStr1.c_str(), "This is a longer string") == 0);
  }

  SUBCASE("c_string_view_assignment") {
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

    constexpr CStringView constStr1("Hello");
    constexpr CStringView constStr2 = constStr1;
    static_assert(constStr2.size() == 5);
    static_assert(cstrcmp(constStr2.c_str(), "Hello") == 0);
  }

  SUBCASE("edge_cases") {
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
    REQUIRE(str1.empty());
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
  }

  SUBCASE("special_characters") {
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

  SUBCASE("unicode_content") {
    CStringView str1;
    CStringView str2;

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

// assign() from C string and CStringView; chaining.
TEST_CASE("core/c_string_view/assign") {
  SUBCASE("c_string_assignment") {
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
    REQUIRE(str1.empty());
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    // Long string assignment
    str2.assign("VeryLongString");
    REQUIRE(str2.size() == 14);
    REQUIRE(std::strcmp(str2.c_str(), "VeryLongString") == 0);

    // Single character
    str3.assign("A");
    REQUIRE(str3.size() == 1);
    REQUIRE(std::strcmp(str3.c_str(), "A") == 0);

    constexpr auto constStr1 = CStringView().assign("Hello");
    constexpr auto constStr2 = CStringView("World").assign("VeryLongString");
    constexpr auto constStr3 = CStringView("A").assign("");
    static_assert(constStr1.size() == 5);
    static_assert(cstrcmp(constStr1.c_str(), "Hello") == 0);
    static_assert(constStr2.size() == 14);
    static_assert(cstrcmp(constStr2.c_str(), "VeryLongString") == 0);
    static_assert(constStr3.empty());
    static_assert(cstrcmp(constStr3.c_str(), "") == 0);
  }

  SUBCASE("c_string_view_assignment") {
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
    REQUIRE(str1.empty());
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);

    constexpr CStringView constStr1("Hello");
    constexpr auto constStr2 = CStringView("World").assign(constStr1);
    constexpr auto constStr3 = CStringView().assign(constStr2);
    static_assert(constStr2.size() == 5);
    static_assert(cstrcmp(constStr2.c_str(), "Hello") == 0);
    static_assert(constStr3.size() == 5);
    static_assert(cstrcmp(constStr3.c_str(), "Hello") == 0);
  }

  SUBCASE("edge_cases") {
    CStringView str1("ABC");
    CStringView str2("ABCD");

    // Assign from own c_str() (no-op path)
    str1.assign(str1.c_str());
    REQUIRE(str1.size() == 3);
    REQUIRE(std::strcmp(str1.c_str(), "ABC") == 0);

    // Assign empty string
    str2.assign("");
    REQUIRE(str2.empty());
    REQUIRE(std::strcmp(str2.c_str(), "") == 0);

    constexpr auto constStr1 = CStringView("ABC").assign("XYZ");
    constexpr auto constStr2 = CStringView("ABCD").assign("");
    static_assert(constStr1.size() == 3);
    static_assert(cstrcmp(constStr1.c_str(), "XYZ") == 0);
    static_assert(constStr2.empty());
    static_assert(cstrcmp(constStr2.c_str(), "") == 0);
  }

  SUBCASE("special_characters") {
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

    constexpr auto constStr1 = CStringView().assign("Hello\n\tWorld");
    constexpr auto constStr2 = CStringView().assign("!@#$%^&*()");
    static_assert(constStr1.size() == 12);
    static_assert(cstrcmp(constStr1.c_str(), "Hello\n\tWorld") == 0);
    static_assert(constStr2.size() == 10);
    static_assert(cstrcmp(constStr2.c_str(), "!@#$%^&*()") == 0);
  }

  SUBCASE("unicode_content") {
    CStringView str1;
    CStringView str2;

    str1.assign("Hello 世界");
    REQUIRE(str1.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(std::strcmp(str1.c_str(), "Hello 世界") == 0);

    str2.assign("Test 🌍");
    REQUIRE(str2.size() == std::char_traits<char>::length("Test 🌍"));
    REQUIRE(std::strcmp(str2.c_str(), "Test 🌍") == 0);

    constexpr auto constStr1 = CStringView().assign("Hello 世界");
    constexpr auto constStr2 = CStringView().assign("Test 🌍");
    static_assert(constStr1.size() == std::char_traits<char>::length("Hello 世界"));
    static_assert(cstrcmp(constStr1.c_str(), "Hello 世界") == 0);
    static_assert(constStr2.size() == std::char_traits<char>::length("Test 🌍"));
    static_assert(cstrcmp(constStr2.c_str(), "Test 🌍") == 0);
  }

  SUBCASE("chaining_assign") {
    CStringView str1;

    // Chaining assign operations
    str1.assign("a").assign("b");
    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "b") == 0);

    // Multiple chaining
    str1.assign("Hello").assign("World").assign("Test");
    REQUIRE(str1.size() == 4);
    REQUIRE(std::strcmp(str1.c_str(), "Test") == 0);

    constexpr auto constStr1 = CStringView("a").assign("b");
    constexpr auto constStr2 = CStringView("Hello").assign("Test");
    static_assert(constStr1.size() == 1);
    static_assert(cstrcmp(constStr1.c_str(), "b") == 0);
    static_assert(constStr2.size() == 4);
    static_assert(cstrcmp(constStr2.c_str(), "Test") == 0);
  }
}

// at() access; bounds-checked.
TEST_CASE("core/c_string_view/at") {
  SUBCASE("at_access") {
    constexpr CStringView str("World");

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
    constexpr CStringView str;

    REQUIRE(str.at(0) == '\0');

    static_assert(str.at(0) == '\0');
  }
}

// operator[] access.
TEST_CASE("core/c_string_view/operator_bracket") {
  SUBCASE("bracket_access") {
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
    static_assert(longStr[13] == 'g');
  }

  SUBCASE("empty_string") {
    constexpr CStringView str;

    static_assert(str[0] == '\0');
  }
}

// front() and back() access.
TEST_CASE("core/c_string_view/front_and_back") {
  SUBCASE("front_method") {
    constexpr CStringView testString("Hello World");

    static_assert(testString.front() == 'H');
    static_assert(testString[0] == 'H');
  }

  SUBCASE("back_method") {
    constexpr CStringView testString("Hello World");

    static_assert(testString.back() == 'd');
    static_assert(testString[testString.size() - 1] == 'd');
  }

  SUBCASE("single_character_string") {
    constexpr CStringView testString("A");

    static_assert(testString.front() == 'A');
    static_assert(testString.back() == 'A');
    static_assert(testString.front() == testString.back());
  }

  SUBCASE("empty_string") {
    constexpr CStringView testString;

    static_assert(testString.front() == '\0');
  }

  SUBCASE("two_character_string") {
    constexpr CStringView testString("AB");

    static_assert(testString.front() == 'A');
    static_assert(testString.back() == 'B');
    static_assert(testString.front() != testString.back());
  }

  SUBCASE("const_references") {
    constexpr CStringView testString("Hello World");

    static_assert(testString.front() == 'H');
    static_assert(testString.back() == 'd');
  }

  SUBCASE("numeric_content") {
    constexpr CStringView testString("12345");

    static_assert(testString.front() == '1');
    static_assert(testString.back() == '5');
  }

  SUBCASE("mixed_content") {
    constexpr CStringView testString("123Hello456");

    static_assert(testString.front() == '1');
    static_assert(testString.back() == '6');
  }

  SUBCASE("long_strings") {
    constexpr CStringView testString("This is a very long string for performance testing");

    static_assert(testString.front() == 'T');
    static_assert(testString.back() == 'g');
  }

  SUBCASE("case_sensitivity") {
    constexpr CStringView testString("Hello World");

    static_assert(testString.front() == 'H'); // Uppercase
    static_assert(testString.back() == 'd'); // Lowercase
  }

  SUBCASE("whitespace_handling") {
    constexpr CStringView testString(" Hello ");

    static_assert(testString.front() == ' ');
    static_assert(testString.back() == ' ');
  }

  SUBCASE("constexpr_operations") {
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("World");
    constexpr CStringView str3("Test");

    // Compile-time front operations
    constexpr const char & front1 = str1.front();
    constexpr const char & front2 = str2.front();
    constexpr const char & front3 = str3.front();

    static_assert(front1 == 'H');
    static_assert(front2 == 'W');
    static_assert(front3 == 'T');

    // Compile-time back operations
    constexpr const char & back1 = str1.back();
    constexpr const char & back2 = str2.back();
    constexpr const char & back3 = str3.back();

    static_assert(back1 == 'o');
    static_assert(back2 == 'd');
    static_assert(back3 == 't');
  }
}

// data() pointer access.
TEST_CASE("core/c_string_view/data") {
  SUBCASE("basic_data_access") {
    constexpr CStringView testString("Hello World");
    constexpr CStringView emptyString("");
    constexpr CStringView singleChar("A");

    // Test that data() points to null-terminated string
    REQUIRE(std::strcmp(testString.data(), "Hello World") == 0);
    REQUIRE(std::strcmp(emptyString.data(), "") == 0);
    REQUIRE(std::strcmp(singleChar.data(), "A") == 0);

    static_assert(cstrcmp(testString.data(), "Hello World") == 0);
    static_assert(cstrcmp(emptyString.data(), "") == 0);
    static_assert(cstrcmp(singleChar.data(), "A") == 0);
  }

  SUBCASE("data_pointer_stability") {
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

    static_assert(testString.data() == testString.data());
    static_assert(copy1.data() == copy1.data());
    static_assert(copy2.data() == copy2.data());

    static_assert(cstrcmp(testString.data(), "Stability Test") == 0);
    static_assert(cstrcmp(copy1.data(), "Stability Test") == 0);
    static_assert(cstrcmp(copy2.data(), "Stability Test") == 0);
  }

  SUBCASE("empty_string") {
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

    // Test data() with empty strings
    REQUIRE(std::strcmp(emptyString.data(), "") == 0);
    REQUIRE(std::strcmp(defaultString.data(), "") == 0);

    static_assert(cstrcmp(emptyString.data(), "") == 0);
    static_assert(cstrcmp(defaultString.data(), "") == 0);
  }
}

// c_str() null-terminated pointer.
TEST_CASE("core/c_string_view/c_str") {
  SUBCASE("basic_c_str_access") {
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

    static_assert(testString.c_str() == testString.data());
    static_assert(emptyString.c_str() == emptyString.data());
    static_assert(singleChar.c_str() == singleChar.data());

    static_assert(cstrcmp(testString.c_str(), "Hello World") == 0);
    static_assert(cstrcmp(emptyString.c_str(), "") == 0);
    static_assert(cstrcmp(singleChar.c_str(), "A") == 0);
  }

  SUBCASE("c_string_stability") {
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

    static_assert(testString.c_str() == testString.c_str());
    static_assert(copy1.c_str() == copy1.c_str());
    static_assert(copy2.c_str() == copy2.c_str());

    static_assert(cstrcmp(testString.c_str(), "Stability Test") == 0);
    static_assert(cstrcmp(copy1.c_str(), "Stability Test") == 0);
    static_assert(cstrcmp(copy2.c_str(), "Stability Test") == 0);
  }

  SUBCASE("empty_string") {
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

    // Test c_str() with empty strings
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);

    static_assert(emptyString.c_str() == emptyString.data());
    static_assert(defaultString.c_str() == defaultString.data());
  }
}

// empty() query.
TEST_CASE("core/c_string_view/empty") {
  SUBCASE("basic_empty_check") {
    constexpr CStringView nonEmptyString("Hello World");
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

    REQUIRE_FALSE(nonEmptyString.empty());
    REQUIRE(emptyString.empty());
    REQUIRE(defaultString.empty());

    static_assert(!nonEmptyString.empty());
    static_assert(emptyString.empty());
    static_assert(defaultString.empty());
  }

  SUBCASE("single_character_strings") {
    constexpr CStringView singleChar("A");
    constexpr CStringView emptyString("");

    REQUIRE_FALSE(singleChar.empty());
    REQUIRE(emptyString.empty());

    static_assert(!singleChar.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("different_capacities") {
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

    static_assert(!smallString.empty());
    static_assert(!mediumString.empty());
    static_assert(!largeString.empty());
    static_assert(emptySmall.empty());
    static_assert(emptyMedium.empty());
    static_assert(emptyLarge.empty());
  }

  SUBCASE("special_characters") {
    constexpr CStringView newlineString("Hello\nWorld");
    constexpr CStringView tabString("Hello\tWorld");
    constexpr CStringView specialString("!@#$%^&*()");
    constexpr CStringView emptyString("");

    REQUIRE_FALSE(newlineString.empty());
    REQUIRE_FALSE(tabString.empty());
    REQUIRE_FALSE(specialString.empty());
    REQUIRE(emptyString.empty());

    static_assert(!newlineString.empty());
    static_assert(!tabString.empty());
    static_assert(!specialString.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("unicode_content") {
    constexpr CStringView unicodeString("Привет мир");
    constexpr CStringView emojiString("Hello 🌍 World");
    constexpr CStringView mixedString("Hello 世界");
    constexpr CStringView emptyString("");

    REQUIRE_FALSE(unicodeString.empty());
    REQUIRE_FALSE(emojiString.empty());
    REQUIRE_FALSE(mixedString.empty());
    REQUIRE(emptyString.empty());

    static_assert(!unicodeString.empty());
    static_assert(!emojiString.empty());
    static_assert(!mixedString.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("numeric_content") {
    constexpr CStringView numericStringView("12345");
    constexpr CStringView floatString("3.14159");
    constexpr CStringView hexString("0xABCD");
    constexpr CStringView emptyString("");

    REQUIRE_FALSE(numericStringView.empty());
    REQUIRE_FALSE(floatString.empty());
    REQUIRE_FALSE(hexString.empty());
    REQUIRE(emptyString.empty());

    static_assert(!numericStringView.empty());
    static_assert(!floatString.empty());
    static_assert(!hexString.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("mixed_content") {
    constexpr CStringView mixedString("Hello123World!@#");
    constexpr CStringView complexString("Test\n123\t!@#");
    constexpr CStringView longString("This is a very long string with mixed content 123!@#");
    constexpr CStringView emptyString("");

    REQUIRE_FALSE(mixedString.empty());
    REQUIRE_FALSE(complexString.empty());
    REQUIRE_FALSE(longString.empty());
    REQUIRE(emptyString.empty());

    static_assert(!mixedString.empty());
    static_assert(!complexString.empty());
    static_assert(!longString.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("maximum_length_strings") {
    constexpr CStringView maxString("123456789012345"); // 15 characters
    constexpr CStringView maxSmall("1234567"); // 7 characters
    constexpr CStringView maxTiny("123"); // 3 characters
    constexpr CStringView emptyString("");

    REQUIRE_FALSE(maxString.empty());
    REQUIRE_FALSE(maxSmall.empty());
    REQUIRE_FALSE(maxTiny.empty());
    REQUIRE(emptyString.empty());

    static_assert(!maxString.empty());
    static_assert(!maxSmall.empty());
    static_assert(!maxTiny.empty());
    static_assert(emptyString.empty());
  }

  SUBCASE("edge_cases") {
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
TEST_CASE("core/c_string_view/size") {
  SUBCASE("basic_size_check") {
    constexpr CStringView testString("Hello World");
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

    REQUIRE(testString.size() == 11);
    REQUIRE(emptyString.size() == 0);
    REQUIRE(defaultString.size() == 0);

    static_assert(testString.size() == 11);
    static_assert(emptyString.size() == 0);
    static_assert(defaultString.size() == 0);
  }

  SUBCASE("single_character_strings") {
    constexpr CStringView singleChar("A");
    constexpr CStringView emptyString("");

    REQUIRE(singleChar.size() == 1);
    REQUIRE(emptyString.size() == 0);

    static_assert(singleChar.size() == 1);
    static_assert(emptyString.size() == 0);
  }

  SUBCASE("different_capacities") {
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

    static_assert(smallString.size() == 2);
    static_assert(mediumString.size() == 11);
    static_assert(largeString.size() == 23);
    static_assert(emptySmall.size() == 0);
    static_assert(emptyMedium.size() == 0);
    static_assert(emptyLarge.size() == 0);
  }

  SUBCASE("special_characters") {
    constexpr CStringView newlineString("Hello\nWorld");
    constexpr CStringView tabString("Hello\tWorld");
    constexpr CStringView specialString("!@#$%^&*()");
    constexpr CStringView emptyString("");

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
    constexpr CStringView unicodeString("Привет мир");
    constexpr CStringView emojiString("Hello 🌍 World");
    constexpr CStringView mixedString("Hello 世界");
    constexpr CStringView emptyString("");

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
    constexpr CStringView numericStringView("12345");
    constexpr CStringView floatString("3.14159");
    constexpr CStringView hexString("0xABCD");
    constexpr CStringView emptyString("");

    REQUIRE(numericStringView.size() == 5);
    REQUIRE(floatString.size() == 7);
    REQUIRE(hexString.size() == 6);
    REQUIRE(emptyString.size() == 0);

    static_assert(numericStringView.size() == 5);
    static_assert(floatString.size() == 7);
    static_assert(hexString.size() == 6);
    static_assert(emptyString.size() == 0);
  }

  SUBCASE("mixed_content") {
    constexpr CStringView mixedString("Hello123World!@#");
    constexpr CStringView complexString("Test\n123\t!@#");
    constexpr CStringView longString("This is a very long string with mixed content 123!@#");
    constexpr CStringView emptyString("");

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
    constexpr CStringView maxString("123456789012345"); // 15 characters
    constexpr CStringView maxSmall("1234567"); // 7 characters
    constexpr CStringView maxTiny("123"); // 3 characters
    constexpr CStringView emptyString("");

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
    constexpr CStringView singleChar("A");
    constexpr CStringView twoChars("AB");
    constexpr CStringView emptyString("");
    constexpr CStringView defaultString;

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
TEST_CASE("core/c_string_view/utf8_size") {
  SUBCASE("ascii_strings") {
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

  SUBCASE("utf8_cyrillic_text") {
    // "Привет мир" in UTF-8
    constexpr const char * cyrillicText = "Привет мир";

    constexpr CStringView cyrillicStringView(cyrillicText);

    REQUIRE(cyrillicStringView.size() == std::char_traits<char>::length("Привет мир"));
    REQUIRE(cyrillicStringView.utf8_size() == 10);
  }

  SUBCASE("mixed_ascii_and_utf8") {
    // "Hello 世界" in UTF-8
    constexpr const char * mixedText = "Hello 世界";

    constexpr CStringView mixedString(mixedText);

    REQUIRE(mixedString.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(mixedString.utf8_size() == 8); // 6 ASCII + 2 Chinese characters
  }

  SUBCASE("emoji_characters") {
    // "Hello 🌍" in UTF-8
    constexpr const char * emojiText = "Hello 🌍";

    constexpr CStringView emojiString(emojiText);

    REQUIRE(emojiString.size() == std::char_traits<char>::length("Hello 🌍"));
    REQUIRE(emojiString.utf8_size() == 7); // 6 ASCII + 1 emoji
  }

  SUBCASE("special_characters") {
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

  SUBCASE("edge_cases") {
    constexpr CStringView singleByte("A");
    constexpr CStringView twoByte("А"); // Cyrillic A
    constexpr CStringView threeByte("中"); // Chinese character
    constexpr CStringView fourByte("🌍"); // Emoji

    REQUIRE(singleByte.utf8_size() == 1);
    REQUIRE(twoByte.utf8_size() == 1);
    REQUIRE(threeByte.utf8_size() == 1);
    REQUIRE(fourByte.utf8_size() == 1);
  }

  SUBCASE("long_utf8_text") {
    // "ToyGine2 - Бесплатный 2D/3D игровой движок." in UTF-8
    constexpr const char * longUtf8Text = "ToyGine2 - Бесплатный 2D/3D игровой движок.";

    constexpr CStringView longString(longUtf8Text);

    REQUIRE(longString.size() == std::char_traits<char>::length("ToyGine2 - Бесплатный 2D/3D игровой движок."));
    REQUIRE(longString.utf8_size() == 43); // 43 characters
  }
}

// length() alias for size().
TEST_CASE("core/c_string_view/length") {
  SUBCASE("basic_length_check") {
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

    static_assert(testString.length() == 11);
    static_assert(emptyString.length() == 0);
    static_assert(singleChar.length() == 1);
  }

  SUBCASE("different_capacities") {
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

    static_assert(smallString.length() == 2);
    static_assert(mediumString.length() == 11);
    static_assert(largeString.length() == 23);
  }

  SUBCASE("special_characters") {
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

    static_assert(newlineString.length() == 11);
    static_assert(tabString.length() == 11);
    static_assert(specialString.length() == 10);
  }

  SUBCASE("unicode_content") {
    constexpr CStringView unicodeString("Привет мир");
    constexpr CStringView emojiString("Hello 🌍 World");
    constexpr CStringView mixedString("Hello 世界");
    constexpr CStringView emptyString("");

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

    static_assert(numericStringView.length() == 5);
    static_assert(floatString.length() == 7);
    static_assert(hexString.length() == 6);
  }

  SUBCASE("mixed_content") {
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

    static_assert(mixedString.length() == 16);
    static_assert(complexString.length() == 12);
    static_assert(longString.length() == 52);
  }

  SUBCASE("edge_cases") {
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

    static_assert(singleChar.length() == 1);
    static_assert(twoChars.length() == 2);
    static_assert(emptyString.length() == 0);
    static_assert(defaultString.length() == 0);
  }
}

// max_size() maximum view length.
TEST_CASE("core/c_string_view/max_size") {
  SUBCASE("basic_max_size_check") {
    constexpr CStringView smallString("Hi");
    constexpr CStringView mediumString("Hello World");
    constexpr CStringView largeString("This is a longer string");
    constexpr CStringView extraLargeString("This is an even longer string for testing");

    REQUIRE(smallString.max_size() == 2);
    REQUIRE(mediumString.max_size() == 11);
    REQUIRE(largeString.max_size() == 23);
    REQUIRE(extraLargeString.max_size() == 41);

    static_assert(smallString.max_size() == 2);
    static_assert(mediumString.max_size() == 11);
    static_assert(largeString.max_size() == 23);
    static_assert(extraLargeString.max_size() == 41);
  }

  SUBCASE("empty_string") {
    constexpr CStringView empty("");

    REQUIRE(empty.max_size() == 0);

    static_assert(empty.max_size() == 0);
  }

  SUBCASE("default_constructed_string") {
    constexpr CStringView defaultString;

    REQUIRE(defaultString.max_size() == 0);

    static_assert(defaultString.max_size() == 0);
  }

  SUBCASE("single_character_string") {
    constexpr CStringView single("A");

    REQUIRE(single.max_size() == 1);

    static_assert(single.max_size() == 1);
  }
}

// capacity() (same as size for view).
TEST_CASE("core/c_string_view/capacity") {
  SUBCASE("basic_capacity_check") {
    constexpr CStringView smallString("Hi");
    constexpr CStringView mediumString("Hello World");
    constexpr CStringView largeString("This is a longer string");
    constexpr CStringView extraLargeString("This is an even longer string for testing");

    REQUIRE(smallString.capacity() == 2);
    REQUIRE(mediumString.capacity() == 11);
    REQUIRE(largeString.capacity() == 23);
    REQUIRE(extraLargeString.capacity() == 41);

    static_assert(smallString.capacity() == 2);
    static_assert(mediumString.capacity() == 11);
    static_assert(largeString.capacity() == 23);
    static_assert(extraLargeString.capacity() == 41);
  }

  SUBCASE("empty_string") {
    constexpr CStringView empty("");

    REQUIRE(empty.capacity() == 0);

    static_assert(empty.capacity() == 0);
  }

  SUBCASE("default_constructed_string") {
    constexpr CStringView defaultString;

    REQUIRE(defaultString.capacity() == 0);

    static_assert(defaultString.capacity() == 0);
  }

  SUBCASE("single_character_string") {
    constexpr CStringView single("A");

    REQUIRE(single.capacity() == 1);

    static_assert(single.capacity() == 1);
  }
}

// clear(); view becomes empty.
TEST_CASE("core/c_string_view/clear") {
  SUBCASE("basic_clear_functionality") {
    CStringView testString("Hello World");

    REQUIRE_FALSE(testString.empty());
    REQUIRE(std::strcmp(testString.c_str(), "Hello World") == 0);

    testString.clear();

    REQUIRE(testString.empty());
    REQUIRE(std::strcmp(testString.c_str(), "") == 0);
  }

  SUBCASE("clear_empty_string") {
    CStringView emptyString("");

    REQUIRE(emptyString.empty());
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);

    emptyString.clear();

    REQUIRE(emptyString.empty());
    REQUIRE(std::strcmp(emptyString.c_str(), "") == 0);
  }

  SUBCASE("clear_default_constructed_string") {
    CStringView defaultString;

    REQUIRE(defaultString.empty());
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);

    defaultString.clear();

    REQUIRE(defaultString.empty());
    REQUIRE(std::strcmp(defaultString.c_str(), "") == 0);
  }

  SUBCASE("clear_single_character_string") {
    CStringView singleChar("A");

    REQUIRE_FALSE(singleChar.empty());
    REQUIRE(std::strcmp(singleChar.c_str(), "A") == 0);

    singleChar.clear();

    REQUIRE(singleChar.empty());
    REQUIRE(std::strcmp(singleChar.c_str(), "") == 0);
  }

  SUBCASE("clear_longer_string") {
    CStringView maxString("1234567890");

    REQUIRE_FALSE(maxString.empty());
    REQUIRE(std::strcmp(maxString.c_str(), "1234567890") == 0);

    maxString.clear();

    REQUIRE(maxString.empty());
    REQUIRE(std::strcmp(maxString.c_str(), "") == 0);
  }

  SUBCASE("clear_different_capacities") {
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

  SUBCASE("clear_special_characters") {
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

  SUBCASE("clear_unicode_content") {
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

  SUBCASE("clear_and_reassignment") {
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

  SUBCASE("multiple_clear_operations") {
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

// swap() member.
TEST_CASE("core/c_string_view/swap") {
  SUBCASE("swap_two_different_strings") {
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

  SUBCASE("swap_strings_of_different_lengths") {
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

  SUBCASE("swap_with_empty_string") {
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

  SUBCASE("swap_two_empty_strings") {
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

  SUBCASE("self_swap_no_op") {
    CStringView string1("Hello World");

    REQUIRE(string1.size() == 11);
    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);

    string1.swap(string1);

    REQUIRE(std::strcmp(string1.c_str(), "Hello World") == 0);
    REQUIRE(string1.size() == 11);
  }

  SUBCASE("swap_with_single_character_strings") {
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

  SUBCASE("chained_swap_operations") {
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

  SUBCASE("swap_with_repeated_characters") {
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

  SUBCASE("swap_with_special_characters") {
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

  SUBCASE("swap_with_unicode_content") {
    CStringView string1("Hello 世界");
    CStringView string2("Привет мир");

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

  SUBCASE("swap_with_mixed_content") {
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

// find() substring and character.
TEST_CASE("core/c_string_view/find") {
  SUBCASE("find_c_string_view_substring") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find(CStringView("World")) == 6);
    REQUIRE(testString.find(CStringView("Hello")) == 0);
    REQUIRE(testString.find(CStringView("lo Wo")) == 3);
    REQUIRE(testString.find(CStringView("xyz")) == CStringView::npos);

    static_assert(testString.find(CStringView("World")) == 6);
    static_assert(testString.find(CStringView("Hello")) == 0);
    static_assert(testString.find(CStringView("lo Wo")) == 3);
    static_assert(testString.find(CStringView("xyz")) == CStringView::npos);
  }

  SUBCASE("find_string_like_substring") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find(std::string("World")) == 6);
    REQUIRE(testString.find(std::string("Hello")) == 0);
    REQUIRE(testString.find(std::string("lo Wo")) == 3);
    REQUIRE(testString.find(std::string("xyz")) == CStringView::npos);

    static_assert(testString.find(CStringView("World")) == 6);
    static_assert(testString.find(CStringView("Hello")) == 0);
    static_assert(testString.find(CStringView("lo Wo")) == 3);
    static_assert(testString.find(CStringView("xyz")) == CStringView::npos);
  }

  SUBCASE("find_c_string_substring") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("lo Wo") == 3);
    REQUIRE(testString.find("xyz") == CStringView::npos);

    static_assert(testString.find("World") == 6);
    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find("lo Wo") == 3);
    static_assert(testString.find("xyz") == CStringView::npos);
  }

  SUBCASE("find_character") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find('H') == 0);
    REQUIRE(testString.find('l') == 2);
    REQUIRE(testString.find('o') == 4);
    REQUIRE(testString.find('W') == 6);
    REQUIRE(testString.find('d') == 10);
    REQUIRE(testString.find('x') == CStringView::npos);

    static_assert(testString.find('H') == 0);
    static_assert(testString.find('l') == 2);
    static_assert(testString.find('o') == 4);
    static_assert(testString.find('W') == 6);
    static_assert(testString.find('d') == 10);
    static_assert(testString.find('x') == CStringView::npos);
  }

  SUBCASE("find_with_position_parameter") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == 12);
    REQUIRE(testString.find("Hello", 13) == CStringView::npos);
    REQUIRE(testString.find('l', 0) == 2);
    REQUIRE(testString.find('l', 3) == 3);
    REQUIRE(testString.find('l', 4) == 9);
    REQUIRE(testString.find('l', 10) == 14);

    static_assert(testString.find("Hello", 0) == 0);
    static_assert(testString.find("Hello", 1) == 12);
    static_assert(testString.find("Hello", 13) == CStringView::npos);
    static_assert(testString.find('l', 0) == 2);
    static_assert(testString.find('l', 3) == 3);
    static_assert(testString.find('l', 4) == 9);
    static_assert(testString.find('l', 10) == 14);
  }

  SUBCASE("find_empty_substring") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find(CStringView("")) == 0);
    REQUIRE(testString.find(std::string("")) == 0);
    REQUIRE(testString.find("") == 0);
    REQUIRE(testString.find("", 5) == 5);
    REQUIRE(testString.find("", 11) == 11);
    REQUIRE(testString.find("", 12) == CStringView::npos);

    static_assert(testString.find(CStringView("")) == 0);
    static_assert(testString.find(FixedString<8>("")) == 0);
    static_assert(testString.find("") == 0);
    static_assert(testString.find("", 5) == 5);
    static_assert(testString.find("", 11) == 11);
    static_assert(testString.find("", 12) == CStringView::npos);
  }

  SUBCASE("find_in_empty_string") {
    constexpr CStringView testString("");

    REQUIRE(testString.find(CStringView("Hello")) == CStringView::npos);
    REQUIRE(testString.find(std::string("Hello")) == CStringView::npos);
    REQUIRE(testString.find("Hello") == CStringView::npos);
    REQUIRE(testString.find('H') == CStringView::npos);
    REQUIRE(testString.find("") == 0);

    static_assert(testString.find(CStringView("Hello")) == CStringView::npos);
    static_assert(testString.find(FixedString<8>("Hello")) == CStringView::npos);
    static_assert(testString.find("Hello") == CStringView::npos);
    static_assert(testString.find('H') == CStringView::npos);
    static_assert(testString.find("") == 0);
  }

  SUBCASE("find_with_position_beyond_string_size") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find("World", 10) == CStringView::npos);
    REQUIRE(testString.find('H', 10) == CStringView::npos);
    REQUIRE(testString.find("", 10) == CStringView::npos);

    static_assert(testString.find("World", 10) == CStringView::npos);
    static_assert(testString.find('H', 10) == CStringView::npos);
    static_assert(testString.find("", 10) == CStringView::npos);
  }

  SUBCASE("find_substring_at_end") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("World") == 6);
    REQUIRE(testString.find("d") == 10);
    REQUIRE(testString.find("ld") == 9);

    static_assert(testString.find("World") == 6);
    static_assert(testString.find("d") == 10);
    static_assert(testString.find("ld") == 9);
  }

  SUBCASE("find_substring_at_beginning") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("H") == 0);
    REQUIRE(testString.find("He") == 0);

    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find("H") == 0);
    static_assert(testString.find("He") == 0);
  }

  SUBCASE("find_overlapping_substrings") {
    constexpr CStringView testString("ababab");

    REQUIRE(testString.find("ab") == 0);
    REQUIRE(testString.find("ab", 1) == 2);
    REQUIRE(testString.find("ab", 3) == 4);
    REQUIRE(testString.find("ab", 5) == CStringView::npos);

    static_assert(testString.find("ab") == 0);
    static_assert(testString.find("ab", 1) == 2);
    static_assert(testString.find("ab", 3) == 4);
    static_assert(testString.find("ab", 5) == CStringView::npos);
  }

  SUBCASE("find_with_repeated_characters") {
    constexpr CStringView testString("aaaaa");

    REQUIRE(testString.find("aa") == 0);
    REQUIRE(testString.find("aa", 1) == 1);
    REQUIRE(testString.find("aa", 2) == 2);
    REQUIRE(testString.find("aa", 3) == 3);
    REQUIRE(testString.find("aa", 4) == CStringView::npos);

    static_assert(testString.find("aa") == 0);
    static_assert(testString.find("aa", 1) == 1);
    static_assert(testString.find("aa", 2) == 2);
    static_assert(testString.find("aa", 3) == 3);
    static_assert(testString.find("aa", 4) == CStringView::npos);
  }

  SUBCASE("find_case_sensitivity") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("hello") == CStringView::npos);
    REQUIRE(testString.find("WORLD") == CStringView::npos);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("World") == 6);

    static_assert(testString.find("hello") == CStringView::npos);
    static_assert(testString.find("WORLD") == CStringView::npos);
    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find("World") == 6);
  }

  SUBCASE("find_with_different_c_string_view_capacities") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find(CStringView("World")) == 6);

    static_assert(testString.find(CStringView("World")) == 6);
  }

  SUBCASE("find_with_exact_match") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == CStringView::npos);

    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find("Hello", 0) == 0);
    static_assert(testString.find("Hello", 1) == CStringView::npos);
  }

  SUBCASE("find_with_single_character_string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.find("A") == 0);
    REQUIRE(testString.find('A') == 0);
    REQUIRE(testString.find("B") == CStringView::npos);
    REQUIRE(testString.find('B') == CStringView::npos);

    static_assert(testString.find("A") == 0);
    static_assert(testString.find('A') == 0);
    static_assert(testString.find("B") == CStringView::npos);
    static_assert(testString.find('B') == CStringView::npos);
  }

  SUBCASE("find_with_special_characters") {
    constexpr CStringView testString("Hello\n\tWorld!");

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
    constexpr CStringView testString("Hello 世界");

    REQUIRE(testString.find("世界") == 6);
    REQUIRE(testString.find("Hello") == 0);
    REQUIRE(testString.find(" ") == 5);

    static_assert(testString.find("世界") == 6);
    static_assert(testString.find("Hello") == 0);
    static_assert(testString.find(" ") == 5);
  }

  SUBCASE("find_with_numeric_content") {
    constexpr CStringView testString("12345Hello67890");

    REQUIRE(testString.find("12345") == 0);
    REQUIRE(testString.find("Hello") == 5);
    REQUIRE(testString.find("67890") == 10);
    REQUIRE(testString.find("456") == CStringView::npos);

    static_assert(testString.find("12345") == 0);
    static_assert(testString.find("Hello") == 5);
    static_assert(testString.find("67890") == 10);
    static_assert(testString.find("456") == CStringView::npos);
  }

  SUBCASE("find_with_mixed_content") {
    constexpr CStringView testString("Hello123World!@#");

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
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find("Hello", 0) == 0);
    REQUIRE(testString.find("Hello", 1) == CStringView::npos);
    REQUIRE(testString.find("World", 6) == 6);
    REQUIRE(testString.find("World", 7) == CStringView::npos);
    REQUIRE(testString.find("", 0) == 0);
    REQUIRE(testString.find("", 11) == 11);
    REQUIRE(testString.find("", 12) == CStringView::npos);

    static_assert(testString.find("Hello", 0) == 0);
    static_assert(testString.find("Hello", 1) == CStringView::npos);
    static_assert(testString.find("World", 6) == 6);
    static_assert(testString.find("World", 7) == CStringView::npos);
    static_assert(testString.find("", 0) == 0);
    static_assert(testString.find("", 11) == 11);
    static_assert(testString.find("", 12) == CStringView::npos);
  }
}

// rfind() from end.
TEST_CASE("core/c_string_view/rfind") {
  SUBCASE("rfind_c_string_view_substring") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind(CStringView("Hello")) == 12);
    REQUIRE(testString.rfind(CStringView("World")) == 6);
    REQUIRE(testString.rfind(CStringView("lo")) == 15);
    REQUIRE(testString.rfind(CStringView("xyz")) == CStringView::npos);

    static_assert(testString.rfind(CStringView("Hello")) == 12);
    static_assert(testString.rfind(CStringView("World")) == 6);
    static_assert(testString.rfind(CStringView("lo")) == 15);
    static_assert(testString.rfind(CStringView("xyz")) == CStringView::npos);
  }

  SUBCASE("rfind_string_like_substring") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind(std::string("Hello")) == 12);
    REQUIRE(testString.rfind(std::string("World")) == 6);
    REQUIRE(testString.rfind(std::string("lo")) == 15);
    REQUIRE(testString.rfind(std::string("xyz")) == CStringView::npos);

    static_assert(testString.rfind(CStringView("Hello")) == 12);
    static_assert(testString.rfind(CStringView("World")) == 6);
    static_assert(testString.rfind(CStringView("lo")) == 15);
    static_assert(testString.rfind(CStringView("xyz")) == CStringView::npos);
  }

  SUBCASE("rfind_c_string_substring") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("lo") == 15);
    REQUIRE(testString.rfind("xyz") == CStringView::npos);

    static_assert(testString.rfind("Hello") == 12);
    static_assert(testString.rfind("World") == 6);
    static_assert(testString.rfind("lo") == 15);
    static_assert(testString.rfind("xyz") == CStringView::npos);
  }

  SUBCASE("rfind_character") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind('H') == 12);
    REQUIRE(testString.rfind('l') == 15);
    REQUIRE(testString.rfind('o') == 16);
    REQUIRE(testString.rfind('W') == 6);
    REQUIRE(testString.rfind('d') == 10);
    REQUIRE(testString.rfind('x') == CStringView::npos);

    static_assert(testString.rfind('H') == 12);
    static_assert(testString.rfind('l') == 15);
    static_assert(testString.rfind('o') == 16);
    static_assert(testString.rfind('W') == 6);
    static_assert(testString.rfind('d') == 10);
    static_assert(testString.rfind('x') == CStringView::npos);
  }

  SUBCASE("rfind_with_position_parameter") {
    constexpr CStringView testString("Hello World Hello");

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

  SUBCASE("rfind_empty_substring") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.rfind(CStringView("")) == 11);
    REQUIRE(testString.rfind(std::string("")) == 11);
    REQUIRE(testString.rfind("") == 11);
    REQUIRE(testString.rfind("", 5) == 5);
    REQUIRE(testString.rfind("", 0) == 0);

    static_assert(testString.rfind(CStringView("")) == 11);
    static_assert(testString.rfind(FixedString<4>("")) == 11);
    static_assert(testString.rfind("") == 11);
    static_assert(testString.rfind("", 5) == 5);
    static_assert(testString.rfind("", 0) == 0);
  }

  SUBCASE("rfind_in_empty_string") {
    constexpr CStringView testString("");

    REQUIRE(testString.rfind(CStringView("Hello")) == CStringView::npos);
    REQUIRE(testString.rfind(std::string("Hello")) == CStringView::npos);
    REQUIRE(testString.rfind("Hello") == CStringView::npos);
    REQUIRE(testString.rfind('H') == CStringView::npos);
    REQUIRE(testString.rfind("") == 0);

    static_assert(testString.rfind(CStringView("Hello")) == CStringView::npos);
    static_assert(testString.rfind(FixedString<8>("Hello")) == CStringView::npos);
    static_assert(testString.rfind("Hello") == CStringView::npos);
    static_assert(testString.rfind('H') == CStringView::npos);
    static_assert(testString.rfind("") == 0);
  }

  SUBCASE("rfind_substring_at_end") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.rfind("World") == 6);
    REQUIRE(testString.rfind("d") == 10);
    REQUIRE(testString.rfind("ld") == 9);

    static_assert(testString.rfind("World") == 6);
    static_assert(testString.rfind("d") == 10);
    static_assert(testString.rfind("ld") == 9);
  }

  SUBCASE("rfind_substring_at_beginning") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("H") == 12);
    REQUIRE(testString.rfind("He") == 12);

    static_assert(testString.rfind("Hello") == 12);
    static_assert(testString.rfind("H") == 12);
    static_assert(testString.rfind("He") == 12);
  }

  SUBCASE("rfind_overlapping_substrings") {
    constexpr CStringView testString("ababab");

    REQUIRE(testString.rfind("ab") == 4);
    REQUIRE(testString.rfind("ab", 3) == 2);
    REQUIRE(testString.rfind("ab", 1) == 0);
    REQUIRE(testString.rfind("ab", 0) == 0);
    REQUIRE(testString.rfind("ab", 5) == CStringView::npos);

    static_assert(testString.rfind("ab") == 4);
    static_assert(testString.rfind("ab", 3) == 2);
    static_assert(testString.rfind("ab", 1) == 0);
    static_assert(testString.rfind("ab", 0) == 0);
    static_assert(testString.rfind("ab", 5) == CStringView::npos);
  }

  SUBCASE("rfind_with_repeated_characters") {
    constexpr CStringView testString("aaaaa");

    REQUIRE(testString.rfind("aa") == 3);
    REQUIRE(testString.rfind("aa", 2) == 2);
    REQUIRE(testString.rfind("aa", 1) == 1);
    REQUIRE(testString.rfind("aa", 0) == 0);

    static_assert(testString.rfind("aa") == 3);
    static_assert(testString.rfind("aa", 2) == 2);
    static_assert(testString.rfind("aa", 1) == 1);
    static_assert(testString.rfind("aa", 0) == 0);
  }

  SUBCASE("rfind_case_sensitivity") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.rfind("hello") == CStringView::npos);
    REQUIRE(testString.rfind("WORLD") == CStringView::npos);
    REQUIRE(testString.rfind("Hello") == 12);
    REQUIRE(testString.rfind("World") == 6);

    static_assert(testString.rfind("hello") == CStringView::npos);
    static_assert(testString.rfind("WORLD") == CStringView::npos);
    static_assert(testString.rfind("Hello") == 12);
    static_assert(testString.rfind("World") == 6);
  }

  SUBCASE("rfind_with_exact_match") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.rfind("Hello") == 0);
    REQUIRE(testString.rfind("Hello", 0) == 0);

    static_assert(testString.rfind("Hello") == 0);
    static_assert(testString.rfind("Hello", 0) == 0);
  }

  SUBCASE("rfind_with_single_character_string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.rfind("A") == 0);
    REQUIRE(testString.rfind('A') == 0);
    REQUIRE(testString.rfind("B") == CStringView::npos);
    REQUIRE(testString.rfind('B') == CStringView::npos);

    static_assert(testString.rfind("A") == 0);
    static_assert(testString.rfind('A') == 0);
    static_assert(testString.rfind("B") == CStringView::npos);
    static_assert(testString.rfind('B') == CStringView::npos);
  }

  SUBCASE("rfind_with_position_0") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.rfind("Hello", 0) == 0);
    REQUIRE(testString.rfind("World", 0) == CStringView::npos);
    REQUIRE(testString.rfind('H', 0) == 0);
    REQUIRE(testString.rfind('W', 0) == CStringView::npos);

    static_assert(testString.rfind("Hello", 0) == 0);
    static_assert(testString.rfind("World", 0) == CStringView::npos);
    static_assert(testString.rfind('H', 0) == 0);
    static_assert(testString.rfind('W', 0) == CStringView::npos);
  }

  SUBCASE("rfind_with_substring_longer_than_string") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.rfind("Hello World") == CStringView::npos);
    REQUIRE(testString.rfind("Hello World", 10) == CStringView::npos);

    static_assert(testString.rfind("Hello World") == CStringView::npos);
    static_assert(testString.rfind("Hello World", 10) == CStringView::npos);
  }

  SUBCASE("rfind_with_multiple_occurrences") {
    constexpr CStringView testString("abababab");

    REQUIRE(testString.rfind("ab") == 6);
    REQUIRE(testString.rfind("ab", 5) == 4);
    REQUIRE(testString.rfind("ab", 3) == 2);
    REQUIRE(testString.rfind("ab", 1) == 0);

    static_assert(testString.rfind("ab") == 6);
    static_assert(testString.rfind("ab", 5) == 4);
    static_assert(testString.rfind("ab", 3) == 2);
    static_assert(testString.rfind("ab", 1) == 0);
  }

  SUBCASE("rfind_with_position_in_middle") {
    constexpr CStringView testString("Hello World Hello");

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
TEST_CASE("core/c_string_view/find_first_of") {
  SUBCASE("find_first_of_c_string_view_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of(CStringView("aeiou")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_of(CStringView("H")) == 0);
    REQUIRE(testString.find_first_of(CStringView("d")) == 10);
    REQUIRE(testString.find_first_of(CStringView("xyz")) == CStringView::npos);

    static_assert(testString.find_first_of(CStringView("aeiou")) == 1);
    static_assert(testString.find_first_of(CStringView("H")) == 0);
    static_assert(testString.find_first_of(CStringView("d")) == 10);
    static_assert(testString.find_first_of(CStringView("xyz")) == CStringView::npos);
  }

  SUBCASE("find_first_of_string_like_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of(std::string("aeiou")) == 1);
    REQUIRE(testString.find_first_of(std::string("H")) == 0);
    REQUIRE(testString.find_first_of(std::string("d")) == 10);
    REQUIRE(testString.find_first_of(std::string("xyz")) == CStringView::npos);

    static_assert(testString.find_first_of(CStringView("aeiou")) == 1);
    static_assert(testString.find_first_of(CStringView("H")) == 0);
    static_assert(testString.find_first_of(CStringView("d")) == 10);
    static_assert(testString.find_first_of(CStringView("xyz")) == CStringView::npos);
  }

  SUBCASE("find_first_of_c_string_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou") == 1);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("d") == 10);
    REQUIRE(testString.find_first_of("xyz") == CStringView::npos);

    static_assert(testString.find_first_of("aeiou") == 1);
    static_assert(testString.find_first_of("H") == 0);
    static_assert(testString.find_first_of("d") == 10);
    static_assert(testString.find_first_of("xyz") == CStringView::npos);
  }

  SUBCASE("find_first_of_single_character") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of('H') == 0);
    REQUIRE(testString.find_first_of('e') == 1);
    REQUIRE(testString.find_first_of('l') == 2);
    REQUIRE(testString.find_first_of('o') == 4);
    REQUIRE(testString.find_first_of('W') == 6);
    REQUIRE(testString.find_first_of('d') == 10);
    REQUIRE(testString.find_first_of('x') == CStringView::npos);

    static_assert(testString.find_first_of('H') == 0);
    static_assert(testString.find_first_of('e') == 1);
    static_assert(testString.find_first_of('l') == 2);
    static_assert(testString.find_first_of('o') == 4);
    static_assert(testString.find_first_of('W') == 6);
    static_assert(testString.find_first_of('d') == 10);
    static_assert(testString.find_first_of('x') == CStringView::npos);
  }

  SUBCASE("find_first_of_with_position_parameter") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("aeiou", 2) == 4);
    REQUIRE(testString.find_first_of("aeiou", 5) == 7);
    REQUIRE(testString.find_first_of("aeiou", 8) == 13);
    REQUIRE(testString.find_first_of("aeiou", 14) == 16);
    REQUIRE(testString.find_first_of("aeiou", 17) == CStringView::npos);

    static_assert(testString.find_first_of("aeiou", 0) == 1);
    static_assert(testString.find_first_of("aeiou", 2) == 4);
    static_assert(testString.find_first_of("aeiou", 5) == 7);
    static_assert(testString.find_first_of("aeiou", 8) == 13);
    static_assert(testString.find_first_of("aeiou", 14) == 16);
    static_assert(testString.find_first_of("aeiou", 17) == CStringView::npos);
  }

  SUBCASE("find_first_of_empty_character_set") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of(CStringView("")) == CStringView::npos);
    REQUIRE(testString.find_first_of(std::string("")) == CStringView::npos);
    REQUIRE(testString.find_first_of("") == CStringView::npos);

    static_assert(testString.find_first_of(CStringView("")) == CStringView::npos);
    static_assert(testString.find_first_of(FixedString<4>("")) == CStringView::npos);
    static_assert(testString.find_first_of("") == CStringView::npos);
  }

  SUBCASE("find_first_of_in_empty_string") {
    constexpr CStringView testString("");

    REQUIRE(testString.find_first_of(CStringView("aeiou")) == CStringView::npos);
    REQUIRE(testString.find_first_of(std::string("aeiou")) == CStringView::npos);
    REQUIRE(testString.find_first_of("aeiou") == CStringView::npos);
    REQUIRE(testString.find_first_of('a') == CStringView::npos);

    static_assert(testString.find_first_of(CStringView("aeiou")) == CStringView::npos);
    static_assert(testString.find_first_of(FixedString<8>("aeiou")) == CStringView::npos);
    static_assert(testString.find_first_of("aeiou") == CStringView::npos);
    static_assert(testString.find_first_of('a') == CStringView::npos);
  }

  SUBCASE("find_first_of_with_position_beyond_string_size") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_first_of("aeiou", 10) == CStringView::npos);
    REQUIRE(testString.find_first_of('a', 10) == CStringView::npos);

    static_assert(testString.find_first_of("aeiou", 10) == CStringView::npos);
    static_assert(testString.find_first_of('a', 10) == CStringView::npos);
  }

  SUBCASE("find_first_of_with_repeated_characters") {
    constexpr CStringView testString("aaaaa");

    REQUIRE(testString.find_first_of("a") == 0);
    REQUIRE(testString.find_first_of("ab") == 0);
    REQUIRE(testString.find_first_of("b") == CStringView::npos);

    static_assert(testString.find_first_of("a") == 0);
    static_assert(testString.find_first_of("ab") == 0);
    static_assert(testString.find_first_of("b") == CStringView::npos);
  }

  SUBCASE("find_first_of_with_multiple_character_sets") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of("Hl") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_of("lo") == 2); // 'l' at position 2
    REQUIRE(testString.find_first_of("Wr") == 6); // 'W' at position 6
    REQUIRE(testString.find_first_of("dl") == 2); // 'l' at position 2

    static_assert(testString.find_first_of("Hl") == 0);
    static_assert(testString.find_first_of("lo") == 2);
    static_assert(testString.find_first_of("Wr") == 6);
    static_assert(testString.find_first_of("dl") == 2);
  }

  SUBCASE("find_first_of_case_sensitivity") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of("h") == CStringView::npos);
    REQUIRE(testString.find_first_of("H") == 0);
    REQUIRE(testString.find_first_of("w") == CStringView::npos);
    REQUIRE(testString.find_first_of("W") == 6);

    static_assert(testString.find_first_of("h") == CStringView::npos);
    static_assert(testString.find_first_of("H") == 0);
    static_assert(testString.find_first_of("w") == CStringView::npos);
    static_assert(testString.find_first_of("W") == 6);
  }

  SUBCASE("find_first_of_with_special_characters") {
    constexpr CStringView testString("Hello, World!");

    REQUIRE(testString.find_first_of("!,") == 5); // ',' at position 5
    REQUIRE(testString.find_first_of("!") == 12); // '!' at position 12
    REQUIRE(testString.find_first_of(".,!") == 5); // ',' at position 5

    static_assert(testString.find_first_of("!,") == 5);
    static_assert(testString.find_first_of("!") == 12);
    static_assert(testString.find_first_of(".,!") == 5);
  }

  SUBCASE("find_first_of_with_numbers") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_first_of("0123456789") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("123") == 5); // '1' at position 5
    REQUIRE(testString.find_first_of("456") == CStringView::npos);

    static_assert(testString.find_first_of("0123456789") == 5);
    static_assert(testString.find_first_of("123") == 5);
    static_assert(testString.find_first_of("456") == CStringView::npos);
  }

  SUBCASE("find_first_of_with_whitespace") {
    constexpr CStringView testString("Hello World\t\n");

    REQUIRE(testString.find_first_of(" \t\n") == 5); // ' ' at position 5
    REQUIRE(testString.find_first_of("\t") == 11);
    REQUIRE(testString.find_first_of("\n") == 12);

    static_assert(testString.find_first_of(" \t\n") == 5);
    static_assert(testString.find_first_of("\t") == 11);
    static_assert(testString.find_first_of("\n") == 12);
  }

  SUBCASE("find_first_of_with_single_character_string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.find_first_of("A") == 0);
    REQUIRE(testString.find_first_of('A') == 0);
    REQUIRE(testString.find_first_of("B") == CStringView::npos);
    REQUIRE(testString.find_first_of('B') == CStringView::npos);

    static_assert(testString.find_first_of("A") == 0);
    static_assert(testString.find_first_of('A') == 0);
    static_assert(testString.find_first_of("B") == CStringView::npos);
    static_assert(testString.find_first_of('B') == CStringView::npos);
  }

  SUBCASE("find_first_of_with_position_0") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of("aeiou", 0) == 1);
    REQUIRE(testString.find_first_of("H", 0) == 0);
    REQUIRE(testString.find_first_of("xyz", 0) == CStringView::npos);

    static_assert(testString.find_first_of("aeiou", 0) == 1);
    static_assert(testString.find_first_of("H", 0) == 0);
    static_assert(testString.find_first_of("xyz", 0) == CStringView::npos);
  }

  SUBCASE("find_first_of_with_all_characters_found") {
    constexpr CStringView testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_of("aeiou") == 0);
    REQUIRE(testString.find_first_of("xyz") == 23);
    REQUIRE(testString.find_first_of("z") == 25);

    static_assert(testString.find_first_of("aeiou") == 0);
    static_assert(testString.find_first_of("xyz") == 23);
    static_assert(testString.find_first_of("z") == 25);
  }

  SUBCASE("find_first_of_with_no_characters_found") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_of("0123456789") == CStringView::npos);
    REQUIRE(testString.find_first_of("!@#$%^&*()") == CStringView::npos);
    REQUIRE(testString.find_first_of("[]{}|\\:;\"'<>?/") == CStringView::npos);

    static_assert(testString.find_first_of("0123456789") == CStringView::npos);
    static_assert(testString.find_first_of("!@#$%^&*()") == CStringView::npos);
    static_assert(testString.find_first_of("[]{}|\\:;\"'<>?/") == CStringView::npos);
  }
}

// find_first_not_of() character set.
TEST_CASE("core/c_string_view/find_first_not_of") {
  SUBCASE("find_first_not_of_c_string_view_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of(CStringView("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(CStringView("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(CStringView("Helo Wrd")) == CStringView::npos);

    static_assert(testString.find_first_not_of(CStringView("H")) == 1);
    static_assert(testString.find_first_not_of(CStringView("Hel")) == 4);
    static_assert(testString.find_first_not_of(CStringView("Helo Wrd")) == CStringView::npos);
  }

  SUBCASE("find_first_not_of_string_like_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of(std::string("H")) == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of(std::string("Hel")) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of(std::string("Helo Wrd")) == CStringView::npos);

    static_assert(testString.find_first_not_of(CStringView("H")) == 1);
    static_assert(testString.find_first_not_of(CStringView("Hel")) == 4);
    static_assert(testString.find_first_not_of(CStringView("Helo Wrd")) == CStringView::npos);
  }

  SUBCASE("find_first_not_of_c_string_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of("H") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Helo Wrd") == CStringView::npos);

    static_assert(testString.find_first_not_of("H") == 1);
    static_assert(testString.find_first_not_of("Hel") == 4);
    static_assert(testString.find_first_not_of("Helo Wrd") == CStringView::npos);
  }

  SUBCASE("find_first_not_of_single_character") {
    constexpr CStringView testString("aaaaab");

    REQUIRE(testString.find_first_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_first_not_of('b') == 0); // 'a' at position 0
    REQUIRE(testString.find_first_not_of('x') == 0); // 'a' at position 0

    static_assert(testString.find_first_not_of('a') == 5);
    static_assert(testString.find_first_not_of('b') == 0);
    static_assert(testString.find_first_not_of('x') == 0);
  }

  SUBCASE("find_first_not_of_with_position_parameter") {
    constexpr CStringView testString("Hello World Hello");

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

  SUBCASE("find_first_not_of_empty_character_set") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of(CStringView("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of(std::string("")) == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("", 5) == 5); // ' ' at position 5

    static_assert(testString.find_first_not_of(CStringView("")) == 0);
    static_assert(testString.find_first_not_of(FixedString<4>("")) == 0);
    static_assert(testString.find_first_not_of("") == 0);
    static_assert(testString.find_first_not_of("", 5) == 5);
  }

  SUBCASE("find_first_not_of_in_empty_string") {
    constexpr CStringView testString("");

    REQUIRE(testString.find_first_not_of(CStringView("aeiou")) == CStringView::npos);
    REQUIRE(testString.find_first_not_of(std::string("aeiou")) == CStringView::npos);
    REQUIRE(testString.find_first_not_of("aeiou") == CStringView::npos);
    REQUIRE(testString.find_first_not_of('a') == CStringView::npos);

    static_assert(testString.find_first_not_of(CStringView("aeiou")) == CStringView::npos);
    static_assert(testString.find_first_not_of(FixedString<8>("aeiou")) == CStringView::npos);
    static_assert(testString.find_first_not_of("aeiou") == CStringView::npos);
    static_assert(testString.find_first_not_of('a') == CStringView::npos);
  }

  SUBCASE("find_first_not_of_with_position_beyond_string_size") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_first_not_of("aeiou", 10) == CStringView::npos);
    REQUIRE(testString.find_first_not_of('a', 10) == CStringView::npos);

    static_assert(testString.find_first_not_of("aeiou", 10) == CStringView::npos);
    static_assert(testString.find_first_not_of('a', 10) == CStringView::npos);
  }

  SUBCASE("find_first_not_of_with_repeated_characters") {
    constexpr CStringView testString("aaaaa");

    REQUIRE(testString.find_first_not_of("a") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("ab") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("b") == 0); // 'a' at position 0

    static_assert(testString.find_first_not_of("a") == CStringView::npos);
    static_assert(testString.find_first_not_of("ab") == CStringView::npos);
    static_assert(testString.find_first_not_of("b") == 0);
  }

  SUBCASE("find_first_not_of_with_multiple_character_sets") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of("Hl") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Helo") == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Helo ") == 6); // 'W' at position 6

    static_assert(testString.find_first_not_of("Hl") == 1);
    static_assert(testString.find_first_not_of("Hel") == 4);
    static_assert(testString.find_first_not_of("Helo") == 5);
    static_assert(testString.find_first_not_of("Helo ") == 6);
  }

  SUBCASE("find_first_not_of_case_sensitivity") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of("h") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("H") == 1); // 'e' at position 1
    REQUIRE(testString.find_first_not_of("w") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("W") == 0); // 'H' at position 0

    static_assert(testString.find_first_not_of("h") == 0);
    static_assert(testString.find_first_not_of("H") == 1);
    static_assert(testString.find_first_not_of("w") == 0);
    static_assert(testString.find_first_not_of("W") == 0);
  }

  SUBCASE("find_first_not_of_with_special_characters") {
    constexpr CStringView testString("Hello, World!");

    REQUIRE(testString.find_first_not_of("Helo, Wrd!") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Helo, Wrd") == 12); // '!' at position 12

    static_assert(testString.find_first_not_of("Helo, Wrd!") == CStringView::npos);
    static_assert(testString.find_first_not_of("Helo, Wrd") == 12);
  }

  SUBCASE("find_first_not_of_with_numbers") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo123Wrd") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12

    static_assert(testString.find_first_not_of("0123456789") == 0);
    static_assert(testString.find_first_not_of("Helo123Wrd") == CStringView::npos);
    static_assert(testString.find_first_not_of("Helo123Wr") == 12);
  }

  SUBCASE("find_first_not_of_with_whitespace") {
    constexpr CStringView testString("Hello World\t\n");

    REQUIRE(testString.find_first_not_of(" \t\n") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("Helo Wrd\t\n") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Helo Wrd") == 11); // '\t' at position 11

    static_assert(testString.find_first_not_of(" \t\n") == 0);
    static_assert(testString.find_first_not_of("Helo Wrd\t\n") == CStringView::npos);
    static_assert(testString.find_first_not_of("Helo Wrd") == 11);
  }

  SUBCASE("find_first_not_of_with_single_character_string") {
    constexpr CStringView testString("A");

    REQUIRE(testString.find_first_not_of("A") == CStringView::npos);
    REQUIRE(testString.find_first_not_of('A') == CStringView::npos);
    REQUIRE(testString.find_first_not_of("B") == 0); // 'A' at position 0
    REQUIRE(testString.find_first_not_of('B') == 0); // 'A' at position 0

    static_assert(testString.find_first_not_of("A") == CStringView::npos);
    static_assert(testString.find_first_not_of('A') == CStringView::npos);
    static_assert(testString.find_first_not_of("B") == 0);
    static_assert(testString.find_first_not_of('B') == 0);
  }

  SUBCASE("find_first_not_of_with_position_0") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of("H", 0) == 1);
    REQUIRE(testString.find_first_not_of("Hel", 0) == 4);
    REQUIRE(testString.find_first_not_of("Helo Wrd", 0) == CStringView::npos);

    static_assert(testString.find_first_not_of("H", 0) == 1);
    static_assert(testString.find_first_not_of("Hel", 0) == 4);
    static_assert(testString.find_first_not_of("Helo Wrd", 0) == CStringView::npos);
  }

  SUBCASE("find_first_not_of_with_all_characters_excluded") {
    constexpr CStringView testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24); // 'y' at position 24

    static_assert(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == CStringView::npos);
    static_assert(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25);
    static_assert(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24);
  }

  SUBCASE("find_first_not_of_with_no_characters_excluded") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_first_not_of("xyz") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    REQUIRE(testString.find_first_not_of("!@#$%^&*()") == 0); // 'H' at position 0

    static_assert(testString.find_first_not_of("xyz") == 0);
    static_assert(testString.find_first_not_of("0123456789") == 0);
    static_assert(testString.find_first_not_of("!@#$%^&*()") == 0);
  }

  SUBCASE("find_first_not_of_with_mixed_content") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_first_not_of("Helo123Wrd") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12
    REQUIRE(testString.find_first_not_of("Helo123Wd") == 10); // 'r' at position 10

    static_assert(testString.find_first_not_of("Helo123Wrd") == CStringView::npos);
    static_assert(testString.find_first_not_of("Helo123Wr") == 12);
    static_assert(testString.find_first_not_of("Helo123Wd") == 10);
  }

  SUBCASE("find_first_not_of_with_position_in_middle") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_first_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel", 5) == 5); // ' ' at position 5
    REQUIRE(testString.find_first_not_of("Hel", 6) == 6); // 'W' at position 6
    REQUIRE(testString.find_first_not_of("Hel", 7) == 7); // 'o' at position 7

    static_assert(testString.find_first_not_of("Hel", 4) == 4);
    static_assert(testString.find_first_not_of("Hel", 5) == 5);
    static_assert(testString.find_first_not_of("Hel", 6) == 6);
    static_assert(testString.find_first_not_of("Hel", 7) == 7);
  }

  SUBCASE("find_first_not_of_with_exact_match") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_first_not_of("Hello") == CStringView::npos);
    REQUIRE(testString.find_first_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_first_not_of("Hel") == 4); // 'o' at position 4

    static_assert(testString.find_first_not_of("Hello") == CStringView::npos);
    static_assert(testString.find_first_not_of("Hell") == 4);
    static_assert(testString.find_first_not_of("Hel") == 4);
  }
}

// find_last_of() character set.
TEST_CASE("core/c_string_view/find_last_of") {
  SUBCASE("find_last_of_c_string_view_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of(CStringView("aeiou")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of(CStringView("l")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of(CStringView("H")) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of(CStringView("d")) == 10); // 'd' at position 10

    static_assert(testString.find_last_of(CStringView("aeiou")) == 7);
    static_assert(testString.find_last_of(CStringView("l")) == 9);
    static_assert(testString.find_last_of(CStringView("H")) == 0);
    static_assert(testString.find_last_of(CStringView("d")) == 10);
  }

  SUBCASE("find_last_of_string_like_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of(std::string("aeiou")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of(std::string("l")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of(std::string("H")) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of(std::string("d")) == 10); // 'd' at position 10

    static_assert(testString.find_last_of(CStringView("aeiou")) == 7);
    static_assert(testString.find_last_of(CStringView("l")) == 9);
    static_assert(testString.find_last_of(CStringView("H")) == 0);
    static_assert(testString.find_last_of(CStringView("d")) == 10);
  }

  SUBCASE("find_last_of_c_string_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of("aeiou") == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of("l") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("H") == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("d") == 10); // 'd' at position 10

    static_assert(testString.find_last_of("aeiou") == 7);
    static_assert(testString.find_last_of("l") == 9);
    static_assert(testString.find_last_of("H") == 0);
    static_assert(testString.find_last_of("d") == 10);
  }

  SUBCASE("find_last_of_single_character") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of('l') == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of('o') == 7); // 'o' at position 7
    REQUIRE(testString.find_last_of('H') == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of('d') == 10); // 'd' at position 10

    static_assert(testString.find_last_of('l') == 9);
    static_assert(testString.find_last_of('o') == 7);
    static_assert(testString.find_last_of('H') == 0);
    static_assert(testString.find_last_of('d') == 10);
  }

  SUBCASE("find_last_of_with_position_parameter") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    REQUIRE(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1
    REQUIRE(testString.find_last_of("Hel", 0) == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("Hel", 17) == CStringView::npos);

    static_assert(testString.find_last_of("Hel", 8) == 3);
    static_assert(testString.find_last_of("Hel", 4) == 3);
    static_assert(testString.find_last_of("Hel", 2) == 2);
    static_assert(testString.find_last_of("Hel", 1) == 1);
    static_assert(testString.find_last_of("Hel", 0) == 0);
    static_assert(testString.find_last_of("Hel", 17) == CStringView::npos);
  }

  SUBCASE("find_last_of_empty_character_set") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of(CStringView("")) == CStringView::npos);
    REQUIRE(testString.find_last_of(std::string("")) == CStringView::npos);
    REQUIRE(testString.find_last_of("") == CStringView::npos);

    static_assert(testString.find_last_of(CStringView("")) == CStringView::npos);
    static_assert(testString.find_last_of(FixedString<4>("")) == CStringView::npos);
    static_assert(testString.find_last_of("") == CStringView::npos);
  }

  SUBCASE("find_last_of_with_no_characters_found") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of("0123456789") == CStringView::npos);
    REQUIRE(testString.find_last_of("!@#$%^&*()") == CStringView::npos);
    REQUIRE(testString.find_last_of("[]{}|\\:;\"'<>?/") == CStringView::npos);

    static_assert(testString.find_last_of("0123456789") == CStringView::npos);
    static_assert(testString.find_last_of("!@#$%^&*()") == CStringView::npos);
    static_assert(testString.find_last_of("[]{}|\\:;\"'<>?/") == CStringView::npos);
  }

  SUBCASE("find_last_of_with_multiple_character_sets") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of("Hl") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Hel") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo ") == 9); // 'l' at position 9

    static_assert(testString.find_last_of("Hl") == 9);
    static_assert(testString.find_last_of("Hel") == 9);
    static_assert(testString.find_last_of("Helo") == 9);
    static_assert(testString.find_last_of("Helo ") == 9);
  }

  SUBCASE("find_last_of_case_sensitivity") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of("h") == CStringView::npos);
    REQUIRE(testString.find_last_of("H") == 0); // 'H' at position 0
    REQUIRE(testString.find_last_of("w") == CStringView::npos);
    REQUIRE(testString.find_last_of("W") == 6); // 'W' at position 6

    static_assert(testString.find_last_of("h") == CStringView::npos);
    static_assert(testString.find_last_of("H") == 0);
    static_assert(testString.find_last_of("w") == CStringView::npos);
    static_assert(testString.find_last_of("W") == 6);
  }

  SUBCASE("find_last_of_with_special_characters") {
    constexpr CStringView testString("Hello, World!");

    REQUIRE(testString.find_last_of("Helo, Wrd!") == 12); // '!' at position 12
    REQUIRE(testString.find_last_of("Helo, Wrd") == 11); // 'd' at position 11
    REQUIRE(testString.find_last_of("Helo, Wr") == 10); // 'l' at position 10

    static_assert(testString.find_last_of("Helo, Wrd!") == 12);
    static_assert(testString.find_last_of("Helo, Wrd") == 11);
    static_assert(testString.find_last_of("Helo, Wr") == 10);
  }

  SUBCASE("find_last_of_with_numbers") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_last_of("0123456789") == 7); // '3' at position 7
    REQUIRE(testString.find_last_of("Helo123Wrd") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_of("Helo123Wr") == 11); // 'l' at position 11

    static_assert(testString.find_last_of("0123456789") == 7);
    static_assert(testString.find_last_of("Helo123Wrd") == 12);
    static_assert(testString.find_last_of("Helo123Wr") == 11);
  }

  SUBCASE("find_last_of_with_whitespace") {
    constexpr CStringView testString("Hello World\t\n");

    REQUIRE(testString.find_last_of(" \t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd\t\n") == 12); // '\n' at position 12
    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10

    static_assert(testString.find_last_of(" \t\n") == 12);
    static_assert(testString.find_last_of("Helo Wrd\t\n") == 12);
    static_assert(testString.find_last_of("Helo Wrd") == 10);
  }

  SUBCASE("find_last_of_with_repeated_characters") {
    constexpr CStringView testString("aaaaab");

    REQUIRE(testString.find_last_of('a') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_of('b') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_of('x') == CStringView::npos);

    static_assert(testString.find_last_of('a') == 4);
    static_assert(testString.find_last_of('b') == 5);
    static_assert(testString.find_last_of('x') == CStringView::npos);
  }

  SUBCASE("find_last_of_with_single_character_string") {
    constexpr CStringView testString("AAAAA");

    REQUIRE(testString.find_last_of("A") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of('A') == 4); // 'A' at position 4
    REQUIRE(testString.find_last_of("B") == CStringView::npos);
    REQUIRE(testString.find_last_of('B') == CStringView::npos);

    static_assert(testString.find_last_of("A") == 4);
    static_assert(testString.find_last_of('A') == 4);
    static_assert(testString.find_last_of("B") == CStringView::npos);
    static_assert(testString.find_last_of('B') == CStringView::npos);
  }

  SUBCASE("find_last_of_with_alphabet") {
    constexpr CStringView testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24); // 'y' at position 24
    REQUIRE(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23); // 'x' at position 23

    static_assert(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25);
    static_assert(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24);
    static_assert(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23);
  }

  SUBCASE("find_last_of_with_all_characters_found") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_of("Helo Wr") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_of("Helo W") == 9); // 'l' at position 9

    static_assert(testString.find_last_of("Helo Wrd") == 10);
    static_assert(testString.find_last_of("Helo Wr") == 9);
    static_assert(testString.find_last_of("Helo W") == 9);
  }

  SUBCASE("find_last_of_with_position_in_middle") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    REQUIRE(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1

    static_assert(testString.find_last_of("Hel", 8) == 3);
    static_assert(testString.find_last_of("Hel", 4) == 3);
    static_assert(testString.find_last_of("Hel", 2) == 2);
    static_assert(testString.find_last_of("Hel", 1) == 1);
  }

  SUBCASE("find_last_of_with_exact_match") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_last_of("Hello") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_of("Hell") == 3); // 'l' at position 3
    REQUIRE(testString.find_last_of("Hel") == 3); // 'l' at position 3

    static_assert(testString.find_last_of("Hello") == 4);
    static_assert(testString.find_last_of("Hell") == 3);
    static_assert(testString.find_last_of("Hel") == 3);
  }
}

// find_last_not_of() character set.
TEST_CASE("core/c_string_view/find_last_not_of") {
  SUBCASE("find_last_not_of_c_string_view_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of(CStringView("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(CStringView("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(CStringView("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(CStringView("World")) == 5); // ' ' at position 5

    static_assert(testString.find_last_not_of(CStringView("d")) == 9);
    static_assert(testString.find_last_not_of(CStringView("ld")) == 8);
    static_assert(testString.find_last_not_of(CStringView("rld")) == 7);
    static_assert(testString.find_last_not_of(CStringView("World")) == 5);
  }

  SUBCASE("find_last_not_of_string_like_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of(std::string("d")) == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of(std::string("ld")) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of(std::string("rld")) == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of(std::string("World")) == 5); // ' ' at position 5

    static_assert(testString.find_last_not_of(CStringView("d")) == 9);
    static_assert(testString.find_last_not_of(CStringView("ld")) == 8);
    static_assert(testString.find_last_not_of(CStringView("rld")) == 7);
    static_assert(testString.find_last_not_of(CStringView("World")) == 5);
  }

  SUBCASE("find_last_not_of_c_string_characters") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("d") == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of("ld") == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("rld") == 7); // 'o' at position 7
    REQUIRE(testString.find_last_not_of("World") == 5); // ' ' at position 5

    static_assert(testString.find_last_not_of("d") == 9);
    static_assert(testString.find_last_not_of("ld") == 8);
    static_assert(testString.find_last_not_of("rld") == 7);
    static_assert(testString.find_last_not_of("World") == 5);
  }

  SUBCASE("find_last_not_of_single_character") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of('d') == 9); // 'l' at position 9
    REQUIRE(testString.find_last_not_of('l') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('o') == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of('H') == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of('d') == 9);
    static_assert(testString.find_last_not_of('l') == 10);
    static_assert(testString.find_last_not_of('o') == 10);
    static_assert(testString.find_last_not_of('H') == 10);
  }

  SUBCASE("find_last_not_of_with_position_parameter") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hel", 0) == CStringView::npos);

    static_assert(testString.find_last_not_of("Hel", 8) == 8);
    static_assert(testString.find_last_not_of("Hel", 4) == 4);
    static_assert(testString.find_last_not_of("Hel", 2) == CStringView::npos);
    static_assert(testString.find_last_not_of("Hel", 1) == CStringView::npos);
    static_assert(testString.find_last_not_of("Hel", 0) == CStringView::npos);
  }

  SUBCASE("find_last_not_of_empty_character_set") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of(CStringView("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of(std::string("")) == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("", 5) == 5); // ' ' at position 5

    static_assert(testString.find_last_not_of(CStringView("")) == 10);
    static_assert(testString.find_last_not_of(FixedString<10>("")) == 10);
    static_assert(testString.find_last_not_of("") == 10);
    static_assert(testString.find_last_not_of("", 5) == 5);
  }

  SUBCASE("find_last_not_of_with_all_characters_excluded") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("Helo Wrd") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Helo Wr") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo W") == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of("Helo Wrd") == CStringView::npos);
    static_assert(testString.find_last_not_of("Helo Wr") == 10);
    static_assert(testString.find_last_not_of("Helo W") == 10);
  }

  SUBCASE("find_last_not_of_with_multiple_character_sets") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("Hl") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Hel") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo ") == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of("Hl") == 10);
    static_assert(testString.find_last_not_of("Hel") == 10);
    static_assert(testString.find_last_not_of("Helo") == 10);
    static_assert(testString.find_last_not_of("Helo ") == 10);
  }

  SUBCASE("find_last_not_of_case_sensitivity") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("h") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("H") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("w") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("W") == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of("h") == 10);
    static_assert(testString.find_last_not_of("H") == 10);
    static_assert(testString.find_last_not_of("w") == 10);
    static_assert(testString.find_last_not_of("W") == 10);
  }

  SUBCASE("find_last_not_of_with_special_characters") {
    constexpr CStringView testString("Hello, World!");

    REQUIRE(testString.find_last_not_of("Helo, Wrd!") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Helo, Wrd") == 12); // '!' at position 12
    REQUIRE(testString.find_last_not_of("Helo, Wr") == 12); // '!' at position 12

    static_assert(testString.find_last_not_of("Helo, Wrd!") == CStringView::npos);
    static_assert(testString.find_last_not_of("Helo, Wrd") == 12);
    static_assert(testString.find_last_not_of("Helo, Wr") == 12);
  }

  SUBCASE("find_last_not_of_with_numbers") {
    constexpr CStringView testString("Hello123World");

    REQUIRE(testString.find_last_not_of("0123456789") == 12); // 'd' at position 12
    REQUIRE(testString.find_last_not_of("Helo123Wrd") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Helo123Wr") == 12); // 'd' at position 12

    static_assert(testString.find_last_not_of("0123456789") == 12);
    static_assert(testString.find_last_not_of("Helo123Wrd") == CStringView::npos);
    static_assert(testString.find_last_not_of("Helo123Wr") == 12);
  }

  SUBCASE("find_last_not_of_with_whitespace") {
    constexpr CStringView testString("Hello World\t\n");

    REQUIRE(testString.find_last_not_of(" \t\n") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("Helo Wrd\t\n") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Helo Wrd") == 12); // '\n' at position 12

    static_assert(testString.find_last_not_of(" \t\n") == 10);
    static_assert(testString.find_last_not_of("Helo Wrd\t\n") == CStringView::npos);
    static_assert(testString.find_last_not_of("Helo Wrd") == 12);
  }

  SUBCASE("find_last_not_of_with_repeated_characters") {
    constexpr CStringView testString("aaaaab");

    REQUIRE(testString.find_last_not_of('a') == 5); // 'b' at position 5
    REQUIRE(testString.find_last_not_of('b') == 4); // 'a' at position 4
    REQUIRE(testString.find_last_not_of('x') == 5); // 'b' at position 5

    static_assert(testString.find_last_not_of('a') == 5);
    static_assert(testString.find_last_not_of('b') == 4);
    static_assert(testString.find_last_not_of('x') == 5);
  }

  SUBCASE("find_last_not_of_with_single_character_string") {
    constexpr CStringView testString("AAAAA");

    REQUIRE(testString.find_last_not_of("A") == CStringView::npos);
    REQUIRE(testString.find_last_not_of('A') == CStringView::npos);
    REQUIRE(testString.find_last_not_of("B") == 4); // 'A' at position 4
    REQUIRE(testString.find_last_not_of('B') == 4); // 'A' at position 4

    static_assert(testString.find_last_not_of("A") == CStringView::npos);
    static_assert(testString.find_last_not_of('A') == CStringView::npos);
    static_assert(testString.find_last_not_of("B") == 4);
    static_assert(testString.find_last_not_of('B') == 4);
  }

  SUBCASE("find_last_not_of_with_alphabet") {
    constexpr CStringView testString("abcdefghijklmnopqrstuvwxyz");

    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxyz") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    REQUIRE(testString.find_last_not_of("abcdefghijklmnopqrstuvwx") == 25); // 'z' at position 25

    static_assert(testString.find_last_not_of("abcdefghijklmnopqrstuvwxyz") == CStringView::npos);
    static_assert(testString.find_last_not_of("abcdefghijklmnopqrstuvwxy") == 25);
    static_assert(testString.find_last_not_of("abcdefghijklmnopqrstuvwx") == 25);
  }

  SUBCASE("find_last_not_of_with_no_characters_excluded") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.find_last_not_of("xyz") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("0123456789") == 10); // 'd' at position 10
    REQUIRE(testString.find_last_not_of("!@#$%^&*()") == 10); // 'd' at position 10

    static_assert(testString.find_last_not_of("xyz") == 10);
    static_assert(testString.find_last_not_of("0123456789") == 10);
    static_assert(testString.find_last_not_of("!@#$%^&*()") == 10);
  }

  SUBCASE("find_last_not_of_with_position_in_middle") {
    constexpr CStringView testString("Hello World Hello");

    REQUIRE(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    REQUIRE(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel", 2) == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hel", 1) == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hel", 17) == CStringView::npos);

    static_assert(testString.find_last_not_of("Hel", 8) == 8);
    static_assert(testString.find_last_not_of("Hel", 4) == 4);
    static_assert(testString.find_last_not_of("Hel", 2) == CStringView::npos);
    static_assert(testString.find_last_not_of("Hel", 1) == CStringView::npos);
    static_assert(testString.find_last_not_of("Hel", 17) == CStringView::npos);
  }

  SUBCASE("find_last_not_of_with_exact_match") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.find_last_not_of("Hello") == CStringView::npos);
    REQUIRE(testString.find_last_not_of("Hell") == 4); // 'o' at position 4
    REQUIRE(testString.find_last_not_of("Hel") == 4); // 'o' at position 4

    static_assert(testString.find_last_not_of("Hello") == CStringView::npos);
    static_assert(testString.find_last_not_of("Hell") == 4);
    static_assert(testString.find_last_not_of("Hel") == 4);
  }

  SUBCASE("find_last_not_of_in_empty_string") {
    constexpr CStringView testString("");

    REQUIRE(testString.find_last_not_of("Hello") == CStringView::npos);

    static_assert(testString.find_last_not_of("Hello") == CStringView::npos);
  }
}

// compare() three-way.
TEST_CASE("core/c_string_view/compare") {
  SUBCASE("compare_c_string_view_with_c_string_view") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hello");
    constexpr CStringView testString3("World");
    constexpr CStringView testString4("Hell");

    REQUIRE(testString1.compare(testString2) == 0);
    REQUIRE(testString1.compare(testString3) < 0);
    REQUIRE(testString1.compare(testString4) > 0);

    static_assert(testString1.compare(testString2) == 0);
    static_assert(testString1.compare(testString3) < 0);
    static_assert(testString1.compare(testString4) > 0);
  }

  SUBCASE("compare_c_string_view_with_string_like") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.compare(std::string("Hello")) == 0);
    REQUIRE(testString.compare(std::string("World")) < 0);
    REQUIRE(testString.compare(std::string("Hell")) > 0);

    static_assert(testString.compare(CStringView("Hello")) == 0);
    static_assert(testString.compare(CStringView("World")) < 0);
    static_assert(testString.compare(CStringView("Hell")) > 0);
  }

  SUBCASE("compare_c_string_view_with_c_string") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.compare("Hello") == 0);
    REQUIRE(testString.compare("World") < 0);
    REQUIRE(testString.compare("Hell") > 0);

    static_assert(testString.compare("Hello") == 0);
    static_assert(testString.compare("World") < 0);
    static_assert(testString.compare("Hell") > 0);
  }

  SUBCASE("compare_identical_strings") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.compare(CStringView("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare("Hello World") == 0);

    static_assert(testString.compare(CStringView("Hello World")) == 0);
    static_assert(testString.compare(FixedString<16>("Hello World")) == 0);
    static_assert(testString.compare("Hello World") == 0);
  }

  SUBCASE("compare_with_empty_strings") {
    constexpr CStringView testString1("");
    constexpr CStringView testString2("Hello");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("") == 0);
    REQUIRE(testString1.compare(std::string("")) == 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("") == 0);
    static_assert(testString1.compare(CStringView("")) == 0);
  }

  SUBCASE("compare_strings_with_different_lengths") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hello World");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
  }

  SUBCASE("compare_strings_with_same_prefix") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hell");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);

    static_assert(testString1.compare(testString2) > 0);
    static_assert(testString2.compare(testString1) < 0);
  }

  SUBCASE("compare_strings_with_different_first_character") {
    constexpr CStringView testString1("Apple");
    constexpr CStringView testString2("Banana");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
  }

  SUBCASE("compare_strings_with_different_middle_character") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hallo");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);

    static_assert(testString1.compare(testString2) > 0);
    static_assert(testString2.compare(testString1) < 0);
  }

  SUBCASE("compare_strings_with_different_last_character") {
    constexpr CStringView testString1("Hello");
    constexpr CStringView testString2("Hellp");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
  }

  SUBCASE("compare_case_sensitivity") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.compare("hello") < 0);
    REQUIRE(testString.compare("HELLO") > 0);
    REQUIRE(testString.compare("Hello") == 0);

    static_assert(testString.compare("hello") < 0);
    static_assert(testString.compare("HELLO") > 0);
    static_assert(testString.compare("Hello") == 0);
  }

  SUBCASE("compare_with_single_character_strings") {
    constexpr CStringView testString1("A");
    constexpr CStringView testString2("B");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("A") == 0);
    REQUIRE(testString1.compare("B") < 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("A") == 0);
    static_assert(testString1.compare("B") < 0);
  }

  SUBCASE("compare_with_repeated_characters") {
    constexpr CStringView testString1("aaa");
    constexpr CStringView testString2("aa");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("aaa") == 0);
    REQUIRE(testString1.compare("aa") > 0);

    static_assert(testString1.compare(testString2) > 0);
    static_assert(testString2.compare(testString1) < 0);
    static_assert(testString1.compare("aaa") == 0);
    static_assert(testString1.compare("aa") > 0);
  }

  SUBCASE("compare_with_special_characters") {
    constexpr CStringView testString1("Hello!");
    constexpr CStringView testString2("Hello");

    REQUIRE(testString1.compare(testString2) > 0);
    REQUIRE(testString2.compare(testString1) < 0);
    REQUIRE(testString1.compare("Hello!") == 0);
    REQUIRE(testString1.compare("Hello") > 0);

    static_assert(testString1.compare(testString2) > 0);
    static_assert(testString2.compare(testString1) < 0);
    static_assert(testString1.compare("Hello!") == 0);
    static_assert(testString1.compare("Hello") > 0);
  }

  SUBCASE("compare_with_numbers") {
    constexpr CStringView testString1("123");
    constexpr CStringView testString2("456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123") == 0);
    REQUIRE(testString1.compare("456") < 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("123") == 0);
    static_assert(testString1.compare("456") < 0);
  }

  SUBCASE("compare_with_mixed_content") {
    constexpr CStringView testString1("Hello123");
    constexpr CStringView testString2("Hello456");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("Hello123") == 0);
    REQUIRE(testString1.compare("Hello456") < 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("Hello123") == 0);
    static_assert(testString1.compare("Hello456") < 0);
  }

  SUBCASE("compare_with_maximum_length_strings") {
    constexpr CStringView testString1("123456789012345");
    constexpr CStringView testString2("123456789012346");

    REQUIRE(testString1.compare(testString2) < 0);
    REQUIRE(testString2.compare(testString1) > 0);
    REQUIRE(testString1.compare("123456789012345") == 0);
    REQUIRE(testString1.compare("123456789012346") < 0);

    static_assert(testString1.compare(testString2) < 0);
    static_assert(testString2.compare(testString1) > 0);
    static_assert(testString1.compare("123456789012345") == 0);
    static_assert(testString1.compare("123456789012346") < 0);
  }

  SUBCASE("compare_with_string_like") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.compare(std::string("Hello World")) == 0);
    REQUIRE(testString.compare(std::string("Hello")) > 0);
    REQUIRE(testString.compare(std::string("World")) < 0);

    static_assert(testString.compare(FixedString<16>("Hello World")) == 0);
    static_assert(testString.compare(FixedString<16>("Hello")) > 0);
    static_assert(testString.compare(FixedString<16>("World")) < 0);
  }

  SUBCASE("compare_with_array") {
    constexpr CStringView testString("Hello");
    constexpr array<char, 6> arr = {'H', 'e', 'l', 'l', 'o', '\0'};

    REQUIRE(testString.compare(arr.data()) == 0);
    REQUIRE(testString.compare("Hello") == 0);

    static_assert(testString.compare(arr.data()) == 0);
    static_assert(testString.compare("Hello") == 0);
  }

  SUBCASE("compare_edge_cases") {
    constexpr CStringView testString("Hello");

    // Compare with null-terminated string
    REQUIRE(testString.compare("Hello\0World") == 0);

    // Compare with string containing null character
    constexpr CStringView testStringWithNull("Hello\0World");
    REQUIRE(testString.compare(testStringWithNull) == 0);

    static_assert(testString.compare("Hello\0World") == 0);
    static_assert(testString.compare(testStringWithNull) == 0);
  }
}

// starts_with() prefix check.
TEST_CASE("core/c_string_view/starts_with") {
  SUBCASE("starts_with_c_string_view") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with(CStringView("Hello")));
    REQUIRE(testString.starts_with(CStringView("Hello World")));
    REQUIRE(testString.starts_with(CStringView("H")));
    REQUIRE(testString.starts_with(CStringView("World")) == false);
    REQUIRE(testString.starts_with(CStringView("xyz")) == false);
    REQUIRE(testString.starts_with(CStringView("")));

    static_assert(testString.starts_with(CStringView("Hello")));
    static_assert(testString.starts_with(CStringView("Hello World")));
    static_assert(testString.starts_with(CStringView("H")));
    static_assert(testString.starts_with(CStringView("World")) == false);
    static_assert(testString.starts_with(CStringView("xyz")) == false);
    static_assert(testString.starts_with(CStringView("")));
  }

  SUBCASE("starts_with_string_like") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")));
    REQUIRE(testString.starts_with(std::string("Hello World")));
    REQUIRE(testString.starts_with(std::string("H")));
    REQUIRE(testString.starts_with(std::string("World")) == false);
    REQUIRE(testString.starts_with(std::string("xyz")) == false);
    REQUIRE(testString.starts_with(std::string("")));

    static_assert(testString.starts_with(FixedString<16>("Hello")));
    static_assert(testString.starts_with(FixedString<16>("Hello World")));
    static_assert(testString.starts_with(FixedString<16>("H")));
    static_assert(testString.starts_with(FixedString<16>("World")) == false);
    static_assert(testString.starts_with(FixedString<16>("xyz")) == false);
    static_assert(testString.starts_with(FixedString<16>("")));
  }

  SUBCASE("starts_with_c_string") {
    constexpr CStringView testString("Hello World");

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

  SUBCASE("starts_with_character") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with('H'));
    REQUIRE(testString.starts_with('h') == false);
    REQUIRE(testString.starts_with('W') == false);
    REQUIRE(testString.starts_with('x') == false);

    static_assert(testString.starts_with('H'));
    static_assert(testString.starts_with('h') == false);
    static_assert(testString.starts_with('W') == false);
    static_assert(testString.starts_with('x') == false);
  }

  SUBCASE("starts_with_empty_string") {
    constexpr CStringView testString("");

    REQUIRE(testString.starts_with(CStringView("Hello")) == false);
    REQUIRE(testString.starts_with(std::string("Hello")) == false);
    REQUIRE(testString.starts_with("Hello") == false);
    REQUIRE(testString.starts_with('H') == false);
    REQUIRE(testString.starts_with(""));

    static_assert(testString.starts_with(CStringView("Hello")) == false);
    static_assert(testString.starts_with(FixedString<16>("Hello")) == false);
    static_assert(testString.starts_with("Hello") == false);
    static_assert(testString.starts_with('H') == false);
    static_assert(testString.starts_with(""));
  }

  SUBCASE("starts_with_single_character_string") {
    constexpr CStringView testString("A");

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

  SUBCASE("starts_with_longer_prefix") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.starts_with("Hello World") == false);
    REQUIRE(testString.starts_with("Hello Universe") == false);
    REQUIRE(testString.starts_with("Hello"));
    REQUIRE(testString.starts_with("Hell"));

    static_assert(testString.starts_with("Hello World") == false);
    static_assert(testString.starts_with("Hello Universe") == false);
    static_assert(testString.starts_with("Hello"));
    static_assert(testString.starts_with("Hell"));
  }

  SUBCASE("starts_with_case_sensitivity") {
    constexpr CStringView testString("Hello World");

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

  SUBCASE("starts_with_repeated_characters") {
    constexpr CStringView testString("aaaab");

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

  SUBCASE("starts_with_special_characters") {
    constexpr CStringView testString("!@#$%");

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

  SUBCASE("starts_with_numbers") {
    constexpr CStringView testString("12345");

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

  SUBCASE("starts_with_mixed_content") {
    constexpr CStringView testString("Hello123");

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

  SUBCASE("starts_with_maximum_length_strings") {
    constexpr CStringView testString("123456789012345");

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

  SUBCASE("starts_with_std_string") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.starts_with(std::string("Hello")));
    REQUIRE(testString.starts_with(std::string("Hello World")));
    REQUIRE(testString.starts_with(std::string("World")) == false);

    static_assert(testString.starts_with(FixedString<16>("Hello")));
    static_assert(testString.starts_with(FixedString<16>("Hello World")));
    static_assert(testString.starts_with(FixedString<16>("World")) == false);
  }

  SUBCASE("starts_with_array") {
    constexpr CStringView testString("Hello");
    constexpr array<char, 4> arr = {'H', 'e', 'l', '\0'};

    REQUIRE(testString.starts_with(arr.data()));
    REQUIRE(testString.starts_with("Hel"));

    static_assert(testString.starts_with(arr.data()));
    static_assert(testString.starts_with("Hel"));
  }

  SUBCASE("starts_with_edge_cases") {
    constexpr CStringView testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.starts_with("Hello\0World"));

    // Test with string containing null character
    constexpr CStringView testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.starts_with("Hello"));

    static_assert(testString.starts_with("Hello\0World"));
    static_assert(testStringWithNull.starts_with("Hello"));
  }

  SUBCASE("starts_with_whitespace") {
    constexpr CStringView testString(" Hello World");

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

  SUBCASE("starts_with_exact_match") {
    constexpr CStringView testString("Hello");

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
TEST_CASE("core/c_string_view/ends_with") {
  SUBCASE("c_string_view_ends_with") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.ends_with(CStringView("World")));
    REQUIRE(testString.ends_with(CStringView("Hello World")));
    REQUIRE(testString.ends_with(CStringView("d")));
    REQUIRE(testString.ends_with(CStringView("Hello")) == false);
    REQUIRE(testString.ends_with(CStringView("xyz")) == false);
    REQUIRE(testString.ends_with(CStringView("")));

    static_assert(testString.ends_with(CStringView("World")));
    static_assert(testString.ends_with(CStringView("Hello World")));
    static_assert(testString.ends_with(CStringView("d")));
    static_assert(testString.ends_with(CStringView("Hello")) == false);
    static_assert(testString.ends_with(CStringView("xyz")) == false);
    static_assert(testString.ends_with(CStringView("")));
  }

  SUBCASE("string_like_ends_with") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.ends_with(std::string("World")));
    REQUIRE(testString.ends_with(std::string("Hello World")));
    REQUIRE(testString.ends_with(std::string("d")));
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("xyz")) == false);
    REQUIRE(testString.ends_with(std::string("")));

    static_assert(testString.ends_with(FixedString<16>("World")));
    static_assert(testString.ends_with(FixedString<16>("Hello World")));
    static_assert(testString.ends_with(FixedString<16>("d")));
    static_assert(testString.ends_with(FixedString<16>("Hello")) == false);
    static_assert(testString.ends_with(FixedString<16>("xyz")) == false);
    static_assert(testString.ends_with(FixedString<16>("")));
  }

  SUBCASE("c_string_ends_with") {
    constexpr CStringView testString("Hello World");

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

  SUBCASE("character_ends_with") {
    constexpr CStringView testString("Hello World");

    REQUIRE(testString.ends_with('d'));
    REQUIRE(testString.ends_with('D') == false);
    REQUIRE(testString.ends_with('H') == false);
    REQUIRE(testString.ends_with('x') == false);

    static_assert(testString.ends_with('d'));
    static_assert(testString.ends_with('D') == false);
    static_assert(testString.ends_with('H') == false);
    static_assert(testString.ends_with('x') == false);
  }

  SUBCASE("empty_string_ends_with") {
    constexpr CStringView testString("");

    REQUIRE(testString.ends_with(CStringView("Hello")) == false);
    REQUIRE(testString.ends_with(std::string("Hello")) == false);
    REQUIRE(testString.ends_with("Hello") == false);
    REQUIRE(testString.ends_with('H') == false);
    REQUIRE(testString.ends_with(""));

    static_assert(testString.ends_with(CStringView("Hello")) == false);
    static_assert(testString.ends_with(FixedString<8>("Hello")) == false);
    static_assert(testString.ends_with("Hello") == false);
    static_assert(testString.ends_with('H') == false);
    static_assert(testString.ends_with(""));
  }

  SUBCASE("single_character_string_ends_with") {
    constexpr CStringView testString("A");

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

  SUBCASE("longer_suffix_ends_with") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.ends_with("Hello World") == false);
    REQUIRE(testString.ends_with("Hello Universe") == false);
    REQUIRE(testString.ends_with("Hello"));
    REQUIRE(testString.ends_with("llo"));

    static_assert(testString.ends_with("Hello World") == false);
    static_assert(testString.ends_with("Hello Universe") == false);
    static_assert(testString.ends_with("Hello"));
    static_assert(testString.ends_with("llo"));
  }

  SUBCASE("case_sensitivity_ends_with") {
    constexpr CStringView testString("Hello World");

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

  SUBCASE("repeated_characters_ends_with") {
    constexpr CStringView testString("baaaa");

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

  SUBCASE("special_characters_ends_with") {
    constexpr CStringView testString("%$#@!");

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

  SUBCASE("numeric_content_ends_with") {
    constexpr CStringView testString("54321");

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

  SUBCASE("mixed_content_ends_with") {
    constexpr CStringView testString("123Hello");

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

  SUBCASE("maximum_length_strings_ends_with") {
    constexpr CStringView testString("123456789012345");

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

  SUBCASE("array_ends_with") {
    constexpr CStringView testString("Hello");
    constexpr array<char, 4> arr = {'l', 'l', 'o', '\0'};

    REQUIRE(testString.ends_with(arr.data()));
    REQUIRE(testString.ends_with("llo"));

    static_assert(testString.ends_with(arr.data()));
    static_assert(testString.ends_with("llo"));
  }

  SUBCASE("edge_cases_ends_with") {
    constexpr CStringView testString("Hello");

    // Test with null-terminated string
    REQUIRE(testString.ends_with("Hello\0World"));

    // Test with string containing null character
    constexpr CStringView testStringWithNull("Hello\0World");
    REQUIRE(testStringWithNull.ends_with("World") == false);

    static_assert(testString.ends_with("Hello\0World"));
    static_assert(testStringWithNull.ends_with("World") == false);
  }

  SUBCASE("whitespace_ends_with") {
    constexpr CStringView testString("Hello World ");

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

  SUBCASE("exact_match_ends_with") {
    constexpr CStringView testString("Hello");

    REQUIRE(testString.ends_with("Hello"));
    REQUIRE(testString.ends_with("llo"));
    REQUIRE(testString.ends_with("o"));
    REQUIRE(testString.ends_with(""));

    static_assert(testString.ends_with("Hello"));
    static_assert(testString.ends_with("llo"));
    static_assert(testString.ends_with("o"));
    static_assert(testString.ends_with(""));
  }

  SUBCASE("overlapping_patterns_ends_with") {
    constexpr CStringView testString("ababab");

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

  SUBCASE("multiple_occurrences_ends_with") {
    constexpr CStringView testString("abababab");

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

  SUBCASE("unicode_content_ends_with") {
    constexpr CStringView testString("Hello 世界");

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

  SUBCASE("long_strings_ends_with") {
    constexpr CStringView testString("This is a very long string for performance testing");

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

  SUBCASE("constexpr_operations_ends_with") {
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("World");
    constexpr CStringView str3("Test");

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
TEST_CASE("core/c_string_view/contains") {
  SUBCASE("c_string_contains") {
    constexpr CStringView str("Hello World");

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

  SUBCASE("c_string_view_contains") {
    constexpr CStringView str("Hello World");

    // Basic contains with CStringView
    REQUIRE(str.contains(CStringView("World")));
    REQUIRE(str.contains(CStringView("Hello")));
    REQUIRE(str.contains(CStringView("lo Wo")));
    REQUIRE(str.contains(CStringView("Hello World")));
    REQUIRE(str.contains(CStringView("xyz")) == false);
    REQUIRE(str.contains(CStringView("")));
    REQUIRE(str.contains(CStringView()));

    static_assert(str.contains(CStringView("World")));
    static_assert(str.contains(CStringView("Hello")));
    static_assert(str.contains(CStringView("lo Wo")));
    static_assert(str.contains(CStringView("Hello World")));
    static_assert(str.contains(CStringView("xyz")) == false);
    static_assert(str.contains(CStringView("")));
    static_assert(str.contains(CStringView()));
  }

  SUBCASE("string_like_contains") {
    constexpr CStringView str("Hello World");

    REQUIRE(str.contains(std::string("World")));
    REQUIRE(str.contains(std::string("Hello")));
    REQUIRE(str.contains(std::string("lo Wo")));
    REQUIRE(str.contains(std::string("Hello World")));
    REQUIRE(str.contains(std::string("xyz")) == false);
    REQUIRE(str.contains(std::string("")));

    static_assert(str.contains(FixedString<16>("World")));
    static_assert(str.contains(FixedString<16>("Hello")));
    static_assert(str.contains(FixedString<16>("lo Wo")));
    static_assert(str.contains(FixedString<16>("Hello World")));
    static_assert(str.contains(FixedString<16>("xyz")) == false);
    static_assert(str.contains(FixedString<16>("")));
  }

  SUBCASE("character_contains") {
    constexpr CStringView str("Hello World");

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

  SUBCASE("empty_string_contains") {
    constexpr CStringView str("");

    // Empty string contains
    REQUIRE(str.contains(CStringView("Hello")) == false);
    REQUIRE(str.contains(std::string("Hello")) == false);
    REQUIRE(str.contains("Hello") == false);
    REQUIRE(str.contains('H') == false);
    REQUIRE(str.contains(""));

    static_assert(str.contains(CStringView("Hello")) == false);
    static_assert(str.contains(FixedString<8>("Hello")) == false);
    static_assert(str.contains("Hello") == false);
    static_assert(str.contains('H') == false);
    static_assert(str.contains(""));
  }

  SUBCASE("single_character_string_contains") {
    constexpr CStringView str("A");

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
    constexpr CStringView str("Hello World");

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
    constexpr CStringView str("Hello");

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
    constexpr CStringView str("Hello\n\tWorld!");

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
    constexpr CStringView str("Hello 世界");

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

  SUBCASE("repeated_patterns") {
    constexpr CStringView str("ababab");

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
    constexpr CStringView str("12345");

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
    constexpr CStringView str("123Hello456");

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

  SUBCASE("position_specific_contains") {
    constexpr CStringView str("Hello World");

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

// operator== and operator!=.
TEST_CASE("core/c_string_view/operator_equal") {
  SUBCASE("c_string_view_equal_c_string_view") {
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("Hello");
    constexpr CStringView str3("World");
    constexpr CStringView empty1;
    constexpr CStringView empty2;

    REQUIRE(str1 == str2);
    REQUIRE(str2 == str1);
    REQUIRE_FALSE(str1 == str3);
    REQUIRE_FALSE(str3 == str1);
    REQUIRE(empty1 == empty2);
    REQUIRE(empty2 == empty1);
    REQUIRE_FALSE(str1 == empty1);
    REQUIRE_FALSE(empty1 == str1);

    static_assert(str1 == str2);
    static_assert(str2 == str1);
    static_assert(str1 != str3);
    static_assert(str3 != str1);
    static_assert(empty1 == empty2);
    static_assert(empty2 == empty1);
    static_assert(str1 != empty1);
    static_assert(empty1 != str1);
  }

  SUBCASE("c_string_view_equal_string_like") {
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

  SUBCASE("c_string_view_equal_c_string") {
    constexpr CStringView str1("Hello");
    constexpr CStringView empty;

    REQUIRE(str1 == "Hello");
    REQUIRE("Hello" == str1);
    REQUIRE_FALSE(str1 == "World");
    REQUIRE_FALSE("World" == str1);
    REQUIRE(empty == "");
    REQUIRE("" == empty);
    REQUIRE_FALSE(str1 == "");
    REQUIRE_FALSE("" == str1);

    static_assert(str1 == "Hello");
    static_assert("Hello" == str1);
    static_assert(str1 != "World");
    static_assert("World" != str1);
    static_assert(empty == "");
    static_assert("" == empty);
    static_assert(str1 != "");
    static_assert("" != str1);
  }

  SUBCASE("edge_cases") {
    constexpr CStringView str("A");
    constexpr CStringView empty1;
    constexpr CStringView empty2;

    // Different sizes with same content
    constexpr CStringView small("Hi");
    constexpr CStringView large("Hi");

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
    constexpr CStringView str1("Hello\nWorld");
    constexpr CStringView str2("Hello\tWorld");
    constexpr CStringView str3("Hello World");

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
    constexpr CStringView str1("Привет");
    constexpr CStringView str2("Мир");
    constexpr CStringView str3("Привет");

    static_assert(str1 == "Привет");
    static_assert("Привет" == str1);
    static_assert(str1 == str3);
    static_assert(str3 == str1);
    static_assert(str1 != str2);
    static_assert(str2 != str1);
  }

  SUBCASE("performance_test") {
    constexpr CStringView str1("This is a longer string for performance testing");
    constexpr CStringView str2("This is a longer string for performance testing");
    constexpr CStringView str3("This is a different string for performance testing");

    static_assert(str1 == str2);
    static_assert(str2 == str1);
    static_assert(str1 != str3);
    static_assert(str3 != str1);
  }

  SUBCASE("constexpr_operations") {
    constexpr CStringView str1("Test");
    constexpr CStringView str2("Test");
    constexpr CStringView str3("Different");

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
TEST_CASE("core/c_string_view/operator_three_way") {
  SUBCASE("c_string_view_three_way_c_string_view") {
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("Hello");
    constexpr CStringView str3("World");
    constexpr CStringView str4("Hi");
    constexpr CStringView str5("Hell");

    // Equal strings
    REQUIRE((str1 <=> str1) == strong_ordering::equal);
    REQUIRE((str1 <=> str2) == strong_ordering::equal);
    REQUIRE((str2 <=> str1) == strong_ordering::equal);

    // Different strings
    REQUIRE((str1 <=> str3) == strong_ordering::less);
    REQUIRE((str3 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> str4) == strong_ordering::less);
    REQUIRE((str4 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> str5) == strong_ordering::greater);
    REQUIRE((str5 <=> str1) == strong_ordering::less);

    static_assert((str1 <=> str1) == strong_ordering::equal);
    static_assert((str1 <=> str2) == strong_ordering::equal);
    static_assert((str2 <=> str1) == strong_ordering::equal);

    static_assert((str1 <=> str3) == strong_ordering::less);
    static_assert((str3 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> str4) == strong_ordering::less);
    static_assert((str4 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> str5) == strong_ordering::greater);
    static_assert((str5 <=> str1) == strong_ordering::less);
  }

  SUBCASE("c_string_view_three_way_string_like") {
    constexpr CStringView str("Hello");
    constexpr CStringView empty;
    const std::string stdStr1("Hello");
    const std::string stdStr2("World");
    const std::string stdEmpty;

    REQUIRE((str <=> stdStr1) == strong_ordering::equal);
    REQUIRE((stdStr1 <=> str) == strong_ordering::equal);
    REQUIRE((str <=> stdStr2) == strong_ordering::less);
    REQUIRE((stdStr2 <=> str) == strong_ordering::greater);
    REQUIRE((str <=> stdEmpty) == strong_ordering::greater);
    REQUIRE((stdEmpty <=> str) == strong_ordering::less);

    REQUIRE((empty <=> stdStr1) == strong_ordering::less);
    REQUIRE((stdStr1 <=> empty) == strong_ordering::greater);
    REQUIRE((empty <=> stdStr2) == strong_ordering::less);
    REQUIRE((stdStr2 <=> empty) == strong_ordering::greater);
    REQUIRE((empty <=> stdEmpty) == strong_ordering::equal);
    REQUIRE((stdEmpty <=> empty) == strong_ordering::equal);
  }

  SUBCASE("c_string_view_three_way_c_string") {
    constexpr CStringView str1("Hello");
    constexpr const char * str2 = "Hello";
    constexpr const char * str3 = "World";
    constexpr const char * str4 = "Hi";
    constexpr const char * str5 = "Hell";

    // Equal strings
    REQUIRE((str1 <=> str2) == strong_ordering::equal);
    REQUIRE((str2 <=> str1) == strong_ordering::equal);

    // Different strings
    REQUIRE((str1 <=> str3) == strong_ordering::less);
    REQUIRE((str3 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> str4) == strong_ordering::less);
    REQUIRE((str4 <=> str1) == strong_ordering::greater);
    REQUIRE((str1 <=> str5) == strong_ordering::greater);
    REQUIRE((str5 <=> str1) == strong_ordering::less);

    static_assert((str1 <=> str2) == strong_ordering::equal);
    static_assert((str2 <=> str1) == strong_ordering::equal);

    static_assert((str1 <=> str3) == strong_ordering::less);
    static_assert((str3 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> str4) == strong_ordering::less);
    static_assert((str4 <=> str1) == strong_ordering::greater);
    static_assert((str1 <=> str5) == strong_ordering::greater);
    static_assert((str5 <=> str1) == strong_ordering::less);
  }

  SUBCASE("empty_string_comparisons") {
    constexpr CStringView empty1("");
    constexpr CStringView empty2("");
    constexpr CStringView nonEmpty("Test");
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

    static_assert((empty1 <=> empty2) == strong_ordering::equal);
    static_assert((empty2 <=> empty1) == strong_ordering::equal);
    static_assert((empty1 <=> emptyCStr) == strong_ordering::equal);
    static_assert((emptyCStr <=> empty1) == strong_ordering::equal);

    static_assert((empty1 <=> nonEmpty) == strong_ordering::less);
    static_assert((nonEmpty <=> empty1) == strong_ordering::greater);
    static_assert((empty1 <=> nonEmptyCStr) == strong_ordering::less);
    static_assert((nonEmptyCStr <=> empty1) == strong_ordering::greater);
  }

  SUBCASE("single_character_strings") {
    constexpr CStringView str1("A");
    constexpr CStringView str2("B");
    constexpr CStringView str3("A");
    constexpr CStringView str4("Z");

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
    constexpr CStringView lower("hello");
    constexpr CStringView upper("HELLO");
    constexpr CStringView mixed("Hello");

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

  SUBCASE("prefix_comparisons") {
    constexpr CStringView str1("Hello");
    constexpr CStringView str2("HelloWorld");
    constexpr CStringView str3("Hell");
    constexpr CStringView str4("Hello");

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
    constexpr CStringView str1("Hello\nWorld");
    constexpr CStringView str2("Hello\tWorld");
    constexpr CStringView str3("Hello World");
    constexpr CStringView str4("Hello\nWorld");

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
    constexpr CStringView str1("Привет");
    constexpr CStringView str2("Мир");
    constexpr CStringView str3("Привет");
    constexpr CStringView str4("Hello 🌍");

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

  SUBCASE("numeric_strings") {
    constexpr CStringView str1("123");
    constexpr CStringView str2("456");
    constexpr CStringView str3("123");
    constexpr CStringView str4("12");
    constexpr CStringView str5("1234");

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

  SUBCASE("constexpr_operations") {
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

    static_assert(eq1 == strong_ordering::equal);
    static_assert(eq2 != strong_ordering::equal);
    static_assert(eq3 == strong_ordering::equal);
    static_assert(eq4 == strong_ordering::equal);
    static_assert(eq5 != strong_ordering::equal);
    static_assert(eq6 == strong_ordering::equal);
  }

  SUBCASE("performance_test") {
    constexpr CStringView str1("This is a very long string for performance testing");
    constexpr CStringView str2("This is a very long string for performance testing");
    constexpr CStringView str3("This is a very long string for performance testing!");
    constexpr CStringView str4("This is a different string for performance testing");

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
TEST_CASE("core/c_string_view/std_swap") {
  SUBCASE("basic_swap_functionality") {
    CStringView str1("Hello");
    CStringView str2("World");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "World") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);
  }

  SUBCASE("swap_with_empty_strings") {
    CStringView str1("Hello");
    CStringView str2("");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
    REQUIRE(str2.size() == 5);
    REQUIRE(std::strcmp(str2.c_str(), "Hello") == 0);
  }

  SUBCASE("swap_two_empty_strings") {
    CStringView str1("");
    CStringView str2("");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 0);
    REQUIRE(std::strcmp(str1.c_str(), "") == 0);
    REQUIRE(str2.size() == 0);
    REQUIRE(std::strcmp(str2.c_str(), "") == 0);
  }

  SUBCASE("self_swap") {
    CStringView str1("Hello");

    std::swap(str1, str1);

    REQUIRE(str1.size() == 5);
    REQUIRE(std::strcmp(str1.c_str(), "Hello") == 0);
  }

  SUBCASE("swap_with_different_sizes") {
    CStringView str1("Hi");
    CStringView str2("VeryLongString");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 14);
    REQUIRE(std::strcmp(str1.c_str(), "VeryLongString") == 0);
    REQUIRE(str2.size() == 2);
    REQUIRE(std::strcmp(str2.c_str(), "Hi") == 0);
  }

  SUBCASE("swap_with_maximum_length_strings") {
    CStringView str1("123456789012345"); // 15 chars
    CStringView str2("ABCDEFGHIJKLMNO"); // 15 chars

    std::swap(str1, str2);

    REQUIRE(str1.size() == 15);
    REQUIRE(std::strcmp(str1.c_str(), "ABCDEFGHIJKLMNO") == 0);
    REQUIRE(str2.size() == 15);
    REQUIRE(std::strcmp(str2.c_str(), "123456789012345") == 0);
  }

  SUBCASE("swap_with_special_characters") {
    CStringView str1("Hello,\n\t!");
    CStringView str2("World,\r\n?");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 9);
    REQUIRE(std::strcmp(str1.c_str(), "World,\r\n?") == 0);
    REQUIRE(str2.size() == 9);
    REQUIRE(std::strcmp(str2.c_str(), "Hello,\n\t!") == 0);
  }

  SUBCASE("swap_with_unicode_content") {
    CStringView str1("Hello 世界");
    CStringView str2("World 宇宙");

    std::swap(str1, str2);

    REQUIRE(str1.size() == std::char_traits<char>::length("World 宇宙"));
    REQUIRE(std::strcmp(str1.c_str(), "World 宇宙") == 0);
    REQUIRE(str2.size() == std::char_traits<char>::length("Hello 世界"));
    REQUIRE(std::strcmp(str2.c_str(), "Hello 世界") == 0);
  }

  SUBCASE("multiple_swaps") {
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

  SUBCASE("performance_test_with_large_strings") {
    CStringView str1("This is a very long string that tests swap performance");
    CStringView str2("Another very long string for performance testing");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 48);
    REQUIRE(std::strcmp(str1.c_str(), "Another very long string for performance testing") == 0);
    REQUIRE(str2.size() == 54);
    REQUIRE(std::strcmp(str2.c_str(), "This is a very long string that tests swap performance") == 0);
  }

  SUBCASE("swap_with_single_character_strings") {
    CStringView str1("A");
    CStringView str2("B");

    std::swap(str1, str2);

    REQUIRE(str1.size() == 1);
    REQUIRE(std::strcmp(str1.c_str(), "B") == 0);
    REQUIRE(str2.size() == 1);
    REQUIRE(std::strcmp(str2.c_str(), "A") == 0);
  }
}

} // namespace toy
*/
