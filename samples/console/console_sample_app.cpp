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
  \file   console_sample_app.cpp
  \brief
*/

#ifdef __GBA__

#include <cstdio>

#include <gba_console.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

int main() {
  irqInit();
  irqEnable(IRQ_VBLANK);

  consoleDemoInit();

  printf("Hello world from Console sample app");

  while (1)
    VBlankIntrWait();

  return 0;
}

#endif // __GBA__

#ifdef __NDS__

#include <cstdio>

#include <nds.h>

int main() {
  consoleDemoInit();

  printf("Hello world from Console sample app");

  while (pmMainLoop())
    swiWaitForVBlank();

  return 0;
}

#endif // __NDS__

#ifdef __3DS__

#include <cstdio>

// clang-format off
#include <3ds.h>
// clang-format on

int main() {
  gfxInitDefault();

  consoleInit(GFX_BOTTOM, NULL);

  printf("Hello world from Console sample app");

  // Main loop
  while (aptMainLoop()) {
    gfxFlushBuffers();
    gfxSwapBuffers();

    gspWaitForVBlank();
  }

  gfxExit();
  return 0;
}

#endif // __3DS__

#ifdef __SWITCH__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>

int main() {
  consoleInit(NULL);

  padConfigureInput(1, HidNpadStyleSet_NpadStandard);

  PadState pad;
  padInitializeDefault(&pad);

  printf("Hello world from Console sample app");

  while (appletMainLoop()) {
    padUpdate(&pad);

    u64 kDown = padGetButtonsDown(&pad);
    if (kDown & HidNpadButton_Plus)
      break;

    consoleUpdate(NULL);
  }

  consoleExit(NULL);
  return 0;
}

#endif // __SWITCH__

#if !defined(__GBA__) && !defined(__NDS__) && !defined(__3DS__) && !defined(__SWITCH__)

#include <iostream>

#include "toygine.hpp"

int main(int argc, char * argv[]) noexcept {
  printf("Hello world from Console sample app\n");

  return EXIT_SUCCESS;
}

#endif // __GBA__
