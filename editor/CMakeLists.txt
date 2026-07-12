#
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
#

cmake_minimum_required(VERSION 3.24)

project(toygine2_editor VERSION 0.1.0 LANGUAGES CXX OBJCXX)

if(NOT APPLE)
  message(FATAL_ERROR "The editor bootstrap currently targets macOS only (Cocoa + MoltenVK).")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OBJCXX_STANDARD 23)
set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)

# CAMetalLayer / MoltenVK need a modern deployment target; bump if you raise the Vulkan floor.
if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version" FORCE)
endif()

include(FetchContent)

# Vulkan-Headers and volk are pinned to exact tags. Bump both together in a dedicated change.
FetchContent_Declare(
  Vulkan-Headers
  GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
  GIT_TAG        vulkan-sdk-1.3.296.0
  GIT_SHALLOW    TRUE
)
FetchContent_Declare(
  volk
  GIT_REPOSITORY https://github.com/zeux/volk.git
  GIT_TAG        1.3.295
  GIT_SHALLOW    TRUE
)

# Order matters: Vulkan-Headers provides the Vulkan::Headers target that volk links against.
FetchContent_MakeAvailable(Vulkan-Headers volk)

# volk and every consumer must agree on the platform surface so vkCreateMetalSurfaceEXT is loaded.
target_compile_definitions(volk PUBLIC VK_USE_PLATFORM_METAL_EXT)

add_executable(editor MACOSX_BUNDLE
  src/main.cpp
  src/window.mm
  src/vulkan_context.cpp
)

# The window backend is Objective-C++; ARC keeps the Cocoa object lifetimes correct.
set_source_files_properties(src/window.mm PROPERTIES COMPILE_OPTIONS "-fobjc-arc")

target_link_libraries(editor PRIVATE
  volk::volk
  "-framework Cocoa"
  "-framework QuartzCore"
  "-framework Metal"
)

target_compile_options(editor PRIVATE
  $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>
  $<$<COMPILE_LANGUAGE:OBJCXX>:-Wall;-Wextra>
)
