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

  Defines \ref toy::test::Approx and the runner's allocation-free name and number helpers. Grows into the shim every
  test translation unit includes in place of doctest; today it carries the pieces that depend on neither runner.
*/

#ifndef INCLUDE_TESTS_RUNNER_TOY_TEST_HPP_
#define INCLUDE_TESTS_RUNNER_TOY_TEST_HPP_

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace toy::test {

/*!
  \class Approx
  \brief Floating-point value carrying a comparison tolerance.

  Compares equal to a floating-point value whose difference falls within a tolerance that scales with magnitude, so the
  same epsilon works for small and large values.

  \tparam T  Floating-point type of the compared value; satisfies \c std::floating_point, so \c float, \c double and
             <tt>long double</tt> are accepted.

  \section features Key Features

  * **Constexpr support**: every operation is usable in a constant expression
  * **Relative tolerance**: the epsilon scales with the larger operand
  * **Mixed precision**: comparison against any floating-point type happens in their common type
  * **Freestanding**: no dependency on \c <cmath> or any hosted header

  \section usage Usage Example

  \code
  #include "toy_test.hpp"

  REQUIRE(computeRatio() == toy::test::Approx(0.5));
  REQUIRE(measureShort() == toy::test::Approx(1.0F).epsilon(0.01F));

  toy::test::Approx<long double> wide{1.0L};
  constexpr auto tolerant = toy::test::Approx<double>(1.0).epsilon(0.5);
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **Comparison**: O(1) constant time
  * **Memory usage**: two objects of type \a T

  \section safety Safety Guarantees

  * **Contracts**: none; every finite value of type \a T is a valid argument
  * **Type safety**: uses C++23 concepts; a non-floating-point argument fails the constraint
  * **Memory safety**: no dynamic allocation
  * **Exception safety**: No operation throws; exceptions are off in the build

  \section compatibility Compatibility

  Requires C++20 concepts. Allocates nothing and includes no hosted header, so it builds on every console and embedded
  target the engine supports. Verdicts match DocTest on every value \c double represents; comparison happens in the
  operands' own precision rather than in \c double, which DocTest always uses.

  \note A comparison involving NaN is false, matching the behaviour of the built-in operator.

  \note The default tolerance is the same constant for every \a T, deliberately: DocTest uses that one value regardless
        of type, and matching it keeps a test's verdict identical under both runners.
*/
template <std::floating_point T>
class Approx final {
public:
  explicit constexpr Approx(T value) noexcept;

  /*!
    \brief Returns a copy carrying the given tolerance.

    \tparam U     Floating-point type of the tolerance; converted to \a T.
    \param  value Relative tolerance; scaled by the larger operand during comparison.

    \return Copy of this value with \a value as its tolerance.

    \sa tolerance()
  */
  template <std::floating_point U>
  [[nodiscard]] constexpr Approx epsilon(U value) const noexcept;

  /// Returns the compared value.
  [[nodiscard]] constexpr T value() const noexcept;

  /// Returns the relative tolerance; see epsilon().
  [[nodiscard]] constexpr T tolerance() const noexcept;

private:
  T _value;
  T _epsilon;
};

/*!
  \brief Compares a tolerant value against a plain one.

  \tparam T    Floating-point type of the tolerant value.
  \tparam U    Floating-point type of the plain value.
  \param  lhs  Tolerant value.
  \param  rhs  Plain value.

  \return \c true when the difference is strictly below <tt>tolerance * (1 + max(|lhs|, |rhs|))</tt>.

  \note The formula and its strict inequality reproduce DocTest 2.5.3. DocTest's adjustable scale factor is fixed at one
        here; no test configures it.

  \note Both operands are widened to their common type before comparison, so mixing precisions loses nothing. DocTest
        instead narrows everything to \c double. The two agree on every value \c double represents, because the gap
        between the precisions stays orders of magnitude below the tolerance; they can disagree on a
        <tt>long double</tt> beyond the range of \c double, where narrowing yields infinity.

  \note The form with the plain value on the left resolves through the reversed candidate C++20 synthesizes, so no
        second operator is declared.

  \sa toy::test::Approx
*/
template <std::floating_point T, std::floating_point U>
[[nodiscard]] constexpr bool operator==(const Approx<T> & lhs, U rhs) noexcept;

namespace detail {

/*!
  \brief Returns the magnitude of a floating-point value.

  \tparam T      Floating-point type of the value.
  \param  value  Value to take the magnitude of.

  \return \a value without its sign.

  \note Exists because \c <cmath> is a hosted header and the runner must build freestanding.
*/
template <std::floating_point T>
[[nodiscard]] constexpr T absoluteValue(T value) noexcept;

/*!
  \brief Orders two null-terminated names by their first differing byte.

  \param lhs  Left name.
  \param rhs  Right name.

  \return \c -1 when \a lhs orders first, \c 1 when \a rhs orders first, \c 0 when the names are equal.

  \note Bytes compare as unsigned, so the order is total on targets where \c char is signed.
*/
[[nodiscard]] constexpr int compareNames(const char * lhs, const char * rhs) noexcept;

/*!
  \brief Copies text into a buffer, stopping at its capacity.

  \param buffer    Destination buffer.
  \param capacity  Size of \a buffer in bytes.
  \param offset    Position to write from.
  \param text      Null-terminated source text.

  \return Offset past the last byte written; equals \a capacity when the text was truncated.

  \pre \a offset does not exceed \a capacity.

  \note No terminating zero is written; the caller tracks the length.
*/
[[nodiscard]] constexpr std::size_t appendText(char * buffer, std::size_t capacity, std::size_t offset,
                                               const char * text) noexcept;

/*!
  \brief Writes a signed decimal representation into a buffer, stopping at its capacity.

  \param buffer    Destination buffer.
  \param capacity  Size of \a buffer in bytes.
  \param offset    Position to write from.
  \param value     Value to format.

  \return Offset past the last byte written; equals \a capacity when the digits were truncated.

  \pre \a offset does not exceed \a capacity.

  \note The magnitude is taken in unsigned arithmetic, so the most negative value formats correctly.
*/
[[nodiscard]] constexpr std::size_t appendInteger(char * buffer, std::size_t capacity, std::size_t offset,
                                                  long long value) noexcept;

} // namespace detail

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

class Context;

/// Function invoked for every failed assertion; see \ref toy::test::Context.
using failure_reporter_type = void (*)(const Context & context, const FailureRecord & failure) noexcept;

/*!
  \class Context
  \brief Per-run state of the built-in test runner.

  Counts assertions, tracks the verdict of the running case and forwards every failure to the reporter given at
  construction. Printing lives in the reporter, so the context stays testable without any output.

  \section features Key Features

  * **Allocation-free**: fixed-size info stack, no container and no heap
  * **Explicit context**: no global state, so two runners can coexist in one process
  * **Reporter seam**: failures leave through a function pointer, not a virtual call
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

    \param reporter  Function invoked for every failed assertion; \c nullptr silences reporting.

    \post Counters are zero and no case is running.
  */
  explicit Context(failure_reporter_type reporter) noexcept;

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

namespace detail {

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
