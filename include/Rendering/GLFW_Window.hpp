#pragma once
#include "Wrapper/Component.hpp"
#include "Wrapper/Instance.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

namespace MoCheng3D {
class Window {

private:
    GLFWwindow* window;

public:
    using Ptr = std::shared_ptr<Window>;
    static Ptr Create(int width, int height)
    {
        return std::make_shared<Window>(width, height);
    }
    [[nodiscard("Missing window")]] auto& get_window() { return window; }
    Window(int width, int height);
    ~Window();
    void PollEvents();
    std::vector<int> WindowUpdate();
    bool Should_Close();
    void CreateWindowSurface();
};
} // namespace MoCheng3D