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

# Vulkan meta-loader for the editor: loads the Vulkan library at runtime and resolves every entry point, so nothing
# links against a Vulkan SDK. The copy drops the upstream CMakeLists, which looks for the headers through
# find_package(Vulkan) and $VULKAN_SDK and installs a CMake package. thirdparty/README.md records the upstream commit.

set(VOLK_DIR ${CMAKE_CURRENT_LIST_DIR}/volk)

# volk.h includes vulkan_core.h, and VOLK_HEADER_VERSION must match the vendored VK_HEADER_VERSION: update both together.
if(NOT TARGET Vulkan::Headers)
  include(${CMAKE_CURRENT_LIST_DIR}/vulkan.cmake)
endif()

set(SRC_VOLK_LIST
  ${VOLK_DIR}/volk.c
)

# The compiler finds this through the include path; listing it puts the whole copy in the IDE's project tree.
set(HDR_VOLK_LIST
  ${VOLK_DIR}/volk.h
)

add_library(volk STATIC ${SRC_VOLK_LIST} ${HDR_VOLK_LIST})

# Keeps the vendored target out of the way in the Xcode and Visual Studio trees; other generators ignore it.
set_target_properties(volk PROPERTIES FOLDER "thirdparty")

# SYSTEM keeps the project's warnings off headers that are not ours.
target_include_directories(volk SYSTEM PUBLIC ${VOLK_DIR})

# volk.h exposes the Vulkan types, so consumers get the headers along with volk. The consumer adds the platform surface
# macro (VK_USE_PLATFORM_*) to volk as PUBLIC, so volk.c loads the same surface entry points that the consumer calls.
target_link_libraries(volk PUBLIC Vulkan::Headers)

# dlopen() lives in libdl on older glibc and on Android; CMAKE_DL_LIBS is empty where the C library already has it.
if(NOT WIN32)
  target_link_libraries(volk PRIVATE ${CMAKE_DL_LIBS})
endif()

add_library(volk::volk ALIAS volk)
