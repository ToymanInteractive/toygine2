//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \file   log_backend.cpp
  \brief  Implementation of \ref toy::log::Backend lifecycle, sink, and timestamp-policy accessors.
*/

#include "core.hpp"

namespace toy::log {

namespace {

/// Process-wide active backend; null when none is registered.
Backend * activeBackend{nullptr};

} // namespace

Backend::Backend() noexcept
  : _timestampFn{&Backend::defaultTimestamp} {
  assert_message(activeBackend == nullptr, "Backend: at most one active instance is allowed per process");

  activeBackend = this;
}

Backend::~Backend() noexcept {
  activeBackend = nullptr;
}

Backend & Backend::instance() noexcept {
  assert_message(activeBackend != nullptr, "Backend::instance: no active Backend");

  return *activeBackend;
}

uint32_t Backend::defaultTimestamp() noexcept {
  return 0;
}

} // namespace toy::log
