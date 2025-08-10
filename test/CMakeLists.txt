#-------------------------------------------------------------------------------------------
if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

  if (MSVC)
    string(REGEX REPLACE "/Wall " "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/Wall " "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    string(REGEX REPLACE "/W." "/W0" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/W." "/W0" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    string(REGEX REPLACE "/sdl" "/sdl-" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    string(REGEX REPLACE "/sdl" "/sdl-" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")

    string(REGEX REPLACE "/sdl" "/sdl-" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REGEX REPLACE "/sdl" "/sdl-" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  endif (MSVC)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")

  string(REGEX REPLACE "-Werror" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  string(REGEX REPLACE "-Weverything" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  string(REGEX REPLACE "-pedantic-errors" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  set(CMAKE_C_FLAGS "-w ${CMAKE_C_FLAGS}")

  string(REGEX REPLACE "-Werror" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REGEX REPLACE "-Weverything" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REGEX REPLACE "-pedantic-errors" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "-w ${CMAKE_CXX_FLAGS}")

endif()
#-------------------------------------------------------------------------------------------
# Remove all of the extra targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
#-------------------------------------------------------------------------------------------

include(FetchContent)
include(CTest)

set(CATCH_INSTALL_HELPERS OFF)
set(CATCH_BUILD_TESTING OFF)
set(CATCH_INSTALL_DOCS OFF)

FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.9.1)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include(Catch)

add_executable(${TOYGINE_LIBRARY_NAME}-tests ${TEST_SRC})
target_include_directories(${TOYGINE_LIBRARY_NAME}-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(${TOYGINE_LIBRARY_NAME}-tests PRIVATE Catch2::Catch2 ${TOYGINE_LIBRARY_NAME})
catch_discover_tests(${TOYGINE_LIBRARY_NAME}-tests)
