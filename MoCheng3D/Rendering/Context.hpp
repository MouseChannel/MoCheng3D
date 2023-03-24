
#pragma once

#include "MoCheng3D/Helper/Instance_base.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
class Semaphore;
class Fence;

class Buffer;
class DescriptorPool;
class DescriptorSet;
class Texture;
class Sampler;
class RenderContext;
class Model;
class Camera;
class Surface;
class Context : public Instance_base<Context> {

public:
    // static std::unique_ptr<Context> _instance;
    // static void Init();
    static void Quit();

    // static Context& Get_Singleton();
    [[nodiscard("Missing Instance")]] auto& Get_Instance()
    {
        return instance;
    }
    [[nodiscard("missing Device")]] auto& Get_Device()
    {
        return device;
    }
    [[nodiscard("missing window")]] auto& Get_Window() { return m_window; }
    [[nodiscard("missing surface")]] auto& Get_Surface() { return m_surface; }
    [[nodiscard("Missing SwapChain")]] auto& Get_SwapChain()
    {
        return swapchain;
    }

    [[nodiscard("Missing RenderPass")]] std::shared_ptr<RenderPass> Get_RenderPass();

    [[nodiscard("Missing CommandPool")]] auto& Get_CommandPool()
    {
        return command_pool;
    }
    [[nodiscard("missing Sampler")]] auto& Get_Sampler() { return sampler; }
    [[nodiscard("missing pipeline")]] auto& Get_Pipeline() { return pipeline; }

    Context();
    ~Context();
    void Init_Vulkan(std::shared_ptr<Window> window);
    void Build_pipeline();

    std::shared_ptr<CommandBuffer> BeginFrame();
    void EndFrame();

    std::shared_ptr<Window> m_window;
    std::shared_ptr<Surface> m_surface;
    std::shared_ptr<Instance> instance;
    std::shared_ptr<Device> device;
    std::shared_ptr<SwapChain> swapchain;
    std::shared_ptr<ShaderModule> vert_shader;

    std::shared_ptr<ShaderModule> frag_shader;
    std::shared_ptr<Pipeline> pipeline;
    std::shared_ptr<RenderPass> render_pass;

    std::shared_ptr<CommandPool> command_pool;
    std::shared_ptr<CommandBuffer> command_buffer;

    // uint32_t current_frame = 0;

    std::shared_ptr<Buffer> uniform_mvp_buffer;
    std::shared_ptr<Buffer> uniform_color_buffer;

    std::shared_ptr<DescriptorPool> descriptorPool_uniform;

    std::shared_ptr<Sampler> sampler;

private:
    void CreateUniformBuffer();
    void CreateDescriptorSet();
    void CreateMVPMatrix();

    float mAngle = 0;

    std::array<glm::mat4, 3> project_view_matrix;

    std::unique_ptr<RenderContext> render_context;
    std::unique_ptr<Model> model;
    std::unique_ptr<Camera> camera;
};
} // namespace MoCheng3D