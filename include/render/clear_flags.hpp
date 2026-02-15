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
  \file   clear_flags.hpp
  \brief  Bit flags for framebuffer clear operations (color, depth, stencil).
*/

#ifndef INCLUDE_RENDER_CLEAR_FLAGS_HPP_
#define INCLUDE_RENDER_CLEAR_FLAGS_HPP_

namespace toy::render {

/*!
  \enum ClearFlags
  \brief Bit flags specifying which framebuffer attachments to clear.

  Values can be combined with \c operator|, \c operator&, and \c operator^ for bitwise operations.
*/
enum class ClearFlags : unsigned int {
  /// Clear nothing.
  None = 0x00,

  /// Clear the color/diffuse target (back buffer or render target).
  Target = 0x01,
  /// Clear the depth buffer.
  Depth = 0x02,
  /// Clear the stencil buffer.
  Stencil = 0x04,

  /// Clear color, depth, and stencil.
  All = Target | Depth | Stencil
};

/*!
  \brief Bitwise AND of two \ref toy::render::ClearFlags values.

  Returns the intersection of the bit flags in \a lhs and \a rhs. Use to test whether specific clear targets are set
  (e.g. \a flags & \ref toy::render::ClearFlags::Depth).

  \param lhs The first clear flags.
  \param rhs The second clear flags.

  \return Clear flags with only the bits set in both \a lhs and \a rhs.
*/
[[nodiscard]] constexpr ClearFlags operator&(ClearFlags lhs, ClearFlags rhs) noexcept {
  return static_cast<ClearFlags>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
}

/*!
  \brief Bitwise OR of two \ref toy::render::ClearFlags values.

  Returns the union of the bit flags in \a lhs and \a rhs. Use to combine clear targets
  (e.g. \ref toy::render::ClearFlags::Target | \ref toy::render::ClearFlags::Depth).

  \param lhs The first clear flags.
  \param rhs The second clear flags.

  \return Clear flags with all bits set in either \a lhs or \a rhs.
*/
[[nodiscard]] constexpr ClearFlags operator|(ClearFlags lhs, ClearFlags rhs) noexcept {
  return static_cast<ClearFlags>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
}

/*!
  \brief Bitwise XOR of two \ref toy::render::ClearFlags values.

  Returns the symmetric difference of the bit flags in \a lhs and \a rhs.

  \param lhs The first clear flags.
  \param rhs The second clear flags.

  \return Clear flags with bits set in exactly one of \a lhs or \a rhs.
*/
[[nodiscard]] constexpr ClearFlags operator^(ClearFlags lhs, ClearFlags rhs) noexcept {
  return static_cast<ClearFlags>(static_cast<unsigned int>(lhs) ^ static_cast<unsigned int>(rhs));
}

} // namespace toy::render

#endif // INCLUDE_RENDER_CLEAR_FLAGS_HPP_
