//
// Copyright (c) 2026 Toyman Interactive
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
/*!
  \file   assertion.inl
  \brief  Inline implementation for the runner's precondition check.

  \note Included by assertion.hpp only; do not include this file directly.
*/

namespace toy::test::detail {

[[noreturn]] inline void assertionFailed([[maybe_unused]] const char * expression,
                                         [[maybe_unused]] const char * message) noexcept {
  // A trap intrinsic is the only stop available here: <cassert> and <cstdlib> are hosted headers, and the runner must
  // build on targets that ship neither.
#if defined(_MSC_VER) && !defined(__clang__)
  __debugbreak();
#else  // defined(_MSC_VER) && !defined(__clang__)
  __builtin_trap();
#endif // defined(_MSC_VER) && !defined(__clang__)
}

} // namespace toy::test::detail
