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

static constexpr std::array<std::uint8_t, 119> utf8Text{
  {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x20, 0x2F, 0x20, 0xD0, 0x9F,
   0xD1, 0x80, 0xD0, 0xB8, 0xD0, 0xB2, 0xD0, 0xB5, 0xD1, 0x82, 0x20, 0xD0, 0xBC, 0xD0, 0xB8, 0xD1, 0x80,
   0x21, 0x20, 0x2F, 0x20, 0x42, 0x6F, 0x6E, 0x6A, 0x6F, 0x75, 0x72, 0x20, 0x74, 0x6F, 0x75, 0x74, 0x20,
   0x6C, 0x65, 0x20, 0x6D, 0x6F, 0x6E, 0x64, 0x65, 0x21, 0x20, 0x2F, 0x20, 0x48, 0x61, 0x6C, 0x6C, 0x6F,
   0x20, 0x57, 0x65, 0x6C, 0x74, 0x21, 0x20, 0x2F, 0x20, 0xEC, 0x95, 0x88, 0xEB, 0x85, 0x95, 0xED, 0x95,
   0x98, 0xEC, 0x84, 0xB8, 0xEC, 0x9A, 0x94, 0x21, 0x20, 0x2F, 0x20, 0xE3, 0x83, 0x8F, 0xE3, 0x83, 0xAD,
   0xE3, 0x83, 0xBC, 0xE3, 0x83, 0xAF, 0xE3, 0x83, 0xBC, 0xE3, 0x83, 0xAB, 0xE3, 0x83, 0x89, 0x21, 0x00}};

static constexpr std::array<wchar_t, 86> unicodeText{
  {0x0048U, 0x0065U, 0x006CU, 0x006CU, 0x006FU, 0x0020U, 0x0057U, 0x006FU, 0x0072U, 0x006CU, 0x0064U, 0x0021U, 0x0020U,
   0x002FU, 0x0020U, 0x041FU, 0x0440U, 0x0438U, 0x0432U, 0x0435U, 0x0442U, 0x0020U, 0x043CU, 0x0438U, 0x0440U, 0x0021U,
   0x0020U, 0x002FU, 0x0020U, 0x0042U, 0x006FU, 0x006EU, 0x006AU, 0x006FU, 0x0075U, 0x0072U, 0x0020U, 0x0074U, 0x006FU,
   0x0075U, 0x0074U, 0x0020U, 0x006CU, 0x0065U, 0x0020U, 0x006DU, 0x006FU, 0x006EU, 0x0064U, 0x0065U, 0x0021U, 0x0020U,
   0x002FU, 0x0020U, 0x0048U, 0x0061U, 0x006CU, 0x006CU, 0x006FU, 0x0020U, 0x0057U, 0x0065U, 0x006CU, 0x0074U, 0x0021U,
   0x0020U, 0x002FU, 0x0020U, 0xC548U, 0xB155U, 0xD558U, 0xC138U, 0xC694U, 0x0021U, 0x0020U, 0x002FU, 0x0020U, 0x30CFU,
   0x30EDU, 0x30FCU, 0x30EFU, 0x30FCU, 0x30EBU, 0x30C9U, 0x0021U, 0x0000}};

static constexpr std::array<float, 16> floatArray{{0.0f, -0.0f, 10000000.0f, -10000000.0f, 100000.0f, -100000.0f,
                                                   4200.0f, -4200.0f, 42.0f, -42.0f, 0.042f, -0.042f, 0.000042f,
                                                   -0.000042f, 0.00000042f, -0.00000042f}};

static constexpr std::array<double, 16> doubleArray{{0.0, -0.0, 10000000.0, -10000000.0, 100000.0, -100000.0, 4200.0,
                                                     -4200.0, 42.0, -42.0, 0.042, -0.042, 0.000042, -0.000042,
                                                     0.00000042, -0.00000042}};

static constexpr std::array<const char *, 16> asciiArray{{"0", "0", "1e+7", "-1e+7", "100000", "-100000", "4200",
                                                          "-4200", "42", "-42", "0.042", "-0.042", "0.000042",
                                                          "-0.000042", "4.2e-7", "-4.2e-7"}};

TEST_CASE("ArraySize returns the size of an array", "[core][utils]") {
  const bool boolArray[]{true, false};
  const int intArray[]{1, 2, 3};
  const double doubleArray[]{1.0, 2.0, 3.0, 4.0};
  const char charArray[]{'a', 'b', 'c', 'd', 'e'};
  const char * stringArray[]{"aaaa", "bbbb", "cccc", "dddd", "eeee", "ffff"};

  CHECK(ArraySize(boolArray) == 2);
  CHECK(ArraySize(intArray) == 3);
  CHECK(ArraySize(doubleArray) == 4);
  CHECK(ArraySize(charArray) == 5);
  CHECK(ArraySize(stringArray) == 6);
}

