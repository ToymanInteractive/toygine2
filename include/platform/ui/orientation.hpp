//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \file   orientation.hpp
  \brief  Screen orientation enumeration for UI layout and rotation handling.
*/

#ifndef INCLUDE_PLATFORM_UI_ORIENTATION_HPP_
#define INCLUDE_PLATFORM_UI_ORIENTATION_HPP_

namespace toy::platform::ui {

/*!
  \enum Orientation
  \brief Screen orientation for UI layout and device rotation.

  Values use bit flags so they can be combined (e.g. to accept both landscape variants).
  Use \ref toy::platform::ui::Orientation::LandscapeAny or \ref toy::platform::ui::Orientation::PortraitAny when the
  rotation direction does not matter.
*/
enum class Orientation : unsigned int {
  /// Landscape: width greater than height (normal).
  Landscape = 1,
  /// Portrait: height greater than width (normal).
  Portrait = 2,
  /// Landscape rotated 180 degrees.
  LandscapeFlipped = 4,
  /// Portrait rotated 180 degrees.
  PortraitFlipped = 8,

  /// Either landscape variant.
  LandscapeAny = Landscape | LandscapeFlipped,
  /// Either portrait variant.
  PortraitAny = Portrait | PortraitFlipped,

  /// Any orientation.
  Any = LandscapeAny | PortraitAny
};

/*!
  \brief Bitwise AND of two \ref toy::platform::ui::Orientation values.

  Returns the intersection of the bit flags in \a lhs and \a rhs.

  \param lhs The first orientation.
  \param rhs The second orientation.

  \return An orientation with only the bits set in both \a lhs and \a rhs.
*/
[[nodiscard]] constexpr Orientation operator&(Orientation lhs, Orientation rhs) noexcept {
  return static_cast<Orientation>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
}

/*!
  \brief Bitwise OR of two \ref toy::platform::ui::Orientation values.

  Returns the union of the bit flags in \a lhs and \a rhs. Use to combine orientations
  (e.g. \ref toy::platform::ui::Orientation::Landscape | \ref toy::platform::ui::Orientation::LandscapeFlipped).

  \param lhs The first orientation.
  \param rhs The second orientation.

  \return An orientation with all bits set in either \a lhs or \a rhs.
*/
[[nodiscard]] constexpr Orientation operator|(Orientation lhs, Orientation rhs) noexcept {
  return static_cast<Orientation>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
}

/*!
  \brief Bitwise XOR of two \ref toy::platform::ui::Orientation values.

  Returns the symmetric difference of the bit flags in \a lhs and \a rhs.

  \param lhs The first orientation.
  \param rhs The second orientation.

  \return An orientation with bits set in exactly one of \a lhs or \a rhs.
*/
[[nodiscard]] constexpr Orientation operator^(Orientation lhs, Orientation rhs) noexcept {
  return static_cast<Orientation>(static_cast<unsigned int>(lhs) ^ static_cast<unsigned int>(rhs));
}

} // namespace toy::platform::ui

#endif // INCLUDE_PLATFORM_UI_ORIENTATION_HPP_
