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
  constexpr FixString<12> testString1("test text 1");
  constexpr FixString<12> testString2(testString1);
  constexpr FixString<24> testString3(testString2);
  constexpr FixString<18> testString4(testString3);
  constexpr FixString<12> testString5;
  constexpr FixString<11> testString6('t', 10);

  CHECK(strcmp(testString1.c_str(), "test text 1") == 0);
  CHECK(testString1.size() == 11);

  CHECK(strcmp(testString2.c_str(), "test text 1") == 0);
  CHECK(testString2.size() == 11);

  CHECK(strcmp(testString3.c_str(), "test text 1") == 0);
  CHECK(testString3.size() == 11);

  CHECK(strcmp(testString4.c_str(), "test text 1") == 0);
  CHECK(testString4.size() == 11);

  CHECK(strcmp(testString5.c_str(), "") == 0);
  CHECK(testString5.size() == 0);

  CHECK(strcmp(testString6.c_str(), "tttttttttt") == 0);
  CHECK(testString6.size() == 10);
}

TEST_CASE("FixString operators=", "[core][fixstring]") {
  FixString<12> testString1;
  FixString<12> testString2;
  FixString<24> testString3;
  FixString<18> testString4;
  FixString<2> testString5;

  testString1 = "test text 1";
  testString2 = testString1;
  testString3 = testString2;
  testString4 = testString3;
  testString5 = 't';

  CHECK(strcmp(testString1.c_str(), "test text 1") == 0);
  CHECK(testString1.size() == 11);

  CHECK(strcmp(testString2.c_str(), "test text 1") == 0);
  CHECK(testString2.size() == 11);

  CHECK(strcmp(testString3.c_str(), "test text 1") == 0);
  CHECK(testString3.size() == 11);

  CHECK(strcmp(testString4.c_str(), "test text 1") == 0);
  CHECK(testString4.size() == 11);

  CHECK(strcmp(testString5.c_str(), "t") == 0);
  CHECK(testString5.size() == 1);

  // Self-assignment (same capacity)
  testString2 = testString2;
  CHECK(strcmp(testString2.c_str(), "test text 1") == 0);
  CHECK(testString2.size() == 11);

  // Assign from own c_str() (no-op path)
  testString3 = testString3.c_str();
  CHECK(strcmp(testString3.c_str(), "test text 1") == 0);
  CHECK(testString3.size() == 11);

  testString1 = "";
  testString2 = FixString<12>("");
  testString3 = FixString<12>("");
  testString4 = FixString<24>("");

  CHECK(strcmp(testString1.c_str(), "") == 0);
  CHECK(testString1.size() == 0);

  CHECK(strcmp(testString2.c_str(), "") == 0);
  CHECK(testString2.size() == 0);

  CHECK(strcmp(testString3.c_str(), "") == 0);
  CHECK(testString3.size() == 0);

  CHECK(strcmp(testString4.c_str(), "") == 0);
  CHECK(testString4.size() == 0);
}

TEST_CASE("FixString assign", "[core][fixstring]") {
  FixString<12> testString1;
  FixString<12> testString2;
  FixString<24> testString3;
  FixString<18> testString4;
  FixString<8> testString5;

  testString1.assign("test text 1");
  testString2.assign(testString1);
  testString3.assign(testString2);
  testString4.assign(testString3);
  testString5.assign('t', 4);

  CHECK(strcmp(testString1.c_str(), "test text 1") == 0);
  CHECK(testString1.size() == 11);

  CHECK(strcmp(testString2.c_str(), "test text 1") == 0);
  CHECK(testString2.size() == 11);

  CHECK(strcmp(testString3.c_str(), "test text 1") == 0);
  CHECK(testString3.size() == 11);

  CHECK(strcmp(testString4.c_str(), "test text 1") == 0);
  CHECK(testString4.size() == 11);

  CHECK(strcmp(testString5.c_str(), "tttt") == 0);
  CHECK(testString5.size() == 4);

  // Self-assignment (same capacity)
  testString2.assign(testString2);
  CHECK(strcmp(testString2.c_str(), "test text 1") == 0);
  CHECK(testString2.size() == 11);

  // Assign from own c_str() (no-op path)
  testString3.assign(testString3.c_str());
  CHECK(strcmp(testString3.c_str(), "test text 1") == 0);
  CHECK(testString3.size() == 11);

  testString1.assign("");
  testString2.assign(FixString<12>(""));
  testString3.assign(FixString<12>(""));
  testString4.assign(FixString<24>(""));

  CHECK(strcmp(testString1.c_str(), "") == 0);
  CHECK(testString1.size() == 0);

  CHECK(strcmp(testString2.c_str(), "") == 0);
  CHECK(testString2.size() == 0);

  CHECK(strcmp(testString3.c_str(), "") == 0);
  CHECK(testString3.size() == 0);

  CHECK(strcmp(testString4.c_str(), "") == 0);
  CHECK(testString4.size() == 0);

  // Chaining assign (after return type becomes non-const and count has default)
  testString1.assign("a").assign("b");
  CHECK(strcmp(testString1.c_str(), "b") == 0);
  CHECK(testString1.size() == 1);
}

TEST_CASE("FixString at", "[core][fixstring]") {
  FixString<8> testString1("abcd");
  constexpr FixString<8> testString2("dcba");

  testString1.at(0) = 'e';
  testString1.at(1) = 'f';
  testString1.at(2) = 'g';
  testString1.at(3) = 'h';

  CHECK(strcmp(testString1.c_str(), "efgh") == 0);

  auto & ref = testString1.at(2);
  ref = 'Z';
  CHECK(testString1.at(2) == 'Z');

  CHECK(testString1.at(1) == 'f');
  CHECK(testString2.at(1) == 'c');

  CHECK(testString1.size() == 4);
  CHECK(strcmp(testString2.c_str(), "dcba") == 0);
}

TEST_CASE("FixString operators[]", "[core][fixstring]") {
  FixString<8> testString1("abcd");
  constexpr FixString<8> testString2("dcba");

  testString1[0] = 'e';
  testString1[1] = 'f';
  testString1[2] = 'g';
  testString1[3] = 'h';

  CHECK(strcmp(testString1.c_str(), "efgh") == 0);

  auto & ref = testString1[2];
  ref = 'Z';
  CHECK(testString1[2] == 'Z');

  CHECK(testString1[1] == 'f');
  CHECK(testString2[1] == 'c');

  CHECK(testString1.size() == 4);
  CHECK(strcmp(testString2.c_str(), "dcba") == 0);
}

TEST_CASE("FixString front and back", "[core][fixstring]") {
  SECTION("Front method") {
    FixString<16> testString("Hello World");

    CHECK(testString.front() == 'H');
    CHECK(testString[0] == 'H');

    // Test modification
    testString.front() = 'h';
    CHECK(testString.front() == 'h');
    CHECK(testString[0] == 'h');
  }

  SECTION("Front const method") {
    constexpr FixString<12> testString("Hello World");

    CHECK(testString.front() == 'H');
    CHECK(testString[0] == 'H');
  }

  SECTION("Back method") {
    FixString<16> testString("Hello World");

    CHECK(testString.back() == 'd');
    CHECK(testString[testString.size() - 1] == 'd');

    // Test modification
    testString.back() = 'D';
    CHECK(testString.back() == 'D');
    CHECK(testString[testString.size() - 1] == 'D');
  }

  SECTION("Back const method") {
    constexpr FixString<12> testString("Hello World");

    CHECK(testString.back() == 'd');
    CHECK(testString[testString.size() - 1] == 'd');
  }

  SECTION("Single character string") {
    FixString<2> testString("A");

    CHECK(testString.front() == 'A');
    CHECK(testString.back() == 'A');
    CHECK(testString.front() == testString.back());

    // Test modification
    testString.front() = 'B';
    CHECK(testString.front() == 'B');
    CHECK(testString.back() == 'B');

    testString.back() = 'C';
    CHECK(testString.front() == 'C');
    CHECK(testString.back() == 'C');
  }

  SECTION("Modification through references") {
    FixString<32> testString("Hello World");

    // Modify through front reference
    char & frontRef = testString.front();
    frontRef = 'X';
    CHECK(testString.front() == 'X');
    CHECK(testString[0] == 'X');

    // Modify through back reference
    char & backRef = testString.back();
    backRef = 'Y';
    CHECK(testString.back() == 'Y');
    CHECK(testString[testString.size() - 1] == 'Y');
  }

  SECTION("Const references") {
    constexpr FixString<32> testString("Hello World");

    // Get constexpr references
    const auto & frontRef = testString.front();
    const auto & backRef = testString.back();

    CHECK(frontRef == 'H');
    CHECK(backRef == 'd');
    CHECK(frontRef == testString[0]);
    CHECK(backRef == testString[testString.size() - 1]);
  }
}

TEST_CASE("FixString data", "[core][fixstring]") {
  constexpr FixString<8> testString1("abcd");
  constexpr FixString<8> testString2("dcba");

  CHECK(strcmp(testString1.data(), "abcd") == 0);
  CHECK(strcmp(testString2.data(), "dcba") == 0);
}

TEST_CASE("FixString c_str", "[core][fixstring]") {
  constexpr FixString<8> testString1("abcd");
  constexpr FixString<8> testString2("dcba");

  CHECK(strcmp(testString1.c_str(), "abcd") == 0);
  CHECK(strcmp(testString2.c_str(), "dcba") == 0);
}

TEST_CASE("FixString empty", "[core][fixstring]") {
  constexpr FixString<16> testString1("ToyGine2");
  constexpr FixString<4> testString2("");

  CHECK(!testString1.empty());
  CHECK(testString2.empty());
}

TEST_CASE("FixString size", "[core][fixstring]") {
  constexpr FixString<64> testString1("ToyGine2 - Free 2D/3D game engine.");
  constexpr FixString<64> testString2;

  CHECK(testString1.size() == 34);
  CHECK(testString2.size() == 0);
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

  CHECK(testString1.size() == testString1.utf8_size());
  CHECK(testString3.size() == testString3.utf8_size());
  CHECK(testString2.size() == 66);
  CHECK(testString2.utf8_size() == 43);
}

TEST_CASE("FixString length", "[core][fixstring]") {
  constexpr FixString<64> testString1("ToyGine2 - Free 2D/3D game engine.");
  constexpr FixString<64> testString2;

  CHECK(testString1.length() == 34);
  CHECK(testString2.length() == 0);
}

TEST_CASE("FixString max_size", "[core][fixstring]") {
  constexpr FixString<64> testString1("ToyGine2 - Free 2D/3D game engine.");
  constexpr FixString<16> testString2("");

  CHECK(testString1.max_size() == 63);
  CHECK(testString2.max_size() == 15);
}

TEST_CASE("FixString capacity", "[core][fixstring]") {
  constexpr FixString<64> testString1("ToyGine2");
  constexpr FixString<16> testString2("");

  CHECK(testString1.capacity() == 63);
  CHECK(testString2.capacity() == 15);
}

TEST_CASE("FixString clear", "[core][fixstring]") {
  FixString<64> testString1("ToyGine2 - Free 2D/3D game engine.");
  FixString<16> testString2;

  CHECK(!testString1.empty());
  CHECK(testString2.empty());

  testString1.clear();
  testString2.clear();

  CHECK(testString1.empty());
  CHECK(testString2.empty());
}

TEST_CASE("FixString insert", "[core][fixstring]") {
  FixString<32> testString1("Hello World");
  FixString<32> testString2("Hello World");
  FixString<32> testString3("Hello World");

  // Insert at beginning
  testString1.insert(0, FixString<32>("Hi "));
  testString2.insert(0, FixString<16>("Hi "));
  testString3.insert(0, "Hi ");
  CHECK(strcmp(testString1.c_str(), "Hi Hello World") == 0);
  CHECK(strcmp(testString2.c_str(), "Hi Hello World") == 0);
  CHECK(strcmp(testString3.c_str(), "Hi Hello World") == 0);
  CHECK(testString1.size() == 14);
  CHECK(testString2.size() == 14);
  CHECK(testString3.size() == 14);

  // Insert in middle
  testString1.insert(9, FixString<32>("Beautiful "));
  testString2.insert(9, FixString<64>("Beautiful "));
  testString3.insert(9, "Beautiful ");
  CHECK(strcmp(testString1.c_str(), "Hi Hello Beautiful World") == 0);
  CHECK(strcmp(testString2.c_str(), "Hi Hello Beautiful World") == 0);
  CHECK(strcmp(testString3.c_str(), "Hi Hello Beautiful World") == 0);
  CHECK(testString1.size() == 24);
  CHECK(testString2.size() == 24);
  CHECK(testString3.size() == 24);

  // Insert at end
  testString1.insert(24, FixString<32>("!"));
  testString2.insert(24, FixString<8>("!"));
  testString3.insert(24, "!");
  CHECK(strcmp(testString1.c_str(), "Hi Hello Beautiful World!") == 0);
  CHECK(strcmp(testString2.c_str(), "Hi Hello Beautiful World!") == 0);
  CHECK(strcmp(testString3.c_str(), "Hi Hello Beautiful World!") == 0);
  CHECK(testString1.size() == 25);
  CHECK(testString2.size() == 25);
  CHECK(testString3.size() == 25);

  FixString<32> testString4("Hello World");

  // Insert single character
  testString4.insert(5, ' ');
  CHECK(strcmp(testString4.c_str(), "Hello  World") == 0);
  CHECK(testString4.size() == 12);

  // Insert multiple characters
  testString4.insert(0, '*', 3);
  CHECK(strcmp(testString4.c_str(), "***Hello  World") == 0);
  CHECK(testString4.size() == 15);

  // Insert at end
  testString4.insert(15, '!', 2);
  CHECK(strcmp(testString4.c_str(), "***Hello  World!!") == 0);
  CHECK(testString4.size() == 17);

  // Insert zero characters
  testString4.insert(0, 'X', 0);
  CHECK(strcmp(testString4.c_str(), "***Hello  World!!") == 0);
  CHECK(testString4.size() == 17);
}

