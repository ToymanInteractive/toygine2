include(FetchContent)
include(CTest)

set(CATCH_INSTALL_HELPERS OFF)
set(CATCH_BUILD_TESTING OFF)
set(CATCH_INSTALL_DOCS OFF)

FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.8.1)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

file(GLOB TEST_SRC CONFIGURE_DEPENDS *.cpp)

include(Catch)

add_executable(${PROJECT_NAME}-tests ${TEST_SRC})
target_link_libraries(${PROJECT_NAME}-tests PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME})
catch_discover_tests(${PROJECT_NAME}-tests)
