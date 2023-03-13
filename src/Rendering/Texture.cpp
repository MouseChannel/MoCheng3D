#include "MoCheng3D/Rendering/Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "MoCheng3D/Tool/stb_image.hpp"
#include "MoCheng3D/Wrapper/Buffer.hpp"
#include "MoCheng3D/Wrapper/Image.hpp"

namespace MoCheng3D {

Texture::Texture(std::string_view file_name)
{
    int w, h, channel;
    stbi_uc* pixels = stbi_load(file_name.data(), &w, &h, &channel, STBI_rgb_alpha);
    size_t size = w * h * 4;
    if (!pixels) {
        throw std::runtime_error("image load failed");
    }
    image.reset(new Image(w, h, vk::Format::eR8G8B8A8Srgb, vk::ImageType::e2D,
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst,
        vk::SampleCountFlagBits::e1));
    image->SetImageLayout(vk::ImageLayout::eTransferDstOptimal,
        vk::AccessFlagBits::eNone,
        vk::AccessFlagBits::eTransferWrite,
        vk::PipelineStageFlagBits::eTopOfPipe,
        vk::PipelineStageFlagBits::eTransfer);

    image->FillImageData(size, pixels);
    image->SetImageLayout(
        vk::ImageLayout::eReadOnlyOptimal, vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eShaderRead, vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eFragmentShader);
    stbi_image_free(pixels);
}
Texture::~Texture() { }

}