TEST_CASE("FixString erase", "[core][fixstring]") {
  FixString<32> testString1("Hello World");

  // Erase first 5 characters
  testString1.erase(0, 5);
  CHECK(strcmp(testString1.c_str(), " World") == 0);
  CHECK(testString1.size() == 6);

  // Erase remaining characters
  testString1.erase(0);
  CHECK(strcmp(testString1.c_str(), "") == 0);
  CHECK(testString1.size() == 0);

  FixString<32> testString2("Hello Beautiful World");

  // Erase middle word
  testString2.erase(6, 10);
  CHECK(strcmp(testString2.c_str(), "Hello World") == 0);
  CHECK(testString2.size() == 11);

  // Erase part of remaining text
  testString2.erase(5, 1);
  CHECK(strcmp(testString2.c_str(), "HelloWorld") == 0);
  CHECK(testString2.size() == 10);

  FixString<32> testString3("Hello World!");

  // Erase last character
  testString3.erase(11, 1);
  CHECK(strcmp(testString3.c_str(), "Hello World") == 0);
  CHECK(testString3.size() == 11);

  // No-op: erase with count == 0
  testString3.erase(5, 0);
  CHECK(strcmp(testString3.c_str(), "Hello World") == 0);
  CHECK(testString3.size() == 11);

  // Erase last word
  testString3.erase(6);
  CHECK(strcmp(testString3.c_str(), "Hello ") == 0);
  CHECK(testString3.size() == 6);

  // Erase everything from position 0
  testString3.erase(0);
  CHECK(testString3.empty());
}

TEST_CASE("FixString push_back", "[core][fixstring]") {
  FixString<16> testString1("Hello");

  testString1.push_back(' ');
  testString1.push_back('W');
  testString1.push_back('o');
  testString1.push_back('r');
  testString1.push_back('l');
  testString1.push_back('d');

  CHECK(strcmp(testString1.c_str(), "Hello World") == 0);
  CHECK(testString1.size() == 11);
}

TEST_CASE("FixString pop_back", "[core][fixstring]") {
  FixString<16> testString("Hi");

  testString.pop_back();
  CHECK(strcmp(testString.c_str(), "H") == 0);
  CHECK(testString.size() == 1);

  testString.pop_back();
  CHECK(strcmp(testString.c_str(), "") == 0);
  CHECK(testString.size() == 0);
}

TEST_CASE("FixString utf8_pop_back", "[core][fixstring]") {
  SECTION("Pop back single ASCII character") {
    FixString<16> testString("Hello");

    testString.utf8_pop_back();

    CHECK(strcmp(testString.c_str(), "Hell") == 0);
    CHECK(testString.size() == 4);
    CHECK(testString.utf8_size() == 4);
  }

  SECTION("Pop back multiple ASCII characters") {
    FixString<16> testString("Hello");

    testString.utf8_pop_back();
    testString.utf8_pop_back();
    testString.utf8_pop_back();

    CHECK(strcmp(testString.c_str(), "He") == 0);
    CHECK(testString.size() == 2);
    CHECK(testString.utf8_size() == 2);
  }

  SECTION("Pop back UTF-8 character (2 bytes)") {
    // "Hello привет" - "привет" contains 6 Cyrillic characters, each 2 bytes in UTF-8
    FixString<32> testString("Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82");

    testString.utf8_pop_back(); // Remove 'т' (2 bytes)

    CHECK(strcmp(testString.c_str(), "Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5") == 0);
    CHECK(testString.size() == 16); // "Hello приве" (5 + 1 + 5*2 = 16 bytes)
    CHECK(testString.utf8_size() == 11);
  }

  SECTION("Pop back multiple UTF-8 characters") {
    // "Hello привет" - "привет" contains 6 Cyrillic characters
    FixString<32> testString("Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82");

    testString.utf8_pop_back(); // Remove 'т'
    testString.utf8_pop_back(); // Remove 'е'
    testString.utf8_pop_back(); // Remove 'в'

    CHECK(strcmp(testString.c_str(), "Hello \xD0\xBF\xD1\x80\xD0\xB8") == 0);
    CHECK(testString.size() == 12); // "Hello при" (5 + 1 + 3*2 = 12 bytes)
    CHECK(testString.utf8_size() == 9);
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

    CHECK(strcmp(testString.c_str(), "Hello ") == 0);
    CHECK(testString.size() == 6);
    CHECK(testString.utf8_size() == 6);
  }

  SECTION("Pop back from single character string") {
    FixString<8> testString("A");

    testString.utf8_pop_back();

    CHECK(strcmp(testString.c_str(), "") == 0);
    CHECK(testString.size() == 0);
    CHECK(testString.utf8_size() == 0);
  }

  SECTION("Pop back from single UTF-8 character string") {
    // Single Cyrillic character 'п'
    FixString<8> testString("\xD0\xBF");

    testString.utf8_pop_back();

    CHECK(strcmp(testString.c_str(), "") == 0);
    CHECK(testString.size() == 0);
    CHECK(testString.utf8_size() == 0);
  }
}

TEST_CASE("FixString append", "[core][fixstring]") {
  FixString<32> testString("");

  testString.append(FixString<16>("Nothing"))
    .append(FixString<8>(" else"))
    .append(std::string(" really"))
    .append(" matters")
    .append('.', 3);

  CHECK(strcmp(testString.c_str(), "Nothing else really matters...") == 0);
  CHECK(testString.size() == 30);
}

TEST_CASE("FixString operators+=", "[core][fixstring]") {
  FixString<32> testString("");

  testString += FixString<16>("Nothing");
  testString += FixString<8>(" else");
  testString += std::string(" really");
  testString += " matters";
  testString += '.';

  CHECK(strcmp(testString.c_str(), "Nothing else really matters.") == 0);
  CHECK(testString.size() == 28);
}

TEST_CASE("FixString replace", "[core][fixstring]") {
  SECTION("Replace with FixString") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, FixString<16>("Universe"));

    CHECK(strcmp(testString.c_str(), "Hello Universe") == 0);
    CHECK(testString.size() == 14);
  }

  SECTION("Replace with string-like object") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, std::string("Universe"));

    CHECK(strcmp(testString.c_str(), "Hello Universe") == 0);
    CHECK(testString.size() == 14);
  }

  SECTION("Replace with C string") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, "Universe");

    CHECK(strcmp(testString.c_str(), "Hello Universe") == 0);
    CHECK(testString.size() == 14);
  }

  SECTION("Replace with repeated characters") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, '*', 3);

    CHECK(strcmp(testString.c_str(), "Hello ***") == 0);
    CHECK(testString.size() == 9);
  }

  SECTION("Replace at beginning") {
    FixString<32> testString("Hello World");

    testString.replace(0, 5, "Hi");

    CHECK(strcmp(testString.c_str(), "Hi World") == 0);
    CHECK(testString.size() == 8);
  }

  SECTION("Replace at end") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, "Universe!");

    CHECK(strcmp(testString.c_str(), "Hello Universe!") == 0);
    CHECK(testString.size() == 15);
  }

  SECTION("Replace with empty string") {
    FixString<32> testString("Hello World");

    testString.replace(5, 1, "");

    CHECK(strcmp(testString.c_str(), "HelloWorld") == 0);
    CHECK(testString.size() == 10);
  }

  SECTION("Replace with longer string") {
    FixString<32> testString("Hi");

    testString.replace(0, 2, "Hello World");

    CHECK(strcmp(testString.c_str(), "Hello World") == 0);
    CHECK(testString.size() == 11);
  }

  SECTION("Replace with shorter string") {
    FixString<32> testString("Hello World");

    testString.replace(0, 5, "Hi");

    CHECK(strcmp(testString.c_str(), "Hi World") == 0);
    CHECK(testString.size() == 8);
  }

  SECTION("Replace single character") {
    FixString<32> testString("Hello World");

    testString.replace(0, 1, "h");

    CHECK(strcmp(testString.c_str(), "hello World") == 0);
    CHECK(testString.size() == 11);
  }

  SECTION("Replace multiple characters with single character") {
    FixString<32> testString("Hello World");

    testString.replace(0, 5, "H");

    CHECK(strcmp(testString.c_str(), "H World") == 0);
    CHECK(testString.size() == 7);
  }

  SECTION("Replace with repeated characters at different positions") {
    FixString<32> testString("Hello World");

    testString.replace(0, 1, '*', 2);
    testString.replace(8, 2, '#', 3);

    CHECK(strcmp(testString.c_str(), "**ello W###ld") == 0);
    CHECK(testString.size() == 13);
  }

  SECTION("Replace entire string") {
    FixString<32> testString("Hello World");

    testString.replace(0, 11, "Goodbye Universe");

    CHECK(strcmp(testString.c_str(), "Goodbye Universe") == 0);
    CHECK(testString.size() == 16);
  }

  SECTION("Replace with zero count") {
    FixString<32> testString("Hello World");

    testString.replace(6, 0, "Beautiful ");

    CHECK(strcmp(testString.c_str(), "Hello Beautiful World") == 0);
    CHECK(testString.size() == 21);
  }

  SECTION("Replace with single character count") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, '!', 1);

    CHECK(strcmp(testString.c_str(), "Hello !") == 0);
    CHECK(testString.size() == 7);
  }

  SECTION("Replace with multiple character count") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, '=', 4);

    CHECK(strcmp(testString.c_str(), "Hello ====") == 0);
    CHECK(testString.size() == 10);
  }

  SECTION("Replace with zero character count") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, 'X', 0);

    CHECK(strcmp(testString.c_str(), "Hello ") == 0);
    CHECK(testString.size() == 6);
  }

  SECTION("Replace middle portion") {
    FixString<32> testString("Hello Beautiful World");

    testString.replace(6, 9, "Amazing");

    CHECK(strcmp(testString.c_str(), "Hello Amazing World") == 0);
    CHECK(testString.size() == 19);
  }

  SECTION("Replace with same length string") {
    FixString<32> testString("Hello World");

    testString.replace(0, 5, "Greet");

    CHECK(strcmp(testString.c_str(), "Greet World") == 0);
    CHECK(testString.size() == 11);
  }

  SECTION("Replace with FixString of different capacity") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, FixString<12>("Universe"));

    CHECK(strcmp(testString.c_str(), "Hello Universe") == 0);
    CHECK(testString.size() == 14);
  }

  SECTION("Replace with std::string") {
    FixString<32> testString("Hello World");

    testString.replace(6, 5, std::string("Universe"));

    CHECK(strcmp(testString.c_str(), "Hello Universe") == 0);
    CHECK(testString.size() == 14);
  }

  SECTION("Replace with array") {
    FixString<32> testString("Hello World");
    constexpr std::array<char, 9> arr = {'U', 'n', 'i', 'v', 'e', 'r', 's', 'e', '\0'};

    testString.replace(6, 5, arr.data());

    CHECK(strcmp(testString.c_str(), "Hello Universe") == 0);
    CHECK(testString.size() == 14);
  }

  SECTION("Replace at position 0 with zero count") {
    FixString<32> testString("Hello World");

    testString.replace(0, 0, "Hi ");

    CHECK(strcmp(testString.c_str(), "Hi Hello World") == 0);
    CHECK(testString.size() == 14);
  }

  SECTION("Replace at end with zero count") {
    FixString<32> testString("Hello World");

    testString.replace(11, 0, "!");

    CHECK(strcmp(testString.c_str(), "Hello World!") == 0);
    CHECK(testString.size() == 12);
  }
}

TEST_CASE("FixString copy", "[core][fixstring]") {
  SECTION("Copy entire string") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 11) == 11);
    CHECK(strncmp(buffer, "Hello World", 11) == 0);
  }

  SECTION("Copy partial string from beginning") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 5) == 5);
    CHECK(strncmp(buffer, "Hello", 5) == 0);
  }

  SECTION("Copy partial string from middle") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 5, 6) == 5);
    CHECK(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with npos count") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, FixString<16>::npos) == 11);
    CHECK(strncmp(buffer, "Hello World", 11) == 0);
  }

  SECTION("Copy with count exceeding remaining characters") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 20, 6) == 5);
    CHECK(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy from position 0") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 5, 0) == 5);
    CHECK(strncmp(buffer, "Hello", 5) == 0);
  }

  SECTION("Copy single character") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 1, 6) == 1);
    CHECK(buffer[0] == 'W');
  }

  SECTION("Copy from end position") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 1, 10) == 1);
    CHECK(buffer[0] == 'd');
  }

  SECTION("Copy with zero count") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 0) == 0);
  }

  SECTION("Copy from empty string") {
    constexpr FixString<16> testString("");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 5) == 0);
  }

  SECTION("Copy to small buffer") {
    constexpr FixString<16> testString("Hello World");
    char buffer[3] = {0};

    CHECK(testString.copy(buffer, 2) == 2);
    CHECK(strncmp(buffer, "He", 2) == 0);
  }

  SECTION("Copy with position at end") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 5, 11) == 0);
  }

  SECTION("Copy with npos from middle") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, FixString<16>::npos, 6) == 5);
    CHECK(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with exact remaining characters") {
    constexpr FixString<16> testString("Hello World");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 5, 6) == 5);
    CHECK(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with count larger than string size") {
    constexpr FixString<16> testString("Hi");
    char buffer[16] = {0};

    CHECK(testString.copy(buffer, 10) == 2);
    CHECK(strncmp(buffer, "Hi", 2) == 0);
  }
}

