 
#include "Example/app.cpp"
#include "MoCheng3D/Rendering/GLFW_Window.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/Instance.hpp"
// #include "SDL.h"
#include <iostream>
int main(int, char**)
{

    std::cout << "Hello, world!\n";
    // SDL_Log("Hello from sdl");

    MoCheng3D::App a;
    a.Init();
    a.Run();

    // SDL_Init(SDL_INIT_EVERYTHING);
    // SDL_CreateWindow("21", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
    //     800, SDL_WINDOW_SHOWN);
    return 0;
}
