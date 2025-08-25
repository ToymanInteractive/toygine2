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
     0x2F, 0x33, 0x44, 0x20, 0xD0, 0xB8, 0xD0, 0xB3, 0xD1, 0x80, 0xD0, 0xBE, 0xD0, 0xB2, 0xD0, 0xBE, 0xD0,
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

TEST_CASE("FixString append", "[core][fixstring]") {
  FixString<32> testString("");

  testString.append(FixString<16>("Nothing"))
    .append(FixString<8>(" else"))
    .append(std::string(" really"))
    .append(std::string(" matters"))
    .append('.', 3);

  CHECK(strcmp(testString.c_str(), "Nothing else really matters...") == 0);
  CHECK(testString.size() == 30);
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
