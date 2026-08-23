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
  \file   registry.test.cpp
  \brief  Unit tests for \ref toy::test::CaseRegistrar and the runner's name-sorted case registry.
*/

#include <doctest/doctest.h>

#include "toy_test.hpp"

namespace {

void emptyBody(toy::test::Context & context) {
  static_cast<void>(context);
}

// A registrar stores the pointers it is handed rather than copying the text, so a case's identity in the list is the
// literal's address. Comparing addresses asserts that contract and keeps the assertions independent of the name
// comparison the insertion sort itself runs on.
constexpr const char * c_alpha = "alpha";
constexpr const char * c_bravo = "bravo";
constexpr const char * c_mike  = "mike";
constexpr const char * c_zulu  = "zulu";

} // namespace

// Registration order does not survive: the list comes out sorted by name. The three register out of order in both
// directions, so neither a plain prepend nor a plain append reproduces the expected sequence.
TEST_CASE("test/case_registrar/insertion_keeps_list_sorted") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar second{head, c_mike, "m.cpp", 2, &emptyBody};
  toy::test::CaseRegistrar third{head, c_zulu, "z.cpp", 3, &emptyBody};
  toy::test::CaseRegistrar first{head, c_alpha, "a.cpp", 1, &emptyBody};

  REQUIRE(head->name() == c_alpha);
  REQUIRE(head->next()->name() == c_mike);
  REQUIRE(head->next()->next()->name() == c_zulu);
  REQUIRE(head->next()->next()->next() == nullptr);
}

// Inserting in alphabetical order gives the same result as inserting in reverse.
TEST_CASE("test/case_registrar/order_is_independent_of_registration_order") {
  toy::test::CaseRegistrar * ascending = nullptr;

  toy::test::CaseRegistrar a1{ascending, c_alpha, "a.cpp", 1, &emptyBody};
  toy::test::CaseRegistrar a2{ascending, c_mike, "m.cpp", 2, &emptyBody};
  toy::test::CaseRegistrar a3{ascending, c_zulu, "z.cpp", 3, &emptyBody};

  toy::test::CaseRegistrar * descending = nullptr;

  toy::test::CaseRegistrar d1{descending, c_zulu, "z.cpp", 3, &emptyBody};
  toy::test::CaseRegistrar d2{descending, c_mike, "m.cpp", 2, &emptyBody};
  toy::test::CaseRegistrar d3{descending, c_alpha, "a.cpp", 1, &emptyBody};

  const toy::test::CaseRegistrar * left  = ascending;
  const toy::test::CaseRegistrar * right = descending;

  while (left != nullptr && right != nullptr) {
    REQUIRE(left->name() == right->name());
    left  = left->next();
    right = right->next();
  }

  REQUIRE(left == nullptr);
  REQUIRE(right == nullptr);
}

// Every field survives registration, so a report can name the source location.
TEST_CASE("test/case_registrar/preserves_registered_fields") {
  constexpr const char * c_caseName = "core/utils/trim";
  constexpr const char * c_fileName = "tests/core/utils.test.cpp";

  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar node{head, c_caseName, c_fileName, 42, &emptyBody};

  REQUIRE(node.name() == c_caseName);
  REQUIRE(node.file() == c_fileName);
  REQUIRE(node.line() == 42);
  REQUIRE(node.body() == &emptyBody);
}

// Two cases sharing a name are neighbours in a sorted list, and the search finds them.
TEST_CASE("test/case_registrar/duplicate_name_is_found") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar first{head, c_alpha, "a.cpp", 1, &emptyBody};
  toy::test::CaseRegistrar second{head, c_alpha, "b.cpp", 2, &emptyBody};
  toy::test::CaseRegistrar third{head, c_bravo, "c.cpp", 3, &emptyBody};

  const toy::test::CaseRegistrar * duplicate = toy::test::detail::findDuplicateName(head);

  REQUIRE(duplicate != nullptr);
  REQUIRE(duplicate->name() == c_alpha);
  REQUIRE(duplicate->next()->name() == c_alpha);
}

// Distinct names report no duplicate.
TEST_CASE("test/case_registrar/distinct_names_report_no_duplicate") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar first{head, c_alpha, "a.cpp", 1, &emptyBody};
  toy::test::CaseRegistrar second{head, c_bravo, "b.cpp", 2, &emptyBody};

  REQUIRE(toy::test::detail::findDuplicateName(head) == nullptr);
}

// An empty registry is not a duplicate, and neither is a registry of one.
TEST_CASE("test/case_registrar/empty_and_single_registry_report_no_duplicate") {
  REQUIRE(toy::test::detail::findDuplicateName(nullptr) == nullptr);

  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, c_alpha, "a.cpp", 1, &emptyBody};

  REQUIRE(toy::test::detail::findDuplicateName(head) == nullptr);
}
