#include <stdexcept>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include "core.hpp"

TEST_CASE("core/fixed_vector/FixedVector constructors") {
  REQUIRE(true == true);
}

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
