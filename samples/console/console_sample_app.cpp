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
  \brief  Console sample: per-platform hello world and a chrono demo.

  Each target branch prints its greeting and version, reads the wall clock from toy::chrono::SystemClock, the steady
  counter's frequency and tick count from toy::chrono::SteadyClock, and the output span its toy::chrono::Stopwatch
  covered. 64-bit readings go through toy::string_utils::itoa: console libcs disagree on printf length modifiers.
*/

#include "toygine.hpp"

#if !defined(__linux__) && !defined(__APPLE__) && !defined(_WIN32) && !defined(__GBA__) && !defined(__gamecube__)
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
  ClownMDSDK::MainCPU::Debug::PrintLine("Hello world from Console sample app");

  for (;;) {
    // Run main-loop logic here.
  }
}

#endif // __MEGA_DRIVE__

#ifdef __NDS__

#include <cstdio>

#include <nds.h>

int main() {
  consoleDemoInit();

  const toy::chrono::Stopwatch watch;

  printf("Hello world from Console sample app\n");
  printf("\n");
  printf("App Version: %lu.%lu.%lu (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");

  if (app.pid() != 0)
    printf("System pid: %lu\n", app.pid());

  const auto calendar = toy::chrono::SystemClock::now();
  printf("System clock: %04d-%02d-%02d %02d:%02d:%02d.%03d dow: %d tz: utc%+d min\n", calendar.year, calendar.month,
         calendar.day, calendar.hour, calendar.minute, calendar.second, calendar.millisecond, calendar.dayOfWeek,
         calendar.tzOffsetMinutes);

  toy::array<char, 24> buffer{};
  printf("Steady clock frequency: %s Hz\n",
         toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::frequency()));
  printf("Steady clock ticks: %s\n", toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::nowTicks()));
  printf("Steady clock elapsed: %s ns\n", toy::string_utils::itoa(toy::span{buffer}, watch.elapsed().count()));

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

  const toy::chrono::Stopwatch watch;

  printf("Hello world from Console sample app\n");
  printf("\n");
  printf("App Version: %lu.%lu.%lu (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");

  if (app.pid() != 0)
    printf("System pid: %lu\n", app.pid());

  const auto calendar = toy::chrono::SystemClock::now();
  printf("System clock: %04d-%02d-%02d %02d:%02d:%02d.%03d dow: %d tz: utc%+d min\n", calendar.year, calendar.month,
         calendar.day, calendar.hour, calendar.minute, calendar.second, calendar.millisecond, calendar.dayOfWeek,
         calendar.tzOffsetMinutes);

  toy::array<char, 24> buffer{};
  printf("Steady clock frequency: %s Hz\n",
         toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::frequency()));
  printf("Steady clock ticks: %s\n", toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::nowTicks()));
  printf("Steady clock elapsed: %s ns\n", toy::string_utils::itoa(toy::span{buffer}, watch.elapsed().count()));

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

  const toy::chrono::Stopwatch watch;

  printf("Hello world from Console sample app\n");
  printf("\n");
  printf("App Version: %u.%u.%u (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");

  if (app.pid() != 0)
    printf("System pid: %u\n", app.pid());

  const auto calendar = toy::chrono::SystemClock::now();
  printf("System clock: %04d-%02d-%02d %02d:%02d:%02d.%03d dow: %d tz: utc%+d min\n", calendar.year, calendar.month,
         calendar.day, calendar.hour, calendar.minute, calendar.second, calendar.millisecond, calendar.dayOfWeek,
         calendar.tzOffsetMinutes);

  toy::array<char, 24> buffer{};
  printf("Steady clock frequency: %s Hz\n",
         toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::frequency()));
  printf("Steady clock ticks: %s\n", toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::nowTicks()));
  printf("Steady clock elapsed: %s ns\n", toy::string_utils::itoa(toy::span{buffer}, watch.elapsed().count()));

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

  const toy::chrono::Stopwatch watch;

  printf("Hello world from Console sample app\n");
  printf("\n");
  printf("App Version: %u.%u.%u (%s)\n", app.version().major, app.version().minor, app.version().patch,
         app.version().revision.c_str());
  printf("\n");
  if (app.pid() != 0)
    printf("System pid: %u\n", app.pid());

  const auto calendar = toy::chrono::SystemClock::now();
  printf("System clock: %04d-%02d-%02d %02d:%02d:%02d.%03d dow: %d tz: utc%+d min\n", calendar.year, calendar.month,
         calendar.day, calendar.hour, calendar.minute, calendar.second, calendar.millisecond, calendar.dayOfWeek,
         calendar.tzOffsetMinutes);

  toy::array<char, 24> buffer{};
  printf("Steady clock frequency: %s Hz\n",
         toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::frequency()));
  printf("Steady clock ticks: %s\n", toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::nowTicks()));
  printf("Steady clock elapsed: %s ns\n", toy::string_utils::itoa(toy::span{buffer}, watch.elapsed().count()));

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

  const toy::chrono::Stopwatch watch;

  WHBLogPrintf("Hello world from Console sample app\n");
  WHBLogPrintf("\n");
  WHBLogPrintf("App Version: %u.%u.%u (%s)\n", app.version().major, app.version().minor, app.version().patch,
               app.version().revision.c_str());
  WHBLogPrintf("\n");
  if (app.pid() != 0)
    WHBLogPrintf("System pid: %u\n", app.pid());

  const auto calendar = toy::chrono::SystemClock::now();
  WHBLogPrintf("System clock: %04d-%02d-%02d %02d:%02d:%02d.%03d dow: %d tz: utc%+d min\n", calendar.year,
               calendar.month, calendar.day, calendar.hour, calendar.minute, calendar.second, calendar.millisecond,
               calendar.dayOfWeek, calendar.tzOffsetMinutes);

  toy::array<char, 24> buffer{};
  WHBLogPrintf("Steady clock frequency: %s Hz\n",
               toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::frequency()));
  WHBLogPrintf("Steady clock ticks: %s\n",
               toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::nowTicks()));
  WHBLogPrintf("Steady clock elapsed: %s ns\n", toy::string_utils::itoa(toy::span{buffer}, watch.elapsed().count()));

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

