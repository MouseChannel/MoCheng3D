#include "MoCheng3D/Wrapper/DescriptorPool.hpp"
#include "MoCheng3D/Wrapper/DescriptorSet.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"

namespace MoCheng3D {
DescriptorPool::DescriptorPool(std::vector<vk::DescriptorType> types, std::vector<uint32_t> size)
{
    auto swapchain_size = Get_Context_Singleton().Get_SwapChain()->Get_Swapchain_Image_size();
    std::vector<vk::DescriptorPoolSize> pool_size(types.size());
    for (int i = 0; i < types.size(); i++) {
        pool_size[i].setDescriptorCount(size[i]).setType(types[i]);
    }
    // pool_size.setType(type)
    //     .setDescriptorCount(size);

    vk::DescriptorPoolCreateInfo create_info;
    create_info.setPoolSizes(pool_size)
        .setMaxSets(swapchain_size);
    m_handle = Get_Context_Singleton()
                   .Get_Device()
                   ->Get_handle()
                   .createDescriptorPool(create_info);
}
DescriptorPool::~DescriptorPool()
{
    Get_Context_Singleton()
        .Get_Device()
        ->Get_handle()
        .destroyDescriptorPool(m_handle);
}
// DescriptorSet::Ptr DescriptorPool::CreateDescriptorSet() {
//     DescriptorSet::Create(Args &&args...)
// }
} // namespace MoCheng3D