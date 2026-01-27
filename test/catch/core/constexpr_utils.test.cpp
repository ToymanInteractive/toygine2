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

#include <catch2/catch_test_macros.hpp>

#include "core.hpp"

namespace toy {

TEST_CASE("cstrcmp function", "[core][constexpr_utils]") {
  SECTION("Equal strings") {
    constexpr const char * str1 = "Hello";
    constexpr const char * str2 = "Hello";
    constexpr const char * empty1 = "";
    constexpr const char * empty2 = "";

    static_assert(cstrcmp(str1, str2) == 0);
    static_assert(cstrcmp(empty1, empty2) == 0);
    static_assert(cstrcmp("Test", "Test") == 0);
    static_assert(cstrcmp("", "") == 0);

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

    static_assert(cstrcmp(str1, str2) < 0); // "Hello" < "World"
    static_assert(cstrcmp(str2, str1) > 0); // "World" > "Hello"
    static_assert(cstrcmp(str1, str3) < 0); // "Hello" < "Hi"
    static_assert(cstrcmp(str3, str1) > 0); // "Hi" > "Hello"
    static_assert(cstrcmp(str1, str4) < 0); // "Hello" < "Hello World"
    static_assert(cstrcmp(str4, str1) > 0); // "Hello World" > "Hello"

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

    static_assert(cstrcmp(empty, empty) == 0);
    static_assert(cstrcmp(empty, nonEmpty) < 0); // "" < "Test"
    static_assert(cstrcmp(nonEmpty, empty) > 0); // "Test" > ""

    // Compare with std::strcmp
    REQUIRE(cstrcmp(empty, empty) == std::strcmp(empty, empty));
    REQUIRE(cstrcmp(empty, nonEmpty) == std::strcmp(empty, nonEmpty));
    REQUIRE(cstrcmp(nonEmpty, empty) == std::strcmp(nonEmpty, empty));
  }

  SECTION("Single character strings") {
    constexpr const char * a = "A";
    constexpr const char * b = "B";
    constexpr const char * z = "Z";

    static_assert(cstrcmp(a, a) == 0);
    static_assert(cstrcmp(a, b) < 0); // "A" < "B"
    static_assert(cstrcmp(b, a) > 0); // "B" > "A"
    static_assert(cstrcmp(a, z) < 0); // "A" < "Z"
    static_assert(cstrcmp(z, a) > 0); // "Z" > "A"
    static_assert(cstrcmp(b, z) < 0); // "B" < "Z"
    static_assert(cstrcmp(z, b) > 0); // "Z" > "B"

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

    static_assert(cstrcmp(lower, upper) > 0); // "hello" > "HELLO" (ASCII)
    static_assert(cstrcmp(upper, lower) < 0); // "HELLO" < "hello" (ASCII)
    static_assert(cstrcmp(lower, mixed) > 0); // "hello" > "Hello" (ASCII)
    static_assert(cstrcmp(mixed, lower) < 0); // "Hello" < "hello" (ASCII)
    static_assert(cstrcmp(upper, mixed) < 0); // "HELLO" < "Hello" (ASCII)
    static_assert(cstrcmp(mixed, upper) > 0); // "Hello" > "HELLO" (ASCII)

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

    static_assert(cstrcmp(newline, newline) == 0);
    static_assert(cstrcmp(tab, tab) == 0);
    static_assert(cstrcmp(space, space) == 0);
    static_assert(cstrcmp(punct, punct) == 0);

    // Different special characters
    static_assert(cstrcmp(newline, tab) != 0);
    static_assert(cstrcmp(tab, space) != 0);
    static_assert(cstrcmp(space, punct) != 0);

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

    static_assert(cstrcmp(unicode1, unicode3) == 0);
    static_assert(cstrcmp(unicode1, unicode2) != 0);
    static_assert(cstrcmp(unicode2, unicode1) != 0);
    static_assert(cstrcmp(unicode1, emoji) != 0);
    static_assert(cstrcmp(emoji, unicode1) != 0);

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

    static_assert(cstrcmp(num1, num3) == 0);
    static_assert(cstrcmp(num1, num2) < 0); // "123" < "456"
    static_assert(cstrcmp(num2, num1) > 0); // "456" > "123"
    static_assert(cstrcmp(num1, num4) > 0); // "123" > "12"
    static_assert(cstrcmp(num4, num1) < 0); // "12" < "123"
    static_assert(cstrcmp(num1, num5) < 0); // "123" < "1234"
    static_assert(cstrcmp(num5, num1) > 0); // "1234" > "123"

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
    static_assert(cstrcmp("", "") == 0);
    static_assert(cstrcmp("a", "a") == 0);
    static_assert(cstrcmp("abc", "abc") == 0);

    // One string is prefix of another
    static_assert(cstrcmp("abc", "abcd") < 0); // "abc" < "abcd"
    static_assert(cstrcmp("abcd", "abc") > 0); // "abcd" > "abc"
    static_assert(cstrcmp("", "a") < 0); // "" < "a"
    static_assert(cstrcmp("a", "") > 0); // "a" > ""

    // Different lengths, same prefix
    static_assert(cstrcmp("hello", "helloworld") < 0);
    static_assert(cstrcmp("helloworld", "hello") > 0);

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

    static_assert(eq == 0);
    static_assert(ne != 0);
    static_assert(lt < 0);
    static_assert(gt > 0);

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

    static_assert(cstrcmp(long1, long2) == 0);
    static_assert(cstrcmp(long1, long3) < 0);
    static_assert(cstrcmp(long3, long1) > 0);

    // Compare with std::strcmp
    REQUIRE(cstrcmp(long1, long2) == std::strcmp(long1, long2));
    REQUIRE(cstrcmp(long1, long3) == std::strcmp(long1, long3));
    REQUIRE(cstrcmp(long3, long1) == std::strcmp(long3, long1));
  }
}

TEST_CASE("cstrchr function", "[core][constexpr_utils]") {
  SECTION("Basic character search") {
    constexpr const char * str = "Hello World";
    constexpr char ch1 = 'H';
    constexpr char ch2 = 'o';
    constexpr char ch3 = 'l';
    constexpr char ch4 = 'd';
    constexpr char ch5 = 'z';

    static_assert(cstrchr(str, ch1) == str); // "H" at position 0
    static_assert(cstrchr(str, ch2) == str + 4); // "o" at position 4
    static_assert(cstrchr(str, ch3) == str + 2); // "l" at position 2 (first occurrence)
    static_assert(cstrchr(str, ch4) == str + 10); // "d" at position 10
    static_assert(cstrchr(str, ch5) == nullptr);

    // Compare with std::strchr
    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));
    REQUIRE(cstrchr(str, ch5) == std::strchr(str, ch5));
  }

  SECTION("Character not found") {
    constexpr const char * str = "Hello World";
    constexpr char ch1 = 'x';
    constexpr char ch2 = 'Z';
    constexpr char ch3 = '9';
    constexpr char ch4 = '@';

    static_assert(cstrchr(str, ch1) == nullptr);
    static_assert(cstrchr(str, ch2) == nullptr);
    static_assert(cstrchr(str, ch3) == nullptr);
    static_assert(cstrchr(str, ch4) == nullptr);

    // Compare with std::strchr
    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));
  }

  SECTION("Empty string") {
    constexpr const char * emptyStr = "";
    constexpr char ch1 = 'a';
    constexpr char ch2 = '\0';

    static_assert(cstrchr(emptyStr, ch1) == nullptr);
    static_assert(cstrchr(emptyStr, ch2) == emptyStr);

    // Compare with std::strchr
    REQUIRE(cstrchr(emptyStr, ch1) == std::strchr(emptyStr, ch1));
    REQUIRE(cstrchr(emptyStr, ch2) == std::strchr(emptyStr, ch2));
  }

  SECTION("Single character string") {
    constexpr const char * singleChar = "A";
    constexpr char ch1 = 'A';
    constexpr char ch2 = 'B';
    constexpr char ch3 = 'a';

    static_assert(cstrchr(singleChar, ch1) == singleChar);
    static_assert(cstrchr(singleChar, ch2) == nullptr);
    static_assert(cstrchr(singleChar, ch3) == nullptr);

    // Compare with std::strchr
    REQUIRE(cstrchr(singleChar, ch1) == std::strchr(singleChar, ch1));
    REQUIRE(cstrchr(singleChar, ch2) == std::strchr(singleChar, ch2));
    REQUIRE(cstrchr(singleChar, ch3) == std::strchr(singleChar, ch3));
  }

  SECTION("Case sensitivity") {
    constexpr const char * str = "Hello World";
    constexpr char ch1 = 'h'; // lowercase
    constexpr char ch2 = 'H'; // uppercase
    constexpr char ch3 = 'w'; // lowercase
    constexpr char ch4 = 'W'; // uppercase

    static_assert(cstrchr(str, ch1) == nullptr); // Case sensitive
    static_assert(cstrchr(str, ch2) == str); // Exact match
    static_assert(cstrchr(str, ch3) == nullptr); // Case sensitive
    static_assert(cstrchr(str, ch4) == str + 6); // Exact match

    // Compare with std::strchr
    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));
  }

  SECTION("Repeated characters") {
    constexpr const char * str = "ababab";
    constexpr char ch1 = 'a';
    constexpr char ch2 = 'b';
    constexpr char ch3 = 'c';

    static_assert(cstrchr(str, ch1) == str); // First 'a' at position 0
    static_assert(cstrchr(str, ch2) == str + 1); // First 'b' at position 1
    static_assert(cstrchr(str, ch3) == nullptr); // 'c' not found

    // Compare with std::strchr
    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
  }

  SECTION("Special characters") {
    constexpr const char * str = "Hello\n\tWorld!";
    constexpr char ch1 = '\n';
    constexpr char ch2 = '\t';
    constexpr char ch3 = '!';
    constexpr char ch4 = ' ';

    static_assert(cstrchr(str, ch1) == str + 5); // "\n" at position 5
    static_assert(cstrchr(str, ch2) == str + 6); // "\t" at position 6
    static_assert(cstrchr(str, ch3) == str + 12); // "!" at position 12
    static_assert(cstrchr(str, ch4) == nullptr);

    // Compare with std::strchr
    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));
  }

  SECTION("Unicode content") {
    constexpr const char * str = "Hello 世界";
    constexpr char ch1 = 'H';
    constexpr char ch2 = 'z';

    static_assert(cstrchr(str, ch1) == str); // "H" at position 0
    static_assert(cstrchr(str, ch2) == nullptr); // "z" not found

    // Compare with std::strchr
    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
  }

  SECTION("Numeric content") {
    constexpr const char * str = "12345";
    constexpr char ch1 = '1';
    constexpr char ch2 = '3';
    constexpr char ch3 = '5';
    constexpr char ch4 = '6';

    static_assert(cstrchr(str, ch1) == str); // "1" at position 0
    static_assert(cstrchr(str, ch2) == str + 2); // "3" at position 2
    static_assert(cstrchr(str, ch3) == str + 4); // "5" at position 4
    static_assert(cstrchr(str, ch4) == nullptr); // "6" not found

    // Compare with std::strchr
    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));
  }

  SECTION("Mixed content") {
    constexpr const char * str = "123Hello456";
    constexpr char ch1 = '1';
    constexpr char ch2 = 'H';
    constexpr char ch3 = 'o';
    constexpr char ch4 = '6';
    constexpr char ch5 = 'z';

    static_assert(cstrchr(str, ch1) == str); // "1" at position 0
    static_assert(cstrchr(str, ch2) == str + 3); // "H" at position 3
    static_assert(cstrchr(str, ch3) == str + 7); // "o" at position 7
    static_assert(cstrchr(str, ch4) == str + 10); // "6" at position 10
    static_assert(cstrchr(str, ch5) == nullptr); // "z" not found

    // Compare with std::strchr
    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));
    REQUIRE(cstrchr(str, ch5) == std::strchr(str, ch5));
  }

  SECTION("Position-specific search") {
    constexpr const char * str = "Hello World";

    // Beginning
    static_assert(cstrchr(str, 'H') == str);
    static_assert(cstrchr(str, 'e') == str + 1);

    // Middle
    static_assert(cstrchr(str, 'l') == str + 2); // First "l" at position 2
    static_assert(cstrchr(str, 'o') == str + 4); // First "o" at position 4

    // End
    static_assert(cstrchr(str, 'd') == str + 10);
    static_assert(cstrchr(str, 'l') == str + 2); // First "l", not the last one

    // Compare with std::strchr
    REQUIRE(cstrchr(str, 'H') == std::strchr(str, 'H'));
    REQUIRE(cstrchr(str, 'e') == std::strchr(str, 'e'));
    REQUIRE(cstrchr(str, 'l') == std::strchr(str, 'l'));
    REQUIRE(cstrchr(str, 'o') == std::strchr(str, 'o'));
    REQUIRE(cstrchr(str, 'd') == std::strchr(str, 'd'));
  }

  SECTION("Edge cases") {
    constexpr const char * empty = "";
    constexpr const char * ch1 = "a";

    // Null terminator
    static_assert(cstrchr(empty, '\0') != nullptr);
    static_assert(cstrchr(ch1, '\0') != nullptr);

    // Single character match
    static_assert(cstrchr(ch1, 'a') != nullptr);
    static_assert(cstrchr(ch1, 'b') == nullptr);

    // Compare with std::strchr
    REQUIRE(cstrchr(empty, '\0') == std::strchr(empty, '\0'));
    REQUIRE(cstrchr(ch1, '\0') == std::strchr(ch1, '\0'));

    REQUIRE(cstrchr(ch1, 'a') == std::strchr(ch1, 'a'));
    REQUIRE(cstrchr(ch1, 'b') == std::strchr(ch1, 'b'));
  }

  SECTION("Constexpr operations") {
    constexpr const char * helloWorld = "Hello World";
    constexpr const char * test = "Test";
    constexpr const char * abc = "ABC";
    constexpr char ch1 = 'o';
    constexpr char ch2 = 'z';

    constexpr const char * result1 = cstrchr(helloWorld, ch1);
    constexpr const char * result2 = cstrchr(helloWorld, ch2);
    constexpr const char * result3 = cstrchr(test, 'e');
    constexpr const char * result4 = cstrchr(abc, 'B');

    static_assert(result1 != nullptr);
    static_assert(result2 == nullptr);
    static_assert(result3 != nullptr);
    static_assert(result4 != nullptr);

    // Complex compile-time checks
    static_assert(cstrchr(helloWorld, 'H') != nullptr);
    static_assert(cstrchr(helloWorld, 'z') == nullptr);
    static_assert(cstrchr(test, 'T') != nullptr);
    static_assert(cstrchr(test, 'Z') == nullptr);
    static_assert(cstrchr(abc, 'A') != nullptr);
    static_assert(cstrchr(abc, 'Z') == nullptr);

    // Compare with std::strchr
    REQUIRE(result1 == std::strchr(helloWorld, ch1));
    REQUIRE(result2 == std::strchr(helloWorld, ch2));
    REQUIRE(result3 == std::strchr(test, 'e'));
    REQUIRE(result4 == std::strchr(abc, 'B'));
    REQUIRE(cstrchr(helloWorld, 'H') == std::strchr(helloWorld, 'H'));
    REQUIRE(cstrchr(helloWorld, 'z') == std::strchr(helloWorld, 'z'));
    REQUIRE(cstrchr(test, 'T') == std::strchr(test, 'T'));
    REQUIRE(cstrchr(test, 'Z') == std::strchr(test, 'Z'));
    REQUIRE(cstrchr(abc, 'A') == std::strchr(abc, 'A'));
    REQUIRE(cstrchr(abc, 'Z') == std::strchr(abc, 'Z'));
  }

  SECTION("Long strings") {
    constexpr const char * longStr = "This is a very long string for performance testing";
    constexpr char ch1 = 'v';
    constexpr char ch2 = 'p';
    constexpr char ch3 = 't';
    constexpr char ch4 = 'z';

    static_assert(cstrchr(longStr, ch1) != nullptr);
    static_assert(cstrchr(longStr, ch2) != nullptr);
    static_assert(cstrchr(longStr, ch3) != nullptr);
    static_assert(cstrchr(longStr, ch4) == nullptr);

    // Compare with std::strchr
    REQUIRE(cstrchr(longStr, ch1) == std::strchr(longStr, ch1));
    REQUIRE(cstrchr(longStr, ch2) == std::strchr(longStr, ch2));
    REQUIRE(cstrchr(longStr, ch3) == std::strchr(longStr, ch3));
    REQUIRE(cstrchr(longStr, ch4) == std::strchr(longStr, ch4));
  }
}

