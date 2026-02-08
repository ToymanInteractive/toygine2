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

#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

constexpr array<uint8_t, 119> utf8TestData{{
  0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x20, 0x2F, 0x20, 0xD0, 0x9F,
  0xD1, 0x80, 0xD0, 0xB8, 0xD0, 0xB2, 0xD0, 0xB5, 0xD1, 0x82, 0x20, 0xD0, 0xBC, 0xD0, 0xB8, 0xD1, 0x80,
  0x21, 0x20, 0x2F, 0x20, 0x42, 0x6F, 0x6E, 0x6A, 0x6F, 0x75, 0x72, 0x20, 0x74, 0x6F, 0x75, 0x74, 0x20,
  0x6C, 0x65, 0x20, 0x6D, 0x6F, 0x6E, 0x64, 0x65, 0x21, 0x20, 0x2F, 0x20, 0x48, 0x61, 0x6C, 0x6C, 0x6F,
  0x20, 0x57, 0x65, 0x6C, 0x74, 0x21, 0x20, 0x2F, 0x20, 0xEC, 0x95, 0x88, 0xEB, 0x85, 0x95, 0xED, 0x95,
  0x98, 0xEC, 0x84, 0xB8, 0xEC, 0x9A, 0x94, 0x21, 0x20, 0x2F, 0x20, 0xE3, 0x83, 0x8F, 0xE3, 0x83, 0xAD,
  0xE3, 0x83, 0xBC, 0xE3, 0x83, 0xAF, 0xE3, 0x83, 0xBC, 0xE3, 0x83, 0xAB, 0xE3, 0x83, 0x89, 0x21, 0x00,
}};

constexpr array<wchar_t, 86> unicodeTestData{{
  0x0048U, 0x0065U, 0x006CU, 0x006CU, 0x006FU, 0x0020U, 0x0057U, 0x006FU, 0x0072U, 0x006CU, 0x0064U, 0x0021U, 0x0020U,
  0x002FU, 0x0020U, 0x041FU, 0x0440U, 0x0438U, 0x0432U, 0x0435U, 0x0442U, 0x0020U, 0x043CU, 0x0438U, 0x0440U, 0x0021U,
  0x0020U, 0x002FU, 0x0020U, 0x0042U, 0x006FU, 0x006EU, 0x006AU, 0x006FU, 0x0075U, 0x0072U, 0x0020U, 0x0074U, 0x006FU,
  0x0075U, 0x0074U, 0x0020U, 0x006CU, 0x0065U, 0x0020U, 0x006DU, 0x006FU, 0x006EU, 0x0064U, 0x0065U, 0x0021U, 0x0020U,
  0x002FU, 0x0020U, 0x0048U, 0x0061U, 0x006CU, 0x006CU, 0x006FU, 0x0020U, 0x0057U, 0x0065U, 0x006CU, 0x0074U, 0x0021U,
  0x0020U, 0x002FU, 0x0020U, 0xC548U, 0xB155U, 0xD558U, 0xC138U, 0xC694U, 0x0021U, 0x0020U, 0x002FU, 0x0020U, 0x30CFU,
  0x30EDU, 0x30FCU, 0x30EFU, 0x30FCU, 0x30EBU, 0x30C9U, 0x0021U, 0x0000,
}};

constexpr array<float, 16> floatTestValues{{0.0f, -0.0f, 10000000.0f, -10000000.0f, 100000.0f, -100000.0f, 4200.0f,
                                            -4200.0f, 42.0f, -42.0f, 0.042f, -0.042f, 0.000042f, -0.000042f,
                                            0.00000042f, -0.00000042f}};

constexpr array<double, 16> doubleTestValues{{0.0, -0.0, 10000000.0, -10000000.0, 100000.0, -100000.0, 4200.0, -4200.0,
                                              42.0, -42.0, 0.042, -0.042, 0.000042, -0.000042, 0.00000042,
                                              -0.00000042}};

