#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <sys/types.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
// #include "vulkan/vulkan.hpp"
// #include <vulkan/vulkan_handles.hpp>

namespace MoCheng3D {
class Image : public Component<vk::Image, Image> {
public:
  struct Image_Format {
    uint32_t width;
    uint32_t height;
    vk::Format format = vk::Format::eR8G8B8A8Srgb;
    vk::ImageType type;
    vk::ImageTiling tiling;
    vk::ImageUsageFlagBits usage;
    vk::SampleCountFlagBits sample;
  };

  Image(Image_Format image_format);
  Image(vk::Image other_image, vk::Format format);
  ~Image();
  void Create_ImageView(vk::Format format = vk::Format::eR8G8B8A8Srgb);
  [[nodiscard("Missing ImageView")]] auto &Get_Image_View() {
    return image_view;
  }

private:
  vk::ImageView image_view;
};
} // namespace MoCheng3D