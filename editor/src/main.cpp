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
  \file   main.cpp
  \brief  Entry point of the Qt-based ToyGine 2 editor.

  Bootstraps the Qt application object and shows a placeholder main window. The viewport, docking panels, and the
  engine bridge are added in later phases; this stage only validates the Qt delivery and build wiring.
*/

#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QObject>

#include "../../include/toygine2.hpp"

int main(int argc, char ** argv) {
  constexpr auto version = toy::application::Version{.major    = TOY_EDITOR_VERSION_MAJOR,
                                                     .minor    = TOY_EDITOR_VERSION_MINOR,
                                                     .patch    = TOY_EDITOR_VERSION_PATCH,
                                                     .revision = TOY_EDITOR_VERSION_REVISION};

  QApplication app(argc, argv);
  QApplication::setApplicationName(QStringLiteral("ToyGine 2 Editor"));

  QMainWindow window;
  window.setWindowTitle(QString("ToyGine 2 Editor %1.%2.%3 (%4)")
                          .arg(version.major)
                          .arg(version.minor)
                          .arg(version.patch)
                          .arg(version.revision.c_str()));

  auto * placeholder = new QLabel(QStringLiteral("Hello, ToyGine 2 Editor"));
  placeholder->setAlignment(Qt::AlignCenter);
  window.setCentralWidget(placeholder);

  window.resize(800, 600);
  window.show();

  return QApplication::exec();
}
