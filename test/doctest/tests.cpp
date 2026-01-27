#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include "core.hpp"

[[noreturn]] bool _assertionCallback(const char * message) {
  throw std::runtime_error(message);
}

void _stackWalkCallback([[maybe_unused]] const char * info) {}

int main(int argc, char ** argv) {
  toy::assertion::initialize();

  toy::assertion::setCallbacks(_assertionCallback, _stackWalkCallback);

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
