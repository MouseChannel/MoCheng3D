#include "MoCheng3D/Helper/CommandManager.hpp"
#include "MoCheng3D/Wrapper/CommandBuffer.hpp"

namespace MoCheng3D {
void CommandManager::ExecuteCmd(vk::Queue queue, RecordCmdFunc func)
{
  std::shared_ptr<CommandBuffer> cmd(new CommandBuffer);
  cmd->Begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

  if (func)
      func(cmd->Get_handle());
  cmd->End();
  vk::SubmitInfo submitInfo;
  submitInfo.setCommandBuffers(cmd->Get_handle());
  queue.submit(submitInfo);
  queue.waitIdle();
}

}