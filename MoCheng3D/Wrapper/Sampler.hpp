#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class Sampler : public Component<vk::Sampler, Sampler> {
public:
    Sampler();
    ~Sampler();

private:
};
} // namespace MoCheng3D