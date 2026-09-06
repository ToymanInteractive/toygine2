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

# Vulkan API headers for the editor. Nothing here is compiled or linked: volk resolves the entry points at runtime and
# MoltenVK provides the ICD.
#
# The vendored copy keeps only include/ and LICENSES/, dropping the upstream CMakeLists that installs the headers and
# builds the C++ module. thirdparty/README.md records the upstream tag.

# volk locates the headers by path rather than through Vulkan::Headers; set this before it is configured.
set(VULKAN_HEADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/vulkan)

add_library(vulkan_headers INTERFACE)

# SYSTEM keeps the project's warning flags off headers that are not ours to keep clean.
target_include_directories(vulkan_headers SYSTEM INTERFACE ${VULKAN_HEADERS_DIR}/include)

add_library(Vulkan::Headers ALIAS vulkan_headers)
