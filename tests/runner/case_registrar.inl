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
  \file   case_registrar.inl
  \brief  Inline implementations for \ref toy::test::CaseRegistrar: registration and accessors.

  \note Included by case_registrar.hpp only; do not include this file directly.
*/

namespace toy::test {

inline CaseRegistrar::CaseRegistrar(CaseRegistrar *& head, const char * name, const char * file, int line,
                                    case_body_type body) noexcept
  : _name{name}
  , _file{file}
  , _line{line}
  , _body{body} {
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

} // namespace toy::test