TEST_CASE("cstrpbrk function", "[core][constexpr_utils]") {
  SECTION("Basic character set search") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "aeiou";
    constexpr const char * accept2 = "H";
    constexpr const char * accept3 = "d";
    constexpr const char * accept4 = "xyz";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) != nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);
    static_assert(cstrpbrk(str, accept4) == nullptr);

    static_assert(cstrpbrk(str, nullptr) == nullptr);
    static_assert(cstrpbrk(nullptr, accept1) == nullptr);
    static_assert(cstrpbrk(nullptr, nullptr) == nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));

    REQUIRE(cstrpbrk(str, nullptr) == nullptr);
    REQUIRE(cstrpbrk(nullptr, accept1) == nullptr);
    REQUIRE(cstrpbrk(nullptr, nullptr) == nullptr);
  }

  SECTION("Single character in accept set") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "e";
    constexpr const char * accept2 = "o";
    constexpr const char * accept3 = "W";
    constexpr const char * accept4 = "Z";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) != nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);
    static_assert(cstrpbrk(str, accept4) == nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));
  }

  SECTION("Multiple characters in accept set") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "aeiou";
    constexpr const char * accept2 = "Hl";
    constexpr const char * accept3 = "Wrd";
    constexpr const char * accept4 = "xyz";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) != nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);
    static_assert(cstrpbrk(str, accept4) == nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));
  }

  SECTION("Empty strings") {
    constexpr const char * emptyStr = "";
    constexpr const char * accept1 = "aeiou";
    constexpr const char * accept2 = "";

    static_assert(cstrpbrk(emptyStr, accept1) == nullptr);
    static_assert(cstrpbrk(emptyStr, accept2) == nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(emptyStr, accept1) == std::strpbrk(emptyStr, accept1));
    REQUIRE(cstrpbrk(emptyStr, accept2) == std::strpbrk(emptyStr, accept2));
  }

  SECTION("First character match") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "H";
    constexpr const char * accept2 = "Hel";
    constexpr const char * accept3 = "Hl";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) != nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
  }

  SECTION("Last character match") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "d";
    constexpr const char * accept2 = "ld";
    constexpr const char * accept3 = "World";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) != nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
  }

  SECTION("Case sensitivity") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "hello";
    constexpr const char * accept2 = "HELLO";
    constexpr const char * accept3 = "Hello";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) != nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
  }

  SECTION("Special characters") {
    constexpr const char * str = "Hello, World!";
    constexpr const char * accept1 = ",!";
    constexpr const char * accept2 = ".,";
    constexpr const char * accept3 = "!?";
    constexpr const char * accept4 = "xyz";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) != nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);
    static_assert(cstrpbrk(str, accept4) == nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));
  }

  SECTION("Numeric characters") {
    constexpr const char * str = "Hello123World";
    constexpr const char * accept1 = "123";
    constexpr const char * accept2 = "456";
    constexpr const char * accept3 = "0123456789";
    constexpr const char * accept4 = "abc";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) == nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);
    static_assert(cstrpbrk(str, accept4) == nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));
  }

  SECTION("Whitespace characters") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = " ";
    constexpr const char * accept2 = " \t\n";
    constexpr const char * accept3 = "\t";
    constexpr const char * accept4 = "xyz";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) != nullptr);
    static_assert(cstrpbrk(str, accept3) == nullptr);
    static_assert(cstrpbrk(str, accept4) == nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));
  }

  SECTION("Repeated characters in accept set") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "lll";
    constexpr const char * accept2 = "HHH";
    constexpr const char * accept3 = "llH";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) != nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
  }

  SECTION("Long strings") {
    constexpr const char * longStr = "This is a very long string for performance testing";
    constexpr const char * accept1 = "aeiou";
    constexpr const char * accept2 = "xyz";
    constexpr const char * accept3 = "T";
    constexpr const char * accept4 = "g";

    static_assert(cstrpbrk(longStr, accept1) != nullptr);
    static_assert(cstrpbrk(longStr, accept2) != nullptr);
    static_assert(cstrpbrk(longStr, accept3) != nullptr);
    static_assert(cstrpbrk(longStr, accept4) != nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(longStr, accept1) == std::strpbrk(longStr, accept1));
    REQUIRE(cstrpbrk(longStr, accept2) == std::strpbrk(longStr, accept2));
    REQUIRE(cstrpbrk(longStr, accept3) == std::strpbrk(longStr, accept3));
    REQUIRE(cstrpbrk(longStr, accept4) == std::strpbrk(longStr, accept4));
  }

  SECTION("Edge cases") {
    constexpr const char * str = "A";
    constexpr const char * accept1 = "A";
    constexpr const char * accept2 = "B";
    constexpr const char * accept3 = "AB";

    static_assert(cstrpbrk(str, accept1) != nullptr);
    static_assert(cstrpbrk(str, accept2) == nullptr);
    static_assert(cstrpbrk(str, accept3) != nullptr);

    // Compare with std::strpbrk
    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
  }
}

