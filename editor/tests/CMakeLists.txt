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

if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

  if (MSVC)

    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /EHsc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

  endif ()

endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Remove all of the extra targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
# Tests translation units
#-----------------------------------------------------------------------------------------------------------------------

# Tests of editor modules. Both runners build them, so a drift between the two splits the verdict or fails the build.
file(GLOB_RECURSE TOYGINE_EDITOR_TESTS_SRC CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")

#-----------------------------------------------------------------------------------------------------------------------
# Tests built with doctest and discovered by CTest
#-----------------------------------------------------------------------------------------------------------------------

set(TOYGINE_DESKTOP_PLATFORMS "Windows Desktop" "Linux Desktop" "macOS Desktop")

if (TOYGINE_TARGET_PLATFORM IN_LIST TOYGINE_DESKTOP_PLATFORMS)
  include(FetchContent)
  include(CTest)

  FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/doctest/doctest GIT_TAG v2.5.3 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(doctest)

  list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

  include(doctest)

  add_executable(editor-units
      ${TOYGINE_EDITOR_TESTS_SRC}
      ctest_main.cpp)
  target_compile_definitions(editor-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)

  target_include_directories(editor-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
  target_link_libraries(editor-units PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})

  # arm64 macOS kills an unsigned binary, and Xcode signs only after the post-build phases, so the linker signs instead.
  set_target_properties(editor-units PROPERTIES XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED NO)

  doctest_discover_tests(editor-units)
endif ()
