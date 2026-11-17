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
  \file   format_string.test.cpp
  \brief  Unit tests for \ref toy::FormatString.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// A pattern the constructor rejects fails the helper's constant evaluation; as a template argument that failure is a
// deduction failure, so the enclosing concept turns false instead of failing the build.
template <bool Accepted>
struct CompileTag {};

// Compiles a pattern against the given argument types the way FormatString's consteval constructor does.
template <class... Args>
consteval bool patternAccepted(const StringView & pattern) {
  [[maybe_unused]] FormatString<Args...> validated{pattern};
  return true;
}

// A failed immediate invocation is a deduction failure where a concept is satisfied, but a hard error in an inline
// requires expression, so every compile-time verdict rides a named concept. Each concept states one pattern against
// one argument list, and its truth is the verdict: the pattern compiles for those arguments.
template <class... Args>
concept AcceptsAutoPlaceholder = requires { CompileTag<patternAccepted<Args...>("{}")>{}; };

template <class... Args>
concept AcceptsMixedPlaceholders = requires { CompileTag<patternAccepted<Args...>("{0} {}")>{}; };

template <class... Args>
concept AcceptsPositionalIndex = requires { CompileTag<patternAccepted<Args...>("{1}")>{}; };

template <class... Args>
concept AcceptsUnmatchedBrace = requires { CompileTag<patternAccepted<Args...>("{")>{}; };

template <class... Args>
concept AcceptsInvalidContent = requires { CompileTag<patternAccepted<Args...>("{0x}")>{}; };

} // namespace

// The copy constructor is defaulted and the deleted assignment keeps the pattern immutable after construction.
TEST_CASE("format_string/special_members") {
  static_assert(std::is_copy_constructible_v<FormatString<>>, "the copy constructor is defaulted");
  static_assert(std::is_nothrow_copy_constructible_v<FormatString<>>, "copying one StringView cannot fail");
  static_assert(!std::is_copy_assignable_v<FormatString<>>, "copy assignment is deleted");

  // The constructor is declared explicit(false), so a StringView builds the type without naming it.
  static_assert(std::is_convertible_v<StringView, FormatString<>>, "a StringView builds the type implicitly");
  static_assert(std::is_nothrow_constructible_v<FormatString<>, const char (&)[3]>,
                "construction from a literal cannot fail");
}

// get() hands back the pattern exactly as written, both in a constant expression and at runtime.
TEST_CASE("format_string/get_returns_source") {
  const FormatString<int> autoPlaceholder{"{}"};
  CHECK(autoPlaceholder.get() == "{}");

  const FormatString<int, int> positional{"{1} {0}"};
  CHECK(positional.get() == "{1} {0}");

  // Escaped braces stay written as they were; get() never rewrites the source.
  const FormatString<> escaped{"{{}}"};
  CHECK(escaped.get() == "{{}}");

  static_assert(std::is_same_v<decltype(FormatString<>{""}.get()), StringView>, "get() exposes the source as a view");
  static_assert(noexcept(FormatString<>{""}.get()), "get() must be noexcept");
  static_assert(FormatString<int>{"{}"}.get() == "{}", "get() returns the pattern as written");
  static_assert(FormatString<int, int>{"{1} {0}"}.get() == "{1} {0}", "get() preserves positional placeholders");
  static_assert(FormatString<>{"{{}}"}.get() == "{{}}", "get() keeps escaped braces as they were written");
}

// A well-formed pattern compiles against exactly the argument count its placeholders bind.
TEST_CASE("format_string/accepts_valid_patterns") {
  static_assert(FormatString<>{""}.get() == "", "an empty pattern compiles without arguments");
  static_assert(FormatString<int, int>{"{} {}"}.get() == "{} {}", "two auto placeholders bind two arguments");
  static_assert(FormatString<int, int, int>{"{} {} {}"}.get() == "{} {} {}",
                "three auto placeholders bind three arguments");
  static_assert(FormatString<int>{"{0} {0}"}.get() == "{0} {0}", "a repeated index reuses one argument");
  static_assert(FormatString<int, int>{"{0}"}.get() == "{0}", "a positional pattern need not use every argument");

  // Through the same probe the rejections ride: the machinery must see acceptance, or a broken probe would make
  // every rejection pass vacuously.
  static_assert(AcceptsAutoPlaceholder<int>, "the probe compiles a pattern that fits its arguments");
  static_assert(AcceptsPositionalIndex<int, int>, "the probe compiles a positional pattern that fits its arguments");
}

// A fixed string builds the type like any other source, the view pointing into its constant buffer.
TEST_CASE("format_string/accepts_string_like_sources") {
  // The consteval constructor reads the source in a constant expression, so the source itself must be one.
  static constexpr FixedString<3> autoPlaceholder{"{}"};
  const FormatString<int>         fromFixed{autoPlaceholder};
  CHECK(fromFixed.get() == "{}");

  static_assert(FormatString<int>{FixedString<3>{"{}"}}.get() == "{}", "a fixed string source compiles like any other");
}

// Every validation error the pattern validator reports rejects the constructor, one representative shape per error.
TEST_CASE("format_string/rejects_invalid_patterns") {
  static_assert(!AcceptsAutoPlaceholder<>, "an auto placeholder with no argument fails to compile");
  static_assert(!AcceptsMixedPlaceholders<int, int>, "mixing auto and positional placeholders fails to compile");
  static_assert(!AcceptsPositionalIndex<int>, "an index naming no argument fails to compile");
  static_assert(!AcceptsUnmatchedBrace<>, "an unmatched opening brace fails to compile");
  static_assert(!AcceptsInvalidContent<int>, "content following an index fails to compile");
}

} // namespace toy
