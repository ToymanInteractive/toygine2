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

  Defines the case registry \ref toy::test::detail::caseListHead, \ref toy::test::detail::SubcaseGuard and the case
  loop, and pulls in the registration node, the run state, the tolerant comparison and the allocation-free helpers they
  rest on. Grows into the shim every test translation unit includes in place of doctest; today it carries the pieces
  that depend on neither runner.
*/

#ifndef INCLUDE_TESTS_RUNNER_TOY_TEST_HPP_
#define INCLUDE_TESTS_RUNNER_TOY_TEST_HPP_

#include <cstddef>

#include "approx.hpp"
#include "case_registrar.hpp"
#include "context.hpp"
#include "utils.hpp"

namespace toy::test {

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
  \brief Writes raw bytes to the platform's diagnostic channel.

  \param text    Bytes to write; not null-terminated.
  \param length  Number of bytes.

  \note Defined per platform in one sink translation unit chosen by the build; the runner never branches on the target
        itself.
*/
void platformWrite(const char * text, std::size_t length) noexcept;

/*!
  \brief Ends the run with the given code.

  \param code  \c 0 when every assertion passed, \c 1 on any failure or a nested subcase, \c 2 on a duplicate
               case name.

  \note On consoles there is no process to exit, so the sink stores the code where a debugger reads it and then parks
        the CPU.
*/
[[noreturn]] void platformExit(int code) noexcept;

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

/*!
  \class InfoGuard
  \brief Scope object pushing one message onto the context's info stack.

  Constructed by the info macro; the message is printed only when an assertion inside the scope fails.

  \section features Key Features

  * **Allocation-free**: the message is a pointer to a literal, and the stack has a fixed depth
  * **Scope-bound**: the entry disappears when the scope ends
  * **Non-copyable**: the scope owns the pushed entry

  \section usage Usage Example

  \code
  INFO("index", index);
  CHECK(values[index] == 0);
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **Destruction**: O(1) constant time
  * **Memory usage**: one pointer

  \section safety Safety Guarantees

  * **Contracts**: none; a push past the fixed depth is dropped, not asserted
  * **Memory safety**: no ownership of the text, which must outlive the scope
  * **Exception safety**: No operation throws; exceptions are off in the build

  \sa \ref toy::test::Context
*/
class InfoGuard final {
public:
  /// Pushes a message without a companion value; see toy::test::Context::pushInfo().
  InfoGuard(Context & context, const char * text) noexcept;

  /// Pushes a message carrying a number; see toy::test::Context::pushInfo().
  InfoGuard(Context & context, const char * text, long long value) noexcept;

  ~InfoGuard() noexcept;

  InfoGuard(const InfoGuard &)             = delete;
  InfoGuard & operator=(const InfoGuard &) = delete;
  InfoGuard(InfoGuard &&)                  = delete;
  InfoGuard & operator=(InfoGuard &&)      = delete;

private:
  Context * _context;
};

} // namespace detail

} // namespace toy::test

// Two levels of indirection are what makes __COUNTER__ expand before it is pasted.
#define TOY_TEST_CONCAT_INNER(lhs, rhs) lhs##rhs
#define TOY_TEST_CONCAT(lhs, rhs)       TOY_TEST_CONCAT_INNER(lhs, rhs)
#define TOY_TEST_UNIQUE(prefix)         TOY_TEST_CONCAT(prefix, __COUNTER__)

// The registrar is not const: a later registration writes the link of an earlier node, see toy::test::CaseRegistrar.
#define TOY_TEST_CASE_IMPL(caseName, bodyName, registrarName)                                                          \
  static void                       bodyName(::toy::test::Context & toyTestContext);                                   \
  static ::toy::test::CaseRegistrar registrarName{::toy::test::detail::caseListHead, caseName, __FILE__, __LINE__,     \
                                                  &bodyName};                                                          \
  static void                       bodyName(::toy::test::Context & toyTestContext)

#define TOY_TEST_SUBCASE_IMPL(subcaseName, guardName)                                                                  \
  if (const ::toy::test::detail::SubcaseGuard guardName{toyTestContext, subcaseName}; guardName.entered())

#define TOY_TEST_INFO_IMPL(guardName, ...)                                                                             \
  const ::toy::test::detail::InfoGuard guardName {                                                                     \
    toyTestContext, __VA_ARGS__                                                                                        \
  }

/// Declares and registers a test case; see \ref toy::test::CaseRegistrar.
#define TOY_TEST_CASE(caseName) TOY_TEST_CASE_IMPL(caseName, TOY_TEST_UNIQUE(toyTestBody), TOY_TEST_UNIQUE(toyTestReg))

/// Opens one branch of the running case; see \ref toy::test::detail::SubcaseGuard.
#define TOY_TEST_SUBCASE(subcaseName) TOY_TEST_SUBCASE_IMPL(subcaseName, TOY_TEST_UNIQUE(toyTestSubcase))

/*!
  \def TOY_TEST_INFO
  \brief Pushes a message shown only if an assertion in this scope fails.

  Takes a string literal, optionally followed by one integer value; see \ref toy::test::detail::InfoGuard.

  \warning Narrower than doctest's \c INFO, which takes any number of streamable arguments: a call with two strings,
           or with three arguments, compiles under doctest and fails to compile under the built-in runner.
*/
#define TOY_TEST_INFO(...) TOY_TEST_INFO_IMPL(TOY_TEST_UNIQUE(toyTestInfo), __VA_ARGS__)

/// Records an assertion and returns from the case body when it fails.
#define TOY_TEST_REQUIRE(expression)                                                                                   \
  do {                                                                                                                 \
    if (!toyTestContext.record(static_cast<bool>(expression), #expression, __FILE__, __LINE__)) {                      \
      return;                                                                                                          \
    }                                                                                                                  \
  } while (false)

/// Records an assertion and continues regardless of the outcome.
#define TOY_TEST_CHECK(expression)                                                                                     \
  static_cast<void>(toyTestContext.record(static_cast<bool>(expression), #expression, __FILE__, __LINE__))

/// Negated form of the returning assertion.
#define TOY_TEST_REQUIRE_FALSE(expression) TOY_TEST_REQUIRE(!(expression))

/// Negated form of the continuing assertion.
#define TOY_TEST_CHECK_FALSE(expression) TOY_TEST_CHECK(!(expression))

#if defined(TOYGINE_TESTS_USE_DOCTEST)

#include <doctest/doctest.h>

#else

#define TEST_CASE(caseName)       TOY_TEST_CASE(caseName)
#define SUBCASE(subcaseName)      TOY_TEST_SUBCASE(subcaseName)
#define REQUIRE(expression)       TOY_TEST_REQUIRE(expression)
#define REQUIRE_FALSE(expression) TOY_TEST_REQUIRE_FALSE(expression)
#define CHECK(expression)         TOY_TEST_CHECK(expression)
#define CHECK_FALSE(expression)   TOY_TEST_CHECK_FALSE(expression)
#define INFO(...)                 TOY_TEST_INFO(__VA_ARGS__)

#endif // TOYGINE_TESTS_USE_DOCTEST

#include "toy_test.inl"

#endif // INCLUDE_TESTS_RUNNER_TOY_TEST_HPP_
