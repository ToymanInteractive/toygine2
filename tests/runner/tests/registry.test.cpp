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

#include <cstddef>

#include <doctest/doctest.h>

#include "toy_test.hpp"

namespace {

void emptyBody(toy::test::Context & context) {
  static_cast<void>(context);
}

// Walks the list and reports whether names appear in non-descending order.
bool isSorted(const toy::test::CaseRegistrar * head) noexcept {
  for (const toy::test::CaseRegistrar * node = head; node != nullptr && node->next() != nullptr; node = node->next())
    if (toy::test::detail::compareNames(node->name(), node->next()->name()) > 0)
      return false;

  return true;
}

std::size_t listLength(const toy::test::CaseRegistrar * head) noexcept {
  std::size_t length = 0;

  for (const toy::test::CaseRegistrar * node = head; node != nullptr; node = node->next())
    ++length;

  return length;
}

} // namespace

// Registration order does not survive: the list comes out sorted by name.
TEST_CASE("test/case_registrar/insertion_keeps_list_sorted") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar third{head, "zulu", "z.cpp", 3, &emptyBody};
  toy::test::CaseRegistrar second{head, "mike", "m.cpp", 2, &emptyBody};
  toy::test::CaseRegistrar first{head, "alpha", "a.cpp", 1, &emptyBody};

  REQUIRE(listLength(head) == 3);
  REQUIRE(isSorted(head) == true);
  REQUIRE(toy::test::detail::compareNames(head->name(), "alpha") == 0);
}

// Inserting in alphabetical order gives the same result as inserting in reverse.
TEST_CASE("test/case_registrar/order_is_independent_of_registration_order") {
  toy::test::CaseRegistrar * ascending = nullptr;

  toy::test::CaseRegistrar a1{ascending, "alpha", "a.cpp", 1, &emptyBody};
  toy::test::CaseRegistrar a2{ascending, "mike", "m.cpp", 2, &emptyBody};
  toy::test::CaseRegistrar a3{ascending, "zulu", "z.cpp", 3, &emptyBody};

  toy::test::CaseRegistrar * descending = nullptr;

  toy::test::CaseRegistrar d1{descending, "zulu", "z.cpp", 3, &emptyBody};
  toy::test::CaseRegistrar d2{descending, "mike", "m.cpp", 2, &emptyBody};
  toy::test::CaseRegistrar d3{descending, "alpha", "a.cpp", 1, &emptyBody};

  const toy::test::CaseRegistrar * left  = ascending;
  const toy::test::CaseRegistrar * right = descending;

  while (left != nullptr && right != nullptr) {
    REQUIRE(toy::test::detail::compareNames(left->name(), right->name()) == 0);
    left  = left->next();
    right = right->next();
  }

  REQUIRE(left == nullptr);
  REQUIRE(right == nullptr);
}

// Every field survives registration, so a report can name the source location.
TEST_CASE("test/case_registrar/preserves_registered_fields") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar node{head, "core/utils/trim", "tests/core/utils.test.cpp", 42, &emptyBody};

  REQUIRE(toy::test::detail::compareNames(node.name(), "core/utils/trim") == 0);
  REQUIRE(toy::test::detail::compareNames(node.file(), "tests/core/utils.test.cpp") == 0);
  REQUIRE(node.line() == 42);
  REQUIRE(node.body() == &emptyBody);
}

// Two cases sharing a name are neighbours in a sorted list, and the search finds them.
TEST_CASE("test/case_registrar/duplicate_name_is_found") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar first{head, "alpha", "a.cpp", 1, &emptyBody};
  toy::test::CaseRegistrar second{head, "alpha", "b.cpp", 2, &emptyBody};
  toy::test::CaseRegistrar third{head, "bravo", "c.cpp", 3, &emptyBody};

  const toy::test::CaseRegistrar * duplicate = toy::test::detail::findDuplicateName(head);

  REQUIRE(duplicate != nullptr);
  REQUIRE(toy::test::detail::compareNames(duplicate->name(), "alpha") == 0);
}

// Distinct names report no duplicate.
TEST_CASE("test/case_registrar/distinct_names_report_no_duplicate") {
  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar first{head, "alpha", "a.cpp", 1, &emptyBody};
  toy::test::CaseRegistrar second{head, "bravo", "b.cpp", 2, &emptyBody};

  REQUIRE(toy::test::detail::findDuplicateName(head) == nullptr);
}

// An empty registry is not a duplicate, and neither is a registry of one.
TEST_CASE("test/case_registrar/empty_and_single_registry_report_no_duplicate") {
  REQUIRE(toy::test::detail::findDuplicateName(nullptr) == nullptr);

  toy::test::CaseRegistrar * head = nullptr;

  toy::test::CaseRegistrar only{head, "alpha", "a.cpp", 1, &emptyBody};

  REQUIRE(toy::test::detail::findDuplicateName(head) == nullptr);
}
