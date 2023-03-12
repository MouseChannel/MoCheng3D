#include "MoCheng3D/Rendering/GLFW_Window.hpp"
#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Wrapper/Base.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <vulkan/vulkan_core.h>

namespace MoCheng3D {
Window::Window(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(width, height, "MoCheng3D11", nullptr, nullptr);
    assert(window);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
void Window::PollEvents()
{
    glfwPollEvents();
    WindowUpdate();
}
bool Window::Should_Close() { return glfwWindowShouldClose(window); }
std::vector<int> Window::WindowUpdate()
{
    std::vector<int> res(4);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        res[0] = -1;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        res[1] = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        res[2] = (-1);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        res[3] = (1);
    }
    return res;
}
void Window::CreateWindowSurface()
{
    auto instance = Context::Get_Singleton().Get_Instance();
    VK_CHECK_SUCCESS(glfwCreateWindowSurface(instance->Get_handle(), window,
                         nullptr, &surface_c),
        "Error: failed to create surface");
    surface = surface_c;
}
} // namespace MoCheng3D