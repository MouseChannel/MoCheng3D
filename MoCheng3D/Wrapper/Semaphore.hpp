#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
// #include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class Semaphore : public Component<vk::Semaphore, Semaphore> {
public:
  Semaphore();
  ~Semaphore();

private:
};
} // namespace MoCheng3D