constexpr array<const char *, 16> asciiTestValues{{"0", "0", "1e+7", "-1e+7", "100000", "-100000", "4200", "-4200",
                                                   "42", "-42", "0.042", "-0.042", "0.000042", "-0.000042", "4.2e-7",
                                                   "-4.2e-7"}};

// UTF-8 to wide character conversion.
TEST_CASE("core/utils/utf8_to_wchar_converts_utf8_to_wide_string") {
  // nullptr input yields empty wide string.
  SUBCASE("nullptr input") {
    wchar_t testBuffer[utf8TestData.size()];

    REQUIRE(wcscmp(L"", utf8toWChar(testBuffer, size(testBuffer), nullptr, 0)) == 0);
    REQUIRE(wcscmp(testBuffer, L"") == 0);
  }

  // Empty string yields empty wide string.
  SUBCASE("empty string") {
    wchar_t testBuffer[utf8TestData.size()];

    REQUIRE(wcscmp(L"", utf8toWChar(testBuffer, size(testBuffer), "", 0)) == 0);
    REQUIRE(wcscmp(testBuffer, L"") == 0);
  }

  // UTF-8 C array converts to wide string.
  SUBCASE("utf8 C array") {
    wchar_t testBuffer[utf8TestData.size()];

    REQUIRE(wcscmp(unicodeTestData.data(),
                   utf8toWChar(testBuffer, size(testBuffer), reinterpret_cast<const char *>(utf8TestData.data()),
                               utf8TestData.size()))
            == 0);
    REQUIRE(wcscmp(testBuffer, unicodeTestData.data()) == 0);
  }

  // UTF-8 C string converts to wide string.
  SUBCASE("utf8 C string") {
    wchar_t testBuffer[utf8TestData.size()];

    REQUIRE(wcscmp(unicodeTestData.data(),
                   utf8toWChar(testBuffer, size(testBuffer), reinterpret_cast<const char *>(utf8TestData.data())))
            == 0);
    REQUIRE(wcscmp(testBuffer, unicodeTestData.data()) == 0);
  }

  // UTF-8 std::string converts to wide string.
  SUBCASE("utf8 std::string") {
    wchar_t testBuffer[utf8TestData.size()];

    REQUIRE(
      wcscmp(unicodeTestData.data(), utf8toWChar(testBuffer, size(testBuffer),
                                                 std::string(reinterpret_cast<const char *>(utf8TestData.data()))))
      == 0);
    REQUIRE(wcscmp(testBuffer, unicodeTestData.data()) == 0);
  }

  // Null buffer returns nullptr.
  SUBCASE("null buffer returns nullptr") {
    REQUIRE(utf8toWChar(nullptr, 10, "test", 4) == nullptr);
  }
}

// Wide character to UTF-8 conversion.
TEST_CASE("core/utils/wchar_to_utf8_converts_wide_to_utf8_string") {
  // nullptr input yields empty UTF-8 string.
  SUBCASE("nullptr input") {
    char testBuffer[unicodeTestData.size() * wcharInUtf8MaxSize];

    REQUIRE(strcmp("", wcharToUtf8(testBuffer, size(testBuffer), nullptr)) == 0);
    REQUIRE(strcmp(testBuffer, "") == 0);
  }

  // Empty wide string yields empty UTF-8 string.
  SUBCASE("empty string") {
    char testBuffer[unicodeTestData.size() * wcharInUtf8MaxSize];

    REQUIRE(strcmp("", wcharToUtf8(testBuffer, size(testBuffer), L"")) == 0);
    REQUIRE(strcmp(testBuffer, "") == 0);
  }

  // Wide C string converts to UTF-8.
  SUBCASE("wchar C string") {
    char testBuffer[unicodeTestData.size() * wcharInUtf8MaxSize];

    REQUIRE(strcmp(reinterpret_cast<const char *>(utf8TestData.data()),
                   wcharToUtf8(testBuffer, size(testBuffer), unicodeTestData.data()))
            == 0);
    REQUIRE(strcmp(reinterpret_cast<const char *>(utf8TestData.data()), testBuffer) == 0);
  }

  // Buffer size limits; insufficient buffer yields empty.
  SUBCASE("buffer size limits") {
    char testBuffer[unicodeTestData.size() * wcharInUtf8MaxSize];

    REQUIRE(strcmp("", wcharToUtf8(testBuffer, 1, L"A")) == 0);
    REQUIRE(strcmp(testBuffer, "") == 0);
  }

  // Null buffer returns nullptr (wcharToUtf8).
  SUBCASE("null buffer returns nullptr") {
    REQUIRE(wcharToUtf8(nullptr, 10, L"test") == nullptr);
  }
}

