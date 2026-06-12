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
  \file   main_window.cpp
  \brief  Implementation of \ref toy::editor::MainWindow.
*/

#include "main_window.hpp"

#include <QAction>
#include <QDesktopServices>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QUrl>

namespace toy::editor {

namespace {

constexpr auto c_documentationUrl = QLatin1StringView("https://toymaninteractive.github.io/toygine2");
constexpr auto c_reportBugUrl     = QLatin1StringView("https://github.com/ToymanInteractive/toygine2/issues");
constexpr auto c_donateUrl        = QLatin1StringView("https://github.com/sponsors/ToymanInteractive");

} // namespace

MainWindow::MainWindow(const application::Version & version, QWidget * parent)
  : QMainWindow(parent) {
  setWindowTitle(tr("ToyGine 2 Editor %1.%2.%3 (%4)")
                   .arg(version.major)
                   .arg(version.minor)
                   .arg(version.patch)
                   .arg(version.revision.c_str()));

  resize(800, 600);

  auto * placeholder = new QLabel(QStringLiteral("Hello, ToyGine 2 Editor"));
  placeholder->setAlignment(Qt::AlignCenter);
  setCentralWidget(placeholder);

  createActions();
  createMenus();
}

void MainWindow::createActions() {
  _aboutAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout), tr("&About"), this);
  _aboutAction->setMenuRole(QAction::AboutRole);
  connect(_aboutQtAction, &QAction::triggered, this, [this] {
    QMessageBox::about(this, tr("About"), tr("My App v1.0\n..."));
  });

  _aboutQtAction = new QAction(tr("About &Qt"), this);
  _aboutQtAction->setMenuRole(QAction::AboutQtRole);
  connect(_aboutQtAction, &QAction::triggered, this, [this] {
    QMessageBox::aboutQt(this, tr("About Qt!"));
  });

  _donateAction = new QAction(tr("&Support ToyGine 2 Development"), this);
  connect(_donateAction, &QAction::triggered, this, [] {
    QDesktopServices::openUrl(QUrl(c_donateUrl));
  });

  _exitAction = new QAction(tr("Exit"), this);
  _exitAction->setShortcuts(QKeySequence::Quit);
  _exitAction->setStatusTip(tr("Exit the editor"));

  // On macOS this role moves the item into the application menu (ToyGine 2 Editor > Quit, Cmd+Q); on Windows/Linux it
  // stays in the File menu. Set explicitly so the placement is intentional rather than guessed from the action text.
  _exitAction->setMenuRole(QAction::QuitRole);

  connect(_exitAction, &QAction::triggered, this, &QWidget::close);

  _openOnlineDocumentationAction = new QAction(tr("Online Documentation"), this);
  connect(_openOnlineDocumentationAction, &QAction::triggered, this, [] {
    QDesktopServices::openUrl(QUrl(c_documentationUrl));
  });

  _reportBugAction = new QAction(tr("Report a Bug"), this);
  connect(_reportBugAction, &QAction::triggered, this, [] {
    QDesktopServices::openUrl(QUrl(c_reportBugUrl));
  });
}

void MainWindow::createMenus() {
  menuBar()->clear();

  auto * fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(_exitAction);

  menuBar()->addMenu(tr("&Project"));

  auto * helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(_openOnlineDocumentationAction);
  helpMenu->addSeparator();
  helpMenu->addAction(_reportBugAction);
  helpMenu->addSeparator();
  helpMenu->addAction(_donateAction);
  helpMenu->addSeparator();
  helpMenu->addAction(_aboutQtAction);
  helpMenu->addAction(_aboutAction);
}

} // namespace toy::editor
