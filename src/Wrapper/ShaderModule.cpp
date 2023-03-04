#include "MoCheng3D/Wrapper/ShaderModule.hpp"
#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "cstring"
#include <cstdint>
#include <fstream>
#include <iosfwd>
#include <string>
#include <vulkan/vulkan_structs.hpp>
namespace MoCheng3D {

std::vector<char> ShaderModule::ReadBinary(const std::string &fileName) {
  std::ifstream file(fileName.data(),
                     std::ios::ate | std::ios::binary | std::ios::in);

  if (!file) {
    throw std::runtime_error("Error: failed to open shader file");
  }

  const size_t fileSize = file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
}
ShaderModule::ShaderModule(const std::string path) {
    

  auto source = ReadBinary(path);

  vk::ShaderModuleCreateInfo createInfo;
  createInfo
      .setCodeSize(source.size())

      .setPCode((uint32_t *)source.data());
  auto device = Get_Context_Singleton().Get_Device();

  m_handle = device->Get_handle().createShaderModule(createInfo);
}
ShaderModule::~ShaderModule() {}
} // namespace MoCheng3D