// UTF-8 string length in code points.
TEST_CASE("core/utils/utf8_len_returns_unicode_code_point_count") {
  // ASCII string length equals byte length.
  SUBCASE("ASCII string length equals byte length") {
    const char * asciiSample = "Hello World!";

    REQUIRE(std::char_traits<char>::length(asciiSample) == utf8Len(asciiSample));
  }

  // Empty string has zero length.
  SUBCASE("empty string") {
    const char * emptySample = "";

    REQUIRE(utf8Len(emptySample) == 0);
  }

  // Multi-byte UTF-8 length equals code point count.
  SUBCASE("multi-byte UTF-8 length equals code point count") {
    REQUIRE(std::char_traits<char>::length(reinterpret_cast<const char *>(utf8TestData.data()))
            != utf8Len(reinterpret_cast<const char *>(utf8TestData.data())));
    REQUIRE(wcslen(unicodeTestData.data()) == utf8Len(reinterpret_cast<const char *>(utf8TestData.data())));
  }
}

// In-place string reversal.
TEST_CASE("core/utils/reverse_string_reverses_in_place") {
  // Initial copy matches source before reverse.
  SUBCASE("initial copy matches source") {
    char reverseBuffer[utf8TestData.size()];

    memcpy(reverseBuffer, utf8TestData.data(), utf8TestData.size());

    REQUIRE(strcmp(reinterpret_cast<const char *>(utf8TestData.data()), reverseBuffer) == 0);
    REQUIRE(std::char_traits<char>::length(reverseBuffer) == utf8TestData.size() - 1);
  }

  // First reverse changes content and preserves length.
  SUBCASE("first reverse changes content and preserves length") {
    char reverseBuffer[utf8TestData.size()];

    memcpy(reverseBuffer, utf8TestData.data(), utf8TestData.size());
    reverseString(reverseBuffer, std::char_traits<char>::length(reverseBuffer));

    REQUIRE(strcmp(reverseBuffer, reinterpret_cast<const char *>(utf8TestData.data())) != 0);
    REQUIRE(std::char_traits<char>::length(reverseBuffer) == utf8TestData.size() - 1);
  }

  // Reversed bytes match source in reverse order.
  SUBCASE("reversed bytes match source in reverse order") {
    char reverseBuffer[utf8TestData.size()];

    memcpy(reverseBuffer, utf8TestData.data(), utf8TestData.size());
    reverseString(reverseBuffer, std::char_traits<char>::length(reverseBuffer));

    for (size_t index = 0; index < (sizeof(reverseBuffer) - 1); ++index) {
      REQUIRE(utf8TestData[index] == static_cast<uint8_t>(reverseBuffer[sizeof(reverseBuffer) - index - 2]));
    }
  }

  // Second reverse restores original.
  SUBCASE("second reverse restores original") {
    char reverseBuffer[utf8TestData.size()];

    memcpy(reverseBuffer, utf8TestData.data(), utf8TestData.size());
    reverseString(reverseBuffer, std::char_traits<char>::length(reverseBuffer));
    reverseString(reverseBuffer);

    REQUIRE(strcmp(reinterpret_cast<const char *>(utf8TestData.data()), reverseBuffer) == 0);
  }
}

