
#include "MoCheng3D/Wrapper/Buffer.hpp"
#include "MoCheng3D/Wrapper/CommandBuffer.hpp"
#include "MoCheng3D/Wrapper/CommandPool.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"

namespace MoCheng3D {
Buffer::Buffer(size_t size,
    vk::BufferUsageFlags usage,
    vk::MemoryPropertyFlags property,
    bool permanent)
    : permanent(permanent)
    , m_size(size)
{
    CreateBuffer(size, usage);
    memory_info = QueryMemoryInfo(property);
    AllocateMemory();
    BindMemory2Buffer();
}
Buffer::~Buffer()
{
    Get_Context_Singleton()

        .Get_Device()
        ->Get_handle()
        .destroyBuffer(m_handle);
}
void Buffer::CreateBuffer(size_t size, vk::BufferUsageFlags usage)
{
    vk::BufferCreateInfo allocate_info;
    allocate_info.setSize(size)
        .setUsage(usage)

        .setSharingMode(vk::SharingMode::eExclusive);
    m_handle = Get_Context_Singleton().Get_Device()->Get_handle().createBuffer(
        allocate_info);
}
Buffer::MemoryInfo
Buffer::QueryMemoryInfo(vk::MemoryPropertyFlags property)
{
    MemoryInfo info;
    auto requirements = Get_Context_Singleton()
                            .Get_Device()
                            ->Get_handle()
                            .getBufferMemoryRequirements(m_handle);
    info.size = requirements.size;
    auto properties = Get_Context_Singleton()
                          .Get_Device()
                          ->Get_Physical_device()
                          .getMemoryProperties();

    for (int i = 0; i < properties.memoryTypeCount; i++) {
        // satisfy both buffer memory_property AND asked memory_property
        if ((1 << i) & requirements.memoryTypeBits && properties.memoryTypes[i].propertyFlags & property) {
            info.index = i;
            break;
        }
    }
    return info;
}
void Buffer::AllocateMemory()
{
    vk::MemoryAllocateInfo allocate_info;
    allocate_info.setMemoryTypeIndex(memory_info.index)
        .setAllocationSize(memory_info.size);
    memory = Get_Context_Singleton().Get_Device()->Get_handle().allocateMemory(
        allocate_info);
}
void Buffer::BindMemory2Buffer()
{
    Get_Context_Singleton()
        .Get_Device()
        ->Get_handle()
        .bindBufferMemory(m_handle, memory, 0);
}
void Buffer::Map(uint32_t offset, uint32_t size)
{
    mapped_data = Get_Context_Singleton()
                      .Get_Device()
                      ->Get_handle()
                      .mapMemory(memory, offset, size);
}
void Buffer::Unmap()
{
    Get_Context_Singleton()
        .Get_Device()
        ->Get_handle()
        .unmapMemory(memory);
}
void Buffer::Update(void* data, size_t size)
{

    Map(0, size);
    std::memcpy(mapped_data, data, size);
    if (permanent) {
        return;
    }
    Unmap();
}

Buffer::Ptr
Buffer::CreateDeviceBuffer(void* data, size_t size, vk::BufferUsageFlags usage)
{
    auto command_buffer = CommandBuffer::Create();
    auto host_buffer = Buffer::Create(size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    host_buffer->Update(data, size);

    auto device_buffer = Buffer::Create(size,
        vk::BufferUsageFlagBits::eTransferDst | usage,
        vk::MemoryPropertyFlagBits::eDeviceLocal, true);

    vk::CommandBufferBeginInfo begin_info;
    begin_info.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

    command_buffer->Begin(begin_info);
    command_buffer->CopyBuffer(host_buffer, device_buffer);
    command_buffer->End();

    vk::SubmitInfo submit_info;
    submit_info.setCommandBuffers(command_buffer->Get_handle());
    auto command_queue = Context::Get_Singleton().Get_Device()->Get_Graphic_queue();
    command_queue.submit(submit_info);
    command_queue.waitIdle();
    return device_buffer;
}

} // namespace MoCheng3D