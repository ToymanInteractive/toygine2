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
  \file   info_entry.hpp
  \brief  One message on the test context's info stack.

  Defines \ref toy::test::InfoEntry: the text a test attaches to a failure, with an optional companion number. Stored
  by \ref toy::test::Context and read back by a reporter.

  \note Included by toy_test.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_TESTS_RUNNER_INFO_ENTRY_HPP_
#define INCLUDE_TESTS_RUNNER_INFO_ENTRY_HPP_

namespace toy::test {

/*!
  \struct InfoEntry
  \brief One message on the context's info stack.

  \section usage Usage Example

  \code
  const toy::test::InfoEntry & entry = context.infoAt(0);
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **Access**: O(1) constant time
  * **Memory usage**: one pointer, one 64-bit integer and one flag

  \section safety Safety Guarantees

  * **Type safety**: \ref hasValue states whether \ref value carries meaning
  * **Memory safety**: no ownership; \ref text refers to a string literal
  * **Exception safety**: No operation throws; exceptions are off in the build

  \sa \ref toy::test::Context
*/
struct InfoEntry final {
  const char * text;     ///< Message text.
  long long    value;    ///< Companion value, meaningful when \ref hasValue is \c true.
  bool         hasValue; ///< Whether \ref value carries a number.
};

} // namespace toy::test

#endif // INCLUDE_TESTS_RUNNER_INFO_ENTRY_HPP_
