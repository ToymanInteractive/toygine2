//
// Copyright (c) 2026 Toyman Interactive
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
/*!
  \file   vulkan_context.hpp
  \brief  Minimal Vulkan/MoltenVK initialization for the editor bootstrap.

  Defines \ref toy::editor::VulkanContext: creates a Vulkan instance through volk, a Metal surface
  from a CAMetalLayer, selects a physical device with graphics + present support, and creates a
  logical device with one queue. Swapchain and rendering are intentionally left out.
*/

#ifndef EDITOR_SRC_VULKAN_CONTEXT_HPP_
#define EDITOR_SRC_VULKAN_CONTEXT_HPP_

#include <cstdint>

#include <volk.h>

namespace toy::editor {

/*!
  \class VulkanContext
  \brief Instance, surface, and device for a single window, via MoltenVK.

  Owns the core Vulkan objects needed before any rendering: a portability-enabled instance, a Metal
  surface, the selected physical device, and a logical device with a combined graphics/present
  queue. All handles are destroyed in reverse order on teardown.
*/
class VulkanContext {
public:
  /*!
    \brief Initialize Vulkan and create a surface for the given Metal layer.

    \param metalLayer Opaque CAMetalLayer pointer, e.g. from \ref toy::editor::Window::metalLayer.

    \pre volk must be able to load a Vulkan/MoltenVK loader at runtime.
  */
  explicit VulkanContext(void * metalLayer);

  VulkanContext(const VulkanContext &)             = delete;
  VulkanContext & operator=(const VulkanContext &) = delete;
  VulkanContext(VulkanContext &&)                  = delete;
  VulkanContext & operator=(VulkanContext &&)      = delete;

  ~VulkanContext();

private:
  VkInstance       _instance       = VK_NULL_HANDLE;
  VkSurfaceKHR     _surface        = VK_NULL_HANDLE;
  VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
  VkDevice         _device         = VK_NULL_HANDLE;
  VkQueue          _queue          = VK_NULL_HANDLE;
  uint32_t         _queueFamily    = 0;
};

} // namespace toy::editor

#endif // EDITOR_SRC_VULKAN_CONTEXT_HPP_
