#include "MoCheng3D/Wrapper/RenderPass.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_structs.hpp>
namespace MoCheng3D {
RenderPass::RenderPass() {

  vk::AttachmentDescription attach_des;
  auto format = Get_Context_Singleton().Get_SwapChain()->Get_Format();
  attach_des.setFormat(format)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setFinalLayout(vk::ImageLayout::ePresentSrcKHR)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setStoreOp(vk::AttachmentStoreOp::eStore)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setSamples(vk::SampleCountFlagBits::e1);

  vk::RenderPassCreateInfo create_info;

  vk::AttachmentReference attach_reference;
  attach_reference.setAttachment(0).setLayout(
      vk::ImageLayout::eColorAttachmentOptimal);

  vk::SubpassDescription subpass;
  subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
      .setColorAttachments(attach_reference);

  vk::SubpassDependency subpass_dependency;
  subpass_dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
      .setDstSubpass(0)
      .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
      .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
      .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

  create_info.setAttachments(attach_des);
  create_info.setSubpasses(subpass);
  create_info.setDependencies(subpass_dependency);
  m_handle =
      Get_Context_Singleton().Get_Device()->Get_handle().createRenderPass(
          create_info);
}

RenderPass::~RenderPass() {}
} // namespace MoCheng3D