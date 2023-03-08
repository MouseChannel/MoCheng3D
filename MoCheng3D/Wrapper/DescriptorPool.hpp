#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D
{
class DescriptorSet;
class DescriptorPool : public Component<vk::DescriptorPool, DescriptorPool>
{
  public:
    DescriptorPool();
    ~DescriptorPool();
    // std::shared_ptr<DescriptorSet> CreateDescriptorSet();
  private:
};
} // namespace MoCheng3D