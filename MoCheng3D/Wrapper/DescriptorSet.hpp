#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class DescriptorPool;
class Buffer;
class DescriptorSet : public Component<std::vector<vk::DescriptorSet>, DescriptorSet> {
public:
    DescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool, vk::DescriptorSetLayout setLayouts);
    ~DescriptorSet();
    void Update(std::shared_ptr<Buffer> buffer, uint32_t binding_index,
        vk::DescriptorType descriptor_type );
    void Update(std::shared_ptr<Image> image, uint32_t binding_index, vk::DescriptorType descriptor_type);

private:
};
} // namespace MoCheng3D