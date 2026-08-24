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
  \file   failure_record.hpp
  \brief  Record of one failed assertion, as handed to a failure reporter.

  Defines \ref toy::test::FailureRecord: what a reporter cannot recover on its own. Built by
  \ref toy::test::Context on every failing assertion.

  \note Included by context.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_TESTS_RUNNER_FAILURE_RECORD_HPP_
#define INCLUDE_TESTS_RUNNER_FAILURE_RECORD_HPP_

namespace toy::test {

/*!
  \struct FailureRecord
  \brief One failed assertion, as handed to a failure reporter.

  Carries only what the reporter cannot recover on its own. The info stack travels separately, through the
  \ref toy::test::Context passed alongside.

  \section usage Usage Example

  \code
  void report(const toy::test::Context & context, const toy::test::FailureRecord & failure) noexcept;
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **Access**: O(1) constant time
  * **Memory usage**: 4 pointers plus one int

  \section safety Safety Guarantees

  * **Type safety**: aggregate of pointers to string literals and one line number
  * **Memory safety**: no ownership; every pointer refers to a string literal that outlives the run
  * **Exception safety**: No operation throws; exceptions are off in the build

  \sa \ref toy::test::Context
*/
struct FailureRecord final {
  const char * caseName;    ///< Name of the running case.
  const char * subcaseName; ///< Name of the running subcase, or \c nullptr outside one.
  const char * expression;  ///< Source text of the failed expression.
  const char * file;        ///< Source file holding the assertion.
  int          line;        ///< Source line holding the assertion.
};

} // namespace toy::test

#endif // INCLUDE_TESTS_RUNNER_FAILURE_RECORD_HPP_
