#include "MoCheng3D/Wrapper/FrameBuffer.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/RenderPass.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"
namespace MoCheng3D {
Framebuffer::Framebuffer(Image::Ptr image) {

  auto render_pass = Get_Context_Singleton().Get_RenderPass();
  auto h = Get_Context_Singleton().Get_SwapChain()->Get_Extent2D().height;
  auto w = Get_Context_Singleton().Get_SwapChain()->Get_Extent2D().width;

  vk::FramebufferCreateInfo create_info;
  //   auto imageView = image->Get_Image_View();
  create_info.setRenderPass(render_pass->Get_handle())
      .setAttachments(image->Get_Image_View())
      .setHeight(h)
      .setWidth(w)
      .setLayers(1);
  m_handle =
      Get_Context_Singleton().Get_Device()->Get_handle().createFramebuffer(
          create_info);
}
Framebuffer::~Framebuffer() {}
} // namespace MoCheng3D