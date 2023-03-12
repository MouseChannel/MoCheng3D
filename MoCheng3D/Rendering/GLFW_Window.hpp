#pragma once
#include "MoCheng3D/Wrapper/Component.hpp"
#include "MoCheng3D/Wrapper/Instance.hpp"
#include <vulkan/vulkan_handles.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

namespace MoCheng3D {
class Window {

private:
  GLFWwindow *window;
  vk::SurfaceKHR surface;
  VkSurfaceKHR surface_c{nullptr};

public:
  using Ptr = std::shared_ptr<Window>;
  static Ptr Create(int width, int height) {
    return std::make_shared<Window>(width, height);
  }
  Window(int width, int height);
  ~Window();
  void PollEvents();
  std::vector<int> WindowUpdate();
  bool Should_Close();
  void CreateWindowSurface();
  [[nodiscard]] auto GetSurface() { return surface; }
};
} // namespace MoCheng3D