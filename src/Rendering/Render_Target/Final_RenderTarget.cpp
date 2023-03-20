#include "MoCheng3D/Rendering/Render_Target/Final_RenderTarget.hpp"
#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/Image.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"

namespace MoCheng3D {
Final_RenderTarget::Final_RenderTarget(std::shared_ptr<Image> image,
                                       vk::AttachmentDescription des)
    : RenderTarget(image, des) {
    clear_color.setColor({ 0.1f, 0.1f, 0.1f, 1.0f });
}
std::unique_ptr<Final_RenderTarget> Final_RenderTarget::Create(std::shared_ptr<Image> image)
{

    auto swapchain_format = Context::Get_Singleton().Get_SwapChain()->Get_Format();

    vk::AttachmentDescription des;
    // auto la = Context::Get_Singleton().Get_SwapChain()->Get_handle().
    auto rer = Context::Get_Singleton().Get_SwapChain()->Get_Swapchain_Image_size();
    des.setFormat(swapchain_format)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setLoadOp(vk::AttachmentLoadOp ::eClear)
        .setStoreOp(vk::AttachmentStoreOp ::eStore)
        .setStencilLoadOp(vk::AttachmentLoadOp ::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp ::eDontCare)
        .setInitialLayout(vk::ImageLayout ::eUndefined)
        .setFinalLayout(vk::ImageLayout ::ePresentSrcKHR);
    return std::unique_ptr<Final_RenderTarget>(new Final_RenderTarget(image, des));
}
void Final_RenderTarget::Make_Subpass(uint32_t attachment_index,
    vk::SubpassDescription& subpass)
{

    attach_reference.setAttachment(attachment_index)
        .setLayout(image->Get_image_layout());
    subpass.setResolveAttachments(attach_reference);
}
}