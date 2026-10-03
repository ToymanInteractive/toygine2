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
  \file   string_view.test.cpp
  \brief  Unit tests for \ref toy::StringView.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// The literal every case reads. Its length is measured from the literal, not written out as a constant.
constexpr const char * c_sample          = "player";
constexpr size_t       c_sampleLength    = std::char_traits<char>::length(c_sample);
// The same characters back to front, the order a reverse walk must yield.
constexpr const char * c_sampleReverse   = "reyalp";
// A leading part of the sample, and a longer string starting with it.
constexpr const char * c_samplePrefix    = "play";
constexpr const char * c_sampleLonger    = "player one";
// Characters without a terminator, the buffer shape a counted comparison has to accept.
constexpr char         c_countedPart[4]  = {'p', 'l', 'a', 'y'};
// A terminator inside the counted range, which a measured length would stop at.
constexpr char         c_embeddedNull[5] = {'p', 'l', '\0', 'a', 'y'};

// A byte above the ASCII range, which orders after an ASCII one only where bytes compare as unsigned char.
constexpr const char * c_highByte = "\xff";

// A literal whose groups repeat, so a search has more than one candidate to pick between.
constexpr const char * c_repeated = "abracadabra";

// Views the compile-time assertions read, so a constant expression names one instead of rebuilding it each time.
constexpr StringView c_sampleView(c_sample);
constexpr StringView c_repeatedView(c_repeated);

// Offset every search reports when it matches nothing.
constexpr size_t c_npos           = StringView::npos;
constexpr size_t c_repeatedLength = std::char_traits<char>::length(c_repeated);

// Distance a forward walk covers, as a length rather than a pointer difference.
[[nodiscard]] constexpr size_t forwardLength(const StringView & view) noexcept {
  return static_cast<size_t>(view.end() - view.begin());
}

// Distance a back-to-front walk covers.
[[nodiscard]] constexpr size_t reverseLength(const StringView & view) noexcept {
  return static_cast<size_t>(view.rend() - view.rbegin());
}

// The view left after dropping the leading characters, so a mutation reads back in a constant expression.
[[nodiscard]] constexpr StringView withoutPrefix(const char * string, size_t count) noexcept {
  StringView view(string);
  view.remove_prefix(count);

  return view;
}

// Byte no copy ever writes, so anything the call leaves past its count shows up against it.
constexpr char c_sentinel = '#';

// Caller storage prefilled with the sentinel.
[[nodiscard]] constexpr std::array<char, 16> sentinelBuffer() noexcept {
  std::array<char, 16> buffer{};
  buffer.fill(c_sentinel);

  return buffer;
}

// Count a copy writes, so a constant expression can drive the one member that touches caller storage.
[[nodiscard]] constexpr size_t copiedCount(const char * string, size_t count, size_t pos) noexcept {
  std::array<char, 16> buffer = sentinelBuffer();

  return StringView(string).copy(buffer.data(), count, pos);
}

// Character a copy leaves at an offset in caller storage.
[[nodiscard]] constexpr char copiedAt(const char * string, size_t count, size_t index) noexcept {
  std::array<char, 16> buffer = sentinelBuffer();
  StringView(string).copy(buffer.data(), count);

  return buffer[index];
}

// The view the first argument names after the two exchange their contents.
[[nodiscard]] constexpr StringView swapped(const char * first, const char * second) noexcept {
  StringView left(first);
  StringView right(second);
  left.swap(right);

  return left;
}

} // namespace

// What copying and leaving scope cost, which initializer a call site may write, and what a bare declaration leaves.
TEST_CASE("string_view/value_semantics") {
  static_assert(std::is_trivially_copyable_v<StringView>, "the view must copy as bytes");
  static_assert(std::is_trivially_destructible_v<StringView>, "leaving scope must run no destructor");
  static_assert(std::is_standard_layout_v<StringView>, "the layout must stay readable from a C interface");

  static_assert(!std::is_aggregate_v<StringView>,
                "the pointer and the length stay private, so no call site may brace-initialize them");
  static_assert(!std::is_trivially_default_constructible_v<StringView>,
                "a default-constructed view names no string and reports no length, which is not a trivial default");
}

