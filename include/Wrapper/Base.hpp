#pragma once
#include "Wrapper/Component.hpp"
#include <iostream>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.hpp>

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
#define VK_CHECK_SUCCESS(error, mes)                               \
    if (error) {                                                   \
        printf("%s, err_code: %s\n", mes, string_VkResult(error)); \
        abort();                                                   \
    }

void LOG(auto mes)
{

    std::cout << mes;

    std::cout << std ::endl;
}

inline void VK_Check(VkResult res, std::string mes)
{

    if (res != VK_SUCCESS) {
        std::cout << mes << "error code : " << string_VkResult(res) << std::endl;
    }
}