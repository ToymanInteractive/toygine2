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
/*
#include <doctest/doctest.h>

#include "core.hpp"

namespace toy {

// cstrcmp: compile-time string comparison.
TEST_CASE("core/constexpr_utils/cstrcmp_function") {
  // Equal strings compare zero.
  SUBCASE("equal_strings") {
    constexpr const char * str1 = "Hello";
    constexpr const char * str2 = "Hello";
    constexpr const char * empty1 = "";
    constexpr const char * empty2 = "";

    REQUIRE(cstrcmp(str1, str2) == std::strcmp(str1, str2));
    REQUIRE(cstrcmp(empty1, empty2) == std::strcmp(empty1, empty2));
    REQUIRE(cstrcmp("Test", "Test") == std::strcmp("Test", "Test"));
    REQUIRE(cstrcmp("", "") == std::strcmp("", ""));

    static_assert(cstrcmp(str1, str2) == 0, "equal strings must compare 0");
    static_assert(cstrcmp(empty1, empty2) == 0, "empty strings must compare 0");
    static_assert(cstrcmp("Test", "Test") == 0, "literal equal must compare 0");
    static_assert(cstrcmp("", "") == 0, "empty literals must compare 0");
  }

  // Different strings compare less or greater.
  SUBCASE("different_strings") {
    constexpr const char * str1 = "Hello";
    constexpr const char * str2 = "World";
    constexpr const char * str3 = "Hi";
    constexpr const char * str4 = "Hello World";

    REQUIRE(cstrcmp(str1, str2) == std::strcmp(str1, str2));
    REQUIRE(cstrcmp(str2, str1) == std::strcmp(str2, str1));
    REQUIRE(cstrcmp(str1, str3) == std::strcmp(str1, str3));
    REQUIRE(cstrcmp(str3, str1) == std::strcmp(str3, str1));
    REQUIRE(cstrcmp(str1, str4) == std::strcmp(str1, str4));
    REQUIRE(cstrcmp(str4, str1) == std::strcmp(str4, str1));

    static_assert(cstrcmp(str1, str2) < 0, "Hello before World");
    static_assert(cstrcmp(str2, str1) > 0, "World after Hello");
    static_assert(cstrcmp(str1, str3) < 0, "Hello before Hi");
    static_assert(cstrcmp(str3, str1) > 0, "Hi after Hello");
    static_assert(cstrcmp(str1, str4) < 0, "prefix before full string");
    static_assert(cstrcmp(str4, str1) > 0, "full string after prefix");
  }

  // Empty string comparisons.
  SUBCASE("empty_string_comparisons") {
    constexpr const char * empty = "";
    constexpr const char * nonEmpty = "Test";

    REQUIRE(cstrcmp(empty, empty) == std::strcmp(empty, empty));
    REQUIRE(cstrcmp(empty, nonEmpty) == std::strcmp(empty, nonEmpty));
    REQUIRE(cstrcmp(nonEmpty, empty) == std::strcmp(nonEmpty, empty));

    static_assert(cstrcmp(empty, empty) == 0, "empty equals empty");
    static_assert(cstrcmp(empty, nonEmpty) < 0, "empty before non-empty");
    static_assert(cstrcmp(nonEmpty, empty) > 0, "non-empty after empty");
  }

  // Single-character strings.
  SUBCASE("single_character_strings") {
    constexpr const char * a = "A";
    constexpr const char * b = "B";
    constexpr const char * z = "Z";

    REQUIRE(cstrcmp(a, a) == std::strcmp(a, a));
    REQUIRE(cstrcmp(a, b) == std::strcmp(a, b));
    REQUIRE(cstrcmp(b, a) == std::strcmp(b, a));
    REQUIRE(cstrcmp(a, z) == std::strcmp(a, z));
    REQUIRE(cstrcmp(z, a) == std::strcmp(z, a));
    REQUIRE(cstrcmp(b, z) == std::strcmp(b, z));
    REQUIRE(cstrcmp(z, b) == std::strcmp(z, b));

    static_assert(cstrcmp(a, a) == 0, "same char equal");
    static_assert(cstrcmp(a, b) < 0, "A before B");
    static_assert(cstrcmp(b, a) > 0, "B after A");
    static_assert(cstrcmp(a, z) < 0, "A before Z");
    static_assert(cstrcmp(z, a) > 0, "Z after A");
    static_assert(cstrcmp(b, z) < 0, "B before Z");
    static_assert(cstrcmp(z, b) > 0, "Z after B");
  }

  // Case sensitivity; ASCII ordering.
  SUBCASE("case_sensitivity") {
    constexpr const char * lower = "hello";
    constexpr const char * upper = "HELLO";
    constexpr const char * mixed = "Hello";

    REQUIRE(cstrcmp(lower, upper) == std::strcmp(lower, upper));
    REQUIRE(cstrcmp(upper, lower) == std::strcmp(upper, lower));
    REQUIRE(cstrcmp(lower, mixed) == std::strcmp(lower, mixed));
    REQUIRE(cstrcmp(mixed, lower) == std::strcmp(mixed, lower));
    REQUIRE(cstrcmp(upper, mixed) == std::strcmp(upper, mixed));
    REQUIRE(cstrcmp(mixed, upper) == std::strcmp(mixed, upper));

    static_assert(cstrcmp(lower, upper) > 0, "lower after upper in ASCII");
    static_assert(cstrcmp(upper, lower) < 0, "upper before lower in ASCII");
    static_assert(cstrcmp(lower, mixed) > 0, "lower after mixed in ASCII");
    static_assert(cstrcmp(mixed, lower) < 0, "mixed before lower in ASCII");
    static_assert(cstrcmp(upper, mixed) < 0, "upper before mixed in ASCII");
    static_assert(cstrcmp(mixed, upper) > 0, "mixed after upper in ASCII");
  }

  // Special characters (newline, tab, space, punctuation).
  SUBCASE("special_characters") {
    constexpr const char * newline = "Line1\nLine2";
    constexpr const char * tab = "Col1\tCol2";
    constexpr const char * space = "Hello World";
    constexpr const char * punct = "Hello, World!";

    REQUIRE(cstrcmp(newline, newline) == std::strcmp(newline, newline));
    REQUIRE(cstrcmp(tab, tab) == std::strcmp(tab, tab));
    REQUIRE(cstrcmp(space, space) == std::strcmp(space, space));
    REQUIRE(cstrcmp(punct, punct) == std::strcmp(punct, punct));
    REQUIRE(cstrcmp(newline, tab) == std::strcmp(newline, tab));
    REQUIRE(cstrcmp(tab, space) == std::strcmp(tab, space));
    REQUIRE(cstrcmp(space, punct) == std::strcmp(space, punct));

    static_assert(cstrcmp(newline, newline) == 0, "newline equal");
    static_assert(cstrcmp(tab, tab) == 0, "tab equal");
    static_assert(cstrcmp(space, space) == 0, "space equal");
    static_assert(cstrcmp(punct, punct) == 0, "punct equal");
    static_assert(cstrcmp(newline, tab) != 0, "newline not tab");
    static_assert(cstrcmp(tab, space) != 0, "tab not space");
    static_assert(cstrcmp(space, punct) != 0, "space not punct");
  }

  // Unicode and emoji content.
  SUBCASE("unicode_content") {
    constexpr const char * unicode1 = "Привет";
    constexpr const char * unicode2 = "Мир";
    constexpr const char * unicode3 = "Привет";
    constexpr const char * emoji = "Hello 🌍";

    REQUIRE(cstrcmp(unicode1, unicode3) == std::strcmp(unicode1, unicode3));
    REQUIRE(cstrcmp(unicode1, unicode2) == std::strcmp(unicode1, unicode2));
    REQUIRE(cstrcmp(unicode2, unicode1) == std::strcmp(unicode2, unicode1));
    REQUIRE(cstrcmp(unicode1, emoji) == std::strcmp(unicode1, emoji));
    REQUIRE(cstrcmp(emoji, unicode1) == std::strcmp(emoji, unicode1));

    static_assert(cstrcmp(unicode1, unicode3) == 0, "same unicode equal");
    static_assert(cstrcmp(unicode1, unicode2) != 0, "different unicode not equal");
    static_assert(cstrcmp(unicode2, unicode1) != 0, "different unicode not equal reverse");
    static_assert(cstrcmp(unicode1, emoji) != 0, "unicode not equal emoji");
    static_assert(cstrcmp(emoji, unicode1) != 0, "emoji not equal unicode");
  }

  // Numeric string comparisons.
  SUBCASE("numeric_strings") {
    constexpr const char * num1 = "123";
    constexpr const char * num2 = "456";
    constexpr const char * num3 = "123";
    constexpr const char * num4 = "12";
    constexpr const char * num5 = "1234";

    REQUIRE(cstrcmp(num1, num3) == std::strcmp(num1, num3));
    REQUIRE(cstrcmp(num1, num2) == std::strcmp(num1, num2));
    REQUIRE(cstrcmp(num2, num1) == std::strcmp(num2, num1));
    REQUIRE(cstrcmp(num1, num4) == std::strcmp(num1, num4));
    REQUIRE(cstrcmp(num4, num1) == std::strcmp(num4, num1));
    REQUIRE(cstrcmp(num1, num5) == std::strcmp(num1, num5));
    REQUIRE(cstrcmp(num5, num1) == std::strcmp(num5, num1));

    static_assert(cstrcmp(num1, num3) == 0, "same numeric equal");
    static_assert(cstrcmp(num1, num2) < 0, "123 before 456");
    static_assert(cstrcmp(num2, num1) > 0, "456 after 123");
    static_assert(cstrcmp(num1, num4) > 0, "123 after 12");
    static_assert(cstrcmp(num4, num1) < 0, "12 before 123");
    static_assert(cstrcmp(num1, num5) < 0, "123 before 1234");
    static_assert(cstrcmp(num5, num1) > 0, "1234 after 123");
  }

  // Edge cases: empty, prefix, extended.
  SUBCASE("edge_cases") {
    REQUIRE(cstrcmp("", "") == std::strcmp("", ""));
    REQUIRE(cstrcmp("a", "a") == std::strcmp("a", "a"));
    REQUIRE(cstrcmp("abc", "abc") == std::strcmp("abc", "abc"));

    REQUIRE(cstrcmp("abc", "abcd") == std::strcmp("abc", "abcd"));
    REQUIRE(cstrcmp("abcd", "abc") == std::strcmp("abcd", "abc"));
    REQUIRE(cstrcmp("", "a") == std::strcmp("", "a"));
    REQUIRE(cstrcmp("a", "") == std::strcmp("a", ""));

    REQUIRE(cstrcmp("hello", "helloworld") == std::strcmp("hello", "helloworld"));
    REQUIRE(cstrcmp("helloworld", "hello") == std::strcmp("helloworld", "hello"));

    static_assert(cstrcmp("", "") == 0, "empty equal");
    static_assert(cstrcmp("a", "a") == 0, "single char equal");
    static_assert(cstrcmp("abc", "abc") == 0, "literal equal");

    static_assert(cstrcmp("abc", "abcd") < 0, "prefix before extended");
    static_assert(cstrcmp("abcd", "abc") > 0, "extended after prefix");
    static_assert(cstrcmp("", "a") < 0, "empty before single");
    static_assert(cstrcmp("a", "") > 0, "single after empty");

    static_assert(cstrcmp("hello", "helloworld") < 0, "prefix before longer");
    static_assert(cstrcmp("helloworld", "hello") > 0, "longer after prefix");
  }

  // Constexpr comparison results.
  SUBCASE("constexpr_operations") {
    constexpr const char * str1 = "Test";
    constexpr const char * str2 = "Test";
    constexpr const char * str3 = "Different";

    constexpr int eq = cstrcmp(str1, str2);
    constexpr int ne = cstrcmp(str1, str3);
    constexpr int lt = cstrcmp("A", "B");
    constexpr int gt = cstrcmp("B", "A");

    REQUIRE(eq == std::strcmp(str1, str2));
    REQUIRE(ne == std::strcmp(str1, str3));
    REQUIRE(lt == std::strcmp("A", "B"));
    REQUIRE(gt == std::strcmp("B", "A"));

    static_assert(eq == 0, "constexpr equal must be 0");
    static_assert(ne != 0, "constexpr not equal must be non-zero");
    static_assert(lt < 0, "constexpr less must be negative");
    static_assert(gt > 0, "constexpr greater must be positive");
  }

  // Long string comparison.
  SUBCASE("long_strings") {
    constexpr const char * long1 = "This is a very long string for performance testing";
    constexpr const char * long2 = "This is a very long string for performance testing";
    constexpr const char * long3 = "This is a very long string for performance testing!";

    REQUIRE(cstrcmp(long1, long2) == std::strcmp(long1, long2));
    REQUIRE(cstrcmp(long1, long3) == std::strcmp(long1, long3));
    REQUIRE(cstrcmp(long3, long1) == std::strcmp(long3, long1));

    static_assert(cstrcmp(long1, long2) == 0, "long equal");
    static_assert(cstrcmp(long1, long3) < 0, "long less");
    static_assert(cstrcmp(long3, long1) > 0, "long greater");
  }
}

// cstrchr: compile-time character search.
TEST_CASE("core/constexpr_utils/cstrchr_function") {
  // Basic character search; first occurrence.
  SUBCASE("basic_character_search") {
    constexpr const char * str = "Hello World";
    constexpr char ch1 = 'H';
    constexpr char ch2 = 'o';
    constexpr char ch3 = 'l';
    constexpr char ch4 = 'd';
    constexpr char ch5 = 'z';

    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));
    REQUIRE(cstrchr(str, ch5) == std::strchr(str, ch5));

    static_assert(cstrchr(str, ch1) == str, "H at position 0");
    static_assert(cstrchr(str, ch2) == str + 4, "o at position 4");
    static_assert(cstrchr(str, ch3) == str + 2, "l at position 2");
    static_assert(cstrchr(str, ch4) == str + 10, "d at position 10");
    static_assert(cstrchr(str, ch5) == nullptr, "z not found");
  }

  // Character not in string returns nullptr.
  SUBCASE("character_not_found") {
    constexpr const char * str = "Hello World";
    constexpr char ch1 = 'x';
    constexpr char ch2 = 'Z';
    constexpr char ch3 = '9';
    constexpr char ch4 = '@';

    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));

    static_assert(cstrchr(str, ch1) == nullptr, "x not found");
    static_assert(cstrchr(str, ch2) == nullptr, "Z not found");
    static_assert(cstrchr(str, ch3) == nullptr, "9 not found");
    static_assert(cstrchr(str, ch4) == nullptr, "@ not found");
  }

  // Empty string; only null terminator match.
  SUBCASE("empty_string") {
    constexpr const char * emptyStr = "";
    constexpr char ch1 = 'a';
    constexpr char ch2 = '\0';

    REQUIRE(cstrchr(emptyStr, ch1) == std::strchr(emptyStr, ch1));
    REQUIRE(cstrchr(emptyStr, ch2) == std::strchr(emptyStr, ch2));

    static_assert(cstrchr(emptyStr, ch1) == nullptr, "char not in empty");
    static_assert(cstrchr(emptyStr, ch2) == emptyStr, "null terminator in empty");
  }

  // Single-character string.
  SUBCASE("single_character_string") {
    constexpr const char * singleChar = "A";
    constexpr char ch1 = 'A';
    constexpr char ch2 = 'B';
    constexpr char ch3 = 'a';

    REQUIRE(cstrchr(singleChar, ch1) == std::strchr(singleChar, ch1));
    REQUIRE(cstrchr(singleChar, ch2) == std::strchr(singleChar, ch2));
    REQUIRE(cstrchr(singleChar, ch3) == std::strchr(singleChar, ch3));

    static_assert(cstrchr(singleChar, ch1) == singleChar, "match single char");
    static_assert(cstrchr(singleChar, ch2) == nullptr, "B not in A");
    static_assert(cstrchr(singleChar, ch3) == nullptr, "lowercase not in A");
  }

  // Case sensitivity; exact match only.
  SUBCASE("case_sensitivity") {
    constexpr const char * str = "Hello World";
    constexpr char ch1 = 'h';
    constexpr char ch2 = 'H';
    constexpr char ch3 = 'w';
    constexpr char ch4 = 'W';

    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));

    static_assert(cstrchr(str, ch1) == nullptr, "lowercase h not found");
    static_assert(cstrchr(str, ch2) == str, "uppercase H at start");
    static_assert(cstrchr(str, ch3) == nullptr, "lowercase w not found");
    static_assert(cstrchr(str, ch4) == str + 6, "uppercase W at 6");
  }

  // Repeated characters; first occurrence returned.
  SUBCASE("repeated_characters") {
    constexpr const char * str = "ababab";
    constexpr char ch1 = 'a';
    constexpr char ch2 = 'b';
    constexpr char ch3 = 'c';

    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));

    static_assert(cstrchr(str, ch1) == str, "first a at 0");
    static_assert(cstrchr(str, ch2) == str + 1, "first b at 1");
    static_assert(cstrchr(str, ch3) == nullptr, "c not found");
  }

  // Special characters (newline, tab, exclamation).
  SUBCASE("special_characters") {
    constexpr const char * str = "Hello\n\tWorld!";
    constexpr char ch1 = '\n';
    constexpr char ch2 = '\t';
    constexpr char ch3 = '!';
    constexpr char ch4 = ' ';

    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));

    static_assert(cstrchr(str, ch1) == str + 5, "newline at 5");
    static_assert(cstrchr(str, ch2) == str + 6, "tab at 6");
    static_assert(cstrchr(str, ch3) == str + 12, "exclamation at 12");
    static_assert(cstrchr(str, ch4) == nullptr, "space not in str");
  }

  // Unicode content.
  SUBCASE("unicode_content") {
    constexpr const char * str = "Hello 世界";
    constexpr char ch1 = 'H';
    constexpr char ch2 = 'z';

    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));

    static_assert(cstrchr(str, ch1) == str, "H at start");
    static_assert(cstrchr(str, ch2) == nullptr, "z not in unicode");
  }

  // Numeric character search.
  SUBCASE("numeric_content") {
    constexpr const char * str = "12345";
    constexpr char ch1 = '1';
    constexpr char ch2 = '3';
    constexpr char ch3 = '5';
    constexpr char ch4 = '6';

    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));

    static_assert(cstrchr(str, ch1) == str, "1 at 0");
    static_assert(cstrchr(str, ch2) == str + 2, "3 at 2");
    static_assert(cstrchr(str, ch3) == str + 4, "5 at 4");
    static_assert(cstrchr(str, ch4) == nullptr, "6 not found");
  }

  SUBCASE("mixed_content") {
    constexpr const char * str = "123Hello456";
    constexpr char ch1 = '1';
    constexpr char ch2 = 'H';
    constexpr char ch3 = 'o';
    constexpr char ch4 = '6';
    constexpr char ch5 = 'z';

    REQUIRE(cstrchr(str, ch1) == std::strchr(str, ch1));
    REQUIRE(cstrchr(str, ch2) == std::strchr(str, ch2));
    REQUIRE(cstrchr(str, ch3) == std::strchr(str, ch3));
    REQUIRE(cstrchr(str, ch4) == std::strchr(str, ch4));
    REQUIRE(cstrchr(str, ch5) == std::strchr(str, ch5));

    static_assert(cstrchr(str, ch1) == str, "1 at 0");
    static_assert(cstrchr(str, ch2) == str + 3, "H at 3");
    static_assert(cstrchr(str, ch3) == str + 7, "o at 7");
    static_assert(cstrchr(str, ch4) == str + 10, "6 at 10");
    static_assert(cstrchr(str, ch5) == nullptr, "z not found");
  }

  // Position-specific search; literal chars.
  SUBCASE("position_specific_search") {
    constexpr const char * str = "Hello World";

    REQUIRE(cstrchr(str, 'H') == std::strchr(str, 'H'));
    REQUIRE(cstrchr(str, 'e') == std::strchr(str, 'e'));
    REQUIRE(cstrchr(str, 'l') == std::strchr(str, 'l'));
    REQUIRE(cstrchr(str, 'o') == std::strchr(str, 'o'));
    REQUIRE(cstrchr(str, 'd') == std::strchr(str, 'd'));

    static_assert(cstrchr(str, 'H') == str, "H at start");
    static_assert(cstrchr(str, 'e') == str + 1, "e at 1");
    static_assert(cstrchr(str, 'l') == str + 2, "first l at 2");
    static_assert(cstrchr(str, 'o') == str + 4, "first o at 4");
    static_assert(cstrchr(str, 'd') == str + 10, "d at end");
    static_assert(cstrchr(str, 'l') == str + 2, "first l not last");
  }

  // Edge cases: null, match, no match.
  SUBCASE("edge_cases") {
    constexpr const char * empty = "";
    constexpr const char * ch1 = "a";

    REQUIRE(cstrchr(empty, '\0') == std::strchr(empty, '\0'));
    REQUIRE(cstrchr(ch1, '\0') == std::strchr(ch1, '\0'));

    REQUIRE(cstrchr(ch1, 'a') == std::strchr(ch1, 'a'));
    REQUIRE(cstrchr(ch1, 'b') == std::strchr(ch1, 'b'));

    static_assert(cstrchr(empty, '\0') != nullptr, "null in empty");
    static_assert(cstrchr(ch1, '\0') != nullptr, "null in single");

    static_assert(cstrchr(ch1, 'a') != nullptr, "match single");
    static_assert(cstrchr(ch1, 'b') == nullptr, "no match single");
  }

  SUBCASE("constexpr_operations") {
    constexpr const char * helloWorld = "Hello World";
    constexpr const char * test = "Test";
    constexpr const char * abc = "ABC";
    constexpr char ch1 = 'o';
    constexpr char ch2 = 'z';

    constexpr const char * result1 = cstrchr(helloWorld, ch1);
    constexpr const char * result2 = cstrchr(helloWorld, ch2);
    constexpr const char * result3 = cstrchr(test, 'e');
    constexpr const char * result4 = cstrchr(abc, 'B');

    REQUIRE(result1 != nullptr);
    REQUIRE(result2 == nullptr);
    REQUIRE(result3 != nullptr);
    REQUIRE(result4 != nullptr);

    static_assert(result1 != nullptr, "cstrchr(helloWorld, H) must be non-null");
    static_assert(result2 == nullptr, "cstrchr(helloWorld, z) must be null");
    static_assert(result3 != nullptr, "cstrchr(test, e) must be non-null");
    static_assert(result4 != nullptr, "cstrchr(abc, B) must be non-null");

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

    static_assert(cstrchr(helloWorld, 'H') != nullptr, "H in helloWorld");
    static_assert(cstrchr(helloWorld, 'z') == nullptr, "z not in helloWorld");
    static_assert(cstrchr(test, 'T') != nullptr, "T in test");
    static_assert(cstrchr(test, 'Z') == nullptr, "Z not in test");
    static_assert(cstrchr(abc, 'A') != nullptr, "A in abc");
    static_assert(cstrchr(abc, 'Z') == nullptr, "Z not in abc");
  }

  SUBCASE("long_strings") {
    constexpr const char * longStr = "This is a very long string for performance testing";
    constexpr char ch1 = 'v';
    constexpr char ch2 = 'p';
    constexpr char ch3 = 't';
    constexpr char ch4 = 'z';

    REQUIRE(cstrchr(longStr, ch1) == std::strchr(longStr, ch1));
    REQUIRE(cstrchr(longStr, ch2) == std::strchr(longStr, ch2));
    REQUIRE(cstrchr(longStr, ch3) == std::strchr(longStr, ch3));
    REQUIRE(cstrchr(longStr, ch4) == std::strchr(longStr, ch4));

    static_assert(cstrchr(longStr, ch1) != nullptr, "v in long");
    static_assert(cstrchr(longStr, ch2) != nullptr, "p in long");
    static_assert(cstrchr(longStr, ch3) != nullptr, "t in long");
    static_assert(cstrchr(longStr, ch4) == nullptr, "z not in long");
  }
}

// cstrpbrk: compile-time character set search.
TEST_CASE("core/constexpr_utils/cstrpbrk_function") {
  // Basic character set search; null accept/str.
  SUBCASE("basic_character_set_search") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "aeiou";
    constexpr const char * accept2 = "H";
    constexpr const char * accept3 = "d";
    constexpr const char * accept4 = "xyz";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));

    REQUIRE(cstrpbrk(str, nullptr) == nullptr);
    REQUIRE(cstrpbrk(nullptr, accept1) == nullptr);
    REQUIRE(cstrpbrk(nullptr, nullptr) == nullptr);

    static_assert(cstrpbrk(str, accept1) != nullptr, "vowel in str");
    static_assert(cstrpbrk(str, accept2) != nullptr, "H in str");
    static_assert(cstrpbrk(str, accept3) != nullptr, "d in str");
    static_assert(cstrpbrk(str, accept4) == nullptr, "xyz not in str");

    static_assert(cstrpbrk(str, nullptr) == nullptr, "null accept");
    static_assert(cstrpbrk(nullptr, accept1) == nullptr, "null str");
    static_assert(cstrpbrk(nullptr, nullptr) == nullptr, "null both");
  }

  // Single character in accept set.
  SUBCASE("single_character_in_accept_set") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "e";
    constexpr const char * accept2 = "o";
    constexpr const char * accept3 = "W";
    constexpr const char * accept4 = "Z";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));

    static_assert(cstrpbrk(str, accept1) != nullptr, "e in str");
    static_assert(cstrpbrk(str, accept2) != nullptr, "o in str");
    static_assert(cstrpbrk(str, accept3) != nullptr, "W in str");
    static_assert(cstrpbrk(str, accept4) == nullptr, "Z not in str");
  }

  // Multiple characters in accept set.
  SUBCASE("multiple_characters_in_accept_set") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "aeiou";
    constexpr const char * accept2 = "Hl";
    constexpr const char * accept3 = "Wrd";
    constexpr const char * accept4 = "xyz";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));

    static_assert(cstrpbrk(str, accept1) != nullptr, "vowel in str");
    static_assert(cstrpbrk(str, accept2) != nullptr, "Hl in str");
    static_assert(cstrpbrk(str, accept3) != nullptr, "Wrd in str");
    static_assert(cstrpbrk(str, accept4) == nullptr, "xyz not in str");
  }

  // Empty string and empty accept set.
  SUBCASE("empty_strings") {
    constexpr const char * emptyStr = "";
    constexpr const char * accept1 = "aeiou";
    constexpr const char * accept2 = "";

    REQUIRE(cstrpbrk(emptyStr, accept1) == std::strpbrk(emptyStr, accept1));
    REQUIRE(cstrpbrk(emptyStr, accept2) == std::strpbrk(emptyStr, accept2));

    static_assert(cstrpbrk(emptyStr, accept1) == nullptr, "empty str no match");
    static_assert(cstrpbrk(emptyStr, accept2) == nullptr, "empty str no null");
  }

  // First character match in accept set.
  SUBCASE("first_character_match") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "H";
    constexpr const char * accept2 = "Hel";
    constexpr const char * accept3 = "Hl";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));

    static_assert(cstrpbrk(str, accept1) != nullptr, "first char match");
    static_assert(cstrpbrk(str, accept2) != nullptr, "first char match 2");
    static_assert(cstrpbrk(str, accept3) != nullptr, "first char match 3");
  }

  // Last character match in accept set.
  SUBCASE("last_character_match") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "d";
    constexpr const char * accept2 = "ld";
    constexpr const char * accept3 = "World";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));

    static_assert(cstrpbrk(str, accept1) != nullptr, "d in str");
    static_assert(cstrpbrk(str, accept2) != nullptr, "ld in str");
    static_assert(cstrpbrk(str, accept3) != nullptr, "World in str");
  }

  // Case sensitivity in accept set.
  SUBCASE("case_sensitivity") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "hello";
    constexpr const char * accept2 = "HELLO";
    constexpr const char * accept3 = "Hello";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));

    static_assert(cstrpbrk(str, accept1) != nullptr, "lowercase in str");
    static_assert(cstrpbrk(str, accept2) != nullptr, "uppercase in str");
    static_assert(cstrpbrk(str, accept3) != nullptr, "mixed in str");
  }

  // Special characters in accept set (cstrpbrk).
  SUBCASE("special_characters") {
    constexpr const char * str = "Hello, World!";
    constexpr const char * accept1 = ",!";
    constexpr const char * accept2 = ".,";
    constexpr const char * accept3 = "!?";
    constexpr const char * accept4 = "xyz";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));

    static_assert(cstrpbrk(str, accept1) != nullptr, ",! in str");
    static_assert(cstrpbrk(str, accept2) != nullptr, "., in str");
    static_assert(cstrpbrk(str, accept3) != nullptr, "!? in str");
    static_assert(cstrpbrk(str, accept4) == nullptr, "xyz not in str");
  }

  // Numeric characters in accept set.
  SUBCASE("numeric_characters") {
    constexpr const char * str = "Hello123World";
    constexpr const char * accept1 = "123";
    constexpr const char * accept2 = "456";
    constexpr const char * accept3 = "0123456789";
    constexpr const char * accept4 = "abc";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));

    static_assert(cstrpbrk(str, accept1) != nullptr, "digit in str");
    static_assert(cstrpbrk(str, accept2) == nullptr, "456 not in str");
    static_assert(cstrpbrk(str, accept3) != nullptr, "any digit in str");
    static_assert(cstrpbrk(str, accept4) == nullptr, "abc not in str");
  }

  // Whitespace characters in accept set.
  SUBCASE("whitespace_characters") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = " ";
    constexpr const char * accept2 = " \t\n";
    constexpr const char * accept3 = "\t";
    constexpr const char * accept4 = "xyz";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));
    REQUIRE(cstrpbrk(str, accept4) == std::strpbrk(str, accept4));

    static_assert(cstrpbrk(str, accept1) != nullptr, "space in str");
    static_assert(cstrpbrk(str, accept2) != nullptr, "whitespace in str");
    static_assert(cstrpbrk(str, accept3) == nullptr, "tab not in str");
    static_assert(cstrpbrk(str, accept4) == nullptr, "xyz not in str");
  }

  // Repeated characters in accept set.
  SUBCASE("repeated_characters_in_accept_set") {
    constexpr const char * str = "Hello World";
    constexpr const char * accept1 = "lll";
    constexpr const char * accept2 = "HHH";
    constexpr const char * accept3 = "llH";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));

    static_assert(cstrpbrk(str, accept1) != nullptr, "l in str");
    static_assert(cstrpbrk(str, accept2) != nullptr, "H in str");
    static_assert(cstrpbrk(str, accept3) != nullptr, "l or H in str");
  }

  // Long string; character set search (cstrpbrk).
  SUBCASE("long_strings") {
    constexpr const char * longStr = "This is a very long string for performance testing";
    constexpr const char * accept1 = "aeiou";
    constexpr const char * accept2 = "xyz";
    constexpr const char * accept3 = "T";
    constexpr const char * accept4 = "g";

    REQUIRE(cstrpbrk(longStr, accept1) == std::strpbrk(longStr, accept1));
    REQUIRE(cstrpbrk(longStr, accept2) == std::strpbrk(longStr, accept2));
    REQUIRE(cstrpbrk(longStr, accept3) == std::strpbrk(longStr, accept3));
    REQUIRE(cstrpbrk(longStr, accept4) == std::strpbrk(longStr, accept4));

    static_assert(cstrpbrk(longStr, accept1) != nullptr, "vowel in long");
    static_assert(cstrpbrk(longStr, accept2) != nullptr, "xyz in long");
    static_assert(cstrpbrk(longStr, accept3) != nullptr, "T in long");
    static_assert(cstrpbrk(longStr, accept4) != nullptr, "g in long");
  }

  // Edge cases: single char str and accept (cstrpbrk).
  SUBCASE("edge_cases") {
    constexpr const char * str = "A";
    constexpr const char * accept1 = "A";
    constexpr const char * accept2 = "B";
    constexpr const char * accept3 = "AB";

    REQUIRE(cstrpbrk(str, accept1) == std::strpbrk(str, accept1));
    REQUIRE(cstrpbrk(str, accept2) == std::strpbrk(str, accept2));
    REQUIRE(cstrpbrk(str, accept3) == std::strpbrk(str, accept3));

    static_assert(cstrpbrk(str, accept1) != nullptr, "A in A");
    static_assert(cstrpbrk(str, accept2) == nullptr, "B not in A");
    static_assert(cstrpbrk(str, accept3) != nullptr, "A or B in A");
  }
}

// cstrstr: compile-time substring search.
TEST_CASE("core/constexpr_utils/cstrstr_function") {
  // Basic substring search; positions and full match.
  SUBCASE("basic_substring_search") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "World";
    constexpr const char * needle2 = "Hello";
    constexpr const char * needle3 = "lo Wo";
    constexpr const char * needle4 = "Hello World";

    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));

    static_assert(cstrstr(haystack, needle1) != nullptr, "World in haystack");
    static_assert(cstrstr(haystack, needle2) != nullptr, "Hello in haystack");
    static_assert(cstrstr(haystack, needle3) != nullptr, "lo Wo in haystack");
    static_assert(cstrstr(haystack, needle4) != nullptr, "full in haystack");
    static_assert(cstrstr(haystack, needle1) == haystack + 6, "World at 6");
    static_assert(cstrstr(haystack, needle2) == haystack, "Hello at 0");
    static_assert(cstrstr(haystack, needle3) == haystack + 3, "lo Wo at 3");
    static_assert(cstrstr(haystack, needle4) == haystack, "full at 0");
  }

  // Substring not in haystack returns nullptr.
  SUBCASE("substring_not_found") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "Universe";
    constexpr const char * needle2 = "xyz";
    constexpr const char * needle3 = "Hello Universe";
    constexpr const char * needle4 = "World Hello";

    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));

    static_assert(cstrstr(haystack, needle1) == nullptr, "Universe not in haystack");
    static_assert(cstrstr(haystack, needle2) == nullptr, "xyz not in haystack");
    static_assert(cstrstr(haystack, needle3) == nullptr, "Hello Universe not in haystack");
    static_assert(cstrstr(haystack, needle4) == nullptr, "World Hello not in haystack");
  }

  SUBCASE("empty_needle") {
    constexpr const char * haystack1 = "Hello World";
    constexpr const char * haystack2 = "";
    constexpr const char * emptyNeedle = "";

    REQUIRE(cstrstr(haystack1, emptyNeedle) == std::strstr(haystack1, emptyNeedle));
    REQUIRE(cstrstr(haystack2, emptyNeedle) == std::strstr(haystack2, emptyNeedle));

    static_assert(cstrstr(haystack1, emptyNeedle) == haystack1, "empty needle returns haystack");
    static_assert(cstrstr(haystack2, emptyNeedle) == haystack2, "empty needle returns empty");
  }

  // Empty haystack; needle not found or empty needle.
  SUBCASE("empty_haystack") {
    constexpr const char * emptyHaystack = "";
    constexpr const char * needle1 = "Hello";
    constexpr const char * needle2 = "";

    REQUIRE(cstrstr(emptyHaystack, needle1) == std::strstr(emptyHaystack, needle1));
    REQUIRE(cstrstr(emptyHaystack, needle2) == std::strstr(emptyHaystack, needle2));

    static_assert(cstrstr(emptyHaystack, needle1) == nullptr, "needle not in empty");
    static_assert(cstrstr(emptyHaystack, needle2) == emptyHaystack, "empty needle in empty");
  }

  // Single-character substring search (cstrstr).
  SUBCASE("single_character_search") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "H";
    constexpr const char * needle2 = "o";
    constexpr const char * needle3 = "l";
    constexpr const char * needle4 = "x";

    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));

    static_assert(cstrstr(haystack, needle1) == haystack, "H at 0");
    static_assert(cstrstr(haystack, needle2) == haystack + 4, "o at 4");
    static_assert(cstrstr(haystack, needle3) == haystack + 2, "first l at 2");
    static_assert(cstrstr(haystack, needle4) == nullptr, "x not found");
  }

  SUBCASE("case_sensitivity") {
    constexpr const char * haystack = "Hello World";
    constexpr const char * needle1 = "hello";
    constexpr const char * needle2 = "WORLD";
    constexpr const char * needle3 = "world";
    constexpr const char * needle4 = "Hello";

    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));

    static_assert(cstrstr(haystack, needle1) == nullptr, "lowercase not found");
    static_assert(cstrstr(haystack, needle2) == nullptr, "uppercase not found");
    static_assert(cstrstr(haystack, needle3) == nullptr, "lowercase world not found");
    static_assert(cstrstr(haystack, needle4) == haystack, "exact match");
  }

  // Repeated patterns; first occurrence (cstrstr).
  SUBCASE("repeated_patterns") {
    constexpr const char * haystack = "ababab";
    constexpr const char * needle1 = "ab";
    constexpr const char * needle2 = "bab";
    constexpr const char * needle3 = "abab";
    constexpr const char * needle4 = "ababab";
    constexpr const char * needle5 = "babab";

    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));

    static_assert(cstrstr(haystack, needle1) == haystack, "ab at 0");
    static_assert(cstrstr(haystack, needle2) == haystack + 1, "bab at 1");
    static_assert(cstrstr(haystack, needle3) == haystack, "abab at 0");
    static_assert(cstrstr(haystack, needle4) == haystack, "ababab at 0");
    static_assert(cstrstr(haystack, needle5) == haystack + 1, "babab at 1");
  }

  // Special characters in substring (cstrstr).
  SUBCASE("special_characters") {
    constexpr const char * haystack = "Hello\n\tWorld!";
    constexpr const char * needle1 = "\n";
    constexpr const char * needle2 = "\t";
    constexpr const char * needle3 = "!";
    constexpr const char * needle4 = "Hello\n";
    constexpr const char * needle5 = "\tWorld";
    constexpr const char * needle6 = "World!";

    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));
    REQUIRE(cstrstr(haystack, needle6) == std::strstr(haystack, needle6));

    static_assert(cstrstr(haystack, needle1) == haystack + 5, "newline at 5");
    static_assert(cstrstr(haystack, needle2) == haystack + 6, "tab at 6");
    static_assert(cstrstr(haystack, needle3) == haystack + 12, "exclamation at 12");
    static_assert(cstrstr(haystack, needle4) == haystack, "Hello newline at 0");
    static_assert(cstrstr(haystack, needle5) == haystack + 6, "tab World at 6");
    static_assert(cstrstr(haystack, needle6) == haystack + 7, "World exclamation at 7");
  }

  // Unicode content in haystack and needle (cstrstr).
  SUBCASE("unicode_content") {
    constexpr const char * haystack = "Hello 世界";
    constexpr const char * needle1 = "世界";
    constexpr const char * needle2 = "Hello 世";
    constexpr const char * needle3 = "界";
    constexpr const char * needle4 = "世";
    constexpr const char * needle5 = "宇宙";

    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));

    static_assert(cstrstr(haystack, needle1) == haystack + 6, "unicode at 6");
    static_assert(cstrstr(haystack, needle2) == haystack, "Hello unicode at 0");
    static_assert(cstrstr(haystack, needle3) == haystack + 9, "char at 9");
    static_assert(cstrstr(haystack, needle4) == haystack + 6, "char at 6");
    static_assert(cstrstr(haystack, needle5) == nullptr, "not in haystack");
  }

  // Numeric substring search (cstrstr).
  SUBCASE("numeric_content") {
    constexpr const char * haystack = "12345";
    constexpr const char * needle1 = "123";
    constexpr const char * needle2 = "345";
    constexpr const char * needle3 = "234";
    constexpr const char * needle4 = "12345";
    constexpr const char * needle5 = "678";

    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));

    static_assert(cstrstr(haystack, needle1) == haystack, "123 at 0");
    static_assert(cstrstr(haystack, needle2) == haystack + 2, "345 at 2");
    static_assert(cstrstr(haystack, needle3) == haystack + 1, "234 at 1");
    static_assert(cstrstr(haystack, needle4) == haystack, "12345 at 0");
    static_assert(cstrstr(haystack, needle5) == nullptr, "678 not found");
  }

  // Mixed alphanumeric substring (cstrstr).
  SUBCASE("mixed_content") {
    constexpr const char * haystack = "123Hello456";
    constexpr const char * needle1 = "123";
    constexpr const char * needle2 = "Hello";
    constexpr const char * needle3 = "456";
    constexpr const char * needle4 = "3Hello4";
    constexpr const char * needle5 = "123Hello456";
    constexpr const char * needle6 = "789";

    REQUIRE(cstrstr(haystack, needle1) == std::strstr(haystack, needle1));
    REQUIRE(cstrstr(haystack, needle2) == std::strstr(haystack, needle2));
    REQUIRE(cstrstr(haystack, needle3) == std::strstr(haystack, needle3));
    REQUIRE(cstrstr(haystack, needle4) == std::strstr(haystack, needle4));
    REQUIRE(cstrstr(haystack, needle5) == std::strstr(haystack, needle5));
    REQUIRE(cstrstr(haystack, needle6) == std::strstr(haystack, needle6));

    static_assert(cstrstr(haystack, needle1) == haystack, "123 at 0");
    static_assert(cstrstr(haystack, needle2) == haystack + 3, "Hello at 3");
    static_assert(cstrstr(haystack, needle3) == haystack + 8, "456 at 8");
    static_assert(cstrstr(haystack, needle4) == haystack + 2, "3Hello4 at 2");
    static_assert(cstrstr(haystack, needle5) == haystack, "full at 0");
    static_assert(cstrstr(haystack, needle6) == nullptr, "789 not found");
  }

  // Position-specific substring search (cstrstr).
  SUBCASE("position_specific_search") {
    constexpr const char * haystack = "Hello World";

    REQUIRE(cstrstr(haystack, "H") == std::strstr(haystack, "H"));
    REQUIRE(cstrstr(haystack, "He") == std::strstr(haystack, "He"));
    REQUIRE(cstrstr(haystack, "Hello") == std::strstr(haystack, "Hello"));

    REQUIRE(cstrstr(haystack, "l") == std::strstr(haystack, "l"));
    REQUIRE(cstrstr(haystack, "ll") == std::strstr(haystack, "ll"));
    REQUIRE(cstrstr(haystack, "lo W") == std::strstr(haystack, "lo W"));

    REQUIRE(cstrstr(haystack, "d") == std::strstr(haystack, "d"));
    REQUIRE(cstrstr(haystack, "ld") == std::strstr(haystack, "ld"));
    REQUIRE(cstrstr(haystack, "World") == std::strstr(haystack, "World"));

    static_assert(cstrstr(haystack, "H") == haystack, "H at 0");
    static_assert(cstrstr(haystack, "He") == haystack, "He at 0");
    static_assert(cstrstr(haystack, "Hello") == haystack, "Hello at 0");

    static_assert(cstrstr(haystack, "l") == haystack + 2, "first l at 2");
    static_assert(cstrstr(haystack, "ll") == haystack + 2, "ll at 2");
    static_assert(cstrstr(haystack, "lo W") == haystack + 3, "lo W at 3");

    static_assert(cstrstr(haystack, "d") == haystack + 10, "d at 10");
    static_assert(cstrstr(haystack, "ld") == haystack + 9, "ld at 9");
    static_assert(cstrstr(haystack, "World") == haystack + 6, "World at 6");
  }

  // Edge cases: empty, prefix, longer needle (cstrstr).
  SUBCASE("edge_cases") {
    constexpr const char * empty = "";
    constexpr const char * a = "a";
    constexpr const char * abc = "abc";
    constexpr const char * abcd = "abcd";
    constexpr const char * hello = "hello";
    constexpr const char * helloWorld = "hello world";

    REQUIRE(cstrstr(empty, "") == std::strstr(empty, ""));
    REQUIRE(cstrstr(a, "a") == std::strstr(a, "a"));
    REQUIRE(cstrstr(abc, "abc") == std::strstr(abc, "abc"));

    REQUIRE(cstrstr(abc, "abcd") == std::strstr(abc, "abcd"));
    REQUIRE(cstrstr(abcd, "abc") == std::strstr(abcd, "abc"));
    REQUIRE(cstrstr(empty, "a") == std::strstr(empty, "a"));
    REQUIRE(cstrstr(a, "") == std::strstr(a, ""));

    REQUIRE(cstrstr(hello, "helloworld") == std::strstr(hello, "helloworld"));
    REQUIRE(cstrstr(helloWorld, "hello") == std::strstr(helloWorld, "hello"));

    static_assert(cstrstr(empty, "") != nullptr, "empty in empty");
    static_assert(cstrstr(a, "a") != nullptr, "single char match");
    static_assert(cstrstr(abc, "abc") != nullptr, "full match");

    static_assert(cstrstr(abc, "abcd") == nullptr, "longer needle not in haystack");
    static_assert(cstrstr(abcd, "abc") != nullptr, "prefix in haystack");
    static_assert(cstrstr(empty, "a") == nullptr, "needle not in empty");
    static_assert(cstrstr(a, "") != nullptr, "empty needle in haystack");

    static_assert(cstrstr(hello, "helloworld") == nullptr, "longer needle not in short");
    static_assert(cstrstr(helloWorld, "hello") != nullptr, "prefix in haystack");
  }

  SUBCASE("constexpr_operations") {
    constexpr const char * helloWorld = "Hello World";
    constexpr const char * word1 = "World";
    constexpr const char * word2 = "Universe";
    constexpr const char * word3 = "Test";
    constexpr const char * abc = "ABC";

    constexpr const char * result1 = cstrstr(helloWorld, word1);
    constexpr const char * result2 = cstrstr(helloWorld, word2);
    constexpr const char * result3 = cstrstr(word3, "es");
    constexpr const char * result4 = cstrstr(abc, "B");

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

    static_assert(result1 != nullptr, "World in Hello World");
    static_assert(result2 == nullptr, "Universe not in Hello World");
    static_assert(result3 != nullptr, "es in Test");
    static_assert(result4 != nullptr, "B in ABC");

    static_assert(cstrstr(helloWorld, "Hello") != nullptr, "Hello in helloWorld");
    static_assert(cstrstr(helloWorld, "xyz") == nullptr, "xyz not in helloWorld");
    static_assert(cstrstr(word3, "Test") != nullptr, "Test in word3");
    static_assert(cstrstr(word3, "Fail") == nullptr, "Fail not in word3");
    static_assert(cstrstr(abc, "ABC") != nullptr, "ABC in abc");
    static_assert(cstrstr(abc, "XYZ") == nullptr, "XYZ not in abc");
  }

  // Long haystack substring search (cstrstr).
  SUBCASE("long_strings") {
    constexpr const char * longHaystack = "This is a very long string for performance testing";
    constexpr const char * needle1 = "very long";
    constexpr const char * needle2 = "performance";
    constexpr const char * needle3 = "testing";
    constexpr const char * needle4 = "not found";

    REQUIRE(cstrstr(longHaystack, needle1) == std::strstr(longHaystack, needle1));
    REQUIRE(cstrstr(longHaystack, needle2) == std::strstr(longHaystack, needle2));
    REQUIRE(cstrstr(longHaystack, needle3) == std::strstr(longHaystack, needle3));
    REQUIRE(cstrstr(longHaystack, needle4) == std::strstr(longHaystack, needle4));

    static_assert(cstrstr(longHaystack, needle1) != nullptr, "very long in haystack");
    static_assert(cstrstr(longHaystack, needle2) != nullptr, "performance in haystack");
    static_assert(cstrstr(longHaystack, needle3) != nullptr, "testing in haystack");
    static_assert(cstrstr(longHaystack, needle4) == nullptr, "not found in haystack");
  }
}

} // namespace toy
*/
