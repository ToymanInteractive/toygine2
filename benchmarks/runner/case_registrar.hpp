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
  \file   case_registrar.hpp
  \brief  Registration node linking one benchmark into the runner's name-sorted list.

  Defines \ref toy::benchmark::CaseRegistrar and \ref toy::benchmark::case_body_type: what the \c BENCHMARK_CASE macro
  constructs as a static object, and what the runner's \c main walks to print one table per benchmark.

  \note Included by toy_benchmark.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_CASE_REGISTRAR_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_CASE_REGISTRAR_HPP_

#include <nanobench.h>

namespace toy::benchmark {

/// Body of a benchmark, as produced by the BENCHMARK_CASE macro; the bench arrives with its title and epoch
/// length already set.
using case_body_type = void (*)(ankerl::nanobench::Bench & bench);

/*!
  \class CaseRegistrar
  \brief Static registration node linking one benchmark into a name-sorted list.

  Constructed as a static object by the \c BENCHMARK_CASE macro. Insertion keeps the list ordered by name, so the print
  order depends only on benchmark names and not on the order of static initialization across translation units, which
  the standard leaves unspecified.

  \section features Key Features

  * **Allocation-free**: the node is the static object itself, so the registry has no capacity limit
  * **Deterministic order**: insertion sorts by name at registration time
  * **Explicit list head**: the list is a parameter, so a caller can build a registry of its own
  * **Non-copyable**: a node belongs to exactly one list

  \section usage Usage Example

  \code
  static void body(ankerl::nanobench::Bench & bench);
  static toy::benchmark::CaseRegistrar registrar{toy::benchmark::detail::caseListHead, "core/string_view/find",
                                                 __FILE__, __LINE__, &body};
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(n) comparisons against the registered benchmarks, so O(n^2) for the whole registry
  * **Traversal**: O(1) per node
  * **Memory usage**: 4 pointers plus one int, about 20 bytes on a 32-bit target

  \section safety Safety Guarantees

  * **Contracts**: none; every name is accepted, and a repeated one registers a second table under it
  * **Memory safety**: no ownership; the name and file pointers refer to string literals
  * **Exception safety**: No operation throws; exceptions are off in the build

  \note Registration happens before \c main, so a node must have static storage duration.
  \note A later registration writes the link of an earlier node, so a node must not be \c const.
*/
class CaseRegistrar final {
public:
  /*!
    \brief Registers a benchmark into the list rooted at \a head, keeping it sorted by name.

    \param head  List head; updated when the new node sorts first.
    \param name  Benchmark name, which the runner uses as the table title; must outlive the run.
    \param file  Source file declaring the benchmark.
    \param line  Source line declaring the benchmark.
    \param body  Benchmark body.

    \post The node is linked into \a head and the list stays ordered by name.
  */
  CaseRegistrar(CaseRegistrar *& head, const char * name, const char * file, int line, case_body_type body) noexcept;

  ~CaseRegistrar() noexcept                        = default;
  CaseRegistrar(const CaseRegistrar &)             = delete;
  CaseRegistrar & operator=(const CaseRegistrar &) = delete;
  CaseRegistrar(CaseRegistrar &&)                  = delete;
  CaseRegistrar & operator=(CaseRegistrar &&)      = delete;

  /// Returns the benchmark name, which titles its table.
  [[nodiscard]] const char * name() const noexcept;

  /// Returns the source file declaring the benchmark.
  [[nodiscard]] const char * file() const noexcept;

  /// Returns the source line declaring the benchmark.
  [[nodiscard]] int line() const noexcept;

  /// Returns the benchmark body.
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

} // namespace toy::benchmark

#include "case_registrar.inl"

#endif // INCLUDE_BENCHMARKS_RUNNER_CASE_REGISTRAR_HPP_