TEST_CASE("utf8toWChar converts a Unicode UTF-8 encoded string to a wide character string", "[core][utils]") {
  wchar_t testBuffer[utf8Text.size()];
  constexpr auto testBufferSize = ArraySize(testBuffer);

  const auto utf8String = reinterpret_cast<const char *>(utf8Text.data());

  // nullptr
  CHECK(wcscmp(L"", utf8toWChar(testBuffer, testBufferSize, nullptr, 0)) == 0);
  CHECK(wcscmp(testBuffer, L"") == 0);

  // empty string
  CHECK(wcscmp(L"", utf8toWChar(testBuffer, testBufferSize, "", 0)) == 0);
  CHECK(wcscmp(testBuffer, L"") == 0);

  // utf8 C array
  CHECK(wcscmp(unicodeText.data(), utf8toWChar(testBuffer, testBufferSize, utf8String, utf8Text.size())) == 0);
  CHECK(wcscmp(testBuffer, unicodeText.data()) == 0);

  // utf8 C string
  CHECK(wcscmp(unicodeText.data(), utf8toWChar(testBuffer, testBufferSize, utf8String)) == 0);
  CHECK(wcscmp(testBuffer, unicodeText.data()) == 0);

  // utf8 std::string
  CHECK(wcscmp(unicodeText.data(), utf8toWChar(testBuffer, testBufferSize, std::string(utf8String))) == 0);
  CHECK(wcscmp(testBuffer, unicodeText.data()) == 0);

  // null inputs
  CHECK(utf8toWChar(nullptr, 10, "test", 4) == nullptr);
}

TEST_CASE("wcharToUtf8 converts a Unicode wide character string to a UTF-8 encoded string", "[core][utils]") {
  char testBuffer[unicodeText.size() * wcharInUtf8MaxSize];
  constexpr auto testBufferSize = ArraySize(testBuffer);

  const auto utf8String = reinterpret_cast<const char *>(utf8Text.data());

  // nullptr
  CHECK(strcmp("", wcharToUtf8(testBuffer, testBufferSize, nullptr)) == 0);
  CHECK(strcmp(testBuffer, "") == 0);

  // empty string
  CHECK(strcmp("", wcharToUtf8(testBuffer, testBufferSize, L"")) == 0);
  CHECK(strcmp(testBuffer, "") == 0);

  // wchar C string
  CHECK(strcmp(utf8String, wcharToUtf8(testBuffer, testBufferSize, unicodeText.data())) == 0);
  CHECK(strcmp(utf8String, testBuffer) == 0);

  // buffer size limits
  CHECK(strcmp("", wcharToUtf8(testBuffer, 1, L"A")) == 0);
  CHECK(strcmp(testBuffer, "") == 0);

  // null inputs
  CHECK(wcharToUtf8(nullptr, 10, L"test") == nullptr);
}

TEST_CASE("utf8Len returns the number of Unicode characters in a UTF-8 encoded string", "[core][utils]") {
  static const char * s_utf8Text = "Hello World!";
  static const char * s_emptyText = "";

  const auto utf8String = reinterpret_cast<const char *>(utf8Text.data());

  CHECK(std::strlen(s_utf8Text) == utf8Len(s_utf8Text));
  CHECK(utf8Len(s_emptyText) == 0);

  CHECK(std::strlen(utf8String) != utf8Len(utf8String));
  CHECK(wcslen(unicodeText.data()) == utf8Len(utf8String));
}

TEST_CASE("reverseString reverses a given string in-place", "[core][utils]") {
  char reverseBuffer[utf8Text.size()];

  const auto utf8String = reinterpret_cast<const char *>(utf8Text.data());
  const auto utf8StrLen = std::strlen(utf8String);

  memcpy(reverseBuffer, utf8Text.data(), utf8Text.size());

  CHECK(strcmp(utf8String, reverseBuffer) == 0);
  CHECK(std::strlen(reverseBuffer) == utf8StrLen);

  reverseString(reverseBuffer, std::strlen(reverseBuffer));
  CHECK(strcmp(reverseBuffer, utf8String) != 0);
  CHECK(std::strlen(reverseBuffer) == utf8StrLen);

  for (std::size_t index = 0; index < (sizeof(reverseBuffer) - 1); ++index) {
    CHECK(utf8Text[index] == static_cast<std::uint8_t>(reverseBuffer[sizeof(reverseBuffer) - index - 2]));
  }

  reverseString(reverseBuffer);
  CHECK(strcmp(utf8String, reverseBuffer) == 0);
}

