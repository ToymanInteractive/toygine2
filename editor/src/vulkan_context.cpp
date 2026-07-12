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
  \file   vulkan_context.cpp
  \brief  Implementation of \ref toy::editor::VulkanContext.
*/

#include "vulkan_context.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace toy::editor {

namespace {

// Bootstrap error handling: no exceptions in this codebase, so a failed init aborts loudly.
// TODO: replace with toy::log / assert_message once the core logging module lands.
void vkCheck(VkResult result, const char * message) {
  if (result != VK_SUCCESS) {
    std::fprintf(stderr, "[editor] Vulkan error %d: %s\n", static_cast<int>(result), message);
    std::abort();
  }
}

// The MoltenVK portability driver must have VK_KHR_portability_subset enabled when it advertises it.
bool hasPortabilitySubset(VkPhysicalDevice device) {
  uint32_t count = 0;
  vkCheck(vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr),
          "vkEnumerateDeviceExtensionProperties (count) failed");
  std::vector<VkExtensionProperties> extensions(count);
  vkCheck(vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data()),
          "vkEnumerateDeviceExtensionProperties failed");

  for (const VkExtensionProperties & extension : extensions)
    if (std::strcmp(extension.extensionName, "VK_KHR_portability_subset") == 0)
      return true;

  return false;
}

} // namespace

VulkanContext::VulkanContext(void * metalLayer) {
  vkCheck(volkInitialize(), "no Vulkan loader found (install the Vulkan SDK or MoltenVK)");

  const VkApplicationInfo applicationInfo{
    .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "toygine2 editor",
    .apiVersion       = VK_API_VERSION_1_2,
  };

  const char * const instanceExtensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_EXT_METAL_SURFACE_EXTENSION_NAME,
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
    VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
  };

  const VkInstanceCreateInfo instanceInfo{
    .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .flags                   = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
    .pApplicationInfo        = &applicationInfo,
    .enabledExtensionCount   = static_cast<uint32_t>(std::size(instanceExtensions)),
    .ppEnabledExtensionNames = instanceExtensions,
  };
  vkCheck(vkCreateInstance(&instanceInfo, nullptr, &_instance), "vkCreateInstance failed");
  volkLoadInstance(_instance);

  const VkMetalSurfaceCreateInfoEXT surfaceInfo{
    .sType  = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT,
    .pLayer = static_cast<const CAMetalLayer *>(metalLayer),
  };
  vkCheck(vkCreateMetalSurfaceEXT(_instance, &surfaceInfo, nullptr, &_surface), "vkCreateMetalSurfaceEXT failed");

  uint32_t deviceCount = 0;
  vkCheck(vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr), "vkEnumeratePhysicalDevices failed");
  if (deviceCount == 0) {
    std::fprintf(stderr, "[editor] no Vulkan physical devices available\n");
    std::abort();
  }
  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkCheck(vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data()), "vkEnumeratePhysicalDevices failed");

  bool found = false;
  for (VkPhysicalDevice candidate : devices) {
    uint32_t familyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(candidate, &familyCount, nullptr);
    std::vector<VkQueueFamilyProperties> families(familyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(candidate, &familyCount, families.data());

    for (uint32_t family = 0; family < familyCount; ++family) {
      VkBool32 present = VK_FALSE;
      vkGetPhysicalDeviceSurfaceSupportKHR(candidate, family, _surface, &present);
      if ((families[family].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0 && present == VK_TRUE) {
        _physicalDevice = candidate;
        _queueFamily    = family;
        found           = true;
        break;
      }
    }
    if (found)
      break;
  }
  if (!found) {
    std::fprintf(stderr, "[editor] no device with a graphics + present queue\n");
    std::abort();
  }

  const float                   queuePriority = 1.0F;
  const VkDeviceQueueCreateInfo queueInfo{
    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .queueFamilyIndex = _queueFamily,
    .queueCount       = 1,
    .pQueuePriorities = &queuePriority,
  };

  std::vector<const char *> deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  if (hasPortabilitySubset(_physicalDevice))
    deviceExtensions.push_back("VK_KHR_portability_subset");

  const VkDeviceCreateInfo deviceInfo{
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .queueCreateInfoCount    = 1,
    .pQueueCreateInfos       = &queueInfo,
    .enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size()),
    .ppEnabledExtensionNames = deviceExtensions.data(),
  };
  vkCheck(vkCreateDevice(_physicalDevice, &deviceInfo, nullptr, &_device), "vkCreateDevice failed");
  volkLoadDevice(_device);

  vkGetDeviceQueue(_device, _queueFamily, 0, &_queue);

  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(_physicalDevice, &properties);
  std::fprintf(stderr, "[editor] Vulkan ready on device: %s\n", properties.deviceName);
}

VulkanContext::~VulkanContext() {
  if (_device != VK_NULL_HANDLE)
    vkDestroyDevice(_device, nullptr);
  if (_surface != VK_NULL_HANDLE)
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
  if (_instance != VK_NULL_HANDLE)
    vkDestroyInstance(_instance, nullptr);
}

} // namespace toy::editor