TEST_CASE("FixString swap", "[core][fixstring]") {
  SECTION("Swap two different strings") {
    FixString<16> string1("Hello");
    FixString<16> string2("World");

    string1.swap(string2);

    CHECK(strcmp(string1.c_str(), "World") == 0);
    CHECK(string1.size() == 5);
    CHECK(strcmp(string2.c_str(), "Hello") == 0);
    CHECK(string2.size() == 5);
  }

  SECTION("Swap strings of different lengths") {
    FixString<32> string1("Short");
    FixString<32> string2("This is a much longer string");

    string1.swap(string2);

    CHECK(strcmp(string1.c_str(), "This is a much longer string") == 0);
    CHECK(string1.size() == 28);
    CHECK(strcmp(string2.c_str(), "Short") == 0);
    CHECK(string2.size() == 5);
  }

  SECTION("Swap with empty string") {
    FixString<16> string1("Hello World");
    FixString<16> string2("");

    string1.swap(string2);

    CHECK(strcmp(string1.c_str(), "") == 0);
    CHECK(string1.size() == 0);
    CHECK(strcmp(string2.c_str(), "Hello World") == 0);
    CHECK(string2.size() == 11);
  }

  SECTION("Swap two empty strings") {
    FixString<16> string1("");
    FixString<16> string2("");

    string1.swap(string2);

    CHECK(strcmp(string1.c_str(), "") == 0);
    CHECK(string1.size() == 0);
    CHECK(strcmp(string2.c_str(), "") == 0);
    CHECK(string2.size() == 0);
  }

  SECTION("Self-swap (no-op)") {
    FixString<16> string1("Hello World");

    string1.swap(string1);

    CHECK(strcmp(string1.c_str(), "Hello World") == 0);
    CHECK(string1.size() == 11);
  }

  SECTION("Swap with single character strings") {
    FixString<8> string1("A");
    FixString<8> string2("B");

    string1.swap(string2);

    CHECK(strcmp(string1.c_str(), "B") == 0);
    CHECK(string1.size() == 1);
    CHECK(strcmp(string2.c_str(), "A") == 0);
    CHECK(string2.size() == 1);
  }

  SECTION("Swap with maximum length strings") {
    FixString<16> string1("123456789012345");
    FixString<16> string2("ABCDEFGHIJKLMNO");

    string1.swap(string2);

    CHECK(strcmp(string1.c_str(), "ABCDEFGHIJKLMNO") == 0);
    CHECK(string1.size() == 15);
    CHECK(strcmp(string2.c_str(), "123456789012345") == 0);
    CHECK(string2.size() == 15);
  }

  SECTION("Chained swap operations") {
    FixString<16> string1("First");
    FixString<16> string2("Second");
    FixString<16> string3("Third");

    string3.swap(string2);
    string2.swap(string1);
    string1.swap(string3);

    CHECK(strcmp(string1.c_str(), "Second") == 0);
    CHECK(string1.size() == 6);
    CHECK(strcmp(string2.c_str(), "First") == 0);
    CHECK(string2.size() == 5);
    CHECK(strcmp(string3.c_str(), "Third") == 0);
    CHECK(string3.size() == 5);
  }

  SECTION("Swap with repeated characters") {
    FixString<20> string1("AAA");
    FixString<20> string2("BBB");

    string1.swap(string2);

    CHECK(strcmp(string1.c_str(), "BBB") == 0);
    CHECK(string1.size() == 3);
    CHECK(strcmp(string2.c_str(), "AAA") == 0);
    CHECK(string2.size() == 3);
  }
}

TEST_CASE("FixString find", "[core][fixstring]") {
  SECTION("Find FixString substring") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find(FixString<16>("World")) == 6);
    CHECK(testString.find(FixString<16>("Hello")) == 0);
    CHECK(testString.find(FixString<16>("lo Wo")) == 3);
    CHECK(testString.find(FixString<16>("xyz")) == FixString<32>::npos);
  }

  SECTION("Find StringLike substring") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find(std::string("World")) == 6);
    CHECK(testString.find(std::string("Hello")) == 0);
    CHECK(testString.find(std::string("lo Wo")) == 3);
    CHECK(testString.find(std::string("xyz")) == FixString<32>::npos);
  }

  SECTION("Find C string substring") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find("World") == 6);
    CHECK(testString.find("Hello") == 0);
    CHECK(testString.find("lo Wo") == 3);
    CHECK(testString.find("xyz") == FixString<32>::npos);
  }

  SECTION("Find character") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find('H') == 0);
    CHECK(testString.find('l') == 2);
    CHECK(testString.find('o') == 4);
    CHECK(testString.find('W') == 6);
    CHECK(testString.find('d') == 10);
    CHECK(testString.find('x') == FixString<32>::npos);
  }

  SECTION("Find with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.find("Hello", 0) == 0);
    CHECK(testString.find("Hello", 1) == 12);
    CHECK(testString.find("Hello", 13) == FixString<32>::npos);
    CHECK(testString.find('l', 0) == 2);
    CHECK(testString.find('l', 3) == 3);
    CHECK(testString.find('l', 4) == 9);
    CHECK(testString.find('l', 10) == 14);
  }

  SECTION("Find empty substring") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find(FixString<16>("")) == 0);
    CHECK(testString.find(std::string("")) == 0);
    CHECK(testString.find("") == 0);
    CHECK(testString.find("", 5) == 5);
    CHECK(testString.find("", 11) == FixString<32>::npos);
  }

  SECTION("Find in empty string") {
    constexpr FixString<32> testString("");

    CHECK(testString.find(FixString<16>("Hello")) == FixString<32>::npos);
    CHECK(testString.find(std::string("Hello")) == FixString<32>::npos);
    CHECK(testString.find("Hello") == FixString<32>::npos);
    CHECK(testString.find('H') == FixString<32>::npos);
    CHECK(testString.find("") == FixString<32>::npos);
  }

  SECTION("Find with position beyond string size") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.find("World", 10) == FixString<32>::npos);
    CHECK(testString.find('H', 10) == FixString<32>::npos);
    CHECK(testString.find("", 10) == FixString<32>::npos);
  }

  SECTION("Find substring at end") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find("World") == 6);
    CHECK(testString.find("d") == 10);
    CHECK(testString.find("ld") == 9);
  }

  SECTION("Find substring at beginning") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find("Hello") == 0);
    CHECK(testString.find("H") == 0);
    CHECK(testString.find("He") == 0);
  }

  SECTION("Find overlapping substrings") {
    constexpr FixString<32> testString("ababab");

    CHECK(testString.find("ab") == 0);
    CHECK(testString.find("ab", 1) == 2);
    CHECK(testString.find("ab", 3) == 4);
    CHECK(testString.find("ab", 5) == FixString<32>::npos);
  }

  SECTION("Find with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    CHECK(testString.find("aa") == 0);
    CHECK(testString.find("aa", 1) == 1);
    CHECK(testString.find("aa", 2) == 2);
    CHECK(testString.find("aa", 3) == 3);
    CHECK(testString.find("aa", 4) == FixString<32>::npos);
  }

  SECTION("Find case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find("hello") == FixString<32>::npos);
    CHECK(testString.find("WORLD") == FixString<32>::npos);
    CHECK(testString.find("Hello") == 0);
    CHECK(testString.find("World") == 6);
  }

  SECTION("Find with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find(FixString<8>("World")) == 6);
    CHECK(testString.find(FixString<16>("World")) == 6);
    CHECK(testString.find(FixString<64>("World")) == 6);
  }

  SECTION("Find with exact match") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.find("Hello") == 0);
    CHECK(testString.find("Hello", 0) == 0);
    CHECK(testString.find("Hello", 1) == FixString<32>::npos);
  }

  SECTION("Find with single character string") {
    constexpr FixString<32> testString("A");

    CHECK(testString.find("A") == 0);
    CHECK(testString.find('A') == 0);
    CHECK(testString.find("B") == FixString<32>::npos);
    CHECK(testString.find('B') == FixString<32>::npos);
  }
}

TEST_CASE("FixString rfind", "[core][fixstring]") {
  SECTION("Rfind FixString substring") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.rfind(FixString<16>("Hello")) == 12);
    CHECK(testString.rfind(FixString<16>("World")) == 6);
    CHECK(testString.rfind(FixString<16>("lo")) == 15);
    CHECK(testString.rfind(FixString<16>("xyz")) == FixString<32>::npos);
  }

  SECTION("Rfind StringLike substring") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.rfind(std::string("Hello")) == 12);
    CHECK(testString.rfind(std::string("World")) == 6);
    CHECK(testString.rfind(std::string("lo")) == 15);
    CHECK(testString.rfind(std::string("xyz")) == FixString<32>::npos);
  }

  SECTION("Rfind C string substring") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.rfind("Hello") == 12);
    CHECK(testString.rfind("World") == 6);
    CHECK(testString.rfind("lo") == 15);
    CHECK(testString.rfind("xyz") == FixString<32>::npos);
  }

  SECTION("Rfind character") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.rfind('H') == 12);
    CHECK(testString.rfind('l') == 15);
    CHECK(testString.rfind('o') == 16);
    CHECK(testString.rfind('W') == 6);
    CHECK(testString.rfind('d') == 10);
    CHECK(testString.rfind('x') == FixString<32>::npos);
  }

  SECTION("Rfind with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.rfind("Hello", 12) == 12);
    CHECK(testString.rfind("Hello", 11) == 0);
    CHECK(testString.rfind("Hello", 0) == 0);
    CHECK(testString.rfind('l', 16) == 15);
    CHECK(testString.rfind('l', 13) == 9);
    CHECK(testString.rfind('l', 8) == 3);
    CHECK(testString.rfind('l', 2) == 2);
  }

  SECTION("Rfind empty substring") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.rfind(FixString<16>("")) == 11);
    CHECK(testString.rfind(std::string("")) == 11);
    CHECK(testString.rfind("") == 11);
    CHECK(testString.rfind("", 5) == 5);
    CHECK(testString.rfind("", 0) == 0);
  }

  SECTION("Rfind in empty string") {
    constexpr FixString<32> testString("");

    CHECK(testString.rfind(FixString<16>("Hello")) == FixString<32>::npos);
    CHECK(testString.rfind(std::string("Hello")) == FixString<32>::npos);
    CHECK(testString.rfind("Hello") == FixString<32>::npos);
    CHECK(testString.rfind('H') == FixString<32>::npos);
    CHECK(testString.rfind("") == 0);
  }

  SECTION("Rfind substring at end") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.rfind("World") == 6);
    CHECK(testString.rfind("d") == 10);
    CHECK(testString.rfind("ld") == 9);
  }

  SECTION("Rfind substring at beginning") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.rfind("Hello") == 12);
    CHECK(testString.rfind("H") == 12);
    CHECK(testString.rfind("He") == 12);
  }

  SECTION("Rfind overlapping substrings") {
    constexpr FixString<32> testString("ababab");

    CHECK(testString.rfind("ab") == 4);
    CHECK(testString.rfind("ab", 3) == 2);
    CHECK(testString.rfind("ab", 1) == 0);
    CHECK(testString.rfind("ab", 0) == 0);
  }

  SECTION("Rfind with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    CHECK(testString.rfind("aa") == 3);
    CHECK(testString.rfind("aa", 2) == 2);
    CHECK(testString.rfind("aa", 1) == 1);
    CHECK(testString.rfind("aa", 0) == 0);
  }

  SECTION("Rfind case sensitivity") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.rfind("hello") == FixString<32>::npos);
    CHECK(testString.rfind("WORLD") == FixString<32>::npos);
    CHECK(testString.rfind("Hello") == 12);
    CHECK(testString.rfind("World") == 6);
  }

  SECTION("Rfind with different FixString capacities") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.rfind(FixString<8>("Hello")) == 12);
    CHECK(testString.rfind(FixString<16>("Hello")) == 12);
    CHECK(testString.rfind(FixString<64>("Hello")) == 12);
  }

  SECTION("Rfind with exact match") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.rfind("Hello") == 0);
    CHECK(testString.rfind("Hello", 0) == 0);
  }

  SECTION("Rfind with single character string") {
    constexpr FixString<32> testString("A");

    CHECK(testString.rfind("A") == 0);
    CHECK(testString.rfind('A') == 0);
    CHECK(testString.rfind("B") == FixString<32>::npos);
    CHECK(testString.rfind('B') == FixString<32>::npos);
  }

  SECTION("Rfind with position 0") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.rfind("Hello", 0) == 0);
    CHECK(testString.rfind("World", 0) == FixString<32>::npos);
    CHECK(testString.rfind('H', 0) == 0);
    CHECK(testString.rfind('W', 0) == FixString<32>::npos);
  }

  SECTION("Rfind with substring longer than string") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.rfind("Hello World") == FixString<32>::npos);
    CHECK(testString.rfind("Hello World", 10) == FixString<32>::npos);
  }

  SECTION("Rfind with multiple occurrences") {
    constexpr FixString<32> testString("abababab");

    CHECK(testString.rfind("ab") == 6);
    CHECK(testString.rfind("ab", 5) == 4);
    CHECK(testString.rfind("ab", 3) == 2);
    CHECK(testString.rfind("ab", 1) == 0);
  }

  SECTION("Rfind with position in middle") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.rfind("Hello", 8) == 0);
    CHECK(testString.rfind("Hello", 12) == 12);
    CHECK(testString.rfind('l', 8) == 3);
    CHECK(testString.rfind('l', 15) == 15);
  }
}

