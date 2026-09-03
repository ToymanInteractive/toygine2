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
constexpr const char * c_sample          = "player";
constexpr size_t       c_sampleLength    = char_traits<char>::length(c_sample);
// The same characters back to front, the order a reverse walk must yield.
constexpr const char * c_sampleReverse   = "reyalp";
// A leading part of the sample, and a longer string starting with it.
constexpr const char * c_samplePrefix    = "play";
constexpr const char * c_sampleLonger    = "player one";
// Characters without a terminator, the buffer shape a counted comparison has to accept.
constexpr char         c_countedPart[4]  = {'p', 'l', 'a', 'y'};
// A terminator inside the counted range, which a measured length would stop at.
constexpr char         c_embeddedNull[5] = {'p', 'l', '\0', 'a', 'y'};

// A literal whose groups repeat, so a search has more than one candidate to pick between.
constexpr const char * c_repeated       = "abracadabra";
constexpr const char * c_repeatedLonger = "abracadabra and then some";

// Views the compile-time assertions read, so a constant expression names one instead of rebuilding it each time.
constexpr CStringView c_sampleView(c_sample);
constexpr CStringView c_repeatedView(c_repeated);

// Offset every search reports when it matches nothing.
constexpr size_t c_npos           = CStringView::npos;
constexpr size_t c_repeatedLength = char_traits<char>::length(c_repeated);

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

// Byte no copy ever writes, so anything the call leaves past its count shows up against it.
constexpr char c_sentinel = '#';

// Caller storage prefilled with the sentinel.
[[nodiscard]] constexpr array<char, 16> sentinelBuffer() noexcept {
  array<char, 16> buffer{};
  buffer.fill(c_sentinel);

  return buffer;
}

// Count a copy writes, so a constant expression can drive the one member that touches caller storage.
[[nodiscard]] constexpr size_t copiedCount(const char * string, size_t count, size_t pos) noexcept {
  array<char, 16> buffer = sentinelBuffer();

  return CStringView(string).copy(buffer.data(), count, pos);
}

