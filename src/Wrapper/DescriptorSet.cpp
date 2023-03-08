#include "MoCheng3D/Wrapper/DescriptorSet.hpp"
#include "MoCheng3D/Wrapper/Buffer.hpp"
#include "MoCheng3D/Wrapper/DescriptorPool.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"
namespace MoCheng3D
{
DescriptorSet::DescriptorSet(DescriptorPool::Ptr descriptorPool, vk::DescriptorSetLayout setLayouts)
{
    vk::DescriptorSetAllocateInfo allocate_info;
    auto swapchain_size = Get_Context_Singleton().Get_SwapChain()->Get_Swapchain_Image_size();
    std::vector<vk::DescriptorSetLayout> descriptor_layouts(swapchain_size, setLayouts);
    allocate_info.setDescriptorPool(descriptorPool->Get_handle())
        .setSetLayouts(descriptor_layouts)
        .setDescriptorSetCount(swapchain_size);
    m_handle = Get_Context_Singleton().Get_Device()->Get_handle().allocateDescriptorSets(allocate_info);
}

DescriptorSet::~DescriptorSet()
{
}
void DescriptorSet::Update(Buffer::Ptr buffer)
{
    for (int i = 0; i < m_handle.size(); i++)
    {

        auto set = m_handle[i];
        vk::DescriptorBufferInfo buffer_info;
        buffer_info
            .setBuffer(buffer->Get_handle())

            .setRange(buffer->GetSize())
            .setOffset(0);
        vk::WriteDescriptorSet writer;
        writer.setDescriptorType(vk::DescriptorType::eUniformBuffer)
            .setBufferInfo(buffer_info)
            .setDstBinding(0)
            .setDstSet(set)
            .setDstArrayElement(0)
            .setDescriptorCount(1);
        Get_Context_Singleton().Get_Device()->Get_handle().updateDescriptorSets(writer, {});
    }
}

} // namespace MoCheng3D