// The aliases the view publishes, and the type each one has to name.
TEST_CASE("string_view/types") {
  static_assert(std::is_same_v<StringView::value_type, char>, "the view reads plain char characters");
  static_assert(std::is_same_v<StringView::const_pointer, const char *>,
                "const_pointer must name a pointer to a viewed character");
  static_assert(std::is_same_v<StringView::const_reference, const char &>,
                "const_reference must name a reference to a viewed character");

  static_assert(std::is_same_v<StringView::const_iterator, const char *>,
                "the forward iterator is a pointer into the viewed string");
  static_assert(std::is_same_v<StringView::iterator, StringView::const_iterator>,
                "the view exposes no mutable iterator, so both aliases name the same type");
  static_assert(std::is_same_v<StringView::const_reverse_iterator, std::reverse_iterator<const char *>>,
                "the reverse iterator adapts the forward one");
  static_assert(std::is_same_v<StringView::reverse_iterator, StringView::const_reverse_iterator>,
                "the view exposes no mutable reverse iterator, so both aliases name the same type");

  static_assert(std::is_same_v<StringView::size_type, size_t>, "lengths are measured in size_t");
  static_assert(std::is_same_v<StringView::difference_type, ptrdiff_t>,
                "the distance between two iterators is a ptrdiff_t");

  static_assert(std::contiguous_iterator<StringView::const_iterator>,
                "a pointer iterator must meet the contiguous iterator requirements");
  static_assert(std::random_access_iterator<StringView::const_reverse_iterator>,
                "the reverse adaptor must stay random access");
}

// What each constructor and the assignment leave behind, read back through the iterators.
TEST_CASE("string_view/construction") {
  const StringView empty;
  CHECK(forwardLength(empty) == 0);
  CHECK(empty.begin() == nullptr);

  const StringView view(c_sample);
  CHECK(forwardLength(view) == c_sampleLength);
  CHECK(view.begin() == c_sample);

  const StringView copy(view);
  CHECK(copy.begin() == view.begin());
  CHECK(forwardLength(copy) == forwardLength(view));

  StringView assigned;
  assigned = view;
  CHECK(assigned.begin() == view.begin());
  CHECK(forwardLength(assigned) == c_sampleLength);

  static_assert(forwardLength(StringView()) == 0, "a default-constructed view spans no character");
  static_assert(forwardLength(StringView(c_sample)) == c_sampleLength,
                "the pointer constructor must measure the literal's byte count");
  static_assert(forwardLength(StringView(StringView(c_sample))) == c_sampleLength,
                "a copy must span the same characters as its source");
}

// Which argument the view accepts at a call site and which one the type system rejects.
TEST_CASE("string_view/construction_from_null") {
  static_assert(std::is_convertible_v<const char *, StringView>,
                "a const char * argument must become a view without a cast");
  static_assert(!std::is_constructible_v<StringView, nullptr_t>,
                "the deleted overload must reject a literal nullptr during compilation");

  static_assert(std::is_nothrow_default_constructible_v<StringView>, "an empty view must be buildable without cost");
}

// The range a forward walk covers and the characters it yields.
TEST_CASE("string_view/iteration") {
  const StringView view(c_sample);

  CHECK(forwardLength(view) == c_sampleLength);
  CHECK(view.cbegin() == view.begin());
  CHECK(view.cend() == view.end());

  for (size_t index = 0; index < c_sampleLength; ++index) {
    INFO("index ", static_cast<long long>(index));
    CHECK(view.begin()[index] == c_sample[index]);
  }

  // The terminator sits one past the last character, unlike std::string_view.
  CHECK(*view.end() == '\0');

  const StringView empty;
  CHECK(empty.begin() == empty.end());
  CHECK(empty.cbegin() == empty.cend());

  static_assert(forwardLength(StringView(c_sample)) == c_sampleLength,
                "the forward range must span the literal's byte count");
  static_assert(*c_sampleView.begin() == 'p', "the first character must be the literal's first byte");
  static_assert(*c_sampleView.end() == '\0', "the position past the last character holds the terminator");
  static_assert(StringView().begin() == StringView().end(), "an empty view must yield an empty range");
}

