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
  \file   main_menu.mm
  \brief  Cocoa implementation of toy::editor::installMainMenu().

  Builds the App, File, and Edit menus with NSMenu. Placeholder items carry no action so AppKit
  auto-disables them; Edit items use standard responder-chain selectors and enable themselves once
  a responder handles them. Compiled with ARC (see CMakeLists.txt).
*/

#include "main_menu.hpp"

#import <Cocoa/Cocoa.h>

namespace toy::editor {

namespace {

// Attaches a titled submenu to the menu bar and returns it for item population.
NSMenu * addMenu(NSMenu * menuBar, NSString * title) {
  NSMenuItem * item = [[NSMenuItem alloc] initWithTitle:title action:nil keyEquivalent:@""];
  NSMenu *     menu = [[NSMenu alloc] initWithTitle:title];

  item.submenu = menu;
  [menuBar addItem:item];

  return menu;
}

void addAppMenu(NSMenu * menuBar) {
  NSMenu * menu = addMenu(menuBar, @"ToyGine2 Editor");

  [menu addItemWithTitle:@"About ToyGine2 Editor" action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];
  [menu addItem:[NSMenuItem separatorItem]];
  [menu addItemWithTitle:@"Hide ToyGine2 Editor" action:@selector(hide:) keyEquivalent:@"h"];

  NSMenuItem * hideOthers              = [menu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:)
                                                  keyEquivalent:@"h"];
  hideOthers.keyEquivalentModifierMask = NSEventModifierFlagCommand | NSEventModifierFlagOption;

  [menu addItemWithTitle:@"Show All" action:@selector(unhideAllApplications:) keyEquivalent:@""];
  [menu addItem:[NSMenuItem separatorItem]];

  // performClose: reaches the key window through the responder chain, so quitting goes through
  // Window::pumpEvents() returning false and the main loop unwinds with normal destructors.
  [menu addItemWithTitle:@"Quit ToyGine2 Editor" action:@selector(performClose:) keyEquivalent:@"q"];
}

void addFileMenu(NSMenu * menuBar) {
  NSMenu * menu = addMenu(menuBar, @"File");

  // Placeholders: no action means AppKit auto-disables them until real handlers exist.
  [menu addItemWithTitle:@"New" action:nil keyEquivalent:@"n"];
  [menu addItemWithTitle:@"Open…" action:nil keyEquivalent:@"o"];
  [menu addItem:[NSMenuItem separatorItem]];
  [menu addItemWithTitle:@"Save" action:nil keyEquivalent:@"s"];
}

void addEditMenu(NSMenu * menuBar) {
  NSMenu * menu = addMenu(menuBar, @"Edit");

  // Standard selectors resolve through the responder chain and enable automatically once text
  // fields or other editable responders appear.
  [menu addItemWithTitle:@"Undo" action:@selector(undo:) keyEquivalent:@"z"];

  NSMenuItem * redo              = [menu addItemWithTitle:@"Redo" action:@selector(redo:) keyEquivalent:@"z"];
  redo.keyEquivalentModifierMask = NSEventModifierFlagCommand | NSEventModifierFlagShift;

  [menu addItem:[NSMenuItem separatorItem]];
  [menu addItemWithTitle:@"Cut" action:@selector(cut:) keyEquivalent:@"x"];
  [menu addItemWithTitle:@"Copy" action:@selector(copy:) keyEquivalent:@"c"];
  [menu addItemWithTitle:@"Paste" action:@selector(paste:) keyEquivalent:@"v"];
  [menu addItemWithTitle:@"Select All" action:@selector(selectAll:) keyEquivalent:@"a"];
}

} // namespace

void installMainMenu() {
  @autoreleasepool {
    NSMenu * menuBar = [[NSMenu alloc] init];

    addAppMenu(menuBar);
    addFileMenu(menuBar);
    addEditMenu(menuBar);

    NSApp.mainMenu = menuBar;
  }
}

} // namespace toy::editor
