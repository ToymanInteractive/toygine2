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
  \brief  Writer of the TAP version 14 report shared by every runner binary.

  Defines \ref toy::test::write_function_type and \ref toy::test::detail::ReportWriter: the line buffer and the walk
  over a case registry that turn a run into a TAP document, subtests included. Sits on top of toy_test.hpp rather than
  inside it, because output is what consumes the runner, not part of it. Used by the runner's entry point and by its
  unit test, which reads the report back through a writer of its own.
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
using write_function_type = void (*)(const char * text, std::size_t length, const void * writerData) noexcept;

namespace detail {

/*!
  \class ReportWriter
  \brief Line buffer and per-case verdict state of the TAP report.

  Builds one line at a time in a fixed buffer and hands it to the writer given at construction. Also holds the running
  test point, so it prints once however many failures land under it, and the indent a TAP subtest needs.

  \section features Key Features

  * **Allocation-free**: one fixed buffer, no container and no heap
  * **Freestanding**: formats integers and text without \c <cstdio> or \c <charconv>
  * **Verdict once**: writeVerdict() prints the running test point at most once
  * **Subtest indent**: setIndent() shifts every following line, so a nested document keeps its four spaces
  * **TAP escaping**: addDescription() escapes the backslash and the hash, which TAP reads as syntax
  * **YAML diagnostics**: one block per test point, opened by the first failure and closed by endPoint()
  * **Truncating**: a line past the buffer is cut and still terminated, never overflowed

  \section usage Usage Example

  \code
  toy::test::detail::ReportWriter writer{&write, &destination};

  writer.beginPoint(1, "core/fixed_string/append");
  writer.writeVerdict(false);
  writer.endPoint();
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **addText(), addDescription() and addQuoted()**: O(n) in the text length
  * **Memory usage**: 256-byte line buffer plus seven words

  \section safety Safety Guarantees

  * **Contracts**: none; a line past the capacity is truncated, not asserted
  * **Bounds safety**: every append stops at the buffer's capacity
  * **Memory safety**: no ownership of the point description, which must outlive the run
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
  ReportWriter(write_function_type write, const void * writerData) noexcept;

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
    \brief Appends text to the line being built with the backslash and the hash escaped.

    \param text  Null-terminated text.

    \post The line grows by the escaped text, or stops at the capacity.

    \note TAP reads an unescaped hash in a test point description or a bail-out reason as the start of a directive.

    \sa addText()
  */
  void addDescription(const char * text) noexcept;

  /*!
    \brief Appends text to the line being built as a single-quoted YAML scalar.

    \param text  Null-terminated text.

    \post The line grows by the quoted text; a scalar cut at the capacity still closes.

    \note The last byte of a full line belongs to the break flush() writes, so the quoting stops before it.

    \sa addText()
  */
  void addQuoted(const char * text) noexcept;

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

    \note A line filled to the capacity loses its last byte to the terminator, so every line the writer receives ends in
          a newline whatever was appended to it.
  */
  void flush() noexcept;

  /*!
    \brief Starts a test point and clears its verdict.

    \param number       Position of the point in the document it belongs to, counted from one.
    \param description  Text following the point number; must outlive the run.

    \post writeVerdict() will print this point, and no diagnostic block is open.

    \sa endPoint()
  */
  void beginPoint(std::size_t number, const char * description) noexcept;

  /*!
    \brief Sets the indent every following line opens with.

    \param spaces  Number of leading spaces (default: \c 0 at construction).

    \post Lines started after this call carry \a spaces leading spaces.

    \note A TAP subtest indents its document by four, and the YAML block inside it by two more.
  */
  void setIndent(std::size_t spaces) noexcept;

  /*!
    \brief Writes the plan line of the document being built.

    \param count  Number of test points the document holds.

    \note The plan of a subtest carries the indent of its document, the plan of the run carries none.
  */
  void writePlan(std::size_t count) noexcept;

  /*!
    \brief Opens the YAML diagnostic block of the running test point unless it is already open.

    \post The block is open and endPoint() closes it.

    \note A test point carries at most one diagnostic block, so every failure of a case writes into the same one.

    \sa diagnosticsOpen()
  */
  void openDiagnostics() noexcept;

  /// Returns whether the running test point has an open diagnostic block.
  [[nodiscard]] bool diagnosticsOpen() const noexcept;