// Signed integer to string conversion.
TEST_CASE("core/utils/itoa_converts_integer_to_string") {
  // int8_t min, max, zero.
  SUBCASE("int8_t") {
    char buffer[8];

    REQUIRE(strcmp(itoa(buffer, size(buffer), numeric_limits<int8_t>::max()), "127") == 0);
    REQUIRE(strcmp(itoa(buffer, size(buffer), numeric_limits<int8_t>::min()), "-128") == 0);
    REQUIRE(strcmp(itoa(buffer, size(buffer), static_cast<int8_t>(0)), "0") == 0);
  }

  // int16_t min, max, zero.
  SUBCASE("int16_t") {
    char buffer[8];

    REQUIRE(strcmp(itoa(buffer, size(buffer), numeric_limits<int16_t>::max()), "32767") == 0);
    REQUIRE(strcmp(itoa(buffer, size(buffer), numeric_limits<int16_t>::min()), "-32768") == 0);
    REQUIRE(strcmp(itoa(buffer, size(buffer), static_cast<int16_t>(0)), "0") == 0);
  }

  // int32_t min, max, zero.
  SUBCASE("int32_t") {
    char buffer[12];

    REQUIRE(strcmp(itoa(buffer, size(buffer), numeric_limits<int32_t>::max()), "2147483647") == 0);
    REQUIRE(strcmp(itoa(buffer, size(buffer), numeric_limits<int32_t>::min()), "-2147483648") == 0);
    REQUIRE(strcmp(itoa(buffer, size(buffer), static_cast<int32_t>(0)), "0") == 0);
  }

  // int64_t min, max, zero.
  SUBCASE("int64_t") {
    char buffer[24];

    REQUIRE(strcmp(itoa(buffer, size(buffer), numeric_limits<int64_t>::max()), "9223372036854775807") == 0);
    REQUIRE(strcmp(itoa(buffer, size(buffer), numeric_limits<int64_t>::min()), "-9223372036854775808") == 0);
    REQUIRE(strcmp(itoa(buffer, size(buffer), static_cast<int64_t>(0)), "0") == 0);
  }
}

// Unsigned integer to string conversion with radix (2, 8, 10, 16).
TEST_CASE("core/utils/utoa_converts_unsigned_integer_to_string") {
  // uint8_t min/max in radix 2, 8, 10, 16.
  SUBCASE("uint8_t") {
    char buffer[12];

    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint8_t>::min(), 2), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint8_t>::min(), 8), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint8_t>::min(), 10), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint8_t>::min(), 16), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint8_t>::max(), 2), "11111111") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint8_t>::max(), 8), "377") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint8_t>::max(), 10), "255") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint8_t>::max(), 16), "FF") == 0);
  }

  // uint16_t min/max in radix 2, 8, 10, 16.
  SUBCASE("uint16_t") {
    char buffer[20];

    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint16_t>::min(), 2), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint16_t>::min(), 8), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint16_t>::min(), 10), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint16_t>::min(), 16), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint16_t>::max(), 2), "1111111111111111") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint16_t>::max(), 8), "177777") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint16_t>::max(), 10), "65535") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint16_t>::max(), 16), "FFFF") == 0);
  }

  // uint32_t min/max in radix 2, 8, 10, 16.
  SUBCASE("uint32_t") {
    char buffer[36];

    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint32_t>::min(), 2), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint32_t>::min(), 8), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint32_t>::min(), 10), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint32_t>::min(), 16), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint32_t>::max(), 2), "11111111111111111111111111111111")
            == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint32_t>::max(), 8), "37777777777") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint32_t>::max(), 10), "4294967295") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint32_t>::max(), 16), "FFFFFFFF") == 0);
  }

  // uint64_t min/max in radix 2, 8, 10, 16.
  SUBCASE("uint64_t") {
    char buffer[68];

    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint64_t>::min(), 2), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint64_t>::min(), 8), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint64_t>::min(), 10), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint64_t>::min(), 16), "0") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint64_t>::max(), 2),
                   "1111111111111111111111111111111111111111111111111111111111111111")
            == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint64_t>::max(), 8), "1777777777777777777777") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint64_t>::max(), 10), "18446744073709551615") == 0);
    REQUIRE(strcmp(utoa(buffer, size(buffer), numeric_limits<uint64_t>::max(), 16), "FFFFFFFFFFFFFFFF") == 0);
  }
}