TEST_CASE("cstrstr function", "[core][constexpr_utils]") {
  SECTION("Basic substring search") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "World";
    constexpr const char * needle2 = "Hello";
    constexpr const char * needle3 = "lo Wo";
    constexpr const char * needle4 = "Hello World";

    static_assert(cstrstr(haystack, needle1) != nullptr);
    static_assert(cstrstr(haystack, needle2) != nullptr);
    static_assert(cstrstr(haystack, needle3) != nullptr);
    static_assert(cstrstr(haystack, needle4) != nullptr);

    // Verify correct positions
    static_assert(cstrstr(haystack, needle1) == haystack + 6); // "World" starts at position 6
    static_assert(cstrstr(haystack, needle2) == haystack); // "Hello" starts at position 0
    static_assert(cstrstr(haystack, needle3) == haystack + 3); // "lo Wo" starts at position 3
    static_assert(cstrstr(haystack, needle4) == haystack); // "Hello World" starts at position 0

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

    static_assert(cstrstr(haystack, needle1) == nullptr);
    static_assert(cstrstr(haystack, needle2) == nullptr);
    static_assert(cstrstr(haystack, needle3) == nullptr);
    static_assert(cstrstr(haystack, needle4) == nullptr);

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

    static_assert(cstrstr(haystack1, emptyNeedle) == haystack1);
    static_assert(cstrstr(haystack2, emptyNeedle) == haystack2);

    // Compare with std::strstr
    REQUIRE(cstrstr(haystack1, emptyNeedle) == std::strstr(haystack1, emptyNeedle));
    REQUIRE(cstrstr(haystack2, emptyNeedle) == std::strstr(haystack2, emptyNeedle));
  }

  SECTION("Empty haystack") {
    constexpr const char * emptyHaystack = "";
    constexpr const char * needle1 = "Hello";
    constexpr const char * needle2 = "";

    static_assert(cstrstr(emptyHaystack, needle1) == nullptr);
    static_assert(cstrstr(emptyHaystack, needle2) == emptyHaystack);

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

    static_assert(cstrstr(haystack, needle1) == haystack); // "H" at position 0
    static_assert(cstrstr(haystack, needle2) == haystack + 4); // "o" at position 4
    static_assert(cstrstr(haystack, needle3) == haystack + 2); // "l" at position 2 (first occurrence)
    static_assert(cstrstr(haystack, needle4) == nullptr); // "x" not found

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

    static_assert(cstrstr(haystack, needle1) == nullptr); // Case sensitive
    static_assert(cstrstr(haystack, needle2) == nullptr); // Case sensitive
    static_assert(cstrstr(haystack, needle3) == nullptr); // Case sensitive
    static_assert(cstrstr(haystack, needle4) == haystack); // Exact match

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

    static_assert(cstrstr(haystack, needle1) == haystack); // "ab" at position 0
    static_assert(cstrstr(haystack, needle2) == haystack + 1); // "bab" at position 1
    static_assert(cstrstr(haystack, needle3) == haystack); // "abab" at position 0
    static_assert(cstrstr(haystack, needle4) == haystack); // "ababab" at position 0
    static_assert(cstrstr(haystack, needle5) == haystack + 1); // "babab" at position 1

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

    static_assert(cstrstr(haystack, needle1) == haystack + 5); // "\n" at position 5
    static_assert(cstrstr(haystack, needle2) == haystack + 6); // "\t" at position 6
    static_assert(cstrstr(haystack, needle3) == haystack + 12); // "!" at position 12
    static_assert(cstrstr(haystack, needle4) == haystack); // "Hello\n" at position 0
    static_assert(cstrstr(haystack, needle5) == haystack + 6); // "\tWorld" at position 6
    static_assert(cstrstr(haystack, needle6) == haystack + 7); // "World!" at position 7

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

    static_assert(cstrstr(haystack, needle1) == haystack + 6); // "世界" at position 6
    static_assert(cstrstr(haystack, needle2) == haystack); // "Hello 世" at position 0
    static_assert(cstrstr(haystack, needle3) == haystack + 9); // "界" at position 9
    static_assert(cstrstr(haystack, needle4) == haystack + 6); // "世" at position 6
    static_assert(cstrstr(haystack, needle5) == nullptr); // "宇宙" not found

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

    static_assert(cstrstr(haystack, needle1) == haystack); // "123" at position 0
    static_assert(cstrstr(haystack, needle2) == haystack + 2); // "345" at position 2
    static_assert(cstrstr(haystack, needle3) == haystack + 1); // "234" at position 1
    static_assert(cstrstr(haystack, needle4) == haystack); // "12345" at position 0
    static_assert(cstrstr(haystack, needle5) == nullptr); // "678" not found

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

    static_assert(cstrstr(haystack, needle1) == haystack); // "123" at position 0
    static_assert(cstrstr(haystack, needle2) == haystack + 3); // "Hello" at position 3
    static_assert(cstrstr(haystack, needle3) == haystack + 8); // "456" at position 8
    static_assert(cstrstr(haystack, needle4) == haystack + 2); // "3Hello4" at position 2
    static_assert(cstrstr(haystack, needle5) == haystack); // "123Hello456" at position 0
    static_assert(cstrstr(haystack, needle6) == nullptr); // "789" not found

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
    static_assert(cstrstr(haystack, "H") == haystack);
    static_assert(cstrstr(haystack, "He") == haystack);
    static_assert(cstrstr(haystack, "Hello") == haystack);

    // Middle
    static_assert(cstrstr(haystack, "l") == haystack + 2); // First "l" at position 2
    static_assert(cstrstr(haystack, "ll") == haystack + 2); // "ll" at position 2
    static_assert(cstrstr(haystack, "lo W") == haystack + 3); // "lo W" at position 3

    // End
    static_assert(cstrstr(haystack, "d") == haystack + 10); // "d" at position 10
    static_assert(cstrstr(haystack, "ld") == haystack + 9); // "ld" at position 9
    static_assert(cstrstr(haystack, "World") == haystack + 6); // "World" at position 6

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
    constexpr const char * empty = "";
    constexpr const char * a = "a";
    constexpr const char * abc = "abc";
    constexpr const char * abcd = "abcd";
    constexpr const char * hello = "hello";
    constexpr const char * helloWorld = "hello world";

    // Identical strings
    static_assert(cstrstr(empty, "") != nullptr);
    static_assert(cstrstr(a, "a") != nullptr); // Single character match
    static_assert(cstrstr(abc, "abc") != nullptr); // Full string match

    // One string is prefix of another
    static_assert(cstrstr(abc, "abcd") == nullptr); // "abc" doesn't contain "abcd"
    static_assert(cstrstr(abcd, "abc") != nullptr); // "abcd" contains "abc"
    static_assert(cstrstr(empty, "a") == nullptr); // Empty doesn't contain "a"
    static_assert(cstrstr(a, "") != nullptr); // "a" contains empty string

    // Different lengths, same prefix
    static_assert(cstrstr(hello, "helloworld") == nullptr); // "hello" doesn't contain "helloworld"
    static_assert(cstrstr(helloWorld, "hello") != nullptr); // "helloworld" contains "hello"

    // Compare with std::strstr
    REQUIRE(cstrstr(empty, "") == std::strstr(empty, ""));
    REQUIRE(cstrstr(a, "a") == std::strstr(a, "a"));
    REQUIRE(cstrstr(abc, "abc") == std::strstr(abc, "abc"));

    REQUIRE(cstrstr(abc, "abcd") == std::strstr(abc, "abcd"));
    REQUIRE(cstrstr(abcd, "abc") == std::strstr(abcd, "abc"));
    REQUIRE(cstrstr(empty, "a") == std::strstr(empty, "a"));
    REQUIRE(cstrstr(a, "") == std::strstr(a, ""));

    REQUIRE(cstrstr(hello, "helloworld") == std::strstr(hello, "helloworld"));
    REQUIRE(cstrstr(helloWorld, "hello") == std::strstr(helloWorld, "hello"));
  }

  SECTION("Constexpr operations") {
    constexpr const char * helloWorld = "Hello World";
    constexpr const char * word1 = "World";
    constexpr const char * word2 = "Universe";
    constexpr const char * word3 = "Test";
    constexpr const char * abc = "ABC";

    constexpr const char * result1 = cstrstr(helloWorld, word1);
    constexpr const char * result2 = cstrstr(helloWorld, word2);
    constexpr const char * result3 = cstrstr(word3, "es");
    constexpr const char * result4 = cstrstr(abc, "B");

    static_assert(result1 != nullptr);
    static_assert(result2 == nullptr);
    static_assert(result3 != nullptr);
    static_assert(result4 != nullptr);

    // Complex compile-time checks
    static_assert(cstrstr(helloWorld, "Hello") != nullptr);
    static_assert(cstrstr(helloWorld, "xyz") == nullptr);
    static_assert(cstrstr(word3, "Test") != nullptr);
    static_assert(cstrstr(word3, "Fail") == nullptr);
    static_assert(cstrstr(abc, "ABC") != nullptr);
    static_assert(cstrstr(abc, "XYZ") == nullptr);

    // Compare with std::strstr
    REQUIRE(result1 == std::strstr(helloWorld, word1));
    REQUIRE(result2 == std::strstr(helloWorld, word2));
    REQUIRE(result3 == std::strstr(word3, "es"));
    REQUIRE(result4 == std::strstr(abc, "B"));

    REQUIRE(cstrstr(helloWorld, "Hello") == std::strstr(helloWorld, "Hello"));
    REQUIRE(cstrstr(helloWorld, "xyz") == std::strstr(helloWorld, "xyz"));
    REQUIRE(cstrstr(word3, "Test") == std::strstr(word3, "Test"));
    REQUIRE(cstrstr(word3, "Fail") == std::strstr(word3, "Fail"));
    REQUIRE(cstrstr(abc, "ABC") == std::strstr(abc, "ABC"));
    REQUIRE(cstrstr(abc, "XYZ") == std::strstr(abc, "XYZ"));
  }

  SECTION("Long strings") {
    constexpr const char * longHaystack = "This is a very long string for performance testing";
    constexpr const char * needle1 = "very long";
    constexpr const char * needle2 = "performance";
    constexpr const char * needle3 = "testing";
    constexpr const char * needle4 = "not found";

    static_assert(cstrstr(longHaystack, needle1) != nullptr);
    static_assert(cstrstr(longHaystack, needle2) != nullptr);
    static_assert(cstrstr(longHaystack, needle3) != nullptr);
    static_assert(cstrstr(longHaystack, needle4) == nullptr);

    // Compare with std::strstr
    REQUIRE(cstrstr(longHaystack, needle1) == std::strstr(longHaystack, needle1));
    REQUIRE(cstrstr(longHaystack, needle2) == std::strstr(longHaystack, needle2));
    REQUIRE(cstrstr(longHaystack, needle3) == std::strstr(longHaystack, needle3));
    REQUIRE(cstrstr(longHaystack, needle4) == std::strstr(longHaystack, needle4));
  }
}

} // namespace toy
