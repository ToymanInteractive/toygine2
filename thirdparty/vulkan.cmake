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
# MoltenVK provides the ICD. The copy keeps include/ and LICENSES/ and drops the upstream CMakeLists, which would
# install the headers and build the C++ module. thirdparty/README.md records the upstream tag.

set(VULKAN_HEADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/vulkan)

# The compiler finds these through the include path; listing them puts the whole copy in the IDE's project tree.
# vulkan.cppm and vulkan_video.cppm stay out: as target sources they would be built as C++ modules.
set(HDR_VULKAN_LIST
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_av1std.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_av1std_decode.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_av1std_encode.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_h264std.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_h264std_decode.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_h264std_encode.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_h265std.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_h265std_decode.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_h265std_encode.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_vp9std.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codec_vp9std_decode.h
  ${VULKAN_HEADERS_DIR}/include/vk_video/vulkan_video_codecs_common.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vk_icd.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vk_layer.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vk_platform.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_android.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_beta.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_core.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_directfb.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_enums.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_extension_inspection.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_format_traits.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_fuchsia.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_funcs.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_ggp.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_handles.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_hash.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_hpp_macros.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_ios.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_macos.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_metal.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_ohos.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_raii.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_screen.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_shared.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_static_assertions.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_structs.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_to_string.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_ubm.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_vi.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_video.hpp
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_wayland.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_win32.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_xcb.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_xlib.h
  ${VULKAN_HEADERS_DIR}/include/vulkan/vulkan_xlib_xrandr.h
)

add_library(vulkan_headers INTERFACE ${HDR_VULKAN_LIST})

# Keeps the vendored target out of the way in the Xcode and Visual Studio trees; other generators ignore it.
set_target_properties(vulkan_headers PROPERTIES FOLDER "thirdparty")

# SYSTEM keeps the project's warnings off headers that are not ours.
target_include_directories(vulkan_headers SYSTEM INTERFACE ${VULKAN_HEADERS_DIR}/include)

add_library(Vulkan::Headers ALIAS vulkan_headers)
