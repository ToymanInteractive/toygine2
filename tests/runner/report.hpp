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
  \file   report.hpp
  \brief  Writer of the TOYTEST report shared by every runner binary.

  Defines \ref toy::test::write_function_type and \ref toy::test::detail::ReportWriter: the line buffer and the walk
  over a case registry that turn a run into text. Sits on top of toy_test.hpp rather than inside it, because output is
  what consumes the runner, not part of it. Used by the runner's entry point and by its unit test, which reads the
  report back through a writer of its own.
*/

#ifndef INCLUDE_TESTS_RUNNER_REPORT_HPP_
#define INCLUDE_TESTS_RUNNER_REPORT_HPP_

#include <cstddef>

#include "toy_test.hpp"

namespace toy::test {

/*!
  \brief Function receiving the report one line at a time.

  \param text        Bytes to write; not null-terminated and ending in a newline.
  \param length      Number of bytes.
  \param writerData  Pointer handed to toy::test::writeReport(); \c nullptr when none was given.

  \note A writer is a plain function pointer, so a destination it must remember between calls arrives through
        \a writerData rather than through storage of its own.

  \sa \ref toy::test::detail::ReportWriter
*/
using write_function_type = void (*)(const char * text, std::size_t length, void * writerData) noexcept;

namespace detail {

/*!
  \class ReportWriter
  \brief Line buffer and per-case verdict state of the TOYTEST report.

  Builds one line at a time in a fixed buffer and hands it to the writer given at construction. Also holds the running
  case, so the verdict line prints once however many failures the case records.

  \section features Key Features

  * **Allocation-free**: one fixed buffer, no container and no heap
  * **Freestanding**: formats integers and text without \c <cstdio> or \c <charconv>
  * **Verdict once**: writeVerdict() prints the entry for a case at most once
  * **Truncating**: a line past the buffer is cut and still terminated, never overflowed

  \section usage Usage Example

  \code
  toy::test::detail::ReportWriter writer{&write, &destination};

  writer.beginCase(1, "core/fixed_string/append");
  writer.writeVerdict(false);
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **addText()**: O(n) in the text length
  * **Memory usage**: 256-byte line buffer plus five words

  \section safety Safety Guarantees

  * **Contracts**: none; a line past the capacity is truncated, not asserted
  * **Bounds safety**: every append stops at the buffer's capacity
  * **Memory safety**: no ownership of the case name, which must outlive the run
  * **Exception safety**: No operation throws; exceptions are off in the build

  \note Every line the writer emits ends in a newline, so a writer may treat one call as one line.

  \sa \ref toy::test::write_function_type
*/
class ReportWriter final {
public:
  /// Capacity of one report line in bytes; a longer line is truncated.
  static constexpr std::size_t c_lineCapacity = 256;

  /*!
    \brief Builds a writer emitting through \a write.

    \param write       Function receiving each finished line; must not be \c nullptr.
    \param writerData  Storage handed back to \a write on every call; owned by the caller, which must outlive the
                       writer.

    \post The line buffer is empty and no case is running.
  */
  ReportWriter(write_function_type write, void * writerData) noexcept;

  ~ReportWriter() noexcept                       = default;
  ReportWriter(const ReportWriter &)             = delete;
  ReportWriter & operator=(const ReportWriter &) = delete;
  ReportWriter(ReportWriter &&)                  = delete;
  ReportWriter & operator=(ReportWriter &&)      = delete;

  /*!
    \brief Appends text to the line being built.

    \param text  Null-terminated text.

    \post The line grows by the text, or stops at the capacity.

    \sa addInteger()
  */
  void addText(const char * text) noexcept;

  /*!
    \brief Appends a signed decimal number to the line being built.

    \param value  Value to format.

    \post The line grows by the digits, or stops at the capacity.

    \sa addText()
  */
  void addInteger(long long value) noexcept;

  /*!
    \brief Terminates the line and hands it to the writer.

    \post The line buffer is empty; the writer has been called exactly once with a line ending in a newline.

    \note A line filled to the capacity loses its last byte to the terminator, so every line the writer receives ends
          in a newline whatever was appended to it.
  */
  void flush() noexcept;

  /*!
    \brief Starts a case and clears its verdict.

    \param number  Position of the case in the run, counted from one.
    \param name    Case name; must outlive the run.

    \post writeVerdict() will print an entry for this case.
  */
  void beginCase(std::size_t number, const char * name) noexcept;

  /*!
    \brief Prints the entry of the running case unless it is already printed.

    \param failed  \c true for a \c "not ok" entry, \c false for an \c "ok" one.

    \post The running case has an entry; a later call prints nothing.

    \note A case failing after its entry is printed keeps the verdict of the first call, which is why the failing
          caller prints first and the passing one last.

    \sa beginCase()
  */
  void writeVerdict(bool failed) noexcept;

private:
  write_function_type _write;
  void *              _writerData;
  const char *        _caseName{nullptr};
  std::size_t         _caseNumber{0};
  bool                _caseReported{false};
  char                _buffer[c_lineCapacity]{};
  std::size_t         _length{0};
};

/*!
  \brief Writes the block describing one failed assertion.

  \param context       Context that recorded the failure; read for the info stack.
  \param failure       The failed assertion.
  \param reporterData  The \ref toy::test::detail::ReportWriter the report is built with.

  \note Matches \ref toy::test::failure_reporter_type, so toy::test::writeReport() installs it into the context it
        drives the run with.
*/
void reportFailure(const Context & context, const FailureRecord & failure, void * reporterData) noexcept;

} // namespace detail

/*!
  \brief Runs every case in a registry and writes the TOYTEST report.

  Prints the format line, one entry per case in registry order, the plan line and the summary. A repeated case name
  aborts the run before any body executes, because two cases sharing a name cannot be told apart in a report.

  \param write       Function receiving each line of the report; must not be \c nullptr.
  \param writerData  Storage handed back to \a write on every call; may be \c nullptr.
  \param head        Head of a name-sorted case registry; \c nullptr runs no case.

  \return \c 0 when every assertion passed, \c 1 on any failure or a nested subcase, \c 2 on a duplicate case name.

  \post Every case in the registry has run, unless a duplicate name aborted the run.

  \note Allocates nothing and touches no global state: the run state and the line buffer are local, so two reports can
        be written in one process.

  \note Deterministic — the report depends on the registry order and the case bodies, never on link order or timing.

  \sa \ref toy::test::write_function_type
*/
[[nodiscard]] int writeReport(write_function_type write, void * writerData, const CaseRegistrar * head) noexcept;

} // namespace toy::test

#include "report.inl"

#endif // INCLUDE_TESTS_RUNNER_REPORT_HPP_
