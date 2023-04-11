
#pragma once
#include "Wrapper/Base.hpp"
#include "Wrapper/Component.hpp"

namespace MoCheng3D {
class Pipeline;
class Buffer;
class CommandBuffer : public Component<vk::CommandBuffer, CommandBuffer> {
public:
    CommandBuffer();
    ~CommandBuffer();
    void Begin(vk::CommandBufferUsageFlags begin_flags);
    void End();
    void Reset();
    void BeginRenderPass(vk::RenderPassBeginInfo renderPassInfo,
        vk::SubpassContents content);
    void EndRenderPass();
    void BindPipeline(vk::PipelineBindPoint type, std::shared_ptr<Pipeline>);
    void Draw(uint32_t vertexCount, uint32_t instanceCount,
        uint32_t firstVertex, uint32_t firstInstance);
    void DrawIndex(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
    void CopyBuffer(std::shared_ptr<Buffer> src_buffer,
        std::shared_ptr<Buffer> dst_buffer);
    void BindIndicesBuffer(std::shared_ptr<Buffer> buffer, vk::DeviceSize offset, vk::IndexType index_type);
    void BindVertexBuffer(uint32_t firstBinding, std::vector<std::shared_ptr<Buffer>> buffers,
        vk::DeviceSize offsets);
    void PushContants(vk::PipelineLayout layout,
        vk::ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, void* value);
    void BindDescriptorSet(vk::PipelineLayout layout, vk::DescriptorSet);
    void Push_Constants(vk::PipelineLayout layout,
        vk::ShaderStageFlags stageFlags, uint32_t offset,
        uint32_t size, const void* data);

private:
};
} // namespace MoCheng3D