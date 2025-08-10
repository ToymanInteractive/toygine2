#include <catch2/catch_session.hpp>

int main(int argc, char * argv[]) {
  // setup ...

  const int result = Catch::Session().run(argc, argv);

  // clean-up...

  return result;
}
