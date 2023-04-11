#pragma once
#include "Wrapper/Base.hpp"
#include "Wrapper/Component.hpp"
// #include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class Semaphore : public Component<vk::Semaphore, Semaphore> {
public:
    Semaphore();
    ~Semaphore();

private:
};
} // namespace MoCheng3D