TEST_CASE("FixString find_first_of", "[core][fixstring]") {
  SECTION("Find first of FixString characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of(FixString<16>("aeiou")) == 1); // 'e' at position 1
    CHECK(testString.find_first_of(FixString<16>("H")) == 0);
    CHECK(testString.find_first_of(FixString<16>("d")) == 10);
    CHECK(testString.find_first_of(FixString<16>("xyz")) == FixString<32>::npos);
  }

  SECTION("Find first of StringLike characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of(std::string("aeiou")) == 1);
    CHECK(testString.find_first_of(std::string("H")) == 0);
    CHECK(testString.find_first_of(std::string("d")) == 10);
    CHECK(testString.find_first_of(std::string("xyz")) == FixString<32>::npos);
  }

  SECTION("Find first of C string characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of("aeiou") == 1);
    CHECK(testString.find_first_of("H") == 0);
    CHECK(testString.find_first_of("d") == 10);
    CHECK(testString.find_first_of("xyz") == FixString<32>::npos);
  }

  SECTION("Find first of single character") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of('H') == 0);
    CHECK(testString.find_first_of('e') == 1);
    CHECK(testString.find_first_of('l') == 2);
    CHECK(testString.find_first_of('o') == 4);
    CHECK(testString.find_first_of('W') == 6);
    CHECK(testString.find_first_of('d') == 10);
    CHECK(testString.find_first_of('x') == FixString<32>::npos);
  }

  SECTION("Find first of with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.find_first_of("aeiou", 0) == 1);
    CHECK(testString.find_first_of("aeiou", 2) == 4);
    CHECK(testString.find_first_of("aeiou", 5) == 7);
    CHECK(testString.find_first_of("aeiou", 8) == 13);
    CHECK(testString.find_first_of("aeiou", 14) == 16);
    CHECK(testString.find_first_of("aeiou", 17) == FixString<32>::npos);
  }

  SECTION("Find first of empty character set") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of(FixString<16>("")) == FixString<32>::npos);
    CHECK(testString.find_first_of(std::string("")) == FixString<32>::npos);
    CHECK(testString.find_first_of("") == FixString<32>::npos);
  }

  SECTION("Find first of in empty string") {
    constexpr FixString<32> testString("");

    CHECK(testString.find_first_of(FixString<16>("aeiou")) == FixString<32>::npos);
    CHECK(testString.find_first_of(std::string("aeiou")) == FixString<32>::npos);
    CHECK(testString.find_first_of("aeiou") == FixString<32>::npos);
    CHECK(testString.find_first_of('a') == FixString<32>::npos);
  }

  SECTION("Find first of with position beyond string size") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.find_first_of("aeiou", 10) == FixString<32>::npos);
    CHECK(testString.find_first_of('a', 10) == FixString<32>::npos);
  }

  SECTION("Find first of with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    CHECK(testString.find_first_of("a") == 0);
    CHECK(testString.find_first_of("ab") == 0);
    CHECK(testString.find_first_of("b") == FixString<32>::npos);
  }

  SECTION("Find first of with multiple character sets") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of("Hl") == 0); // 'H' at position 0
    CHECK(testString.find_first_of("lo") == 2); // 'l' at position 2
    CHECK(testString.find_first_of("Wr") == 6); // 'W' at position 6
    CHECK(testString.find_first_of("dl") == 2); // 'l' at position 2
  }

  SECTION("Find first of case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of("h") == FixString<32>::npos);
    CHECK(testString.find_first_of("H") == 0);
    CHECK(testString.find_first_of("w") == FixString<32>::npos);
    CHECK(testString.find_first_of("W") == 6);
  }

  SECTION("Find first of with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    CHECK(testString.find_first_of("!,") == 5); // ',' at position 5
    CHECK(testString.find_first_of("!") == 12); // '!' at position 12
    CHECK(testString.find_first_of(".,!") == 5); // ',' at position 5
  }

  SECTION("Find first of with numbers") {
    constexpr FixString<32> testString("Hello123World");

    CHECK(testString.find_first_of("0123456789") == 5); // '1' at position 5
    CHECK(testString.find_first_of("123") == 5); // '1' at position 5
    CHECK(testString.find_first_of("456") == FixString<32>::npos);
  }

  SECTION("Find first of with whitespace") {
    constexpr FixString<32> testString("Hello World\t\n");

    CHECK(testString.find_first_of(" \t\n") == 5); // ' ' at position 5
    CHECK(testString.find_first_of("\t") == 11);
    CHECK(testString.find_first_of("\n") == 12);
  }

  SECTION("Find first of with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of(FixString<8>("aeiou")) == 1);
    CHECK(testString.find_first_of(FixString<16>("aeiou")) == 1);
    CHECK(testString.find_first_of(FixString<64>("aeiou")) == 1);
  }

  SECTION("Find first of with single character string") {
    constexpr FixString<32> testString("A");

    CHECK(testString.find_first_of("A") == 0);
    CHECK(testString.find_first_of('A') == 0);
    CHECK(testString.find_first_of("B") == FixString<32>::npos);
    CHECK(testString.find_first_of('B') == FixString<32>::npos);
  }

  SECTION("Find first of with position 0") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of("aeiou", 0) == 1);
    CHECK(testString.find_first_of("H", 0) == 0);
    CHECK(testString.find_first_of("xyz", 0) == FixString<32>::npos);
  }

  SECTION("Find first of with all characters found") {
    constexpr FixString<32> testString("abcdefghijklmnopqrstuvwxyz");

    CHECK(testString.find_first_of("aeiou") == 0);
    CHECK(testString.find_first_of("xyz") == 23);
    CHECK(testString.find_first_of("z") == 25);
  }

  SECTION("Find first of with no characters found") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_of("0123456789") == FixString<32>::npos);
    CHECK(testString.find_first_of("!@#$%^&*()") == FixString<32>::npos);
    CHECK(testString.find_first_of("[]{}|\\:;\"'<>?/") == FixString<32>::npos);
  }
}

TEST_CASE("FixString find_first_not_of", "[core][fixstring]") {
  SECTION("Find first not of FixString characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_not_of(FixString<16>("H")) == 1); // 'e' at position 1
    CHECK(testString.find_first_not_of(FixString<16>("Hel")) == 4); // 'o' at position 4
    CHECK(testString.find_first_not_of(FixString<16>("Helo Wrd")) == FixString<32>::npos);
  }

  SECTION("Find first not of StringLike characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_not_of(std::string("H")) == 1); // 'e' at position 1
    CHECK(testString.find_first_not_of(std::string("Hel")) == 4); // 'o' at position 4
    CHECK(testString.find_first_not_of(std::string("Helo Wrd")) == FixString<32>::npos);
  }

  SECTION("Find first not of C string characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_not_of("H") == 1); // 'e' at position 1
    CHECK(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    CHECK(testString.find_first_not_of("Helo Wrd") == FixString<32>::npos);
  }

  SECTION("Find first not of single character") {
    constexpr FixString<32> testString("aaaaab");

    CHECK(testString.find_first_not_of('a') == 5); // 'b' at position 5
    CHECK(testString.find_first_not_of('b') == 0); // 'a' at position 0
    CHECK(testString.find_first_not_of('x') == 0); // 'a' at position 0
  }

  SECTION("Find first not of with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.find_first_not_of("Hel", 0) == 4); // 'o' at position 4
    CHECK(testString.find_first_not_of("Hel", 4) == 4); // 'o' at position 4
    CHECK(testString.find_first_not_of("Hel", 5) == 5); // ' ' at position 5
    CHECK(testString.find_first_not_of("Hel", 6) == 6); // 'W' at position 6
    CHECK(testString.find_first_not_of("Hel", 7) == 7); // 'o' at position 7
  }

  SECTION("Find first not of empty character set") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_not_of(FixString<16>("")) == 0); // 'H' at position 0
    CHECK(testString.find_first_not_of(std::string("")) == 0); // 'H' at position 0
    CHECK(testString.find_first_not_of("") == 0); // 'H' at position 0
    CHECK(testString.find_first_not_of("", 5) == 5); // ' ' at position 5
  }

  SECTION("Find first not of in empty string") {
    constexpr FixString<32> testString("");

    CHECK(testString.find_first_not_of(FixString<16>("aeiou")) == FixString<32>::npos);
    CHECK(testString.find_first_not_of(std::string("aeiou")) == FixString<32>::npos);
    CHECK(testString.find_first_not_of("aeiou") == FixString<32>::npos);
    CHECK(testString.find_first_not_of('a') == FixString<32>::npos);
  }

  SECTION("Find first not of with position beyond string size") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.find_first_not_of("aeiou", 10) == FixString<32>::npos);
    CHECK(testString.find_first_not_of('a', 10) == FixString<32>::npos);
  }

  SECTION("Find first not of with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    CHECK(testString.find_first_not_of("a") == FixString<32>::npos);
    CHECK(testString.find_first_not_of("ab") == FixString<32>::npos);
    CHECK(testString.find_first_not_of("b") == 0); // 'a' at position 0
  }

  SECTION("Find first not of with multiple character sets") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_not_of("Hl") == 1); // 'e' at position 1
    CHECK(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
    CHECK(testString.find_first_not_of("Helo") == 5); // ' ' at position 5
    CHECK(testString.find_first_not_of("Helo ") == 6); // 'W' at position 6
  }

  SECTION("Find first not of case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_not_of("h") == 0); // 'H' at position 0
    CHECK(testString.find_first_not_of("H") == 1); // 'e' at position 1
    CHECK(testString.find_first_not_of("w") == 0); // 'H' at position 0
    CHECK(testString.find_first_not_of("W") == 0); // 'H' at position 0
  }

  SECTION("Find first not of with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    CHECK(testString.find_first_not_of("Helo, Wrd!") == FixString<32>::npos);
    CHECK(testString.find_first_not_of("Helo, Wrd") == 12); // '!' at position 12
  }

  SECTION("Find first not of with numbers") {
    constexpr FixString<32> testString("Hello123World");

    CHECK(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    CHECK(testString.find_first_not_of("Helo123Wrd") == FixString<32>::npos);
    CHECK(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12
  }

  SECTION("Find first not of with whitespace") {
    constexpr FixString<32> testString("Hello World\t\n");

    CHECK(testString.find_first_not_of(" \t\n") == 0); // 'H' at position 0
    CHECK(testString.find_first_not_of("Helo Wrd\t\n") == FixString<32>::npos);
    CHECK(testString.find_first_not_of("Helo Wrd") == 11); // '\t' at position 11
  }

  SECTION("Find first not of with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_not_of(FixString<8>("H")) == 1);
    CHECK(testString.find_first_not_of(FixString<16>("H")) == 1);
    CHECK(testString.find_first_not_of(FixString<64>("H")) == 1);
  }

  SECTION("Find first not of with single character string") {
    constexpr FixString<32> testString("A");

    CHECK(testString.find_first_not_of("A") == FixString<32>::npos);
    CHECK(testString.find_first_not_of('A') == FixString<32>::npos);
    CHECK(testString.find_first_not_of("B") == 0); // 'A' at position 0
    CHECK(testString.find_first_not_of('B') == 0); // 'A' at position 0
  }

  SECTION("Find first not of with position 0") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_not_of("H", 0) == 1);
    CHECK(testString.find_first_not_of("Hel", 0) == 4);
    CHECK(testString.find_first_not_of("Helo Wrd", 0) == FixString<32>::npos);
  }

  SECTION("Find first not of with all characters excluded") {
    constexpr FixString<32> testString("abcdefghijklmnopqrstuvwxyz");

    CHECK(testString.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == FixString<32>::npos);
    CHECK(testString.find_first_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    CHECK(testString.find_first_not_of("abcdefghijklmnopqrstuvwx") == 24); // 'y' at position 24
  }

  SECTION("Find first not of with no characters excluded") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_first_not_of("xyz") == 0); // 'H' at position 0
    CHECK(testString.find_first_not_of("0123456789") == 0); // 'H' at position 0
    CHECK(testString.find_first_not_of("!@#$%^&*()") == 0); // 'H' at position 0
  }

  SECTION("Find first not of with mixed content") {
    constexpr FixString<32> testString("Hello123World");

    CHECK(testString.find_first_not_of("Helo123Wrd") == FixString<32>::npos);
    CHECK(testString.find_first_not_of("Helo123Wr") == 12); // 'd' at position 12
    CHECK(testString.find_first_not_of("Helo123Wd") == 10); // 'r' at position 10
  }

  SECTION("Find first not of with position in middle") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.find_first_not_of("Hel", 4) == 4); // 'o' at position 4
    CHECK(testString.find_first_not_of("Hel", 5) == 5); // ' ' at position 5
    CHECK(testString.find_first_not_of("Hel", 6) == 6); // 'W' at position 6
    CHECK(testString.find_first_not_of("Hel", 7) == 7); // 'o' at position 7
  }

  SECTION("Find first not of with exact match") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.find_first_not_of("Hello") == FixString<32>::npos);
    CHECK(testString.find_first_not_of("Hell") == 4); // 'o' at position 4
    CHECK(testString.find_first_not_of("Hel") == 4); // 'o' at position 4
  }
}

