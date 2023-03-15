#include "MoCheng3D/Rendering/Render_Frame.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/FrameBuffer.hpp"
#include "MoCheng3D/Wrapper/Semaphore.hpp"
namespace MoCheng3D {

RenderFrame::RenderFrame(std::shared_ptr<Device> device,
    std::unique_ptr<RenderTarget> render_target)
    : m_device(device)
{
    render_targets.push_back(std::move(render_target));
    m_framebuffer.reset(new Framebuffer(render_targets[0]->Get_Image()));

    render_semaphore.reset(new Semaphore);
    present_semaphore.reset(new Semaphore);
}
RenderFrame::~RenderFrame() { }

}