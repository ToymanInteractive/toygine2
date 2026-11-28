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
  \file   format_args.test.cpp
  \brief  Unit tests for \ref toy::detail::FormatArg and the toy::detail::packArg() packers.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// A type no packer serves; the negative probes must reject it at the concept.
struct Unpackable {
  int payload;
};

// A type that converts to bool without being one.
struct Truthy {
  constexpr operator bool() const noexcept {
    return true;
  }
};

// An enumeration whose underlying type packs, so packing it directly would be a silent conversion.
enum class Level : int {
  low,
};

} // namespace

// The probe must fail for every type the packers do not serve.
template <typename T>
concept CanPackArg = requires(T value) { detail::packArg(value); };

// One packed argument stays small and trivially copyable, so a pack of arguments stays cheap to build and pass.
TEST_CASE("detail/format_arg/size/trivially_copyable") {
  static_assert(sizeof(detail::FormatArg) <= 16 && std::is_trivially_copyable_v<detail::FormatArg>,
                "detail::FormatArg must stay a trivial value of at most sixteen bytes");
}

// Runtime and constant-evaluated packing produce the same tagged slots, payloads included.
TEST_CASE("detail/format_arg/packing/scalar_slots") {
  CHECK(detail::packArg('x').type == detail::FormatArgType::charValue);
  CHECK(detail::packArg('x').character == 'x');
  CHECK(detail::packArg(true).type == detail::FormatArgType::boolValue);
  CHECK(detail::packArg(true).boolean == true);
  CHECK(detail::packArg(false).type == detail::FormatArgType::boolValue);
  CHECK(detail::packArg(false).boolean == false);

  static_assert(detail::packArg('x').type == detail::FormatArgType::charValue && detail::packArg('x').character == 'x',
                "char keeps its own slot");
  static_assert(detail::packArg(true).type == detail::FormatArgType::boolValue && detail::packArg(true).boolean,
                "bool keeps its own slot");
  static_assert(detail::packArg(false).type == detail::FormatArgType::boolValue && !detail::packArg(false).boolean,
                "a false value packs unchanged");
}

// Integers pack to the slot of their own width, tag and payload included; constexpr and runtime agree.
TEST_CASE("detail/format_arg/packing/integer_slots") {
  CHECK(detail::packArg(int8_t{-1}).type == detail::FormatArgType::int8Value);
  CHECK(detail::packArg(int8_t{-1}).int8 == int8_t{-1});
  CHECK(detail::packArg(int16_t{300}).type == detail::FormatArgType::int16Value);
  CHECK(detail::packArg(int16_t{300}).int16 == int16_t{300});
  CHECK(detail::packArg(int32_t{70'000}).type == detail::FormatArgType::int32Value);
  CHECK(detail::packArg(int32_t{70'000}).int32 == int32_t{70'000});
  CHECK(detail::packArg(int64_t{1}).type == detail::FormatArgType::int64Value);
  CHECK(detail::packArg(int64_t{1}).int64 == int64_t{1});
  CHECK(detail::packArg(uint8_t{200}).type == detail::FormatArgType::uint8Value);
  CHECK(detail::packArg(uint8_t{200}).uint8 == uint8_t{200});
  CHECK(detail::packArg(uint16_t{40'000}).type == detail::FormatArgType::uint16Value);
  CHECK(detail::packArg(uint16_t{40'000}).uint16 == uint16_t{40'000});
  CHECK(detail::packArg(uint32_t{3'000'000'000U}).type == detail::FormatArgType::uint32Value);
  CHECK(detail::packArg(uint32_t{3'000'000'000U}).uint32 == uint32_t{3'000'000'000U});
  CHECK(detail::packArg(uint64_t{1}).type == detail::FormatArgType::uint64Value);
  CHECK(detail::packArg(uint64_t{1}).uint64 == uint64_t{1});

  static_assert(detail::packArg(int8_t{-1}).type == detail::FormatArgType::int8Value
                  && detail::packArg(int8_t{-1}).int8 == int8_t{-1},
                "int8_t keeps its own slot");
  static_assert(detail::packArg(int16_t{300}).type == detail::FormatArgType::int16Value
                  && detail::packArg(int16_t{300}).int16 == int16_t{300},
                "int16_t keeps its own slot");
  static_assert(detail::packArg(int32_t{70'000}).type == detail::FormatArgType::int32Value
                  && detail::packArg(int32_t{70'000}).int32 == int32_t{70'000},
                "int32_t keeps its own slot");
  static_assert(detail::packArg(int64_t{1}).type == detail::FormatArgType::int64Value
                  && detail::packArg(int64_t{1}).int64 == int64_t{1},
                "int64_t keeps its own slot");
  static_assert(detail::packArg(uint8_t{200}).type == detail::FormatArgType::uint8Value
                  && detail::packArg(uint8_t{200}).uint8 == uint8_t{200},
                "uint8_t keeps its own slot");
  static_assert(detail::packArg(uint16_t{40'000}).type == detail::FormatArgType::uint16Value
                  && detail::packArg(uint16_t{40'000}).uint16 == uint16_t{40'000},
                "uint16_t keeps its own slot");
  static_assert(detail::packArg(uint32_t{3'000'000'000U}).type == detail::FormatArgType::uint32Value
                  && detail::packArg(uint32_t{3'000'000'000U}).uint32 == uint32_t{3'000'000'000U},
                "uint32_t keeps its own slot");
  static_assert(detail::packArg(uint64_t{1}).type == detail::FormatArgType::uint64Value
                  && detail::packArg(uint64_t{1}).uint64 == uint64_t{1},
                "uint64_t keeps its own slot");
  static_assert(CanPackArg<int>, "an int packs to the slot of its own width");
}

// Every type outside the pack surface fails to pack.
TEST_CASE("detail/format_arg/packing/negative_probes") {
  static_assert(!CanPackArg<Unpackable>, "a type no packer serves must fail the concept");
  static_assert(!CanPackArg<float>, "floating-point values pack nowhere");
  static_assert(!CanPackArg<double>, "floating-point values pack nowhere");
  static_assert(!CanPackArg<Level>, "an enumeration packs through std::to_underlying at the call site");
  static_assert(!CanPackArg<const char *>, "a pointer must not pack as its implicit bool");
  static_assert(!CanPackArg<std::nullptr_t>, "a null pointer constant must not pack as false");
  static_assert(!CanPackArg<int Unpackable::*>, "a pointer to member data must not pack as its implicit bool");
  static_assert(!CanPackArg<void (Unpackable::*)()>, "a pointer to member function must not pack as its implicit bool");
  static_assert(!CanPackArg<Truthy>, "a class converting to bool must not pack");
  static_assert(!CanPackArg<StringView>, "views pack nowhere");
}

} // namespace toy
