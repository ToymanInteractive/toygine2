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

cmake_minimum_required(VERSION 3.27.0 FATAL_ERROR)

if (APPLE)
  set(CMAKE_OBJCXX_STANDARD 23)
  set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)

  # CAMetalLayer / MoltenVK need a modern deployment target; bump if you raise the Vulkan floor.
  if (NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version" FORCE)
  endif ()
endif ()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty)

include(ckdl)

if (APPLE)
  include(vulkan)

  include(FetchContent)

  # volk reads VULKAN_HEADERS_INSTALL_DIR before it falls back to find_package(Vulkan) and $VULKAN_SDK, so the vendored
  # headers win over whatever SDK the build machine happens to carry, and a machine without one still builds.
  set(VULKAN_HEADERS_INSTALL_DIR ${VULKAN_HEADERS_DIR})

  # volk is pinned to an exact tag; bump it against the vendored header version in a dedicated change.
  FetchContent_Declare(
    volk
    GIT_REPOSITORY https://github.com/zeux/volk.git
    GIT_TAG        1.4.350
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(volk)

  # volk and every consumer must agree on the platform surface so vkCreateMetalSurfaceEXT is loaded.
  target_compile_definitions(volk PUBLIC VK_USE_PLATFORM_METAL_EXT)

  add_executable(editor MACOSX_BUNDLE
    src/main.cpp
    src/main_menu.mm
    src/window.mm
    src/vulkan_context.cpp
  )

  # The window and menu backends are Objective-C++; ARC keeps the Cocoa object lifetimes correct.
  set_source_files_properties(src/main_menu.mm src/window.mm PROPERTIES COMPILE_OPTIONS "-fobjc-arc")

  target_link_libraries(editor PRIVATE
    volk::volk
    Vulkan::Headers
    "-framework Cocoa"
    "-framework QuartzCore"
    "-framework Metal"
  )

  target_compile_options(editor PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>
    $<$<COMPILE_LANGUAGE:OBJCXX>:-Wall;-Wextra>
  )

  # The bundle is the whole artifact: everything the editor needs is inside editor.app. Its component joins the runtime
  # group in the root CMakeLists, so it ships in the same archive as the library and the samples.
  install(TARGETS editor BUNDLE DESTINATION bin COMPONENT editor)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Unit tests
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif ()
