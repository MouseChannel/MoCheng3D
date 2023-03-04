#pragma once

// #include "MoCheng3D/Rendering/GLFW_Window.hpp"
// #include "MoCheng3D/Wrapper/Device.hpp"
// #include "MoCheng3D/Wrapper/Instance.hpp"

#include <cstddef>
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

namespace MoCheng3D {
class Window;
class Instance;
class Device;
class SwapChain;
class Image;
class ShaderModule;
class Pipeline;
class RenderPass;
class Framebuffer;
class CommandPool;
class CommandBuffer;
class Context {

public:
  static std::unique_ptr<Context> _instance;
  static void Init();
  static void Quit();

  static Context &Get_Singleton();
  [[nodiscard("Missing Instance")]] auto &Get_Instance() { return instance; }
  [[nodiscard("missing Device")]] auto &Get_Device() { return device; }
  [[nodiscard("missing window")]] auto &Get_Window() { return window; }
  [[nodiscard("Missing SwapChain")]] auto &Get_SwapChain() { return swapchain; }
  [[nodiscard("Missing RenderPass")]] auto &Get_RenderPass() {
    return render_pass;
  }
  [[nodiscard("Missing CommandPool")]] auto &Get_CommandPool() {
    return command_pool;
  }
  Context() = default;
  void Init_Value();
  void Build_pipeline();
  void Create_FrameBuffer();
  void Create_Fence_Semaphore();
  void Render();
  void Update();
  void Record_Command_Buffer(uint32_t index);

  std::shared_ptr<Window> window;
  std::shared_ptr<Instance> instance;
  std::shared_ptr<Device> device;
  std::shared_ptr<SwapChain> swapchain;
  std::shared_ptr<ShaderModule> vert_shader;

  std::shared_ptr<ShaderModule> frag_shader;
  std::shared_ptr<Pipeline> pipeline;
  std::shared_ptr<RenderPass> render_pass;
  std::vector<std::shared_ptr<Framebuffer>> frame_buffers;
  std::shared_ptr<CommandPool> command_pool;
  std::shared_ptr<CommandBuffer> command_buffer;
  vk::Semaphore render_semaphore;
  vk::Semaphore present_semaphore;
  vk::Fence fence;

private:
};
} // namespace MoCheng3D