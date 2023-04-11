#include "Rendering/Context.hpp"

#include <memory>
namespace MoCheng3D {
class ImGuiContext;
class App {
public:
    App() = default;
    ~App();
    void Init();
    void InitGlfwWindow();

    void InitVulkan();
    void Update();
    void Run();

private:
    std::shared_ptr<Window> window;

    std::unique_ptr<ImGuiContext> imgui;
};
} // namespace MoCheng3D