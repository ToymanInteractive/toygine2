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
  \brief  Inline implementations for \ref toy::String constructors, assignment, and accessors.

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
  : String(string.c_str() + min(pos, string.size()), min(count, string.size() - min(pos, string.size()))) {
  assert_message(pos <= string.size(), "the substring must start inside the source string");
}

template <StringStorage storageType>
constexpr String<storageType>::String(String && other, size_type pos, size_type count) noexcept
  : _storage(std::move(other._storage)) {
  const size_type sourceLength = _storage.size();
  assert_message(pos <= sourceLength, "the substring must start inside the source string");

  const size_type offset = min(pos, sourceLength);
  const size_type length = min(count, sourceLength - offset);
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

  return assign(string.c_str() + pos, min(count, string.size() - pos));
}

template <StringStorage storageType>
template <std::ranges::input_range Range>
  requires std::same_as<std::ranges::range_value_t<Range>, char>
constexpr String<storageType> & String<storageType>::assign_range(Range && range) noexcept {
  _copyRange(std::ranges::begin(range), std::ranges::end(range));

  return *this;
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::size() const noexcept {
  return _storage.size();
}

template <StringStorage storageType>
constexpr String<storageType>::const_pointer String<storageType>::c_str() const noexcept {
  return _storage.data();
}

template <StringStorage storageType>
constexpr bool String<storageType>::_mayOverlap(const_pointer pointer) const noexcept {
  if consteval {
    return true;
  } else {
    // std::less would order unrelated pointers as well, but lives in <functional>, which the barrel does not include.
    const auto buffer  = std::bit_cast<uintptr_t>(_storage.data());
    const auto address = std::bit_cast<uintptr_t>(pointer);

    return address >= buffer && address <= buffer + _storage.capacity();
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

} // namespace toy

#endif // INCLUDE_CORE_STRING_INL_