TEST_CASE("itoa converts an integer value to a string representation.", "[core][utils]") {
  char buffer[80];

  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::int8_t>::max()), "127") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::int8_t>::min()), "-128") == 0);

  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::int16_t>::max()), "32767") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::int16_t>::min()), "-32768") == 0);

  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::int32_t>::max()), "2147483647") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::int32_t>::min()), "-2147483648") == 0);

  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::int64_t>::max()), "9223372036854775807") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::int64_t>::min()), "-9223372036854775808") == 0);

  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint8_t>::min(), 8), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint8_t>::min(), 10), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint8_t>::min(), 16), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint8_t>::max(), 2), "11111111") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint8_t>::max(), 8), "377") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint8_t>::max(), 10), "255") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint8_t>::max(), 16), "FF") == 0);

  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint16_t>::min(), 8), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint16_t>::min(), 10), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint16_t>::min(), 16), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint16_t>::max(), 2), "1111111111111111") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint16_t>::max(), 8), "177777") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint16_t>::max(), 10), "65535") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint16_t>::max(), 16), "FFFF") == 0);

  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint32_t>::min(), 8), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint32_t>::min(), 10), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint32_t>::min(), 16), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint32_t>::max(), 2),
               "11111111111111111111111111111111")
        == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint32_t>::max(), 8), "37777777777") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint32_t>::max(), 10), "4294967295") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint32_t>::max(), 16), "FFFFFFFF") == 0);

  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint64_t>::min(), 8), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint64_t>::min(), 10), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint64_t>::min(), 16), "0") == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint64_t>::max(), 2),
               "1111111111111111111111111111111111111111111111111111111111111111")
        == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint64_t>::max(), 8), "1777777777777777777777")
        == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint64_t>::max(), 10), "18446744073709551615")
        == 0);
  CHECK(strcmp(itoa(buffer, ArraySize(buffer), std::numeric_limits<std::uint64_t>::max(), 16), "FFFFFFFFFFFFFFFF")
        == 0);
}

TEST_CASE("ftoa converts a floating-point number to its string representation in a specified precision",
          "[core][utils]") {
  char buffer[32];

  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), 3.1415926535897932384626433832795f), "3.141592") == 0);
  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), 3.1415926535897932384626433832795), "3.14159244298935") == 0);

  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), -3.1415926535897932384626433832795f), "-3.141592") == 0);
  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), -3.1415926535897932384626433832795), "-3.14159244298935") == 0);

  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), std::numeric_limits<float>::infinity()), "+INF") == 0);
  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), -std::numeric_limits<float>::infinity()), "-INF") == 0);
  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), std::numeric_limits<float>::quiet_NaN()), "+NAN") == 0);
  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), -std::numeric_limits<float>::quiet_NaN()), "-NAN") == 0);
  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), std::numeric_limits<double>::infinity()), "+INF") == 0);
  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), -std::numeric_limits<double>::infinity()), "-INF") == 0);
  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), std::numeric_limits<double>::quiet_NaN()), "+NAN") == 0);
  CHECK(strcmp(ftoa(buffer, ArraySize(buffer), -std::numeric_limits<double>::quiet_NaN()), "-NAN") == 0);

  static_assert(floatArray.size() == doubleArray.size() && floatArray.size() == asciiArray.size(),
                "Invalid arrays size");

  for (size_t index = 0; index < floatArray.size(); ++index) {
    CHECK(strcmp(ftoa(buffer, ArraySize(buffer), floatArray[index]), asciiArray[index]) == 0);
    CHECK(strcmp(ftoa(buffer, ArraySize(buffer), doubleArray[index], 7), asciiArray[index]) == 0);
  }
}

TEST_CASE("formatNumberString format number string", "[core][utils]") {
  static constexpr std::array<const char *, 14> numbers{{"", "Hello World", "-256192.12", "32", "4192", "+2561921.2",
                                                         "1", "12", "123", "12345678", "-1234567890", "+0", "-0",
                                                         "0.0"}};
  static constexpr std::array<const char *, 14> parsedNumbers{{"", "Hello World", "-256 192.12", "32", "4 192",
                                                               "+2 561 921.2", "1", "12", "123", "12 345 678",
                                                               "-1 234 567 890", "+0", "-0", "0.0"}};

  static_assert(numbers.size() == parsedNumbers.size(), "Invalid arrays size");

  for (std::size_t index = 0; index < parsedNumbers.size(); ++index) {
    char buffer[128];

#ifdef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
    strcpy_s<sizeof(buffer)>(buffer, numbers[index]);
#else
    strncpy(buffer, numbers[index], sizeof(buffer));
#endif // _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES

    formatNumberString(buffer, sizeof(buffer), " ");
    CHECK(strcmp(buffer, parsedNumbers[index]) == 0);
  }
}
