#pragma once

#include "MoCheng3D/Rendering/Render_Target.hpp"
#include <vector>
#include <vulkan/vulkan.hpp>

namespace MoCheng3D {
class Device;
class Framebuffer;
class Semaphore;
class RenderFrame {

public:
    RenderFrame(std::shared_ptr<Device> device, std::unique_ptr<RenderTarget> render_target);
    ~RenderFrame();
    [[nodiscard("Missing FrameBuffer")]] auto& Get_Framebuffer()
    {
        return m_framebuffer;
    }
    [[nodiscard("missing render_semaphore")]] auto& Get_render_semaphore()
    {
        return render_semaphore;
    }

    [[nodiscard("missing present_semaphore")]] auto& Get_present_semaphore() { return present_semaphore; }

private:
    std::vector<std::unique_ptr<RenderTarget>> render_targets;
    std::shared_ptr<Device> m_device;
    std::shared_ptr<Framebuffer> m_framebuffer;
    std::shared_ptr<Semaphore> render_semaphore;

    std::shared_ptr<Semaphore> present_semaphore;
};

}