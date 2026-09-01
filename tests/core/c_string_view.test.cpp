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
  \file   c_string_view.test.cpp
  \brief  Unit tests for \ref toy::CStringView.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// The literal every case reads. Its length is measured from the literal, not written out as a constant.
constexpr const char * c_sample        = "player";
constexpr size_t       c_sampleLength  = char_traits<char>::length(c_sample);
// The same characters back to front, the order a reverse walk must yield.
constexpr const char * c_sampleReverse = "reyalp";

// Distance a forward walk covers, as a length rather than a pointer difference.
[[nodiscard]] constexpr size_t forwardLength(const CStringView & view) noexcept {
  return static_cast<size_t>(view.end() - view.begin());
}

// Distance a back-to-front walk covers.
[[nodiscard]] constexpr size_t reverseLength(const CStringView & view) noexcept {
  return static_cast<size_t>(view.rend() - view.rbegin());
}

} // namespace

// The aliases the view publishes, and the type each one has to name.
TEST_CASE("c_string_view/types") {
  static_assert(std::is_same_v<CStringView::value_type, char>, "the view reads plain char characters");
  static_assert(std::is_same_v<CStringView::const_pointer, const char *>,
                "const_pointer must name a pointer to a viewed character");
  static_assert(std::is_same_v<CStringView::const_reference, const char &>,
                "const_reference must name a reference to a viewed character");

  static_assert(std::is_same_v<CStringView::const_iterator, const char *>,
                "the forward iterator is a pointer into the viewed string");
  static_assert(std::is_same_v<CStringView::iterator, CStringView::const_iterator>,
                "the view exposes no mutable iterator, so both aliases name the same type");
  static_assert(std::is_same_v<CStringView::const_reverse_iterator, std::reverse_iterator<const char *>>,
                "the reverse iterator adapts the forward one");
  static_assert(std::is_same_v<CStringView::reverse_iterator, CStringView::const_reverse_iterator>,
                "the view exposes no mutable reverse iterator, so both aliases name the same type");

  static_assert(std::is_same_v<CStringView::size_type, size_t>, "lengths are measured in size_t");
  static_assert(std::is_same_v<CStringView::difference_type, ptrdiff_t>,
                "the distance between two iterators is a ptrdiff_t");

  static_assert(std::contiguous_iterator<CStringView::const_iterator>,
                "a pointer iterator must meet the contiguous iterator requirements");
  static_assert(std::random_access_iterator<CStringView::const_reverse_iterator>,
                "the reverse adaptor must stay random access");
}

// What each constructor and the assignment leave behind, read back through the iterators.
TEST_CASE("c_string_view/construction") {
  const CStringView empty;
  CHECK(forwardLength(empty) == 0);
  CHECK(empty.begin() == nullptr);

  const CStringView view(c_sample);
  CHECK(forwardLength(view) == c_sampleLength);
  CHECK(view.begin() == c_sample);

  const CStringView copy(view);
  CHECK(copy.begin() == view.begin());
  CHECK(forwardLength(copy) == forwardLength(view));

  CStringView assigned;
  assigned = view;
  CHECK(assigned.begin() == view.begin());
  CHECK(forwardLength(assigned) == c_sampleLength);

  static_assert(forwardLength(CStringView()) == 0, "a default-constructed view spans no character");
  static_assert(forwardLength(CStringView(c_sample)) == c_sampleLength,
                "the pointer constructor must measure the literal's byte count");
  static_assert(forwardLength(CStringView(CStringView(c_sample))) == c_sampleLength,
                "a copy must span the same characters as its source");
}

// Which argument the view accepts at a call site and which one the type system rejects.
TEST_CASE("c_string_view/construction_from_null") {
  static_assert(std::is_convertible_v<const char *, CStringView>,
                "a const char * argument must become a view without a cast");
  static_assert(!std::is_constructible_v<CStringView, nullptr_t>,
                "the deleted overload must reject a literal nullptr during compilation");

  static_assert(std::is_nothrow_default_constructible_v<CStringView>, "an empty view must be buildable without cost");
  static_assert(std::is_trivially_copyable_v<CStringView>, "the view owns nothing, so a copy must stay trivial");
}

// The range a forward walk covers and the characters it yields.
TEST_CASE("c_string_view/iteration") {
  const CStringView view(c_sample);

  CHECK(forwardLength(view) == c_sampleLength);
  CHECK(view.cbegin() == view.begin());
  CHECK(view.cend() == view.end());

  for (size_t index = 0; index < c_sampleLength; ++index) {
    INFO("index ", static_cast<long long>(index));
    CHECK(view.begin()[index] == c_sample[index]);
  }

  // The terminator sits one past the last character, unlike std::string_view.
  CHECK(*view.end() == '\0');

  const CStringView empty;
  CHECK(empty.begin() == empty.end());
  CHECK(empty.cbegin() == empty.cend());

  static_assert(forwardLength(CStringView(c_sample)) == c_sampleLength,
                "the forward range must span the literal's byte count");
  static_assert(*CStringView(c_sample).begin() == 'p', "the first character must be the literal's first byte");
  static_assert(*CStringView(c_sample).end() == '\0', "the position past the last character holds the terminator");
  static_assert(CStringView().begin() == CStringView().end(), "an empty view must yield an empty range");
}

// The range a back-to-front walk covers and the order it yields the characters in.
TEST_CASE("c_string_view/iteration_reverse") {
  const CStringView view(c_sample);

  CHECK(reverseLength(view) == c_sampleLength);
  CHECK(view.crbegin() == view.rbegin());
  CHECK(view.crend() == view.rend());

  for (size_t index = 0; index < c_sampleLength; ++index) {
    INFO("index ", static_cast<long long>(index));
    CHECK(view.rbegin()[index] == c_sampleReverse[index]);
  }

  const CStringView empty;
  CHECK(empty.rbegin() == empty.rend());
  CHECK(empty.crbegin() == empty.crend());

  static_assert(reverseLength(CStringView(c_sample)) == c_sampleLength,
                "the reverse range must span the literal's byte count");
  static_assert(*CStringView(c_sample).rbegin() == 'r', "a reverse walk must start at the last character");
  static_assert(CStringView(c_sample).rend().base() == CStringView(c_sample).begin(),
                "the reverse end must adapt the forward beginning");
  static_assert(CStringView().rbegin() == CStringView().rend(), "an empty view must yield an empty reverse range");
}

} // namespace toy
