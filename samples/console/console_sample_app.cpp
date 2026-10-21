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

#include "toygine.hpp"

#if !defined(__APPLE__)
class SampleApp final : public toy::application::ConsoleApplication {
public:
  constexpr explicit SampleApp(const toy::application::Version & version) noexcept
    : ConsoleApplication(version) {}

private:
  bool runInternal() noexcept override {
    return true;
  }
};

constexpr SampleApp app{
  toy::application::Version{26, 20, 0, "f7e57d4"},
};

#endif

#ifdef __MEGA_DRIVE__

#include <clownmdsdk.h>

void _EntryPoint() {
  // Do initialisation here.

  ClownMDSDK::MainCPU::Debug::PrintLine("Hello world from Console sample app");

  for (;;) {
    // Run main-loop logic here.
  }
}

#endif // __MEGA_DRIVE__

#ifdef __GBA__

#include <cstdio>

#include <gba_console.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

int main() {
  irqInit();
  irqEnable(IRQ_VBLANK);

  consoleDemoInit();

  printf("Hello world from Console sample app\n");
  printf("v%lu.%lu.%lu (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");
  if (app.pid() != 0)
    printf("Process ID: %lu\n", app.pid());

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

  printf("Hello world from Console sample app\n");
  printf("v%lu.%lu.%lu (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");
  if (app.pid() != 0)
    printf("Process ID: %lu\n", app.pid());

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

  printf("Hello world from Console sample app\n");
  printf("v%lu.%lu.%lu (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");
  if (app.pid() != 0)
    printf("Process ID: %lu\n", app.pid());

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

  printf("Hello world from Console sample app\n");
  printf("v%u.%u.%u (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");
  if (app.pid() != 0)
    printf("Process ID: %u\n", app.pid());

  while (appletMainLoop()) {
    padUpdate(&pad);

    if (padGetButtonsDown(&pad) & HidNpadButton_Plus)
      break;

    consoleUpdate(NULL);
  }

  consoleExit(NULL);
  return 0;
}

#endif // __SWITCH__

#ifdef __gamecube__

#include <cstdio>

#include <gccore.h>

int main() {
  VIDEO_Init();

  auto * rmode = VIDEO_GetPreferredMode(NULL);

  auto * framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
  console_init(framebuffer, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

  VIDEO_Configure(rmode);
  VIDEO_SetNextFramebuffer(framebuffer);
  VIDEO_SetBlack(false);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  if (rmode->viTVMode & VI_NON_INTERLACE)
    VIDEO_WaitVSync();

  printf("Hello world from Console sample app\n");
  printf("v%u.%u.%u (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");
  if (app.pid() != 0)
    printf("Process ID: %u\n", app.pid());

  while (SYS_MainLoop())
    VIDEO_WaitVSync();

  return 0;
}

#endif // __gamecube__

#ifdef __wii__

#include <cstdio>

#include <gccore.h>

int main() {
  VIDEO_Init();

  auto * rmode = VIDEO_GetPreferredMode(NULL);

  auto * framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
  console_init(framebuffer, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

  VIDEO_Configure(rmode);
  VIDEO_SetNextFramebuffer(framebuffer);
  VIDEO_SetBlack(false);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  if (rmode->viTVMode & VI_NON_INTERLACE)
    VIDEO_WaitVSync();

  printf("Hello world from Console sample app\n");
  printf("v%u.%u.%u (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");
  if (app.pid() != 0)
    printf("Process ID: %u\n", app.pid());

  while (SYS_MainLoop())
    VIDEO_WaitVSync();

  return 0;
}

#endif // __wii__

#ifdef __WIIU__

#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

int main(int argc, char ** argv) {
  WHBProcInit();
  WHBLogConsoleInit();

  WHBLogPrintf("Hello world from Console sample app\n");
  WHBLogPrintf("v%u.%u.%u (%s)\n", app.version().major, app.version().minor, app.version().patch,
               app.version().revision.c_str());
  WHBLogPrintf("\n");
  if (app.pid() != 0)
    WHBLogPrintf("Process ID: %u\n", app.pid());

  while (WHBProcIsRunning()) {
    WHBLogConsoleDraw();
    OSSleepTicks(OSMillisecondsToTicks(100));
  }

  WHBLogPrintf("Exiting... good bye.");
  WHBLogConsoleDraw();
  OSSleepTicks(OSMillisecondsToTicks(1000));

  WHBLogConsoleFree();
  WHBProcShutdown();

  return 0;
}

#endif // __WIIU__

#ifdef __APPLE__

#include <iostream>

class ConsoleSampleApp final : public toy::application::ConsoleApplication {
public:
  constexpr explicit ConsoleSampleApp(const toy::application::Version & version) noexcept
    : ConsoleApplication(version) {}

private:
  bool runInternal() noexcept override;
};

bool ConsoleSampleApp::runInternal() noexcept {
  writeOutputLine(toy::StringView{"Hello world from Console sample app"});

  printf("v%i.%i.%i (%s)\n", version().major, version().minor, version().patch, version().revision.c_str());
  writeOutputLine(toy::StringView{""});
  if (pid() != 0)
    printf("Process ID: %u\n", pid());

  if (argsCount() > 1) {
    writeOutputLine(toy::StringView{""});
    writeOutputLine(toy::StringView{"arguments:"});
    for (size_t index = 1; index < argsCount(); ++index)
      printf("- %s\n", arg(index));
  }

  return true;
}

IMPLEMENT_CONSOLE_APP(ConsoleSampleApp)

#endif // __APPLE__

#if !defined(__MEGA_DRIVE__) && !defined(__GBA__) && !defined(__NDS__) && !defined(__3DS__) && !defined(__SWITCH__)    \
  && !defined(__gamecube__) && !defined(__wii__) && !defined(__WIIU__) && !defined(__APPLE__)

#include <iostream>

int main() noexcept {
  toy::application::ConsoleApplication::writeOutputLine(toy::StringView{"Hello world from Console sample app"});

  printf("v%i.%i.%i (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  toy::application::ConsoleApplication::writeOutputLine(toy::StringView{""});
  if (app.pid() != 0)
    printf("Process ID: %u\n", app.pid());

  return EXIT_SUCCESS;
}

#endif
