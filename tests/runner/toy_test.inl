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
  \file   toy_test.inl
  \brief  Inline implementations for \ref toy::test::Context, \ref toy::test::CaseRegistrar and the runner's
          name and number helpers.

  \note Included by toy_test.hpp only; do not include this file directly.
*/

namespace toy::test {

inline Context::Context(failure_reporter_type reporter, void * reporterData) noexcept
  : _reporter{reporter}
  , _reporterData{reporterData}
  , _caseName{nullptr}
  , _subcaseName{nullptr}
  , _passedCount{0}
  , _failedCount{0}
  , _infoDepth{0}
  , _caseFailed{false}
  , _targetSubcase{0}
  , _seenSubcases{0}
  , _subcaseCount{0}
  , _insideSubcase{false}
  , _nestedSubcase{false}
  , _infoStack{} {}

inline void Context::beginCase(const char * name) noexcept {
  _caseName      = name;
  _subcaseName   = nullptr;
  _caseFailed    = false;
  _infoDepth     = 0;
  _targetSubcase = 0;
  _seenSubcases  = 0;
  _subcaseCount  = 0;
  _insideSubcase = false;
  _nestedSubcase = false;
}

inline bool Context::record(bool passed, const char * expression, const char * file, int line) noexcept {
  if (passed) {
    ++_passedCount;

    return true;
  }

  ++_failedCount;
  _caseFailed = true;

  if (_reporter != nullptr)
    _reporter(*this, FailureRecord{_caseName, _subcaseName, expression, file, line}, _reporterData);

  return false;
}

inline void Context::pushInfo(const char * text) noexcept {
  if (_infoDepth < c_maxInfoDepth)
    _infoStack[_infoDepth] = InfoEntry{text, 0, false};

  ++_infoDepth;
}

inline void Context::pushInfo(const char * text, long long value) noexcept {
  if (_infoDepth < c_maxInfoDepth)
    _infoStack[_infoDepth] = InfoEntry{text, value, true};

  ++_infoDepth;
}

inline void Context::popInfo() noexcept {
  if (_infoDepth > 0)
    --_infoDepth;
}

inline std::size_t Context::passedCount() const noexcept {
  return _passedCount;
}

inline std::size_t Context::failedCount() const noexcept {
  return _failedCount;
}

inline bool Context::caseFailed() const noexcept {
  return _caseFailed;
}

inline std::size_t Context::infoCount() const noexcept {
  // The depth counter keeps growing past the fixed stack so that pushes and pops stay balanced;
  // what is readable is capped at the stack itself.
  return std::min(_infoDepth, c_maxInfoDepth);
}

inline const InfoEntry & Context::infoAt(std::size_t index) const noexcept {
  return _infoStack[index];
}

inline const char * Context::caseName() const noexcept {
  return _caseName;
}

inline const char * Context::subcaseName() const noexcept {
  return _subcaseName;
}

inline void Context::beginRun(std::size_t targetSubcase) noexcept {
  _targetSubcase = targetSubcase;
  _seenSubcases  = 0;
  _infoDepth     = 0;
  _subcaseName   = nullptr;
  _insideSubcase = false;
}

inline bool Context::enterSubcase(const char * name) noexcept {
  if (_insideSubcase) {
    _nestedSubcase = true;

    return false;
  }

  const auto index = _seenSubcases;
  ++_seenSubcases;

  if (index >= _subcaseCount)
    _subcaseCount = index + 1;

  if (index != _targetSubcase)
    return false;

  _insideSubcase = true;
  _subcaseName   = name;

  return true;
}

inline void Context::leaveSubcase() noexcept {
  _insideSubcase = false;
  _subcaseName   = nullptr;
}

inline std::size_t Context::subcaseCount() const noexcept {
  return _subcaseCount;
}

inline bool Context::nestedSubcaseDetected() const noexcept {
  return _nestedSubcase;
}

inline void runCase(Context & context, const char * name, case_body_type body) noexcept {
  context.beginCase(name);

  std::size_t targetSubcase = 0;

  do {
    context.beginRun(targetSubcase);
    body(context);
    ++targetSubcase;
  } while (targetSubcase < context.subcaseCount());
}

namespace detail {

inline SubcaseGuard::SubcaseGuard(Context & context, const char * name) noexcept
  : _context{&context}
  , _entered{context.enterSubcase(name)} {}

inline SubcaseGuard::~SubcaseGuard() noexcept {
  if (_entered)
    _context->leaveSubcase();
}

inline bool SubcaseGuard::entered() const noexcept {
  return _entered;
}

} // namespace detail

inline CaseRegistrar::CaseRegistrar(CaseRegistrar *& head, const char * name, const char * file, int line,
                                    case_body_type body) noexcept
  : _name{name}
  , _file{file}
  , _line{line}
  , _body{body}
  , _next{nullptr} {
  // Insertion sort at registration time: O(n) per node, but it makes the run order depend on names alone.
  CaseRegistrar ** link = &head;

  while (*link != nullptr && detail::compareNames((*link)->_name, name) < 0)
    link = &(*link)->_next;

  _next = *link;
  *link = this;
}

inline const char * CaseRegistrar::name() const noexcept {
  return _name;
}

inline const char * CaseRegistrar::file() const noexcept {
  return _file;
}

inline int CaseRegistrar::line() const noexcept {
  return _line;
}

inline case_body_type CaseRegistrar::body() const noexcept {
  return _body;
}

inline const CaseRegistrar * CaseRegistrar::next() const noexcept {
  return _next;
}

namespace detail {

inline const CaseRegistrar * findDuplicateName(const CaseRegistrar * head) noexcept {
  for (const CaseRegistrar * node = head; node != nullptr && node->next() != nullptr; node = node->next())
    if (compareNames(node->name(), node->next()->name()) == 0)
      return node;

  return nullptr;
}

} // namespace detail

} // namespace toy::test
