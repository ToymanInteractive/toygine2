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
  \file   string_view.inl
  \brief  Inline implementations for \ref toy::StringView.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_VIEW_INL_
#define INCLUDE_CORE_STRING_VIEW_INL_

#include "string_utils.hpp"
#include "utils.hpp"

namespace toy {

constexpr StringView::StringView(const value_type * string) noexcept
  : _data(string)
  , _size(string ? traits_type::length(string) : 0) {
  assert_message(string != nullptr, "C string must not be null");
}

constexpr StringView::const_iterator StringView::begin() const noexcept {
  return cbegin();
}

constexpr StringView::const_iterator StringView::cbegin() const noexcept {
  return const_iterator(_data);
}

constexpr StringView::const_iterator StringView::end() const noexcept {
  return cend();
}

constexpr StringView::const_iterator StringView::cend() const noexcept {
  return const_iterator(_data + _size);
}

constexpr StringView::const_reverse_iterator StringView::rbegin() const noexcept {
  return const_reverse_iterator(cend());
}

constexpr StringView::const_reverse_iterator StringView::crbegin() const noexcept {
  return const_reverse_iterator(cend());
}

constexpr StringView::const_reverse_iterator StringView::rend() const noexcept {
  return const_reverse_iterator(cbegin());
}

constexpr StringView::const_reverse_iterator StringView::crend() const noexcept {
  return const_reverse_iterator(cbegin());
}

constexpr StringView::const_reference StringView::operator[](size_type pos) const noexcept {
  assert_message(pos < size(), "Offset must be within bounds");

  return _data[pos];
}

constexpr StringView::const_reference StringView::at(size_type pos) const noexcept {
  assert_message(pos < size(), "Offset must be within bounds");

  return _data[pos];
}

constexpr StringView::const_reference StringView::front() const noexcept {
  assert_message(!empty(), "String view must not be empty");

  return _data[0];
}

constexpr StringView::const_reference StringView::back() const noexcept {
  assert_message(!empty(), "String view must not be empty");

  return _data[_size - 1];
}

constexpr StringView::const_pointer StringView::data() const noexcept {
  return _data;
}

constexpr StringView::const_pointer StringView::c_str() const noexcept {
  return _data;
}

constexpr StringView::size_type StringView::size() const noexcept {
  return _size;
}

constexpr StringView::size_type StringView::length() const noexcept {
  return _size;
}

constexpr StringView::size_type StringView::max_size() const noexcept {
  return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

inline StringView::size_type StringView::utf8_size() const noexcept {
  return _data != nullptr ? utf8Len(_data) : 0;
}

constexpr bool StringView::empty() const noexcept {
  return _size == 0;
}

constexpr void StringView::remove_prefix(size_type n) noexcept {
  assert_message(n <= size(), "remove_prefix() can't remove more than size()");

  _data += n;
  _size -= n;
}

constexpr void StringView::swap(StringView & v) noexcept {
  const value_type * p = _data;
  _data                = v._data;
  v._data              = p;

  const auto sz = _size;
  _size         = v._size;
  v._size       = sz;
}

constexpr StringView::size_type StringView::copy(value_type * dest, size_type count, size_type pos) const noexcept {
  assert_message(pos <= size(), "copy() can't copy past the end of the string");

  const auto rLength = std::min(count, size() - pos);
  if (rLength != 0)
    traits_type::copy(dest, data() + pos, rLength);

  return rLength;
}

constexpr int StringView::compare(StringView v) const noexcept {
  return string_utils::compare(data(), size(), v.data(), v.size());
}

constexpr int StringView::compare(size_type pos1, size_type count1, StringView v) const noexcept {
  assert_message(pos1 <= size(), "Range out of bounds");

  return string_utils::compare(data() + pos1, std::min(count1, size() - pos1), v.data(), v.size());
}

constexpr int StringView::compare(size_type pos1, size_type count1, StringView v, size_type pos2,
                                  size_type count2) const noexcept {
  assert_message(pos1 <= size(), "Range out of bounds");
  assert_message(pos2 <= v.size(), "Range out of bounds");

  return string_utils::compare(data() + pos1, std::min(count1, size() - pos1), v.data() + pos2,
                               std::min(count2, v.size() - pos2));
}

constexpr int StringView::compare(const value_type * s) const noexcept {
  return compare(StringView(s));
}

constexpr int StringView::compare(size_type pos1, size_type count1, const value_type * s) const noexcept {
  return compare(pos1, count1, StringView(s));
}

constexpr int StringView::compare(size_type pos1, size_type count1, const value_type * s,
                                  size_type count2) const noexcept {
  assert_message(pos1 <= size(), "Range out of bounds");
  assert_message(s != nullptr, "C string must not be null");

  return string_utils::compare(data() + pos1, std::min(count1, size() - pos1), s, count2);
}

constexpr bool StringView::starts_with(StringView sv) const noexcept {
  return size() >= sv.size() && compare(0, sv.size(), sv) == 0;
}

constexpr bool StringView::starts_with(value_type ch) const noexcept {
  return !empty() && traits_type::eq(front(), ch);
}

constexpr bool StringView::starts_with(const value_type * s) const noexcept {
  return starts_with(StringView(s));
}

constexpr bool StringView::ends_with(StringView sv) const noexcept {
  return size() >= sv.size() && compare(size() - sv.size(), sv.size(), sv) == 0;
}

constexpr bool StringView::ends_with(value_type ch) const noexcept {
  return !empty() && traits_type::eq(back(), ch);
}

constexpr bool StringView::ends_with(const value_type * s) const noexcept {
  return ends_with(StringView(s));
}

constexpr bool StringView::contains(StringView sv) const noexcept {
  return find(sv) != npos;
}

constexpr bool StringView::contains(value_type ch) const noexcept {
  return find(ch) != npos;
}

constexpr bool StringView::contains(const value_type * s) const noexcept {
  return find(s) != npos;
}

constexpr StringView::size_type StringView::find(StringView v, size_type pos) const noexcept {
  return find(v.data(), pos, v.size());
}

constexpr StringView::size_type StringView::find(value_type ch, size_type pos) const noexcept {
  return string_utils::find(data(), size(), ch, pos);
}

constexpr StringView::size_type StringView::find(const value_type * s, size_type pos, size_type count) const noexcept {
  return string_utils::find(data(), size(), s, pos, count);
}

constexpr StringView::size_type StringView::find(const value_type * s, size_type pos) const noexcept {
  return find(StringView(s), pos);
}

constexpr StringView::size_type StringView::rfind(StringView v, size_type pos) const noexcept {
  return rfind(v.data(), pos, v.size());
}

constexpr StringView::size_type StringView::rfind(value_type ch, size_type pos) const noexcept {
  return string_utils::rfind(data(), size(), ch, pos);
}

constexpr StringView::size_type StringView::rfind(const value_type * s, size_type pos, size_type count) const noexcept {
  return string_utils::rfind(data(), size(), s, pos, count);
}

constexpr StringView::size_type StringView::rfind(const value_type * s, size_type pos) const noexcept {
  return rfind(StringView(s), pos);
}

constexpr StringView::size_type StringView::find_first_of(StringView v, size_type pos) const noexcept {
  return find_first_of(v.data(), pos, v.size());
}

constexpr StringView::size_type StringView::find_first_of(value_type ch, size_type pos) const noexcept {
  return find(ch, pos);
}

constexpr StringView::size_type StringView::find_first_of(const value_type * s, size_type pos,
                                                          size_type count) const noexcept {
  return string_utils::findFirstOf(data(), size(), s, pos, count);
}

constexpr StringView::size_type StringView::find_first_of(const value_type * s, size_type pos) const noexcept {
  return find_first_of(StringView(s), pos);
}

constexpr StringView::size_type StringView::find_last_of(StringView v, size_type pos) const noexcept {
  return find_last_of(v.data(), pos, v.size());
}

constexpr StringView::size_type StringView::find_last_of(value_type ch, size_type pos) const noexcept {
  return rfind(ch, pos);
}

constexpr StringView::size_type StringView::find_last_of(const value_type * s, size_type pos,
                                                         size_type count) const noexcept {
  return string_utils::findLastOf(data(), size(), s, pos, count);
}

constexpr StringView::size_type StringView::find_last_of(const value_type * s, size_type pos) const noexcept {
  return find_last_of(StringView(s), pos);
}

constexpr StringView::size_type StringView::find_first_not_of(StringView v, size_type pos) const noexcept {
  return find_first_not_of(v.data(), pos, v.size());
}

constexpr StringView::size_type StringView::find_first_not_of(value_type ch, size_type pos) const noexcept {
  return string_utils::findFirstNotOf(data(), size(), ch, pos);
}

constexpr StringView::size_type StringView::find_first_not_of(const value_type * s, size_type pos,
                                                              size_type count) const noexcept {
  return string_utils::findFirstNotOf(data(), size(), s, pos, count);
}

constexpr StringView::size_type StringView::find_first_not_of(const value_type * s, size_type pos) const noexcept {
  return find_first_not_of(StringView(s), pos);
}

constexpr StringView::size_type StringView::find_last_not_of(StringView v, size_type pos) const noexcept {
  return find_last_not_of(v.data(), pos, v.size());
}

constexpr StringView::size_type StringView::find_last_not_of(value_type ch, size_type pos) const noexcept {
  return string_utils::findLastNotOf(data(), size(), ch, pos);
}

constexpr StringView::size_type StringView::find_last_not_of(const value_type * s, size_type pos,
                                                             size_type count) const noexcept {
  return string_utils::findLastNotOf(data(), size(), s, pos, count);
}

constexpr StringView::size_type StringView::find_last_not_of(const value_type * s, size_type pos) const noexcept {
  return find_last_not_of(StringView(s), pos);
}

constexpr bool operator==(StringView lhs, StringView rhs) noexcept {
  return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
}

constexpr StringView::traits_type::comparison_category operator<=>(StringView lhs, StringView rhs) noexcept {
  return lhs.compare(rhs) <=> 0;
}

} // namespace toy

#endif // INCLUDE_CORE_STRING_VIEW_INL_
