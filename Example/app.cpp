#include "Example/app.hpp"
#include "Helper/CommandManager.hpp"
#include "Helper/DescriptorManager.hpp"
#include "Helper/ImGui_Context.hpp"
#include "Rendering/Context.hpp"
#include "Rendering/GLFW_Window.hpp"
#include "Wrapper/CommandBuffer.hpp"
#include "Wrapper/DescriptorPool.hpp"
#include "Wrapper/Device.hpp"
#include "Wrapper/Instance.hpp"
#include "Wrapper/RenderPass.hpp"
#include "Wrapper/ShaderModule.hpp"
#include "Wrapper/SwapChain.hpp"
#include <memory>
namespace MoCheng3D {

void App::Init()
{
    // Context::Init();
    window = Window::Create(800, 800);

    Context::Get_Singleton()->Init_Vulkan(window);
    imgui.reset(new ImGuiContext);
    imgui->Init(window);
}
void App::InitGlfwWindow() { window = Window::Create(800, 800); }

void App::Run()
{

    auto& instance = Context::Get_Singleton();
    while (!window->Should_Close()) {

        window->PollEvents();
        auto cmd = instance->BeginFrame();
        imgui->Update(cmd);

        instance->EndFrame();
    }
    Context::Get_Singleton()->Get_Device()->Get_handle().waitIdle();
}
App::~App()
{

    imgui.reset();
    Context::Get_Singleton()->Quit();
}
} // namespace MoCheng3D