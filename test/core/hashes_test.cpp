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

#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "core.hpp"

namespace toy {

TEST_CASE("crc8 function", "[core][hashes]") {
  SECTION("Empty data") {
    constexpr CStringView empty("");

    REQUIRE(crc8(empty.c_str(), empty.size()) == 0x00);
  }

  SECTION("Single character") {
    constexpr CStringView single("A");

    REQUIRE(crc8(single.c_str(), single.size()) == 0x18);
  }

  SECTION("Short string") {
    constexpr CStringView shortStr("Hello");

    REQUIRE(crc8(shortStr.c_str(), shortStr.size()) == 0xEB);
  }

  SECTION("Medium string") {
    constexpr CStringView medium("Toygine2 - Free 2D/3D game engine.");

    REQUIRE(crc8(medium.c_str(), medium.size()) == 0x5B);
  }

  SECTION("Long string") {
    constexpr CStringView longStr(
      "This is a very long string that contains multiple words and should test the CRC8 algorithm with a substantial amount of data to process.");

    REQUIRE(crc8(longStr.c_str(), longStr.size()) == 0xAA);
  }

  SECTION("String with special characters") {
    constexpr CStringView special("Hello, World! @#$%^&*()_+-=[]{}|;':\",./<>?");

    REQUIRE(crc8(special.c_str(), special.size()) == 0x77);
  }

  SECTION("String with numbers") {
    constexpr CStringView numbers("1234567890");

    REQUIRE(crc8(numbers.c_str(), numbers.size()) == 0x4F);
  }

  SECTION("String with mixed content") {
    constexpr CStringView mixed("ABC123def456GHI789jkl");

    REQUIRE(crc8(mixed.c_str(), mixed.size()) == 0x23);
  }

  SECTION("Unicode string") {
    constexpr CStringView unicode("Привет, мир! 🌍");

    REQUIRE(crc8(unicode.c_str(), unicode.size()) == 0xDB);
  }

  SECTION("Binary data") {
    constexpr unsigned char binaryData[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE};

    REQUIRE(crc8(binaryData, sizeof(binaryData)) == 0x2C);
  }

  SECTION("All zeros") {
    constexpr unsigned char zeros[] = {0x00, 0x00, 0x00, 0x00, 0x00};

    REQUIRE(crc8(zeros, sizeof(zeros)) == 0x00);
  }

  SECTION("All ones") {
    constexpr unsigned char ones[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    REQUIRE(crc8(ones, sizeof(ones)) == 0x44);
  }

  SECTION("Pattern data") {
    constexpr unsigned char pattern[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};

    REQUIRE(crc8(pattern, sizeof(pattern)) == 0xC5);
  }

  SECTION("Sequential data") {
    constexpr unsigned char sequential[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    REQUIRE(crc8(sequential, sizeof(sequential)) == 0xF2);
  }

  SECTION("Incremental CRC calculation") {
    constexpr CStringView part1("Toygine2 - Free ");
    constexpr CStringView part2("2D/3D game engine.");

    auto crc1 = crc8(part1.c_str(), part1.size());
    auto crc2 = crc8(part2.c_str(), part2.size(), crc1);

    REQUIRE(crc2 == 0x5B);
  }

  SECTION("Consistency check") {
    constexpr CStringView testString("Consistency test string");

    auto crc1 = crc8(testString.c_str(), testString.size());
    auto crc2 = crc8(testString.c_str(), testString.size());
    auto crc3 = crc8(testString.c_str(), testString.size());

    REQUIRE(crc1 == crc2);
    REQUIRE(crc2 == crc3);
    REQUIRE(crc1 == 0x77);
  }

  SECTION("Different data produces different CRC") {
    constexpr CStringView str1("ABC");
    constexpr CStringView str2("CBA");
    constexpr CStringView str3("ABD");

    auto crc1 = crc8(str1.c_str(), str1.size());
    auto crc2 = crc8(str2.c_str(), str2.size());
    auto crc3 = crc8(str3.c_str(), str3.size());

    REQUIRE(crc1 != crc2);
    REQUIRE(crc1 != crc3);
    REQUIRE(crc2 != crc3);
  }

  SECTION("Edge case - single bit difference") {
    constexpr unsigned char data1[] = {0x00, 0x00};
    constexpr unsigned char data2[] = {0x01, 0x00};

    auto crc1 = crc8(data1, sizeof(data1));
    auto crc2 = crc8(data2, sizeof(data2));

    REQUIRE(crc1 != crc2);
  }
}

TEST_CASE("crc16 function", "[core][hashes]") {
  SECTION("Empty data") {
    constexpr CStringView empty("");

    REQUIRE(crc16(empty.c_str(), empty.size()) == 0x0000);
  }

  SECTION("Single character") {
    constexpr CStringView single("A");

    REQUIRE(crc16(single.c_str(), single.size()) == 0x30C0);
  }

  SECTION("Short string") {
    constexpr CStringView shortStr("Hello");

    REQUIRE(crc16(shortStr.c_str(), shortStr.size()) == 0xF353);
  }

  SECTION("Medium string") {
    constexpr CStringView medium("Toygine2 - Free 2D/3D game engine.");

    REQUIRE(crc16(medium.c_str(), medium.size()) == 0x0676);
  }

  SECTION("Long string") {
    constexpr CStringView longStr(
      "This is a very long string that contains multiple words and should test the CRC16 algorithm with a substantial amount of data to process.");

    REQUIRE(crc16(longStr.c_str(), longStr.size()) == 0xB504);
  }

  SECTION("String with special characters") {
    constexpr CStringView special("Hello, World! @#$%^&*()_+-=[]{}|;':\",./<>?");

    REQUIRE(crc16(special.c_str(), special.size()) == 0x9618);
  }

  SECTION("String with numbers") {
    constexpr CStringView numbers("1234567890");

    REQUIRE(crc16(numbers.c_str(), numbers.size()) == 0xC57A);
  }

  SECTION("String with mixed content") {
    constexpr CStringView mixed("ABC123def456GHI789jkl");

    REQUIRE(crc16(mixed.c_str(), mixed.size()) == 0xFF9D);
  }

  SECTION("Unicode string") {
    constexpr CStringView unicode("Привет, мир! 🌍");

    REQUIRE(crc16(unicode.c_str(), unicode.size()) == 0xF20D);
  }

  SECTION("Binary data") {
    constexpr unsigned char binaryData[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE};

    REQUIRE(crc16(binaryData, sizeof(binaryData)) == 0x9B7D);
  }

  SECTION("All zeros") {
    constexpr unsigned char zeros[] = {0x00, 0x00, 0x00, 0x00, 0x00};

    REQUIRE(crc16(zeros, sizeof(zeros)) == 0x0000);
  }

  SECTION("All ones") {
    constexpr unsigned char ones[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    REQUIRE(crc16(ones, sizeof(ones)) == 0x8015);
  }

  SECTION("Pattern data") {
    constexpr unsigned char pattern[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};

    REQUIRE(crc16(pattern, sizeof(pattern)) == 0x519B);
  }

  SECTION("Sequential data") {
    constexpr unsigned char sequential[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    REQUIRE(crc16(sequential, sizeof(sequential)) == 0x4204);
  }

  SECTION("Incremental CRC calculation") {
    constexpr CStringView part1("Toygine2 - Free ");
    constexpr CStringView part2("2D/3D game engine.");

    const auto crc1 = crc16(part1.c_str(), part1.size());
    const auto crc2 = crc16(part2.c_str(), part2.size(), crc1);

    REQUIRE(crc2 == 0x0676);
  }

  SECTION("Consistency check") {
    constexpr CStringView testString("Consistency test string");

    auto crc1 = crc16(testString.c_str(), testString.size());
    auto crc2 = crc16(testString.c_str(), testString.size());
    auto crc3 = crc16(testString.c_str(), testString.size());

    REQUIRE(crc1 == crc2);
    REQUIRE(crc2 == crc3);
    REQUIRE(crc1 == 0xC45E);
  }

  SECTION("Different data produces different CRC") {
    constexpr CStringView str1("ABC");
    constexpr CStringView str2("CBA");
    constexpr CStringView str3("ABD");

    auto crc1 = crc16(str1.c_str(), str1.size());
    auto crc2 = crc16(str2.c_str(), str2.size());
    auto crc3 = crc16(str3.c_str(), str3.size());

    REQUIRE(crc1 != crc2);
    REQUIRE(crc1 != crc3);
    REQUIRE(crc2 != crc3);
  }

  SECTION("Edge case - single bit difference") {
    constexpr unsigned char data1[] = {0x00, 0x00};
    constexpr unsigned char data2[] = {0x01, 0x00};

    auto crc1 = crc16(data1, sizeof(data1));
    auto crc2 = crc16(data2, sizeof(data2));

    REQUIRE(crc1 != crc2);
  }
}

TEST_CASE("crc32 function", "[core][hashes]") {
  SECTION("Empty data") {
    constexpr CStringView empty("");

    REQUIRE(crc32(empty.c_str(), empty.size()) == 0x00000000);
  }

  SECTION("Single character") {
    constexpr CStringView single("A");

    REQUIRE(crc32(single.c_str(), single.size()) == 0xd3d99e8b);
  }

  SECTION("Short string") {
    constexpr CStringView shortStr("Hello");

    REQUIRE(crc32(shortStr.c_str(), shortStr.size()) == 0xF7D18982);
  }

  SECTION("Medium string") {
    constexpr CStringView medium("Toygine2 - Free 2D/3D game engine.");

    REQUIRE(crc32(medium.c_str(), medium.size()) == 0xF78FCD49);
  }

  SECTION("Long string") {
    constexpr CStringView longStr(
      "This is a very long string that contains multiple words and should test the CRC32 algorithm with a substantial amount of data to process.");

    REQUIRE(crc32(longStr.c_str(), longStr.size()) == 0x9fd194da);
  }

  SECTION("String with special characters") {
    constexpr CStringView special("Hello, World! @#$%^&*()_+-=[]{}|;':\",./<>?");

    REQUIRE(crc32(special.c_str(), special.size()) == 0xa07c9757);
  }

  SECTION("String with numbers") {
    constexpr CStringView numbers("1234567890");

    REQUIRE(crc32(numbers.c_str(), numbers.size()) == 0x261DAEE5);
  }

  SECTION("String with mixed content") {
    constexpr CStringView mixed("ABC123def456GHI789jkl");

    REQUIRE(crc32(mixed.c_str(), mixed.size()) == 0xc473d6d6);
  }

  SECTION("Unicode string") {
    constexpr CStringView unicode("Привет, мир! 🌍");

    REQUIRE(crc32(unicode.c_str(), unicode.size()) == 0xc35cc603);
  }

  SECTION("Binary data") {
    constexpr unsigned char binaryData[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE};

    REQUIRE(crc32(binaryData, sizeof(binaryData)) == 0x2542c930);
  }

  SECTION("All zeros") {
    constexpr unsigned char zeros[] = {0x00, 0x00, 0x00, 0x00, 0x00};

    REQUIRE(crc32(zeros, sizeof(zeros)) == 0xc622f71d);
  }

  SECTION("All ones") {
    constexpr unsigned char ones[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    REQUIRE(crc32(ones, sizeof(ones)) == 0xd2fd1072);
  }

  SECTION("Pattern data") {
    constexpr unsigned char pattern[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};

    REQUIRE(crc32(pattern, sizeof(pattern)) == 0xfefe37b2);
  }

  SECTION("Sequential data") {
    constexpr unsigned char sequential[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    REQUIRE(crc32(sequential, sizeof(sequential)) == 0x456cd746);
  }

  SECTION("Incremental CRC calculation") {
    constexpr CStringView part1("Toygine2 - Free ");
    constexpr CStringView part2("2D/3D game engine.");

    const auto crc1 = crc32(part1.c_str(), part1.size());
    const auto crc2 = crc32(part2.c_str(), part2.size(), crc1);

    REQUIRE(crc2 == 0xf78fcd49);
  }

  SECTION("Consistency check") {
    constexpr CStringView testString("Consistency test string");

    auto crc1 = crc32(testString.c_str(), testString.size());
    auto crc2 = crc32(testString.c_str(), testString.size());
    auto crc3 = crc32(testString.c_str(), testString.size());

    REQUIRE(crc1 == crc2);
    REQUIRE(crc2 == crc3);
    REQUIRE(crc1 == 0x8fadd6d9);
  }

  SECTION("Different data produces different CRC") {
    constexpr CStringView str1("ABC");
    constexpr CStringView str2("CBA");
    constexpr CStringView str3("ABD");

    auto crc1 = crc32(str1.c_str(), str1.size());
    auto crc2 = crc32(str2.c_str(), str2.size());
    auto crc3 = crc32(str3.c_str(), str3.size());

    REQUIRE(crc1 != crc2);
    REQUIRE(crc1 != crc3);
    REQUIRE(crc2 != crc3);
  }

  SECTION("Edge case - single bit difference") {
    constexpr unsigned char data1[] = {0x00, 0x00};
    constexpr unsigned char data2[] = {0x01, 0x00};

    auto crc1 = crc32(data1, sizeof(data1));
    auto crc2 = crc32(data2, sizeof(data2));

    REQUIRE(crc1 != crc2);
  }
}

} // namespace toy