// The range a back-to-front walk covers and the order it yields the characters in.
TEST_CASE("string_view/iteration_reverse") {
  const StringView view(c_sample);

  CHECK(reverseLength(view) == c_sampleLength);
  CHECK(view.crbegin() == view.rbegin());
  CHECK(view.crend() == view.rend());

  for (size_t index = 0; index < c_sampleLength; ++index) {
    INFO("index ", static_cast<long long>(index));
    CHECK(view.rbegin()[index] == c_sampleReverse[index]);
  }

  const StringView empty;
  CHECK(empty.rbegin() == empty.rend());
  CHECK(empty.crbegin() == empty.crend());

  static_assert(reverseLength(StringView(c_sample)) == c_sampleLength,
                "the reverse range must span the literal's byte count");
  static_assert(*c_sampleView.rbegin() == 'r', "a reverse walk must start at the last character");
  static_assert(c_sampleView.rend().base() == c_sampleView.begin(), "the reverse end must adapt the forward beginning");
  static_assert(StringView().rbegin() == StringView().rend(), "an empty view must yield an empty reverse range");
}

// The character each indexed access path yields, and where in the viewed string it reads it.
TEST_CASE("string_view/element_access") {
  const StringView view(c_sample);

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

  static_assert(StringView(c_sample)[0] == 'p', "an indexed read must yield the literal's byte at that offset");
  static_assert(c_sampleView.at(c_sampleLength - 1) == 'r', "at() must read the same byte as operator[]");
  static_assert(c_sampleView.front() == 'p', "the front character must be the literal's first byte");
  static_assert(c_sampleView.back() == 'r', "the back character must be the literal's last byte");
}

// The pointer the view hands to a C interface, and the terminator that pointer reaches.
TEST_CASE("string_view/pointer_access") {
  const StringView view(c_sample);

  CHECK(view.data() == c_sample);
  CHECK(view.c_str() == view.data());

  // Null-termination is the contract std::string_view does not carry.
  CHECK(view.c_str()[view.size()] == '\0');

  const StringView empty;
  CHECK(empty.data() == nullptr);
  CHECK(empty.c_str() == nullptr);

  static_assert(c_sampleView.data() == c_sample, "the view must hand back the pointer it was built from");
  static_assert(c_sampleView.c_str()[c_sampleLength] == '\0',
                "the byte past the last character must be the terminator");
  static_assert(StringView().data() == nullptr, "a default-constructed view names no string");
}

// What the view reports about its extent, and the bound a length may not cross.
TEST_CASE("string_view/capacity") {
  const StringView view(c_sample);

  CHECK(view.size() == c_sampleLength);
  CHECK(view.length() == view.size());
  CHECK_FALSE(view.empty());

  const StringView empty;
  CHECK(empty.size() == 0);
  CHECK(empty.length() == 0);
  CHECK(empty.empty());

  // A view over an empty literal reports no character, yet still names a string.
  const StringView emptyLiteral("");
  CHECK(emptyLiteral.empty());
  CHECK(emptyLiteral.data() != nullptr);

  CHECK(view.max_size() == static_cast<StringView::size_type>(-1));

  static_assert(c_sampleView.size() == c_sampleLength, "size() must report the literal's UTF-8 byte count");
  static_assert(c_sampleView.length() == c_sampleView.size(), "length() and size() must report the same count");
  static_assert(StringView().empty(), "a default-constructed view holds no character");
  static_assert(!c_sampleView.empty(), "a view over a non-empty literal holds characters");
  static_assert(c_npos == static_cast<StringView::size_type>(-1),
                "npos must be the largest value the length type carries");
}

// How many characters the viewed string holds, against the bytes it measures.
TEST_CASE("string_view/utf8_size") {
  const StringView view(c_sample);

  // An ASCII string stores one byte per character, so both counts agree.
  CHECK(view.utf8_size() == view.size());

  // Cyrillic stores two bytes per character, so the character count is half the byte count.
  const StringView cyrillic("привет");
  CHECK(cyrillic.utf8_size() == cyrillic.size() / 2);

  CHECK(StringView("").utf8_size() == 0);

  // A view over no string counts nothing, the way it reports no length.
  CHECK(StringView().utf8_size() == 0);
}

