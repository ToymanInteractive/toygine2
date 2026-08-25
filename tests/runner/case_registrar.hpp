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
/*!
  \file   case_registrar.hpp
  \brief  Registration node linking one test case into the runner's name-sorted list.

  Defines \ref toy::test::CaseRegistrar and \ref toy::test::case_body_type: what the \c TEST_CASE macro constructs as a
  static object, and what the runner's \c main walks to execute the cases.

  \note Reached through toy_test.hpp, and included by the runner's own headers; a test translation unit must not include
        this file directly.
*/

#ifndef INCLUDE_TESTS_RUNNER_CASE_REGISTRAR_HPP_
#define INCLUDE_TESTS_RUNNER_CASE_REGISTRAR_HPP_

#include "context.hpp"
#include "utils.hpp"

namespace toy::test {

/// Body of a test case, as produced by the TEST_CASE macro.
using case_body_type = void (*)(Context & context);

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
  CaseRegistrar * _next{nullptr};
};

} // namespace toy::test

#include "case_registrar.inl"

#endif // INCLUDE_TESTS_RUNNER_CASE_REGISTRAR_HPP_
