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
  \file   command_line.test.cpp
  \brief  Unit tests for the benchmark runner's option parsing and pattern matching.
*/

#include <initializer_list>
#include <string>
#include <vector>

#include <doctest/doctest.h>

#include "command_line.hpp"

namespace {

// The runner reads argv and never writes it, so a test owns the strings and passes pointers into them.
class Arguments final {
public:
  explicit Arguments(std::initializer_list<const char *> values) {
    for (const char * value : values)
      _storage.emplace_back(value);

    for (std::string & value : _storage)
      _pointers.push_back(value.data());
  }

  ~Arguments() noexcept                    = default;
  Arguments(const Arguments &)             = delete;
  Arguments & operator=(const Arguments &) = delete;
  Arguments(Arguments &&)                  = delete;
  Arguments & operator=(Arguments &&)      = delete;

  [[nodiscard]] int count() const noexcept {
    return static_cast<int>(_pointers.size());
  }

  [[nodiscard]] char ** data() noexcept {
    return _pointers.data();
  }

private:
  std::vector<std::string> _storage;
  std::vector<char *>      _pointers;
};

} // namespace

// parseOptions() reads the leading options in any order, and the first pattern follows them.
TEST_CASE("benchmark::detail/parse_options/reads_options_in_any_order") {
  Arguments arguments{"toygine-benchmarks", "--csv", "--no-table", "--list", "find"};

  const auto options = toy::benchmark::detail::parseOptions(arguments.count(), arguments.data());

  CHECK(options.csv);
  CHECK(options.noTable);
  CHECK(options.listOnly);
  CHECK(options.firstPattern == 4);
}

// Parsing stops at the first argument that is not an option, so a later option stays a pattern.
TEST_CASE("benchmark::detail/parse_options/stops_at_the_first_pattern") {
  Arguments arguments{"toygine-benchmarks", "find", "--csv"};

  const auto options = toy::benchmark::detail::parseOptions(arguments.count(), arguments.data());

  CHECK_FALSE(options.csv);
  CHECK(options.firstPattern == 1);
}

// A run without options carries no pattern either.
TEST_CASE("benchmark::detail/parse_options/bare_run_has_no_pattern") {
  Arguments arguments{"toygine-benchmarks"};

  const auto options = toy::benchmark::detail::parseOptions(arguments.count(), arguments.data());

  CHECK_FALSE(options.listOnly);
  CHECK_FALSE(options.csv);
  CHECK_FALSE(options.noTable);
  CHECK(options.firstPattern == 1);
}

// No pattern selects every benchmark; a pattern selects the names carrying it as a substring.
TEST_CASE("benchmark::detail/selected/matches_names_by_substring") {
  Arguments patterns{"find_char", "starts_with"};

  CHECK(toy::benchmark::detail::selected("core/string_view/compare", 0, patterns.data()));
  CHECK(toy::benchmark::detail::selected("core/string_view/find_char_large", patterns.count(), patterns.data()));
  CHECK(toy::benchmark::detail::selected("core/string_view/starts_with", patterns.count(), patterns.data()));
  CHECK_FALSE(toy::benchmark::detail::selected("core/string_view/compare", patterns.count(), patterns.data()));
}
