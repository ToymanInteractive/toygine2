#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Toyman Interactive
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the "Software"), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and / or sell copies of the Software, and to permit
# persons to whom the Software is furnished to do so, subject to the following conditions :
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
# Tests translation units
#-----------------------------------------------------------------------------------------------------------------------

# Tests of the built-in runner, the one the consoles depend on. Written against doctest and inspecting the runner's
# internals, so they build on desktop alone, whichever runner TOYGINE_TESTS_RUNNER selects for the engine's own tests.
file(GLOB_RECURSE TOYGINE_TEST_RUNNER_TESTS_SRC CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")

#-----------------------------------------------------------------------------------------------------------------------
# Tests built with doctest and discovered by CTest
#-----------------------------------------------------------------------------------------------------------------------

# Declares doctest::doctest and doctest_discover_tests() over the vendored copy in thirdparty/doctest.
include(doctest)

add_executable(console-test-runner-units
    ${TOYGINE_TEST_RUNNER_TESTS_SRC}
    ctest_main.cpp)

# The shim resolves TEST_CASE to doctest's, leaving the TOY_TEST_ macros beside it as what these cases exercise.
target_compile_definitions(console-test-runner-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS TOYGINE_TESTS_USE_DOCTEST)

target_include_directories(console-test-runner-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(console-test-runner-units PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})
set_target_properties(console-test-runner-units PROPERTIES FOLDER "tests")

# arm64 macOS kills an unsigned binary, and Xcode signs only after the post-build phases, so the linker signs instead.
set_target_properties(console-test-runner-units PROPERTIES XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED NO)

doctest_discover_tests(console-test-runner-units)