TEST_CASE("FixString find_last_of", "[core][fixstring]") {
  SECTION("Find last of FixString characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_of(FixString<16>("aeiou")) == 7); // 'o' at position 7
    CHECK(testString.find_last_of(FixString<16>("l")) == 9); // 'l' at position 9
    CHECK(testString.find_last_of(FixString<16>("H")) == 0); // 'H' at position 0
    CHECK(testString.find_last_of(FixString<16>("d")) == 10); // 'd' at position 10
  }

  SECTION("Find last of StringLike characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_of(std::string("aeiou")) == 7); // 'o' at position 7
    CHECK(testString.find_last_of(std::string("l")) == 9); // 'l' at position 9
    CHECK(testString.find_last_of(std::string("H")) == 0); // 'H' at position 0
    CHECK(testString.find_last_of(std::string("d")) == 10); // 'd' at position 10
  }

  SECTION("Find last of C string characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_of("aeiou") == 7); // 'o' at position 7
    CHECK(testString.find_last_of("l") == 9); // 'l' at position 9
    CHECK(testString.find_last_of("H") == 0); // 'H' at position 0
    CHECK(testString.find_last_of("d") == 10); // 'd' at position 10
  }

  SECTION("Find last of single character") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_of('l') == 9); // 'l' at position 9
    CHECK(testString.find_last_of('o') == 7); // 'o' at position 7
    CHECK(testString.find_last_of('H') == 0); // 'H' at position 0
    CHECK(testString.find_last_of('d') == 10); // 'd' at position 10
  }

  SECTION("Find last of with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    CHECK(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    CHECK(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    CHECK(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1
    CHECK(testString.find_last_of("Hel", 0) == 0); // 'H' at position 0
  }

  SECTION("Find last of empty character set") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_of(FixString<16>("")) == FixString<32>::npos);
    CHECK(testString.find_last_of(std::string("")) == FixString<32>::npos);
    CHECK(testString.find_last_of("") == FixString<32>::npos);
  }

  SECTION("Find last of with no characters found") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_of("0123456789") == FixString<32>::npos);
    CHECK(testString.find_last_of("!@#$%^&*()") == FixString<32>::npos);
    CHECK(testString.find_last_of("[]{}|\\:;\"'<>?/") == FixString<32>::npos);
  }

  SECTION("Find last of with multiple character sets") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_of("Hl") == 9); // 'l' at position 9
    CHECK(testString.find_last_of("Hel") == 9); // 'l' at position 9
    CHECK(testString.find_last_of("Helo") == 9); // 'l' at position 9
    CHECK(testString.find_last_of("Helo ") == 9); // 'l' at position 9
  }

  SECTION("Find last of case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_of("h") == FixString<32>::npos);
    CHECK(testString.find_last_of("H") == 0); // 'H' at position 0
    CHECK(testString.find_last_of("w") == FixString<32>::npos);
    CHECK(testString.find_last_of("W") == 6); // 'W' at position 6
  }

  SECTION("Find last of with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    CHECK(testString.find_last_of("Helo, Wrd!") == 12); // '!' at position 12
    CHECK(testString.find_last_of("Helo, Wrd") == 11); // 'd' at position 11
    CHECK(testString.find_last_of("Helo, Wr") == 10); // 'l' at position 10
  }

  SECTION("Find last of with numbers") {
    constexpr FixString<32> testString("Hello123World");

    CHECK(testString.find_last_of("0123456789") == 7); // '3' at position 7
    CHECK(testString.find_last_of("Helo123Wrd") == 12); // 'd' at position 12
    CHECK(testString.find_last_of("Helo123Wr") == 11); // 'l' at position 11
  }

  SECTION("Find last of with whitespace") {
    constexpr FixString<32> testString("Hello World\t\n");

    CHECK(testString.find_last_of(" \t\n") == 12); // '\n' at position 12
    CHECK(testString.find_last_of("Helo Wrd\t\n") == 12); // '\n' at position 12
    CHECK(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10
  }

  SECTION("Find last of with repeated characters") {
    constexpr FixString<32> testString("aaaaab");

    CHECK(testString.find_last_of('a') == 4); // 'a' at position 4
    CHECK(testString.find_last_of('b') == 5); // 'b' at position 5
    CHECK(testString.find_last_of('x') == FixString<32>::npos);
  }

  SECTION("Find last of with single character string") {
    constexpr FixString<32> testString("AAAAA");

    CHECK(testString.find_last_of("A") == 4); // 'A' at position 4
    CHECK(testString.find_last_of('A') == 4); // 'A' at position 4
    CHECK(testString.find_last_of("B") == FixString<32>::npos);
    CHECK(testString.find_last_of('B') == FixString<32>::npos);
  }

  SECTION("Find last of with alphabet") {
    constexpr FixString<32> testString("abcdefghijklmnopqrstuvwxyz");

    CHECK(testString.find_last_of("abcdefghijklmnopqrstuvwxyz") == 25); // 'z' at position 25
    CHECK(testString.find_last_of("abcdefghijklmnopqrstuvwxy") == 24); // 'y' at position 24
    CHECK(testString.find_last_of("abcdefghijklmnopqrstuvwx") == 23); // 'x' at position 23
  }

  SECTION("Find last of with all characters found") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_of("Helo Wrd") == 10); // 'd' at position 10
    CHECK(testString.find_last_of("Helo Wr") == 9); // 'l' at position 9
    CHECK(testString.find_last_of("Helo W") == 9); // 'l' at position 9
  }

  SECTION("Find last of with position in middle") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.find_last_of("Hel", 8) == 3); // 'l' at position 3
    CHECK(testString.find_last_of("Hel", 4) == 3); // 'l' at position 3
    CHECK(testString.find_last_of("Hel", 2) == 2); // 'l' at position 2
    CHECK(testString.find_last_of("Hel", 1) == 1); // 'e' at position 1
  }

  SECTION("Find last of with exact match") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.find_last_of("Hello") == 4); // 'o' at position 4
    CHECK(testString.find_last_of("Hell") == 3); // 'l' at position 3
    CHECK(testString.find_last_of("Hel") == 3); // 'l' at position 3
  }
}

TEST_CASE("FixString find_last_not_of", "[core][fixstring]") {
  SECTION("Find last not of FixString characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_not_of(FixString<16>("d")) == 9); // 'l' at position 9
    CHECK(testString.find_last_not_of(FixString<16>("ld")) == 8); // 'r' at position 8
    CHECK(testString.find_last_not_of(FixString<16>("rld")) == 7); // 'o' at position 7
    CHECK(testString.find_last_not_of(FixString<16>("World")) == 5); // ' ' at position 5
  }

  SECTION("Find last not of StringLike characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_not_of(std::string("d")) == 9); // 'l' at position 9
    CHECK(testString.find_last_not_of(std::string("ld")) == 8); // 'r' at position 8
    CHECK(testString.find_last_not_of(std::string("rld")) == 7); // 'o' at position 7
    CHECK(testString.find_last_not_of(std::string("World")) == 5); // ' ' at position 5
  }

  SECTION("Find last not of C string characters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_not_of("d") == 9); // 'l' at position 9
    CHECK(testString.find_last_not_of("ld") == 8); // 'r' at position 8
    CHECK(testString.find_last_not_of("rld") == 7); // 'o' at position 7
    CHECK(testString.find_last_not_of("World") == 5); // ' ' at position 5
  }

  SECTION("Find last not of single character") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_not_of('d') == 9); // 'l' at position 9
    CHECK(testString.find_last_not_of('l') == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of('o') == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of('H') == 10); // 'd' at position 10
  }

  SECTION("Find last not of with position parameter") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    CHECK(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    CHECK(testString.find_last_not_of("Hel", 2) == FixString<32>::npos);
    CHECK(testString.find_last_not_of("Hel", 1) == FixString<32>::npos);
    CHECK(testString.find_last_not_of("Hel", 0) == FixString<32>::npos);
  }

  SECTION("Find last not of empty character set") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_not_of(FixString<16>("")) == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of(std::string("")) == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("", 5) == 5); // ' ' at position 5
  }

  SECTION("Find last not of with all characters excluded") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_not_of("Helo Wrd") == FixString<32>::npos);
    CHECK(testString.find_last_not_of("Helo Wr") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("Helo W") == 10); // 'd' at position 10
  }

  SECTION("Find last not of with multiple character sets") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_not_of("Hl") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("Hel") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("Helo") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("Helo ") == 10); // 'd' at position 10
  }

  SECTION("Find last not of case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_not_of("h") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("H") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("w") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("W") == 10); // 'd' at position 10
  }

  SECTION("Find last not of with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    CHECK(testString.find_last_not_of("Helo, Wrd!") == FixString<32>::npos);
    CHECK(testString.find_last_not_of("Helo, Wrd") == 12); // '!' at position 12
    CHECK(testString.find_last_not_of("Helo, Wr") == 12); // '!' at position 12
  }

  SECTION("Find last not of with numbers") {
    constexpr FixString<32> testString("Hello123World");

    CHECK(testString.find_last_not_of("0123456789") == 12); // 'd' at position 12
    CHECK(testString.find_last_not_of("Helo123Wrd") == FixString<32>::npos);
    CHECK(testString.find_last_not_of("Helo123Wr") == 12); // 'd' at position 12
  }

  SECTION("Find last not of with whitespace") {
    constexpr FixString<32> testString("Hello World\t\n");

    CHECK(testString.find_last_not_of(" \t\n") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("Helo Wrd\t\n") == FixString<32>::npos);
    CHECK(testString.find_last_not_of("Helo Wrd") == 12); // '\n' at position 12
  }

  SECTION("Find last not of with repeated characters") {
    constexpr FixString<32> testString("aaaaab");

    CHECK(testString.find_last_not_of('a') == 5); // 'b' at position 5
    CHECK(testString.find_last_not_of('b') == 4); // 'a' at position 4
    CHECK(testString.find_last_not_of('x') == 5); // 'b' at position 5
  }

  SECTION("Find last not of with single character string") {
    constexpr FixString<32> testString("AAAAA");

    CHECK(testString.find_last_not_of("A") == FixString<32>::npos);
    CHECK(testString.find_last_not_of('A') == FixString<32>::npos);
    CHECK(testString.find_last_not_of("B") == 4); // 'A' at position 4
    CHECK(testString.find_last_not_of('B') == 4); // 'A' at position 4
  }

  SECTION("Find last not of with alphabet") {
    constexpr FixString<32> testString("abcdefghijklmnopqrstuvwxyz");

    CHECK(testString.find_last_not_of("abcdefghijklmnopqrstuvwxyz") == FixString<32>::npos);
    CHECK(testString.find_last_not_of("abcdefghijklmnopqrstuvwxy") == 25); // 'z' at position 25
    CHECK(testString.find_last_not_of("abcdefghijklmnopqrstuvwx") == 25); // 'z' at position 25
  }

  SECTION("Find last not of with no characters excluded") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.find_last_not_of("xyz") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("0123456789") == 10); // 'd' at position 10
    CHECK(testString.find_last_not_of("!@#$%^&*()") == 10); // 'd' at position 10
  }

  SECTION("Find last not of with position in middle") {
    constexpr FixString<32> testString("Hello World Hello");

    CHECK(testString.find_last_not_of("Hel", 8) == 8); // 'r' at position 8
    CHECK(testString.find_last_not_of("Hel", 4) == 4); // 'o' at position 4
    CHECK(testString.find_last_not_of("Hel", 2) == FixString<32>::npos);
    CHECK(testString.find_last_not_of("Hel", 1) == FixString<32>::npos);
  }

  SECTION("Find last not of with exact match") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.find_last_not_of("Hello") == FixString<32>::npos);
    CHECK(testString.find_last_not_of("Hell") == 4); // 'o' at position 4
    CHECK(testString.find_last_not_of("Hel") == 4); // 'o' at position 4
  }
}

