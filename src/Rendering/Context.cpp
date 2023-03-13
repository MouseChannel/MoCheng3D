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
#include "MoCheng3D/Wrapper/Sampler.hpp"
#include "MoCheng3D/Wrapper/Semaphore.hpp"
#include "MoCheng3D/Wrapper/ShaderModule.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"
#include "MoCheng3D/Wrapper/Uniform.hpp"
#include "MoCheng3D/Wrapper/Vertex.hpp"
#include <cassert>
#include <limits>
#include <memory>

#include "MoCheng3D/Rendering/Texture.hpp"
namespace MoCheng3D {
const std::array<Vertex, 4> vertices = {
    Vertex { { -0.5, -0.5 }, { 0, 0 } },
    Vertex { { 0.5, -0.5 }, { 1, 0 } },
    Vertex { { 0.5, 0.5 }, { 1, 1 } },
    Vertex { { -0.5, 0.5 }, { 0, 1 } },
};
const std::array<uint32_t, 6> indices = {
    0,
    1,
    3,
    1,
    2,
    3,
};
// const std::array<Mat4, 2> project_view_data;
const std::array<Uniform, 1> color_data { { 0, 0, 1 } };
const Uniform color_data1 { Color { 0, 0, 1 } };
std::unique_ptr<Context> Context::_instance = nullptr;
void Context::Init()
{
    _instance.reset(new Context);
    _instance->Init_Value();
}
void Context::Quit() { _instance.reset(); }

Context& Context::Get_Singleton()
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
    texture.reset(new Texture("D:/MoCheng3D/assets/texture.jpg"));
    sampler.reset(new Sampler());
    Build_pipeline();
    Create_FrameBuffer();
    CreateVertexBuffer();
    CreateMVPMatrix();
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
    for (int i = 0; i < swapchain_image.size(); i++) {
        auto image = swapchain_image[i];
        swapchain_images[i] = Image::Create(image, swapchain->Get_Format());
        frame_buffers[i] = Framebuffer::Create(swapchain_images[i]);
    }
}

void Context::Build_pipeline()
{

    pipeline = Pipeline::Create();
    CreatePipelineLayout();
    vert_shader = ShaderModule::Create("D:/MoCheng3D/Shader/vert.spv");
    frag_shader = ShaderModule::Create("D:/MoCheng3D/Shader/frag.spv");

    auto attr = Vertex::Get_Attr();
    auto binding = Vertex::Get_Binding();
    vk::PipelineVertexInputStateCreateInfo vertex_input_create_info;
    vertex_input_create_info.setVertexBindingDescriptions(binding)
        .setVertexAttributeDescriptions(attr);
    pipeline->Add_Shader_Modules(vert_shader->Get_handle(),
        vk::ShaderStageFlagBits::eVertex);
    pipeline->Add_Shader_Modules(frag_shader->Get_handle(),
        vk::ShaderStageFlagBits::eFragment);

    pipeline->Make_Layout(pipeline_layout);
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
    for (int i = 0; i < 3; i++) {
        render_semaphore.emplace_back(Semaphore::Create());

        present_semaphore.emplace_back(Semaphore::Create());

        fences.emplace_back(Fence::Create());
    }
}
void Context::CreateMVPMatrix()
{
    m_view_matrix = Mat4::CreateIdentity();
    m_project_matrix = Mat4::CreateOrtho(0, 800, 800, 0, -1, 1);
}
void Context::CreateVertexBuffer()
{
    indice_buffer = Buffer::CreateDeviceBuffer((void*)indices.data(), sizeof(indices), vk::BufferUsageFlagBits::eIndexBuffer);
    vertex_buffer = Buffer::CreateDeviceBuffer((void*)vertices.data(), sizeof(vertices),
        vk::BufferUsageFlagBits::eVertexBuffer);
}
void Context::CreateUniformBuffer()
{
    project_view_data[0] = m_project_matrix;
    project_view_data[1] = m_view_matrix;

    auto model = Mat4::CreateTranslate(drawed_rect.pos).Mul(Mat4::CreateScale(drawed_rect.size));
    project_view_data[2] = model;
    uniform_mvp_buffer = Buffer::CreateDeviceBuffer(
        (void*)project_view_data.data(),
        sizeof(project_view_data),
        vk::BufferUsageFlagBits::eUniformBuffer);

    uniform_color_buffer = Buffer::CreateDeviceBuffer(
        (void*)color_data.data(),
        sizeof(color_data),
        vk::BufferUsageFlagBits::eUniformBuffer);
}
void Context::CreateDescriptorSet()
{
    uint32_t uniform_descriptorset_size = 2 * Get_SwapChain()->Get_Swapchain_Image_size();
    uint32_t texture_descriptorset_size = Get_SwapChain()->Get_Swapchain_Image_size();
    descriptorPool_uniform.reset(new DescriptorPool(
        { vk::DescriptorType::eUniformBuffer,
            vk::DescriptorType::eCombinedImageSampler },
        { uniform_descriptorset_size,
            texture_descriptorset_size }));
    descriptorset_uniform = DescriptorSet::Create(descriptorPool_uniform, descriptor_layout);
    descriptorset_uniform->Update(uniform_mvp_buffer, 0, vk::DescriptorType::eUniformBuffer);
    descriptorset_uniform->Update(uniform_color_buffer, 1,
        vk::DescriptorType::eUniformBuffer);
    descriptorset_uniform->Update(texture->GetImage(), 2, vk::DescriptorType::eCombinedImageSampler);
}
void Context::Record_Command_Buffer(uint32_t index)
{
    auto& cmd = command_buffer;

    cmd->Reset();

    vk::RenderPassBeginInfo render_pass_begin_info;
    vk::Rect2D rect;

    rect.setOffset({ 0, 0 }).setExtent({ 800, 800 });
    vk::ClearValue clear_value;
    vk::ClearColorValue clear_color_value;
    clear_color_value.setFloat32({ 0.1, 0.1, 0.1, 1 });
    clear_value.setColor(clear_color_value);
    render_pass_begin_info.setRenderPass(render_pass->Get_handle())
        .setRenderArea(rect)
        .setFramebuffer(frame_buffers[index]->Get_handle())
        .setClearValues(clear_value);
    cmd->Begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    {

        cmd->BeginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
        {
            cmd->BindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
            cmd->BindDescriptorSet(pipeline->GetLayout(),
                descriptorset_uniform->Get_handle()[index]);
            vk::DeviceSize offset = 0;
            auto model = Mat4::CreateTranslate(drawed_rect.pos).Mul(Mat4::CreateScale(drawed_rect.size));
            cmd->PushContants(pipeline_layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(Mat4), (void*)model.GetData());
            cmd->BindVertexBuffer(0, vertex_buffer, offset);
            cmd->BindIndicesBuffer(indice_buffer, 0, vk::IndexType::eUint32);
            cmd->DrawIndex(6, 1, 0, 0, 0);
            // cmd->Draw(6, 1, 0, 0);
        }
        cmd->EndRenderPass();
    }
    cmd->End();
}
void Context::Update()
{
    while (!window->Should_Close()) {
        window->PollEvents();
        auto mov = window->WindowUpdate();
        drawed_rect.pos.y += mov[0] + mov[1];

        drawed_rect.pos.x += mov[2] + mov[3];
        Render();
    }
}

