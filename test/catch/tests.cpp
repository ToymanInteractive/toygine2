#include <stdexcept>

#include <catch2/catch_session.hpp>

#include "core.hpp"

[[noreturn]] bool _assertionCallback(const char * message) {
  throw std::runtime_error(message);
}

void _stackWalkCallback([[maybe_unused]] const char * info) {}

int main(int argc, char * argv[]) {
  toy::assertion::initialize();

  toy::assertion::setCallbacks(_assertionCallback, _stackWalkCallback);

  const int result = Catch::Session().run(argc, argv);

  toy::assertion::deInitialize();

  return result;
}
