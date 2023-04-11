#pragma once
#include "Wrapper/Base.hpp"
namespace MoCheng3D {
class Image;
class Texture {
public:
    Texture(std::string_view file_name);
    ~Texture();
    [[nodiscard("Missing image")]] auto GetImage() { return image; }

private:
    std::shared_ptr<Image> image;
};
} // namespace MoCheng3D