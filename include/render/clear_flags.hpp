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
enum class ClearFlags : uint8_t {
  /// Clear nothing.
  None = 0x00,

  /// Clear the color/diffuse target (back buffer or render target).
  Target = 0x01,
  /// Clear the depth buffer.
  Depth = 0x02,
  /// Clear the stencil buffer.
  Stencil = 0x04,

  /// Clear color, depth, and stencil.
  All = Target | Depth | Stencil,
};

} // namespace toy::render

ENABLE_BITWISE_OPERATORS(toy::render::ClearFlags)

#endif // INCLUDE_RENDER_CLEAR_FLAGS_HPP_
