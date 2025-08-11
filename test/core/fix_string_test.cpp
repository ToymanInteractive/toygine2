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
