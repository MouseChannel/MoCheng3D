#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Rendering/GLFW_Window.hpp"
#include "MoCheng3D/Wrapper/Buffer.hpp"
#include "MoCheng3D/Wrapper/CommandBuffer.hpp"
#include "MoCheng3D/Wrapper/CommandPool.hpp"
#include "MoCheng3D/Wrapper/DescriptorPool.hpp"
#include "MoCheng3D/Wrapper/DescriptorSet.hpp"
#include "MoCheng3D/Wrapper/Fence.hpp"
#include "MoCheng3D/Wrapper/FrameBuffer.hpp"
#include "MoCheng3D/Wrapper/Image.hpp"
#include "MoCheng3D/Wrapper/Instance.hpp"
#include "MoCheng3D/Wrapper/Pipeline.hpp"
#include "MoCheng3D/Wrapper/RenderPass.hpp"
#include "MoCheng3D/Wrapper/Semaphore.hpp"
#include "MoCheng3D/Wrapper/ShaderModule.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"
#include "MoCheng3D/Wrapper/Uniform.hpp"
#include "MoCheng3D/Wrapper/Vertex.hpp"
#include <cassert>
#include <limits>
#include <memory>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
namespace MoCheng3D
{
const std::array<Vertex, 3> vertices = {
    Vertex{0.0, -0.1},
    Vertex{0.5, 0.5},
    Vertex{-0.5, 0.5},
};
const Uniform uniform{Color{0, 0, 1}};
std::unique_ptr<Context> Context::_instance = nullptr;
void Context::Init()
{
    _instance.reset(new Context);
    _instance->Init_Value();
}
void Context::Quit()
{
    _instance.reset();
}

Context &Context::Get_Singleton()
{

    assert(_instance);
    return *_instance;
}

void Context::Init_Value()
{
    window = Window::Create(800, 800);
    instance = Instance::Create();
    window->CreateWindowSurface();
    device = Device::Create();
    // create command_buffer
    command_pool = CommandPool::Create();
    command_buffer = CommandBuffer::Create();
    swapchain = SwapChain::Create();
    render_pass = RenderPass::Create();
    Build_pipeline();
    Create_FrameBuffer();
    CreateVertexBuffer();
    CreateUniformBuffer();
    CreateDescriptorSet();

    Create_Fence_Semaphore();
}

void Context::Create_FrameBuffer()
{
    auto swapchain_image = swapchain->Get_Swapchain_Images();
    swapchain_images.resize(3);
    frame_buffers.resize(3);
    //   std::vector<vk::ImageView> swapchain_imageViews;
    for (int i = 0; i < swapchain_image.size(); i++)
    {
        auto image = swapchain_image[i];
        swapchain_images[i] = Image::Create(image, swapchain->Get_Format());
        frame_buffers[i] = Framebuffer::Create(swapchain_images[i]);
    }
}

void Context::Build_pipeline()
{

    pipeline = Pipeline::Create();
    vert_shader = ShaderModule::Create("D:/MoCheng3D/Shader/vert.spv");
    frag_shader = ShaderModule::Create("D:/MoCheng3D/Shader/frag.spv");

    auto attr = Vertex::Get_Attr();
    auto binding = Vertex::Get_Binding();
    vk::PipelineVertexInputStateCreateInfo vertex_input_create_info;
    vertex_input_create_info.setVertexBindingDescriptions(binding).setVertexAttributeDescriptions(attr);
    pipeline->Add_Shader_Modules(vert_shader->Get_handle(), vk::ShaderStageFlagBits::eVertex);
    pipeline->Add_Shader_Modules(frag_shader->Get_handle(), vk::ShaderStageFlagBits::eFragment);

    pipeline->Make_Layout(CreatePipelineLayout());
    pipeline->Make_VertexInput(binding, attr);
    pipeline->Make_VertexAssembly();
    pipeline->Make_viewPort();
    pipeline->Make_MultiSample();
    pipeline->Make_Resterization();
    pipeline->Make_attach();
    pipeline->Make_Blend();
    pipeline->Make_DepthTest();

    pipeline->Build_Pipeline(render_pass);
}
void Context::Create_Fence_Semaphore()
{
    //   semaphore_create_info.setFlags(vk::SemaphoreCreateFlagBits::e);
    for (int i = 0; i < 3; i++)
    {
        render_semaphore.emplace_back(Semaphore::Create());

        present_semaphore.emplace_back(Semaphore::Create());

        fences.emplace_back(Fence::Create());
    }
}
void Context::CreateVertexBuffer()
{
    vertex_buffer =
        Buffer::CreateDeviceBuffer((void *)vertices.data(), sizeof(vertices), vk::BufferUsageFlagBits::eVertexBuffer);
}
void Context::CreateUniformBuffer()
{
    uniform_buffer =
        Buffer::CreateDeviceBuffer((void *)&uniform, sizeof(uniform), vk::BufferUsageFlagBits::eUniformBuffer);
}
void Context::CreateDescriptorSet()
{
    descriptorPool = DescriptorPool::Create();
    descriptorset = DescriptorSet::Create(descriptorPool, descriptor_layout);
    descriptorset->Update(uniform_buffer);
}
void Context::Record_Command_Buffer(uint32_t index)
{
    auto &cmd = command_buffer;

    cmd->Reset();
    vk::CommandBufferBeginInfo begin_info;
    begin_info.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    vk::RenderPassBeginInfo render_pass_begin_info;
    vk::Rect2D rect;

    rect.setOffset({0, 0}).setExtent({800, 800});
    vk::ClearValue clear_value;
    vk::ClearColorValue clear_color_value;
    clear_color_value.setFloat32({0.1, 0.1, 0.1, 1});
    clear_value.setColor(clear_color_value);
    render_pass_begin_info.setRenderPass(render_pass->Get_handle())
        .setRenderArea(rect)
        .setFramebuffer(frame_buffers[index]->Get_handle())
        .setClearValues(clear_value);
    cmd->Begin(begin_info);
    {

        cmd->BeginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
        {
            cmd->BindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
            cmd->BindUniformBuffer(pipeline->GetLayout(), descriptorset->Get_handle()[index]);
            vk::DeviceSize offset = 0;
            cmd->BindVertexBuffer(0, vertex_buffer, offset);
            cmd->Draw(3, 1, 0, 0);
        }
        cmd->EndRenderPass();
    }
    cmd->End();
}
void Context::Update()
{
    while (!window->Should_Close())
    {
        window->PollEvents();
        Render();
    }
}

void Context::Render()
{
    auto cur_render_semaphore = render_semaphore[current_frame]->Get_handle();
    auto cur_present_semaphore = present_semaphore[current_frame]->Get_handle();
    auto cur_fence = fences[current_frame]->Get_handle();
    auto result = device->Get_handle().acquireNextImageKHR(swapchain->Get_handle(),
                                                           std::numeric_limits<uint64_t>::max(), cur_render_semaphore);

    if (result.result != vk::Result::eSuccess)
    {
        std::cout << "render fail" << std::endl;
    }
    Record_Command_Buffer(result.value);

    auto graphic_queue = device->Get_Graphic_queue();
    vk::SubmitInfo submit_info;
    submit_info.setCommandBuffers(command_buffer->Get_handle())
        .setWaitSemaphores(cur_render_semaphore)
        .setSignalSemaphores(cur_present_semaphore);
    graphic_queue.submit(submit_info, cur_fence);
    //   device->Get_handle().waitIdle();;

    //   graphic_queue.waitIdle();

    auto fence_res = device->Get_handle().waitForFences(cur_fence, true, std::numeric_limits<uint64_t>::max());
    if (fence_res != vk::Result::eSuccess)
    {
        std::cout << "Wait fence fail" << std::endl;
    }

    device->Get_handle().resetFences(cur_fence);

    vk::PresentInfoKHR present_info;
    present_info.setImageIndices(result.value)
        .setSwapchains(swapchain->Get_handle())
        .setWaitSemaphores(cur_present_semaphore);

    auto present_queue = device->Get_present_queue();

    auto present_result = present_queue.presentKHR(present_info);

    if (present_result != vk::Result::eSuccess)
    {
        std::cout << "present fail" << std::endl;
    }
    //   present_queue.waitIdle();
    current_frame %= 3;
}
vk::PipelineLayout Context::CreatePipelineLayout()
{
    auto layout_bindings = Uniform::GetBinding();
    vk::DescriptorSetLayoutCreateInfo layout_create_info;

    layout_create_info.setBindings(layout_bindings);
    descriptor_layout = Get_Device()->Get_handle().createDescriptorSetLayout(layout_create_info);

    vk::PipelineLayoutCreateInfo pipeline_create_info;
    pipeline_create_info.setSetLayouts(descriptor_layout);
    return Get_Device()->Get_handle().createPipelineLayout(pipeline_create_info);
}
} // namespace MoCheng3D