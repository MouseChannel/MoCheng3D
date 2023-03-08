#pragma once
#include <vulkan/vulkan.hpp>

namespace MoCheng3D
{
struct Vertex final
{

    float x, y;

    static vk::VertexInputAttributeDescription Get_Attr()
    {
        vk::VertexInputAttributeDescription attr;
        attr.setBinding(0)

            .setFormat(vk::Format::eR32G32Sfloat)
            .setLocation(0)
            .setOffset(0);
        return attr;
    }
    static vk::VertexInputBindingDescription Get_Binding()
    {
        vk::VertexInputBindingDescription binding;
        binding
            .setBinding(0)

            .setInputRate(vk::VertexInputRate::eVertex)
            .setStride(sizeof(Vertex));
        return binding;
    }
};

} // namespace MoCheng3D