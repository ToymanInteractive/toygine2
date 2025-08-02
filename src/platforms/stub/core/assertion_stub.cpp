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

#include "core.hpp"

namespace toygine {
  namespace assertion {

    void initialize() {}

    void deInitialize() {}

    void setCallback(AssertCallback TOY_UNUSED(assertCallback), StackWalkCallback TOY_UNUSED(stackWalkCallback)) {}

#ifdef _DEBUG

    void assertion(char const * TOY_UNUSED(code), char const * TOY_UNUSED(message), char const * TOY_UNUSED(fileName),
                   char const * TOY_UNUSED(functionName), std::size_t TOY_UNUSED(lineNumber)) {}

#endif // _DEBUG

  } // namespace assertion
} // namespace toygine
