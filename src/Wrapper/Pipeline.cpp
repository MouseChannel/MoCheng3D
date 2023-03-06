#include "MoCheng3D/Wrapper/Pipeline.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/RenderPass.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"
#include <vector>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
namespace MoCheng3D {
Pipeline::Pipeline() {}
Pipeline::~Pipeline() {

  Get_Context_Singleton().Get_Device()->Get_handle().destroyPipeline(m_handle);
}
void Pipeline::Build_Pipeline(RenderPass::Ptr render_pass) {
  vk::GraphicsPipelineCreateInfo create_info;
  create_info.setLayout(layout)
      .setRenderPass(render_pass->Get_handle())
      // vertex input
      .setPVertexInputState(&input_state)
      // vertex assembly
      .setPInputAssemblyState(&input_assembly)
      // viewport and scissor
      .setPViewportState(&viewportInfo)
      // shader
      .setStages(shader_stage)
      // rasterization
      .setPRasterizationState(&rasterization_info)
      .setPMultisampleState(&multi_sample)
      .setPDepthStencilState(&depth_test)
      .setPColorBlendState(&blend);
  auto res =
      Get_Context_Singleton().Get_Device()->Get_handle().createGraphicsPipeline(
          nullptr, create_info);
  m_handle = res.value;
}
void Pipeline::Make_Layout() {
  vk::PipelineLayoutCreateInfo createInfo;
  layout = Get_Context_Singleton()

               .Get_Device()
               ->Get_handle()
               .createPipelineLayout(createInfo);
}
void Pipeline::Make_VertexInput(
    std::vector<vk::VertexInputBindingDescription> des,
    std::vector<vk::VertexInputAttributeDescription> attr) {
  input_state.setVertexAttributeDescriptionCount(des.size())
      .setPVertexBindingDescriptions(des.data())
      .setVertexAttributeDescriptionCount(attr.size())
      .setPVertexAttributeDescriptions(attr.data());
}
void Pipeline::Make_VertexAssembly() {
  input_assembly.setTopology(vk::PrimitiveTopology::eTriangleList)
      .setPrimitiveRestartEnable(false);
}

void Pipeline::Make_viewPort() {

  auto extent2D = Get_Context_Singleton().Get_SwapChain()->Get_Extent2D();
  viewport.setX(0)
      .setY(0)
      .setHeight(extent2D.height)
      .setWidth(extent2D.width)
      .setMinDepth(0)
      .setMaxDepth(1);

  scissor.setExtent(extent2D).setOffset(vk::Offset2D{0, 0});
  viewportInfo.setViewports(viewport)
      .setScissors(scissor)
      .setViewportCount(1)
      .setScissorCount(1);
}
void Pipeline::Add_Shader_Modules(vk::ShaderModule module,
                                  vk::ShaderStageFlagBits stage) {
  vk::PipelineShaderStageCreateInfo shader_create_info;
  shader_create_info
      .setModule(module)

      .setPName("main")
      .setStage(stage);
  shader_stage.push_back(shader_create_info);
}
void Pipeline::Make_Resterization() {
  rasterization_info.setCullMode(vk::CullModeFlagBits::eBack)
      .setFrontFace(vk::FrontFace::eClockwise)

      .setLineWidth(1)
      .setPolygonMode(vk::PolygonMode::eFill)
      .setRasterizerDiscardEnable(false);
}
void Pipeline::Make_MultiSample() {
  multi_sample.setSampleShadingEnable(false).setRasterizationSamples(
      vk::SampleCountFlagBits::e1);
}
void Pipeline::Make_DepthTest() {}
void Pipeline::Make_attach() {
  vk::PipelineColorBlendAttachmentState attach;
  attach.setBlendEnable(true)
      .setColorWriteMask(
          vk::ColorComponentFlagBits::eA | vk::ColorComponentFlagBits::eB |
          vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eR)
      .setSrcColorBlendFactor(vk::BlendFactor::eOne)
      .setDstAlphaBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
      .setColorBlendOp(vk::BlendOp::eAdd)
      .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
      .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
      .setAlphaBlendOp(vk::BlendOp::eAdd);
  attachs.push_back(attach);
}
void Pipeline::Make_Blend() {
  blend.setAttachments(attachs).setLogicOpEnable(false);
}

} // namespace MoCheng3D