// Floating-point to string conversion.
TEST_CASE("core/utils/ftoa_converts_float_to_string") {
  // Float and double positive values.
  SUBCASE("float and double positive") {
    char buffer[32];

    REQUIRE(strcmp(ftoa(buffer, size(buffer), 3.1415926535897932384626433832795f), "3.141592") == 0);
    REQUIRE(strcmp(ftoa(buffer, size(buffer), 3.1415926535897932384626433832795), "3.14159244298935") == 0);
  }

  // Float and double negative values.
  SUBCASE("float and double negative") {
    char buffer[32];

    REQUIRE(strcmp(ftoa(buffer, size(buffer), -3.1415926535897932384626433832795f), "-3.141592") == 0);
    REQUIRE(strcmp(ftoa(buffer, size(buffer), -3.1415926535897932384626433832795), "-3.14159244298935") == 0);
  }

  // Infinity and NaN formatting.
  SUBCASE("infinity and nan") {
    char buffer[32];

    REQUIRE(strcmp(ftoa(buffer, size(buffer), numeric_limits<float>::infinity()), "+INF") == 0);
    REQUIRE(strcmp(ftoa(buffer, size(buffer), -numeric_limits<float>::infinity()), "-INF") == 0);
    REQUIRE(strcmp(ftoa(buffer, size(buffer), numeric_limits<float>::quiet_NaN()), "+NAN") == 0);
    REQUIRE(strcmp(ftoa(buffer, size(buffer), -numeric_limits<float>::quiet_NaN()), "-NAN") == 0);
    REQUIRE(strcmp(ftoa(buffer, size(buffer), numeric_limits<double>::infinity()), "+INF") == 0);
    REQUIRE(strcmp(ftoa(buffer, size(buffer), -numeric_limits<double>::infinity()), "-INF") == 0);
    REQUIRE(strcmp(ftoa(buffer, size(buffer), numeric_limits<double>::quiet_NaN()), "+NAN") == 0);
    REQUIRE(strcmp(ftoa(buffer, size(buffer), -numeric_limits<double>::quiet_NaN()), "-NAN") == 0);
  }

  // Array of float and double values vs expected ASCII.
  SUBCASE("array of float and double values") {
    char buffer[32];

    static_assert(floatTestValues.size() == doubleTestValues.size() && floatTestValues.size() == asciiTestValues.size(),
                  "float, double and expected ASCII arrays must have same size");

    for (size_t index = 0; index < floatTestValues.size(); ++index) {
      REQUIRE(strcmp(ftoa(buffer, size(buffer), floatTestValues[index]), asciiTestValues[index]) == 0);
      REQUIRE(strcmp(ftoa(buffer, size(buffer), doubleTestValues[index], 7), asciiTestValues[index]) == 0);
    }
  }
}

// Format number string with thousand separator.
TEST_CASE("core/utils/format_number_string_adds_thousand_separator") {
  constexpr array<const char *, 14> numbers{{"", "Hello World", "-256192.12", "32", "4192", "+2561921.2", "1", "12",
                                             "123", "12345678", "-1234567890", "+0", "-0", "0.0"}};

  constexpr array<const char *, 14> parsedNumbers{{"", "Hello World", "-256 192.12", "32", "4 192", "+2 561 921.2", "1",
                                                   "12", "123", "12 345 678", "-1 234 567 890", "+0", "-0", "0.0"}};

  static_assert(numbers.size() == parsedNumbers.size(), "input and expected arrays must have same size");

  for (size_t index = 0; index < parsedNumbers.size(); ++index) {
    char buffer[128];

#ifdef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
    strcpy_s<sizeof(buffer)>(buffer, numbers[index]);
#else
    strncpy(buffer, numbers[index], sizeof(buffer));
#endif // _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES

    formatNumberString(buffer, sizeof(buffer), " ");
    REQUIRE(strcmp(buffer, parsedNumbers[index]) == 0);
  }
}

} // namespace toy
