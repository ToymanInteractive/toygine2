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
constexpr const char * sampleText      = "player";
constexpr size_t       sampleLength    = std::char_traits<char>::length(sampleText);
// The same characters back to front, the order a reverse walk must yield.
constexpr const char * sampleReverse   = "reyalp";
// A leading part of the sample, and a longer string starting with it.
constexpr const char * samplePrefix    = "play";
constexpr const char * sampleLonger    = "player one";
// Characters without a terminator, the buffer shape a counted comparison has to accept.
constexpr char         countedPart[4]  = {'p', 'l', 'a', 'y'};
// A terminator inside the counted range, which a measured length would stop at.
constexpr char         embeddedNull[5] = {'p', 'l', '\0', 'a', 'y'};

// The same characters followed by a terminator, as a StringLike source has to provide them.
constexpr char   terminatedEmbeddedNull[]     = "pl\0ay";
constexpr size_t terminatedEmbeddedNullLength = sizeof(terminatedEmbeddedNull) - 1;

// The least a type exposes to satisfy StringLike: a C-string pointer and the length it reports.
struct ReportedString {
  const char * characters;
  size_t       length;

  [[nodiscard]] constexpr const char * c_str() const noexcept {
    return characters;
  }

  [[nodiscard]] constexpr size_t size() const noexcept {
    return length;
  }
};

// A source whose reported length runs past a null character a scan would stop at.
constexpr ReportedString reportedSource{.characters = terminatedEmbeddedNull, .length = terminatedEmbeddedNullLength};

// A byte above the ASCII range, which orders after an ASCII one only where bytes compare as unsigned char.
constexpr const char * highByte = "\xff";

// A literal whose groups repeat, so a search has more than one candidate to pick between.
constexpr const char * repeated = "abracadabra";

// Views the compile-time assertions read, so a constant expression names one instead of rebuilding it each time.
constexpr StringView sampleView(sampleText);
constexpr StringView repeatedView(repeated);

// Offset every search reports when it matches nothing.
constexpr size_t npos           = StringView::npos;
constexpr size_t repeatedLength = std::char_traits<char>::length(repeated);

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
constexpr char sentinel = '#';

