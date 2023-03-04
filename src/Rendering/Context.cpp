#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Rendering/GLFW_Window.hpp"
#include "MoCheng3D/Wrapper/CommandBuffer.hpp"
#include "MoCheng3D/Wrapper/CommandPool.hpp"
#include "MoCheng3D/Wrapper/FrameBuffer.hpp"
#include "MoCheng3D/Wrapper/Image.hpp"
#include "MoCheng3D/Wrapper/Instance.hpp"
#include "MoCheng3D/Wrapper/Pipeline.hpp"
#include "MoCheng3D/Wrapper/RenderPass.hpp"
#include "MoCheng3D/Wrapper/ShaderModule.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"
#include <cassert>
#include <limits>
#include <memory>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
std::unique_ptr<Context> Context::_instance = nullptr;
void Context::Init() {
  _instance.reset(new Context);
  _instance->Init_Value();
}
void Context::Quit() { _instance.reset(); }

Context &Context::Get_Singleton() {

  assert(_instance);
  return *_instance;
}

void Context::Init_Value() {
  window = Window::Create(800, 800);
  instance = Instance::Create();
  window->CreateWindowSurface();
  device = Device::Create();

  swapchain = SwapChain::Create();
  render_pass = RenderPass::Create();
  Create_FrameBuffer();
  Build_pipeline();
  Create_Fence_Semaphore();
  // create command_buffer
  command_pool = CommandPool::Create();
  command_buffer = CommandBuffer::Create();
}

void Context::Create_FrameBuffer() {
  auto swapchain_image = swapchain->Get_Swapchian_Images();
  std::vector<Image::Ptr> swapchain_images;
  std::vector<vk::ImageView> swapchain_imageViews;
  for (auto &i : swapchain_image) {

    swapchain_images.emplace_back(Image::Create(i, swapchain->Get_Format()));
    frame_buffers.emplace_back(Framebuffer::Create(swapchain_images.back()));
  }
}
void Context::Build_pipeline() {

  pipeline = Pipeline::Create();
  auto vert_shader = ShaderModule::Create("/home/MoCheng3D/Shader/vert.spv");
  auto frag_shader = ShaderModule::Create("/home/MoCheng3D/Shader/frag.spv");
  pipeline->Add_Shader_Modules(vert_shader->Get_handle(),
                               vk::ShaderStageFlagBits::eVertex);
  pipeline->Add_Shader_Modules(frag_shader->Get_handle(),
                               vk::ShaderStageFlagBits::eFragment);

  pipeline->Make_Layout();
  pipeline->Make_VertexInput({}, {});
  pipeline->Make_VertexAssembly();
  pipeline->Make_viewPort();
  pipeline->Make_MultiSample();
  pipeline->Make_Resterization();
  pipeline->Make_attach();
  pipeline->Make_Blend();
  pipeline->Make_DepthTest();

  pipeline->Build_Pipeline(render_pass);
}
void Context::Create_Fence_Semaphore() {
  vk::SemaphoreCreateInfo semaphore_create_info;
  //   semaphore_create_info.setFlags(vk::SemaphoreCreateFlagBits::)
  render_semaphore =
      device->Get_handle().createSemaphore(semaphore_create_info);

  present_semaphore =
      device->Get_handle().createSemaphore(semaphore_create_info);
  vk::FenceCreateInfo fence_create_info;
  fence = device->Get_handle().createFence(fence_create_info);
}
void Context::Record_Command_Buffer(uint32_t index) {
  auto cmd = command_buffer->Get_handle();

  cmd.reset();
  vk::CommandBufferBeginInfo begin_info;
  begin_info.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
  vk::RenderPassBeginInfo render_pass_begin_info;
  vk::Rect2D rect;

  rect.setOffset({0, 0}).setExtent({800, 800});
  vk::ClearValue clear_value;
  vk::ClearColorValue clear_color_value;
  clear_color_value.setFloat32({0, 0, 0, 0});
  clear_value.setColor(clear_color_value);
  render_pass_begin_info.setRenderPass(render_pass->Get_handle())
      .setRenderArea(rect)
      .setFramebuffer(frame_buffers[index]->Get_handle())
      .setClearValues(clear_value);
  cmd.begin(begin_info);
  {

    cmd.beginRenderPass(render_pass_begin_info, {});
    {
      cmd.bindPipeline(vk::PipelineBindPoint::eGraphics,
                       pipeline->Get_handle());
      cmd.draw(3, 1, 0, 0);
    }
    cmd.endRenderPass();
  }
  cmd.end();
}
void Context::Update() {
  while (!window->Should_Close()) {
    window->PollEvents();
    Render();
  }
}

void Context::Render() {

  auto result = device->Get_handle().acquireNextImageKHR(
      swapchain->Get_handle(), std::numeric_limits<uint64_t>::max(),
      render_semaphore);

  if (result.result != vk::Result::eSuccess) {
    std::cout << "render fail" << std::endl;
  }
  Record_Command_Buffer(result.value);

  auto graphic_queue = device->Get_Graphic_queue();
  vk::SubmitInfo submit_info;
  submit_info.setCommandBuffers(command_buffer->Get_handle())
      .setWaitSemaphores(render_semaphore)
      .setSignalSemaphores(present_semaphore)
      .setWaitDstStageMask(nullptr);
  graphic_queue.submit(submit_info, fence);
  auto fence_res = device->Get_handle().waitForFences(
      fence, true, std::numeric_limits<uint64_t>::max());
  if (fence_res != vk::Result::eSuccess) {
    std::cout << "Wait fence fail" << std::endl;
  }
  device->Get_handle().resetFences(fence);

  vk::PresentInfoKHR present_info;
  present_info.setImageIndices(result.value)
      .setSwapchains(swapchain->Get_handle())
      .setWaitSemaphores(present_semaphore);

  auto present_queue = device->Get_present_queue();

  auto present_result = present_queue.presentKHR(present_info);

  if (present_result != vk::Result::eSuccess) {
    std::cout << "present fail" << std::endl;
  }
}
} // namespace MoCheng3D