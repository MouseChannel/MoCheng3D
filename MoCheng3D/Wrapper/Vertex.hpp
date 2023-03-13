#pragma once
#include "MoCheng3D/Math/math.hpp"
#include <vulkan/vulkan.hpp>

namespace MoCheng3D {
struct Vertex final {

    Vec2 position;
    Vec2 texcoord;
    // float x, y;

    static std::vector<vk::VertexInputAttributeDescription> Get_Attr()
    {
        std::vector<vk::VertexInputAttributeDescription> descriptions(2);
        descriptions[0]
            .setBinding(0)
            .setFormat(vk::Format::eR32G32Sfloat)
            .setLocation(0)
            .setOffset(0);
        descriptions[1]
            .setBinding(0)
            .setFormat(vk::Format::eR32G32Sfloat)
            .setLocation(1)
            .setOffset(offsetof(Vertex, texcoord));
        vk::VertexInputAttributeDescription attr;
      
        return descriptions;
    }
    static std::vector<vk::VertexInputBindingDescription> Get_Binding()
    {
        vk::VertexInputBindingDescription binding;
        binding
            .setBinding(0)

            .setInputRate(vk::VertexInputRate::eVertex)
            .setStride(sizeof(Vertex));
        return { binding };
    }
};

} // namespace MoCheng3D