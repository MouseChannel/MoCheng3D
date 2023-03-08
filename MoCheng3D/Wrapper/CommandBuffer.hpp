
#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"

namespace MoCheng3D
{
class Pipeline;
class Buffer;
class CommandBuffer : public Component<vk::CommandBuffer, CommandBuffer>
{
  public:
    CommandBuffer();
    ~CommandBuffer();
    void Begin(vk::CommandBufferBeginInfo begin);
    void End();
    void Reset();
    void BeginRenderPass(vk::RenderPassBeginInfo renderPassInfo, vk::SubpassContents content);
    void EndRenderPass();
    void BindPipeline(vk::PipelineBindPoint type, std::shared_ptr<Pipeline>);
    void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    void CopyBuffer(std::shared_ptr<Buffer> src_buffer, std::shared_ptr<Buffer> dst_buffer);
    void BindVertexBuffer(uint32_t firstBinding, std::shared_ptr<Buffer> buffer, vk::DeviceSize offsets);
    void BindUniformBuffer(vk::PipelineLayout layout, vk::DescriptorSet);
    //   {

    //       m_handle.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vk::PipelineLayout layout, uint32_t firstSet,
    //                                   const vk::ArrayProxy<const vk::DescriptorSet> &descriptorSets,
    //                                   const vk::ArrayProxy<const uint32_t> &dynamicOffsets)
    //   }

  private:
};
} // namespace MoCheng3D