#pragma once
#include <memory>
template <typename T>
class Instance_base {
public:
    static std::unique_ptr<T> _instance;
    static T& Get_Singleton()
    {
        if (_instance == nullptr) {
            _instance.reset(new T);
        }
        return *_instance;
    }
};