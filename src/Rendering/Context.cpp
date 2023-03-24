#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Helper/DescriptorManager.hpp"
#include "MoCheng3D/Rendering/GLFW_Window.hpp"
#include "MoCheng3D/Rendering/Model.hpp"
#include "MoCheng3D/Rendering/Render_Context.hpp"
#include "MoCheng3D/Rendering/Render_Frame.hpp"
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

#include "MoCheng3D/Wrapper/Window_Surface.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"
#include <cassert>
#include <limits>
#include <memory>

#include "MoCheng3D/Helper/Camera.hpp"
#include "MoCheng3D/Rendering/Texture.hpp"

namespace MoCheng3D {

const std::array<Uniform, 1> color_data { { 0, 0, 1 } };

std::unique_ptr<Context> Context::_instance = nullptr;
void Context::Init()
{
    _instance.reset(new Context);
}
void Context::Quit()
{

    Descriptor_Manager::_instance.reset();
    _instance->device->Get_handle().waitIdle();

    _instance->model.reset();
    _instance->render_context.reset();

    _instance->sampler.reset();

    _instance->uniform_color_buffer.reset();
    _instance->uniform_mvp_buffer.reset();

    _instance->command_buffer.reset();
    _instance->command_pool.reset();
    _instance->render_pass.reset();
    _instance->pipeline.reset();
    _instance->frag_shader.reset();
    _instance->vert_shader.reset();
    _instance->swapchain.reset();
    _instance->m_surface.reset();
    _instance->m_window.reset();

    _instance->device.reset();
    _instance->instance.reset();
    _instance.reset();
}

Context::~Context() { }
Context& Context::Get_Singleton()
{

    assert(_instance);
    return *_instance;
}

void Context::Init_Vulkan(std::shared_ptr<Window> window)
{
    m_window = window;
    instance.reset(new Instance);
    m_surface.reset(new Surface);
    device.reset(new Device);
    camera.reset(new Camera);

    // create command_buffer
    command_pool.reset(new CommandPool);
    model.reset(new Model("D:/MoCheng3D/assets/model.obj", "D:/MoCheng3D/assets/model.png"));

    command_buffer.reset(new CommandBuffer);
    swapchain.reset(new SwapChain);
    render_pass.reset(new RenderPass);

    // texture.reset(new Texture("D:/MoCheng3D/assets/model.png"));
    sampler.reset(new Sampler());

    CreateUniformBuffer();

    // CreatePipelineLayout();
    CreateDescriptorSet();

    render_context.reset(new RenderContext(device));
    render_context->Prepare();
    Build_pipeline();
}

void Context::Build_pipeline()
{

    pipeline.reset(new Pipeline);

    vert_shader.reset(new ShaderModule("D:/MoCheng3D/Shader/vert.spv"));
    frag_shader.reset(new ShaderModule("D:/MoCheng3D/Shader/frag.spv"));

    // auto attr = Vertex::Get_Attr();
    // auto binding = Vertex::Get_Binding();
    auto attr = model->Get_Attr();
    auto binding = model->Get_Binding();

    pipeline->Add_Shader_Modules(vert_shader->Get_handle(), vk::ShaderStageFlagBits::eVertex);
    pipeline->Add_Shader_Modules(frag_shader->Get_handle(), vk::ShaderStageFlagBits::eFragment);

    pipeline->Make_Layout(Descriptor_Manager::Get_Singleton().Get_DescriptorSet_layout(), sizeof(glm::mat4), vk::ShaderStageFlagBits::eVertex);
    pipeline->Make_VertexInput(binding, attr);
    pipeline->Make_VertexAssembly();
    pipeline->Make_viewPort();
    pipeline->Make_MultiSample();
    pipeline->Make_Resterization();
    pipeline->Make_attach();
    pipeline->Make_Blend();
    pipeline->Make_DepthTest();

    pipeline->Build_Pipeline(Get_RenderPass());
}

void Context::CreateUniformBuffer()
{

    project_view_matrix[0] = camera->Get_p_matrix();
    project_view_matrix[1] = camera->Get_v_matrix();
    project_view_matrix[2] = model->Get_m_matrix();
    uniform_mvp_buffer = Buffer::CreateDeviceBuffer(
        (void*)project_view_matrix.data(), sizeof(project_view_matrix), vk::BufferUsageFlagBits::eUniformBuffer);

    uniform_color_buffer = Buffer::CreateDeviceBuffer(
        (void*)color_data.data(), sizeof(color_data), vk::BufferUsageFlagBits::eUniformBuffer);
}
void Context::CreateDescriptorSet()
{
    Descriptor_Manager::Get_Singleton().Make_DescriptorSet(
        uniform_mvp_buffer, 0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex);
    Descriptor_Manager::Get_Singleton().Make_DescriptorSet(
        uniform_color_buffer, 1, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment);
    Descriptor_Manager::Get_Singleton().Make_DescriptorSet(
        model->get_texture()->GetImage(), 2, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment);

    Descriptor_Manager::Get_Singleton().CreateDescriptorPool();

    Descriptor_Manager::Get_Singleton().CreateUpdateDescriptorSet();
}
std::shared_ptr<RenderPass> Context::Get_RenderPass()
{
    return render_context->Get_render_pass();
}

std::shared_ptr<CommandBuffer> Context::BeginFrame()
{

    model->Update();
    Get_Device()->Get_handle().waitIdle();
    auto cmd = render_context->BeginFrame();
    {
        cmd->BindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

        cmd->BindDescriptorSet(
            pipeline->GetLayout(),
            Descriptor_Manager::Get_Singleton().Get_DescriptorSet()->Get_handle()[render_context->Get_cur_index()]);

        cmd->PushContants(pipeline->GetLayout(),
            vk::ShaderStageFlagBits::eVertex,
            0,
            sizeof(model->Get_m_matrix()),
            (void*)&model->Get_m_matrix());

        cmd->BindVertexBuffer(0, model->Get_vertex_buffer(), 0);
        cmd->BindIndicesBuffer(model->Get_index_buffer(), 0, vk::IndexType::eUint32);
        cmd->DrawIndex(model->Get_index(), 1, 0, 0, 0);
    }
    return cmd;
}
void Context::EndFrame()
{
    render_context->Submit();
    render_context->EndFrame();
}

} // namespace MoCheng3D