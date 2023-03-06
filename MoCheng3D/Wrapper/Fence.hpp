#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class Fence : public Component<vk::Fence, Fence> {
public:
  Fence();
  ~Fence();

private:
};
} // namespace MoCheng3D