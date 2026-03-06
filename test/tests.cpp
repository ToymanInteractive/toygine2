#define DOCTEST_CONFIG_IMPLEMENT

#include <doctest/doctest.h>

#include "core.hpp"

[[noreturn]] static bool assertionCallback(const char * message) {
  throw std::runtime_error(message);
}

static void stackWalkCallback([[maybe_unused]] const char * info) {}

#if defined(__GBA__)

#include <gba_console.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_video.h>
#include <stdio.h>

#include "gba_filtering_stream_buf.hpp"

int main() {
  irqInit();
  irqEnable(IRQ_VBLANK);

  consoleDemoInit();

  toy::assertion::initialize();

  toy::assertion::setCallbacks(assertionCallback, stackWalkCallback);

  GbaFilteringStreamBuf gbaStreamBuf;
  auto gbaStream = std::ostream(&gbaStreamBuf);

  doctest::Context context;
  context.setCout(&gbaStream);

  //  context.applyCommandLine(argc, argv);

  int res = context.run(); // run doctest

  toy::assertion::deInitialize();

  // important - query flags (and --exit) rely on the user doing this
  if (context.shouldExit()) {
    // propagate the result of the tests
    return res;
  }

  return res;
}

#else

int main(int argc, char ** argv) {
  toy::assertion::initialize();

  toy::assertion::setCallbacks(assertionCallback, stackWalkCallback);

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int res = context.run(); // run doctest

  toy::assertion::deInitialize();

  // important - query flags (and --exit) rely on the user doing this
  if (context.shouldExit()) {
    // propagate the result of the tests
    return res;
  }

  return res;
}

#endif
