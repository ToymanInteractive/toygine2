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
  \file   c_string_view.inl
  \brief  Inline implementations for \ref toy::CStringView.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_C_STRING_VIEW_INL_
#define INCLUDE_CORE_C_STRING_VIEW_INL_

namespace toy {

constexpr CStringView::CStringView(const char * string) noexcept
  : _data(string)
  , _size(string ? traits_type::length(string) : 0) {
  assert_message(string != nullptr, "C string must not be null");
}

constexpr CStringView::const_iterator CStringView::begin() const noexcept {
  return cbegin();
}

constexpr CStringView::const_iterator CStringView::cbegin() const noexcept {
  return const_iterator(_data);
}

constexpr CStringView::const_iterator CStringView::end() const noexcept {
  return cend();
}

constexpr CStringView::const_iterator CStringView::cend() const noexcept {
  return const_iterator(_data + _size);
}

constexpr CStringView::const_reverse_iterator CStringView::rbegin() const noexcept {
  return const_reverse_iterator(cend());
}

constexpr CStringView::const_reverse_iterator CStringView::crbegin() const noexcept {
  return const_reverse_iterator(cend());
}

constexpr CStringView::const_reverse_iterator CStringView::rend() const noexcept {
  return const_reverse_iterator(cbegin());
}

constexpr CStringView::const_reverse_iterator CStringView::crend() const noexcept {
  return const_reverse_iterator(cbegin());
}

constexpr CStringView::const_reference CStringView::operator[](size_type pos) const noexcept {
  assert_message(pos < size(), "Offset must be within bounds");

  return _data[pos];
}

constexpr CStringView::const_reference CStringView::at(size_type pos) const noexcept {
  assert_message(pos < size(), "Offset must be within bounds");

  return _data[pos];
}

constexpr CStringView::const_reference CStringView::front() const noexcept {
  assert_message(!empty(), "String view must not be empty");

  return _data[0];
}

constexpr CStringView::const_reference CStringView::back() const noexcept {
  assert_message(!empty(), "String view must not be empty");

  return _data[_size - 1];
}

constexpr CStringView::const_pointer CStringView::data() const noexcept {
  return _data;
}

constexpr CStringView::const_pointer CStringView::c_str() const noexcept {
  return _data;
}

constexpr CStringView::size_type CStringView::size() const noexcept {
  return _size;
}

constexpr CStringView::size_type CStringView::length() const noexcept {
  return _size;
}

constexpr CStringView::size_type CStringView::max_size() const noexcept {
  return numeric_limits<size_type>::max() / sizeof(value_type);
}

constexpr bool CStringView::empty() const noexcept {
  return _size == 0;
}

constexpr void CStringView::remove_prefix(size_type n) noexcept {
  assert_message(n <= size(), "remove_prefix() can't remove more than size()");

  _data += n;
  _size -= n;
}

constexpr void CStringView::swap(CStringView & v) noexcept {
  const value_type * p = _data;
  _data                = v._data;
  v._data              = p;

  const auto sz = _size;
  _size         = v._size;
  v._size       = sz;
}

constexpr CStringView::size_type CStringView::copy(value_type * dest, size_type count, size_type pos) const noexcept {
  assert_message(pos <= size(), "copy() can't copy past the end of the string");

  const auto rLength = min(count, size() - pos);
  traits_type::copy(dest, data() + pos, rLength);

  return rLength;
}

constexpr int CStringView::compare(CStringView v) const noexcept {
  const int retVal = traits_type::compare(data(), v.data(), min(size(), v.size()));
  if (retVal == 0)
    return size() == v.size() ? 0 : (size() < v.size() ? -1 : 1);

  return retVal;
}

constexpr int CStringView::compare(size_type pos1, size_type count1, CStringView v) const noexcept {
  assert_message(pos1 <= size() && count1 <= size() - pos1, "Range out of bounds");

  const int retVal = traits_type::compare(data() + pos1, v.data(), min(count1, v.size()));
  if (retVal == 0)
    return count1 == v.size() ? 0 : (count1 < v.size() ? -1 : 1);

  return retVal;
}

constexpr int CStringView::compare(size_type pos1, size_type count1, CStringView v, size_type pos2,
                                   size_type count2) const noexcept {
  assert_message(pos1 <= size() && count1 <= size() - pos1, "Range out of bounds");
  assert_message(pos2 <= v.size() && count2 <= v.size() - pos2, "Range out of bounds");

  const int retVal = traits_type::compare(data() + pos1, v.data() + pos2, min(count1, count2));
  if (retVal == 0)
    return count1 == count2 ? 0 : (count1 < count2 ? -1 : 1);

  return retVal;
}

constexpr int CStringView::compare(const value_type * s) const noexcept {
  return compare(CStringView(s));
}

constexpr int CStringView::compare(size_type pos1, size_type count1, const value_type * s) const noexcept {
  return compare(pos1, count1, CStringView(s));
}

constexpr int CStringView::compare(size_type pos1, size_type count1, const value_type * s,
                                   size_type count2) const noexcept {
  assert_message(pos1 <= size() && count1 <= size() - pos1, "Range out of bounds");
  assert_message(s != nullptr, "C string must not be null");

  const int retVal = traits_type::compare(data() + pos1, s, min(count1, count2));
  if (retVal == 0)
    return count1 == count2 ? 0 : (count1 < count2 ? -1 : 1);

  return retVal;
}

constexpr bool CStringView::starts_with(CStringView sv) const noexcept {
  return size() >= sv.size() && compare(0, sv.size(), sv) == 0;
}

constexpr bool CStringView::starts_with(value_type ch) const noexcept {
  return !empty() && traits_type::eq(front(), ch);
}

constexpr bool CStringView::starts_with(const value_type * s) const noexcept {
  return starts_with(CStringView(s));
}

constexpr bool CStringView::ends_with(CStringView sv) const noexcept {
  return size() >= sv.size() && compare(size() - sv.size(), sv.size(), sv) == 0;
}

constexpr bool CStringView::ends_with(value_type ch) const noexcept {
  return !empty() && traits_type::eq(back(), ch);
}

constexpr bool CStringView::ends_with(const value_type * s) const noexcept {
  return ends_with(CStringView(s));
}

constexpr bool CStringView::contains(CStringView sv) const noexcept {
  return find(sv) != npos;
}

constexpr bool CStringView::contains(value_type ch) const noexcept {
  return find(ch) != npos;
}

constexpr bool CStringView::contains(const value_type * s) const noexcept {
  return find(s) != npos;
}

constexpr CStringView::size_type CStringView::find(CStringView v, size_type pos) const noexcept {
  return find(v.data(), pos, v.size());
}

constexpr CStringView::size_type CStringView::find(value_type ch, size_type pos) const noexcept {
  for (size_type index = pos; index < size(); ++index)
    if (traits_type::eq(_data[index], ch))
      return index;

  return npos;
}

constexpr CStringView::size_type CStringView::find(const value_type * s, size_type pos,
                                                   size_type count) const noexcept {
  if (count == 0)
    return pos <= size() ? pos : npos;
  else if (count > size())
    return npos;

  const size_type last = size() - count;
  for (size_type index = pos; index <= last; ++index)
    if (traits_type::compare(_data + index, s, count) == 0)
      return index;

  return npos;
}

constexpr CStringView::size_type CStringView::find(const value_type * s, size_type pos) const noexcept {
  return find(CStringView(s), pos);
}

constexpr CStringView::size_type CStringView::rfind(CStringView v, size_type pos) const noexcept {
  return rfind(v.data(), pos, v.size());
}

constexpr CStringView::size_type CStringView::rfind(value_type ch, size_type pos) const noexcept {
  for (size_type remaining = pos < size() ? pos + 1 : size(); remaining > 0; --remaining)
    if (traits_type::eq(_data[remaining - 1], ch))
      return remaining - 1;

  return npos;
}

constexpr CStringView::size_type CStringView::rfind(const value_type * s, size_type pos,
                                                    size_type count) const noexcept {
  if (count == 0)
    return min(pos, size());
  else if (count > size())
    return npos;

  for (size_type remaining = min(pos, size() - count) + 1; remaining > 0; --remaining)
    if (traits_type::compare(_data + remaining - 1, s, count) == 0)
      return remaining - 1;

  return npos;
}

constexpr CStringView::size_type CStringView::rfind(const value_type * s, size_type pos) const noexcept {
  return rfind(CStringView(s), pos);
}

constexpr CStringView::size_type CStringView::find_first_of(CStringView v, size_type pos) const noexcept {
  return find_first_of(v.data(), pos, v.size());
}

constexpr CStringView::size_type CStringView::find_first_of(value_type ch, size_type pos) const noexcept {
  return find(ch, pos);
}

constexpr CStringView::size_type CStringView::find_first_of(const value_type * s, size_type pos,
                                                            size_type count) const noexcept {
  for (size_type index = pos; index < size(); ++index)
    if (traits_type::find(s, count, _data[index]) != nullptr)
      return index;

  return npos;
}

constexpr CStringView::size_type CStringView::find_first_of(const value_type * s, size_type pos) const noexcept {
  return find_first_of(CStringView(s), pos);
}

constexpr CStringView::size_type CStringView::find_last_of(CStringView v, size_type pos) const noexcept {
  return find_last_of(v.data(), pos, v.size());
}

constexpr CStringView::size_type CStringView::find_last_of(value_type ch, size_type pos) const noexcept {
  return rfind(ch, pos);
}

constexpr CStringView::size_type CStringView::find_last_of(const value_type * s, size_type pos,
                                                           size_type count) const noexcept {
  for (size_type remaining = pos < size() ? pos + 1 : size(); remaining > 0; --remaining)
    if (traits_type::find(s, count, _data[remaining - 1]) != nullptr)
      return remaining - 1;

  return npos;
}

constexpr CStringView::size_type CStringView::find_last_of(const value_type * s, size_type pos) const noexcept {
  return find_last_of(CStringView(s), pos);
}

constexpr CStringView::size_type CStringView::find_first_not_of(CStringView v, size_type pos) const noexcept {
  return find_first_not_of(v.data(), pos, v.size());
}

constexpr CStringView::size_type CStringView::find_first_not_of(value_type ch, size_type pos) const noexcept {
  for (size_type index = pos; index < size(); ++index)
    if (!traits_type::eq(_data[index], ch))
      return index;

  return npos;
}

constexpr CStringView::size_type CStringView::find_first_not_of(const value_type * s, size_type pos,
                                                                size_type count) const noexcept {
  for (size_type index = pos; index < size(); ++index)
    if (traits_type::find(s, count, _data[index]) == nullptr)
      return index;

  return npos;
}

constexpr CStringView::size_type CStringView::find_first_not_of(const value_type * s, size_type pos) const noexcept {
  return find_first_not_of(CStringView(s), pos);
}

constexpr CStringView::size_type CStringView::find_last_not_of(CStringView v, size_type pos) const noexcept {
  return find_last_not_of(v.data(), pos, v.size());
}

constexpr CStringView::size_type CStringView::find_last_not_of(value_type ch, size_type pos) const noexcept {
  for (size_type remaining = pos < size() ? pos + 1 : size(); remaining > 0; --remaining)
    if (!traits_type::eq(_data[remaining - 1], ch))
      return remaining - 1;

  return npos;
}

constexpr CStringView::size_type CStringView::find_last_not_of(const value_type * s, size_type pos,
                                                               size_type count) const noexcept {
  for (size_type remaining = pos < size() ? pos + 1 : size(); remaining > 0; --remaining)
    if (traits_type::find(s, count, _data[remaining - 1]) == nullptr)
      return remaining - 1;

  return npos;
}

constexpr CStringView::size_type CStringView::find_last_not_of(const value_type * s, size_type pos) const noexcept {
  return find_last_not_of(CStringView(s), pos);
}

constexpr bool operator==(CStringView lhs, CStringView rhs) noexcept {
  return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
}

constexpr CStringView::traits_type::comparison_category operator<=>(CStringView lhs, CStringView rhs) noexcept {
  return lhs.compare(rhs) <=> 0;
}

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_INL_
