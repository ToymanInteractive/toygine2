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
  \file   context.hpp
  \brief  Per-run state of the built-in test runner.

  Defines \ref toy::test::Context and \ref toy::test::failure_reporter_type: the counters, the per-case verdict and the
  info stack a reporter reads back. Driven by toy::test::runCase() and read back by whichever runner prints the run.

  \note Reached through toy_test.hpp; included directly only by the runner's own headers and by the unit test for this
        type.
*/

#ifndef INCLUDE_TESTS_RUNNER_CONTEXT_HPP_
#define INCLUDE_TESTS_RUNNER_CONTEXT_HPP_

#include <algorithm>
#include <array>
#include <cstddef>

#include "assertion.hpp"
#include "failure_record.hpp"
#include "info_entry.hpp"

namespace toy::test {

class Context;

/*!
  \brief Function invoked for every failed assertion.

  \param context       Context that recorded the failure.
  \param failure       The failed assertion.
  \param reporterData  Pointer handed to the context at construction; \c nullptr when none was given.

  \note A reporter is a plain function pointer, so anything it must remember between calls — a case number, a header
        already printed, a captured record — arrives through \a reporterData rather than through storage of its own.

  \sa \ref toy::test::Context
*/
using failure_reporter_type = void (*)(const Context & context, const FailureRecord & failure,
                                       void * reporterData) noexcept;

/*!
  \class Context
  \brief Per-run state of the built-in test runner.

  Counts assertions, tracks the verdict of the running case and forwards every failure to the reporter given at
  construction. Printing lives in the reporter, so the context stays testable without any output.

  \section features Key Features

  * **Allocation-free**: fixed-size info stack, no container and no heap
  * **Explicit context**: no global state, so two runners can coexist in one process
  * **Reporter seam**: failures leave through a function pointer carrying caller-owned data, not a virtual call
  * **Per-case verdict**: totals span the run while caseFailed() covers the running case

  \section usage Usage Example

  \code
  #include "toy_test.hpp"

  toy::test::Context context{&reportFailure};
  context.beginCase("core/fixed_string/append");
  context.record(str.size() == 4, "str.size() == 4", __FILE__, __LINE__);
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **record()**: O(1) constant time
  * **Memory usage**: 8 info entries plus counters, about 200 bytes

  \section safety Safety Guarantees

  * **Contracts**: infoAt() checks its index against infoCount() in a debug build
  * **Bounds safety**: pushInfo() past the depth drops the surplus and keeps push and pop balanced
  * **Memory safety**: no dynamic allocation
  * **Exception safety**: No operation throws; exceptions are off in the build

  \note Every stored pointer refers to a string literal, so the context owns nothing.

  \sa \ref toy::test::FailureRecord
*/
class Context final {
public:
  /// Depth of the info stack; messages beyond it are dropped.
  static constexpr std::size_t c_maxInfoDepth = 8;

  /*!
    \brief Builds a context reporting failures through \a reporter.

    \param reporter      Function invoked for every failed assertion; \c nullptr silences reporting.
    \param reporterData  Storage handed back to \a reporter on every call (default: \c nullptr); owned by the caller,
                         which must outlive the context.

    \post Counters are zero and no case is running.
  */
  explicit Context(failure_reporter_type reporter, void * reporterData = nullptr) noexcept;

  ~Context() noexcept                  = default;
  Context(const Context &)             = delete;
  Context & operator=(const Context &) = delete;
  Context(Context &&)                  = delete;
  Context & operator=(Context &&)      = delete;

  /*!
    \brief Starts a case and clears the per-case verdict.

    \param name  Case name; must outlive the run.

    \post caseFailed() is \c false, caseName() returns \a name, totals are unchanged.
  */
  void beginCase(const char * name) noexcept;

  /*!
    \brief Records one assertion result.

    \param passed      Outcome of the asserted expression.
    \param expression  Source text of the expression.
    \param file        Source file holding the assertion.
    \param line        Source line holding the assertion.

    \return \a passed, so a caller can return early on failure.

    \post On failure the case is marked. If the reporter is non-null, it is invoked exactly once.

    \sa caseFailed()
  */
  bool record(bool passed, const char * expression, const char * file, int line) noexcept;

  /*!
    \brief Pushes a message onto the info stack.

    \param text  Message text; must outlive the entry.

    \post infoCount() grows by one unless the depth is already reached.

    \sa popInfo()
  */
  void pushInfo(const char * text) noexcept;

  /// Pushes a message carrying a number; see pushInfo().
  void pushInfo(const char * text, long long value) noexcept;

  /*!
    \brief Pops the most recent message.

    \post infoCount() shrinks by one unless the stack is empty.

    \sa pushInfo()
  */
  void popInfo() noexcept;

  /*!
    \brief Starts one run of the running case.

    \param targetSubcase  Index of the subcase this run enters.

    \post The seen-subcase counter is zero and the info stack is empty; the case verdict and the totals are unchanged.

    \sa toy::test::runCase()
  */
  void beginRun(std::size_t targetSubcase) noexcept;

  /*!
    \brief Enters a subcase if this run targets it.

    \param name  Subcase name; must outlive the run.

    \return \c true when this run targets the subcase and the body must execute.

    \post On a nested entry the nested flag is raised and \c false is returned.

    \note Called by \ref toy::test::detail::SubcaseGuard, not from a test body.

    \sa leaveSubcase()
  */
  bool enterSubcase(const char * name) noexcept;

  /*!
    \brief Leaves the entered subcase.

    \post subcaseName() returns \c nullptr.

    \sa enterSubcase()
  */
  void leaveSubcase() noexcept;

  /// Returns how many subcases the case has revealed so far across its runs.
  [[nodiscard]] std::size_t subcaseCount() const noexcept;

  /// Returns whether a subcase was entered from inside another subcase, which the rules forbid.
  [[nodiscard]] bool nestedSubcaseDetected() const noexcept;

  /// Returns the number of assertions that passed during the run.
  [[nodiscard]] std::size_t passedCount() const noexcept;

  /// Returns the number of assertions that failed during the run.
  [[nodiscard]] std::size_t failedCount() const noexcept;

  /// Returns whether the running case has already failed an assertion.
  [[nodiscard]] bool caseFailed() const noexcept;

  /// Returns the number of messages currently on the info stack.
  [[nodiscard]] std::size_t infoCount() const noexcept;

  /*!
    \brief Returns the message at \a index.

    \param index  Position on the stack, oldest first.

    \return Entry stored at \a index.

    \pre \a index is below infoCount().
  */
  [[nodiscard]] const InfoEntry & infoAt(std::size_t index) const noexcept;

  /// Returns the name of the running case, or \c nullptr before the first beginCase().
  [[nodiscard]] const char * caseName() const noexcept;

  /// Returns the name of the running subcase, or \c nullptr outside one.
  [[nodiscard]] const char * subcaseName() const noexcept;

private:
  failure_reporter_type                 _reporter;
  void *                                _reporterData;
  const char *                          _caseName;
  const char *                          _subcaseName;
  std::size_t                           _passedCount;
  std::size_t                           _failedCount;
  std::size_t                           _infoDepth;
  bool                                  _caseFailed;
  std::size_t                           _targetSubcase;
  std::size_t                           _seenSubcases;
  std::size_t                           _subcaseCount;
  bool                                  _insideSubcase;
  bool                                  _nestedSubcase;
  std::array<InfoEntry, c_maxInfoDepth> _infoStack;
};

} // namespace toy::test

#include "context.inl"

#endif // INCLUDE_TESTS_RUNNER_CONTEXT_HPP_