TEST_CASE("FixString compare", "[core][fixstring]") {
  SECTION("Compare FixString with FixString") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hello");
    constexpr FixString<32> testString3("World");
    constexpr FixString<32> testString4("Hell");

    CHECK(testString1.compare(testString2) == 0);
    CHECK(testString1.compare(testString3) < 0);
    CHECK(testString1.compare(testString4) > 0);
  }

  SECTION("Compare FixString with StringLike") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.compare(std::string("Hello")) == 0);
    CHECK(testString.compare(std::string("World")) < 0);
    CHECK(testString.compare(std::string("Hell")) > 0);
  }

  SECTION("Compare FixString with C string") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.compare("Hello") == 0);
    CHECK(testString.compare("World") < 0);
    CHECK(testString.compare("Hell") > 0);
  }

  SECTION("Compare identical strings") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.compare(FixString<32>("Hello World")) == 0);
    CHECK(testString.compare(std::string("Hello World")) == 0);
    CHECK(testString.compare("Hello World") == 0);
  }

  SECTION("Compare with empty strings") {
    constexpr FixString<32> testString1("");
    constexpr FixString<32> testString2("Hello");

    CHECK(testString1.compare(testString2) < 0);
    CHECK(testString2.compare(testString1) > 0);
    CHECK(testString1.compare("") == 0);
    CHECK(testString1.compare(std::string("")) == 0);
  }

  SECTION("Compare strings with different lengths") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hello World");

    CHECK(testString1.compare(testString2) < 0);
    CHECK(testString2.compare(testString1) > 0);
  }

  SECTION("Compare strings with same prefix") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hell");

    CHECK(testString1.compare(testString2) > 0);
    CHECK(testString2.compare(testString1) < 0);
  }

  SECTION("Compare strings with different first character") {
    constexpr FixString<32> testString1("Apple");
    constexpr FixString<32> testString2("Banana");

    CHECK(testString1.compare(testString2) < 0);
    CHECK(testString2.compare(testString1) > 0);
  }

  SECTION("Compare strings with different middle character") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hallo");

    CHECK(testString1.compare(testString2) > 0);
    CHECK(testString2.compare(testString1) < 0);
  }

  SECTION("Compare strings with different last character") {
    constexpr FixString<32> testString1("Hello");
    constexpr FixString<32> testString2("Hellp");

    CHECK(testString1.compare(testString2) < 0);
    CHECK(testString2.compare(testString1) > 0);
  }

  SECTION("Compare case sensitivity") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.compare("hello") < 0);
    CHECK(testString.compare("HELLO") > 0);
    CHECK(testString.compare("Hello") == 0);
  }

  SECTION("Compare with different FixString capacities") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.compare(FixString<8>("Hello")) == 0);
    CHECK(testString.compare(FixString<16>("Hello")) == 0);
    CHECK(testString.compare(FixString<64>("Hello")) == 0);
    CHECK(testString.compare(FixString<8>("World")) < 0);
    CHECK(testString.compare(FixString<16>("Hell")) > 0);
  }

  SECTION("Compare with single character strings") {
    constexpr FixString<32> testString1("A");
    constexpr FixString<32> testString2("B");

    CHECK(testString1.compare(testString2) < 0);
    CHECK(testString2.compare(testString1) > 0);
    CHECK(testString1.compare("A") == 0);
    CHECK(testString1.compare("B") < 0);
  }

  SECTION("Compare with repeated characters") {
    constexpr FixString<32> testString1("aaa");
    constexpr FixString<32> testString2("aa");

    CHECK(testString1.compare(testString2) > 0);
    CHECK(testString2.compare(testString1) < 0);
    CHECK(testString1.compare("aaa") == 0);
    CHECK(testString1.compare("aa") > 0);
  }

  SECTION("Compare with special characters") {
    constexpr FixString<32> testString1("Hello!");
    constexpr FixString<32> testString2("Hello");

    CHECK(testString1.compare(testString2) > 0);
    CHECK(testString2.compare(testString1) < 0);
    CHECK(testString1.compare("Hello!") == 0);
    CHECK(testString1.compare("Hello") > 0);
  }

  SECTION("Compare with numbers") {
    constexpr FixString<32> testString1("123");
    constexpr FixString<32> testString2("456");

    CHECK(testString1.compare(testString2) < 0);
    CHECK(testString2.compare(testString1) > 0);
    CHECK(testString1.compare("123") == 0);
    CHECK(testString1.compare("456") < 0);
  }

  SECTION("Compare with mixed content") {
    constexpr FixString<32> testString1("Hello123");
    constexpr FixString<32> testString2("Hello456");

    CHECK(testString1.compare(testString2) < 0);
    CHECK(testString2.compare(testString1) > 0);
    CHECK(testString1.compare("Hello123") == 0);
    CHECK(testString1.compare("Hello456") < 0);
  }

  SECTION("Compare with maximum length strings") {
    constexpr FixString<16> testString1("123456789012345");
    constexpr FixString<16> testString2("123456789012346");

    CHECK(testString1.compare(testString2) < 0);
    CHECK(testString2.compare(testString1) > 0);
    CHECK(testString1.compare("123456789012345") == 0);
    CHECK(testString1.compare("123456789012346") < 0);
  }

  SECTION("Compare with std::string") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.compare(std::string("Hello World")) == 0);
    CHECK(testString.compare(std::string("Hello")) > 0);
    CHECK(testString.compare(std::string("World")) < 0);
  }

  SECTION("Compare with array") {
    constexpr FixString<32> testString("Hello");
    constexpr std::array<char, 6> arr = {'H', 'e', 'l', 'l', 'o', '\0'};

    CHECK(testString.compare(arr.data()) == 0);
    CHECK(testString.compare("Hello") == 0);
  }

  SECTION("Compare edge cases") {
    constexpr FixString<32> testString("Hello");

    // Compare with null-terminated string
    CHECK(testString.compare("Hello\0World") == 0);

    // Compare with string containing null character
    constexpr FixString<32> testStringWithNull("Hello\0World");
    CHECK(testString.compare(testStringWithNull) == 0);
  }
}

TEST_CASE("FixString starts_with", "[core][fixstring]") {
  SECTION("Starts with FixString") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.starts_with(FixString<16>("Hello")) == true);
    CHECK(testString.starts_with(FixString<16>("Hello World")) == true);
    CHECK(testString.starts_with(FixString<16>("H")) == true);
    CHECK(testString.starts_with(FixString<16>("World")) == false);
    CHECK(testString.starts_with(FixString<16>("xyz")) == false);
    CHECK(testString.starts_with(FixString<16>("")) == true);
  }

  SECTION("Starts with StringLike") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.starts_with(std::string("Hello")) == true);
    CHECK(testString.starts_with(std::string("Hello World")) == true);
    CHECK(testString.starts_with(std::string("H")) == true);
    CHECK(testString.starts_with(std::string("World")) == false);
    CHECK(testString.starts_with(std::string("xyz")) == false);
    CHECK(testString.starts_with(std::string("")) == true);
  }

  SECTION("Starts with C string") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.starts_with("Hello") == true);
    CHECK(testString.starts_with("Hello World") == true);
    CHECK(testString.starts_with("H") == true);
    CHECK(testString.starts_with("World") == false);
    CHECK(testString.starts_with("xyz") == false);
    CHECK(testString.starts_with("") == true);
  }

  SECTION("Starts with character") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.starts_with('H') == true);
    CHECK(testString.starts_with('h') == false);
    CHECK(testString.starts_with('W') == false);
    CHECK(testString.starts_with('x') == false);
  }

  SECTION("Starts with empty string") {
    constexpr FixString<32> testString("");

    CHECK(testString.starts_with(FixString<16>("Hello")) == false);
    CHECK(testString.starts_with(std::string("Hello")) == false);
    CHECK(testString.starts_with("Hello") == false);
    CHECK(testString.starts_with('H') == false);
    CHECK(testString.starts_with("") == true);
  }

  SECTION("Starts with single character string") {
    constexpr FixString<32> testString("A");

    CHECK(testString.starts_with("A") == true);
    CHECK(testString.starts_with('A') == true);
    CHECK(testString.starts_with("B") == false);
    CHECK(testString.starts_with('B') == false);
    CHECK(testString.starts_with("") == true);
  }

  SECTION("Starts with longer prefix") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.starts_with("Hello World") == false);
    CHECK(testString.starts_with("Hello Universe") == false);
    CHECK(testString.starts_with("Hello") == true);
    CHECK(testString.starts_with("Hell") == true);
  }

  SECTION("Starts with case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.starts_with("hello") == false);
    CHECK(testString.starts_with("HELLO") == false);
    CHECK(testString.starts_with("Hello") == true);
    CHECK(testString.starts_with('h') == false);
    CHECK(testString.starts_with('H') == true);
  }

  SECTION("Starts with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.starts_with(FixString<8>("Hello")) == true);
    CHECK(testString.starts_with(FixString<16>("Hello")) == true);
    CHECK(testString.starts_with(FixString<64>("Hello")) == true);
    CHECK(testString.starts_with(FixString<8>("World")) == false);
  }

  SECTION("Starts with repeated characters") {
    constexpr FixString<32> testString("aaaab");

    CHECK(testString.starts_with("aaa") == true);
    CHECK(testString.starts_with("aaaa") == true);
    CHECK(testString.starts_with("aaaab") == true);
    CHECK(testString.starts_with("aaab") == false);
    CHECK(testString.starts_with('a') == true);
    CHECK(testString.starts_with('b') == false);
  }

  SECTION("Starts with special characters") {
    constexpr FixString<32> testString("!@#$%");

    CHECK(testString.starts_with("!@#") == true);
    CHECK(testString.starts_with("!@#$%") == true);
    CHECK(testString.starts_with("!@#$%^") == false);
    CHECK(testString.starts_with('!') == true);
    CHECK(testString.starts_with('@') == false);
  }

  SECTION("Starts with numbers") {
    constexpr FixString<32> testString("12345");

    CHECK(testString.starts_with("123") == true);
    CHECK(testString.starts_with("12345") == true);
    CHECK(testString.starts_with("123456") == false);
    CHECK(testString.starts_with('1') == true);
    CHECK(testString.starts_with('2') == false);
  }

  SECTION("Starts with mixed content") {
    constexpr FixString<32> testString("Hello123");

    CHECK(testString.starts_with("Hello") == true);
    CHECK(testString.starts_with("Hello1") == true);
    CHECK(testString.starts_with("Hello123") == true);
    CHECK(testString.starts_with("Hello1234") == false);
    CHECK(testString.starts_with('H') == true);
    CHECK(testString.starts_with('1') == false);
  }

  SECTION("Starts with maximum length strings") {
    constexpr FixString<16> testString("123456789012345");

    CHECK(testString.starts_with("123456789012345") == true);
    CHECK(testString.starts_with("12345678901234") == true);
    CHECK(testString.starts_with("1234567890123456") == false);
    CHECK(testString.starts_with('1') == true);
    CHECK(testString.starts_with('5') == false);
  }

  SECTION("Starts with std::string") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.starts_with(std::string("Hello")) == true);
    CHECK(testString.starts_with(std::string("Hello World")) == true);
    CHECK(testString.starts_with(std::string("World")) == false);
  }

  SECTION("Starts with array") {
    constexpr FixString<32> testString("Hello");
    constexpr std::array<char, 4> arr = {'H', 'e', 'l', '\0'};

    CHECK(testString.starts_with(arr.data()) == true);
    CHECK(testString.starts_with("Hel") == true);
  }

  SECTION("Starts with edge cases") {
    constexpr FixString<32> testString("Hello");

    // Test with null-terminated string
    CHECK(testString.starts_with("Hello\0World") == true);

    // Test with string containing null character
    constexpr FixString<32> testStringWithNull("Hello\0World");
    CHECK(testStringWithNull.starts_with("Hello") == true);
  }

  SECTION("Starts with whitespace") {
    constexpr FixString<32> testString(" Hello World");

    CHECK(testString.starts_with(" ") == true);
    CHECK(testString.starts_with(" Hello") == true);
    CHECK(testString.starts_with("Hello") == false);
    CHECK(testString.starts_with(' ') == true);
    CHECK(testString.starts_with('H') == false);
  }

  SECTION("Starts with exact match") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.starts_with("Hello") == true);
    CHECK(testString.starts_with("Hell") == true);
    CHECK(testString.starts_with("H") == true);
    CHECK(testString.starts_with("") == true);
  }
}

