#pragma once
#include "MoCheng3D/Rendering/Context.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>
#include <memory>


namespace MoCheng3D {

template <typename T, typename K>
class Component {

protected:
    T m_handle;

    [[nodiscard]] auto& Get_Context_Singleton()
    {
        return Context::Get_Singleton();
    }

public:
    using Ptr = std::shared_ptr<K>;
    template <typename... _Args>
    static Ptr Create(_Args&&... args)
    {
        return std::make_shared<K>(args...);
    }
    [[nodiscard]] T& Get_handle() { return m_handle; }
    ~Component<T, K>()
    {
        std::cout << "delete  " << typeid(T).name() << std::endl;
    }
};
} // namespace MoCheng3D