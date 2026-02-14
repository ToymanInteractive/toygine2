#define DOCTEST_CONFIG_IMPLEMENT

#include <doctest/doctest.h>

#include "core.hpp"

[[noreturn]] static bool assertionCallback(const char * message) {
  throw std::runtime_error(message);
}

static void stackWalkCallback([[maybe_unused]] const char * info) {}

int __cdecl main(int argc, char ** argv) {
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
