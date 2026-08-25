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
/*!
  \file   registry.test.cpp
  \brief  Unit tests for the runner's case registry: the duplicate-name scan over a name-sorted list.
*/

#include <doctest/doctest.h>

#include "toy_test.hpp"

namespace {

void emptyBody(toy::test::Context & context) {
  static_cast<void>(context);
}

// A registrar stores the pointer it is handed rather than copying the text, so a case's identity in the list is the
// literal's address, and the assertions compare addresses.
constexpr const char * c_alpha = "alpha";
constexpr const char * c_bravo = "bravo";

} // namespace

// Two cases sharing a name are neighbours in a sorted list, and the search finds them.
TEST_CASE("test::detail/find_duplicate_name/finds_the_adjacent_repeat") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar first{head, c_alpha, "a.cpp", 1, &emptyBody};
  toy::test::CaseRegistrar second{head, c_alpha, "b.cpp", 2, &emptyBody};
  toy::test::CaseRegistrar third{head, c_bravo, "c.cpp", 3, &emptyBody};

  const toy::test::CaseRegistrar * duplicate = toy::test::detail::findDuplicateName(head);

  REQUIRE(duplicate != nullptr);
  REQUIRE(duplicate->name() == c_alpha);
  REQUIRE(duplicate->next() != nullptr);
  REQUIRE(duplicate->next()->name() == c_alpha);
}

// Distinct names report nothing.
TEST_CASE("test::detail/find_duplicate_name/distinct_names_report_none") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar first{head, c_alpha, "a.cpp", 1, &emptyBody};
  toy::test::CaseRegistrar second{head, c_bravo, "b.cpp", 2, &emptyBody};

  REQUIRE(toy::test::detail::findDuplicateName(head) == nullptr);
}

// A list too short to hold a neighbouring pair reports nothing instead of walking off its end.
TEST_CASE("test::detail/find_duplicate_name/empty_and_single_registry_report_none") {
  REQUIRE(toy::test::detail::findDuplicateName(nullptr) == nullptr);

  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, c_alpha, "a.cpp", 1, &emptyBody};

  REQUIRE(toy::test::detail::findDuplicateName(head) == nullptr);
}
