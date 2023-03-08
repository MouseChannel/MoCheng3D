
#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
class CommandBuffer;
class Buffer : public Component<vk::Buffer, Buffer> {
public:
  Buffer(size_t size, vk::BufferUsageFlags usage,
         vk::MemoryPropertyFlags property);
  ~Buffer();
  void Update(void *data, size_t size);
  [[nodiscard("Missing Size")]] auto GetSize() { return m_size; }
  static Buffer::Ptr CreateDeviceBuffer(void *data, size_t size,
                                        vk::BufferUsageFlags usage);

private:
  struct MemoryInfo {
    size_t size;
    uint32_t index;
  } memory_info;
  vk::DeviceMemory memory;
  size_t m_size;
  void CreateBuffer(size_t size, vk::BufferUsageFlags usage);
  MemoryInfo QueryMemoryInfo(vk::MemoryPropertyFlags property);
  void AllocateMemory();
  void BindMemory2Buffer();
};
} // namespace MoCheng3D