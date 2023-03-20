#pragma once
#include "MoCheng3D/Wrapper/Base.hpp"
#include "MoCheng3D/Wrapper/Component.hpp"
 
namespace MoCheng3D {
class Surface : public Component<vk::SurfaceKHR, Surface> {
public:
    Surface();
    ~Surface();

private:
};
} // namespace MoCheng3D