#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D
{
struct Color final
{
    float r, g, b;
};
struct Uniform final
{
    Color color;
    static vk::DescriptorSetLayoutBinding GetBinding()
    {
        vk::DescriptorSetLayoutBinding binding;
        binding.setBinding(0)
            .setDescriptorType(vk::DescriptorType::eUniformBuffer)
            .setStageFlags(vk::ShaderStageFlagBits::eFragment)
            .setDescriptorCount(1);
        return binding;
    }
};

} // namespace MoCheng3D