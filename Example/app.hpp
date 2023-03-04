#include "MoCheng3D/Rendering/Context.hpp"
#include <memory>
namespace MoCheng3D {
class App {
public:
  App() = default;
  ~App() = default;
  void Init();
  void InitWindow();
  void InitVulkan();
  void Update();
  void Run();

private:
};
} // namespace MoCheng3D