// Caller storage prefilled with the sentinel.
[[nodiscard]] constexpr std::array<char, 16> sentinelBuffer() noexcept {
  std::array<char, 16> buffer{};
  buffer.fill(sentinel);

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

  const StringView view(sampleText);
  CHECK(forwardLength(view) == sampleLength);
  CHECK(view.begin() == sampleText);

  const StringView copy(view);
  CHECK(copy.begin() == view.begin());
  CHECK(forwardLength(copy) == forwardLength(view));

  StringView assigned;
  assigned = view;
  CHECK(assigned.begin() == view.begin());
  CHECK(forwardLength(assigned) == sampleLength);

  static_assert(forwardLength(StringView()) == 0, "a default-constructed view spans no character");
  static_assert(forwardLength(StringView(sampleText)) == sampleLength,
                "the pointer constructor must measure the literal's byte count");
  static_assert(forwardLength(StringView(StringView(sampleText))) == sampleLength,
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

// A string-like source hands the view its pointer and its reported length, which may run past a null character.
TEST_CASE("string_view/construction_from_string_like") {
  const StringView view = reportedSource;

  CHECK(view.begin() == terminatedEmbeddedNull);
  CHECK(forwardLength(view) == terminatedEmbeddedNullLength);

  static_assert(StringView(reportedSource).begin() == terminatedEmbeddedNull,
                "the view must read the source's characters, not a copy");
  static_assert(forwardLength(StringView(reportedSource)) == terminatedEmbeddedNullLength,
                "the view must take the reported length instead of measuring to the first null character");
  static_assert(std::is_nothrow_convertible_v<ReportedString, StringView>,
                "a string-like argument must become a view without a cast");
}

// The range a forward walk covers and the characters it yields.
TEST_CASE("string_view/iteration") {
  const StringView view(sampleText);

  CHECK(forwardLength(view) == sampleLength);
  CHECK(view.cbegin() == view.begin());
  CHECK(view.cend() == view.end());

  for (size_t index = 0; index < sampleLength; ++index) {
    INFO("index ", index);
    CHECK(view.begin()[index] == sampleText[index]);
  }

  // The terminator sits one past the last character, unlike std::string_view.
  CHECK(*view.end() == '\0');

  const StringView empty;
  CHECK(empty.begin() == empty.end());
  CHECK(empty.cbegin() == empty.cend());

  static_assert(forwardLength(StringView(sampleText)) == sampleLength,
                "the forward range must span the literal's byte count");
  static_assert(*sampleView.begin() == 'p', "the first character must be the literal's first byte");
  static_assert(*sampleView.end() == '\0', "the position past the last character holds the terminator");
  static_assert(StringView().begin() == StringView().end(), "an empty view must yield an empty range");
}

// The range a back-to-front walk covers and the order it yields the characters in.
TEST_CASE("string_view/iteration_reverse") {
  const StringView view(sampleText);

  CHECK(reverseLength(view) == sampleLength);
  CHECK(view.crbegin() == view.rbegin());
  CHECK(view.crend() == view.rend());

  for (size_t index = 0; index < sampleLength; ++index) {
    INFO("index ", index);
    CHECK(view.rbegin()[index] == sampleReverse[index]);
  }

  const StringView empty;
  CHECK(empty.rbegin() == empty.rend());
  CHECK(empty.crbegin() == empty.crend());

  static_assert(reverseLength(StringView(sampleText)) == sampleLength,
                "the reverse range must span the literal's byte count");
  static_assert(*sampleView.rbegin() == 'r', "a reverse walk must start at the last character");
  static_assert(sampleView.rend().base() == sampleView.begin(), "the reverse end must adapt the forward beginning");
  static_assert(StringView().rbegin() == StringView().rend(), "an empty view must yield an empty reverse range");
}

// The character each indexed access path yields, and where in the viewed string it reads it.
TEST_CASE("string_view/element_access") {
  const StringView view(sampleText);

  for (size_t index = 0; index < sampleLength; ++index) {
    INFO("index ", index);
    CHECK(view[index] == sampleText[index]);
    CHECK(view.at(index) == sampleText[index]);
  }

  CHECK(view.front() == sampleText[0]);
  CHECK(view.back() == sampleText[sampleLength - 1]);

  // Each access hands back a reference into the viewed string, not a copy of the character.
  CHECK(&view.front() == view.data());
  CHECK(&view.back() == view.data() + sampleLength - 1);

  static_assert(StringView(sampleText)[0] == 'p', "an indexed read must yield the literal's byte at that offset");
  static_assert(sampleView.at(sampleLength - 1) == 'r', "at() must read the same byte as operator[]");
  static_assert(sampleView.front() == 'p', "the front character must be the literal's first byte");
  static_assert(sampleView.back() == 'r', "the back character must be the literal's last byte");
}

// The pointer the view hands to a C interface, and the terminator that pointer reaches.
TEST_CASE("string_view/pointer_access") {
  const StringView view(sampleText);

  CHECK(view.data() == sampleText);
  CHECK(view.c_str() == view.data());

  // Null-termination is the contract std::string_view does not carry.
  CHECK(view.c_str()[view.size()] == '\0');

  const StringView empty;
  CHECK(empty.data() == nullptr);
  CHECK(empty.c_str() == nullptr);

  static_assert(sampleView.data() == sampleText, "the view must hand back the pointer it was built from");
  static_assert(sampleView.c_str()[sampleLength] == '\0', "the byte past the last character must be the terminator");
  static_assert(StringView().data() == nullptr, "a default-constructed view names no string");
}

// What the view reports about its extent, and the bound a length may not cross.
TEST_CASE("string_view/capacity") {
  const StringView view(sampleText);

  CHECK(view.size() == sampleLength);
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

  static_assert(sampleView.size() == sampleLength, "size() must report the literal's UTF-8 byte count");
  static_assert(sampleView.length() == sampleView.size(), "length() and size() must report the same count");
  static_assert(StringView().empty(), "a default-constructed view holds no character");
  static_assert(!sampleView.empty(), "a view over a non-empty literal holds characters");
  static_assert(npos == static_cast<StringView::size_type>(-1),
                "npos must be the largest value the length type carries");
  static_assert(npos == string_utils::npos, "npos must match the value the shared search functions return");
}

// How many characters the viewed string holds, against the bytes it measures.
TEST_CASE("string_view/utf8_size") {
  const StringView view(sampleText);

  // An ASCII string stores one byte per character, so both counts agree.
  CHECK(view.utf8_size() == view.size());

  // Cyrillic stores two bytes per character, so the character count is half the byte count.
  const StringView cyrillic("привет");
  CHECK(cyrillic.utf8_size() == cyrillic.size() / 2);

  CHECK(StringView("").utf8_size() == 0);

  // A view over no string counts nothing, the way it reports no length.
  CHECK(StringView().utf8_size() == 0);

  // A view that dropped a whole leading character counts only what it still holds.
  StringView tail("привет");
  tail.remove_prefix(2);
  CHECK(tail.utf8_size() == 5);

  static_assert(sampleView.utf8_size() == sampleView.size(), "an ASCII view counts one character per byte");
  static_assert(StringView("привет").utf8_size() == 6, "a two-byte character counts once");
  static_assert(StringView().utf8_size() == 0, "a view over no string counts nothing");
}

// Where the view starts reading after it drops leading characters.
TEST_CASE("string_view/remove_prefix") {
  StringView view(sampleText);

  view.remove_prefix(4);
  CHECK(view.size() == sampleLength - 4);
  CHECK(view.data() == sampleText + 4);
  CHECK(view.front() == 'e');

  // Dropping the whole length leaves a view over the terminator alone.
  view.remove_prefix(view.size());
  CHECK(view.empty());
  CHECK(view.data() == sampleText + sampleLength);

  static_assert(withoutPrefix(sampleText, 0).size() == sampleLength, "dropping nothing must leave the view as it was");
  static_assert(withoutPrefix(sampleText, 4).size() == sampleLength - 4,
                "the length must shrink by the number of characters dropped");
  static_assert(withoutPrefix(sampleText, 4).data() == sampleText + 4,
                "the view must start reading past the characters it dropped");
  static_assert(withoutPrefix(sampleText, sampleLength).empty(), "dropping the whole length must leave an empty view");
}

// The debug checks compile out without TOYGINE_DEBUG, so the cases below reach the documented fallbacks.
#if !defined(TOYGINE_DEBUG)
// A count past the length drops every character and stops at the terminator.
TEST_CASE("string_view/remove_prefix_past_end") {
  StringView view(sampleText);
  view.remove_prefix(sampleLength + 1);
  CHECK(view.empty());
  CHECK(view.data() == sampleText + sampleLength);

  // A default view has no characters to drop, so its pointer stays null.
  StringView defaulted;
  defaulted.remove_prefix(1);
  CHECK(defaulted.empty());
  CHECK(defaulted.data() == nullptr);

  constexpr auto defaultedDropped = [] {
    StringView empty;
    empty.remove_prefix(1);

    return empty;
  };

  static_assert(withoutPrefix(sampleText, sampleLength + 1).empty(), "an oversized count must drop every character");
  static_assert(withoutPrefix(sampleText, sampleLength + 1).data() == sampleText + sampleLength,
                "an oversized count must leave the view over the terminator");
  static_assert(defaultedDropped().data() == nullptr, "a default view must keep its null pointer");
}

// A read from an offset past the end sees an empty range.
TEST_CASE("string_view/read_past_end") {
  const StringView view(sampleText);

  std::array<char, 16> buffer = sentinelBuffer();
  CHECK(view.copy(buffer.data(), 4, sampleLength + 1) == 0);
  CHECK(buffer[0] == sentinel);
  CHECK(view.compare(sampleLength + 1, 1, StringView{}) == 0);
  CHECK(view.compare(sampleLength + 1, 1, StringView(sampleText)) < 0);
  CHECK(view.compare(0, sampleLength, StringView(sampleText), sampleLength + 1, 1) > 0);
  CHECK(view.compare(sampleLength + 1, 1, sampleText) < 0);
  CHECK(view.compare(sampleLength + 1, 1, sampleText, sampleLength) < 0);

  static_assert(copiedCount(sampleText, 4, sampleLength + 1) == 0, "a copy from past the end must write nothing");
  static_assert(sampleView.compare(sampleLength + 1, 1, StringView{}) == 0,
                "a part past the end must compare equal to an empty view");
  static_assert(sampleView.compare(sampleLength + 1, 1, StringView(sampleText)) < 0,
                "a part past the end must order before a non-empty view");
  static_assert(sampleView.compare(0, sampleLength, StringView(sampleText), sampleLength + 1, 1) > 0,
                "an argument part past its end must compare as empty");
  static_assert(sampleView.compare(sampleLength + 1, 1, sampleText) < 0,
                "a part past the end must order before a non-empty pointer");
  static_assert(sampleView.compare(sampleLength + 1, 1, sampleText, sampleLength) < 0,
                "a part past the end must order before a non-empty counted pointer");
}
#endif // !TOYGINE_DEBUG

// Which string each of the two views reads after they exchange their contents.
TEST_CASE("string_view/swap") {
  StringView left(sampleText);
  StringView right(samplePrefix);

  left.swap(right);
  CHECK(left.data() == samplePrefix);
  CHECK(left.size() == std::char_traits<char>::length(samplePrefix));
  CHECK(right.data() == sampleText);
  CHECK(right.size() == sampleLength);

  left.swap(right);
  CHECK(left.data() == sampleText);
  CHECK(right.data() == samplePrefix);

  static_assert(swapped(sampleText, samplePrefix).data() == samplePrefix,
                "the left view must read the string the right one named");
  static_assert(swapped(sampleText, samplePrefix).size() == std::char_traits<char>::length(samplePrefix),
                "the length must travel with the pointer");
}

// How many characters reach the caller's buffer, and which ones.
TEST_CASE("string_view/copy") {
  const StringView view(sampleText);

  std::array<char, 16> buffer = sentinelBuffer();
  CHECK(view.copy(buffer.data(), 4) == 4);
  CHECK(std::char_traits<char>::compare(buffer.data(), samplePrefix, 4) == 0);

  // The call writes characters only; the sentinel past the count survives, so no terminator was added.
  CHECK(buffer[4] == sentinel);

  // A count past the end copies what remains rather than reading past the terminator.
  std::array<char, 16> tail = sentinelBuffer();
  CHECK(view.copy(tail.data(), 32, 2) == sampleLength - 2);
  CHECK(std::char_traits<char>::compare(tail.data(), "ayer", 4) == 0);
  CHECK(tail[sampleLength - 2] == sentinel);

  // Starting at the end names an empty range rather than a range out of bounds.
  std::array<char, 16> none = sentinelBuffer();
  CHECK(view.copy(none.data(), 4, sampleLength) == 0);
  CHECK(none[0] == sentinel);

  // A view holding no character can be copied from, and the call writes nothing.
  CHECK(StringView("").copy(none.data(), 4) == 0);
  CHECK(StringView().copy(none.data(), 4) == 0);
  CHECK(none[0] == sentinel);

  // copy() takes a null destination when nothing is left to copy.
  CHECK(view.copy(nullptr, 0) == 0);

  static_assert(copiedCount(sampleText, 4, 0) == 4, "a copy writes what the count names");
  static_assert(copiedCount(sampleText, 32, 2) == sampleLength - 2, "a copy stops at the end of the string");
  static_assert(copiedCount(sampleText, 4, sampleLength) == 0, "a copy starting at the end writes nothing");
  static_assert(copiedAt(sampleText, 4, 0) == 'p', "a copy writes the characters the view holds");
  static_assert(copiedAt(sampleText, 4, 4) == sentinel, "a copy writes nothing past its count");
  static_assert(sampleView.copy(nullptr, 0) == 0, "a null destination with a count of zero must copy nothing");
}

// The sign each comparison of two whole strings yields.
TEST_CASE("string_view/compare") {
  const StringView view(sampleText);

  CHECK(view.compare(StringView(sampleText)) == 0);
  CHECK(view.compare(StringView(samplePrefix)) > 0);
  CHECK(view.compare(StringView(sampleLonger)) < 0);
  CHECK(view.compare(StringView("playz")) < 0);

  // The pointer overload measures its argument and compares the same way.
  CHECK(view.compare(sampleText) == 0);
  CHECK(view.compare(samplePrefix) > 0);
  CHECK(view.compare(sampleLonger) < 0);

  static_assert(sampleView.compare(StringView(sampleText)) == 0, "a string must compare equal to itself");
  static_assert(sampleView.compare(StringView(samplePrefix)) > 0,
                "a longer string sharing a prefix must order after the shorter one");
  static_assert(sampleView.compare(sampleLonger) < 0, "the pointer overload must order by the first differing byte");
}

// The sign a comparison yields when it reads a part of one or both strings.
TEST_CASE("string_view/compare_substring") {
  const StringView view(sampleText);

  CHECK(view.compare(0, 4, StringView(samplePrefix)) == 0);
  CHECK(view.compare(2, 4, StringView("ayer")) == 0);
  CHECK(view.compare(0, 4, StringView(sampleText)) < 0);

  CHECK(view.compare(0, 4, StringView(sampleLonger), 0, 4) == 0);
  CHECK(view.compare(0, sampleLength, StringView(sampleLonger), 0, 4) > 0);

  // Equal-length parts that differ order on the first differing character, with no length left to decide.
  CHECK(view.compare(0, 4, StringView("plaz"), 0, 4) < 0);
  CHECK(view.compare(0, 4, StringView("plaa"), 0, 4) > 0);

  CHECK(view.compare(0, 4, samplePrefix) == 0);
  CHECK(view.compare(0, 4, sampleLonger, 4) == 0);

  // The counted overload reads exactly the count, so the argument needs no terminator and may hold one inside.
  CHECK(view.compare(0, 4, countedPart, 4) == 0);
  CHECK(view.compare(0, 2, countedPart, 2) == 0);
  CHECK(view.compare(0, 4, embeddedNull, 4) > 0);
  CHECK(StringView("pl").compare(0, 2, embeddedNull, 4) < 0);

  static_assert(sampleView.compare(0, 4, StringView(samplePrefix)) == 0,
                "a part must compare equal to the string it repeats");
  static_assert(sampleView.compare(0, 4, StringView(sampleText)) < 0,
                "a shorter part must order before the whole string it starts");
  static_assert(sampleView.compare(0, 4, sampleLonger, 4) == 0, "the count bounds what is read");
  static_assert(sampleView.compare(0, 4, StringView("plaz"), 0, 4) < 0,
                "equal-length parts must order on the first differing character");
  static_assert(sampleView.compare(0, 4, countedPart, 4) == 0,
                "a counted argument needs no terminator to compare against");
  static_assert(sampleView.compare(0, 4, embeddedNull, 4) > 0,
                "a terminator inside the count is an ordinary character");
  static_assert(StringView("pl").compare(0, 2, embeddedNull, 4) < 0, "the count sets the length, not a terminator");
}

// A count past the end of the part names the rest of it, the way copy() reads its own count.
TEST_CASE("string_view/compare_substring_clamped_count") {
  const StringView view(sampleText);

  CHECK(view.compare(0, npos, StringView(sampleText)) == 0);
  CHECK(view.compare(2, npos, StringView("ayer")) == 0);
  CHECK(view.compare(sampleLength, npos, StringView("")) == 0);

  CHECK(view.compare(0, npos, StringView(sampleLonger), 0, npos) < 0);
  CHECK(view.compare(2, npos, StringView("xayer"), 1, npos) == 0);

  CHECK(view.compare(0, npos, sampleText) == 0);
  CHECK(view.compare(2, npos, "ayer", 4) == 0);

  static_assert(sampleView.compare(0, npos, StringView(sampleText)) == 0,
                "a count past the end must name the rest of the string");
  static_assert(sampleView.compare(2, npos, StringView("ayer")) == 0,
                "a clamped count must settle the length tie-break on what was read");
  static_assert(sampleView.compare(0, npos, StringView(sampleLonger), 0, npos) < 0,
                "both counts must clamp to the parts they name");
}

// The sign a comparison yields against a view that holds no string, which reads no character to order on.
TEST_CASE("string_view/compare_empty") {
  const StringView empty;
  const StringView view(sampleText);

  CHECK(empty.compare(empty) == 0);
  CHECK(empty.compare(view) < 0);
  CHECK(view.compare(empty) > 0);
  CHECK(empty.compare(0, npos, empty) == 0);
  CHECK(empty.compare(0, npos, empty, 0, npos) == 0);

  // A null pointer with a count of zero names an empty part, as a counted std::string_view comparison accepts.
  CHECK(view.compare(0, 0, nullptr, 0) == 0);
  CHECK(view.compare(0, npos, nullptr, 0) > 0);

  static_assert(StringView().compare(StringView()) == 0, "two views holding no string must compare equal");
  static_assert(StringView().compare(sampleView) < 0, "a view holding no string must order before any string");
  static_assert(sampleView.compare(0, 0, nullptr, 0) == 0, "a null pointer with a count of zero must be empty");
}

// Whether two views hold the same characters, and what the synthesized inequality reports.
TEST_CASE("string_view/equality") {
  const StringView view(sampleText);

  CHECK(view == StringView(sampleText));
  CHECK_FALSE(view == StringView(samplePrefix));
  CHECK(view != StringView(sampleLonger));

  // Either side converts from a null-terminated string, so a literal compares against a view directly.
  CHECK(view == sampleText);
  CHECK(sampleText == view);
  CHECK(view != samplePrefix);

  // A view over no string and one over an empty string both hold no character.
  CHECK(StringView() == StringView(""));
  CHECK(StringView() == StringView());

  static_assert(sampleView == StringView(sampleText), "a string must compare equal to itself");
  static_assert(!(sampleView == StringView(samplePrefix)), "strings of different lengths must not compare equal");
  static_assert(sampleView != StringView(sampleLonger), "the synthesized inequality must negate the comparison");
  static_assert(sampleView == sampleText, "a null-terminated string converts on either side");
  static_assert(StringView() == StringView(""), "a view over no string equals a view over an empty one");
}

// Where a view sits in a lexicographic order, and what the synthesized relations report.
TEST_CASE("string_view/ordering") {
  const StringView view(sampleText);

  CHECK((view <=> StringView(sampleText)) == std::strong_ordering::equal);
  CHECK((view <=> StringView(samplePrefix)) == std::strong_ordering::greater);
  CHECK((view <=> StringView("playz")) == std::strong_ordering::less);

  // A string that starts another one orders before it, the tie settled on the lengths.
  CHECK(StringView(samplePrefix) < view);
  CHECK(view < StringView(sampleLonger));
  CHECK(view <= StringView(sampleText));
  CHECK(view >= StringView(sampleText));
  CHECK(view > StringView("plaa"));

  // Either side converts from a null-terminated string, the way equality accepts one.
  CHECK(view > samplePrefix);
  CHECK(samplePrefix < view);

  // Bytes order by their unsigned value, so a high byte sits after an ASCII one even where plain char is signed.
  CHECK(StringView(highByte) > StringView("a"));

  CHECK((StringView() <=> StringView("")) == std::strong_ordering::equal);

  static_assert((sampleView <=> StringView(sampleText)) == std::strong_ordering::equal,
                "a string must order equal to itself");
  static_assert((sampleView <=> StringView(samplePrefix)) == std::strong_ordering::greater,
                "a longer string sharing a prefix must order after the shorter one");
  static_assert(sampleView < StringView(sampleLonger), "the synthesized relations must follow the ordering");
  static_assert(sampleView > samplePrefix, "a null-terminated string converts on either side");
  static_assert(StringView(highByte) > StringView("a"), "bytes must order by their unsigned char value");
  static_assert((StringView() <=> StringView("")) == std::strong_ordering::equal,
                "a view over no string orders equal to a view over an empty one");
}

// Whether the viewed string opens with the characters the argument names.
TEST_CASE("string_view/starts_with") {
  const StringView view(sampleText);

  CHECK(view.starts_with(StringView(samplePrefix)));
  CHECK(view.starts_with(StringView(sampleText)));
  CHECK_FALSE(view.starts_with(StringView(sampleLonger)));
  CHECK(view.starts_with(StringView("")));

  CHECK(view.starts_with('p'));
  CHECK_FALSE(view.starts_with('l'));
  CHECK_FALSE(StringView("").starts_with('p'));

  CHECK(view.starts_with(samplePrefix));
  CHECK_FALSE(view.starts_with(sampleLonger));

  static_assert(sampleView.starts_with(StringView(samplePrefix)), "a string must start with its own leading part");
  static_assert(!sampleView.starts_with(StringView(sampleLonger)), "a string cannot start with a longer one");
  static_assert(sampleView.starts_with('p'), "the character overload reads the first byte");
  static_assert(sampleView.starts_with(samplePrefix), "the pointer overload measures its argument");
}

// Whether the viewed string closes with the characters the argument names.
TEST_CASE("string_view/ends_with") {
  const StringView view(sampleText);

  CHECK(view.ends_with(StringView("yer")));
  CHECK(view.ends_with(StringView(sampleText)));
  CHECK_FALSE(view.ends_with(StringView(samplePrefix)));
  CHECK(view.ends_with(StringView("")));

  CHECK(view.ends_with('r'));
  CHECK_FALSE(view.ends_with('e'));
  CHECK_FALSE(StringView("").ends_with('r'));

  CHECK(view.ends_with("yer"));
  CHECK_FALSE(view.ends_with(sampleLonger));

  static_assert(sampleView.ends_with(StringView("yer")), "a string must end with its own trailing part");
  static_assert(!sampleView.ends_with(StringView(sampleLonger)), "a string cannot end with a longer one");
  static_assert(sampleView.ends_with('r'), "the character overload reads the last byte");
  static_assert(sampleView.ends_with("yer"), "the pointer overload measures its argument");
}

// Which offset each overload reports; the search itself is covered by the toy::string_utils tests.
TEST_CASE("string_view/find") {
  const StringView view(repeated);

  CHECK(view.find(StringView("abra")) == 0);
  CHECK(view.find(StringView("abra"), 1) == 7);
  CHECK(view.find("abrasive", 1, 4) == 7);
  CHECK(view.find("cad") == 4);
  CHECK(view.find("cad", 5) == npos);
  CHECK(view.find('a') == 0);
  CHECK(view.find('a', 1) == 3);

  static_assert(repeatedView.find(StringView("abra"), 1) == 7, "the view overload passes its offset on");
  static_assert(repeatedView.find("abrasive", 1, 4) == 7, "the count bounds what is read");
  static_assert(repeatedView.find("cad") == 4, "the pointer overload measures its argument");
  static_assert(repeatedView.find('a', 1) == 3, "the character overload passes its offset on");
}

// Which offset each overload of the backward search reports.
TEST_CASE("string_view/rfind") {
  const StringView view(repeated);

  CHECK(view.rfind(StringView("abra")) == 7);
  CHECK(view.rfind(StringView("abra"), 6) == 0);
  CHECK(view.rfind("abrasive", npos, 4) == 7);
  CHECK(view.rfind("cad") == 4);
  CHECK(view.rfind("cad", 3) == npos);
  CHECK(view.rfind('a') == repeatedLength - 1);
  CHECK(view.rfind('a', 9) == 7);

  static_assert(repeatedView.rfind(StringView("abra"), 6) == 0, "the view overload passes its offset on");
  static_assert(repeatedView.rfind("abrasive", npos, 4) == 7, "the count bounds what is read");
  static_assert(repeatedView.rfind("cad") == 4, "the pointer overload measures its argument");
  static_assert(repeatedView.rfind('a', 9) == 7, "the character overload passes its offset on");
}

// Whether the characters the argument names appear anywhere in the viewed string.
TEST_CASE("string_view/contains") {
  const StringView view(repeated);

  CHECK(view.contains(StringView("cad")));
  CHECK_FALSE(view.contains(StringView("xyz")));
  CHECK(view.contains(StringView("")));

  CHECK(view.contains('d'));
  CHECK_FALSE(view.contains('z'));

  CHECK(view.contains("dabra"));
  CHECK_FALSE(view.contains("dabraz"));

  static_assert(repeatedView.contains(StringView("cad")), "a needle the string holds is contained");
  static_assert(!repeatedView.contains(StringView("xyz")), "a needle the string lacks is not contained");
  static_assert(repeatedView.contains('d'), "the character overload matches the same offsets");
  static_assert(repeatedView.contains("dabra"), "the pointer overload measures its argument");
}

// Which offset each overload reports for a set; a set of one character reads the same as a character search.
TEST_CASE("string_view/find_first_of") {
  const StringView view(repeated);

  CHECK(view.find_first_of(StringView("rc")) == 2);
  CHECK(view.find_first_of(StringView("rc"), 3) == 4);
  CHECK(view.find_first_of("rcxyz", 3, 2) == 4);
  CHECK(view.find_first_of("rc") == 2);
  CHECK(view.find_first_of("rc", 10) == npos);
  CHECK(view.find_first_of('d') == 6);
  CHECK(view.find_first_of('a', 1) == 3);

  static_assert(repeatedView.find_first_of(StringView("rc"), 3) == 4, "the view overload passes its offset on");
  static_assert(repeatedView.find_first_of("rcxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(repeatedView.find_first_of("rc") == 2, "the pointer overload measures its argument");
  static_assert(repeatedView.find_first_of('d') == 6, "a set of one is that character");
}

// Which offset each overload of the backward set search reports.
TEST_CASE("string_view/find_last_of") {
  const StringView view(repeated);

  CHECK(view.find_last_of(StringView("rc")) == 9);
  CHECK(view.find_last_of(StringView("rc"), 8) == 4);
  CHECK(view.find_last_of("rcxyz", 8, 2) == 4);
  CHECK(view.find_last_of("rc") == 9);
  CHECK(view.find_last_of("rc", 1) == npos);
  CHECK(view.find_last_of('a') == repeatedLength - 1);
  CHECK(view.find_last_of('a', 9) == 7);

  static_assert(repeatedView.find_last_of(StringView("rc"), 8) == 4, "the view overload passes its offset on");
  static_assert(repeatedView.find_last_of("rcxyz", 8, 2) == 4, "the count bounds what is read");
  static_assert(repeatedView.find_last_of("rc") == 9, "the pointer overload measures its argument");
  static_assert(repeatedView.find_last_of('a') == repeatedLength - 1, "a set of one is that character");
}

// Which offset each overload reports for the first character outside a set.
TEST_CASE("string_view/find_first_not_of") {
  const StringView view(repeated);

  CHECK(view.find_first_not_of(StringView("ab")) == 2);
  CHECK(view.find_first_not_of(StringView("ab"), 3) == 4);
  CHECK(view.find_first_not_of("abxyz", 3, 2) == 4);
  CHECK(view.find_first_not_of("ab") == 2);
  CHECK(view.find_first_not_of("ab", 10) == npos);
  CHECK(view.find_first_not_of('a') == 1);
  CHECK(view.find_first_not_of('a', 3) == 4);

  static_assert(repeatedView.find_first_not_of(StringView("ab"), 3) == 4, "the view overload passes its offset on");
  static_assert(repeatedView.find_first_not_of("abxyz", 3, 2) == 4, "the count bounds what is read");
  static_assert(repeatedView.find_first_not_of("ab") == 2, "the pointer overload measures its argument");
  static_assert(repeatedView.find_first_not_of('a', 3) == 4, "the character overload passes its offset on");
}

// Which offset each overload reports for the last character outside a set.
TEST_CASE("string_view/find_last_not_of") {
  const StringView view(repeated);

  CHECK(view.find_last_not_of(StringView("ab")) == 9);
  CHECK(view.find_last_not_of(StringView("ab"), 8) == 6);
  CHECK(view.find_last_not_of("abxyz", 8, 2) == 6);
  CHECK(view.find_last_not_of("ab") == 9);
  CHECK(view.find_last_not_of("ab", 1) == npos);
  CHECK(view.find_last_not_of('a') == 9);
  CHECK(view.find_last_not_of('a', 8) == 8);

  static_assert(repeatedView.find_last_not_of(StringView("ab"), 8) == 6, "the view overload passes its offset on");
  static_assert(repeatedView.find_last_not_of("abxyz", 8, 2) == 6, "the count bounds what is read");
  static_assert(repeatedView.find_last_not_of("ab") == 9, "the pointer overload measures its argument");
  static_assert(repeatedView.find_last_not_of('a', 8) == 8, "the character overload passes its offset on");
}

} // namespace toy
