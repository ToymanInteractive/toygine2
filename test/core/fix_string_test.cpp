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

TEST_CASE("FixString utf8Size", "[core][fixstring]") {
  static char const ansiText[] = "ToyGine2 - Free 2D/3D game engine.";
  // UTF8 encoding               "ToyGine2 - Бесплатный 2D/3D игровой движок.";

  static constexpr std::array<std::uint8_t, 67> utf8Text{
    {0x54, 0x6F, 0x79, 0x47, 0x69, 0x6E, 0x65, 0x32, 0x20, 0x2D, 0x20, 0xD0, 0x91, 0xD0, 0xB5, 0xD1, 0x81,
     0xD0, 0xBF, 0xD0, 0xBB, 0xD0, 0xB0, 0xD1, 0x82, 0xD0, 0xBD, 0xD1, 0x8B, 0xD0, 0xB9, 0x20, 0x32, 0x44,
     0x2F, 0x33, 0x44, 0x20, 0xD0, 0xB8, 0xD0, 0xB3, 0xD1, 0x80, 0xD0, 0xBE, 0xD0, 0xB2, 0xD0, 0xB8, 0xD0,
     0xB9, 0x20, 0xD0, 0xB4, 0xD0, 0xB2, 0xD0, 0xB8, 0xD0, 0xB6, 0xD0, 0xBE, 0xD0, 0xBA, 0x2E, 0x00}};

  const auto testString1 = FixString<64>(ansiText);
  const auto testString2 = FixString<80>(reinterpret_cast<char const *>(utf8Text.data()));
  FixString<96> const testString3;

  CHECK(testString1.size() == testString1.utf8Size());
  CHECK(testString3.size() == testString3.utf8Size());
  CHECK(testString2.size() == 66);
  CHECK(testString2.utf8Size() == 43);
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

TEST_CASE("FixString insert methods", "[core][fixstring]") {
  SECTION("Insert FixString at index") {
    FixString<32> testString("Hello World");

    // Insert at beginning
    testString.insert(0, FixString<16>("Hi "));
    CHECK(strcmp(testString.c_str(), "Hi Hello World") == 0);
    CHECK(testString.size() == 14);

    // Insert in middle
    testString.insert(6, FixString<8>("Beautiful "));
    CHECK(strcmp(testString.c_str(), "Hi Hello Beautiful World") == 0);
    CHECK(testString.size() == 25);

    // Insert at end
    testString.insert(25, FixString<8>("!"));
    CHECK(strcmp(testString.c_str(), "Hi Hello Beautiful World!") == 0);
    CHECK(testString.size() == 26);
  }

  SECTION("Insert FixString with different allocated size") {
    FixString<32> testString("Hello World");

    // Insert smaller string
    testString.insert(6, FixString<8>("Beautiful "));
    CHECK(strcmp(testString.c_str(), "Hello Beautiful World") == 0);
    CHECK(testString.size() == 22);

    // Insert larger string (should be truncated)
    testString.insert(0, FixString<64>("Very Long Prefix That Should Be Truncated "));
    CHECK(strcmp(testString.c_str(), "Very Long Prefix That Should Be Truncated Hello Beautiful World") == 0);
    CHECK(testString.size() == 31); // Max capacity - 1
  }

  SECTION("Insert C string at index") {
    FixString<32> testString("Hello World");

    // Insert at beginning
    testString.insert(0, "Hi ");
    CHECK(strcmp(testString.c_str(), "Hi Hello World") == 0);
    CHECK(testString.size() == 14);

    // Insert in middle
    testString.insert(6, "Beautiful ");
    CHECK(strcmp(testString.c_str(), "Hi Hello Beautiful World") == 0);
    CHECK(testString.size() == 25);

    // Insert at end
    testString.insert(25, "!");
    CHECK(strcmp(testString.c_str(), "Hi Hello Beautiful World!") == 0);
    CHECK(testString.size() == 26);

    // Insert empty string
    testString.insert(0, "");
    CHECK(strcmp(testString.c_str(), "Hi Hello Beautiful World!") == 0);
    CHECK(testString.size() == 26);
  }

  SECTION("Insert character repeated count times") {
    FixString<32> testString("Hello World");

    // Insert single character
    testString.insert(5, ' ', 1);
    CHECK(strcmp(testString.c_str(), "Hello  World") == 0);
    CHECK(testString.size() == 12);

    // Insert multiple characters
    testString.insert(0, '*', 3);
    CHECK(strcmp(testString.c_str(), "***Hello  World") == 0);
    CHECK(testString.size() == 15);

    // Insert at end
    testString.insert(15, '!', 2);
    CHECK(strcmp(testString.c_str(), "***Hello  World!!") == 0);
    CHECK(testString.size() == 17);

    // Insert zero characters
    testString.insert(0, 'X', 0);
    CHECK(strcmp(testString.c_str(), "***Hello  World!!") == 0);
    CHECK(testString.size() == 17);
  }
  /*

SECTION("Insert with bounds checking") {
FixString<16> testString("Hello");

// Insert at valid index
testString.insert(5, " World");
CHECK(strcmp(testString.c_str(), "Hello World") == 0);
CHECK(testString.size() == 11);

// Insert at end
testString.insert(11, "!");
CHECK(strcmp(testString.c_str(), "Hello World!") == 0);
CHECK(testString.size() == 12);

// Insert at index 0
testString.insert(0, "Hi ");
CHECK(strcmp(testString.c_str(), "Hi Hello World!") == 0);
CHECK(testString.size() == 15);
}

SECTION("Insert with capacity limits") {
FixString<12> testString("Hello");

// Insert within capacity
testString.insert(5, " World");
CHECK(strcmp(testString.c_str(), "Hello World") == 0);
CHECK(testString.size() == 11);

// Try to insert beyond capacity (should be truncated)
testString.insert(0, "Very Long Prefix ");
CHECK(strcmp(testString.c_str(), "Very Long Prefix Hello World") == 0);
CHECK(testString.size() == 11); // Max capacity - 1
}

SECTION("Insert edge cases") {
FixString<20> testString;

// Insert into empty string
testString.insert(0, "Hello");
CHECK(strcmp(testString.c_str(), "Hello") == 0);
CHECK(testString.size() == 5);

// Insert at index 0 of non-empty string
testString.insert(0, "Hi ");
CHECK(strcmp(testString.c_str(), "Hi Hello") == 0);
CHECK(testString.size() == 8);

// Insert at current size position
testString.insert(8, " World");
CHECK(strcmp(testString.c_str(), "Hi Hello World") == 0);
CHECK(testString.size() == 14);
}

SECTION("Insert with special characters") {
FixString<32> testString("Hello");

// Insert with spaces
testString.insert(5, "   ");
CHECK(strcmp(testString.c_str(), "Hello   ") == 0);
CHECK(testString.size() == 8);

// Insert with newlines
testString.insert(0, "\n");
CHECK(strcmp(testString.c_str(), "\nHello   ") == 0);
CHECK(testString.size() == 9);

// Insert with tabs
testString.insert(9, "\tWorld");
CHECK(strcmp(testString.c_str(), "\nHello   \tWorld") == 0);
CHECK(testString.size() == 15);
}

SECTION("Insert performance and correctness") {
FixString<64> testString("Start");

// Multiple insertions
testString.insert(5, " Middle");
CHECK(strcmp(testString.c_str(), "Start Middle") == 0);

testString.insert(0, "Very ");
CHECK(strcmp(testString.c_str(), "Very Start Middle") == 0);

testString.insert(15, " End");
CHECK(strcmp(testString.c_str(), "Very Start Middle End") == 0);

testString.insert(5, "Long ");
CHECK(strcmp(testString.c_str(), "Very Long Start Middle End") == 0);

// Verify final result
CHECK(testString.size() == 29);
CHECK(strcmp(testString.c_str(), "Very Long Start Middle End") == 0);
}
*/
}

TEST_CASE("FixString operators+=", "[core][fixstring]") {
  FixString<14> testString1("12");
  FixString<14> testString2("12");
  FixString<26> testString3("12");
  FixString<20> testString4("12");
  FixString<4> testString5("12");

  testString1 += "test text 1";
  testString2 += FixString<14>("test text 1");
  testString3 += FixString<14>("test text 1");
  testString4 += FixString<26>("test text 1");
  testString5 += 't';

  CHECK(strcmp(testString1.c_str(), "12test text 1") == 0);
  CHECK(testString1.size() == 13);

  CHECK(strcmp(testString2.c_str(), "12test text 1") == 0);
  CHECK(testString2.size() == 13);

  CHECK(strcmp(testString3.c_str(), "12test text 1") == 0);
  CHECK(testString3.size() == 13);

  CHECK(strcmp(testString4.c_str(), "12test text 1") == 0);
  CHECK(testString4.size() == 13);

  CHECK(strcmp(testString5.c_str(), "12t") == 0);
  CHECK(testString5.size() == 3);
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