#if defined(__linux__) || defined(__APPLE__) || defined(_WIN32) || defined(__GBA__) || defined(__gamecube__)

class ConsoleSampleApp final : public toy::application::ConsoleApplication {
public:
  constexpr explicit ConsoleSampleApp(const toy::application::Version & version) noexcept
    : ConsoleApplication(version) {}

private:
  bool runInternal() noexcept override;
};

bool ConsoleSampleApp::runInternal() noexcept {
  const toy::chrono::Stopwatch watch;

  writeOutputLine(toy::StringView{"Hello world from Console sample app"});
  if constexpr (supportsUtf8()) {
    writeOutputLine(toy::StringView{"Привет, мир из консольного тестового приложения"});
    writeOutputLine(toy::StringView{"コンソールサンプルアプリからこんにちは世界"});
    writeOutputLine(toy::StringView{"来自控制台示例应用的世界你好"});
    writeOutputLine(toy::StringView{"콘솔 샘플 앱에서 안녕하세요 세계"});
    writeOutputLine(toy::StringView{"कंसोल नमूना ऐप से हैलो वर्ल्ड"});
    writeOutputLine(toy::StringView{"Γεια σου κόσμε από την εφαρμογή δείγματος κονσόλας"});
    writeOutputLine(toy::StringView{"🌍👋 Hello World! 🎮🕹️👾🐛"});
  }
  writeOutputLine(toy::StringView{""});

  toy::array<char, 24> bufferMajor{};
  toy::array<char, 24> bufferMinor{};
  toy::array<char, 24> bufferPatch{};
  printf("App Version: %s.%s.%s (%s)\n", toy::string_utils::utoa(toy::span{bufferMajor}, version().major),
         toy::string_utils::utoa(toy::span{bufferMinor}, version().minor),
         toy::string_utils::utoa(toy::span{bufferPatch}, version().patch), version().revision.c_str());
  writeOutputLine(toy::StringView{""});

  if (argsCount() > 1) {
    writeOutputLine(toy::StringView{""});
    writeOutputLine(toy::StringView{"arguments:"});
    for (size_t index = 1; index < argsCount(); ++index)
      if (const char * value = arg(index); value != nullptr)
        printf("- %s\n", value);
  }

  if (pid() != 0) {
    toy::array<char, 24> buffer{};
    printf("System pid: %s\n", toy::string_utils::utoa(toy::span{buffer}, pid()));
  }

  const auto calendar = toy::chrono::SystemClock::now();
  printf("System clock: %04d-%02d-%02d %02d:%02d:%02d.%03d dow: %d tz: utc%+d min\n", calendar.year, calendar.month,
         calendar.day, calendar.hour, calendar.minute, calendar.second, calendar.millisecond, calendar.dayOfWeek,
         calendar.tzOffsetMinutes);

  toy::array<char, 24> buffer{};
  printf("Steady clock frequency: %s Hz\n",
         toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::frequency()));
  printf("Steady clock ticks: %s\n", toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::nowTicks()));
  printf("Steady clock elapsed: %s ns\n", toy::string_utils::itoa(toy::span{buffer}, watch.elapsed().count()));

  return true;
}

IMPLEMENT_CONSOLE_APP(ConsoleSampleApp)

#endif
