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

TEST_CASE("cstrcmp function", "[core][constexpr_utils]") {
  SECTION("Equal strings") {
    constexpr const char * str1 = "Hello";
    constexpr const char * str2 = "Hello";
    constexpr const char * empty1 = "";
    constexpr const char * empty2 = "";

    STATIC_REQUIRE(cstrcmp(str1, str2) == 0);
    STATIC_REQUIRE(cstrcmp(empty1, empty2) == 0);
    STATIC_REQUIRE(cstrcmp("Test", "Test") == 0);
    STATIC_REQUIRE(cstrcmp("", "") == 0);
  }

  SECTION("Different strings") {
    constexpr const char * str1 = "Hello";
    constexpr const char * str2 = "World";
    constexpr const char * str3 = "Hi";
    constexpr const char * str4 = "Hello World";

    STATIC_REQUIRE(cstrcmp(str1, str2) < 0); // "Hello" < "World"
    STATIC_REQUIRE(cstrcmp(str2, str1) > 0); // "World" > "Hello"
    STATIC_REQUIRE(cstrcmp(str1, str3) < 0); // "Hello" > "Hi"
    STATIC_REQUIRE(cstrcmp(str3, str1) > 0); // "Hi" < "Hello"
    STATIC_REQUIRE(cstrcmp(str1, str4) < 0); // "Hello" < "Hello World"
    STATIC_REQUIRE(cstrcmp(str4, str1) > 0); // "Hello World" > "Hello"
  }

  SECTION("Empty string comparisons") {
    constexpr const char * empty = "";
    constexpr const char * nonEmpty = "Test";

    STATIC_REQUIRE(cstrcmp(empty, empty) == 0);
    STATIC_REQUIRE(cstrcmp(empty, nonEmpty) < 0); // "" < "Test"
    STATIC_REQUIRE(cstrcmp(nonEmpty, empty) > 0); // "Test" > ""
  }

  SECTION("Single character strings") {
    constexpr const char * a = "A";
    constexpr const char * b = "B";
    constexpr const char * z = "Z";

    STATIC_REQUIRE(cstrcmp(a, a) == 0);
    STATIC_REQUIRE(cstrcmp(a, b) < 0); // "A" < "B"
    STATIC_REQUIRE(cstrcmp(b, a) > 0); // "B" > "A"
    STATIC_REQUIRE(cstrcmp(a, z) < 0); // "A" < "Z"
    STATIC_REQUIRE(cstrcmp(z, a) > 0); // "Z" > "A"
    STATIC_REQUIRE(cstrcmp(b, z) < 0); // "B" < "Z"
    STATIC_REQUIRE(cstrcmp(z, b) > 0); // "Z" > "B"
  }

  SECTION("Case sensitivity") {
    constexpr const char * lower = "hello";
    constexpr const char * upper = "HELLO";
    constexpr const char * mixed = "Hello";

    STATIC_REQUIRE(cstrcmp(lower, upper) > 0); // "hello" > "HELLO" (ASCII)
    STATIC_REQUIRE(cstrcmp(upper, lower) < 0); // "HELLO" < "hello" (ASCII)
    STATIC_REQUIRE(cstrcmp(lower, mixed) > 0); // "hello" > "Hello" (ASCII)
    STATIC_REQUIRE(cstrcmp(mixed, lower) < 0); // "Hello" < "hello" (ASCII)
    STATIC_REQUIRE(cstrcmp(upper, mixed) < 0); // "HELLO" < "Hello" (ASCII)
    STATIC_REQUIRE(cstrcmp(mixed, upper) > 0); // "Hello" > "HELLO" (ASCII)
  }

  SECTION("Special characters") {
    constexpr const char * newline = "Line1\nLine2";
    constexpr const char * tab = "Col1\tCol2";
    constexpr const char * space = "Hello World";
    constexpr const char * punct = "Hello, World!";

    STATIC_REQUIRE(cstrcmp(newline, newline) == 0);
    STATIC_REQUIRE(cstrcmp(tab, tab) == 0);
    STATIC_REQUIRE(cstrcmp(space, space) == 0);
    STATIC_REQUIRE(cstrcmp(punct, punct) == 0);

    // Different special characters
    STATIC_REQUIRE(cstrcmp(newline, tab) != 0);
    STATIC_REQUIRE(cstrcmp(tab, space) != 0);
    STATIC_REQUIRE(cstrcmp(space, punct) != 0);
  }

  SECTION("Unicode content") {
    constexpr const char * unicode1 = "Привет";
    constexpr const char * unicode2 = "Мир";
    constexpr const char * unicode3 = "Привет";
    constexpr const char * emoji = "Hello 🌍";

    STATIC_REQUIRE(cstrcmp(unicode1, unicode3) == 0);
    STATIC_REQUIRE(cstrcmp(unicode1, unicode2) != 0);
    STATIC_REQUIRE(cstrcmp(unicode2, unicode1) != 0);
    STATIC_REQUIRE(cstrcmp(unicode1, emoji) != 0);
    STATIC_REQUIRE(cstrcmp(emoji, unicode1) != 0);
  }

  SECTION("Numeric strings") {
    constexpr const char * num1 = "123";
    constexpr const char * num2 = "456";
    constexpr const char * num3 = "123";
    constexpr const char * num4 = "12";
    constexpr const char * num5 = "1234";

    STATIC_REQUIRE(cstrcmp(num1, num3) == 0);
    STATIC_REQUIRE(cstrcmp(num1, num2) < 0); // "123" < "456"
    STATIC_REQUIRE(cstrcmp(num2, num1) > 0); // "456" > "123"
    STATIC_REQUIRE(cstrcmp(num1, num4) > 0); // "123" > "12"
    STATIC_REQUIRE(cstrcmp(num4, num1) < 0); // "12" < "123"
    STATIC_REQUIRE(cstrcmp(num1, num5) < 0); // "123" < "1234"
    STATIC_REQUIRE(cstrcmp(num5, num1) > 0); // "1234" > "123"
  }

  SECTION("Edge cases") {
    // Identical strings
    STATIC_REQUIRE(cstrcmp("", "") == 0);
    STATIC_REQUIRE(cstrcmp("a", "a") == 0);
    STATIC_REQUIRE(cstrcmp("abc", "abc") == 0);

    // One string is prefix of another
    STATIC_REQUIRE(cstrcmp("abc", "abcd") < 0); // "abc" < "abcd"
    STATIC_REQUIRE(cstrcmp("abcd", "abc") > 0); // "abcd" > "abc"
    STATIC_REQUIRE(cstrcmp("", "a") < 0); // "" < "a"
    STATIC_REQUIRE(cstrcmp("a", "") > 0); // "a" > ""

    // Different lengths, same prefix
    STATIC_REQUIRE(cstrcmp("hello", "helloworld") < 0);
    STATIC_REQUIRE(cstrcmp("helloworld", "hello") > 0);
  }

  SECTION("Constexpr operations") {
    constexpr const char * str1 = "Test";
    constexpr const char * str2 = "Test";
    constexpr const char * str3 = "Different";

    constexpr int eq = cstrcmp(str1, str2);
    constexpr int ne = cstrcmp(str1, str3);
    constexpr int lt = cstrcmp("A", "B");
    constexpr int gt = cstrcmp("B", "A");

    STATIC_REQUIRE(eq == 0);
    STATIC_REQUIRE(ne != 0);
    STATIC_REQUIRE(lt < 0);
    STATIC_REQUIRE(gt > 0);
  }

  SECTION("Performance test") {
    constexpr const char * long1 = "This is a very long string for performance testing";
    constexpr const char * long2 = "This is a very long string for performance testing";
    constexpr const char * long3 = "This is a very long string for performance testing!";

    STATIC_REQUIRE(cstrcmp(long1, long2) == 0);
    STATIC_REQUIRE(cstrcmp(long1, long3) < 0);
    STATIC_REQUIRE(cstrcmp(long3, long1) > 0);
  }
}
