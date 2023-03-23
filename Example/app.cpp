#include "Example/app.hpp"
#include "MoCheng3D/Helper/CommandManager.hpp"
#include "MoCheng3D/Helper/DescriptorManager.hpp"
#include "MoCheng3D/Helper/ImGui_Context.hpp"
#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Rendering/GLFW_Window.hpp"
#include "MoCheng3D/Wrapper/CommandBuffer.hpp"
#include "MoCheng3D/Wrapper/DescriptorPool.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/Instance.hpp"
#include "MoCheng3D/Wrapper/RenderPass.hpp"
#include "MoCheng3D/Wrapper/ShaderModule.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"
#include <memory>
namespace MoCheng3D {

void App::Init()
{
    Context::Init();
    window = Window::Create(800, 800);
    

    Context::Get_Singleton().Init_Vulkan(window);
    imgui.reset(new ImGuiContext);
    imgui->Init(window);
  
}
void App::InitGlfwWindow() { window = Window::Create(800, 800); }
 
 

void App::Run()
{
   

    auto& instance = Context::Get_Singleton();
    while (!window->Should_Close()) {
     
        window->PollEvents();
        auto cmd = instance.BeginFrame();
        imgui->Update(cmd);
    
        instance.EndFrame();
    }
    Context::Get_Singleton().Get_Device()->Get_handle().waitIdle();
}
App::~App()
{

    // Context::Get_Singleton().Get_Device()->Get_handle().destroyDescriptorPool(descriptor_pool);
    // ImGui_ImplVulkan_Shutdown();
    // ImGui_ImplGlfw_Shutdown();
    // ImGui::DestroyContext();
    imgui.reset();
    Context::Quit();
}
} // namespace MoCheng3D