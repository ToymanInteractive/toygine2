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
  \file   toy_test.hpp
  \brief  Test macros and runner declarations shared by the doctest and built-in test runners.

  Defines \ref toy::test::Context, \ref toy::test::CaseRegistrar and \ref toy::test::detail::SubcaseGuard, and pulls
  in the records, the tolerant comparison and the allocation-free helpers they rest on. Grows into the shim every test
  translation unit includes in place of doctest; today it carries the pieces that depend on neither runner.
*/

#ifndef INCLUDE_TESTS_RUNNER_TOY_TEST_HPP_
#define INCLUDE_TESTS_RUNNER_TOY_TEST_HPP_

#include <array>
#include <cstddef>

#include "approx.hpp"
#include "failure_record.hpp"
#include "info_entry.hpp"
#include "utils.hpp"

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

  * **Contracts**: infoAt() requires an index below infoCount()
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

/// Body of a test case, as produced by the TEST_CASE macro.
using case_body_type = void (*)(Context & context);

/*!
  \brief Runs one case, repeating the body once per subcase it declares.

  \param context  Run state; receives the counters and the failures.
  \param name     Case name; must outlive the run.
  \param body     Case body.

  \post The body has executed once for a case without subcases, or once per subcase otherwise.

  \note A case whose body returns early through a failed \c REQUIRE may reveal fewer subcases on that run; the case is
        already failed, so the shortfall changes no verdict.

  \sa \ref toy::test::Context
*/
void runCase(Context & context, const char * name, case_body_type body) noexcept;

/*!
  \class CaseRegistrar
  \brief Static registration node linking one test case into a name-sorted list.

  Constructed as a static object by the \c TEST_CASE macro. Insertion keeps the list ordered by name, so the run order
  depends only on case names and not on the order of static initialization across translation units, which the standard
  leaves unspecified.

  \section features Key Features

  * **Allocation-free**: the node is the static object itself, so the registry has no capacity limit
  * **Deterministic order**: insertion sorts by name at registration time
  * **Explicit list head**: the list is a parameter, so a test can build an isolated registry
  * **Non-copyable**: a node belongs to exactly one list

  \section usage Usage Example

  \code
  static void body(toy::test::Context & context);
  static toy::test::CaseRegistrar registrar{toy::test::detail::caseListHead, "core/utils/trim", __FILE__, __LINE__,
                                            &body};
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(n) comparisons against the registered cases, so O(n^2) for the whole registry
  * **Traversal**: O(1) per node
  * **Memory usage**: 4 pointers plus one int, about 20 bytes on a 32-bit target

  \section safety Safety Guarantees

  * **Contracts**: none; every name is accepted, and duplicates are reported by findDuplicateName()
  * **Memory safety**: no ownership; the name and file pointers refer to string literals
  * **Exception safety**: No operation throws; exceptions are off in the build

  \note Registration happens before \c main, so a node must have static storage duration in production use.

  \note A later registration writes the link of an earlier node, so a node must not be \c const.

  \sa \ref toy::test::Context
*/
class CaseRegistrar final {
public:
  /*!
    \brief Registers a case into the list rooted at \a head, keeping it sorted by name.

    \param head  List head; updated when the new node sorts first.
    \param name  Case name; must outlive the run.
    \param file  Source file declaring the case.
    \param line  Source line declaring the case.
    \param body  Case body.

    \post The node is linked into \a head and the list stays ordered by name.
  */
  CaseRegistrar(CaseRegistrar *& head, const char * name, const char * file, int line, case_body_type body) noexcept;

  ~CaseRegistrar() noexcept                        = default;
  CaseRegistrar(const CaseRegistrar &)             = delete;
  CaseRegistrar & operator=(const CaseRegistrar &) = delete;
  CaseRegistrar(CaseRegistrar &&)                  = delete;
  CaseRegistrar & operator=(CaseRegistrar &&)      = delete;

  /// Returns the case name.
  [[nodiscard]] const char * name() const noexcept;

  /// Returns the source file declaring the case.
  [[nodiscard]] const char * file() const noexcept;

  /// Returns the source line declaring the case.
  [[nodiscard]] int line() const noexcept;

  /// Returns the case body.
  [[nodiscard]] case_body_type body() const noexcept;

  /// Returns the next node in name order, or \c nullptr at the end of the list.
  [[nodiscard]] const CaseRegistrar * next() const noexcept;

private:
  const char *    _name;
  const char *    _file;
  int             _line;
  case_body_type  _body;
  CaseRegistrar * _next;
};

namespace detail {

/*!
  \brief Head of the registry every \c TEST_CASE registers into.

  Initialized with a constant, so it is set before any dynamic initialization runs and no registrar can observe it
  uninitialized.
*/
inline CaseRegistrar * caseListHead = nullptr;

/*!
  \brief Finds the first case whose name repeats.

  \param head  List head; may be \c nullptr.

  \return First node whose name equals its successor's, or \c nullptr when every name is unique.

  \note Relies on the list being sorted, which puts equal names next to each other.
*/
[[nodiscard]] const CaseRegistrar * findDuplicateName(const CaseRegistrar * head) noexcept;

/*!
  \class SubcaseGuard
  \brief Scope object entering one subcase of the running case.

  Constructed by the \c SUBCASE macro; entered() reports whether the current run targets this subcase.

  \section features Key Features

  * **Allocation-free**: two members, no state beyond the context
  * **Non-copyable**: the scope owns the entered subcase
  * **Nesting diagnosed**: entering from inside a subcase raises the context's nested flag

  \section usage Usage Example

  \code
  const toy::test::detail::SubcaseGuard guard{context, "branch"};
  if (guard.entered()) {
    // subcase body
  }
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **Destruction**: O(1) constant time
  * **Memory usage**: one pointer and one flag

  \section safety Safety Guarantees

  * **Contracts**: none; a nested entry is reported, not asserted
  * **Memory safety**: no ownership of the name, which must outlive the run
  * **Exception safety**: No operation throws; exceptions are off in the build

  \sa \ref toy::test::Context
*/
class SubcaseGuard final {
public:
  /*!
    \brief Enters the subcase named \a name when the current run targets it.

    \param context  Run state.
    \param name     Subcase name; must outlive the run.
  */
  SubcaseGuard(Context & context, const char * name) noexcept;

  ~SubcaseGuard() noexcept;

  SubcaseGuard(const SubcaseGuard &)             = delete;
  SubcaseGuard & operator=(const SubcaseGuard &) = delete;
  SubcaseGuard(SubcaseGuard &&)                  = delete;
  SubcaseGuard & operator=(SubcaseGuard &&)      = delete;

  /// Returns whether the current run entered this subcase.
  [[nodiscard]] bool entered() const noexcept;

private:
  Context * _context;
  bool      _entered;
};

} // namespace detail

} // namespace toy::test

#include "toy_test.inl"

#endif // INCLUDE_TESTS_RUNNER_TOY_TEST_HPP_
