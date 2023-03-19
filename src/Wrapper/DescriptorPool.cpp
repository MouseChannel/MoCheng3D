#include "MoCheng3D/Wrapper/DescriptorPool.hpp"
#include "MoCheng3D/Wrapper/DescriptorSet.hpp"
#include "MoCheng3D/Wrapper/SwapChain.hpp"

namespace MoCheng3D {

DescriptorPool::DescriptorPool(std::vector<std::tuple<vk::DescriptorType, uint32_t>> type_size)
{
    auto swapchain_size = Get_Context_Singleton().Get_SwapChain()->Get_Swapchain_Image_size();
    std::vector<vk::DescriptorPoolSize> pool_size(type_size.size());
    for (int i = 0; i < type_size.size(); i++) {
        auto type = std::get<0>(type_size[i]);
        auto size = std::get<1>(type_size[i]);
        pool_size[i].setDescriptorCount(size).setType(type);
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
 
} // namespace MoCheng3D