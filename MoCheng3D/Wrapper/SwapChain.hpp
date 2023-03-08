/*
 * @Author: error: error: git config user.name & please set dead value or
 * install git && error: git config user.email & please set dead value or
 * install git & please set dead value or install git
 * @Date: 2023-03-03 16:49:24
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config
 * user.email & please set dead value or install git & please set dead value or install git install git && error: git
 * config user.email & please set dead value or install git & please set dead value or install git install git && error:
 * git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-03-04 13:17:24
 * @FilePath: \MYP\MoCheng3D\Wrapper\SwapChain.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "MoCheng3D/Rendering/Context.hpp"
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/FrameBuffer.hpp"
#include "MoCheng3D/Wrapper/Image.hpp"
#include <cassert>
#include <set>
#include <sys/types.h>
#include <vulkan/vulkan_structs.hpp>

namespace MoCheng3D
{
class SwapChain : public Component<vk::SwapchainKHR, SwapChain>
{
  public:
    SwapChain();
    ~SwapChain();

    struct SwapChain_Info
    {
        vk::Extent2D image_extent;
        uint32_t image_count;
        vk::SurfaceFormatKHR format;
    };
    struct SurfaceInfo
    {
        vk::SurfaceFormatKHR format;
        vk::Extent2D extent;
        std::uint32_t count;
        vk::SurfaceTransformFlagBitsKHR transform;
    } surfaceInfo;

    [[nodiscard("Missing Swapchain_Image")]] auto Get_Swapchain_Images()
    {
        assert(m_handle);
        if (swapchain_Image_handles.empty())
        {
            swapchain_Image_handles =
                Get_Context_Singleton().Get_Device()->Get_handle().getSwapchainImagesKHR(m_handle);
        }
        return swapchain_Image_handles;
    }
    [[nodiscard("missing extent2D")]] auto Get_Extent2D()
    {
        return surfaceInfo.extent;
    }

    [[nodiscard("Missing format")]] auto Get_Format()
    {
        return surfaceInfo.format.format;
    }
    // [[nodiscard("Missing Images")]] auto Get_Swapchain_Images()
    // {
    //     return images;
    // }
    [[nodiscard("Missing FramgeBUffer")]] auto Get_FrameBuffers()
    {
        return framebuffers;
    }
    [[nodiscard("Missing  Swapchian_Images_Size")]] auto Get_Swapchain_Image_size()
    {
        return Get_Swapchain_Images().size();
    }

  private:
    vk::SurfaceFormatKHR Query_surface_Format();

    std::vector<Image::Ptr> images;
    std::vector<Framebuffer::Ptr> framebuffers;
    vk::Extent2D Query_surface_Extent(const vk::SurfaceCapabilitiesKHR &capability, int windowWidth, int windowHeight);
    std::vector<vk::Image> swapchain_Image_handles;
    void Query_info();
};

} // namespace MoCheng3D