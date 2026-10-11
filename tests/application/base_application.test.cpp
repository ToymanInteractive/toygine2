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
  \file   base_application.test.cpp
  \brief  Unit tests for \ref toy::application::BaseApplication.
*/

#include "application.hpp"
#include "toy_test.hpp"

namespace toy::application {

namespace {

// Static storage, so the revision outlives every application the cases build.
constexpr const char * c_revision = "f7e57d4";

// The build every case hands to an application.
constexpr Version c_build{26, 20, 0, c_revision};

// Built at compile time, the way the console sample declares its application.
constexpr BaseApplication c_application{c_build};

} // namespace

// How the call site may construct an application, and which forms it may not write.
TEST_CASE("application/base_application/construction") {
  static_assert(std::is_nothrow_constructible_v<BaseApplication, const Version &>,
                "construction from a version must be declared non-throwing");
  static_assert(!std::is_convertible_v<Version, BaseApplication>,
                "a version must not turn into an application without naming the type");
  static_assert(!std::is_default_constructible_v<BaseApplication>, "an application must not exist without a version");
}

// What version() hands back, and that it reports the version passed at construction.
TEST_CASE("application/base_application/version") {
  const BaseApplication application{c_build};

  CHECK(application.version() == c_build);
  CHECK(application.version().major == 26);
  CHECK(application.version().minor == 20);
  CHECK(application.version().patch == 0);

  static_assert(std::is_same_v<decltype(c_application.version()), const Version &>,
                "the accessor must return a read-only reference, not a copy");
  static_assert(noexcept(c_application.version()), "the accessor must be declared non-throwing");

  static_assert(c_application.version() == c_build, "a constexpr application must report its version");
}

// The application keeps its own copy of the version, while the revision characters stay with the caller.
TEST_CASE("application/base_application/version_ownership") {
  Version               source{c_build};
  const BaseApplication application{source};

  source.patch = 1;

  CHECK(application.version().patch == 0);
  CHECK(application.version().revision.data() == c_revision);

  static_assert(c_application.version().revision.data() == c_revision,
                "the revision must view the caller's characters, not a copy");
}

} // namespace toy::application
