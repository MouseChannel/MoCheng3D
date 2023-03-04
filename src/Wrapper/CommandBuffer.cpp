#include "MoCheng3D/Wrapper/CommandBuffer.hpp"
#include "MoCheng3D/Wrapper/CommandPool.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include <vulkan/vulkan_handles.hpp>
namespace MoCheng3D {
CommandBuffer::CommandBuffer() {

  vk::CommandBufferAllocateInfo allocate_info;

  auto command_pool = Get_Context_Singleton().Get_CommandPool();
  allocate_info.setCommandBufferCount(1)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandPool(command_pool->Get_handle());

  m_handle =
      Get_Context_Singleton().Get_Device()->Get_handle().allocateCommandBuffers(
          allocate_info)[0];
}
CommandBuffer::~CommandBuffer() {}
} // namespace MoCheng3D