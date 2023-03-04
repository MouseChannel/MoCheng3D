#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/RenderPass.hpp"
#include <vulkan/vulkan_structs.hpp>
#pragma once
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>

namespace MoCheng3D {
class Pipeline : public Component<vk::Pipeline, Pipeline> {

public:
  Pipeline();
  ~Pipeline();
  void Make_Layout();
  void Make_VertexInput(std::vector<vk::VertexInputBindingDescription> des,
                        std::vector<vk::VertexInputAttributeDescription> attr);
  void Make_VertexAssembly();
  void Make_viewPort();
  void Add_Shader_Modules(vk::ShaderModule shader_module,
                          vk::ShaderStageFlagBits stage);
  void Make_Resterization();
  void Make_MultiSample();
  void Make_DepthTest();
  void Make_Blend();
  void Make_attach();

  void Build_Pipeline(RenderPass::Ptr render_pass);

private:
  vk::PipelineVertexInputStateCreateInfo input_state;

  vk::PipelineInputAssemblyStateCreateInfo input_assembly;

  vk::PipelineViewportStateCreateInfo viewportInfo;
  std::vector<vk::PipelineShaderStageCreateInfo> shader_stage;
  vk::PipelineRasterizationStateCreateInfo rasterization_info;
  vk::PipelineMultisampleStateCreateInfo multi_sample;
  vk::PipelineDepthStencilStateCreateInfo depth_test;
  vk::PipelineColorBlendStateCreateInfo blend;
  std::vector<vk::PipelineColorBlendAttachmentState> attachs;
  vk::PipelineLayout layout;

  vk::Viewport viewport;
  vk::Rect2D scissor;
};
} // namespace MoCheng3D