  /*!
    \brief Ends the running test point, closing its diagnostic block when one is open.

    \post No diagnostic block is open.

    \sa beginPoint()
  */
  void endPoint() noexcept;

  /*!
    \brief Prints the running test point unless it is already printed.

    \param failed  \c true for a \c "not ok" line, \c false for an \c "ok" one.

    \post The running point is printed; a later call prints nothing.

    \note A point failing after it is printed keeps the verdict of the first call, which is why the failing caller
          prints first and the passing one last.

    \sa beginPoint()
  */
  void writeVerdict(bool failed) noexcept;

private:
  /// Opens a line with the indent set by setIndent(), doing nothing once the line has bytes in it.
  void openLine() noexcept;

  write_function_type _write;
  const void *        _writerData;
  const char *        _pointDescription{nullptr};
  std::size_t         _pointNumber{0};
  std::size_t         _indent{0};
  bool                _pointReported{false};
  bool                _diagnosticsOpen{false};
  char                _buffer[c_lineCapacity]{};
  std::size_t         _length{0};
};

/*!
  \brief Writes the YAML list item describing one failed assertion.

  Opens the diagnostic block of the case on the first failure and appends to it on every later one, so several
  failures share the one block their test point may carry.

  \param context       Context that recorded the failure; read for the info stack.
  \param failure       The failed assertion.
  \param reporterData  The \ref toy::test::detail::ReportWriter the report is built with.

  \note Matches \ref toy::test::failure_reporter_type, so toy::test::writeReport() installs it into the context it
        drives the run with.
*/
void reportFailure(const Context & context, const FailureRecord & failure, void * reporterData) noexcept;

/*!
  \brief Runs every branch of a case and writes the TAP subtest they form.

  \param writer        Writer the document is built with.
  \param context       Context the branches run against; its case is restarted here.
  \param probe         Context that already ran the body once, read for the branch names.
  \param registrar     Registry node holding the case name and body.
  \param subcaseCount  Number of branches the probe revealed.

  \pre \a subcaseCount is above zero.

  \post The subtest document is written, the indent is back to zero, and the point closing the subtest is not written
        yet.

  \note A branch the probe left unnamed takes the case name, which happens past
        \ref toy::test::Context::c_maxSubcaseNames.
*/
void reportSubtest(ReportWriter & writer, Context & context, const Context & probe, const CaseRegistrar & registrar,
                   std::size_t subcaseCount) noexcept;

/*!
  \brief Runs one case and writes its test point, or the subtest a case with branches becomes.

  \param writer     Writer the document is built with.
  \param context    Context the case runs against.
  \param registrar  Registry node holding the case name and body.
  \param number     Position of the case in the run, counted from one.

  \post The case has been probed and run, and its test point is written.

  \note The body runs once against a silent context before anything is printed, because the shape of the case decides
        its first line.

  \sa \ref toy::test::Context
*/
void reportCase(ReportWriter & writer, Context & context, const CaseRegistrar & registrar, std::size_t number) noexcept;

} // namespace detail

/*!
  \brief Runs every case in a registry and writes the TAP report.

  Prints the version line, one test point per case in registry order, the plan line and a summary comment. A case
  declaring subcases prints as a TAP subtest instead: one point per branch, then the point closing it. A repeated case
  name ends the document with \c "Bail out!" before any body executes, because two cases sharing a name cannot be told
  apart in a report.

  \param write       Function receiving each line of the report; must not be \c nullptr.
  \param writerData  Storage handed back to \a write on every call; may be \c nullptr.
  \param head        Head of a name-sorted case registry; \c nullptr runs no case.

  \return \c 0 when every assertion passed, \c 1 on any failure or a nested subcase, \c 2 on a duplicate case name.

  \post Every case in the registry has been probed and run, unless a duplicate name bailed the run out.

  \note A silent probe run reveals the branches of a case before its first line is printed, so a body must reach the
        same branches on every run.
  \note A failing case carries its diagnostics in the YAML block under its test point.
  \note Allocates nothing and touches no global state: the run state and the line buffer are local, so two reports can
        be written in one process.
  \note Deterministic — the report depends on the registry order and the case bodies, never on link order or timing.

  \sa \ref toy::test::write_function_type
*/
[[nodiscard]] int writeReport(write_function_type write, const void * writerData, const CaseRegistrar * head) noexcept;

} // namespace toy::test

#include "report.inl"

#endif // INCLUDE_TESTS_RUNNER_REPORT_HPP_
