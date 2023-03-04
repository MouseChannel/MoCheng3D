#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
#include "MoCheng3D/Wrapper/Image.hpp"
namespace MoCheng3D {
class Framebuffer : public Component<vk::Framebuffer, Framebuffer> {
public:
  Framebuffer(Image::Ptr image);
  ~Framebuffer();

private:
};
} // namespace MoCheng3D