// Where the view starts reading after it drops leading characters.
TEST_CASE("string_view/remove_prefix") {
  StringView view(c_sample);

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
TEST_CASE("string_view/swap") {
  StringView left(c_sample);
  StringView right(c_samplePrefix);

  left.swap(right);
  CHECK(left.data() == c_samplePrefix);
  CHECK(left.size() == std::char_traits<char>::length(c_samplePrefix));
  CHECK(right.data() == c_sample);
  CHECK(right.size() == c_sampleLength);

  left.swap(right);
  CHECK(left.data() == c_sample);
  CHECK(right.data() == c_samplePrefix);

  static_assert(swapped(c_sample, c_samplePrefix).data() == c_samplePrefix,
                "the left view must read the string the right one named");
  static_assert(swapped(c_sample, c_samplePrefix).size() == std::char_traits<char>::length(c_samplePrefix),
                "the length must travel with the pointer");
}

// How many characters reach the caller's buffer, and which ones.
TEST_CASE("string_view/copy") {
  const StringView view(c_sample);

  std::array<char, 16> buffer = sentinelBuffer();
  CHECK(view.copy(buffer.data(), 4) == 4);
  CHECK(std::char_traits<char>::compare(buffer.data(), c_samplePrefix, 4) == 0);

  // The call writes characters only; the sentinel past the count survives, so no terminator was added.
  CHECK(buffer[4] == c_sentinel);

  // A count past the end copies what remains rather than reading past the terminator.
  std::array<char, 16> tail = sentinelBuffer();
  CHECK(view.copy(tail.data(), 32, 2) == c_sampleLength - 2);
  CHECK(std::char_traits<char>::compare(tail.data(), "ayer", 4) == 0);
  CHECK(tail[c_sampleLength - 2] == c_sentinel);

  // Starting at the end names an empty range rather than a range out of bounds.
  std::array<char, 16> none = sentinelBuffer();
  CHECK(view.copy(none.data(), 4, c_sampleLength) == 0);
  CHECK(none[0] == c_sentinel);

  // A view holding no character can be copied from, and the call writes nothing.
  CHECK(StringView("").copy(none.data(), 4) == 0);
  CHECK(StringView().copy(none.data(), 4) == 0);
  CHECK(none[0] == c_sentinel);

  static_assert(copiedCount(c_sample, 4, 0) == 4, "a copy writes what the count names");
  static_assert(copiedCount(c_sample, 32, 2) == c_sampleLength - 2, "a copy stops at the end of the string");
  static_assert(copiedCount(c_sample, 4, c_sampleLength) == 0, "a copy starting at the end writes nothing");
  static_assert(copiedAt(c_sample, 4, 0) == 'p', "a copy writes the characters the view holds");
  static_assert(copiedAt(c_sample, 4, 4) == c_sentinel, "a copy writes nothing past its count");
}

#if !defined(_DEBUG)
// Without the debug check an offset past the end copies nothing, so nothing past the view is read.
TEST_CASE("string_view/copy_rejected") {
  const StringView view(c_sample);

  std::array<char, 16> buffer = sentinelBuffer();
  CHECK(view.copy(buffer.data(), 4, c_sampleLength + 1) == 0);
  CHECK(buffer[0] == c_sentinel);

  static_assert(copiedCount(c_sample, 4, c_sampleLength + 1) == 0, "an offset past the end must copy nothing");
}
#endif // !_DEBUG

// The sign each comparison of two whole strings yields.
TEST_CASE("string_view/compare") {
  const StringView view(c_sample);

  CHECK(view.compare(StringView(c_sample)) == 0);
  CHECK(view.compare(StringView(c_samplePrefix)) > 0);
  CHECK(view.compare(StringView(c_sampleLonger)) < 0);
  CHECK(view.compare(StringView("playz")) < 0);

  // The pointer overload measures its argument and compares the same way.
  CHECK(view.compare(c_sample) == 0);
  CHECK(view.compare(c_samplePrefix) > 0);
  CHECK(view.compare(c_sampleLonger) < 0);

  static_assert(c_sampleView.compare(StringView(c_sample)) == 0, "a string must compare equal to itself");
  static_assert(c_sampleView.compare(StringView(c_samplePrefix)) > 0,
                "a longer string sharing a prefix must order after the shorter one");
  static_assert(c_sampleView.compare(c_sampleLonger) < 0,
                "the pointer overload must order by the first differing byte");
}

// The sign a comparison yields when it reads a part of one or both strings.
TEST_CASE("string_view/compare_substring") {
  const StringView view(c_sample);

  CHECK(view.compare(0, 4, StringView(c_samplePrefix)) == 0);
  CHECK(view.compare(2, 4, StringView("ayer")) == 0);
  CHECK(view.compare(0, 4, StringView(c_sample)) < 0);

  CHECK(view.compare(0, 4, StringView(c_sampleLonger), 0, 4) == 0);
  CHECK(view.compare(0, c_sampleLength, StringView(c_sampleLonger), 0, 4) > 0);

  // Equal-length parts that differ order on the first differing character, with no length left to decide.
  CHECK(view.compare(0, 4, StringView("plaz"), 0, 4) < 0);
  CHECK(view.compare(0, 4, StringView("plaa"), 0, 4) > 0);

  CHECK(view.compare(0, 4, c_samplePrefix) == 0);
  CHECK(view.compare(0, 4, c_sampleLonger, 4) == 0);

  // The counted overload reads exactly the count, so the argument needs no terminator and may hold one inside.
  CHECK(view.compare(0, 4, c_countedPart, 4) == 0);
  CHECK(view.compare(0, 2, c_countedPart, 2) == 0);
  CHECK(view.compare(0, 4, c_embeddedNull, 4) > 0);
  CHECK(StringView("pl").compare(0, 2, c_embeddedNull, 4) < 0);

  static_assert(c_sampleView.compare(0, 4, StringView(c_samplePrefix)) == 0,
                "a part must compare equal to the string it repeats");
  static_assert(c_sampleView.compare(0, 4, StringView(c_sample)) < 0,
                "a shorter part must order before the whole string it starts");
  static_assert(c_sampleView.compare(0, 4, c_sampleLonger, 4) == 0, "the count bounds what is read");
  static_assert(c_sampleView.compare(0, 4, StringView("plaz"), 0, 4) < 0,
                "equal-length parts must order on the first differing character");
  static_assert(c_sampleView.compare(0, 4, c_countedPart, 4) == 0,
                "a counted argument needs no terminator to compare against");
  static_assert(c_sampleView.compare(0, 4, c_embeddedNull, 4) > 0,
                "a terminator inside the count is an ordinary character");
  static_assert(StringView("pl").compare(0, 2, c_embeddedNull, 4) < 0, "the count sets the length, not a terminator");
}

// A count past the end of the part names the rest of it, the way copy() reads its own count.
TEST_CASE("string_view/compare_substring_clamped_count") {
  const StringView view(c_sample);

  CHECK(view.compare(0, c_npos, StringView(c_sample)) == 0);
  CHECK(view.compare(2, c_npos, StringView("ayer")) == 0);
  CHECK(view.compare(c_sampleLength, c_npos, StringView("")) == 0);

  CHECK(view.compare(0, c_npos, StringView(c_sampleLonger), 0, c_npos) < 0);
  CHECK(view.compare(2, c_npos, StringView("xayer"), 1, c_npos) == 0);

  CHECK(view.compare(0, c_npos, c_sample) == 0);
  CHECK(view.compare(2, c_npos, "ayer", 4) == 0);

  static_assert(c_sampleView.compare(0, c_npos, StringView(c_sample)) == 0,
                "a count past the end must name the rest of the string");
  static_assert(c_sampleView.compare(2, c_npos, StringView("ayer")) == 0,
                "a clamped count must settle the length tie-break on what was read");
  static_assert(c_sampleView.compare(0, c_npos, StringView(c_sampleLonger), 0, c_npos) < 0,
                "both counts must clamp to the parts they name");
}

// The sign a comparison yields against a view that holds no string, which reads no character to order on.
TEST_CASE("string_view/compare_empty") {
  const StringView empty;
  const StringView view(c_sample);

  CHECK(empty.compare(empty) == 0);
  CHECK(empty.compare(view) < 0);
  CHECK(view.compare(empty) > 0);
  CHECK(empty.compare(0, c_npos, empty) == 0);
  CHECK(empty.compare(0, c_npos, empty, 0, c_npos) == 0);

  // A null pointer with a count of zero names an empty part, as a counted std::string_view comparison accepts.
  CHECK(view.compare(0, 0, nullptr, 0) == 0);
  CHECK(view.compare(0, c_npos, nullptr, 0) > 0);

  static_assert(StringView().compare(StringView()) == 0, "two views holding no string must compare equal");
  static_assert(StringView().compare(c_sampleView) < 0, "a view holding no string must order before any string");
  static_assert(c_sampleView.compare(0, 0, nullptr, 0) == 0, "a null pointer with a count of zero must be empty");
}

#if !defined(_DEBUG)
// Without the debug checks an offset past the end clamps to it: the part is empty, and nothing past the view is read.
TEST_CASE("string_view/compare_rejected") {
  const StringView view(c_sample);

  CHECK(view.compare(c_sampleLength + 1, 1, StringView("")) == 0);
  CHECK(view.compare(c_sampleLength + 1, 1, StringView("a")) < 0);
  CHECK(view.compare(0, c_npos, view, c_sampleLength + 1, 1) > 0);
  CHECK(view.compare(c_sampleLength + 1, 1, "") == 0);
  CHECK(view.compare(c_sampleLength + 1, 1, "a", 1) < 0);

  static_assert(c_sampleView.compare(c_sampleLength + 1, 1, StringView("")) == 0,
                "an offset past the end must compare an empty part");
  static_assert(c_sampleView.compare(0, c_npos, c_sampleView, c_sampleLength + 1, 1) > 0,
                "an offset past the end of the other view must compare against an empty part");
  static_assert(c_sampleView.compare(c_sampleLength + 1, 1, "a", 1) < 0,
                "an offset past the end must compare an empty part against a counted string");
}
#endif // !_DEBUG

// Whether two views hold the same characters, and what the synthesized inequality reports.
TEST_CASE("string_view/equality") {
  const StringView view(c_sample);

  CHECK(view == StringView(c_sample));
  CHECK_FALSE(view == StringView(c_samplePrefix));
  CHECK(view != StringView(c_sampleLonger));

  // Either side converts from a null-terminated string, so a literal compares against a view directly.
  CHECK(view == c_sample);
  CHECK(c_sample == view);
  CHECK(view != c_samplePrefix);

  // A view over no string and one over an empty string both hold no character.
  CHECK(StringView() == StringView(""));
  CHECK(StringView() == StringView());

  static_assert(c_sampleView == StringView(c_sample), "a string must compare equal to itself");
  static_assert(!(c_sampleView == StringView(c_samplePrefix)), "strings of different lengths must not compare equal");
  static_assert(c_sampleView != StringView(c_sampleLonger), "the synthesized inequality must negate the comparison");
  static_assert(c_sampleView == c_sample, "a null-terminated string converts on either side");
  static_assert(StringView() == StringView(""), "a view over no string equals a view over an empty one");
}

// Where a view sits in a lexicographic order, and what the synthesized relations report.
TEST_CASE("string_view/ordering") {
  const StringView view(c_sample);

  CHECK((view <=> StringView(c_sample)) == std::strong_ordering::equal);
  CHECK((view <=> StringView(c_samplePrefix)) == std::strong_ordering::greater);
  CHECK((view <=> StringView("playz")) == std::strong_ordering::less);

  // A string that starts another one orders before it, the tie settled on the lengths.
  CHECK(StringView(c_samplePrefix) < view);
  CHECK(view < StringView(c_sampleLonger));
  CHECK(view <= StringView(c_sample));
  CHECK(view >= StringView(c_sample));
  CHECK(view > StringView("plaa"));

  // Either side converts from a null-terminated string, the way equality accepts one.
  CHECK(view > c_samplePrefix);
  CHECK(c_samplePrefix < view);

  // Bytes order by their unsigned value, so a high byte sits after an ASCII one even where plain char is signed.
  CHECK(StringView(c_highByte) > StringView("a"));

  CHECK((StringView() <=> StringView("")) == std::strong_ordering::equal);

  static_assert((c_sampleView <=> StringView(c_sample)) == std::strong_ordering::equal,
                "a string must order equal to itself");
  static_assert((c_sampleView <=> StringView(c_samplePrefix)) == std::strong_ordering::greater,
                "a longer string sharing a prefix must order after the shorter one");
  static_assert(c_sampleView < StringView(c_sampleLonger), "the synthesized relations must follow the ordering");
  static_assert(c_sampleView > c_samplePrefix, "a null-terminated string converts on either side");
  static_assert(StringView(c_highByte) > StringView("a"), "bytes must order by their unsigned char value");
  static_assert((StringView() <=> StringView("")) == std::strong_ordering::equal,
                "a view over no string orders equal to a view over an empty one");
}

// Whether the viewed string opens with the characters the argument names.
TEST_CASE("string_view/starts_with") {
  const StringView view(c_sample);

  CHECK(view.starts_with(StringView(c_samplePrefix)));
  CHECK(view.starts_with(StringView(c_sample)));
  CHECK_FALSE(view.starts_with(StringView(c_sampleLonger)));
  CHECK(view.starts_with(StringView("")));

  CHECK(view.starts_with('p'));
  CHECK_FALSE(view.starts_with('l'));
  CHECK_FALSE(StringView("").starts_with('p'));

  CHECK(view.starts_with(c_samplePrefix));
  CHECK_FALSE(view.starts_with(c_sampleLonger));

  static_assert(c_sampleView.starts_with(StringView(c_samplePrefix)), "a string must start with its own leading part");
  static_assert(!c_sampleView.starts_with(StringView(c_sampleLonger)), "a string cannot start with a longer one");
  static_assert(c_sampleView.starts_with('p'), "the character overload reads the first byte");
  static_assert(c_sampleView.starts_with(c_samplePrefix), "the pointer overload measures its argument");
}

// Whether the viewed string closes with the characters the argument names.
TEST_CASE("string_view/ends_with") {
  const StringView view(c_sample);

  CHECK(view.ends_with(StringView("yer")));
  CHECK(view.ends_with(StringView(c_sample)));
  CHECK_FALSE(view.ends_with(StringView(c_samplePrefix)));
  CHECK(view.ends_with(StringView("")));

  CHECK(view.ends_with('r'));
  CHECK_FALSE(view.ends_with('e'));
  CHECK_FALSE(StringView("").ends_with('r'));

  CHECK(view.ends_with("yer"));
  CHECK_FALSE(view.ends_with(c_sampleLonger));

  static_assert(c_sampleView.ends_with(StringView("yer")), "a string must end with its own trailing part");
  static_assert(!c_sampleView.ends_with(StringView(c_sampleLonger)), "a string cannot end with a longer one");
  static_assert(c_sampleView.ends_with('r'), "the character overload reads the last byte");
  static_assert(c_sampleView.ends_with("yer"), "the pointer overload measures its argument");
}

// Which offset each overload reports; the search itself is covered by the toy::string_utils tests.
TEST_CASE("string_view/find") {
  const StringView view(c_repeated);

  CHECK(view.find(StringView("abra")) == 0);
  CHECK(view.find(StringView("abra"), 1) == 7);
  CHECK(view.find("abrasive", 1, 4) == 7);
  CHECK(view.find("cad") == 4);
  CHECK(view.find("cad", 5) == c_npos);
  CHECK(view.find('a') == 0);
  CHECK(view.find('a', 1) == 3);

  static_assert(c_repeatedView.find(StringView("abra"), 1) == 7, "the view overload passes its offset on");
  static_assert(c_repeatedView.find("abrasive", 1, 4) == 7, "the count bounds what is read");
  static_assert(c_repeatedView.find("cad") == 4, "the pointer overload measures its argument");
  static_assert(c_repeatedView.find('a', 1) == 3, "the character overload passes its offset on");
}

// Which offset each overload of the backward search reports.
TEST_CASE("string_view/rfind") {
  const StringView view(c_repeated);

  CHECK(view.rfind(StringView("abra")) == 7);
  CHECK(view.rfind(StringView("abra"), 6) == 0);
  CHECK(view.rfind("abrasive", c_npos, 4) == 7);
  CHECK(view.rfind("cad") == 4);
  CHECK(view.rfind("cad", 3) == c_npos);
  CHECK(view.rfind('a') == c_repeatedLength - 1);
  CHECK(view.rfind('a', 9) == 7);

  static_assert(c_repeatedView.rfind(StringView("abra"), 6) == 0, "the view overload passes its offset on");
  static_assert(c_repeatedView.rfind("abrasive", c_npos, 4) == 7, "the count bounds what is read");
  static_assert(c_repeatedView.rfind("cad") == 4, "the pointer overload measures its argument");
  static_assert(c_repeatedView.rfind('a', 9) == 7, "the character overload passes its offset on");
}

// Whether the characters the argument names appear anywhere in the viewed string.
TEST_CASE("string_view/contains") {
  const StringView view(c_repeated);

  CHECK(view.contains(StringView("cad")));
  CHECK_FALSE(view.contains(StringView("xyz")));
  CHECK(view.contains(StringView("")));

  CHECK(view.contains('d'));
  CHECK_FALSE(view.contains('z'));

  CHECK(view.contains("dabra"));
  CHECK_FALSE(view.contains("dabraz"));

  static_assert(c_repeatedView.contains(StringView("cad")), "a needle the string holds is contained");
  static_assert(!c_repeatedView.contains(StringView("xyz")), "a needle the string lacks is not contained");
  static_assert(c_repeatedView.contains('d'), "the character overload matches the same offsets");
  static_assert(c_repeatedView.contains("dabra"), "the pointer overload measures its argument");
}

// Which offset each overload reports for a set; a set of one character reads the same as a character search.
TEST_CASE("string_view/find_first_of") {
  const StringView view(c_repeated);

  CHECK(view.find_first_of(StringView("rc")) == 2);
  CHECK(view.find_first_of(StringView("rc"), 3) == 4);
  CHECK(view.find_first_of("rcxyz", 3, 2) == 4);
  CHECK(view.find_first_of("rc") == 2);
  CHECK(view.find_first_of("rc", 10) == c_npos);
  CHECK(view.find_first_of('d') == 6);
  CHECK(view.find_first_of('a', 1) == 3);

  static_assert(c_repeatedView.find_first_of(StringView("rc"), 3) == 4, "the view overload passes its offset on");
  static_assert(c_repeatedView.find_first_of("rcxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(c_repeatedView.find_first_of("rc") == 2, "the pointer overload measures its argument");
  static_assert(c_repeatedView.find_first_of('d') == 6, "a set of one is that character");
}

// Which offset each overload of the backward set search reports.
TEST_CASE("string_view/find_last_of") {
  const StringView view(c_repeated);

  CHECK(view.find_last_of(StringView("rc")) == 9);
  CHECK(view.find_last_of(StringView("rc"), 8) == 4);
  CHECK(view.find_last_of("rcxyz", 8, 2) == 4);
  CHECK(view.find_last_of("rc") == 9);
  CHECK(view.find_last_of("rc", 1) == c_npos);
  CHECK(view.find_last_of('a') == c_repeatedLength - 1);
  CHECK(view.find_last_of('a', 9) == 7);

  static_assert(c_repeatedView.find_last_of(StringView("rc"), 8) == 4, "the view overload passes its offset on");
  static_assert(c_repeatedView.find_last_of("rcxyz", 8, 2) == 4, "the count bounds what is read");
  static_assert(c_repeatedView.find_last_of("rc") == 9, "the pointer overload measures its argument");
  static_assert(c_repeatedView.find_last_of('a') == c_repeatedLength - 1, "a set of one is that character");
}

// Which offset each overload reports for the first character outside a set.
TEST_CASE("string_view/find_first_not_of") {
  const StringView view(c_repeated);

  CHECK(view.find_first_not_of(StringView("ab")) == 2);
  CHECK(view.find_first_not_of(StringView("ab"), 3) == 4);
  CHECK(view.find_first_not_of("abxyz", 3, 2) == 4);
  CHECK(view.find_first_not_of("ab") == 2);
  CHECK(view.find_first_not_of("ab", 10) == c_npos);
  CHECK(view.find_first_not_of('a') == 1);
  CHECK(view.find_first_not_of('a', 3) == 4);

  static_assert(c_repeatedView.find_first_not_of(StringView("ab"), 3) == 4, "the view overload passes its offset on");
  static_assert(c_repeatedView.find_first_not_of("abxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(c_repeatedView.find_first_not_of("ab") == 2, "the pointer overload measures its argument");
  static_assert(c_repeatedView.find_first_not_of('a', 3) == 4, "the character overload passes its offset on");
}

// Which offset each overload reports for the last character outside a set.
TEST_CASE("string_view/find_last_not_of") {
  const StringView view(c_repeated);

  CHECK(view.find_last_not_of(StringView("ab")) == 9);
  CHECK(view.find_last_not_of(StringView("ab"), 8) == 6);
  CHECK(view.find_last_not_of("abxyz", 8, 2) == 6);
  CHECK(view.find_last_not_of("ab") == 9);
  CHECK(view.find_last_not_of("ab", 1) == c_npos);
  CHECK(view.find_last_not_of('a') == 9);
  CHECK(view.find_last_not_of('a', 8) == 8);

  static_assert(c_repeatedView.find_last_not_of(StringView("ab"), 8) == 6, "the view overload passes its offset on");
  static_assert(c_repeatedView.find_last_not_of("abxyz", 8, 2) == 6, "the count bounds what is read");
  static_assert(c_repeatedView.find_last_not_of("ab") == 9, "the pointer overload measures its argument");
  static_assert(c_repeatedView.find_last_not_of('a', 8) == 8, "the character overload passes its offset on");
}

} // namespace toy
