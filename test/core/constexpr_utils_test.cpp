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

    // Compare with std::strcmp
    REQUIRE(cstrcmp(str1, str2) == std::strcmp(str1, str2));
    REQUIRE(cstrcmp(empty1, empty2) == std::strcmp(empty1, empty2));
    REQUIRE(cstrcmp("Test", "Test") == std::strcmp("Test", "Test"));
    REQUIRE(cstrcmp("", "") == std::strcmp("", ""));
  }

  SECTION("Different strings") {
    constexpr const char * str1 = "Hello";
    constexpr const char * str2 = "World";
    constexpr const char * str3 = "Hi";
    constexpr const char * str4 = "Hello World";

    STATIC_REQUIRE(cstrcmp(str1, str2) < 0); // "Hello" < "World"
    STATIC_REQUIRE(cstrcmp(str2, str1) > 0); // "World" > "Hello"
    STATIC_REQUIRE(cstrcmp(str1, str3) < 0); // "Hello" < "Hi"
    STATIC_REQUIRE(cstrcmp(str3, str1) > 0); // "Hi" > "Hello"
    STATIC_REQUIRE(cstrcmp(str1, str4) < 0); // "Hello" < "Hello World"
    STATIC_REQUIRE(cstrcmp(str4, str1) > 0); // "Hello World" > "Hello"

    // Compare with std::strcmp
    REQUIRE(cstrcmp(str1, str2) == std::strcmp(str1, str2));
    REQUIRE(cstrcmp(str2, str1) == std::strcmp(str2, str1));
    REQUIRE(cstrcmp(str1, str3) == std::strcmp(str1, str3));
    REQUIRE(cstrcmp(str3, str1) == std::strcmp(str3, str1));
    REQUIRE(cstrcmp(str1, str4) == std::strcmp(str1, str4));
    REQUIRE(cstrcmp(str4, str1) == std::strcmp(str4, str1));
  }

  SECTION("Empty string comparisons") {
    constexpr const char * empty = "";
    constexpr const char * nonEmpty = "Test";

    STATIC_REQUIRE(cstrcmp(empty, empty) == 0);
    STATIC_REQUIRE(cstrcmp(empty, nonEmpty) < 0); // "" < "Test"
    STATIC_REQUIRE(cstrcmp(nonEmpty, empty) > 0); // "Test" > ""

    // Compare with std::strcmp
    REQUIRE(cstrcmp(empty, empty) == std::strcmp(empty, empty));
    REQUIRE(cstrcmp(empty, nonEmpty) == std::strcmp(empty, nonEmpty));
    REQUIRE(cstrcmp(nonEmpty, empty) == std::strcmp(nonEmpty, empty));
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

    // Compare with std::strcmp
    REQUIRE(cstrcmp(a, a) == std::strcmp(a, a));
    REQUIRE(cstrcmp(a, b) == std::strcmp(a, b));
    REQUIRE(cstrcmp(b, a) == std::strcmp(b, a));
    REQUIRE(cstrcmp(a, z) == std::strcmp(a, z));
    REQUIRE(cstrcmp(z, a) == std::strcmp(z, a));
    REQUIRE(cstrcmp(b, z) == std::strcmp(b, z));
    REQUIRE(cstrcmp(z, b) == std::strcmp(z, b));
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

    // Compare with std::strcmp
    REQUIRE(cstrcmp(lower, upper) == std::strcmp(lower, upper));
    REQUIRE(cstrcmp(upper, lower) == std::strcmp(upper, lower));
    REQUIRE(cstrcmp(lower, mixed) == std::strcmp(lower, mixed));
    REQUIRE(cstrcmp(mixed, lower) == std::strcmp(mixed, lower));
    REQUIRE(cstrcmp(upper, mixed) == std::strcmp(upper, mixed));
    REQUIRE(cstrcmp(mixed, upper) == std::strcmp(mixed, upper));
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

    // Compare with std::strcmp
    REQUIRE(cstrcmp(newline, newline) == std::strcmp(newline, newline));
    REQUIRE(cstrcmp(tab, tab) == std::strcmp(tab, tab));
    REQUIRE(cstrcmp(space, space) == std::strcmp(space, space));
    REQUIRE(cstrcmp(punct, punct) == std::strcmp(punct, punct));

    REQUIRE(cstrcmp(newline, tab) == std::strcmp(newline, tab));
    REQUIRE(cstrcmp(tab, space) == std::strcmp(tab, space));
    REQUIRE(cstrcmp(space, punct) == std::strcmp(space, punct));
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

    // Compare with std::strcmp
    REQUIRE(cstrcmp(unicode1, unicode3) == std::strcmp(unicode1, unicode3));
    REQUIRE(cstrcmp(unicode1, unicode2) == std::strcmp(unicode1, unicode2));
    REQUIRE(cstrcmp(unicode2, unicode1) == std::strcmp(unicode2, unicode1));
    REQUIRE(cstrcmp(unicode1, emoji) == std::strcmp(unicode1, emoji));
    REQUIRE(cstrcmp(emoji, unicode1) == std::strcmp(emoji, unicode1));
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

    // Compare with std::strcmp
    REQUIRE(cstrcmp(num1, num3) == std::strcmp(num1, num3));
    REQUIRE(cstrcmp(num1, num2) == std::strcmp(num1, num2));
    REQUIRE(cstrcmp(num2, num1) == std::strcmp(num2, num1));
    REQUIRE(cstrcmp(num1, num4) == std::strcmp(num1, num4));
    REQUIRE(cstrcmp(num4, num1) == std::strcmp(num4, num1));
    REQUIRE(cstrcmp(num1, num5) == std::strcmp(num1, num5));
    REQUIRE(cstrcmp(num5, num1) == std::strcmp(num5, num1));
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

    // Compare with std::strcmp
    REQUIRE(cstrcmp("", "") == std::strcmp("", ""));
    REQUIRE(cstrcmp("a", "a") == std::strcmp("a", "a"));
    REQUIRE(cstrcmp("abc", "abc") == std::strcmp("abc", "abc"));

    REQUIRE(cstrcmp("abc", "abcd") == std::strcmp("abc", "abcd"));
    REQUIRE(cstrcmp("abcd", "abc") == std::strcmp("abcd", "abc"));
    REQUIRE(cstrcmp("", "a") == std::strcmp("", "a"));
    REQUIRE(cstrcmp("a", "") == std::strcmp("a", ""));

    REQUIRE(cstrcmp("hello", "helloworld") == std::strcmp("hello", "helloworld"));
    REQUIRE(cstrcmp("helloworld", "hello") == std::strcmp("helloworld", "hello"));
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

    // Compare with std::strcmp
    REQUIRE(eq == std::strcmp(str1, str2));
    REQUIRE(ne == std::strcmp(str1, str3));
    REQUIRE(lt == std::strcmp("A", "B"));
    REQUIRE(gt == std::strcmp("B", "A"));
  }

  SECTION("Long strings") {
    constexpr const char * long1 = "This is a very long string for performance testing";
    constexpr const char * long2 = "This is a very long string for performance testing";
    constexpr const char * long3 = "This is a very long string for performance testing!";

    STATIC_REQUIRE(cstrcmp(long1, long2) == 0);
    STATIC_REQUIRE(cstrcmp(long1, long3) < 0);
    STATIC_REQUIRE(cstrcmp(long3, long1) > 0);

    // Compare with std::strcmp
    REQUIRE(cstrcmp(long1, long2) == std::strcmp(long1, long2));
    REQUIRE(cstrcmp(long1, long3) == std::strcmp(long1, long3));
    REQUIRE(cstrcmp(long3, long1) == std::strcmp(long3, long1));
  }
}

