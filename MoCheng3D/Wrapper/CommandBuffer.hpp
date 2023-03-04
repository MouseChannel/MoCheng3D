#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {

class CommandBuffer : public Component<vk::CommandBuffer, CommandBuffer> {
public:
  CommandBuffer();
  ~CommandBuffer();

private:
};
} // namespace MoCheng3D