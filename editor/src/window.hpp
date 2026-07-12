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
  \file   window.hpp
  \brief  Native macOS main window for the editor bootstrap.

  Defines \ref toy::editor::Window: a single Cocoa window backed by a CAMetalLayer that a Vulkan
  surface can be created from. The Objective-C++ backend is hidden behind an opaque pointer so this
  header stays pure C++ and can be included from the plain C++ entry point.
*/

#ifndef EDITOR_SRC_WINDOW_HPP_
#define EDITOR_SRC_WINDOW_HPP_

namespace toy::editor {

/*!
  \class Window
  \brief Owns a single native macOS window and its CAMetalLayer.

  Wraps the Cocoa objects (NSApplication activation, NSWindow, layer-hosting NSView) required to
  present a Vulkan surface through MoltenVK. Events are pumped cooperatively so the caller keeps
  control of the main loop.
*/
class Window {
public:
  /*!
    \brief Create and show the main window, activating the application.
    \param title  Window title, UTF-8 encoded.
    \param width  Initial content width in points.
    \param height Initial content height in points.
  */
  Window(const char * title, int width, int height);

  Window(const Window &)             = delete;
  Window & operator=(const Window &) = delete;
  Window(Window &&)                  = delete;
  Window & operator=(Window &&)      = delete;

  ~Window();

  /*!
    \brief Metal layer backing the window, for Vulkan surface creation.
    \return Opaque pointer to the CAMetalLayer, usable as \c VkMetalSurfaceCreateInfoEXT::pLayer.
    \note The pointer stays valid for the lifetime of this window.
  */
  [[nodiscard]] void * metalLayer() const noexcept;

  /*!
    \brief Drain pending native events without blocking.
    \return \c true while the window remains open, \c false once the user has closed it.
  */
  [[nodiscard]] bool pumpEvents() noexcept;

private:
  struct Impl;
  Impl * _impl;
};

} // namespace toy::editor

#endif // EDITOR_SRC_WINDOW_HPP_
