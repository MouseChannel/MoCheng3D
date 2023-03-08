#include "MoCheng3D/Wrapper/DescriptorPool.hpp"
#include "MoCheng3D/Wrapper/DescriptorSet.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"

namespace MoCheng3D
{
DescriptorPool::DescriptorPool()
{
    auto swapchain_size = Get_Context_Singleton().Get_SwapChain()->Get_Swapchain_Image_size();
    vk::DescriptorPoolSize pool_size;
    pool_size.setType(vk::DescriptorType::eUniformBuffer).setDescriptorCount(swapchain_size);

    vk::DescriptorPoolCreateInfo create_info;
    create_info.setPoolSizes(pool_size).setMaxSets(swapchain_size);
    m_handle = Get_Context_Singleton().Get_Device()->Get_handle().createDescriptorPool(create_info);
}
DescriptorPool::~DescriptorPool()
{
    Get_Context_Singleton().Get_Device()->Get_handle().destroyDescriptorPool(m_handle);
}
// DescriptorSet::Ptr DescriptorPool::CreateDescriptorSet() {
//     DescriptorSet::Create(Args &&args...)
// }
} // namespace MoCheng3D