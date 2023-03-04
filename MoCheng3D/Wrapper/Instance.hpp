#pragma once

#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan.hpp>
namespace MoCheng3D {

class Instance : public Component<vk::Instance, Instance> {

public:
  Instance();
  ~Instance();
  std::vector<const char *> GetRequiredExtensions();
  void Enable_vaildLayer();
   void print_available_extensions();

private:
  VkDebugUtilsMessengerEXT m_Debugger{VK_NULL_HANDLE};
};

} // namespace MoCheng3D