TEST_CASE("FixString ends_with", "[core][fixstring]") {
  SECTION("Ends with FixString") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.ends_with(FixString<16>("World")) == true);
    CHECK(testString.ends_with(FixString<16>("Hello World")) == true);
    CHECK(testString.ends_with(FixString<16>("d")) == true);
    CHECK(testString.ends_with(FixString<16>("Hello")) == false);
    CHECK(testString.ends_with(FixString<16>("xyz")) == false);
    CHECK(testString.ends_with(FixString<16>("")) == true);
  }

  SECTION("Ends with StringLike") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.ends_with(std::string("World")) == true);
    CHECK(testString.ends_with(std::string("Hello World")) == true);
    CHECK(testString.ends_with(std::string("d")) == true);
    CHECK(testString.ends_with(std::string("Hello")) == false);
    CHECK(testString.ends_with(std::string("xyz")) == false);
    CHECK(testString.ends_with(std::string("")) == true);
  }

  SECTION("Ends with C string") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.ends_with("World") == true);
    CHECK(testString.ends_with("Hello World") == true);
    CHECK(testString.ends_with("d") == true);
    CHECK(testString.ends_with("Hello") == false);
    CHECK(testString.ends_with("xyz") == false);
    CHECK(testString.ends_with("") == true);
  }

  SECTION("Ends with character") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.ends_with('d') == true);
    CHECK(testString.ends_with('D') == false);
    CHECK(testString.ends_with('H') == false);
    CHECK(testString.ends_with('x') == false);
  }

  SECTION("Ends with empty string") {
    constexpr FixString<32> testString("");

    CHECK(testString.ends_with(FixString<16>("Hello")) == false);
    CHECK(testString.ends_with(std::string("Hello")) == false);
    CHECK(testString.ends_with("Hello") == false);
    CHECK(testString.ends_with('H') == false);
    CHECK(testString.ends_with("") == true);
  }

  SECTION("Ends with single character string") {
    constexpr FixString<32> testString("A");

    CHECK(testString.ends_with("A") == true);
    CHECK(testString.ends_with('A') == true);
    CHECK(testString.ends_with("B") == false);
    CHECK(testString.ends_with('B') == false);
    CHECK(testString.ends_with("") == true);
  }

  SECTION("Ends with longer suffix") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.ends_with("Hello World") == false);
    CHECK(testString.ends_with("Hello Universe") == false);
    CHECK(testString.ends_with("Hello") == true);
    CHECK(testString.ends_with("llo") == true);
  }

  SECTION("Ends with case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.ends_with("world") == false);
    CHECK(testString.ends_with("WORLD") == false);
    CHECK(testString.ends_with("World") == true);
    CHECK(testString.ends_with('d') == true);
    CHECK(testString.ends_with('D') == false);
  }

  SECTION("Ends with different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.ends_with(FixString<8>("World")) == true);
    CHECK(testString.ends_with(FixString<16>("World")) == true);
    CHECK(testString.ends_with(FixString<64>("World")) == true);
    CHECK(testString.ends_with(FixString<8>("Hello")) == false);
  }

  SECTION("Ends with repeated characters") {
    constexpr FixString<32> testString("baaaa");

    CHECK(testString.ends_with("aaa") == true);
    CHECK(testString.ends_with("aaaa") == true);
    CHECK(testString.ends_with("baaaa") == true);
    CHECK(testString.ends_with("aaaaa") == false);
    CHECK(testString.ends_with('a') == true);
    CHECK(testString.ends_with('b') == false);
  }

  SECTION("Ends with special characters") {
    constexpr FixString<32> testString("%$#@!");

    CHECK(testString.ends_with("@!") == true);
    CHECK(testString.ends_with("%$#@!") == true);
    CHECK(testString.ends_with("^%$#@!") == false);
    CHECK(testString.ends_with('!') == true);
    CHECK(testString.ends_with('@') == false);
  }

  SECTION("Ends with numbers") {
    constexpr FixString<32> testString("54321");

    CHECK(testString.ends_with("321") == true);
    CHECK(testString.ends_with("54321") == true);
    CHECK(testString.ends_with("654321") == false);
    CHECK(testString.ends_with('1') == true);
    CHECK(testString.ends_with('2') == false);
  }

  SECTION("Ends with mixed content") {
    constexpr FixString<32> testString("123Hello");

    CHECK(testString.ends_with("Hello") == true);
    CHECK(testString.ends_with("3Hello") == true);
    CHECK(testString.ends_with("123Hello") == true);
    CHECK(testString.ends_with("0123Hello") == false);
    CHECK(testString.ends_with('o') == true);
    CHECK(testString.ends_with('1') == false);
  }

  SECTION("Ends with maximum length strings") {
    constexpr FixString<16> testString("123456789012345");

    CHECK(testString.ends_with("123456789012345") == true);
    CHECK(testString.ends_with("23456789012345") == true);
    CHECK(testString.ends_with("0123456789012345") == false);
    CHECK(testString.ends_with('5') == true);
    CHECK(testString.ends_with('1') == false);
  }

  SECTION("Ends with std::string") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.ends_with(std::string("World")) == true);
    CHECK(testString.ends_with(std::string("Hello World")) == true);
    CHECK(testString.ends_with(std::string("Hello")) == false);
  }

  SECTION("Ends with array") {
    constexpr FixString<32> testString("Hello");
    constexpr std::array<char, 4> arr = {'l', 'l', 'o', '\0'};

    CHECK(testString.ends_with(arr.data()) == true);
    CHECK(testString.ends_with("llo") == true);
  }

  SECTION("Ends with edge cases") {
    constexpr FixString<32> testString("Hello");

    // Test with null-terminated string
    CHECK(testString.ends_with("Hello\0World") == true);

    // Test with string containing null character
    constexpr FixString<32> testStringWithNull("Hello\0World");
    CHECK(testStringWithNull.ends_with("World") == false);
  }

  SECTION("Ends with whitespace") {
    constexpr FixString<32> testString("Hello World ");

    CHECK(testString.ends_with(" ") == true);
    CHECK(testString.ends_with("World ") == true);
    CHECK(testString.ends_with("World") == false);
    CHECK(testString.ends_with(' ') == true);
    CHECK(testString.ends_with('d') == false);
  }

  SECTION("Ends with exact match") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.ends_with("Hello") == true);
    CHECK(testString.ends_with("llo") == true);
    CHECK(testString.ends_with("o") == true);
    CHECK(testString.ends_with("") == true);
  }

  SECTION("Ends with overlapping patterns") {
    constexpr FixString<32> testString("ababab");

    CHECK(testString.ends_with("ab") == true);
    CHECK(testString.ends_with("bab") == true);
    CHECK(testString.ends_with("abab") == true);
    CHECK(testString.ends_with("babab") == true);
    CHECK(testString.ends_with("ababab") == true);
    CHECK(testString.ends_with("bababab") == false);
  }

  SECTION("Ends with multiple occurrences") {
    constexpr FixString<32> testString("abababab");

    CHECK(testString.ends_with("ab") == true);
    CHECK(testString.ends_with("bab") == true);
    CHECK(testString.ends_with("abab") == true);
    CHECK(testString.ends_with("ababab") == true);
    CHECK(testString.ends_with("abababab") == true);
  }
}

TEST_CASE("FixString contains", "[core][fixstring]") {
  SECTION("Contains FixString") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains(FixString<16>("World")) == true);
    CHECK(testString.contains(FixString<16>("Hello")) == true);
    CHECK(testString.contains(FixString<16>("lo Wo")) == true);
    CHECK(testString.contains(FixString<16>("Hello World")) == true);
    CHECK(testString.contains(FixString<16>("xyz")) == false);
    CHECK(testString.contains(FixString<16>("")) == true);
  }

  SECTION("Contains StringLike") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains(std::string("World")) == true);
    CHECK(testString.contains(std::string("Hello")) == true);
    CHECK(testString.contains(std::string("lo Wo")) == true);
    CHECK(testString.contains(std::string("Hello World")) == true);
    CHECK(testString.contains(std::string("xyz")) == false);
    CHECK(testString.contains(std::string("")) == true);
  }

  SECTION("Contains C string") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains("World") == true);
    CHECK(testString.contains("Hello") == true);
    CHECK(testString.contains("lo Wo") == true);
    CHECK(testString.contains("Hello World") == true);
    CHECK(testString.contains("xyz") == false);
    CHECK(testString.contains("") == true);
  }

  SECTION("Contains character") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains('H') == true);
    CHECK(testString.contains('o') == true);
    CHECK(testString.contains('l') == true);
    CHECK(testString.contains(' ') == true);
    CHECK(testString.contains('x') == false);
    CHECK(testString.contains('Z') == false);
  }

  SECTION("Contains in empty string") {
    constexpr FixString<32> testString("");

    CHECK(testString.contains(FixString<16>("Hello")) == false);
    CHECK(testString.contains(std::string("Hello")) == false);
    CHECK(testString.contains("Hello") == false);
    CHECK(testString.contains('H') == false);
    CHECK(testString.contains("") == true);
  }

  SECTION("Contains in single character string") {
    constexpr FixString<32> testString("A");

    CHECK(testString.contains("A") == true);
    CHECK(testString.contains('A') == true);
    CHECK(testString.contains("B") == false);
    CHECK(testString.contains('B') == false);
    CHECK(testString.contains("") == true);
  }

  SECTION("Contains longer substring") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.contains("Hello World") == false);
    CHECK(testString.contains("Hello Universe") == false);
    CHECK(testString.contains("Hello") == true);
    CHECK(testString.contains("llo") == true);
    CHECK(testString.contains("ell") == true);
  }

  SECTION("Contains case sensitivity") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains("world") == false);
    CHECK(testString.contains("WORLD") == false);
    CHECK(testString.contains("World") == true);
    CHECK(testString.contains('h') == false);
    CHECK(testString.contains('H') == true);
  }

  SECTION("Contains different FixString capacities") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains(FixString<8>("World")) == true);
    CHECK(testString.contains(FixString<16>("World")) == true);
    CHECK(testString.contains(FixString<64>("World")) == true);
    CHECK(testString.contains(FixString<8>("Hello")) == true);
  }

  SECTION("Contains repeated characters") {
    constexpr FixString<32> testString("baaaa");

    CHECK(testString.contains("aaa") == true);
    CHECK(testString.contains("aaaa") == true);
    CHECK(testString.contains("baaaa") == true);
    CHECK(testString.contains("aaaaa") == false);
    CHECK(testString.contains('a') == true);
    CHECK(testString.contains('b') == true);
  }

  SECTION("Contains special characters") {
    constexpr FixString<32> testString("%$#@!");

    CHECK(testString.contains("@!") == true);
    CHECK(testString.contains("%$#@!") == true);
    CHECK(testString.contains("^%$#@!") == false);
    CHECK(testString.contains('!') == true);
    CHECK(testString.contains('@') == true);
    CHECK(testString.contains('$') == true);
  }

  SECTION("Contains numbers") {
    constexpr FixString<32> testString("54321");

    CHECK(testString.contains("321") == true);
    CHECK(testString.contains("54321") == true);
    CHECK(testString.contains("654321") == false);
    CHECK(testString.contains('1') == true);
    CHECK(testString.contains('2') == true);
    CHECK(testString.contains('5') == true);
  }

  SECTION("Contains mixed content") {
    constexpr FixString<32> testString("123Hello");

    CHECK(testString.contains("Hello") == true);
    CHECK(testString.contains("3Hello") == true);
    CHECK(testString.contains("123Hello") == true);
    CHECK(testString.contains("0123Hello") == false);
    CHECK(testString.contains('o') == true);
    CHECK(testString.contains('1') == true);
    CHECK(testString.contains('3') == true);
  }

  SECTION("Contains maximum length strings") {
    constexpr FixString<16> testString("123456789012345"); // 15 characters

    CHECK(testString.contains("123456789012345") == true);
    CHECK(testString.contains("23456789012345") == true);
    CHECK(testString.contains("0123456789012345") == false);
    CHECK(testString.contains('5') == true);
    CHECK(testString.contains('1') == true);
  }

  SECTION("Contains std::string") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains(std::string("World")) == true);
    CHECK(testString.contains(std::string("Hello World")) == true);
    CHECK(testString.contains(std::string("Hello")) == true);
  }

  SECTION("Contains array") {
    constexpr FixString<32> testString("Hello");
    constexpr std::array<char, 4> arr = {'l', 'l', 'o', '\0'};

    CHECK(testString.contains(arr.data()) == true);
    CHECK(testString.contains("llo") == true);
  }

  SECTION("Contains whitespace") {
    constexpr FixString<32> testString("Hello World ");

    CHECK(testString.contains(" ") == true);
    CHECK(testString.contains("World ") == true);
    CHECK(testString.contains("World") == true);
    CHECK(testString.contains(' ') == true);
    CHECK(testString.contains('d') == true);
  }

  SECTION("Contains exact match") {
    constexpr FixString<32> testString("Hello");

    CHECK(testString.contains("Hello") == true);
    CHECK(testString.contains("llo") == true);
    CHECK(testString.contains("o") == true);
    CHECK(testString.contains("") == true);
  }

  SECTION("Contains overlapping patterns") {
    constexpr FixString<32> testString("ababab");

    CHECK(testString.contains("ab") == true);
    CHECK(testString.contains("bab") == true);
    CHECK(testString.contains("abab") == true);
    CHECK(testString.contains("ababab") == true);
    CHECK(testString.contains("babab") == true);
  }

  SECTION("Contains multiple occurrences") {
    constexpr FixString<32> testString("abababab");

    CHECK(testString.contains("ab") == true);
    CHECK(testString.contains("bab") == true);
    CHECK(testString.contains("abab") == true);
    CHECK(testString.contains("ababab") == true);
    CHECK(testString.contains("abababab") == true);
  }

  SECTION("Contains at beginning") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains("H") == true);
    CHECK(testString.contains("He") == true);
    CHECK(testString.contains("Hel") == true);
    CHECK(testString.contains("Hello") == true);
  }

  SECTION("Contains at end") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains("d") == true);
    CHECK(testString.contains("ld") == true);
    CHECK(testString.contains("rld") == true);
    CHECK(testString.contains("World") == true);
  }

  SECTION("Contains in middle") {
    constexpr FixString<32> testString("Hello World");

    CHECK(testString.contains("l") == true);
    CHECK(testString.contains("ll") == true);
    CHECK(testString.contains("lo") == true);
    CHECK(testString.contains("lo W") == true);
  }

  SECTION("Contains single character multiple times") {
    constexpr FixString<32> testString("aaaaa");

    CHECK(testString.contains("a") == true);
    CHECK(testString.contains("aa") == true);
    CHECK(testString.contains("aaa") == true);
    CHECK(testString.contains("aaaa") == true);
    CHECK(testString.contains("aaaaa") == true);
    CHECK(testString.contains("aaaaaa") == false);
  }

  SECTION("Contains with punctuation") {
    constexpr FixString<32> testString("Hello, World!");

    CHECK(testString.contains(",") == true);
    CHECK(testString.contains("!") == true);
    CHECK(testString.contains(", ") == true);
    CHECK(testString.contains("World!") == true);
    CHECK(testString.contains("Hello,") == true);
  }

  SECTION("Contains with newlines") {
    constexpr FixString<32> testString("Hello\nWorld");

    CHECK(testString.contains("\n") == true);
    CHECK(testString.contains("Hello\n") == true);
    CHECK(testString.contains("\nWorld") == true);
    CHECK(testString.contains("Hello\nWorld") == true);
  }

  SECTION("Contains with tabs") {
    constexpr FixString<32> testString("Hello\tWorld");

    CHECK(testString.contains("\t") == true);
    CHECK(testString.contains("Hello\t") == true);
    CHECK(testString.contains("\tWorld") == true);
    CHECK(testString.contains("Hello\tWorld") == true);
  }
}

