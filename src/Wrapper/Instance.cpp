#include "MoCheng3D/Wrapper/Instance.hpp"
#include "GLFW/glfw3.h"
#include "MoCheng3D/Wrapper/Base.hpp"
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.hpp>
namespace MoCheng3D {

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pMessageData, void* pUserData)
{
    std::cout << "ValidationLayer: " << pMessageData->pMessage << std::endl;

    return VK_FALSE;
}
static VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* debugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, debugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

Instance::Instance()
{

    print_available_extensions();
    vk ::InstanceCreateInfo create_info;
    vk::ApplicationInfo app_info;
    std::vector<const char*> vaild_layer = { "VK_LAYER_KHRONOS_validation" };

    app_info.setApiVersion(VK_API_VERSION_1_3);
    auto extensions = GetRequiredExtensions();
    create_info
        .setPApplicationInfo(&app_info)

        .setPEnabledExtensionNames(extensions)
        .setPEnabledLayerNames(vaild_layer);
    m_handle = vk::createInstance(create_info);

    Enable_vaildLayer();
}

std::vector<const char*> Instance::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;

    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions,
        glfwExtensions + glfwExtensionCount);

    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}
void Instance::print_available_extensions()
{

    uint32_t extensionCount = 0;
    std::string d { "123" };
    auto dd = d.c_str();

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
        extensions.data());

    std::cout << "Available extensions:" << std::endl;

    for (const auto& extension : extensions) {
        std::cout << extension.extensionName << std::endl;
    }
}
void Instance::Enable_vaildLayer()
{

    // VkDebugUtilsMessengerCreateInfoEXT createInfo = {};

    // createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    // createInfo.messageSeverity =
    //     // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    //     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    // createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    // createInfo.pfnUserCallback = debugCallBack;
    // createInfo.pUserData = nullptr;
    // if (CreateDebugUtilsMessengerEXT(m_handle, &createInfo, nullptr,
    //         (VkDebugUtilsMessengerEXT*)(&m_debugger))
    //     != VK_SUCCESS) {
    //     throw std::runtime_error("Error:failed to create debugger");
    // }
    // std::cout << "vaild here" << std::endl;
}

Instance::~Instance()
{
    
    m_handle.destroy();
}
// "VK_LAYER_KHRONOS_validation"
} // namespace MoCheng3D