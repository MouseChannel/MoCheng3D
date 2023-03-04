#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class RenderPass : public Component<vk::RenderPass, RenderPass> {
public:
  RenderPass();
  ~RenderPass();

private:
};
} // namespace MoCheng3D