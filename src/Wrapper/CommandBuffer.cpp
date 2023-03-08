#include "MoCheng3D/Wrapper/CommandBuffer.hpp"
#include "MoCheng3D/Wrapper/Buffer.hpp"
#include "MoCheng3D/Wrapper/CommandPool.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/Pipeline.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D
{
CommandBuffer::CommandBuffer()
{

    vk::CommandBufferAllocateInfo allocate_info;

    auto command_pool = Get_Context_Singleton().Get_CommandPool();
    allocate_info.setCommandBufferCount(1)
        .setLevel(vk::CommandBufferLevel::ePrimary)
        .setCommandPool(command_pool->Get_handle());

    m_handle = Get_Context_Singleton().Get_Device()->Get_handle().allocateCommandBuffers(allocate_info)[0];
}
void CommandBuffer::Begin(vk::CommandBufferBeginInfo begin)
{
    m_handle.begin(begin);
}
void CommandBuffer::End()
{
    m_handle.end();
}
void CommandBuffer::Reset()
{
    m_handle.reset();
}
void CommandBuffer::BeginRenderPass(vk::RenderPassBeginInfo renderPassInfo, vk::SubpassContents content)
{
    m_handle.beginRenderPass(renderPassInfo, content);
}
void CommandBuffer::EndRenderPass()
{
    m_handle.endRenderPass();
}
void CommandBuffer::BindPipeline(vk::PipelineBindPoint type, Pipeline::Ptr pipeline)
{
    m_handle.bindPipeline(type, pipeline->Get_handle());
}

void CommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    m_handle.draw(vertexCount, instanceCount, firstVertex, firstInstance);
}
void CommandBuffer::CopyBuffer(Buffer::Ptr src_buffer, Buffer::Ptr dst_buffer)
{

    vk::BufferCopy regin;
    regin.setSize(src_buffer->GetSize()).setDstOffset(0).setSrcOffset(0);

    m_handle.copyBuffer(src_buffer->Get_handle(), dst_buffer->Get_handle(), regin);
}
void CommandBuffer::BindVertexBuffer(uint32_t firstBinding, Buffer::Ptr buffer, vk::DeviceSize offsets)
{

    m_handle.bindVertexBuffers(firstBinding, {buffer->Get_handle()}, offsets);
}
void CommandBuffer::BindUniformBuffer(vk::PipelineLayout layout, vk::DescriptorSet descriptorset)
{
    m_handle.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 0, descriptorset, {});
}

CommandBuffer::~CommandBuffer()
{
    auto command_pool = Get_Context_Singleton().Get_CommandPool();
    Get_Context_Singleton().Get_Device()->Get_handle().freeCommandBuffers(command_pool->Get_handle(), m_handle);
}

} // namespace MoCheng3D