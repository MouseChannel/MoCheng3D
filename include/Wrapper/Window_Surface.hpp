#pragma once
#include "Wrapper/Base.hpp"
#include "Wrapper/Component.hpp"

namespace MoCheng3D {
class Surface : public Component<vk::SurfaceKHR, Surface> {
public:
    Surface();
    ~Surface();

private:
};
} // namespace MoCheng3D