#include "MoCheng3D/Wrapper/Image.hpp"
#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace MoCheng3D {
Image::Image(Image_Format image_format) {

  auto &device = Get_Context_Singleton().Get_Device();
  vk::ImageCreateInfo create_info;
  vk::Extent3D extent{image_format.width, image_format.height, 1};
  create_info.setImageType(vk::ImageType::e2D)
      .setArrayLayers(1)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setFormat(image_format.format)
      .setUsage(image_format.usage)
      .setExtent(extent);

  m_handle = device->Get_handle().createImage(create_info);
}

Image::Image(vk::Image other_image, vk::Format format) {
  m_handle = other_image;
  Create_ImageView(format);
}

void Image::Create_ImageView(vk::Format format) {
  vk::ImageSubresourceRange range;
  range.setBaseMipLevel(0)
      .setLevelCount(1)
      .setBaseArrayLayer(0)
      .setLayerCount(1)
      .setAspectMask(vk::ImageAspectFlagBits::eColor);
  vk::ImageViewCreateInfo view_create_info;
  view_create_info.setImage(m_handle)
      .setViewType(vk::ImageViewType::e2D)
      .setFormat(format)
      .setSubresourceRange(range);
  image_view =
      Get_Context_Singleton().Get_Device()->Get_handle().createImageView(
          view_create_info);
}

Image::~Image() {}
} // namespace MoCheng3D