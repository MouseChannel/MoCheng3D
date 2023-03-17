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
#include "MoCheng3D/Wrapper/Vertex.hpp"
#include <cassert>
#include <limits>
#include <memory>

#include "MoCheng3D/Helper/Camera.hpp"
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
    camera.reset(new Camera);

    // create command_buffer
    command_pool = CommandPool::Create();
    model.reset(new Model("D:/MoCheng3D/assets/model.obj"));

    command_buffer = CommandBuffer::Create();
    swapchain = SwapChain::Create();
    render_pass = RenderPass::Create();

    texture.reset(new Texture("D:/MoCheng3D/assets/model.png"));
    sampler.reset(new Sampler());
    CreateUniformBuffer();

    CreateVertexBuffer();
    CreateMVPMatrix();
    CreatePipelineLayout();
    CreateDescriptorSet();

    render_context.reset(new RenderContext(device));
    render_context->Prepare();
    Build_pipeline();
}

void Context::Build_pipeline()
{

    pipeline = Pipeline::Create();

    vert_shader = ShaderModule::Create("D:/MoCheng3D/Shader/vert.spv");
    frag_shader = ShaderModule::Create("D:/MoCheng3D/Shader/frag.spv");

    // auto attr = Vertex::Get_Attr();
    // auto binding = Vertex::Get_Binding();
    auto attr = model->Get_Attr();
    auto binding = model->Get_Binding();
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

    pipeline->Build_Pipeline(Get_RenderPass());
}

void Context::CreateMVPMatrix()
{
    m_view_matrix = Mat4::CreateIdentity();
    m_project_matrix = Mat4::CreateOrtho(0, 800, 800, 0, -1, 1);
}
void Context::CreateVertexBuffer()
{
    // indice_buffer = Buffer::CreateDeviceBuffer((void*)indices.data(),
    // sizeof(indices), vk::BufferUsageFlagBits::eIndexBuffer); vertex_buffer =
    // Buffer::CreateDeviceBuffer((void*)vertices.data(), sizeof(vertices),
    //     vk::BufferUsageFlagBits::eVertexBuffer);
    indice_buffer = model->Get_index_buffer();
    vertex_buffers = model->Get_vertex_buffer();
}
void Context::CreateUniformBuffer()
{

#ifdef using_glm
    project_view_matrix[0] = project_matrix;
    project_view_matrix[1] = view_matrix;

    project_view_matrix[2] = m_Matrix;
    //
    project_view_matrix[0] = camera->Get_p_matrix();
    project_view_matrix[1] = camera->Get_v_matrix();

    project_view_matrix[2] = m_Matrix;

    // project_view_matrix[0] = camera->Get_p_matrix();
    // project_view_matrix[1] = camera->Get_v_matrix();
    // project_view_matrix[2] = model->Get_m_matrix();
    uniform_mvp_buffer = Buffer::CreateDeviceBuffer(
        (void*)project_view_matrix.data(),
        sizeof(project_view_matrix),
        vk::BufferUsageFlagBits::eUniformBuffer);

#else

    project_view_data[0] = m_project_matrix;
    project_view_data[1] = m_view_matrix;

    auto model = Mat4::CreateTranslate(drawed_rect.pos).Mul(Mat4::CreateScale(drawed_rect.size));
    project_view_data[2] = model;
    uniform_mvp_buffer = Buffer::CreateDeviceBuffer(
        (void*)project_view_data.data(),
        sizeof(project_view_data),
        vk::BufferUsageFlagBits::eUniformBuffer);

#endif
    uniform_color_buffer = Buffer::CreateDeviceBuffer(
        (void*)color_data.data(), sizeof(color_data),
        vk::BufferUsageFlagBits::eUniformBuffer);
}
void Context::CreateDescriptorSet()
{

    Descriptor_Manager::Get_Singleton().CreateDescriptorPool();

    Descriptor_Manager::Get_Singleton().CreateUpdateDescriptorSet();
}
std::shared_ptr<RenderPass> Context::Get_RenderPass() { return render_context->Get_render_pass(); }

void Context::Update()
{
    while (!window->Should_Close()) {
        window->PollEvents();
        auto mov = window->WindowUpdate();
        drawed_rect.pos.y += mov[0] + mov[1];

        drawed_rect.pos.x += mov[2] + mov[3];
        ModelMatrixUpdate();

        auto cmd = render_context->BeginFrame();
        {
            cmd->BindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
            cmd->BindDescriptorSet(pipeline->GetLayout(),
                Descriptor_Manager::Get_Singleton().Get_DescriptorSet()->Get_handle()[render_context->Get_cur_index()]);
            vk::DeviceSize offset = 0;

            cmd->PushContants(pipeline_layout, vk::ShaderStageFlagBits::eVertex,
                0, sizeof(model->Get_m_matrix()),
                (void*)&model->Get_m_matrix());
            // cmd->PushContants(pipeline_layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(m_Matrix), (void*)&m_Matrix);

            cmd->BindVertexBuffer(0, vertex_buffers, offset);
            cmd->BindIndicesBuffer(indice_buffer, 0, vk::IndexType::eUint32);
            cmd->DrawIndex(model->Get_index(), 1, 0, 0, 0);
        }
        render_context->Submit();
        render_context->EndFrame();
    }
}

void Context::CreatePipelineLayout()
{

    Descriptor_Manager::Get_Singleton().Make_DescriptorSet(
        uniform_mvp_buffer, 0, vk::DescriptorType::eUniformBuffer,
        vk::ShaderStageFlagBits::eVertex);
    Descriptor_Manager::Get_Singleton().Make_DescriptorSet(
        uniform_color_buffer, 1, vk::DescriptorType::eUniformBuffer,
        vk::ShaderStageFlagBits::eFragment);
    Descriptor_Manager::Get_Singleton().Make_DescriptorSet(
        texture->GetImage(), 2, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment);

    descriptor_layout = Descriptor_Manager::Get_Singleton().Get_DescriptorSet_layout();
    vk::PipelineLayoutCreateInfo pipeline_create_info;
    vk::PushConstantRange push_constants_range;
    push_constants_range.setOffset(0).setSize(sizeof(Mat4)).setStageFlags(vk::ShaderStageFlagBits::eVertex);
    pipeline_create_info.setSetLayouts(descriptor_layout).setPushConstantRanges(push_constants_range);
    pipeline_layout = Get_Device()->Get_handle().createPipelineLayout(pipeline_create_info);
}
void Context::ModelMatrixUpdate()
{
//  rotateMatrix = glm::mat4(1.0f);
#ifdef using_glm
    m_Matrix = glm::rotate(m_Matrix, glm::radians(mAngle),
        glm::vec3(0.0f, 0.0f, 1.0f));

    mAngle = 0.1f;
#endif
}
} // namespace MoCheng3D