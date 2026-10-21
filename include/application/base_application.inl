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
  \file   base_application.inl
  \brief  Inline implementations for \ref toy::application::BaseApplication construction, run(), and the accessors.

  \note Included by application.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_APPLICATION_BASE_APPLICATION_INL_
#define INCLUDE_APPLICATION_BASE_APPLICATION_INL_

namespace toy::application {

constexpr const Version & BaseApplication::version() const noexcept {
  return _version;
}

inline bool BaseApplication::run(int argc, char ** argv) noexcept {
  _argC = argc;
  _argV = argv;

  return runInternal();
}

inline size_t BaseApplication::argsCount() const noexcept {
  return _argC;
}

inline char ** BaseApplication::args() noexcept {
  return _argV;
}

inline const char * BaseApplication::arg(size_t index) const {
  assert_message(index < _argC, "index out of range");

  return (index < _argC && _argV != nullptr) ? _argV[index] : nullptr;
}

constexpr BaseApplication::BaseApplication(const Version & version) noexcept
  : _version(version) {}

} // namespace toy::application

#endif // INCLUDE_APPLICATION_BASE_APPLICATION_INL_
