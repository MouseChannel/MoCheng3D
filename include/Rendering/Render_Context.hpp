#pragma once

#include "Rendering/Render_Frame.hpp"
#include <vector>
#include <vulkan/vulkan.hpp>

namespace MoCheng3D {
class Device;
class SwapChain;
class RenderFrame;
class RenderPass;
class Fence;
class CommandBuffer;
class RenderContext {
public:
    RenderContext(std::shared_ptr<Device> device);
    ~RenderContext();
    void Prepare();
    [[nodiscard("missing Render_Frame")]] auto&
    Get_RenderFrame(uint32_t index)
    {
        return render_frames[index];
    }
    [[nodiscard("Missing semaphore")]] auto& Get_cur_render_semaphore()
    {
        return Get_RenderFrame(current_frame)->Get_render_semaphore();
    }
    [[nodiscard("Missing semaphore")]] auto& Get_cur_present_semaphore()
    {
        return Get_RenderFrame(current_frame)->Get_present_semaphore();
    }
    [[nodiscard("missing fence")]] auto& Get_cur_fence()
    {
        return fences[current_frame];
    }
    [[nodiscard("missing Render_pass")]] auto Get_render_pass() { return m_renderpass; }
    [[nodiscard]] auto Get_cur_index() { return current_index; }
    std::shared_ptr<CommandBuffer> BeginFrame();
    void Submit();
    void EndFrame();

private:
    std::shared_ptr<Device> m_device;
    std::shared_ptr<SwapChain> m_swapchain;
    std::vector<std::unique_ptr<RenderFrame>> render_frames;
    void Prepare_RenderPass(std::vector<std::shared_ptr<RenderTarget>>& render_targets);
    std::shared_ptr<RenderPass> m_renderpass;
    std::vector<std::shared_ptr<Fence>> fences;
    uint32_t current_frame { 0 };
    uint32_t current_index { 0 };
    std::shared_ptr<CommandBuffer> command_buffer;
    std::shared_ptr<CommandBuffer> Begin_Record_Command_Buffer();
    void End_Record_Command_Buffer();
};

}