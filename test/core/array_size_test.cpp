//
// Copyright (c) 2025 Toyman Interactive
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

} // namespace toy
