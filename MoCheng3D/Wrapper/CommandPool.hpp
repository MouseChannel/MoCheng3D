#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class CommandPool : public Component<vk::CommandPool, CommandPool> {
public:
  CommandPool();
  ~CommandPool();

private:
};
} // namespace MoCheng3D