void Context::Render()
{
    auto cur_render_semaphore = render_semaphore[current_frame]->Get_handle();
    auto cur_present_semaphore = present_semaphore[current_frame]->Get_handle();
    auto cur_fence = fences[current_frame]->Get_handle();
    auto result = device->Get_handle().acquireNextImageKHR(
        swapchain->Get_handle(), std::numeric_limits<uint64_t>::max(),
        cur_render_semaphore);

    if (result.result != vk::Result::eSuccess) {
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

    auto fence_res = device->Get_handle().waitForFences(
        cur_fence, true, std::numeric_limits<uint64_t>::max());
    if (fence_res != vk::Result::eSuccess) {
        std::cout << "Wait fence fail" << std::endl;
    }

    device->Get_handle().resetFences(cur_fence);

    vk::PresentInfoKHR present_info;
    present_info.setImageIndices(result.value)
        .setSwapchains(swapchain->Get_handle())
        .setWaitSemaphores(cur_present_semaphore);

    auto present_queue = device->Get_present_queue();

    auto present_result = present_queue.presentKHR(present_info);

    if (present_result != vk::Result::eSuccess) {
        std::cout << "present fail" << std::endl;
    }
    //   present_queue.waitIdle();
    current_frame %= 3;
}
void Context::CreatePipelineLayout()
{

    vk::DescriptorSetLayoutCreateInfo layout_create_info;
    std::vector<vk::DescriptorSetLayoutBinding> layout_bindings {
        Uniform::GetBinding(0, vk::ShaderStageFlagBits::eVertex,
            vk::DescriptorType::eUniformBuffer),
        Uniform::GetBinding(1, vk::ShaderStageFlagBits::eFragment,
            vk::DescriptorType::eUniformBuffer),
        Uniform::GetBinding(2, vk::ShaderStageFlagBits::eFragment, vk::DescriptorType::eCombinedImageSampler)
    };
    layout_create_info.setBindings(layout_bindings);
    descriptor_layout = Get_Device()->Get_handle().createDescriptorSetLayout(layout_create_info);

    vk::PipelineLayoutCreateInfo pipeline_create_info;
    vk::PushConstantRange push_constants_range;
    push_constants_range.setOffset(0).setSize(sizeof(Mat4)).setStageFlags(vk::ShaderStageFlagBits::eVertex);
    pipeline_create_info.setSetLayouts(descriptor_layout).setPushConstantRanges(push_constants_range);
    pipeline_layout = Get_Device()->Get_handle().createPipelineLayout(pipeline_create_info);
}
} // namespace MoCheng3D