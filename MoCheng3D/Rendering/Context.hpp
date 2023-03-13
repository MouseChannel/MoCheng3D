
#pragma once

// #include "MoCheng3D/Rendering/GLFW_Window.hpp"
// #include "MoCheng3D/Wrapper/Device.hpp"
// #include "MoCheng3D/Wrapper/Instance.hpp"

#include "MoCheng3D/Math/math.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

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
class Context {

public:
    static std::unique_ptr<Context> _instance;
    static void Init();
    static void Quit();

    static Context& Get_Singleton();
    [[nodiscard("Missing Instance")]] auto& Get_Instance()
    {
        return instance;
    }
    [[nodiscard("missing Device")]] auto& Get_Device()
    {
        return device;
    }
    [[nodiscard("missing window")]] auto& Get_Window()
    {
        return window;
    }
    [[nodiscard("Missing SwapChain")]] auto& Get_SwapChain()
    {
        return swapchain;
    }
    [[nodiscard("Missing RenderPass")]] auto& Get_RenderPass()
    {
        return render_pass;
    }
    [[nodiscard("Missing CommandPool")]] auto& Get_CommandPool()
    {
        return command_pool;
    }
    [[nodiscard("missing Sampler")]] auto& Get_Sampler() { return sampler; }

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
    std::vector<std::shared_ptr<Image>> swapchain_images;
    std::vector<std::shared_ptr<Framebuffer>> frame_buffers;

    std::shared_ptr<CommandPool> command_pool;
    std::shared_ptr<CommandBuffer> command_buffer;
    std::vector<std::shared_ptr<Semaphore>> render_semaphore;
    std::vector<std::shared_ptr<Semaphore>> present_semaphore;

    std::vector<std::shared_ptr<Fence>> fences;
    uint32_t current_frame = 0;
    std::shared_ptr<Buffer> vertex_buffer;
    std::shared_ptr<Buffer> indice_buffer;
    std::shared_ptr<Buffer> uniform_mvp_buffer;
    std::shared_ptr<Buffer> uniform_color_buffer;

    std::shared_ptr<DescriptorPool> descriptorPool_uniform;

    std::shared_ptr<DescriptorPool> descriptorPool_texture;
    vk::DescriptorSetLayout descriptor_layout;
    std::shared_ptr<DescriptorSet> descriptorset_uniform;
    std::shared_ptr<DescriptorSet> descriptorset_texture;
    std::shared_ptr<Sampler> sampler;

    Rect drawed_rect { .pos { 455, 455 }, .size { 100, 100 } };

private:
    void CreatePipelineLayout();
    void CreateVertexBuffer();
    void CreateUniformBuffer();
    void CreateDescriptorSet();
    void CreateMVPMatrix();
    vk::PipelineLayout pipeline_layout;
    std::array<Mat4, 3> project_view_data;
    Mat4 m_project_matrix;
    Mat4 m_view_matrix;
    std::shared_ptr<Texture> texture;
};
} // namespace MoCheng3D