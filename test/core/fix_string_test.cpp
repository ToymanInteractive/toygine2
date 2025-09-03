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

#include <array>

#include <catch2/catch_test_macros.hpp>

#include "core.hpp"

using namespace toygine;

TEST_CASE("FixString constructors", "[core][fixstring]") {
  FixString<12> testString1("test text 1");
  FixString<12> testString2(testString1);
  FixString<24> testString3(testString2);
  FixString<18> testString4(testString3);
  FixString<12> testString5;
  FixString<11> testString6('t', 10);

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
  auto testString1 = FixString<8>("abcd");
  const auto testString2 = FixString<8>("dcba");

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
  auto testString1 = FixString<8>("abcd");
  const auto testString2 = FixString<8>("dcba");

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

TEST_CASE("FixString data", "[core][fixstring]") {
  auto testString1 = FixString<8>("abcd");
  const auto testString2 = FixString<8>("dcba");

  CHECK(strcmp(testString1.data(), "abcd") == 0);
  CHECK(strcmp(testString2.data(), "dcba") == 0);
}

TEST_CASE("FixString c_str", "[core][fixstring]") {
  auto testString1 = FixString<8>("abcd");
  const auto testString2 = FixString<8>("dcba");

  CHECK(strcmp(testString1.c_str(), "abcd") == 0);
  CHECK(strcmp(testString2.c_str(), "dcba") == 0);
}

TEST_CASE("FixString empty", "[core][fixstring]") {
  CHECK(!FixString<16>("ToyGine2").empty());

  CHECK(FixString<4>("").empty());
}

TEST_CASE("FixString size", "[core][fixstring]") {
  const auto testString1 = FixString<64>("ToyGine2 - Free 2D/3D game engine.");
  const FixString<64> testString2;

  CHECK(testString1.size() == 34);
  CHECK(testString2.size() == 0);
}

TEST_CASE("FixString utf8_size", "[core][fixstring]") {
  static char const ansiText[] = "ToyGine2 - Free 2D/3D game engine.";
  // UTF8 encoding               "ToyGine2 - Бесплатный 2D/3D игровой движок.";

  static constexpr std::array<std::uint8_t, 67> utf8Text{
    {0x54, 0x6F, 0x79, 0x47, 0x69, 0x6E, 0x65, 0x32, 0x20, 0x2D, 0x20, 0xD0, 0x91, 0xD0, 0xB5, 0xD1, 0x81,
     0xD0, 0xBF, 0xD0, 0xBB, 0xD0, 0xB0, 0xD1, 0x82, 0xD0, 0xBD, 0xD1, 0x8B, 0xD0, 0xB9, 0x20, 0x32, 0x44,
     0x2F, 0x33, 0x44, 0x20, 0xD0, 0xB8, 0xD0, 0xB3, 0xD1, 0x80, 0xD0, 0xBE, 0xD0, 0xB2, 0xD0, 0xBE, 0xD0,
     0xB9, 0x20, 0xD0, 0xB4, 0xD0, 0xB2, 0xD0, 0xB8, 0xD0, 0xB6, 0xD0, 0xBE, 0xD0, 0xBA, 0x2E, 0x00}};

  const auto testString1 = FixString<64>(ansiText);
  const auto testString2 = FixString<80>(reinterpret_cast<char const *>(utf8Text.data()));
  FixString<96> const testString3;

  CHECK(testString1.size() == testString1.utf8_size());
  CHECK(testString3.size() == testString3.utf8_size());
  CHECK(testString2.size() == 66);
  CHECK(testString2.utf8_size() == 43);
}

TEST_CASE("FixString length", "[core][fixstring]") {
  const auto testString1 = FixString<64>("ToyGine2 - Free 2D/3D game engine.");
  const FixString<64> testString2;

  CHECK(testString1.length() == 34);
  CHECK(testString2.length() == 0);
}

TEST_CASE("FixString max_size", "[core][fixstring]") {
  CHECK(FixString<64>("ToyGine2 - Free 2D/3D game engine.").max_size() == 63);
  CHECK(FixString<16>("").max_size() == 15);
}

TEST_CASE("FixString capacity", "[core][fixstring]") {
  CHECK(FixString<64>("ToyGine2").capacity() == 63);
  CHECK(FixString<16>("").capacity() == 15);
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
    const char * utf8Text = "Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82";
    FixString<32> testString(utf8Text);

    testString.utf8_pop_back(); // Remove 'т' (2 bytes)

    CHECK(strcmp(testString.c_str(), "Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5") == 0);
    CHECK(testString.size() == 16); // "Hello приве" (5 + 1 + 5*2 = 16 bytes)
    CHECK(testString.utf8_size() == 11);
  }

  SECTION("Pop back multiple UTF-8 characters") {
    // "Hello привет" - "привет" contains 6 Cyrillic characters
    const char * utf8Text = "Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82";
    FixString<32> testString(utf8Text);

    testString.utf8_pop_back(); // Remove 'т'
    testString.utf8_pop_back(); // Remove 'е'
    testString.utf8_pop_back(); // Remove 'в'

    CHECK(strcmp(testString.c_str(), "Hello \xD0\xBF\xD1\x80\xD0\xB8") == 0);
    CHECK(testString.size() == 12); // "Hello при" (5 + 1 + 3*2 = 12 bytes)
    CHECK(testString.utf8_size() == 9);
  }

  SECTION("Pop back mixed ASCII and UTF-8") {
    // "Hello привет" - mix of ASCII and Cyrillic
    const char * utf8Text = "Hello \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82";
    FixString<32> testString(utf8Text);

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
    const char * utf8Text = "\xD0\xBF";
    FixString<8> testString(utf8Text);

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
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 11);

    CHECK(copied == 11);
    CHECK(strncmp(buffer, "Hello World", 11) == 0);
  }

  SECTION("Copy partial string from beginning") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 5);

    CHECK(copied == 5);
    CHECK(strncmp(buffer, "Hello", 5) == 0);
  }

  SECTION("Copy partial string from middle") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 5, 6);

    CHECK(copied == 5);
    CHECK(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with npos count") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, FixString<16>::npos);

    CHECK(copied == 11);
    CHECK(strncmp(buffer, "Hello World", 11) == 0);
  }

  SECTION("Copy with count exceeding remaining characters") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 20, 6);

    CHECK(copied == 5);
    CHECK(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy from position 0") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 5, 0);

    CHECK(copied == 5);
    CHECK(strncmp(buffer, "Hello", 5) == 0);
  }

  SECTION("Copy single character") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 1, 6);

    CHECK(copied == 1);
    CHECK(buffer[0] == 'W');
  }

  SECTION("Copy from end position") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 1, 10);

    CHECK(copied == 1);
    CHECK(buffer[0] == 'd');
  }

  SECTION("Copy with zero count") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 0);

    CHECK(copied == 0);
  }

  SECTION("Copy from empty string") {
    FixString<16> testString("");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 5);

    CHECK(copied == 0);
  }

  SECTION("Copy to small buffer") {
    FixString<16> testString("Hello World");
    char buffer[3] = {0};

    const auto copied = testString.copy(buffer, 2);

    CHECK(copied == 2);
    CHECK(strncmp(buffer, "He", 2) == 0);
  }

  SECTION("Copy with position at end") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    const auto copied = testString.copy(buffer, 5, 11);

    CHECK(copied == 0);
  }

  SECTION("Copy with npos from middle") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    std::size_t copied = testString.copy(buffer, FixString<16>::npos, 6);

    CHECK(copied == 5);
    CHECK(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with exact remaining characters") {
    FixString<16> testString("Hello World");
    char buffer[16] = {0};

    std::size_t copied = testString.copy(buffer, 5, 6);

    CHECK(copied == 5);
    CHECK(strncmp(buffer, "World", 5) == 0);
  }

  SECTION("Copy with count larger than string size") {
    FixString<16> testString("Hi");
    char buffer[16] = {0};

    std::size_t copied = testString.copy(buffer, 10);

    CHECK(copied == 2);
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
    FixString<16> string1("123456789012345"); // 15 characters (max for size 16)
    FixString<16> string2("ABCDEFGHIJKLMNO"); // 15 characters (max for size 16)

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
    FixString<32> testString("Hello World");

    CHECK(testString.find(FixString<16>("World")) == 6);
    CHECK(testString.find(FixString<16>("Hello")) == 0);
    CHECK(testString.find(FixString<16>("lo Wo")) == 3);
    CHECK(testString.find(FixString<16>("xyz")) == FixString<32>::npos);
  }

  SECTION("Find StringLike substring") {
    FixString<32> testString("Hello World");

    CHECK(testString.find(std::string("World")) == 6);
    CHECK(testString.find(std::string("Hello")) == 0);
    CHECK(testString.find(std::string("lo Wo")) == 3);
    CHECK(testString.find(std::string("xyz")) == FixString<32>::npos);
  }

  SECTION("Find C string substring") {
    FixString<32> testString("Hello World");

    CHECK(testString.find("World") == 6);
    CHECK(testString.find("Hello") == 0);
    CHECK(testString.find("lo Wo") == 3);
    CHECK(testString.find("xyz") == FixString<32>::npos);
  }

  SECTION("Find character") {
    FixString<32> testString("Hello World");

    CHECK(testString.find('H') == 0);
    CHECK(testString.find('l') == 2);
    CHECK(testString.find('o') == 4);
    CHECK(testString.find('W') == 6);
    CHECK(testString.find('d') == 10);
    CHECK(testString.find('x') == FixString<32>::npos);
  }

  SECTION("Find with position parameter") {
    FixString<32> testString("Hello World Hello");

    CHECK(testString.find("Hello", 0) == 0);
    CHECK(testString.find("Hello", 1) == 12);
    CHECK(testString.find("Hello", 13) == FixString<32>::npos);
    CHECK(testString.find('l', 0) == 2);
    CHECK(testString.find('l', 3) == 3);
    CHECK(testString.find('l', 4) == 9);
    CHECK(testString.find('l', 10) == 14);
  }

  SECTION("Find empty substring") {
    FixString<32> testString("Hello World");

    CHECK(testString.find(FixString<16>("")) == 0);
    CHECK(testString.find(std::string("")) == 0);
    CHECK(testString.find("") == 0);
    CHECK(testString.find("", 5) == 5);
    CHECK(testString.find("", 11) == FixString<32>::npos);
  }

  SECTION("Find in empty string") {
    FixString<32> testString("");

    CHECK(testString.find(FixString<16>("Hello")) == FixString<32>::npos);
    CHECK(testString.find(std::string("Hello")) == FixString<32>::npos);
    CHECK(testString.find("Hello") == FixString<32>::npos);
    CHECK(testString.find('H') == FixString<32>::npos);
    CHECK(testString.find("") == FixString<32>::npos);
  }

  SECTION("Find with position beyond string size") {
    FixString<32> testString("Hello");

    CHECK(testString.find("World", 10) == FixString<32>::npos);
    CHECK(testString.find('H', 10) == FixString<32>::npos);
    CHECK(testString.find("", 10) == FixString<32>::npos);
  }

  SECTION("Find substring at end") {
    FixString<32> testString("Hello World");

    CHECK(testString.find("World") == 6);
    CHECK(testString.find("d") == 10);
    CHECK(testString.find("ld") == 9);
  }

  SECTION("Find substring at beginning") {
    FixString<32> testString("Hello World");

    CHECK(testString.find("Hello") == 0);
    CHECK(testString.find("H") == 0);
    CHECK(testString.find("He") == 0);
  }

  SECTION("Find overlapping substrings") {
    FixString<32> testString("ababab");

    CHECK(testString.find("ab") == 0);
    CHECK(testString.find("ab", 1) == 2);
    CHECK(testString.find("ab", 3) == 4);
    CHECK(testString.find("ab", 5) == FixString<32>::npos);
  }

  SECTION("Find with repeated characters") {
    FixString<32> testString("aaaaa");

    CHECK(testString.find("aa") == 0);
    CHECK(testString.find("aa", 1) == 1);
    CHECK(testString.find("aa", 2) == 2);
    CHECK(testString.find("aa", 3) == 3);
    CHECK(testString.find("aa", 4) == FixString<32>::npos);
  }

  SECTION("Find case sensitivity") {
    FixString<32> testString("Hello World");

    CHECK(testString.find("hello") == FixString<32>::npos);
    CHECK(testString.find("WORLD") == FixString<32>::npos);
    CHECK(testString.find("Hello") == 0);
    CHECK(testString.find("World") == 6);
  }

  SECTION("Find with different FixString capacities") {
    FixString<32> testString("Hello World");

    CHECK(testString.find(FixString<8>("World")) == 6);
    CHECK(testString.find(FixString<16>("World")) == 6);
    CHECK(testString.find(FixString<64>("World")) == 6);
  }

  SECTION("Find with exact match") {
    FixString<32> testString("Hello");

    CHECK(testString.find("Hello") == 0);
    CHECK(testString.find("Hello", 0) == 0);
    CHECK(testString.find("Hello", 1) == FixString<32>::npos);
  }

  SECTION("Find with single character string") {
    FixString<32> testString("A");

    CHECK(testString.find("A") == 0);
    CHECK(testString.find('A') == 0);
    CHECK(testString.find("B") == FixString<32>::npos);
    CHECK(testString.find('B') == FixString<32>::npos);
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
