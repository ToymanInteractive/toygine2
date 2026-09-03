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
constexpr const char * c_sample         = "player";
constexpr size_t       c_sampleLength   = char_traits<char>::length(c_sample);
// The same characters back to front, the order a reverse walk must yield.
constexpr const char * c_sampleReverse  = "reyalp";
// A leading part of the sample, and a longer string starting with it.
constexpr const char * c_samplePrefix   = "play";
constexpr const char * c_sampleLonger   = "player one";
// A literal whose groups repeat, so a search has more than one candidate to pick between.
constexpr const char * c_repeated       = "abracadabra";
constexpr size_t       c_repeatedLength = char_traits<char>::length(c_repeated);

// Distance a forward walk covers, as a length rather than a pointer difference.
[[nodiscard]] constexpr size_t forwardLength(const CStringView & view) noexcept {
  return static_cast<size_t>(view.end() - view.begin());
}

// Distance a back-to-front walk covers.
[[nodiscard]] constexpr size_t reverseLength(const CStringView & view) noexcept {
  return static_cast<size_t>(view.rend() - view.rbegin());
}

// The view left after dropping the leading characters, so a mutation reads back in a constant expression.
[[nodiscard]] constexpr CStringView withoutPrefix(const char * string, size_t count) noexcept {
  CStringView view(string);
  view.remove_prefix(count);

  return view;
}

