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
  \file   string.inl
  \brief  Inline implementations for \ref toy::String members and the free functions that take it.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_INL_
#define INCLUDE_CORE_STRING_INL_

namespace toy {

template <StringStorage storageType>
constexpr String<storageType>::String(size_type count, value_type ch) noexcept {
  const bool reserved = _storage.reserve(count);
  assert_message(reserved, "the string must fit the storage it is built in");
  if (!reserved)
    return;

  traits_type::assign(_storage.data(), count, ch);
  _storage.setSize(count);
}

template <StringStorage storageType>
template <std::input_iterator InputIterator>
  requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
constexpr String<storageType>::String(InputIterator first, InputIterator last) noexcept {
  _copyExternalRange(std::move(first), std::move(last));
}

#if defined(__cpp_lib_ranges_to_container)
template <StringStorage storageType>
template <std::ranges::input_range Range>
  requires std::same_as<std::ranges::range_value_t<Range>, char>
constexpr String<storageType>::String(std::from_range_t, Range && range) noexcept {
  _copyExternalRange(std::ranges::begin(range), std::ranges::end(range));
}
#endif // __cpp_lib_ranges_to_container

template <StringStorage storageType>
constexpr String<storageType>::String(const value_type * string, size_type count) noexcept {
  assert_message(string != nullptr || count == 0, "a null pointer names no characters to copy");

  const bool reserved = _storage.reserve(count);
  assert_message(reserved, "the string must fit the storage it is built in");
  if (!reserved)
    return;

  // memmove behind std::char_traits::copy takes no null source, even for an empty range.
  if (count != 0)
    traits_type::copy(_storage.data(), string, count);

  _storage.setSize(count);
}

template <StringStorage storageType>
constexpr String<storageType>::String(const value_type * string) noexcept
  : String(string, string != nullptr ? traits_type::length(string) : 0) {
  assert_message(string != nullptr, "C string must not be null");
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType>::String(const StringType & string) noexcept
  : String(string.c_str(), string.size()) {}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType>::String(const StringType & string, size_type pos, size_type count) noexcept
  : String(string.c_str() + std::min(pos, string.size()),
           std::min(count, string.size() - std::min(pos, string.size()))) {
  assert_message(pos <= string.size(), "the substring must start inside the source string");
}

template <StringStorage storageType>
constexpr String<storageType>::String(String && other, size_type pos, size_type count) noexcept
  : _storage(std::move(other._storage)) {
  const size_type sourceLength = _storage.size();
  assert_message(pos <= sourceLength, "the substring must start inside the source string");

  const size_type offset = std::min(pos, sourceLength);
  const size_type length = std::min(count, sourceLength - offset);
  if (offset != 0 && length != 0)
    traits_type::move(_storage.data(), _storage.data() + offset, length);

  _storage.setSize(length);
}

template <StringStorage storageType>
constexpr String<storageType>::String(std::initializer_list<value_type> list) noexcept
  : String(list.begin(), list.size()) {}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::operator=(const value_type * string) noexcept {
  return assign(string);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::operator=(value_type ch) noexcept {
  return assign(size_type{1}, ch);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::operator=(std::initializer_list<value_type> list) noexcept {
  return assign(list);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::operator=(const StringType & string) noexcept {
  return assign(string);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::assign(size_type count, value_type ch) noexcept {
  const bool reserved = _storage.reserve(count);
  assert_message(reserved, "the assigned string must fit the storage");
  if (reserved) {
    traits_type::assign(_storage.data(), count, ch);
    _storage.setSize(count);
  }

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::assign(const String & other) noexcept {
  return *this = other;
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::assign(String && other) noexcept {
  return *this = std::move(other);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::assign(const value_type * string, size_type count) noexcept {
  assert_message(string != nullptr || count == 0, "a null pointer names no characters to copy");

  const bool reserved = _storage.reserve(count);
  assert_message(reserved, "the assigned string must fit the storage");
  if (reserved) {
    // A source inside this string is no longer than size(), so reserve() has kept the buffer it points into.
    if (count != 0) {
      if (_mayOverlap(string))
        traits_type::move(_storage.data(), string, count);
      else
        traits_type::copy(_storage.data(), string, count);
    }

    _storage.setSize(count);
  }

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::assign(const value_type * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return assign(string, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
template <std::input_iterator InputIterator>
  requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
constexpr String<storageType> & String<storageType>::assign(InputIterator first, InputIterator last) noexcept {
  _copyRange(std::move(first), std::move(last));

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::assign(std::initializer_list<value_type> list) noexcept {
  return assign(list.begin(), list.size());
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::assign(const StringType & string) noexcept {
  return assign(string.c_str(), string.size());
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::assign(const StringType & string, size_type pos,
                                                            size_type count) noexcept {
  assert_message(pos <= string.size(), "the substring must start inside the source string");
  if (pos > string.size())
    return *this;

  return assign(string.c_str() + pos, std::min(count, string.size() - pos));
}

template <StringStorage storageType>
template <std::ranges::input_range Range>
  requires std::same_as<std::ranges::range_value_t<Range>, char>
constexpr String<storageType> & String<storageType>::assign_range(Range && range) noexcept {
  _copyRange(std::ranges::begin(range), std::ranges::end(range));

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType>::reference String<storageType>::at(size_type pos) noexcept {
  assert_message(pos < size(), "the position must lie inside the string");

  return data()[pos];
}

template <StringStorage storageType>
constexpr String<storageType>::const_reference String<storageType>::at(size_type pos) const noexcept {
  assert_message(pos < size(), "the position must lie inside the string");

  return data()[pos];
}

template <StringStorage storageType>
constexpr String<storageType>::reference String<storageType>::operator[](size_type pos) noexcept {
  assert_message(pos <= size(), "the position must lie inside the string or on its terminator");

  return data()[pos];
}

template <StringStorage storageType>
constexpr String<storageType>::const_reference String<storageType>::operator[](size_type pos) const noexcept {
  assert_message(pos <= size(), "the position must lie inside the string or on its terminator");

  return data()[pos];
}

template <StringStorage storageType>
constexpr String<storageType>::reference String<storageType>::front() noexcept {
  assert_message(size() != 0, "the string must not be empty");

  return data()[0];
}

template <StringStorage storageType>
constexpr String<storageType>::const_reference String<storageType>::front() const noexcept {
  assert_message(size() != 0, "the string must not be empty");

  return data()[0];
}

template <StringStorage storageType>
constexpr String<storageType>::reference String<storageType>::back() noexcept {
  assert_message(size() != 0, "the string must not be empty");

  return data()[size() - 1];
}

template <StringStorage storageType>
constexpr String<storageType>::const_reference String<storageType>::back() const noexcept {
  assert_message(size() != 0, "the string must not be empty");

  return data()[size() - 1];
}

template <StringStorage storageType>
constexpr String<storageType>::pointer String<storageType>::data() noexcept {
  return _storage.data();
}

template <StringStorage storageType>
constexpr String<storageType>::const_pointer String<storageType>::data() const noexcept {
  return _storage.data();
}

template <StringStorage storageType>
constexpr String<storageType>::const_pointer String<storageType>::c_str() const noexcept {
  return data();
}

template <StringStorage storageType>
constexpr String<storageType>::operator StringView() const noexcept {
  return StringView(c_str());
}

template <StringStorage storageType>
constexpr String<storageType>::iterator String<storageType>::begin() noexcept {
  return data();
}

template <StringStorage storageType>
constexpr String<storageType>::const_iterator String<storageType>::begin() const noexcept {
  return data();
}

template <StringStorage storageType>
constexpr String<storageType>::const_iterator String<storageType>::cbegin() const noexcept {
  return begin();
}

template <StringStorage storageType>
constexpr String<storageType>::iterator String<storageType>::end() noexcept {
  return data() + size();
}

template <StringStorage storageType>
constexpr String<storageType>::const_iterator String<storageType>::end() const noexcept {
  return data() + size();
}

template <StringStorage storageType>
constexpr String<storageType>::const_iterator String<storageType>::cend() const noexcept {
  return end();
}

template <StringStorage storageType>
constexpr String<storageType>::reverse_iterator String<storageType>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <StringStorage storageType>
constexpr String<storageType>::const_reverse_iterator String<storageType>::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <StringStorage storageType>
constexpr String<storageType>::const_reverse_iterator String<storageType>::crbegin() const noexcept {
  return rbegin();
}

template <StringStorage storageType>
constexpr String<storageType>::reverse_iterator String<storageType>::rend() noexcept {
  return reverse_iterator(begin());
}

template <StringStorage storageType>
constexpr String<storageType>::const_reverse_iterator String<storageType>::rend() const noexcept {
  return const_reverse_iterator(begin());
}

template <StringStorage storageType>
constexpr String<storageType>::const_reverse_iterator String<storageType>::crend() const noexcept {
  return rend();
}

template <StringStorage storageType>
constexpr bool String<storageType>::empty() const noexcept {
  return size() == 0;
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::size() const noexcept {
  return _storage.size();
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::length() const noexcept {
  return size();
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::max_size() const noexcept {
  return _storage.max_size();
}

template <StringStorage storageType>
constexpr void String<storageType>::reserve(size_type newCapacity) noexcept {
  [[maybe_unused]] const bool reserved = _storage.reserve(newCapacity);
  assert_message(reserved, "the requested capacity must fit the storage");
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::capacity() const noexcept {
  return _storage.capacity();
}

template <StringStorage storageType>
constexpr void String<storageType>::shrink_to_fit() noexcept {
  _storage.shrink_to_fit();
}

template <StringStorage storageType>
constexpr void String<storageType>::clear() noexcept {
  _storage.setSize(0);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::insert(size_type index, size_type count, value_type ch) noexcept {
  assert_message(index <= size(), "the insertion point must lie inside the string or at its end");
  if (index <= size() && _openGap(index, 0, count))
    traits_type::assign(data() + index, count, ch);

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::insert(size_type index, const value_type * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return insert(index, string, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::insert(size_type index, const value_type * string,
                                                            size_type count) noexcept {
  assert_message(index <= size(), "the insertion point must lie inside the string or at its end");
  if (index <= size())
    _replace(index, 0, string, count);

  return *this;
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::insert(size_type index, const StringType & string) noexcept {
  return insert(index, string.c_str(), string.size());
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::insert(size_type index, const StringType & string, size_type pos,
                                                            size_type count) noexcept {
  assert_message(pos <= string.size(), "the substring must start inside the source string");
  if (pos > string.size())
    return *this;

  return insert(index, string.c_str() + pos, std::min(count, string.size() - pos));
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator>
constexpr String<storageType>::iterator String<storageType>::insert(PositionIterator pos, value_type ch) noexcept {
  return insert(pos, size_type{1}, ch);
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator>
constexpr String<storageType>::iterator String<storageType>::insert(PositionIterator pos, size_type count,
                                                                    value_type ch) noexcept {
  const size_type index = _indexOf(pos);
  insert(index, count, ch);

  return begin() + index;
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator, std::input_iterator InputIterator>
  requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
constexpr String<storageType>::iterator String<storageType>::insert(PositionIterator pos, InputIterator first,
                                                                    InputIterator last) noexcept {
  const size_type index = _indexOf(pos);
  if (index <= size())
    _replaceRange(index, 0, std::move(first), std::move(last));

  return begin() + index;
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator>
constexpr String<storageType>::iterator String<storageType>::insert(PositionIterator                  pos,
                                                                    std::initializer_list<value_type> list) noexcept {
  return insert(pos, list.begin(), list.end());
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator, std::ranges::input_range Range>
  requires std::same_as<std::ranges::range_value_t<Range>, char>
constexpr String<storageType>::iterator String<storageType>::insert_range(PositionIterator pos,
                                                                          Range &&         range) noexcept {
  const size_type index = _indexOf(pos);
  if (index <= size())
    _replaceRange(index, 0, std::ranges::begin(range), std::ranges::end(range));

  return begin() + index;
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::erase(size_type index, size_type count) noexcept {
  assert_message(index <= size(), "the erased range must start inside the string or at its end");
  if (index <= size())
    _replace(index, std::min(count, size() - index), nullptr, 0);

  return *this;
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator>
constexpr String<storageType>::iterator String<storageType>::erase(PositionIterator pos) noexcept {
  const size_type index = _indexOf(pos);
  assert_message(index < size(), "the erased iterator must point at a character of the string");
  erase(index, 1);

  return begin() + index;
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator>
constexpr String<storageType>::iterator String<storageType>::erase(PositionIterator first,
                                                                   const_iterator   last) noexcept {
  const size_type index = _indexOf(first);
  const size_type end   = _indexOf(last);
  assert_message(index <= end, "the erased range must not end before it starts");
  if (index <= end && end <= size())
    erase(index, end - index);

  return begin() + index;
}

template <StringStorage storageType>
constexpr void String<storageType>::push_back(value_type ch) noexcept {
  append(size_type{1}, ch);
}

template <StringStorage storageType>
constexpr void String<storageType>::pop_back() noexcept {
  assert_message(!empty(), "the string must not be empty");
  if (!empty())
    _storage.setSize(size() - 1);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::append(size_type count, value_type ch) noexcept {
  return insert(size(), count, ch);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::append(const value_type * string, size_type count) noexcept {
  return insert(size(), string, count);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::append(const value_type * string) noexcept {
  return insert(size(), string);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::append(const StringType & string) noexcept {
  return insert(size(), string);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::append(const StringType & string, size_type pos,
                                                            size_type count) noexcept {
  return insert(size(), string, pos, count);
}

template <StringStorage storageType>
template <std::input_iterator InputIterator>
  requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
constexpr String<storageType> & String<storageType>::append(InputIterator first, InputIterator last) noexcept {
  _replaceRange(size(), 0, std::move(first), std::move(last));

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::append(std::initializer_list<value_type> list) noexcept {
  return append(list.begin(), list.size());
}

template <StringStorage storageType>
template <std::ranges::input_range Range>
  requires std::same_as<std::ranges::range_value_t<Range>, char>
constexpr String<storageType> & String<storageType>::append_range(Range && range) noexcept {
  _replaceRange(size(), 0, std::ranges::begin(range), std::ranges::end(range));

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::operator+=(value_type ch) noexcept {
  return append(size_type{1}, ch);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::operator+=(const value_type * string) noexcept {
  return append(string);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::operator+=(std::initializer_list<value_type> list) noexcept {
  return append(list);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::operator+=(const StringType & string) noexcept {
  return append(string);
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::replace(size_type pos, size_type count, const value_type * string,
                                                             size_type count2) noexcept {
  assert_message(pos <= size(), "the replaced range must start inside the string or at its end");
  if (pos <= size())
    _replace(pos, std::min(count, size() - pos), string, count2);

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::replace(size_type pos, size_type count,
                                                             const value_type * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return replace(pos, count, string, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::replace(size_type pos, size_type count,
                                                             const StringType & string) noexcept {
  return replace(pos, count, string.c_str(), string.size());
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType> & String<storageType>::replace(size_type pos, size_type count, const StringType & string,
                                                             size_type pos2, size_type count2) noexcept {
  assert_message(pos2 <= string.size(), "the substring must start inside the source string");
  if (pos2 > string.size())
    return *this;

  return replace(pos, count, string.c_str() + pos2, std::min(count2, string.size() - pos2));
}

template <StringStorage storageType>
constexpr String<storageType> & String<storageType>::replace(size_type pos, size_type count, size_type count2,
                                                             value_type ch) noexcept {
  assert_message(pos <= size(), "the replaced range must start inside the string or at its end");
  if (pos <= size() && _openGap(pos, std::min(count, size() - pos), count2))
    traits_type::assign(data() + pos, count2, ch);

  return *this;
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator>
constexpr String<storageType> & String<storageType>::replace(PositionIterator first, const_iterator last,
                                                             const value_type * string, size_type count2) noexcept {
  const size_type index = _indexOf(first);
  const size_type end   = _indexOf(last);
  assert_message(index <= end, "the replaced range must not end before it starts");
  if (index <= end && end <= size())
    replace(index, end - index, string, count2);

  return *this;
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator>
constexpr String<storageType> & String<storageType>::replace(PositionIterator first, const_iterator last,
                                                             const value_type * string) noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return replace(first, last, string, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator, StringLike StringType>
constexpr String<storageType> & String<storageType>::replace(PositionIterator first, const_iterator last,
                                                             const StringType & string) noexcept {
  return replace(first, last, string.c_str(), string.size());
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator>
constexpr String<storageType> & String<storageType>::replace(PositionIterator first, const_iterator last,
                                                             size_type count2, value_type ch) noexcept {
  const size_type index = _indexOf(first);
  const size_type end   = _indexOf(last);
  assert_message(index <= end, "the replaced range must not end before it starts");
  if (index <= end && end <= size())
    replace(index, end - index, count2, ch);

  return *this;
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator, std::input_iterator InputIterator>
  requires std::sentinel_for<InputIterator, InputIterator> && std::same_as<std::iter_value_t<InputIterator>, char>
constexpr String<storageType> & String<storageType>::replace(PositionIterator first, const_iterator last,
                                                             InputIterator first2, InputIterator last2) noexcept {
  const size_type index = _indexOf(first);
  const size_type end   = _indexOf(last);
  assert_message(index <= end, "the replaced range must not end before it starts");
  if (index <= end && end <= size())
    _replaceRange(index, end - index, std::move(first2), std::move(last2));

  return *this;
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator>
constexpr String<storageType> & String<storageType>::replace(PositionIterator first, const_iterator last,
                                                             std::initializer_list<value_type> list) noexcept {
  return replace(first, last, list.begin(), list.size());
}

template <StringStorage storageType>
template <std::convertible_to<const char *> PositionIterator, std::ranges::input_range Range>
  requires std::same_as<std::ranges::range_value_t<Range>, char>
constexpr String<storageType> & String<storageType>::replace_with_range(PositionIterator first, const_iterator last,
                                                                        Range && range) noexcept {
  const size_type index = _indexOf(first);
  const size_type end   = _indexOf(last);
  assert_message(index <= end, "the replaced range must not end before it starts");
  if (index <= end && end <= size())
    _replaceRange(index, end - index, std::ranges::begin(range), std::ranges::end(range));

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::copy(value_type * destination, size_type count,
                                                                   size_type pos) const noexcept {
  assert_message(pos <= size(), "the copied range must start inside the string or at its end");
  if (pos > size())
    return 0;

  const size_type copied = std::min(count, size() - pos);
  assert_message(destination != nullptr || copied == 0, "a null pointer has no room for the copied characters");

  // memmove behind std::char_traits::copy takes no null destination, even for an empty range.
  if (copied != 0)
    traits_type::copy(destination, data() + pos, copied);

  return copied;
}

template <StringStorage storageType>
constexpr void String<storageType>::resize(size_type count) noexcept {
  resize(count, value_type{});
}

template <StringStorage storageType>
constexpr void String<storageType>::resize(size_type count, value_type ch) noexcept {
  if (count <= size())
    erase(count);
  else
    append(count - size(), ch);
}

template <StringStorage storageType>
template <typename Operation>
  requires std::integral<std::invoke_result_t<Operation, char *, size_t>>
constexpr void String<storageType>::resize_and_overwrite(size_type count, Operation operation) noexcept {
  const bool reserved = _storage.reserve(count);
  assert_message(reserved, "the requested size must fit the storage");
  if (!reserved)
    return;

  const auto newSize = std::move(operation)(data(), count);
  const bool fits    = std::cmp_greater_equal(newSize, 0) && std::cmp_less_equal(newSize, count);
  assert_message(fits, "the operation must return a size between zero and the requested one");

  // The operation may have overwritten the old terminator; setSize() writes it again.
  _storage.setSize(fits ? static_cast<size_type>(newSize) : size());
}

template <StringStorage storageType>
constexpr void String<storageType>::swap(String & other) noexcept {
  std::ranges::swap(_storage, other._storage);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType>::size_type String<storageType>::find(const StringType & string,
                                                                   size_type          pos) const noexcept {
  return find(string.c_str(), pos, string.size());
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find(const value_type * string, size_type pos,
                                                                   size_type count) const noexcept {
  assert_message(string != nullptr || count == 0, "a null pointer names no characters to search for");

  return string_utils::find(data(), size(), string, pos, count);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find(const value_type * string,
                                                                   size_type          pos) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return find(string, pos, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find(value_type ch, size_type pos) const noexcept {
  return string_utils::find(data(), size(), ch, pos);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType>::size_type String<storageType>::rfind(const StringType & string,
                                                                    size_type          pos) const noexcept {
  return rfind(string.c_str(), pos, string.size());
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::rfind(const value_type * string, size_type pos,
                                                                    size_type count) const noexcept {
  assert_message(string != nullptr || count == 0, "a null pointer names no characters to search for");

  return string_utils::rfind(data(), size(), string, pos, count);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::rfind(const value_type * string,
                                                                    size_type          pos) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return rfind(string, pos, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::rfind(value_type ch, size_type pos) const noexcept {
  return string_utils::rfind(data(), size(), ch, pos);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType>::size_type String<storageType>::find_first_of(const StringType & string,
                                                                            size_type          pos) const noexcept {
  return find_first_of(string.c_str(), pos, string.size());
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_first_of(const value_type * string, size_type pos,
                                                                            size_type count) const noexcept {
  assert_message(string != nullptr || count == 0, "a null pointer names no characters to search for");

  return string_utils::findFirstOf(data(), size(), string, pos, count);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_first_of(const value_type * string,
                                                                            size_type          pos) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return find_first_of(string, pos, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_first_of(value_type ch,
                                                                            size_type  pos) const noexcept {
  return find(ch, pos);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType>::size_type String<storageType>::find_first_not_of(const StringType & string,
                                                                                size_type          pos) const noexcept {
  return find_first_not_of(string.c_str(), pos, string.size());
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_first_not_of(const value_type * string,
                                                                                size_type          pos,
                                                                                size_type count) const noexcept {
  assert_message(string != nullptr || count == 0, "a null pointer names no characters to search for");

  return string_utils::findFirstNotOf(data(), size(), string, pos, count);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_first_not_of(const value_type * string,
                                                                                size_type          pos) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return find_first_not_of(string, pos, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_first_not_of(value_type ch,
                                                                                size_type  pos) const noexcept {
  return string_utils::findFirstNotOf(data(), size(), ch, pos);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType>::size_type String<storageType>::find_last_of(const StringType & string,
                                                                           size_type          pos) const noexcept {
  return find_last_of(string.c_str(), pos, string.size());
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_last_of(const value_type * string, size_type pos,
                                                                           size_type count) const noexcept {
  assert_message(string != nullptr || count == 0, "a null pointer names no characters to search for");

  return string_utils::findLastOf(data(), size(), string, pos, count);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_last_of(const value_type * string,
                                                                           size_type          pos) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return find_last_of(string, pos, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_last_of(value_type ch,
                                                                           size_type  pos) const noexcept {
  return rfind(ch, pos);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr String<storageType>::size_type String<storageType>::find_last_not_of(const StringType & string,
                                                                               size_type          pos) const noexcept {
  return find_last_not_of(string.c_str(), pos, string.size());
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_last_not_of(const value_type * string, size_type pos,
                                                                               size_type count) const noexcept {
  assert_message(string != nullptr || count == 0, "a null pointer names no characters to search for");

  return string_utils::findLastNotOf(data(), size(), string, pos, count);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_last_not_of(const value_type * string,
                                                                               size_type          pos) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return find_last_not_of(string, pos, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::find_last_not_of(value_type ch,
                                                                               size_type  pos) const noexcept {
  return string_utils::findLastNotOf(data(), size(), ch, pos);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr int String<storageType>::compare(const StringType & string) const noexcept {
  return compare(0, npos, string);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr int String<storageType>::compare(size_type pos1, size_type count1, const StringType & string) const noexcept {
  return compare(pos1, count1, string.c_str(), string.size());
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr int String<storageType>::compare(size_type pos1, size_type count1, const StringType & string, size_type pos2,
                                           size_type count2) const noexcept {
  assert_message(pos2 <= string.size(), "the substring must start inside the compared string");

  const size_type offset = std::min(pos2, string.size());

  return compare(pos1, count1, string.c_str() + offset, std::min(count2, string.size() - offset));
}

template <StringStorage storageType>
constexpr int String<storageType>::compare(const value_type * string) const noexcept {
  return compare(0, npos, string);
}

template <StringStorage storageType>
constexpr int String<storageType>::compare(size_type pos1, size_type count1, const value_type * string) const noexcept {
  assert_message(string != nullptr, "C string must not be null");

  return compare(pos1, count1, string, string != nullptr ? traits_type::length(string) : 0);
}

template <StringStorage storageType>
constexpr int String<storageType>::compare(size_type pos1, size_type count1, const value_type * string,
                                           size_type count2) const noexcept {
  assert_message(pos1 <= size(), "the compared range must start inside the string or at its end");

  const size_type offset = std::min(pos1, size());

  return string_utils::compare(data() + offset, std::min(count1, size() - offset), string, count2);
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr bool String<storageType>::starts_with(const StringType & string) const noexcept {
  return compare(0, string.size(), string) == 0;
}

template <StringStorage storageType>
constexpr bool String<storageType>::starts_with(value_type ch) const noexcept {
  return !empty() && traits_type::eq(front(), ch);
}

template <StringStorage storageType>
constexpr bool String<storageType>::starts_with(const value_type * string) const noexcept {
  return starts_with(StringView(string));
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr bool String<storageType>::ends_with(const StringType & string) const noexcept {
  return size() >= string.size() && compare(size() - string.size(), string.size(), string) == 0;
}

template <StringStorage storageType>
constexpr bool String<storageType>::ends_with(value_type ch) const noexcept {
  return !empty() && traits_type::eq(back(), ch);
}

template <StringStorage storageType>
constexpr bool String<storageType>::ends_with(const value_type * string) const noexcept {
  return ends_with(StringView(string));
}

template <StringStorage storageType>
template <StringLike StringType>
constexpr bool String<storageType>::contains(const StringType & string) const noexcept {
  return find(string) != npos;
}

template <StringStorage storageType>
constexpr bool String<storageType>::contains(value_type ch) const noexcept {
  return find(ch) != npos;
}

template <StringStorage storageType>
constexpr bool String<storageType>::contains(const value_type * string) const noexcept {
  return find(string) != npos;
}

template <StringStorage storageType>
constexpr String<storageType> String<storageType>::substr(size_type pos, size_type count) const & noexcept {
  return String(*this, pos, count);
}

template <StringStorage storageType>
constexpr String<storageType> String<storageType>::substr(size_type pos, size_type count) && noexcept {
  return String(std::move(*this), pos, count);
}

template <StringStorage storageType>
constexpr StringView String<storageType>::subview(size_type pos) const noexcept {
  assert_message(pos <= size(), "the view must start inside the string or at its end");

  // StringView measures to the terminator, so a null byte inside the tail ends the view early.
  return StringView(c_str() + std::min(pos, size()));
}

template <StringStorage storageType>
constexpr bool String<storageType>::_mayOverlap(const_pointer pointer) const noexcept {
  if consteval {
    return true;
  } else {
    // Built-in < leaves the order of pointers into unrelated objects unspecified; std::less defines one.
    constexpr std::less<const_pointer> before;
    const const_pointer                first = _storage.data();
    const const_pointer                last  = first + _storage.capacity();

    return !before(pointer, first) && !before(last, pointer);
  }
}

template <StringStorage storageType>
template <std::input_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
constexpr void String<storageType>::_copyRange(InputIterator first, Sentinel last) noexcept {
  if constexpr (std::contiguous_iterator<InputIterator>) {
    const auto count = static_cast<size_t>(std::ranges::distance(first, last));

    // An empty range may hold an end iterator that must not be dereferenced.
    assign(count != 0 ? &*first : nullptr, count);
  } else {
    // Overlap is supported for contiguous ranges only; any other range must lie outside this string.
    _copyExternalRange(std::move(first), std::move(last));
  }
}

template <StringStorage storageType>
template <std::input_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
constexpr void String<storageType>::_copyExternalRange(InputIterator first, Sentinel last) noexcept {
  if constexpr (std::forward_iterator<InputIterator>) {
    const auto count    = static_cast<size_t>(std::ranges::distance(first, last));
    const bool reserved = _storage.reserve(count);
    assert_message(reserved, "the string must fit the storage it is built in");
    if (!reserved)
      return;

    if constexpr (std::contiguous_iterator<InputIterator>) {
      if (count != 0)
        traits_type::copy(_storage.data(), &*first, count);
    } else
      std::ranges::copy(std::move(first), std::move(last), _storage.data());

    _storage.setSize(count);
  } else {
    // A single pass reveals the length only at the end, so the storage is asked again each time the buffer fills.
    size_type count    = 0;
    size_type capacity = _storage.capacity();
    char *    buffer   = _storage.data();
    for (; first != last; ++first, ++count) {
      if (count == capacity) {
        // reserve() may move the buffer and keeps only the characters size() reports.
        _storage.setSize(count);

        const bool reserved = _storage.reserve(count + 1);
        assert_message(reserved, "the string must fit the storage it is built in");
        if (!reserved) {
          _storage.setSize(0);

          return;
        }

        capacity = _storage.capacity();
        buffer   = _storage.data();
      }

      buffer[count] = *first;
    }

    _storage.setSize(count);
  }
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::_indexOf(const_iterator pos) const noexcept {
  assert_message(pos >= cbegin() && pos <= cend(), "the iterator must point into the string or at its end");

  return static_cast<size_type>(pos - cbegin());
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::_offsetOf(const_pointer pointer) const noexcept {
  if consteval {
    // Equality, unlike ordering, stays a constant expression for pointers into unrelated objects.
    for (size_type index = 0; index != size(); ++index)
      if (pointer == data() + index)
        return index;

    return npos;
  } else {
    const auto buffer  = std::bit_cast<uintptr_t>(data());
    const auto address = std::bit_cast<uintptr_t>(pointer);

    return address >= buffer && address < buffer + size() ? static_cast<size_type>(address - buffer) : npos;
  }
}

template <StringStorage storageType>
constexpr bool String<storageType>::_openGap(size_type pos, size_type removed, size_type count) noexcept {
  const size_type kept     = size() - removed;
  const bool      reserved = count <= max_size() - kept && _storage.reserve(kept + count);
  assert_message(reserved, "the edited string must fit the storage");
  if (!reserved)
    return false;

  if (const size_type tail = kept - pos; tail != 0 && removed != count)
    traits_type::move(data() + pos + count, data() + pos + removed, tail);

  _storage.setSize(kept + count);

  return true;
}

template <StringStorage storageType>
constexpr void String<storageType>::_replace(size_type pos, size_type removed, const_pointer source,
                                             size_type count) noexcept {
  assert_message(source != nullptr || count == 0, "a null pointer names no characters to copy");

  // Taken before _openGap(), which may move the buffer a source inside this string points into.
  const size_type offset = count != 0 ? _offsetOf(source) : npos;
  if (offset == npos) {
    if (_openGap(pos, removed, count) && count != 0)
      traits_type::copy(data() + pos, source, count);

    return;
  }

  if (count <= removed) {
    // Shrinking keeps the buffer, so the source moves into place before the tail closes up behind it.
    traits_type::move(data() + pos, data() + offset, count);
    [[maybe_unused]] const bool closed = _openGap(pos, removed, count);

    return;
  }

  if (!_openGap(pos, removed, count))
    return;

  // Growing has shifted the old tail right, carrying along whatever part of the source lay in it.
  const size_type tailStart = pos + removed;
  const size_type growth    = count - removed;
  if (offset + count <= tailStart)
    traits_type::move(data() + pos, data() + offset, count);
  else if (offset >= tailStart)
    traits_type::copy(data() + pos, data() + offset + growth, count);
  else {
    const size_type head = tailStart - offset;
    traits_type::move(data() + pos, data() + offset, head);
    traits_type::copy(data() + pos + head, data() + pos + count, count - head);
  }
}

template <StringStorage storageType>
template <std::input_iterator InputIterator, std::sentinel_for<InputIterator> Sentinel>
constexpr void String<storageType>::_replaceRange(size_type pos, size_type removed, InputIterator first,
                                                  Sentinel last) noexcept {
  if constexpr (std::contiguous_iterator<InputIterator>) {
    const auto count = static_cast<size_type>(std::ranges::distance(first, last));

    // An empty range may hold an end iterator that must not be dereferenced.
    _replace(pos, removed, count != 0 ? &*first : nullptr, count);
  } else if constexpr (std::forward_iterator<InputIterator>) {
    // Overlap is supported for contiguous ranges only; any other range must lie outside this string.
    const auto count = static_cast<size_type>(std::ranges::distance(first, last));
    if (_openGap(pos, removed, count))
      std::ranges::copy(std::move(first), std::move(last), data() + pos);
  } else {
    // Overwriting the replaced characters first keeps the peak length at the final one, which a full buffer may hold.
    size_type overwritten = 0;
    for (; overwritten != removed && first != last; ++first, ++overwritten)
      data()[pos + overwritten] = *first;

    if (overwritten != removed) {
      erase(pos + overwritten, removed - overwritten);

      return;
    }

    // A single pass reveals the length only at the end, so the rest goes after the old ones and rotates into place.
    const size_type insertAt = pos + removed;
    const size_type oldSize  = size();
    size_type       count    = oldSize;
    size_type       capacity = _storage.capacity();
    char *          buffer   = _storage.data();
    for (; first != last; ++first, ++count) {
      if (count == capacity) {
        // reserve() may move the buffer and keeps only the characters size() reports.
        _storage.setSize(count);

        const bool reserved = _storage.reserve(count + 1);
        assert_message(reserved, "the edited string must fit the storage");
        if (!reserved) {
          _storage.setSize(oldSize);

          return;
        }

        capacity = _storage.capacity();
        buffer   = _storage.data();
      }

      buffer[count] = *first;
    }

    _storage.setSize(count);
    std::rotate(data() + insertAt, data() + oldSize, data() + count);
  }
}

template <StringStorage storageType, StringLike StringType>
constexpr String<storageType> operator+(String<storageType> lhs, const StringType & rhs) noexcept {
  lhs.append(rhs);

  return lhs;
}

template <StringStorage storageType>
constexpr String<storageType> operator+(String<storageType> lhs, const char * rhs) noexcept {
  lhs.append(rhs);

  return lhs;
}

template <StringStorage storageType>
constexpr String<storageType> operator+(String<storageType> lhs, char rhs) noexcept {
  lhs.push_back(rhs);

  return lhs;
}

template <StringStorage storageType>
constexpr String<storageType> operator+(const char * lhs, const String<storageType> & rhs) noexcept {
  String<storageType> result(lhs);
  result.append(rhs);

  return result;
}

template <StringStorage storageType>
constexpr String<storageType> operator+(char lhs, const String<storageType> & rhs) noexcept {
  String<storageType> result(1, lhs);
  result.append(rhs);

  return result;
}

template <StringStorage storageType>
constexpr String<storageType> operator+(StringView lhs, const String<storageType> & rhs) noexcept {
  String<storageType> result(lhs);
  result.append(rhs);

  return result;
}

template <StringStorage storageType, StringLike StringType>
constexpr bool operator==(const String<storageType> & lhs, const StringType & rhs) noexcept {
  return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
}

template <StringStorage storageType>
constexpr bool operator==(const String<storageType> & lhs, const char * rhs) noexcept {
  return lhs.compare(rhs) == 0;
}

template <StringStorage storageType, StringLike StringType>
constexpr std::char_traits<char>::comparison_category operator<=>(const String<storageType> & lhs,
                                                                  const StringType &          rhs) noexcept {
  return lhs.compare(rhs) <=> 0;
}

template <StringStorage storageType>
constexpr std::char_traits<char>::comparison_category operator<=>(const String<storageType> & lhs,
                                                                  const char *                rhs) noexcept {
  return lhs.compare(rhs) <=> 0;
}

template <StringStorage storageType>
constexpr void swap(String<storageType> & lhs, String<storageType> & rhs) noexcept {
  lhs.swap(rhs);
}

template <StringStorage storageType>
constexpr String<storageType>::size_type erase(String<storageType> & string, char value) noexcept {
  const auto oldSize = string.size();
  string.erase(std::remove(string.begin(), string.end(), value), string.end());

  return oldSize - string.size();
}

template <StringStorage storageType, typename Predicate>
  requires std::predicate<Predicate &, char &>
constexpr String<storageType>::size_type erase_if(String<storageType> & string, Predicate predicate) noexcept {
  const auto oldSize = string.size();
  string.erase(std::remove_if(string.begin(), string.end(), std::ref(predicate)), string.end());

  return oldSize - string.size();
}

} // namespace toy

#endif // INCLUDE_CORE_STRING_INL_
