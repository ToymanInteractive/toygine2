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
  \brief  Unit tests for \ref toy::FormatArg and the toy::packArg() packers.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy {

namespace {

// A type without a packArg() overload; the negative probes must reject it at the concept.
struct Unpackable {
  int payload;
};

} // namespace

// The probe must fail for every type the two packers do not serve, without instantiating any dump.
template <typename T>
concept CanPackArg = requires(T value) { packArg(value); };

// The packed argument stays a small trivial value, so the pack rides the stack of the calling wrapper.
TEST_CASE("format_arg/size/trivially_copyable") {
  static_assert(sizeof(FormatArg) <= 8 && std::is_trivially_copyable_v<FormatArg>,
                "FormatArg must stay a trivial value under eight bytes");
}

// The two supported argument kinds pack to their tagged slots.
TEST_CASE("format_arg/packing/scalar_slots") {
  static_assert(packArg('x').type == FormatArgType::charValue && packArg('x').character == 'x',
                "char keeps its own slot");
  static_assert(packArg(true).type == FormatArgType::boolValue && packArg(true).boolean, "bool keeps its own slot");
  static_assert(packArg(false).type == FormatArgType::boolValue && !packArg(false).boolean,
                "a false value packs unchanged");
}

// Runtime packing produces the same slots the constant expressions above assert.
TEST_CASE("format_arg/packing/runtime_parity") {
  CHECK(packArg('x').type == FormatArgType::charValue);
  CHECK(packArg('x').character == 'x');
  CHECK(packArg(true).type == FormatArgType::boolValue);
  CHECK(packArg(false).type == FormatArgType::boolValue);
}

// Every type outside char and bool fails the pack at the concept.
TEST_CASE("format_arg/packing/negative_probes") {
  static_assert(!CanPackArg<Unpackable>, "a type without a packArg() overload must fail the concept");
  static_assert(!CanPackArg<int>, "integers pack nowhere in this build");
  static_assert(!CanPackArg<double>, "floating-point values pack nowhere in this build");
  static_assert(!CanPackArg<const char *>, "a pointer must not pack as its implicit bool");
  static_assert(!CanPackArg<std::nullptr_t>, "a null pointer constant must not pack as false");
  static_assert(!CanPackArg<StringView>, "views pack nowhere in this build");
}

} // namespace toy
