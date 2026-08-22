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
  \file   toy_test.inl
  \brief  Inline implementations for \ref toy::test::Approx and the runner's name and number helpers.

  \note Included by toy_test.hpp only; do not include this file directly.
*/

namespace toy::test {

template <std::floating_point T>
constexpr Approx<T>::Approx(T value) noexcept
  : _value{value}
  , _epsilon{static_cast<T>(std::numeric_limits<float>::epsilon() * 100.0)} {}

template <std::floating_point T>
template <std::floating_point U>
constexpr Approx<T> Approx<T>::epsilon(U value) const noexcept {
  Approx result{_value};
  result._epsilon = static_cast<T>(value);

  return result;
}

template <std::floating_point T>
constexpr T Approx<T>::value() const noexcept {
  return _value;
}

template <std::floating_point T>
constexpr T Approx<T>::tolerance() const noexcept {
  return _epsilon;
}

template <std::floating_point T, std::floating_point U>
constexpr bool operator==(const Approx<T> & lhs, U rhs) noexcept {
  // Widening both operands first keeps a mixed-precision comparison from narrowing either side.
  using common_type = std::common_type_t<T, U>;

  const common_type expected   = static_cast<common_type>(lhs.value());
  const common_type actual     = static_cast<common_type>(rhs);
  const common_type difference = detail::absoluteValue(actual - expected);
  const common_type left       = detail::absoluteValue(actual);
  const common_type right      = detail::absoluteValue(expected);
  const common_type larger     = left > right ? left : right;

  // Reproduces doctest 2.5.3 exactly, down to the strict comparison: difference < epsilon * (1 + larger).
  // The added one keeps values near zero from demanding exact equality, and the larger operand carries the
  // scaling across magnitudes. Any departure here makes one test disagree with itself under the two runners.
  return difference < static_cast<common_type>(lhs.tolerance()) * (common_type{1} + larger);
}

namespace detail {

template <std::floating_point T>
constexpr T absoluteValue(T value) noexcept {
  return value < T{0} ? -value : value;
}

constexpr int compareNames(const char * lhs, const char * rhs) noexcept {
  while (*lhs != '\0' && *lhs == *rhs) {
    ++lhs;
    ++rhs;
  }

  const auto left  = static_cast<unsigned char>(*lhs);
  const auto right = static_cast<unsigned char>(*rhs);

  if (left < right)
    return -1;

  return left > right ? 1 : 0;
}

constexpr std::size_t appendText(char * buffer, std::size_t capacity, std::size_t offset, const char * text) noexcept {
  while (*text != '\0' && offset < capacity) {
    buffer[offset] = *text;
    ++offset;
    ++text;
  }

  return offset;
}

constexpr std::size_t appendInteger(char * buffer, std::size_t capacity, std::size_t offset, long long value) noexcept {
  // Taking the magnitude in unsigned arithmetic keeps the most negative value representable.
  unsigned long long magnitude = value < 0 ? 0ULL - static_cast<unsigned long long>(value)
                                           : static_cast<unsigned long long>(value);

  std::array<char, 20> digits     = {};
  std::size_t          digitCount = 0;

  do {
    digits[digitCount] = static_cast<char>('0' + static_cast<char>(magnitude % 10ULL));
    ++digitCount;
    magnitude /= 10ULL;
  } while (magnitude != 0ULL);

  if (value < 0 && offset < capacity) {
    buffer[offset] = '-';
    ++offset;
  }

  while (digitCount > 0 && offset < capacity) {
    --digitCount;
    buffer[offset] = digits[digitCount];
    ++offset;
  }

  return offset;
}

} // namespace detail

inline Context::Context(failure_reporter_type reporter) noexcept
  : _reporter{reporter}
  , _caseName{nullptr}
  , _subcaseName{nullptr}
  , _passedCount{0}
  , _failedCount{0}
  , _infoDepth{0}
  , _caseFailed{false}
  , _infoStack{} {}

inline void Context::beginCase(const char * name) noexcept {
  _caseName    = name;
  _subcaseName = nullptr;
  _caseFailed  = false;
  _infoDepth   = 0;
}

inline bool Context::record(bool passed, const char * expression, const char * file, int line) noexcept {
  if (passed) {
    ++_passedCount;

    return true;
  }

  ++_failedCount;
  _caseFailed = true;

  if (_reporter != nullptr) {
    const FailureRecord failure{_caseName, _subcaseName, expression, file, line};
    _reporter(*this, failure);
  }

  return false;
}

inline void Context::pushInfo(const char * text) noexcept {
  if (_infoDepth < c_maxInfoDepth)
    _infoStack[_infoDepth] = InfoEntry{text, 0, false};

  ++_infoDepth;
}

inline void Context::pushInfo(const char * text, long long value) noexcept {
  if (_infoDepth < c_maxInfoDepth)
    _infoStack[_infoDepth] = InfoEntry{text, value, true};

  ++_infoDepth;
}

inline void Context::popInfo() noexcept {
  if (_infoDepth > 0)
    --_infoDepth;
}

inline std::size_t Context::passedCount() const noexcept {
  return _passedCount;
}

inline std::size_t Context::failedCount() const noexcept {
  return _failedCount;
}

inline bool Context::caseFailed() const noexcept {
  return _caseFailed;
}

inline std::size_t Context::infoCount() const noexcept {
  // The depth counter keeps growing past the fixed stack so that pushes and pops stay balanced; what is
  // readable is capped at the stack itself.
  return _infoDepth < c_maxInfoDepth ? _infoDepth : c_maxInfoDepth;
}

inline const InfoEntry & Context::infoAt(std::size_t index) const noexcept {
  return _infoStack[index];
}

inline const char * Context::caseName() const noexcept {
  return _caseName;
}

inline const char * Context::subcaseName() const noexcept {
  return _subcaseName;
}

} // namespace toy::test
