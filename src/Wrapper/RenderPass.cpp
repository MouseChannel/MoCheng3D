#include "MoCheng3D/Wrapper/RenderPass.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_structs.hpp>
namespace MoCheng3D {
RenderPass::RenderPass()
{

    
}

void RenderPass::Add_Attachment_description(vk::AttachmentDescription attach_des )
{
    attachment_descriptions.push_back(attach_des);
   
}
void RenderPass::Build()
{
 
    vk::RenderPassCreateInfo create_info;
    vk::SubpassDependency subpass_dependency;
    subpass_dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
        .setDstSubpass(0)
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead)
        .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

    create_info.setAttachments(attachment_descriptions);
    create_info.setSubpasses(subpass);
    create_info.setDependencies(subpass_dependency);
    m_handle = Get_Context_Singleton().Get_Device()->Get_handle().createRenderPass(
        create_info);
}
RenderPass::~RenderPass() { }
} // namespace MoCheng3D