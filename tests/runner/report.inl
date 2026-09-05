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
  \file   report.inl
  \brief  Inline implementations for \ref toy::test::detail::ReportWriter and the run that writes the report.

  \note Included by report.hpp only; do not include this file directly.
*/

namespace toy::test {

namespace detail {

inline ReportWriter::ReportWriter(write_function_type write, const void * writerData) noexcept
  : _write{write}
  , _writerData{writerData} {}

inline void ReportWriter::openLine() noexcept {
  if (_length != 0)
    return;

  while (_length < _indent && _length < c_lineCapacity) {
    _buffer[_length] = ' ';
    ++_length;
  }
}

inline void ReportWriter::addText(const char * text) noexcept {
  openLine();

  _length = appendText(_buffer, c_lineCapacity, _length, text);
}

inline void ReportWriter::addDescription(const char * text) noexcept {
  openLine();

  _length = appendEscaped(_buffer, c_lineCapacity, _length, text);
}

inline void ReportWriter::addQuoted(const char * text) noexcept {
  // flush() spends the last byte of a full line on the line break, which would take the closing quote with it.
  constexpr std::size_t quotedCapacity = c_lineCapacity - 1;

  openLine();

  if (_length <= quotedCapacity)
    _length = appendQuoted(_buffer, quotedCapacity, _length, text);
}

inline void ReportWriter::addInteger(long long value) noexcept {
  openLine();

  _length = appendInteger(_buffer, c_lineCapacity, _length, value);
}

inline void ReportWriter::flush() noexcept {
  if (_length == c_lineCapacity)
    --_length;

  _buffer[_length] = '\n';
  ++_length;

  _write(_buffer, _length, _writerData);

  _length = 0;
}

inline void ReportWriter::beginPoint(std::size_t number, const char * description) noexcept {
  _pointDescription = description;
  _pointNumber      = number;
  _pointReported    = false;
  _diagnosticsOpen  = false;
}

inline void ReportWriter::setIndent(std::size_t spaces) noexcept {
  _indent = spaces;
}

inline void ReportWriter::writePlan(std::size_t count) noexcept {
  addText("1..");
  addInteger(static_cast<long long>(count));
  flush();
}

inline void ReportWriter::openDiagnostics() noexcept {
  if (_diagnosticsOpen)
    return;

  _diagnosticsOpen = true;

  addText("  ---");
  flush();

  addText("  severity: fail");
  flush();
}

inline bool ReportWriter::diagnosticsOpen() const noexcept {
  return _diagnosticsOpen;
}

inline void ReportWriter::endPoint() noexcept {
  if (!_diagnosticsOpen)
    return;

  _diagnosticsOpen = false;

  addText("  ...");
  flush();
}

inline void ReportWriter::writeVerdict(bool failed) noexcept {
  if (_pointReported)
    return;

  _pointReported = true;

  addText(failed ? "not ok " : "ok ");
  addInteger(static_cast<long long>(_pointNumber));
  addText(" - ");
  addDescription(_pointDescription);

  flush();
}

inline void reportFailure(const Context & context, const FailureRecord & failure, void * reporterData) noexcept {
  ReportWriter & writer = *static_cast<ReportWriter *>(reporterData);

  // The test point prints once per case, so several failures in one case stay under one verdict.
  writer.writeVerdict(true);

  // The list header belongs to the failure that opens the block; every later one appends an item to it.
  const bool firstFailure = !writer.diagnosticsOpen();

  writer.openDiagnostics();

  if (firstFailure) {
    writer.addText("  failures:");
    writer.flush();
  }

  writer.addText("    - at:");
  writer.flush();

  writer.addText("        file: ");
  writer.addQuoted(failure.file);
  writer.flush();

  writer.addText("        line: ");
  writer.addInteger(failure.line);
  writer.flush();

  writer.addText("      expr: ");
  writer.addQuoted(failure.expression);
  writer.flush();

  if (context.infoCount() > 0) {
    writer.addText("      info:");
    writer.flush();
  }

  for (std::size_t index = 0; index < context.infoCount(); ++index) {
    const InfoEntry & entry = context.infoAt(index);

    writer.addText("        - text: ");
    writer.addQuoted(entry.text);
    writer.flush();

    if (entry.hasValue) {
      writer.addText("          value: ");
      writer.addInteger(entry.value);
      writer.flush();
    }
  }
}

inline void reportSubtest(ReportWriter & writer, Context & context, const Context & probe,
                          const CaseRegistrar & registrar, std::size_t subcaseCount) noexcept {
  TOY_TEST_ASSERT(subcaseCount > 0, "a subtest must hold at least one branch");

  constexpr std::size_t subtestIndent = 4;

  // The header names the subtest, and TAP requires the point closing it to carry the same description.
  writer.addText("# Subtest: ");
  writer.addText(registrar.name());
  writer.flush();

  writer.setIndent(subtestIndent);

  context.beginCase(registrar.name());

  for (std::size_t index = 0; index < subcaseCount; ++index) {
    const char * const subcaseName = probe.subcaseNameAt(index);

    writer.beginPoint(index + 1, subcaseName != nullptr ? subcaseName : registrar.name());

    context.beginRun(index);
    registrar.body()(context);

    // Prints only for a branch that failed nothing, since a failure inside it wrote the verdict already.
    writer.writeVerdict(false);
    writer.endPoint();
  }

  TOY_TEST_ASSERT(context.subcaseCount() == subcaseCount, "the reported runs must cover every branch the probe found");

  writer.writePlan(subcaseCount);
  writer.setIndent(0);
}

inline void reportCase(ReportWriter & writer, Context & context, const CaseRegistrar & registrar,
                       std::size_t number) noexcept {
  // A subtest prints its whole document before the test point that closes it, so the branches of a case must be known
  // before its first line. A silent context runs the body once to reveal them, which a deterministic body repeats.
  Context probe{nullptr};

  probe.beginCase(registrar.name());
  probe.beginRun(0);
  registrar.body()(probe);

  const std::size_t subcaseCount = probe.subcaseCount();

  if (subcaseCount == 0) {
    writer.beginPoint(number, registrar.name());

    runCase(context, registrar.name(), registrar.body());
  } else {
    reportSubtest(writer, context, probe, registrar, subcaseCount);

    writer.beginPoint(number, registrar.name());
  }

  // A nested subcase condemns the case, so the test point must read "not ok" even when every assertion passed.
  if (context.nestedSubcaseDetected()) {
    writer.writeVerdict(true);
    writer.openDiagnostics();
    writer.addText("  error: ");
    writer.addQuoted("nested subcase");
    writer.flush();
  }

  // Prints only for a point that reached here unprinted, since a failure inside the case wrote the verdict already.
  writer.writeVerdict(context.caseFailed());
  writer.endPoint();
}

} // namespace detail

inline int writeReport(write_function_type write, const void * writerData, const CaseRegistrar * head) noexcept {
  detail::ReportWriter writer{write, writerData};

  writer.addText("TAP version 14");
  writer.flush();

  const CaseRegistrar * duplicate = detail::findDuplicateName(head);

  if (duplicate != nullptr) {
    writer.addText("Bail out! duplicate case name: ");
    writer.addDescription(duplicate->name());
    writer.flush();

    return 2;
  }

  Context context{&detail::reportFailure, &writer};

  std::size_t caseCount  = 0;
  bool        nestedSeen = false;

  for (const CaseRegistrar * node = head; node != nullptr; node = node->next()) {
    ++caseCount;

    detail::reportCase(writer, context, *node, caseCount);

    // The flag survives until the next case starts, so the run reads it back here rather than through a return value.
    nestedSeen = nestedSeen || context.nestedSubcaseDetected();
  }

  writer.writePlan(caseCount);

  writer.addText("# assertions passed=");
  writer.addInteger(static_cast<long long>(context.passedCount()));
  writer.addText(" failed=");
  writer.addInteger(static_cast<long long>(context.failedCount()));
  writer.flush();

  return context.failedCount() != 0 || nestedSeen ? 1 : 0;
}

} // namespace toy::test
