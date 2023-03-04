#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include <cassert>
#include <set>
#include <sys/types.h>
#include <vulkan/vulkan_structs.hpp>

namespace MoCheng3D {
class SwapChain : public Component<vk::SwapchainKHR, SwapChain> {
public:
  SwapChain();
  ~SwapChain();

  struct SwapChain_Info {
    vk::Extent2D image_extent;
    u_int32_t image_count;
    vk::SurfaceFormatKHR format;
  };
  struct SurfaceInfo {
    vk::SurfaceFormatKHR format;
    vk::Extent2D extent;
    std::uint32_t count;
    vk::SurfaceTransformFlagBitsKHR transform;
  } surfaceInfo;

  [[nodiscard("Missing Swapchain_Image")]] auto Get_Swapchian_Images() {
    assert(m_handle);
    return Get_Context_Singleton()
        .Get_Device()
        ->Get_handle()
        .getSwapchainImagesKHR(m_handle);
  }
  [[nodiscard("missing extent2D")]] auto Get_Extent2D() {
    return surfaceInfo.extent;
  }

  [[nodiscard("Missing format")]] auto Get_Format() {
    return surfaceInfo.format.format;
  }

private:
  vk::SurfaceFormatKHR Query_surface_Format();
  vk::Extent2D
  Query_surface_Extent(const vk::SurfaceCapabilitiesKHR &capability,
                       int windowWidth, int windowHeight);
  void Query_info();
};

} // namespace MoCheng3D