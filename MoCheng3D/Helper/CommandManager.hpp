#pragma once

#include "vulkan/vulkan.hpp"
#include <functional>

namespace MoCheng3D {
class CommandBuffer;
class CommandManager final {
public:
    using RecordCmdFunc = std::function<void(vk::CommandBuffer&)>;
    static void ExecuteCmd(vk::Queue, RecordCmdFunc);
};

}