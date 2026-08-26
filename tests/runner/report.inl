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

inline ReportWriter::ReportWriter(write_function_type write, void * writerData) noexcept
  : _write{write}
  , _writerData{writerData} {}

inline void ReportWriter::addText(const char * text) noexcept {
  _length = appendText(_buffer, c_lineCapacity, _length, text);
}

inline void ReportWriter::addInteger(long long value) noexcept {
  _length = appendInteger(_buffer, c_lineCapacity, _length, value);
}

inline void ReportWriter::flush() noexcept {
  addText("\n");

  _write(_buffer, _length, _writerData);

  _length = 0;
}

inline void ReportWriter::beginCase(std::size_t number, const char * name) noexcept {
  _caseName     = name;
  _caseNumber   = number;
  _caseReported = false;
}

inline void ReportWriter::writeVerdict(bool failed) noexcept {
  if (_caseReported)
    return;

  _caseReported = true;

  addText(failed ? "not ok " : "ok ");
  addInteger(static_cast<long long>(_caseNumber));
  addText(" - ");
  addText(_caseName);

  flush();
}

inline void reportFailure(const Context & context, const FailureRecord & failure, void * reporterData) noexcept {
  ReportWriter & writer = *static_cast<ReportWriter *>(reporterData);

  // The header prints once per case, so several failures in one case stay under one entry.
  writer.writeVerdict(true);

  writer.addText("  file: ");
  writer.addText(failure.file);
  writer.flush();

  writer.addText("  line: ");
  writer.addInteger(failure.line);
  writer.flush();

  writer.addText("  expr: ");
  writer.addText(failure.expression);
  writer.flush();

  if (failure.subcaseName != nullptr) {
    writer.addText("  subcase: ");
    writer.addText(failure.subcaseName);
    writer.flush();
  }

  for (std::size_t index = 0; index < context.infoCount(); ++index) {
    const InfoEntry & entry = context.infoAt(index);

    writer.addText("  info: ");
    writer.addText(entry.text);

    if (entry.hasValue) {
      writer.addText(": ");
      writer.addInteger(entry.value);
    }

    writer.flush();
  }
}

} // namespace detail

inline int writeReport(write_function_type write, void * writerData, const CaseRegistrar * head) noexcept {
  detail::ReportWriter writer{write, writerData};

  writer.addText("TOYTEST 1");
  writer.flush();

  const CaseRegistrar * duplicate = detail::findDuplicateName(head);

  if (duplicate != nullptr) {
    writer.addText("TOYTEST ERROR duplicate case name: ");
    writer.addText(duplicate->name());
    writer.flush();

    return 2;
  }

  Context context{&detail::reportFailure, &writer};

  std::size_t caseCount  = 0;
  bool        nestedSeen = false;

  for (const CaseRegistrar * node = head; node != nullptr; node = node->next()) {
    ++caseCount;
    writer.beginCase(caseCount, node->name());

    runCase(context, node->name(), node->body());

    // A nested subcase condemns the case, so the entry must read "not ok" even when every assertion passed.
    if (context.nestedSubcaseDetected()) {
      nestedSeen = true;

      writer.writeVerdict(true);
      writer.addText("  error: nested subcase");
      writer.flush();
    }

    // Prints only for a case that reached here without a failure, since the verdict is already written otherwise.
    writer.writeVerdict(false);
  }

  writer.addText("1..");
  writer.addInteger(static_cast<long long>(caseCount));
  writer.flush();

  writer.addText("TOYTEST SUMMARY passed=");
  writer.addInteger(static_cast<long long>(context.passedCount()));
  writer.addText(" failed=");
  writer.addInteger(static_cast<long long>(context.failedCount()));
  writer.flush();

  return context.failedCount() != 0 || nestedSeen ? 1 : 0;
}

} // namespace toy::test
