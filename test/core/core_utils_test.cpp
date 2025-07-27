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
#include <core.hpp>

using namespace toygine;

static const std::array<uint8_t, 119> utf8Text{0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21,
                                               0x20, 0x2F, 0x20, 0xD0, 0x9F, 0xD1, 0x80, 0xD0, 0xB8, 0xD0, 0xB2, 0xD0,
                                               0xB5, 0xD1, 0x82, 0x20, 0xD0, 0xBC, 0xD0, 0xB8, 0xD1, 0x80, 0x21, 0x20,
                                               0x2F, 0x20, 0x42, 0x6F, 0x6E, 0x6A, 0x6F, 0x75, 0x72, 0x20, 0x74, 0x6F,
                                               0x75, 0x74, 0x20, 0x6C, 0x65, 0x20, 0x6D, 0x6F, 0x6E, 0x64, 0x65, 0x21,
                                               0x20, 0x2F, 0x20, 0x48, 0x61, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x65, 0x6C,
                                               0x74, 0x21, 0x20, 0x2F, 0x20, 0xEC, 0x95, 0x88, 0xEB, 0x85, 0x95, 0xED,
                                               0x95, 0x98, 0xEC, 0x84, 0xB8, 0xEC, 0x9A, 0x94, 0x21, 0x20, 0x2F, 0x20,
                                               0xE3, 0x83, 0x8F, 0xE3, 0x83, 0xAD, 0xE3, 0x83, 0xBC, 0xE3, 0x83, 0xAF,
                                               0xE3, 0x83, 0xBC, 0xE3, 0x83, 0xAB, 0xE3, 0x83, 0x89, 0x21, 0x00};
static const std::array<wchar_t, 86> unicodeText{0x0048U, 0x0065U, 0x006CU, 0x006CU, 0x006FU, 0x0020U, 0x0057U, 0x006FU,
                                                 0x0072U, 0x006CU, 0x0064U, 0x0021U, 0x0020U, 0x002FU, 0x0020U, 0x041FU,
                                                 0x0440U, 0x0438U, 0x0432U, 0x0435U, 0x0442U, 0x0020U, 0x043CU, 0x0438U,
                                                 0x0440U, 0x0021U, 0x0020U, 0x002FU, 0x0020U, 0x0042U, 0x006FU, 0x006EU,
                                                 0x006AU, 0x006FU, 0x0075U, 0x0072U, 0x0020U, 0x0074U, 0x006FU, 0x0075U,
                                                 0x0074U, 0x0020U, 0x006CU, 0x0065U, 0x0020U, 0x006DU, 0x006FU, 0x006EU,
                                                 0x0064U, 0x0065U, 0x0021U, 0x0020U, 0x002FU, 0x0020U, 0x0048U, 0x0061U,
                                                 0x006CU, 0x006CU, 0x006FU, 0x0020U, 0x0057U, 0x0065U, 0x006CU, 0x0074U,
                                                 0x0021U, 0x0020U, 0x002FU, 0x0020U, 0xC548U, 0xB155U, 0xD558U, 0xC138U,
                                                 0xC694U, 0x0021U, 0x0020U, 0x002FU, 0x0020U, 0x30CFU, 0x30EDU, 0x30FCU,
                                                 0x30EFU, 0x30FCU, 0x30EBU, 0x30C9U, 0x0021U, 0x0000};

TEST_CASE("returns the size of an array", "[ArraySize]") {
  const bool boolArray[] = {true, false};
  const int intArray[] = {1, 2, 3};
  const double doubleArray[] = {1.0, 2.0, 3.0, 4.0};
  const char charArray[] = {'a', 'b', 'c', 'd', 'e'};
  const char * stringArray[] = {"aaaa", "bbbb", "cccc", "dddd", "eeee", "ffff"};

  CHECK(ArraySize(boolArray) == 2);
  CHECK(ArraySize(intArray) == 3);
  CHECK(ArraySize(doubleArray) == 4);
  CHECK(ArraySize(charArray) == 5);
  CHECK(ArraySize(stringArray) == 6);
}

TEST_CASE("utf8 string converts to widechar string", "[utf8toWChar]") {
  wchar_t testUnicodeString[utf8Text.size()];

  CHECK(wcscmp(unicodeText.data(), utf8toWChar(testUnicodeString, ArraySize(testUnicodeString),
                                               reinterpret_cast<char const *>(utf8Text.data()),
                                               strlen(reinterpret_cast<char const *>(utf8Text.data()))))
        == 0);

  CHECK(wcscmp(L"", utf8toWChar(testUnicodeString, ArraySize(testUnicodeString), nullptr, 0)) == 0);

  CHECK(wcscmp(unicodeText.data(), utf8toWChar(testUnicodeString, ArraySize(testUnicodeString),
                                               reinterpret_cast<char const *>(utf8Text.data())))
        == 0);

  CHECK(wcscmp(unicodeText.data(), utf8toWChar(testUnicodeString, ArraySize(testUnicodeString),
                                               std::string(reinterpret_cast<char const *>(utf8Text.data()))))
        == 0);
}
