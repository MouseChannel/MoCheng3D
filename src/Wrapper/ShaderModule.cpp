/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-03-03 16:49:27
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-03-03 17:03:47
 * @FilePath: \MYP\src\Wrapper\ShaderModule.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Wrapper/ShaderModule.hpp"
#include "Rendering/Context.hpp"
#include "Wrapper/Device.hpp"
#include "cstring"
#include <cstdint>
#include <fstream>
#include <iosfwd>
#include <string>
#include <vulkan/vulkan_structs.hpp>
namespace MoCheng3D {

std::vector<char> ShaderModule::ReadBinary(const std::string& fileName)
{
    std::ifstream file(fileName.c_str(),
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
ShaderModule::ShaderModule(const std::string path)
{

    auto source = ReadBinary(path);

    vk::ShaderModuleCreateInfo createInfo;
    createInfo
        .setCodeSize(source.size())

        .setPCode((const uint32_t*)source.data());

    m_handle = Get_Context_Singleton()
                   ->Get_Device()
                   ->Get_handle()
                   .createShaderModule(createInfo);
}
ShaderModule::~ShaderModule()
{
    Get_Context_Singleton()
        ->Get_Device()
        ->Get_handle()
        .destroyShaderModule(m_handle);
}
} // namespace MoCheng3D