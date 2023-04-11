#include "Wrapper/Window_Surface.hpp"
#include "Rendering/GLFW_Window.hpp"
#include "Wrapper/Instance.hpp"

namespace MoCheng3D {

Surface::Surface()
{

    auto vk_instance = Get_Context_Singleton()->Get_Instance();
    auto glfw_window = Get_Context_Singleton()->Get_Window();
    VK_CHECK_SUCCESS(glfwCreateWindowSurface(vk_instance->Get_handle(), glfw_window->get_window(),
                         nullptr, (VkSurfaceKHR*)(&m_handle)),
        "Error: failed to create surface");
}
Surface::~Surface()
{
    Get_Context_Singleton()->Get_Instance()->Get_handle().destroySurfaceKHR(m_handle);
}
}