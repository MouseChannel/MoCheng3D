#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Rendering/GLFW_Window.hpp"
#include "MoCheng3D/Wrapper/Instance.hpp"
#include <cassert>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace MoCheng3D {
Device::Device() {

  auto &instance = Get_Context_Singleton().Get_Instance();
  
  auto avalible_physical_device =
      instance->Get_handle().enumeratePhysicalDevices();
  physical_device = avalible_physical_device[0];

  assert(physical_device);
  QueryQueueFamilyIndices();
  vk::DeviceCreateInfo create_info;
  vk::DeviceQueueCreateInfo queue_create_info;

  float priorities = 1.0f;

  if (queue_family_indices.present_queue.value() ==
      queue_family_indices.graphic_queue.value()) {
  }

  queue_create_info.setPQueuePriorities(&priorities)
      .setQueueCount(1)
      .setQueueFamilyIndex(queue_family_indices.graphic_queue.value());

  create_info.setQueueCreateInfos(queue_create_info)
      .setPEnabledExtensionNames(deviceRequiredExtensions);
  m_handle = physical_device.createDevice(create_info);

  graphic_queue =
      m_handle.getQueue(queue_family_indices.graphic_queue.value(), 0);
  present_queue =
      m_handle.getQueue(queue_family_indices.present_queue.value(), 0);
}

Device::~Device() {}

void Device::QueryQueueFamilyIndices() {

  auto properties = physical_device.getQueueFamilyProperties();

  auto surface = Get_Context_Singleton().Get_Window()->GetSurface();
  for (int i = 0; i < properties.size(); i++) {
    auto property = properties[i];
    if (property.queueFlags | vk::QueueFlagBits::eGraphics) {
      queue_family_indices.graphic_queue = i;
    }
    if (physical_device.getSurfaceSupportKHR(i, surface)) {
      queue_family_indices.present_queue = i;
    }
    if (queue_family_indices.Complete()) {
      break;
    }
  }
}

} // namespace MoCheng3D