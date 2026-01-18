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

TEST_CASE("FormatString constructor", "[core][format]") {
  SECTION("String without placeholders") {
    constexpr FormatString<> format("Hello World");

    REQUIRE(format.get() == "Hello World");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "Hello World");
  }

  SECTION("Single placeholder") {
    constexpr FormatString<int> format("Value: {}");

    REQUIRE(format.get() == "Value: {}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "Value: {}");
  }

  SECTION("Multiple placeholders") {
    constexpr FormatString<int, float, double> format("{}, {}, {}");

    REQUIRE(format.get() == "{}, {}, {}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "{}, {}, {}");
  }

  SECTION("Escaped braces") {
    constexpr FormatString<> format("{{}}");

    REQUIRE(format.get() == "{{}}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "{{}}");
  }

  SECTION("Mixed placeholders and escaped braces") {
    constexpr FormatString<int, float> format("{{{} and {} and }}");

    REQUIRE(format.get() == "{{{} and {} and }}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "{{{} and {} and }}");
  }

  SECTION("Adjacent placeholders") {
    constexpr FormatString<int, int> format("{}{}");

    REQUIRE(format.get() == "{}{}");

    // Compile-time checks
    STATIC_REQUIRE(format.get() == "{}{}");
  }
}

TEST_CASE("FormatString get method", "[core][format]") {
  SECTION("Returns stored string") {
    constexpr FormatString<> format("Test");

    constexpr const auto result = format.get();

    REQUIRE(result == "Test");

    // Compile-time checks
    STATIC_REQUIRE(result == "Test");
  }

  SECTION("Returns copy of format string") {
    constexpr FormatString<> format("Reference");

    constexpr const auto result1 = format.get();
    constexpr const auto result2 = format.get();

    REQUIRE(result1 == "Reference");
    REQUIRE(result2 == "Reference");
    REQUIRE(result1 == result2);

    // Compile-time checks
    STATIC_REQUIRE(result1 == "Reference");
    STATIC_REQUIRE(result2 == "Reference");
    STATIC_REQUIRE(result1 == result2);
  }
}

TEST_CASE("vformat function", "[core][format]") {
  SECTION("Format without arguments") {
    const auto result = vformat<64>("Hello World");

    REQUIRE(result.c_str() == "Hello World");
    REQUIRE(result.size() == 11);
  }
  /*
    SECTION("Format with single integer argument") {
      FixedString<64> result;
      CStringView formatStr("Value: {}");

      vformat(result, formatStr, 42);

      REQUIRE(result == "Value: 42");
      REQUIRE(result.size() == 9);
    }

    SECTION("Format with multiple integer arguments") {
      FixedString<64> result;
      CStringView formatStr("{}, {}, {}");

      vformat(result, formatStr, 1, 2, 3);

      REQUIRE(result == "1, 2, 3");
      REQUIRE(result.size() == 7);
    }

    SECTION("Format with floating-point arguments") {
      FixedString<64> result;
      CStringView formatStr("Float: {}, Double: {}");

      vformat(result, formatStr, 3.14f, 2.718);

      REQUIRE(result == "Float: 3.14, Double: 2.718");
    }

    SECTION("Format with boolean arguments") {
      FixedString<64> result;
      CStringView formatStr("True: {}, False: {}");

      vformat(result, formatStr, true, false);

      REQUIRE(result == "True: 1, False: 0");
    }

    SECTION("Format with string arguments") {
      FixedString<64> result;
      CStringView formatStr("Hello, {}!");

      vformat(result, formatStr, "World");

      REQUIRE(result == "Hello, World!");
    }

    SECTION("Format with FixedString argument") {
      FixedString<64> result;
      CStringView formatStr("String: {}");
      FixedString<32> arg("test");

      vformat(result, formatStr, arg);

      REQUIRE(result == "String: test");
    }

    SECTION("Format with CStringView argument") {
      FixedString<64> result;
      CStringView formatStr("View: {}");
      CStringView arg("view");

      vformat(result, formatStr, arg);

      REQUIRE(result == "View: view");
    }

    SECTION("Format with escaped braces") {
      FixedString<64> result;
      CStringView formatStr("Escaped: {{}}");

      vformat(result, formatStr);

      REQUIRE(result == "Escaped: {}");
    }

    SECTION("Format with mixed placeholders and escaped braces") {
      FixedString<64> result;
      CStringView formatStr("{{{} and {} and }}");

      vformat(result, formatStr, 42, 3.14f);

      REQUIRE(result == "{42 and 3.14 and }");
    }

    SECTION("Format with adjacent placeholders") {
      FixedString<64> result;
      CStringView formatStr("{}{}");

      vformat(result, formatStr, 1, 2);

      REQUIRE(result == "12");
    }

    SECTION("Format with pointer argument") {
      FixedString<64> result;
      int value = 42;
      CStringView formatStr("Pointer: {}");

      vformat(result, formatStr, static_cast<const void *>(&value));

      REQUIRE(result.starts_with("Pointer: 0x"));
    }

    SECTION("Format with nullptr") {
      FixedString<64> result;
      CStringView formatStr("Null: {}");

      vformat(result, formatStr, nullptr);

      REQUIRE(result == "Null: nullptr");
    }

    SECTION("Format with empty string") {
      FixedString<64> result;
      CStringView formatStr("");

      vformat(result, formatStr);

      REQUIRE(result.empty());
    }

    SECTION("Format with empty string and argument") {
      FixedString<64> result;

      vformat(result, "{}", 42);

      REQUIRE(result == "42");
    }

    SECTION("Format with mixed types") {
      FixedString<64> result;
      CStringView formatStr("Int: {}, Float: {}, Str: {}, Bool: {}");

      vformat(result, formatStr, 42, 3.14f, "test", true);

      REQUIRE(result == "Int: 42, Float: 3.14, Str: test, Bool: 1");
    }

    SECTION("Format with more placeholders than arguments") {
      FixedString<64> result;
      CStringView formatStr("Value: {}, Extra: {}");

      vformat(result, formatStr, 42);

      REQUIRE(result == "Value: 42, Extra: {}");
    }

    SECTION("Format with more arguments than placeholders") {
      FixedString<64> result;

      vformat(result, "Value: {}", 42, 100);

      REQUIRE(result == "Value: 42");
    }

    SECTION("Format clears output string") {
      FixedString<64> result("Previous content");
      CStringView formatStr("New: {}");

      vformat(result, formatStr, 42);

      REQUIRE(result == "New: 42");
      REQUIRE(result.size() == 6);
    }
  */
}

} // namespace toy
