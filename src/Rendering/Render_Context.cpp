#include "MoCheng3D/Rendering/Render_Context.hpp"
#include "MoCheng3D/Rendering/Render_Frame.hpp"
#include "MoCheng3D/Wrapper/CommandBuffer.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/Fence.hpp"
#include "MoCheng3D/Wrapper/Pipeline.hpp"
#include "MoCheng3D/Wrapper/RenderPass.hpp"
#include "MoCheng3D/Wrapper/Semaphore.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"

namespace MoCheng3D {

RenderContext::RenderContext(std::shared_ptr<Device> device)
    : m_device(device)

{
    m_swapchain = Context::Get_Singleton().Get_SwapChain();
    for (int i = 0; i < Context::Get_Singleton().Get_SwapChain()->Get_Swapchain_Image_size();
         i++) {
        fences.emplace_back(new Fence);
    }

    command_buffer.reset(new CommandBuffer);
}

RenderContext::~RenderContext() { }

void RenderContext::Prepare()
{

    for (int i = 0; i < m_swapchain->Get_Swapchain_Image_size(); i++) {

        // todo need add multisampler stuff
        std::shared_ptr<Image> swapchain_image { new Image(
            m_swapchain->Get_Swapchain_Images()[i], m_swapchain->Get_Format()) };

        vk::AttachmentDescription attach_des;

        attach_des.setFormat(m_swapchain->Get_Format())
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::ePresentSrcKHR)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setSamples(vk::SampleCountFlagBits::e1);
        //

        std::unique_ptr<RenderTarget> render_target { new RenderTarget(swapchain_image, attach_des) };

        render_frames.emplace_back(new RenderFrame(m_device, std::move(render_target)));
    }
}
void RenderContext::Prepare_RenderPass() { m_renderpass.reset(new RenderPass); }

std::shared_ptr<CommandBuffer> RenderContext::BeginFrame()
{
    // auto cur_render_semaphore = Get_RenderFrame(current_frame)->Get_render_semaphore()->Get_handle();
    // auto cur_present_semaphore = Get_RenderFrame(current_frame)->Get_render_semaphore()->Get_handle();
    // auto cur_fence = fences[current_frame]->Get_handle();

    auto result = m_device->Get_handle().acquireNextImageKHR(
        m_swapchain->Get_handle(), std::numeric_limits<uint64_t>::max(),
        Get_cur_render_semaphore()->Get_handle());

    if (result.result != vk::Result::eSuccess) {
        std::cout << "render fail" << std::endl;
    }
    current_index = result.value;
    return Begin_Record_Command_Buffer();
}

//---
std::shared_ptr<CommandBuffer> RenderContext::Begin_Record_Command_Buffer()
{
    auto& cmd = command_buffer;

    cmd->Reset();
    auto render_pass = Context::Get_Singleton().Get_RenderPass();
    vk::RenderPassBeginInfo render_pass_begin_info;
    vk::Rect2D rect;

    rect.setOffset({ 0, 0 }).setExtent({ 800, 800 });
    vk::ClearValue clear_value;
    vk::ClearColorValue clear_color_value;
    clear_color_value.setFloat32({ 0.1, 0.1, 0.1, 1 });
    clear_value.setColor(clear_color_value);
    render_pass_begin_info.setRenderPass(render_pass->Get_handle())
        .setRenderArea(rect)
#ifdef myf
        .setFramebuffer(
            Get_RenderFrame(current_index)->Get_Framebuffer()->Get_handle())
#else
        .setFramebuffer(frame_buffers[index]->Get_handle())
#endif
        .setClearValues(clear_value);
    auto pipeline = Context::Get_Singleton().Get_Pipeline();
    cmd->Begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

    cmd->BeginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
    return cmd;

    //             cmd->BindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
    //             cmd->BindDescriptorSet(pipeline->GetLayout(),
    //                 descriptorset_uniform->Get_handle()[index]);
    //             vk::DeviceSize offset = 0;
    // #ifdef using_glm
    //             cmd->PushContants(pipeline_layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(rotateMatrix), (void*)&rotateMatrix);
    // #else
    //             auto model = Mat4::CreateTranslate(drawed_rect.pos)
    //                              .Mul(Mat4::(drawed_rect.size));

    //             cmd->PushContants(pipeline_layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(model), (void*)model.GetData());

    // #endif
    //             cmd->BindVertexBuffer(0, vertex_buffer, offset);
    //             cmd->BindIndicesBuffer(indice_buffer, 0, vk::IndexType::eUint32);
    //             cmd->DrawIndex(6, 1, 0, 0, 0);
}

void RenderContext::Submit()
{
    End_Record_Command_Buffer();
    auto graphic_queue = m_device->Get_Graphic_queue();
    vk::SubmitInfo submit_info;
    submit_info.setCommandBuffers(command_buffer->Get_handle())
        .setWaitSemaphores(Get_cur_render_semaphore()->Get_handle())
        .setSignalSemaphores(Get_cur_present_semaphore()->Get_handle());
    graphic_queue.submit(submit_info, Get_cur_fence()->Get_handle());
}

//---
void RenderContext::End_Record_Command_Buffer()
{
    auto& cmd = command_buffer;
    cmd->EndRenderPass();
    cmd->End();
}
void RenderContext::EndFrame()
{
    auto fence_res = m_device->Get_handle().waitForFences(
        Get_cur_fence()->Get_handle(), true, std::numeric_limits<uint64_t>::max());
    if (fence_res != vk::Result::eSuccess) {
        std::cout << "Wait fence fail" << std::endl;
    }

    m_device->Get_handle().resetFences(Get_cur_fence()->Get_handle());
    vk::PresentInfoKHR present_info;
    present_info.setImageIndices(current_index)
        .setSwapchains(m_swapchain->Get_handle())
        .setWaitSemaphores(Get_cur_present_semaphore()->Get_handle());

    auto present_queue = m_device->Get_present_queue();

    auto present_result = present_queue.presentKHR(present_info);

    if (present_result != vk::Result::eSuccess) {
        std::cout << "present fail" << std::endl;
    }
    current_frame %= 3;
}
} // namespace MoCheng3D