TEST_CASE("cstrstr function", "[core][constexpr_utils]") {
  SECTION("Basic substring search") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "World";
    constexpr const char * needle2 = "Hello";
    constexpr const char * needle3 = "lo Wo";
    constexpr const char * needle4 = "Hello World";

    STATIC_REQUIRE(cstrstr(haystack, needle1) != nullptr);
    STATIC_REQUIRE(cstrstr(haystack, needle2) != nullptr);
    STATIC_REQUIRE(cstrstr(haystack, needle3) != nullptr);
    STATIC_REQUIRE(cstrstr(haystack, needle4) != nullptr);

    // Verify correct positions
    STATIC_REQUIRE(cstrstr(haystack, needle1) == haystack + 6); // "World" starts at position 6
    STATIC_REQUIRE(cstrstr(haystack, needle2) == haystack); // "Hello" starts at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle3) == haystack + 3); // "lo Wo" starts at position 3
    STATIC_REQUIRE(cstrstr(haystack, needle4) == haystack); // "Hello World" starts at position 0

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
  }

  SECTION("Substring not found") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "Universe";
    constexpr const char * needle2 = "xyz";
    constexpr const char * needle3 = "Hello Universe";
    constexpr const char * needle4 = "World Hello";

    STATIC_REQUIRE(cstrstr(haystack, needle1) == nullptr);
    STATIC_REQUIRE(cstrstr(haystack, needle2) == nullptr);
    STATIC_REQUIRE(cstrstr(haystack, needle3) == nullptr);
    STATIC_REQUIRE(cstrstr(haystack, needle4) == nullptr);

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
  }

  SECTION("Empty needle") {
    constexpr const char * haystack1 = "Hello World";
    constexpr const char * haystack2 = "";
    constexpr const char * emptyNeedle = "";

    STATIC_REQUIRE(cstrstr(haystack1, emptyNeedle) == haystack1);
    STATIC_REQUIRE(cstrstr(haystack2, emptyNeedle) == haystack2);

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack1, emptyNeedle) == std::strstr(haystack1, emptyNeedle));
    REQUIRE(cstrstr(haystack2, emptyNeedle) == std::strstr(haystack2, emptyNeedle));
  }

  SECTION("Empty haystack") {
    constexpr const char * emptyHaystack = "";
    constexpr const char * needle1 = "Hello";
    constexpr const char * needle2 = "";

    STATIC_REQUIRE(cstrstr(emptyHaystack, needle1) == nullptr);
    STATIC_REQUIRE(cstrstr(emptyHaystack, needle2) == emptyHaystack);

    // Compare with std::strstr
    REQUIRE(cstrstr(emptyHaystack, needle1) == std::strstr(emptyHaystack, needle1));
    REQUIRE(cstrstr(emptyHaystack, needle2) == std::strstr(emptyHaystack, needle2));
  }

  SECTION("Single character search") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "H";
    constexpr const char * needle2 = "o";
    constexpr const char * needle3 = "l";
    constexpr const char * needle4 = "x";

    STATIC_REQUIRE(cstrstr(haystack, needle1) == haystack); // "H" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle2) == haystack + 4); // "o" at position 4
    STATIC_REQUIRE(cstrstr(haystack, needle3) == haystack + 2); // "l" at position 2 (first occurrence)
    STATIC_REQUIRE(cstrstr(haystack, needle4) == nullptr); // "x" not found

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
  }

  SECTION("Case sensitivity") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "hello";
    constexpr const char * needle2 = "WORLD";
    constexpr const char * needle3 = "world";
    constexpr const char * needle4 = "Hello";

    STATIC_REQUIRE(cstrstr(haystack, needle1) == nullptr); // Case sensitive
    STATIC_REQUIRE(cstrstr(haystack, needle2) == nullptr); // Case sensitive
    STATIC_REQUIRE(cstrstr(haystack, needle3) == nullptr); // Case sensitive
    STATIC_REQUIRE(cstrstr(haystack, needle4) == haystack); // Exact match

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
  }

  SECTION("Repeated patterns") {
    constexpr const char * haystack = "ababab";
    constexpr const char * needle1 = "ab";
    constexpr const char * needle2 = "bab";
    constexpr const char * needle3 = "abab";
    constexpr const char * needle4 = "ababab";
    constexpr const char * needle5 = "babab";

    STATIC_REQUIRE(cstrstr(haystack, needle1) == haystack); // "ab" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle2) == haystack + 1); // "bab" at position 1
    STATIC_REQUIRE(cstrstr(haystack, needle3) == haystack); // "abab" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle4) == haystack); // "ababab" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle5) == haystack + 1); // "babab" at position 1

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));
  }

  SECTION("Special characters") {
    constexpr const char * haystack = "Hello\n\tWorld!";
    constexpr const char * needle1 = "\n";
    constexpr const char * needle2 = "\t";
    constexpr const char * needle3 = "!";
    constexpr const char * needle4 = "Hello\n";
    constexpr const char * needle5 = "\tWorld";
    constexpr const char * needle6 = "World!";

    STATIC_REQUIRE(cstrstr(haystack, needle1) == haystack + 5); // "\n" at position 5
    STATIC_REQUIRE(cstrstr(haystack, needle2) == haystack + 6); // "\t" at position 6
    STATIC_REQUIRE(cstrstr(haystack, needle3) == haystack + 12); // "!" at position 12
    STATIC_REQUIRE(cstrstr(haystack, needle4) == haystack); // "Hello\n" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle5) == haystack + 6); // "\tWorld" at position 6
    STATIC_REQUIRE(cstrstr(haystack, needle6) == haystack + 7); // "World!" at position 7

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));
    REQUIRE(cstrstr(haystack, needle6) == std::strstr(haystack, needle6));
  }

  SECTION("Unicode content") {
    constexpr const char * haystack = "Hello 世界";
    constexpr const char * needle1 = "世界";
    constexpr const char * needle2 = "Hello 世";
    constexpr const char * needle3 = "界";
    constexpr const char * needle4 = "世";
    constexpr const char * needle5 = "宇宙";

    STATIC_REQUIRE(cstrstr(haystack, needle1) == haystack + 6); // "世界" at position 6
    STATIC_REQUIRE(cstrstr(haystack, needle2) == haystack); // "Hello 世" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle3) == haystack + 9); // "界" at position 9
    STATIC_REQUIRE(cstrstr(haystack, needle4) == haystack + 6); // "世" at position 6
    STATIC_REQUIRE(cstrstr(haystack, needle5) == nullptr); // "宇宙" not found

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));
  }

  SECTION("Numeric content") {
    constexpr const char * haystack = "12345";
    constexpr const char * needle1 = "123";
    constexpr const char * needle2 = "345";
    constexpr const char * needle3 = "234";
    constexpr const char * needle4 = "12345";
    constexpr const char * needle5 = "678";

    STATIC_REQUIRE(cstrstr(haystack, needle1) == haystack); // "123" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle2) == haystack + 2); // "345" at position 2
    STATIC_REQUIRE(cstrstr(haystack, needle3) == haystack + 1); // "234" at position 1
    STATIC_REQUIRE(cstrstr(haystack, needle4) == haystack); // "12345" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle5) == nullptr); // "678" not found

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));
  }

  SECTION("Mixed content") {
    constexpr const char * haystack = "123Hello456";
    constexpr const char * needle1 = "123";
    constexpr const char * needle2 = "Hello";
    constexpr const char * needle3 = "456";
    constexpr const char * needle4 = "3Hello4";
    constexpr const char * needle5 = "123Hello456";
    constexpr const char * needle6 = "789";

    STATIC_REQUIRE(cstrstr(haystack, needle1) == haystack); // "123" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle2) == haystack + 3); // "Hello" at position 3
    STATIC_REQUIRE(cstrstr(haystack, needle3) == haystack + 8); // "456" at position 8
    STATIC_REQUIRE(cstrstr(haystack, needle4) == haystack + 2); // "3Hello4" at position 2
    STATIC_REQUIRE(cstrstr(haystack, needle5) == haystack); // "123Hello456" at position 0
    STATIC_REQUIRE(cstrstr(haystack, needle6) == nullptr); // "789" not found

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));
    REQUIRE(cstrstr(haystack, needle6) == std::strstr(haystack, needle6));
  }

  SECTION("Position-specific search") {
    constexpr const char * haystack = "Hello World";

    // Beginning
    STATIC_REQUIRE(cstrstr(haystack, "H") == haystack);
    STATIC_REQUIRE(cstrstr(haystack, "He") == haystack);
    STATIC_REQUIRE(cstrstr(haystack, "Hello") == haystack);

    // Middle
    STATIC_REQUIRE(cstrstr(haystack, "l") == haystack + 2); // First "l" at position 2
    STATIC_REQUIRE(cstrstr(haystack, "ll") == haystack + 2); // "ll" at position 2
    STATIC_REQUIRE(cstrstr(haystack, "lo W") == haystack + 3); // "lo W" at position 3

    // End
    STATIC_REQUIRE(cstrstr(haystack, "d") == haystack + 10); // "d" at position 10
    STATIC_REQUIRE(cstrstr(haystack, "ld") == haystack + 9); // "ld" at position 9
    STATIC_REQUIRE(cstrstr(haystack, "World") == haystack + 6); // "World" at position 6

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack, "H") == std::strstr(haystack, "H"));
    REQUIRE(cstrstr(haystack, "He") == std::strstr(haystack, "He"));
    REQUIRE(cstrstr(haystack, "Hello") == std::strstr(haystack, "Hello"));

    REQUIRE(cstrstr(haystack, "l") == std::strstr(haystack, "l"));
    REQUIRE(cstrstr(haystack, "ll") == std::strstr(haystack, "ll"));
    REQUIRE(cstrstr(haystack, "lo W") == std::strstr(haystack, "lo W"));

    REQUIRE(cstrstr(haystack, "d") == std::strstr(haystack, "d"));
    REQUIRE(cstrstr(haystack, "ld") == std::strstr(haystack, "ld"));
    REQUIRE(cstrstr(haystack, "World") == std::strstr(haystack, "World"));
  }

  SECTION("Edge cases") {
    // Identical strings
    STATIC_REQUIRE(cstrstr("", "") != nullptr);
    STATIC_REQUIRE(cstrstr("a", "a") != nullptr); // Single character match
    STATIC_REQUIRE(cstrstr("abc", "abc") != nullptr); // Full string match

    // One string is prefix of another
    STATIC_REQUIRE(cstrstr("abc", "abcd") == nullptr); // "abc" doesn't contain "abcd"
    STATIC_REQUIRE(cstrstr("abcd", "abc") != nullptr); // "abcd" contains "abc"
    STATIC_REQUIRE(cstrstr("", "a") == nullptr); // Empty doesn't contain "a"
    STATIC_REQUIRE(cstrstr("a", "") != nullptr); // "a" contains empty string

    // Different lengths, same prefix
    STATIC_REQUIRE(cstrstr("hello", "helloworld") == nullptr); // "hello" doesn't contain "helloworld"
    STATIC_REQUIRE(cstrstr("helloworld", "hello") != nullptr); // "helloworld" contains "hello"

    // Compare with std::strstr
    REQUIRE(cstrstr("", "") == std::strstr("", ""));
    REQUIRE(cstrstr("a", "a") == std::strstr("a", "a"));
    REQUIRE(cstrstr("abc", "abc") == std::strstr("abc", "abc"));

    REQUIRE(cstrstr("abc", "abcd") == std::strstr("abc", "abcd"));
    REQUIRE(cstrstr("abcd", "abc") == std::strstr("abcd", "abc"));
    REQUIRE(cstrstr("", "a") == std::strstr("", "a"));
    REQUIRE(cstrstr("a", "") == std::strstr("a", ""));

    REQUIRE(cstrstr("hello", "helloworld") == std::strstr("hello", "helloworld"));
    REQUIRE(cstrstr("helloworld", "hello") == std::strstr("helloworld", "hello"));
  }

  SECTION("Constexpr operations") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "World";
    constexpr const char * needle2 = "Universe";

    constexpr const char * result1 = cstrstr(haystack, needle1);
    constexpr const char * result2 = cstrstr(haystack, needle2);
    constexpr const char * result3 = cstrstr("Test", "es");
    constexpr const char * result4 = cstrstr("ABC", "B");

    STATIC_REQUIRE(result1 != nullptr);
    STATIC_REQUIRE(result2 == nullptr);
    STATIC_REQUIRE(result3 != nullptr);
    STATIC_REQUIRE(result4 != nullptr);

    // Complex compile-time checks
    STATIC_REQUIRE(cstrstr("Hello World", "Hello") != nullptr);
    STATIC_REQUIRE(cstrstr("Hello World", "xyz") == nullptr);
    STATIC_REQUIRE(cstrstr("Test", "Test") != nullptr);
    STATIC_REQUIRE(cstrstr("Test", "Fail") == nullptr);
    STATIC_REQUIRE(cstrstr("ABC", "ABC") != nullptr);
    STATIC_REQUIRE(cstrstr("ABC", "XYZ") == nullptr);

    // Compare with std::strstr
    REQUIRE(result1 == std::strstr(haystack, needle1));
    REQUIRE(result2 == std::strstr(haystack, needle2));
    REQUIRE(result3 == std::strstr("Test", "es"));
    REQUIRE(result4 == std::strstr("ABC", "B"));
    
    REQUIRE(cstrstr("Hello World", "Hello") == std::strstr("Hello World", "Hello"));
    REQUIRE(cstrstr("Hello World", "xyz") == std::strstr("Hello World", "xyz"));
    REQUIRE(cstrstr("Test", "Test") == std::strstr("Test", "Test"));
    REQUIRE(cstrstr("Test", "Fail") == std::strstr("Test", "Fail"));
    REQUIRE(cstrstr("ABC", "ABC") == std::strstr("ABC", "ABC"));
    REQUIRE(cstrstr("ABC", "XYZ") == std::strstr("ABC", "XYZ"));
  }

  SECTION("Long strings") {
    constexpr const char * longHaystack = "This is a very long string for performance testing";
    constexpr const char * needle1 = "very long";
    constexpr const char * needle2 = "performance";
    constexpr const char * needle3 = "testing";
    constexpr const char * needle4 = "not found";

    STATIC_REQUIRE(cstrstr(longHaystack, needle1) != nullptr);
    STATIC_REQUIRE(cstrstr(longHaystack, needle2) != nullptr);
    STATIC_REQUIRE(cstrstr(longHaystack, needle3) != nullptr);
    STATIC_REQUIRE(cstrstr(longHaystack, needle4) == nullptr);

    // Compare with std::strstr
    REQUIRE(cstrstr(longHaystack, needle1) == std::strstr(longHaystack, needle1));
    REQUIRE(cstrstr(longHaystack, needle2) == std::strstr(longHaystack, needle2));
    REQUIRE(cstrstr(longHaystack, needle3) == std::strstr(longHaystack, needle3));
    REQUIRE(cstrstr(longHaystack, needle4) == std::strstr(longHaystack, needle4));
  }
}