TEST_CASE("FixString substr", "[core][fixstring]") {
  SECTION("Substr basic functionality") {
    constexpr FixString<32> testString("Hello World");

    CHECK(strcmp(testString.substr(0).c_str(), "Hello World") == 0);
    CHECK(strcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    CHECK(strcmp(testString.substr(6).c_str(), "World") == 0);
    CHECK(strcmp(testString.substr(6, 5).c_str(), "World") == 0);
    CHECK(strcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    CHECK(strcmp(testString.substr(0, 0).c_str(), "") == 0);
  }

  SECTION("Substr with default parameters") {
    constexpr FixString<32> testString("Hello World");

    CHECK(strcmp(testString.substr().c_str(), "Hello World") == 0);
    CHECK(strcmp(testString.substr(0).c_str(), "Hello World") == 0);
    CHECK(strcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SECTION("Substr with count parameter") {
    constexpr FixString<32> testString("Hello World");

    CHECK(strcmp(testString.substr(0, 1).c_str(), "H") == 0);
    CHECK(strcmp(testString.substr(0, 2).c_str(), "He") == 0);
    CHECK(strcmp(testString.substr(0, 3).c_str(), "Hel") == 0);
    CHECK(strcmp(testString.substr(0, 4).c_str(), "Hell") == 0);
    CHECK(strcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    CHECK(strcmp(testString.substr(6, 1).c_str(), "W") == 0);
    CHECK(strcmp(testString.substr(6, 2).c_str(), "Wo") == 0);
    CHECK(strcmp(testString.substr(6, 3).c_str(), "Wor") == 0);
    CHECK(strcmp(testString.substr(6, 4).c_str(), "Worl") == 0);
    CHECK(strcmp(testString.substr(6, 5).c_str(), "World") == 0);
  }

  SECTION("Substr with npos count") {
    constexpr FixString<32> testString("Hello World");

    CHECK(strcmp(testString.substr(0, FixString<32>::npos).c_str(), "Hello World") == 0);
    CHECK(strcmp(testString.substr(6, FixString<32>::npos).c_str(), "World") == 0);
    CHECK(strcmp(testString.substr(10, FixString<32>::npos).c_str(), "d") == 0);
  }

  SECTION("Substr with position at end") {
    constexpr FixString<32> testString("Hello World");

    CHECK(strcmp(testString.substr(11).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(11, 0).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(11, 5).c_str(), "") == 0);
  }

  SECTION("Substr from empty string") {
    constexpr FixString<32> testString("");

    CHECK(strcmp(testString.substr(0).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(0, 0).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(0, 5).c_str(), "") == 0);
  }

  SECTION("Substr single character") {
    constexpr FixString<32> testString("A");

    CHECK(strcmp(testString.substr(0).c_str(), "A") == 0);
    CHECK(strcmp(testString.substr(0, 1).c_str(), "A") == 0);
    CHECK(strcmp(testString.substr(0, 0).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(1).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(1, 1).c_str(), "") == 0);
  }

  SECTION("Substr with repeated characters") {
    constexpr FixString<32> testString("aaaaa");

    CHECK(strcmp(testString.substr(0).c_str(), "aaaaa") == 0);
    CHECK(strcmp(testString.substr(0, 1).c_str(), "a") == 0);
    CHECK(strcmp(testString.substr(0, 2).c_str(), "aa") == 0);
    CHECK(strcmp(testString.substr(0, 3).c_str(), "aaa") == 0);
    CHECK(strcmp(testString.substr(0, 4).c_str(), "aaaa") == 0);
    CHECK(strcmp(testString.substr(0, 5).c_str(), "aaaaa") == 0);
    CHECK(strcmp(testString.substr(1).c_str(), "aaaa") == 0);
    CHECK(strcmp(testString.substr(2).c_str(), "aaa") == 0);
    CHECK(strcmp(testString.substr(3).c_str(), "aa") == 0);
    CHECK(strcmp(testString.substr(4).c_str(), "a") == 0);
    CHECK(strcmp(testString.substr(5).c_str(), "") == 0);
  }

  SECTION("Substr with special characters") {
    constexpr FixString<32> testString("Hello, World!");

    CHECK(strcmp(testString.substr(5).c_str(), ", World!") == 0);
    CHECK(strcmp(testString.substr(5, 1).c_str(), ",") == 0);
    CHECK(strcmp(testString.substr(5, 2).c_str(), ", ") == 0);
    CHECK(strcmp(testString.substr(12).c_str(), "!") == 0);
    CHECK(strcmp(testString.substr(12, 1).c_str(), "!") == 0);
  }

  SECTION("Substr with numbers") {
    constexpr FixString<32> testString("12345");

    CHECK(strcmp(testString.substr(0).c_str(), "12345") == 0);
    CHECK(strcmp(testString.substr(0, 1).c_str(), "1") == 0);
    CHECK(strcmp(testString.substr(0, 2).c_str(), "12") == 0);
    CHECK(strcmp(testString.substr(0, 3).c_str(), "123") == 0);
    CHECK(strcmp(testString.substr(0, 4).c_str(), "1234") == 0);
    CHECK(strcmp(testString.substr(0, 5).c_str(), "12345") == 0);
    CHECK(strcmp(testString.substr(1).c_str(), "2345") == 0);
    CHECK(strcmp(testString.substr(2).c_str(), "345") == 0);
    CHECK(strcmp(testString.substr(3).c_str(), "45") == 0);
    CHECK(strcmp(testString.substr(4).c_str(), "5") == 0);
    CHECK(strcmp(testString.substr(5).c_str(), "") == 0);
  }

  SECTION("Substr with whitespace") {
    constexpr FixString<32> testString("Hello World");

    CHECK(strcmp(testString.substr(5).c_str(), " World") == 0);
    CHECK(strcmp(testString.substr(5, 1).c_str(), " ") == 0);
    CHECK(strcmp(testString.substr(5, 2).c_str(), " W") == 0);
    CHECK(strcmp(testString.substr(5, 3).c_str(), " Wo") == 0);
  }

  SECTION("Substr with newlines") {
    constexpr FixString<32> testString("Hello\nWorld");

    CHECK(strcmp(testString.substr(5).c_str(), "\nWorld") == 0);
    CHECK(strcmp(testString.substr(5, 1).c_str(), "\n") == 0);
    CHECK(strcmp(testString.substr(5, 2).c_str(), "\nW") == 0);
    CHECK(strcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SECTION("Substr with tabs") {
    constexpr FixString<32> testString("Hello\tWorld");

    CHECK(strcmp(testString.substr(5).c_str(), "\tWorld") == 0);
    CHECK(strcmp(testString.substr(5, 1).c_str(), "\t") == 0);
    CHECK(strcmp(testString.substr(5, 2).c_str(), "\tW") == 0);
    CHECK(strcmp(testString.substr(6).c_str(), "World") == 0);
  }

  SECTION("Substr maximum length") {
    constexpr FixString<16> testString("123456789012345"); // 15 characters

    CHECK(strcmp(testString.substr(0).c_str(), "123456789012345") == 0);
    CHECK(strcmp(testString.substr(0, 15).c_str(), "123456789012345") == 0);
    CHECK(strcmp(testString.substr(0, 16).c_str(), "123456789012345") == 0);
    CHECK(strcmp(testString.substr(14).c_str(), "5") == 0);
    CHECK(strcmp(testString.substr(14, 1).c_str(), "5") == 0);
    CHECK(strcmp(testString.substr(15).c_str(), "") == 0);
  }

  SECTION("Substr with mixed content") {
    constexpr FixString<32> testString("123Hello");

    CHECK(strcmp(testString.substr(0).c_str(), "123Hello") == 0);
    CHECK(strcmp(testString.substr(0, 3).c_str(), "123") == 0);
    CHECK(strcmp(testString.substr(3).c_str(), "Hello") == 0);
    CHECK(strcmp(testString.substr(3, 5).c_str(), "Hello") == 0);
    CHECK(strcmp(testString.substr(2, 4).c_str(), "3Hel") == 0);
  }

  SECTION("Substr with overlapping ranges") {
    constexpr FixString<32> testString("abcdef");

    CHECK(strcmp(testString.substr(0, 1).c_str(), "a") == 0);
    CHECK(strcmp(testString.substr(1, 1).c_str(), "b") == 0);
    CHECK(strcmp(testString.substr(2, 1).c_str(), "c") == 0);
    CHECK(strcmp(testString.substr(3, 1).c_str(), "d") == 0);
    CHECK(strcmp(testString.substr(4, 1).c_str(), "e") == 0);
    CHECK(strcmp(testString.substr(5, 1).c_str(), "f") == 0);
    CHECK(strcmp(testString.substr(0, 2).c_str(), "ab") == 0);
    CHECK(strcmp(testString.substr(1, 2).c_str(), "bc") == 0);
    CHECK(strcmp(testString.substr(2, 2).c_str(), "cd") == 0);
    CHECK(strcmp(testString.substr(3, 2).c_str(), "de") == 0);
    CHECK(strcmp(testString.substr(4, 2).c_str(), "ef") == 0);
  }

  SECTION("Substr with exact string length") {
    constexpr FixString<32> testString("Hello");

    CHECK(strcmp(testString.substr(0, 5).c_str(), "Hello") == 0);
    CHECK(strcmp(testString.substr(0, 6).c_str(), "Hello") == 0);
    CHECK(strcmp(testString.substr(1, 4).c_str(), "ello") == 0);
    CHECK(strcmp(testString.substr(2, 3).c_str(), "llo") == 0);
    CHECK(strcmp(testString.substr(3, 2).c_str(), "lo") == 0);
    CHECK(strcmp(testString.substr(4, 1).c_str(), "o") == 0);
  }

  SECTION("Substr with zero count") {
    constexpr FixString<32> testString("Hello World");

    CHECK(strcmp(testString.substr(0, 0).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(5, 0).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(10, 0).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(11, 0).c_str(), "") == 0);
  }

  SECTION("Substr with position at string size") {
    constexpr FixString<32> testString("Hello");

    CHECK(strcmp(testString.substr(5).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(5, 0).c_str(), "") == 0);
    CHECK(strcmp(testString.substr(5, 1).c_str(), "") == 0);
  }

  SECTION("Substr with different FixString capacities") {
    constexpr FixString<8> smallString("Hello");
    constexpr FixString<16> mediumString("Hello World");
    constexpr FixString<32> largeString("Hello World Universe");

    CHECK(strcmp(smallString.substr(0, 3).c_str(), "Hel") == 0);
    CHECK(strcmp(mediumString.substr(0, 5).c_str(), "Hello") == 0);
    CHECK(strcmp(largeString.substr(0, 5).c_str(), "Hello") == 0);
    CHECK(strcmp(smallString.substr(2).c_str(), "llo") == 0);
    CHECK(strcmp(mediumString.substr(6).c_str(), "World") == 0);
    CHECK(strcmp(largeString.substr(12).c_str(), "Universe") == 0);
  }

  SECTION("Substr with punctuation") {
    constexpr FixString<32> testString("Hello, World!");

    CHECK(strcmp(testString.substr(5, 1).c_str(), ",") == 0);
    CHECK(strcmp(testString.substr(6, 1).c_str(), " ") == 0);
    CHECK(strcmp(testString.substr(12, 1).c_str(), "!") == 0);
    CHECK(strcmp(testString.substr(5, 3).c_str(), ", W") == 0);
    CHECK(strcmp(testString.substr(11, 2).c_str(), "d!") == 0);
  }

  SECTION("Substr with unicode-like content") {
    constexpr FixString<32> testString("Hello 世界");

    CHECK(strcmp(testString.substr(0, 6).c_str(), "Hello ") == 0);
    CHECK(strcmp(testString.substr(6).c_str(), "世界") == 0);
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

  CHECK(strcmp(testString1.c_str(), "12test text 1") == 0);
  CHECK(testString1.size() == 13);

  CHECK(strcmp(testString2.c_str(), "12test text 2") == 0);
  CHECK(testString2.size() == 13);

  CHECK(strcmp(testString3.c_str(), "12test text 3") == 0);
  CHECK(testString3.size() == 13);

  CHECK(strcmp(testString4.c_str(), "12test text 4") == 0);
  CHECK(testString4.size() == 13);

  CHECK(strcmp(testString5.c_str(), "12t") == 0);
  CHECK(testString5.size() == 3);

  CHECK(strcmp(testString6.c_str(), "abc") == 0);
  CHECK(testString6.size() == 3);

  CHECK(strcmp(testString7.c_str(), "abcabc") == 0);
  CHECK(testString7.size() == 6);
}
