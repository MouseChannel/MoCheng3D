#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include "MoCheng3D/Wrapper/Image.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class Framebuffer : public Component<vk::Framebuffer, Framebuffer> {
public:
    Framebuffer(std::vector<std::shared_ptr<Image>> images);
    ~Framebuffer();

private:
};
} // namespace MoCheng3D