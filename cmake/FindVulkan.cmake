IF(WIN32)
    MESSAGE(STATUS "Finding Windows Vulkan😊")
    set(VULKAN_DIR "D:/Some_software/Vulkan" CACHE PATH "Vulkan root dir, example: C:/VulkanSDK/1.3.216.0")

    set(VULKAN_INCLUDE_DIR "${VULKAN_DIR}/Include")
    set(VULKAN_LIB_LOCATION "${VULKAN_DIR}/Lib/vulkan-1.lib")
    add_library(Vulkan::Vulkan STATIC IMPORTED GLOBAL)
    set_target_properties(
        Vulkan::Vulkan
        PROPERTIES
        IMPORTED_LOCATION ${VULKAN_LIB_LOCATION}
        INTERFACE_INCLUDE_DIRECTORIES ${VULKAN_INCLUDE_DIR}
    )
    MESSAGE(STATUS "link Windows Vulkan down👌")
ELSEIF(UNIX)
    MESSAGE(STATUS "Find linux Vulkan........")
    find_package(Vulkan::Vulkan REQUIRED)
ENDIF()