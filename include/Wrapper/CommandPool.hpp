#pragma once
#include "Wrapper/Base.hpp"
#include "Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class CommandPool : public Component<vk::CommandPool, CommandPool> {
public:
    CommandPool();
    ~CommandPool();

private:
};
} // namespace MoCheng3D