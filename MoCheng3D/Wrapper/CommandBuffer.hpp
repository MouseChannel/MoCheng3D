
#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"

namespace MoCheng3D {

class CommandBuffer : public Component<vk::CommandBuffer, CommandBuffer> {
public:
  CommandBuffer();
  ~CommandBuffer();

private:
  //  vk::DescriptorBuffe
};
} // namespace MoCheng3D