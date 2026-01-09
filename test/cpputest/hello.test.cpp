#include "CppUTest/TestHarness.h"

TEST_GROUP(FirstTestGroup){};

TEST(FirstTestGroup, FirstTest) {
  STRCMP_EQUAL("hello", "hello");
}
