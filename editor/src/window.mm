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
  \file   window.mm
  \brief  Cocoa implementation of \ref toy::editor::Window.

  Creates the NSApplication, a single NSWindow with a layer-hosting NSView whose backing layer is a
  CAMetalLayer, and pumps native events cooperatively. Compiled with ARC (see CMakeLists.txt).
*/

#include "window.hpp"

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

//----------------------------------------------------------------------------------------------------------------------

// Tracks the user closing the window so pumpEvents() can report it back to the main loop.
@interface ToyWindowDelegate : NSObject <NSWindowDelegate>

@property (nonatomic) BOOL closed;
@end

@implementation ToyWindowDelegate

- (void)windowWillClose:(NSNotification *)notification {
  (void)notification;
  self.closed = YES;
}

@end

//----------------------------------------------------------------------------------------------------------------------

namespace toy::editor {

struct Window::Impl {
  NSWindow *          window   = nil;
  NSView *            view     = nil;
  CAMetalLayer *      layer    = nil;
  ToyWindowDelegate * delegate = nil;
};

Window::Window(const char * title, int width, int height)
  : _impl(new Impl) {
  @autoreleasepool {
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    const NSRect contentRect      = NSMakeRect(0.0, 0.0, static_cast<CGFloat>(width), static_cast<CGFloat>(height));
    const NSWindowStyleMask style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable
                                    | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

    _impl->window = [[NSWindow alloc] initWithContentRect:contentRect styleMask:style backing:NSBackingStoreBuffered
                                                    defer:NO];

    _impl->layer               = [CAMetalLayer layer];
    _impl->layer.contentsScale = _impl->window.backingScaleFactor;

    // Layer-hosting view: MoltenVK renders into this CAMetalLayer via the Vulkan surface.
    _impl->view            = [[NSView alloc] initWithFrame:contentRect];
    _impl->view.wantsLayer = YES;
    _impl->view.layer      = _impl->layer;

    _impl->delegate = [[ToyWindowDelegate alloc] init];

    _impl->window.contentView = _impl->view;
    _impl->window.delegate    = _impl->delegate;
    _impl->window.title       = [NSString stringWithUTF8String:(title != nullptr ? title : "")];
    [_impl->window center];

    [_impl->window makeKeyAndOrderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];
  }
}

Window::~Window() {
  delete _impl;
}

void * Window::metalLayer() const noexcept {
  return (__bridge void *)_impl->layer;
}

bool Window::pumpEvents() noexcept {
  @autoreleasepool {
    for (;;) {
      NSEvent * event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:[NSDate distantPast]
                                              inMode:NSDefaultRunLoopMode
                                             dequeue:YES];
      if (event == nil)
        break;

      [NSApp sendEvent:event];
    }
  }

  return !_impl->delegate.closed;
}

} // namespace toy::editor
