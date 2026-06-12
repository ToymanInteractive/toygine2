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
  \file   main_window.hpp
  \brief  Top-level window of the Qt-based ToyGine 2 editor.

  Defines \ref toy::editor::MainWindow, the \c QMainWindow subclass that hosts the editor UI.
*/

#ifndef EDITOR_SRC_MAIN_WINDOW_MAIN_WINDOW_HPP_
#define EDITOR_SRC_MAIN_WINDOW_MAIN_WINDOW_HPP_

#include <QMainWindow>

#include <toygine2.hpp>

class QAction;

namespace toy::editor {

/*!
  \class MainWindow
  \brief Top-level editor window.

  Composes the window title from the application version and installs a placeholder central widget. Child widgets are
  owned through Qt's parent/child mechanism.
*/
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  /// Builds the main window, deriving its title from \a version (whose \c revision string must outlive the call).
  explicit MainWindow(const application::Version & version, QWidget * parent = nullptr) noexcept;

  MainWindow(const MainWindow &)             = delete;
  MainWindow(MainWindow &&)                  = delete;
  MainWindow & operator=(const MainWindow &) = delete;
  MainWindow & operator=(MainWindow &&)      = delete;

  ~MainWindow() override = default;

private:
  /// Creates the window's actions; call before createMenus().
  void _createActions() noexcept;

  /// Builds the menu bar from the actions created by createActions().
  void _createMenus() noexcept;

  QAction * _aboutAction{nullptr};
  QAction * _aboutQtAction{nullptr};
  QAction * _donateAction{nullptr};
  QAction * _exitAction{nullptr};
  QAction * _openOnlineDocumentationAction{nullptr};
  QAction * _reportBugAction{nullptr};
};

} // namespace toy::editor

#endif // EDITOR_SRC_MAIN_WINDOW_MAIN_WINDOW_HPP_
