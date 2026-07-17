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
  \file   main_menu.hpp
  \brief  Native macOS menu bar installation for the editor.

  Declares toy::editor::installMainMenu(): builds the application menu bar (App, File, Edit) and
  attaches it to the shared NSApplication. The Objective-C++ backend lives in main_menu.mm so this
  header stays pure C++ and can be included from the plain C++ entry point.
*/

#ifndef EDITOR_SRC_MAIN_MENU_HPP_
#define EDITOR_SRC_MAIN_MENU_HPP_

namespace toy::editor {

/*!
  \brief Build and install the editor menu bar on the shared application.

  Installs three menus: the application menu (About, Hide, Quit), File (New / Open / Save
  placeholders, disabled until wired to actions), and Edit (standard responder-chain actions).
  Quit closes the key window instead of terminating the process, so the main loop exits through
  Window::pumpEvents() and stack objects are destroyed normally.

  \pre NSApplication has been created (a \ref toy::editor::Window exists).
  \post The shared application's main menu is replaced with the editor menu bar.

  \sa \ref toy::editor::Window
*/
void installMainMenu();

} // namespace toy::editor

#endif // EDITOR_SRC_MAIN_MENU_HPP_
