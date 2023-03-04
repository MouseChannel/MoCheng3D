#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
#include <vulkan/vulkan_handles.hpp>

namespace MoCheng3D {
class ShaderModule : public Component<vk::ShaderModule, ShaderModule> {
private:
public:
  ShaderModule(const std::string path);
  ~ShaderModule();
  static std::vector<char> ReadBinary(const std::string &fileName);
};
} // namespace MoCheng3D