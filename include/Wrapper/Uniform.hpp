#pragma once
#include "Wrapper/Base.hpp"
#include "Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
struct Color final {
    float r, g, b;
};
struct Uniform final {
    Color color;
    static vk::DescriptorSetLayoutBinding GetBinding(uint32_t binding_index, vk::ShaderStageFlags shader_stage, vk::DescriptorType type)
    {
        vk::DescriptorSetLayoutBinding binding;
        binding.setBinding(binding_index)
            .setDescriptorType(type)
            .setStageFlags(shader_stage)
            .setDescriptorCount(1);
        return binding;
    }
    // static vk::DescriptorSetLayoutBinding GetDefaultBinding()
    // {
    //     return GetBinding(0, vk::ShaderStageFlagBits::eFragment);
    // }
};

} // namespace MoCheng3D