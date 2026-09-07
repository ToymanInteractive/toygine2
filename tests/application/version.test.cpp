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
  \file   version.test.cpp
  \brief  Unit tests for \ref toy::application::Version.
*/

#include "application.hpp"
#include "toy_test.hpp"

namespace toy::application {

namespace {

// Two revisions the cases order against each other. The first byte settles it: '1' sits below '9' in the byte order,
// so c_earlierRevision orders before c_revision without either string being a prefix of the other.
constexpr const char * c_earlierRevision = "1f3c1ab";
constexpr const char * c_revision        = "9f3c1ab";

// The build every case compares against.
constexpr Version c_build{1, 4, 2, c_revision};

} // namespace

// What a copy and a scope exit cost the caller, the initializer form the call site may write, and what a
// declaration without one leaves behind.
TEST_CASE("application/version/value_semantics") {
  static_assert(std::is_trivially_copyable_v<Version>, "the version must copy as bytes");
  static_assert(std::is_trivially_destructible_v<Version>, "leaving scope must run no destructor");
  static_assert(std::is_standard_layout_v<Version>, "the layout must stay readable from a C interface");

  static_assert(std::is_aggregate_v<Version>, "designated initializers require the type to stay an aggregate");
  static_assert(!std::is_trivially_default_constructible_v<Version>,
                "a default-constructed version zeroes its numbers and empties its revision, which is not a trivial "
                "default");
}

// What an omitted field leaves behind, read back through the members.
TEST_CASE("application/version/defaults") {
  const Version omitted{};

  CHECK(omitted.major == 0);
  CHECK(omitted.minor == 0);
  CHECK(omitted.patch == 0);
  CHECK(omitted.revision.empty());

  // A designated initializer names one field and leaves the rest at their defaults.
  const Version partial{.minor = 4};

  CHECK(partial.major == 0);
  CHECK(partial.minor == 4);
  CHECK(partial.patch == 0);
  CHECK(partial.revision.empty());

  static_assert(Version{}.major == 0, "an omitted major number must read zero");
  static_assert(Version{}.minor == 0, "an omitted minor number must read zero");
  static_assert(Version{}.patch == 0, "an omitted patch number must read zero");
  static_assert(Version{}.revision.empty(), "an omitted revision must hold no character");
  static_assert(Version{.minor = 4}.patch == 0, "a designated initializer must leave the unnamed fields defaulted");
}

// The type-level contract the comparison operators rest on.
TEST_CASE("application/version/traits") {
  static_assert(std::is_same_v<decltype(c_build <=> c_build), std::strong_ordering>,
                "the ordering must yield a strong ordering, not a weak or partial one");
  static_assert(std::is_same_v<decltype(c_build == c_build), bool>, "equality must yield a plain bool");

  static_assert(noexcept(c_build == c_build), "equality must be declared non-throwing");
  static_assert(noexcept(c_build <=> c_build), "the ordering must be declared non-throwing");
}

// Which fields equality reads, and what the synthesized inequality reports.
TEST_CASE("application/version/equality") {
  constexpr Version same{1, 4, 2, c_revision};
  constexpr Version higherMajor{2, 4, 2, c_revision};
  constexpr Version higherMinor{1, 5, 2, c_revision};
  constexpr Version higherPatch{1, 4, 3, c_revision};
  constexpr Version otherRevision{1, 4, 2, c_earlierRevision};
  constexpr Version unnamed{1, 4, 2};
  constexpr Version alsoUnnamed{1, 4, 2};

  CHECK(c_build == same);
  CHECK(c_build != higherMajor);
  CHECK(c_build != higherMinor);
  CHECK(c_build != higherPatch);

  // The revision counts like any other field, compared by its bytes.
  CHECK(c_build != otherRevision);
  CHECK(c_build != unnamed);

  // A version naming no revision equals only another one that names none.
  CHECK(unnamed == alsoUnnamed);

  static_assert(c_build == same, "a version must compare equal to itself");
  static_assert(c_build != higherPatch, "the synthesized inequality must negate the comparison");
  static_assert(c_build != otherRevision, "a differing revision must break equality");
  static_assert(c_build != unnamed, "a named revision must not equal an omitted one");
  static_assert(unnamed == alsoUnnamed, "two versions naming no revision must compare equal");
}

// Which number settles the order, and what the synthesized relations report.
TEST_CASE("application/version/ordering_numbers") {
  // A raised major number against every lower field raised instead.
  constexpr Version majorRaised{2, 0, 0};
  constexpr Version lowerFieldsRaised{1, 9, 9};
  // Equal major numbers, the minor raised on one side and the patch on the other.
  constexpr Version minorRaised{1, 2, 0};
  constexpr Version patchRaised{1, 1, 9};
  // Equal major and minor numbers, differing only in the patch.
  constexpr Version release{1, 0, 0};
  constexpr Version releasePatched{1, 0, 1};

  // The major number decides before either of the lower ones is read.
  CHECK((majorRaised <=> lowerFieldsRaised) == std::strong_ordering::greater);
  // The minor number decides once the major numbers match.
  CHECK((minorRaised <=> patchRaised) == std::strong_ordering::greater);
  // The patch number decides last among the numbers.
  CHECK((release <=> releasePatched) == std::strong_ordering::less);
  CHECK((c_build <=> c_build) == std::strong_ordering::equal);

  CHECK(release < releasePatched);
  CHECK(releasePatched > release);
  CHECK(release <= release);
  CHECK(release >= release);

  static_assert((majorRaised <=> lowerFieldsRaised) == std::strong_ordering::greater,
                "the major number must outrank both lower numbers");
  static_assert((minorRaised <=> patchRaised) == std::strong_ordering::greater,
                "the minor number must outrank the patch number");
  static_assert((release <=> releasePatched) == std::strong_ordering::less,
                "the patch number must settle an order the higher numbers leave open");
  static_assert(release < releasePatched, "the synthesized relations must follow the ordering");
}

// Where the revision enters the order, and where a version naming none lands.
TEST_CASE("application/version/ordering_revision") {
  constexpr Version earlier{1, 4, 2, c_earlierRevision};
  constexpr Version unnamed{1, 4, 2};
  constexpr Version majorRaised{2, 0, 0, c_earlierRevision};

  // Three equal numbers hand the decision to the revision, ordered byte by byte.
  CHECK((c_build <=> earlier) == std::strong_ordering::greater);
  CHECK((earlier <=> c_build) == std::strong_ordering::less);

  // An empty revision orders before every other, the reverse of semantic-versioning pre-release precedence.
  CHECK(unnamed < c_build);

  // A differing number settles the order before the revision is read.
  CHECK(majorRaised > c_build);

  static_assert((c_build <=> earlier) == std::strong_ordering::greater,
                "equal numbers must hand the order to the revision");
  static_assert(unnamed < c_build, "an omitted revision must order before a named one");
  static_assert(majorRaised > c_build, "a differing number must settle the order alone");
}

} // namespace toy::application
