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
  \brief  Inline implementations for the case loop, the registry's duplicate-name scan and
          \ref toy::test::detail::SubcaseGuard.

  \note Included by toy_test.hpp only; do not include this file directly.
*/

namespace toy::test {

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

inline const CaseRegistrar * findDuplicateName(const CaseRegistrar * head) noexcept {
  for (const CaseRegistrar * node = head; node != nullptr && node->next() != nullptr; node = node->next())
    if (compareNames(node->name(), node->next()->name()) == 0)
      return node;

  return nullptr;
}

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

inline InfoGuard::InfoGuard(Context & context, const char * text) noexcept
  : _context{&context} {
  context.pushInfo(text);
}

inline InfoGuard::InfoGuard(Context & context, const char * text, long long value) noexcept
  : _context{&context} {
  context.pushInfo(text, value);
}

inline InfoGuard::~InfoGuard() noexcept {
  _context->popInfo();
}

} // namespace detail

} // namespace toy::test