// The view the first argument names after the two exchange their contents.
[[nodiscard]] constexpr CStringView swapped(const char * first, const char * second) noexcept {
  CStringView left(first);
  CStringView right(second);
  left.swap(right);

  return left;
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

// The character each indexed access path yields, and where in the viewed string it reads it.
TEST_CASE("c_string_view/element_access") {
  const CStringView view(c_sample);

  for (size_t index = 0; index < c_sampleLength; ++index) {
    INFO("index ", static_cast<long long>(index));
    CHECK(view[index] == c_sample[index]);
    CHECK(view.at(index) == c_sample[index]);
  }

  CHECK(view.front() == c_sample[0]);
  CHECK(view.back() == c_sample[c_sampleLength - 1]);

  // Each access hands back a reference into the viewed string, not a copy of the character.
  CHECK(&view.front() == view.data());
  CHECK(&view.back() == view.data() + c_sampleLength - 1);

  static_assert(CStringView(c_sample)[0] == 'p', "an indexed read must yield the literal's byte at that offset");
  static_assert(CStringView(c_sample).at(c_sampleLength - 1) == 'r', "at() must read the same byte as operator[]");
  static_assert(CStringView(c_sample).front() == 'p', "the front character must be the literal's first byte");
  static_assert(CStringView(c_sample).back() == 'r', "the back character must be the literal's last byte");
}

// The pointer the view hands to a C interface, and the terminator that pointer reaches.
TEST_CASE("c_string_view/pointer_access") {
  const CStringView view(c_sample);

  CHECK(view.data() == c_sample);
  CHECK(view.c_str() == view.data());

  // Null-termination is the contract std::string_view does not carry.
  CHECK(view.c_str()[view.size()] == '\0');

  const CStringView empty;
  CHECK(empty.data() == nullptr);
  CHECK(empty.c_str() == nullptr);

  static_assert(CStringView(c_sample).data() == c_sample, "the view must hand back the pointer it was built from");
  static_assert(CStringView(c_sample).c_str()[c_sampleLength] == '\0',
                "the byte past the last character must be the terminator");
  static_assert(CStringView().data() == nullptr, "a default-constructed view names no string");
}

// What the view reports about its extent, and the bound a length may not cross.
TEST_CASE("c_string_view/capacity") {
  const CStringView view(c_sample);

  CHECK(view.size() == c_sampleLength);
  CHECK(view.length() == view.size());
  CHECK_FALSE(view.empty());

  const CStringView empty;
  CHECK(empty.size() == 0);
  CHECK(empty.length() == 0);
  CHECK(empty.empty());

  // A view over an empty literal reports no character, yet still names a string.
  const CStringView emptyLiteral("");
  CHECK(emptyLiteral.empty());
  CHECK(emptyLiteral.data() != nullptr);

  CHECK(view.max_size() == static_cast<CStringView::size_type>(-1));

  static_assert(CStringView(c_sample).size() == c_sampleLength, "size() must report the literal's UTF-8 byte count");
  static_assert(CStringView(c_sample).length() == CStringView(c_sample).size(),
                "length() and size() must report the same count");
  static_assert(CStringView().empty(), "a default-constructed view holds no character");
  static_assert(!CStringView(c_sample).empty(), "a view over a non-empty literal holds characters");
  static_assert(CStringView::npos == static_cast<CStringView::size_type>(-1),
                "npos must be the largest value the length type carries");
}

// Where the view starts reading after it drops leading characters.
TEST_CASE("c_string_view/remove_prefix") {
  CStringView view(c_sample);

  view.remove_prefix(4);
  CHECK(view.size() == c_sampleLength - 4);
  CHECK(view.data() == c_sample + 4);
  CHECK(view.front() == 'e');

  // Dropping the whole length leaves a view over the terminator alone.
  view.remove_prefix(view.size());
  CHECK(view.empty());
  CHECK(view.data() == c_sample + c_sampleLength);

  static_assert(withoutPrefix(c_sample, 0).size() == c_sampleLength, "dropping nothing must leave the view as it was");
  static_assert(withoutPrefix(c_sample, 4).size() == c_sampleLength - 4,
                "the length must shrink by the number of characters dropped");
  static_assert(withoutPrefix(c_sample, 4).data() == c_sample + 4,
                "the view must start reading past the characters it dropped");
  static_assert(withoutPrefix(c_sample, c_sampleLength).empty(), "dropping the whole length must leave an empty view");
}

// Which string each of the two views reads after they exchange their contents.
TEST_CASE("c_string_view/swap") {
  CStringView left(c_sample);
  CStringView right(c_samplePrefix);

  left.swap(right);
  CHECK(left.data() == c_samplePrefix);
  CHECK(left.size() == char_traits<char>::length(c_samplePrefix));
  CHECK(right.data() == c_sample);
  CHECK(right.size() == c_sampleLength);

  left.swap(right);
  CHECK(left.data() == c_sample);
  CHECK(right.data() == c_samplePrefix);

  static_assert(swapped(c_sample, c_samplePrefix).data() == c_samplePrefix,
                "the left view must read the string the right one named");
  static_assert(swapped(c_sample, c_samplePrefix).size() == char_traits<char>::length(c_samplePrefix),
                "the length must travel with the pointer");
}

// How many characters reach the caller's buffer, and which ones.
TEST_CASE("c_string_view/copy") {
  const CStringView view(c_sample);

  char buffer[16] = {};
  CHECK(view.copy(buffer, 4) == 4);
  CHECK(char_traits<char>::compare(buffer, c_samplePrefix, 4) == 0);

  // A count past the end copies what remains rather than reading past the terminator.
  char tail[16] = {};
  CHECK(view.copy(tail, 32, 2) == c_sampleLength - 2);
  CHECK(char_traits<char>::compare(tail, "ayer", 4) == 0);

  // The call writes characters only; the terminator stays the caller's business.
  char exact[16] = {};
  CHECK(view.copy(exact, c_sampleLength) == c_sampleLength);
  CHECK(exact[c_sampleLength] == '\0');
}

// The sign each comparison of two whole strings yields.
TEST_CASE("c_string_view/compare") {
  const CStringView view(c_sample);

  CHECK(view.compare(CStringView(c_sample)) == 0);
  CHECK(view.compare(CStringView(c_samplePrefix)) > 0);
  CHECK(view.compare(CStringView(c_sampleLonger)) < 0);
  CHECK(view.compare(CStringView("playz")) < 0);

  // The pointer overload measures its argument and compares the same way.
  CHECK(view.compare(c_sample) == 0);
  CHECK(view.compare(c_samplePrefix) > 0);
  CHECK(view.compare(c_sampleLonger) < 0);

  static_assert(CStringView(c_sample).compare(CStringView(c_sample)) == 0, "a string must compare equal to itself");
  static_assert(CStringView(c_sample).compare(CStringView(c_samplePrefix)) > 0,
                "a longer string sharing a prefix must order after the shorter one");
  static_assert(CStringView(c_sample).compare(c_sampleLonger) < 0,
                "the pointer overload must order by the first differing byte");
}

// The sign a comparison yields when it reads a part of one or both strings.
TEST_CASE("c_string_view/compare_substring") {
  const CStringView view(c_sample);

  CHECK(view.compare(0, 4, CStringView(c_samplePrefix)) == 0);
  CHECK(view.compare(2, 4, CStringView("ayer")) == 0);
  CHECK(view.compare(0, 4, CStringView(c_sample)) < 0);

  CHECK(view.compare(0, 4, CStringView(c_sampleLonger), 0, 4) == 0);
  CHECK(view.compare(0, c_sampleLength, CStringView(c_sampleLonger), 0, 4) > 0);

  CHECK(view.compare(0, 4, c_samplePrefix) == 0);
  CHECK(view.compare(0, 4, c_sampleLonger, 4) == 0);

  static_assert(CStringView(c_sample).compare(0, 4, CStringView(c_samplePrefix)) == 0,
                "a part must compare equal to the string it repeats");
  static_assert(CStringView(c_sample).compare(0, 4, CStringView(c_sample)) < 0,
                "a shorter part must order before the whole string it starts");
  static_assert(CStringView(c_sample).compare(0, 4, c_sampleLonger, 4) == 0,
                "the pointer overload must read only the characters the count names");
}

// Whether the viewed string opens with the characters the argument names.
TEST_CASE("c_string_view/starts_with") {
  const CStringView view(c_sample);

  CHECK(view.starts_with(CStringView(c_samplePrefix)));
  CHECK(view.starts_with(CStringView(c_sample)));
  CHECK_FALSE(view.starts_with(CStringView(c_sampleLonger)));
  CHECK(view.starts_with(CStringView("")));

  CHECK(view.starts_with('p'));
  CHECK_FALSE(view.starts_with('l'));
  CHECK_FALSE(CStringView("").starts_with('p'));

  CHECK(view.starts_with(c_samplePrefix));
  CHECK_FALSE(view.starts_with(c_sampleLonger));

  static_assert(CStringView(c_sample).starts_with(CStringView(c_samplePrefix)),
                "a string must start with its own leading part");
  static_assert(!CStringView(c_sample).starts_with(CStringView(c_sampleLonger)),
                "a string cannot start with a longer one");
  static_assert(CStringView(c_sample).starts_with('p'), "the character overload must read the first byte");
  static_assert(CStringView(c_sample).starts_with(c_samplePrefix),
                "the pointer overload must measure its argument and match the same characters");
}

// Whether the viewed string closes with the characters the argument names.
TEST_CASE("c_string_view/ends_with") {
  const CStringView view(c_sample);

  CHECK(view.ends_with(CStringView("yer")));
  CHECK(view.ends_with(CStringView(c_sample)));
  CHECK_FALSE(view.ends_with(CStringView(c_samplePrefix)));
  CHECK(view.ends_with(CStringView("")));

  CHECK(view.ends_with('r'));
  CHECK_FALSE(view.ends_with('e'));
  CHECK_FALSE(CStringView("").ends_with('r'));

  CHECK(view.ends_with("yer"));
  CHECK_FALSE(view.ends_with(c_sampleLonger));

  static_assert(CStringView(c_sample).ends_with(CStringView("yer")), "a string must end with its own trailing part");
  static_assert(!CStringView(c_sample).ends_with(CStringView(c_sampleLonger)), "a string cannot end with a longer one");
  static_assert(CStringView(c_sample).ends_with('r'), "the character overload must read the last byte");
  static_assert(CStringView(c_sample).ends_with("yer"),
                "the pointer overload must measure its argument and match the same characters");
}

// Where a forward search first matches the characters the argument names.
TEST_CASE("c_string_view/find") {
  const CStringView view(c_repeated);

  CHECK(view.find(CStringView("abra")) == 0);
  CHECK(view.find(CStringView("abra"), 1) == 7);
  CHECK(view.find(CStringView("abra"), 8) == CStringView::npos);
  CHECK(view.find(CStringView("cad")) == 4);
  CHECK(view.find(CStringView("xyz")) == CStringView::npos);

  // A needle longer than the viewed string matches nowhere.
  CHECK(view.find(CStringView(c_sampleLonger)) == CStringView::npos);

  // An empty needle matches at the offset the search starts from; past the end it matches nowhere.
  CHECK(view.find(CStringView("")) == 0);
  CHECK(view.find(CStringView(""), c_repeatedLength) == c_repeatedLength);
  CHECK(view.find(CStringView(""), c_repeatedLength + 1) == CStringView::npos);

  // The counted overload reads only the leading characters its count names.
  CHECK(view.find("abrasive", 0, 4) == 0);
  CHECK(view.find("abrasive", 1, 4) == 7);

  CHECK(view.find("cad") == 4);
  CHECK(view.find("cad", 5) == CStringView::npos);

  static_assert(CStringView(c_repeated).find(CStringView("abra")) == 0,
                "a search must report the first offset the needle matches at");
  static_assert(CStringView(c_repeated).find(CStringView("abra"), 1) == 7,
                "a search must skip a match starting before the offset it was given");
  static_assert(CStringView(c_repeated).find(CStringView("xyz")) == CStringView::npos,
                "a needle absent from the string must report npos");
  static_assert(CStringView(c_repeated).find(CStringView("")) == 0,
                "an empty needle must match at the offset the search starts from");
  static_assert(CStringView(c_repeated).find("abrasive", 1, 4) == 7,
                "the counted overload must read only the characters its count names");
  static_assert(CStringView(c_repeated).find("cad") == 4,
                "the pointer overload must measure its argument and match the same characters");
}

// Where a forward search first meets the character the argument names.
TEST_CASE("c_string_view/find_character") {
  const CStringView view(c_repeated);

  CHECK(view.find('a') == 0);
  CHECK(view.find('a', 1) == 3);
  CHECK(view.find('d') == 6);
  CHECK(view.find('z') == CStringView::npos);

  // A start at or past the last character matches nowhere.
  CHECK(view.find('a', c_repeatedLength) == CStringView::npos);
  CHECK(CStringView("").find('a') == CStringView::npos);

  static_assert(CStringView(c_repeated).find('a') == 0,
                "a character search must report the first offset the character sits at");
  static_assert(CStringView(c_repeated).find('a', 1) == 3,
                "a character search must skip an occurrence before the offset it was given");
  static_assert(CStringView(c_repeated).find('z') == CStringView::npos,
                "a character absent from the string must report npos");
}

// Where a backward search last matches the characters the argument names.
TEST_CASE("c_string_view/rfind") {
  const CStringView view(c_repeated);

  CHECK(view.rfind(CStringView("abra")) == 7);
  CHECK(view.rfind(CStringView("abra"), 6) == 0);
  CHECK(view.rfind(CStringView("abra"), 0) == 0);
  CHECK(view.rfind(CStringView("cad")) == 4);
  CHECK(view.rfind(CStringView("xyz")) == CStringView::npos);
  CHECK(view.rfind(CStringView(c_sampleLonger)) == CStringView::npos);

  // An empty needle matches at the offset the search starts from, capped at the length.
  CHECK(view.rfind(CStringView("")) == c_repeatedLength);
  CHECK(view.rfind(CStringView(""), 5) == 5);

  CHECK(view.rfind("abrasive", CStringView::npos, 4) == 7);
  CHECK(view.rfind("cad") == 4);
  CHECK(view.rfind("cad", 3) == CStringView::npos);

  static_assert(CStringView(c_repeated).rfind(CStringView("abra")) == 7,
                "a backward search must report the last offset the needle matches at");
  static_assert(CStringView(c_repeated).rfind(CStringView("abra"), 6) == 0,
                "the offset caps where a match may start, not where it may end");
  static_assert(CStringView(c_repeated).rfind(CStringView("xyz")) == CStringView::npos,
                "a needle absent from the string must report npos");
  static_assert(CStringView(c_repeated).rfind(CStringView("")) == c_repeatedLength,
                "an empty needle must match at the end when the search starts there");
  static_assert(CStringView(c_repeated).rfind("abrasive", CStringView::npos, 4) == 7,
                "the counted overload must read only the characters its count names");
  static_assert(CStringView(c_repeated).rfind("cad") == 4,
                "the pointer overload must measure its argument and match the same characters");
}

// Where a backward search last meets the character the argument names.
TEST_CASE("c_string_view/rfind_character") {
  const CStringView view(c_repeated);

  CHECK(view.rfind('a') == c_repeatedLength - 1);
  CHECK(view.rfind('a', 9) == 7);
  CHECK(view.rfind('d') == 6);
  CHECK(view.rfind('z') == CStringView::npos);

  // An offset of zero leaves the first character as the only candidate.
  CHECK(view.rfind('b', 0) == CStringView::npos);
  CHECK(view.rfind('a', 0) == 0);
  CHECK(CStringView("").rfind('a') == CStringView::npos);

  static_assert(CStringView(c_repeated).rfind('a') == c_repeatedLength - 1,
                "a backward character search must report the last offset the character sits at");
  static_assert(CStringView(c_repeated).rfind('a', 9) == 7,
                "the offset caps how far back the search may report a match");
  static_assert(CStringView(c_repeated).rfind('z') == CStringView::npos,
                "a character absent from the string must report npos");
}

// Whether the characters the argument names appear anywhere in the viewed string.
TEST_CASE("c_string_view/contains") {
  const CStringView view(c_repeated);

  CHECK(view.contains(CStringView("cad")));
  CHECK_FALSE(view.contains(CStringView("xyz")));
  CHECK(view.contains(CStringView("")));

  CHECK(view.contains('d'));
  CHECK_FALSE(view.contains('z'));

  CHECK(view.contains("dabra"));
  CHECK_FALSE(view.contains("dabraz"));

  static_assert(CStringView(c_repeated).contains(CStringView("cad")),
                "a needle the string holds must report as contained");
  static_assert(!CStringView(c_repeated).contains(CStringView("xyz")),
                "a needle the string lacks must not report as contained");
  static_assert(CStringView(c_repeated).contains('d'), "the character overload must read the same occurrences");
  static_assert(CStringView(c_repeated).contains("dabra"),
                "the pointer overload must measure its argument and match the same characters");
}

// Where a forward search first meets any character the argument's set holds.
TEST_CASE("c_string_view/find_first_of") {
  const CStringView view(c_repeated);

  CHECK(view.find_first_of(CStringView("rc")) == 2);
  CHECK(view.find_first_of(CStringView("rc"), 3) == 4);
  CHECK(view.find_first_of(CStringView("xyz")) == CStringView::npos);

  // An empty set holds no character to match, unlike an empty needle in a substring search.
  CHECK(view.find_first_of(CStringView("")) == CStringView::npos);
  CHECK(CStringView("").find_first_of(CStringView("a")) == CStringView::npos);

  // A set of one character reads the same as a character search.
  CHECK(view.find_first_of('d') == 6);
  CHECK(view.find_first_of('a', 1) == 3);
  CHECK(view.find_first_of('z') == CStringView::npos);

  CHECK(view.find_first_of("rcxyz", 0, 2) == 2);
  CHECK(view.find_first_of("rcxyz", 3, 2) == 4);

  CHECK(view.find_first_of("rc") == 2);
  CHECK(view.find_first_of("rc", 10) == CStringView::npos);

  static_assert(CStringView(c_repeated).find_first_of(CStringView("rc")) == 2,
                "a set search must report the first offset any of its characters sits at");
  static_assert(CStringView(c_repeated).find_first_of(CStringView("rc"), 3) == 4,
                "a set search must skip an occurrence before the offset it was given");
  static_assert(CStringView(c_repeated).find_first_of(CStringView("")) == CStringView::npos,
                "an empty set must match nowhere");
  static_assert(CStringView(c_repeated).find_first_of('d') == 6,
                "the character overload must read the same occurrences as a set of one");
  static_assert(CStringView(c_repeated).find_first_of("rcxyz", 3, 2) == 4,
                "the counted overload must read only the characters its count names");
  static_assert(CStringView(c_repeated).find_first_of("rc") == 2,
                "the pointer overload must measure its argument and match the same set");
}

// Where a backward search last meets any character the argument's set holds.
TEST_CASE("c_string_view/find_last_of") {
  const CStringView view(c_repeated);

  CHECK(view.find_last_of(CStringView("rc")) == 9);
  CHECK(view.find_last_of(CStringView("rc"), 8) == 4);
  CHECK(view.find_last_of(CStringView("xyz")) == CStringView::npos);

  CHECK(view.find_last_of(CStringView("")) == CStringView::npos);
  CHECK(CStringView("").find_last_of(CStringView("a")) == CStringView::npos);

  CHECK(view.find_last_of('a') == c_repeatedLength - 1);
  CHECK(view.find_last_of('a', 9) == 7);
  CHECK(view.find_last_of('z') == CStringView::npos);

  CHECK(view.find_last_of("rcxyz", CStringView::npos, 2) == 9);
  CHECK(view.find_last_of("rcxyz", 8, 2) == 4);

  CHECK(view.find_last_of("rc") == 9);
  CHECK(view.find_last_of("rc", 1) == CStringView::npos);

  static_assert(CStringView(c_repeated).find_last_of(CStringView("rc")) == 9,
                "a backward set search must report the last offset any of its characters sits at");
  static_assert(CStringView(c_repeated).find_last_of(CStringView("rc"), 8) == 4,
                "the offset caps how far back the search may report a match");
  static_assert(CStringView(c_repeated).find_last_of(CStringView("")) == CStringView::npos,
                "an empty set must match nowhere");
  static_assert(CStringView(c_repeated).find_last_of('a') == c_repeatedLength - 1,
                "the character overload must read the same occurrences as a set of one");
  static_assert(CStringView(c_repeated).find_last_of("rcxyz", 8, 2) == 4,
                "the counted overload must read only the characters its count names");
  static_assert(CStringView(c_repeated).find_last_of("rc") == 9,
                "the pointer overload must measure its argument and match the same set");
}

// Where a forward search first meets a character the argument's set does not hold.
TEST_CASE("c_string_view/find_first_not_of") {
  const CStringView view(c_repeated);

  CHECK(view.find_first_not_of(CStringView("ab")) == 2);
  CHECK(view.find_first_not_of(CStringView("ab"), 3) == 4);
  CHECK(view.find_first_not_of(CStringView("abr")) == 4);

  // A set holding every character the string uses leaves nothing to report.
  CHECK(view.find_first_not_of(CStringView("abcdr")) == CStringView::npos);

  // An empty set excludes no character, so the search stops where it starts.
  CHECK(view.find_first_not_of(CStringView("")) == 0);
  CHECK(CStringView("").find_first_not_of(CStringView("")) == CStringView::npos);

  CHECK(view.find_first_not_of('a') == 1);
  CHECK(view.find_first_not_of('a', 3) == 4);
  CHECK(CStringView("aaa").find_first_not_of('a') == CStringView::npos);

  CHECK(view.find_first_not_of("abxyz", 0, 2) == 2);
  CHECK(view.find_first_not_of("abxyz", 3, 2) == 4);

  CHECK(view.find_first_not_of("ab") == 2);
  CHECK(view.find_first_not_of("ab", 10) == CStringView::npos);

  static_assert(CStringView(c_repeated).find_first_not_of(CStringView("ab")) == 2,
                "the search must report the first offset the set leaves out");
  static_assert(CStringView(c_repeated).find_first_not_of(CStringView("ab"), 3) == 4,
                "the search must skip an excluded character before the offset it was given");
  static_assert(CStringView(c_repeated).find_first_not_of(CStringView("abcdr")) == CStringView::npos,
                "a set covering every character must report npos");
  static_assert(CStringView(c_repeated).find_first_not_of(CStringView("")) == 0,
                "an empty set must leave the first character reported");
  static_assert(CStringView(c_repeated).find_first_not_of('a') == 1,
                "the character overload must exclude that character alone");
  static_assert(CStringView(c_repeated).find_first_not_of("abxyz", 3, 2) == 4,
                "the counted overload must read only the characters its count names");
  static_assert(CStringView(c_repeated).find_first_not_of("ab") == 2,
                "the pointer overload must measure its argument and exclude the same set");
}

// Where a backward search last meets a character the argument's set does not hold.
TEST_CASE("c_string_view/find_last_not_of") {
  const CStringView view(c_repeated);

  CHECK(view.find_last_not_of(CStringView("ab")) == 9);
  CHECK(view.find_last_not_of(CStringView("ab"), 8) == 6);
  CHECK(view.find_last_not_of(CStringView("abcdr")) == CStringView::npos);

  CHECK(view.find_last_not_of(CStringView("")) == c_repeatedLength - 1);
  CHECK(CStringView("").find_last_not_of(CStringView("")) == CStringView::npos);

  CHECK(view.find_last_not_of('a') == 9);
  CHECK(view.find_last_not_of('a', 8) == 8);
  CHECK(CStringView("aaa").find_last_not_of('a') == CStringView::npos);

  CHECK(view.find_last_not_of("abxyz", CStringView::npos, 2) == 9);
  CHECK(view.find_last_not_of("abxyz", 8, 2) == 6);

  CHECK(view.find_last_not_of("ab") == 9);
  CHECK(view.find_last_not_of("ab", 1) == CStringView::npos);

  static_assert(CStringView(c_repeated).find_last_not_of(CStringView("ab")) == 9,
                "the search must report the last offset the set leaves out");
  static_assert(CStringView(c_repeated).find_last_not_of(CStringView("ab"), 8) == 6,
                "the offset caps how far back the search may report a match");
  static_assert(CStringView(c_repeated).find_last_not_of(CStringView("abcdr")) == CStringView::npos,
                "a set covering every character must report npos");
  static_assert(CStringView(c_repeated).find_last_not_of(CStringView("")) == c_repeatedLength - 1,
                "an empty set must leave the last character reported");
  static_assert(CStringView(c_repeated).find_last_not_of('a') == 9,
                "the character overload must exclude that character alone");
  static_assert(CStringView(c_repeated).find_last_not_of("abxyz", 8, 2) == 6,
                "the counted overload must read only the characters its count names");
  static_assert(CStringView(c_repeated).find_last_not_of("ab") == 9,
                "the pointer overload must measure its argument and exclude the same set");
}

} // namespace toy
