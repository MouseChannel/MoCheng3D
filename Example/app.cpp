#include "Example/app.hpp"
#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Rendering/GLFW_Window.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/Instance.hpp"
#include "MoCheng3D/Wrapper/ShaderModule.hpp"
#include <GLFW/glfw3.h>
#include <memory>
namespace MoCheng3D {

void App::Init()
{
    InitWindow();
    InitVulkan();
    Context::Init();
    //   auto mm = ShaderModule::ReadBinary("/home/MoCheng3D/Shader/frag.frag");
    //   instance = std::make_shared<Instance>();
    //   device = std::make_shared<Device>();
}
void App::InitWindow() { }
void App::InitVulkan() { }
void App::Run()
{

    vk::DescriptorSetLayout ss;
    if (!ss) {
        std::cout << "here" << std::endl;
    }
    auto& instance = Context::Get_Singleton();
    instance.Update();
}
} // namespace MoCheng3D