// Character a copy leaves at an offset in caller storage.
[[nodiscard]] constexpr char copiedAt(const char * string, size_t count, size_t index) noexcept {
  array<char, 16> buffer = sentinelBuffer();
  CStringView(string).copy(buffer.data(), count);

  return buffer[index];
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
  static_assert(*c_sampleView.begin() == 'p', "the first character must be the literal's first byte");
  static_assert(*c_sampleView.end() == '\0', "the position past the last character holds the terminator");
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
  static_assert(*c_sampleView.rbegin() == 'r', "a reverse walk must start at the last character");
  static_assert(c_sampleView.rend().base() == c_sampleView.begin(), "the reverse end must adapt the forward beginning");
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
  static_assert(c_sampleView.at(c_sampleLength - 1) == 'r', "at() must read the same byte as operator[]");
  static_assert(c_sampleView.front() == 'p', "the front character must be the literal's first byte");
  static_assert(c_sampleView.back() == 'r', "the back character must be the literal's last byte");
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

  static_assert(c_sampleView.data() == c_sample, "the view must hand back the pointer it was built from");
  static_assert(c_sampleView.c_str()[c_sampleLength] == '\0',
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

  static_assert(c_sampleView.size() == c_sampleLength, "size() must report the literal's UTF-8 byte count");
  static_assert(c_sampleView.length() == c_sampleView.size(), "length() and size() must report the same count");
  static_assert(CStringView().empty(), "a default-constructed view holds no character");
  static_assert(!c_sampleView.empty(), "a view over a non-empty literal holds characters");
  static_assert(c_npos == static_cast<CStringView::size_type>(-1),
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

  array<char, 16> buffer = sentinelBuffer();
  CHECK(view.copy(buffer.data(), 4) == 4);
  CHECK(char_traits<char>::compare(buffer.data(), c_samplePrefix, 4) == 0);

  // The call writes characters only; the sentinel past the count survives, so no terminator was added.
  CHECK(buffer[4] == c_sentinel);

  // A count past the end copies what remains rather than reading past the terminator.
  array<char, 16> tail = sentinelBuffer();
  CHECK(view.copy(tail.data(), 32, 2) == c_sampleLength - 2);
  CHECK(char_traits<char>::compare(tail.data(), "ayer", 4) == 0);
  CHECK(tail[c_sampleLength - 2] == c_sentinel);

  // Starting at the end names an empty range rather than a range out of bounds.
  array<char, 16> none = sentinelBuffer();
  CHECK(view.copy(none.data(), 4, c_sampleLength) == 0);
  CHECK(none[0] == c_sentinel);

  // A view holding no character can be copied from; there is simply nothing to write.
  CHECK(CStringView("").copy(none.data(), 4) == 0);
  CHECK(CStringView().copy(none.data(), 4) == 0);
  CHECK(none[0] == c_sentinel);

  static_assert(copiedCount(c_sample, 4, 0) == 4, "a copy writes what the count names");
  static_assert(copiedCount(c_sample, 32, 2) == c_sampleLength - 2, "a copy stops at the end of the string");
  static_assert(copiedCount(c_sample, 4, c_sampleLength) == 0, "a copy starting at the end writes nothing");
  static_assert(copiedAt(c_sample, 4, 0) == 'p', "a copy writes the characters the view holds");
  static_assert(copiedAt(c_sample, 4, 4) == c_sentinel, "a copy writes nothing past its count");
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

  static_assert(c_sampleView.compare(CStringView(c_sample)) == 0, "a string must compare equal to itself");
  static_assert(c_sampleView.compare(CStringView(c_samplePrefix)) > 0,
                "a longer string sharing a prefix must order after the shorter one");
  static_assert(c_sampleView.compare(c_sampleLonger) < 0,
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

  // Equal-length parts that differ order on the first differing character, with no length left to decide.
  CHECK(view.compare(0, 4, CStringView("plaz"), 0, 4) < 0);
  CHECK(view.compare(0, 4, CStringView("plaa"), 0, 4) > 0);

  CHECK(view.compare(0, 4, c_samplePrefix) == 0);
  CHECK(view.compare(0, 4, c_sampleLonger, 4) == 0);

  // The counted overload reads exactly the count, so the argument needs no terminator and may hold one inside.
  CHECK(view.compare(0, 4, c_countedPart, 4) == 0);
  CHECK(view.compare(0, 2, c_countedPart, 2) == 0);
  CHECK(view.compare(0, 4, c_embeddedNull, 4) > 0);
  CHECK(CStringView("pl").compare(0, 2, c_embeddedNull, 4) < 0);

  static_assert(c_sampleView.compare(0, 4, CStringView(c_samplePrefix)) == 0,
                "a part must compare equal to the string it repeats");
  static_assert(c_sampleView.compare(0, 4, CStringView(c_sample)) < 0,
                "a shorter part must order before the whole string it starts");
  static_assert(c_sampleView.compare(0, 4, c_sampleLonger, 4) == 0, "the count bounds what is read");
  static_assert(c_sampleView.compare(0, 4, CStringView("plaz"), 0, 4) < 0,
                "equal-length parts must order on the first differing character");
  static_assert(c_sampleView.compare(0, 4, c_countedPart, 4) == 0,
                "a counted argument needs no terminator to compare against");
  static_assert(c_sampleView.compare(0, 4, c_embeddedNull, 4) > 0,
                "a terminator inside the count is an ordinary character");
  static_assert(CStringView("pl").compare(0, 2, c_embeddedNull, 4) < 0, "the count sets the length, not a terminator");
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

  static_assert(c_sampleView.starts_with(CStringView(c_samplePrefix)), "a string must start with its own leading part");
  static_assert(!c_sampleView.starts_with(CStringView(c_sampleLonger)), "a string cannot start with a longer one");
  static_assert(c_sampleView.starts_with('p'), "the character overload reads the first byte");
  static_assert(c_sampleView.starts_with(c_samplePrefix), "the pointer overload measures its argument");
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

  static_assert(c_sampleView.ends_with(CStringView("yer")), "a string must end with its own trailing part");
  static_assert(!c_sampleView.ends_with(CStringView(c_sampleLonger)), "a string cannot end with a longer one");
  static_assert(c_sampleView.ends_with('r'), "the character overload reads the last byte");
  static_assert(c_sampleView.ends_with("yer"), "the pointer overload measures its argument");
}

// Where a forward search first matches the characters the argument names.
TEST_CASE("c_string_view/find") {
  const CStringView view(c_repeated);

  CHECK(view.find(CStringView("abra")) == 0);
  CHECK(view.find(CStringView("abra"), 1) == 7);
  CHECK(view.find(CStringView("abra"), 8) == c_npos);
  CHECK(view.find(CStringView("cad")) == 4);
  CHECK(view.find(CStringView("xyz")) == c_npos);

  // A needle longer than the viewed string matches nowhere, decided on the lengths before any character is read.
  CHECK(view.find(CStringView(c_repeatedLonger)) == c_npos);
  CHECK(view.find(c_repeatedLonger, 0, char_traits<char>::length(c_repeatedLonger)) == c_npos);

  // An empty needle matches at the offset the search starts from; past the end it matches nowhere.
  CHECK(view.find(CStringView("")) == 0);
  CHECK(view.find(CStringView(""), c_repeatedLength) == c_repeatedLength);
  CHECK(view.find(CStringView(""), c_repeatedLength + 1) == c_npos);

  // The counted overload reads only the leading characters its count names.
  CHECK(view.find("abrasive", 0, 4) == 0);
  CHECK(view.find("abrasive", 1, 4) == 7);

  CHECK(view.find("cad") == 4);
  CHECK(view.find("cad", 5) == c_npos);

  // The character overload reads single characters rather than a sequence.
  CHECK(view.find('a') == 0);
  CHECK(view.find('a', 1) == 3);
  CHECK(view.find('d') == 6);
  CHECK(view.find('z') == c_npos);

  // A start at or past the last character matches nowhere.
  CHECK(view.find('a', c_repeatedLength) == c_npos);
  CHECK(CStringView("").find('a') == c_npos);

  static_assert(c_repeatedView.find('a') == 0, "the first occurrence wins");
  static_assert(c_repeatedView.find('a', 1) == 3, "the offset skips earlier matches");
  static_assert(c_repeatedView.find('z') == c_npos, "an absent character reports npos");
  static_assert(c_repeatedView.find(CStringView("abra")) == 0, "the first match wins");
  static_assert(c_repeatedView.find(CStringView("abra"), 1) == 7, "the offset skips earlier matches");
  static_assert(c_repeatedView.find(CStringView("xyz")) == c_npos, "an absent needle reports npos");
  static_assert(c_repeatedView.find(CStringView(c_repeatedLonger)) == c_npos,
                "a needle longer than the string reports npos");
  static_assert(c_repeatedView.find(CStringView("")) == 0, "an empty needle matches at the start offset");
  static_assert(c_repeatedView.find("abrasive", 1, 4) == 7, "the count bounds what is read");
  static_assert(c_repeatedView.find("cad") == 4, "the pointer overload measures its argument");
}

// Where a backward search last matches the characters the argument names.
TEST_CASE("c_string_view/rfind") {
  const CStringView view(c_repeated);

  CHECK(view.rfind(CStringView("abra")) == 7);
  CHECK(view.rfind(CStringView("abra"), 6) == 0);
  CHECK(view.rfind(CStringView("abra"), 0) == 0);
  CHECK(view.rfind(CStringView("cad")) == 4);
  CHECK(view.rfind(CStringView("xyz")) == c_npos);
  CHECK(view.rfind(CStringView(c_repeatedLonger)) == c_npos);
  CHECK(view.rfind(c_repeatedLonger, c_npos, char_traits<char>::length(c_repeatedLonger)) == c_npos);

  // An empty needle matches at the offset the search starts from, capped at the length.
  CHECK(view.rfind(CStringView("")) == c_repeatedLength);
  CHECK(view.rfind(CStringView(""), 5) == 5);

  CHECK(view.rfind("abrasive", c_npos, 4) == 7);
  CHECK(view.rfind("cad") == 4);
  CHECK(view.rfind("cad", 3) == c_npos);

  // The character overload reads single characters rather than a sequence.
  CHECK(view.rfind('a') == c_repeatedLength - 1);
  CHECK(view.rfind('a', 9) == 7);
  CHECK(view.rfind('d') == 6);
  CHECK(view.rfind('z') == c_npos);

  // An offset of zero leaves the first character as the only candidate.
  CHECK(view.rfind('b', 0) == c_npos);
  CHECK(view.rfind('a', 0) == 0);
  CHECK(CStringView("").rfind('a') == c_npos);

  static_assert(c_repeatedView.rfind('a') == c_repeatedLength - 1, "the last occurrence wins");
  static_assert(c_repeatedView.rfind('a', 9) == 7, "the offset caps how far back a match may sit");
  static_assert(c_repeatedView.rfind('z') == c_npos, "an absent character reports npos");
  static_assert(c_repeatedView.rfind(CStringView("abra")) == 7, "the last match wins");
  static_assert(c_repeatedView.rfind(CStringView("abra"), 6) == 0, "the offset caps where a match starts");
  static_assert(c_repeatedView.rfind(CStringView("xyz")) == c_npos, "an absent needle reports npos");
  static_assert(c_repeatedView.rfind(CStringView(c_repeatedLonger)) == c_npos,
                "a needle longer than the string reports npos");
  static_assert(c_repeatedView.rfind(CStringView("")) == c_repeatedLength, "an empty needle matches at the end");
  static_assert(c_repeatedView.rfind("abrasive", c_npos, 4) == 7, "the count bounds what is read");
  static_assert(c_repeatedView.rfind("cad") == 4, "the pointer overload measures its argument");
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

  static_assert(c_repeatedView.contains(CStringView("cad")), "a needle the string holds is contained");
  static_assert(!c_repeatedView.contains(CStringView("xyz")), "a needle the string lacks is not contained");
  static_assert(c_repeatedView.contains('d'), "the character overload matches the same offsets");
  static_assert(c_repeatedView.contains("dabra"), "the pointer overload measures its argument");
}

// Where a forward search first meets any character the argument's set holds.
TEST_CASE("c_string_view/find_first_of") {
  const CStringView view(c_repeated);

  CHECK(view.find_first_of(CStringView("rc")) == 2);
  CHECK(view.find_first_of(CStringView("rc"), 3) == 4);
  CHECK(view.find_first_of(CStringView("xyz")) == c_npos);

  // An empty set holds no character to match, unlike an empty needle in a substring search.
  CHECK(view.find_first_of(CStringView("")) == c_npos);
  CHECK(CStringView("").find_first_of(CStringView("a")) == c_npos);

  // A set of one character reads the same as a character search.
  CHECK(view.find_first_of('d') == 6);
  CHECK(view.find_first_of('a', 1) == 3);
  CHECK(view.find_first_of('z') == c_npos);

  CHECK(view.find_first_of("rcxyz", 0, 2) == 2);
  CHECK(view.find_first_of("rcxyz", 3, 2) == 4);

  CHECK(view.find_first_of("rc") == 2);
  CHECK(view.find_first_of("rc", 10) == c_npos);

  static_assert(c_repeatedView.find_first_of(CStringView("rc")) == 2, "the first set member wins");
  static_assert(c_repeatedView.find_first_of(CStringView("rc"), 3) == 4, "the offset skips earlier matches");
  static_assert(c_repeatedView.find_first_of(CStringView("")) == c_npos, "an empty set matches nowhere");
  static_assert(c_repeatedView.find_first_of('d') == 6, "a set of one is that character");
  static_assert(c_repeatedView.find_first_of("rcxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(c_repeatedView.find_first_of("rc") == 2, "the pointer overload measures its argument");
}

// Where a backward search last meets any character the argument's set holds.
TEST_CASE("c_string_view/find_last_of") {
  const CStringView view(c_repeated);

  CHECK(view.find_last_of(CStringView("rc")) == 9);
  CHECK(view.find_last_of(CStringView("rc"), 8) == 4);
  CHECK(view.find_last_of(CStringView("xyz")) == c_npos);

  CHECK(view.find_last_of(CStringView("")) == c_npos);
  CHECK(CStringView("").find_last_of(CStringView("a")) == c_npos);

  CHECK(view.find_last_of('a') == c_repeatedLength - 1);
  CHECK(view.find_last_of('a', 9) == 7);
  CHECK(view.find_last_of('z') == c_npos);

  CHECK(view.find_last_of("rcxyz", c_npos, 2) == 9);
  CHECK(view.find_last_of("rcxyz", 8, 2) == 4);

  CHECK(view.find_last_of("rc") == 9);
  CHECK(view.find_last_of("rc", 1) == c_npos);

  static_assert(c_repeatedView.find_last_of(CStringView("rc")) == 9, "the last set member wins");
  static_assert(c_repeatedView.find_last_of(CStringView("rc"), 8) == 4, "the offset caps how far back a match may sit");
  static_assert(c_repeatedView.find_last_of(CStringView("")) == c_npos, "an empty set matches nowhere");
  static_assert(c_repeatedView.find_last_of('a') == c_repeatedLength - 1, "a set of one is that character");
  static_assert(c_repeatedView.find_last_of("rcxyz", 8, 2) == 4, "the count bounds what is read");
  static_assert(c_repeatedView.find_last_of("rc") == 9, "the pointer overload measures its argument");
}

// Where a forward search first meets a character the argument's set does not hold.
TEST_CASE("c_string_view/find_first_not_of") {
  const CStringView view(c_repeated);

  CHECK(view.find_first_not_of(CStringView("ab")) == 2);
  CHECK(view.find_first_not_of(CStringView("ab"), 3) == 4);
  CHECK(view.find_first_not_of(CStringView("abr")) == 4);

  // A set holding every character the string uses leaves nothing to report.
  CHECK(view.find_first_not_of(CStringView("abcdr")) == c_npos);

  // An empty set excludes no character, so the search stops where it starts.
  CHECK(view.find_first_not_of(CStringView("")) == 0);
  CHECK(CStringView("").find_first_not_of(CStringView("")) == c_npos);

  CHECK(view.find_first_not_of('a') == 1);
  CHECK(view.find_first_not_of('a', 3) == 4);
  CHECK(CStringView("aaa").find_first_not_of('a') == c_npos);

  CHECK(view.find_first_not_of("abxyz", 0, 2) == 2);
  CHECK(view.find_first_not_of("abxyz", 3, 2) == 4);

  CHECK(view.find_first_not_of("ab") == 2);
  CHECK(view.find_first_not_of("ab", 10) == c_npos);

  static_assert(c_repeatedView.find_first_not_of(CStringView("ab")) == 2, "the first excluded character wins");
  static_assert(c_repeatedView.find_first_not_of(CStringView("ab"), 3) == 4, "the offset skips earlier matches");
  static_assert(c_repeatedView.find_first_not_of(CStringView("abcdr")) == c_npos,
                "a set covering every character reports npos");
  static_assert(c_repeatedView.find_first_not_of(CStringView("")) == 0, "an empty set excludes nothing");
  static_assert(c_repeatedView.find_first_not_of('a') == 1, "the character overload excludes one character");
  static_assert(c_repeatedView.find_first_not_of("abxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(c_repeatedView.find_first_not_of("ab") == 2, "the pointer overload measures its argument");
}

// Where a backward search last meets a character the argument's set does not hold.
TEST_CASE("c_string_view/find_last_not_of") {
  const CStringView view(c_repeated);

  CHECK(view.find_last_not_of(CStringView("ab")) == 9);
  CHECK(view.find_last_not_of(CStringView("ab"), 8) == 6);
  CHECK(view.find_last_not_of(CStringView("abcdr")) == c_npos);

  CHECK(view.find_last_not_of(CStringView("")) == c_repeatedLength - 1);
  CHECK(CStringView("").find_last_not_of(CStringView("")) == c_npos);

  CHECK(view.find_last_not_of('a') == 9);
  CHECK(view.find_last_not_of('a', 8) == 8);
  CHECK(CStringView("aaa").find_last_not_of('a') == c_npos);

  CHECK(view.find_last_not_of("abxyz", c_npos, 2) == 9);
  CHECK(view.find_last_not_of("abxyz", 8, 2) == 6);

  CHECK(view.find_last_not_of("ab") == 9);
  CHECK(view.find_last_not_of("ab", 1) == c_npos);

  static_assert(c_repeatedView.find_last_not_of(CStringView("ab")) == 9, "the last excluded character wins");
  static_assert(c_repeatedView.find_last_not_of(CStringView("ab"), 8) == 6,
                "the offset caps how far back a match may sit");
  static_assert(c_repeatedView.find_last_not_of(CStringView("abcdr")) == c_npos,
                "a set covering every character reports npos");
  static_assert(c_repeatedView.find_last_not_of(CStringView("")) == c_repeatedLength - 1,
                "an empty set excludes nothing");
  static_assert(c_repeatedView.find_last_not_of('a') == 9, "the character overload excludes one character");
  static_assert(c_repeatedView.find_last_not_of("abxyz", 8, 2) == 6, "the count bounds what is read");
  static_assert(c_repeatedView.find_last_not_of("ab") == 9, "the pointer overload measures its argument");
}

} // namespace toy
