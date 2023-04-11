#pragma once

#include "Wrapper/Component.hpp"
#include <vulkan/vulkan.hpp>
namespace MoCheng3D {

class Instance : public Component<vk::Instance, Instance> {

public:
    Instance();
    ~Instance();
    std::vector<const char*> GetRequiredExtensions();
    void Enable_vaildLayer();
    void print_available_extensions();

    void print_available_layers();

private:
    //   VkDebugUtilsMessengerEXT m_Debugger{VK_NULL_HANDLE};
    vk::DebugUtilsMessengerEXT m_debugger;
};

} // namespace MoCheng3D
