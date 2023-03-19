#include "MoCheng3D/Rendering/Context.hpp"
#include "imgui/imgui.h"
#include <memory>
namespace MoCheng3D {
class App {
public:
    App() = default;
    ~App()  ;
    void Init();
    void InitGlfwWindow();
    void InitImGui();
    void InitVulkan();
    void Update();
    void Run();

private:
    std::shared_ptr<Window> window;
    void create_imgui_descriptorpool();
    vk::DescriptorPool descriptor_pool;
    ImGuiIO  io;
};
} // namespace MoCheng3D