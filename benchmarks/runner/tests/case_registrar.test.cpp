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
  \file   case_registrar.test.cpp
  \brief  Unit tests for the order of the benchmark registry and the search for a repeated name.
*/

#include <ostream>
#include <string_view>
#include <vector>

#include <doctest/doctest.h>

#include "toy_benchmark.hpp"

namespace {

void emptyBody([[maybe_unused]] toy::benchmark::Bench & bench) {}

[[nodiscard]] std::vector<std::string_view> namesInOrder(const toy::benchmark::CaseRegistrar * head) {
  std::vector<std::string_view> names;

  for (const auto * node = head; node != nullptr; node = node->next())
    names.emplace_back(node->name());

  return names;
}

} // namespace

// A name sorting first, last or between two others lands in its place, whatever order the registrars ran in.
TEST_CASE("benchmark/case_registrar/keeps_the_list_sorted") {
  toy::benchmark::CaseRegistrar * head = nullptr;
  toy::benchmark::CaseRegistrar   second{head, "core/b", "b.cpp", 20, &emptyBody};
  toy::benchmark::CaseRegistrar   fourth{head, "core/d", "d.cpp", 40, &emptyBody};
  toy::benchmark::CaseRegistrar   first{head, "core/a", "a.cpp", 10, &emptyBody};
  toy::benchmark::CaseRegistrar   third{head, "core/c", "c.cpp", 30, &emptyBody};

  CHECK(namesInOrder(head) == std::vector<std::string_view>{"core/a", "core/b", "core/c", "core/d"});
  CHECK(toy::benchmark::detail::findDuplicateName(head) == nullptr);
}

// Two registrations of one name end up side by side even with another name registered between them.
TEST_CASE("benchmark::detail/find_duplicate_name/finds_a_repeat_registered_apart") {
  toy::benchmark::CaseRegistrar * head = nullptr;
  toy::benchmark::CaseRegistrar   earlier{head, "core/same", "a.cpp", 10, &emptyBody};
  toy::benchmark::CaseRegistrar   other{head, "core/other", "b.cpp", 20, &emptyBody};
  toy::benchmark::CaseRegistrar   later{head, "core/same", "c.cpp", 30, &emptyBody};

  const auto * duplicate = toy::benchmark::detail::findDuplicateName(head);

  REQUIRE(duplicate != nullptr);
  CHECK(std::string_view{duplicate->name()} == "core/same");
  CHECK(std::string_view{duplicate->next()->name()} == "core/same");
}
