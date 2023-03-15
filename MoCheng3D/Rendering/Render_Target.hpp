#pragma once
#include "MoCheng3D/Wrapper/Image.hpp"
#include "vulkan/vulkan.hpp"

namespace MoCheng3D {
class RenderTarget {
public:
    [[nodiscard("Missing format")]] auto &Get_Extent() { return extent; }
    [[nodiscard("Missing Image")]] auto &Get_Image() { return image; }
    RenderTarget(std::shared_ptr<Image>, vk::AttachmentDescription);

protected:
    vk::Extent2D extent;

    std::shared_ptr<Image> image;
    vk::AttachmentDescription attachment_description;
};

}